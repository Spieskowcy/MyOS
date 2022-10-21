#!/bin/sh
set -e
. ./iso.sh

qemu-system-i386 -kernel sysroot/boot/myos.kernel
