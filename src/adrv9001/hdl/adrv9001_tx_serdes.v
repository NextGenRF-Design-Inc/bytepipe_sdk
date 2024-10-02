`timescale 1ns / 1ps
/***************************************************************************//**
 *  \file       adrv9001_tx_serdes.v
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
 
module adrv9001_tx_serdes#( 
  parameter SWAP_DIFF = 0               // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )(
  input   wire          rst,            // reset
	input   wire          serdes_clk_div, // data/divided clock synchronous to parallel input 
  input   wire          serdes_clk,     // data clock synchronous to output
  output  wire          dout_p,         // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  output  wire			    dout_n,         // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  input   wire 	[7:0]   din,            // Parallel data output
  input   wire          din_t           // Data in tristate
    );
    	

wire        dout_t;
wire        dout;
reg  [7:0]  din_serdes = 'd0;
                  
/* Invert if swapping diff pair */                  
generate 

  if( SWAP_DIFF ) begin
  
    always @(posedge serdes_clk_div) din_serdes <= ~din;
    
    OBUFTDS OBUFTDS_inst (
      .O(dout_n),              
      .OB(dout_p),         
      .I(dout),          
      .T(dout_t)
    );  
    
  end else begin  
  
    always @(posedge serdes_clk_div) din_serdes <= din;
    
    OBUFTDS OBUFTDS_inst (
      .O(dout_p),  
      .OB(dout_n),  
      .I(dout),  
      .T(dout_t)
      );                
  
  end
  
endgenerate

OSERDESE3 #(
  .DATA_WIDTH(8),                   // Parallel Data Width (4-8)
  .INIT(1'b0),                      // Initialization value of the OSERDES flip-flops
  .IS_CLKDIV_INVERTED(1'b0),        // Optional inversion for CLKDIV
  .IS_CLK_INVERTED(1'b0),           // Optional inversion for CLK
  .IS_RST_INVERTED(1'b0),           // Optional inversion for RST
  .SIM_DEVICE("ULTRASCALE_PLUS")    // Set the device version (ULTRASCALE, ULTRASCALE_PLUS, ULTRASCALE_PLUS_ES1, ULTRASCALE_PLUS_ES2)
)
OSERDESE3_inst (
  .OQ(dout),                        // 1-bit output: Serial Output Data
  .T_OUT(dout_t),                   // 1-bit output: 3-state control output to IOB
  .CLK(serdes_clk),                 // 1-bit input: High-speed clock
  .CLKDIV(serdes_clk_div),          // 1-bit input: Divided Clock
  .D({din_serdes[0],
      din_serdes[1],
      din_serdes[2],
      din_serdes[3],
      din_serdes[4],
      din_serdes[5],
      din_serdes[6],
      din_serdes[7]}),              // 8-bit input: Parallel Data Input
  .RST(rst),                        // 1-bit input: Asynchronous Reset
  .T(din_t)                         // 1-bit input: Tristate input from fabric
  );
    
endmodule
