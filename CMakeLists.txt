CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(os)

INCLUDE_DIRECTORIES(include)

STRING(COMPARE EQUAL "$ENV{ARCH}" "i386" res)
IF(res)
  SET(ARCH "i386")
  SET(CMAKE_ASM_NASM_OBJECT_FORMAT "elf32")
  SET(CMAKE_C_COMPILER "i686-elf-gcc")
  MESSAGE("TARGET ARCHITECTURE: i686")
ELSE()
  SET(ARCH "x86_64")
  SET(CMAKE_ASM_NASM_OBJECT_FORMAT "elf64")
  SET(CMAKE_C_COMPILER "x86_64-elf-gcc")
  SET(CMAKE_C_FLAGS "-mno-red-zone -mcmodel=large")
  MESSAGE("TARGET ARCHITECTURE: x86_64")
ENDIF()

EXECUTE_PROCESS(COMMAND limine --print-datadir OUTPUT_VARIABLE LIMINE_DATADIR)
ENABLE_LANGUAGE(ASM_NASM)

SET(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/linker.ld")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -nostdlib -nostdinc -O2 -g")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "") # removes -rdynamic

FILE(GLOB_RECURSE kernel_C_SRC "kernel/*.c")
FILE(GLOB_RECURSE kernel_ARCH_ASM_SRC "arch/${ARCH}/*.nasm")
FILE(GLOB_RECURSE kernel_ARCH_C_SRC "arch/${ARCH}/*.c")

ADD_EXECUTABLE(os.kernel ${kernel_C_SRC} ${kernel_ARCH_ASM_SRC} ${kernel_ARCH_CSRC})
SET_TARGET_PROPERTIES(os.kernel PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT} -lgcc")

ADD_CUSTOM_COMMAND(
  OUTPUT os.iso
  COMMAND ${CMAKE_CURRENT_LIST_DIR}/setup_iso.sh ${CMAKE_BINARY_DIR}/os.kernel
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/limine.cfg
                                                 ${LIMINE_DATADIR}
  COMMAND xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin
          -no-emul-boot -boot-load-size 4 -boot-info-table
          --efi-boot boot/limine/limine-uefi-cd.bin
          --efi-boot-part --efi-boot-image --protective-msdos-label
          iso -o os.iso
  COMMAND rm -rf iso
  COMMAND limine bios-install os.iso
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/limine.cfg ${CMAKE_BINARY_DIR}/os.kernel
  VERBATIM)

ADD_CUSTOM_TARGET(iso DEPENDS os.iso)

