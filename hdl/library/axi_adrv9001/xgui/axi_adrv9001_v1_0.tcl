# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "C_s_axi_ADDR_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_s_axi_DATA_WIDTH" -parent ${Page_0}
  ipgui::add_static_text $IPINST -name "text1" -parent ${Page_0} -text { 

This module instantiates an axi module for controlling the adrv9001.


}
  ipgui::add_static_text $IPINST -name "text2" -parent ${Page_0} -text {


      Copyright 2020 (c) NextGen RF Design. All rights reserved.

 This core is distributed WITHOUT ANY WARRANTY; without even the implied 
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
}


}

proc update_PARAM_VALUE.C_s_axi_ADDR_WIDTH { PARAM_VALUE.C_s_axi_ADDR_WIDTH } {
	# Procedure called to update C_s_axi_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_s_axi_ADDR_WIDTH { PARAM_VALUE.C_s_axi_ADDR_WIDTH } {
	# Procedure called to validate C_s_axi_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_s_axi_DATA_WIDTH { PARAM_VALUE.C_s_axi_DATA_WIDTH } {
	# Procedure called to update C_s_axi_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_s_axi_DATA_WIDTH { PARAM_VALUE.C_s_axi_DATA_WIDTH } {
	# Procedure called to validate C_s_axi_DATA_WIDTH
	return true
}


proc update_MODELPARAM_VALUE.C_s_axi_DATA_WIDTH { MODELPARAM_VALUE.C_s_axi_DATA_WIDTH PARAM_VALUE.C_s_axi_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_s_axi_DATA_WIDTH}] ${MODELPARAM_VALUE.C_s_axi_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_s_axi_ADDR_WIDTH { MODELPARAM_VALUE.C_s_axi_ADDR_WIDTH PARAM_VALUE.C_s_axi_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_s_axi_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_s_axi_ADDR_WIDTH}
}

