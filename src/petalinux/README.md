![logo](../../docs/BytePipe_Logo.png)

# Building Source

Building the source files is straightforward using the supplied build scripts.  The first step in building the Petalinux image is to build the HDL.  Once the HDL is built the Petalinux Makefile can be ran to build the Linux image.  The example outlines this process using a Linux machine running Unbuntu 18.04.

```
source ~/tools/Vivado/2021.1/settings64.sh
source ~/tools/Vitis/2021.1/settings64.sh
source ~/tools/PetaLinux/2021.1/settings.sh
git clone https://github.com/NextGenRF-Design-Inc/bytepipe_sdk.git
cd bytepipe_sdk/
mkdir workspace
cd workspace/
make -f ../src/rflan/Makefile hdl

make -f ../src/petalinux/Makefile

```
# Frequently Asked Questions / Common Problems
<br>

<!---
|Question/Problem|Answer/Solution|
|-|-|
|lsusb isn't returning any USB busses/devices|add the following to the bottom of the system-user.dtsi source file: \
```
&usb0 {
    status = "okay";
};
  
&dwc3_0 {
    status = "okay";
    dr_mode = "host";
};

&sdhci1{
	status = "okay";
	disable-wp;
	max-frequency = <50000000>;
	no-1-8-v;
};

```|
--->
### (1) Question/Problem: lsusb isn't returning any USB busses/devices
[ Applies to release v10.04.24 and all older releases ]
<br>
<br>
<br>
**Answer/Solution:** Add information to system-user.dtsi

Add the following
```
&usb0 {
    status = "okay";
};
  
&dwc3_0 {
    status = "okay";
    dr_mode = "host";
};
```
directly above 

```
&sdhci1{
	status = "okay";
	disable-wp;
	max-frequency = <50000000>;
	no-1-8-v;
};
```
in the system-user.dtsi source file

### (2) Question/Problem: OS cannot connect to USB device and throws this error message: "usb usb1-port1: connect-debounce failed"
[ Applies to release v10.04.24 and all older releases ]
<br>
<br>
<br>
**Answer/Solution:** Patch the petalinux kernel

0. **The following steps must be completed after the "project" step and before the "build" step in the petalinux Makefile.**
1. Obtain 0001-usb-dwc3-xilinx-Deselect-the-PIPE-clock-for-USB2.0-o.patch from...
 - bytepipe_sdk/src/petalinux/
 - (or) https://adaptivesupport.amd.com/s/article/76694?language=en_US
2. Copy the patch file to bytepipe_sdk/workspace/bpLinux/project-spec/meta-user/recipes-kernel/linux/linux-xlnx
 - Petalinux v2021.1 does not create the folders "recipes-kernel", "linux", and "linux-xlnx", so they will need to be created.
3. Obtain linux-xlnx_%.bbappend from...
 - bytepipe_sdk/src/petalinux/
 - (or) create it yourself with the following contents:
   ```
   SRC_URI_append = " file://0001-usb-dwc3-xilinx-Deselect-the-PIPE-clock-for-USB2.0-o.patch"
  
   FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
   ```
4. Copy linux-xlnx_%.bbappend to bytepipe_sdk/workspace/bpLinux/project-spec/meta-user/recipes-kernel/linux 



