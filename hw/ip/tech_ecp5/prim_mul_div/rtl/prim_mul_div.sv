// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_mul_div -- ECP5-friendly FF-based mul/div primitive.
//
// This implementation intentionally diverges from the generic latch-based
// body. It keeps the public request/response contract used by intpipe, while
// replacing the internal two-phase local-clock machinery with a simple
// edge-triggered implementation suitable for FPGA bring-up.

module prim_mul_div
  import minion_pkg::*;
  import dft_pkg::*;
(
  input  logic                  clk_i,
  input  logic                  rst_ni,
  input  dft_t                  dft_i,
  output logic                  req_ready,
  input  logic                  req_valid,
  input  logic                  req_valid_early,
  input  alu_func_e             req_fn,
  input  alu_func_e             req_fn_early,
  input  logic                  req_dw,
  input  logic [XregSize-1:0]   req_in1,
  input  logic [XregSize-1:0]   req_in2,
  input  minion_reg_dest_t      req_dest,
  input  logic                  kill,
  input  logic                  chicken_bit_mul_div,
  input  logic                  resp_ready,
  output logic                  resp_valid,
  output logic                  resp_valid_early,
  output logic [XregSize-1:0]   resp_data,
  output minion_reg_dest_t      resp_dest
);

  localparam logic [6:0] MulLatencyM1        = 7'd7;   // keep mul alive long enough for the leaf kill test to exercise squash behavior
  localparam logic [6:0] DivSpecialLatencyM1 = 7'd1;   // same short path for div-by-zero / overflow
  localparam logic [6:0] DivBits32           = 7'd32;
  localparam logic [6:0] DivBits64           = 7'd64;
  localparam logic [63:0] MinSigned64        = 64'h8000_0000_0000_0000;
  localparam logic [31:0] MinSigned32        = 32'h8000_0000;

  typedef enum logic [1:0] {
    StIdle,
    StDelay,
    StDivIter,
    StResp
  } state_e;

  state_e             state_q;
  logic [6:0]         count_q;
  logic [63:0]        resp_data_q;
  minion_reg_dest_t   resp_dest_q;

  logic               div_dw_q;
  logic               div_return_quot_q;
  logic               div_result_neg_q;
  logic               div_remainder_neg_q;
  logic [63:0]        dividend_mag_q;
  logic [63:0]        divisor_mag_q;
  logic [63:0]        quotient_work_q;
  /* verilator lint_off UNUSEDSIGNAL */  // The iterative divider keeps a 65-bit intermediate remainder for the subtract/compare step; bit 64 is only consumed transiently inside the step logic.
  logic [64:0]        remainder_work_q;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [5:0]         div_bit_idx;
  logic [64:0]        div_rem_step;
  logic [63:0]        div_quot_step;

  /* verilator lint_off UNUSEDSIGNAL */  // FPGA mul/div replacement does not use DFT or early request hints internally, but the public contract preserves those ports.
  logic               unused_req_valid_early;
  alu_func_e          unused_req_fn_early;
  logic               unused_chicken_bit_mul_div;
  dft_t               unused_dft;
  /* verilator lint_on UNUSEDSIGNAL */

  function automatic logic [63:0] sext32(input logic [31:0] value);
    sext32 = {{32{value[31]}}, value};
  endfunction

  function automatic logic is_mul_fn(input alu_func_e fn);
    is_mul_fn = (fn == AluMul) || (fn == AluMulh) || (fn == AluMulhu) || (fn == AluMulhsu);
  endfunction

  function automatic logic is_signed_div_fn(input alu_func_e fn);
    is_signed_div_fn = (fn == AluDiv) || (fn == AluRem);
  endfunction

  function automatic logic is_quot_result_fn(input alu_func_e fn);
    is_quot_result_fn = (fn == AluDiv) || (fn == AluDivu);
  endfunction

  function automatic logic [63:0] abs64(input logic [63:0] value);
    abs64 = value[63] ? (~value + 64'd1) : value;
  endfunction

  function automatic logic [31:0] abs32(input logic [31:0] value);
    abs32 = value[31] ? (~value + 32'd1) : value;
  endfunction

  function automatic logic [63:0] mul_result(
    input alu_func_e          fn,
    input logic               dw,
    input logic [63:0]        in1,
    input logic [63:0]        in2
  );
    logic signed [63:0] lhs_s64;
    logic signed [63:0] rhs_s64;
    logic [63:0]        lhs_u64;
    logic [63:0]        rhs_u64;
    logic signed [64:0] lhs_s65;
    logic signed [64:0] rhs_u65;
    /* verilator lint_off UNUSEDSIGNAL */  // Different mul variants consume different product halves; the unused half is intentional.
    logic signed [127:0] prod_ss64;
    logic [127:0]        prod_uu64;
    logic signed [129:0] prod_su64;
    logic signed [31:0]  lhs_s32;
    logic signed [31:0]  rhs_s32;
    logic [31:0]         lhs_u32;
    logic [31:0]         rhs_u32;
    logic signed [32:0]  lhs_s33;
    logic signed [32:0]  rhs_u33;
    logic signed [63:0]  prod_ss32;
    logic [63:0]         prod_uu32;
    logic signed [65:0]  prod_su32;
    /* verilator lint_on UNUSEDSIGNAL */

    lhs_s64   = $signed(in1);
    rhs_s64   = $signed(in2);
    lhs_u64   = in1;
    rhs_u64   = in2;
    lhs_s65   = $signed({in1[63], in1});
    rhs_u65   = $signed({1'b0, in2});
    prod_ss64 = lhs_s64 * rhs_s64;
    prod_uu64 = lhs_u64 * rhs_u64;
    prod_su64 = lhs_s65 * rhs_u65;

    lhs_s32   = $signed(in1[31:0]);
    rhs_s32   = $signed(in2[31:0]);
    lhs_u32   = in1[31:0];
    rhs_u32   = in2[31:0];
    lhs_s33   = $signed({in1[31], in1[31:0]});
    rhs_u33   = $signed({1'b0, in2[31:0]});
    prod_ss32 = lhs_s32 * rhs_s32;
    prod_uu32 = lhs_u32 * rhs_u32;
    prod_su32 = lhs_s33 * rhs_u33;

    if (dw) begin
      unique case (fn)
        AluMul:    mul_result = prod_uu64[63:0];
        AluMulh:   mul_result = prod_ss64[127:64];
        AluMulhu:  mul_result = prod_uu64[127:64];
        default:   mul_result = prod_su64[127:64];
      endcase
    end else begin
      unique case (fn)
        AluMul:    mul_result = sext32(prod_uu32[31:0]);
        AluMulh:   mul_result = sext32(prod_ss32[63:32]);
        AluMulhu:  mul_result = sext32(prod_uu32[63:32]);
        default:   mul_result = sext32(prod_su32[63:32]);
      endcase
    end
  endfunction

  function automatic logic [63:0] finalize_div_result(
    input logic               dw,
    input logic               return_quot,
    input logic               result_neg,
    input logic               remainder_neg,
    input logic [63:0]        quotient_mag,
    input logic [63:0]        remainder_mag
  );
    logic [63:0] raw_result;

    if (return_quot) begin
      raw_result = result_neg ? (~quotient_mag + 64'd1) : quotient_mag;
    end else begin
      raw_result = remainder_neg ? (~remainder_mag + 64'd1) : remainder_mag;
    end

    if (dw) begin
      finalize_div_result = raw_result;
    end else begin
      finalize_div_result = sext32(raw_result[31:0]);
    end
  endfunction

  assign req_ready = (state_q == StIdle);
  assign resp_valid = (state_q == StResp);
  assign resp_valid_early = (((state_q == StDelay) || (state_q == StDivIter)) && (count_q == 7'd1)) ||
                            ((state_q == StResp) && !resp_ready);
  assign resp_data = resp_data_q;
  assign resp_dest = resp_dest_q;

  always_comb begin
    div_bit_idx  = count_q[5:0] - 6'd1;
    div_rem_step = {remainder_work_q[63:0], dividend_mag_q[div_bit_idx]};
    div_quot_step = quotient_work_q;
    if (div_rem_step >= {1'b0, divisor_mag_q}) begin
      div_rem_step = div_rem_step - {1'b0, divisor_mag_q};
      div_quot_step[div_bit_idx] = 1'b1;
    end else begin
      div_quot_step[div_bit_idx] = 1'b0;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    logic              div_signed;
    logic              div_return_quot;
    logic              dividend_neg;
    logic              divisor_neg;
    logic              div_zero;
    logic              div_overflow;
    logic [63:0]       dividend_mag;
    logic [63:0]       divisor_mag;
    if (!rst_ni) begin
      state_q            <= StIdle;
      count_q            <= '0;
      resp_data_q        <= '0;
      resp_dest_q        <= '0;
      div_dw_q           <= 1'b0;
      div_return_quot_q  <= 1'b0;
      div_result_neg_q   <= 1'b0;
      div_remainder_neg_q <= 1'b0;
      dividend_mag_q     <= '0;
      divisor_mag_q      <= '0;
      quotient_work_q    <= '0;
      remainder_work_q   <= '0;
      unused_req_valid_early <= 1'b0;
      unused_req_fn_early    <= AluAdd;
      unused_chicken_bit_mul_div <= 1'b0;
      unused_dft         <= '0;
    end else begin
      unused_req_valid_early     <= req_valid_early;
      unused_req_fn_early        <= req_fn_early;
      unused_chicken_bit_mul_div <= chicken_bit_mul_div;
      unused_dft                 <= dft_i;

      unique case (state_q)
        StIdle: begin
          if (!kill && req_valid) begin
            resp_dest_q <= req_dest;
            if (is_mul_fn(req_fn)) begin
              resp_data_q <= mul_result(req_fn, req_dw, req_in1, req_in2);
              count_q     <= MulLatencyM1;
              state_q     <= StDelay;
            end else begin
              div_signed      = is_signed_div_fn(req_fn);
              div_return_quot = is_quot_result_fn(req_fn);

              if (req_dw) begin
                dividend_neg = div_signed && req_in1[63];
                divisor_neg  = div_signed && req_in2[63];
                dividend_mag = div_signed ? abs64(req_in1) : req_in1;
                divisor_mag  = div_signed ? abs64(req_in2) : req_in2;
                div_zero     = (req_in2 == 64'd0);
                div_overflow = div_signed && (req_in1 == MinSigned64) && (req_in2 == 64'hFFFF_FFFF_FFFF_FFFF);
              end else begin
                dividend_neg = div_signed && req_in1[31];
                divisor_neg  = div_signed && req_in2[31];
                dividend_mag = {32'b0, div_signed ? abs32(req_in1[31:0]) : req_in1[31:0]};
                divisor_mag  = {32'b0, div_signed ? abs32(req_in2[31:0]) : req_in2[31:0]};
                div_zero     = (req_in2[31:0] == 32'd0);
                div_overflow = div_signed && (req_in1[31:0] == MinSigned32) && (req_in2[31:0] == 32'hFFFF_FFFF);
              end

              if (div_zero) begin
                if (div_return_quot) begin
                  resp_data_q <= req_dw ? 64'hFFFF_FFFF_FFFF_FFFF : sext32(32'hFFFF_FFFF);
                end else begin
                  resp_data_q <= req_dw ? req_in1 : sext32(req_in1[31:0]);
                end
                count_q <= DivSpecialLatencyM1;
                state_q <= StDelay;
              end else if (div_overflow) begin
                if (div_return_quot) begin
                  resp_data_q <= req_dw ? MinSigned64 : sext32(MinSigned32);
                end else begin
                  resp_data_q <= 64'd0;
                end
                count_q <= DivSpecialLatencyM1;
                state_q <= StDelay;
              end else begin
                div_dw_q            <= req_dw;
                div_return_quot_q   <= div_return_quot;
                div_result_neg_q    <= div_signed && (dividend_neg ^ divisor_neg);
                div_remainder_neg_q <= div_signed && dividend_neg;
                dividend_mag_q      <= dividend_mag;
                divisor_mag_q       <= divisor_mag;
                quotient_work_q     <= '0;
                remainder_work_q    <= '0;
                count_q             <= req_dw ? DivBits64 : DivBits32;
                state_q             <= StDivIter;
              end
            end
          end
        end

        StDelay: begin
          if (kill) begin
            state_q <= StIdle;
          end else if (count_q == 7'd1) begin
            state_q <= StResp;
          end else begin
            count_q <= count_q - 1'b1;
          end
        end

        StDivIter: begin
          if (kill) begin
            state_q <= StIdle;
          end else begin
            quotient_work_q  <= div_quot_step;
            remainder_work_q <= div_rem_step;

            if (count_q == 7'd1) begin
              resp_data_q <= finalize_div_result(
                div_dw_q,
                div_return_quot_q,
                div_result_neg_q,
                div_remainder_neg_q,
                div_quot_step,
                div_rem_step[63:0]
              );
              state_q <= StResp;
            end else begin
              count_q <= count_q - 1'b1;
            end
          end
        end

        default: begin
          if (kill || resp_ready) begin
            state_q <= StIdle;
          end
        end
      endcase
    end
  end

endmodule
