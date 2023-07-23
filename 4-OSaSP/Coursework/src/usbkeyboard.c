#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/hid.h>
#include <linux/usb/input.h>

#define DRIVER_VERSION ""
#define DRIVER_AUTHOR "Inqognitoo <alexinq6@gmail.com>"
#define DRIVER_DESC "USB HID Boot Protocol Keyboard module"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

static const unsigned char usbkeyboard_keycode[256] = {
	  0,  0,  0,  0, 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38,
	 50, 49, 24, 25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44,  2,  3,
	  4,  5,  6,  7,  8,  9, 10, 11, 28,  1, 14, 15, 57, 12, 13, 26,
	 27, 43, 43, 39, 40, 41, 51, 52, 53, 58, 59, 60, 61, 62, 63, 64,
	 65, 66, 67, 68, 87, 88, 99, 70,119,110,102,104,111,107,109,106,
	105,108,103, 69, 98, 55, 74, 78, 96, 79, 80, 81, 75, 76, 77, 71,
	 72, 73, 82, 83, 86,127,116,117,183,184,185,186,187,188,189,190,
	191,192,193,194,134,138,130,132,128,129,131,137,133,135,136,113,
	115,114,  0,  0,  0,121,  0, 89, 93,124, 92, 94, 95,  0,  0,  0,
	122,123, 90, 91, 85,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 29, 42, 56,125, 97, 54,100,126,164,166,165,163,161,115,114,113,
	150,158,159,128,136,177,178,176,142,152,173,140,
};

#define KBD_NAME_MAX 128
#define KBD_PHYS_MAX 64

struct usbkeyboard {
    struct input_dev* dev;
    struct usb_device* usbdev;

    struct urb* irq;
    struct urb* led;

    struct usb_ctrlrequest* creq;

    char name[KBD_NAME_MAX];
    char phys[KBD_PHYS_MAX];
    
    unsigned char prev_presses[8];
    unsigned char* presses;

    unsigned char curr_leds;
    unsigned char* leds;    

    dma_addr_t presses_dma;
    dma_addr_t leds_dma;

    bool leds_urb_submitted;
    spinlock_t leds_lock;
};

static void usbkeyboard_irq(struct urb* urb)
{
    struct usbkeyboard* kbd = urb->context;
    int i;
    
    switch(urb->status){
        case 0:
            break;

        case -ECONNRESET:
        case -ENOENT:
        case -ESHUTDOWN:
            return;

        default:
            goto resubmit;
    }

    kbd = urb->context;

    for(i = 0; i < 8; i++){
        input_report_key(kbd->dev, usbkeyboard_keycode[i + 224], (kbd->presses[0] >> i) & 1);
    }

    for(i = 2; i < 8; i++){
        if(kbd->prev_presses[i] > 3 && memscan(kbd->presses + 2, kbd->prev_presses[i], 6) == kbd->presses + 8)
        {
            if(usbkeyboard_keycode[kbd->prev_presses[i]]){
                input_report_key(kbd->dev, usbkeyboard_keycode[kbd->prev_presses[i]], 0);
            }
            else {
                hid_info(urb->dev, "Unknown key with scancode %#x released\n", kbd->prev_presses[i]);
            }
        }

        if(kbd->presses[i] > 3 && memscan(kbd->prev_presses + 2, kbd->presses[i], 6) == kbd->prev_presses + 8)
        {
            if(usbkeyboard_keycode[kbd->presses[i]]){
                input_report_key(kbd->dev, usbkeyboard_keycode[kbd->presses[i]], 1);
            }
            else {
                hid_info(urb->dev, "Unknown key with scancode %#x pressed\n", kbd->presses[i]);
            }
        }
    }

    input_sync(kbd->dev);

    memcpy(kbd->prev_presses, kbd->presses, 8);

resubmit:
    i = usb_submit_urb(urb, GFP_ATOMIC);

    if(i){
        hid_err(urb->dev, "Can't resubmit interrupt at %s-%s/input0 (status %d)\n",
                kbd->usbdev->bus->bus_name,
                kbd->usbdev->devpath, 
                i);
    }
}

