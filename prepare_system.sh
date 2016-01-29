#!/bin/bash

# installing arm toolchain and needed packages
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install gcc-arm-none-eabi
sudo apt-get install automake
sudo apt-get install make
sudo apt-get install autoconf pkg-config libusb-1.0 git
sudo apt-get install libusb-1.0-0 libusb-1.0-0-dbg libusb-1.0-0-dev

# Allow Users Access to USB Devices
sudo touch /etc/udev/rules/45-usb-stlink-v2.rules
sudo sh -c 'SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666"' > /etc/udev/rules/45-usb-stlink-v2.rules
sudo service udev restart

if [ -n "$(lsusb -d 0483:)" ]; then
    echo "## Please, disconnect board and connect it again."
    while [ -n "$(lsusb -d 0483:)" ]; do
        sleep 1
    done
fi
# Building stlink
cd ./stlink
./autogen.sh
./configure
make
cd -

cat ~/.bashrc > ~/.bashrc.old
sed -i '/stlink/d' ~/.bashrc
echo "export PATH=\$PATH:$PWD/stlink" >> ~/.bashrc
source ~/.bashrc

