`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_rx_serdes
// 
// Description:
// 
// This module instantiates a 8-bit input serdes for a single lane.  It
// supports a fixed configurable delay in pico-seconds.  Includes the
// differential IO buffer.
//
// 
// ***************************************************************************
// ***************************************************************************

module adrv9001_rx_serdes#(
  parameter SWAP_DIFF = 0              // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )(
  input   wire          rst,            // reset
  input   wire          dclk_div,       // data/divided clock 
  input   wire          dclk,           // data clock synchronous to input
  input   wire          din_p,          // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  input   wire			din_n,          // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  output  wire 	[7:0]   dout            // Parallel data output
    );
    	
wire        din;
wire [7:0]  dout_serdes;

generate
if( SWAP_DIFF == 1 ) begin

  IBUFDS IBUFDS_inst (
    .O(din),                            // 1-bit output: Buffer output
    .I(din_n),                          // 1-bit input: Diff_p buffer input (connect directly to top-level port)
    .IB(din_p)                          // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  );   
    
  assign dout = ~dout_serdes;
  
end else begin

  IBUFDS IBUFDS_inst (
    .O(din),                            // 1-bit output: Buffer output
    .I(din_p),                          // 1-bit input: Diff_p buffer input (connect directly to top-level port)
    .IB(din_n)                          // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  );   
  
  assign dout = dout_serdes;
      
end
endgenerate  
                    


ISERDESE3 #(
  .DATA_WIDTH(8),                 // Parallel data width (4,8)
  .FIFO_ENABLE("FALSE"),          // Enables the use of the FIFO
  .FIFO_SYNC_MODE("FALSE"),       // Always set to FALSE. TRUE is reserved for later use.
  .IS_CLK_B_INVERTED(1'b0),       // Optional inversion for CLK_B
  .IS_CLK_INVERTED(1'b0),         // Optional inversion for CLK
  .IS_RST_INVERTED(1'b0),         // Optional inversion for RST
  .SIM_DEVICE("ULTRASCALE_PLUS")) // Set the device version (ULTRASCALE, ULTRASCALE_PLUS, ULTRASCALE_PLUS_ES1, ULTRASCALE_PLUS_ES2)
ISERDESE3_inst (
  .FIFO_EMPTY(),                  // 1-bit output: FIFO empty flag
  .INTERNAL_DIVCLK(),             // 1-bit output: Internally divided down clock used when FIFO is disabled (do not connect)
  .Q({dout_serdes[0],
      dout_serdes[1],
      dout_serdes[2],
      dout_serdes[3],
      dout_serdes[4],
      dout_serdes[5],
      dout_serdes[6],
      dout_serdes[7]}),           // 8-bit registered output
  .CLK(dclk),                     // 1-bit input: High-speed clock
  .CLKDIV(dclk_div),              // 1-bit input: Divided Clock
  .CLK_B(~dclk),                  // 1-bit input: Inversion of High-speed clock CLK
  .D(din),                        // 1-bit input: Serial Data Input
  .FIFO_RD_CLK(dclk_div),         // 1-bit input: FIFO read clock	
  .FIFO_RD_EN(1'b1),              // 1-bit input: Enables reading the FIFO when asserted
  .RST(rst)                       // 1-bit input: Asynchronous Reset
);

    
endmodule
