`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_tx.v
// 
// Description: 
//
// This module instantiates a transmit channel for interfacing to the 
// ADRV9001/2.  
//
// ***************************************************************************
// ***************************************************************************

module adrv9001_tx#(
  parameter LVDS_OUTPUT = 1,              // Output is LVDS or CMOS
  parameter SWAP_DIFF_IDATA = 0,          // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_QDATA = 0,          // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_STROBE = 0,         // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_DCLK_IN = 0,        // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_DCLK_OUT = 0        // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )(
  input  wire            ref_clk_p,        // 1-bit Reference clock input(connect directly to top-level port)
  input  wire            ref_clk_n,        // 1-bit Reference clock input(connect directly to top-level port)  
  
  output wire            dclk_p,           // 1-bit data clock output(connect directly to top-level port)
  output wire			 dclk_n,           // 1-bit data clock output(connect directly to top-level port)
  output wire			 strobe_p,         // 1-bit strobe output(connect directly to top-level port)
  output wire			 strobe_n,         // 1-bit strobe output(connect directly to top-level port)
  output wire			 idata_p,          // 1-bit data output(connect directly to top-level port)
  output wire        	 idata_n,          // 1-bit data output(connect directly to top-level port)
  output wire        	 qdata_p,          // 1-bit data output(connect directly to top-level port)
  output wire			 qdata_n,          // 1-bit data output(connect directly to top-level port)
  
  input  wire            data_src,         // Data Source
  input  wire [31:0]     tdata,            // Constant Data
  
  input  wire            tdd_en,
  output reg             enable,   
  input  wire [31:0]     disable_cnt,
  input  wire [31:0]     ssi_enable_cnt,
      
  input  wire [31:0]	 s_axis_tdata,     // iq data input
  output wire            s_axis_tready,    // ready for data
  input  wire            s_axis_tvalid,    // input data valid
  output wire            s_axis_aclk,   
  output wire            s_axis_rstn  
    );

wire dclk;
wire dclk_div;
reg serdes_rst = 0;

assign s_axis_aclk = dclk_div;

wire [7:0]  s_unpacked;
wire [7:0]  i_unpacked;
wire [7:0]  q_unpacked;

//async_rst async_rst_inst (
//  .slowest_sync_clk(dclk),          // input wire slowest_sync_clk
//  .ext_reset_in(rst),                  // input wire ext_reset_in
//  .aux_reset_in(1'b0),                  // input wire aux_reset_in
//  .mb_debug_sys_rst(1'b0),          // input wire mb_debug_sys_rst
//  .dcm_locked(1'b1),                      // input wire dcm_locked
//  .mb_reset(serdes_rst),                          // output wire mb_reset
//  .bus_struct_reset(),          // output wire [0 : 0] bus_struct_reset
//  .peripheral_reset(),          // output wire [0 : 0] peripheral_reset
//  .interconnect_aresetn(),  // output wire [0 : 0] interconnect_aresetn
//  .peripheral_aresetn(s_axis_rstn)      // output wire [0 : 0] peripheral_aresetn
//);
  
adrv9001_tx_clk#(
  .LVDS(LVDS_OUTPUT),
  .SWAP_DIFF_CLK_OUT(SWAP_DIFF_DCLK_OUT),     // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  .SWAP_DIFF_CLK_IN(SWAP_DIFF_DCLK_IN)      // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin  
  )
tx_clk_inst(  
  .dclk_div_rst(1'b0),        // reset dclk_div
  .ref_clk_p(ref_clk_p),            // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .ref_clk_n(ref_clk_n),            // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .dclk_p(dclk_p),                  // 1-bit data clock output(connect directly to top-level port)
  .dclk_n(dclk_n),                  // 1-bit data clock output(connect directly to top-level port)    
  .dclk(dclk),                      // 1-bit data clock output
  .dclk_div(dclk_div)               // 1-bit divided data clock output 
);  

adrv9001_tx_serdes#(
  .LVDS(LVDS_OUTPUT),
  .SWAP_DIFF(SWAP_DIFF_STROBE)     // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )
s_serdes(
  .rst(serdes_rst),                 // reset
  .dclk_div(dclk_div),              // data/divided clock synchronous to parallel input 
  .dclk(dclk),                      // data clock synchronous to output
  .dout_p(strobe_p),                // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .dout_n(strobe_n),                // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .din(s_unpacked),                 // Parallel data output
  .din_t(1'b0)                      // Data in tristate
);

adrv9001_tx_serdes#( 
  .LVDS(LVDS_OUTPUT),
  .SWAP_DIFF(SWAP_DIFF_IDATA)       // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )
i_serdes(
  .rst(serdes_rst),                 // reset
  .dclk_div(dclk_div),              // data/divided clock synchronous to parallel input 
  .dclk(dclk),                      // data clock synchronous to output
  .dout_p(idata_p),                 // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .dout_n(idata_n),                 // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .din(i_unpacked),                 // Parallel data output
  .din_t(1'b0)                      // Data in tristate
);

adrv9001_tx_serdes#( 
  .LVDS(LVDS_OUTPUT),
  .SWAP_DIFF(SWAP_DIFF_QDATA)       // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )
q_serdes(
  .rst(serdes_rst),                 // reset
  .dclk_div(dclk_div),              // data/divided clock synchronous to parallel input 
  .dclk(dclk),                      // data clock synchronous to output
  .dout_p(qdata_p),                 // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .dout_n(qdata_n),                 // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .din(q_unpacked),                 // Parallel data output
  .din_t(1'b0)                      // Data in tristate
);

wire [31:0]  data_unpack_in;
wire [31:0]  tdata_cdc;
wire         data_src_cdc;
wire [31:0]  disable_cnt_cdc;
wire [31:0]  ssi_enable_cnt_cdc;
wire         tdd_en_cdc;

assign data_unpack_in = data_src_cdc? tdata_cdc : s_axis_tdata;

cdc #(
  .DATA_WIDTH(32) )
tdata_cdc_i (
  .s_cdc_tdata  (tdata),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (tdata_cdc)
);

cdc #(
  .DATA_WIDTH(1) )
tdata_src_cdc_i (
  .s_cdc_tdata  (data_src),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (data_src_cdc)
);

cdc #(
  .DATA_WIDTH(32) )
disable_cnt_cdc_i (
  .s_cdc_tdata  (disable_cnt),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (disable_cnt_cdc)
);

cdc #(
  .DATA_WIDTH(32) )
ssi_enable_cnt_cdc_i (
  .s_cdc_tdata  (ssi_enable_cnt),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (ssi_enable_cnt_cdc)
);

cdc #(
  .DATA_WIDTH(1) )
tdd_en_cdc_i (
  .s_cdc_tdata  (tdd_en),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (tdd_en_cdc)
);

reg [31:0] cnt = 0;
reg ce = 0;

always @( posedge dclk_div ) begin

  if( tdd_en_cdc == 1'b0 )
    ce <= 1'b0;
  else
    ce <= ~ce;

  if( tdd_en_cdc == 1'b0 )
    cnt <= 0;
  else if( (cnt < 32'hffffffff) && ( ce == 1'b1) )
    cnt <= cnt + 1;   
  else
    cnt <= cnt;   
    
  if( (cnt <= disable_cnt_cdc) && (cnt > 32'h0) )
    enable <= 1'b1;
  else
    enable <= 1'b0;
    
  if( ( cnt > ssi_enable_cnt_cdc ) && ( cnt <= disable_cnt_cdc ) )
    serdes_rst <= 1'b0;
  else
    serdes_rst <= 1'b1;     
    
end

adrv9001_serdes_unpack unpack_inst(
  .clk(dclk_div),                   // data clock
  .rst(serdes_rst),
  .din(data_unpack_in),                   // packed i/q data input 
  .din_rdy(s_axis_tready),          // Ready for new data
  .strb_out(s_unpacked),            // 8-bit unpacked strobe output to serdes
  .i_out(i_unpacked),               // 8-bit unpacked i data output to serdes
  .q_out(q_unpacked)                // 8-bit unpacked q data output to serdes
);   



    
endmodule
