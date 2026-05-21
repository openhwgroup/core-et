// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_shared_icache_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import minion_frontend_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,

  input  logic                         bypass_icache_i,
  input  logic [7:0]                   req_valid_i,
  input  logic [7:0]                   req_thread_id_i,
  input  logic [VaSizeExt-1:0]         req_addr0_i,
  input  logic [VaSizeExt-1:0]         req_addr1_i,
  input  logic [VaSizeExt-1:0]         req_addr2_i,
  input  logic [VaSizeExt-1:0]         req_addr3_i,
  input  logic [VaSizeExt-1:0]         req_addr4_i,
  input  logic [VaSizeExt-1:0]         req_addr5_i,
  input  logic [VaSizeExt-1:0]         req_addr6_i,
  input  logic [VaSizeExt-1:0]         req_addr7_i,
  input  logic [7:0]                   flush_i,
  input  logic                         l2_req_ready_i,
  input  logic                         l2_resp_valid_i,
  input  logic [1:0]                   l2_resp_opcode_i,
  input  logic [1:0]                   ptw_req_ready_i,
  input  logic [1:0]                   ptw_resp_valid_i,
  input  logic                         sram_req_ready_i,
  input  logic                         sram_resp_valid_i,
  input  logic [63:0]                  sram_resp_word0_i,
  input  logic [IcacheNeighApbAddrWidth-1:0] apb_paddr_i,
  input  logic                         apb_pwrite_i,
  input  logic                         apb_psel_i,
  input  logic                         apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0] apb_pwdata_i,

  output logic [7:0]                   req_ready_o,
  output logic [1:0]                   resp_valid_o,
  output logic [1:0]                   resp_miss_o,
  output logic [1:0]                   fill_done_o,
  output logic                         prefetch_done_o,
  output logic                         l2_req_valid_o,
  output logic [PaSize-1:0]            l2_req_addr_o,
  output logic                         l2_resp_ready_o,
  output logic [1:0]                   ptw_req_valid_o,
  output logic [1:0]                   ptw_invalidate_o,
  output logic                         sram_req_valid_o,
  output logic                         sram_resp_ready_o,
  output logic                         apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] apb_prdata_o,
  output logic                         apb_pslverr_o,
  output logic [1:0]                   dbg_req_valid_o,
  output logic [VaSize-1:6]            dbg_req_addr0_o,
  output logic [VaSize-1:6]            dbg_req_addr1_o
);

  localparam int unsigned NrMinions = 8;
  localparam int unsigned NrRequestors = 2;

  frontend_icache_req_t req [NrMinions-1:0];
  logic [NrMinions-1:0][VaSizeExt-1:0] req_addr;
  minion_satp_info satp_info [NrMinions-1:0];
  minion_satp_info matp_info [NrMinions-1:0];
  icache_prefetch_conf_t prefetch_conf;
  esr_icache_err_log_ctl_t err_log_ctl;
  esr_mprot_t mprot;
  minion_ptw_pte ptw_resp_data [NrRequestors-1:0];
  /* verilator lint_off UNUSEDSIGNAL */  // Unit TB intentionally observes only a subset of pass-through ICache outputs.
  minion_ptw_req ptw_req_data [NrRequestors-1:0];
  icache_frontend_resp_t resp [NrRequestors-1:0];
  etlink_pkg::req_nodata_t l2_req;
  icache_err_log_info_t err_log_info;
  icache_dbg_sm_t dbg_sm_signals;
  logic err_log_sbe;
  logic err_log_dbe;
  logic sram_req_write;
  logic [IcacheSramAddrWidth-1:0] sram_req_addr;
  /* verilator lint_on UNUSEDSIGNAL */
  etlink_pkg::rsp_t l2_resp;
  logic [IcacheSramDataWidth-1:0] sram_dout;

  assign req_addr[0] = req_addr0_i;
  assign req_addr[1] = req_addr1_i;
  assign req_addr[2] = req_addr2_i;
  assign req_addr[3] = req_addr3_i;
  assign req_addr[4] = req_addr4_i;
  assign req_addr[5] = req_addr5_i;
  assign req_addr[6] = req_addr6_i;
  assign req_addr[7] = req_addr7_i;

  always_comb begin
    prefetch_conf = '0;
    err_log_ctl = '0;
    mprot = '0;
    l2_resp = '0;
    l2_resp.opcode = etlink_pkg::rsp_opcode_e'(l2_resp_opcode_i);
    sram_dout = '0;
    sram_dout[63:0] = sram_resp_word0_i;

    for (int unsigned minion = 0; minion < NrMinions; minion++) begin
      req[minion] = '0;
      req[minion].thread_id = req_thread_id_i[minion];
      req[minion].vm_status.prv = minion_pkg::PrvM;
      req[minion].addr = req_addr[minion];
      satp_info[minion] = '0;
      matp_info[minion] = '0;
    end

    for (int unsigned req_idx = 0; req_idx < NrRequestors; req_idx++) begin
      ptw_resp_data[req_idx] = '0;
    end
  end

  neigh_shared_icache #(
    .NrMinions    (NrMinions),
    .NrRequestors (NrRequestors)
  ) u_dut (
    .clk_i,
    .rst_ni,
    .esr_prefetch_conf_i             (prefetch_conf),
    .esr_prefetch_start_i            (1'b0),
    .esr_prefetch_done_o             (prefetch_done_o),
    .esr_err_log_ctl_i               (err_log_ctl),
    .esr_err_log_sbe_o               (err_log_sbe),
    .esr_err_log_dbe_o               (err_log_dbe),
    .esr_err_log_info_o              (err_log_info),
    .esr_mprot_i                     (mprot),
    .esr_vmspagesize_i               (minion_pkg::tlb_entry_type_4K),
    .esr_bypass_icache_i             (bypass_icache_i),
    .esr_shire_coop_mode_i           (1'b0),
    .f0_req_ready_o                  (req_ready_o),
    .f0_req_valid_i                  (req_valid_i),
    .f0_req_i                        (req),
    .f4_resp_valid_o                 (resp_valid_o),
    .f4_resp_miss_o                  (resp_miss_o),
    .f4_resp_o                       (resp),
    .f5_resp_fill_done_o             (fill_done_o),
    .f0_flush_data_i                 (flush_i),
    .f0_l2_miss_req_disable_next_i   (1'b0),
    .f0_l2_miss_req_ready_i          (l2_req_ready_i),
    .f0_l2_miss_req_valid_o          (l2_req_valid_o),
    .f0_l2_miss_req_o                (l2_req),
    .f0_l2_miss_resp_ready_o         (l2_resp_ready_o),
    .f0_l2_miss_resp_valid_i         (l2_resp_valid_i),
    .f0_l2_miss_resp_i               (l2_resp),
    .satp_info_i                     (satp_info),
    .matp_info_i                     (matp_info),
    .tlb_invalidate_i                ('0),
    .ptw_req_data_o                  (ptw_req_data),
    .ptw_req_valid_o                 (ptw_req_valid_o),
    .ptw_req_ready_i                 (ptw_req_ready_i),
    .ptw_invalidate_o                (ptw_invalidate_o),
    .ptw_resp_valid_i                (ptw_resp_valid_i),
    .ptw_resp_data_i                 (ptw_resp_data),
    .icache_f2_sram_req_write_o      (sram_req_write),
    .icache_f2_sram_req_addr_o       (sram_req_addr),
    .icache_f2_sram_req_valid_o      (sram_req_valid_o),
    .icache_f2_sram_req_ready_i      (sram_req_ready_i),
    .icache_f0_sram_resp_dout_i      (sram_dout),
    .icache_f0_sram_resp_valid_i     (sram_resp_valid_i),
    .icache_f0_sram_resp_ready_o     (sram_resp_ready_o),
    .apb_paddr_i                     (apb_paddr_i),
    .apb_pwrite_i                    (apb_pwrite_i),
    .apb_psel_i                      (apb_psel_i),
    .apb_penable_i                   (apb_penable_i),
    .apb_pwdata_i                    (apb_pwdata_i),
    .apb_pready_o                    (apb_pready_o),
    .apb_prdata_o                    (apb_prdata_o),
    .apb_pslverr_o                   (apb_pslverr_o),
    .dbg_sm_signals_o                (dbg_sm_signals)
  );

  assign l2_req_addr_o = l2_req.address;
  assign dbg_req_valid_o = dbg_sm_signals.f1_req_valid;
  assign dbg_req_addr0_o = dbg_sm_signals.f1_req_addr[0];
  assign dbg_req_addr1_o = dbg_sm_signals.f1_req_addr[1];

endmodule : neigh_shared_icache_tb
