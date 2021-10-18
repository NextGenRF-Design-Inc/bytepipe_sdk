
set project_name adrv9001_tx_tb

# Create Project
create_project $project_name vivado_sim -part xczu2cg-sbva484-1-e -force

# Add source Files
add_files -fileset sim_1 -norecurse src/adrv9001_tx_tb.v
add_files -norecurse src/adrv9001_tx.v
add_files -norecurse src/adrv9001_tx_clk.v
add_files -norecurse src/adrv9001_tx_serdes.v
add_files -norecurse src/adrv9001_serdes_unpack.v
add_files -norecurse src/adrv9001_tx_rtl.xml


update_compile_order -fileset sources_1
update_compile_order -fileset sources_1

