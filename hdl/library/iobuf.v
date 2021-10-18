`timescale 1ns/100ps

module iobuf #(

  parameter     WIDTH = 1) (

  input       [(WIDTH-1):0]  io_t,
  input       [(WIDTH-1):0]  io_i,
  output      [(WIDTH-1):0]  io_o,
  inout       [(WIDTH-1):0]  io);


  genvar n;
  generate
  for (n = 0; n < WIDTH; n = n + 1) begin: iobuf
  assign io_o[n] = io[n];
  assign io[n] = (io_t[n] == 1'b1) ? 1'bz : io_i[n];
  end
  endgenerate

endmodule

// ***************************************************************************
// ***************************************************************************
