`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_serdes_aligner
// 
// Description:
// 
// This module aligns 8-bit serdes data with the strobe signal.  The strobe
// can be configured as 8-bits on 8-bits off or 1-bit on 15-bits off.
//
// 
// ***************************************************************************
// ***************************************************************************

module adrv9001_serdes_aligner(
  input   wire        	clk,            // main clock
  input   wire          rst,
  input   wire  [15:0]  i_in,           // 16-bit serdes data
  input   wire  [15:0]  q_in,           // 16-bit serdes data
  input   wire  [15:0]  strb_in,        // 16-bit serdes data  
  input   wire			valid_in,       // Input data valid
  output  reg   [15:0]  i_out = 0,      // Shifted i/q data aligning with strobe
  output  reg   [15:0]  q_out = 0,      // Shifted i/q data aligning with strobe
  output  reg           valid_out = 0   // dout valid
);
    

wire  [3:0] phase;
reg  [15:0] iReg = 0;
reg  [15:0] qReg = 0;


/* Determine Serdes Phase */
adrv9001_rx_serdes_phase serdes_phase(
  .clk(clk),                          // strobe clock
  .strb_valid(valid_in),              // strobe valid
  .strb(strb_in),                     // 8-bit strobe input from serdes
  .phase(phase)                       // Phase offset of strobe
);

always @(posedge clk) begin
  if( rst )
    valid_out <= 1'b0;
  else
    valid_out <= valid_in;
  
  if(valid_in) begin
    iReg <= i_in;
    qReg <= q_in;
  end
  else begin
    iReg <= iReg;
    qReg <= qReg;
  end
  
  case(phase)
     0: i_out <=  iReg[15:0];
     1: i_out <= {iReg[14:0], i_in[15:15]};
     2: i_out <= {iReg[13:0], i_in[15:14]};
     3: i_out <= {iReg[12:0], i_in[15:13]};
     4: i_out <= {iReg[11:0], i_in[15:12]};
     5: i_out <= {iReg[10:0], i_in[15:11]};
     6: i_out <= {iReg[ 9:0], i_in[15:10]};
     7: i_out <= {iReg[ 8:0], i_in[15: 9]};
     8: i_out <= {iReg[ 7:0], i_in[15: 8]};
     9: i_out <= {iReg[ 6:0], i_in[15: 7]};
    10: i_out <= {iReg[ 5:0], i_in[15: 6]};
    11: i_out <= {iReg[ 4:0], i_in[15: 5]};
    12: i_out <= {iReg[ 3:0], i_in[15: 4]};
    13: i_out <= {iReg[ 2:0], i_in[15: 3]};
    14: i_out <= {iReg[ 1:0], i_in[15: 2]};
    15: i_out <= {iReg[ 0:0], i_in[15: 1]};
  endcase
  
  case(phase)
     0: q_out <=  qReg[15:0];
     1: q_out <= {qReg[14:0], q_in[15:15]};
     2: q_out <= {qReg[13:0], q_in[15:14]};
     3: q_out <= {qReg[12:0], q_in[15:13]};
     4: q_out <= {qReg[11:0], q_in[15:12]};
     5: q_out <= {qReg[10:0], q_in[15:11]};
     6: q_out <= {qReg[ 9:0], q_in[15:10]};
     7: q_out <= {qReg[ 8:0], q_in[15: 9]};
     8: q_out <= {qReg[ 7:0], q_in[15: 8]};
     9: q_out <= {qReg[ 6:0], q_in[15: 7]};
    10: q_out <= {qReg[ 5:0], q_in[15: 6]};
    11: q_out <= {qReg[ 4:0], q_in[15: 5]};
    12: q_out <= {qReg[ 3:0], q_in[15: 4]};
    13: q_out <= {qReg[ 2:0], q_in[15: 3]};
    14: q_out <= {qReg[ 1:0], q_in[15: 2]};
    15: q_out <= {qReg[ 0:0], q_in[15: 1]};
  endcase  
  
end  
    
endmodule
