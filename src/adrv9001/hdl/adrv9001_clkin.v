`timescale 1ns / 1ps
/***************************************************************************//**
 *  \file       adrv9001_clkin.v
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
module adrv9001_clkin#(
  parameter SWAP_DIFF_REF_CLK = 0                  // Swap diff pair allowing physical connection of P signal to N pin and N signal to P pin
  )(
   
  input  wire         ref_clk_p,        // Clock pair
  input  wire         ref_clk_n,         

    
  output wire         clk_out,
  output wire         clk_out_div
    );


wire clk_in;
      
/* clk_in io buffer */
generate
  if( SWAP_DIFF_REF_CLK == 1) begin   
    IBUFGDS ssi_clk_in_buf_i (
      .O(clk_in),            
      .I(ref_clk_n),           
      .IB(ref_clk_p)
    );                    
  end else begin       
    IBUFGDS ssi_clk_in_buf_i (
      .O(clk_in),                
      .I(ref_clk_p),                 
      .IB(ref_clk_n)
    );                  
  end    
endgenerate


/* Clock buffer */
BUFGCE #(
  .CE_TYPE ("SYNC"),
  .IS_CE_INVERTED (1'b0),
  .IS_I_INVERTED (SWAP_DIFF_REF_CLK)
) 
ssi_clk_buf_i (
  .O (clk_out),
  .CE (1'b1),
  .I (clk_in)
); 
 
wire ssi_clk_div;  
/* Div buffer */  
BUFGCE_DIV #(
  .BUFGCE_DIVIDE(4),          
  .IS_CE_INVERTED(1'b0),      
  .IS_CLR_INVERTED(1'b0),      
  .IS_I_INVERTED(SWAP_DIFF_REF_CLK)        
)
clk_div_buf_inst (
  .O(clk_out_div),   
  .CE(1'b1),         
  .CLR(1'b0),      
  .I(clk_in)              
);





    
endmodule
