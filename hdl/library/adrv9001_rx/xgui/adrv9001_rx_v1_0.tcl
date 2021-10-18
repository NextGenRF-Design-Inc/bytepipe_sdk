# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "DEVICE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_DCLK" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_IDATA" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_QDATA" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_STROBE" -parent ${Page_0}
  ipgui::add_static_text $IPINST -name "text1" -parent ${Page_0} -text { 

This module instantiates a receive channel for interfacing to the ADRV9001/2.  Only LVDS
is supported.  The strobe can be configured as 8-bits on 8-bits off or 1-bit on 15-bits off.
Each lane supports a fixed configurable delay specified in pico-seconds relative to the 
clock lane.


}
  ipgui::add_static_text $IPINST -name "text2" -parent ${Page_0} -text {


      Copyright 2020 (c) NextGen RF Design. All rights reserved.

 This core is distributed WITHOUT ANY WARRANTY; without even the implied 
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
}


}

proc update_PARAM_VALUE.DEVICE { PARAM_VALUE.DEVICE } {
	# Procedure called to update DEVICE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DEVICE { PARAM_VALUE.DEVICE } {
	# Procedure called to validate DEVICE
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_DCLK { PARAM_VALUE.SWAP_DIFF_DCLK } {
	# Procedure called to update SWAP_DIFF_DCLK when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_DCLK { PARAM_VALUE.SWAP_DIFF_DCLK } {
	# Procedure called to validate SWAP_DIFF_DCLK
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_IDATA { PARAM_VALUE.SWAP_DIFF_IDATA } {
	# Procedure called to update SWAP_DIFF_IDATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_IDATA { PARAM_VALUE.SWAP_DIFF_IDATA } {
	# Procedure called to validate SWAP_DIFF_IDATA
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_QDATA { PARAM_VALUE.SWAP_DIFF_QDATA } {
	# Procedure called to update SWAP_DIFF_QDATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_QDATA { PARAM_VALUE.SWAP_DIFF_QDATA } {
	# Procedure called to validate SWAP_DIFF_QDATA
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_STROBE { PARAM_VALUE.SWAP_DIFF_STROBE } {
	# Procedure called to update SWAP_DIFF_STROBE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_STROBE { PARAM_VALUE.SWAP_DIFF_STROBE } {
	# Procedure called to validate SWAP_DIFF_STROBE
	return true
}


proc update_MODELPARAM_VALUE.DEVICE { MODELPARAM_VALUE.DEVICE PARAM_VALUE.DEVICE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DEVICE}] ${MODELPARAM_VALUE.DEVICE}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_IDATA { MODELPARAM_VALUE.SWAP_DIFF_IDATA PARAM_VALUE.SWAP_DIFF_IDATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_IDATA}] ${MODELPARAM_VALUE.SWAP_DIFF_IDATA}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_QDATA { MODELPARAM_VALUE.SWAP_DIFF_QDATA PARAM_VALUE.SWAP_DIFF_QDATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_QDATA}] ${MODELPARAM_VALUE.SWAP_DIFF_QDATA}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_STROBE { MODELPARAM_VALUE.SWAP_DIFF_STROBE PARAM_VALUE.SWAP_DIFF_STROBE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_STROBE}] ${MODELPARAM_VALUE.SWAP_DIFF_STROBE}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_DCLK { MODELPARAM_VALUE.SWAP_DIFF_DCLK PARAM_VALUE.SWAP_DIFF_DCLK } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_DCLK}] ${MODELPARAM_VALUE.SWAP_DIFF_DCLK}
}

