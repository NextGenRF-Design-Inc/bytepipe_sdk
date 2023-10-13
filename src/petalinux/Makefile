wrkDir:=${PWD}
srcDir:=$(dir $(lastword $(MAKEFILE_LIST)))

.PHONY:	all

all: clean project config build package

clean:
	rm -rf bpLinux

project:
	petalinux-create -t project --template zynqMP --name bpLinux
	petalinux-config -p bpLinux/ --get-hw-description=$(wrkDir)/rflan --silentconfig

config:
	cp -f $(srcDir)sw/rootfs_config $(wrkDir)/bpLinux/project-spec/configs/
	cp -f $(srcDir)sw/config $(wrkDir)/bpLinux/project-spec/configs/
	cp -f $(srcDir)sw/system-user.dtsi $(wrkDir)/bpLinux/project-spec/meta-user/recipes-bsp/device-tree/files/
	petalinux-create -p bpLinux/ -t apps --template c -n rflan --enable
	rm -rf $(wrkDir)/bpLinux/project-spec/meta-user/recipes-apps/rflan
	cp -rf $(srcDir)/sw/rflan $(wrkDir)/bpLinux/project-spec/meta-user/recipes-apps/
	petalinux-create -p bpLinux/ -t apps --template c -n sampleapp --enable			
	petalinux-config -p bpLinux/ -c rootfs --silentconfig

build:
	petalinux-build -p bpLinux

package:
	#cp -f $(srcDir)/../fsbl/zynqmp_fsbl.elf $(wrkDir)/bpLinux/images/linux/
	petalinux-package -p bpLinux/ --force --boot --fsbl --fpga --u-boot
	#petalinux-package -p bpLinux/ --force --boot --fsbl --fpga --add ./rpu.elf --cpu=r5-0 --u-boot
	dtc -I dtb -O dts -o bpLinux/images/linux/system.dts bpLinux/images/linux/system.dtb
	petalinux-package --bsp -p bpLinux/ --force --output bpLinux/images/linux/bytepipe_x9002.bsp

sdcard:
	cp -f bpLinux/images/linux/BOOT.BIN /media/ngrf/boot/
	cp -f bpLinux/images/linux/boot.scr /media/ngrf/boot/
	cp -f bpLinux/images/linux/Image /media/ngrf/boot/
	cp -f bpLinux/images/linux/rootfs.cpio.gz.u-boot /media/ngrf/boot/
	#sudo rm -rf /media/ngrf/rootfs/ || true;
	#sudo tar -xvf bpLinux/images/linux/rootfs.tar.gz -C /media/ngrf/rootfs/
	#sync /media/ngrf/rootfs



