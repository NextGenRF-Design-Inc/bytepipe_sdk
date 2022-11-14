`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name:          adrv9001_tx.v
// 
// Description: 
//
// This module instantiates a transmit channel for interfacing to the 
// ADRV9001/2.  
//
// ***************************************************************************
// ***************************************************************************

module adrv9001_tx#(
  parameter DBG_EN = 0,
  parameter LVDS_OUTPUT = 1,                 // Output is LVDS or CMOS
  parameter SWAP_DIFF_IDATA = 0,             // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_QDATA = 0,             // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_STROBE = 0,            // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_CLK_OUT = 0            // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin  
  )(
  
// ADRV9001 interface.  Connect directly to top-level port  
  input  wire         dclk,     
  input  wire         dclk_div,
  
  output wire         adrv9001_dclk_p,       // Output data clock
  output wire         adrv9001_dclk_n,      
  output wire         adrv9001_strobe_p,     // SSI strobe pair
  output wire         adrv9001_strobe_n,    
  output wire         adrv9001_idata_p,      // SSI in-phase data pair
  output wire         adrv9001_idata_n,      
  output wire         adrv9001_qdata_p,      // SSI quadrature phase data pair
  output wire         adrv9001_qdata_n,     
  output wire         adrv9001_enable,       // ADRV9001 Transmit enable pin
    
// User control interface    
  input  wire         enable,                // Enable transmitter  
  input  wire         enable_mode,           // 0 = spi mode, 1 = pin mode
  input  wire [15:0]  disable_delay,         // Number of samples from falling edge of enable to falling edge of adrv9001_enable  
  input  wire [15:0]  enable_delay,          // Number of samples from rising edge of enable to rising edge of s_axis_tready
      
// User data interface        
  input  wire [31:0]  s_axis_tdata,          // IQ data to be transmitted
  output reg          s_axis_tready,         // Ready for new IQ data sample
  input  wire         s_axis_tvalid,         // Input IQ data valid
  output wire         s_axis_aclk,           // IQ data clock, 2x sample rate
  
  input  wire [31:0]  s_axi_data,
  input  wire         data_src,
  
  output wire [63:0]  dbg
    );


wire [7:0]  s_unpacked;
wire [7:0]  i_unpacked;
wire [7:0]  q_unpacked;
reg         ssi_enable = 0;

assign s_axis_aclk = dclk_div;

generate

  if( SWAP_DIFF_CLK_OUT == 1) begin    
    OBUFDS dclk_ds_buf (
      .O(adrv9001_dclk_n),             
      .OB(adrv9001_dclk_p),            
      .I(~dclk));                     
  end else begin  
    OBUFDS dclk_ds_buf (
      .O(adrv9001_dclk_p),                     
      .OB(adrv9001_dclk_n),                  
      .I(dclk));                         
 end  

endgenerate


adrv9001_tx_serdes#(
  .LVDS(LVDS_OUTPUT),
  .SWAP_DIFF(SWAP_DIFF_STROBE)      // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )
s_serdes(
  .rst(~ssi_enable),                // reset
  .dclk_div(dclk_div),              // data/divided clock synchronous to parallel input 
  .dclk(dclk),                      // data clock synchronous to output
  .dout_p(adrv9001_strobe_p),       // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .dout_n(adrv9001_strobe_n),       // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .din(s_unpacked),                 // Parallel data output
  .din_t(1'b0)                      // Data in tristate
);

adrv9001_tx_serdes#( 
  .LVDS(LVDS_OUTPUT),
  .SWAP_DIFF(SWAP_DIFF_IDATA)       // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )
i_serdes(
  .rst(~ssi_enable),                // reset
  .dclk_div(dclk_div),              // data/divided clock synchronous to parallel input 
  .dclk(dclk),                      // data clock synchronous to output
  .dout_p(adrv9001_idata_p),        // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .dout_n(adrv9001_idata_n),        // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .din(i_unpacked),                 // Parallel data output
  .din_t(1'b0)                      // Data in tristate
);

adrv9001_tx_serdes#( 
  .LVDS(LVDS_OUTPUT),
  .SWAP_DIFF(SWAP_DIFF_QDATA)       // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )
q_serdes(
  .rst(~ssi_enable),                // reset
  .dclk_div(dclk_div),              // data/divided clock synchronous to parallel input 
  .dclk(dclk),                      // data clock synchronous to output
  .dout_p(adrv9001_qdata_p),        // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .dout_n(adrv9001_qdata_n),        // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .din(q_unpacked),                 // Parallel data output
  .din_t(1'b0)                      // Data in tristate
);

reg         ce = 0;

always @( posedge dclk_div ) begin
  ce <= ~ce;  
end


wire        enable_cdc;

cdc #(
  .DATA_WIDTH(1) )
enable_cdc_i (
  .s_cdc_tdata  (enable),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (enable_cdc)
);


wire        rf_enable_mode_cdc;
wire [15:0] rf_disable_delay_cdc;
reg  [15:0] rf_disable_cnt = 0;
reg         rf_enable = 0;

cdc #(
  .DATA_WIDTH(1) )
enable_mode_cdc_i (
  .s_cdc_tdata  (enable_mode),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (rf_enable_mode_cdc)
);

cdc #(
  .DATA_WIDTH(16) )
disable_delay_cdc_i (
  .s_cdc_tdata  (disable_delay),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (rf_disable_delay_cdc)
);

assign adrv9001_enable = rf_enable;


always @( posedge dclk_div ) begin   

  if( rf_enable_mode_cdc == 1'b0 )
    rf_disable_cnt <= 16'h0;
  else if( enable_cdc )
     rf_disable_cnt <= rf_disable_delay_cdc;
  else if( (rf_disable_cnt > 'h0) && ( ce == 1'b1) )
    rf_disable_cnt <= rf_disable_cnt - 'd1;
  else 
    rf_disable_cnt <= rf_disable_cnt; 
    
  if( rf_disable_cnt > 16'h0 )
    rf_enable <= 1'b1;
  else 
    rf_enable <= 1'b0;       
    
end


wire [15:0] ssi_enable_delay_cdc;
reg  [15:0] ssi_enable_cnt = 0;



cdc #(
  .DATA_WIDTH(16) )
ssi_enable_delay_cdc_i (
  .s_cdc_tdata  (enable_delay),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (ssi_enable_delay_cdc)
);

always @( posedge dclk_div ) begin   
  
  if( rf_enable_mode_cdc == 1'b0 )
    ssi_enable_cnt <= 16'h0;
  else if( ( enable_cdc == 1'b0 ) & ( rf_enable == 1'b0 ) )
     ssi_enable_cnt <= ssi_enable_delay_cdc;
  else if( (ssi_enable_cnt > 'h0) && ( ce == 1'b1) )
    ssi_enable_cnt <= ssi_enable_cnt - 'd1;
  else 
    ssi_enable_cnt <= ssi_enable_cnt;    
    
  if(ssi_enable_cnt > 16'h0 )
    ssi_enable <= 1'b0;
  else 
    ssi_enable <= 1'b1;  
    
end


wire        data_src_cdc;
wire [31:0] s_axi_data_cdc;
reg  [31:0] unpack_din = 0;
wire        unpack_ready;

cdc #(
  .DATA_WIDTH(32))
s_axi_data_cdc_i (
  .s_cdc_tdata(s_axi_data),
  .m_cdc_clk  (dclk_div),
  .m_cdc_tdata(s_axi_data_cdc)
);

cdc #(
  .DATA_WIDTH(1) )
data_src_cdc_i (
  .s_cdc_tdata  (data_src),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (data_src_cdc)
);

always @( posedge dclk_div ) begin

  if( data_src_cdc )
    unpack_din <= s_axi_data_cdc;
  else if( ssi_enable == 'b0 )
    unpack_din <= 32'h00000000;
  else if( unpack_ready )
    unpack_din <= s_axis_tdata;  
  else
    unpack_din <= unpack_din;
    
  s_axis_tready <= unpack_ready;  
    
end    


adrv9001_serdes_unpack unpack_inst(
  .clk(dclk_div),                   // data clock
  .rst(~ssi_enable),                // Reset
  .din(unpack_din),                 // packed i/q data input 
  .din_rdy(unpack_ready),           // Ready for new data
  .strb_out(s_unpacked),            // 8-bit unpacked strobe output to serdes
  .i_out(i_unpacked),               // 8-bit unpacked i data output to serdes
  .q_out(q_unpacked)                // 8-bit unpacked q data output to serdes
);   


generate

  if( DBG_EN ) begin
    assign dbg = {ssi_enable_cnt, rf_disable_cnt, unpack_din};
  end else begin
    assign dbg = 0;
  end  
  
endgenerate
    
endmodule
