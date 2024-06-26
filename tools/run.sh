#!/usr/bin/env sh

set -e

tools/build.sh

QEMU_BINARY="qemu-system-x86_64"
EXTRA_ARGS=""
ACCELERATORS="$(${QEMU_BINARY} -accel help)"

if [ "${OVMF_PATH}" = "" ]; then
  EXTRA_ARGS=$EXTRA_ARGS"-bios ${OVMF_PATH} \
  "
fi

if [ "${DEBUG}" != "YES" ]; then
  case "${ACCELERATORS}" in *kvm*)
    EXTRA_ARGS=$EXTRA_ARGS"-accel kvm \
    "
  ;;
  esac
fi

QEMU_ARGS="-d int \
           -cdrom sysroot/pelican.iso \
           -M smm=off \
           -m 256m \
           -smp $(nproc) \
	   -s \
           $EXTRA_ARGS \
           -no-shutdown" 

${QEMU_BINARY} ${QEMU_ARGS} "$@"
