#!/bin/bash


# This function downloads the ADRV9001 SDK from ADI
# Inputs: none
# Prerequisites: none
download_sdk()
{	
	wget -nc "https://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/adrv9001-sdk-eval-software.zip"
	unzip adrv9001-sdk-eval-software.zip  
	chmod -R 777 adrv9001-sdk
}
    
# This function parses the ADRV9001 TES generated C99 code into the adrv9001 driver API
# Inputs:	
#	$1 = Directory or zip file containing TES generated c99 code
#	$2 = Directory to be updated with parsed c99 code
# Prerequisites: none
profile_parse()
{
	if [ -z "$1" ]
    then
      echo "Enter path of directory containing exported C99 code from TES: "  
      read FILENAME
      
      echo "Enter path of directory to ouput parsed C99 code: "  
      read outDir
      
  else
    FILENAME=$1
	  outDir=$2    
  fi

  # Indicate Build
	echo "$(printf '\033')[0;33mBuilding adrv9001 profile $(printf '\033')[0m"

	# Check for zip file
	if file $FILENAME | grep "Zip archive"; then
		echo "Input Zip File: $1"
    rm -rf c99
		unzip -j $FILENAME -d ./c99
		inDir="c99"
	else
		echo "Input Directory: $1"		
		inDir=$FILENAME
	fi
		
	echo "output Directory: $2"

	# Clean destination directory
	rm -rf $outDir
	mkdir -p $outDir

	cp -r $inDir/initialize*.c $outDir
	cp -r $inDir/initialize*.h $outDir
	cp -r $inDir/calibrate*.c $outDir
	cp -r $inDir/calibrate*.h $outDir
	cp -r $inDir/configure*.c $outDir
	cp -r $inDir/configure*.h $outDir

	sed -i '/printf/d' $outDir/*.c
	sed -i '/getchar/d' $outDir/*.c
	sed -i 's/int initialize(adi_adrv9001_Device_t * adrv9001Device_0.*/int initialize(adi_adrv9001_Device_t * adrv9001Device_0)/g' $outDir/*
	sed -i '/#include "adi_fpga9001/d' $outDir/*
	sed -i '/error_code = adi_fpga9001/d' $outDir/*
	sed -i '/linux_uio_init/d' $outDir/*

	sed -i '1i #include "adrv9001.h"' $outDir/initialize*
	sed -i '1i #include "adrv9001.h"' $outDir/configure*
	sed -i 's/, adi_fpga9001_Device_t \* fpga9001Device_0//g' $outDir/*

	sed -i -e '/adi_fpga9001_SsiCalibrationCfg_t/,+8d' $outDir/initialize.c
	sed -i -e '/adi_fpga9001_ClockStatus_t/,+5d' $outDir/initialize.c
	sed -i -e '/adi_fpga9001_Version_t/,+3d' $outDir/initialize.c

}


package_hdl()
{
  srcDir="$(dirname -- "$(readlink -f "${BASH_SOURCE}")")"
  if [[ $srcDir == *"cygdrive"* ]]; then
    srcDir=$(realpath $(cygpath -w $srcDir))
  fi 
  
  # Define project
  project="$(basename $srcDir)"
    
  # Abstract working directory
  wrkDir=$1 
   
  # Make projct directory
  mkdir -p $wrkDir/$project

  # Cleanup
  rm -rf $wrkDir/$project/vivado
  rm -rf $wrkDir/$project/*.xml
  rm -rf $wrkDir/$project/xgui    

  # Indicate Build
  echo "$(printf '\033')[0;33mPackaging $project Programmable Logic $(printf '\033')[0m"
  
  echo "Working Dir = $wrkDir"
  echo "Src Director = $srcDir"
      
  # Package with Vivado    
  vivado -mode batch -nojournal -quiet -source $srcDir/../make_ip.tcl -notrace -tclargs $project $srcDir/hdl $wrkDir
}
