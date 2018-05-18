#!/bin/bash

# https://blog.csdn.net/a13526758473/article/details/60468652
# http://www.waveshare.net/Datasheet_PDF/STM32F205RG-PDF.html
# git clone https://github.com/texane/stlink.git

#sudo add-apt-repository ppa:george-edison55/cmake-3.x
sudo apt-get update
sudo apt-get install cmake libusb-dev libusb-1.0-0-dev

cd stlink-master && \
make release && cd build/Release && make install && \
sudo ldconfig && \
cd ../../ && \
sudo cp ./etc/udev/rules.d/49-stlinkv2.rules /etc/udev/rules.d && \
sudo udevadm control --reload-rules && \
sudo udevadm trigger

# test.
st-util -p 4242

#sudo st-flash write test.bin 0x8000000下载程序了
#It includes:
#a communication library (libstlink.a),
#a GDB server (st-util),
#a flash manipulation tool (st-flash).
#a programmer and chip information tool (st-info)

