`timescale 1 ns / 1 ps

module adrv9001_regs (
    output wire           rx1_en,
    output wire           rx2_en,
    output wire           tx1_en,
    output wire           tx2_en,
    output wire           rstn,
    inout  wire [11:0]    dgpio,  
    output reg  [1:0]     tx1_data_src = 0,
    output reg  [1:0]     tx2_data_src = 0,
    output reg  [31:0]    tx1_data = 0,     
    output reg  [31:0]    tx2_data = 0,  
    input  wire [31:0]    rx1_data,
    input  wire [31:0]    rx2_data,
    output wire           tx1_ssi_en,
    output wire           tx2_ssi_en,
    output wire           rx1_ssi_en,
    output wire           rx2_ssi_en,        

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


// AXI4LITE signals
reg [4:0]   axi_awaddr;
reg         axi_awready;
reg         axi_wready;
reg [1:0]   axi_bresp;
reg  	      axi_bvalid;
reg [4:0]   axi_araddr;
reg  	      axi_arready;
reg [31:0]  axi_rdata;
reg [1:0]   axi_rresp;
reg  	      axi_rvalid;

// Internal Registers
reg [8:0]   ctrl_reg = 0;
reg [8:0]   ctrl_reg1 = 0;
reg [11:0]  gpio_reg = 0;	
reg [11:0]  gpio_reg_t = 0;		
reg [31:0]  rx1_data_reg0 = 0;
reg [31:0]  rx1_data_reg1 = 0;
reg [31:0]  rx1_data_reg2 = 0;
reg [31:0]  rx2_data_reg0 = 0;
reg [31:0]  rx2_data_reg1 = 0;
reg [31:0]  rx2_data_reg2 = 0;
reg [15:0]  tx1_trig_cntr = 0;
reg [15:0]  tx2_trig_cntr = 0;
reg [15:0]  rx1_trig_cntr = 0;
reg [15:0]  rx2_trig_cntr = 0;
reg [15:0]  tx1_trig = 0;
reg [15:0]  tx2_trig = 0;
reg [15:0]  rx1_trig = 0;
reg [15:0]  rx2_trig = 0;


/* Control Register */
assign tx1_en   = ctrl_reg[0]; 	
assign tx2_en   = ctrl_reg[1]; 	
assign rx1_en   = ctrl_reg[2]; 	
assign rx2_en   = ctrl_reg[3]; 			
assign rstn     = ctrl_reg[8]; 				

wire tx1_en_rising;
wire tx2_en_rising;
wire rx1_en_rising;
wire rx2_en_rising;

assign tx1_en_rising = ctrl_reg[0] & ~ctrl_reg1[0];
assign tx2_en_rising = ctrl_reg[1] & ~ctrl_reg1[1];
assign rx1_en_rising = ctrl_reg[2] & ~ctrl_reg1[2];
assign rx2_en_rising = ctrl_reg[3] & ~ctrl_reg1[3];

assign tx1_ssi_en = (tx1_trig_cntr == tx1_trig);
assign tx2_ssi_en = (tx2_trig_cntr == tx2_trig);
assign rx1_ssi_en = (rx1_trig_cntr == rx1_trig);
assign rx2_ssi_en = (rx2_trig_cntr == rx2_trig);

/* GPIO */
genvar n;
generate
  for (n = 0; n < 12; n = n + 1) begin: gpio_io
    assign dgpio[n] = (gpio_reg_t[n] == 1'b1) ? 1'bz : gpio_reg[n];
  end
endgenerate


wire	      slv_reg_rden;
wire	      slv_reg_wren;
reg [31:0]	reg_data_out;
integer	    byte_index;
reg	        aw_en;


assign s_axi_awready	  = axi_awready;
assign s_axi_wready	    = axi_wready;
assign s_axi_bresp	    = axi_bresp;
assign s_axi_bvalid	    = axi_bvalid;
assign s_axi_arready	  = axi_arready;
assign s_axi_rdata	    = axi_rdata;
assign s_axi_rresp	    = axi_rresp;
assign s_axi_rvalid	    = axi_rvalid;


always @( posedge s_axi_aclk )
begin
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

always @( posedge s_axi_aclk )
begin
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

always @( posedge s_axi_aclk )
begin
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

always @( posedge s_axi_aclk )
begin
  if( tx1_en_rising )
    tx1_trig_cntr <= 0;
  else if (tx1_trig_cntr < tx1_trig)
    tx1_trig_cntr <= tx1_trig_cntr + 1;
  else
    tx1_trig_cntr <= tx1_trig_cntr;
end

always @( posedge s_axi_aclk )
begin
  if( tx2_en_rising )
    tx2_trig_cntr <= 0;
  else if (tx2_trig_cntr < tx2_trig)
    tx2_trig_cntr <= tx2_trig_cntr + 1;
  else
    tx2_trig_cntr <= tx2_trig_cntr;
end

always @( posedge s_axi_aclk )
begin
  if( rx1_en_rising )
    rx1_trig_cntr <= 0;
  else if (rx1_trig_cntr < rx1_trig)
    rx1_trig_cntr <= rx1_trig_cntr + 1;
  else
    rx1_trig_cntr <= rx1_trig_cntr;
end

always @( posedge s_axi_aclk )
begin
  if( rx2_en_rising )
    rx2_trig_cntr <= 0;
  else if (rx2_trig_cntr < rx2_trig)
    rx2_trig_cntr <= rx2_trig_cntr + 1;
  else
    rx2_trig_cntr <= rx2_trig_cntr;
end

assign slv_reg_wren = axi_wready && s_axi_wvalid && axi_awready && s_axi_awvalid;

/* AXI Write */
always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 ) begin                           
      ctrl_reg <= 0;
      ctrl_reg1 <= 0;
      gpio_reg <= 0; 
      gpio_reg_t <= 12'h3FF;
      tx1_data_src <= 2'h0;
      tx2_data_src <= 2'h0;
      tx1_data <= 32'h12345678;     
      tx2_data <= 32'hABCD1234;       
      tx1_trig <= 16'd100;
      tx2_trig <= 16'd100;
      rx1_trig <= 16'd100;
      rx2_trig <= 16'd100;      
  end else if (slv_reg_wren) begin
    case ( axi_awaddr )
      5'd0: begin
        ctrl_reg <= s_axi_wdata[8:0];
      end
       
      5'd1:begin 
        gpio_reg <= s_axi_wdata[11:0];
      end
      
      5'd2:begin 
        gpio_reg_t <= s_axi_wdata[11:0];
      end  
      
      5'd3:begin 
        tx1_data_src <= s_axi_wdata[1:0];
      end   
               
      5'd4:begin 
        tx2_data_src <= s_axi_wdata[1:0];
      end 
       
      5'd5:begin 
        tx1_data <= s_axi_wdata;
      end  
       
      5'd6:begin 
        tx2_data <= s_axi_wdata;
      end  

      5'd9:begin 
        tx1_trig <= s_axi_wdata[15:0];
      end  
 
      5'd10:begin 
        tx2_trig <= s_axi_wdata[15:0];
      end  
      
      5'd11:begin 
        rx1_trig <= s_axi_wdata[15:0];
      end  
 
      5'd12:begin 
        rx2_trig <= s_axi_wdata[15:0];
      end        
          
      default: begin
        ctrl_reg <= ctrl_reg;
        gpio_reg <= gpio_reg;
        gpio_reg_t <= gpio_reg_t;
        tx1_data_src <= tx1_data_src;
        tx2_data_src <= tx2_data_src;
        tx1_data <= tx1_data;     
        tx2_data <= tx2_data;    
        tx1_trig <= tx1_trig; 
        tx2_trig <= tx2_trig; 
        rx1_trig <= rx1_trig;         
        rx2_trig <= rx2_trig;        
      end
    endcase
  end else begin
    ctrl_reg <= ctrl_reg;
    ctrl_reg1 <= ctrl_reg;
    gpio_reg <= gpio_reg;
    gpio_reg_t <= gpio_reg_t;
    tx1_data_src <= tx1_data_src;
    tx2_data_src <= tx2_data_src;
    tx1_data <= tx1_data;     
    tx2_data <= tx2_data;  
    tx1_trig <= tx1_trig; 
    tx2_trig <= tx2_trig; 
    rx1_trig <= rx1_trig;         
    rx2_trig <= rx2_trig;         
  end  
end    

always @( posedge s_axi_aclk )
begin
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

always @( posedge s_axi_aclk )
begin
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
 
always @( posedge s_axi_aclk )
begin
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
        5'd0   : reg_data_out <= {23'h0, ctrl_reg};
        5'd1   : reg_data_out <= {20'h0, gpio_reg};
        5'd2   : reg_data_out <= {20'h0, gpio_reg_t};
        5'd3   : reg_data_out <= {30'h0, tx1_data_src};   
        5'd4   : reg_data_out <= {30'h0, tx2_data_src};  
        5'd5   : reg_data_out <= tx1_data;  
        5'd6   : reg_data_out <= tx2_data;   
        5'd7   : reg_data_out <= rx1_data_reg2;   
        5'd8   : reg_data_out <= rx2_data_reg2;  
        5'd9   : reg_data_out <= tx1_trig_cntr;  
        5'd10  : reg_data_out <= tx2_trig_cntr;  
        5'd11  : reg_data_out <= rx1_trig_cntr;  
        5'd12  : reg_data_out <= rx2_trig_cntr;    
        5'd15  : reg_data_out <= 32'h12345678;     
        5'd31  : reg_data_out <= 32'h12345678;             
        
        default : reg_data_out <= 0;
      endcase
end

always @( posedge s_axi_aclk )
begin
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

/* Cross Clock Domains */
always @( posedge s_axi_aclk ) begin
  if ( s_axi_aresetn == 1'b0 )
    begin
      rx1_data_reg0  <= 0;
      rx1_data_reg1  <= 0;
      rx1_data_reg2  <= 0;   
      rx2_data_reg0  <= 0;
      rx2_data_reg1  <= 0;
      rx2_data_reg2  <= 0;                  
    end 
  else
      begin 
  
      rx1_data_reg0 <= rx1_data;
      rx1_data_reg1 <= rx1_data_reg0;
      rx1_data_reg2 <= rx1_data_reg1;
        
      rx2_data_reg0 <= rx2_data;
      rx2_data_reg1 <= rx2_data_reg0;
      rx2_data_reg2 <= rx2_data_reg1;  
  end
end

endmodule
