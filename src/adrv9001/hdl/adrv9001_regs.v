`timescale 1 ns / 1 ps

module adrv9001_regs (
    output reg            tx1_tdd_en = 0,
    output reg            tx2_tdd_en = 0,
    output reg            rx1_tdd_en = 0,
    output reg            rx2_tdd_en = 0,       
    output reg            rstn = 0, 
    output reg            tx1_data_src = 0,
    output reg            tx2_data_src = 0,
    output reg  [31:0]    tx1_data = 0,     
    output reg  [31:0]    tx2_data = 0,  
    input  wire [31:0]    rx1_data,
    input  wire [31:0]    rx2_data,
    output reg  [11:0]    gpio_o,
    output reg  [11:0]    gpio_t,
    input  wire [11:0]    gpio_i,
    output reg  [31:0]    tx1_disable_cnt = 'd100,
    output reg  [31:0]    tx1_ssi_enable_cnt = 'd20,
    
    output reg  [31:0]    tx2_disable_cnt = 'd100,       
    output reg  [31:0]    tx2_ssi_enable_cnt = 'd20,  
    
    output reg  [31:0]    rx1_disable_cnt = 'd100,
    output reg  [31:0]    rx1_ssi_enable_cnt = 'd20,
    output reg  [31:0]    rx1_ssi_disable_cnt = 'd130,
    
    output reg  [31:0]    rx2_disable_cnt = 'd100, 
    output reg  [31:0]    rx2_ssi_enable_cnt = 'd20,
    output reg  [31:0]    rx2_ssi_disable_cnt = 'd130,
    
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





wire [31:0] rx1_data_cdc;
wire [31:0] rx2_data_cdc;

cdc #(
  .DATA_WIDTH(32))
rx1_data_cdc_i (
  .s_cdc_tdata(rx1_data),
  .m_cdc_clk  (s_axi_aclk),
  .m_cdc_tdata(rx1_data_cdc)
);

cdc #(
  .DATA_WIDTH(32))
rx2_data_cdc_i (
  .s_cdc_tdata(rx2_data),
  .m_cdc_clk  (s_axi_aclk),
  .m_cdc_tdata(rx2_data_cdc)
);

// AXI4LITE signals
reg [4:0]   axi_awaddr;
reg         axi_awready;
reg         axi_wready;
reg [1:0]   axi_bresp;
reg  	    axi_bvalid;
reg [4:0]   axi_araddr;
reg  	    axi_arready;
reg [31:0]  axi_rdata;
reg [1:0]   axi_rresp;
reg  	    axi_rvalid;
wire	    slv_reg_rden;
wire	    slv_reg_wren;
reg [31:0]	reg_data_out;
integer	    byte_index;
reg	        aw_en;


assign s_axi_awready	= axi_awready;
assign s_axi_wready	    = axi_wready;
assign s_axi_bresp	    = axi_bresp;
assign s_axi_bvalid	    = axi_bvalid;
assign s_axi_arready	= axi_arready;
assign s_axi_rdata	    = axi_rdata;
assign s_axi_rresp	    = axi_rresp;
assign s_axi_rvalid	    = axi_rvalid;


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
      tx1_tdd_en <= 0;
      tx2_tdd_en <= 0;
      rx1_tdd_en <= 0;
      rx2_tdd_en <= 0;              
      rstn <= 0;
      tx1_data_src <= 0;
      tx2_data_src <= 0;         
      tx1_data <= 32'h12345678;     
      tx2_data <= 32'hABCD1234;   
      gpio_t <= 12'hfff;
      gpio_o <= 0;    
      tx1_disable_cnt <= 'd100; 
      tx1_ssi_enable_cnt <= 'd20; 
      tx2_disable_cnt <= 'd100;         
      tx2_ssi_enable_cnt <= 'd20;    
      rx1_disable_cnt <= 'd100;    
      rx1_ssi_enable_cnt <= 'd20;
      rx1_ssi_disable_cnt <= 'd130;
      rx2_disable_cnt <= 'd100;    
      rx2_ssi_enable_cnt <= 'd20;
      rx2_ssi_disable_cnt <= 'd130;   
  end else if (slv_reg_wren) begin
    case ( axi_awaddr )
      5'd0:  tx1_tdd_en <= s_axi_wdata[0];
      5'd1:  tx2_tdd_en <= s_axi_wdata[0];
      5'd2:  rx1_tdd_en <= s_axi_wdata[0];
      5'd3:  rx2_tdd_en <= s_axi_wdata[0];                  
      5'd4:  rstn <= s_axi_wdata[0];          
      5'd5:  tx1_data_src <= s_axi_wdata[0];          
      5'd6:  tx2_data_src <= s_axi_wdata[0];   
      5'd7:  tx1_data <= s_axi_wdata;
      5'd8:  tx2_data <= s_axi_wdata;            
      5'd9:  gpio_t <= s_axi_wdata[11:0];       
      5'd10: gpio_o <= s_axi_wdata[11:0];                                 
      5'd11: tx1_disable_cnt <= s_axi_wdata;   
      5'd12: tx1_ssi_enable_cnt <= s_axi_wdata;   
      5'd13: tx2_disable_cnt <= s_axi_wdata;   
      5'd14: tx2_ssi_enable_cnt <= s_axi_wdata;   
      5'd15: rx1_disable_cnt <= s_axi_wdata;   
      5'd16: rx1_ssi_enable_cnt <= s_axi_wdata;  
      5'd17: rx1_ssi_disable_cnt <= s_axi_wdata;     
      5'd18: rx2_disable_cnt <= s_axi_wdata;   
      5'd19: rx2_ssi_enable_cnt <= s_axi_wdata;  
      5'd20: rx2_ssi_disable_cnt <= s_axi_wdata;                                       
      default: begin
        tx1_tdd_en <= tx1_tdd_en;
        tx2_tdd_en <= tx2_tdd_en;
        rx1_tdd_en <= rx1_tdd_en;
        rx2_tdd_en <= rx2_tdd_en;                       
        rstn <= rstn;
        tx1_data_src <= tx1_data_src;
        tx2_data_src <= tx2_data_src;
        tx1_data <= tx1_data;     
        tx2_data <= tx2_data;    
        gpio_o <= gpio_o;
        gpio_t <= gpio_t;             
        tx1_disable_cnt <= tx1_disable_cnt; 
        tx1_ssi_enable_cnt <= tx1_ssi_enable_cnt; 
        tx2_disable_cnt <= tx2_disable_cnt;         
        tx2_ssi_enable_cnt <= tx2_ssi_enable_cnt;    
        rx1_disable_cnt <= rx1_disable_cnt;    
        rx1_ssi_enable_cnt <= rx1_ssi_enable_cnt;
        rx1_ssi_disable_cnt <= rx1_ssi_disable_cnt;
        rx2_disable_cnt <= rx2_disable_cnt;    
        rx2_ssi_enable_cnt <= rx2_ssi_enable_cnt;
        rx2_ssi_disable_cnt <= rx2_ssi_disable_cnt;        
      end
    endcase
  end else begin
    tx1_tdd_en <= tx1_tdd_en;
    tx2_tdd_en <= tx2_tdd_en;
    rx1_tdd_en <= rx1_tdd_en;
    rx2_tdd_en <= rx2_tdd_en;                       
    rstn <= rstn;
    tx1_data_src <= tx1_data_src;
    tx2_data_src <= tx2_data_src;
    tx1_data <= tx1_data;     
    tx2_data <= tx2_data;    
    gpio_o <= gpio_o;
    gpio_t <= gpio_t;             
    tx1_disable_cnt <= tx1_disable_cnt; 
    tx1_ssi_enable_cnt <= tx1_ssi_enable_cnt; 
    tx2_disable_cnt <= tx2_disable_cnt;         
    tx2_ssi_enable_cnt <= tx2_ssi_enable_cnt;    
    rx1_disable_cnt <= rx1_disable_cnt;    
    rx1_ssi_enable_cnt <= rx1_ssi_enable_cnt;
    rx1_ssi_disable_cnt <= rx1_ssi_disable_cnt;
    rx2_disable_cnt <= rx2_disable_cnt;    
    rx2_ssi_enable_cnt <= rx2_ssi_enable_cnt;
    rx2_ssi_disable_cnt <= rx2_ssi_disable_cnt;          
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
        5'd0:  reg_data_out <= {31'h0, tx1_tdd_en};
        5'd1:  reg_data_out <= {31'h0, tx2_tdd_en};
        5'd2:  reg_data_out <= {31'h0, rx1_tdd_en}; 
        5'd3:  reg_data_out <= {31'h0, rx2_tdd_en};    
        5'd4:  reg_data_out <= {31'h0, rstn};               
        5'd5:  reg_data_out <= {31'h0, tx1_data_src};    
        5'd6:  reg_data_out <= {31'h0, tx2_data_src};                   
        5'd7:  reg_data_out <= rx1_data_cdc;            
        5'd8:  reg_data_out <= rx2_data_cdc;            
        5'd9:  reg_data_out <= {20'h0, gpio_t};
        5'd10: reg_data_out <= {20'h0, gpio_i};                                                             
        5'd31  : reg_data_out <= 32'h12345678;             
        
        default : reg_data_out <= 0;
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
