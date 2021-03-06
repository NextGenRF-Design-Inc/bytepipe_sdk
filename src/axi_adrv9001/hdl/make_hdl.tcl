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
set proj_name axi_adrv9001

# Get Source Directory
set srcDir [file dirname [file normalize [ info script ] ] ]

# Create project
create_project $proj_name $proj_name/vivado -part xczu2cg-sbva484-1-e -force

# Add include tcl processes
source $srcDir/../../hdl_include.tcl

# Add IP Repo
set_property ip_repo_paths $proj_name [current_fileset]
update_ip_catalog

# Create Source List
set src_list [glob $srcDir/*.v $srcDir/*.xdc $srcDir/*.xci]

# Import Sources
import_files -norecurse -fileset [get_filesets sources_1] $src_list
set_property source_set sources_1 [get_filesets sim_1]

# Create adrv9001 Interface
create_bus_interface adrv9001 $proj_name
add_interface_ports rx1_en out 1
add_interface_ports rx2_en out 1
add_interface_ports tx1_en out 1
add_interface_ports tx2_en out 1
add_interface_ports rstn out 1
add_interface_ports irq in 1
add_interface_ports dgpio inout 12
add_interface_ports rx1_dclk_p in 1
add_interface_ports rx1_dclk_n in 1
add_interface_ports rx1_strobe_p in 1
add_interface_ports rx1_strobe_n in 1
add_interface_ports rx1_idata_p in 1
add_interface_ports rx1_idata_n in 1
add_interface_ports rx1_qdata_p in 1
add_interface_ports rx1_qdata_n in 1
add_interface_ports rx2_dclk_p in 1
add_interface_ports rx2_dclk_n in 1
add_interface_ports rx2_strobe_p in 1
add_interface_ports rx2_strobe_n in 1
add_interface_ports rx2_idata_p in 1
add_interface_ports rx2_idata_n in 1
add_interface_ports rx2_qdata_p in 1
add_interface_ports rx2_qdata_n in 1
add_interface_ports tx1_ref_clk_p in 1
add_interface_ports rx1_ref_clk_n in 1
add_interface_ports tx1_dclk_p out 1
add_interface_ports tx1_dclk_n out 1
add_interface_ports tx1_strobe_p out 1
add_interface_ports tx1_strobe_n out 1
add_interface_ports tx1_idata_p out 1
add_interface_ports tx1_idata_n out 1
add_interface_ports tx1_qdata_p out 1
add_interface_ports tx1_qdata_n out 1
add_interface_ports tx2_ref_clk_p in 1
add_interface_ports rx2_ref_clk_n in 1
add_interface_ports tx2_dclk_p out 1
add_interface_ports tx2_dclk_n out 1
add_interface_ports tx2_strobe_p out 1
add_interface_ports tx2_strobe_n out 1
add_interface_ports tx2_idata_p out 1
add_interface_ports tx2_idata_n out 1
add_interface_ports tx2_qdata_p out 1
add_interface_ports tx2_qdata_n out 1
save_bus_interface adrv9001

# Package IP Core
pkg_ip_core $proj_name 

# Remove Incorrect Interfaces
ipx::remove_bus_interface ext [ipx::current_core]
ipx::remove_bus_interface rx1_axis [ipx::current_core]
ipx::remove_bus_interface rx2_axis [ipx::current_core]
ipx::remove_bus_interface tx1_axis [ipx::current_core]
ipx::remove_bus_interface tx2_axis [ipx::current_core]

# Map adrv9001 Interfaces 
ipx::add_bus_interface adrv9001 [ipx::current_core]
set_property abstraction_type_vlnv nextgenrf.com:user:adrv9001_rtl:1.0 [ipx::get_bus_interfaces adrv9001 -of_objects [ipx::current_core]]
set_property bus_type_vlnv nextgenrf.com:user:adrv9001:1.0 [ipx::get_bus_interfaces adrv9001 -of_objects [ipx::current_core]]
set_property interface_mode master [ipx::get_bus_interfaces adrv9001 -of_objects [ipx::current_core]]
set_property display_name adrv9001 [ipx::get_bus_interfaces adrv9001 -of_objects [ipx::current_core]]
map_port adrv9001 rx1_en
map_port adrv9001 rx2_en
map_port adrv9001 tx1_en
map_port adrv9001 tx2_en
map_port adrv9001 rstn
map_port adrv9001 irq
map_port adrv9001 dgpio
map_port adrv9001 rx1_dclk_p
map_port adrv9001 rx1_dclk_n
map_port adrv9001 rx1_strobe_p
map_port adrv9001 rx1_strobe_n
map_port adrv9001 rx1_idata_p
map_port adrv9001 rx1_idata_n
map_port adrv9001 rx1_qdata_p
map_port adrv9001 rx1_qdata_n
map_port adrv9001 rx2_dclk_p
map_port adrv9001 rx2_dclk_n
map_port adrv9001 rx2_strobe_p
map_port adrv9001 rx2_strobe_n
map_port adrv9001 rx2_idata_p
map_port adrv9001 rx2_idata_n
map_port adrv9001 rx2_qdata_p
map_port adrv9001 rx2_qdata_n
map_port adrv9001 tx1_ref_clk_p
map_port adrv9001 tx1_ref_clk_n
map_port adrv9001 tx1_dclk_p
map_port adrv9001 tx1_dclk_n
map_port adrv9001 tx1_strobe_p
map_port adrv9001 tx1_strobe_n
map_port adrv9001 tx1_idata_p
map_port adrv9001 tx1_idata_n
map_port adrv9001 tx1_qdata_p
map_port adrv9001 tx1_qdata_n
map_port adrv9001 tx2_ref_clk_p
map_port adrv9001 tx2_ref_clk_n
map_port adrv9001 tx2_dclk_p
map_port adrv9001 tx2_dclk_n
map_port adrv9001 tx2_strobe_p
map_port adrv9001 tx2_strobe_n
map_port adrv9001 tx2_idata_p
map_port adrv9001 tx2_idata_n
map_port adrv9001 tx2_qdata_p
map_port adrv9001 tx2_qdata_n

# Infer Axis bus interface
ipx::infer_bus_interface {rx1_axis_tdata rx1_axis_tvalid} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface {rx2_axis_tdata rx2_axis_tvalid} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface {tx1_axis_tdata tx1_axis_tvalid tx1_axis_tready} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface {tx2_axis_tdata tx2_axis_tvalid tx2_axis_tready} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::associate_bus_interfaces -busif rx1_axis -clock rx1_axis_aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif rx2_axis -clock rx2_axis_aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif tx1_axis -clock tx1_axis_aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif tx2_axis -clock tx2_axis_aclk [ipx::current_core]

# Create IP files and save
set_property core_revision 2 [ipx::current_core]
ipx::create_xgui_files [ipx::current_core]
ipx::update_checksums [ipx::current_core]
ipx::check_integrity [ipx::current_core]
ipx::save_core [ipx::current_core]
update_ip_catalog -rebuild -repo_path $proj_name




