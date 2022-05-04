`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_serdes_pack
// 
// Description:
// 
// This module packs two 8-bit aligned serdes inputs.  The strobe
// can be configured as 8-bits on 8-bits off or 1-bit on 15-bits off.
//
// 
// ***************************************************************************
// ***************************************************************************

module adrv9001_serdes_pack(
  input   wire        	clk,            // main clock
  input   wire  [7:0]   i_in,           // 8-bit i/q data input from serdes
  input   wire  [7:0]   q_in,           // 8-bit i/q data input from serdes  
  input   wire 	[7:0]   strb_in,        // 8-bit strobe input from serdes
  output  wire  [15:0]  i_out,          // Packed output data
  output  wire  [15:0]  q_out,          // Packed output data
  output  wire  [15:0]  strb_out,       // Packed strobe data
  output  wire          valid_out       // Output data valid
    );

reg         validReg = 0;
reg [15:0]  iReg = 0;
reg [15:0]  qReg = 0;
reg [15:0]  sReg = 0;

assign i_out = iReg;
assign q_out = qReg;
assign strb_out = sReg;
assign valid_out = validReg;

    
always @(posedge clk) begin
  validReg <= ~validReg;
    
  if(validReg)
    iReg <= {i_in, iReg[7:0]};
  else 
    iReg <= {iReg[15:8], i_in};
    
  if(validReg)
    qReg <= {q_in, qReg[7:0]};
  else 
    qReg <= {qReg[15:8], q_in};

  if(validReg)
    sReg <= {strb_in, sReg[7:0]};
  else 
    sReg <= {sReg[15:8], strb_in};   

end
    
endmodule
