// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module intpipe_alu (
  // Control
  input  core_dw_ctrl        dw,
  input  core_alu_func       fn,
  // Data input
  input  logic [`XREG_RANGE] in1,
  input  logic [`XREG_RANGE] in2,
  // Output
  output logic [`XREG_RANGE] out,
  output logic [`XREG_RANGE] adder_out
);

  // Helper function to reverse the bits
  function automatic [`XREG_RANGE] reverse_bits;
    input [`XREG_RANGE] in;
    for(integer i = 0; i < `XREG_SIZE; i++)
      reverse_bits[i] = in[`XREG_MSB - i];
  endfunction

  // ADD/SUB and compares

  logic [`XREG_RANGE] in2_inv;      // Second source bitwise negate
  logic [`XREG_RANGE] in1_xor_in2;  // Xor of both inputs
  logic               is_sub;       // Is a sub
  logic               cmp_inverted; // Invert the result of comparation
  logic               cmp_equal;    // The compare operation is equal
  logic               cmp_unsigned; // The compare operation is unsigned
  logic               is_cmp;       // The operation is a compare
  logic               cmp_res;      // Result of the compare

  `ifdef mmi_adder
    m64LFadder mmi_adder(.a(in1), .b(in2_inv), .carryinb(!is_sub), .sum(adder_out));
  `endif

  always_comb begin
    // Computes the ADD/SUB
    is_sub = fn[3];
    in2_inv = is_sub ? ~in2 : in2;
    `ifndef mmi_adder
      adder_out = in1 + in2_inv + `ZX(`XREG_SIZE, is_sub);
    `endif
    in1_xor_in2 = in1 ^ in2_inv;

    // SLT, SLTU
    is_cmp = (fn == core_alu_func_SEQ) || (fn == core_alu_func_SNE) || (fn >= core_alu_func_SLT);
    cmp_inverted = fn[0];
    cmp_unsigned = fn[1];
    cmp_equal    = !fn[3];
    cmp_res      = cmp_equal                             ? (in1_xor_in2 == `XREG_SIZE'b0)
                 : (in1[`XREG_MSB] == in2[`XREG_MSB])    ? adder_out[`XREG_MSB]
                 : cmp_unsigned                          ? in2[`XREG_MSB]
                 :                                         in1[`XREG_MSB];
    cmp_res = (cmp_inverted ^ cmp_res) & is_cmp;
  end

  // Shifts
  logic [`XREG_SIZE*2-1:0] shift_in_ext;
  logic [`XREG_SIZE-1:0]   shift_out_ext;
  logic [`XREG_RANGE]      shift_in;
  logic [`XREG_RANGE]      shift_in_mux;
  logic [`XREG_RANGE]      shift_out;
  logic [`WREG_RANGE]      shift_in_hi_32;
  logic [`WREG_RANGE]      shift_in_hi;
  logic [5:0]              shift_amount;

  always_comb begin
    // Generates shifter input based on 64b/32b
    shift_in_hi_32 = {`WREG_SIZE{is_sub && in1[`WREG_MSB]}};
    shift_in_hi    = dw ? in1[63:32] : shift_in_hi_32;
    shift_in       = { shift_in_hi, in1[`WREG_RANGE] };
    shift_amount   = { in2[5] && dw, in2[4:0] };

    // Reverse bits for shift left
    shift_in_mux = ((fn == core_alu_func_SR) || (fn == core_alu_func_SRA)) ? shift_in : reverse_bits(shift_in);

    // Replicates the sign if operation is a sub
    shift_in_ext = { {`XREG_SIZE{is_sub && shift_in[`XREG_MSB]}}, shift_in_mux };

    // Shifter itself
    shift_out_ext = (shift_in_ext >> shift_amount);
    shift_out = ((fn == core_alu_func_SR) || (fn == core_alu_func_SRA)) ? shift_out_ext[`XREG_RANGE]
              : (fn == core_alu_func_SL)                                ? reverse_bits(shift_out_ext[`XREG_RANGE])
              :                                                           `XREG_SIZE'b0;
  end

  // Logic
  logic [`XREG_RANGE] log_res;

  always_comb begin
    // AND, OR, XOR
    log_res  = ((fn == core_alu_func_XOR) || (fn == core_alu_func_OR))  ? in1_xor_in2 : `XREG_SIZE'b0;
    log_res |= ((fn == core_alu_func_OR)  || (fn == core_alu_func_AND)) ? in1 & in2   : `XREG_SIZE'b0;
  end

  // Special instructions

  // PACKB
  logic [`XREG_RANGE] packb_res;

  always_comb begin
    logic [15:0] packb_tmp;

    packb_tmp = {in2[7:0], in1[7:0]};
    packb_res = `ZX(`XREG_SIZE, packb_tmp);
  end

  // BITMIXB
  logic [`XREG_RANGE] bitmixb_res;

  always_comb begin
    logic [15:0] bitmixb_tmp;

    // Select bits starting from bit 0 or bit 8 of the second source register based
    // on a mask in the first source register.  For every every result bit, up to 16
    // bits, if the corresponding mask bit is 0 then select the next bit from the
    // low bits, if the mask bit is 1 then select the next bit from the high
    // bits.
    for(integer result_bit = 0; result_bit < 16; result_bit++) begin
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
      bitmixb_tmp[result_bit] = in1[result_bit] ? in2[high_sel_bit] : in2[low_sel_bit];
    end
    bitmixb_res = `ZX(`XREG_SIZE, bitmixb_tmp[15:0]);
  end

  // Result mux
  always_comb begin
    case (fn)
      core_alu_func_ADD:     out = adder_out;                                // Select adder output for ADD/SUB
      core_alu_func_SUB:     out = adder_out;                                // Select adder output for ADD/SUB
      core_alu_func_PACKB:   out = packb_res;
      core_alu_func_BITMIXB: out = bitmixb_res;
      default:               out = `ZX(`XREG_SIZE, cmp_res) | log_res | shift_out;     // Select bit-wise OR of other components otherwise
    endcase

    // Sign extend bit 31 for W operations
    if(!dw)
      out = { {`WREG_SIZE{out[`WREG_MSB]}}, out[`WREG_RANGE] };
  end

endmodule

