# Configure Axis clocks
set_property CONFIG.FREQ_HZ 122880000 [get_bd_pins /adrv9002/rx1_axis_aclk]
set_property CONFIG.FREQ_HZ 122880000 [get_bd_pins /adrv9002/rx2_axis_aclk]
set_property CONFIG.FREQ_HZ 122880000 [get_bd_pins /adrv9002/tx2_axis_aclk]
set_property CONFIG.FREQ_HZ 122880000 [get_bd_pins /adrv9002/tx1_axis_aclk]


