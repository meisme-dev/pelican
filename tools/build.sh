#!/usr/bin/env sh

TARGET=x86_64-elf

yes n | tools/toolchain.sh $TARGET

meson setup build --cross-file "$TARGET.ini" --cross-file "default.ini"
ninja clang-format -C build 
meson compile -C build 

tools/iso.sh
