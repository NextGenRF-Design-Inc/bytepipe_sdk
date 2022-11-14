`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name:          adrv9002.v
// 
// Description: 
//
// This module instantiates a receive channel for interfacing to the 
// ADRV9002.  
//
// ***************************************************************************
// ***************************************************************************

module axi_adrv9001#(
  parameter DEFAULT_DGPIO_DIR       = 16'hffff,
  parameter TX1_REF_CLK_SRC         = "TX1_CLK",
  parameter TX2_REF_CLK_SRC         = "TX2_CLK",  
  parameter SWAP_DIFF_TX1_IDATA     = 1,        
  parameter SWAP_DIFF_TX1_QDATA     = 0,       
  parameter SWAP_DIFF_TX1_STROBE    = 1,       
  parameter SWAP_DIFF_TX1_DCLK_IN   = 1,        
  parameter SWAP_DIFF_TX1_DCLK_OUT  = 1, 
  parameter SWAP_DIFF_TX2_IDATA     = 0,        
  parameter SWAP_DIFF_TX2_QDATA     = 0,       
  parameter SWAP_DIFF_TX2_STROBE    = 0,       
  parameter SWAP_DIFF_TX2_DCLK_IN   = 0,        
  parameter SWAP_DIFF_TX2_DCLK_OUT  = 0,     
  parameter SWAP_DIFF_RX1_IDATA     = 0,      
  parameter SWAP_DIFF_RX1_QDATA     = 0,    
  parameter SWAP_DIFF_RX1_STROBE    = 0,       
  parameter SWAP_DIFF_RX1_DCLK      = 0,  
  parameter SWAP_DIFF_RX2_IDATA     = 1,      
  parameter SWAP_DIFF_RX2_QDATA     = 1,    
  parameter SWAP_DIFF_RX2_STROBE    = 1,       
  parameter SWAP_DIFF_RX2_DCLK      = 0,
  parameter ENABLE_TX1_ILA          = 0,
  parameter ENABLE_TX2_ILA          = 0,
  parameter ENABLE_RX1_ILA          = 0,
  parameter ENABLE_RX2_ILA          = 0,
  parameter ENABLE_SPI_ILA          = 0,
  parameter ENABLE_PL_DGPIO         = 0,    
  parameter ENABLE_PL_RX1_ENABLE    = 0,   
  parameter ENABLE_PL_RX2_ENABLE    = 0,   
  parameter ENABLE_PL_TX1_ENABLE    = 0,   
  parameter ENABLE_PL_TX2_ENABLE    = 0
  )(
  output wire           rx1_en,
  output wire           rx2_en,
  output wire           tx1_en,
  output wire           tx2_en,
  output wire           rstn,
  input  wire           irq,
  output wire [15:0]    dgpio_o,
  input  wire [15:0]    dgpio_i,
  output wire [15:0]    dgpio_t,
    
  input  wire           rx1_dclk_p,          
  input  wire           rx1_dclk_n,       
  input  wire           rx1_strobe_p, 
  input  wire           rx1_strobe_n,    
  input  wire           rx1_idata_p, 
  input  wire           rx1_idata_n,      
  input  wire           rx1_qdata_p,  
  input  wire           rx1_qdata_n,     
  
  output wire [31:0]    rx1_axis_tdata,    
  output wire           rx1_axis_tvalid, 
  input  wire           rx1_axis_tready,  
  output wire           rx1_axis_aclk, 
    
  input  wire           rx2_dclk_p,          
  input  wire           rx2_dclk_n,       
  input  wire           rx2_strobe_p, 
  input  wire           rx2_strobe_n,    
  input  wire           rx2_idata_p, 
  input  wire           rx2_idata_n,      
  input  wire           rx2_qdata_p,  
  input  wire           rx2_qdata_n,     
  
  output wire [31:0]    rx2_axis_tdata,    
  output wire           rx2_axis_tvalid, 
  input  wire           rx2_axis_tready,
  output wire           rx2_axis_aclk, 

  input  wire           tx1_ref_clk_p,     
  input  wire           tx1_ref_clk_n,       
  output wire           tx1_dclk_p,  
  output wire           tx1_dclk_n,      
  output wire           tx1_strobe_p,   
  output wire           tx1_strobe_n,    
  output wire           tx1_idata_p,     
  output wire           tx1_idata_n,       
  output wire           tx1_qdata_p,    
  output wire           tx1_qdata_n, 
   
  input  wire [31:0]    tx1_axis_tdata,  
  output wire           tx1_axis_tready,    
  input  wire           tx1_axis_tvalid,   
  output wire           tx1_axis_aclk,  

  input  wire           tx2_ref_clk_p,     
  input  wire           tx2_ref_clk_n,  
  output wire           tx2_dclk_p,  
  output wire           tx2_dclk_n,      
  output wire           tx2_strobe_p,   
  output wire           tx2_strobe_n,    
  output wire           tx2_idata_p,     
  output wire           tx2_idata_n,       
  output wire           tx2_qdata_p,    
  output wire           tx2_qdata_n, 
  
  input  wire [31:0]    tx2_axis_tdata,  
  output wire           tx2_axis_tready,    
  input  wire           tx2_axis_tvalid,   
  output wire           tx2_axis_aclk,
  
  input  wire           rx1_pl_en,
  input  wire           rx2_pl_en,
  input  wire           tx1_pl_en,
  input  wire           tx2_pl_en,  
  
  input  wire [15:0]    dgpio_pl_o,
  output wire [15:0]    dgpio_pl_i,
    
  output wire           spi_clk,
  input  wire           spi_miso,
  output wire           spi_mosi,
  output wire           spi_csn,
  output wire           spi_irq,  
  
  output wire           pl_irq,  
  
  input  wire           s_axi_aclk,
  input  wire           s_axi_aresetn,
  input  wire [6:0]     s_axi_awaddr,
  input  wire [2:0]     s_axi_awprot,
  input  wire           s_axi_awvalid,
  output wire           s_axi_awready,
  input  wire [31:0]    s_axi_wdata,   
  input  wire [3:0]     s_axi_wstrb,
  input  wire           s_axi_wvalid,
  output wire           s_axi_wready,
  output wire [1:0]     s_axi_bresp,
  output wire           s_axi_bvalid,
  input  wire           s_axi_bready,
  input  wire [6:0]     s_axi_araddr,
  input  wire [2:0]     s_axi_arprot,
  input  wire           s_axi_arvalid,
  output wire           s_axi_arready,
  output wire [31:0]    s_axi_rdata,
  output wire [1:0]     s_axi_rresp,
  output wire           s_axi_rvalid,
  input  wire           s_axi_rready
);


wire [15:0]   dgpio_ps_t;
wire [15:0]   dgpio_ps_o;
wire [15:0]   dgpio_ps_i;

wire [7:0]    mspi_axis_tdata;
wire          mspi_axis_tvalid;
wire          mspi_axis_tready;  
wire [7:0]    sspi_axis_tdata;
wire          sspi_axis_tvalid;
wire          sspi_axis_tready;
wire          mspi_axis_enable;

wire          rx1_enable;
wire          rx2_enable;
wire          tx1_enable;
wire          tx2_enable;

wire          tx1_enable_mode;
wire          tx2_enable_mode;
wire          rx1_enable_mode;
wire          rx2_enable_mode;

wire [15:0]   tx1_enable_delay;
wire [15:0]   tx2_enable_delay;
wire [15:0]   rx1_enable_delay;
wire [15:0]   rx2_enable_delay;
wire [15:0]   tx1_disable_delay;
wire [15:0]   tx2_disable_delay;
wire [15:0]   rx1_disable_delay;
wire [15:0]   rx2_disable_delay;
wire          tx1_data_src;
wire          tx2_data_src;
wire [31:0]   tx1_ps_data;
wire [31:0]   tx2_ps_data;
wire [31:0]   rx1_ps_data;
wire [31:0]   rx2_ps_data;

wire [63:0]   tx1_dbg;
wire [63:0]   tx2_dbg;
wire [31:0]   rx1_dbg;
wire [31:0]   rx2_dbg;


assign dgpio_t = dgpio_ps_t;
assign dgpio_ps_i = dgpio_i;

assign pl_irq = irq;

generate

  if( ENABLE_PL_DGPIO ) begin
    assign dgpio_o = dgpio_ps_o | dgpio_pl_o;
    assign dgpio_pl_i = dgpio_i;
  end else begin
    assign dgpio_o = dgpio_ps_o;
    assign dgpio_pl_i = 'h0;
  end  
  
  if( ENABLE_TX1_ILA) begin
      
    adrv9001_axis_ila tx1_ila(  
      .clk(s_axi_aclk),
      .enable_mode(tx1_enable_mode),
      .pl_en(tx1_pl_en),      
      .adrv9001_enable(tx1_en),      
      .s_axis_tdata(tx1_dbg[31:0]),
      .s_axis_tvalid(tx1_axis_tvalid),
      .s_axis_tready(tx1_axis_tready),
      .dgpio_i(dgpio_i),
      .dgpio_o(dgpio_o),
      .dgpio_t(dgpio_t),
      .enable_cnt(tx1_dbg[63:48]),
      .disable_cnt(tx1_dbg[47:32])      
      );  
  end
  
  if( ENABLE_TX2_ILA) begin
    adrv9001_axis_ila tx2_ila(  
      .clk(s_axi_aclk),
      .enable_mode(tx2_enable_mode),
      .pl_en(tx2_pl_en),      
      .adrv9001_enable(tx2_en),      
      .s_axis_tdata(tx2_dbg[31:0]),
      .s_axis_tvalid(tx2_axis_tvalid),
      .s_axis_tready(tx2_axis_tready),
      .dgpio_i(dgpio_i),
      .dgpio_o(dgpio_o),
      .dgpio_t(dgpio_t),
      .enable_cnt(tx2_dbg[63:48]),
      .disable_cnt(tx2_dbg[47:32])        
      ); 
  end  
  
  if( ENABLE_RX1_ILA) begin
    adrv9001_axis_ila rx1_ila(  
      .clk(s_axi_aclk),
      .enable_mode(rx1_enable_mode),
      .pl_en(rx1_pl_en),      
      .adrv9001_enable(rx1_en),      
      .s_axis_tdata(rx1_axis_tdata),
      .s_axis_tvalid(rx1_axis_tvalid),
      .s_axis_tready(rx1_axis_tready),
      .dgpio_i(dgpio_i),
      .dgpio_o(dgpio_o),
      .dgpio_t(dgpio_t),
      .enable_cnt(rx1_dbg[31:16]),
      .disable_cnt(rx1_dbg[15:0])  
      ); 
  end 
  
  if( ENABLE_RX2_ILA) begin
    adrv9001_axis_ila rx2_ila(  
      .clk(s_axi_aclk),
      .enable_mode(rx2_enable_mode),
      .pl_en(rx2_pl_en),      
      .adrv9001_enable(rx2_en),      
      .s_axis_tdata(rx2_axis_tdata),
      .s_axis_tvalid(rx2_axis_tvalid),
      .s_axis_tready(rx2_axis_tready),
      .dgpio_i(dgpio_i),
      .dgpio_o(dgpio_o),
      .dgpio_t(dgpio_t),
      .enable_cnt(rx2_dbg[31:16]),
      .disable_cnt(rx2_dbg[15:0])  
      );
  end  
  
  if( ENABLE_SPI_ILA) begin     
    ila_spi ila_spi_i (
      .clk(s_axi_aclk), // input wire clk
      .probe0(spi_csn), // input wire [0:0]  probe0  
      .probe1(spi_mosi), // input wire [0:0]  probe1 
      .probe2(spi_miso), // input wire [0:0]  probe2 
      .probe3(spi_clk), // input wire [0:0]  probe3
      .probe4(mspi_axis_tvalid), // input wire [0:0]  probe4 
      .probe5(mspi_axis_tready), // input wire [0:0]  probe5 
      .probe6(mspi_axis_tdata), // input wire [7:0]  probe6 
      .probe7(sspi_axis_tvalid), // input wire [0:0]  probe7 
      .probe8(sspi_axis_tready), // input wire [0:0]  probe8 
      .probe9(sspi_axis_tdata), // input wire [7:0]  probe9        
      .probe10(spi_irq) // input wire [0:0]  probe10
    );      
  end      
    
  if( ENABLE_PL_RX1_ENABLE ) begin
    assign rx1_enable = rx1_pl_en;
  end else begin
    assign rx1_enable = 0;     
  end  
  
  if( ENABLE_PL_RX2_ENABLE ) begin
    assign rx2_enable = rx2_pl_en;
  end else begin
    assign rx2_enable = 0;     
  end  

  if( ENABLE_PL_TX1_ENABLE ) begin
    assign tx1_enable = tx1_pl_en;
  end else begin
    assign tx1_enable = 0;     
  end  
  
  if( ENABLE_PL_TX2_ENABLE ) begin
    assign tx2_enable = tx2_pl_en;
  end else begin
    assign tx2_enable = 0;     
  end   
  
endgenerate


adrv9001_mspi adrv9001_mspi_inst (
  .clk(s_axi_aclk),                
  .spi_clk(spi_clk),
  .spi_miso(spi_miso),       
  .spi_mosi(spi_mosi),                 
  .spi_csn(spi_csn),
  .spi_done(spi_irq),
  .s_axis_tdata(mspi_axis_tdata),
  .s_axis_tvalid(mspi_axis_tvalid),
  .s_axis_tready(mspi_axis_tready),  
  .s_axis_enable(mspi_axis_enable),
  .m_axis_tdata(sspi_axis_tdata),
  .m_axis_tvalid(sspi_axis_tvalid),
  .m_axis_tready(sspi_axis_tready)
);
   
adrv9001_regs#(
    .DEFAULT_DGPIO_DIR(DEFAULT_DGPIO_DIR)
    )
    adrv9001_regs_i (
    .s_axi_aclk(s_axi_aclk),
    .s_axi_aresetn(s_axi_aresetn),
    .s_axi_awaddr(s_axi_awaddr),
    .s_axi_awprot(s_axi_awprot),
    .s_axi_awvalid(s_axi_awvalid),
    .s_axi_awready(s_axi_awready),
    .s_axi_wdata(s_axi_wdata),
    .s_axi_wstrb(s_axi_wstrb),
    .s_axi_wvalid(s_axi_wvalid),
    .s_axi_wready(s_axi_wready),
    .s_axi_bresp(s_axi_bresp),
    .s_axi_bvalid(s_axi_bvalid),
    .s_axi_bready(s_axi_bready),
    .s_axi_araddr(s_axi_araddr),
    .s_axi_arprot(s_axi_arprot),
    .s_axi_arvalid(s_axi_arvalid),
    .s_axi_arready(s_axi_arready),
    .s_axi_rdata(s_axi_rdata),
    .s_axi_rresp(s_axi_rresp),
    .s_axi_rvalid(s_axi_rvalid),
    .s_axi_rready(s_axi_rready),
    
    .tx1_enable_mode(tx1_enable_mode),
    .tx2_enable_mode(tx2_enable_mode),
    .rx1_enable_mode(rx1_enable_mode),
    .rx2_enable_mode(rx2_enable_mode),    
    
    
    .adrv9001_rstn(rstn),
    
    .tx1_enable_delay(tx1_enable_delay),
    .tx2_enable_delay(tx2_enable_delay),
    .rx1_enable_delay(rx1_enable_delay),      
    .rx2_enable_delay(rx2_enable_delay),
    
    .tx1_disable_delay(tx1_disable_delay),
    .tx2_disable_delay(tx2_disable_delay),
    .rx1_disable_delay(rx1_disable_delay),      
    .rx2_disable_delay(rx2_disable_delay),
    
    .dgpio_ps_t(dgpio_ps_t),
    .dgpio_ps_i(dgpio_ps_i),
    .dgpio_ps_o(dgpio_ps_o),    
    
    .tx1_data_src(tx1_data_src),
    .tx2_data_src(tx2_data_src),
    
    .tx1_ps_data(tx1_ps_data),
    .tx2_ps_data(tx2_ps_data),
    
    .rx1_ps_data(rx1_ps_data),
    .rx2_ps_data(rx2_ps_data),
    
    .mspi_axis_tdata(mspi_axis_tdata),
    .mspi_axis_tvalid(mspi_axis_tvalid),
    .mspi_axis_tready(mspi_axis_tready),  
    .mspi_axis_enable(mspi_axis_enable),
    .sspi_axis_tdata(sspi_axis_tdata),
    .sspi_axis_tvalid(sspi_axis_tvalid),
    .sspi_axis_tready(sspi_axis_tready)    
);
  
wire rx1_dclk;
wire rx1_dclk_div;
  
adrv9001_clk_in#(
  .SWAP_DIFF_CLK_IN(SWAP_DIFF_RX1_DCLK)      
  )
rx1_clk_inst(  
  .rst(1'b0),              
  .clk_p(rx1_dclk_p),
  .clk_n(rx1_dclk_n),   
  .clk(rx1_dclk),                   
  .clk_div(rx1_dclk_div)         
);  
 
adrv9001_rx#(
    .DBG_EN( ENABLE_RX1_ILA ),
    .SWAP_DIFF_IDATA(SWAP_DIFF_RX1_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_RX1_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_RX1_STROBE)
) adrv9001_rx1_inst (
    .dclk(rx1_dclk),
    .dclk_div(rx1_dclk_div),
    .adrv9001_strobe_p(rx1_strobe_p),
    .adrv9001_strobe_n(rx1_strobe_n),
    .adrv9001_idata_p(rx1_idata_p),
    .adrv9001_idata_n(rx1_idata_n),
    .adrv9001_qdata_p(rx1_qdata_p),
    .adrv9001_qdata_n(rx1_qdata_n),
    .adrv9001_enable(rx1_en),
    
    .enable(rx1_enable),
    .enable_mode(rx1_enable_mode),
    .enable_delay(rx1_enable_delay),
    .disable_delay(rx1_disable_delay),     
    
    .m_axis_tdata(rx1_axis_tdata),
    .m_axis_tvalid(rx1_axis_tvalid),
    .m_axis_aclk(rx1_axis_aclk),
    
    .m_axi_data( rx1_ps_data),
    .dbg( rx1_dbg ) 
);
  
  
wire rx2_dclk;
wire rx2_dclk_div;
  
adrv9001_clk_in#(
  .SWAP_DIFF_CLK_IN(SWAP_DIFF_RX2_DCLK)      
  )
rx2_clk_inst(  
  .rst(1'b0),              
  .clk_p(rx2_dclk_p),
  .clk_n(rx2_dclk_n),   
  .clk(rx2_dclk),                   
  .clk_div(rx2_dclk_div)         
);   
  
adrv9001_rx #(
    .DBG_EN( ENABLE_RX2_ILA ),
    .SWAP_DIFF_IDATA(SWAP_DIFF_RX2_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_RX2_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_RX2_STROBE)
) adrv9001_rx2_inst (
    .dclk(rx2_dclk),
    .dclk_div(rx2_dclk_div),
    .adrv9001_strobe_p(rx2_strobe_p),
    .adrv9001_strobe_n(rx2_strobe_n),
    .adrv9001_idata_p(rx2_idata_p),
    .adrv9001_idata_n(rx2_idata_n),
    .adrv9001_qdata_p(rx2_qdata_p),
    .adrv9001_qdata_n(rx2_qdata_n),
    .adrv9001_enable(rx2_en),
    
    .enable(rx2_enable),
    .enable_mode(rx2_enable_mode),    
    .enable_delay(rx2_enable_delay),
    .disable_delay(rx2_disable_delay),     
    
    .m_axis_tdata(rx2_axis_tdata),
    .m_axis_tvalid(rx2_axis_tvalid),
    .m_axis_aclk(rx2_axis_aclk),
    
    .m_axi_data( rx2_ps_data),    
    .dbg( rx2_dbg )  
);  


wire tx1_dclk;
wire tx1_dclk_div;

generate

  if (TX1_REF_CLK_SRC == "TX1_CLK") begin
  
    adrv9001_clk_in#(
      .SWAP_DIFF_CLK_IN(SWAP_DIFF_TX1_DCLK_IN)      
      )
    tx1_clk_inst(  
      .rst(1'b0),              
      .clk_p(tx1_ref_clk_p),
      .clk_n(tx1_ref_clk_n),   
      .clk(tx1_dclk),                   
      .clk_div(tx1_dclk_div)         
    );  
   
  end else if (TX1_REF_CLK_SRC == "RX1_CLK") begin
  
    assign tx1_dclk     = rx1_dclk;
    assign tx1_dclk_div = rx1_dclk_div;
    
  end else if (TX1_REF_CLK_SRC == "RX2_CLK") begin
  
    assign tx1_dclk     = rx2_dclk;
    assign tx1_dclk_div = rx2_dclk_div;

  end    
  
endgenerate 

adrv9001_tx #(
    .LVDS_OUTPUT(1),
    .DBG_EN( ENABLE_TX1_ILA ),
    .SWAP_DIFF_IDATA(SWAP_DIFF_TX1_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_TX1_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_TX1_STROBE),
    .SWAP_DIFF_CLK_OUT(SWAP_DIFF_TX1_DCLK_OUT)
) adrv9001_tx1_inst (
    .dclk(tx1_dclk),
    .dclk_div(tx1_dclk_div),
    .adrv9001_dclk_p(tx1_dclk_p),
    .adrv9001_dclk_n(tx1_dclk_n),
    .adrv9001_strobe_p(tx1_strobe_p),
    .adrv9001_strobe_n(tx1_strobe_n),
    .adrv9001_idata_p(tx1_idata_p),
    .adrv9001_idata_n(tx1_idata_n),
    .adrv9001_qdata_p(tx1_qdata_p),
    .adrv9001_qdata_n(tx1_qdata_n),
    .adrv9001_enable(tx1_en),
    
    .enable(tx1_enable),
    .enable_mode(tx1_enable_mode),    
    .enable_delay(tx1_enable_delay),
    .disable_delay(tx1_disable_delay),
  
    .s_axis_tdata(tx1_axis_tdata),
    .s_axis_tready(tx1_axis_tready),
    .s_axis_tvalid(tx1_axis_tvalid),
    .s_axis_aclk(tx1_axis_aclk),
    
    .s_axi_data(tx1_ps_data),
    .data_src(tx1_data_src), 

    .dbg( tx1_dbg )     
);
  
  
  
  
wire tx2_dclk;
wire tx2_dclk_div;

generate

  if (TX2_REF_CLK_SRC == "TX2_CLK") begin
  
    adrv9001_clk_in#(
      .SWAP_DIFF_CLK_IN(SWAP_DIFF_TX2_DCLK_IN)      
      )
    tx2_clk_inst(  
      .rst(1'b0),              
      .clk_p(tx2_ref_clk_p),
      .clk_n(tx2_ref_clk_n),   
      .clk(tx2_dclk),                   
      .clk_div(tx2_dclk_div)         
    );  
   
  end else if (TX2_REF_CLK_SRC == "RX1_CLK") begin
  
    assign tx2_dclk     = rx1_dclk;
    assign tx2_dclk_div = rx1_dclk_div;
    
  end else if (TX2_REF_CLK_SRC == "RX2_CLK") begin
  
    assign tx2_dclk     = rx2_dclk;
    assign tx2_dclk_div = rx2_dclk_div;

  end    
  
endgenerate 
  
adrv9001_tx #(
    .LVDS_OUTPUT(1),
    .DBG_EN( ENABLE_TX2_ILA ),    
    .SWAP_DIFF_IDATA(SWAP_DIFF_TX2_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_TX2_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_TX2_STROBE),
    .SWAP_DIFF_CLK_OUT(SWAP_DIFF_TX2_DCLK_OUT) 
) adrv9001_tx2_inst (
    .dclk(tx2_dclk),
    .dclk_div(tx2_dclk_div),
    .adrv9001_dclk_p(tx2_dclk_p),
    .adrv9001_dclk_n(tx2_dclk_n),    
    .adrv9001_strobe_p(tx2_strobe_p),
    .adrv9001_strobe_n(tx2_strobe_n),
    .adrv9001_idata_p(tx2_idata_p),
    .adrv9001_idata_n(tx2_idata_n),
    .adrv9001_qdata_p(tx2_qdata_p),
    .adrv9001_qdata_n(tx2_qdata_n),
    .adrv9001_enable(tx2_en),   

    .enable(tx2_enable),
    .enable_mode(tx2_enable_mode),    
    .enable_delay(tx2_enable_delay),
    .disable_delay(tx2_disable_delay),
    
    .s_axis_tdata(tx2_axis_tdata),
    .s_axis_tready(tx2_axis_tready),
    .s_axis_tvalid(tx2_axis_tvalid),
    .s_axis_aclk(tx2_axis_aclk),
    
    .s_axi_data(tx2_ps_data),
    .data_src(tx2_data_src),

    .dbg( tx2_dbg )    
);



  
endmodule
