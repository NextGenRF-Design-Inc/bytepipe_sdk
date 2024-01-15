`timescale 1 ns / 1 ps

module adrv9001_regs #(
    parameter DEFAULT_DGPIO_DIR     = 'hffff
    )(
    output reg            tx1_enable = 0,   
    output reg            tx2_enable = 0, 
    output reg            rx1_enable = 0, 
    output reg            rx2_enable = 0, 
    
    output reg            hopping_mode = 0,
    output reg            hop_trig_enable = 0,
    output reg            hop_trig_clear = 0,
    input  wire           hop_trig_status,
    
    output reg  [23:0]    rx1_hop_setup_delay_cnt,
    output reg  [23:0]    rx2_hop_setup_delay_cnt,
    output reg  [23:0]    tx1_hop_setup_delay_cnt,
    output reg  [23:0]    tx2_hop_setup_delay_cnt,
    output reg  [23:0]    hop_dgpio_delay_cnt,
    
    output reg            rx1_ps_setup = 0,
    output reg            rx2_ps_setup = 0,
    output reg            tx1_ps_setup = 0,
    output reg            tx2_ps_setup = 0,    
    
    output reg            hop_trig = 'd0,
    output reg  [3:0]     next_hop_enable_mask = 'd0,
    output reg            enable_pl_hop_trig = 'd0,
    
    output reg            tx1_swap_iq = 0,   
    output reg            tx2_swap_iq = 0, 
    output reg            rx1_swap_iq = 0, 
    output reg            rx2_swap_iq = 0,     
    
    output reg            adrv9001_rstn = 0, 
        
    input  wire           rx1_ramp_detected,
    input  wire           rx2_ramp_detected,    
    
    input  wire           rx1_pn15_detected,  
    input  wire           rx2_pn15_detected,   
    
    input  wire           rx1_fixed_detected,  
    input  wire           rx2_fixed_detected,      
    
    output reg  [31:0]    rx1_fixed_pattern = 'h12345678,
    output reg  [31:0]    rx2_fixed_pattern = 'h12345678,  
    output reg  [31:0]    tx1_fixed_pattern = 'h12345678,
    output reg  [31:0]    tx2_fixed_pattern = 'h12345678,      

    output reg  [2:0]     tx1_data_src = 0,    
    output reg  [2:0]     tx2_data_src = 0,        
    
    output reg  [15:0]    dgpio_ps_o = 'd0,
    output reg  [15:0]    dgpio_ps_t = 'd0,
    input  wire [15:0]    dgpio_ps_i,
        
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
      tx1_enable <= 0;
      tx2_enable <= 0;
      rx1_enable <= 0;
      rx2_enable <= 0;     
      
      tx1_swap_iq <= 0;
      tx2_swap_iq <= 0;
      rx1_swap_iq <= 0;
      rx2_swap_iq <= 0;  
      
      rx1_fixed_pattern <= 32'h12345678;
      rx2_fixed_pattern <= 32'h12345678;  
      tx1_fixed_pattern <= 32'h12345678;
      tx2_fixed_pattern <= 32'h12345678;        
      
      adrv9001_rstn <= 0;
            
      dgpio_ps_t <= DEFAULT_DGPIO_DIR;
      dgpio_ps_o <= 0;    
      
      tx1_data_src <= 'd0;
      tx2_data_src <= 'd0;
      
      mspi_axis_tdata <= 8'd0;
      mspi_axis_tvalid <= 1'b0;       
      mspi_axis_enable <= 1'b0;
      
      hopping_mode <= 'd0;      
      hop_dgpio_delay_cnt <= 'd0;
      hop_trig <= 'd0;      
      next_hop_enable_mask <= 'd0;    
      
      rx1_hop_setup_delay_cnt <= 'd0;
      rx2_hop_setup_delay_cnt <= 'd0; 
      tx1_hop_setup_delay_cnt <= 'd0;
      tx2_hop_setup_delay_cnt <= 'd0;       
      
      rx1_ps_setup <= 'd0;
      rx2_ps_setup <= 'd0;
      tx1_ps_setup <= 'd0;      
      tx2_ps_setup <= 'd0;
      
      hop_trig_enable <= 'd0;
      hop_trig_clear <= 'd0;
    
  end else if (slv_reg_wren) begin
    case ( axi_awaddr )
      5'd0 : tx1_enable <= s_axi_wdata[0];
      5'd1 : tx2_enable <= s_axi_wdata[0];
      5'd2 : rx1_enable <= s_axi_wdata[0];
      5'd3 : rx2_enable <= s_axi_wdata[0];      
      5'd4 : adrv9001_rstn <= s_axi_wdata[0];            
      5'd5 : dgpio_ps_t <= s_axi_wdata[15:0];       
      5'd6 : dgpio_ps_o <= s_axi_wdata[15:0];    
      5'd7 : tx1_data_src <= s_axi_wdata[2:0];  
      5'd8 : tx2_data_src <= s_axi_wdata[2:0];        
      
      5'd12: rx1_fixed_pattern <= s_axi_wdata;
      5'd13: rx2_fixed_pattern <= s_axi_wdata;  
      5'd14: tx1_fixed_pattern <= s_axi_wdata;
      5'd15: tx2_fixed_pattern <= s_axi_wdata;  
      
      5'd16: rx1_hop_setup_delay_cnt <= s_axi_wdata[23:0];
      5'd17: rx2_hop_setup_delay_cnt <= s_axi_wdata[23:0];
      5'd18: tx1_hop_setup_delay_cnt <= s_axi_wdata[23:0]; 
      5'd19: tx2_hop_setup_delay_cnt <= s_axi_wdata[23:0];     
              
      5'd20: hop_dgpio_delay_cnt <= s_axi_wdata[23:0];
           
      5'd21: begin
        mspi_axis_tdata <= s_axi_wdata[7:0];
        mspi_axis_tvalid <= 1'b1;
        mspi_axis_enable <= s_axi_wdata[8];
      end
            
      5'd23: begin
        hop_trig_clear <= s_axi_wdata[0];
        hop_trig_enable <= s_axi_wdata[1];        
      end             
      
      5'd24: hop_trig <= s_axi_wdata[0];
      5'd25: next_hop_enable_mask <= s_axi_wdata[3:0];      
      5'd26: enable_pl_hop_trig <= s_axi_wdata[0]; 

      5'd27: begin
        tx1_swap_iq <= s_axi_wdata[3];  
        tx2_swap_iq <= s_axi_wdata[2];   
        rx1_swap_iq <= s_axi_wdata[1];  
        rx2_swap_iq <= s_axi_wdata[0];           
      end

      5'd28: hopping_mode <= s_axi_wdata[0];        
      5'd30: begin
        tx1_ps_setup <= s_axi_wdata[3];
        tx2_ps_setup <= s_axi_wdata[2];
        rx1_ps_setup <= s_axi_wdata[1];
        rx2_ps_setup <= s_axi_wdata[0];
      end
      
                                               
      default: begin
        rx1_hop_setup_delay_cnt <= rx1_hop_setup_delay_cnt;
        rx2_hop_setup_delay_cnt <= rx2_hop_setup_delay_cnt;
        tx1_hop_setup_delay_cnt <= tx1_hop_setup_delay_cnt;
        tx2_hop_setup_delay_cnt <= tx2_hop_setup_delay_cnt; 
        tx1_enable <= tx1_enable; 
        tx2_enable <= tx2_enable; 
        rx1_enable <= rx1_enable; 
        rx2_enable <= rx2_enable;
        tx1_swap_iq <= tx1_swap_iq; 
        tx2_swap_iq <= tx2_swap_iq; 
        rx1_swap_iq <= rx1_swap_iq; 
        rx2_swap_iq <= rx2_swap_iq;                
        rx1_fixed_pattern <= rx1_fixed_pattern;
        rx2_fixed_pattern <= rx2_fixed_pattern; 
        tx1_fixed_pattern <= tx1_fixed_pattern;
        tx2_fixed_pattern <= tx2_fixed_pattern;         
        adrv9001_rstn <= adrv9001_rstn;      
        dgpio_ps_t <= dgpio_ps_t;
        dgpio_ps_o <= dgpio_ps_o;
        mspi_axis_tdata <= mspi_axis_tdata;
        mspi_axis_tvalid <= 1'b0;       
        mspi_axis_enable <= mspi_axis_enable;
        hopping_mode <= hopping_mode;
        hop_dgpio_delay_cnt <= hop_dgpio_delay_cnt;
        hop_trig <= 'd0;
        next_hop_enable_mask <= next_hop_enable_mask;
        enable_pl_hop_trig <= enable_pl_hop_trig;
        rx1_ps_setup <= 'd0;
        rx2_ps_setup <= 'd0;
        tx1_ps_setup <= 'd0;      
        tx2_ps_setup <= 'd0;        
        hop_trig_clear <= 'd0;
        hop_trig_enable <= hop_trig_enable;
      end
    endcase
  end else begin
    rx1_hop_setup_delay_cnt <= rx1_hop_setup_delay_cnt;
    rx2_hop_setup_delay_cnt <= rx2_hop_setup_delay_cnt;
    tx1_hop_setup_delay_cnt <= tx1_hop_setup_delay_cnt;
    tx2_hop_setup_delay_cnt <= tx2_hop_setup_delay_cnt;   
    tx1_enable <= tx1_enable; 
    tx2_enable <= tx2_enable; 
    rx1_enable <= rx1_enable; 
    rx2_enable <= rx2_enable;      
    tx1_swap_iq <= tx1_swap_iq; 
    tx2_swap_iq <= tx2_swap_iq; 
    rx1_swap_iq <= rx1_swap_iq; 
    rx2_swap_iq <= rx2_swap_iq;        
    rx1_fixed_pattern <= rx1_fixed_pattern;
    rx2_fixed_pattern <= rx2_fixed_pattern; 
    tx1_fixed_pattern <= tx1_fixed_pattern;
    tx2_fixed_pattern <= tx2_fixed_pattern;     
    adrv9001_rstn <= adrv9001_rstn;      
    dgpio_ps_t <= dgpio_ps_t;
    dgpio_ps_o <= dgpio_ps_o; 
    mspi_axis_tdata <= mspi_axis_tdata;
    mspi_axis_tvalid <= 1'b0;              
    mspi_axis_enable <= mspi_axis_enable;    
    hopping_mode <= hopping_mode;
    hop_dgpio_delay_cnt <= hop_dgpio_delay_cnt;
    hop_trig <= 'd0;    
    next_hop_enable_mask <= next_hop_enable_mask;
    enable_pl_hop_trig <= enable_pl_hop_trig;
    rx1_ps_setup <= 'd0;
    rx2_ps_setup <= 'd0;
    tx1_ps_setup <= 'd0;      
    tx2_ps_setup <= 'd0;    
    hop_trig_clear <= 'd0;
    hop_trig_enable <= hop_trig_enable;
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
        5'd0:  reg_data_out <= {31'h0, tx1_enable};
        5'd1:  reg_data_out <= {31'h0, tx2_enable};
        5'd2:  reg_data_out <= {31'h0, rx1_enable};
        5'd3:  reg_data_out <= {31'h0, rx2_enable};
        5'd4:  reg_data_out <= {31'h0, adrv9001_rstn};
        5'd5:  reg_data_out <= {16'h0, dgpio_ps_t};
        5'd6:  reg_data_out <= {16'h0, dgpio_ps_i};
        5'd7:  reg_data_out <= {29'h0, tx1_data_src};
        5'd8:  reg_data_out <= {29'h0, tx2_data_src};
        5'd9:  reg_data_out <= {30'h0, rx2_ramp_detected, rx1_ramp_detected};
        5'd10: reg_data_out <= {30'h0, rx2_pn15_detected, rx1_pn15_detected};
        5'd11: reg_data_out <= {30'h0, rx2_fixed_detected, rx1_fixed_detected};

        5'd12: reg_data_out <= rx1_fixed_pattern;
        5'd13: reg_data_out <= rx2_fixed_pattern;
        5'd14: reg_data_out <= tx1_fixed_pattern;
        5'd15: reg_data_out <= tx2_fixed_pattern;
                
        5'd16: reg_data_out <= {8'h0, rx1_hop_setup_delay_cnt}; 
        5'd17: reg_data_out <= {8'h0, rx2_hop_setup_delay_cnt};         
        5'd18: reg_data_out <= {8'h0, tx1_hop_setup_delay_cnt}; 
        5'd19: reg_data_out <= {8'h0, tx2_hop_setup_delay_cnt}; 
        5'd20: reg_data_out <= {8'h0, hop_dgpio_delay_cnt};        
       
        5'd21: begin 
          reg_data_out <= {23'h0, sspi_axis_tvalid, sspi_axis_tdata};   
        end                                       
    
        5'd23: reg_data_out <= {29'h0, hop_trig_status, hop_trig_enable, hop_trig_clear };   
        5'd24: reg_data_out <= {31'h0, hop_trig};    
        5'd25: reg_data_out <= {28'h0, next_hop_enable_mask};
        5'd26: reg_data_out <= {31'h0, enable_pl_hop_trig};
        5'd27: reg_data_out <= {28'h0, tx1_swap_iq, tx2_swap_iq, rx1_swap_iq, rx2_swap_iq};
        5'd28: reg_data_out <= {31'h0, hopping_mode};                                  
        5'd30: reg_data_out <= {28'h0, tx1_ps_setup, tx2_ps_setup, rx1_ps_setup, rx2_ps_setup};
        
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
