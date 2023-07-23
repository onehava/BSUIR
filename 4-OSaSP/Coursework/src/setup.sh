#/usb/bin/env bash

if [ `id -u` -ne 0 ]; then
    echo "You must be a superuser to run this script"
    exit 1
fi

# Remove USB HID and AT keyboard drivers
modprobe -r usbhid
modprobe -r atkbd

# Insert custom USB keyboard driver
insmod usbkeyboard.ko
