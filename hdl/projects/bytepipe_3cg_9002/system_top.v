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

  wire adrv9001_spi_io0_i;
  wire adrv9001_spi_io0_o;
  wire adrv9001_spi_io0_t;
  wire adrv9001_spi_io1_i;
  wire adrv9001_spi_io1_o;
  wire adrv9001_spi_io1_t;
  wire adrv9001_spi_sck_i;
  wire adrv9001_spi_sck_o;
  wire adrv9001_spi_sck_t;
  wire adrv9001_spi_ss_i;
  wire adrv9001_spi_ss_o;
  wire adrv9001_spi_ss_t;

IOBUF adrv9001_spi_io0_iobuf (
  .I(adrv9001_spi_io0_o),
  .IO(adrv9001_miso),
  .O(adrv9001_spi_io0_i),
  .T(adrv9001_spi_io0_t)
);
  
IOBUF adrv9001_spi_io1_iobuf(
  .I(adrv9001_spi_io1_o),
  .IO(adrv9001_mosi),
  .O(adrv9001_spi_io1_i),
  .T(adrv9001_spi_io1_t)
);
  
IOBUF adrv9001_spi_sck_iobuf(
  .I(adrv9001_spi_sck_o),
  .IO(adrv9001_sclk),
  .O(adrv9001_spi_sck_i),
  .T(adrv9001_spi_sck_t)
);

IOBUF adrv9001_spi_ss_iobuf(
  .I(adrv9001_spi_ss_o),
  .IO(adrv9001_csn),
  .O(adrv9001_spi_ss_i),
  .T(adrv9001_spi_ss_t)
);
        
system system_i(
  .adrv9001_dgpio(adrv9001_dgpio),
  .adrv9001_irq(adrv9001_irq),
  .adrv9001_rstn(adrv9001_rstn),
  .adrv9001_rx1_dclk_n(adrv9001_rx1_dclk_n),
  .adrv9001_rx1_dclk_p(adrv9001_rx1_dclk_p),
  .adrv9001_rx1_en(adrv9001_rx1_en),
  .adrv9001_rx1_idata_n(adrv9001_rx1_idata_n),
  .adrv9001_rx1_idata_p(adrv9001_rx1_idata_p),
  .adrv9001_rx1_qdata_n(adrv9001_rx1_qdata_n),
  .adrv9001_rx1_qdata_p(adrv9001_rx1_qdata_p),
  .adrv9001_rx1_strobe_n(adrv9001_rx1_strobe_n),
  .adrv9001_rx1_strobe_p(adrv9001_rx1_strobe_p),
  .adrv9001_rx2_dclk_n(adrv9001_rx2_dclk_n),
  .adrv9001_rx2_dclk_p(adrv9001_rx2_dclk_p),
  .adrv9001_rx2_en(adrv9001_rx2_en),
  .adrv9001_rx2_idata_n(adrv9001_rx2_idata_n),
  .adrv9001_rx2_idata_p(adrv9001_rx2_idata_p),
  .adrv9001_rx2_qdata_n(adrv9001_rx2_qdata_n),
  .adrv9001_rx2_qdata_p(adrv9001_rx2_qdata_p),
  .adrv9001_rx2_strobe_n(adrv9001_rx2_strobe_n),
  .adrv9001_rx2_strobe_p(adrv9001_rx2_strobe_p),
  .adrv9001_spi_io0_i(adrv9001_spi_io0_i),
  .adrv9001_spi_io0_o(adrv9001_spi_io0_o),
  .adrv9001_spi_io0_t(adrv9001_spi_io0_t),
  .adrv9001_spi_io1_i(adrv9001_spi_io1_i),
  .adrv9001_spi_io1_o(adrv9001_spi_io1_o),
  .adrv9001_spi_io1_t(adrv9001_spi_io1_t),
  .adrv9001_spi_sck_i(adrv9001_spi_sck_i),
  .adrv9001_spi_sck_o(adrv9001_spi_sck_o),
  .adrv9001_spi_sck_t(adrv9001_spi_sck_t),
  .adrv9001_spi_ss_i(adrv9001_spi_ss_i),
  .adrv9001_spi_ss_o(adrv9001_spi_ss_o),
  .adrv9001_spi_ss_t(adrv9001_spi_ss_t),
  .adrv9001_tx1_dclk_n(adrv9001_tx1_dclk_n),
  .adrv9001_tx1_dclk_p(adrv9001_tx1_dclk_p),
  .adrv9001_tx1_en(adrv9001_tx1_en),
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
  .adrv9001_tx2_en(adrv9001_tx2_en),
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
