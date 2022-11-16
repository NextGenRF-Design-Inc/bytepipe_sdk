#!/bin/bash

# This function abstracts a software build using Vitis.  The project and details 
# of the build are defined in "$srcDir/$project/sw/make_sw.tcl" and executed by
# Vitis.  
#   
# Inputs: 
#  $1 = Source Directory
#  $2 = project (ie. rflan)
#  $3 = device (ie. xczu3cg)
#
# Prerequisites:
#	  Cygwin: export PATH=/cygdrive/c/Xilinx/Vitis/2021.1/bin/:$PATH
#	  Linux: source ~/home/Xilinx/Vitis/2021.1/settings64.sh
#   
build_sw()
{
  # Abstract Project Directory
  srcDir=$1
  if [[ $srcDir == *"cygdrive"* ]]; then
    srcDir=$(realpath $(cygpath -w $srcDir))
  fi  

  # Abstract working directory
  wrkDir=$(pwd)
  
  # Define project
  project=$2
  
  # Abstract Device Inputs
  device=$3
  
  # Determin Host Environment
  env=$(uname)

  # Make projct directory
  mkdir -p $wrkDir/$project

  # Cleanup existing software
  rm -rf $wrkDir/$project/rpu
  rm -rf $wrkDir/$project/rpu_system
  rm -rf $wrkDir/$project/.metadata
  rm -rf $wrkDir/$project/hwp
  rm -rf $wrkDir/$project/RemoteSystemsTempFiles
  rm -rf $wrkDir/$project/.analytics
  rm -rf $wrkDir/$project/*.log
  rm -rf $wrkDir/$project/*.bin
  rm -rf $wrkDir/$project/*.zip
  rm -rf $wrkDir/$project/sd_card	
  rm -rf $wrkDir/$project/output
  
  # Indicate Build
  echo "$(printf '\033')[0;33mBuilding $project Software $(printf '\033')[0m"
  
  if [[ $env == *"Linux"* ]]; then
    xsct "$srcDir/$project/sw/make_sw.tcl" $device
  else
    xsct.bat "$srcDir/$project/sw/make_sw.tcl" $device
  fi
    
  cp -rf $wrkDir/$project/rpu_system/Debug/sd_card $wrkDir/$project
  cp -rf $srcDir/$project/resources/* $wrkDir/$project/sd_card    
  
  mkdir $wrkDir/$project/output
  cp -rf $wrkDir/$project/rpu/Debug/rpu.elf $wrkDir/$project/output
  cp -rf $wrkDir/$project/rpu/Debug/rpu.elf.size $wrkDir/$project/output
  cp -rf $wrkDir/$project/hwp/zynqmp_fsbl/fsbl_a53.elf $wrkDir/$project/output
  cp -rf $wrkDir/$project/hwp/zynqmp_pmufw/pmufw.elf $wrkDir/$project/output
  cp -rf $wrkDir/$project/rpu_system/Debug/sd_card/boot.bin $wrkDir/$project/output  
  cp -rf $wrkDir/$project/${project}_${device}.xsa $wrkDir/$project/output    
  
  zip -rj "$wrkDir/$project/${project}_${device}_sdcard.zip" $project/sd_card/
  zip -rj "$wrkDir/$project/${project}_${device}_artifacts.zip" $project/output/  
  
  rm -rf $wrkDir/$project/sd_card	
  rm -rf $wrkDir/$project/output  
  
}

# This function abstracts a programmable logic build using Vivado.  The project 
# and details of the build are defined in "$srcDir/$project/sw/make_sw.tcl" and 
# executed by Vivado.
#
#  $1 = Source Directory
#  $2 = project (ie. rflan)
#  $3 = device (ie. xczu3cg)
#
# Prerequisites:
#	  Cygwin: export PATH=/cygdrive/c/Xilinx/Vivado/2021.1/bin/:$PATH
#	  Linux: source ~/home/Xilinx/Vivado/2021.1/settings64.sh
#
build_hdl()
{	
  # Abstract Script Directory
  scriptDir="$(dirname -- "$(readlink -f "${BASH_SOURCE}")")"
  if [[ $scriptDir == *"cygdrive"* ]]; then
    scriptDir=$(realpath $(cygpath -w $scriptDir))
  fi 
  
  # Abstract Source Directory
  srcDir=$1
  if [[ $srcDir == *"cygdrive"* ]]; then
    srcDir=$(realpath $(cygpath -w $srcDir))
  fi
  
  # Define project
  project=$2
  
  # Abstract Device Inputs
  device=$3
  
  # Abstract working directory
  wrkDir=$(pwd)  
   
  # Make projct directory
  mkdir -p $wrkDir/$project
  
	# Cleanup
	rm -rf $wrkDir/$project/vivado
	rm -rf $wrkDir/$project/$project.xsa  
  
	# Indicate Build
	echo "$(printf '\033')[0;33mBuilding $project Programmable Logic $(printf '\033')[0m"
	  
  # Build with Vivado    
  vivado -mode batch -nojournal -quiet -source $scriptDir/make_hdl.tcl -notrace -tclargs $project $srcDir/$project/hdl $device
  
}

