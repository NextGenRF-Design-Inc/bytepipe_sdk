![logo](/docs/BytePipe_Logo.png)

---

# Linux OS Boot options using a .wic image
This tutorial assumes that you have already created your petalinux project using petalinux-create
<br />

### Create SD Boot Image:
**(1)** Use petalinux-config to designate rootfs type as EXT4

```
petalinux-config -p <path to petalinux project directory>/ --get-hw-description=<path to directory that contains .xsa file>/<.xsa file>
```

![image_pack_config](/docs/image_pack_config.png)

![root_fs_type](/docs/root_fs_type.png)

![ext4](/docs/ext4.png)

**(2)** Use petalinux-config to designate the partition in which the root file system exists

![dev_node_sd_device](/docs/dev_node_sd_device.png)

My sd-card mmc device is set to be mmc0 in my device tree. "mmc0" is associated with "mmcblk0"

![edit_dev_node_sd_device](/docs/edit_dev_node_sd_device.png)

**(3)** Now, exit the config menu to lock-in changes.

![exit_config](/docs/exit_config.png)

**(4)** Build using petalinux-build

```
petalinux-build -p <path to petalinux project directory>/ || true
```
**(5)** Create a .wks file (If using default rootfs.wks file, go to step 6)

.wks file example:

```
part /boot --source bootimg-partition --ondisk mmcblk0 --fstype=vfat --label boot --active --align 4 --fixed-size 128M
part /     --source rootfs            --ondisk mmcblk0 --fstype=ext4 --label root          --align 4 --fixed-size 2G
```
**(6)** Package build artifacts as .wic

The following assumes that you are currently in a directory that is not your petalinux project directory:

```
cd <petalinux project directory> && \
petalinux-package --wic --wks ../rootfs.wks; \
cd <original directory>
```
A default .wks file named rootfs.wks exists in \<petalinux project directory>/build/

Using the default rootfs.wks file looks like this: 

```
cd <petalinux project directory> && \
petalinux-package --wic; \
cd <original directory>
```



### Create eMMC Boot Image

**(1)** Use petalinux-config to designate rootfs type as EXT4

```
petalinux-config -p <path to petalinux project directory>/ --get-hw-description=<path to directory that contains .xsa file>/<.xsa file>
```

![image_pack_config](/docs/image_pack_config.png)

![root_fs_type](/docs/root_fs_type.png)

![ext4](/docs/ext4.png)

**(2)** Use petalinux-config to designate the partition in which the root file system exists

My eMMC device is set to be mmc1 in my device tree. "mmc1" is associated with "mmcblk1"

![edit_dev_node_emmc_device](/docs/edit_dev_node_emmc_device.png)

**(3)** Now, exit the config menu to lock-in changes.

![exit_config](/docs/exit_config.png)

**(4)** Build using petalinux-build

```
petalinux-build -p <path to petalinux project directory>/ || true
```

**(5)** Create a .wks file (If using default rootfs.wks file, go to step 6)

.wks file example:

```
part /boot --source bootimg-partition --ondisk mmcblk1 --fstype=vfat --label boot --active --align 4 --fixed-size 128M
part /     --source rootfs            --ondisk mmcblk1 --fstype=ext4 --label root          --align 4 --fixed-size 2G
```

**(6)** Package build artifacts as .wic

The following assumes that you are currently in a directory that is not your petalinux project directory:

```
cd <petalinux project directory> && \
petalinux-package --wic --wks ../rootfs.wks; \
cd <original directory>
```
A default .wks file named rootfs.wks exists in \<petalinux project directory>/build/

Using the default rootfs.wks file looks like this: 

```
cd <petalinux project directory> && \
petalinux-package --wic; \
cd <original directory>
```
### TFTP Server Setup

**(1)** Install the following packages:

```
sudo apt-get install xinetd tftpd tftp
```

**(2)** in the directory /etc/xinetd.d, create a file named "tftp" with the following contents:

```
service tftp
{
protocol        = udp
port            = 69
socket_type     = dgram
wait            = yes
user            = nobody
server          = /usr/sbin/in.tftpd
server_args     = /tftpboot
disable         = no
}
```

**(3)** Create a directory named "tftpboot". 

This directory's path should match whatever you gave in server_args. 

