#!/bin/bash
# set -e

cd trezor-mcu

TAG=bl1.4.0
echo "build bootloader: $PWD, tag: $TAG"

BINFILE=$PWD/build/bootloader-$TAG.bin
ELFFILE=$PWD/build/bootloader-$TAG.elf

git checkout $TAG
git submodule update --init
sudo make -C vendor/libopencm3
sudo make
sudo make -C bootloader align

if [ $? -ne 0 ]; then
    echo -e "\033[31m #### make error! ####\n\033[0m" 
    exit 1
fi

if [ -e $PWD/build ] ;then
    rm -rf $BINFILE $ELFFILE
else
    mkdir -p $PWD/build
fi

cp -a bootloader/bootloader.bin $BINFILE
cp -a bootloader/bootloader.elf $ELFFILE
# tree $PWD/build

/usr/bin/env python -c "
from __future__ import print_function
import hashlib
import sys
fn = sys.argv[1]
data = open(fn, 'rb').read()
print('\n\n')
print('Filename    :', fn)
print('Fingerprint :', hashlib.sha256(hashlib.sha256(data).digest()).hexdigest())
print('Size        : %d bytes (out of %d maximum)' % (len(data), 32768))
" $BINFILE

echo -e "\033[32m #### make success! ####\n\033[0m" 
