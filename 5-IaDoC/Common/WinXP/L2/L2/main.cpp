#include "hexioctrl.h"
#include "pci_codes.h"

#include <conio.h>
#include <utility>

#define uint unsigned int

struct pci_device {
private:
	bool valid;
	uint venid;
	uint devid;

	inline uint calc_address(uint bus, uint dev, uint fun, uint reg){
		uint address = 1;
		address <<= 15;
		address += bus;
		address <<= 5;
		address += dev;
		address <<= 3;
		address += fun;
		address <<= 8;
		address += reg;
		return address;
	}

	inline uint read_register(uint address){
		_outpd(0x0CF8, address);
		return _inpd(0x0CFC);
	}

public:
	pci_device(uint bus, uint dev, uint fun){
		uint addr = calc_address(bus, dev, fun, 0x00);
		uint data = read_register(addr);

		if(data == -1){
			valid = false;
			devid = -1;
			venid = -1;
		}
		else {
			valid = true;
			devid = data >> 16;
			venid = data & 0x0000FFFF;
		}
	}

	bool is_valid(){
		return valid;
	}

	operator bool() const {
		return valid;
	}

	int vendor_id(){
		return venid;
	}

	int device_id(){
		return devid;
	}

	static std::pair<const char* const, const char* const> get_device_description(uint device_id, uint vendor_id){
		for(PCI_DEVTABLE* dt = PciDevTable; dt != PciDevTable + PCI_DEVTABLE_LEN; dt++)
			if(dt->VenId == vendor_id && dt->DevId == device_id)
				return std::make_pair(dt->Chip, dt->ChipDesc);

		return std::make_pair("Unknown", "Unknown");
	}

	static const char* const get_vendor_name(uint vendor_id){
		for(PCI_VENTABLE* vt = PciVenTable; vt != PciVenTable + PCI_VENTABLE_LEN; vt++)
			if(vt->VenId == vendor_id)
				return vt->VenFull;

		return "Unknown";
	}
};

int main()
{
	ALLOW_IO_OPERATIONS;

	for(uint bus = 0; bus < 256; bus++)
		for(uint dev = 0; dev < 32; dev++)
			for(uint fun = 0; fun < 8; fun++){
				pci_device device(bus, dev, fun);

				if(device){
					uint device_id = device.device_id();
					uint vendor_id = device.vendor_id();
					std::pair<const char* const, const char* const> device_desc = pci_device::get_device_description(device_id, vendor_id);
					const char* const vendor_name = pci_device::get_vendor_name(vendor_id);

					printf("Bus: %d, Device: %d, Function: %d\n"
						   "DeviceID: %04x\n"
						   "VendorID: %04x\n"
						   "Name: %s\n"
						   "Description: %s\n"
						   "Manufacturer: %s\n\n",
						   bus, dev, fun, device.device_id(), device.vendor_id(), device_desc.first, device_desc.second, vendor_name);
			}
	}

	system("pause");
}