#!/bin/bash

cd trezor-mcu

TAG=v1.6.1
echo "build fw,tag:$TAG"

BINFILE=$PWD/build/trezor-$TAG.bin
ELFFILE=$PWD/build/trezor-$TAG.elf

#git clone https://github.com/trezor/trezor-mcu
git checkout $TAG
git submodule update --init
sudo make -C vendor/libopencm3
sudo make -C vendor/nanopb/generator/proto
sudo make -C firmware/protob
sudo make clean
sudo make

# 目前这一步还有问题待解决.
sudo make -C firmware sign

if [ $? -ne 0 ]; then
    echo -e "\033[31m #### make error! ####\n\033[0m" 
    exit 1
fi

if [ -e $PWD/build ]; then
    rm -rf $BINFILE $ELFFILE
else
    mkdir -p $PWD/build

fi
cp -a firmware/trezor.bin $BINFILE
cp -a firmware/trezor.elf $ELFFILE

/usr/bin/env python -c "
from __future__ import print_function
import hashlib
import sys
fn = sys.argv[1]
data = open(fn, 'rb').read()
print('\n\n')
print('Filename    :', fn)
print('Fingerprint :', hashlib.sha256(data[256:]).hexdigest())
print('Size        : %d bytes (out of %d maximum)' % (len(data), 491520))
" $BINFILE

echo -e "\033[32m #### make success! ####\n\033[0m" 
