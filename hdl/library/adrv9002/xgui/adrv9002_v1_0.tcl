# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "SWAP_DIFF_RX1_DCLK" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_RX1_IDATA" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_RX1_QDATA" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_RX1_STROBE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_RX2_DCLK" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_RX2_IDATA" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_RX2_QDATA" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_RX2_STROBE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_TX1_DCLK_IN" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_TX1_DCLK_OUT" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_TX1_IDATA" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_TX1_QDATA" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_TX1_STROBE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_TX2_DCLK_IN" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_TX2_DCLK_OUT" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_TX2_IDATA" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_TX2_QDATA" -parent ${Page_0}
  ipgui::add_param $IPINST -name "SWAP_DIFF_TX2_STROBE" -parent ${Page_0}
  ipgui::add_static_text $IPINST -name "text1" -parent ${Page_0} -text { 

This module combines the receive SSI, transmit SSI, and AXI configuration module for the 
ADRV9002.


}
  ipgui::add_static_text $IPINST -name "text2" -parent ${Page_0} -text {


      Copyright 2020 (c) NextGen RF Design. All rights reserved.

 This core is distributed WITHOUT ANY WARRANTY; without even the implied 
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
}


}

proc update_PARAM_VALUE.SWAP_DIFF_RX1_DCLK { PARAM_VALUE.SWAP_DIFF_RX1_DCLK } {
	# Procedure called to update SWAP_DIFF_RX1_DCLK when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_RX1_DCLK { PARAM_VALUE.SWAP_DIFF_RX1_DCLK } {
	# Procedure called to validate SWAP_DIFF_RX1_DCLK
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_RX1_IDATA { PARAM_VALUE.SWAP_DIFF_RX1_IDATA } {
	# Procedure called to update SWAP_DIFF_RX1_IDATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_RX1_IDATA { PARAM_VALUE.SWAP_DIFF_RX1_IDATA } {
	# Procedure called to validate SWAP_DIFF_RX1_IDATA
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_RX1_QDATA { PARAM_VALUE.SWAP_DIFF_RX1_QDATA } {
	# Procedure called to update SWAP_DIFF_RX1_QDATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_RX1_QDATA { PARAM_VALUE.SWAP_DIFF_RX1_QDATA } {
	# Procedure called to validate SWAP_DIFF_RX1_QDATA
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_RX1_STROBE { PARAM_VALUE.SWAP_DIFF_RX1_STROBE } {
	# Procedure called to update SWAP_DIFF_RX1_STROBE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_RX1_STROBE { PARAM_VALUE.SWAP_DIFF_RX1_STROBE } {
	# Procedure called to validate SWAP_DIFF_RX1_STROBE
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_RX2_DCLK { PARAM_VALUE.SWAP_DIFF_RX2_DCLK } {
	# Procedure called to update SWAP_DIFF_RX2_DCLK when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_RX2_DCLK { PARAM_VALUE.SWAP_DIFF_RX2_DCLK } {
	# Procedure called to validate SWAP_DIFF_RX2_DCLK
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_RX2_IDATA { PARAM_VALUE.SWAP_DIFF_RX2_IDATA } {
	# Procedure called to update SWAP_DIFF_RX2_IDATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_RX2_IDATA { PARAM_VALUE.SWAP_DIFF_RX2_IDATA } {
	# Procedure called to validate SWAP_DIFF_RX2_IDATA
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_RX2_QDATA { PARAM_VALUE.SWAP_DIFF_RX2_QDATA } {
	# Procedure called to update SWAP_DIFF_RX2_QDATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_RX2_QDATA { PARAM_VALUE.SWAP_DIFF_RX2_QDATA } {
	# Procedure called to validate SWAP_DIFF_RX2_QDATA
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_RX2_STROBE { PARAM_VALUE.SWAP_DIFF_RX2_STROBE } {
	# Procedure called to update SWAP_DIFF_RX2_STROBE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_RX2_STROBE { PARAM_VALUE.SWAP_DIFF_RX2_STROBE } {
	# Procedure called to validate SWAP_DIFF_RX2_STROBE
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_TX1_DCLK_IN { PARAM_VALUE.SWAP_DIFF_TX1_DCLK_IN } {
	# Procedure called to update SWAP_DIFF_TX1_DCLK_IN when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_TX1_DCLK_IN { PARAM_VALUE.SWAP_DIFF_TX1_DCLK_IN } {
	# Procedure called to validate SWAP_DIFF_TX1_DCLK_IN
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_TX1_DCLK_OUT { PARAM_VALUE.SWAP_DIFF_TX1_DCLK_OUT } {
	# Procedure called to update SWAP_DIFF_TX1_DCLK_OUT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_TX1_DCLK_OUT { PARAM_VALUE.SWAP_DIFF_TX1_DCLK_OUT } {
	# Procedure called to validate SWAP_DIFF_TX1_DCLK_OUT
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_TX1_IDATA { PARAM_VALUE.SWAP_DIFF_TX1_IDATA } {
	# Procedure called to update SWAP_DIFF_TX1_IDATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_TX1_IDATA { PARAM_VALUE.SWAP_DIFF_TX1_IDATA } {
	# Procedure called to validate SWAP_DIFF_TX1_IDATA
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_TX1_QDATA { PARAM_VALUE.SWAP_DIFF_TX1_QDATA } {
	# Procedure called to update SWAP_DIFF_TX1_QDATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_TX1_QDATA { PARAM_VALUE.SWAP_DIFF_TX1_QDATA } {
	# Procedure called to validate SWAP_DIFF_TX1_QDATA
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_TX1_STROBE { PARAM_VALUE.SWAP_DIFF_TX1_STROBE } {
	# Procedure called to update SWAP_DIFF_TX1_STROBE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_TX1_STROBE { PARAM_VALUE.SWAP_DIFF_TX1_STROBE } {
	# Procedure called to validate SWAP_DIFF_TX1_STROBE
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_TX2_DCLK_IN { PARAM_VALUE.SWAP_DIFF_TX2_DCLK_IN } {
	# Procedure called to update SWAP_DIFF_TX2_DCLK_IN when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_TX2_DCLK_IN { PARAM_VALUE.SWAP_DIFF_TX2_DCLK_IN } {
	# Procedure called to validate SWAP_DIFF_TX2_DCLK_IN
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_TX2_DCLK_OUT { PARAM_VALUE.SWAP_DIFF_TX2_DCLK_OUT } {
	# Procedure called to update SWAP_DIFF_TX2_DCLK_OUT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_TX2_DCLK_OUT { PARAM_VALUE.SWAP_DIFF_TX2_DCLK_OUT } {
	# Procedure called to validate SWAP_DIFF_TX2_DCLK_OUT
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_TX2_IDATA { PARAM_VALUE.SWAP_DIFF_TX2_IDATA } {
	# Procedure called to update SWAP_DIFF_TX2_IDATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_TX2_IDATA { PARAM_VALUE.SWAP_DIFF_TX2_IDATA } {
	# Procedure called to validate SWAP_DIFF_TX2_IDATA
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_TX2_QDATA { PARAM_VALUE.SWAP_DIFF_TX2_QDATA } {
	# Procedure called to update SWAP_DIFF_TX2_QDATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_TX2_QDATA { PARAM_VALUE.SWAP_DIFF_TX2_QDATA } {
	# Procedure called to validate SWAP_DIFF_TX2_QDATA
	return true
}

proc update_PARAM_VALUE.SWAP_DIFF_TX2_STROBE { PARAM_VALUE.SWAP_DIFF_TX2_STROBE } {
	# Procedure called to update SWAP_DIFF_TX2_STROBE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SWAP_DIFF_TX2_STROBE { PARAM_VALUE.SWAP_DIFF_TX2_STROBE } {
	# Procedure called to validate SWAP_DIFF_TX2_STROBE
	return true
}


proc update_MODELPARAM_VALUE.SWAP_DIFF_TX1_IDATA { MODELPARAM_VALUE.SWAP_DIFF_TX1_IDATA PARAM_VALUE.SWAP_DIFF_TX1_IDATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_TX1_IDATA}] ${MODELPARAM_VALUE.SWAP_DIFF_TX1_IDATA}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_TX1_QDATA { MODELPARAM_VALUE.SWAP_DIFF_TX1_QDATA PARAM_VALUE.SWAP_DIFF_TX1_QDATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_TX1_QDATA}] ${MODELPARAM_VALUE.SWAP_DIFF_TX1_QDATA}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_TX1_STROBE { MODELPARAM_VALUE.SWAP_DIFF_TX1_STROBE PARAM_VALUE.SWAP_DIFF_TX1_STROBE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_TX1_STROBE}] ${MODELPARAM_VALUE.SWAP_DIFF_TX1_STROBE}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_TX1_DCLK_IN { MODELPARAM_VALUE.SWAP_DIFF_TX1_DCLK_IN PARAM_VALUE.SWAP_DIFF_TX1_DCLK_IN } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_TX1_DCLK_IN}] ${MODELPARAM_VALUE.SWAP_DIFF_TX1_DCLK_IN}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_TX1_DCLK_OUT { MODELPARAM_VALUE.SWAP_DIFF_TX1_DCLK_OUT PARAM_VALUE.SWAP_DIFF_TX1_DCLK_OUT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_TX1_DCLK_OUT}] ${MODELPARAM_VALUE.SWAP_DIFF_TX1_DCLK_OUT}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_TX2_IDATA { MODELPARAM_VALUE.SWAP_DIFF_TX2_IDATA PARAM_VALUE.SWAP_DIFF_TX2_IDATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_TX2_IDATA}] ${MODELPARAM_VALUE.SWAP_DIFF_TX2_IDATA}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_TX2_QDATA { MODELPARAM_VALUE.SWAP_DIFF_TX2_QDATA PARAM_VALUE.SWAP_DIFF_TX2_QDATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_TX2_QDATA}] ${MODELPARAM_VALUE.SWAP_DIFF_TX2_QDATA}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_TX2_STROBE { MODELPARAM_VALUE.SWAP_DIFF_TX2_STROBE PARAM_VALUE.SWAP_DIFF_TX2_STROBE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_TX2_STROBE}] ${MODELPARAM_VALUE.SWAP_DIFF_TX2_STROBE}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_TX2_DCLK_IN { MODELPARAM_VALUE.SWAP_DIFF_TX2_DCLK_IN PARAM_VALUE.SWAP_DIFF_TX2_DCLK_IN } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_TX2_DCLK_IN}] ${MODELPARAM_VALUE.SWAP_DIFF_TX2_DCLK_IN}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_TX2_DCLK_OUT { MODELPARAM_VALUE.SWAP_DIFF_TX2_DCLK_OUT PARAM_VALUE.SWAP_DIFF_TX2_DCLK_OUT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_TX2_DCLK_OUT}] ${MODELPARAM_VALUE.SWAP_DIFF_TX2_DCLK_OUT}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_RX1_IDATA { MODELPARAM_VALUE.SWAP_DIFF_RX1_IDATA PARAM_VALUE.SWAP_DIFF_RX1_IDATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_RX1_IDATA}] ${MODELPARAM_VALUE.SWAP_DIFF_RX1_IDATA}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_RX1_QDATA { MODELPARAM_VALUE.SWAP_DIFF_RX1_QDATA PARAM_VALUE.SWAP_DIFF_RX1_QDATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_RX1_QDATA}] ${MODELPARAM_VALUE.SWAP_DIFF_RX1_QDATA}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_RX1_STROBE { MODELPARAM_VALUE.SWAP_DIFF_RX1_STROBE PARAM_VALUE.SWAP_DIFF_RX1_STROBE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_RX1_STROBE}] ${MODELPARAM_VALUE.SWAP_DIFF_RX1_STROBE}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_RX1_DCLK { MODELPARAM_VALUE.SWAP_DIFF_RX1_DCLK PARAM_VALUE.SWAP_DIFF_RX1_DCLK } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_RX1_DCLK}] ${MODELPARAM_VALUE.SWAP_DIFF_RX1_DCLK}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_RX2_IDATA { MODELPARAM_VALUE.SWAP_DIFF_RX2_IDATA PARAM_VALUE.SWAP_DIFF_RX2_IDATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_RX2_IDATA}] ${MODELPARAM_VALUE.SWAP_DIFF_RX2_IDATA}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_RX2_QDATA { MODELPARAM_VALUE.SWAP_DIFF_RX2_QDATA PARAM_VALUE.SWAP_DIFF_RX2_QDATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_RX2_QDATA}] ${MODELPARAM_VALUE.SWAP_DIFF_RX2_QDATA}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_RX2_STROBE { MODELPARAM_VALUE.SWAP_DIFF_RX2_STROBE PARAM_VALUE.SWAP_DIFF_RX2_STROBE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_RX2_STROBE}] ${MODELPARAM_VALUE.SWAP_DIFF_RX2_STROBE}
}

proc update_MODELPARAM_VALUE.SWAP_DIFF_RX2_DCLK { MODELPARAM_VALUE.SWAP_DIFF_RX2_DCLK PARAM_VALUE.SWAP_DIFF_RX2_DCLK } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SWAP_DIFF_RX2_DCLK}] ${MODELPARAM_VALUE.SWAP_DIFF_RX2_DCLK}
}

