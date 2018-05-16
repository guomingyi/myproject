#!/bin/bash

ACTION=$1
MY_IMAGE=linux_mcu_build_env.image

if [ "y$ACTION" = "yclean" ] || [ "y$ACTION" = "yc" ] ;then
  rm -rf ./build && \
  make clean && \
  make clean -C bootloader && \
  make clean -C firmware && \
  make clean -C firmware/protob && \
  make clean -C vendor/libopencm3
  echo -e "\033[32m #### clean done!#### \033[0m" 
  exit 0
fi

docker run -it -v $PWD:/tmp/build $MY_IMAGE /bin/sh -c "\
    cd /tmp/build && \
    ./build-bootloader-no-docker.sh && \
    ./build-firmware-no-docker.sh"

echo "ls -lh $PWD/build/"
ret=$(ls -lh $PWD/build)
echo -e "\033[32m$ret \033[0m" 

