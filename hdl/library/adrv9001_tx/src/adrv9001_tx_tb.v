`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_tx_tb.v
// 
// Description: 
//
// This module is a testbench for adrv9001_tx.
//
// 
// ***************************************************************************
// ***************************************************************************

module adrv9001_tx_tb( );

parameter CLK_MHZ = 300;    
parameter DATA_CLK_MHZ = 20;   


reg         rst = 1;
reg         clk = 0;
reg         ref_clk_p = 0;
reg         ref_clk_n = 0;

wire        dclk_p;
wire        dclk_n;
wire        strobe_p;
wire        strobe_n;
wire        idata_p;
wire        idata_n;
wire        qdata_p;
wire        qdata_n;


wire 			  s_axis_tready;
reg  [31:0]	s_axis_tdata = 0;
reg         s_axis_tvalid = 0;
  
adrv9001_tx#(
  .DEVICE("ULTRASCALE_PLUS"),     // Device Type (7SERIES, ULTRASCALE, ULTRASCALE_PLUS, ULTRASCALE_PLUS_ES1, ULTRASCALE_PLUS_ES2)
  .INVERT_DIFF_IDATA(1),         // Invert diff pair allowing physical connection of P signal to N pin and N signal to P pin
  .INVERT_DIFF_QDATA(0),         // Invert diff pair allowing physical connection of P signal to N pin and N signal to P pin
  .INVERT_DIFF_STROBE(0),        // Invert diff pair allowing physical connection of P signal to N pin and N signal to P pin
  .INVERT_DIFF_DCLK(0)           // Invert diff pair allowing physical connection of P signal to N pin and N signal to P pin  
  )
adrv9001_tx_inst (
  .clk(clk),
	.rst(rst),                      // Asynchronous reset
  
  .ref_clk_p(ref_clk_p),          // 1-bit data clock input(connect directly to top-level port)
  .ref_clk_n(ref_clk_n),          // 1-bit data clock input(connect directly to top-level port)  
  
  .dclk_p(dclk_p),                // 1-bit data clock output(connect directly to top-level port)
  .dclk_n(dclk_n),                // 1-bit data clock output(connect directly to top-level port)  
  .strobe_p(strobe_p),            // 1-bit strobe output(connect directly to top-level port)
  .strobe_n(strobe_n),            // 1-bit strobe output(connect directly to top-level port)
  .idata_p(idata_p),              // 1-bit data output(connect directly to top-level port)
  .idata_n(idata_n),              // 1-bit data output(connect directly to top-level port)
  .qdata_p(qdata_p),              // 1-bit data output(connect directly to top-level port)
  .qdata_n(qdata_n),              // 1-bit data output(connect directly to top-level port)
  
  .s_axis_tready(s_axis_tready),         
  .s_axis_tdata(s_axis_tdata),                 
  .s_axis_tvalid(s_axis_tvalid)
);

       
initial begin
  #((1000 * 1000)/(CLK_MHZ));
  rst = 0;
  
  #((999 * 1000)/(CLK_MHZ));
  s_axis_tvalid = 1;  

  s_axis_tdata = 32'h12345678;
  #(1000/(CLK_MHZ));
  s_axis_tdata = 32'haa015502;
  #(1000/(CLK_MHZ));
  s_axis_tdata = 32'h32324545;
  #(1000/(CLK_MHZ));  
  
  s_axis_tvalid = 0;
    
end

always begin
  ref_clk_p = ~ref_clk_p;
  ref_clk_n = ~ref_clk_p;
   
  #(1000/(2*DATA_CLK_MHZ));
end    



always begin
  clk = ~clk;
  #(1000/(2*CLK_MHZ));
end

    
endmodule
