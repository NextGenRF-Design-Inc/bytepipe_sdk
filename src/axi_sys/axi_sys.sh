#!/bin/bash

srcDir="$(dirname -- "$(readlink -f "${BASH_SOURCE}")")"
if [[ $srcDir == *"cygdrive"* ]]; then
  srcDir=$(realpath $(cygpath -w $srcDir))
fi 

# Define project
project="$(basename $srcDir)"
  
# Abstract working directory
wrkDir=$(pwd)
 
if [[ $wrkDir == *"cygdrive"* ]]; then
  wrkDir=$(realpath $(cygpath -w $wrkDir))
fi  
 
# Make projct directory
mkdir -p $wrkDir/$projec
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


