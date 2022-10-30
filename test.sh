#!/bin/sh

export TEST_ENV="test"

. ./iso.sh

clear

qemu-system-i386 -kernel sysroot/boot/tests.kernel -serial stdio -display none -device isa-debug-exit,iobase=0xf4,iosize=0x04
