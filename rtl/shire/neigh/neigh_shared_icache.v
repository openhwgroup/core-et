// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_shared_icache #(
  parameter NR_MINIONS    = 8,
  parameter NR_REQUESTORS = 2
) (
  // System signals
  input  logic                                    clock,
  input  logic                                    reset,
  // ESRs
  input  icache_prefetch_conf_t                   esr_prefetch_conf,
  input  logic                                    esr_prefetch_start,
  output logic                                    esr_prefetch_done,
  input  esr_icache_err_log_ctl_t                 esr_err_log_ctl,
  output logic                                    esr_err_log_sbe,
  output logic                                    esr_err_log_dbe,
  output icache_err_log_info_t                    esr_err_log_info,
  input  esr_mprot_t                              esr_mprot,
  input  tlb_entry_type                           esr_vmspagesize,
  input  logic                                    esr_bypass_icache,
  input  logic                                    esr_shire_coop_mode,
  // Request port
  output logic [NR_MINIONS-1:0]                   f0_req_ready,
  input  logic [NR_MINIONS-1:0]                   f0_req_valid,
  input  frontend_icache_req [NR_MINIONS-1:0]     f0_req,
  // Response
  output logic [NR_REQUESTORS-1:0]                f4_resp_valid,
  output logic [NR_REQUESTORS-1:0]                f4_resp_miss,
  output icache_frontend_resp [NR_REQUESTORS-1:0] f4_resp,
  output logic [NR_REQUESTORS-1:0]                f5_resp_fill_done,
  // Flush control
  input  logic [NR_MINIONS-1:0]                   f0_flush_data,
  // Request to L2 port
  input  logic                                    f0_l2_miss_req_disable_next,
  input  logic                                    f0_l2_miss_req_ready,
  output logic                                    f0_l2_miss_req_valid,
  output et_link_nodata_req_info_t                f0_l2_miss_req,
  // Fill from L2 port
  output logic                                    f0_l2_miss_resp_ready,
  input  logic                                    f0_l2_miss_resp_valid,
  input  et_link_rsp_info_t                       f0_l2_miss_resp,
  // TLB/PTW control
  input  minion_satp_info [NR_MINIONS-1:0]        satp_info,
  input  minion_satp_info [NR_MINIONS-1:0]        matp_info,
  input  logic [NR_MINIONS-1:0]                   tlb_invalidate,
  // PTW request
  output minion_ptw_req [NR_REQUESTORS-1:0]       ptw_req_data,
  output logic [NR_REQUESTORS-1:0]                ptw_req_valid,
  input  logic [NR_REQUESTORS-1:0]                ptw_req_ready,
  output logic [NR_REQUESTORS-1:0]                ptw_invalidate,
  // PTW response
  input  logic [NR_REQUESTORS-1:0]                ptw_resp_valid,
  input  minion_ptw_pte [NR_REQUESTORS-1:0]       ptw_resp_data,
  // Request to L1 SRAM blocks
  output logic                                    icache_f2_sram_req_write,
  output logic [`ICACHE_SRAM_ADD_WIDTH-1:0]       icache_f2_sram_req_addr,
  output logic                                    icache_f2_sram_req_valid,
  input  logic                                    icache_f2_sram_req_ready,
  // Response from L1 SRAM blocks
  input  logic [`ICACHE_SRAM_DATA_WIDTH-1:0]      icache_f0_sram_resp_dout,
  input  logic                                    icache_f0_sram_resp_valid,
  output logic                                    icache_f0_sram_resp_ready,
  // APB signals from BPAM for debug
  input  logic [`ICACHE_NEIGH_APB_ADDR_WIDTH-1:0] apb_paddr,
  input  logic                                    apb_pwrite,
  input  logic                                    apb_psel,
  input  logic                                    apb_penable,
  input  logic [`bpam_shire_apb_data_width-1:0]   apb_pwdata,
  output logic                                    apb_pready,
  output logic [`bpam_shire_apb_data_width-1:0]   apb_prdata,
  output logic                                    apb_pslverr,
  // Output debug signals going to Status Monitor
  output icache_dbg_sm_t                          dbg_sm_signals
  );

  localparam NR_MIN_PER_REQ       = NR_MINIONS/NR_REQUESTORS;
  localparam NR_THREADS_PER_REQ   = NR_MINIONS*`CORE_NR_THREADS/NR_REQUESTORS;
  localparam NR_MIN_PER_REQ_L     = $clog2(NR_MIN_PER_REQ);
  localparam NR_THREADS_PER_REQ_L = $clog2(NR_THREADS_PER_REQ);

  // INTERNAL DECLARATIONS
  logic [NR_MINIONS-1:0][`CORE_NR_THREADS-1:0]               f1_req_ready;
  logic [NR_MINIONS-1:0][`CORE_NR_THREADS-1:0]               f1_req_valid, f1_req_valid_next;
  logic [NR_MINIONS-1:0][`CORE_NR_THREADS-1:0]               f1_req_bid;
  frontend_icache_req [NR_MINIONS-1:0][`CORE_NR_THREADS-1:0] f1_req;
  logic [NR_REQUESTORS-1:0]                                  f1_req_arb_ready;
  logic [NR_REQUESTORS-1:0]                                  f1_req_arb_valid;
  frontend_icache_req [NR_REQUESTORS-1:0]                    f1_req_winner; // Winner requests from arbiters to Icache
  logic [NR_REQUESTORS-1:0][NR_THREADS_PER_REQ_L-1:0]        f1_req_hart_id;
  logic [NR_REQUESTORS-1:0][NR_MIN_PER_REQ_L-1:0]            f1_req_min_id;
  logic [NR_MINIONS-1:0]                                     f1_flush_data;
  icache_dbg_sm_t                                            icache_dbg_sm_signals;

  // //////////////////////////////////////////////////////////////////////////////
  // Flop icache request
  // //////////////////////////////////////////////////////////////////////////////

  for (genvar i = 0; i < NR_MINIONS; i++) begin: MIN_REQ_FF
    for (genvar t = 0; t < `CORE_NR_THREADS; t++) begin: TH_REQ_FF
      logic f0_thread_req_valid;
      assign f0_thread_req_valid = (f0_req[i].thread_id == t[`CORE_NR_THREADS_R]) && f0_req_valid[i];

      // CLK    RST    EN                                         DOUT                DIN                      DEF
      `RST_FF (clock, reset,                                            f1_req_valid[i][t], f1_req_valid_next[i][t], 1'b0)
      `EN_FF  (clock,        f0_thread_req_valid & ~f1_req_valid[i][t], f1_req[i][t],       f0_req[i])

      always_comb begin
        f1_req_valid_next[i][t] = f1_req_valid[i][t];

        if (f0_thread_req_valid)
        f1_req_valid_next[i][t] = 1'b1;

        if (f1_req_ready[i][t])
        f1_req_valid_next[i][t] = 1'b0;
      end
    end

    assign f0_req_ready[i] = |f1_req_ready[i];
  end

  // Flop icache flush control

  // CLK    RST    DOUT           DIN            DEF
  `RST_FF (clock, reset, f1_flush_data, f0_flush_data, '0)

  // //////////////////////////////////////////////////////////////////////////////
  // Request arbiter
  // //////////////////////////////////////////////////////////////////////////////

  for (genvar i = 0; i < NR_REQUESTORS; i++) begin: REQ_ARB
    logic [NR_THREADS_PER_REQ-1:0] f1_grant;
    logic [NR_THREADS_PER_REQ-1:0] f2_grant;
    logic [NR_THREADS_PER_REQ-1:0] f3_grant;
    logic [NR_THREADS_PER_REQ-1:0] f4_grant;
    logic                          f4_update_prio;
    logic [NR_THREADS_PER_REQ-1:0] th_miss_mask, th_miss_mask_next;

    assign f1_req_bid[i*NR_MIN_PER_REQ +: NR_MIN_PER_REQ] = f1_req_valid[i*NR_MIN_PER_REQ +: NR_MIN_PER_REQ] & th_miss_mask;

    arb_lru_delayed_data #(
      .WIDTH         ( $bits(frontend_icache_req)                       ),
      .NUM_CLIENTS   ( NR_THREADS_PER_REQ                               )
    ) req_arb (
      // System signals
      .clock         ( clock                                            ),
      .reset         ( reset                                            ),
      // Bidding
      .bid           ( f1_req_bid[i*NR_MIN_PER_REQ +: NR_MIN_PER_REQ]   ),
      .stall         ( ~f1_req_arb_ready[i]                             ),
      // Data
      .data_in       ( f1_req[i*NR_MIN_PER_REQ +: NR_MIN_PER_REQ]       ),
      // Winner
      .grant         ( f1_req_ready[i*NR_MIN_PER_REQ +: NR_MIN_PER_REQ] ),
      .data_winner   ( f1_req_winner[i]                                 ),
      .winner        ( f1_req_hart_id[i]                                ),
      // Updating priority
      .update_prio   ( f4_update_prio                                   ),
      .grant_delayed ( f4_grant                                         )
    );

    assign f1_req_min_id[i]    = f1_req_hart_id[i][`CORE_NR_THREADS_L +: NR_MIN_PER_REQ_L];
    assign f1_req_arb_valid[i] = |f1_req_bid[i*NR_MIN_PER_REQ +: NR_MIN_PER_REQ];

    // Update priority if there is a hit
    //         CLK    RST    EN                   DOUT      DIN                                               DEF
    `RST_EN_FF(clock, reset, f1_req_arb_ready[i], f1_grant, f1_req_ready[i*NR_MIN_PER_REQ +: NR_MIN_PER_REQ], '0)
    `RST_FF   (clock, reset,                      f2_grant, f1_grant,                                         '0)
    `RST_FF   (clock, reset,                      f3_grant, f2_grant,                                         '0)
    `RST_FF   (clock, reset,                      f4_grant, f3_grant,                                         '0)

    assign f4_update_prio = f4_resp_valid[i] & (~f4_resp_miss[i] | f4_resp[i].page_fault | f4_resp[i].access_fault);


    // //////////////////////////////////////////////////////////////////////////////
    // Mechanism to avoid livelock
    // //////////////////////////////////////////////////////////////////////////////
    logic                               f4_resp_err;
    logic [NR_THREADS_PER_REQ-1:0][4:0] th_miss_cnt;
    logic [NR_THREADS_PER_REQ-1:0]      th_miss_cnt_sat;
    logic [NR_THREADS_PER_REQ-1:0]      th_miss_cnt_en;
    logic                               th_miss_mask_en;
    logic                               th_miss_any_masked;

    assign f4_resp_err = f4_resp[i].page_fault | f4_resp[i].access_fault | f4_resp[i].bus_err | f4_resp[i].ecc_err;

    // Count the number of consecutive misses per thread
    for (genvar t = 0; t < NR_THREADS_PER_REQ; t++) begin: TH_MISS_CNT
      logic [4:0] th_miss_cnt_next;

      // CLK    RST    EN                 DOUT            DIN               DEF
      `RST_EN_FF(clock, reset, th_miss_cnt_en[t], th_miss_cnt[t], th_miss_cnt_next, '0)

      assign th_miss_cnt_sat[t] = th_miss_cnt[t] == '1;
      assign th_miss_cnt_en[t]  = f4_resp_valid[i] & (f4_grant[t] | f4_resp_err);

      always_comb begin
        th_miss_cnt_next = th_miss_cnt[t];

        // Counter is reset if this thread hits or on error
        if (~f4_resp_miss[i] | f4_resp_err)
        th_miss_cnt_next = '0;
        // Otherwise increment counter if it is not saturated
        else if (~th_miss_cnt_sat[t])
        th_miss_cnt_next = th_miss_cnt_next + 1'b1;
      end
    end

    // If any counters saturates, mask threads from other minions until those ones hit
    // CLK    RST    EN               DOUT             DIN                DEF
    `RST_FF   (clock, reset,                  th_miss_mask_en, |th_miss_cnt_en,   1'b0)
    `RST_EN_FF(clock, reset, th_miss_mask_en, th_miss_mask,    th_miss_mask_next, '1)

    always_comb begin
      th_miss_any_masked = 1'b0;
      if (|th_miss_cnt_sat) begin
        // Allow both threads of a minion, so that saturating thread is
        // not blocked by a masked thread
        for (integer t_idx = 0; t_idx < NR_THREADS_PER_REQ; t_idx=t_idx+2) begin
          th_miss_mask_next[t_idx]   = ~th_miss_any_masked & (th_miss_cnt_sat[t_idx] | th_miss_cnt_sat[t_idx+1]);
          th_miss_mask_next[t_idx+1] = ~th_miss_any_masked & (th_miss_cnt_sat[t_idx] | th_miss_cnt_sat[t_idx+1]);
          // If more than one thread saturates, allow only one minion simultaneously
          th_miss_any_masked         = th_miss_any_masked | th_miss_cnt_sat[t_idx] | th_miss_cnt_sat[t_idx+1];
        end
      end
      else
      th_miss_mask_next = '1;
    end
  end
  

  // //////////////////////////////////////////////////////////////////////////////
  // icache instance
  // //////////////////////////////////////////////////////////////////////////////

  icache_top #(
    .NR_MINIONS                  ( NR_MINIONS                  ),
    .NR_REQUESTORS               ( NR_REQUESTORS               )
  )
  icache (
    // System signals
    .clock                       ( clock                       ),
    .reset                       ( reset                       ),
    // ESRs
    .esr_prefetch_conf           ( esr_prefetch_conf           ),
    .esr_prefetch_start          ( esr_prefetch_start          ),
    .esr_prefetch_done           ( esr_prefetch_done           ),
    .esr_err_log_ctl             ( esr_err_log_ctl             ),
    .esr_err_log_sbe             ( esr_err_log_sbe             ),
    .esr_err_log_dbe             ( esr_err_log_dbe             ),
    .esr_err_log_info            ( esr_err_log_info            ),
    .esr_mprot                   ( esr_mprot                   ),
    .esr_vmspagesize             ( esr_vmspagesize             ),
    .esr_bypass_icache           ( esr_bypass_icache           ),
    .esr_shire_coop_mode         ( esr_shire_coop_mode         ),
    // Request port
    .f0_req_ready                ( f1_req_arb_ready            ),
    .f0_req_valid                ( f1_req_arb_valid            ),
    .f0_req                      ( f1_req_winner               ),
    .f0_req_min_id               ( f1_req_min_id               ),
    // Response
    .f4_resp_valid               ( f4_resp_valid               ),
    .f4_resp_miss                ( f4_resp_miss                ),
    .f4_resp                     ( f4_resp                     ),
    .f5_resp_fill_done           ( f5_resp_fill_done           ),
    // Flush control
    .f0_flush_data               ( |f1_flush_data              ),
    // Request to L2 port
    .f0_l2_miss_req_disable_next ( f0_l2_miss_req_disable_next ),
    .f0_l2_miss_req_ready        ( f0_l2_miss_req_ready        ),
    .f0_l2_miss_req_valid        ( f0_l2_miss_req_valid        ),
    .f0_l2_miss_req              ( f0_l2_miss_req              ),
    // Fill from L2 port
    .f0_l2_miss_resp_ready       ( f0_l2_miss_resp_ready       ),
    .f0_l2_miss_resp_valid       ( f0_l2_miss_resp_valid       ),
    .f0_l2_miss_resp             ( f0_l2_miss_resp             ),
    // TLB/PTW control
    .satp_info                   ( satp_info                   ),
    .matp_info                   ( matp_info                   ),
    .tlb_invalidate              ( tlb_invalidate              ),
    // PTW request
    .ptw_req_ready               ( ptw_req_ready               ),
    .ptw_req_valid               ( ptw_req_valid               ),
    .ptw_req_data                ( ptw_req_data                ),
    .ptw_invalidate              ( ptw_invalidate              ),
    // PTW response
    .ptw_resp_valid              ( ptw_resp_valid              ),
    .ptw_resp_data               ( ptw_resp_data               ),
    // Request to L1 SRAM blocks
    .f2_sram_req_write           ( icache_f2_sram_req_write    ),
    .f2_sram_req_addr            ( icache_f2_sram_req_addr     ),
    .f2_sram_req_valid           ( icache_f2_sram_req_valid    ),
    .f2_sram_req_ready           ( icache_f2_sram_req_ready    ),
    // Response from L1 SRAM blocks
    .f0_sram_resp_dout           ( icache_f0_sram_resp_dout    ),
    .f0_sram_resp_valid          ( icache_f0_sram_resp_valid   ),
    .f0_sram_resp_ready          ( icache_f0_sram_resp_ready   ),
    // APB signals from BPAM for debug
    .apb_paddr                   ( apb_paddr                   ),
    .apb_pwrite                  ( apb_pwrite                  ),
    .apb_psel                    ( apb_psel                    ),
    .apb_penable                 ( apb_penable                 ),
    .apb_pwdata                  ( apb_pwdata                  ),
    .apb_pready                  ( apb_pready                  ),
    .apb_prdata                  ( apb_prdata                  ),
    .apb_pslverr                 ( apb_pslverr                 ),
    // Output debug signals going to Status Monitor
    .dbg_sm_signals              ( icache_dbg_sm_signals       )
  );

  // //////////////////////////////////////////////////////////////////////////////
  // Connect output debug signals
  // //////////////////////////////////////////////////////////////////////////////
  always_comb begin
    // Take signals from icache
    dbg_sm_signals = icache_dbg_sm_signals;
    // Connect shared_icache signals
    for (integer req_idx = 0; req_idx < NR_REQUESTORS; req_idx++) begin
      dbg_sm_signals.f1_req_valid[req_idx] = f1_req_arb_valid[req_idx] & f1_req_arb_ready[req_idx];
      dbg_sm_signals.f1_req_addr[req_idx] = f1_req_winner[req_idx].addr[`VA_RANGE_CA];
    end
  end

endmodule

