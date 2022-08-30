`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9002.v
// 
// Description: 
//
// This module instantiates a receive channel for interfacing to the 
// ADRV9002.  
//
// ***************************************************************************
// ***************************************************************************

module axi_adrv9001#(
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
  parameter ENABLE_RX2_ILA          = 0      
  )(
  output wire           rx1_en,
  output wire           rx2_en,
  output wire           tx1_en,
  output wire           tx2_en,
  output wire           rstn,
  input  wire           irq,
  inout  wire [11:0]    dgpio,
    
  input   wire          rx1_dclk_p,          
  input   wire			rx1_dclk_n,       
  input   wire			rx1_strobe_p, 
  input   wire			rx1_strobe_n,    
  input   wire			rx1_idata_p, 
  input   wire        	rx1_idata_n,      
  input   wire        	rx1_qdata_p,  
  input   wire		    rx1_qdata_n,     
  
  output  wire [31:0]	rx1_axis_tdata,    
  output  wire          rx1_axis_tvalid, 
  output  wire          rx1_axis_aclk,
  output  wire          rx1_axis_rstn,  
    
  input   wire          rx2_dclk_p,          
  input   wire			rx2_dclk_n,       
  input   wire			rx2_strobe_p, 
  input   wire			rx2_strobe_n,    
  input   wire			rx2_idata_p, 
  input   wire        	rx2_idata_n,      
  input   wire        	rx2_qdata_p,  
  input   wire		    rx2_qdata_n,     
  
  output  wire [31:0]	rx2_axis_tdata,    
  output  wire          rx2_axis_tvalid, 
  output  wire          rx2_axis_aclk,
  output  wire          rx2_axis_rstn,  

  input   wire          tx1_ref_clk_p,     
  input   wire          tx1_ref_clk_n,       
  output  wire          tx1_dclk_p,  
  output  wire			tx1_dclk_n,      
  output  wire			tx1_strobe_p,   
  output  wire			tx1_strobe_n,    
  output  wire			tx1_idata_p,     
  output  wire        	tx1_idata_n,       
  output  wire        	tx1_qdata_p,    
  output  wire			tx1_qdata_n, 
   
  input   wire [31:0]	tx1_axis_tdata,  
  output  wire          tx1_axis_tready,    
  input   wire          tx1_axis_tvalid,   
  output  wire          tx1_axis_aclk,    
  output  wire          tx1_axis_rstn,  

  input   wire          tx2_ref_clk_p,     
  input   wire          tx2_ref_clk_n,  
  output  wire          tx2_dclk_p,  
  output  wire			tx2_dclk_n,      
  output  wire			tx2_strobe_p,   
  output  wire			tx2_strobe_n,    
  output  wire			tx2_idata_p,     
  output  wire        	tx2_idata_n,       
  output  wire        	tx2_qdata_p,    
  output  wire			tx2_qdata_n, 
  
  input   wire [31:0]	tx2_axis_tdata,  
  output  wire          tx2_axis_tready,    
  input   wire          tx2_axis_tvalid,   
  output  wire          tx2_axis_aclk,        
  output  wire          tx2_axis_rstn,
  
  input  wire           rx1_pl_en,
  input  wire           rx2_pl_en,
  input  wire           tx1_pl_en,
  input  wire           tx2_pl_en,  
  
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
   
  
  
wire            tx1_tdd_en;
wire            tx2_tdd_en;
wire            rx1_tdd_en;
wire            rx2_tdd_en;      
wire            tx1_pl_en_cdc;
wire            tx2_pl_en_cdc;
wire            rx1_pl_en_cdc;
wire            rx2_pl_en_cdc;   
wire [31:0]     tx1_disable_cnt;
wire [31:0]     tx1_ssi_enable_cnt;
wire [31:0]     tx2_disable_cnt;
wire [31:0]     tx2_ssi_enable_cnt;
wire [31:0]     rx1_disable_cnt;
wire [31:0]     rx1_ssi_enable_cnt;
wire [31:0]     rx1_ssi_disable_cnt;
wire [31:0]     rx2_disable_cnt;
wire [31:0]     rx2_ssi_enable_cnt;
wire [31:0]     rx2_ssi_disable_cnt;

wire            tx1_data_src;
wire            tx2_data_src;
wire [31:0]     tx1_tdata;
wire [31:0]     tx2_tdata;
wire [31:0]     rx1_tdata;
wire [31:0]     rx2_tdata;

wire [11:0]     dgpio_t;
wire [11:0]     dgpio_o;
wire [11:0]     dgpio_i;


/* GPIO */
genvar n;
generate
  for (n = 0; n < 12; n = n + 1) begin: dgpio_io
    assign dgpio[n] = (dgpio_t[n] == 1'b1) ? 1'bz : dgpio_o[n];
    assign dgpio_i[n] = dgpio[n];        
  end
endgenerate


generate
  
  if( ENABLE_TX1_ILA) begin
    adrv9001_axis_ila tx1_ila(  
      .clk(s_axi_aclk),
      .enable(tx1_en),
      .tdata(tx1_axis_tdata),
      .tvalid(tx1_axis_tvalid),
      .tready(tx1_axis_tready)
      );  
  end
  
  if( ENABLE_TX2_ILA) begin
    adrv9001_axis_ila tx2_ila(  
      .clk(s_axi_aclk),
      .enable(tx2_en),
      .tdata(tx2_axis_tdata),
      .tvalid(tx2_axis_tvalid),
      .tready(tx2_axis_tready)
      );  
  end  
  
  if( ENABLE_RX1_ILA) begin
    adrv9001_axis_ila rx1_ila(  
      .clk(s_axi_aclk),
      .enable(rx1_en),
      .tdata(rx1_axis_tdata),
      .tvalid(rx1_axis_tvalid),
      .tready(1'b0)
      );  
  end 
  
  if( ENABLE_RX2_ILA) begin
    adrv9001_axis_ila rx2_ila(  
      .clk(s_axi_aclk),
      .enable(rx2_en),
      .tdata(rx2_axis_tdata),
      .tvalid(rx2_axis_tvalid),
      .tready(1'b0)
      );  
  end    
  
endgenerate

cdc #(
  .DATA_WIDTH(4) )
pl_en_cdc_i (
  .s_cdc_tdata  ({rx1_pl_en, rx2_pl_en, tx1_pl_en, tx2_pl_en}),
  .m_cdc_clk    (s_axi_aclk),
  .m_cdc_tdata  ({rx1_pl_en_cdc, rx2_pl_en_cdc, tx1_pl_en_cdc, tx2_pl_en_cdc})
);
 
adrv9001_regs adrv9001_regs_i (
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
    .rx1_tdd_en(rx1_tdd_en),
    .rx2_tdd_en(rx2_tdd_en),
    .tx1_tdd_en(tx1_tdd_en),
    .tx2_tdd_en(tx2_tdd_en),
    .tx1_disable_cnt(tx1_disable_cnt),
    .tx1_ssi_enable_cnt(tx1_ssi_enable_cnt),
    .tx2_disable_cnt(tx2_disable_cnt),      
    .tx2_ssi_enable_cnt(tx2_ssi_enable_cnt),
    .rx1_disable_cnt(rx1_disable_cnt),
    .rx1_ssi_enable_cnt(rx1_ssi_enable_cnt),
    .rx1_ssi_disable_cnt(rx1_ssi_disable_cnt),
    .rx2_disable_cnt(rx2_disable_cnt), 
    .rx2_ssi_enable_cnt(rx2_ssi_enable_cnt),
    .rx2_ssi_disable_cnt(rx2_ssi_disable_cnt),
    .rstn(rstn),
    .gpio_i(dgpio_i),
    .gpio_o(dgpio_o),
    .gpio_t(dgpio_t),        
    .tx1_data_src(tx1_data_src),
    .tx2_data_src(tx2_data_src),
    .tx1_data(tx1_tdata),
    .tx2_data(tx2_tdata),
    .rx1_data(rx1_tdata),
    .rx2_data(rx2_tdata)
);
  
adrv9001_rx#(
    .SWAP_DIFF_IDATA(SWAP_DIFF_RX1_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_RX1_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_RX1_STROBE),
    .SWAP_DIFF_DCLK(SWAP_DIFF_RX1_DCLK)
) adrv9001_rx1_inst (
    .dclk_p(rx1_dclk_p),
    .dclk_n(rx1_dclk_n),
    .strobe_p(rx1_strobe_p),
    .strobe_n(rx1_strobe_n),
    .idata_p(rx1_idata_p),
    .idata_n(rx1_idata_n),
    .qdata_p(rx1_qdata_p),
    .qdata_n(rx1_qdata_n),
    .tdata(rx1_tdata),     
    .enable(rx1_en),
    .tdd_en(rx1_tdd_en | rx1_pl_en_cdc),
    .disable_cnt(rx1_disable_cnt), 
    .ssi_enable_cnt(rx1_ssi_enable_cnt),
    .ssi_disable_cnt(rx1_ssi_disable_cnt),      
    .m_axis_tdata(rx1_axis_tdata),
    .m_axis_tvalid(rx1_axis_tvalid),
    .m_axis_aclk(rx1_axis_aclk),
    .m_axis_rstn(rx1_axis_rstn)
);
  
adrv9001_rx #(
    .SWAP_DIFF_IDATA(SWAP_DIFF_RX2_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_RX2_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_RX2_STROBE),
    .SWAP_DIFF_DCLK(SWAP_DIFF_RX2_DCLK)
) adrv9001_rx2_inst (
    .dclk_p(rx2_dclk_p),
    .dclk_n(rx2_dclk_n),
    .strobe_p(rx2_strobe_p),
    .strobe_n(rx2_strobe_n),
    .idata_p(rx2_idata_p),
    .idata_n(rx2_idata_n),
    .qdata_p(rx2_qdata_p),
    .qdata_n(rx2_qdata_n),
    .tdata(rx2_tdata),  
    .enable(rx2_en),
    .tdd_en(rx2_tdd_en | rx2_pl_en_cdc),
    .disable_cnt(rx2_disable_cnt), 
    .ssi_enable_cnt(rx2_ssi_enable_cnt),
    .ssi_disable_cnt(rx2_ssi_disable_cnt),     
    .m_axis_tdata(rx2_axis_tdata),
    .m_axis_tvalid(rx2_axis_tvalid),
    .m_axis_aclk(rx2_axis_aclk),
    .m_axis_rstn(rx2_axis_rstn)    
);  

adrv9001_tx #(
    .LVDS_OUTPUT(1),
    .SWAP_DIFF_IDATA(SWAP_DIFF_TX1_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_TX1_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_TX1_STROBE),
    .SWAP_DIFF_DCLK_IN(SWAP_DIFF_TX1_DCLK_IN),
    .SWAP_DIFF_DCLK_OUT(SWAP_DIFF_TX1_DCLK_OUT)
) adrv9001_tx1_inst (
    .ref_clk_p(tx1_ref_clk_p),
    .ref_clk_n(tx1_ref_clk_n),
    .dclk_p(tx1_dclk_p),
    .dclk_n(tx1_dclk_n),
    .strobe_p(tx1_strobe_p),
    .strobe_n(tx1_strobe_n),
    .idata_p(tx1_idata_p),
    .idata_n(tx1_idata_n),
    .qdata_p(tx1_qdata_p),
    .qdata_n(tx1_qdata_n),
    .tdata(tx1_tdata),
    .data_src(tx1_data_src),
    .tdd_en(tx1_tdd_en | tx1_pl_en_cdc),
    .enable(tx1_en),
    .disable_cnt(tx1_disable_cnt),
    .ssi_enable_cnt(tx1_ssi_enable_cnt),    
    .s_axis_tdata(tx1_axis_tdata),
    .s_axis_tready(tx1_axis_tready),
    .s_axis_tvalid(tx1_axis_tvalid),
    .s_axis_aclk(tx1_axis_aclk),
    .s_axis_rstn(tx1_axis_rstn)
);
  
adrv9001_tx #(
    .LVDS_OUTPUT(1),
    .SWAP_DIFF_IDATA(SWAP_DIFF_TX2_IDATA),
    .SWAP_DIFF_QDATA(SWAP_DIFF_TX2_QDATA),
    .SWAP_DIFF_STROBE(SWAP_DIFF_TX2_STROBE),
    .SWAP_DIFF_DCLK_IN(SWAP_DIFF_TX2_DCLK_IN),
    .SWAP_DIFF_DCLK_OUT(SWAP_DIFF_TX2_DCLK_OUT)
) adrv9001_tx2_inst (
    .ref_clk_p(tx2_ref_clk_p),
    .ref_clk_n(tx2_ref_clk_n),
    .dclk_p(tx2_dclk_p),
    .dclk_n(tx2_dclk_n),
    .strobe_p(tx2_strobe_p),
    .strobe_n(tx2_strobe_n),
    .idata_p(tx2_idata_p),
    .idata_n(tx2_idata_n),
    .qdata_p(tx2_qdata_p),
    .qdata_n(tx2_qdata_n),
    .tdata(tx2_tdata),
    .data_src(tx2_data_src),    
    .tdd_en(tx2_tdd_en | tx2_pl_en_cdc),
    .enable(tx2_en),
    .disable_cnt(tx2_disable_cnt),
    .ssi_enable_cnt(tx2_ssi_enable_cnt),        
    .s_axis_tdata(tx2_axis_tdata),
    .s_axis_tready(tx2_axis_tready),
    .s_axis_tvalid(tx2_axis_tvalid),
    .s_axis_aclk(tx2_axis_aclk),
    .s_axis_rstn(tx2_axis_rstn)
);
	
endmodule
