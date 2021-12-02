# ******************************************************************************************
# ******************************************************************************************
#              Copyright 2020 (c) NextGen RF Design. All rights reserved.
#
#        This core is distributed WITHOUT ANY WARRANTY; without even the implied 
#           warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# ******************************************************************************************
#
# Parameters:
#
set ip_name axi_adrv9001
set ver 1.0
set vendor NGRF
set url http://www.nextgenrf.com
set description { 

This module instantiates an axi module for controlling the adrv9001.


}
# ******************************************************************************************
# ******************************************************************************************

create_project $ip_name vivado -part xczu2cg-sbva484-1-e -force

# Add source Files
add_files -norecurse src/axi_adrv9001.vp
add_files -norecurse src/axi_adrv9001_regs.vp

# Add IP catalog
set_property  ip_repo_paths  src [current_project]
update_ip_catalog

ipx::package_project -root_dir .
   
# Set the IP core information properties.
set_property vendor $vendor [ipx::current_core]
set_property library {ip} [ipx::current_core]
set_property name $ip_name [ipx::current_core]
set_property version $ver [ipx::current_core]
set_property display_name $ip_name [ipx::current_core]
set_property description $description [ipx::current_core]
set_property vendor_display_name $vendor [ipx::current_core]
set_property company_url $url [ipx::current_core]
set_property supported_families {zynquplus Beta zynq Beta artix7 Beta kintex7 Beta} [ipx::current_core]

set disclaimer {


      Copyright 2020 (c) NextGen RF Design. All rights reserved.

 This core is distributed WITHOUT ANY WARRANTY; without even the implied 
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
}

ipgui::add_static_text -name {text1} -component [ipx::current_core] -parent [ipgui::get_pagespec -name "Page 0" -component [ipx::current_core] ] -text $description
ipgui::add_static_text -name {text2} -component [ipx::current_core] -parent [ipgui::get_pagespec -name "Page 0" -component [ipx::current_core] ] -text $disclaimer
    
# Set the File Groups for this IP core.
ipx::add_file_group -type utility xilinx_utilityxitfiles [ipx::current_core]
   
update_compile_order -fileset sources_1
launch_runs synth_1 -jobs 10
   
# Generate the XGUI files to accompany this IP core.
ipx::create_xgui_files [ipx::current_core]
  
# Save the IP core.
ipx::save_core [ipx::current_core]

# Close project and cleanup.
close_project
file delete -force vivado
file delete -force .Xil
file delete -force *.log      
