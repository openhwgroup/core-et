// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_alu — Integer ALU.
//
// Pure combinational. Performs ADD/SUB, shifts, logic ops, compares,
// PACKB, and BITMIXB.
//
// Line-by-line translation of: intpipe_alu (etcore-soc)
//
// Note: The original has an optional m64LFadder (gate-level 64-bit
// carry-lookahead adder) behind `ifdef mmi_adder. This define is
// never set in the default build, so the + operator path is used.
// We translate the default (ifndef) path.

module intpipe_alu
  import minion_pkg::*;
(
  // Control
  input  dw_ctrl_e              dw,
  input  alu_func_e             fn,
  // Data input
  input  logic [XregSize-1:0]   in1,
  input  logic [XregSize-1:0]   in2,
  // Output
  output logic [XregSize-1:0]   out,
  output logic [XregSize-1:0]   adder_out
);

  localparam int unsigned WregSize = 32;
  localparam int unsigned WregMsb  = WregSize - 1;
  localparam int unsigned XregMsb  = XregSize - 1;

  // Helper function to reverse the bits
  function automatic [XregSize-1:0] reverse_bits;
    input [XregSize-1:0] in;
    for (integer i = 0; i < XregSize; i++)
      reverse_bits[i] = in[XregMsb - i];
  endfunction

  // ADD/SUB and compares
  logic [XregSize-1:0] in2_inv;
  logic [XregSize-1:0] in1_xor_in2;
  logic                is_sub;
  logic                cmp_inverted;
  logic                cmp_equal;
  logic                cmp_unsigned;
  logic                is_cmp;
  logic                cmp_res;

  always_comb begin
    // Computes the ADD/SUB
    is_sub = fn[3];
    in2_inv = is_sub ? ~in2 : in2;
    adder_out = in1 + in2_inv + XregSize'(is_sub);
    in1_xor_in2 = in1 ^ in2_inv;

    // SLT, SLTU
    is_cmp = (fn == AluSeq) || (fn == AluSne) || (fn >= AluSlt);
    cmp_inverted = fn[0];
    cmp_unsigned = fn[1];
    cmp_equal    = !fn[3];
    cmp_res   = cmp_equal                          ? (in1_xor_in2 == '0)
              : (in1[XregMsb] == in2[XregMsb])     ? adder_out[XregMsb]
              : cmp_unsigned                        ? in2[XregMsb]
              :                                       in1[XregMsb];
    cmp_res = (cmp_inverted ^ cmp_res) & is_cmp;
  end

  // Shifts
  logic [XregSize*2-1:0]  shift_in_ext;
  logic [XregSize-1:0]    shift_out_ext;
  logic [XregSize-1:0]    shift_in;
  logic [XregSize-1:0]    shift_in_mux;
  logic [XregSize-1:0]    shift_out;
  logic [WregSize-1:0]    shift_in_hi_32;
  logic [WregSize-1:0]    shift_in_hi;
  logic [5:0]             shift_amount;

  always_comb begin
    // Generates shifter input based on 64b/32b
    shift_in_hi_32 = {WregSize{is_sub && in1[WregMsb]}};
    shift_in_hi    = dw ? in1[63:32] : shift_in_hi_32;
    shift_in       = { shift_in_hi, in1[WregSize-1:0] };
    shift_amount   = { in2[5] && dw, in2[4:0] };

    // Reverse bits for shift left
    shift_in_mux = ((fn == AluSr) || (fn == AluSra)) ? shift_in : reverse_bits(shift_in);

    // Replicates the sign if operation is a sub
    shift_in_ext = { {XregSize{is_sub && shift_in[XregMsb]}}, shift_in_mux };

    // Shifter itself
    /* verilator lint_off WIDTHTRUNC */  // 128-bit shift result truncated to 64 bits (same as original)
    shift_out_ext = (shift_in_ext >> shift_amount);
    /* verilator lint_on WIDTHTRUNC */
    shift_out = ((fn == AluSr) || (fn == AluSra)) ? shift_out_ext[XregSize-1:0]
              : (fn == AluSl)                      ? reverse_bits(shift_out_ext[XregSize-1:0])
              :                                      '0;
  end

  // Logic
  logic [XregSize-1:0] log_res;

  always_comb begin
    // AND, OR, XOR
    log_res  = ((fn == AluXor) || (fn == AluOr))  ? in1_xor_in2          : '0;
    log_res |= ((fn == AluOr)  || (fn == AluAnd)) ? (in1 & in2)          : '0;
  end

  // Special instructions

  // PACKB
  logic [XregSize-1:0] packb_res;

  always_comb begin
    logic [15:0] packb_tmp;
    packb_tmp = {in2[7:0], in1[7:0]};
    packb_res = XregSize'(packb_tmp);
  end

  // BITMIXB
  logic [XregSize-1:0] bitmixb_res;

  always_comb begin
    logic [15:0] bitmixb_tmp;

    for (integer result_bit = 0; result_bit < 16; result_bit++) begin
      logic [4:0] low_sel_bit;
      logic [4:0] high_sel_bit;
      low_sel_bit = 0;
      high_sel_bit = 8;
      for (integer mask_bit = 0; mask_bit < result_bit; mask_bit++) begin
        if (in1[mask_bit])
          high_sel_bit = high_sel_bit + 1;
        else
          low_sel_bit = low_sel_bit + 1;
      end
      /* verilator lint_off WIDTHEXPAND */  // 5-bit index into 64-bit array (max value 23, fits in 6 bits)
      bitmixb_tmp[result_bit] = in1[result_bit] ? in2[high_sel_bit] : in2[low_sel_bit];
      /* verilator lint_on WIDTHEXPAND */
    end
    bitmixb_res = XregSize'(bitmixb_tmp[15:0]);
  end

  // Result mux
  always_comb begin
    case (fn)
      AluAdd     : out = adder_out;
      AluSub     : out = adder_out;
      AluPackb   : out = packb_res;
      AluBitmixb : out = bitmixb_res;
      default    : out = XregSize'(cmp_res) | log_res | shift_out;
    endcase

    // Sign extend bit 31 for W operations
    if (!dw)
      out = { {WregSize{out[WregMsb]}}, out[WregSize-1:0] };
  end

endmodule
