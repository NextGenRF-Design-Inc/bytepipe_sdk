`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_tx_clk.v
// 
// Description: 
//
// This module instantiates the transmit clocking for the ADRV9001/2.  
//
// ***************************************************************************
// ***************************************************************************

module adrv9001_clk_in#(
  parameter SWAP_DIFF_CLK_IN = 0          // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin 
  )(
  input   wire          rst,              // rest clk_div
  input   wire          clk_p,            // 1-bit data clock input(connect directly to top-level port)
  input   wire          clk_n,            // 1-bit data clock input(connect directly to top-level port) 
  output  wire          clk,              // 1-bit data clock output
  output  wire          clk_div           // 1-bit divided data clock output 
    );

wire clk_in;
wire clk_in_buf; 
      
/* clk_in io buffer */
generate
if( SWAP_DIFF_CLK_IN == 1) begin   
  IBUFGDS refclk_ds_buf (
    .O(clk_in),            
    .I(clk_n),           
    .IB(clk_p));                    
end else begin       
  IBUFGDS refclk_ds_buf (
    .O(clk_in),                
    .I(clk_p),                 
    .IB(clk_n));                  
end    
endgenerate

BUFGCE #(
  .CE_TYPE ("SYNC"),
  .IS_CE_INVERTED (1'b0),
  .IS_I_INVERTED (1'b0)
) clk_in_buf_i (
  .O (clk_in_buf),
  .CE (1'b1),
  .I (clk_in)
);

/* clk buffer */
BUFGCE #(
  .CE_TYPE ("SYNC"),
  .IS_CE_INVERTED (1'b0),
  .IS_I_INVERTED (1'b0)
) 
clk_in_bufgce (
  .O (clk),
  .CE (1'b1),
  .I (clk_in_buf)
);  

 
/* clk_div buffer/divider */  
BUFGCE_DIV #(
  .BUFGCE_DIVIDE(4),          
  .IS_CE_INVERTED(1'b0),      
  .IS_CLR_INVERTED(1'b0),      
  .IS_I_INVERTED(1'b0)        
)
clk_div_buf_inst (
  .O(clk_div),   
  .CE(1'b1),         
  .CLR(rst),      
  .I(clk_in_buf)              
);



    
endmodule
