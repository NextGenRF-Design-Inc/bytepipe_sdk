# ******************************************************************************************
# ******************************************************************************************
#              Copyright 2022 (c) NextGen RF Design. All rights reserved.
#
#        This core is distributed WITHOUT ANY WARRANTY; without even the implied 
#           warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# ******************************************************************************************
# ******************************************************************************************

set proj_name [lindex $argv 0]
set srcDir [lindex $argv 1]
set includeDir [file dirname [file normalize [ info script ] ] ]

# Create project
create_project $proj_name $proj_name/vivado -part xczu2cg-sbva484-1-e -force

# Add include tcl processes
source $includeDir/include.tcl

# Add IP Repo
set_property ip_repo_paths . [current_fileset]
update_ip_catalog

# Create Source List
set src_list [glob $srcDir/*.v $srcDir/*.xdc $srcDir/*.xci $srcDir/*.xcix]

# Import Sources
import_files -norecurse -fileset [get_filesets sources_1] $src_list
set_property source_set sources_1 [get_filesets sim_1]

# Package IP Core
pkg_ip_core $proj_name 

# Run optional scripts
package require fileutil
foreach script [fileutil::findByPattern $srcDir *.tcl] {
    source $script
}

# Create IP files and save
set_property core_revision 2 [ipx::current_core]
ipx::create_xgui_files [ipx::current_core]
ipx::update_checksums [ipx::current_core]
ipx::check_integrity [ipx::current_core]
ipx::save_core [ipx::current_core]
#update_ip_catalog -rebuild -repo_path $proj_name
