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
