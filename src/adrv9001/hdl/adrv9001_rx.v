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
  parameter SWAP_DIFF_CLK = 0,                  // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_IDATA = 0,                // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_QDATA = 0,                // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  parameter SWAP_DIFF_STROBE = 0                // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )(
 
// ADRV9001 interface.  Connect directly to top-level port     
  input  wire         adrv9001_rx_clk_p,        // Clock pair
  input  wire         adrv9001_rx_clk_n,         
  input  wire         adrv9001_rx_strobe_p,     // SSI strobe pair
  input  wire         adrv9001_rx_strobe_n,     
  input  wire         adrv9001_rx_idata_p,      // SSI in-phase data pair
  input  wire         adrv9001_rx_idata_n,      
  input  wire         adrv9001_rx_qdata_p,      // SSI quadrature data pair
  input  wire         adrv9001_rx_qdata_n,      
  
// User interface
  input  wire         enable,                   // Enable Receiver 
  input  wire         swap_iq,
  input  wire [31:0]  fixed_pattern,  
  output reg          ramp_detected_out = 0,
  output reg          pn15_detected_out = 0,
  output reg          fixed_detected_out = 0,
    
  output wire         m_axis_aclk,
  output reg  [31:0]  m_axis_tdata = 0,         // Received IQ data sample
  output reg          m_axis_tvalid = 0         // IQ data sample valid  
    );

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
  
  // pn7 function
  function [31:0] pn7;
    input [31:0] din;
    reg   [31:0] dout;
    begin
      dout[31] = din[20] ^ din[18] ^ din[19] ^ din[17];
      dout[30] = din[19] ^ din[17] ^ din[18] ^ din[16];
      dout[29] = din[18] ^ din[16] ^ din[17] ^ din[22] ^ din[21];
      dout[28] = din[17] ^ din[22] ^ din[16] ^ din[20];
      dout[27] = din[16] ^ din[22] ^ din[19];
      dout[26] = din[22] ^ din[18];
      dout[25] = din[21] ^ din[17];
      dout[24] = din[20] ^ din[16];
      dout[23] = din[19] ^ din[22] ^ din[21];
      dout[22] = din[18] ^ din[21] ^ din[20];
      dout[21] = din[17] ^ din[20] ^ din[19];
      dout[20] = din[16] ^ din[19] ^ din[18];
      dout[19] = din[22] ^ din[18] ^ din[21] ^ din[17];
      dout[18] = din[21] ^ din[17] ^ din[20] ^ din[16];
      dout[17] = din[20] ^ din[16] ^ din[19] ^ din[22] ^ din[21];
      dout[16] = din[19] ^ din[22] ^ din[18] ^ din[20];
      dout[15] = din[22] ^ din[21];
      dout[14] = din[21] ^ din[20];
      dout[13] = din[20] ^ din[19];
      dout[12] = din[19] ^ din[18];
      dout[11] = din[18] ^ din[17];
      dout[10] = din[17] ^ din[16];
      dout[ 9] = din[16] ^ din[22] ^ din[21];
      dout[ 8] = din[22] ^ din[20];
      dout[ 7] = din[21] ^ din[19];
      dout[ 6] = din[20] ^ din[18];
      dout[ 5] = din[19] ^ din[17];
      dout[ 4] = din[18] ^ din[16];
      dout[ 3] = din[17] ^ din[22] ^ din[21];
      dout[ 2] = din[16] ^ din[21] ^ din[20];
      dout[ 1] = din[22] ^ din[20] ^ din[21] ^ din[19];
      dout[ 0] = din[21] ^ din[19] ^ din[20] ^ din[18];
      pn7 = dout;
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
  

wire ssi_clk_in;
      
/* clk_in io buffer */
generate
  if( SWAP_DIFF_CLK == 1) begin   
    IBUFGDS ssi_clk_in_buf_i (
      .O(ssi_clk_in),            
      .I(adrv9001_rx_clk_n),           
      .IB(adrv9001_rx_clk_p)
    );                    
  end else begin       
    IBUFGDS ssi_clk_in_buf_i (
      .O(ssi_clk_in),                
      .I(adrv9001_rx_clk_p),                 
      .IB(adrv9001_rx_clk_n)
    );                  
  end    
