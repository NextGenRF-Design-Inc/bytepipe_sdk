`timescale 1ns/100ps

module system_top (
  inout  wire             adrv9001_sclk,
  inout  wire             adrv9001_mosi,
  inout  wire             adrv9001_miso,
  inout  wire             adrv9001_csn,

  output wire             adrv9001_rstn,
  output wire             adrv9001_tx1_en,
  output wire             adrv9001_tx2_en,
  output wire             adrv9001_rx1_en,
  output wire             adrv9001_rx2_en,
  input  wire             adrv9001_irq,
  inout  wire [11:0]      adrv9001_dgpio,  
  
  input  wire             adrv9001_rx1_dclk_n,
  input  wire             adrv9001_rx1_dclk_p,
  input  wire             adrv9001_rx1_idata_n,
  input  wire             adrv9001_rx1_idata_p,
  input  wire             adrv9001_rx1_qdata_n,
  input  wire             adrv9001_rx1_qdata_p,
  input  wire             adrv9001_rx1_strobe_n,
  input  wire             adrv9001_rx1_strobe_p,

  input  wire             adrv9001_rx2_dclk_n,
  input  wire             adrv9001_rx2_dclk_p,
  input  wire             adrv9001_rx2_idata_n,
  input  wire             adrv9001_rx2_idata_p,
  input  wire             adrv9001_rx2_qdata_n,
  input  wire             adrv9001_rx2_qdata_p,
  input  wire             adrv9001_rx2_strobe_n,
  input  wire             adrv9001_rx2_strobe_p,

  output wire             adrv9001_tx1_dclk_n,
  output wire             adrv9001_tx1_dclk_p,
  input  wire             adrv9001_tx1_ref_clk_n,
  input  wire             adrv9001_tx1_ref_clk_p,
  output wire             adrv9001_tx1_idata_n,
  output wire             adrv9001_tx1_idata_p,
  output wire             adrv9001_tx1_qdata_n,
  output wire             adrv9001_tx1_qdata_p,
  output wire             adrv9001_tx1_strobe_n,
  output wire             adrv9001_tx1_strobe_p,
  
  output wire             adrv9001_tx2_dclk_n,
  output wire             adrv9001_tx2_dclk_p,
  input  wire             adrv9001_tx2_ref_clk_n,
  input  wire             adrv9001_tx2_ref_clk_p,
  output wire             adrv9001_tx2_idata_n,
  output wire             adrv9001_tx2_idata_p,
  output wire             adrv9001_tx2_qdata_n,
  output wire             adrv9001_tx2_qdata_p,
  output wire             adrv9001_tx2_strobe_n,
  output wire             adrv9001_tx2_strobe_p
);

wire [4:0]cpu_gpio_o;

assign adrv9001_rstn    = cpu_gpio_o[0];
assign adrv9001_tx1_en  = cpu_gpio_o[1];
assign adrv9001_tx2_en  = cpu_gpio_o[2];
assign adrv9001_rx1_en  = cpu_gpio_o[3];
assign adrv9001_rx2_en  = cpu_gpio_o[4];

wire [0:0]adrv9001_dgpio_tri_i_0;
wire [1:1]adrv9001_dgpio_tri_i_1;
wire [10:10]adrv9001_dgpio_tri_i_10;
wire [11:11]adrv9001_dgpio_tri_i_11;
wire [2:2]adrv9001_dgpio_tri_i_2;
wire [3:3]adrv9001_dgpio_tri_i_3;
wire [4:4]adrv9001_dgpio_tri_i_4;
wire [5:5]adrv9001_dgpio_tri_i_5;
wire [6:6]adrv9001_dgpio_tri_i_6;
wire [7:7]adrv9001_dgpio_tri_i_7;
wire [8:8]adrv9001_dgpio_tri_i_8;
wire [9:9]adrv9001_dgpio_tri_i_9;
wire [0:0]adrv9001_dgpio_tri_io_0;
wire [1:1]adrv9001_dgpio_tri_io_1;
wire [10:10]adrv9001_dgpio_tri_io_10;
wire [11:11]adrv9001_dgpio_tri_io_11;
wire [2:2]adrv9001_dgpio_tri_io_2;
wire [3:3]adrv9001_dgpio_tri_io_3;
wire [4:4]adrv9001_dgpio_tri_io_4;
wire [5:5]adrv9001_dgpio_tri_io_5;
wire [6:6]adrv9001_dgpio_tri_io_6;
wire [7:7]adrv9001_dgpio_tri_io_7;
wire [8:8]adrv9001_dgpio_tri_io_8;
wire [9:9]adrv9001_dgpio_tri_io_9;
wire [0:0]adrv9001_dgpio_tri_o_0;
wire [1:1]adrv9001_dgpio_tri_o_1;
wire [10:10]adrv9001_dgpio_tri_o_10;
wire [11:11]adrv9001_dgpio_tri_o_11;
wire [2:2]adrv9001_dgpio_tri_o_2;
wire [3:3]adrv9001_dgpio_tri_o_3;
wire [4:4]adrv9001_dgpio_tri_o_4;
wire [5:5]adrv9001_dgpio_tri_o_5;
wire [6:6]adrv9001_dgpio_tri_o_6;
wire [7:7]adrv9001_dgpio_tri_o_7;
wire [8:8]adrv9001_dgpio_tri_o_8;
wire [9:9]adrv9001_dgpio_tri_o_9;
wire [0:0]adrv9001_dgpio_tri_t_0;
wire [1:1]adrv9001_dgpio_tri_t_1;
wire [10:10]adrv9001_dgpio_tri_t_10;
wire [11:11]adrv9001_dgpio_tri_t_11;
wire [2:2]adrv9001_dgpio_tri_t_2;
wire [3:3]adrv9001_dgpio_tri_t_3;
wire [4:4]adrv9001_dgpio_tri_t_4;
wire [5:5]adrv9001_dgpio_tri_t_5;
wire [6:6]adrv9001_dgpio_tri_t_6;
wire [7:7]adrv9001_dgpio_tri_t_7;
wire [8:8]adrv9001_dgpio_tri_t_8;
wire [9:9]adrv9001_dgpio_tri_t_9;

IOBUF adrv9001_dgpio_tri_iobuf_0
   (.I(adrv9001_dgpio_tri_o_0),
    .IO(adrv9001_dgpio[0]),
    .O(adrv9001_dgpio_tri_i_0),
    .T(adrv9001_dgpio_tri_t_0));
IOBUF adrv9001_dgpio_tri_iobuf_1
   (.I(adrv9001_dgpio_tri_o_1),
    .IO(adrv9001_dgpio[1]),
    .O(adrv9001_dgpio_tri_i_1),
    .T(adrv9001_dgpio_tri_t_1));
IOBUF adrv9001_dgpio_tri_iobuf_2
   (.I(adrv9001_dgpio_tri_o_2),
    .IO(adrv9001_dgpio[2]),
    .O(adrv9001_dgpio_tri_i_2),
    .T(adrv9001_dgpio_tri_t_2));
IOBUF adrv9001_dgpio_tri_iobuf_3
   (.I(adrv9001_dgpio_tri_o_3),
    .IO(adrv9001_dgpio[3]),
    .O(adrv9001_dgpio_tri_i_3),
    .T(adrv9001_dgpio_tri_t_3));
IOBUF adrv9001_dgpio_tri_iobuf_4
   (.I(adrv9001_dgpio_tri_o_4),
    .IO(adrv9001_dgpio[4]),
    .O(adrv9001_dgpio_tri_i_4),
    .T(adrv9001_dgpio_tri_t_4));
IOBUF adrv9001_dgpio_tri_iobuf_5
   (.I(adrv9001_dgpio_tri_o_5),
    .IO(adrv9001_dgpio[5]),
    .O(adrv9001_dgpio_tri_i_5),
    .T(adrv9001_dgpio_tri_t_5));
IOBUF adrv9001_dgpio_tri_iobuf_6
   (.I(adrv9001_dgpio_tri_o_6),
    .IO(adrv9001_dgpio[6]),
    .O(adrv9001_dgpio_tri_i_6),
    .T(adrv9001_dgpio_tri_t_6));
IOBUF adrv9001_dgpio_tri_iobuf_7
   (.I(adrv9001_dgpio_tri_o_7),
    .IO(adrv9001_dgpio[7]),
    .O(adrv9001_dgpio_tri_i_7),
    .T(adrv9001_dgpio_tri_t_7));
IOBUF adrv9001_dgpio_tri_iobuf_8
   (.I(adrv9001_dgpio_tri_o_8),
    .IO(adrv9001_dgpio[8]),
    .O(adrv9001_dgpio_tri_i_8),
    .T(adrv9001_dgpio_tri_t_8));
IOBUF adrv9001_dgpio_tri_iobuf_9
   (.I(adrv9001_dgpio_tri_o_9),
    .IO(adrv9001_dgpio[9]),
    .O(adrv9001_dgpio_tri_i_9),
    .T(adrv9001_dgpio_tri_t_9));
IOBUF adrv9001_dgpio_tri_iobuf_10
   (.I(adrv9001_dgpio_tri_o_10),
    .IO(adrv9001_dgpio[10]),
    .O(adrv9001_dgpio_tri_i_10),
    .T(adrv9001_dgpio_tri_t_10));
IOBUF adrv9001_dgpio_tri_iobuf_11
   (.I(adrv9001_dgpio_tri_o_11),
    .IO(adrv9001_dgpio[11]),
    .O(adrv9001_dgpio_tri_i_11),
    .T(adrv9001_dgpio_tri_t_11));        
          
system system_i(
  .cpu_gpio_tri_i(5'h0),
  .cpu_gpio_tri_o(cpu_gpio_o),
  .cpu_gpio_tri_t(),
  .adrv9001_dgpio_tri_i({adrv9001_dgpio_tri_i_11,adrv9001_dgpio_tri_i_10,adrv9001_dgpio_tri_i_9,adrv9001_dgpio_tri_i_8,adrv9001_dgpio_tri_i_7,adrv9001_dgpio_tri_i_6,adrv9001_dgpio_tri_i_5,adrv9001_dgpio_tri_i_4,adrv9001_dgpio_tri_i_3,adrv9001_dgpio_tri_i_2,adrv9001_dgpio_tri_i_1,adrv9001_dgpio_tri_i_0}),
  .adrv9001_dgpio_tri_o({adrv9001_dgpio_tri_o_11,adrv9001_dgpio_tri_o_10,adrv9001_dgpio_tri_o_9,adrv9001_dgpio_tri_o_8,adrv9001_dgpio_tri_o_7,adrv9001_dgpio_tri_o_6,adrv9001_dgpio_tri_o_5,adrv9001_dgpio_tri_o_4,adrv9001_dgpio_tri_o_3,adrv9001_dgpio_tri_o_2,adrv9001_dgpio_tri_o_1,adrv9001_dgpio_tri_o_0}),
  .adrv9001_dgpio_tri_t({adrv9001_dgpio_tri_t_11,adrv9001_dgpio_tri_t_10,adrv9001_dgpio_tri_t_9,adrv9001_dgpio_tri_t_8,adrv9001_dgpio_tri_t_7,adrv9001_dgpio_tri_t_6,adrv9001_dgpio_tri_t_5,adrv9001_dgpio_tri_t_4,adrv9001_dgpio_tri_t_3,adrv9001_dgpio_tri_t_2,adrv9001_dgpio_tri_t_1,adrv9001_dgpio_tri_t_0}),
  .adrv9001_irq(adrv9001_irq),
  .adrv9001_rx1_dclk_n(adrv9001_rx1_dclk_n),
  .adrv9001_rx1_dclk_p(adrv9001_rx1_dclk_p),
  .adrv9001_rx1_idata_n(adrv9001_rx1_idata_n),
  .adrv9001_rx1_idata_p(adrv9001_rx1_idata_p),
  .adrv9001_rx1_qdata_n(adrv9001_rx1_qdata_n),
  .adrv9001_rx1_qdata_p(adrv9001_rx1_qdata_p),
  .adrv9001_rx1_strobe_n(adrv9001_rx1_strobe_n),
  .adrv9001_rx1_strobe_p(adrv9001_rx1_strobe_p),
  .adrv9001_rx2_dclk_n(adrv9001_rx2_dclk_n),
  .adrv9001_rx2_dclk_p(adrv9001_rx2_dclk_p),
  .adrv9001_rx2_idata_n(adrv9001_rx2_idata_n),
  .adrv9001_rx2_idata_p(adrv9001_rx2_idata_p),
  .adrv9001_rx2_qdata_n(adrv9001_rx2_qdata_n),
  .adrv9001_rx2_qdata_p(adrv9001_rx2_qdata_p),
  .adrv9001_rx2_strobe_n(adrv9001_rx2_strobe_n),
  .adrv9001_rx2_strobe_p(adrv9001_rx2_strobe_p),
  .adrv9001_csn(adrv9001_csn),
  .adrv9001_miso(adrv9001_miso),
  .adrv9001_mosi(adrv9001_mosi),
  .adrv9001_sclk(adrv9001_sclk),
  .adrv9001_tx1_dclk_n(adrv9001_tx1_dclk_n),
  .adrv9001_tx1_dclk_p(adrv9001_tx1_dclk_p),
  .adrv9001_tx1_idata_n(adrv9001_tx1_idata_n),
  .adrv9001_tx1_idata_p(adrv9001_tx1_idata_p),
  .adrv9001_tx1_qdata_n(adrv9001_tx1_qdata_n),
  .adrv9001_tx1_qdata_p(adrv9001_tx1_qdata_p),
  .adrv9001_tx1_ref_clk_n(adrv9001_tx1_ref_clk_n),
  .adrv9001_tx1_ref_clk_p(adrv9001_tx1_ref_clk_p),
  .adrv9001_tx1_strobe_n(adrv9001_tx1_strobe_n),
  .adrv9001_tx1_strobe_p(adrv9001_tx1_strobe_p),
  .adrv9001_tx2_dclk_n(adrv9001_tx2_dclk_n),
  .adrv9001_tx2_dclk_p(adrv9001_tx2_dclk_p),
  .adrv9001_tx2_idata_n(adrv9001_tx2_idata_n),
  .adrv9001_tx2_idata_p(adrv9001_tx2_idata_p),
  .adrv9001_tx2_qdata_n(adrv9001_tx2_qdata_n),
  .adrv9001_tx2_qdata_p(adrv9001_tx2_qdata_p),
  .adrv9001_tx2_ref_clk_n(adrv9001_tx2_ref_clk_n),
  .adrv9001_tx2_ref_clk_p(adrv9001_tx2_ref_clk_p),
  .adrv9001_tx2_strobe_n(adrv9001_tx2_strobe_n),
  .adrv9001_tx2_strobe_p(adrv9001_tx2_strobe_p)
);
        

endmodule

// ***************************************************************************
// ***************************************************************************
