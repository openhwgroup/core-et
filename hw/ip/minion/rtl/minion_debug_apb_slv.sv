// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion debug APB slave.
//
// Mechanical translation of minion_debug_apb_slv.v. This block decodes the
// minion-local debug APB window, exposes the ddata0 register bank back to the
// debug module, and drives frontend program-buffer writes / execute strobes.

module minion_debug_apb_slv
  import minion_pkg::*;
(
  input  logic                                      clk_i,
  input  logic                                      rst_ni,

  input  logic [MinRegApbAddrWidth-1:0]             apb_paddr_i,
  input  logic                                      apb_pwrite_i,
  input  logic                                      apb_psel_i,
  input  logic                                      apb_penable_i,
  input  logic [MinionDebugApbDataWidth-1:0]        apb_pwdata_i,

  output logic                                      apb_pready_o,
  output logic [MinionDebugApbDataWidth-1:0]        apb_prdata_o,
  output logic                                      apb_pslverr_o,

  input  logic [63:0]                               csr_wdata_i,
  input  logic [NrThreads-1:0]                      update_ddata0_i,
  output logic [NrThreads-1:0][63:0]               read_ddata0_o,

  output logic [MinionDebugApbDataWidth-1:0]       ffb_wdata_o,
  output logic [MinionDebugFfbWords-1:0]           ffb_en_o,
  output logic                                     ffb_thread_sel_o,

  output logic [NrThreads-1:0]                     ex_program_buffer_o,
  input  logic [NrThreads-1:0]                     in_debug_mode_i
);

  localparam int unsigned NumRegs = int'(minion_debug_invalid) + 1;

  logic                                   thread_sel;
  /* verilator lint_off UNUSEDSIGNAL */  // The debug APB keeps the full original register index field even though only the low enum bits are decoded here.
  logic [MinionDebugApbAddrWidth-2:0]     reg_sel;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [AdEsrProtBitsSize-1:0]           pp;

  logic                                   s0_execute;
  logic                                   s0_en;
  logic                                   s0_addr_valid;
  logic [31:0]                            s0_rdata;
  logic [NumRegs-1:0]                     s0_reg_en;
  logic                                   s0_pp_valid;

  logic                                   s1_execute_q;
  logic                                   s1_en;
  logic                                   s1_addr_valid_q;
  logic [31:0]                            s1_rdata_q;
  logic [NumRegs-1:0]                     s1_reg_en_q;
  logic                                   s1_pp_valid_q;

  logic                                   s2_execute_q;

  logic [NrThreads-1:0][63:0]             reg_ddata0_q;
  logic [NrThreads-1:0][63:0]             reg_ddata0_d;

  logic                                   ffb_en_lo;
  logic [1:0]                             ffb_en_hi;

  assign {pp, thread_sel, reg_sel} = apb_paddr_i;

  always_comb begin
    s0_en = apb_psel_i && !apb_penable_i;
    s0_addr_valid = 1'b1;
    s0_rdata = '0;
    s0_execute = 1'b0;
    s0_pp_valid = 1'b0;
    s0_reg_en = '0;

    if (s0_en) begin
      s0_pp_valid = (pp == AdEsrProtBitsMsg);

      unique case (minion_debug_apb_regs_t'(reg_sel[$bits(minion_debug_apb_regs_t)-1:0]))
        minion_debug_nxdata0: begin
          s0_reg_en[int'(minion_debug_nxdata0)] = apb_pwrite_i;
          s0_rdata = reg_ddata0_q[thread_sel][31:0];
        end
        minion_debug_nxdata1: begin
          s0_reg_en[int'(minion_debug_nxdata1)] = apb_pwrite_i;
          s0_rdata = reg_ddata0_q[thread_sel][63:32];
        end
        minion_debug_axdata0: begin
          s0_reg_en[int'(minion_debug_axdata0)] = apb_pwrite_i;
          s0_rdata = reg_ddata0_q[thread_sel][31:0];
          s0_execute = apb_pwrite_i;
        end
        minion_debug_axdata1: begin
          s0_reg_en[int'(minion_debug_axdata1)] = apb_pwrite_i;
          s0_rdata = reg_ddata0_q[thread_sel][63:32];
          s0_execute = apb_pwrite_i;
        end
        minion_debug_abscmd,
        minion_debug_axprogbuf0,
        minion_debug_axprogbuf1: begin
          s0_reg_en[int'(minion_debug_apb_regs_t'(reg_sel[$bits(minion_debug_apb_regs_t)-1:0]))] = apb_pwrite_i;
          s0_execute = apb_pwrite_i;
        end
        minion_debug_nxprogbuf0,
        minion_debug_nxprogbuf1: begin
          s0_reg_en[int'(minion_debug_apb_regs_t'(reg_sel[$bits(minion_debug_apb_regs_t)-1:0]))] = apb_pwrite_i;
        end
        default: begin
          s0_addr_valid = 1'b0;
          s0_reg_en[int'(minion_debug_invalid)] = 1'b1;
        end
      endcase
    end
  end

  always_comb begin
    s1_en = apb_psel_i && apb_penable_i && apb_pready_o;

    reg_ddata0_d = reg_ddata0_q;

    if (!apb_pslverr_o) begin
      if (s1_reg_en_q[int'(minion_debug_nxdata0)] || s1_reg_en_q[int'(minion_debug_axdata0)])
        reg_ddata0_d[thread_sel][31:0] = apb_pwdata_i[31:0];
      if (s1_reg_en_q[int'(minion_debug_nxdata1)] || s1_reg_en_q[int'(minion_debug_axdata1)])
        reg_ddata0_d[thread_sel][63:32] = apb_pwdata_i[31:0];
    end

    for (int i = 0; i < NrThreads; i++) begin
      if (update_ddata0_i[i])
        reg_ddata0_d[i] = csr_wdata_i;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s1_rdata_q <= '0;
      s1_reg_en_q <= '0;
      s1_addr_valid_q <= 1'b0;
      s1_pp_valid_q <= 1'b0;
    end else begin
      if (s0_en && !apb_pwrite_i)
        s1_rdata_q <= s0_rdata;

      if (s0_en || s1_en) begin
        s1_reg_en_q <= s0_reg_en;
      end

      if (s0_en) begin
        s1_addr_valid_q <= s0_addr_valid;
        s1_pp_valid_q <= s0_pp_valid;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    reg_ddata0_q <= reg_ddata0_d;
    s1_execute_q <= s0_execute;
    s2_execute_q <= s1_execute_q;
  end

  assign apb_prdata_o = {{(MinionDebugApbDataWidth - 32){1'b0}}, s1_rdata_q};
  assign apb_pslverr_o = !s1_addr_valid_q || !s1_pp_valid_q || !in_debug_mode_i[thread_sel];
  assign apb_pready_o = apb_psel_i && apb_penable_i &&
                        (s1_reg_en_q[int'(minion_debug_invalid)] ||
                         (s1_addr_valid_q && !apb_pwrite_i) ||
                         (s1_addr_valid_q && apb_pwrite_i && !s1_execute_q) ||
                         s2_execute_q);

  always_comb begin
    ffb_en_lo = s1_reg_en_q[int'(minion_debug_abscmd)];
    ffb_en_hi = {
      s1_reg_en_q[int'(minion_debug_axprogbuf1)] || s1_reg_en_q[int'(minion_debug_nxprogbuf1)],
      s1_reg_en_q[int'(minion_debug_axprogbuf0)] || s1_reg_en_q[int'(minion_debug_nxprogbuf0)]
    };

    ffb_en_o = {ffb_en_hi, ffb_en_lo, ffb_en_lo} & {MinionDebugFfbWords{!apb_pslverr_o}};
    ffb_thread_sel_o = thread_sel;
    ffb_wdata_o = apb_pwdata_i;
    read_ddata0_o = reg_ddata0_q;

    ex_program_buffer_o = '0;
    ex_program_buffer_o[thread_sel] = s1_execute_q && !apb_pslverr_o;
  end

endmodule
