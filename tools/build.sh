#!/usr/bin/env sh

set -e

TARGET=x86_64-elf

yes n | tools/toolchain.sh $TARGET

git submodule update --init --remote --recursive || true

make -C vendor/limine
meson setup build --cross-file "$TARGET.ini" --cross-file "default.ini"
ninja clang-format -C build
meson compile -C build

tools/iso.sh
