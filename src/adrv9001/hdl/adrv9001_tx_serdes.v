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
// This module instantiates a 8-bit output serdes for a single lane. Includes
// the differential IO buffer.
//
// 
// ***************************************************************************
// ***************************************************************************

module adrv9001_tx_serdes#( 
  parameter SWAP_DIFF = 0,              // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter LVDS = 1                   // Output is LVDS or CMOS
  )(
  input   wire          rst,            // reset
	input   wire          dclk_div,       // data/divided clock synchronous to parallel input 
  input   wire          dclk,           // data clock synchronous to output
  output  wire          dout_p,         // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  output  wire			    dout_n,         // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  input   wire 	[7:0]   din,            // Parallel data output
  input   wire          din_t           // Data in tristate
    );
    	

wire        dout_t;
wire        dout;
wire [7:0]  din_serdes;
                  
/* Invert if swapping diff pair */                  
generate

  if( SWAP_DIFF == 1 ) begin
    assign din_serdes = ~din;
  end else begin  
    assign din_serdes = din;
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
  .CLK(dclk),                       // 1-bit input: High-speed clock
  .CLKDIV(dclk_div),                // 1-bit input: Divided Clock
  .D({din_serdes[0],
      din_serdes[1],
      din_serdes[2],
      din_serdes[3],
      din_serdes[4],
      din_serdes[5],
      din_serdes[6],
      din_serdes[7]}),                     // 8-bit input: Parallel Data Input
  .RST(rst),                        // 1-bit input: Asynchronous Reset
  .T(din_t)                         // 1-bit input: Tristate input from fabric
  );

/* output io buffer */
generate

if( LVDS == 0 ) begin

  OBUFT OBUFT_inst (
    .O(dout_p),     
    .I(dout),     
    .T(dout_t));
    
  assign dout_n = 0;
  
end else begin

  if( SWAP_DIFF == 1 ) begin
    
  OBUFTDS OBUFTDS_inst (
    .O(dout_n),              
    .OB(dout_p),         
    .I(dout),          
    .T(dout_t));  
      
  end else begin

  OBUFTDS OBUFTDS_inst (
    .O(dout_p),  
    .OB(dout_n),  
    .I(dout),  
    .T(dout_t));     
        
    end
end

endgenerate
    
endmodule
