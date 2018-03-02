#!/bin/bash -x

BUILDDIR="./build"

mkdir -p ${BUILDDIR}/isodir/boot/grub
cp ${BUILDDIR}/myos.bin ${BUILDDIR}/isodir/boot/myos.bin
cp grub.cfg ${BUILDDIR}/isodir/boot/grub/grub.cfg
grub-mkrescue -o ${BUILDDIR}/myos.iso ${BUILDDIR}/isodir
