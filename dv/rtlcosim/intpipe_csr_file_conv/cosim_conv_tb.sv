// Co-simulation testbench: intpipe_csr_file_conv (new) vs original.

`include "soc.vh"

module cosim_conv_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stimulus — struct fields flattened to bit vectors
  input  logic [63:0] stim_tensorconv_size_data,
  input  logic [47:0] stim_tensorconv_ctrl_data,
  input  logic        stim_tensorconv_size_wen,
  input  logic        stim_tensorconv_ctrl_wen,
  input  logic        stim_tensorload_wen,
  input  logic        stim_tensorfma_wen,
  input  logic        stim_cacheop_wen,

  // New outputs
  output logic        new_can_accept_new_write,
  output logic        new_conv_save_mask,
  output logic        new_conv_bits_ready,
  output logic [15:0] new_conv_bits,

  // Original outputs
  output logic        orig_can_accept_new_write,
  output logic        orig_conv_save_mask,
  output logic        orig_conv_bits_ready,
  output logic [15:0] orig_conv_bits
);

  import minion_pkg::*;

  // Cast flat stimulus to typed structs
  tensorconv_size    size_data;
  tensorconv_control ctrl_data;
  assign size_data = stim_tensorconv_size_data;
  assign ctrl_data = stim_tensorconv_ctrl_data;

  // -- New module --
  intpipe_csr_file_conv u_new (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .tensorconv_size_data   (size_data),
    .tensorconv_ctrl_data   (ctrl_data),
    .tensorconv_size_wen    (stim_tensorconv_size_wen),
    .tensorconv_ctrl_wen    (stim_tensorconv_ctrl_wen),
    .tensorload_wen         (stim_tensorload_wen),
    .tensorfma_wen          (stim_tensorfma_wen),
    .cacheop_wen            (stim_cacheop_wen),
    .can_accept_new_write   (new_can_accept_new_write),
    .conv_save_mask         (new_conv_save_mask),
    .conv_bits_ready        (new_conv_bits_ready),
    .conv_bits              (new_conv_bits)
  );

  // -- Original module --
  intpipe_csr_file_conv_orig u_orig (
    .clock                  (clk_i),
    .reset                  (~rst_ni),
    .tensorconv_size_data   (size_data),
    .tensorconv_ctrl_data   (ctrl_data),
    .tensorconv_size_wen    (stim_tensorconv_size_wen),
    .tensorconv_ctrl_wen    (stim_tensorconv_ctrl_wen),
    .tensorload_wen         (stim_tensorload_wen),
    .tensorfma_wen          (stim_tensorfma_wen),
    .cacheop_wen            (stim_cacheop_wen),
    .can_accept_new_write   (orig_can_accept_new_write),
    .conv_save_mask         (orig_conv_save_mask),
    .conv_bits_ready        (orig_conv_bits_ready),
    .conv_bits              (orig_conv_bits)
  );

endmodule
