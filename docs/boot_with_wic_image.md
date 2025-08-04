![logo](/docs/BytePipe_Logo.png)

---

# Linux OS Boot options using a .wic image
This tutorial assumes that you have already created your petalinux project using petalinux-create
<br />

### SD boot:
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
**(7)** flash .wic image onto sd card

/dev/sdx represents a general sd-card. Your sd-card will show up in your build machine's /dev directory is sda, sdb, sdc, ...

```
wic_image=<path to SD (.wic) image file>
sudo dd if=$wic_image of=/dev/sdx conv=fsync
sudo sync
sudo umount /dev/sdx*
```

**(8)** Insert sd-card into HDK sd-card slot and turn HDK power on.

Analyze the boot log to identify any error messages that may appear.


### SD-first, eMMC boot:

Two sets of build artifacts will need to be generated for this boot mode.
The first will be flashed to the sd-card to acheive the initial sd boot.
The second will be zipped and copied to the "boot" partition of the sd-card for use after the sd boot has been completed successfully.

**(1)** Build SD .wic image and flash it to your sd-card

Follow the diections under [SD boot](#sd-boot).

If you have bootargs in your devicetree, make sure your bootargs are pointing at the corret mmc device.

**(2)** Build eMMC .wic image

**(2.1)** Use petalinux-config to designate rootfs type as EXT4

```
petalinux-config -p <path to petalinux project directory>/ --get-hw-description=<path to directory that contains .xsa file>/<.xsa file>
```

![image_pack_config](/docs/image_pack_config.png)

![root_fs_type](/docs/root_fs_type.png)

![ext4](/docs/ext4.png)

**(2.2)** Use petalinux-config to designate the partition in which the root file system exists

My eMMC device is set to be mmc1 in my device tree. "mmc1" is associated with "mmcblk1"

![edit_dev_node_emmc_device](/docs/edit_dev_node_emmc_device.png)

**(2.3)** Now, exit the config menu to lock-in changes.

![exit_config](/docs/exit_config.png)

**(2.4)** Build using petalinux-build

```
petalinux-build -p <path to petalinux project directory>/ || true
```

**(2.5)** Create a .wks file (If using default rootfs.wks file, go to step 6)

.wks file example:

```
part /boot --source bootimg-partition --ondisk mmcblk1 --fstype=vfat --label boot --active --align 4 --fixed-size 128M
part /     --source rootfs            --ondisk mmcblk1 --fstype=ext4 --label root          --align 4 --fixed-size 2G
```

**(2.6)** Package build artifacts as .wic

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

**(2.7)** compress .wic file

```
gzip -k <address of .wic file>
```

**(2.8)** copy compressed .wic file onto boot partition of your sd-card


**(3)** Insert sd-card into HDK sd-card slot and turn HDK power on.

Analyze the boot log to identify any error messages that may appear.

**(3.1)** When/if your uart terminal presents you with a login prompt, login.

Our default username/password is analog/analog.


**(3.2)** Unzip compressed .wic image and flash to eMMC
```
gunzip -c /boot/<name of emmc .wic image>.wic.gz | dd of=/dev/mmcblk1
sync
```

**(3.3)** Power down and remove sd-card

**(3.4)** Flip boot switch to "eMMC"

**(3.5)** Power HDK on

Analyze the boot log to identify any error messages that may appear.

<br />

### JTAG-first, eMMC boot:
