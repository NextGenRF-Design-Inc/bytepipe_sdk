#!/bin/bash

# This function builds the rflan programmable logic using Cygwin or Bash
# Inputs: 
#  $1 = device (ie. xczu3cg)
#
# Prerequisites:
#	Cygwin: export PATH=/cygdrive/c/Xilinx/Vitis/2021.1/bin/:$PATH
#	Linux: source ~/home/Xilinx/Vitis/2021.1/settings64.sh
#
build_sw()
{
  # Abstract Project Directory
  projDir="$(dirname "$BASH_SOURCE")"
  if [[ $projDir == *"cygdrive"* ]]; then
    projDir=$(realpath $(cygpath -w $projDir))
  fi  

  # Abstract working directory
  wrkDir=$(pwd)
  
  # Abstract Device Inputs
  device=$1
  
  # Define project
  project="rflan_dpd"
  
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
  
  # Download latest hardware
	#wget -nc "https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/releases/download/v09.16.22/$project_$device.xsa" -O $wrkDir/$project/${project}_${device}.xsa

  # Indicate Build
  echo "$(printf '\033')[0;33mBuilding $project Software $(printf '\033')[0m"

  if [[ $env == *"Linux"* ]]; then
    xsct "$projDir/sw/make_sw.tcl" $device
  else
    xsct.bat "$projDir/sw/make_sw.tcl" $device
  fi
  
  cp -rf $wrkDir/$project/rpu_system/Debug/sd_card $wrkDir/$project
  cp -rf $projDir/resources/* $wrkDir/$project/sd_card    

  zip -r "$wrkDir/$project/${project}_${device}_sdcard.zip" $wrkDir/$project/sd_card/  
}
