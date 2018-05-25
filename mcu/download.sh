#!/bin/bash

ACTION=$1

if [ "y$ACTION" = "yb" ] ;then
  echo -e "\033[32m\n bootloader-bl1.4.0.bin 0x08000000. \033[0m"
  st-flash write build/bootloader-bl1.4.0.bin 0x08000000
  exit 0
fi

if [ "y$ACTION" = "yf" ]; then
  echo -e "\033[32m\n firmware-v1.6.1.bin 0x08010000. \033[0m"
  st-flash write build/firmware-v1.6.1.bin 0x08010000
  exit 0
fi

echo -e "\033[32m\n firmware-v1.6.1.bin 0x08010000. \033[0m"
st-flash write build/firmware-v1.6.1.bin 0x08010000

echo -e "\033[32m\n bootloader-bl1.4.0.bin 0x08000000. \033[0m"
st-flash write build/bootloader-bl1.4.0.bin 0x08000000


