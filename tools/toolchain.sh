#!/usr/bin/env sh
set -e

BINUTILS_VERSION="2.41"
GCC_VERSION="13.2.0"

BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.xz"
GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.xz"
CORES=$(nproc)
TARGET=x86_64-elf
PREFIX=$(readlink -f toolchain)

if [ "$1" ]; then
  TARGET=$1
fi

export PATH="$PREFIX/bin"

if [ -e "toolchain/bin/$TARGET-gcc" ]; then 
  printf "Your toolchain is likely already built. Continue? (Y/n) "
  read -r continue
  
  if [ "$continue" = "${continue#[Yy]}" ]; then
    echo Exiting
    exit
  fi
  
  echo Continuing
fi

sleep 3

mkdir -p toolchain/binutils
mkdir -p toolchain/gcc

echo Downloading sources
[ -e "toolchain/binutils.tar.xz" ] || curl $BINUTILS_URL -o toolchain/binutils.tar.xz
[ -e "toolchain/gcc.tar.xz" ] || curl $GCC_URL -o toolchain/gcc.tar.xz
echo Done

echo Extracting sources
tar -x --skip-old-files -f toolchain/binutils.tar.xz -C toolchain/binutils
tar -x --skip-old-files -f toolchain/gcc.tar.xz -C toolchain/gcc
echo Done

echo Compiling binutils
mkdir -p toolchain/binutils/build
cd toolchain/binutils/build
../binutils-$BINUTILS_VERSION/configure --target="$TARGET" --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
cd ../../
echo Done

echo Compiling gcc
mkdir -p gcc/build
cd gcc/gcc-$GCC_VERSION
./contrib/download_prerequisites

echo "MULTILIB_OPTIONS += mno-red-zone \
MULTILIB_DIRNAMES += no-red-zone" \
> gcc/config/i386/t-x86_64-elf

sed '/^x86_64-\*-elf\*).*/a\	tmake_file="\${tmake_file} i386/t-x86_64-elf"' gcc/config.gcc

cd ../build
../gcc-$GCC_VERSION/configure --target="$TARGET" --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-werror CXXFLAGS="-Wno-format-security"
make all-gcc -j"$CORES"
make all-target-libgcc -j"$CORES"
make install-gcc
make install-target-libgcc
cd ../../../
echo Done

echo Toolchain now built
echo Make sure "$PREFIX/bin" is in your \$PATH