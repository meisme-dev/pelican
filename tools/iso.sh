#!/usr/bin/env sh

rm -rf sysroot
mkdir -p sysroot

cp build/pelican \
limine.cfg \
vendor/limine/limine.sys \
vendor/limine/limine-cd.bin \
vendor/limine/limine-cd-efi.bin \
-t sysroot

xorriso -as mkisofs -b limine-cd.bin \
-no-emul-boot -boot-load-size 4 -boot-info-table \
--efi-boot limine-cd-efi.bin \
--efi-boot-part --efi-boot-image --protective-msdos-label \
sysroot -o sysroot/pelican.iso

vendor/limine/limine-deploy sysroot/pelican.iso