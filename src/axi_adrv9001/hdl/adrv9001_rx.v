`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_rx.v
// 
// Description: 
//
// This module instantiates a receive channel for interfacing to the 
// ADRV9001/2.  
//
// ***************************************************************************
// ***************************************************************************

module adrv9001_rx#(
  parameter SWAP_DIFF_IDATA = 0,          // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_QDATA = 0,          // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_STROBE = 0,         // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_DCLK = 0            // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )(
  input   wire          rst,              // 
  
  input   wire          dclk_p,           // 1-bit data clock input(connect directly to top-level port)
  input   wire			    dclk_n,           // 1-bit data clock input(connect directly to top-level port)
  input   wire			    strobe_p,         // 1-bit strobe input(connect directly to top-level port)
  input   wire			    strobe_n,         // 1-bit strobe input(connect directly to top-level port)
  input   wire			    idata_p,          // 1-bit data input(connect directly to top-level port)
  input   wire        	idata_n,          // 1-bit data input(connect directly to top-level port)
  input   wire        	qdata_p,          // 1-bit data input(connect directly to top-level port)
  input   wire		      qdata_n,          // 1-bit data input(connect directly to top-level port)
  
  output  wire [31:0]	  m_axis_tdata,     // iq data output
  output  wire          m_axis_tvalid,    // valid output  
  output  wire          m_axis_aclk,
  output  wire          m_axis_rstn
    );

wire dclk_in;
wire dclk;
wire dclk_div;

wire [7:0]  i_data;
wire [7:0]  q_data;
wire [7:0]  strobe;

wire [15:0] i_packed;
wire [15:0] q_packed;
wire [15:0] strb_packed;
wire        valid_packed;

wire [15:0] i_aligned;
wire [15:0] q_aligned;
wire        valid_aligned;

assign m_axis_tvalid = valid_aligned;
assign m_axis_tdata = {i_aligned,q_aligned};
assign m_axis_aclk = dclk_div;


/* Differential IO Buffer */  
IBUFGDS dclk_ds_buf (
  .O(dclk_in),                    // 1-bit output: Buffer output
  .I(dclk_p),                     // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .IB(dclk_n)                     // 1-bit input: Diff_n buffer input (connect directly to top-level port)
);  
      
wire dclk_in_buf;      
wire serdes_rst;

async_rst async_rst_inst (
  .slowest_sync_clk(dclk_in_buf),          // input wire slowest_sync_clk
  .ext_reset_in(rst),                  // input wire ext_reset_in
  .aux_reset_in(1'b0),                  // input wire aux_reset_in
  .mb_debug_sys_rst(1'b0),          // input wire mb_debug_sys_rst
  .dcm_locked(1'b1),                      // input wire dcm_locked
  .mb_reset(serdes_rst),                          // output wire mb_reset
  .bus_struct_reset(),          // output wire [0 : 0] bus_struct_reset
  .peripheral_reset(),          // output wire [0 : 0] peripheral_reset
  .interconnect_aresetn(),  // output wire [0 : 0] interconnect_aresetn
  .peripheral_aresetn(m_axis_rstn)      // output wire [0 : 0] peripheral_aresetn
);
          
BUFGCE #(
  .CE_TYPE ("SYNC"),
  .IS_CE_INVERTED (1'b0),
  .IS_I_INVERTED (1'b0)
) dclk_in_buf_i (
  .O (dclk_in_buf),
  .CE (1'b1),
  .I (dclk_in)
);

BUFGCE #(
  .CE_TYPE ("SYNC"),
  .IS_CE_INVERTED (1'b0),
  .IS_I_INVERTED (1'b0)
  ) 
dclk_buf_i (
  .O (dclk),
  .CE (1'b1),
  .I (dclk_in_buf)
);

BUFGCE_DIV #(
  .BUFGCE_DIVIDE (4),
  .IS_CE_INVERTED (1'b0),
  .IS_CLR_INVERTED (1'b0),
  .IS_I_INVERTED (1'b0)
  )
dclk_div_buf_i (
  .O (dclk_div),
  .CE (1'b1),
  .CLR (serdes_rst),
  .I (dclk_in_buf)
);      
           
/* I Data Serdes */
adrv9001_rx_serdes #(
  .SWAP_DIFF(SWAP_DIFF_IDATA)     // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
)
i_serdes (
  .rst(serdes_rst),               // Reset
  .dclk_div(dclk_div),            // data or divided clock
  .dclk(dclk),                    // clock synchronous with data
  .din_p(idata_p),                // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .din_n(idata_n),                // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .dout(i_data)                   // Parallel data output
);

/* Q Data Serdes */
adrv9001_rx_serdes #(
  .SWAP_DIFF(SWAP_DIFF_QDATA)     // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
)
q_serdes (
  .rst(serdes_rst),               // Reset
  .dclk_div(dclk_div),            // data or divided clock
  .dclk(dclk),                    // clock synchronous with data
  .din_p(qdata_p),                // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .din_n(qdata_n),                // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .dout(q_data)                   // Parallel data output
);

/* Strobe Serdes */
adrv9001_rx_serdes #(
  .SWAP_DIFF(SWAP_DIFF_STROBE)    // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
)
s_serdes (
  .rst(serdes_rst),               // Reset
  .dclk_div(dclk_div),            // data or divided clock
  .dclk(dclk),                    // clock synchronous with data
  .din_p(strobe_p),               // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .din_n(strobe_n),               // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .dout(strobe)                   // Parallel data output
);

/* Pack Signals */
adrv9001_serdes_pack pack(
  .clk(dclk_div),                 // clock
  .i_in(i_data),                  // 8-bit i/q data input from serdes
  .q_in(q_data),                  // 8-bit i/q data input from serdes  
  .strb_in(strobe),               // 8-bit strobe input from serdes
  .i_out(i_packed),               // Packed output data
  .q_out(q_packed),               // Packed output data
  .strb_out(strb_packed),         // Packed strobe data
  .valid_out(valid_packed)        // Output data valid
);

/* Align Signals */
adrv9001_serdes_aligner align(
  .clk(dclk_div),                 // clock
  .i_in(i_packed),                // 16-bit i/q data input from serdes
  .q_in(q_packed),                // 16-bit i/q data input from serdes  
  .strb_in(strb_packed),          // 16-bit strobe input from serdes
  .valid_in(valid_packed),        // Valid input data
  .i_out(i_aligned),              // Packed output data
  .q_out(q_aligned),              // Packed output data
  .valid_out(valid_aligned)       // Output data valid
);


    
endmodule
