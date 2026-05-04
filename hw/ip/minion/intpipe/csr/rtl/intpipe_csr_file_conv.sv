// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_csr_file_conv — Tensor convolution boundary computation.
//
// When a new write to either tensor convolution CSR comes, starts the
// computation. Accepts new writes as long as there is no pending result
// needed by tensorfma or tensorload.
//
// Replaces: intpipe_csr_file_conv (etcore-soc)

module intpipe_csr_file_conv
  import minion_pkg::*;
(
  input  logic         clk_i,
  input  logic         rst_ni,

  // CSR information
  /* verilator lint_off UNUSEDSIGNAL */  // Tensor-convolution CSRs keep their full packed layout; this helper only consumes the fields needed for boundary generation.
  input  tensorconv_size    tensorconv_size_data,
  input  tensorconv_control tensorconv_ctrl_data,
  /* verilator lint_on UNUSEDSIGNAL */

  // CSR write enable information to know when to start computing
  input  logic         tensorconv_size_wen,
  input  logic         tensorconv_ctrl_wen,
  input  logic         tensorload_wen,
  input  logic         tensorfma_wen,
  input  logic         cacheop_wen,

  // Ready signal to CSR file to accept a new write
  output logic         can_accept_new_write,
  // Ready signal to consumers for the computed skip bits
  output logic         conv_save_mask,
  output logic         conv_bits_ready,
  output logic [15:0]  conv_bits
);

  logic [3:0] conv_idx;
  logic       clk_en;
  logic       compute_done_q, compute_done_d;
  logic       pending_q, pending_d;
  logic       trigger;
  logic       conv_save_mask_d;

  // compute_done register
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      compute_done_q <= 1'b1;
    else if (clk_en)
      compute_done_q <= compute_done_d;
  end

  // pending register
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      pending_q <= 1'b0;
    else if (clk_en)
      pending_q <= pending_d;
  end

  // conv_save_mask register
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      conv_save_mask <= 1'b0;
    else
      conv_save_mask <= conv_save_mask_d;
  end

  always_comb begin
    can_accept_new_write = !pending_q;

    pending_d = pending_q;
    if ((tensorload_wen || tensorfma_wen || cacheop_wen) && !compute_done_q)
      pending_d = 1'b1;
    if (compute_done_q)
      pending_d = 1'b0;
  end

  always_comb begin
    compute_done_d = compute_done_q;
    if (trigger)
      compute_done_d = 1'b0;
    else if (conv_idx == 4'b1111)
      compute_done_d = 1'b1;

    clk_en = trigger || !compute_done_q || pending_q;

    conv_save_mask_d = compute_done_d && !compute_done_q ? 1'b1 : conv_save_mask ? 1'b0 : conv_save_mask;
  end

  // Convolution computation registers
  logic [17:0] conv_col_pos;
  logic [17:0] conv_row_pos;
  logic [15:0] conv_col_offset, conv_col_offset_d;
  logic [15:0] conv_row_offset, conv_row_offset_d;
  logic [15:0] conv_bits_d;
  logic [3:0]  conv_idx_d;
  logic        conv_pass_result;

  always_ff @(posedge clk_i) begin
    if (clk_en) begin
      conv_bits       <= conv_bits_d;
      conv_idx        <= conv_idx_d;
      conv_col_offset <= conv_col_offset_d;
      conv_row_offset <= conv_row_offset_d;
    end
  end

  always_comb begin
    trigger = (tensorconv_size_wen || tensorconv_ctrl_wen) && can_accept_new_write;

    conv_idx_d = conv_idx + 3'b1;
    if (trigger)
      conv_idx_d = 4'b0;

    conv_col_offset_d = conv_col_offset + {{(16-$bits(tensorconv_size_data.col_step_offset)){tensorconv_size_data.col_step_offset[$bits(tensorconv_size_data.col_step_offset)-1]}}, tensorconv_size_data.col_step_offset};
    conv_row_offset_d = conv_row_offset + {{(16-$bits(tensorconv_size_data.row_step_offset)){tensorconv_size_data.row_step_offset[$bits(tensorconv_size_data.row_step_offset)-1]}}, tensorconv_size_data.row_step_offset};
    if (trigger) begin
      conv_col_offset_d = 16'b0;
      conv_row_offset_d = 16'b0;
    end
  end

  always_comb begin
    conv_col_pos = {{2{tensorconv_ctrl_data.col_pos[$bits(tensorconv_ctrl_data.col_pos)-1]}},
                    tensorconv_ctrl_data.col_pos} +
                   {{2{conv_col_offset[15]}}, conv_col_offset};
    conv_row_pos = {{2{tensorconv_ctrl_data.row_pos[$bits(tensorconv_ctrl_data.row_pos)-1]}},
                    tensorconv_ctrl_data.row_pos} +
                   {{2{conv_row_offset[15]}}, conv_row_offset};

    conv_pass_result = 1'b1;
    if (conv_col_pos[17]) conv_pass_result = 1'b0;
    if (conv_row_pos[17]) conv_pass_result = 1'b0;
    if (conv_col_pos >= {{2{1'b0}}, tensorconv_size_data.col_size}) conv_pass_result = 1'b0;
    if (conv_row_pos >= {{2{1'b0}}, tensorconv_size_data.row_size}) conv_pass_result = 1'b0;

    conv_bits_d = conv_bits;
    conv_bits_d[conv_idx] = conv_pass_result;

    conv_bits_ready = compute_done_q;
  end

endmodule
