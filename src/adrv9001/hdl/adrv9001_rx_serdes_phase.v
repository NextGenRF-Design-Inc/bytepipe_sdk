`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_rx_serdes_phase
// 
// Description:
// 
// This module determines the phase of a serdes based on strobe data. The 
// strobe must be configured as 1-bit on and 15-bits off.
//
// 
// ***************************************************************************
// ***************************************************************************

module adrv9001_rx_serdes_phase(
  input   wire        	clk,            // main clock
  input   wire			    strb_valid,     // strobe valid
  input   wire 	[15:0]  strb,           // 16-bit strobe 
  output  wire  [3:0]   phase           // Phase offset of strobe
    );
    

reg [3:0] phaseReg = 0;

assign phase = phaseReg;

always @(posedge clk) begin  
  if (strb_valid) begin
    if(strb[15])
      phaseReg <= 4'd0;
    else if(strb[14])
      phaseReg <= 4'd1;
    else if(strb[13])
      phaseReg <= 4'd2;
    else if(strb[12])
      phaseReg <= 4'd3;
    else if(strb[11])
      phaseReg <= 4'd4;
    else if(strb[10])
      phaseReg <= 4'd5;
    else if(strb[9])
      phaseReg <= 4'd6;
    else if(strb[8])
      phaseReg <= 4'd7;
    else if(strb[7])
      phaseReg <= 4'd8;
    else if(strb[6])
      phaseReg <= 4'd9;
    else if(strb[5])
      phaseReg <= 4'd10;
    else if(strb[4])
      phaseReg <= 4'd11;
    else if(strb[3])
      phaseReg <= 4'd12;
    else if(strb[2])
      phaseReg <= 4'd13;
    else if(strb[1])
      phaseReg <= 4'd14;
    else
      phaseReg <= 4'd15;   
  end
  else begin
    phaseReg <= phaseReg;      
  end
end  
    
endmodule
