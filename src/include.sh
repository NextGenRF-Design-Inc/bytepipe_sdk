#!/bin/bash

# This function builds the rflan programmable logic using Cygwin or Bash
# Inputs: 
#  $1 = Source Directory
#  $2 = project (ie. rflan)
#  $3 = device (ie. xczu3cg)
#
# Prerequisites:
#	Cygwin: export PATH=/cygdrive/c/Xilinx/Vitis/2021.1/bin/:$PATH
#	Linux: source ~/home/Xilinx/Vitis/2021.1/settings64.sh
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
  rm -rf $wrkDir/$project/sd_card	
  
  # Indicate Build
  echo "$(printf '\033')[0;33mBuilding $project Software $(printf '\033')[0m"
  
  if [ -z "$device" ] 
  then
    if [[ $env == *"Linux"* ]]; then
      xsct "$srcDir/$project/sw/make_sw.tcl"
    else
      xsct.bat "$srcDir/$project/sw/make_sw.tcl"
    fi
    
    cp -rf $wrkDir/$project/rpu_system/Debug/sd_card $wrkDir/$project
    cp -rf $srcDir/$project/resources/* $wrkDir/$project/sd_card    

    zip -rj "$wrkDir/$project/${project}_sdcard.zip" $project/sd_card/
  else
    if [[ $env == *"Linux"* ]]; then
      xsct "$srcDir/$project/sw/make_sw.tcl" $device
    else
      xsct.bat "$srcDir/$project/sw/make_sw.tcl" $device
    fi
    
    cp -rf $wrkDir/$project/rpu_system/Debug/sd_card $wrkDir/$project
    cp -rf $srcDir/$project/resources/* $wrkDir/$project/sd_card    

    zip -rj "$wrkDir/$project/${project}_${device}_sdcard.zip" $project/sd_card/
  fi
  
}

# This function builds the rflan programmable logic using Cygwin or Bash
# Inputs: 
#  $1 = Source Directory
#  $2 = project (ie. rflan)
#  $3 = device (ie. xczu3cg)
#
# Prerequisites:
#	Cygwin: export PATH=/cygdrive/c/Xilinx/Vivado/2021.1/bin/:$PATH
#	Linux: source ~/home/Xilinx/Vivado/2021.1/settings64.sh
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

# This function builds the rflan programmable logic using Cygwin or Bash
# Inputs: 
#  $1 = project (ie. rflan)
#  $2 = device (ie. xczu3cg)
#
# Prerequisites:
#	Cygwin: export PATH=/cygdrive/c/Xilinx/Vivado/2021.1/bin/:$PATH
#	Linux: source ~/home/Xilinx/Vivado/2021.1/settings64.sh
#
build()
{	
  # Abstract Source Directory
  srcDir="$(dirname -- "$(readlink -f "${BASH_SOURCE}")")"
  if [[ $srcDir == *"cygdrive"* ]]; then
    srcDir=$(realpath $(cygpath -w $srcDir))
  fi 
  
  # Define project
  project=$1
   
  source $srcDir/$project/$project.sh; build $@
}

# This function prompts the user to provide the device
# Inputs: 
#
# Prerequisites:
#
prompt_device()
{
	read -p "Select Device [ xczu3cg, xczu3eg, xczu2cg, xczu2eg ]: " device
  echo $device
}

# This function downloads the latest released hardware definition
# Inputs: 
#  $1 = project (ie. rflan)
#  $2 = device (ie. xczu3cg)
#
# Prerequisites:
#
download_xsa()
{
  project=$1
  
  device=$2
  
  if [ -z "$device" ] 
  then
	  wget -nc "https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/releases/download/v10.25.22/$project.xsa" -O $project/$project.xsa
  else  
	  wget -nc "https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/releases/download/v10.25.22/$project_$device.xsa" -O $project/$project_$device.xsa
  fi
}

test_build()
{
  project=$1
  echo "$project"
  
    device=$2
  
  if [ -z "$device" ] 
  then
	  echo "missing device"
  else
	  echo $device
  fi  
  




  srcDir="$(dirname -- "$(readlink -f "${BASH_SOURCE}")")"
  if [[ $srcDir == *"cygdrive"* ]]; then
    srcDir=$(realpath $(cygpath -w $srcDir))
  fi  
  
  echo "$srcDir"  

}

