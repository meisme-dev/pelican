#!/usr/bin/env sh
yes n | tools/toolchain.sh

meson setup build --cross-file x86_64.ini --cross-file default.ini
meson compile -C build 

tools/iso.sh