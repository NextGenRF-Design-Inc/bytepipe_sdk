# RAMPART CONFIDENTIAL
# __________________
#
# (C) 2025 Rampart Communications Inc
# All Rights Reserved.
#
# NOTICE:  All information contained herein is, and remains
# the property of Rampart Communications, Inc and its suppliers, if any.
# The intellectual and technical concepts contained herein
# are proprietary to Rampart Communications, Inc and its suppliers and may be
# covered by U.S. and Foreign Patents, patents in process, and
# are protected by trade secret and/or copyright law. Dissemination
# of this information or reproduction of this material is
# prohibited unless prior written permission is obtained
# from Rampart Communications, Inc.

#!/usr/bin/tclsh

set script_name [file normalize [info script]]

if { $::argc != 1 } {
    puts "Error: Invalid number of arguments"
    puts ""

    puts "Usage: $script_name <artifact_base_dir>"
    puts ""
    puts "  <artifact_base_dir> => Path to the directory that contains the built artifacts"
    puts ""

    exit 1
}

set base_dir [lindex $::argv 0]
set source_dir $base_dir

puts $source_dir

# Connect to the JTAG adapter
connect

# Attempt to select the PSU target and reset the full system
targets -set -nocase -filter {name =~ "*PSU*"} -timeout 20
rst -sys

# Set the JTAG to run as fast as it can. If there are download problems
# try removing this line, closing xsct, running `pkill hw_server` and 
# re-launching xsct
jtag targets 1
jtag frequency [lindex [jtag frequency -list] end]

# Force the device into JTAG boot mode and reset to finish the change
# THIS WILL BREAK THE ABILITY TO BOOT FROM AN SD CARD UNTIL CHANGED BACK
# BY ANOTHER WRITE TO 0xff5e0200 WITH THE CORRECT VALUE OR A FULL POWER CYCLE
mwr 0xffca0010 0x0
mwr 0xff5e0200 0x0100
rst -sys

# Unlock the device 
mask_write 0xFFCA0038 0x1C0 0x1C0

# Load the platform management unit firmware and let it run
# This is running on the MicroBlaze cores, so no reason to stop
# and wait for any period of time for it to run as it will always
# be running in the "background"
targets -set -nocase -filter {name =~ "*MicroBlaze PMU*"}
dow $source_dir/pmufw.elf
con

# Load the ZynqMP first stage bootloader firmware on the first A53 core
# and let it run for 5 seconds. The time is arbitrary, but must be long
# enough for the application to finish setting the device up or there will
# be memory errors later
targets -set -nocase -filter {name =~ "*A53*#0"}
rst -processor
dow $source_dir/zynqmp_fsbl.elf
con
after 5000
stop

# Load the device tree blob (the adress seems arbitrary?)
dow -data $source_dir/system.dtb 0x00100000

# Download the U-Boot and BL31 applications and then start BL31 (ATF) which will kick off U-Boot
dow $source_dir/u-boot.elf
dow $source_dir/bl31.elf

con
#EOF

