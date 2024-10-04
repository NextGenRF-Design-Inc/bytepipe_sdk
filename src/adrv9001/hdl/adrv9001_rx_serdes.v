`timescale 1ns / 1ps
/***************************************************************************//**
 *  \file       adrv9001_rx_serdes.v
 *
 *  \details
 *
 *  \copyright
 *
 *  Copyright 2021(c) NextGen RF Design, Inc.  
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *   - Use of the software either in source or binary form, must be run on or
 *     directly connected to a NextGen RF Design, Inc. product.
 *
 *  THIS SOFTWARE IS PROVIDED BY NEXTGEN RF DESIGN "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 *  EVENT SHALL NEXTGEN RF DESIGN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************/
 
module adrv9001_rx_serdes#(
  parameter SWAP_DIFF = 0              // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )(
  input   wire          rst,            // reset
  input   wire          clk_div,       // data/divided clock 
  input   wire          clk,           // data clock synchronous to input
  input   wire          din_p,          // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  input   wire			    din_n,          // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  output  reg  	[7:0]   dout            // Parallel data output
    );
    	
wire        din;
wire [7:0]  dout_serdes;

generate
if( SWAP_DIFF ) begin

  IBUFDS IBUFDS_inst (
    .O(din),                            // 1-bit output: Buffer output
    .I(din_n),                          // 1-bit input: Diff_p buffer input (connect directly to top-level port)
    .IB(din_p)                          // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  );   
    
  always @(posedge clk_div) begin
    dout <= ~dout_serdes;  
  end
  
end else begin

  IBUFDS IBUFDS_inst (
    .O(din),                            // 1-bit output: Buffer output
    .I(din_p),                          // 1-bit input: Diff_p buffer input (connect directly to top-level port)
    .IB(din_n)                          // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  );   
  
  always @(posedge clk_div) begin
    dout <= dout_serdes;  
  end
      
end
endgenerate  


ISERDESE3 #(
  .DATA_WIDTH(8),                 // Parallel data width (4,8)
  .FIFO_ENABLE("TRUE"),           // Enables the use of the FIFO
  .FIFO_SYNC_MODE("FALSE"),       // Always set to FALSE. TRUE is reserved for later use.
  .IS_CLK_B_INVERTED(1'b1),       // Optional inversion for CLK_B
  .IS_CLK_INVERTED(1'b0),         // Optional inversion for CLK
  .IS_RST_INVERTED(1'b0),         // Optional inversion for RST
  .SIM_DEVICE("ULTRASCALE_PLUS")) // Set the device version (ULTRASCALE, ULTRASCALE_PLUS, ULTRASCALE_PLUS_ES1, ULTRASCALE_PLUS_ES2)
ISERDESE3_inst (
  .INTERNAL_DIVCLK(),             // 1-bit output: Internally divided down clock used when FIFO is disabled (do not connect)
  .Q({dout_serdes[0],
      dout_serdes[1],
      dout_serdes[2],
      dout_serdes[3],
      dout_serdes[4],
      dout_serdes[5],
      dout_serdes[6],
      dout_serdes[7]}),           // 8-bit registered output
  .CLK(clk),                     // 1-bit input: High-speed clock
  .CLKDIV(clk_div),              // 1-bit input: Divided Clock
  .CLK_B(clk),                   // 1-bit input: Inversion of High-speed clock CLK
  .D(din),                        // 1-bit input: Serial Data Input
  .FIFO_RD_CLK(clk_div),        // 1-bit input: FIFO read clock	
  .FIFO_RD_EN(1'b1),            // 1-bit input: Enables reading the FIFO when asserted
  .FIFO_EMPTY(),
  .RST(rst)                       // 1-bit input: Asynchronous Reset
);


    
endmodule
