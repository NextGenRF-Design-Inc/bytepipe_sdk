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
set ip_name adrv9002
set ver 1.0
set vendor NGRF
set url http://www.nextgenrf.com
set description { 

This module combines the receive SSI, transmit SSI, and AXI configuration module for the 
ADRV9002.


}
# ******************************************************************************************
# ******************************************************************************************

create_project $ip_name vivado -part xczu2cg-sbva484-1-e -force

# Add source Files
add_files -norecurse src/adrv9002.vp
add_files -norecurse src/adrv9002.xml
add_files -norecurse src/axi_adrv9001.vp
add_files -norecurse src/axi_adrv9001_regs.vp
add_files -norecurse src/adrv9001_tx.vp
add_files -norecurse src/adrv9001_tx_clk.vp
add_files -norecurse src/adrv9001_tx_serdes.vp
add_files -norecurse src/adrv9001_serdes_unpack.vp
add_files -norecurse src/adrv9001_rx.vp
add_files -norecurse src/adrv9001_rx_serdes.vp
add_files -norecurse src/adrv9001_rx_serdes_phase.vp
add_files -norecurse src/adrv9001_serdes_aligner.vp
add_files -norecurse src/adrv9001_serdes_pack.vp
add_files -norecurse src/async_rst/async_rst.xci

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

ipx::remove_bus_interface ext [ipx::current_core]
ipx::remove_bus_interface rx1_axis [ipx::current_core]
ipx::remove_bus_interface rx2_axis [ipx::current_core]
ipx::remove_bus_interface tx1_axis [ipx::current_core]
ipx::remove_bus_interface tx2_axis [ipx::current_core]
ipx::infer_bus_interface {rx1_en rx2_en tx1_en tx2_en rstn irq dgpio rx1_dclk_p rx1_dclk_n rx1_strobe_p rx1_strobe_n rx1_idata_p rx1_idata_n rx1_qdata_p rx1_qdata_n rx2_dclk_p rx2_dclk_n rx2_strobe_p rx2_strobe_n rx2_idata_p rx2_idata_n rx2_qdata_p rx2_qdata_n tx1_ref_clk_p tx1_ref_clk_n tx1_dclk_p tx1_dclk_n tx1_strobe_p tx1_strobe_n tx1_idata_p tx1_idata_n tx1_qdata_p tx1_qdata_n tx2_ref_clk_p tx2_ref_clk_n tx2_dclk_p tx2_dclk_n tx2_strobe_p tx2_strobe_n tx2_idata_p tx2_idata_n tx2_qdata_p tx2_qdata_n} nextgenrf.com:user:adrv9002_rtl:1.0 [ipx::current_core]
set_property name adrv9002 [ipx::get_bus_interfaces adrv9002_1 -of_objects [ipx::current_core]]
set_property display_name adrv9002 [ipx::get_bus_interfaces adrv9002 -of_objects [ipx::current_core]]
ipx::infer_bus_interface {rx1_axis_tdata rx1_axis_tvalid} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface {rx2_axis_tdata rx2_axis_tvalid} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface {tx1_axis_tdata tx1_axis_tvalid tx1_axis_tready} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface {tx2_axis_tdata tx2_axis_tvalid tx2_axis_tready} xilinx.com:interface:axis_rtl:1.0 [ipx::current_core]
ipx::associate_bus_interfaces -busif rx1_axis -clock rx1_axis_aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif rx2_axis -clock rx2_axis_aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif tx1_axis -clock tx1_axis_aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif tx2_axis -clock tx2_axis_aclk [ipx::current_core]
ipx::add_bus_parameter FREQ_HZ [ipx::get_bus_interfaces rx1_axis_aclk -of_objects [ipx::current_core]]
ipx::add_bus_parameter FREQ_HZ [ipx::get_bus_interfaces rx2_axis_aclk -of_objects [ipx::current_core]]
ipx::add_bus_parameter FREQ_HZ [ipx::get_bus_interfaces tx1_axis_aclk -of_objects [ipx::current_core]]
ipx::add_bus_parameter FREQ_HZ [ipx::get_bus_interfaces tx2_axis_aclk -of_objects [ipx::current_core]]



set disclaimer {


      Copyright 2020 (c) NextGen RF Design. All rights reserved.

 This core is distributed WITHOUT ANY WARRANTY; without even the implied 
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
}

ipgui::add_static_text -name {text1} -component [ipx::current_core] -parent [ipgui::get_pagespec -name "Page 0" -component [ipx::current_core] ] -text $description
ipgui::add_static_text -name {text2} -component [ipx::current_core] -parent [ipgui::get_pagespec -name "Page 0" -component [ipx::current_core] ] -text $disclaimer

update_compile_order -fileset sources_1
launch_runs synth_1 -jobs 10
wait_on_run -quiet synth_1

ipx::merge_project_changes files [ipx::current_core]
    
ipx::add_file_group -type utility xilinx_utilityxitfiles [ipx::current_core]
ipx::create_xgui_files [ipx::current_core]
ipx::update_checksums [ipx::current_core]
ipx::save_core [ipx::current_core]
ipx::merge_project_changes files [ipx::current_core]
ipx::save_core [ipx::current_core]

# Close the current project.
close_project
file delete -force vivado
file delete -force .Xil
file delete -force *.log      

