#!/bin/bash

ACTION=$1

echo "target remote :4242" > ~/.gdbinit
st-util & arm-none-eabi-gdb ./bootloader.elf
