// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module intpipe_csr_file_conv
(
    // System signals
    input logic         clock,
    input logic         reset,
    // CSR information
    input               tensorconv_size tensorconv_size_data,
    input               tensorconv_control tensorconv_ctrl_data,
    // CSR write enable information to know when to start computing
    input logic         tensorconv_size_wen,
    input logic         tensorconv_ctrl_wen,
    input logic         tensorload_wen,
    input logic         tensorfma_wen,
    input logic         cacheop_wen,
    // Ready signal to CSR file to accept a new write
    output logic        can_accept_new_write,
    // Ready signal to consumers for the computed skip bits
    output logic        conv_save_mask,
    output logic        conv_bits_ready,
    output logic [15:0] conv_bits
);

////////////////////////////////////////////////////////////////////////////////
// When a new write to either tensor convolution comes, we start the
// computation. It is ok to receive a new write to any of these registers and
// restart computation as far as there is no pending data to be sent to
// tensorfma or tensorload logic.
////////////////////////////////////////////////////////////////////////////////

logic [3:0] conv_idx;          // Which convolution bit will be updated
logic       clk_en;            // Generic clock enable of the unit
logic       compute_done;      // Computation is done
logic       compute_done_next;
logic       pending;           // Pending computation to be finished that is needed, so it can't be overwritten
logic       pending_next;
logic       trigger;           // Trigger a new computation
logic       conv_save_mask_next;
   

//         CLK    RST    EN      DOUT           DIN                   DEF
`RST_EN_FF(clock, reset, clk_en, compute_done,   compute_done_next,   1'b1)
`RST_EN_FF(clock, reset, clk_en, pending,        pending_next,        1'b0)
`RST_FF   (clock, reset,         conv_save_mask, conv_save_mask_next, 1'b0)
always_comb
begin
    // We can accept a new write when there's no pending computation to be
    // done
    can_accept_new_write = !pending;

    // Compute pending bit
    pending_next = pending;
    // Upon a new tensorload or tensorfma write and the bits are still not
    // computed, move to pending
    if((tensorload_wen || tensorfma_wen || cacheop_wen) && !compute_done)
        pending_next = 1'b1;
    if(compute_done)
        pending_next = 1'b0;
end

always_comb
begin
    // Compute the done bit
    compute_done_next = compute_done;
    // Upon a new trigger, compute is not done
    if(trigger)
        compute_done_next = 1'b0;
    // When last index is computed, we are done
    else if(conv_idx == 4'b1111)
        compute_done_next = 1'b1;

    // Clock enable
    clk_en = trigger || !compute_done || pending;

    conv_save_mask_next = compute_done_next && !compute_done ? 1'b1 : conv_save_mask? 1'b0 : conv_save_mask;
      
end

////////////////////////////////////////////////////////////////////////////////
// Computes the new set of results after a write to either convolution
// register.
////////////////////////////////////////////////////////////////////////////////

logic [17:0] conv_col_pos;         // Current col pass position
logic [17:0] conv_row_pos;         // Current row pass position
logic [15:0] conv_col_offset;      // Accumulated col offset for the different steps
logic [15:0] conv_col_offset_next;
logic [15:0] conv_row_offset;      // Accumulated row offset for the different steps
logic [15:0] conv_row_offset_next;
logic [15:0] conv_bits_next;       // Contents of the convolution bits for the next cycle
logic [3:0]  conv_idx_next;
logic        conv_pass_result;     // Results of convolution check for one pass

//     CLK    EN      DOUT             DIN
`EN_FF(clock, clk_en, conv_bits,       conv_bits_next)
`EN_FF(clock, clk_en, conv_idx,        conv_idx_next)
`EN_FF(clock, clk_en, conv_col_offset, conv_col_offset_next)
`EN_FF(clock, clk_en, conv_row_offset, conv_row_offset_next)

always_comb
begin
    // Trigger when there's a new write to convolution CSR and it can be
    // accepted
    trigger = (tensorconv_size_wen || tensorconv_ctrl_wen) && can_accept_new_write;

    // Updates the convolution index
    conv_idx_next = conv_idx + 3'b1;
    if(trigger)
    begin
        conv_idx_next = 4'b0;
    end

    // Computes next accumulated offsets
    conv_col_offset_next = conv_col_offset + `SX(16, tensorconv_size_data.col_step_offset);
    conv_row_offset_next = conv_row_offset + `SX(16, tensorconv_size_data.row_step_offset);
    if(trigger)
    begin
        conv_col_offset_next = 16'b0;
        conv_row_offset_next = 16'b0;
    end
end

always_comb
begin
    // Computes current 2D position in the map
    conv_col_pos = $signed(tensorconv_ctrl_data.col_pos) + $signed(conv_col_offset);
    conv_row_pos = $signed(tensorconv_ctrl_data.row_pos) + $signed(conv_row_offset);

    // Computes the pass/fail bit
    conv_pass_result = 1'b1;

    // Negative position
    if(conv_col_pos[17]) conv_pass_result = 1'b0;
    if(conv_row_pos[17]) conv_pass_result = 1'b0;
    // Outside position
    if(conv_col_pos >= tensorconv_size_data.col_size) conv_pass_result = 1'b0;
    if(conv_row_pos >= tensorconv_size_data.row_size) conv_pass_result = 1'b0;

    // Updates the convolution bits
    conv_bits_next = conv_bits;
    conv_bits_next[conv_idx] = conv_pass_result;

    conv_bits_ready = compute_done;
end

endmodule

