CC = gcc
AS = as
LD = ld
CFLAGS = -std=gnu11 \
 		 -ffreestanding \
		 -fno-stack-protector \
		 -fno-stack-check \
		 -fno-lto -fno-pie \
		 -fno-pic \
		 -m64 \
		 -march=x86-64 \
		 -mabi=sysv \
		 -mno-80387 \
		 -mno-mmx \
		 -mno-sse \
		 -mno-sse2 \
		 -mno-red-zone \
		 -mcmodel=kernel \
		 -masm=att \
		 -Wall \
		 -pedantic \
		 -c 

LDFLAGS = -nostdlib -static -m elf_x86_64 -z max-page-size=0x1000 -T linker.ld
ASFLAGS = --64
SRC := $(shell find libk -name "*.c") $(shell find kernel -name "*.c")
OBJECTS := $(patsubst %.c, %.o, $(SRC))

all:
	mkdir -p build
	mkdir -p sysroot
	$(MAKE) -C vendor/limine
	$(MAKE) compile
	$(MAKE) create

rebuild:
	$(MAKE) clean
	$(MAKE) all

compile: $(OBJECTS)
	$(LD) -r -b binary -o assets/font.o assets/font.sfn

clean:
	rm -rf sysroot
	rm -rf build
	find . -name "*.o" -exec rm {} +

run:
	qemu-system-x86_64 -cdrom sysroot/pelican.iso -d int

create:
	$(LD) $(LDFLAGS) $(shell find assets -name "*.o") $(shell find libk -name "*.o") $(shell find kernel -name "*.o") -o build/pelican.elf
	cp build/pelican.elf limine.cfg vendor/limine/limine.sys vendor/limine/limine-cd.bin vendor/limine/limine-cd-efi.bin sysroot/
	xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-cd-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        sysroot -o sysroot/pelican.iso
	./vendor/limine/limine-deploy sysroot/pelican.iso

%.o: kernel/%.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: all compile clean run bin rebuild