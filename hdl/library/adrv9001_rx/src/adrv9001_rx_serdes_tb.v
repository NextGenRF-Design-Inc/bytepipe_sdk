`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/07/2020 10:38:41 AM
// Design Name: 
// Module Name: adrv9001_rx_serdes_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module adrv9001_rx_serdes_tb(

    );
    
parameter DLY_CLK_MHZ = 300;    
parameter DATA_CLK_MHZ = 20;    
parameter DEVICE = "ULTRASCALE_PLUS"; // 7SERIES, ULTRASCALE_PLUS

parameter CTRL_DEVICE = DEVICE == "7SERIES"? "7SERIES" : "ULTRASCALE";
     
reg dly_clk = 0;
reg dly_rst = 1;
wire dly_rdy;
reg dclk = 0;
reg clk = 0;
reg rst = 1;
reg idata_p = 0;
reg idata_n = 0;
wire [7:0] i_data;
    
    
    
IDELAYCTRL #(
.SIM_DEVICE(CTRL_DEVICE)      
)
IDELAYCTRL_inst (
.RDY(dly_rdy),                    // 1-bit output: Ready output
.REFCLK(dly_clk),                 // 1-bit input: Reference clock input
.RST()                     // 1-bit input: Active high reset input. Asynchronous assert, synchronous deassert to REFCLK.
);

/* I Data Serdes */
adrv9001_rx_serdes #(
	.DELAY(0),                // 9bit fixed delay in ps.
  .DELAY_CLK_MHZ(DLY_CLK_MHZ),  // IDELAYCTRL clock input frequency in MHz (200.0-2667.0)
  .DEVICE(DEVICE)                 // Device Type (7SERIES, ULTRASCALE, ULTRASCALE_PLUS, ULTRASCALE_PLUS_ES1, ULTRASCALE_PLUS_ES2)
)
i_serdes (
  .clk(clk),                      // clock
  .dclk(dclk),                    // data or divided clock
  .rst(rst),
  .din_p(idata_p),                // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .din_n(idata_n),                // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .dout(i_data)                   // Parallel data output
);    
    
initial begin
  #50;   
  dly_rst = 0;  
  #200;
  rst = 0;  
end

always begin
  dclk = ~dclk;
  #(4000/(2*DATA_CLK_MHZ));
end

always begin
  clk = ~clk;
  idata_p = $random;  
  idata_n = ~idata_p;
  #(1000/(2*DATA_CLK_MHZ));
end

always begin
  dly_clk = ~dly_clk;
  #(1000/(2*DLY_CLK_MHZ));
end
    
endmodule
