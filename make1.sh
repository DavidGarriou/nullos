#!/bin/bash -x

TOPDIR="./src/1"
INC="${TOPDIR}/include"
SRC=${TOPDIR}
BUILD="./build"

#mkdir build

#as --32 src/0/boot/grub/multiboot.S -o ${BUILD}/boot.o
gcc -m32 -I${SRC} -c ${SRC}/boot/grub/multiboot.S -o ${BUILD}/boot.o -O2 -Wall -Wextra

gcc -m32 -I${INC} -I${SRC} -I${SRC}/drivers -c ${SRC}/drivers/x86_videomem.c -o ${BUILD}/x86_videomem.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gcc -m32 -I${INC} -I${SRC} -I${SRC}/drivers -c ${SRC}/drivers/klibc.c -o ${BUILD}/klibc.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gcc -m32 -I${INC} -I${SRC} -c ${SRC}/kernel/kernel.c -o ${BUILD}/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gcc -m32 -I${SRC} -I${SRC}/drivers -T linker.ld -o ${BUILD}/myos.bin -ffreestanding -O2 -nostdlib ${BUILD}/boot.o ${BUILD}/klibc.o ${BUILD}/x86_videomem.o ${BUILD}/kernel.o -lgcc -Wl,--build-id=none

