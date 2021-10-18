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
  inout  wire         adrv9001_dgpio0, 
  inout  wire         adrv9001_dgpio1,
  inout  wire         adrv9001_dgpio2,
  inout  wire         adrv9001_dgpio3,
  inout  wire         adrv9001_dgpio4,
  inout  wire         adrv9001_dgpio5,
  inout  wire         adrv9001_dgpio6,
  inout  wire         adrv9001_dgpio7,
  inout  wire         adrv9001_dgpio8,
  inout  wire         adrv9001_dgpio9,
  inout  wire         adrv9001_dgpio10,
  inout  wire         adrv9001_dgpio11                 
  
);

  
assign adrv9001_dgpio0 = (gpio_tri_t[0] == 1'b1) ? 1'bz : gpio_tri_o[0];
assign adrv9001_dgpio1 = (gpio_tri_t[1] == 1'b1) ? 1'bz : gpio_tri_o[1];
assign adrv9001_dgpio2 = (gpio_tri_t[2] == 1'b1) ? 1'bz : gpio_tri_o[2];
assign adrv9001_dgpio3 = (gpio_tri_t[3] == 1'b1) ? 1'bz : gpio_tri_o[3];
assign adrv9001_dgpio4 = (gpio_tri_t[4] == 1'b1) ? 1'bz : gpio_tri_o[4];
assign adrv9001_dgpio5 = (gpio_tri_t[5] == 1'b1) ? 1'bz : gpio_tri_o[5];
assign adrv9001_dgpio6 = (gpio_tri_t[6] == 1'b1) ? 1'bz : gpio_tri_o[6];
assign adrv9001_dgpio7 = (gpio_tri_t[7] == 1'b1) ? 1'bz : gpio_tri_o[7];
assign adrv9001_dgpio8 = (gpio_tri_t[8] == 1'b1) ? 1'bz : gpio_tri_o[8];
assign adrv9001_dgpio9 = (gpio_tri_t[9] == 1'b1) ? 1'bz : gpio_tri_o[9];
assign adrv9001_dgpio10 = (gpio_tri_t[10] == 1'b1) ? 1'bz : gpio_tri_o[10];
assign adrv9001_dgpio11 = (gpio_tri_t[11] == 1'b1) ? 1'bz : gpio_tri_o[11];
      
assign adrv9001_rstn      = gpio_tri_o[13];
assign adrv9001_tx2       = gpio_tri_o[14];
assign adrv9001_tx1       = gpio_tri_o[15];
assign adrv9001_rx2       = gpio_tri_o[16];
assign adrv9001_rx1       = gpio_tri_o[17];

endmodule

