// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX edge sample to FP32 converter.
//
// Two-stage pipeline that converts a signed fixed-point edge sample
// (SAMPLE_INT_BITS.SAMPLE_FRC_BITS) to IEEE754 FP32.
//
// Stage 1 (combinational → registered): normalize mantissa, compute exponent
// Stage 2 (combinational): round-to-nearest-even, final output

module rbox_sample_to_fp32
  import rbox_pkg::*;
/* verilator lint_off UNUSEDSIGNAL */ // Lower mantissa bits unused after normalization; MSB of mant_final implicit
#(
  parameter int unsigned SampleIntBits = EdgeSampIntBits,
  parameter int unsigned SampleFrcBits = EdgeSampFrcBits,
  localparam int unsigned SampleBits = SampleIntBits + SampleFrcBits,
  localparam int unsigned BaseExp    = 127 - SampleFrcBits
) (
  input  logic              clk_i,
  input  edge_sample_t      in_i,
  input  logic              valid_i,
  output logic [31:0]       out_o
);

  logic                    sign_d;
  logic                    sign_q;
  logic [7:0]              exp_d;
  logic [7:0]              exp_q;
  logic [7:0]              exp_final;
  logic [7:0]              norm_shift;
  logic [SampleBits-1:0]   mant_magnitude;
  logic [SampleBits-1:0]   mant_normalized_d;
  logic [SampleBits-1:0]   mant_normalized_q;

  logic [$clog2(SampleBits)-1:0] leading_one;
  logic                          round_up;

  // Stage 1: normalize
  always_comb begin
    sign_d = in_i[SampleBits-1];
    mant_magnitude = sign_d ? (~in_i + SampleBits'(1)) : in_i;

    // Leading-one detector: find position of highest set bit.
    // Replicates the original's MSB-to-LSB scan with short-circuit:
    //   leading_one = SAMPLE_BITS-1;
    //   for(b=0; (b<SAMPLE_BITS) && (~mant[MSB-b]); b++) leading_one--;
    // Uses a constant-bound loop with a "found" flag for synthesizability.
    leading_one = $clog2(SampleBits)'(SampleBits - 1);
    begin : lod_scan
      logic found;
      found = 1'b0;
      for (int b = 0; b < SampleBits; b++) begin
        if (!found && !mant_magnitude[SampleBits - 1 - b])
          leading_one = leading_one - $clog2(SampleBits)'(1);
        else
          found = 1'b1;
      end
    end

    exp_d = 8'(BaseExp) + 8'(leading_one);

    norm_shift = 8'(SampleBits - 1) - 8'(leading_one);

    mant_normalized_d = mant_magnitude << norm_shift;
  end

  // Pipeline registers (enable-only, no reset)
  always_ff @(posedge clk_i) begin
    if (valid_i) begin
      mant_normalized_q <= mant_normalized_d;
      sign_q            <= sign_d;
      exp_q             <= exp_d;
    end
  end

  // Stage 2: round and output
  logic [25:0] mant_normalized_msb;
  logic [24:0] mant_round;
  logic [23:0] mant_final;

  always_comb begin
    mant_normalized_msb = mant_normalized_q[SampleBits-1 -: 26];

    round_up =  (mant_normalized_msb[1:0] == 2'b11)
             || (mant_normalized_msb[2] && (mant_normalized_msb[1:0] == 2'b10));
    mant_round = round_up ? {1'b0, mant_normalized_msb[25:2]} + 25'd1
                          : {1'b0, mant_normalized_msb[25:2]};

    exp_final  = mant_round[24] ? exp_q + 8'd1 : exp_q;
    mant_final = mant_round[24] ? mant_round[24:1] : mant_round[23:0];

    out_o = {sign_q, exp_final, mant_final[22:0]};
  end

  /* verilator lint_on UNUSEDSIGNAL */

endmodule
