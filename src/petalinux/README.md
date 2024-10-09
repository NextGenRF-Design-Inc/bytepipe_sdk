![logo](../../docs/BytePipe_Logo.png)

# PetaLinux v2021.1 Depedencies 

Install needed dependencies using the following command:

```
sudo apt-get install iproute2 gawk python3 python build-essential gcc git make net-tools libncurses5-dev tftpd zlib1g-dev libssl-dev flex bison libselinux1 gnupg wget git-core diffstat chrpath socat xterm autoconf libtool tar unzip texinfo zlib1g-dev gcc-multilib automake zlib1g:i386 screen pax gzip cpio python3-pip python3-pexpect xz-utils debianutils iputils-ping python3-git python3-jinja2 libegl1-mesa libsdl1.2-dev pylint3 dtc

```

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


