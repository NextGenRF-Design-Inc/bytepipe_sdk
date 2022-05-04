# ******************************************************************************************
# ******************************************************************************************
#              Copyright 2022 (c) NextGen RF Design. All rights reserved.
#
#        This core is distributed WITHOUT ANY WARRANTY; without even the implied 
#           warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# ******************************************************************************************
# ******************************************************************************************
PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: hdl
hdl: download unzip ip hdl_project

.PHONY: sw
sw: 

.PHONY: sd_card
sd_card:	
	cp -f BOOT.bin /media/ngrf/BOOT/
	cp -f adrv9001_zcu102_cmos.bin /media/ngrf/BOOT/	
	cp -f adrv9001_zcu102_lvds.bin /media/ngrf/BOOT/
	#cp -f axirw.pl /media/ngrf/BOOT/
	#cp -f axirw.regs /media/ngrf/BOOT/
	cp -f Image /media/ngrf/BOOT/
	cp -f system.dtb /media/ngrf/BOOT/
	sudo cp -f adrv9001_zcu102_cmos.bin /media/ngrf/rootfs/home/analog/adrv9001_server/resources/Adi.Adrv9001.FpgaBinaries/	
	sudo cp -f adrv9001_zcu102_lvds.bin /media/ngrf/rootfs/home/analog/adrv9001_server/resources/Adi.Adrv9001.FpgaBinaries/	
	sudo cp -f adrv9001_zcu102_cmos.bin /media/ngrf/rootfs/home/analog/adrv9001_server/resources/Adi.Adrv9001.FpgaBinaries/adrv9001_zcu102_cmos_8.2.21.bin
	sudo cp -f adrv9001_zcu102_lvds.bin /media/ngrf/rootfs/home/analog/adrv9001_server/resources/Adi.Adrv9001.FpgaBinaries/adrv9001_zcu102_lvds_8.2.21.bin
	sudo cp -f adrv9001_zcu102.h /media/ngrf/rootfs/home/analog/adrv9001_server/resources/Adi.Adrv9001.FpgaDrivers


hdl_project: | $(PROJECT)/vivado
$(PROJECT)/vivado:
	@echo 'Building $(shell printf '\033')[0;33m$(PROJECT)$(shell printf '\033')[0m hdl'
	vivado -mode batch -nojournal -quiet -source $(shell cygpath -w $(PROJECT_DIR))\hdl\make_hdl.tcl -notrace 
  
ip:
	make -C $(PROJECT)/library/axi_adrv9001/hdl/
	make -C $(PROJECT)/library/axi_mspi/hdl/ 
	make -C $(PROJECT)/library/axi_rxdma/hdl/
	make -C $(PROJECT)/library/axi_txdma/hdl/
	make -C $(PROJECT)/library/axi_sysid/hdl/  
  
clean:
	rm -rf $(PROJECT)

# Download adrv9001-sdk Note:  See ADI license agreement
download: | adrv9001-sdk-eval-software.zip
adrv9001-sdk-eval-software.zip:
	$ wget https://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/adrv9001-sdk-eval-software.zip

