#!/bin/bash -x

BUILDDIR="./src/build"

mkdir -p ${BUILDDIR}/isodir/boot/grub
cp ${BUILDDIR}/nullos ${BUILDDIR}/isodir/boot/myos.bin
cp grub.cfg ${BUILDDIR}/isodir/boot/grub/grub.cfg
grub-mkrescue -o ${BUILDDIR}/myos.iso ${BUILDDIR}/isodir
