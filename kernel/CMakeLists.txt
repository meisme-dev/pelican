cmake_minimum_required(VERSION 3.13.0)
project(kernel C ASM)

include_directories(${PROJECT_SOURCE_DIR}/../libk/include ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../vendor)

set(CMAKE_CXX_COMPILER_TARGET x86_64-unknown-none-elf)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ARCH x86-64)

add_compile_options(
        -std=gnu11 
        -ffreestanding 
        -fno-stack-protector 
        -fno-stack-check 
        -fno-lto 
        -fPIC
        -fPIE
        -m64 
        -march=${ARCH} 
	-ffile-prefix-map=${CMAKE_SOURCE_DIR}=
        -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=
        -mabi=sysv 
        -mno-80387 
        -mno-mmx 
        -mno-sse 
        -mno-sse2 
        -mno-red-zone 
        -masm=att 
        -Wall 
        -Wextra 
        -pedantic
)

set(kernel_src
        "descriptors/gdt.c"
        "descriptors/idt.c"
        "descriptors/idt.s"
        "device/display/framebuffer.c"
        "device/display/terminal.c"
        "device/display/log.c"
        "device/serial/serial.c"
        "device/pci/pci.c"
        "system/panic.c"
        "memory/pmm.c"
)

add_library(kernel OBJECT ${kernel_src})
