#!/bin/bash -x

SRCDIR="./src/3"
BUILDDIR="./build"

#mkdir build

#as --32 src/0/boot/grub/multiboot.S -o ${BUILDDIR}/boot.o
gcc -m32 -I${SRCDIR} -c ${SRCDIR}/boot/grub/multiboot.S -o ${BUILDDIR}/boot.o -O2 -Wall -Wextra

gcc -m32 -I${SRCDIR} -c ${SRCDIR}/kernel/kernel.c -o ${BUILDDIR}/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gcc -m32 -I${SRCDIR} -T linker.ld -o ${BUILDDIR}/myos.bin -ffreestanding -O2 -nostdlib ${BUILDDIR}/boot.o ${BUILDDIR}/kernel.o -lgcc -Wl,--build-id=none

