![logo](/docs/BytePipe_Logo.png)

---

# Linux OS Boot options using a .wic image
This section assumes that your project has already been built and all necessary artifacts are available for packaging into a .wic file.

<br />

### SD boot:
**(1)** Use petalinux-config to designate rootfs type as EXT4

![image_pack_config](/docs/image_pack_config.png)

![root_fs_type](/docs/root_fs_type.png)

![ext4](/docs/ext4.png)

**(2)** Use petalinux-config to designate the partition in which the root file system exists

![dev_node_sd_device](/docs/dev_node_sd_device.png)

![edit_dev_node_sd_device](/docs/edit_dev_node_sd_device.png)

**(3)** Now, exit the config menu to lock-in changes.

**(4)** Create a .wks file

.wks file example:

```
part /boot --source bootimg-partition --ondisk mmcblk1 --fstype=vfat --label boot --active --align 4 --fixed-size 128M
part /     --source rootfs            --ondisk mmcblk1 --fstype=ext4 --label root          --align 4 --fixed-size 2G
```
**(5)** Package build artifacts as .wic

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
**(6)** flash .wic image onto sd card

/dev/sdx represents a general sd-card. Your sd-card will show up in your build machine's /dev directory is sda, sdb, sdc, ...

```
wic_image=<path to SD (.wic) image file>
sudo dd if=$wic_image of=/dev/sdx conv=fsync
sudo sync
sudo umount /dev/sdx*
```

**(7)** Insert sd-card into HDK sd-card slot and turn HDK power on.

Analyze the boot log to identify any error messages that may appear.


### SD-first, eMMC boot:

Two sets of build artifacts will need to be generated for this boot mode.
The first will be flashed to the sd-card to acheive the initial sd boot.
The second will be zipped and copied to the "boot" partition of the sd-card for use after the sd boot has been completed successfully.

**(1)** Build SD .wic image

**(2)** Build eMMC .wic image
<br />

### JTAG-first, eMMC boot:
