// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_shared_icache
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import minion_frontend_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
#(
  parameter int unsigned NrMinions = 8,
  parameter int unsigned NrRequestors = 2,
  localparam int unsigned NrMinPerReq = NrMinions / NrRequestors,
  localparam int unsigned NrMinPerReqW = (NrMinPerReq <= 1) ? 1 : $clog2(NrMinPerReq),
  localparam int unsigned ThreadsPerMinion = minion_pkg::NrThreads,
  localparam int unsigned ThreadsPerMinionW = (ThreadsPerMinion <= 1) ? 1 : $clog2(ThreadsPerMinion),
  localparam int unsigned NrThreadsPerReq = NrMinions * ThreadsPerMinion / NrRequestors,
  localparam int unsigned NrThreadsPerReqW = (NrThreadsPerReq <= 1) ? 1 : $clog2(NrThreadsPerReq)
) (
  input  logic                                             clk_i,
  input  logic                                             rst_ni,

  input  icache_prefetch_conf_t                            esr_prefetch_conf_i,
  input  logic                                             esr_prefetch_start_i,
  output logic                                             esr_prefetch_done_o,
  input  esr_icache_err_log_ctl_t                          esr_err_log_ctl_i,
  output logic                                             esr_err_log_sbe_o,
  output logic                                             esr_err_log_dbe_o,
  output icache_err_log_info_t                             esr_err_log_info_o,
  input  esr_mprot_t                                       esr_mprot_i,
  input  tlb_entry_type                                    esr_vmspagesize_i,
  input  logic                                             esr_bypass_icache_i,
  input  logic                                             esr_shire_coop_mode_i,

  output logic [NrMinions-1:0]                             f0_req_ready_o,
  input  logic [NrMinions-1:0]                             f0_req_valid_i,
  input  frontend_icache_req_t                             f0_req_i [NrMinions-1:0],

  output logic [NrRequestors-1:0]                          f4_resp_valid_o,
  output logic [NrRequestors-1:0]                          f4_resp_miss_o,
  output icache_frontend_resp_t                            f4_resp_o [NrRequestors-1:0],
  output logic [NrRequestors-1:0]                          f5_resp_fill_done_o,

  input  logic [NrMinions-1:0]                             f0_flush_data_i,

  input  logic                                             f0_l2_miss_req_disable_next_i,
  input  logic                                             f0_l2_miss_req_ready_i,
  output logic                                             f0_l2_miss_req_valid_o,
  output etlink_pkg::req_nodata_t                          f0_l2_miss_req_o,

  output logic                                             f0_l2_miss_resp_ready_o,
  input  logic                                             f0_l2_miss_resp_valid_i,
  input  etlink_pkg::rsp_t                                 f0_l2_miss_resp_i,

  input  minion_satp_info                                  satp_info_i [NrMinions-1:0],
  input  minion_satp_info                                  matp_info_i [NrMinions-1:0],
  input  logic [NrMinions-1:0]                             tlb_invalidate_i,

  output minion_ptw_req                                    ptw_req_data_o [NrRequestors-1:0],
  output logic [NrRequestors-1:0]                          ptw_req_valid_o,
  input  logic [NrRequestors-1:0]                          ptw_req_ready_i,
  output logic [NrRequestors-1:0]                          ptw_invalidate_o,

  input  logic [NrRequestors-1:0]                          ptw_resp_valid_i,
  input  minion_ptw_pte                                    ptw_resp_data_i [NrRequestors-1:0],

  output logic                                             icache_f2_sram_req_write_o,
  output logic [IcacheSramAddrWidth-1:0]                   icache_f2_sram_req_addr_o,
  output logic                                             icache_f2_sram_req_valid_o,
  input  logic                                             icache_f2_sram_req_ready_i,

  input  logic [IcacheSramDataWidth-1:0]                   icache_f0_sram_resp_dout_i,
  input  logic                                             icache_f0_sram_resp_valid_i,
  output logic                                             icache_f0_sram_resp_ready_o,

  input  logic [IcacheNeighApbAddrWidth-1:0]               apb_paddr_i,
  input  logic                                             apb_pwrite_i,
  input  logic                                             apb_psel_i,
  input  logic                                             apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0]                 apb_pwdata_i,
  output logic                                             apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0]                 apb_prdata_o,
  output logic                                             apb_pslverr_o,

  output icache_dbg_sm_t                                   dbg_sm_signals_o
);

  localparam int unsigned ReqWidth = $bits(frontend_icache_req_t);

  logic [NrMinions-1:0][ThreadsPerMinion-1:0]              f0_thread_req_valid;
  logic [NrMinions-1:0][ThreadsPerMinion-1:0]              f1_req_ready;
  logic [NrMinions-1:0][ThreadsPerMinion-1:0]              f1_req_valid_q;
  logic [NrMinions-1:0][ThreadsPerMinion-1:0]              f1_req_valid_d;
  frontend_icache_req_t                                    f1_req_q [NrMinions-1:0][ThreadsPerMinion-1:0];

  logic [NrRequestors-1:0]                                 f1_req_arb_ready;
  logic [NrRequestors-1:0]                                 f1_req_arb_valid;
  frontend_icache_req_t                                    f1_req_winner [NrRequestors-1:0];
  logic [NrRequestors-1:0][NrThreadsPerReqW-1:0]           f1_req_hart_id;
  logic [NrRequestors-1:0][NrMinPerReqW-1:0]               f1_req_min_id;

  logic [NrMinions-1:0]                                    f1_flush_data_q;
  icache_dbg_sm_t                                          icache_dbg_sm_signals;

  always_comb begin
    for (int unsigned minion = 0; minion < NrMinions; minion++) begin
      for (int unsigned thread = 0; thread < ThreadsPerMinion; thread++) begin
        f0_thread_req_valid[minion][thread] = f0_req_valid_i[minion] &&
            (f0_req_i[minion].thread_id == thread[ThreadsPerMinionW-1:0]);

        f1_req_valid_d[minion][thread] = f1_req_valid_q[minion][thread];
        if (f0_thread_req_valid[minion][thread]) begin
          f1_req_valid_d[minion][thread] = 1'b1;
        end
        if (f1_req_ready[minion][thread]) begin
          f1_req_valid_d[minion][thread] = 1'b0;
        end
      end
      f0_req_ready_o[minion] = |f1_req_ready[minion];
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f1_req_valid_q <= '0;
      f1_flush_data_q <= '0;
    end else begin
      f1_req_valid_q <= f1_req_valid_d;
      f1_flush_data_q <= f0_flush_data_i;
    end
  end

  always_ff @(posedge clk_i) begin
    for (int unsigned minion = 0; minion < NrMinions; minion++) begin
      for (int unsigned thread = 0; thread < ThreadsPerMinion; thread++) begin
        if (f0_thread_req_valid[minion][thread] && !f1_req_valid_q[minion][thread]) begin
          f1_req_q[minion][thread] <= f0_req_i[minion];
        end
      end
    end
  end

  for (genvar g_req = 0; g_req < NrRequestors; g_req++) begin : gen_req_arb
    logic [NrThreadsPerReq-1:0]                            f1_bid;
    logic [NrThreadsPerReq-1:0]                            f1_grant;
    logic [NrThreadsPerReq-1:0]                            f1_grant_q;
    logic [NrThreadsPerReq-1:0]                            f2_grant_q;
    logic [NrThreadsPerReq-1:0]                            f3_grant_q;
    logic [NrThreadsPerReq-1:0]                            f4_grant_q;
    logic                                                   f4_update_prio;

    logic [NrThreadsPerReq-1:0][NrThreadsPerReq-1:0]       prio_q;
    logic [NrThreadsPerReq-1:0][NrThreadsPerReq-1:0]       prio_d;
    logic [NrThreadsPerReq-1:0]                            win_per_client;
    logic [NrThreadsPerReq-1:0][ReqWidth-1:0]              f1_req_data_bits;
    logic [ReqWidth-1:0]                                   f1_req_winner_bits;

    logic                                                  f4_resp_err;
    logic [NrThreadsPerReq-1:0][4:0]                       th_miss_cnt_q;
    logic [NrThreadsPerReq-1:0][4:0]                       th_miss_cnt_d;
    logic [NrThreadsPerReq-1:0]                            th_miss_cnt_sat;
    logic [NrThreadsPerReq-1:0]                            th_miss_cnt_en;
    logic                                                  th_miss_mask_en_q;
    logic [NrThreadsPerReq-1:0]                            th_miss_mask_q;
    logic [NrThreadsPerReq-1:0]                            th_miss_mask_d;
    logic                                                  th_miss_any_masked;

    for (genvar g_min = 0; g_min < NrMinPerReq; g_min++) begin : gen_minion_map
      for (genvar g_thread = 0; g_thread < ThreadsPerMinion; g_thread++) begin : gen_thread_map
        localparam int unsigned GlobalMinion = g_req * NrMinPerReq + g_min;
        localparam int unsigned ThreadIdx = g_min * ThreadsPerMinion + g_thread;

        assign f1_bid[ThreadIdx] = f1_req_valid_q[GlobalMinion][g_thread] &
                                   th_miss_mask_q[ThreadIdx];
        assign f1_req_ready[GlobalMinion][g_thread] = f1_grant[ThreadIdx];
        assign f1_req_data_bits[ThreadIdx] = f1_req_q[GlobalMinion][g_thread];
      end
    end

    assign f1_req_arb_valid[g_req] = |f1_bid;
    assign f1_req_winner_bits = f1_req_data_bits[f1_req_hart_id[g_req]];
    assign f1_req_winner[g_req] = f1_req_winner_bits;
    assign f1_req_min_id[g_req] = f1_req_hart_id[g_req][ThreadsPerMinionW +: NrMinPerReqW];

    always_comb begin
      for (int unsigned client = 0; client < NrThreadsPerReq; client++) begin
        win_per_client[client] = f1_bid[client];
        for (int unsigned peer = 0; peer < NrThreadsPerReq; peer++) begin
          if (peer < client) begin
            win_per_client[client] = win_per_client[client] &&
                (!f1_bid[peer] || prio_q[client][peer]);
          end else if (peer > client) begin
            win_per_client[client] = win_per_client[client] &&
                (!f1_bid[peer] || !prio_q[peer][client]);
          end
        end
        f1_grant[client] = win_per_client[client] && f1_req_arb_ready[g_req];
      end

      f1_req_hart_id[g_req] = '0;
      for (int unsigned client = 0; client < NrThreadsPerReq; client++) begin
        if (win_per_client[client]) begin
          f1_req_hart_id[g_req] = client[NrThreadsPerReqW-1:0];
        end
      end
    end

    always_comb begin
      prio_d = prio_q;
      for (int unsigned client = 0; client < NrThreadsPerReq; client++) begin
        if (f4_grant_q[client]) begin
          for (int unsigned peer = 0; peer < NrThreadsPerReq; peer++) begin
            if (peer < client) begin
              prio_d[client][peer] = 1'b0;
            end else if (peer > client) begin
              prio_d[peer][client] = 1'b1;
            end
          end
        end
      end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        prio_q <= '0;
      end else if (f4_update_prio) begin
        prio_q <= prio_d;
      end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        f1_grant_q <= '0;
        f2_grant_q <= '0;
        f3_grant_q <= '0;
        f4_grant_q <= '0;
      end else begin
        if (f1_req_arb_ready[g_req]) begin
          f1_grant_q <= f1_grant;
        end
        f2_grant_q <= f1_grant_q;
        f3_grant_q <= f2_grant_q;
        f4_grant_q <= f3_grant_q;
      end
    end

    assign f4_update_prio = f4_resp_valid_o[g_req] &&
                            (!f4_resp_miss_o[g_req] ||
                             f4_resp_o[g_req].page_fault ||
                             f4_resp_o[g_req].access_fault);

    assign f4_resp_err = f4_resp_o[g_req].page_fault |
                         f4_resp_o[g_req].access_fault |
                         f4_resp_o[g_req].bus_err |
                         f4_resp_o[g_req].ecc_err;

    always_comb begin
      for (int unsigned thread = 0; thread < NrThreadsPerReq; thread++) begin
        th_miss_cnt_sat[thread] = &th_miss_cnt_q[thread];
        th_miss_cnt_en[thread] = f4_resp_valid_o[g_req] &&
                                 (f4_grant_q[thread] || f4_resp_err);
        th_miss_cnt_d[thread] = th_miss_cnt_q[thread];
        if (!f4_resp_miss_o[g_req] || f4_resp_err) begin
          th_miss_cnt_d[thread] = '0;
        end else if (!th_miss_cnt_sat[thread]) begin
          th_miss_cnt_d[thread] = th_miss_cnt_q[thread] + 5'd1;
        end
      end
    end

    always_comb begin
      th_miss_any_masked = 1'b0;
      th_miss_mask_d = '1;
      if (|th_miss_cnt_sat) begin
        for (int unsigned thread = 0; thread < NrThreadsPerReq; thread += ThreadsPerMinion) begin
          th_miss_mask_d[thread] = !th_miss_any_masked &&
                                   (th_miss_cnt_sat[thread] |
                                    th_miss_cnt_sat[thread + 1]);
          th_miss_mask_d[thread + 1] = !th_miss_any_masked &&
                                       (th_miss_cnt_sat[thread] |
                                        th_miss_cnt_sat[thread + 1]);
          th_miss_any_masked = th_miss_any_masked |
                               th_miss_cnt_sat[thread] |
                               th_miss_cnt_sat[thread + 1];
        end
      end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        th_miss_cnt_q <= '0;
        th_miss_mask_en_q <= 1'b0;
        th_miss_mask_q <= '1;
      end else begin
        for (int unsigned thread = 0; thread < NrThreadsPerReq; thread++) begin
          if (th_miss_cnt_en[thread]) begin
            th_miss_cnt_q[thread] <= th_miss_cnt_d[thread];
          end
        end
        th_miss_mask_en_q <= |th_miss_cnt_en;
        if (th_miss_mask_en_q) begin
          th_miss_mask_q <= th_miss_mask_d;
        end
      end
    end
  end

  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0] icache_f0_sram_resp_dout;

  assign icache_f0_sram_resp_dout = icache_f0_sram_resp_dout_i;

  icache_top #(
    .NrMinions    (NrMinions),
    .NrRequestors (NrRequestors)
  ) u_icache_top (
    .clk_i,
    .rst_ni,
    .ioshire_i                       (1'b0),
    .esr_prefetch_conf_i             (esr_prefetch_conf_i),
    .esr_prefetch_start_i            (esr_prefetch_start_i),
    .esr_prefetch_done_o             (esr_prefetch_done_o),
    .esr_err_log_ctl_i               (esr_err_log_ctl_i),
    .esr_err_log_sbe_o               (esr_err_log_sbe_o),
    .esr_err_log_dbe_o               (esr_err_log_dbe_o),
    .esr_err_log_info_o              (esr_err_log_info_o),
    .esr_mprot_i                     (esr_mprot_i),
    .esr_vmspagesize_i               (esr_vmspagesize_i),
    .esr_bypass_icache_i             (esr_bypass_icache_i),
    .esr_shire_coop_mode_i           (esr_shire_coop_mode_i),
    .f0_req_ready_o                  (f1_req_arb_ready),
    .f0_req_valid_i                  (f1_req_arb_valid),
    .f0_req_i                        (f1_req_winner),
    .f0_req_min_id_i                 (f1_req_min_id),
    .f4_resp_valid_o                 (f4_resp_valid_o),
    .f4_resp_miss_o                  (f4_resp_miss_o),
    .f4_resp_o                       (f4_resp_o),
    .f5_resp_fill_done_o             (f5_resp_fill_done_o),
    .f0_flush_data_i                 (|f1_flush_data_q),
    .f0_l2_miss_req_disable_next_i   (f0_l2_miss_req_disable_next_i),
    .f0_l2_miss_req_ready_i          (f0_l2_miss_req_ready_i),
    .f0_l2_miss_req_valid_o          (f0_l2_miss_req_valid_o),
    .f0_l2_miss_req_o                (f0_l2_miss_req_o),
    .f0_l2_miss_resp_ready_o         (f0_l2_miss_resp_ready_o),
    .f0_l2_miss_resp_i               (f0_l2_miss_resp_i),
    .f0_l2_miss_resp_valid_i         (f0_l2_miss_resp_valid_i),
    .satp_info_i                     (satp_info_i),
    .matp_info_i                     (matp_info_i),
    .tlb_invalidate_i                (tlb_invalidate_i),
    .ptw_req_data_o                  (ptw_req_data_o),
    .ptw_req_valid_o                 (ptw_req_valid_o),
    .ptw_req_ready_i                 (ptw_req_ready_i),
    .ptw_invalidate_o                (ptw_invalidate_o),
    .ptw_resp_valid_i                (ptw_resp_valid_i),
    .ptw_resp_data_i                 (ptw_resp_data_i),
    .f2_sram_req_write_o             (icache_f2_sram_req_write_o),
    .f2_sram_req_addr_o              (icache_f2_sram_req_addr_o),
    .f2_sram_req_valid_o             (icache_f2_sram_req_valid_o),
    .f2_sram_req_ready_i             (icache_f2_sram_req_ready_i),
    .f0_sram_resp_dout_i             (icache_f0_sram_resp_dout),
    .f0_sram_resp_valid_i            (icache_f0_sram_resp_valid_i),
    .f0_sram_resp_ready_o            (icache_f0_sram_resp_ready_o),
    .apb_paddr_i                     (apb_paddr_i),
    .apb_pwrite_i                    (apb_pwrite_i),
    .apb_psel_i                      (apb_psel_i),
    .apb_penable_i                   (apb_penable_i),
    .apb_pwdata_i                    (apb_pwdata_i),
    .apb_pready_o                    (apb_pready_o),
    .apb_prdata_o                    (apb_prdata_o),
    .apb_pslverr_o                   (apb_pslverr_o),
    .dbg_sm_signals_o                (icache_dbg_sm_signals)
  );

  always_comb begin
    dbg_sm_signals_o = icache_dbg_sm_signals;
    for (int unsigned req = 0; req < NrRequestors; req++) begin
      dbg_sm_signals_o.f1_req_valid[req] = f1_req_arb_valid[req] & f1_req_arb_ready[req];
      dbg_sm_signals_o.f1_req_addr[req] = f1_req_winner[req].addr[VaSize-1:6];
    end
  end

endmodule : neigh_shared_icache
