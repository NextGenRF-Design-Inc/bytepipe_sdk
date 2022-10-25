`timescale 1 ns / 1 ps

module adrv9001_regs (
    output reg            tx1_enable_mode = 0,   
    output reg            tx2_enable_mode = 0, 
    output reg            rx1_enable_mode = 0, 
    output reg            rx2_enable_mode = 0, 
    
    output reg            adrv9001_rstn = 0, 
    
    output reg  [15:0]    tx1_enable_delay = 10,
    output reg  [15:0]    tx2_enable_delay = 10,
    output reg  [15:0]    rx1_enable_delay = 10,
    output reg  [15:0]    rx2_enable_delay = 10,    
    
    output reg  [15:0]    tx1_disable_delay = 10,    
    output reg  [15:0]    tx2_disable_delay = 10,    
    output reg  [15:0]    rx1_disable_delay = 10,    
    output reg  [15:0]    rx2_disable_delay = 10,  

    output reg  [31:0]    tx1_ps_data = 0,     
    output reg  [31:0]    tx2_ps_data = 0,    
    
    output reg            tx1_data_src = 0,
    output reg            tx2_data_src = 0,

    input  wire [31:0]    rx1_ps_data,
    input  wire [31:0]    rx2_ps_data,
    
    output reg  [11:0]    dgpio_ps_o,
    output reg  [11:0]    dgpio_ps_t,
    input  wire [11:0]    dgpio_ps_i,
    
    input  wire [7:0]     sspi_axis_tdata,
    input  wire           sspi_axis_tvalid,
    output reg            sspi_axis_tready = 1'b0,
  
    output reg            mspi_axis_enable = 'b0,
    output reg  [7:0]     mspi_axis_tdata = 'd0,
    output reg            mspi_axis_tvalid = 'b0,
    input  wire           mspi_axis_tready,    
    
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





wire [31:0] rx1_ps_data_cdc;
wire [31:0] rx2_ps_data_cdc;

cdc #(
  .DATA_WIDTH(32))
rx1_data_cdc_i (
  .s_cdc_tdata(rx1_ps_data),
  .m_cdc_clk  (s_axi_aclk),
  .m_cdc_tdata(rx1_ps_data_cdc)
);

cdc #(
  .DATA_WIDTH(32))
rx2_data_cdc_i (
  .s_cdc_tdata(rx2_ps_data),
  .m_cdc_clk  (s_axi_aclk),
  .m_cdc_tdata(rx2_ps_data_cdc)
);

// AXI4LITE signals
reg [4:0]   axi_awaddr;
reg         axi_awready;
reg         axi_wready;
reg [1:0]   axi_bresp;
reg         axi_bvalid;
reg [4:0]   axi_araddr;
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
          axi_awaddr <= s_axi_awaddr[6:2];
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
      tx1_enable_mode <= 0;
      tx2_enable_mode <= 0;
      rx1_enable_mode <= 0;
      rx2_enable_mode <= 0;      
      
      adrv9001_rstn <= 0;
      
      tx1_enable_delay <= 10;
      tx2_enable_delay <= 10;
      rx1_enable_delay <= 10;
      rx2_enable_delay <= 10;

      tx1_disable_delay <= 10;
      tx2_disable_delay <= 10;
      rx1_disable_delay <= 10;
      rx2_disable_delay <= 10;
            
      dgpio_ps_t <= 12'hfff;
      dgpio_ps_o <= 0;    
      
      tx1_ps_data <= 32'h12345678;
      tx2_ps_data <= 32'hABCD1234;    

      tx1_data_src <= 0;           
      tx2_data_src <= 0;         

      mspi_axis_tdata <= 8'd0;
      mspi_axis_tvalid <= 1'b0;       
      mspi_axis_enable <= 1'b0;
  end else if (slv_reg_wren) begin
    case ( axi_awaddr )
      5'd0 : tx1_enable_mode <= s_axi_wdata[0];
      5'd1 : tx2_enable_mode <= s_axi_wdata[0];
      5'd2 : rx1_enable_mode <= s_axi_wdata[0];
      5'd3 : rx2_enable_mode <= s_axi_wdata[0];
      
      5'd4 : adrv9001_rstn <= s_axi_wdata[0];    
      
      5'd5 : tx1_enable_delay <= s_axi_wdata[15:0];   
      5'd6 : tx2_enable_delay <= s_axi_wdata[15:0];  
      5'd7 : rx1_enable_delay <= s_axi_wdata[15:0];   
      5'd8 : rx2_enable_delay <= s_axi_wdata[15:0];        
      
      5'd9 : tx1_disable_delay <= s_axi_wdata[15:0];         
      5'd10: tx2_disable_delay <= s_axi_wdata[15:0];   
      5'd11: rx1_disable_delay <= s_axi_wdata[15:0];   
      5'd12: rx2_disable_delay <= s_axi_wdata[15:0];    
      
      5'd13: dgpio_ps_t <= s_axi_wdata[11:0];       
      5'd14: dgpio_ps_o <= s_axi_wdata[11:0];   

      5'd15: tx1_ps_data <= s_axi_wdata;
      5'd16: tx2_ps_data <= s_axi_wdata;   
      
      5'd17: tx1_data_src <= s_axi_wdata[0];
      5'd18: tx2_data_src <= s_axi_wdata[0];        
  
      
      5'd21: begin
        mspi_axis_tdata <= s_axi_wdata[7:0];
        mspi_axis_tvalid <= 1'b1;
        mspi_axis_enable <= s_axi_wdata[8];
      end
                                               
      default: begin
        tx1_enable_mode <= tx1_enable_mode; 
        tx2_enable_mode <= tx2_enable_mode; 
        rx1_enable_mode <= rx1_enable_mode; 
        rx2_enable_mode <= rx2_enable_mode;         
        adrv9001_rstn <= adrv9001_rstn;
        tx1_enable_delay <= tx1_enable_delay;
        tx2_enable_delay <= tx2_enable_delay;
        rx1_enable_delay <= rx1_enable_delay;
        rx2_enable_delay <= rx2_enable_delay;
        tx1_disable_delay <= tx1_disable_delay;
        tx2_disable_delay <= tx2_disable_delay;
        rx1_disable_delay <= rx1_disable_delay;
        rx2_disable_delay <= rx2_disable_delay;        
        dgpio_ps_t <= dgpio_ps_t;
        dgpio_ps_o <= dgpio_ps_o;
        tx1_ps_data <= tx1_ps_data;
        tx2_ps_data <= tx2_ps_data;                
        tx1_data_src <= tx1_data_src;
        tx2_data_src <= tx2_data_src;
        mspi_axis_tdata <= mspi_axis_tdata;
        mspi_axis_tvalid <= 1'b0;       
        mspi_axis_enable <= mspi_axis_enable;
      end
    endcase
  end else begin
    tx1_enable_mode <= tx1_enable_mode; 
    tx2_enable_mode <= tx2_enable_mode; 
    rx1_enable_mode <= rx1_enable_mode; 
    rx2_enable_mode <= rx2_enable_mode;                     
    adrv9001_rstn <= adrv9001_rstn;
    tx1_enable_delay <= tx1_enable_delay;
    tx2_enable_delay <= tx2_enable_delay;
    rx1_enable_delay <= rx1_enable_delay;
    rx2_enable_delay <= rx2_enable_delay;
    tx1_disable_delay <= tx1_disable_delay;
    tx2_disable_delay <= tx2_disable_delay;
    rx1_disable_delay <= rx1_disable_delay;
    rx2_disable_delay <= rx2_disable_delay;        
    dgpio_ps_t <= dgpio_ps_t;
    dgpio_ps_o <= dgpio_ps_o;
    tx1_ps_data <= tx1_ps_data;
    tx2_ps_data <= tx2_ps_data;                
    tx1_data_src <= tx1_data_src;
    tx2_data_src <= tx2_data_src;  
    mspi_axis_tdata <= mspi_axis_tdata;
    mspi_axis_tvalid <= 1'b0;              
    mspi_axis_enable <= mspi_axis_enable;    
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
          axi_araddr  <= s_axi_araddr[6:2];
        end
      else
        begin
          axi_arready <= 1'b0;
        end
    end 
end   

always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 )
    sspi_axis_tready <= 1'b0;
  else begin    
    if (~axi_arready && s_axi_arvalid && (s_axi_araddr[6:2] == 5'd21))
      sspi_axis_tready <= 1'b1;
    else
      sspi_axis_tready <= 1'b0;
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
        5'd0:  reg_data_out <= {31'h0, tx1_enable_mode};
        5'd1:  reg_data_out <= {31'h0, tx2_enable_mode};
        5'd2:  reg_data_out <= {31'h0, rx1_enable_mode};
        5'd3:  reg_data_out <= {31'h0, rx2_enable_mode};        
        5'd4:  reg_data_out <= {31'h0, adrv9001_rstn};    
        
        5'd5 : reg_data_out <= {16'h0, tx1_enable_delay}; 
        5'd6 : reg_data_out <= {16'h0, tx2_enable_delay};
        5'd7 : reg_data_out <= {16'h0, rx1_enable_delay};  
        5'd8 : reg_data_out <= {16'h0, rx2_enable_delay};      
      
        5'd9 : reg_data_out <= {16'h0, tx1_disable_delay};      
        5'd10: reg_data_out <= {16'h0, tx2_disable_delay};  
        5'd11: reg_data_out <= {16'h0, rx1_disable_delay}; 
        5'd12: reg_data_out <= {16'h0, rx2_disable_delay};        
        
        5'd13: reg_data_out <= {20'h0, dgpio_ps_t};     
        5'd14: reg_data_out <= {20'h0, dgpio_ps_o}; 

        5'd15: reg_data_out <= rx1_ps_data_cdc;
        5'd16: reg_data_out <= rx2_ps_data_cdc;  
      
        5'd17: reg_data_out <= {31'h0, tx1_data_src};
        5'd18: reg_data_out <= {31'h0, tx2_data_src};      
       
        5'd21: begin 
          reg_data_out <= {23'h0, sspi_axis_tvalid, sspi_axis_tdata};   
        end                                                    
        5'd31  : reg_data_out <= 32'h12345678;             
        
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
