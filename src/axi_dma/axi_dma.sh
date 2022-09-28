#!/bin/bash

# This function builds the axi_dma programmable logic
# Inputs: none
#
# Prerequisites:
#	Cygwin: export PATH=/cygdrive/c/Xilinx/Vivado/2021.1/bin/:$PATH
#	Linux: source /cygdrive/c/Xilinx/Vivado/2021.1/settings64.sh

# Abstract Project Directory
projDir="$(dirname "$BASH_SOURCE")"
if [[ $projDir == *"cygdrive"* ]]; then
  projDir=$(realpath $(cygpath -w $projDir))
fi  

# Abstract working directory
wrkDir=$(pwd)
  
# Define project
project="axi_dma"

# Cleanup
rm -rf $wrkDir/$project

# Make projct directory
mkdir -p $wrkDir/$project

# Indicate Build
echo "$(printf '\033')[0;33mBuilding axi_dma $(printf '\033')[0m"

git clone --branch hdl_2019_r2 https://github.com/analogdevicesinc/hdl.git $wrkDir/$project
sed -i 's/2019.1/2021.1/g' $wrkDir/$project/library/scripts/adi_ip_xilinx.tcl
make -C $wrkDir/$project/library/util_pack/util_upack2 xilinx
make -C $wrkDir/$project/library/util_pack/util_cpack2 xilinx
make -C $wrkDir/$project/library/axi_dmac xilinx

