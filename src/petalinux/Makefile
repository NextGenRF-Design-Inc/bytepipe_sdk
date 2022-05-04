wrkDir:=${PWD}
srcDir:=$(dir $(lastword $(MAKEFILE_LIST)))

.PHONY:	all

all:directories project dts build package output

directories:
	@echo 'Source Directory: $(srcDir)'
	@echo 'Build Directory: $(wrkDir)'

fetch:
	git clone --branch 2019_R2 https://github.com/analogdevicesinc/meta-adi.git

project:
	petalinux-create -t project --template zynqMP --name bpLinux
	petalinux-config -p bpLinux/ --get-hw-description=$(wrkDir)

dts:
	cp $(srcDir)/zynqmp-bytepipe.dts bpLinux/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi
	@echo "KERNEL_DTB=\"system-user\"" >> bpLinux/project-spec/meta-user/conf/petalinuxbsp.conf

kernel: 
	petalinux-config -p bpLinux/ -c kernel

build:
	petalinux-build -p bpLinux/

package:
	petalinux-package -p bpLinux/ --boot --fsbl bpLinux/images/linux/zynqmp_fsbl.elf --fpga bpLinux/images/linux/system.bit --pmufw bpLinux/images/linux/pmufw.elf --atf bpLinux/images/linux/bl31.elf --u-boot bpLinux/images/linux/u-boot.elf 

output:
	cp bpLinux/images/linux/image.ub ./
	cp bpLinux/images/linux/BOOT.BIN ./
	cp bpLinux/images/linux/system.dtb ./
	cp bpLinux/images/linux/zynqmp_fsbl.elf ./
	cp bpLinux/images/linux/rootfs.tar.gz ./


	

