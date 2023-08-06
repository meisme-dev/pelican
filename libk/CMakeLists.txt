cmake_minimum_required(VERSION 3.13.0)
project(libk C ASM)
include_directories(${PROJECT_SOURCE_DIR}/include)

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
	-fno-omit-frame-pointer
        -pedantic
)

set(libk_src
        "stdlib.c"
        "string.c"
        "sys/io.c"
        "sys/list.c"
)

add_library(libk OBJECT ${libk_src})
