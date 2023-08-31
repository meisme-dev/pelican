#!/usr/bin/env sh

rm -rf sysroot
mkdir -p sysroot

cp build/pelican \
limine.cfg \
vendor/limine/limine-bios.sys \
vendor/limine/limine-bios-cd.bin \
vendor/limine/limine-uefi-cd.bin \
-t sysroot

xorriso -as mkisofs -b limine-bios-cd.bin \
-no-emul-boot -boot-load-size 4 -boot-info-table \
--efi-boot limine-uefi-cd.bin \
--efi-boot-part --efi-boot-image --protective-msdos-label \
sysroot -o sysroot/pelican.iso

vendor/limine/limine bios-install sysroot/pelican.iso