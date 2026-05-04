// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Standalone-Minion shared I-cache top.
//
// Replaces: `icache_top` from etcore-soc.

module icache_top
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import minion_frontend_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
#(
  parameter int unsigned NrMinions = 8,
  parameter int unsigned NrRequestors = 2,
  localparam int unsigned NrMinPerReq = NrMinions / NrRequestors,
  localparam int unsigned NrMinPerReqW = (NrMinPerReq == 1) ? 1 : $clog2(NrMinPerReq),
  localparam int unsigned NrRequestorsW = (NrRequestors == 1) ? 1 : $clog2(NrRequestors)
) (
  input  logic                                               clk_i,
  input  logic                                               rst_ni,
  input  logic                                               ioshire_i,
  input  icache_prefetch_conf_t                              esr_prefetch_conf_i,
  input  logic                                               esr_prefetch_start_i,
  output logic                                               esr_prefetch_done_o,
  input  esr_icache_err_log_ctl_t                            esr_err_log_ctl_i,
  output logic                                               esr_err_log_sbe_o,
  output logic                                               esr_err_log_dbe_o,
  output icache_err_log_info_t                               esr_err_log_info_o,
  input  esr_mprot_t                                         esr_mprot_i,
  input  tlb_entry_type                                      esr_vmspagesize_i,
  input  logic                                               esr_bypass_icache_i,
  input  logic                                               esr_shire_coop_mode_i,
  output logic [NrRequestors-1:0]                            f0_req_ready_o,
  input  logic [NrRequestors-1:0]                            f0_req_valid_i,
  input  frontend_icache_req_t                               f0_req_i [NrRequestors-1:0],
  input  logic [NrRequestors-1:0][NrMinPerReqW-1:0]          f0_req_min_id_i,
  output logic [NrRequestors-1:0]                            f4_resp_valid_o,
  output logic [NrRequestors-1:0]                            f4_resp_miss_o,
  output icache_frontend_resp_t                              f4_resp_o [NrRequestors-1:0],
  output logic [NrRequestors-1:0]                            f5_resp_fill_done_o,
  input  logic                                               f0_flush_data_i,
  input  logic                                               f0_l2_miss_req_disable_next_i,
  input  logic                                               f0_l2_miss_req_ready_i,
  output logic                                               f0_l2_miss_req_valid_o,
  output etlink_pkg::req_nodata_t                            f0_l2_miss_req_o,
  output logic                                               f0_l2_miss_resp_ready_o,
  /* verilator lint_off UNUSEDSIGNAL */  // The shared I-cache top currently only consumes the response opcode; data payload routing stays inside the SRAM side.
  input  etlink_pkg::rsp_t                                   f0_l2_miss_resp_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic                                               f0_l2_miss_resp_valid_i,
  input  minion_satp_info                                    satp_info_i [NrMinions-1:0],
  input  minion_satp_info                                    matp_info_i [NrMinions-1:0],
  input  logic [NrMinions-1:0]                               tlb_invalidate_i,
  output minion_ptw_req                                      ptw_req_data_o [NrRequestors-1:0],
  output logic [NrRequestors-1:0]                            ptw_req_valid_o,
  input  logic [NrRequestors-1:0]                            ptw_req_ready_i,
  output logic [NrRequestors-1:0]                            ptw_invalidate_o,
  input  logic [NrRequestors-1:0]                            ptw_resp_valid_i,
  input  minion_ptw_pte                                      ptw_resp_data_i [NrRequestors-1:0],
  output logic                                               f2_sram_req_write_o,
  output logic [IcacheSramAddrWidth-1:0]                     f2_sram_req_addr_o,
  output logic                                               f2_sram_req_valid_o,
  input  logic                                               f2_sram_req_ready_i,
  input  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0]  f0_sram_resp_dout_i,
  input  logic                                               f0_sram_resp_valid_i,
  output logic                                               f0_sram_resp_ready_o,
  input  logic [IcacheNeighApbAddrWidth-1:0]                 apb_paddr_i,
  input  logic                                               apb_pwrite_i,
  input  logic                                               apb_psel_i,
  input  logic                                               apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0]                   apb_pwdata_i,
  output logic                                               apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0]                   apb_prdata_o,
  output logic                                               apb_pslverr_o,
  output icache_dbg_sm_t                                     dbg_sm_signals_o
);

  localparam logic [etlink_pkg::SubopcodeSize-1:0] EtLinkReadL2 = '0;
  localparam int unsigned IcacheErrCodeSbeIdx = 0;
  localparam int unsigned IcacheErrCodeDbeIdx = 1;

  /* verilator lint_off UNUSEDSIGNAL */  // Only requestor 0 receives the shared prefetch ESRs, so only its done pulse is observed at the top level.
  logic [NrRequestors-1:0]                             ucache_prefetch_done;
  /* verilator lint_on UNUSEDSIGNAL */
  icache_prefetch_conf_t                               ucache_prefetch_conf [NrRequestors-1:0];
  logic [NrRequestors-1:0]                             ucache_prefetch_start;
  logic [NrRequestors-1:0]                             ucache_l1_miss_req_ready;
  logic [NrRequestors-1:0]                             ucache_l1_miss_req_valid;
  logic [NrRequestors-1:0][PaSize-1:0]                 ucache_l1_miss_req_addr;
  logic [NrRequestors-1:0]                             ucache_l1_miss_req_ff_available_d;
  logic [NrRequestors-1:0]                             ucache_l1_miss_req_ff_available_q;
  logic [NrRequestors-1:0]                             ucache_l1_miss_req_ff_in_accepted;
  logic [NrRequestors-1:0]                             ucache_l1_miss_req_ff_out_accepted;
  logic [NrRequestors-1:0][PaSize-1:0]                 ucache_l1_miss_req_addr_ff;
  logic [NrRequestors-1:0]                             ucache_l1_miss_resp_early_valid;
  logic [NrRequestors-1:0]                             ucache_l1_miss_resp_valid;
  logic [NrRequestors-1:0][IcacheBlockBits-1:0]        ucache_l1_miss_resp_data;
  logic [NrRequestors-1:0]                             ucache_l1_miss_resp_ecc_err;
  logic [NrRequestors-1:0]                             ucache_l1_miss_resp_l2_err;
  logic [NrRequestors-1:0][IcacheDbgUcacheAddrWidth-1:0] ucache_apb_paddr;
  logic [NrRequestors-1:0]                             ucache_apb_pwrite;
  logic [NrRequestors-1:0]                             ucache_apb_psel;
  logic [NrRequestors-1:0]                             ucache_apb_penable;
  logic [NrRequestors-1:0][BpamShireApbDataWidth-1:0]  ucache_apb_pwdata;
  logic [NrRequestors-1:0]                             ucache_apb_pready;
  logic [NrRequestors-1:0][BpamShireApbDataWidth-1:0]  ucache_apb_prdata;
  logic [NrRequestors-1:0]                             ucache_apb_pslverr;
  icache_dbg_sm_t                                      ucache_dbg_sm_signals [NrRequestors-1:0];

  logic                                                f0_l1_miss_req_ready_d;
  logic                                                f0_l1_miss_req_ready_q;
  logic                                                f0_l1_miss_req_valid_d;
  logic                                                f0_l1_miss_req_valid_q;
  logic [PaSize-1:0]                                   f0_l1_miss_req_addr_d;
  logic [PaSize-1:0]                                   f0_l1_miss_req_addr_q;
  logic [NrRequestorsW-1:0]                            f0_l1_miss_req_idx_d;
  logic [NrRequestorsW-1:0]                            f0_l1_miss_req_idx_q;

  icache_miss_state_e                                  f0_miss_state_d;
  icache_miss_state_e                                  f0_miss_state_q;
  /* verilator lint_off UNUSEDSIGNAL */  // The shared tag/data side intentionally ignores block-offset bits; the stored address mirrors the original full-address staging.
  logic [PaSize-1:0]                                   f0_paddr_q;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [IcacheWayAddrWidth-1:0]                       f0_read_way_q;
  logic [IcacheWayAddrWidth-1:0]                       f0_write_way_q;
  logic                                                f0_l2_miss_resp_done_d;
  logic                                                f0_l2_miss_resp_done_q;
  logic                                                f0_l1_resp_done_d;
  logic                                                f0_l1_resp_done_q;
  logic                                                f0_l1_miss_resp_early_valid;
  logic                                                f0_l1_miss_resp_valid;
  logic [IcacheBlockBits-1:0]                          f0_l1_miss_resp_data_q;
  logic                                                f0_l1_miss_resp_ecc_err_d;
  logic                                                f0_l1_miss_resp_ecc_err_q;
  logic                                                f0_l1_miss_resp_l2_err;
  logic                                                f0_data_array_resp_early_valid;
  logic                                                f0_data_array_resp_valid;
  logic [IcacheBlockBits-1:0]                          f0_data_array_resp_data;
  logic [IcacheEccBlocks-1:0]                          f0_data_array_resp_sbe_per_block;
  logic [IcacheEccBlocks-1:0]                          f0_data_array_resp_dbe_per_block;
  logic                                                f0_da_sbe;
  logic                                                f0_da_dbe;
  logic                                                f0_l2_err_q;
  logic [IcacheSetAddrWidth-1:0]                       f0_set;

  logic                                                f0_l2_miss_req_valid_d;
  logic                                                f0_l2_miss_req_acc;
  /* verilator lint_off UNUSEDSIGNAL */  // The outer miss FSM only consumes the response opcode; the rest of the captured ET-link payload remains for interface fidelity.
  etlink_pkg::rsp_t                                    f0_l2_miss_resp_reg_q;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                                                f0_l2_miss_resp_valid_reg_q;
  logic                                                f0_l2_miss_resp_err;

  logic [IcacheDbgTagAddrWidth-1:0]                    tag_apb_paddr;
  logic                                                tag_apb_pwrite;
  logic                                                tag_apb_psel;
  logic                                                tag_apb_penable;
  logic [BpamShireApbDataWidth-1:0]                    tag_apb_pwdata;
  logic                                                tag_apb_pready;
  logic [BpamShireApbDataWidth-1:0]                    tag_apb_prdata;
  logic                                                tag_apb_pslverr;

  logic [PaSize-1:0]                                   f1_paddr_q;
  logic [IcacheWayAddrWidth-1:0]                       f1_read_way;
  logic [IcacheWayAddrWidth-1:0]                       f1_write_way;
  logic [IcacheSetAddrWidth-1:0]                       f1_set;
  logic                                                f1_valid_q;
  logic                                                f1_hit;
  logic                                                f1_miss;
  logic [IcacheWays-1:0]                               f1_replace_way;
  logic                                                f1_lru_update_en_early;
  logic                                                f1_lru_update_en;
  logic [IcacheWays-1:0]                               f1_accessed_way;
  logic [IcacheWays-1:0]                               f1_tag_hit;
  logic                                                f1_tag_array_miss;
  logic                                                f1_any_tag_hit;

  icache_tag_write_req_t                               f0_tag_write_req;
  logic                                                f0_tag_write_early_valid;
  logic                                                f0_tag_write_valid;

  logic                                                esr_err_log_sbe_d;
  logic                                                esr_err_log_dbe_d;
  icache_err_log_info_t                                esr_err_log_info_d;

  always_comb begin
    for (int i = 0; i < NrRequestors; i++) begin
      ucache_prefetch_conf[i] = '0;
      ucache_prefetch_start[i] = 1'b0;
      if (i == 0) begin
        ucache_prefetch_conf[i] = esr_prefetch_conf_i;
        ucache_prefetch_start[i] = esr_prefetch_start_i;
      end
    end
  end

  assign esr_prefetch_done_o = ucache_prefetch_done[0];

  for (genvar g_req = 0; g_req < NrRequestors; g_req++) begin : gen_ucache
    icache_micro_cache #(
      .Id       (g_req),
      .NrMinions(NrMinPerReq)
    ) u_micro_cache (
      .clk_i                      (clk_i),
      .rst_ni                     (rst_ni),
      .ioshire_i                  (ioshire_i),
      .esr_prefetch_conf_i        (ucache_prefetch_conf[g_req]),
      .esr_prefetch_start_i       (ucache_prefetch_start[g_req]),
      .esr_prefetch_done_o        (ucache_prefetch_done[g_req]),
      .esr_mprot_i                (esr_mprot_i),
      .esr_vmspagesize_i          (esr_vmspagesize_i),
      .esr_bypass_icache_i        (esr_bypass_icache_i),
      .esr_shire_coop_mode_i      (esr_shire_coop_mode_i),
      .f0_req_ready_o             (f0_req_ready_o[g_req]),
      .f0_req_valid_i             (f0_req_valid_i[g_req]),
      .f0_req_i                   (f0_req_i[g_req]),
      .f0_req_min_id_i            (f0_req_min_id_i[g_req]),
      .f4_resp_valid_o            (f4_resp_valid_o[g_req]),
      .f4_resp_miss_o             (f4_resp_miss_o[g_req]),
      .f4_resp_o                  (f4_resp_o[g_req]),
      .f5_resp_fill_done_o        (f5_resp_fill_done_o[g_req]),
      .f0_flush_data_i            (f0_flush_data_i),
      .f0_l1_miss_req_ready_i     (ucache_l1_miss_req_ready[g_req]),
      .f0_l1_miss_req_valid_o     (ucache_l1_miss_req_valid[g_req]),
      .f0_l1_miss_req_addr_o      (ucache_l1_miss_req_addr[g_req]),
      .f0_l1_miss_resp_early_valid_i(ucache_l1_miss_resp_early_valid[g_req]),
      .f0_l1_miss_resp_valid_i    (ucache_l1_miss_resp_valid[g_req]),
      .f0_l1_miss_resp_data_i     (ucache_l1_miss_resp_data[g_req]),
      .f0_l1_miss_resp_ecc_err_i  (ucache_l1_miss_resp_ecc_err[g_req]),
      .f0_l1_miss_resp_l2_err_i   (ucache_l1_miss_resp_l2_err[g_req]),
      .satp_info_i                (satp_info_i[g_req*NrMinPerReq +: NrMinPerReq]),
      .matp_info_i                (matp_info_i[g_req*NrMinPerReq +: NrMinPerReq]),
      .tlb_invalidate_i           (tlb_invalidate_i[g_req*NrMinPerReq +: NrMinPerReq]),
      .ptw_req_data_o             (ptw_req_data_o[g_req]),
      .ptw_req_valid_o            (ptw_req_valid_o[g_req]),
      .ptw_req_ready_i            (ptw_req_ready_i[g_req]),
      .ptw_invalidate_o           (ptw_invalidate_o[g_req]),
      .ptw_resp_valid_i           (ptw_resp_valid_i[g_req]),
      .ptw_resp_data_i            (ptw_resp_data_i[g_req]),
      .apb_paddr_i                (ucache_apb_paddr[g_req]),
      .apb_pwrite_i               (ucache_apb_pwrite[g_req]),
      .apb_psel_i                 (ucache_apb_psel[g_req]),
      .apb_penable_i              (ucache_apb_penable[g_req]),
      .apb_pwdata_i               (ucache_apb_pwdata[g_req]),
      .apb_pready_o               (ucache_apb_pready[g_req]),
      .apb_prdata_o               (ucache_apb_prdata[g_req]),
      .apb_pslverr_o              (ucache_apb_pslverr[g_req]),
      .dbg_sm_signals_o           (ucache_dbg_sm_signals[g_req])
    );

    assign ucache_l1_miss_req_ready[g_req] = ucache_l1_miss_req_ff_available_q[g_req];
    assign ucache_l1_miss_req_ff_in_accepted[g_req] =
        ucache_l1_miss_req_valid[g_req] & ucache_l1_miss_req_ff_available_q[g_req];
  end

  always_comb begin
    for (int i = 0; i < NrRequestors; i++) begin
      if (ucache_l1_miss_req_ff_available_q[i]) begin
        ucache_l1_miss_req_ff_available_d[i] = ~ucache_l1_miss_req_valid[i];
      end else begin
        ucache_l1_miss_req_ff_available_d[i] = ucache_l1_miss_req_ff_out_accepted[i];
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      ucache_l1_miss_req_ff_available_q <= '1;
    end else begin
      ucache_l1_miss_req_ff_available_q <= ucache_l1_miss_req_ff_available_d;
    end
  end

  always_ff @(posedge clk_i) begin
    for (int i = 0; i < NrRequestors; i++) begin
      if (ucache_l1_miss_req_ff_in_accepted[i]) begin
        ucache_l1_miss_req_addr_ff[i] <= ucache_l1_miss_req_addr[i];
      end
    end
  end

  if (NrRequestors > 1) begin : gen_l1_miss_req_arb
    logic [NrRequestors-1:0]             ucache_l1_miss_req_bid;
    logic [NrRequestors-1:0]             ucache_l1_miss_req_grant;

    assign ucache_l1_miss_req_bid = ~ucache_l1_miss_req_ff_available_q;
    assign ucache_l1_miss_req_ff_out_accepted =
        f0_l1_miss_req_ready_q ? ucache_l1_miss_req_grant : '0;

    prim_arb_rr #(
      .NumReqs(NrRequestors),
      .Width  (PaSize)
    ) u_l1_miss_req_arb (
      .clk_i   (clk_i),
      .rst_ni  (rst_ni),
      .reqs_i  (ucache_l1_miss_req_bid),
      .pop_i   (f0_l1_miss_req_ready_q),
      .grants_o(ucache_l1_miss_req_grant),
      .data_i  (ucache_l1_miss_req_addr_ff),
      .data_o  (f0_l1_miss_req_addr_d)
    );

    always_comb begin
      f0_l1_miss_req_idx_d = '0;
      for (int i = 0; i < NrRequestors; i++) begin
        if (ucache_l1_miss_req_grant[i]) begin
          f0_l1_miss_req_idx_d |= i[NrRequestorsW-1:0];
        end
      end
    end

    assign f0_l1_miss_req_valid_d = (|ucache_l1_miss_req_bid) & f0_l1_miss_req_ready_q;
  end else begin : gen_one_l1_miss_req
    assign ucache_l1_miss_req_ff_out_accepted[0] = f0_l1_miss_req_ready_q;
    assign f0_l1_miss_req_addr_d = ucache_l1_miss_req_addr_ff[0];
    assign f0_l1_miss_req_idx_d = '0;
    assign f0_l1_miss_req_valid_d =
        ~ucache_l1_miss_req_ff_available_q[0] & f0_l1_miss_req_ready_q;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f0_l1_miss_req_ready_q <= 1'b1;
      f0_l1_miss_req_valid_q <= 1'b0;
    end else begin
      f0_l1_miss_req_ready_q <= f0_l1_miss_req_ready_d;
      f0_l1_miss_req_valid_q <= f0_l1_miss_req_valid_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (f0_l1_miss_req_ready_q && f0_l1_miss_req_valid_d) begin
      f0_l1_miss_req_addr_q <= f0_l1_miss_req_addr_d;
      f0_l1_miss_req_idx_q <= f0_l1_miss_req_idx_d;
    end
  end

  always_comb begin
    if (f0_l1_miss_req_ready_q) begin
      f0_l1_miss_req_ready_d = ~f0_l1_miss_req_valid_d;
    end else begin
      f0_l1_miss_req_ready_d = f0_l1_miss_resp_valid;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f0_miss_state_q <= IcacheMissStateReady;
      f0_l2_miss_resp_done_q <= 1'b0;
      f0_l1_resp_done_q <= 1'b0;
      esr_err_log_sbe_o <= 1'b0;
      esr_err_log_dbe_o <= 1'b0;
    end else begin
      f0_miss_state_q <= f0_miss_state_d;
      f0_l2_miss_resp_done_q <= f0_l2_miss_resp_done_d;
      f0_l1_resp_done_q <= f0_l1_resp_done_d;
      esr_err_log_sbe_o <= esr_err_log_sbe_d;
      esr_err_log_dbe_o <= esr_err_log_dbe_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (f1_miss | f1_hit) begin
      f0_paddr_q <= f1_paddr_q;
    end
    if (f1_hit) begin
      f0_read_way_q <= f1_read_way;
    end
    if (f1_miss) begin
      f0_write_way_q <= f1_write_way;
    end
    if (f0_data_array_resp_valid) begin
      f0_l1_miss_resp_data_q <= f0_data_array_resp_data;
      f0_l1_miss_resp_ecc_err_q <= f0_l1_miss_resp_ecc_err_d;
    end
    if (f0_l2_miss_resp_valid_reg_q) begin
      f0_l2_err_q <= f0_l2_miss_resp_err;
    end
    if (esr_err_log_sbe_d || esr_err_log_dbe_d) begin
      esr_err_log_info_o <= esr_err_log_info_d;
    end
  end

  always_comb begin
    f0_miss_state_d = f0_miss_state_q;

    if (f0_miss_state_q == IcacheMissStateReady) begin
      if (f1_miss) begin
        f0_miss_state_d = IcacheMissStateRequest;
      end
    end else if (f0_miss_state_q == IcacheMissStateRequest) begin
      if (f0_l2_miss_req_ready_i) begin
        f0_miss_state_d = IcacheMissStateFillWait;
      end
    end else begin
      if (f0_l1_miss_resp_valid) begin
        f0_miss_state_d = IcacheMissStateReady;
      end
    end
  end

  always_comb begin
    f0_l2_miss_req_valid_d =
        (f0_miss_state_d == IcacheMissStateRequest) && !f0_l2_miss_req_disable_next_i;
    f0_l2_miss_req_o.id = '0;
    f0_l2_miss_req_o.source = '0;
    f0_l2_miss_req_o.wdata = 1'b0;
    f0_l2_miss_req_o.opcode = etlink_pkg::ReqRead;
    f0_l2_miss_req_o.subopcode = EtLinkReadL2;
    f0_l2_miss_req_o.address = {f0_paddr_q[PaSize-1:IcacheBlockAddrSize], {IcacheBlockAddrSize{1'b0}}};
    f0_l2_miss_req_o.size = etlink_pkg::SizeLine;
    f0_l2_miss_req_o.qwen = '0;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f0_l2_miss_req_valid_o <= 1'b0;
      f0_l2_miss_resp_valid_reg_q <= 1'b0;
    end else begin
      f0_l2_miss_req_valid_o <= f0_l2_miss_req_valid_d;
      f0_l2_miss_resp_valid_reg_q <= f0_l2_miss_resp_valid_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (f0_l2_miss_resp_valid_i) begin
      f0_l2_miss_resp_reg_q <= f0_l2_miss_resp_i;
    end
  end

  assign f0_l2_miss_req_acc = f0_l2_miss_req_valid_o & f0_l2_miss_req_ready_i;
  assign f0_l2_miss_resp_err = f0_l2_miss_resp_reg_q.opcode == etlink_pkg::RspErr;
  assign f0_l2_miss_resp_ready_o = 1'b1;

  always_comb begin
    f0_l2_miss_resp_done_d = f0_l2_miss_resp_done_q;
    f0_l1_resp_done_d = f0_l1_resp_done_q;

    if (f0_l2_miss_resp_valid_reg_q) begin
      f0_l2_miss_resp_done_d = 1'b1;
    end else if (f0_l1_miss_resp_valid) begin
      f0_l2_miss_resp_done_d = 1'b0;
    end

    if (f0_data_array_resp_valid) begin
      f0_l1_resp_done_d = 1'b1;
    end else if (f0_l1_miss_resp_valid) begin
      f0_l1_resp_done_d = 1'b0;
    end
  end

  assign f0_l1_miss_resp_early_valid =
      f0_l1_resp_done_d &&
      ((f0_miss_state_d != IcacheMissStateFillWait) || f0_l2_miss_resp_done_d);
  assign f0_l1_miss_resp_valid =
      f0_l1_resp_done_q &&
      ((f0_miss_state_q != IcacheMissStateFillWait) || f0_l2_miss_resp_done_q);

  assign f0_da_sbe = |f0_data_array_resp_sbe_per_block &
                     f0_data_array_resp_valid &
                     (f0_miss_state_q != IcacheMissStateFillWait);
  assign f0_da_dbe = |f0_data_array_resp_dbe_per_block &
                     f0_data_array_resp_valid &
                     (f0_miss_state_q != IcacheMissStateFillWait);

  assign f0_l1_miss_resp_ecc_err_d =
      esr_err_log_ctl_i.err_rsp_enable &&
      ((f0_da_sbe && esr_err_log_ctl_i.err_interrupt_enable[IcacheErrCodeSbeIdx]) ||
       (f0_da_dbe && esr_err_log_ctl_i.err_interrupt_enable[IcacheErrCodeDbeIdx]));
  assign f0_l1_miss_resp_l2_err = f0_l2_err_q & f0_l2_miss_resp_done_q;

  assign esr_err_log_sbe_d = f0_da_sbe;
  assign esr_err_log_dbe_d = f0_da_dbe;

  always_comb begin
    esr_err_log_info_d = '0;
    esr_err_log_info_d.way = f0_read_way_q;
    esr_err_log_info_d.set = f0_paddr_q[IcacheBlockAddrSize +: IcacheSetAddrWidth];
    esr_err_log_info_d.dbe_per_block = f0_data_array_resp_dbe_per_block;
    esr_err_log_info_d.sbe_per_block = f0_data_array_resp_sbe_per_block;
    esr_err_log_info_d.address = f0_paddr_q[PaSize-1:IcacheBlockAddrSize];
  end

  always_comb begin
    for (int i = 0; i < NrRequestors; i++) begin
      ucache_l1_miss_resp_early_valid[i] =
          f0_l1_miss_resp_early_valid && (f0_l1_miss_req_idx_q == i[NrRequestorsW-1:0]);
      ucache_l1_miss_resp_valid[i] =
          f0_l1_miss_resp_valid && (f0_l1_miss_req_idx_q == i[NrRequestorsW-1:0]);
      ucache_l1_miss_resp_data[i] = f0_l1_miss_resp_data_q;
      ucache_l1_miss_resp_ecc_err[i] = f0_l1_miss_resp_ecc_err_q;
      ucache_l1_miss_resp_l2_err[i] = f0_l1_miss_resp_l2_err;
    end
  end

  assign f0_set = f0_l1_miss_req_addr_q[IcacheBlockAddrSize +: IcacheSetAddrWidth];

  assign tag_apb_paddr = apb_paddr_i[IcacheDbgTagAddrWidth-1:0];
  assign tag_apb_pwrite = apb_pwrite_i;
  assign tag_apb_psel =
      apb_psel_i &&
      (apb_paddr_i[IcacheDbgAddrRegionStart +: IcacheDbgAddrRegionSize] == IcacheDbgTagAddrRegion);
  assign tag_apb_penable =
      apb_penable_i &&
      (apb_paddr_i[IcacheDbgAddrRegionStart +: IcacheDbgAddrRegionSize] == IcacheDbgTagAddrRegion);
  assign tag_apb_pwdata = apb_pwdata_i;

  always_comb begin
    for (int i = 0; i < NrRequestors; i++) begin
      ucache_apb_paddr[i] = apb_paddr_i[IcacheDbgUcacheAddrWidth-1:0];
      ucache_apb_pwrite[i] = apb_pwrite_i;
      ucache_apb_psel[i] =
          apb_psel_i &&
          (apb_paddr_i[IcacheDbgUcacheAddrStart +: IcacheDbgUcacheAddrSize] ==
           i[IcacheDbgUcacheAddrSize-1:0]);
      ucache_apb_penable[i] =
          apb_penable_i &&
          (apb_paddr_i[IcacheDbgUcacheAddrStart +: IcacheDbgUcacheAddrSize] ==
           i[IcacheDbgUcacheAddrSize-1:0]);
      ucache_apb_pwdata[i] = apb_pwdata_i;
    end
  end

  always_comb begin
    apb_pready_o = tag_apb_pready;
    apb_prdata_o = tag_apb_prdata & {BpamShireApbDataWidth{tag_apb_pready}};
    apb_pslverr_o = tag_apb_pslverr;

    for (int i = 0; i < NrRequestors; i++) begin
      apb_pready_o |= ucache_apb_pready[i];
      apb_prdata_o |= ucache_apb_prdata[i] & {BpamShireApbDataWidth{ucache_apb_pready[i]}};
      apb_pslverr_o |= ucache_apb_pslverr[i];
    end
  end

  assign f1_tag_array_miss = f1_miss & ~esr_bypass_icache_i;

  icache_tag_array u_tag_array (
    .clk_i                 (clk_i),
    .rst_ni                (rst_ni),
    .f0_read_valid_i       (f0_l1_miss_req_valid_q),
    .f0_read_paddr_i       (f0_l1_miss_req_addr_q),
    .f1_read_tag_hit_o     (f1_tag_hit),
    .f0_write_early_valid_i(f0_tag_write_early_valid),
    .f0_write_valid_i      (f0_tag_write_valid),
    .f0_write_req_i        (f0_tag_write_req),
    .f1_miss_i             (f1_tag_array_miss),
    .f1_miss_idx_i         (f1_set),
    .f1_miss_way_i         (f1_write_way),
    .f0_miss_state_i       (f0_miss_state_q),
    .f0_flush_data_i       (f0_flush_data_i),
    .apb_paddr_i           (tag_apb_paddr),
    .apb_pwrite_i          (tag_apb_pwrite),
    .apb_psel_i            (tag_apb_psel),
    .apb_penable_i         (tag_apb_penable),
    .apb_pwdata_i          (tag_apb_pwdata),
    .apb_pready_o          (tag_apb_pready),
    .apb_prdata_o          (tag_apb_prdata),
    .apb_pslverr_o         (tag_apb_pslverr)
  );

  assign f0_tag_write_early_valid = f0_l2_miss_resp_valid_i | f0_data_array_resp_early_valid;
  assign f0_tag_write_valid =
      (f0_l2_miss_resp_valid_reg_q & ~f0_l2_miss_resp_err & ~esr_bypass_icache_i) | f0_da_dbe;
  always_comb begin
    f0_tag_write_req = '0;
    f0_tag_write_req.valid = ~f0_da_dbe;
    f0_tag_write_req.idx = f0_paddr_q[IcacheBlockAddrSize +: IcacheSetAddrWidth];
    f0_tag_write_req.way = f0_da_dbe ? f0_read_way_q : f0_write_way_q;
    f0_tag_write_req.data = f0_paddr_q[PaSize-1:IcacheDataAddrSize];
  end

  icache_lru_array u_lru_array (
    .clk_i                 (clk_i),
    .rst_ni                (rst_ni),
    .f0_valid_i            (f0_l1_miss_req_valid_q),
    .f0_access_set_i       (f0_set),
    .f1_victim_o           (f1_replace_way),
    .f1_update_en_early_i  (f1_lru_update_en_early),
    .f1_update_en_i        (f1_lru_update_en),
    .f1_update_set_i       (f1_set),
    .f1_update_access_way_i(f1_accessed_way)
  );

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f1_valid_q <= 1'b0;
    end else begin
      f1_valid_q <= f0_l1_miss_req_valid_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (f0_l1_miss_req_valid_q) begin
      f1_paddr_q <= f0_l1_miss_req_addr_q;
    end
  end

  assign f1_set = f1_paddr_q[IcacheBlockAddrSize +: IcacheSetAddrWidth];
  assign f1_any_tag_hit = (|f1_tag_hit) & ~esr_bypass_icache_i;
  assign f1_hit = f1_any_tag_hit & f1_valid_q;
  assign f1_miss = ~f1_any_tag_hit & f1_valid_q & (f0_miss_state_q == IcacheMissStateReady);
  assign f1_accessed_way = f1_any_tag_hit ? f1_tag_hit : f1_replace_way;
  assign f1_lru_update_en_early = f0_l1_miss_req_valid_q;
  assign f1_lru_update_en = (f1_hit | f1_miss) & ~esr_bypass_icache_i;

  icache_data_array u_data_array (
    .clk_i                      (clk_i),
    .rst_ni                     (rst_ni),
    .f1_read_req_valid_i        (f1_hit),
    .f1_read_req_way_i          (f1_read_way),
    .f1_read_req_set_i          (f1_set),
    .f0_read_resp_early_valid_o (f0_data_array_resp_early_valid),
    .f0_read_resp_valid_o       (f0_data_array_resp_valid),
    .f0_read_resp_data_o        (f0_data_array_resp_data),
    .f0_read_resp_sbe_per_block_o(f0_data_array_resp_sbe_per_block),
    .f0_read_resp_dbe_per_block_o(f0_data_array_resp_dbe_per_block),
    .f1_write_req_valid_i       (f0_l2_miss_req_acc),
    .f1_write_req_way_i         (f0_write_way_q),
    .f1_write_req_set_i         (f0_paddr_q[IcacheBlockAddrSize +: IcacheSetAddrWidth]),
    .f2_mem_req_write_o         (f2_sram_req_write_o),
    .f2_mem_req_addr_o          (f2_sram_req_addr_o),
    .f2_mem_req_valid_o         (f2_sram_req_valid_o),
    .f2_mem_req_ready_i         (f2_sram_req_ready_i),
    .f0_mem_resp_dout_i         (f0_sram_resp_dout_i),
    .f0_mem_resp_valid_i        (f0_sram_resp_valid_i),
    .f0_mem_resp_ready_o        (f0_sram_resp_ready_o)
  );

  always_comb begin
    f1_read_way = '0;
    f1_write_way = '0;
    for (int i = 0; i < IcacheWays; i++) begin
      if (f1_tag_hit[i]) begin
        f1_read_way |= i[IcacheWayAddrWidth-1:0];
      end
      if (f1_replace_way[i]) begin
        f1_write_way |= i[IcacheWayAddrWidth-1:0];
      end
    end
  end

  always_comb begin
    logic [$bits(icache_dbg_sm_t)-1:0] dbg_accum;

    dbg_accum = '0;
    for (int i = 0; i < NrRequestors; i++) begin
      dbg_accum |= ucache_dbg_sm_signals[i];
    end

    dbg_sm_signals_o = icache_dbg_sm_t'(dbg_accum);
    dbg_sm_signals_o.f1_tag_hit = f1_tag_hit;
    dbg_sm_signals_o.f0_miss_state = f0_miss_state_q;
    dbg_sm_signals_o.f0_l1_miss_req_valid = f0_l1_miss_req_valid_q;
    dbg_sm_signals_o.f0_l1_miss_req_addr = f0_l1_miss_req_addr_q[PaSize-1:IcacheBlockAddrSize];
  end

endmodule : icache_top