static void usbkeyboard_led(struct urb* urb)
{
    unsigned long flags;
    struct usbkeyboard* kbd;

    if(urb->status){
        hid_warn(urb->dev, "Received led urb with non-zero status %d\n", urb->status);
    }

    kbd = urb->context;

    spin_lock_irqsave(&kbd->leds_lock, flags);

    if(*(kbd->leds) == kbd->curr_leds){
        kbd->leds_urb_submitted = false;
        spin_unlock_irqrestore(&kbd->leds_lock, flags);
        return;
    }

    *(kbd->leds) = kbd->curr_leds;

    kbd->led->dev = kbd->usbdev;

    if(usb_submit_urb(kbd->led, GFP_ATOMIC)){
        hid_err(urb->dev, "Failed to submit led urb\n");
        kbd->leds_urb_submitted = false;
    }

    spin_unlock_irqrestore(&kbd->leds_lock, flags);
}

static int usbkeyboard_event(struct input_dev* dev, unsigned int type, unsigned int code, int value)
{
    unsigned long flags;
    struct usbkeyboard* kbd;

    if(type != EV_LED){
        return -1;
    }

    kbd = input_get_drvdata(dev);

    spin_lock_irqsave(&kbd->leds_lock, flags);

    kbd->curr_leds = (!!test_bit(LED_KANA,    dev->led) << 4) |
                     (!!test_bit(LED_COMPOSE, dev->led) << 3) |
                     (!!test_bit(LED_SCROLLL, dev->led) << 2) |
                     (!!test_bit(LED_CAPSL,   dev->led) << 1) |
                     (!!test_bit(LED_NUML,    dev->led));

    if(kbd->leds_urb_submitted){
        spin_unlock_irqrestore(&kbd->leds_lock, flags);
        return 0;
    }

    if(*(kbd->leds) == kbd->curr_leds){
        spin_unlock_irqrestore(&kbd->leds_lock, flags);
        return 0;
    }

    *(kbd->leds) = kbd->curr_leds;

    kbd->led->dev = kbd->usbdev;

    if(usb_submit_urb(kbd->led, GFP_ATOMIC)){
        pr_err("Failed to submit led urb\n");
    }
    else {
        kbd->leds_urb_submitted = true;
    }

    spin_unlock_irqrestore(&kbd->leds_lock, flags);

    return 0;
}

static int usbkeyboard_open(struct input_dev* dev)
{
    struct usbkeyboard* kbd = input_get_drvdata(dev);

    kbd->irq->dev = kbd->usbdev;
    
    if(usb_submit_urb(kbd->irq, GFP_KERNEL)){
        return -EIO;
    }

    return 0;
}

static void usbkeyboard_close(struct input_dev* dev)
{
    struct usbkeyboard* kbd = input_get_drvdata(dev);

    usb_kill_urb(kbd->irq);
}

static int usbkeyboard_alloc_memory(struct usb_device* dev, struct usbkeyboard* kbd)
{
    if(!(kbd->irq = usb_alloc_urb(0, GFP_KERNEL))){
        return -1;
    }

    if(!(kbd->led = usb_alloc_urb(0, GFP_KERNEL))){
        return -1;
    }

    if(!(kbd->presses = usb_alloc_coherent(dev, 8, GFP_KERNEL, &kbd->presses_dma))){
        return -1;
    }

    if(!(kbd->creq = kmalloc(sizeof(struct usb_ctrlrequest), GFP_KERNEL))){
        return -1;
    }

    if(!(kbd->leds = usb_alloc_coherent(dev, 1, GFP_KERNEL, &kbd->leds_dma))){
        return -1;
    }

    return 0;
}

static void usbkeyboard_free_memory(struct usb_device* dev, struct usbkeyboard* kbd)
{
    usb_free_urb(kbd->irq);
    usb_free_urb(kbd->led);
    usb_free_coherent(dev, 8, kbd->presses, kbd->presses_dma);
    kfree(kbd->creq);
    usb_free_coherent(dev, 1, kbd->leds, kbd->leds_dma);
}

