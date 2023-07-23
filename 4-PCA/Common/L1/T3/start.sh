#!/bin/bash
#

COM1=/dev/tnt0
COM2=/dev/tnt1

DISK=main.vfd

qemu-system-i386 -serial $COM1 -serial $COM2 -drive format=raw,file=$DISK
