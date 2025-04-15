#!/bin/bash

#/***************************************************************************//**
# *  \file       adrv9001.sh
# *
# *  \details
# *
# *  \copyright
# *
# *  Copyright 2021(c) NextGen RF Design, Inc.  
# *
# *  All rights reserved.
# *
# *  Redistribution and use in source and binary forms, with or without
# *  modification, are permitted provided that the following conditions are met:
# *   - Redistributions of source code must retain the above copyright
# *     notice, this list of conditions and the following disclaimer.
# *   - Redistributions in binary form must reproduce the above copyright notice,
# *     this list of conditions and the following disclaimer in the documentation
# *     and/or other materials provided with the distribution.
# *   - The use of this software may or may not infringe the patent rights of one
# *     or more patent holders.  This license does not release you from the
# *     requirement that you obtain separate licenses from these patent holders
# *     to use this software.
# *   - Use of the software either in source or binary form, must be run on or
# *     directly connected to a NextGen RF Design, Inc. product.
# *
# *  THIS SOFTWARE IS PROVIDED BY NEXTGEN RF DESIGN "AS IS" AND ANY EXPRESS OR
# *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
# *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# *  EVENT SHALL NEXTGEN RF DESIGN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# *  INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# *
# *******************************************************************************/
 
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
# $3 = Name of profile
# Prerequisites: none
profile_parse()
{
	if [ -z "$1" ]
    then
      echo "Enter path of directory containing exported C99 code from TES: "  
      read FILENAME
      
      echo "Enter path of directory to ouput parsed C99 code: "  
      read outDir
      
      echo "Enter name of profile: "  
      read name      
      
  else
    FILENAME=$1
	  outDir=$2    
    name=$3
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
  
  echo "Profile Name: $3"

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
  
	sed -i '/fpga9001Device_0/d' $outDir/*
  
  for f in $outDir/* ; do mv -- "$outDir/$(basename ${f})" "$outDir/${name}_$(basename ${f})" ; done  
  
  sed -i "s/initialize/${name}_initialize/g" $outDir/*
  sed -i "s/calibrate/${name}_calibrate/g" $outDir/*
  sed -i "s/configure/${name}_configure/g" $outDir/*  
  
  sed -i "s/_INITIALIZE_H_/_${name^^}_INITIALIZE_H_/g" $outDir/*    
  sed -i "s/_CONFIGURE_H_/_${name^^}_CONFIGURE_H_/g" $outDir/*   
  sed -i "s/_CALIBRATE_H_/_${name^^}_CALIBRATE_H_/g" $outDir/*     


  
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
