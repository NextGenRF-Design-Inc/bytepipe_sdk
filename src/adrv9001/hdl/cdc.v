`timescale 1ps/1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    cdc.v
// 
// Description: 
//
// This module instantiates a clock domain crossing
//
// ***************************************************************************
// ***************************************************************************

module cdc#(
  parameter DATA_WIDTH = 1
  )(
  input  wire                     m_cdc_clk,
  output reg  [(DATA_WIDTH-1):0]  m_cdc_tdata = 'd0,
  input  wire [(DATA_WIDTH-1):0]  s_cdc_tdata
);

reg  [(DATA_WIDTH-1):0]           cdc_tdata_reg = 'd0;

always @(posedge m_cdc_clk) begin
  cdc_tdata_reg <= s_cdc_tdata;
  m_cdc_tdata <= cdc_tdata_reg;
  
end

endmodule
