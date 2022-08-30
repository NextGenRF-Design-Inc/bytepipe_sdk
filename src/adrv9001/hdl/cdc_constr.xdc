set_property ASYNC_REG TRUE [get_cells -hier -filter {name =~ *m_cdc_tdata*}]
set_false_path -to [get_cells -hier -filter {name =~ *cdc_tdata_reg* && IS_SEQUENTIAL}]

