PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))
SRCDIR := $(realpath $(dir $(PROJECT_DIR)/../))
SDK_SRCDIR := $(SRCDIR)
xsa_file = $(shell bash -c 'read -p "Enter relative or absolute path of desired .xsa: " xsafile; echo $$xsafile')
fsbl_file = $(shell bash -c 'read -p "Enter relative or absolute path of desired fsbl.elf: " fsblfile; echo $$fsblfile')
rpu_file = $(shell bash -c 'read -p "Enter relative or absolute path of desired rpu.elf: " rpufile; echo $$rpufile')
SD_DIR= $(shell bash -c 'read -p "Enter relative or absolute path to SD card (ie. /media/ngrf/BOOT): " sddir; echo $$sddir')

.PHONY:	all

all: clean kernel device_tree custom_fsbl package_apu&rpu

clean:
	rm -rf bpLinux

kernel:
	petalinux-create -t project --template zynqMP --name bpLinux
	petalinux-config -p bpLinux/ --get-hw-description=$(xsa_file) --silentconfig
	cp -f $(PROJECT_DIR)/system-user.dtsi bpLinux/project-spec/meta-user/recipes-bsp/device-tree/files/
	petalinux-build -p bpLinux -c kernel

device_tree:
	dtc -I dtb -O dts -o bpLinux/images/linux/system.dts bpLinux/images/linux/system.dtb
	petalinux-package --bsp -p bpLinux/ --force --output bytepipe_x9002.bsp

custom_fsbl:
	cp -f $(fsbl_file) bpLinux/images/linux

package_apu:
	petalinux-package -p bpLinux/ --force --boot --fsbl --fpga --u-boot

package_apu&rpu:
	petalinux-package -p bpLinux/ --force --boot --fsbl --fpga --add $(rpu_file) --cpu=r5-0 --u-boot

sdcard:
	cp -f bpLinux/images/linux/BOOT.BIN $(SD_DIR)
	cp -f bpLinux/images/linux/boot.scr $(SD_DIR)
	cp -f bpLinux/images/linux/Image $(SD_DIR)
	cp -f bpLinux/images/linux/rootfs.cpio.gz.u-boot $(SD_DIR)
	#sudo tar -xf bpLinux/images/linux/rootfs.tar.gz -C $(SD_DIR)


