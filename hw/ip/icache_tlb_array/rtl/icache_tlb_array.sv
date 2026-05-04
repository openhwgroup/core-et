// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Standalone-Minion I-cache TLB wrapper.
//
// Replaces: `icache_tlb_array` from etcore-soc.

/* verilator lint_off SYNCASYNCNET */  // This wrapper intentionally keeps its original async-reset staging while forwarding the same reset into shared synchronous-reset minion_tlb state.

module icache_tlb_array
  import minion_pkg::*;
  import icache_pkg::*;
#(
  parameter int unsigned Entries = 8,
  parameter int unsigned NrMinions = 1,
  localparam int unsigned NrMinionsW = (NrMinions == 1) ? 1 : $clog2(NrMinions)
) (
  input  logic                             clk_i,
  input  logic                             rst_ni,
  input  tlb_entry_type                    esr_vmspagesize_i,
  input  logic                             esr_shire_coop_mode_i,
  input  logic [NrMinionsW-1:0]            req_min_id_i,
  input  icache_tlb_req_t                  req_data_i,
  input  logic                             req_valid_i,
  output icache_tlb_resp_t                 resp_data_o,
  input  minion_satp_info                  satp_info_i [NrMinions-1:0],
  input  minion_satp_info                  matp_info_i [NrMinions-1:0],
  input  logic [NrMinions-1:0]             tlb_invalidate_i,
  output minion_ptw_req                    ptw_req_data_o,
  output logic                             ptw_req_valid_o,
  input  logic                             ptw_req_ready_i,
  output logic                             ptw_invalidate_o,
  input  logic                             ptw_resp_valid_i,
  input  minion_ptw_pte                    ptw_resp_data_i
);

  tlb_entry_type   esr_vmspagesize_q;
  minion_satp_info satp_info_q [NrMinions-1:0];
  minion_satp_info matp_info_q [NrMinions-1:0];
  tlb_req          tlb_req_data;
  /* verilator lint_off UNUSEDSIGNAL */  // The icache-facing wrapper intentionally drops ld/st fault fields from the shared TLB response.
  tlb_resp         tlb_resp_data;
  /* verilator lint_on UNUSEDSIGNAL */

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      esr_vmspagesize_q <= tlb_entry_type'(0);
      for (int i = 0; i < NrMinions; i++) begin
        satp_info_q[i] <= '0;
        matp_info_q[i] <= '0;
      end
    end else begin
      esr_vmspagesize_q <= esr_vmspagesize_i;
      for (int i = 0; i < NrMinions; i++) begin
        satp_info_q[i] <= satp_info_i[i];
        matp_info_q[i] <= matp_info_i[i];
      end
    end
  end

  always_comb begin
    tlb_req_data = '0;
    tlb_req_data.status = req_data_i.status;
    tlb_req_data.vpn = req_data_i.vpn;
    tlb_req_data.passthrough = req_data_i.passthrough;
    tlb_req_data.instruction = 1'b1;
    tlb_req_data.store = 1'b0;
    tlb_req_data.msb_err = req_data_i.msb_err;

    resp_data_o = '0;
    resp_data_o.fill_pending = tlb_resp_data.fill_pending;
    resp_data_o.miss = tlb_resp_data.miss;
    resp_data_o.ppn = tlb_resp_data.ppn;
    resp_data_o.xcpt_if = tlb_resp_data.xcpt_if;
    resp_data_o.access_fault = tlb_resp_data.access_fault;
  end

  /* verilator lint_off PINCONNECTEMPTY */  // The standalone wrapper preserves the original unused VM-enabled port.
  minion_tlb #(
    .Entries   (Entries),
    .NrMinions (NrMinions)
  ) u_tlb (
    .clk_i            (clk_i),
    .rst_ni           (rst_ni),
    .dft_i            ('0),
    .vmspagesize_i    (esr_vmspagesize_q),
    .coop_mode_i      (esr_shire_coop_mode_i),
    .req_min_id_i     (req_min_id_i),
    .req_data_i       (tlb_req_data),
    .req_valid_i      (req_valid_i),
    .resp_data_o      (tlb_resp_data),
    .satp_info_i      (satp_info_q),
    .matp_info_i      (matp_info_q),
    .tlb_invalidate_i (tlb_invalidate_i),
    .ptw_req_data_o   (ptw_req_data_o),
    .ptw_req_valid_o  (ptw_req_valid_o),
    .ptw_req_ready_i  (ptw_req_ready_i),
    .ptw_invalidate_o (ptw_invalidate_o),
    .ptw_resp_data_i  (ptw_resp_data_i),
    .ptw_resp_valid_i (ptw_resp_valid_i),
    .vm_enabled_o     ()
  );
  /* verilator lint_on PINCONNECTEMPTY */

endmodule : icache_tlb_array

/* verilator lint_on SYNCASYNCNET */
