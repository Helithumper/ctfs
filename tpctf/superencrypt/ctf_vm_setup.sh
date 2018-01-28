#!/bin/bash

# Allow installing i386 packages
sudo dpkg --add-architecture i386

# Make sure packages up to date
sudo apt-get update
sudo apt-get upgrade -y

# Install useful tools:
#
# vim                 -- Best text editor
# clang               -- Alternative compiler to gcc with many more features
# htop                -- Better process monitor than top
# build-essential     -- A bunch of useful tools such as gcc, objdump, strings, gdb, etc
# git                 -- Version control, also needed to clone gdb-peda
# python-setuptools   -- Required for pip. Not installed by default on some distributions
# python-wheel        -- Same as above
# python-wheel-common -- Same as above
# python-pip          -- The pip package manager for Python
# python-dev          -- Headers for Python needed for installing pwntools
# libssl-dev          -- Headers for OpenSSL needed for installing pwntools
# libc6:i386          -- 32-bit version of the standard C library, for running 32-bit programs
# gcc-multilib        -- Allow building 32-bit binaries with gcc
# strace              -- Trace system calls performed by a program
sudo apt-get install -y vim clang htop build-essential git python-setuptools python-wheel python-wheel-common python-pip python-dev libssl-dev libc6:i386 gcc-multilib strace

# Install pwntools
sudo -H pip install pwn

# Install gdb-peda
if [ ! -d ~/peda ]; then
	git clone https://github.com/longld/peda.git ~/peda
	echo "source ~/peda/peda.py" >> ~/.gdbinit
fi

# Install Python Imaging Library
sudo -H pip install pillow

# Check if this is running in VirtualBox or VMware
platform=$(sudo dmidecode -s system-product-name)
if [ "$platform" == "VirtualBox" ]; then
	# Remove Open VM Tools if it was installed previously
	sudo apt-get remove -y open-vm-tools
	
	# Tell the user how to install VirtualBox Guest Additions
	echo -e "\n\nNow, to enable support for copy/paste, drag&drop, and shared folders between your host and VM, you should install VirtualBox Guest Additions. Do this by going to VirtualBox's menu bar and clicking Devices, then click 'Insert Guest Additions CD Image...'. After this, a dialog box should pop up in your VM asking you to run the software on the CD. Click Run. After the installation completes, reboot your VM with 'sudo reboot'.\n"
elif [ "$platform" == "VMware Virtual Platform" ]; then
	# Install VMware Tools
	sudo apt-get install -y open-vm-tools-desktop
else
	echo "Unknown virtualization platform: $platform"
fi

# Finished installing tools
echo "All done!"
