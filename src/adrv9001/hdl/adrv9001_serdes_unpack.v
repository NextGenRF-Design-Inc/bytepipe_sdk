`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_serdes_unpack
// 
// Description:
// 
// This module unpacks 32-bit IQ data into 8-bit outputs for interfacing to
// an output serdes. Assumes it takes one cycle for new data after din_rdy is
// asserted
//
// 
// ***************************************************************************
// ***************************************************************************

module adrv9001_serdes_unpack(
  input   wire        	clk,            // main clock
  input   wire          rst,
  input   wire  [31:0]  din,            // packed i/q data input 
  output  wire          din_rdy,        // ready for new input data
  output  wire 	[7:0]   strb_out,       // 8-bit unpacked strobe output to serdes
  output  wire  [7:0]   i_out,          // 8-bit unpacked i data output to serdes
  output  wire  [7:0]   q_out           // 8-bit unpacked q data output to serdes
    );

reg [7:0]   iReg = 0;
reg [7:0]   qReg = 0;
reg [7:0]   sReg = 0;
reg         rdy = 1;

assign i_out = iReg;
assign q_out = qReg;
assign strb_out = sReg;
assign din_rdy = rdy;  

always @(posedge clk) begin

  if( rst )
    rdy <= 1'b0;
  else
    rdy <= ~rdy;
  
  if( rdy ) begin
    iReg <= din[23:16];
    qReg <= din[7:0];     
    sReg <= 8'h0;       
  end else begin  
    iReg <= din[31:24];
    qReg <= din[15:8];
    sReg <= 8'h80;        
  end

end
    
endmodule
