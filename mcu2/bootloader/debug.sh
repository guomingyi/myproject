#!/bin/bash

ACTION=$1

st-util & arm-none-eabi-gdb ./bootloader.elf
