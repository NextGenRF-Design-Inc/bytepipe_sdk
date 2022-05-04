# ******************************************************************************************
# ******************************************************************************************
#              Copyright 2022 (c) NextGen RF Design. All rights reserved.
#
#        This core is distributed WITHOUT ANY WARRANTY; without even the implied 
#           warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# ******************************************************************************************
# ******************************************************************************************

# Define IP Name
variable proj_name

set proj_name rflan_iqdma

# Get Source Directory
set srcDir [file dirname [file normalize [ info script ] ] ]

# Create Hardware Platform
platform create -name {hwp} -hw {rflan_iqdma/rflan_iqdma.xsa}\
-proc {psu_cortexr5_0} -os {freertos10_xilinx} -fsbl-target {psu_cortexa53_0} -out {rflan_iqdma/}

platform write
platform generate -domains 
platform active {hwp}
bsp reload
bsp setlib -name libmetal -ver 2.1
bsp setlib -name openamp -ver 1.6
bsp setlib -name xilffs -ver 4.5
bsp config tick_rate "10000"
bsp config total_heap_size "1048576"
bsp config generate_runtime_stats "1"
bsp config use_strfunc "0"
bsp config use_lfn "1"
bsp write
bsp reload
catch {bsp regenerate}
platform generate
