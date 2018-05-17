#!/bin/bash
#git clone https://github.com/trezor/trezor-mcu

if [ -f "$PWD/vendor/libopencm3/Makefile" ]; then
  echo "<~~>"
else

echo -e "\033[32m fetch submodule..\n\033[0m" && \
cd vendor && \
git clone https://github.com/libopencm3/libopencm3.git  && \
cd libopencm3 && \
git checkout b0e050d10d12c42be031c34822117cfd3c5a0ea7 && \
echo -e "\033[32m #### libopencm3 b0e050d10d12c42be031c34822117cfd3c5a0ea7 ####\n\033[0m" && \
cd ../ && \
\
git clone https://github.com/nanopb/nanopb.git && \
cd nanopb && \
git checkout 71ba4e68da4b3c986d454e34c4666a82fbdf4176 && \
echo -e "\033[32m #### nanopb 71ba4e68da4b3c986d454e34c4666a82fbdf4176 ####\n\033[0m"  && \
cd ../ && \
\
git clone https://github.com/trezor/trezor-common.git && \
cd trezor-common && \
git checkout 0924bd6826bb63f66010e2e511356d54ea733df3 && \
echo -e "\033[32m #### trezor-common 0924bd6826bb63f66010e2e511356d54ea733df3 ####\n\033[0m"  && \
cd ../ && \
\
git clone https://github.com/trezor/trezor-crypto.git && \
cd trezor-crypto && \
git checkout bb4c3d052561bd31856a03d975ca226571f6a893 && \
echo -e "\033[32m #### trezor-crypto bb4c3d052561bd31856a03d975ca226571f6a893 ####\n\033[0m"  && \
cd ../ && \
\
git clone https://github.com/trezor/trezor-qrenc.git && \
cd trezor-qrenc && \
git checkout 91115b1806aa506d819c0063893984a28a5ae3a5 && \
echo -e "\033[32m #### trezor-qrenc 91115b1806aa506d819c0063893984a28a5ae3a5 ####\n\033[0m"  && \
cd ../ && \
cd ../ && \
\
echo -e "\033[32mdone.\n\033[0m" 

fi

