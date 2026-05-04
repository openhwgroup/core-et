// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module trans_top
  import minion_pkg::*;
  import vpu_pkg::*;
  #(
    parameter bit EnableExtraTrans = EnableExtraTransDefault
  )
(
  input  logic                     clk_i,
  input  logic                     rst_ni,
  input  logic                     ex_valid_i,
  /* verilator lint_off UNUSEDSIGNAL */  // The original trans_top interface carries the full vpu_input_t, but this block only consumes cmd and in2.
  input  vpu_input_t               ex_in_bits_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic [VpuFlagsSz-1:0]    ex_txfma_exc_i,
  output trans_fma_coefficients_t  f8_rom_response_o
);

  localparam int unsigned TransRcpAddrSize    = 7;
  localparam int unsigned TransLogAddrSize    = 6;
  localparam int unsigned TransExpAddrSize    = 6;
  localparam int unsigned TransSinAddrSize    = 6;
  localparam int unsigned TransRomIndexWidth  = TransRomMaxIndexSize;
  localparam logic [31:0] SinP1TaylorC0       = 32'b11001001000011111101101100000000;

  logic                     f2_trans_valid_q;
  logic                     f3_trans_valid_q;
  logic                     f4_trans_valid_q;
  logic                     f5_trans_valid_q;
  logic                     f6_trans_valid_q;
  logic                     f7_trans_valid_q;

  /* verilator lint_off UNUSEDSIGNAL */  // The original ROM address extraction ignores bit 31 and bits [15:0].
  logic [VpuDataSSz-1:0]    f2_rom_raddr_q;
  /* verilator lint_on UNUSEDSIGNAL */
  vpu_cmd_e                 f2_rom_cmd_q;
  logic                     f2_exc_q;
  logic                     f3_exc_q;
  logic                     f4_exc_q;
  logic                     f5_exc_q;
  logic                     f6_exc_q;
  logic                     f7_exc_q;
  logic                     f8_exc_q;

  logic [TransSinAddrSize-1:0] f2_sin_addr_d;
  logic                        f2_sin_taylor_d;
  logic                        f3_sin_taylor_q;
  logic                        f4_sin_taylor_q;
  logic                        f5_sin_taylor_q;

  trans_rom_ctrl_t            f2_rom_ctrl_d;
  trans_rom_ctrl_t            f3_rom_ctrl_q;
  trans_rom_ctrl_t            f4_rom_ctrl_q;
  trans_rom_ctrl_t            f5_rom_ctrl_q;

  trans_rom_index_t           f5_rom_rcp1_index_d;
  trans_rom_index_t           f5_rom_rcp2_index_d;
  /* verilator lint_off UNUSEDSIGNAL */  // These extra-trans indices are only consumed when EnableExtraTrans=1.
  trans_rom_index_t           f5_rom_rsqrt1_index_d;
  trans_rom_index_t           f5_rom_rsqrt2_index_d;
  trans_rom_index_t           f5_rom_log1_index_d;
  trans_rom_index_t           f5_rom_log2_index_d;
  trans_rom_index_t           f5_rom_exp1_index_d;
  trans_rom_index_t           f5_rom_exp2_index_d;
  trans_rom_index_t           f5_rom_sin1_index_d;
  trans_rom_index_t           f5_rom_sin2_index_d;
  /* verilator lint_on UNUSEDSIGNAL */

  trans_fma_coefficients_t    f5_rom_rcp1_coefs;
  trans_fma_coefficients_t    f5_rom_rcp2_coefs;
  trans_fma_coefficients_t    f5_rom_rsqrt1_coefs;
  trans_fma_coefficients_t    f5_rom_rsqrt2_coefs;
  trans_fma_coefficients_t    f5_rom_log1_coefs;
  trans_fma_coefficients_t    f5_rom_log2_coefs;
  trans_fma_coefficients_t    f5_rom_exp1_coefs;
  trans_fma_coefficients_t    f5_rom_exp2_coefs;
  trans_fma_coefficients_t    f5_rom_sin1_coefs;
  trans_fma_coefficients_t    f5_rom_sin2_coefs;

  trans_fma_coefficients_t    f5_rom_response_l;
  trans_fma_coefficients_t    f6_rom_response_q;
  trans_fma_coefficients_t    f7_rom_response_q;

  logic                       ex_exc_int_d;

  always_comb begin
    if (ex_in_bits_i.sigs.cmd == VpuTransExpRr) begin
      ex_exc_int_d = |ex_txfma_exc_i;
    end else begin
      ex_exc_int_d = f8_exc_q;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f2_trans_valid_q <= 1'b0;
    end else begin
      f2_trans_valid_q <= ex_valid_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (ex_valid_i) begin
      f2_rom_raddr_q <= ex_in_bits_i.in2;
      f2_rom_cmd_q   <= ex_in_bits_i.sigs.cmd;
      f2_exc_q       <= ex_exc_int_d;
    end
  end

  always_comb begin
    f2_sin_addr_d   = '0;
    f2_sin_taylor_d = 1'b0;

    if (EnableExtraTrans) begin
      unique case (f2_rom_raddr_q[30:23])
        8'd124:   f2_sin_addr_d = {1'b1,  f2_rom_raddr_q[22:18]};
        8'd123:   f2_sin_addr_d = {1'b0, 1'b1, f2_rom_raddr_q[22:19]};
        8'd122:   f2_sin_addr_d = {2'b0, 1'b1, f2_rom_raddr_q[22:20]};
        8'd121:   f2_sin_addr_d = {4'b0, 1'b1, f2_rom_raddr_q[22]};
        default: begin
        end
      endcase

      f2_sin_taylor_d = f2_rom_raddr_q[30:23] < 8'd120;
    end
  end

  always_comb begin
    f2_rom_ctrl_d = '0;
    unique case (f2_rom_cmd_q)
      VpuTransRcpRr: begin
        f2_rom_ctrl_d.cmd   = TransRomRcp1Id;
        f2_rom_ctrl_d.index = {{(TransRomIndexWidth-TransRcpAddrSize){1'b0}}, f2_rom_raddr_q[22:16]};
      end
      VpuTransRcpFma1: begin
        f2_rom_ctrl_d.cmd   = TransRomRcp2Id;
        f2_rom_ctrl_d.index = {{(TransRomIndexWidth-TransRcpAddrSize){1'b0}}, f2_rom_raddr_q[22:16]};
      end
      VpuTransRsqrtRr: if (EnableExtraTrans) begin
        f2_rom_ctrl_d.cmd   = TransRomRsqrt1Id;
        f2_rom_ctrl_d.index = f2_rom_raddr_q[23:16];
      end
      VpuTransRsqrtFma1: if (EnableExtraTrans) begin
        f2_rom_ctrl_d.cmd   = TransRomRsqrt2Id;
        f2_rom_ctrl_d.index = f2_rom_raddr_q[23:16];
      end
      VpuTransLogRr: begin
        f2_rom_ctrl_d.cmd   = TransRomLog1Id;
        f2_rom_ctrl_d.index = {{(TransRomIndexWidth-TransLogAddrSize){1'b0}}, f2_rom_raddr_q[22:17]};
      end
      VpuTransLogFma1: begin
        f2_rom_ctrl_d.cmd   = TransRomLog2Id;
        f2_rom_ctrl_d.index = {{(TransRomIndexWidth-TransLogAddrSize){1'b0}}, f2_rom_raddr_q[22:17]};
      end
      VpuTransExpRr: begin
        f2_rom_ctrl_d.cmd   = TransRomExp1Id;
        f2_rom_ctrl_d.index = {{(TransRomIndexWidth-TransExpAddrSize){1'b0}}, f2_rom_raddr_q[23:18]};
      end
      VpuTransExpFma1: begin
        f2_rom_ctrl_d.cmd   = TransRomExp2Id;
        f2_rom_ctrl_d.index = {{(TransRomIndexWidth-TransExpAddrSize){1'b0}}, f2_rom_raddr_q[23:18]};
      end
      VpuTransSinRr: if (EnableExtraTrans) begin
        f2_rom_ctrl_d.cmd   = TransRomSin1Id;
        f2_rom_ctrl_d.index = {{(TransRomIndexWidth-TransSinAddrSize){1'b0}}, f2_sin_addr_d};
      end
      VpuTransSinP1: if (EnableExtraTrans) begin
        f2_rom_ctrl_d.cmd   = TransRomSin2Id;
        f2_rom_ctrl_d.index = {{(TransRomIndexWidth-TransSinAddrSize){1'b0}}, f2_sin_addr_d};
      end
      VpuTransSinP2: if (EnableExtraTrans) begin
        f2_rom_ctrl_d.cmd   = TransRomSin3Id;
        f2_rom_ctrl_d.index = '0;
      end
      default: begin
      end
    endcase
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f3_trans_valid_q <= 1'b0;
      f4_trans_valid_q <= 1'b0;
      f5_trans_valid_q <= 1'b0;
      f6_trans_valid_q <= 1'b0;
      f7_trans_valid_q <= 1'b0;
    end else begin
      f3_trans_valid_q <= f2_trans_valid_q;
      f4_trans_valid_q <= f3_trans_valid_q;
      f5_trans_valid_q <= f4_trans_valid_q;
      f6_trans_valid_q <= f5_trans_valid_q;
      f7_trans_valid_q <= f6_trans_valid_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (f2_trans_valid_q) begin
      f3_rom_ctrl_q    <= f2_rom_ctrl_d;
      f3_exc_q         <= f2_exc_q;
      f3_sin_taylor_q  <= f2_sin_taylor_d;
    end
    if (f3_trans_valid_q) begin
      f4_rom_ctrl_q    <= f3_rom_ctrl_q;
      f4_exc_q         <= f3_exc_q;
      f4_sin_taylor_q  <= f3_sin_taylor_q;
    end
    if (f4_trans_valid_q) begin
      f5_rom_ctrl_q    <= f4_rom_ctrl_q;
      f5_exc_q         <= f4_exc_q;
      f5_sin_taylor_q  <= f4_sin_taylor_q;
    end
    if (f5_trans_valid_q) begin
      f6_rom_response_q <= f5_rom_response_l;
      f6_exc_q          <= f5_exc_q;
    end
    if (f6_trans_valid_q) begin
      f7_rom_response_q <= f6_rom_response_q;
      f7_exc_q          <= f6_exc_q;
    end
    if (f7_trans_valid_q) begin
      f8_rom_response_o <= f7_rom_response_q;
      f8_exc_q          <= f7_exc_q;
    end
  end

  always_comb begin
    f5_rom_rcp1_index_d    = '0;
    f5_rom_rcp2_index_d    = '0;
    f5_rom_rsqrt1_index_d  = '0;
    f5_rom_rsqrt2_index_d  = '0;
    f5_rom_log1_index_d    = '0;
    f5_rom_log2_index_d    = '0;
    f5_rom_exp1_index_d    = '0;
    f5_rom_exp2_index_d    = '0;
    f5_rom_sin1_index_d    = '0;
    f5_rom_sin2_index_d    = '0;

    unique case (f5_rom_ctrl_q.cmd)
      TransRomRcp1Id:    f5_rom_rcp1_index_d   = f5_rom_ctrl_q.index;
      TransRomRcp2Id:    f5_rom_rcp2_index_d   = f5_rom_ctrl_q.index;
      TransRomRsqrt1Id:  f5_rom_rsqrt1_index_d = f5_rom_ctrl_q.index;
      TransRomRsqrt2Id:  f5_rom_rsqrt2_index_d = f5_rom_ctrl_q.index;
      TransRomLog1Id:    f5_rom_log1_index_d   = f5_rom_ctrl_q.index;
      TransRomLog2Id:    f5_rom_log2_index_d   = f5_rom_ctrl_q.index;
      TransRomExp1Id:    f5_rom_exp1_index_d   = f5_rom_ctrl_q.index;
      TransRomExp2Id:    f5_rom_exp2_index_d   = f5_rom_ctrl_q.index;
      TransRomSin1Id:    f5_rom_sin1_index_d   = f5_rom_ctrl_q.index;
      TransRomSin2Id:    f5_rom_sin2_index_d   = f5_rom_ctrl_q.index;
      default: begin
      end
    endcase
  end

  trans_rcp_rom_case_c1c2 u_trans_rcp_rom_c1c2 (
    .index_i (f5_rom_rcp1_index_d),
    .data_o  (f5_rom_rcp1_coefs)
  );

  trans_rcp_rom_case_c0 u_trans_rcp_rom_c0 (
    .index_i (f5_rom_rcp2_index_d),
    .data_o  (f5_rom_rcp2_coefs)
  );

  generate
    if (EnableExtraTrans) begin : gen_extra_trans_roms
      trans_rsqrt_rom_case_c1c2 u_trans_rsqrt_rom_c1c2 (
        .index_i (f5_rom_rsqrt1_index_d),
        .data_o  (f5_rom_rsqrt1_coefs)
      );

      trans_rsqrt_rom_case_c0 u_trans_rsqrt_rom_c0 (
        .index_i (f5_rom_rsqrt2_index_d),
        .data_o  (f5_rom_rsqrt2_coefs)
      );

      trans_sin_rom_case_c1c2 u_trans_sin_rom_c1c2 (
        .index_i (f5_rom_sin1_index_d),
        .data_o  (f5_rom_sin1_coefs)
      );

      trans_sin_rom_case_c0 u_trans_sin_rom_c0 (
        .index_i (f5_rom_sin2_index_d),
        .data_o  (f5_rom_sin2_coefs)
      );
    end else begin : gen_no_extra_trans_roms
      assign f5_rom_rsqrt1_coefs = '0;
      assign f5_rom_rsqrt2_coefs = '0;
      assign f5_rom_sin1_coefs   = '0;
      assign f5_rom_sin2_coefs   = '0;
    end
  endgenerate

  trans_log_rom_case_c1c2 u_trans_log_rom_c1c2 (
    .index_i (f5_rom_log1_index_d),
    .data_o  (f5_rom_log1_coefs)
  );

  trans_log_rom_case_c0 u_trans_log_rom_c0 (
    .index_i (f5_rom_log2_index_d),
    .data_o  (f5_rom_log2_coefs)
  );

  trans_exp_rom_case_c1c2 u_trans_exp_rom_c1c2 (
    .index_i (f5_rom_exp1_index_d),
    .data_o  (f5_rom_exp1_coefs)
  );

  trans_exp_rom_case_c0 u_trans_exp_rom_c0 (
    .index_i (f5_rom_exp2_index_d),
    .data_o  (f5_rom_exp2_coefs)
  );

  // Preserve the original partial-assignment behavior for TRANS_ROM_SIN3_ID:
  // only the `taylor` field is written in that branch, so the other packed
  // fields remain latched from the previous selected response.
  /* verilator lint_off NOLATCH */  // TRANS_ROM_SIN3_ID intentionally preserves the previous packed coefficients and only updates the taylor bit.
  always_latch begin
    unique case (f5_rom_ctrl_q.cmd)
      TransRomRcp1Id: begin
        f5_rom_response_l = f5_rom_rcp1_coefs;
      end
      TransRomRcp2Id: begin
        f5_rom_response_l = f5_rom_rcp2_coefs;
      end
      TransRomRsqrt1Id: begin
        f5_rom_response_l = f5_rom_rsqrt1_coefs;
      end
      TransRomRsqrt2Id: begin
        f5_rom_response_l = f5_rom_rsqrt2_coefs;
      end
      TransRomLog1Id: begin
        f5_rom_response_l = f5_rom_log1_coefs;
      end
      TransRomLog2Id: begin
        f5_rom_response_l = f5_rom_log2_coefs;
      end
      TransRomExp1Id: begin
        f5_rom_response_l = f5_rom_exp1_coefs;
      end
      TransRomExp2Id: begin
        f5_rom_response_l      = f5_rom_exp2_coefs;
        f5_rom_response_l.exc  = f5_exc_q;
      end
      TransRomSin1Id: begin
        f5_rom_response_l         = f5_rom_sin1_coefs;
        f5_rom_response_l.taylor  = f5_sin_taylor_q;
      end
      TransRomSin2Id: begin
        if (f5_sin_taylor_q) begin
          f5_rom_response_l      = '0;
          f5_rom_response_l.c0   = SinP1TaylorC0;
        end else begin
          f5_rom_response_l      = f5_rom_sin2_coefs;
        end
        f5_rom_response_l.taylor = f5_sin_taylor_q;
      end
      TransRomSin3Id: begin
        f5_rom_response_l.taylor = f5_sin_taylor_q;
      end
      default: begin
        f5_rom_response_l = '0;
      end
    endcase
  end
  /* verilator lint_on NOLATCH */

endmodule : trans_top
