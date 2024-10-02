`timescale 1ns / 1ps
/***************************************************************************//**
 *  \file       adrv9001_tx.v
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

module adrv9001_tx#(
  parameter SWAP_DIFF_IDATA = 0,             // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_QDATA = 0,             // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_STROBE = 0,            // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_CLK_OUT = 0,           // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter ENABLE_OTX_AXIS = 0
  )(
  
  wire                ssi_clk,
  wire                ssi_clk_div,
  
// ADRV9001 interface.  Connect directly to top-level port      
  output wire         adrv9001_tx_dclk_p,       // Output data clock
  output wire         adrv9001_tx_dclk_n,      
  output wire         adrv9001_tx_strobe_p,     // SSI strobe pair
  output wire         adrv9001_tx_strobe_n,    
  output wire         adrv9001_tx_idata_p,      // SSI in-phase data pair
  output wire         adrv9001_tx_idata_n,      
  output wire         adrv9001_tx_qdata_p,      // SSI quadrature phase data pair
  output wire         adrv9001_tx_qdata_n,     
    
// User control interface    
  input  wire         enable,                // Enable transmitter
  input  wire         swap_iq,
  input  wire [2:0]   data_src,              // Data Source
  input  wire [31:0]  fixed_pattern,  
  
  output wire [31:0]  otx_axis_tdata,
  output wire         otx_axis_tvalid,
  
// User data interface        
  input  wire [31:0]  s_axis_tdata,          // IQ data to be transmitted
  output wire         s_axis_tready,         // Ready for new IQ data sample
  input  wire         s_axis_tvalid,         // Input IQ data valid
  output wire         s_axis_aclk            // IQ data clock, 2x sample rate

    );
    
localparam DATA_SRC_AXIS  = 3'h0;
localparam DATA_SRC_ZEROS = 3'h1;
localparam DATA_SRC_ONES  = 3'h2;
localparam DATA_SRC_RAMP  = 3'h3;
localparam DATA_SRC_PN15  = 3'h4;    
localparam DATA_SRC_FIXED = 3'h5;    

// ramp function
function [31:0] ramp;
  input [31:0] din;
  reg   [31:0] dout;
  begin
    dout[31:16] = din[31:16] + 1'b1;
    dout[15:0] = din[15:0] + 1'b1;
    ramp = dout;
  end
endfunction    

// pn15 function
function [31:0] pn15;
  input [31:0] din;
  reg   [31:0] dout;
  begin
    dout[31] = din[29] ^ din[27];
    dout[30] = din[28] ^ din[26];
    dout[29] = din[27] ^ din[25];
    dout[28] = din[26] ^ din[24];
    dout[27] = din[25] ^ din[23];
    dout[26] = din[24] ^ din[22];
    dout[25] = din[23] ^ din[21];
    dout[24] = din[22] ^ din[20];
    dout[23] = din[21] ^ din[19];
    dout[22] = din[20] ^ din[18];
    dout[21] = din[19] ^ din[17];
    dout[20] = din[18] ^ din[16];
    dout[19] = din[17] ^ din[30] ^ din[29];
    dout[18] = din[16] ^ din[29] ^ din[28];
    dout[17] = din[30] ^ din[28] ^ din[29] ^ din[27];
    dout[16] = din[29] ^ din[27] ^ din[28] ^ din[26];
    dout[15] = din[30] ^ din[29];
    dout[14] = din[29] ^ din[28];
    dout[13] = din[28] ^ din[27];
    dout[12] = din[27] ^ din[26];
    dout[11] = din[26] ^ din[25];
    dout[10] = din[25] ^ din[24];
    dout[ 9] = din[24] ^ din[23];
    dout[ 8] = din[23] ^ din[22];
    dout[ 7] = din[22] ^ din[21];
    dout[ 6] = din[21] ^ din[20];
    dout[ 5] = din[20] ^ din[19];
    dout[ 4] = din[19] ^ din[18];
    dout[ 3] = din[18] ^ din[17];
    dout[ 2] = din[17] ^ din[16];
    dout[ 1] = din[16] ^ din[30] ^ din[29];
    dout[ 0] = din[30] ^ din[28];
    pn15 = dout;
  end
endfunction


reg    iq_ready = 'd0;

wire   serdes_rst;
xpm_cdc_async_rst #(
  .DEST_SYNC_FF(4),   
  .INIT_SYNC_FF(0),  
  .RST_ACTIVE_HIGH(1)  
)
serdes_rst_cdc_i (
  .dest_arst(serdes_rst), 
  .dest_clk(ssi_clk_div),   
  .src_arst(~enable)   
);


always @(posedge ssi_clk_div) begin
    
  if( serdes_rst )
    iq_ready <= 1'b0;
  else    
    iq_ready <= ~iq_ready;
    
end

reg  [31:0] pn15_packed = 'd0;
reg  [31:0] iq_packed = 'd0;
reg         iq_packed_valid = 'd0;
reg  [31:0] iq_in_packed = 'd0;

always @(posedge ssi_clk_div) begin

  if( swap_iq )
    iq_in_packed <= {s_axis_tdata[15:0], s_axis_tdata[31:16]};
  else
    iq_in_packed <= s_axis_tdata;

  if( iq_ready ) begin 
    case ( data_src )      
      
      DATA_SRC_AXIS: 
        iq_packed <= {iq_in_packed[15:0], iq_in_packed[31:16]};
      
      DATA_SRC_ZEROS:
        iq_packed <= 32'h0;
      
      DATA_SRC_ONES: 
        iq_packed <= 32'h7fff7fff;
      
      DATA_SRC_RAMP: 
        iq_packed <= ramp(iq_packed);
            
      DATA_SRC_PN15: begin                 
        if( pn15_packed == 'd0 )
          pn15_packed <= pn15(~pn15_packed);
        else
          pn15_packed <= pn15(pn15_packed);
           
        iq_packed <= pn15_packed;
      end     

      DATA_SRC_FIXED: 
        iq_packed <= fixed_pattern;     
        
      default:
        iq_packed <= 32'h0;
            
    endcase
  end else begin
    iq_packed <= iq_packed;
  end
  
  iq_packed_valid <= iq_ready;    
end
 
reg  [7:0]  s_unpacked = 'd0;
reg  [7:0]  i_unpacked = 'd0;
reg  [7:0]  q_unpacked = 'd0; 
  
always @(posedge ssi_clk_div) begin  
  
  if( iq_packed_valid ) begin
    i_unpacked <= iq_packed[31:24];
    q_unpacked <= iq_packed[15:8];
    s_unpacked <= 8'h80;           
  end else begin  
    i_unpacked <= iq_packed[23:16];
    q_unpacked <= iq_packed[7:0];     
    s_unpacked <= 8'h0;      
  end

end

reg  [7:0]  s_serdes_in = 'd0;
reg  [7:0]  i_serdes_in = 'd0;
reg  [7:0]  q_serdes_in = 'd0; 
  
always @(posedge ssi_clk_div) begin  
  
  s_serdes_in <= s_unpacked;
  q_serdes_in <= q_unpacked;
  i_serdes_in <= i_unpacked;
  
end

adrv9001_tx_serdes#(
  .SWAP_DIFF(SWAP_DIFF_STROBE)      // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )
s_serdes(
  .rst(serdes_rst),                 // reset
  .serdes_clk_div(ssi_clk_div),     // data/divided clock synchronous to parallel input 
  .serdes_clk(ssi_clk),             // data clock synchronous to output
  .dout_p(adrv9001_tx_strobe_p),    // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .dout_n(adrv9001_tx_strobe_n),    // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .din(s_serdes_in),                // Parallel data output
  .din_t(1'b0)                      // Data in tristate
);

adrv9001_tx_serdes#( 
  .SWAP_DIFF(SWAP_DIFF_IDATA)       // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )
i_serdes(
  .rst(serdes_rst),                 // reset
  .serdes_clk_div(ssi_clk_div),     // data/divided clock synchronous to parallel input 
  .serdes_clk(ssi_clk),             // data clock synchronous to output
  .dout_p(adrv9001_tx_idata_p),     // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .dout_n(adrv9001_tx_idata_n),     // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .din(i_serdes_in),                // Parallel data output
  .din_t(1'b0)                      // Data in tristate
);

adrv9001_tx_serdes#( 
  .SWAP_DIFF(SWAP_DIFF_QDATA)       // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )
q_serdes(
  .rst(serdes_rst),                 // reset
  .serdes_clk_div(ssi_clk_div),     // data/divided clock synchronous to parallel input 
  .serdes_clk(ssi_clk),             // data clock synchronous to output
  .dout_p(adrv9001_tx_qdata_p),     // 1-bit input: Diff_p buffer input (connect directly to top-level port)
  .dout_n(adrv9001_tx_qdata_n),     // 1-bit input: Diff_n buffer input (connect directly to top-level port)
  .din(q_serdes_in),                // Parallel data output
  .din_t(1'b0)                      // Data in tristate
);

    
wire ssi_clk_out;

ODDRE1 #(
  .IS_C_INVERTED(1'b0),             // Optional inversion for C
  .IS_D1_INVERTED(1'b0),            // Unsupported, do not use
  .IS_D2_INVERTED(1'b0),            // Unsupported, do not use
  .SIM_DEVICE("ULTRASCALE_PLUS"),   // Set the device version for simulation
  .SRVAL(1'b0)                      // Initializes the ODDRE1 Flip-Flops to the specified value (1'b0, 1'b1)
)
oddre1_ssi_clk_out_i (
  .Q(ssi_clk_out),                // 1-bit output: Data output to IOB
  .C(ssi_clk),                    // 1-bit input: High-speed clock input
  .D1(~SWAP_DIFF_CLK_OUT),        // 1-bit input: Parallel data input 1
  .D2(SWAP_DIFF_CLK_OUT),         // 1-bit input: Parallel data input 2
  .SR(1'b0)                       // 1-bit input: Active-High Async Reset
);

generate

  if( SWAP_DIFF_CLK_OUT) begin  
  
    OBUFDS ssi_clk_out_buf_i (
      .O(adrv9001_tx_dclk_n),
      .OB(adrv9001_tx_dclk_p),
      .I(ssi_clk_out)
    );                    
    
  end else begin  
  
    OBUFDS ssi_clk_out_buf_i (
      .O(adrv9001_tx_dclk_p),
      .OB(adrv9001_tx_dclk_n),
      .I(ssi_clk_out)
    );                 
    
  end  

endgenerate    


    
generate 

  if( ENABLE_OTX_AXIS == 1) begin    
    assign otx_axis_tdata = iq_packed;
    assign otx_axis_tvalid = iq_packed_valid;
  end else begin
    assign otx_axis_tdata = 0;
    assign otx_axis_tvalid = 0;
  end    

endgenerate

assign s_axis_aclk = ssi_clk_div;
assign s_axis_tready = iq_ready;
    
endmodule
