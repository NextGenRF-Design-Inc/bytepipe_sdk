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

module adrv9001_tx_clk#(
  parameter LVDS = 1,                     // Output is LVDS or CMOS
  parameter SWAP_DIFF_CLK_IN = 0,         // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_CLK_OUT = 0         // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin  
  )(
  input   wire          dclk_div_rst,     // rest dclk_div
  input   wire          ref_clk_p,        // 1-bit data clock input(connect directly to top-level port)
  input   wire			ref_clk_n,        // 1-bit data clock input(connect directly to top-level port)
  output  wire          dclk_p,           // 1-bit data clock output(connect directly to top-level port)
  output  wire          dclk_n,           // 1-bit data clock output(connect directly to top-level port)  
  output  wire          dclk,             // 1-bit data clock output
  output  wire          dclk_div          // 1-bit divided data clock output 
    );

wire dclk_in;
      
/* dclk_in io buffer */
generate
if( SWAP_DIFF_CLK_IN == 1) begin   
  IBUFDS refclk_ds_buf (
    .O(dclk_in),            
    .I(ref_clk_n),           
    .IB(ref_clk_p));                    
end else begin       
  IBUFDS refclk_ds_buf (
    .O(dclk_in),                
    .I(ref_clk_p),                 
    .IB(ref_clk_n));                  
end    
endgenerate
 
/* dclk_div buffer/divider */  
BUFGCE_DIV #(
  .BUFGCE_DIVIDE(4),          
  .IS_CE_INVERTED(1'b0),      
  .IS_CLR_INVERTED(1'b0),      
  .IS_I_INVERTED(1'b0)        
)
dclk_div_buf_inst (
  .O(dclk_div),   
  .CE(1'b1),         
  .CLR(dclk_div_rst),      
  .I(dclk_in)              
);

/* dclk buffer */
BUFGCE #(
  .CE_TYPE ("SYNC"),
  .IS_CE_INVERTED (1'b0),
  .IS_I_INVERTED (1'b0)
) 
dclk_in_bufgce (
  .O (dclk),
  .CE (1'b1),
  .I (dclk_in)
);  

/* dclk_out io buffer */
generate
if( LVDS == 0 ) begin

  OBUF dclk_ds_buf (
    .O(dclk_p),                
    .I(dclk_in));       
          
  assign dclk_n = 0;
  
end else begin

  if( SWAP_DIFF_CLK_OUT == 1) begin    
    OBUFDS dclk_ds_buf (
      .O(dclk_n),             
      .OB(dclk_p),            
      .I(~dclk_in));                     
  end else begin  
    OBUFDS dclk_ds_buf (
      .O(dclk_p),                     
      .OB(dclk_n),                  
      .I(dclk_in));                         
 end 
 
end

endgenerate

    
endmodule
