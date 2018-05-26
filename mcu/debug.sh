#!/bin/bash

ACTION=$1

# auto connect gdbserver
echo "target remote : 4242 > ~/.gdbinit"

# auto enable gdb-tui
echo "- >> ~/.gdbinit"

if [ "y$ACTION" = "yb" ] ;then
  echo -e "\033[32m\n debug bootloader \033[0m"
  st-util & arm-none-eabi-gdb ./build/bootloader-bl1.4.0.elf
  exit 0
fi

if [ "y$ACTION" = "yf" ]; then
  echo -e "\033[32m\n debug firmware. \033[0m"
  st-util & arm-none-eabi-gdb ./build/firmware-v1.6.1.elf
  exit 0
fi

# default.
st-util & arm-none-eabi-gdb ./build/bootloader-bl1.4.0.elf
