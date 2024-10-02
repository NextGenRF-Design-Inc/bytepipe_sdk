#/***************************************************************************//**
# *  \file       make_ip.tcl
# *
# *  \details
# *
# *  \copyright
# *
# *  Copyright 2022(c) NextGen RF Design, Inc.  
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
 
set proj_name [lindex $argv 0]
set srcDir [lindex $argv 1]
set wrkDir [lindex $argv 2]
set includeDir [file dirname [file normalize [ info script ] ] ]

# Create project
create_project $proj_name $wrkDir/$proj_name/vivado -part xczu2cg-sbva484-1-e -force

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

# Create Package
ipx::package_project -root_dir $wrkDir/$proj_name -vendor nextgenrf.com -library user -taxonomy /NextGenRF
set_property name $proj_name [ipx::current_core]
set_property vendor_display_name {NextGenRF Design} [ipx::current_core]
set_property company_url {http://www.nextgenrf.com} [ipx::current_core]
ipx::save_core

set_property supported_families {zynquplus Beta} [ipx::current_core]
ipx::save_core

# Run optional scripts
package require fileutil
foreach script [fileutil::findByPattern $srcDir *.tcl] {
  source $script
}

ipx::merge_project_changes ports [ipx::current_core]

# Create IP files and save
set_property core_revision 2 [ipx::current_core]
ipx::create_xgui_files [ipx::current_core]
ipx::update_checksums [ipx::current_core]
ipx::check_integrity [ipx::current_core]
ipx::save_core [ipx::current_core]


