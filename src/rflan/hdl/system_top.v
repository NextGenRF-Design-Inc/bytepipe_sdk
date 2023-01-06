`timescale 1ns/100ps

module system_top (
  output wire             adrv9001_sclk,
  output wire             adrv9001_mosi,
  input  wire             adrv9001_miso,
  output wire             adrv9001_csn,

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
  output wire             adrv9001_tx2_strobe_p,
  input  wire             adrv9001_dev_clk_in
);       
          
wire [15:0] dgpio_i;
wire [15:0] dgpio_t;
wire [15:0] dgpio_o;
         
genvar n;
generate
  for (n = 0; n < 12; n = n + 1) begin: dgpio_iobuf

    IOBUF IOBUF_inst (
       .O(dgpio_i[n]),   
       .I(dgpio_o[n]), 
       .IO(adrv9001_dgpio[n]), 
       .T(dgpio_t[n])    
    );

  end
endgenerate
          

assign dgpio_i[15:12] = 0;
          
system system_i
   (.adrv9001_spi_csn(adrv9001_csn),
    .adrv9001_dgpio_t(dgpio_t),
    .adrv9001_dgpio_i(dgpio_i),
    .adrv9001_dgpio_o(dgpio_o),    
    .adrv9001_irq(adrv9001_irq),
    .adrv9001_spi_miso(adrv9001_miso),
    .adrv9001_spi_mosi(adrv9001_mosi),
    .adrv9001_rstn(adrv9001_rstn),
    .adrv9001_dev_clk_in(adrv9001_dev_clk_in),   
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
    .adrv9001_spi_clk(adrv9001_sclk),
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
