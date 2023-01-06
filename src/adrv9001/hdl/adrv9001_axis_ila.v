`timescale 1ps/1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name:          adrv9001_axis_ila.v
// 
// Description: 
//
// This module instantiates an integrated logic analyzer
//
// ***************************************************************************
// ***************************************************************************

module adrv9001_axis_ila(
  input  wire         clk,
  input  wire         pl_en,
  input  wire         adrv9001_enable,
  input  wire [31:0]  s_axis_tdata,
  input  wire         s_axis_tvalid,
  input  wire         s_axis_tready,
  input  wire         s_axis_tlast,
  input  wire [15:0]  dgpio_i,
  input  wire [15:0]  dgpio_o,
  input  wire [15:0]  dgpio_t,
  input  wire [15:0]  enable_cnt,
  input  wire [15:0]  disable_cnt,
  input  wire [9:0]   capture_control_cnt
);

wire [31:0] s_axis_tdata_cdc;
wire        adrv9001_enable_cdc;
wire        pl_en_cdc;
wire        s_axis_tvalid_cdc;
wire        s_axis_tready_cdc;
wire        s_axis_tlast_cdc;
wire [15:0] dgpio_monitor_cdc;
wire [15:0] dgpio_monitor;
wire [15:0] enable_cnt_cdc;
wire [15:0] disable_cnt_cdc;

genvar n;
generate
  for (n = 0; n < 16; n = n + 1) begin: dgpio_iobuf
    assign dgpio_monitor[n] = (dgpio_t[n] == 1'b1) ? dgpio_i[n] : dgpio_o[n];
  end
endgenerate

cdc #(
  .DATA_WIDTH(32) )
iqdata_cdc_i (
  .s_cdc_tdata  ( s_axis_tdata ),
  .m_cdc_clk    (clk),
  .m_cdc_tdata  (s_axis_tdata_cdc )
);

cdc #(
  .DATA_WIDTH(16) )
dgpio_cdc_i (
  .s_cdc_tdata  ( dgpio_monitor ),
  .m_cdc_clk    (clk),
  .m_cdc_tdata  ( dgpio_monitor_cdc )
);

cdc #(
  .DATA_WIDTH(16) )
enable_cnt_cdc_i (
  .s_cdc_tdata  ( enable_cnt ),
  .m_cdc_clk    (clk),
  .m_cdc_tdata  ( enable_cnt_cdc )
);

cdc #(
  .DATA_WIDTH(16) )
disable_cnt_cdc_i (
  .s_cdc_tdata  ( disable_cnt ),
  .m_cdc_clk    (clk),
  .m_cdc_tdata  ( disable_cnt_cdc )
);

cdc #(
  .DATA_WIDTH(5) )
ctrl_cdc_i (
  .s_cdc_tdata  ( { pl_en, adrv9001_enable, s_axis_tvalid, s_axis_tready, s_axis_tlast } ),
  .m_cdc_clk    (clk),
  .m_cdc_tdata  ( { pl_en_cdc, adrv9001_enable_cdc, s_axis_tvalid_cdc, s_axis_tready_cdc, s_axis_tlast_cdc }  )
);

reg  [9:0]  capture_control_cntr = 0;

wire capture_control_trig = (capture_control_cntr >= capture_control_cnt); 

always @(posedge clk) begin
  if( capture_control_trig )
    capture_control_cntr <= 0;
  else
    capture_control_cntr <= capture_control_cntr + 1;
end

ila_axis ila_axis_i (
  .clk(clk), // input wire clk
  .probe0(adrv9001_enable_cdc), // input wire [0:0]  probe0  
  .probe1(s_axis_tvalid_cdc), // input wire [0:0]  probe1 
  .probe2(s_axis_tready_cdc), // input wire [0:0]  probe2 
  .probe3(s_axis_tdata_cdc[31:16]), // input wire [15:0]  probe3 
  .probe4(s_axis_tdata_cdc[15:0]), // input wire [15:0]  probe4
  .probe5(dgpio_monitor_cdc),
  .probe6(pl_en_cdc),
  .probe7(enable_cnt_cdc),
  .probe8(disable_cnt_cdc),
  .probe9(capture_control_trig),
  .probe10(s_axis_tlast_cdc)
);

endmodule
