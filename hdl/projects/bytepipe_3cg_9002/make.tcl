# Create Path Variables
set wrkDir [pwd]
set srcDir [file dirname [file normalize [ info script ] ] ]

# Set Vivado Project Name
set project_name bytepipe_3cg_som_sdk

# Create Project
create_project $project_name $wrkDir/vivado -part xczu3cg-sbva484-1-e -force

# Set Board File
set_property board_part nextgenrf.com:bytepipe_3cg_som:part0:1.0 [current_project]

# Add Source Files
add_files -fileset constrs_1 -norecurse $srcDir/constr.xdc
add_files -norecurse $srcDir/system_top.v
add_files -norecurse $srcDir/gpio.v

# Disable Messages
set_msg_config -severity {CRITICAL WARNING} -suppress
set_msg_config -severity INFO -suppress
set_msg_config -severity WARNING -suppress
set_msg_config -severity STATUS -suppress

# Add IP Catalog
set_property ip_repo_paths [list \
  [file normalize [file join $wrkDir/Adi/hdl/library]] \
  [file normalize [file join $srcDir/../../library]] \
] [current_project]
update_ip_catalog

# Generate Block Design
source $srcDir/system.tcl

# Refresh and Save Project
update_compile_order -fileset sources_1
regenerate_bd_layout
save_bd_design

# Make Top Module Wrapper
update_compile_order -fileset sources_1
set_property top system_top [current_fileset]
update_compile_order -fileset sources_1

# Generate Targets
generate_target all [get_files  $wrkDir/vivado/$project_name.srcs/sources_1/bd/system/system.bd]

# Launch Runs
launch_runs impl_1 -to_step write_bitstream -jobs 8
wait_on_run -quiet impl_1

# Export Hardware Defintion Files
update_compile_order -fileset sources_1
file copy -force $wrkDir/vivado/$project_name.runs/impl_1/system_top.sysdef $wrkDir/system_top.hdf

exit
