cmake_minimum_required(VERSION 3.13.0)
project(kernel C)

include_directories(${PROJECT_SOURCE_DIR}/../libk/include ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../vendor)

set(CMAKE_CXX_COMPILER_TARGET x86_64-unknown-none-elf)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
        -std=gnu11 
        -ffreestanding 
        -fno-stack-protector 
        -fno-stack-check 
        -fno-lto 
        -fno-pie 
        -fno-pic 
        -m64 
        -march=x86-64 
        -mabi=sysv 
        -mno-80387 
        -mno-mmx 
        -mno-sse 
        -mno-sse2 
        -mno-red-zone 
        -mcmodel=kernel 
        -masm=att 
        -Wall 
        -Wextra 
        -pedantic
)

set(kernel_src
        "descriptors/gdt.c"
        "device/display/framebuffer.c"
        "device/display/terminal.c"
        "device/pci/pci.c"
        "memory/pmm.c"
)

add_library(kernel OBJECT ${kernel_src})