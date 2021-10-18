
set project_name adrv9001_rx_tb

# Create Project
create_project $project_name vivado_sim -part xczu2cg-sbva484-1-e -force

# Add source Files
add_files -fileset sim_1 -norecurse src/adrv9001_rx_tb.v
add_files -fileset sim_1 -norecurse src/adrv9001_rx_serdes_tb.v
add_files -norecurse src/adrv9001_rx.v
add_files -norecurse src/adrv9001_rx_serdes.v
add_files -norecurse src/adrv9001_rx_serdes_phase.v
add_files -norecurse src/adrv9001_serdes_aligner.v
add_files -norecurse src/adrv9001_serdes_pack.v
add_files -norecurse src/adrv9001_rx_clk.v
add_files -norecurse src/adrv9001_rx_rtl.xml

update_compile_order -fileset sources_1
update_compile_order -fileset sources_1

