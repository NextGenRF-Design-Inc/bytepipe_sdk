# ******************************************************************************************
# ******************************************************************************************
#              Copyright 2020 (c) NextGen RF Design. All rights reserved.
#
#        This core is distributed WITHOUT ANY WARRANTY; without even the implied 
#           warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# ******************************************************************************************
# ******************************************************************************************

proc add_interface_ports {name dir width {type none}} {

  ipx::add_bus_abstraction_port $name [ipx::current_busabs]
  set if_param_list {}
  lappend if_param_list master_presence required
  lappend if_param_list slave_presence required
  if {$width > 0} {
    lappend if_param_list master_width $width
    lappend if_param_list slave_width $width
  }
  lappend if_param_list master_direction $dir
  if {$dir eq "in"} {
    lappend if_param_list slave_direction out
  } elseif {$dir eq "out"} {
    lappend if_param_list slave_direction in
  } else {
    lappend if_param_list slave_direction $dir
  }
  if {$type ne "none"} {
    lappend if_param_list is_${type} true
  }
  set_property -dict $if_param_list [ipx::get_bus_abstraction_ports $name -of_objects [ipx::current_busabs]]
}

proc create_bus_interface {if_name projDir} {
  ipx::create_abstraction_definition nextgenrf.com user ${if_name}_rtl 1.0
  ipx::create_bus_definition nextgenrf.com user ${if_name} 1.0
  set_property xml_file_name $projDir/${if_name}_rtl.xml [ipx::current_busabs]
  set_property xml_file_name $projDir/${if_name}.xml [ipx::current_busdef]
  set_property bus_type_vlnv nextgenrf.com:user:${if_name}:1.0 [ipx::current_busabs]
  ipx::save_abstraction_definition [ipx::current_busabs]
  ipx::save_bus_definition [ipx::current_busdef]
}

proc save_bus_interface {if_name} {
  ipx::save_bus_definition [ipx::current_busdef]
  set_property bus_type_vlnv nextgenrf.com:user:${if_name}:1.0 [ipx::current_busabs]
  ipx::save_abstraction_definition [ipx::current_busabs]
}

proc pkg_ip_core {ip_name} {

  ipx::package_project -root_dir $ip_name -vendor nextgenrf.com -library user -taxonomy /NextGenRF
  set_property name $ip_name [ipx::current_core]
  set_property vendor_display_name {NextGenRF Design} [ipx::current_core]
  set_property company_url {http://www.nextgenrf.com} [ipx::current_core]
  ipx::save_core

  set xil_devices {}
  foreach xil_part [get_parts] {
    lappend xil_devices [get_property family $xil_part]
  }
  set xil_devices [lsort -unique $xil_devices]
  set ip_devices [get_property supported_families [ipx::current_core]]
  foreach xil_device $xil_devices {
    set ip_devices "$ip_devices $xil_device Production"
    set ip_devices "$ip_devices $xil_device Beta"
  }
  set_property supported_families $ip_devices [ipx::current_core]
  ipx::save_core
}

proc map_port {if_name name} {
  ipx::remove_bus_interface $name [ipx::current_core]
  ipx::add_port_map $name [ipx::get_bus_interfaces $if_name -of_objects [ipx::current_core]]
  set_property physical_name $name [ipx::get_port_maps $name -of_objects [ipx::get_bus_interfaces $if_name -of_objects [ipx::current_core]]]
}

