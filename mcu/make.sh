#!/bin/bash

# submodule.
./fetch-submodule.sh

# docker images
# $ sudo docker images
# REPOSITORY                             TAG                 IMAGE ID            CREATED             SIZE
# linux_mcu_build_env.image              latest              4421482ea61f        25 hours ago        903MB
IMAGE=mingyi/mcu.envx

TAG_bl=bl1.4.0
TAG_FW=v1.6.1
BINFILE_BL=build/bootloader-$TAG_bl.bin
ELFFILE_BL=build/bootloader-$TAG_bl.elf
BINFILE_FW=build/firmware-$TAG_FW.bin
ELFFILE_FW=build/firmware-$TAG_FW.elf

# clean
ACTION=$1
if [ "y$ACTION" = "yclean" ] || [ "y$ACTION" = "yc" ] || [ "y$ACTION" = "yall" ] ;then
  rm -rf ./build/* && \
  make clean && \
  make clean -C bootloader && \
  make clean -C firmware && \
  make clean -C firmware/protob && \
  make clean -C vendor/libopencm3
  echo -e "\033[32m #### clean done!#### \033[0m" 
  if [ ! "y$ACTION" = "yall" ];then
      exit 0;
  fi
fi

if [ ! -d ./build ];then
  mkdir ./build
fi

build_path=$PWD
echo "build path: $build_path"

if [ "y$ACTION" = "ybl" ] ;then
  docker run -it -v $build_path:$build_path $IMAGE /bin/sh -c "\
    cd $build_path && \
    make clean && \
    make clean -C bootloader && \
    make && \
    make -C bootloader && \
    cp bootloader/bootloader.bin $BINFILE_BL && \
    cp bootloader/bootloader.elf $ELFFILE_BL \
  "
  exit 0
fi


if [ "y$ACTION" = "yfw" ] ;then
  docker run -it -v $build_path:$build_path $IMAGE /bin/sh -c "\
    cd $build_path && \
    make clean && \
    make clean -C firmware && \
    make && \
    make -C firmware && \
    cp firmware/trezor.bin $BINFILE_FW && \
    cp firmware/trezor.elf $ELFFILE_FW \
  "
  exit 0
fi

# docker build
# if [ "y$ACTION" = "yall" ] ;then
  docker run --rm -it -v $build_path:$build_path $IMAGE /bin/sh -c "\
    cd $build_path && \
    make -C vendor/libopencm3 && \
    make && \
    make -C bootloader && \
    cp bootloader/bootloader.bin $BINFILE_BL && \
    cp bootloader/bootloader.elf $ELFFILE_BL && \
    make -C vendor/libopencm3 && \
    make -C vendor/nanopb/generator/proto && \
    make -C firmware/protob && \
    make && \
    make -C firmware && \
    cp firmware/trezor.bin $BINFILE_FW && \
    cp firmware/trezor.elf $ELFFILE_FW \
  "
# fi

#docker rm `docker ps -a|sed -n '2p'|awk '{print $1}'`

# show
/usr/bin/env python -c "
from __future__ import print_function
import hashlib
import sys
for arg in sys.argv[1:]:
  (fn, max_size) = arg.split(':')
  data = open(fn, 'rb').read()
  print('\n\n')
  print('Filename    :', fn)
  print('Fingerprint :', hashlib.sha256(hashlib.sha256(data).digest()).hexdigest())
  print('Size        : %d bytes (out of %d maximum)' % (len(data), int(max_size, 10)))
" $BINFILE_BL:32768 $BINFILE_FW:491520

# echo -e "\033[32m\nDone. \033[0m"
#echo -e "\033[32m$(ls -lh $PWD/build) \033[0m"

