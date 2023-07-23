#!/bin/bash

#COM1=/dev/tnt0
#COM2=/dev/tnt1
#
#NAME=Booter
#DISK=main.vfd
#
#vm_init(){
#    VBoxManage modifyvm $NAME --uart1 0x3F8 4 --uart-mode1=$COM1
#    VBoxManage modifyvm $NAME --uart2 0x2F8 3 --uart-mode2=$COM2
#    VBoxManage storageattach $NAME --storagectl Floppy --port 0 --device 0 --type fdd --medium $DISK
#}
#vm_init
#
#VBoxManage startvm $NAME

COM1=/dev/tnt0
COM2=/dev/tnt1

DISK=main.vfd

qemu-system-i386 -serial $COM1 -serial $COM2 -drive format=raw,file=$DISK
