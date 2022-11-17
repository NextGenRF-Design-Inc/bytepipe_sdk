# ******************************************************************************************
# ******************************************************************************************
#              Copyright 2022 (c) NextGen RF Design. All rights reserved.
#
#        This core is distributed WITHOUT ANY WARRANTY; without even the implied 
#           warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# ******************************************************************************************
# ******************************************************************************************

set proj_name rflan

# Get Source Directory
set srcDir [ file dirname [ file dirname [file dirname [file normalize [ info script ] ] ]]]
set wrkDir [pwd]
set projDir [file join $wrkDir $proj_name ]

file delete -force -- $projDir/hwp
file delete -force -- $projDir/rpu
file delete -force -- $projDir/rpu_system
file delete -force -- $projDir/RemoteSystemsTempFiles
file delete -force -- $projDir/.metadata
file delete -force -- $projDir/.analytics
file delete -force -- $projDir/IDE.log

setws $projDir

# Create Hardware Platform
platform create -name {hwp} -hw $proj_name/${proj_name}_${argv}.xsa\
-proc {psu_cortexr5_0} -os {freertos10_xilinx} -fsbl-target {psu_cortexa53_0} -out $proj_name/

platform write
platform generate -domains 
platform active {hwp}
bsp reload
#bsp setlib -name libmetal -ver 2.1
#bsp setlib -name openamp -ver 1.6
bsp setlib -name lwip211 -ver 1.5
bsp setlib -name xilffs -ver 4.5
bsp config api_mode "SOCKET_API"
bsp config tick_rate "1000"
bsp config total_heap_size "16777216"
bsp config generate_runtime_stats "1"
bsp config use_strfunc "0"
bsp config use_lfn "1"
#bsp config tick_setup "true"
#bsp config enable_stm_event_trace "true"
#bsp config enable_timer_tick_trace "true"
#bsp config stm_channel "0"
bsp write
bsp reload
catch {bsp regenerate}
platform generate

# Create RPU Application
app create -name rpu -platform hwp -os {freertos10_xilinx} -proc {psu_cortexr5_0} -template {Empty Application(C)}

# Import RPU Sources
importsources -name rpu -path $srcDir/rflan/sw -soft-link -target-path src
importsources -name rpu -path $wrkDir/adrv9001-sdk/pkg/production/c_src/devices/adrv9001 -soft-link -target-path src/adrv9001/adi_adrv9001
importsources -name rpu -path $wrkDir/adrv9001-sdk/pkg/production/c_src/common -soft-link -target-path src/adrv9001/common
importsources -name rpu -path $wrkDir/adrv9001-sdk/pkg/production/c_src/third_party/jsmn/jsmn.c -soft-link -target-path src/jsmn
importsources -name rpu -path $wrkDir/adrv9001-sdk/pkg/production/c_src/third_party/jsmn/jsmn.h -soft-link -target-path src/jsmn
importsources -name rpu -path $wrkDir/adrv9001-sdk/pkg/production/c_src/third_party/adi_pmag_macros/adi_pmag_macros.h -soft-link -target-path /src/adrv9001/
importsources -name rpu -path $srcDir/adrv9001/sw -soft-link -target-path src/adrv9001
importsources -name rpu -path $srcDir/axi_dma/sw -soft-link -target-path src/axi_dma
importsources -name rpu -path $srcDir/rflan/sw/lscript.ld -target-path src -linker-script

# Configure RPU Project
app config -name rpu define-compiler-symbols ADI_COMMON_VERBOSE
app config -name rpu define-compiler-symbols CUSTOMER_PLATFORM
app config -name rpu define-compiler-symbols ADI_ADRV9001_ARM_VERBOSE
app config -name rpu define-compiler-symbols XPS_BOARD_ZCU102
app config -name rpu define-compiler-symbols RFLAN_STREAM_ENABLE
app config -name rpu define-compiler-symbols RFLAN_LWIP_ENABLE
app config -name rpu define-compiler-symbols VERSA_CLOCK5_ENABLE
app config -name rpu define-compiler-symbols FILE_SYSTEM_LOGICAL_DRIVE=1

app config -name rpu undef-compiler-symbols CLIENT_IGNORE
app config -name rpu undef-compiler-symbols ADI_DYNAMIC_PROFILE_LOAD
app config -name rpu undef-compiler-symbols ADRV9001_BITFIELD_NULL_CHECK

# Configure Project Paths
app config -name rpu include-path $srcDir/rflan/sw
app config -name rpu include-path $srcDir/rflan/sw/csl
app config -name rpu include-path $srcDir/rflan/sw/lib
app config -name rpu include-path $srcDir/rflan/sw/adrv9001/profile
app config -name rpu include-path $srcDir/adrv9001/sw
app config -name rpu include-path $wrkDir/adrv9001-sdk/pkg/production/c_src/third_party/jsmn/
app config -name rpu include-path $wrkDir/adrv9001-sdk/pkg/production/c_src/devices/adrv9001
app config -name rpu include-path $wrkDir/adrv9001-sdk/pkg/production/c_src/devices/adrv9001/private/include
app config -name rpu include-path $wrkDir/adrv9001-sdk/pkg/production/c_src/devices/adrv9001/private/include/bitfields/c0
app config -name rpu include-path $wrkDir/adrv9001-sdk/pkg/production/c_src/devices/adrv9001/public/include
app config -name rpu include-path $wrkDir/adrv9001-sdk/pkg/production/c_src/common
app config -name rpu include-path $wrkDir/adrv9001-sdk/pkg/production/c_src/third_party/adi_pmag_macros
app config -name rpu include-path $srcDir/axi_dma/sw

app config -name rpu libraries m
app build all