If server_args = \<parent directory path>/tftpboot, then "tftpboot" should reside inside of \<parent directory path>/tftpboot.
```
 sudo mkdir \<parent directory path>/tftpboot
 sudo chmod -R 777 \<parent directory path>/tftpboot
 sudo chown -R nobody \<parent directory path>/tftpboot
```

**(4)** Restart xinetd service

```
sudo service xinetd restart
```
**or**
```
sudo service xinetd stop
sudo service xinetd start
```

### TFTP Server Test

**(1)** Create a file named "test" with some content and save it in the "tftpboot" directory.

**(2)** Obtain the IP address of the TFTP server by using the "ifconfig" command.

**(3)** Open TFTP session:

```
tftp <ip address>
```

**(4)** Test the tftp server

```
tftp> get test
```

If the test is successful you should see something like this: 

```
Sent 159 bytes in 0.0 seconds

```
### SD-first, eMMC boot:

Two sets of build artifacts will need to be generated for this boot mode.
The first will be flashed to the sd-card to acheive the initial sd boot.
The second will be zipped and copied to the "boot" partition of the sd-card for use after the sd boot has been completed successfully.

**(1)** Flip boot switch to "SD"

![boot_switch](/docs/hardware/BytePipe_x900x/boot_switch.png)

**(2)** Build SD .wic image and flash it to your sd-card

Follow the diections under [Create SD Boot Image](#create-sd-boot-image).

If you have bootargs in your devicetree, make sure your bootargs are pointing at the corret mmc device.

**(3)** Flash sd-card .wic image onto sd card

/dev/sdx represents a general sd-card. Your sd-card will show up in your build machine's /dev directory is sda, sdb, sdc, ...

```
wic_image=<path to SD (.wic) image file>
sudo dd if=$wic_image of=/dev/sdx conv=fsync
sudo sync
sudo umount /dev/sdx*
```

**(4)** Build eMMC .wic image

Follow the diections under [Create eMMC Boot Image](#create-emmc-boot-image).


**(5)** Compress .wic file using gzip

```
gzip -k <address of .wic file>
```

**(6)** Copy compressed .wic file onto boot partition of your sd-card

**(7)** Insert sd-card into HDK sd-card slot and turn HDK power on.

Analyze the boot log to identify any error messages that may appear.

**(8)** When your uart terminal presents you with a login prompt, login.

Our default username/password is analog/analog.


**(9)** Unzip compressed .wic image and flash to eMMC
```
gunzip -c /boot/<name of emmc .wic image>.wic.gz | dd of=/dev/mmcblk1
sync
```

**(10)** Power down and remove sd-card

**(11)** Flip boot switch to "eMMC"

![boot_switch](/docs/hardware/BytePipe_x900x/boot_switch.png)

**(12)** Power HDK on

Analyze the boot log to identify any error messages that may appear.

<br />

### JTAG-first, eMMC boot:
**(1)** Put the Bytepipe SoM into eMMC boot mode.

![boot_switch](/docs/hardware/BytePipe_x900x/boot_switch.png)

**(2)** Make sure the u-boot bsp.cfg files contains the following:

bsp.cfg is located at \<petalinux project directory>/project-spec/meta-user/recipes-bsp/u-boot/files/bsp.cfg

```
CONFIG_ZYNQ_GEM=y
CONFIG_DM_ETH=y
CONFIG_NET=y
CONFIG_NET_RANDOM_ETHADDR=y
```

**(3)** Build eMMC .wic image

Follow the diections under [Create eMMC Boot Image](#create-emmc-boot-image).

**(4)** Compress .wic file using gzip

```
gzip -k <address of .wic file>
```

**(5)** Setup a tftp server

Follow the diections under [TFTP Server Setup](#tftp-server-setup).

**(6)** Run jtag_uboot.tcl and interrupt at u-boot

Click [here](/docs/jtag_uboot.tcl) for jtag_uboot.tcl.

You will start seeing output on your uart terminal. 

Interrupt at u-boot.

**(7)** Once boot has been interrupted at u-boot, run the followng commands (change ipaddr and serverip to match your setup).

```
setenv ipaddr 10.33.33.1
setenv serverip 10.33.33.2
setenv netmask 255.255.255.0
tftpboot petalinux-emmcimage.wic.gz
gzwrite mmc 0 ${fileaddr} ${filesize}
```
Expect tftpboot and gzwrite to take some time to complete.

**(8)** Power Cycle

Do not interrupt the boot at u-boot this time.