static int usbkeyboard_probe(struct usb_interface* intf, const struct usb_device_id* id)
{
    struct usb_device* dev;
    struct usb_host_interface* interface;
    struct usb_endpoint_descriptor* endpoint;
    struct usbkeyboard* kbd;
    struct input_dev* input_dev;
    int pipe, maxp;
    int error;
    int i;

    dev = interface_to_usbdev(intf);
    error = -ENOMEM;
    interface = intf->cur_altsetting;

    if(interface->desc.bNumEndpoints != 1){
        return -ENODEV;
    }

    endpoint = &interface->endpoint[0].desc;

    if(!usb_endpoint_is_int_in(endpoint)){
        return -ENODEV;
    }

    pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
    maxp = usb_maxpacket(dev, pipe);

    kbd = kzalloc(sizeof(struct usbkeyboard), GFP_KERNEL);
    input_dev = input_allocate_device();

    if(!kbd || !input_dev){
        goto halt1;
    }

    if(usbkeyboard_alloc_memory(dev, kbd)){
        goto halt2;
    }

    kbd->usbdev = dev;
    kbd->dev = input_dev;

    spin_lock_init(&kbd->leds_lock);
    
    if(dev->manufacturer){
        strscpy(kbd->name, dev->manufacturer, sizeof(kbd->name));
    }

    if(dev->product){
        if(dev->manufacturer){
            strlcat(kbd->name, " ", sizeof(kbd->name));
        }

        strlcat(kbd->name, dev->product, sizeof(kbd->name));
    }

    if(!strlen(kbd->name)){
        snprintf(kbd->name, sizeof(kbd->name), 
                 "USB HID Boot Protocol Keyboard %04x:%04x",
                 le16_to_cpu(dev->descriptor.idVendor),
                 le16_to_cpu(dev->descriptor.idProduct));
    }

    usb_make_path(dev, kbd->phys, sizeof(kbd->phys));
    strlcat(kbd->phys, "/input0", sizeof(kbd->phys));

    input_dev->name = kbd->name;
    input_dev->phys = kbd->phys;
    usb_to_input_id(dev, &input_dev->id);
    input_dev->dev.parent = &intf->dev;

    input_set_drvdata(input_dev, kbd);

    input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP) | BIT_MASK(EV_LED);

    input_dev->ledbit[0] = BIT_MASK(LED_NUML) | BIT_MASK(LED_CAPSL) | 
                           BIT_MASK(LED_SCROLLL) | BIT_MASK(LED_COMPOSE) | BIT_MASK(LED_KANA);
    
    for(i = 0; i < 255; i++){
        set_bit(usbkeyboard_keycode[i], input_dev->keybit);
    }

    clear_bit(0, input_dev->keybit);

    input_dev->event = usbkeyboard_event;
    input_dev->open = usbkeyboard_open;
    input_dev->close = usbkeyboard_close;

    usb_fill_int_urb(kbd->irq, 
                     dev, pipe, kbd->presses, 
                     (maxp > 8 ? 8 : maxp), 
                     usbkeyboard_irq, kbd, endpoint->bInterval);
    
    kbd->irq->transfer_dma = kbd->presses_dma;
    kbd->irq->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

    kbd->creq->bRequestType = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
    kbd->creq->bRequest = 0x09;
    kbd->creq->wValue = cpu_to_le16(0x0200);
    kbd->creq->wIndex = cpu_to_le16(interface->desc.bInterfaceNumber);
    kbd->creq->wLength = 1;

    pipe = usb_sndctrlpipe(dev, 0);

    usb_fill_control_urb(kbd->led, 
                         dev, pipe, (void*) kbd->creq, kbd->leds,
                         1,
                         usbkeyboard_led, kbd);

    kbd->led->transfer_dma = kbd->leds_dma;
    kbd->led->transfer_dma |= URB_NO_TRANSFER_DMA_MAP;

    error = input_register_device(kbd->dev);

    if(error){
        goto halt2;
    }

    usb_set_intfdata(intf, kbd);
    device_set_wakeup_enable(&dev->dev, 1);
    return 0;

halt1:
    usbkeyboard_free_memory(dev, kbd);
halt2:
    input_free_device(input_dev);
    kfree(kbd);
    return error;
}

static void usbkeyboard_disconnect(struct usb_interface *intf)
{
    struct usbkeyboard* kbd = usb_get_intfdata(intf);

    usb_set_intfdata(intf, NULL);

    if(kbd){
        usb_kill_urb(kbd->irq);
        input_unregister_device(kbd->dev);
        usb_kill_urb(kbd->led);
        usbkeyboard_free_memory(interface_to_usbdev(intf), kbd);
        kfree(kbd);
    }
}

static const struct usb_device_id usbkeyboard_id_table[] = {
    { USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT, USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{}
};

MODULE_DEVICE_TABLE(usb, usbkeyboard_id_table);

static struct usb_driver usbkeyboard_driver = {
    .name = "usbkeyboard",
    .probe = usbkeyboard_probe,
    .disconnect = usbkeyboard_disconnect,
    .id_table = usbkeyboard_id_table,
};

module_usb_driver(usbkeyboard_driver);