CC = x86_64-elf-gcc
AS = x86_64-elf-as
LD = x86_64-elf-ld
CFLAGS = -m32 -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -pedantic
LDFLAGS = -m elf_i386 -T linker.ld
ASFLAGS = --32
SRC := $(shell find . -name "*.c")
OBJECTS := $(patsubst %.c, %.o, $(SRC))

all:
	mkdir -p build/boot/grub
	$(AS) $(ASFLAGS) kernel/boot.S -o kernel/boot.o
	$(MAKE) compile
	$(MAKE) create

rebuild:
	$(MAKE) clean
	$(MAKE) all

compile: $(OBJECTS)

clean:
	rm -rf build
	find . -name "*.o" -exec rm {} +

run:
	qemu-system-x86_64 -cdrom build/boot/bkrnl.iso

create:
	$(LD) $(LDFLAGS) $(shell find . -name "*.o") -o build/boot/bkrnl.bin
	cp grub.cfg build/boot/grub/grub.cfg
	grub2-mkrescue -o build/boot/bkrnl.iso build

%.o: kernel/%.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: all compile clean run bin rebuild