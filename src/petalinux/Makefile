wrkDir:=${PWD}
srcDir:=$(dir $(lastword $(MAKEFILE_LIST)))
sdDir:=/media/ngrf


.PHONY:	all

all: clean project uboot kernel package

clean:
	rm -rf bpLinux

directories:
	@echo 'Source Directory: $(srcDir)'
	@echo 'Workspace Directory: $(wrkDir)'
	@echo 'SD card Directory: $(sdDir)'

create_project:
	petalinux-create -t project --template zynqMP --name bpLinux

config_project:
	petalinux-config -p bpLinux/ --get-hw-description=$(wrkDir)

config_uboot:
	petalinux-config -p bpLinux/ -c u-boot --silentconfig

config_kernel:
	cp -f $(srcDir)/system-user.dtsi $(wrkDir)/bpLinux/project-spec/meta-user/recipes-bsp/device-tree/files/
	petalinux-config -p bpLinux -c kernel

config_rootfs:
	petalinux-config -p bpLinux/ -c rootfs

fetch_rootfs: | ubuntu-20.04.3-minimal-armhf-2021-12-20.tar.xz
ubuntu-20.04.3-minimal-armhf-2021-12-20.tar.xz:
	wget -O rootfs.tar.gz https://rcn-ee.com/rootfs/eewiki/minfs/ubuntu-20.04.3-minimal-armhf-2021-12-20.tar.xz
	tar -xf rootfs.tar.gz
	wget -O rootfs.tar.gz https://people.canonical.com/~platform/images/xilinx/kria/iot-kria-classic-desktop-2004-x03-20211110-98-rootfs.tar.gz?_ga=2.82958110.921810150.1656536693-1790180461.1650332538 

	
build:
	petalinux-build -p bpLinux/

package:
	petalinux-package -p bpLinux/ --boot --force --fsbl images/linux/zynq_fsbl.elf --fpga images/linux/system.bit --u-boot

device_tree:
	dtc -I dtb -O dts -o bpLinux/images/linux/system.dts bpLinux/images/linux/system.dtb
	petalinux-package --bsp -p bpLinux/ --force --output bytepipe_x9002.bsp

package_apu:
	petalinux-package -p bpLinux/ --force --boot --fsbl --fpga --u-boot

package_apu&rpu:
	petalinux-package -p bpLinux/ --force --boot --fsbl --fpga --add ./rflan.elf --cpu=r5-0 --u-boot

sdcard:
	#sudo rm -rf $(sdDir)/rootfs/
	cp -f bpLinux/images/linux/BOOT.BIN $(sdDir)/BOOT/
	cp -f bpLinux/images/linux/boot.scr $(sdDir)/BOOT/
	cp -f bpLinux/images/linux/image.ub $(sdDir)/BOOT/
	#cp -f bpLinux/images/linux/rootfs.cpio.gz.u-boot $(sdDir)/BOOT/
	#sudo tar xfvp ./rootfs.tar -C $(sdDir)/rootfs/
	#sync


