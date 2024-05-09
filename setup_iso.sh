#!/bin/sh
# args: kernel_file, limine.cfg, limine-datadir
mkdir -p iso/boot/limine/
cp -v $1 iso/boot/
cp -v $2 $3/limine-bios.sys $3/limine-bios-cd.bin \
      $3/limine-uefi-cd.bin iso/boot/limine/
mkdir -p iso/EFI/BOOT/
cp -v $3/BOOTX64.EFI iso/EFI/BOOT/

