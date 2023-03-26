cmake_minimum_required(VERSION 3.13.0)
project(pelican C)
include_directories(${PROJECT_SOURCE_DIR}/libk/include ${PROJECT_SOURCE_DIR}/vendor ${PROJECT_SOURCE_DIR}/kernel/include)

set(CMAKE_CXX_COMPILER_TARGET x86_64-unknown-none-elf)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib")
set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
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

find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/vendor
			RESULT_VARIABLE GIT_SUBMOD_RESULT)

		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()

add_subdirectory(kernel)
add_subdirectory(libk)
add_executable(pelican ${PROJECT_SOURCE_DIR}/kernel/kernel.c)

target_link_options(
		pelican PRIVATE
        LINKER:-static
        LINKER:-m
		LINKER:elf_x86_64
        LINKER:-z
		LINKER:max-page-size=0x1000
        LINKER:-T
		LINKER:${PROJECT_SOURCE_DIR}/linker.ld
)

execute_process(
				WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/assets
                COMMAND 
                ${CMAKE_LINKER}
				-r 
                -b binary 
				font.sfn
                -o ${CMAKE_CURRENT_BINARY_DIR}/font.o
				-m elf_x86_64
)

target_link_libraries(pelican ${CMAKE_CURRENT_BINARY_DIR}/font.o libk kernel)

add_custom_command(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/sysroot/
				   TARGET pelican
				   POST_BUILD
)

add_custom_command(COMMAND ${CMAKE_COMMAND} -E copy 	
				${CMAKE_CURRENT_BINARY_DIR}/pelican.elf
				${CMAKE_SOURCE_DIR}/limine.cfg 
				${CMAKE_SOURCE_DIR}/vendor/limine/limine.sys
				${CMAKE_SOURCE_DIR}/vendor/limine/limine-cd.bin
				${CMAKE_SOURCE_DIR}/vendor/limine/limine-cd-efi.bin
				${CMAKE_CURRENT_BINARY_DIR}/sysroot/
				TARGET pelican
				POST_BUILD
)

add_custom_command(
				WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMAND xorriso
				-as mkisofs -b limine-cd.bin
                -no-emul-boot -boot-load-size 4 -boot-info-table
				--efi-boot limine-cd-efi.bin
                --efi-boot-part --efi-boot-image --protective-msdos-label
				sysroot -o sysroot/pelican.iso
				TARGET pelican
				POST_BUILD
)

add_custom_command(
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMAND vendor/limine/limine-deploy ${CMAKE_CURRENT_BINARY_DIR}/sysroot/pelican.iso
	TARGET pelican
	POST_BUILD
)
