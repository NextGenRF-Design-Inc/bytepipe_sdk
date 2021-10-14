`timescale 1ns/100ps

module gpio (

  input  wire [17:0]  gpio_tri_t,
  output wire [17:0]  gpio_tri_i,
  input  wire [17:0]  gpio_tri_o,
  output wire         adrv9001_rx1,
  output wire         adrv9001_rx2,
  output wire         adrv9001_tx1,
  output wire         adrv9001_tx2,   
  output wire         adrv9001_rstn,
  input  wire         adrv9001_irq,
  inout  wire [11:0]  adrv9001_dgpio                   
  
);
wire [11:0] dgpio_o;

assign gpio_tri_i = {5'h0, adrv9001_irq, adrv9001_dgpio};
  
assign adrv9001_dgpio[0] = (gpio_tri_t[0] == 1'b1) ? 1'bz : gpio_tri_o[0];
assign adrv9001_dgpio[1] = (gpio_tri_t[1] == 1'b1) ? 1'bz : gpio_tri_o[1];
assign adrv9001_dgpio[2] = (gpio_tri_t[2] == 1'b1) ? 1'bz : gpio_tri_o[2];
assign adrv9001_dgpio[3] = (gpio_tri_t[3] == 1'b1) ? 1'bz : gpio_tri_o[3];
assign adrv9001_dgpio[4] = (gpio_tri_t[4] == 1'b1) ? 1'bz : gpio_tri_o[4];
assign adrv9001_dgpio[5] = (gpio_tri_t[5] == 1'b1) ? 1'bz : gpio_tri_o[5];
assign adrv9001_dgpio[6] = (gpio_tri_t[6] == 1'b1) ? 1'bz : gpio_tri_o[6];
assign adrv9001_dgpio[7] = (gpio_tri_t[7] == 1'b1) ? 1'bz : gpio_tri_o[7];
assign adrv9001_dgpio[8] = (gpio_tri_t[8] == 1'b1) ? 1'bz : gpio_tri_o[8];
assign adrv9001_dgpio[9] = (gpio_tri_t[9] == 1'b1) ? 1'bz : gpio_tri_o[9];
assign adrv9001_dgpio[10] = (gpio_tri_t[10] == 1'b1) ? 1'bz : gpio_tri_o[10];
assign adrv9001_dgpio[11] = (gpio_tri_t[11] == 1'b1) ? 1'bz : gpio_tri_o[11];
      
assign adrv9001_rstn      = gpio_tri_o[13];
assign adrv9001_tx2       = gpio_tri_o[14];
assign adrv9001_tx1       = gpio_tri_o[15];
assign adrv9001_rx2       = gpio_tri_o[16];
assign adrv9001_rx1       = gpio_tri_o[17];

endmodule

