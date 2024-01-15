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
  
  # Indicate Build
  echo "$(printf '\033')[0;33mBuilding $project Software $(printf '\033')[0m"
  
  if [[ $env == *"Linux"* ]]; then
    xsct "$srcDir/$project/sw/make_sw.tcl" $device
  else
    xsct.bat "$srcDir/$project/sw/make_sw.tcl" $device
  fi
    
  
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

# This function abstracts a programmable logic build using Vivado.  The project 
# and details of the build are defined in "$srcDir/$project/sw/make_sw.tcl" and 
# executed by Vivado.
#
#  $1 = Source Directory
#  $2 = project (ie. rflan)
#
# Prerequisites:
#	  Cygwin: export PATH=/cygdrive/c/Xilinx/Vivado/2021.1/bin/:$PATH
#	  Linux: source ~/home/Xilinx/Vivado/2021.1/settings64.sh
#
build_ip()
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
    
  # Abstract working directory
  wrkDir=$(pwd)  
   
  # Make projct directory
  mkdir -p $wrkDir/$project
  
	# Cleanup
	rm -rf $wrkDir/$project/vivado
  rm -rf $wrkDir/$project/*.xml
  rm -rf $wrkDir/$project/xgui    
  
	# Indicate Build
	echo "$(printf '\033')[0;33mBuilding $project Programmable Logic $(printf '\033')[0m"
	    
  # Build with Vivado    
  vivado -mode batch -nojournal -quiet -source $scriptDir/make_ip.tcl -notrace -tclargs $project $srcDir/$project/hdl
  
}

# This function programs flash using Vitis tools. 
#
#  $1 = project (ie. rflan)
#  $2 = flash_type (ie. qspi-x4-single, emmc)
#
# Prerequisites:
#	  Cygwin: export PATH=/cygdrive/c/Xilinx/Vitis/2021.1/bin/:$PATH
#	  Linux: source ~/home/Xilinx/Vitis/2021.1/settings64.sh
#
program_flash()
{  
  # Define project
  project=$1

  # Define flash type
  flash_type=$2 
  
  # Determin Host Environment
  env=$(uname)
    
  # Indicate Build
  echo "$(printf '\033')[0;33mProgramming $project $flash_type $(printf '\033')[0m"
  
  # Abstract working directory
  wrkDir=$(pwd)
  if [[ $wrkDir == *"cygdrive"* ]]; then
    wrkDir=$(realpath $(cygpath -w $wrkDir))
  fi
  
  if [[ $env == *"Linux"* ]]; then
    hw_server & program_flash -f $wrkDir/$project/rpu_system/Debug/sd_card/BOOT.BIN -offset 0 -flash_type $flash_type -fsbl $wrkDir/$project/hwp/export/hwp/sw/hwp/boot/fsbl.elf -blank_check -verify -cable type xilinx_tcf url TCP:127.0.0.1:3121
  else
    hw_server.bat & program_flash.bat -f $wrkDir/$project/rpu_system/Debug/sd_card/BOOT.BIN -offset 0 -flash_type $flash_type -fsbl $wrkDir/$project/hwp/export/hwp/sw/hwp/boot/fsbl.elf -blank_check -verify -cable type xilinx_tcf url TCP:127.0.0.1:3121
  fi
 
}
