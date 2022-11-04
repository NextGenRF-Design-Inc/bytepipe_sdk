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
set proj_name adrv9001

# Get Source Directory
set srcDir [file dirname [file normalize [ info script ] ] ]

# Create project
create_project $proj_name $proj_name/vivado -part xczu2cg-sbva484-1-e -force

# Add include tcl processes
source $srcDir/../../include.tcl

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
add_interface_ports dgpio_o out 12
add_interface_ports dgpio_i in 12
add_interface_ports dgpio_t out 12
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
add_interface_ports spi_mosi out 1
add_interface_ports spi_miso in 1
add_interface_ports spi_clk out 1
add_interface_ports spi_csn out 1
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
map_port adrv9001 dgpio_i
map_port adrv9001 dgpio_o
map_port adrv9001 dgpio_t
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
map_port adrv9001 spi_mosi
map_port adrv9001 spi_miso
map_port adrv9001 spi_clk
map_port adrv9001 spi_csn

# Map axis data interface
ipx::infer_bus_interface {rx1_axis_tdata rx1_axis_tvalid rx1_axis_tready} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface {rx2_axis_tdata rx2_axis_tvalid rx2_axis_tready} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface {tx1_axis_tdata tx1_axis_tvalid tx1_axis_tready} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface {tx2_axis_tdata tx2_axis_tvalid tx2_axis_tready} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::associate_bus_interfaces -busif rx1_axis -clock rx1_axis_aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif rx2_axis -clock rx2_axis_aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif tx1_axis -clock tx1_axis_aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif tx2_axis -clock tx2_axis_aclk [ipx::current_core]

# Update Parameter Types
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_RX1_DCLK" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_RX1_DCLK -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_RX1_DCLK -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_RX1_DCLK -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_RX1_DCLK -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_RX1_IDATA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_RX1_IDATA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_RX1_IDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_RX1_IDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_RX1_IDATA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_RX1_QDATA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_RX1_QDATA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_RX1_QDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_RX1_QDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_RX1_QDATA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_RX1_STROBE" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_RX1_STROBE -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_RX1_STROBE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_RX1_STROBE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_RX1_STROBE -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_RX2_DCLK" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_RX2_DCLK -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_RX2_DCLK -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_RX2_DCLK -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_RX2_DCLK -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_RX2_IDATA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_RX2_IDATA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_RX2_IDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_RX2_IDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_RX2_IDATA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_RX2_QDATA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_RX2_QDATA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_RX2_QDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_RX2_QDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_RX2_QDATA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_RX2_STROBE" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_RX2_STROBE -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_RX2_STROBE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_RX2_STROBE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_RX2_STROBE -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_TX1_DCLK_IN" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_TX1_DCLK_IN -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_TX1_DCLK_IN -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_TX1_DCLK_IN -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_TX1_DCLK_IN -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_TX1_DCLK_OUT" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_TX1_DCLK_OUT -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_TX1_DCLK_OUT -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_TX1_DCLK_OUT -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_TX1_DCLK_OUT -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_TX1_IDATA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_TX1_IDATA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_TX1_IDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_TX1_IDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_TX1_IDATA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_TX1_QDATA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_TX1_QDATA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_TX1_QDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_TX1_QDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_TX1_QDATA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_TX1_STROBE" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_TX1_STROBE -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_TX1_STROBE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_TX1_STROBE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_TX1_STROBE -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_TX2_DCLK_IN" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_TX2_DCLK_IN -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_TX2_DCLK_IN -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_TX2_DCLK_IN -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_TX2_DCLK_IN -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_TX2_DCLK_OUT" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_TX2_DCLK_OUT -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_TX2_DCLK_OUT -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_TX2_DCLK_OUT -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_TX2_DCLK_OUT -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_TX2_IDATA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_TX2_IDATA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_TX2_IDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_TX2_IDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_TX2_IDATA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_TX2_QDATA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_TX2_QDATA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_TX2_QDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_TX2_QDATA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_TX2_QDATA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "SWAP_DIFF_TX2_STROBE" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters SWAP_DIFF_TX2_STROBE -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters SWAP_DIFF_TX2_STROBE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters SWAP_DIFF_TX2_STROBE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters SWAP_DIFF_TX2_STROBE -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_TX1_ILA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_TX1_ILA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_TX1_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_TX1_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_TX1_ILA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_TX2_ILA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_TX2_ILA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_TX2_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_TX2_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_TX2_ILA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_RX1_ILA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_RX1_ILA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_RX1_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_RX1_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_RX1_ILA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_RX2_ILA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_RX2_ILA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_RX2_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_RX2_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_RX2_ILA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_RX2_ILA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_RX2_ILA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_RX2_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_RX2_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_RX2_ILA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_SPI_ILA" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_SPI_ILA -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_SPI_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_SPI_ILA -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_SPI_ILA -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_PL_DGPIO" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_PL_DGPIO -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_PL_DGPIO -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_PL_DGPIO -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_PL_DGPIO -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_PL_RX1_ENABLE" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_PL_RX1_ENABLE -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_PL_RX1_ENABLE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_PL_RX1_ENABLE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_PL_RX1_ENABLE -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_PL_RX2_ENABLE" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_PL_RX2_ENABLE -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_PL_RX2_ENABLE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_PL_RX2_ENABLE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_PL_RX2_ENABLE -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_PL_TX1_ENABLE" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_PL_TX1_ENABLE -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_PL_TX1_ENABLE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_PL_TX1_ENABLE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_PL_TX1_ENABLE -of_objects [ipx::current_core]]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_PL_TX2_ENABLE" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_PL_TX2_ENABLE -of_objects [ipx::current_core]]
set_property value true [ipx::get_hdl_parameters ENABLE_PL_TX2_ENABLE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_PL_TX2_ENABLE -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_hdl_parameters ENABLE_PL_TX2_ENABLE -of_objects [ipx::current_core]]


