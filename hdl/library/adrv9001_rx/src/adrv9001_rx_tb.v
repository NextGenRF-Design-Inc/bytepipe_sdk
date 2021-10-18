`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_rx_tb.v
// 
// Description: 
//
// This module is a testbench for adrv9001_rx.
//
// 
// ***************************************************************************
// ***************************************************************************

module adrv9001_rx_tb( );

parameter CLK_MHZ = 300;    
parameter DATA_CLK_MHZ = 20;   
parameter DATA_PHASE = 15;

reg [3:0]   strbCnt = 15 - DATA_PHASE - 2;
reg         rst = 1;
reg         dclk_p = 0;
reg         dclk_n = 0;
reg         strobe_p = 0;
reg         strobe_n = 0;
reg         idata_p = 0;
reg         idata_n = 0;
reg         qdata_p = 0;
reg         qdata_n = 0;
reg         clk = 0;
reg 			  m_axis_tready = 1;
wire [31:0]	m_axis_tdata;
wire        m_axis_tvalid;
  
adrv9001_rx#(
  .DEVICE("7SERIES"),     // Device Type (7SERIES, ULTRASCALE, ULTRASCALE_PLUS, ULTRASCALE_PLUS_ES1, ULTRASCALE_PLUS_ES2)
  .I_DELAY(0),                    // Delay in ps relative to rx_dclk_p/n
  .Q_DELAY(0),                    // Delay in ps relative to rx_dclk_p/n
  .STRB_DELAY(0),                 // Delay in ps relative to rx_dclk_p/n
  .CLK_MHZ(CLK_MHZ)               // IDELAYCTRL clock input frequency in MHz (200.0-2667.0) 
  )
adrv9001_rx_inst (
  .clk(clk),
	.rst(rst),                      // Asynchronous reset
  .dclk_p(dclk_p),                // 1-bit data clock input(connect directly to top-level port)
  .dclk_n(dclk_n),                // 1-bit data clock input(connect directly to top-level port)
  .strobe_p(strobe_p),            // 1-bit strobe input(connect directly to top-level port)
  .strobe_n(strobe_n),            // 1-bit strobe input(connect directly to top-level port)
  .idata_p(idata_p),              // 1-bit data input(connect directly to top-level port)
  .idata_n(idata_n),              // 1-bit data input(connect directly to top-level port)
  .qdata_p(qdata_p),              // 1-bit data input(connect directly to top-level port)
  .qdata_n(qdata_n),              // 1-bit data input(connect directly to top-level port)
  
  .m_axis_tready(m_axis_tready),         
  .m_axis_tdata(m_axis_tdata),                 
  .m_axis_tvalid(m_axis_tvalid)
);

       
initial begin
  #1000;
  rst = 0;
  
  #30000;
  m_axis_tready = 0;
  #1000;
  m_axis_tready = 1;
    
end

always begin
  dclk_p = ~dclk_p;
  dclk_n = ~dclk_p;
  strbCnt = strbCnt + 1;
  
  if(strbCnt == 0)
    strobe_p = 1;
  else
    strobe_p = 0;
    
  strobe_n = ~strobe_p;    
    
  idata_p = $random;
  qdata_p = $random;  
  
  idata_n = ~idata_p;
  qdata_n = ~qdata_p;  
  
  #(1000/(2*DATA_CLK_MHZ));
end    



always begin
  clk = ~clk;
  #(1000/(2*CLK_MHZ));
end

    
endmodule
