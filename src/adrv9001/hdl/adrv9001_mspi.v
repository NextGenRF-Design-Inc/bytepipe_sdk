`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/07/2022 11:34:54 AM
// Design Name: 
// Module Name: adrv9001_mspi
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


module adrv9001_mspi(
  input  wire           clk,
  output reg            spi_clk = 'b1,
  input  wire           spi_miso,
  output reg            spi_mosi = 'b1,
  output reg            spi_csn = 'b1,
  
  input  wire           s_axis_enable,
  output reg            spi_done = 1'b0,
  
  input  wire [7:0]     s_axis_tdata,
  input  wire           s_axis_tvalid,
  output wire           s_axis_tready,
  
  output wire  [7:0]    m_axis_tdata,
  output wire           m_axis_tvalid,
  input  wire           m_axis_tready
  
    );
    
reg  [5:0]  cnt = 0;    
reg         mosiRead = 0;
wire        sclkFalling;
wire        sclkRising;
wire [2:0]  bitCnt;
wire [7:0]  mosiData;
reg  [7:0]  misoData;
reg         misoWrite;
wire        mosiEmpty;
    
    
assign sclkFalling = spi_clk & ~cnt[1] & ~cnt[0];
assign sclkRising = ~spi_clk & ~cnt[1] & ~cnt[0];
    
assign bitCnt = cnt[5:3];


always @(posedge clk) begin

  if( mosiEmpty & sclkFalling )
    spi_csn <= 1'b1;
  else if( ~mosiEmpty & s_axis_enable )
    spi_csn <= 1'b0;  
  else
    spi_csn <= spi_csn;
  
  if( spi_csn )
    spi_clk <= 1'b1;
  else if( cnt[1:0] == 'd0 )
    spi_clk <= ~spi_clk;
  else
    spi_clk <= spi_clk;      

  if( spi_csn )
    cnt <= 6'h3f;
  else
    cnt <= cnt - 1'b1; 

  if( sclkFalling ) 
    spi_mosi <= mosiData[bitCnt];    
  else
    spi_mosi <= spi_mosi;
  
  if( sclkFalling && (bitCnt == 'd0))
    mosiRead <= 1'b1;
  else
    mosiRead <= 1'b0;
    
  if( sclkRising )
    misoData <= {misoData[6:0],spi_miso};
  else
    misoData <= misoData;    
    
  if( sclkRising && (bitCnt == 'd0) )
    misoWrite <= 1'b1;
  else
    misoWrite <= 1'b0;      
    
  if( mosiEmpty & sclkFalling )
    spi_done <= 1'b1;    
  else if( misoEmpty )
    spi_done <= 1'b0;
  else
    spi_done <= spi_done;
    
end

wire mosiFull;
wire misoEmpty;

assign s_axis_tready = !mosiFull;
assign m_axis_tvalid = ~misoEmpty;

fifo fifo_mosi_inst (
  .clk(clk),                  // input wire clk
  .srst(1'b0),                // input wire srst
  .din(s_axis_tdata),         // input wire [7 : 0] din
  .wr_en(s_axis_tvalid),      // input wire wr_en
  .rd_en(mosiRead),           // input wire rd_en
  .dout(mosiData),            // output wire [7 : 0] dout
  .full(mosiFull),            // output wire full
  .empty(mosiEmpty),          // output wire empty
  .wr_rst_busy(),             // output wire wr_rst_busy
  .rd_rst_busy()              // output wire rd_rst_busy
);

fifo fifo_miso_inst (
  .clk(clk),                  // input wire clk
  .srst(1'b0),                // input wire srst
  .din(misoData),             // input wire [7 : 0] din
  .wr_en(misoWrite),          // input wire wr_en
  .rd_en(m_axis_tready),      // input wire rd_en
  .dout(m_axis_tdata),        // output wire [7 : 0] dout
  .full(),                    // output wire full
  .empty(misoEmpty),          // output wire empty
  .wr_rst_busy(),             // output wire wr_rst_busy
  .rd_rst_busy()              // output wire rd_rst_busy
);





    
    
    
    
endmodule
