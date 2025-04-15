#echo $(PWD)
wrkDir=${PWD}
#wrkDir=/home/ngrf/bytepipe_sdk/workspace
cd bpLinux/project-spec/meta-user/

if [ -d recipes-kernel ]; then
  echo "recipes-kernel exists"
else
  mkdir recipes-kernel
  cd recipes-kernel
  mkdir linux 
  cd linux 
  mkdir linux-xlnx
  cd $wrkDir
fi
echo $PWD
cp -f ../src/petalinux/0001-usb-dwc3-xilinx-Deselect-the-PIPE-clock-for-USB2.0-o.patch bpLinux/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/
cp -f ../src/petalinux/linux-xlnx_%.bbappend bpLinux/project-spec/meta-user/recipes-kernel/linux/