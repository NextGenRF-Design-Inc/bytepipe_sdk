`timescale 1ns / 1ps
/***************************************************************************//**
 *  \file       axi_adrv9001.v
 *
 *  \details
 *
 *  \copyright
 *
 *  Copyright 2021(c) NextGen RF Design, Inc.  
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *   - Use of the software either in source or binary form, must be run on or
 *     directly connected to a NextGen RF Design, Inc. product.
 *
 *  THIS SOFTWARE IS PROVIDED BY NEXTGEN RF DESIGN "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 *  EVENT SHALL NEXTGEN RF DESIGN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************/
 
module axi_adrv9001#(
  parameter DEFAULT_DGPIO_DIR       = 16'hffff,
  parameter SWAP_DIFF_TX1_IDATA     = 1,        
  parameter SWAP_DIFF_TX1_QDATA     = 0,       
  parameter SWAP_DIFF_TX1_STROBE    = 1,       
  parameter SWAP_DIFF_TX1_DCLK_IN   = 1,        
  parameter SWAP_DIFF_TX1_DCLK_OUT  = 1, 
  parameter SWAP_DIFF_TX2_IDATA     = 0,        
  parameter SWAP_DIFF_TX2_QDATA     = 0,       
  parameter SWAP_DIFF_TX2_STROBE    = 0,       
  parameter SWAP_DIFF_TX2_DCLK_IN   = 0,        
  parameter SWAP_DIFF_TX2_DCLK_OUT  = 0,     
  parameter SWAP_DIFF_RX1_IDATA     = 0,      
  parameter SWAP_DIFF_RX1_QDATA     = 0,    
  parameter SWAP_DIFF_RX1_STROBE    = 0,       
  parameter SWAP_DIFF_RX1_DCLK      = 0,  
  parameter SWAP_DIFF_RX2_IDATA     = 1,      
  parameter SWAP_DIFF_RX2_QDATA     = 1,    
  parameter SWAP_DIFF_RX2_STROBE    = 1,       
  parameter SWAP_DIFF_RX2_DCLK      = 0,
  parameter ENABLE_PL_DGPIO         = 0,
  parameter ENABLE_PL_RX1_ENABLE    = 0,   
  parameter ENABLE_PL_RX2_ENABLE    = 0,   
  parameter ENABLE_PL_TX1_ENABLE    = 0,   
  parameter ENABLE_PL_TX2_ENABLE    = 0,
  parameter ENABLE_RX1_AXIS         = 1,
  parameter ENABLE_RX2_AXIS         = 1,
  parameter ENABLE_TX1_AXIS         = 1,
  parameter ENABLE_TX2_AXIS         = 1,  
  parameter ENABLE_OTX1_AXIS        = 1,  
  parameter ENABLE_OTX2_AXIS        = 1,
  parameter ENABLE_HOP_TRIG_PIN     = 0,
  parameter ENABLE_HOP_IRQ          = 0,  
  parameter ENABLE_HOPPING_SUPPORT  = 0,
  parameter ENABLE_HOPPING_DEBUG    = 0,
  parameter ENABLE_SPI_DEBUG        = 0,
  parameter ENABLE_DEV_CLOCK_OUT    = 0,
  parameter TX1_REF_CLK_USE_RX1     = 0,
  parameter TX2_REF_CLK_USE_RX2     = 0
  )(
  input  wire           dev_clk_in,
  output wire           dev_clk,
  output reg            rx1_en = 'd0,
  output reg            rx2_en = 'd0,
  output reg            tx1_en = 'd0,
  output reg            tx2_en = 'd0,
  output wire           rstn,
  input  wire           irq,
  output wire [15:0]    dgpio_o,
  input  wire [15:0]    dgpio_i,
  output wire [15:0]    dgpio_t,
  input  wire           hop_trig,
      
  input  wire           rx1_dclk_p,          
  input  wire           rx1_dclk_n,       
  input  wire           rx1_strobe_p, 
  input  wire           rx1_strobe_n,    
  input  wire           rx1_idata_p, 
  input  wire           rx1_idata_n,      
  input  wire           rx1_qdata_p,  
  input  wire           rx1_qdata_n,     
  
  output wire [31:0]    rx1_axis_tdata,    
  output wire           rx1_axis_tvalid, 
  output wire           rx1_axis_aclk, 
    
  input  wire           rx2_dclk_p,          
  input  wire           rx2_dclk_n,       
  input  wire           rx2_strobe_p, 
  input  wire           rx2_strobe_n,    
  input  wire           rx2_idata_p, 
  input  wire           rx2_idata_n,      
  input  wire           rx2_qdata_p,  
  input  wire           rx2_qdata_n,     
  
  output wire [31:0]    rx2_axis_tdata,    
  output wire           rx2_axis_tvalid,  
  output wire           rx2_axis_aclk, 

  input  wire           tx1_ref_clk_p,     
  input  wire           tx1_ref_clk_n,       
  output wire           tx1_dclk_p,  
  output wire           tx1_dclk_n,      
  output wire           tx1_strobe_p,   
  output wire           tx1_strobe_n,    
  output wire           tx1_idata_p,     
  output wire           tx1_idata_n,       
  output wire           tx1_qdata_p,    
  output wire           tx1_qdata_n, 
   
  input  wire [31:0]    tx1_axis_tdata,  
  output wire           tx1_axis_tready,    
  input  wire           tx1_axis_tvalid,   
  output wire           tx1_axis_aclk,  

  input  wire           tx2_ref_clk_p,     
  input  wire           tx2_ref_clk_n,  
  output wire           tx2_dclk_p,  
  output wire           tx2_dclk_n,      
  output wire           tx2_strobe_p,   
  output wire           tx2_strobe_n,    
  output wire           tx2_idata_p,     
  output wire           tx2_idata_n,       
  output wire           tx2_qdata_p,    
  output wire           tx2_qdata_n, 
  
  input  wire [31:0]    tx2_axis_tdata,  
  output wire           tx2_axis_tready,    
  input  wire           tx2_axis_tvalid,   
  output wire           tx2_axis_aclk,
  
  input  wire           rx1_pl_en,
  input  wire           rx2_pl_en,
  input  wire           tx1_pl_en,
  input  wire           tx2_pl_en,  
  
  input  wire [15:0]    dgpio_pl_o,
  output wire [15:0]    dgpio_pl_i,
    
  output wire           spi_clk,
  input  wire           spi_miso,
  output wire           spi_mosi,
  output wire           spi_csn,
  output wire           spi_irq,  
  
  output wire           pl_irq,   
  output reg            hop_irq = 'd0,
  
  output wire [31:0]    otx1_axis_tdata,
  output wire           otx1_axis_tvalid,
  
  output wire [31:0]    otx2_axis_tdata,
  output wire           otx2_axis_tvalid,    
  
  input  wire           s_axi_aclk,
  input  wire           s_axi_aresetn,
  input  wire [6:0]     s_axi_awaddr,
  input  wire [2:0]     s_axi_awprot,
  input  wire           s_axi_awvalid,
  output wire           s_axi_awready,
  input  wire [31:0]    s_axi_wdata,   
  input  wire [3:0]     s_axi_wstrb,
  input  wire           s_axi_wvalid,
  output wire           s_axi_wready,
  output wire [1:0]     s_axi_bresp,
  output wire           s_axi_bvalid,
  input  wire           s_axi_bready,
  input  wire [6:0]     s_axi_araddr,
  input  wire [2:0]     s_axi_arprot,
  input  wire           s_axi_arvalid,
  output wire           s_axi_arready,
  output wire [31:0]    s_axi_rdata,
  output wire [1:0]     s_axi_rresp,
  output wire           s_axi_rvalid,
  input  wire           s_axi_rready,
  
  output wire           HopDbg_HopPin,
  output wire           HopDbg_Rx1EnPin,
  output wire           HopDbg_Rx2EnPin,
  output wire           HopDbg_Tx1EnPin,
  output wire           HopDbg_Tx2EnPin,
  output wire           HopDbg_HopIrq,
  output wire           HopDbg_HopIrqClear,
  output wire           HopDbg_HopTrig,
  
  output wire           SpiDbg_Sclk,
  output wire           SpiDbg_Csn,
  output wire           SpiDbg_Mosi,
  output wire           SpiDbg_Miso
  
);

generate

  if( ENABLE_DEV_CLOCK_OUT ) begin
  
    assign dev_clk = dev_clk_in;
  
  end else begin
  
    assign dev_clk = 0;
  
  end

endgenerate

assign pl_irq = irq;

/**** Dgpio ***************************************************************************************/

reg           hop_dgpio = 'd0;
wire [15:0]   dgpio_ps_t;
wire [15:0]   dgpio_ps_o;
wire [15:0]   dgpio_ps_i;

assign dgpio_t = dgpio_ps_t;
assign dgpio_ps_i = dgpio_i;

generate

  if( ENABLE_PL_DGPIO ) begin
    assign dgpio_o[15] = dgpio_ps_o[15] | dgpio_pl_o[15];
    assign dgpio_o[14] = dgpio_ps_o[14] | dgpio_pl_o[14];
    assign dgpio_o[13] = dgpio_ps_o[13] | dgpio_pl_o[13];
    assign dgpio_o[12] = dgpio_ps_o[12] | dgpio_pl_o[12];
    assign dgpio_o[11] = dgpio_ps_o[11] | dgpio_pl_o[11];
    assign dgpio_o[10] = dgpio_ps_o[10] | dgpio_pl_o[10];
    assign dgpio_o[9] = dgpio_ps_o[9] | dgpio_pl_o[9];
    assign dgpio_o[8] = dgpio_ps_o[8] | dgpio_pl_o[8];
    assign dgpio_o[7] = dgpio_ps_o[7] | dgpio_pl_o[7];
    assign dgpio_o[6] = dgpio_ps_o[6] | dgpio_pl_o[6];
    assign dgpio_o[5] = dgpio_ps_o[5] | dgpio_pl_o[5]; 
    assign dgpio_o[4] = dgpio_ps_o[4] | dgpio_pl_o[4];
    assign dgpio_o[3] = dgpio_ps_o[3] | dgpio_pl_o[3];
    assign dgpio_o[2] = dgpio_ps_o[2] | dgpio_pl_o[2];
    assign dgpio_o[1] = dgpio_ps_o[1] | dgpio_pl_o[1];
    assign dgpio_o[0] = dgpio_ps_o[0] | dgpio_pl_o[0] | hop_dgpio;     
    assign dgpio_pl_i = dgpio_i;
  end else begin
    assign dgpio_o[15] = dgpio_ps_o[15];
    assign dgpio_o[14] = dgpio_ps_o[14];
    assign dgpio_o[13] = dgpio_ps_o[13];
    assign dgpio_o[12] = dgpio_ps_o[12];
    assign dgpio_o[11] = dgpio_ps_o[11];
    assign dgpio_o[10] = dgpio_ps_o[10];
    assign dgpio_o[9] = dgpio_ps_o[9];
    assign dgpio_o[8] = dgpio_ps_o[8];
    assign dgpio_o[7] = dgpio_ps_o[7];
    assign dgpio_o[6] = dgpio_ps_o[6];
    assign dgpio_o[5] = dgpio_ps_o[5]; 
    assign dgpio_o[4] = dgpio_ps_o[4];
    assign dgpio_o[3] = dgpio_ps_o[3];
    assign dgpio_o[2] = dgpio_ps_o[2];
    assign dgpio_o[1] = dgpio_ps_o[1];
    assign dgpio_o[0] = dgpio_ps_o[0] | hop_dgpio;  
    assign dgpio_pl_i = 'h0;
  end     
  
endgenerate


/**** Enable ***************************************************************************************/

wire tx1_ps_en;
wire tx2_ps_en;
wire rx1_ps_en;
wire rx2_ps_en;
wire tx1_enable;
wire tx2_enable;
wire rx1_enable;
wire rx2_enable;

generate
 
  if( ENABLE_PL_RX1_ENABLE ) begin
    assign rx1_enable = rx1_pl_en | rx1_ps_en;
  end else begin
    assign rx1_enable = rx1_ps_en;     
  end  
  
  if( ENABLE_PL_RX2_ENABLE ) begin
    assign rx2_enable = rx2_pl_en | rx2_ps_en;
  end else begin
    assign rx2_enable = rx2_ps_en;     
  end  

  if( ENABLE_PL_TX1_ENABLE ) begin
    assign tx1_enable = tx1_pl_en | tx1_ps_en;
  end else begin
    assign tx1_enable = tx1_ps_en;     
  end  
  
  if( ENABLE_PL_TX2_ENABLE ) begin
    assign tx2_enable = tx2_pl_en | tx2_ps_en;
  end else begin
    assign tx2_enable = tx2_ps_en;     
  end     
  
endgenerate


/**** Hopping ***************************************************************************************/

wire          hop_mode;
wire [23:0]   rx1_hop_setup_delay_cnt;
wire [23:0]   rx2_hop_setup_delay_cnt;
wire [23:0]   tx1_hop_setup_delay_cnt;
wire [23:0]   tx2_hop_setup_delay_cnt;
wire [23:0]   hop_dgpio_delay_cnt;
wire [3:0]    next_hop_enable_mask;
wire          hop_ps_trig;  
wire          enable_pl_hop_trig;
wire          rx1_ps_setup;
wire          rx2_ps_setup;
wire          tx1_ps_setup;
wire          tx2_ps_setup;
wire          hop_trig_enable;
wire          hop_trig_clear;
wire          hop_trig_rising;
  
generate

if( ENABLE_HOPPING_SUPPORT ) begin

  reg           rx1_setup = 'd0;
  reg           rx2_setup = 'd0;
  reg           tx1_setup = 'd0;
  reg           tx2_setup = 'd0;
  reg           hop_trig_reg0 = 0;
  reg           hop_trig_reg1 = 0;
  reg  [23:0]   hop_delay_cnt = 'd0;
  reg           hop_pl_trig = 'd0;
  
  assign hop_trig_rising = hop_trig_reg0 & ~hop_trig_reg1;
  
  always @(posedge s_axi_aclk) begin

    if( ENABLE_HOP_TRIG_PIN )
      hop_pl_trig <= hop_trig & enable_pl_hop_trig;
    else
      hop_pl_trig <= 'd0;
    
    hop_trig_reg0 <= hop_pl_trig | hop_ps_trig;
      
    hop_trig_reg1 <= hop_trig_reg0;
    
    if( hop_trig_rising )
      hop_delay_cnt <= 'd0;
    else if( hop_delay_cnt < 24'hfffffE )
      hop_delay_cnt <= hop_delay_cnt + 'd1;
    else
      hop_delay_cnt <= hop_delay_cnt;
      
    if( hop_delay_cnt == hop_dgpio_delay_cnt )
      hop_dgpio <= ~hop_dgpio;
    else
      hop_dgpio <= hop_dgpio;
      
    if( hop_trig_clear )
      hop_irq <= 1'b0;
    else if( (hop_delay_cnt == hop_dgpio_delay_cnt) && hop_trig_enable )
      hop_irq <= 1'b1;
    else
      hop_irq <= hop_irq;    
                  
    if( rx1_ps_setup )
      rx1_setup <= 1'b1;        
    else if( hop_delay_cnt == 'd1 )
      rx1_setup <= next_hop_enable_mask[0];
    else if( hop_delay_cnt == rx1_hop_setup_delay_cnt )
      rx1_setup <= 'd0;      
    else
      rx1_setup <= rx1_setup;
      
    if( rx2_ps_setup )
      rx2_setup <= 1'b1;        
    else if( hop_delay_cnt == 'd1 )
      rx2_setup <= next_hop_enable_mask[1];
    else if( hop_delay_cnt == rx2_hop_setup_delay_cnt )
      rx2_setup <= 'd0;      
    else
      rx2_setup <= rx2_setup;
      
    if( tx1_ps_setup )
      tx1_setup <= 1'b1;        
    else if( hop_delay_cnt == 'd1 )
      tx1_setup <= next_hop_enable_mask[2];
    else if( hop_delay_cnt == tx1_hop_setup_delay_cnt )
      tx1_setup <= 'd0;      
    else
      tx1_setup <= tx1_setup;
      
    if( tx2_ps_setup )
      tx2_setup <= 1'b1;    
    else if( hop_delay_cnt == 'd1 )
      tx2_setup <= next_hop_enable_mask[3];
    else if( hop_delay_cnt == tx2_hop_setup_delay_cnt )
      tx2_setup <= 'd0;      
    else
      tx2_setup <= tx2_setup;      
    
    if( hop_mode )
      rx1_en <= rx1_setup;
    else
      rx1_en <= rx1_enable;
      
    if( hop_mode )
      rx2_en <= rx2_setup;
    else
      rx2_en <= rx2_enable;

    if( hop_mode )
      tx1_en <= tx1_setup;
    else
      tx1_en <= tx1_enable;
      
    if( hop_mode )
      tx2_en <= tx2_setup;
    else
      tx2_en <= tx2_enable;    
      
  end      
  
end else begin
  
  always @(posedge s_axi_aclk) begin

    rx1_en <= rx1_enable;     
    rx2_en <= rx2_enable;  
    tx1_en <= tx1_enable;     
    tx2_en <= tx2_enable;   

  end
  
  assign hop_trig_rising = 0;  

end
endgenerate


/**** Swap IQ ***************************************************************************************/

wire          tx1_swap_iq;
wire          tx2_swap_iq;
wire          rx1_swap_iq;
wire          rx2_swap_iq;
wire          tx1_swap_iq_cdc;
wire          tx2_swap_iq_cdc;
wire          rx1_swap_iq_cdc;
wire          rx2_swap_iq_cdc;

xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
tx1_swap_iq_cdc_i (
  .dest_out(tx1_swap_iq_cdc), 
  .dest_clk(tx1_axis_aclk), 
  .src_clk(s_axi_aclk), 
  .src_in(tx1_swap_iq) 
);

xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
tx2_swap_iq_cdc_i (
  .dest_out(tx2_swap_iq_cdc), 
  .dest_clk(tx2_axis_aclk), 
  .src_clk(s_axi_aclk), 
  .src_in(tx2_swap_iq) 
);

xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
rx1_swap_iq_cdc_i (
  .dest_out(rx1_swap_iq_cdc), 
  .dest_clk(rx1_axis_aclk), 
  .src_clk(s_axi_aclk), 
  .src_in(rx1_swap_iq) 
);

xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
rx2_swap_iq_cdc_i (
  .dest_out(rx2_swap_iq_cdc), 
  .dest_clk(rx2_axis_aclk), 
  .src_clk(s_axi_aclk), 
  .src_in(rx2_swap_iq) 
);

/**** Pattern detection ***************************************************************************************/

wire rx1_ramp_detected;
wire rx2_ramp_detected;
wire rx1_ramp_detected_cdc;
wire rx2_ramp_detected_cdc;
wire rx1_pn15_detected;
wire rx2_pn15_detected;
wire rx1_pn15_detected_cdc;
wire rx2_pn15_detected_cdc;
wire rx1_fixed_detected;
wire rx2_fixed_detected;
wire rx1_fixed_detected_cdc;
wire rx2_fixed_detected_cdc;

xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
rx1_ramp_detected_cdc_i (
  .dest_out(rx1_ramp_detected_cdc), 
  .dest_clk(s_axi_aclk), 
  .src_clk(rx1_axis_aclk), 
  .src_in(rx1_ramp_detected) 
);

xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
rx2_ramp_detected_cdc_i (
  .dest_out(rx2_ramp_detected_cdc), 
  .dest_clk(s_axi_aclk), 
  .src_clk(rx2_axis_aclk), 
  .src_in(rx2_ramp_detected) 
);

xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
rx1_pn15_detected_cdc_i (
  .dest_out(rx1_pn15_detected_cdc), 
  .dest_clk(s_axi_aclk), 
  .src_clk(rx1_axis_aclk), 
  .src_in(rx1_pn15_detected) 
);

xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
rx2_pn15_detected_cdc_i (
  .dest_out(rx2_pn15_detected_cdc), 
  .dest_clk(s_axi_aclk), 
  .src_clk(rx2_axis_aclk), 
  .src_in(rx2_pn15_detected) 
);

xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
rx1_fixed_detected_cdc_i (
  .dest_out(rx1_fixed_detected_cdc), 
  .dest_clk(s_axi_aclk), 
  .src_clk(rx1_axis_aclk), 
  .src_in(rx1_fixed_detected) 
);

xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
rx2_fixed_detected_cdc_i (
  .dest_out(rx2_fixed_detected_cdc), 
  .dest_clk(s_axi_aclk), 
  .src_clk(rx2_axis_aclk), 
  .src_in(rx2_fixed_detected) 
);

/**** Fixed Pattern ***************************************************************************************/

wire [31:0]   rx1_fixed_pattern;
wire [31:0]   rx2_fixed_pattern;
wire [31:0]   tx1_fixed_pattern;
wire [31:0]   tx2_fixed_pattern;
wire [31:0]   rx1_fixed_pattern_cdc;
wire [31:0]   rx2_fixed_pattern_cdc;
wire [31:0]   tx1_fixed_pattern_cdc;
wire [31:0]   tx2_fixed_pattern_cdc;

xpm_cdc_array_single #(
  .DEST_SYNC_FF(3),  
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1),  
  .WIDTH(32)         
)
rx1_fixed_pattern_cdc_i (
  .dest_out(rx1_fixed_pattern_cdc),
  .dest_clk(rx1_axis_aclk),
  .src_clk(s_axi_aclk), 
  .src_in(rx1_fixed_pattern) 
);

xpm_cdc_array_single #(
  .DEST_SYNC_FF(3),  
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1),  
  .WIDTH(32)         
)
rx2_fixed_pattern_cdc_i (
  .dest_out(rx2_fixed_pattern_cdc),
  .dest_clk(rx2_axis_aclk),
  .src_clk(s_axi_aclk), 
  .src_in(rx2_fixed_pattern) 
);

xpm_cdc_array_single #(
  .DEST_SYNC_FF(3),  
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1),  
  .WIDTH(32)         
)
tx1_fixed_pattern_cdc_i (
  .dest_out(tx1_fixed_pattern_cdc),
  .dest_clk(tx1_axis_aclk),
  .src_clk(s_axi_aclk), 
  .src_in(tx1_fixed_pattern) 
);

xpm_cdc_array_single #(
  .DEST_SYNC_FF(3),  
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1),  
  .WIDTH(32)         
)
tx2_fixed_pattern_cdc_i (
  .dest_out(tx2_fixed_pattern_cdc),
  .dest_clk(tx2_axis_aclk),
  .src_clk(s_axi_aclk), 
  .src_in(tx2_fixed_pattern) 
);


/**** Transmit Data Source ***************************************************************************************/

wire [2:0]    tx1_data_src;
wire [2:0]    tx2_data_src; 
wire [2:0]    tx1_data_src_cdc;
wire [2:0]    tx2_data_src_cdc; 

xpm_cdc_array_single #(
  .DEST_SYNC_FF(3),  
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1),  
  .WIDTH(3)         
)
tx1_data_src_cdc_i (
  .dest_out(tx1_data_src_cdc),
  .dest_clk(tx1_axis_aclk),
  .src_clk(s_axi_aclk), 
  .src_in(tx1_data_src) 
);

xpm_cdc_array_single #(
  .DEST_SYNC_FF(3),  
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1),  
  .WIDTH(3)         
)
tx2_data_src_cdc_i (
  .dest_out(tx2_data_src_cdc),
  .dest_clk(tx2_axis_aclk),
  .src_clk(s_axi_aclk), 
  .src_in(tx2_data_src) 
);

/**** SPI ***************************************************************************************/

wire [7:0]    mspi_axis_tdata;
wire          mspi_axis_tvalid;
wire          mspi_axis_tready;  
wire [7:0]    sspi_axis_tdata;
wire          sspi_axis_tvalid;
wire          sspi_axis_tready;
wire          mspi_axis_enable;

adrv9001_mspi adrv9001_mspi_inst (
  .clk(s_axi_aclk),                
  .spi_clk(spi_clk),
  .spi_miso(spi_miso),       
  .spi_mosi(spi_mosi),                 
  .spi_csn(spi_csn),
  .spi_done(spi_irq),
  .s_axis_tdata(mspi_axis_tdata),
  .s_axis_tvalid(mspi_axis_tvalid),
  .s_axis_tready(mspi_axis_tready),  
  .s_axis_enable(mspi_axis_enable),
  .m_axis_tdata(sspi_axis_tdata),
  .m_axis_tvalid(sspi_axis_tvalid),
  .m_axis_tready(sspi_axis_tready)
);
   
/**** Registers ***************************************************************************************/
   
adrv9001_regs#(
    .DEFAULT_DGPIO_DIR(DEFAULT_DGPIO_DIR)
    )
    adrv9001_regs_i (
    .s_axi_aclk(s_axi_aclk),
    .s_axi_aresetn(s_axi_aresetn),
    .s_axi_awaddr(s_axi_awaddr),
    .s_axi_awprot(s_axi_awprot),
    .s_axi_awvalid(s_axi_awvalid),
    .s_axi_awready(s_axi_awready),
    .s_axi_wdata(s_axi_wdata),
    .s_axi_wstrb(s_axi_wstrb),
    .s_axi_wvalid(s_axi_wvalid),
    .s_axi_wready(s_axi_wready),
    .s_axi_bresp(s_axi_bresp),
    .s_axi_bvalid(s_axi_bvalid),
    .s_axi_bready(s_axi_bready),
    .s_axi_araddr(s_axi_araddr),
    .s_axi_arprot(s_axi_arprot),
    .s_axi_arvalid(s_axi_arvalid),
    .s_axi_arready(s_axi_arready),
    .s_axi_rdata(s_axi_rdata),
    .s_axi_rresp(s_axi_rresp),
    .s_axi_rvalid(s_axi_rvalid),
    .s_axi_rready(s_axi_rready),
    
    .tx1_enable(tx1_ps_en),
    .tx2_enable(tx2_ps_en),
    .rx1_enable(rx1_ps_en),
    .rx2_enable(rx2_ps_en),        
    
    .hopping_mode(hop_mode),
    .rx1_hop_setup_delay_cnt(rx1_hop_setup_delay_cnt),
    .rx2_hop_setup_delay_cnt(rx2_hop_setup_delay_cnt),
    .tx1_hop_setup_delay_cnt(tx1_hop_setup_delay_cnt),
    .tx2_hop_setup_delay_cnt(tx2_hop_setup_delay_cnt),
    .hop_dgpio_delay_cnt(hop_dgpio_delay_cnt),

    .rx1_ps_setup(rx1_ps_setup),
    .rx2_ps_setup(rx2_ps_setup),
    .tx1_ps_setup(tx1_ps_setup),    
    .tx2_ps_setup(tx2_ps_setup),
    
    .hop_trig(hop_ps_trig),
    .next_hop_enable_mask(next_hop_enable_mask),
    .enable_pl_hop_trig(enable_pl_hop_trig),
    .hop_trig_enable(hop_trig_enable),
    .hop_trig_clear(hop_trig_clear),
    .hop_trig_status(hop_irq),
    
    .tx1_swap_iq(tx1_swap_iq),
    .tx2_swap_iq(tx2_swap_iq),
    .rx1_swap_iq(rx1_swap_iq),
    .rx2_swap_iq(rx2_swap_iq),
        
    .tx1_data_src(tx1_data_src),
    .tx2_data_src(tx2_data_src),
    
    .rx1_ramp_detected(rx1_ramp_detected_cdc),
    .rx2_ramp_detected(rx2_ramp_detected_cdc),
    .rx1_pn15_detected(rx1_pn15_detected_cdc),
    .rx2_pn15_detected(rx2_pn15_detected_cdc),    
    .rx1_fixed_detected(rx1_fixed_detected_cdc),   
    .rx2_fixed_detected(rx2_fixed_detected_cdc),       
    
    .rx1_fixed_pattern(rx1_fixed_pattern),        
    .rx2_fixed_pattern(rx2_fixed_pattern),   
    .tx1_fixed_pattern(tx1_fixed_pattern),        
    .tx2_fixed_pattern(tx2_fixed_pattern),       
    
    .adrv9001_rstn(rstn),
    
    .dgpio_ps_t(dgpio_ps_t),
    .dgpio_ps_i(dgpio_ps_i),
    .dgpio_ps_o(dgpio_ps_o),    
            
    .mspi_axis_tdata(mspi_axis_tdata),
    .mspi_axis_tvalid(mspi_axis_tvalid),
    .mspi_axis_tready(mspi_axis_tready),  
    .mspi_axis_enable(mspi_axis_enable),
    .sspi_axis_tdata(sspi_axis_tdata),
    .sspi_axis_tvalid(sspi_axis_tvalid),
    .sspi_axis_tready(sspi_axis_tready)    
);
    
/**** Rx ***************************************************************************************/
    
wire rx1_ssi_clk_div;
wire rx1_ssi_clk;
    
adrv9001_clkin#(
  .SWAP_DIFF_REF_CLK( SWAP_DIFF_RX1_DCLK )
) adrv9001_rx1_clkin (   
  .ref_clk_p(rx1_dclk_p),       
  .ref_clk_n(rx1_dclk_n), 
  .clk_out(rx1_ssi_clk),
  .clk_out_div(rx1_ssi_clk_div)
);
    
adrv9001_rx#(
    .SWAP_DIFF_IDATA(SWAP_DIFF_RX1_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_RX1_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_RX1_STROBE)
) adrv9001_rx1_inst (
    .ssi_clk(rx1_ssi_clk),
    .ssi_clk_div(rx1_ssi_clk_div), 
    
    .adrv9001_rx_strobe_p(rx1_strobe_p),
    .adrv9001_rx_strobe_n(rx1_strobe_n),
    .adrv9001_rx_idata_p(rx1_idata_p),
    .adrv9001_rx_idata_n(rx1_idata_n),
    .adrv9001_rx_qdata_p(rx1_qdata_p),
    .adrv9001_rx_qdata_n(rx1_qdata_n),
    
    .enable(rx1_enable),           
    .swap_iq(rx1_swap_iq_cdc),
    .fixed_pattern(rx1_fixed_pattern_cdc),     
    .ramp_detected_out(rx1_ramp_detected),
    .pn15_detected_out(rx1_pn15_detected), 
    .fixed_detected_out(rx1_fixed_detected),   
  
    .m_axis_tdata(rx1_axis_tdata),
    .m_axis_tvalid(rx1_axis_tvalid),   
    .m_axis_aclk(rx1_axis_aclk)
);
    
wire rx2_ssi_clk_div;
wire rx2_ssi_clk;
    
adrv9001_clkin#(
  .SWAP_DIFF_REF_CLK( SWAP_DIFF_RX2_DCLK )
) adrv9001_rx2_clkin (   
  .ref_clk_p(rx2_dclk_p),       
  .ref_clk_n(rx2_dclk_n), 
  .clk_out(rx2_ssi_clk),
  .clk_out_div(rx2_ssi_clk_div)
);
    
adrv9001_rx #(
    .SWAP_DIFF_IDATA(SWAP_DIFF_RX2_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_RX2_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_RX2_STROBE)
) adrv9001_rx2_inst (
    .ssi_clk(rx1_ssi_clk),
    .ssi_clk_div(rx1_ssi_clk_div), 
    
    .adrv9001_rx_strobe_p(rx2_strobe_p),
    .adrv9001_rx_strobe_n(rx2_strobe_n),
    .adrv9001_rx_idata_p(rx2_idata_p),
    .adrv9001_rx_idata_n(rx2_idata_n),
    .adrv9001_rx_qdata_p(rx2_qdata_p),
    .adrv9001_rx_qdata_n(rx2_qdata_n),
    
    .enable(rx2_enable),       
    .swap_iq(rx2_swap_iq_cdc),    
    .fixed_pattern(rx2_fixed_pattern_cdc),     
    .ramp_detected_out(rx2_ramp_detected),
    .pn15_detected_out(rx2_pn15_detected),     
    .fixed_detected_out(rx2_fixed_detected),           
    
    .m_axis_tdata(rx2_axis_tdata),
    .m_axis_tvalid(rx2_axis_tvalid),
    .m_axis_aclk(rx2_axis_aclk) 
);  

/**** Tx ***************************************************************************************/

wire tx1_ssi_clk;
wire tx1_ssi_clk_div;

generate

  if( TX1_REF_CLK_USE_RX1 ) begin
    
    assign tx1_ssi_clk = rx1_ssi_clk;
    assign tx1_ssi_clk_div = rx1_ssi_clk_div;  
  
  end else begin
  
    adrv9001_clkin#(
      .SWAP_DIFF_REF_CLK( SWAP_DIFF_TX1_DCLK_IN )
    ) adrv9001_tx1_clkin (   
     .ref_clk_p(tx1_ref_clk_p),       
     .ref_clk_n(tx1_ref_clk_n), 
     .clk_out(tx1_ssi_clk),
     .clk_out_div(tx1_ssi_clk_div)
    );    
   
  end
endgenerate

adrv9001_tx #(
    .SWAP_DIFF_IDATA(SWAP_DIFF_TX1_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_TX1_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_TX1_STROBE),
    .SWAP_DIFF_CLK_OUT(SWAP_DIFF_TX1_DCLK_OUT),
    .ENABLE_OTX_AXIS(ENABLE_OTX1_AXIS)
) adrv9001_tx1_inst (
    .ssi_clk(tx1_ssi_clk),
    .ssi_clk_div(tx1_ssi_clk_div), 
    
    .adrv9001_tx_dclk_p(tx1_dclk_p),
    .adrv9001_tx_dclk_n(tx1_dclk_n),
    .adrv9001_tx_strobe_p(tx1_strobe_p),
    .adrv9001_tx_strobe_n(tx1_strobe_n),
    .adrv9001_tx_idata_p(tx1_idata_p),
    .adrv9001_tx_idata_n(tx1_idata_n),
    .adrv9001_tx_qdata_p(tx1_qdata_p),
    .adrv9001_tx_qdata_n(tx1_qdata_n),
    
    .enable(tx1_enable),
    .swap_iq(tx1_swap_iq_cdc),
    .data_src(tx1_data_src_cdc),    
    .fixed_pattern(tx1_fixed_pattern_cdc), 

    .otx_axis_tdata(otx1_axis_tdata),
    .otx_axis_tvalid(otx1_axis_tvalid),        
  
    .s_axis_tdata(tx1_axis_tdata),
    .s_axis_tready(tx1_axis_tready),
    .s_axis_tvalid(tx1_axis_tvalid),
    .s_axis_aclk(tx1_axis_aclk)   
);
    
wire tx2_ssi_clk;
wire tx2_ssi_clk_div;

generate

  if( TX2_REF_CLK_USE_RX2 ) begin
    
    assign tx2_ssi_clk = rx2_ssi_clk;
    assign tx2_ssi_clk_div = rx2_ssi_clk_div;  
  
  end else begin
  
    adrv9001_clkin#(
      .SWAP_DIFF_REF_CLK( SWAP_DIFF_TX2_DCLK_IN )
    ) adrv9001_tx2_clkin (   
     .ref_clk_p(tx2_ref_clk_p),       
     .ref_clk_n(tx2_ref_clk_n), 
     .clk_out(tx2_ssi_clk),
     .clk_out_div(tx2_ssi_clk_div)
    );    
   
  end
endgenerate
    
adrv9001_tx #(  
    .SWAP_DIFF_IDATA(SWAP_DIFF_TX2_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_TX2_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_TX2_STROBE),
    .SWAP_DIFF_CLK_OUT(SWAP_DIFF_TX2_DCLK_OUT),
    .ENABLE_OTX_AXIS(ENABLE_OTX2_AXIS)
) adrv9001_tx2_inst (
    .ssi_clk(tx1_ssi_clk),
    .ssi_clk_div(tx1_ssi_clk_div), 
    
    .adrv9001_tx_dclk_p(tx2_dclk_p),
    .adrv9001_tx_dclk_n(tx2_dclk_n),    
    .adrv9001_tx_strobe_p(tx2_strobe_p),
    .adrv9001_tx_strobe_n(tx2_strobe_n),
    .adrv9001_tx_idata_p(tx2_idata_p),
    .adrv9001_tx_idata_n(tx2_idata_n),
    .adrv9001_tx_qdata_p(tx2_qdata_p),
    .adrv9001_tx_qdata_n(tx2_qdata_n),

    .enable(tx2_enable),    
    .swap_iq(tx2_swap_iq_cdc),    
    .data_src(tx2_data_src_cdc),  
    .fixed_pattern(tx2_fixed_pattern_cdc),    
    
    .otx_axis_tdata(otx2_axis_tdata),
    .otx_axis_tvalid(otx2_axis_tvalid),    
    
    .s_axis_tdata(tx2_axis_tdata),
    .s_axis_tready(tx2_axis_tready),
    .s_axis_tvalid(tx2_axis_tvalid),
    .s_axis_aclk(tx2_axis_aclk) 
);


/**** Debug ***************************************************************************************/

generate

  if( ENABLE_HOPPING_DEBUG ) begin

    assign HopDbg_HopPin = hop_dgpio;
    assign HopDbg_Rx1EnPin = rx1_en;
    assign HopDbg_Rx2EnPin = rx2_en;
    assign HopDbg_Tx1EnPin = tx1_en;
    assign HopDbg_Tx2EnPin = tx2_en;
    assign HopDbg_HopIrq = hop_irq;
    assign HopDbg_HopIrqClear = hop_trig_clear;
    assign HopDbg_HopTrig = hop_trig_rising;

  end else begin
      
    assign HopDbg_HopPin = 0;
    assign HopDbg_Rx1EnPin = 0;
    assign HopDbg_Rx2EnPin = 0;
    assign HopDbg_Tx1EnPin = 0;
    assign HopDbg_Tx2EnPin = 0;  
    assign HopDbg_HopIrq = 0;
    assign HopDbg_HopIrqClear = 0;
    assign HopDbg_HopTrig = 0;    
  
  end
  
  if( ENABLE_SPI_DEBUG ) begin
  
    assign SpiDbg_Sclk = spi_clk;
    assign SpiDbg_Csn = spi_csn;
    assign SpiDbg_Mosi = spi_mosi;
    assign SpiDbg_Miso = spi_miso;
    
  end else begin

    assign SpiDbg_Sclk = 0;
    assign SpiDbg_Csn = 0;
    assign SpiDbg_Mosi = 0;
    assign SpiDbg_Miso = 0;  
  
  end
  
  
endgenerate  
  
endmodule
