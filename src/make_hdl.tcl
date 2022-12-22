# ******************************************************************************************
# ******************************************************************************************
#              Copyright 2020 (c) NextGen RF Design. All rights reserved.
#
#        This core is distributed WITHOUT ANY WARRANTY; without even the implied 
#           warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# ******************************************************************************************
# ******************************************************************************************

set proj_name [lindex $argv 0]
set srcDir [lindex $argv 1]
set part [lindex $argv 2]

# Create project
create_project $proj_name $proj_name/vivado -part $part -force

# Add IP Catalog
set_property ip_repo_paths . [current_fileset]
update_ip_catalog

# Create Source List
set src_list [glob $srcDir/*.v $srcDir/*.xdc $srcDir/*.xci $srcDir/*.coe]

# Import Sources
import_files -norecurse -fileset [get_filesets sources_1] $src_list
set_property source_set sources_1 [get_filesets sim_1]

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
generate_target all [get_files  $proj_name/vivado/${proj_name}.srcs/sources_1/bd/system/system.bd]

# Launch Runs
launch_runs impl_1 -to_step write_bitstream -jobs 4
wait_on_run -quiet impl_1

# Export Hardware Defintion Files
update_compile_order -fileset sources_1
write_hw_platform -fixed -include_bit -force -file [file normalize $proj_name/${proj_name}_${part}.xsa]