# Changes required to support bytepipe - Note:  See ADI license agreement  
unzip: | adrv9001-sdk
adrv9001-sdk:
	unzip adrv9001-sdk-eval-software.zip  
	cp -r adrv9001-sdk/pkg/prototype/platform/library adrv9001-sdk/library
	cp -r adrv9001-sdk/pkg/prototype/platform/bin adrv9001-sdk/bin
	chmod +x adrv9001-sdk/bin/tcl2make.pl 
	sed -i '652 a\    .SWAP_DIFF_IDATA                  (1),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001.v
	sed -i '653 a\    .SWAP_DIFF_STROBE                 (1),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001.v    
	sed -i '654 a\    .SWAP_DIFF_DCLK_IN                (1),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001.v
	sed -i '655 a\    .SWAP_DIFF_DCLK_OUT               (1),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001.v 
	sed -i '711 a\    .SWAP_DIFF_IDATA                  (1),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001.v
	sed -i '712 a\    .SWAP_DIFF_QDATA                  (1),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001.v
	sed -i '713 a\    .SWAP_DIFF_STROBE                 (1),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001.v  
	sed -i '23 a\  parameter SWAP_DIFF_IDATA = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v      
	sed -i '24 a\  parameter SWAP_DIFF_QDATA = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v     
	sed -i '25 a\  parameter SWAP_DIFF_STROBE = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v     
	sed -i '26 a\  parameter SWAP_DIFF_DCLK_IN = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v  
	sed -i 's/IBUFDS i_rx_sclock (/\    IBUFDS i_rx_sclock (/g' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v 
	sed -i 's/IBUF i_rx_sclock (/\  IBUF i_rx_sclock (/g' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v   
	sed -i 's/.I                                      (rx_cssi_clk_lssi_clk_p),/\    .I                                  (rx_cssi_clk_lssi_clk_p),/g' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v   
	sed -i 's/.IB                                     (rx_cssi_nc_lssi_clk_n),/\    .IB                                 (rx_cssi_nc_lssi_clk_n),/g' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v 
	sed -i 's/.O                                      (rx_buf_clk));/\    .O                                  (rx_buf_clk));/g' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v 	  
	sed -i '2217 a\    .SWAP_DIFF                              ( SWAP_DIFF_IDATA ),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v
	sed -i '2237 a\    .SWAP_DIFF                              ( SWAP_DIFF_QDATA ),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v  
	sed -i '2295 a\    .SWAP_DIFF                              ( SWAP_DIFF_STROBE ),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v  
	sed -i '2378 a\    if( SWAP_DIFF_DCLK_IN == 1) begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v    
	sed -i '2379 a\      IBUFDS i_rx_sclock (' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v   
	sed -i '2380 a\        .I                                  (rx_cssi_clk_lssi_clk_n),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v  
	sed -i '2381 a\        .IB                                 (rx_cssi_nc_lssi_clk_p),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v  
	sed -i '2382 a\        .O                                  (rx_buf_clk));' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v    
	sed -i '2383 a\    end else begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v 
	sed -i '2389 a\    end' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx.v 
	sed -i '22 a\  parameter SWAP_DIFF = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v  
	sed -i '98 a\generate' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v  
	sed -i '99 a\if( SWAP_DIFF == 1 ) begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '100 a\  always @(negedge rx_resetn or posedge rx_clk) begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '101 a\    if (rx_resetn == 1\x27b0) begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '102 a\      rx_mdata_d <= 8\x27d0;' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '103 a\      rx_pdata <= 8\x27d0;' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '104 a\    end else begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '105 a\      rx_mdata_d <= rx_mdata;' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '106 a\      rx_pdata <= ~rx_mdata;' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '107 a\    end' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '108 a\  end' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v    
	sed -i '109 a\end else begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '128 a\end' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v    
	sed -i '129 a\endgenerate' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v   
	sed -i '309 a\    if( SWAP_DIFF == 1 ) begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v  
	sed -i '310 a\      IBUFDS i_ibuf (' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v  
	sed -i '311 a\        .I                      (rx_data_n),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v  
	sed -i '312 a\        .IB                     (rx_data_p),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v  
	sed -i '313 a\        .O                      (rx_sdata));  ' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v  
	sed -i '314 a\    end else begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v  
	sed -i '319 a\    end' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_rx_if.v      
	sed -i '23 a\  parameter SWAP_DIFF_IDATA = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx.v      
	sed -i '24 a\  parameter SWAP_DIFF_QDATA = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx.v     
	sed -i '25 a\  parameter SWAP_DIFF_STROBE = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx.v     
	sed -i '26 a\  parameter SWAP_DIFF_DCLK_IN = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx.v  
	sed -i '27 a\  parameter SWAP_DIFF_DCLK_OUT = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx.v    
	sed -i '743 a\    .SWAP_DIFF                              ( SWAP_DIFF_IDATA ),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx.v  
	sed -i '762 a\    .SWAP_DIFF                              ( SWAP_DIFF_QDATA ),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx.v 
	sed -i '817 a\    .SWAP_DIFF                              ( SWAP_DIFF_STROBE ),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx.v   
	sed -i '836 a\    .SWAP_DIFF_CLK_IN                       ( SWAP_DIFF_DCLK_IN ), ' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx.v  
	sed -i '837 a\    .SWAP_DIFF_CLK_OUT                      ( SWAP_DIFF_DCLK_OUT ),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx.v  
	sed -i '23 a\  parameter SWAP_DIFF_CLK_IN = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_clk.v   
	sed -i '24 a\  parameter SWAP_DIFF_CLK_OUT = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_clk.v     
	sed -i '251 a\    .SWAP_DIFF                              ( SWAP_DIFF_CLK_OUT ),  ' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_clk.v    
	sed -i '390 a\     if( SWAP_DIFF_CLK_IN == 1) begin  ' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_clk.v    
	sed -i '391 a\       IBUFDS i_ibuf (' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_clk.v    
	sed -i '392 a\         .I                                      (tx_ref_clk_n),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_clk.v    
	sed -i '393 a\         .IB                                     (tx_ref_clk_p),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_clk.v    
	sed -i '394 a\         .O                                      (tx_hs_ref_clk));' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_clk.v    
	sed -i '395 a\     end else begin ' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_clk.v   
	sed -i '400 a\     end' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_clk.v 
	sed -i '22 a\  parameter SWAP_DIFF = 0,' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v    
	sed -i '89 a\generate' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v  
	sed -i '90 a\if( SWAP_DIFF == 1 ) begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '91 a\  always @(negedge tx_resetn or posedge tx_clk) begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '92 a\    if (tx_resetn == 1\x27b0) begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '93 a\      tx_pdata_d <= 8\x27d0;' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '94 a\      tx_mdata <= 8\x27d0;' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '95 a\    end else begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '96 a\      tx_pdata_d <= tx_pdata;' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '97 a\      tx_mdata <= ~tx_pdata;' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '98 a\    end' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '99 a\ end' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v    
	sed -i '100 a\end else begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '121 a\end' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v    
	sed -i '122 a\endgenerate' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v   
	sed -i '272 a\    if( SWAP_DIFF == 1 ) begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v  
	sed -i '273 a\      OBUFTDS i_obuf (' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v  
	sed -i '274 a\        .T                  (tx_tdata),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v  
	sed -i '275 a\        .I                  (tx_sdata),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v  
	sed -i '276 a\        .O                  (tx_data_n),' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v  
	sed -i '277 a\        .OB                 (tx_data_p));' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v  
	sed -i '278 a\    end else begin' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v    
	sed -i '285 a\    end' adrv9001-sdk/library/axi_adrv9001/hdl/axi_adrv9001_tx_if.v  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  