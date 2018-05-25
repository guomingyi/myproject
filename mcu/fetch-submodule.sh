#!/bin/bash
#git clone https://github.com/trezor/trezor-mcu

if [ -f "$PWD/vendor/libopencm3/Makefile" ]; then
  echo "<~~>"
else

if [ ! -d ./vendor ];then
  mkdir ./vendor
fi

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
git checkout 54c34a9fda152937d4cd0c7fd85c067fca23af75 && \
echo -e "\033[32m #### nanopb 54c34a9fda152937d4cd0c7fd85c067fca23af75 ####\n\033[0m"  && \
cd ../ && \
\
git clone https://github.com/trezor/trezor-common.git && \
cd trezor-common && \
git checkout 7325936f8612ce1cadff6cfb0c0045b3668c3260 && \
echo -e "\033[32m #### trezor-common 7325936f8612ce1cadff6cfb0c0045b3668c3260 ####\n\033[0m"  && \
cd ../ && \
\
git clone https://github.com/trezor/trezor-crypto.git && \
cd trezor-crypto && \
git checkout 95a522bf1a453880050521661258d7943e966d1f && \
echo -e "\033[32m #### trezor-crypto 95a522bf1a453880050521661258d7943e966d1f ####\n\033[0m"  && \
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

