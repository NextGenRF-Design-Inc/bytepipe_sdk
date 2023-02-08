PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))
SRCDIR := $(realpath $(dir $(PROJECT_DIR)/../))
SDK_SRCDIR := $(SRCDIR)
DEV := $(shell bash -c 'read -p "Select Device [ xczu3cg, xczu3eg, xczu2cg, xczu2eg ]: " dev; echo $$dev-sbva484-1-e')

.PHONY:	all

all: clean kernel package

clean:
	rm -rf bpLinux

kernel:
	petalinux-create -t project --template zynqMP --name bpLinux
	petalinux-config -p bpLinux/ --get-hw-description=$(wrkDir) --silentconfig
	cp -f $(srcDir)/system-user.dtsi $(wrkDir)/bpLinux/project-spec/meta-user/recipes-bsp/device-tree/files/
	petalinux-build -p bpLinux -c kernel

device_tree:
	dtc -I dtb -O dts -o bpLinux/images/linux/system.dts bpLinux/images/linux/system.dtb
	petalinux-package --bsp -p bpLinux/ --force --output bytepipe_x9002.bsp

package_apu:
	petalinux-package -p bpLinux/ --force --boot --fsbl --fpga --u-boot

package_apu&rpu:
	petalinux-package -p bpLinux/ --force --boot --fsbl --fpga --add ./rflan.elf --cpu=r5-0 --u-boot

sdcard:
	cp -f bpLinux/images/linux/BOOT.BIN /media/ngrf/BOOT/
	cp -f bpLinux/images/linux/boot.scr /media/ngrf/BOOT/
	cp -f bpLinux/images/linux/Image /media/ngrf/BOOT/
	cp -f bpLinux/images/linux/rootfs.cpio.gz.u-boot /media/ngrf/BOOT/
	#sudo tar -xf bpLinux/images/linux/rootfs.tar.gz -C /media/ngrf/rootfs/


