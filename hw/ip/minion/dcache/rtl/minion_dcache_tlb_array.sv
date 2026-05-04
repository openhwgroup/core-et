// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache TLB wrapper.
//
// Replaces: dcache_tlb_array

module minion_dcache_tlb_array
  import dft_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic             clk_i,
  input  logic             rst_ni,
  input  dft_t             dft_i,
  input  tlb_entry_type    esr_vmspagesize_i,
  input  dcache_tlb_req_t  req_data_i,
  input  logic             req_valid_i,
  output dcache_tlb_resp_t resp_data_o,
  input  minion_satp_info  satp_info_i,
  input  minion_satp_info  matp_info_i,
  input  logic             satp_info_en_i,
  input  logic             matp_info_en_i,
  input  logic             tlb_invalidate_i,
  output minion_ptw_req    ptw_req_data_o,
  output logic             ptw_req_valid_o,
  input  logic             ptw_req_ready_i,
  input  minion_ptw_pte    ptw_resp_data_i,
  input  logic             ptw_resp_valid_i,
  output logic             vm_enabled_o
);

  localparam int unsigned NrMinions = 1;

  logic            satp_info_en_del_q;
  logic            matp_info_en_del_q;
  logic            en_del;
  minion_satp_info satp_info_reg_q;
  minion_satp_info matp_info_reg_q;
  tlb_req          tlb_req_data;
  /* verilator lint_off UNUSEDSIGNAL */  // dcache_tlb_array does not expose tlb_resp.xcpt_if, so this wrapper intentionally drops that bit.
  tlb_resp         tlb_resp_data;
  /* verilator lint_on UNUSEDSIGNAL */
  minion_satp_info satp_info [NrMinions-1:0];
  minion_satp_info matp_info [NrMinions-1:0];

  assign en_del = satp_info_en_del_q || satp_info_en_i || matp_info_en_del_q || matp_info_en_i;

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      satp_info_en_del_q <= 1'b0;
      matp_info_en_del_q <= 1'b0;
    end else if (en_del) begin
      satp_info_en_del_q <= satp_info_en_i;
      matp_info_en_del_q <= matp_info_en_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      satp_info_reg_q <= '0;
    end else if (satp_info_en_del_q) begin
      satp_info_reg_q <= satp_info_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      matp_info_reg_q <= '0;
    end else if (matp_info_en_del_q) begin
      matp_info_reg_q <= matp_info_i;
    end
  end

  always_comb begin
    tlb_req_data = '0;
    tlb_req_data.status = req_data_i.status;
    tlb_req_data.vpn = req_data_i.vpn;
    tlb_req_data.passthrough = req_data_i.passthrough;
    tlb_req_data.instruction = 1'b0;
    tlb_req_data.store = req_data_i.store;
    tlb_req_data.msb_err = req_data_i.msb_err;

    satp_info[0] = satp_info_reg_q;
    matp_info[0] = matp_info_reg_q;

    resp_data_o.fill_pending = tlb_resp_data.fill_pending;
    resp_data_o.miss = tlb_resp_data.miss;
    resp_data_o.ppn = tlb_resp_data.ppn;
    resp_data_o.xcpt_ld = tlb_resp_data.xcpt_ld;
    resp_data_o.xcpt_st = tlb_resp_data.xcpt_st;
    resp_data_o.access_fault = tlb_resp_data.access_fault;
  end

  /* verilator lint_off PINCONNECTEMPTY */  // Original dcache_tlb_array does not use the PTW invalidate output.
  minion_tlb #(
    .Entries   (DcacheTlbEntries),
    .NrMinions (NrMinions)
  ) u_tlb (
    .clk_i            (clk_i),
    .rst_ni           (rst_ni),
    .dft_i            (dft_i),
    .vmspagesize_i    (esr_vmspagesize_i),
    .coop_mode_i      (1'b0),
    .req_min_id_i     ('0),
    .req_data_i       (tlb_req_data),
    .req_valid_i      (req_valid_i),
    .resp_data_o      (tlb_resp_data),
    .satp_info_i      (satp_info),
    .matp_info_i      (matp_info),
    .tlb_invalidate_i (tlb_invalidate_i),
    .ptw_req_data_o   (ptw_req_data_o),
    .ptw_req_valid_o  (ptw_req_valid_o),
    .ptw_req_ready_i  (ptw_req_ready_i),
    .ptw_invalidate_o (),
    .ptw_resp_data_i  (ptw_resp_data_i),
    .ptw_resp_valid_i (ptw_resp_valid_i),
    .vm_enabled_o     (vm_enabled_o)
  );
  /* verilator lint_on PINCONNECTEMPTY */

endmodule : minion_dcache_tlb_array