# Add parameters
ipx::add_user_parameter ENABLE_PL_IRQ [ipx::current_core]
set_property value_resolve_type user [ipx::get_user_parameters ENABLE_PL_IRQ -of_objects [ipx::current_core]]
ipgui::add_param -name {ENABLE_PL_IRQ} -component [ipx::current_core]
set_property display_name {Enable Pl Irq} [ipgui::get_guiparamspec -name "ENABLE_PL_IRQ" -component [ipx::current_core] ]
set_property widget {checkBox} [ipgui::get_guiparamspec -name "ENABLE_PL_IRQ" -component [ipx::current_core] ]
set_property value true [ipx::get_user_parameters ENABLE_PL_IRQ -of_objects [ipx::current_core]]
set_property value_format bool [ipx::get_user_parameters ENABLE_PL_IRQ -of_objects [ipx::current_core]]

# Associate Signal Display to parameters
set_property enablement_resolve_type dependent [ipx::get_ports rx1_pl_en -of_objects [ipx::current_core]]
set_property driver_value 0 [ipx::get_ports rx1_pl_en -of_objects [ipx::current_core]]
set_property enablement_dependency {$ENABLE_PL_RX1_ENABLE > 0} [ipx::get_ports rx1_pl_en -of_objects [ipx::current_core]]
set_property enablement_resolve_type dependent [ipx::get_ports rx2_pl_en -of_objects [ipx::current_core]]
set_property driver_value 0 [ipx::get_ports rx2_pl_en -of_objects [ipx::current_core]]
set_property enablement_dependency {$ENABLE_PL_RX2_ENABLE > 0} [ipx::get_ports rx2_pl_en -of_objects [ipx::current_core]]
set_property enablement_resolve_type dependent [ipx::get_ports tx1_pl_en -of_objects [ipx::current_core]]
set_property driver_value 0 [ipx::get_ports tx1_pl_en -of_objects [ipx::current_core]]
set_property enablement_dependency {$ENABLE_PL_TX1_ENABLE > 0} [ipx::get_ports tx1_pl_en -of_objects [ipx::current_core]]
set_property enablement_resolve_type dependent [ipx::get_ports tx2_pl_en -of_objects [ipx::current_core]]
set_property driver_value 0 [ipx::get_ports tx2_pl_en -of_objects [ipx::current_core]]
set_property enablement_dependency {$ENABLE_PL_TX2_ENABLE > 0} [ipx::get_ports tx2_pl_en -of_objects [ipx::current_core]]
set_property enablement_resolve_type dependent [ipx::get_ports dgpio_pl_o -of_objects [ipx::current_core]]
set_property driver_value 0 [ipx::get_ports dgpio_pl_o -of_objects [ipx::current_core]]
set_property enablement_dependency {$ENABLE_PL_DGPIO > 0} [ipx::get_ports dgpio_pl_o -of_objects [ipx::current_core]]
set_property enablement_resolve_type dependent [ipx::get_ports dgpio_pl_i -of_objects [ipx::current_core]]
set_property driver_value 0 [ipx::get_ports dgpio_pl_i -of_objects [ipx::current_core]]
set_property enablement_dependency {$ENABLE_PL_DGPIO > 0} [ipx::get_ports dgpio_pl_i -of_objects [ipx::current_core]]
set_property enablement_dependency {$ENABLE_PL_IRQ > 0} [ipx::get_bus_interfaces pl_irq -of_objects [ipx::current_core]]


