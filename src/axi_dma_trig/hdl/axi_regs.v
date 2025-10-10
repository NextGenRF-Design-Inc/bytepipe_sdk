`timescale 1ns / 1ps
/***************************************************************************//**
 *  \file       axi_regs.v
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

module axi_regs#(
    parameter             DEFAULT_TRIG_LVL = 2048 // 
    )(               
    output reg  [31:0]    trig_lvl,                  
    output reg            trig_edge,    
    output reg  [1:0]     trig_type,  
        
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


// AXI4LITE signals
reg [5:0]   axi_awaddr;
reg         axi_awready;
reg         axi_wready;
reg [1:0]   axi_bresp;
reg         axi_bvalid;
reg [5:0]   axi_araddr;
reg         axi_arready;
reg [31:0]  axi_rdata;
reg [1:0]   axi_rresp;
reg         axi_rvalid;
wire        slv_reg_rden;
wire        slv_reg_wren;
reg [31:0]  reg_data_out;
integer     byte_index;
reg         aw_en;


assign s_axi_awready    = axi_awready;
assign s_axi_wready     = axi_wready;
assign s_axi_bresp      = axi_bresp;
assign s_axi_bvalid     = axi_bvalid;
assign s_axi_arready    = axi_arready;
assign s_axi_rdata      = axi_rdata;
assign s_axi_rresp      = axi_rresp;
assign s_axi_rvalid     = axi_rvalid;


always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 )
    begin
      axi_awready <= 1'b0;
      aw_en <= 1'b1;
    end 
  else
    begin    
      if (~axi_awready && s_axi_awvalid && s_axi_wvalid && aw_en)
        begin
          axi_awready <= 1'b1;
          aw_en <= 1'b0;
        end
        else if (s_axi_bready && axi_bvalid)
            begin
              aw_en <= 1'b1;
              axi_awready <= 1'b0;
            end
      else           
        begin
          axi_awready <= 1'b0;
        end
    end 
end       

always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 )
    begin
      axi_awaddr <= 0;
    end 
  else
    begin    
      if (~axi_awready && s_axi_awvalid && s_axi_wvalid && aw_en)
        begin
          axi_awaddr <= s_axi_awaddr[7:2];
        end
    end 
end       

always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 )
    begin
      axi_wready <= 1'b0;
    end 
  else
    begin    
      if (~axi_wready && s_axi_wvalid && s_axi_awvalid && aw_en )
        begin
          axi_wready <= 1'b1;
        end
      else
        begin
          axi_wready <= 1'b0;
        end
    end 
end       

assign slv_reg_wren = axi_wready && s_axi_wvalid && axi_awready && s_axi_awvalid;

/* AXI Write */
always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 ) begin                                 
    trig_lvl <= DEFAULT_TRIG_LVL;
    trig_type <= 1'b0;
    trig_edge = 1'b0;

  end else if (slv_reg_wren) begin
    case ( axi_awaddr )  
      5'd0 : trig_lvl <= s_axi_wdata;    
      5'd1 : trig_type <= s_axi_wdata[1:0];     
      5'd2 : trig_edge <= s_axi_wdata[0]; 
    
                 
      default: begin
          trig_lvl <= trig_lvl;
          trig_type <= trig_type;
          trig_edge <= trig_edge;
      end
    endcase
  end else begin
      trig_lvl <= trig_lvl;
      trig_type <= trig_type;
      trig_edge <= trig_edge;

  end  
end    

always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 )
    begin
      axi_bvalid  <= 0;
      axi_bresp   <= 2'b0;
    end 
  else
    begin    
      if (axi_awready && s_axi_awvalid && ~axi_bvalid && axi_wready && s_axi_wvalid)
        begin
          axi_bvalid <= 1'b1;
          axi_bresp  <= 2'b0; 
        end                 
      else
        begin
          if (s_axi_bready && axi_bvalid) 
            begin
              axi_bvalid <= 1'b0; 
            end  
        end
    end
end   

always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 )
    begin
      axi_arready <= 1'b0;
      axi_araddr  <= 'd0;
    end 
  else
    begin    
      if (~axi_arready && s_axi_arvalid)
        begin
          axi_arready <= 1'b1;
          axi_araddr  <= s_axi_araddr[7:2];
        end
      else
        begin
          axi_arready <= 1'b0;
        end
    end 
end     
 
always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 )
    begin
      axi_rvalid <= 0;
      axi_rresp  <= 0;
    end 
  else
    begin    
      if (axi_arready && s_axi_arvalid && ~axi_rvalid)
        begin
          axi_rvalid <= 1'b1;
          axi_rresp  <= 2'b0; 
        end   
      else if (axi_rvalid && s_axi_rready)
        begin
          axi_rvalid <= 1'b0;
        end                
    end
end    

/* AXI Read */
assign slv_reg_rden = axi_arready & s_axi_arvalid & ~axi_rvalid;
always @(*)
begin       
      case ( axi_araddr )      
        5'd0 : reg_data_out <= trig_lvl;        
        5'd1 : reg_data_out <= {30'h0, trig_type};  
        5'd2 : reg_data_out <= {31'h0, trig_edge};                                      
                                            
        5'd31: reg_data_out <= 32'h12345678;             
        
        default : begin 
          reg_data_out <= 0;          
        end
      endcase
end

always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 )
    begin
      axi_rdata  <= 0;
    end 
  else
    begin    
      if (slv_reg_rden)
        begin
          axi_rdata <= reg_data_out;    
        end   
    end
end    



endmodule


