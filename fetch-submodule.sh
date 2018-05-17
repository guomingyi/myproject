#!/bin/bash

cd src/
echo -e "\033[32m #### submodule update ... ####\n\033[0m" 

git clone https://github.com/trezor/trezor-crypto.git vendor/trezor-crypto
git clone https://github.com/trezor/trezor-common.git vendor/trezor-common
git clone https://github.com/trezor/trezor-qrenc.git vendor/trezor-qrenc
git clone https://github.com/libopencm3/libopencm3.git vendor/libopencm3
git clone https://github.com/nanopb/nanopb.git vendor/nanopb

echo -e "\033[32m #### done! ####\n\033[0m" 
exit 0
