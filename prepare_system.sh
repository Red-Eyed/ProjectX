#!/bin/bash

# installing arm toolchain and needed packages
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install gcc-arm-none-eabi
sudo apt-get install automake
sudo apt-get install make
sudo apt-get install autoconf pkg-config libusb-1.0 git
sudo apt-get install libusb-1.0-0 libusb-1.0-0-dbg libusb-1.0-0-dev
sudo aptget install openocd

# Allow Users Access to USB Devices
ST_LINK_FILE_RULES="45-usb-stlink-v2.rules"
echo "SUBSYSTEM==\"usb\", ATTR{idVendor}==\"0483\", ATTR{idProduct}==\"3748\", MODE=\"0666\", OWNER=\"$USER\"" > $ST_LINK_FILE_RULES
sudo sh -c "mv $ST_LINK_FILE_RULES /etc/udev/rules.d/"
sudo service udev restart

if [ -n "$(lsusb -d 0483:)" ]; then
    echo "## Please, disconnect board and connect it again."
    while [ -n "$(lsusb -d 0483:)" ]; do
        sleep 1
    done
fi

# initialization and downloading the third party projects
git submodule init
git submodule update

# Building and installing stlink
cd ./stlink
./autogen.sh
./configure
make
sudo make install
cd -

