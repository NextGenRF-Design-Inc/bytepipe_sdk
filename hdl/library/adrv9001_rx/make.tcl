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
set ip_name adrv9001_rx
set ver 1.0
set vendor NGRF
set url http://www.nextgenrf.com
set description { 

This module instantiates a receive channel for interfacing to the ADRV9001/2.  Only LVDS
is supported.  The strobe can be configured as 8-bits on 8-bits off or 1-bit on 15-bits off.
Each lane supports a fixed configurable delay specified in pico-seconds relative to the 
clock lane.


}
# ******************************************************************************************
# ******************************************************************************************

create_project $ip_name vivado -part xczu2cg-sbva484-1-e -force

# Add source Files
add_files -norecurse src/adrv9001_rx.vp
add_files -norecurse src/adrv9001_rx_clk.vp
add_files -norecurse src/adrv9001_rx_serdes.vp
add_files -norecurse src/adrv9001_rx_serdes_phase.vp
add_files -norecurse src/adrv9001_serdes_aligner.vp
add_files -norecurse src/adrv9001_serdes_pack.vp
add_files -norecurse src/adrv9001_rx_rtl.xml

# Disable Messages
set_msg_config -severity INFO -suppress
set_msg_config -severity WARNING -suppress
set_msg_config -severity STATUS -suppress
set_msg_config -severity {CRITICAL WARNING} -suppress

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

ipx::infer_bus_interface {m_axis_tdata m_axis_tvalid m_axis_tready} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::associate_bus_interfaces -busif m_axis -clock m_axis_aclk [ipx::current_core]
set_property name adrv9001_rx [ipx::get_bus_interfaces adrv9001_adrv9001_rx -of_objects [ipx::current_core]]


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

reset_msg_config -suppress -severity INFO
reset_msg_config -suppress -severity WARNING
reset_msg_config -suppress -severity STATUS
reset_msg_config -suppress -severity {CRITICAL WARNING}

# Close the current project.
close_project
file delete -force vivado
file delete -force .Xil
file delete -force *.log   
