#!/bin/bash

wget -nc "https://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/adrv9001-sdk-eval-software.zip" || true
unzip adrv9001-sdk-eval-software.zip  
mkdir rflan_dpd
cp rflan/rflan_xczu3cg.xsa rflan_dpd/
FILE=c99.zip
if test -f "$FILE"; then
  mkdir -p adrv9001/profile
  unzip -j c99.zip -d ./c99
  chmod +x src/adrv9001/sw/profile_parse.sh
  src/adrv9001/sw/profile_parse.sh c99 adrv9001/profile
  rm -rf src/rflan/sw/adrv9001/profile
  cp -r adrv9001/profile src/rflan/sw/adrv9001
fi
xsct src/rflan_dpd/sw/make_sw.tcl $1
cp rflan_dpd/rpu_system/Debug/sd_card/BOOT.BIN ./${BootFilename}.bin
cp -r src/rflan_dpd/resources/ ./sdcard
cp rflan_dpd/rpu_system/Debug/sd_card/BOOT.BIN ./sdcard/
cp ${BootFilename}.bin ./sdcard/
zip -r "rflan_dpd_$1_sdcard.zip" sdcard/