endgenerate

wire ssi_clk;
/* SSI clock buffer */
BUFGCE #(
  .CE_TYPE ("SYNC"),
  .IS_CE_INVERTED (1'b0),
  .IS_I_INVERTED (SWAP_DIFF_CLK)
) 
ssi_clk_buf_i (
  .O (ssi_clk),
  .CE (1'b1),
  .I (ssi_clk_in)
); 
 
wire ssi_clk_div;  
/* ssi_clk_div buffer */  
BUFGCE_DIV #(
  .BUFGCE_DIVIDE(4),          
  .IS_CE_INVERTED(1'b0),      
  .IS_CLR_INVERTED(1'b0),      
  .IS_I_INVERTED(SWAP_DIFF_CLK)        
)
clk_div_buf_inst (
  .O(ssi_clk_div),   
  .CE(1'b1),         
  .CLR(1'b0),      
  .I(ssi_clk_in)              
);

wire [7:0]  i_serdes_dout;
wire [7:0]  q_serdes_dout;
wire [7:0]  s_serdes_dout;        
 
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
           
/* I Data Serdes */
adrv9001_rx_serdes #(
  .SWAP_DIFF(SWAP_DIFF_IDATA)    
)
i_serdes_i (
  .rst(serdes_rst),             
  .clk_div(ssi_clk_div),     
  .clk(ssi_clk),              
  .din_p(adrv9001_rx_idata_p),    
  .din_n(adrv9001_rx_idata_n),      
  .dout(q_serdes_dout)       
);

/* Q Data Serdes */
adrv9001_rx_serdes #(
  .SWAP_DIFF(SWAP_DIFF_QDATA)
)
q_serdes_i (
  .rst(serdes_rst),  
  .clk_div(ssi_clk_div),      
  .clk(ssi_clk),           
  .din_p(adrv9001_rx_qdata_p),
  .din_n(adrv9001_rx_qdata_n),   
  .dout(i_serdes_dout)       
);

/* Strobe Serdes */
adrv9001_rx_serdes #(
  .SWAP_DIFF(SWAP_DIFF_STROBE) 
)
s_serdes_i (
  .rst(serdes_rst),   
  .clk_div(ssi_clk_div), 
  .clk(ssi_clk),      
  .din_p(adrv9001_rx_strobe_p),  
  .din_n(adrv9001_rx_strobe_n), 
  .dout(s_serdes_dout)
);

reg  [15:0] i_packed = 'd0;
reg  [15:0] q_packed = 'd0;
reg  [15:0] s_packed = 'd0;
reg         packed_valid = 'd0;

reg  [15:0] i_packed_prev = 'd0;
reg  [15:0] q_packed_prev = 'd0;
reg  [15:0] s_packed_prev = 'd0;
reg         packed_valid_prev = 'd0;


always @( posedge ssi_clk_div ) begin   
    
  if( serdes_rst )
    packed_valid <= 1'b0;    
  else
    packed_valid <= ~packed_valid;  
    
end

always @( posedge ssi_clk_div ) begin   
       
  i_packed <= { i_packed[7:0], i_serdes_dout };
  q_packed <= { q_packed[7:0], q_serdes_dout };
  s_packed <= { s_packed[7:0], s_serdes_dout };
  
  packed_valid_prev <= packed_valid;
  
  if( packed_valid ) begin 
    i_packed_prev <= i_packed; 
    q_packed_prev <= q_packed; 
    s_packed_prev <= s_packed;     
  end else begin
    i_packed_prev <= i_packed_prev;
    q_packed_prev <= q_packed_prev;
    s_packed_prev <= s_packed_prev;  
  end  
    
end

reg  [15:0] q_aligned = 0;
reg  [15:0] i_aligned = 0;
reg  [15:0] s_aligned = 0;
reg         aligned_valid = 'd0;
wire [31:0] iq_aligned = {i_aligned, q_aligned};

always @(posedge ssi_clk_div) begin  

  aligned_valid <= packed_valid;
  
  if (packed_valid) begin
    if(s_packed[15]) begin
      i_aligned <= i_packed_prev[15:0];
      q_aligned <= q_packed_prev[15:0];
      s_aligned <= s_packed_prev[15:0];      
    end else if(s_packed[14]) begin
      i_aligned <= {i_packed_prev[14:0], i_packed[15:15]};
      q_aligned <= {q_packed_prev[14:0], q_packed[15:15]};
      s_aligned <= {s_packed_prev[14:0], s_packed[15:15]};      
    end else if(s_packed[13]) begin
      i_aligned <= {i_packed_prev[13:0], i_packed[15:14]};
      q_aligned <= {q_packed_prev[13:0], q_packed[15:14]};
      s_aligned <= {s_packed_prev[13:0], s_packed[15:14]};      
    end else if(s_packed[12]) begin
      i_aligned <= {i_packed_prev[12:0], i_packed[15:13]};
      q_aligned <= {q_packed_prev[12:0], q_packed[15:13]};
      s_aligned <= {s_packed_prev[12:0], s_packed[15:13]};      
    end else if(s_packed[11]) begin
      i_aligned <= {i_packed_prev[11:0], i_packed[15:12]};
      q_aligned <= {q_packed_prev[11:0], q_packed[15:12]};
      s_aligned <= {s_packed_prev[11:0], s_packed[15:12]};      
    end else if(s_packed[10]) begin
      i_aligned <= {i_packed_prev[10:0], i_packed[15:11]};
      q_aligned <= {q_packed_prev[10:0], q_packed[15:11]};
      s_aligned <= {s_packed_prev[10:0], s_packed[15:11]};      
    end else if(s_packed[9]) begin
      i_aligned <= {i_packed_prev[9:0], i_packed[15:10]};
      q_aligned <= {q_packed_prev[9:0], q_packed[15:10]};
      s_aligned <= {s_packed_prev[9:0], s_packed[15:10]};      
    end else if(s_packed[8]) begin
      i_aligned <= {i_packed_prev[8:0], i_packed[15:9]};
      q_aligned <= {q_packed_prev[8:0], q_packed[15:9]};
      s_aligned <= {s_packed_prev[8:0], s_packed[15:9]};      
    end else if(s_packed[7]) begin
      i_aligned <= {i_packed_prev[7:0], i_packed[15:8]};
      q_aligned <= {q_packed_prev[7:0], q_packed[15:8]};
      s_aligned <= {s_packed_prev[7:0], s_packed[15:8]};      
    end else if(s_packed[6]) begin
      i_aligned <= {i_packed_prev[6:0], i_packed[15:7]};
      q_aligned <= {q_packed_prev[6:0], q_packed[15:7]};
      s_aligned <= {s_packed_prev[6:0], s_packed[15:7]};      
    end else if(s_packed[5]) begin
      i_aligned <= {i_packed_prev[5:0], i_packed[15:6]};
      q_aligned <= {q_packed_prev[5:0], q_packed[15:6]};
      s_aligned <= {s_packed_prev[5:0], s_packed[15:6]};      
    end else if(s_packed[4]) begin
      i_aligned <= {i_packed_prev[4:0], i_packed[15:5]};
      q_aligned <= {q_packed_prev[4:0], q_packed[15:5]};
      s_aligned <= {s_packed_prev[4:0], s_packed[15:5]};      
    end else if(s_packed[3]) begin
      i_aligned <= {i_packed_prev[3:0], i_packed[15:4]};
      q_aligned <= {q_packed_prev[3:0], q_packed[15:4]};
      s_aligned <= {s_packed_prev[3:0], s_packed[15:4]};      
    end else if(s_packed[2]) begin
      i_aligned <= {i_packed_prev[2:0], i_packed[15:3]};
      q_aligned <= {q_packed_prev[2:0], q_packed[15:3]};
      s_aligned <= {s_packed_prev[2:0], s_packed[15:3]};      
    end else if(s_packed[1]) begin
      i_aligned <= {i_packed_prev[1:0], i_packed[15:2]};
      q_aligned <= {q_packed_prev[1:0], q_packed[15:2]};
      s_aligned <= {s_packed_prev[1:0], s_packed[15:2]};      
    end else begin
      i_aligned <= {i_packed_prev[0:0], i_packed[15:1]};
      q_aligned <= {q_packed_prev[0:0], q_packed[15:1]};
      s_aligned <= {s_packed_prev[0:0], s_packed[15:1]};      
    end
  end else begin
    i_aligned <= i_aligned; 
    q_aligned <= q_aligned; 
    s_aligned <= s_aligned;     
  end
end  

reg  [31:0] ramp_in = 0;
reg  [31:0] ramp_prev_predicted = 0;
reg         ramp_detected = 0;
reg  [7:0]  ramp_detected_cnt = 0;
reg         ramp_valid = 0;
   
always @(posedge ssi_clk_div) begin    
  
  if( aligned_valid )
    ramp_in <= iq_aligned; 
  else 
    ramp_in <= ramp_in;
    
  ramp_valid <= aligned_valid;  
    
  if( aligned_valid )
    ramp_prev_predicted = ramp(ramp_in); 
  else 
    ramp_prev_predicted = ramp_prev_predicted;    
  
  if( ramp_valid && (ramp_prev_predicted == ramp_in))
    ramp_detected <= 1'b1;
  else if( ramp_valid )
    ramp_detected <= 1'b0;
  else 
    ramp_detected <= ramp_detected;
    
  if( !ramp_detected )
    ramp_detected_cnt <= 'd0;    
  else if( ramp_valid && (ramp_detected_cnt < 8'hff) )
    ramp_detected_cnt <= ramp_detected_cnt + 1'b1;
  else
    ramp_detected_cnt <= ramp_detected_cnt;    
    
  if( ramp_detected_cnt == 8'hff )
    ramp_detected_out <= 1'b1;
  else
    ramp_detected_out <= 1'b0;  
end

reg  [31:0] pn15_in = 0;
reg  [31:0] pn15_prev_predicted = 0;
reg         pn15_detected = 0;
reg  [7:0]  pn15_detected_cnt = 0;
reg         pn15_valid = 0;
   
always @(posedge ssi_clk_div) begin    
  
  if( aligned_valid )
    pn15_in <= iq_aligned; 
  else 
    pn15_in <= pn15_in;
    
  pn15_valid <= aligned_valid;  
    
  if( aligned_valid && (iq_aligned == 'd0))
    pn15_prev_predicted = pn15(~pn15_in); 
  else if( aligned_valid )
    pn15_prev_predicted = pn15(pn15_in);     
  else 
    pn15_prev_predicted = pn15_prev_predicted;    
  
  if( pn15_valid && ({pn15_prev_predicted[15:0],pn15_prev_predicted[15:0]} == pn15_in))
    pn15_detected <= 1'b1;
  else if( pn15_valid )
    pn15_detected <= 1'b0;
  else 
    pn15_detected <= pn15_detected;
    
  if( !pn15_detected )
    pn15_detected_cnt <= 'd0;    
  else if( pn15_valid && (pn15_detected_cnt < 8'hff) )
    pn15_detected_cnt <= pn15_detected_cnt + 1'b1;
  else
    pn15_detected_cnt <= pn15_detected_cnt;    
    
  if( pn15_detected_cnt == 8'hff )
    pn15_detected_out <= 1'b1;
  else
    pn15_detected_out <= 1'b0;  
end

reg         fixed_detected = 0;
reg  [7:0]  fixed_detected_cnt = 0;
   
always @(posedge ssi_clk_div) begin           
  
  if( aligned_valid && (fixed_pattern == iq_aligned))
    fixed_detected <= 1'b1;
  else if( aligned_valid )
    fixed_detected <= 1'b0;
  else 
    fixed_detected <= fixed_detected;
    
  if( !fixed_detected )
    fixed_detected_cnt <= 'd0;    
  else if( aligned_valid && (fixed_detected_cnt < 8'hff) )
    fixed_detected_cnt <= fixed_detected_cnt + 1'b1;
  else
    fixed_detected_cnt <= fixed_detected_cnt;    
    
  if( fixed_detected_cnt == 8'hff )
    fixed_detected_out <= 1'b1;
  else
    fixed_detected_out <= 1'b0;  
end

assign m_axis_aclk = ssi_clk_div;

always @(posedge ssi_clk_div) begin    
  if( swap_iq )
    m_axis_tdata <= { q_aligned, i_aligned };
  else
    m_axis_tdata <= { i_aligned, q_aligned };
    
  m_axis_tvalid <= aligned_valid;
end

    
endmodule
