// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_inst_bits_stage — Pipeline register for instruction bits.
//
// Selectively flops instruction bits based on masks. Bits in MASK are
// always flopped. Bits in BrMask are flopped for branches/jumps.
// All bits are flopped if trace encoder is on, instruction is CSR,
// or there is an exception.
//
// Line-by-line translation of: intpipe_inst_bits_stage (etcore-soc)

module intpipe_inst_bits_stage
  import minion_pkg::*;
#(
  parameter logic [31:0] Mask   = 32'hFF,
  parameter logic [31:0] BrMask = 32'hFF
) (
  input  logic                  clk_i,       // gated clock
  input  logic                  enable_i,
  input  logic                  is_csr_i,
  input  logic                  is_br_jal_i,
  input  logic                  te_enable_i,
  input  logic                  xcpt_i,
  input  logic [InstSize-1:0]   in_i,

  output logic [InstSize-1:0]   out_o
);

  for (genvar i = 0; i < InstSize; i++) begin : gen_ff_bits
    logic en;

    assign en = enable_i &&
                ( Mask[i]                    ||   // bits saved in all cases
                  (BrMask[i] && is_br_jal_i) ||   // bits saved for branches/jumps
                  te_enable_i || is_csr_i);        // trace encoder on, or CSR insn

    // EN_FF(clock, en || xcpt, out[i], in[i])
    always_ff @(posedge clk_i)
      if (en || xcpt_i)
        out_o[i] <= in_i[i];
  end

endmodule
