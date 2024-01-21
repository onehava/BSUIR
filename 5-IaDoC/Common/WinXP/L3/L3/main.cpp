#include "hexioctrl.h"

#include <conio.h>

//const unsigned int  ATA_AS[2] = {0x3F6, 0x376};	// R: Alternative status
//const unsigned int* ATA_DC	  = ATA_AS;			// W: Drive control
//const unsigned int  ATA_DA[2] = {0x3F7, 0x377};	// Drive address
const unsigned int  ATA_DR[2] = {0x1F0, 0x170};	// Data register
//const unsigned int  ATA_ER[2] = {0x1F1, 0x171};	// R: Error register
//const unsigned int* ATA_FR    = ATA_ER;			// W: Features register
//const unsigned int  ATA_SC[2] = {0x1F2, 0x172};	// Sector count
//const unsigned int  ATA_SN[2] = {0x1F3, 0x173};	// Sector number or LBA[07:00]
//const unsigned int  ATA_CL[2] = {0x1F4, 0x174};	// Cylinder low or LBA[15:08]
//const unsigned int  ATA_CH[2] = {0x1F5, 0x175};	// Cylinder high or LBA[23:16]
const unsigned int  ATA_DH[2] = {0x1F6, 0x176};	// Drive/head or LBA[27:24]
const unsigned int  ATA_SR[2] = {0x1F7, 0x177};	// R: Status register
const unsigned int* ATA_CR	  = ATA_SR;			// W: Command register

const unsigned int ATA_CMD_IDENTITY_DEVICE		  = 0xEC; // ATA Identity Command
const unsigned int ATA_CMD_IDENTITY_PACKET_DEVICE = 0xA1; // ATAPI Identity Command
const unsigned int ATA_CMD_FLUSH_CASH			  = 0xE7; // Flush Cash Command

inline bool is_busy(int channel){
	return (bool)(_inp(ATA_SR[channel]) & 0x80);
}

inline bool is_transferrable(int channel){
	return (bool)(_inp(ATA_SR[channel]) & 0x08);
}

inline void wait_not_busy(int channel){
	while(is_busy(channel));
}

inline void select_drive(int channel, int drive){
	_outp(ATA_DH[channel], 0xA0 | (drive & 0x1) << 4);
}

inline void write_command(int channel, unsigned short command){
	_outp(ATA_CR[channel], command);
}

inline int read_word(int channel){
	return _inpw(ATA_DR[channel]);
}

void print_device_identity(int channel, int device)
{
	wait_not_busy(channel);
	select_drive(channel, device);

	write_command(channel, ATA_CMD_IDENTITY_DEVICE);
	wait_not_busy(channel);

	if(!is_transferrable(channel)){
		write_command(channel, ATA_CMD_IDENTITY_PACKET_DEVICE);
		wait_not_busy(channel);

		if(!is_transferrable(channel)){
			return;
		}
	}

	printf("Device (%d, %d):\n", channel, device);

	unsigned short identity[256];

	for(int i = 0; i < sizeof(identity) / sizeof(unsigned short); i++){
		identity[i] = read_word(channel);
	}

	write_command(channel, ATA_CMD_FLUSH_CASH);

	printf("\tModel: ");
	for(int i = 27; i <= 46; i++)
		printf("%c%c", identity[i] >> 8, identity[i] & 0x00FF);

	printf("\n\tSerial: ");
	for(int i = 10; i <= 19; i++)
		printf("%c%c", identity[i] >> 8, identity[i] & 0x00FF);
	
	printf("\n\tVersion: ");
	for(int i = 23; i <= 26; i++)
		printf("%c%c", identity[i] >> 8, identity[i] & 0x00FF);

	printf("\n\tTotal memory: %lu MB", *((unsigned long*)(identity + 60)) / 2048);

	printf("\n\tInterface: %s\n", identity[0] & 0x80 ? "ATAPI" : "ATA");

	
	unsigned short dma = identity[63];
	unsigned short pio = identity[64];

	printf("\tSupports: ");
		
	char* print_delimeter = "";

	for(int version = 0, mask = 1; version <= 2; version++, mask <<= 1){
		if(dma & mask)
			printf("%sMultiword DMA %d", print_delimeter, version);

		print_delimeter = ", ";
	}

	for(int version = 0, mask = 1; version <= 7; version++, mask <<= 1){
		if(pio & mask)
			printf("%sPIO %d", print_delimeter, version);
	}

	printf("\n\n");
}

int main()
{
	ALLOW_IO_OPERATIONS;

	for(int i = 0; i < 4; i++){
		print_device_identity(i / 2, i % 2);
	}

	system("pause");
}