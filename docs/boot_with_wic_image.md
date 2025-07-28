![logo](/docs/BytePipe_Logo.png)

---

# Linux OS Boot options using a .wic image
This section assumes that your project has already been built and all necessary artifacts are available for packaging into a .wic file.

<br />

### SD boot:
**(1)** Create a .wks file

.wks file example:

```
part /boot --source bootimg-partition --ondisk mmcblk1 --fstype=vfat --label boot --active --align 4 --fixed-size 128M
part /     --source rootfs            --ondisk mmcblk1 --fstype=ext4 --label root          --align 4 --fixed-size 2G
```
**(2)** Package build artifacts as .wic

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

### SD-first, eMMC boot:
<br />

### JTAG-first, eMMC boot:
