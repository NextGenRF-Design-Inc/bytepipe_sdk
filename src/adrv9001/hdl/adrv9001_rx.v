`timescale 1ns / 1ps
// ***************************************************************************
// ***************************************************************************
//        Copyright 2020 (c) NextGen RF Design. All rights reserved.
//
// This core is distributed WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Name: 			    adrv9001_rx.v
// 
// Description: 
//
// This module instantiates a receive channel for interfacing to the 
// ADRV9001/2.  
//
// ***************************************************************************
// ***************************************************************************

module adrv9001_rx#(
  parameter DBG_EN = 0,
  parameter SWAP_DIFF_IDATA = 0,          // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_QDATA = 0,          // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_STROBE = 0          // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )(
 
// ADRV9001 interface.  Connect directly to top-level port
  input  wire         dclk,   
  input  wire         dclk_div,       
  input  wire         adrv9001_strobe_p,     // SSI strobe pair
  input  wire         adrv9001_strobe_n,     
  input  wire         adrv9001_idata_p,      // SSI in-phase data pair
  input  wire         adrv9001_idata_n,      
  input  wire         adrv9001_qdata_p,      // SSI quadrature data pair
  input  wire         adrv9001_qdata_n,      
  output wire         adrv9001_enable,       // ADRV9001 receive enable
  
// User control interface
  input  wire         enable,                // Enable Receiver  
  input  wire         enable_mode,           // 0 = SPI enable, 1 = Pin enable
  input  wire [15:0]  enable_delay,          // Number of samples from rising edge of enable to rising edge of m_axis_tvalid
  input  wire [15:0]  disable_delay,         // Number of samples from falling edge of enable to falling edge of m_axis_tvalid

// User data interface      
  output wire [31:0]  m_axis_tdata,          // Received IQ data sample
  output wire         m_axis_tvalid,         // IQ data sample valid  
  output wire         m_axis_tlast,          // Last IQ sample before disable delay
  output wire         m_axis_aclk,
  output wire         m_axis_rstn,
  
  output reg  [31:0]  m_axi_data = 0,
  
  output wire [31:0]  dbg
    );



wire [7:0]  i_data;
wire [7:0]  q_data;
wire [7:0]  strobe;


wire [15:0] enable_delay_cdc;
wire [15:0] disable_delay_cdc;
wire        enable_pin_cdc;
wire        enable_mode_cdc;
reg         ssi_enable = 0;

     
           
/* I Data Serdes */
adrv9001_rx_serdes #(
  .SWAP_DIFF(SWAP_DIFF_IDATA)     // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
)
i_serdes (
  .rst(~ssi_enable),              // Reset
  .dclk_div(dclk_div),            // data or divided clock
  .dclk(dclk),                    // clock synchronous with data
  .din_p(adrv9001_idata_p),       // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .din_n(adrv9001_idata_n),       // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .dout(i_data)                   // Parallel data output
);

/* Q Data Serdes */
adrv9001_rx_serdes #(
  .SWAP_DIFF(SWAP_DIFF_QDATA)     // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
)
q_serdes (
  .rst(~ssi_enable),              // Reset
  .dclk_div(dclk_div),            // data or divided clock
  .dclk(dclk),                    // clock synchronous with data
  .din_p(adrv9001_qdata_p),       // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .din_n(adrv9001_qdata_n),       // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .dout(q_data)                   // Parallel data output
);

/* Strobe Serdes */
adrv9001_rx_serdes #(
  .SWAP_DIFF(SWAP_DIFF_STROBE)    // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
)
s_serdes (
  .rst(~ssi_enable),              // Reset
  .dclk_div(dclk_div),            // data or divided clock
  .dclk(dclk),                    // clock synchronous with data
  .din_p(adrv9001_strobe_p),      // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .din_n(adrv9001_strobe_n),      // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .dout(strobe)                   // Parallel data output
);


reg         ce = 0;

always @( posedge dclk_div ) begin
  ce <= ~ce;  
end


wire        rf_enable_mode_cdc;
reg         rf_enable = 0;
wire        enable_cdc;

cdc #(
  .DATA_WIDTH(1) )
enable_cdc_i (
  .s_cdc_tdata  (enable),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (enable_cdc)
);

cdc #(
  .DATA_WIDTH(1) )
ssi_enable_mode_cdc_i (
  .s_cdc_tdata  (enable_mode),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (rf_enable_mode_cdc)
);

assign adrv9001_enable = rf_enable;

always @( posedge dclk_div ) begin   
    
  if( enable_cdc && ( rf_enable_mode_cdc == 'b1) )
    rf_enable <= 1'b1;
  else 
    rf_enable <= 1'b0;       
    
end


wire [15:0] ssi_disable_delay_cdc;
wire [15:0] ssi_enable_delay_cdc;

cdc #(
  .DATA_WIDTH(16) )
ssi_disable_delay_cdc_i (
  .s_cdc_tdata  (disable_delay),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (ssi_disable_delay_cdc)
);

cdc #(
  .DATA_WIDTH(16) )
enable_delay_cdc_i (
  .s_cdc_tdata  (enable_delay),
  .m_cdc_clk    (dclk_div),
  .m_cdc_tdata  (ssi_enable_delay_cdc)
);

reg  [15:0] ssi_enable_cnt = 0;
reg  [15:0] ssi_disable_cnt = 0;

always @( posedge dclk_div ) begin   
  
  if( rf_enable_mode_cdc == 1'b0 )
    ssi_enable_cnt <= 16'h0;
  else if( (enable_cdc == 1'b0 ) && (ssi_disable_cnt == 16'h0))
     ssi_enable_cnt <= ssi_enable_delay_cdc;
  else if( (ssi_enable_cnt > 'h0) && ( ce == 1'b1) )
    ssi_enable_cnt <= ssi_enable_cnt - 'd1;
  else 
    ssi_enable_cnt <= ssi_enable_cnt; 

  if( (enable_cdc == 1'b1) || (rf_enable_mode_cdc == 1'b0) )
     ssi_disable_cnt <= ssi_disable_delay_cdc;
  else if( (ssi_disable_cnt > 'h0) && ( ce == 1'b1) )
    ssi_disable_cnt <= ssi_disable_cnt - 'd1;
  else 
    ssi_disable_cnt <= ssi_disable_cnt;     
    
  if( (ssi_enable_cnt == 16'h0) && (ssi_disable_cnt > 16'h0) )
    ssi_enable <= 1'b1;
  else 
    ssi_enable <= 1'b0;     
    
  m_axi_data <= {i_aligned, q_aligned};
    
end

wire [15:0] i_packed;
wire [15:0] q_packed;
wire [15:0] strb_packed;
wire        valid_packed;
wire [15:0] i_aligned;
wire [15:0] q_aligned;
wire        valid_aligned;
wire        last_aligned;

/* Pack Signals */
adrv9001_serdes_pack pack(
  .clk(dclk_div),                 // clock
  .i_in(i_data),                  // 8-bit i/q data input from serdes
  .q_in(q_data),                  // 8-bit i/q data input from serdes  
  .strb_in(strobe),               // 8-bit strobe input from serdes
  .i_out(i_packed),               // Packed output data
  .q_out(q_packed),               // Packed output data
  .strb_out(strb_packed),         // Packed strobe data
  .valid_out(valid_packed)        // Output data valid
);

/* Align Signals */
adrv9001_serdes_aligner align(
  .clk(dclk_div),                 // clock
  .rst(~ssi_enable),
  .i_in(i_packed),                // 16-bit i/q data input from serdes
  .q_in(q_packed),                // 16-bit i/q data input from serdes  
  .strb_in(strb_packed),          // 16-bit strobe input from serdes
  .valid_in(valid_packed),        // Valid input data
  .i_out(i_aligned),              // Packed output data
  .q_out(q_aligned),              // Packed output data
  .valid_out(valid_aligned),      // Output data valid
  .last_out(last_aligned) 
);

assign m_axis_tvalid = valid_aligned;
assign m_axis_tdata = {i_aligned, q_aligned};
assign m_axis_aclk = dclk_div;
assign m_axis_tlast = last_aligned;

generate

  if( DBG_EN ) begin
    assign dbg = {ssi_enable_cnt, ssi_disable_cnt};
  end else begin
    assign dbg = 0;
  end  
  
endgenerate
    
endmodule
