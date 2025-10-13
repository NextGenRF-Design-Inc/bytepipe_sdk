`timescale 1ns / 1ps
/***************************************************************************//**
 *  \file       axi_dma_trig.v
 *
 *  \details
 *
 *  \copyright
 *
 *  Copyright 2021(c) NextGen RF Design, Inc.  
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *   - Use of the software either in source or binary form, must be run on or
 *     directly connected to a NextGen RF Design, Inc. product.
 *
 *  THIS SOFTWARE IS PROVIDED BY NEXTGEN RF DESIGN "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 *  EVENT SHALL NEXTGEN RF DESIGN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************/
module axi_dma_trig(
    input  wire           axis_aclk,
    input  wire [31:0]    s_axis_tdata,
    input  wire           s_axis_tvalid,
    output wire [31:0]    m_axis_tdata,
    output wire           m_axis_tvalid,

    input  wire           ext_trig,
    input  wire           arm,
                 
    input  wire           s_axi_aclk,
    input  wire           s_axi_aresetn,
    input  wire [7:0]     s_axi_awaddr,
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
    input  wire [7:0]     s_axi_araddr,
    input  wire [2:0]     s_axi_arprot,
    input  wire           s_axi_arvalid,
    output wire           s_axi_arready,
    output wire [31:0]    s_axi_rdata,
    output wire [1:0]     s_axi_rresp,
    output wire           s_axi_rvalid,
    input  wire           s_axi_rready
);

wire  [31:0]    trig_lvl;           
wire            trig_edge;    
wire  [1:0]     trig_type; 

axi_regs regs_i (
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
    
    .trig_lvl(trig_lvl),                  
    .trig_edge(trig_edge),    
    .trig_type(trig_type) 
);


wire [1:0] trig_type_cdc; 
xpm_cdc_array_single #(
  .DEST_SYNC_FF(3),  
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1),  
  .WIDTH(2)         
)
trig_type_cdc_i (
  .dest_out(trig_type_cdc),
  .dest_clk(axis_aclk),
  .src_clk(s_axi_aclk), 
  .src_in(trig_type) 
);

wire [31:0] trig_lvl_cdc; 
xpm_cdc_array_single #(
  .DEST_SYNC_FF(3),  
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1),  
  .WIDTH(32)         
)
trig_lvl_cdc_i (
  .dest_out(trig_lvl_cdc),
  .dest_clk(axis_aclk),
  .src_clk(s_axi_aclk), 
  .src_in(trig_lvl) 
);

wire            trig_edge_cdc;   
xpm_cdc_single #(
  .DEST_SYNC_FF(3),   
  .INIT_SYNC_FF(0),  
  .SIM_ASSERT_CHK(0),
  .SRC_INPUT_REG(1)  
)
trig_edge_cdc_i (
  .dest_out(trig_edge_cdc), 
  .dest_clk(axis_aclk), 
  .src_clk(s_axi_aclk), 
  .src_in(trig_edge) 
);

dma_trig dma_trig_i(
  .axis_aclk(axis_aclk),
  .s_axis_tdata(s_axis_tdata),
  .s_axis_tvalid(s_axis_tvalid),
  .threshold(trig_lvl_cdc),
  .arm(arm),
  .trig_edge(trig_edge_cdc),
  .trig_type(trig_type_cdc),
  .ext_trig(ext_trig),
  .m_axis_tdata(m_axis_tdata),
  .m_axis_tvalid(m_axis_tvalid)
);

endmodule