# Add SSI Configuration Page
ipgui::add_page -name {SSI Pins} -component [ipx::current_core] -display_name {SSI Pins}
set_property tooltip {Configure SSI Pins} [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core] ]
ipgui::add_static_text -name {Description} -component [ipx::current_core] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core] ] -text {
Each SSI differential pair can be swapped to account for PCB designs with swapped pairs.  
This allows PCB designers to swap p/n signals on each differential pair to facilitate PCB routing.
}
ipgui::move_param -component [ipx::current_core] -order 1  [ipgui::get_guiparamspec -name "SWAP_DIFF_RX1_DCLK" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 2  [ipgui::get_guiparamspec -name "SWAP_DIFF_RX1_IDATA" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 3  [ipgui::get_guiparamspec -name "SWAP_DIFF_RX1_QDATA" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 4  [ipgui::get_guiparamspec -name "SWAP_DIFF_RX1_STROBE" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 5  [ipgui::get_guiparamspec -name "SWAP_DIFF_RX2_DCLK" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 6  [ipgui::get_guiparamspec -name "SWAP_DIFF_RX2_IDATA" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 7  [ipgui::get_guiparamspec -name "SWAP_DIFF_RX2_QDATA" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 8  [ipgui::get_guiparamspec -name "SWAP_DIFF_RX2_STROBE" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 9  [ipgui::get_guiparamspec -name "SWAP_DIFF_TX1_DCLK_IN" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 10 [ipgui::get_guiparamspec -name "SWAP_DIFF_TX1_DCLK_OUT" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 11 [ipgui::get_guiparamspec -name "SWAP_DIFF_TX1_IDATA" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 12 [ipgui::get_guiparamspec -name "SWAP_DIFF_TX1_QDATA" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 13 [ipgui::get_guiparamspec -name "SWAP_DIFF_TX1_STROBE" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 14 [ipgui::get_guiparamspec -name "SWAP_DIFF_TX2_DCLK_IN" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 15 [ipgui::get_guiparamspec -name "SWAP_DIFF_TX2_DCLK_OUT" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 16 [ipgui::get_guiparamspec -name "SWAP_DIFF_TX2_IDATA" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 17 [ipgui::get_guiparamspec -name "SWAP_DIFF_TX2_QDATA" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 18 [ipgui::get_guiparamspec -name "SWAP_DIFF_TX2_STROBE" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "SSI Pins" -component [ipx::current_core]]


# Add ILA Configuration Page
set_property display_name {ILA} [ipgui::get_pagespec -name "Page 0" -component [ipx::current_core] ]
set_property tooltip {Configure Integrated Logic Analyzers} [ipgui::get_pagespec -name "Page 0" -component [ipx::current_core] ]
ipgui::add_static_text -name {ILA Description} -component [ipx::current_core] -parent [ipgui::get_pagespec -name "Page 0" -component [ipx::current_core] ] -text {
The following integrated logic analyzers can be enabled to facilitate evaluation of this module.
}
ipgui::move_text -component [ipx::current_core] -order 0 [ipgui::get_textspec -name "ILA Description" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "Page 0" -component [ipx::current_core]]

# Add PL Interface Page
ipgui::add_page -name {PL Interface} -component [ipx::current_core] -display_name {PL Interface}
set_property tooltip {Configure Programmable Logic Interface Signals} [ipgui::get_pagespec -name "PL Interface" -component [ipx::current_core] ]

ipgui::add_static_text -name {PL Enable Interface Description} -component [ipx::current_core] -parent [ipgui::get_pagespec -name "PL Interface" -component [ipx::current_core] ] -text {
Enable control of signals from programmable logic:
}
ipgui::move_param -component [ipx::current_core] -order 1 [ipgui::get_guiparamspec -name "ENABLE_PL_RX1_ENABLE" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "PL Interface" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 2 [ipgui::get_guiparamspec -name "ENABLE_PL_RX2_ENABLE" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "PL Interface" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 3 [ipgui::get_guiparamspec -name "ENABLE_PL_TX1_ENABLE" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "PL Interface" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 4 [ipgui::get_guiparamspec -name "ENABLE_PL_TX2_ENABLE" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "PL Interface" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 5 [ipgui::get_guiparamspec -name "ENABLE_PL_DGPIO" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "PL Interface" -component [ipx::current_core]]
ipgui::move_param -component [ipx::current_core] -order 6 [ipgui::get_guiparamspec -name "ENABLE_PL_IRQ" -component [ipx::current_core]] -parent [ipgui::get_pagespec -name "PL Interface" -component [ipx::current_core]]


# Create IP files and save
set_property core_revision 2 [ipx::current_core]
ipx::create_xgui_files [ipx::current_core]
ipx::update_checksums [ipx::current_core]
ipx::check_integrity [ipx::current_core]
ipx::save_core [ipx::current_core]
update_ip_catalog -rebuild -repo_path $proj_name
