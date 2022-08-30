`timescale 1ps/1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_axis_ila.v
// 
// Description: 
//
// This module instantiates an integrated logic analyzer
//
// ***************************************************************************
// ***************************************************************************

module adrv9001_axis_ila(
  input  wire         clk,
  input  wire         enable,
  input  wire [31:0]  tdata,
  input  wire         tvalid,
  input  wire         tready
);

wire [31:0] tdata_cdc;
wire        enable_cdc;
wire        tvalid_cdc;
wire        tready_cdc;

cdc #(
  .DATA_WIDTH(35) )
tx1_dgb_cdc_i (
  .s_cdc_tdata  ({enable, tdata, tvalid, tready}),
  .m_cdc_clk    (clk),
  .m_cdc_tdata  ({enable_cdc, tdata_cdc, tvalid_cdc, tready_cdc})
);

ila_axis ila_axis_i (
	.clk(clk), // input wire clk
	.probe0(enable_cdc), // input wire [0:0]  probe0  
	.probe1(tvalid_cdc), // input wire [0:0]  probe1 
	.probe2(tready_cdc), // input wire [0:0]  probe2 
	.probe3(tdata_cdc[31:16]), // input wire [15:0]  probe3 
	.probe4(tdata_cdc[15:0]) // input wire [15:0]  probe4
);

endmodule
