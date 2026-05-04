// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Standalone-Minion I-cache micro-cache controller.
//
// Replaces: `icache_micro_cache` from etcore-soc.

module icache_micro_cache
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import minion_frontend_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
#(
  parameter int unsigned Id = 0,
  parameter int unsigned NrMinions = 8,
  localparam int unsigned NrMinionsW = (NrMinions == 1) ? 1 : $clog2(NrMinions)
) (
  input  logic                             clk_i,
  input  logic                             rst_ni,
  input  logic                             ioshire_i,
  input  icache_prefetch_conf_t            esr_prefetch_conf_i,
  input  logic                             esr_prefetch_start_i,
  output logic                             esr_prefetch_done_o,
  input  esr_mprot_t                       esr_mprot_i,
  input  tlb_entry_type                    esr_vmspagesize_i,
  input  logic                             esr_bypass_icache_i,
  input  logic                             esr_shire_coop_mode_i,
  output logic                             f0_req_ready_o,
  input  logic                             f0_req_valid_i,
  input  frontend_icache_req_t             f0_req_i,
  input  logic [NrMinionsW-1:0]            f0_req_min_id_i,
  output logic                             f4_resp_valid_o,
  output logic                             f4_resp_miss_o,
  output icache_frontend_resp_t            f4_resp_o,
  output logic                             f5_resp_fill_done_o,
  input  logic                             f0_flush_data_i,
  input  logic                             f0_l1_miss_req_ready_i,
  output logic                             f0_l1_miss_req_valid_o,
  output logic [PaSize-1:0]                f0_l1_miss_req_addr_o,
  input  logic                             f0_l1_miss_resp_early_valid_i,
  input  logic                             f0_l1_miss_resp_valid_i,
  input  logic [IcacheBlockBits-1:0]       f0_l1_miss_resp_data_i,
  input  logic                             f0_l1_miss_resp_ecc_err_i,
  input  logic                             f0_l1_miss_resp_l2_err_i,
  input  minion_satp_info                  satp_info_i [NrMinions-1:0],
  input  minion_satp_info                  matp_info_i [NrMinions-1:0],
  input  logic [NrMinions-1:0]             tlb_invalidate_i,
  output minion_ptw_req                    ptw_req_data_o,
  output logic                             ptw_req_valid_o,
  input  logic                             ptw_req_ready_i,
  output logic                             ptw_invalidate_o,
  input  logic                             ptw_resp_valid_i,
  input  minion_ptw_pte                    ptw_resp_data_i,
  input  logic [IcacheDbgUcacheAddrWidth-1:0] apb_paddr_i,
  input  logic                             apb_pwrite_i,
  input  logic                             apb_psel_i,
  input  logic                             apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0] apb_pwdata_i,
  output logic                             apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] apb_prdata_o,
  output logic                             apb_pslverr_o,
  output icache_dbg_sm_t                   dbg_sm_signals_o
);

  logic                             f1_bypass_icache_q;
  logic                             f2_bypass_icache_q;
  logic                             f3_bypass_icache_q;
  logic                             f1_is_prefetch_q;
  logic                             f2_is_prefetch_q;
  logic                             f3_is_prefetch_q;
  logic                             f2_disable_l0_access_q;
  logic [NrMinionsW-1:0]            f0_req_min_id_pipe;
  frontend_icache_req_t             f0_req_pipe;
  logic                             f0_req_pipe_valid;
  logic                             f1_valid_q;
  logic [NrMinionsW-1:0]            f1_min_id_q;
  icache_tlb_req_t                  f1_tlb_req_data;
  icache_tlb_resp_t                 f1_tlb_resp_data;
  logic                             f2_tlb_fill_done;
  logic                             f2_valid_q;
  logic [PaSize-1:0]                f2_paddr;
  /* verilator lint_off UNUSEDSIGNAL */  // Original TLB response struct includes fields only consumed by some standalone-Icache variants; this micro-cache uses the translated subset.
  icache_tlb_resp_t                 f2_tlb_resp_data_q;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [IcacheMicroTagSize-1:0]    f2_tag;
  logic                             f2_hit;
  logic                             f2_miss;
  logic [IcacheMicroCacheSize-1:0]  f2_victim;
  logic [IcacheMicroCacheSize-1:0]  f2_hit_array;
  logic [IcacheMicroCacheSize-1:0]  f3_hit_array_q;
  logic [IcacheMicroCacheSize-1:0]  f2_access_victim_array;
  logic [IcacheMicroCacheSize-1:0]  f2_write_victim_array;
  logic [IcacheMicroCacheSize-1:0]  f2_valid_hit_array;
  logic [IcacheMicroCacheSize-1:0]  f2_access_way_array;
  logic                             f3_valid_q;
  logic                             f3_hit_q;
  logic                             f3_tlb_miss_q;
  logic [IcacheBlockBits-1:0]       f3_read_data;
  logic                             f3_read_ecc_err;
  logic                             f3_read_l2_err;
  logic                             f2_page_fault;
  logic                             f3_page_fault_q;
  logic                             f2_access_fault;
  logic                             f3_access_fault_q;
  logic                             f3_cacheable_q;
  logic                             f2_bypass_hit;
  logic [IcacheDbgUtagAddrWidth-1:0] utag_apb_paddr;
  logic                             utag_apb_pwrite;
  logic                             utag_apb_psel;
  logic                             utag_apb_penable;
  logic [BpamShireApbDataWidth-1:0] utag_apb_pwdata;
  logic                             utag_apb_pready;
  logic [BpamShireApbDataWidth-1:0] utag_apb_prdata;
  logic                             utag_apb_pslverr;
  logic [IcacheDbgUdataAddrWidth-1:0] udata_err_apb_paddr;
  logic                             udata_err_apb_pwrite;
  logic                             udata_apb_psel;
  logic                             err_apb_psel;
  /* verilator lint_off UNUSEDSIGNAL */  // Preserved APB decode split mirrors the original micro-cache shell even though only the select terms feed the translated debug FSM today.
  logic                             udata_apb_penable;
  logic                             err_apb_penable;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [BpamShireApbDataWidth-1:0] udata_err_apb_pwdata;
  logic                             udata_err_apb_pready_q;
  logic [BpamShireApbDataWidth-1:0] udata_err_apb_prdata_q;
  logic                             udata_err_apb_pslverr;

  icache_prefetch_state_e           f0_prefetch_state_d;
  icache_prefetch_state_e           f0_prefetch_state_q;
  frontend_icache_req_t             f0_prefetch_req_d;
  frontend_icache_req_t             f0_prefetch_req_q;
  logic [IcachePrefetchLinesSize-1:0] f0_prefetch_lines_d;
  logic [IcachePrefetchLinesSize-1:0] f0_prefetch_lines_q;
  logic                             f0_prefetch_req_valid_d;
  logic                             f0_prefetch_req_valid_q;
  logic                             f0_prefetch_req_ready;
  logic                             f0_prefetch_req_access;
  logic                             f0_prefetch_load_new_req;
  logic                             f0_prefetch_done_d;
  icache_miss_state_e               f0_miss_state_d;
  icache_miss_state_e               f0_miss_state_q;
  logic [PaSize-1:0]                f0_miss_addr_q;
  logic [IcacheMicroCacheSize-1:0]  f0_miss_victim_q;
  logic                             f0_miss_bypass_icache_q;
  logic                             f0_miss_is_prefetch_q;
  logic                             f0_miss_disable_l0_access_q;

  icache_data_dbg_state_e           udata_dbg_state_d;
  icache_data_dbg_state_e           udata_dbg_state_q;
  logic [IcacheDbgUdataMemChunkAddrWidth-1:0] udata_dbg_mem_chunk_d;
  logic [IcacheDbgUdataMemChunkAddrWidth-1:0] udata_dbg_mem_chunk_q;
  logic [IcacheMicroCacheSize-1:0]  udata_dbg_mem_addr_dec_d;
  logic [IcacheMicroCacheSize-1:0]  udata_dbg_mem_addr_dec_q;
  logic                             udata_dbg_mem_err_d;
  logic                             udata_dbg_mem_err_q;
  logic                             udata_dbg_write_en_d;
  logic                             udata_dbg_write_en_q;
  logic [IcacheBlockBits+1:0]       udata_dbg_write_data_d;
  logic [IcacheBlockBits+1:0]       udata_dbg_write_data_q;
  logic                             udata_dbg_write_ready;
  logic                             udata_dbg_read_en_d;
  logic                             udata_dbg_read_en_q;
  logic [IcacheBlockBits+1:0]       udata_dbg_read_data;
  logic                             udata_dbg_read_ready;
  logic [BpamShireApbDataWidth-1:0] udata_err_apb_prdata_d;
  logic                             udata_err_apb_pready_d;

  frontend_icache_req_t             f1_req_q;
  logic                             f2_tlb_fill_was_pending_q;
  /* verilator lint_off UNUSEDSIGNAL */  // The translated request struct preserves the original fetch-thread fielding; only the VM status and address bits are consumed in this block.
  frontend_icache_req_t             f2_req_q;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                             f2_any_tag_hit;
  logic                             f2_tag_array_miss;
  logic                             f0_tag_array_wr_early_valid;
  logic                             f0_tag_array_wr_valid;
  logic [IcacheMicroTagSize-1:0]    f0_tag_array_wr_tag;
  icache_pma_resp_t                 f2_pma_resp_data;
  logic                             bypass_en;
  logic                             bypass_valid_d;
  logic                             bypass_valid_q;
  logic [IcacheMicroTagSize-1:0]    bypass_tag_q;
  logic [IcacheBlockBits-1:0]       bypass_data_q;
  logic                             bypass_ecc_err_q;
  logic                             bypass_l2_err_q;
  logic                             f2_resp_fill_done;
  logic [IcacheChunkAddrSize-1:0]   f3_data_chunk_q;
  logic [IcacheBlockBits+1:0]       f3_data_array_rd_data;
  logic                             f0_data_array_wr_en;
  logic                             f0_data_array_wr_en_early;
  logic [IcacheBlockBits+1:0]       f0_data_array_wr_data;
  logic                             f3_resp_valid;
  logic                             f3_resp_miss;
  icache_frontend_resp_t            f3_resp;
  logic                             f3_resp_fill_done_q;
  logic                             f4_resp_fill_done_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f0_prefetch_state_q <= IcachePrefetchStateIdle;
      f0_prefetch_req_valid_q <= 1'b0;
      esr_prefetch_done_o <= 1'b1;
      f0_miss_state_q <= IcacheMissStateReady;
    end else begin
      f0_prefetch_state_q <= f0_prefetch_state_d;
      if (f0_prefetch_load_new_req) begin
        f0_prefetch_req_q <= f0_prefetch_req_d;
        f0_prefetch_lines_q <= f0_prefetch_lines_d;
      end
      f0_prefetch_req_valid_q <= f0_prefetch_req_valid_d;
      esr_prefetch_done_o <= f0_prefetch_done_d;
      f0_miss_state_q <= f0_miss_state_d;
      if (f2_miss) begin
        f0_miss_addr_q <= f2_paddr;
        f0_miss_victim_q <= f2_victim;
        f0_miss_bypass_icache_q <= f2_bypass_icache_q;
        f0_miss_is_prefetch_q <= f2_is_prefetch_q;
        f0_miss_disable_l0_access_q <= f2_disable_l0_access_q;
      end
    end
  end

  always_comb begin
    f0_prefetch_state_d = f0_prefetch_state_q;
    f0_prefetch_load_new_req = 1'b0;
    f0_prefetch_done_d = esr_prefetch_done_o;

    if (esr_prefetch_start_i) begin
      f0_prefetch_state_d = IcachePrefetchStateRequest;
      f0_prefetch_load_new_req = 1'b1;
      f0_prefetch_done_d = 1'b1;
    end else if (f0_prefetch_state_q == IcachePrefetchStateRequest) begin
      f0_prefetch_done_d = 1'b0;
      if (f0_prefetch_req_ready) begin
        f0_prefetch_state_d = IcachePrefetchStateCheckHit;
      end
    end else if (f0_prefetch_state_q == IcachePrefetchStateCheckHit) begin
      if (f2_valid_q && f2_is_prefetch_q) begin
        if (f2_miss) begin
          f0_prefetch_state_d = IcachePrefetchStateFillWait;
        end else begin
          f0_prefetch_state_d = IcachePrefetchStateRequest;
        end
      end
    end else if (f0_prefetch_state_q == IcachePrefetchStateFillWait) begin
      if (f0_l1_miss_resp_valid_i) begin
        if (f0_prefetch_lines_q == '0) begin
          f0_prefetch_state_d = IcachePrefetchStateIdle;
          f0_prefetch_done_d = 1'b1;
        end else begin
          f0_prefetch_state_d = IcachePrefetchStateRequest;
          f0_prefetch_load_new_req = 1'b1;
        end
      end
    end
  end

  always_comb begin
    f0_prefetch_req_d = '0;

    if (esr_prefetch_start_i) begin
      f0_prefetch_req_d.vm_status.prv = esr_prefetch_conf_i.prv;
      f0_prefetch_req_d.addr[VaSize-1:6] = esr_prefetch_conf_i.start_addr;
      f0_prefetch_req_d.addr[VaSizeExt-1] = esr_prefetch_conf_i.start_addr[VaSize-1];
      f0_prefetch_lines_d = esr_prefetch_conf_i.num_lines;
    end else begin
      f0_prefetch_req_d.vm_status.prv = f0_prefetch_req_q.vm_status.prv;
      f0_prefetch_req_d.addr[VaSizeExt-1:6] = f0_prefetch_req_q.addr[VaSizeExt-1:6] + 1'b1;
      f0_prefetch_lines_d = f0_prefetch_lines_q - 1'b1;
    end

    f0_prefetch_req_valid_d = (f0_prefetch_state_d == IcachePrefetchStateRequest);
  end

  assign f0_prefetch_req_ready = ~f0_req_valid_i;
  assign f0_prefetch_req_access = f0_prefetch_req_valid_q & f0_prefetch_req_ready;
  assign f0_req_ready_o = 1'b1;

  always_comb begin
    f0_miss_state_d = f0_miss_state_q;
    if (f0_miss_state_q == IcacheMissStateReady) begin
      if (f2_miss) begin
        f0_miss_state_d = IcacheMissStateRequest;
      end
    end else if (f0_miss_state_q == IcacheMissStateRequest) begin
      if (f0_l1_miss_req_ready_i) begin
        f0_miss_state_d = IcacheMissStateFillWait;
      end
    end else begin
      if (f0_l1_miss_resp_valid_i) begin
        f0_miss_state_d = IcacheMissStateReady;
      end
    end
  end

  assign f0_l1_miss_req_valid_o = (f0_miss_state_q == IcacheMissStateRequest);
  assign f0_l1_miss_req_addr_o = f0_miss_addr_q;

  assign udata_err_apb_paddr = apb_paddr_i[IcacheDbgUdataAddrWidth-1:0];
  assign udata_err_apb_pwrite = apb_pwrite_i;
  assign udata_apb_psel = apb_psel_i && !apb_paddr_i[IcacheDbgUcacheAddrWidth-1];
  assign err_apb_psel = apb_psel_i
                      && apb_paddr_i[IcacheDbgUcacheAddrWidth-1]
                      && apb_paddr_i[IcacheDbgUdataMemChunkAddrWidth-1];
  assign udata_apb_penable = apb_penable_i && !apb_paddr_i[IcacheDbgUcacheAddrWidth-1];
  assign err_apb_penable = apb_penable_i
                         && apb_paddr_i[IcacheDbgUcacheAddrWidth-1]
                         && apb_paddr_i[IcacheDbgUdataMemChunkAddrWidth-1];
  assign udata_err_apb_pwdata = apb_pwdata_i;
  assign utag_apb_paddr = apb_paddr_i[IcacheDbgUdataAddrWidth-1:IcacheDbgUdataMemChunkAddrWidth];
  assign utag_apb_pwrite = apb_pwrite_i;
  assign utag_apb_psel = apb_psel_i
                       && apb_paddr_i[IcacheDbgUcacheAddrWidth-1]
                       && !apb_paddr_i[IcacheDbgUdataMemChunkAddrWidth-1];
  assign utag_apb_penable = apb_penable_i
                          && apb_paddr_i[IcacheDbgUcacheAddrWidth-1]
                          && !apb_paddr_i[IcacheDbgUdataMemChunkAddrWidth-1];
  assign utag_apb_pwdata = apb_pwdata_i;

  assign apb_pready_o = udata_err_apb_pready_q | utag_apb_pready;
  assign apb_prdata_o =
      (udata_err_apb_prdata_q & {BpamShireApbDataWidth{udata_err_apb_pready_q}}) |
      (utag_apb_prdata & {BpamShireApbDataWidth{utag_apb_pready}});
  assign apb_pslverr_o = udata_err_apb_pslverr | utag_apb_pslverr;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      udata_dbg_state_q <= IcacheDataDbgStateIdle;
      udata_dbg_write_en_q <= 1'b0;
      udata_dbg_read_en_q <= 1'b0;
      udata_err_apb_pready_q <= 1'b0;
    end else begin
      udata_dbg_state_q <= udata_dbg_state_d;
      if (udata_dbg_read_en_d) begin
        udata_dbg_mem_chunk_q <= udata_dbg_mem_chunk_d;
        udata_dbg_mem_addr_dec_q <= udata_dbg_mem_addr_dec_d;
        udata_dbg_mem_err_q <= udata_dbg_mem_err_d;
      end
      udata_dbg_write_en_q <= udata_dbg_write_en_d;
      if (udata_dbg_write_en_d) begin
        udata_dbg_write_data_q <= udata_dbg_write_data_d;
      end
      udata_dbg_read_en_q <= udata_dbg_read_en_d;
      if (udata_err_apb_pready_d && !udata_err_apb_pwrite) begin
        udata_err_apb_prdata_q <= udata_err_apb_prdata_d;
      end
      udata_err_apb_pready_q <= udata_err_apb_pready_d;
    end
  end

  always_comb begin
    udata_dbg_state_d = udata_dbg_state_q;
    udata_dbg_write_en_d = udata_dbg_write_en_q;
    udata_dbg_read_en_d = udata_dbg_read_en_q;
    udata_err_apb_pready_d = udata_err_apb_pready_q;

    if (udata_dbg_state_q == IcacheDataDbgStateIdle) begin
      if (udata_apb_psel || err_apb_psel) begin
        udata_dbg_state_d = IcacheDataDbgStateWaitReadReady;
        udata_dbg_read_en_d = 1'b1;
      end
    end else if (udata_dbg_state_q == IcacheDataDbgStateWaitReadReady) begin
      if (udata_dbg_read_ready) begin
        udata_dbg_read_en_d = 1'b0;
        if (udata_err_apb_pwrite) begin
          udata_dbg_state_d = IcacheDataDbgStateWaitWriteReady;
          udata_dbg_write_en_d = 1'b1;
        end else begin
          udata_dbg_state_d = IcacheDataDbgStateDone;
          udata_err_apb_pready_d = 1'b1;
        end
      end
    end else if (udata_dbg_state_q == IcacheDataDbgStateWaitWriteReady) begin
      if (udata_dbg_write_ready) begin
        udata_dbg_write_en_d = 1'b0;
        udata_dbg_state_d = IcacheDataDbgStateDone;
        udata_err_apb_pready_d = 1'b1;
      end
    end else if (udata_dbg_state_q == IcacheDataDbgStateDone) begin
      udata_dbg_state_d = IcacheDataDbgStateIdle;
      udata_err_apb_pready_d = 1'b0;
    end
  end

  always_comb begin
    udata_dbg_mem_chunk_d = udata_err_apb_paddr[IcacheDbgUdataMemChunkAddrWidth-1:0];
    udata_dbg_mem_addr_dec_d = '0;
    udata_dbg_mem_addr_dec_d[udata_err_apb_paddr[IcacheDbgUdataAddrWidth-1 -: IcacheMicroCacheAdWidth]] = 1'b1;
    udata_dbg_mem_err_d = err_apb_psel;
  end

  always_comb begin
    udata_dbg_write_data_d = udata_dbg_read_data;
    if (udata_dbg_mem_err_q) begin
      udata_dbg_write_data_d[IcacheBlockBits +: 2] = udata_err_apb_pwdata[1:0];
    end else begin
      udata_dbg_write_data_d[udata_dbg_mem_chunk_q * BpamShireApbDataWidth +: BpamShireApbDataWidth] = udata_err_apb_pwdata;
    end
  end

  assign udata_err_apb_prdata_d =
      udata_dbg_mem_err_q
          ? BpamShireApbDataWidth'(udata_dbg_read_data[IcacheBlockBits +: 2])
          : udata_dbg_read_data[udata_dbg_mem_chunk_q * BpamShireApbDataWidth +: BpamShireApbDataWidth];

  assign udata_err_apb_pslverr = 1'b0;

  always_comb begin
    dbg_sm_signals_o = '0;
    dbg_sm_signals_o.ucache_f0_prefetch_state = f0_prefetch_state_q;
    dbg_sm_signals_o.ucache_f0_miss_state[Id] = f0_miss_state_q;
    dbg_sm_signals_o.ucache_f2_valid[Id] = f2_valid_q;
    dbg_sm_signals_o.ucache_f2_hit_array[Id] = f2_hit_array;
    dbg_sm_signals_o.ucache_f2_paddr[Id] = f2_paddr[PaSize-1:6];
  end

  assign f0_req_min_id_pipe = f0_prefetch_req_access ? '0 : f0_req_min_id_i;
  assign f0_req_pipe = f0_prefetch_req_access ? f0_prefetch_req_q : f0_req_i;
  assign f0_req_pipe_valid = f0_req_valid_i || f0_prefetch_req_valid_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f1_valid_q <= 1'b0;
    end else begin
      f1_valid_q <= f0_req_pipe_valid;
      if (f0_req_pipe_valid) begin
        f1_min_id_q <= f0_req_min_id_pipe;
        f1_req_q <= f0_req_pipe;
        f1_bypass_icache_q <= esr_bypass_icache_i;
        f1_is_prefetch_q <= f0_prefetch_req_access;
      end
    end
  end

  always_comb begin
    f1_tlb_req_data = '0;
    f1_tlb_req_data.status = f1_req_q.vm_status;
    f1_tlb_req_data.vpn = f1_req_q.addr[VaSize-1:VaUntransSize];
    f1_tlb_req_data.passthrough = 1'b0;
    f1_tlb_req_data.msb_err = f1_req_q.addr[VaSizeExt-1] ^ f1_req_q.addr[VaSize-1];
  end

  icache_tlb_array #(
    .Entries   (IcacheTlbEntries),
    .NrMinions (NrMinions)
  ) u_tlb_array (
    .clk_i               (clk_i),
    .rst_ni              (rst_ni),
    .esr_vmspagesize_i   (esr_vmspagesize_i),
    .esr_shire_coop_mode_i(esr_shire_coop_mode_i),
    .req_min_id_i        (f1_min_id_q),
    .req_data_i          (f1_tlb_req_data),
    .req_valid_i         (f1_valid_q),
    .resp_data_o         (f1_tlb_resp_data),
    .satp_info_i         (satp_info_i),
    .matp_info_i         (matp_info_i),
    .tlb_invalidate_i    (tlb_invalidate_i),
    .ptw_req_data_o      (ptw_req_data_o),
    .ptw_req_valid_o     (ptw_req_valid_o),
    .ptw_req_ready_i     (ptw_req_ready_i),
    .ptw_invalidate_o    (ptw_invalidate_o),
    .ptw_resp_valid_i    (ptw_resp_valid_i),
    .ptw_resp_data_i     (ptw_resp_data_i)
  );

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f2_tlb_fill_was_pending_q <= 1'b0;
    end else begin
      f2_tlb_fill_was_pending_q <= f1_tlb_resp_data.fill_pending;
    end
  end

  assign f2_tlb_fill_done = f2_tlb_fill_was_pending_q && !f1_tlb_resp_data.fill_pending;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f2_valid_q <= 1'b0;
      f2_tlb_resp_data_q <= '0;
    end else begin
      f2_valid_q <= f1_valid_q;
      if (f1_valid_q) begin
        f2_req_q <= f1_req_q;
        f2_tlb_resp_data_q <= f1_tlb_resp_data;
        f2_bypass_icache_q <= f1_bypass_icache_q;
        f2_is_prefetch_q <= f1_is_prefetch_q;
      end
    end
  end

  assign f2_paddr = {f2_tlb_resp_data_q.ppn, f2_req_q.addr[PaUntransSize-1:0]};
  assign f2_page_fault = f2_tlb_resp_data_q.xcpt_if;
  assign f2_access_fault =
      (~f2_tlb_resp_data_q.miss & f2_pma_resp_data.access_fault) | f2_tlb_resp_data_q.access_fault;
  assign f2_disable_l0_access_q = f2_bypass_icache_q || f2_is_prefetch_q;

  assign f2_tag = f2_paddr[PaSize-1:IcacheBlockAddrSize];
  assign f2_tag_array_miss = f2_miss & ~f2_disable_l0_access_q;
  assign f0_tag_array_wr_early_valid = f0_l1_miss_resp_early_valid_i;
  assign f0_tag_array_wr_valid = f0_l1_miss_resp_valid_i & ~f0_miss_disable_l0_access_q;
  assign f0_tag_array_wr_tag = f0_miss_addr_q[PaSize-1:IcacheBlockAddrSize];

  icache_micro_tag_array u_micro_tag_array (
    .clk_i,
    .rst_ni,
    .f2_rd_tag_i          (f2_tag),
    .f2_rd_hit_array_o    (f2_hit_array),
    .f0_wr_early_valid_i  (f0_tag_array_wr_early_valid),
    .f0_wr_valid_i        (f0_tag_array_wr_valid),
    .f0_wr_victim_i       (f0_miss_victim_q),
    .f0_wr_tag_i          (f0_tag_array_wr_tag),
    .f2_miss_i            (f2_tag_array_miss),
    .f2_miss_victim_i     (f2_victim),
    .f0_miss_state_i      (f0_miss_state_q),
    .f0_flush_data_i      (f0_flush_data_i),
    .apb_paddr_i          (utag_apb_paddr),
    .apb_pwrite_i         (utag_apb_pwrite),
    .apb_psel_i           (utag_apb_psel),
    .apb_penable_i        (utag_apb_penable),
    .apb_pwdata_i         (utag_apb_pwdata),
    .apb_pready_o         (utag_apb_pready),
    .apb_prdata_o         (utag_apb_prdata),
    .apb_pslverr_o        (utag_apb_pslverr)
  );

  assign f2_any_tag_hit = f2_is_prefetch_q ? 1'b0
                        : (f2_bypass_icache_q ? f2_bypass_hit : (|f2_hit_array));

  assign f2_hit = f2_any_tag_hit && f2_valid_q;
  assign f2_miss = !f2_any_tag_hit && f2_valid_q &&
                   !f2_tlb_resp_data_q.miss &&
                   !f2_page_fault &&
                   !f2_access_fault &&
                   (f0_miss_state_q == IcacheMissStateReady);

  assign f2_valid_hit_array = f2_hit_array & {IcacheMicroCacheSize{f2_hit}};
  assign f2_access_victim_array = f2_victim & {IcacheMicroCacheSize{f2_miss}};
  assign f2_write_victim_array = f0_miss_victim_q & {IcacheMicroCacheSize{f0_l1_miss_resp_valid_i}};
  assign f2_access_way_array =
      (f2_valid_hit_array | f2_access_victim_array | f2_write_victim_array) &
      {IcacheMicroCacheSize{~f2_disable_l0_access_q}};

  lru_sel #(
    .NumClients(IcacheMicroCacheSize)
  ) u_lru (
    .clk_i,
    .rst_ni,
    .access_entry_i(f2_access_way_array),
    .victim_o      (f2_victim)
  );

  icache_pma_unit u_pma_unit (
    .ioshire_i    (ioshire_i),
    .paddr_i      (f2_paddr),
    .mprot_i      (esr_mprot_i),
    .ptw_prv_i    (f2_req_q.vm_status.prv),
    .resp_data_o  (f2_pma_resp_data)
  );

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      bypass_valid_q <= 1'b0;
    end else begin
      bypass_valid_q <= bypass_valid_d;
      if (bypass_en) begin
        bypass_tag_q <= f0_miss_addr_q[PaSize-1:IcacheBlockAddrSize];
        bypass_data_q <= f0_l1_miss_resp_data_i;
        bypass_ecc_err_q <= f0_l1_miss_resp_ecc_err_i;
        bypass_l2_err_q <= f0_l1_miss_resp_l2_err_i;
      end
    end
  end

  assign bypass_en = f0_l1_miss_resp_valid_i & f0_miss_bypass_icache_q & ~f0_miss_is_prefetch_q;

  always_comb begin
    bypass_valid_d = bypass_valid_q;
    if (bypass_en) begin
      bypass_valid_d = 1'b1;
    end else if (!f2_bypass_icache_q) begin
      bypass_valid_d = 1'b0;
    end
  end

  assign f2_bypass_hit = bypass_valid_q & ~bypass_en & (bypass_tag_q == f2_tag);
  assign f2_resp_fill_done = f2_tlb_fill_done | f0_l1_miss_resp_valid_i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f3_valid_q <= 1'b0;
      f3_hit_q <= 1'b0;
      f3_tlb_miss_q <= 1'b0;
    end else begin
      f3_valid_q <= f2_valid_q;
      if (f2_valid_q) begin
        f3_data_chunk_q <= f2_paddr[IcacheBlockAddrSize-1 -: IcacheChunkAddrSize];
        f3_hit_array_q <= f2_hit_array;
        f3_hit_q <= f2_hit;
        f3_tlb_miss_q <= f2_tlb_resp_data_q.miss;
        f3_page_fault_q <= f2_page_fault;
        f3_access_fault_q <= f2_access_fault;
        f3_bypass_icache_q <= f2_bypass_icache_q;
        f3_is_prefetch_q <= f2_is_prefetch_q;
      end
      if (f2_hit) begin
        f3_cacheable_q <= f2_pma_resp_data.cacheable;
      end
      f3_resp_fill_done_q <= f2_resp_fill_done;
    end
  end

  assign f0_data_array_wr_en = f0_l1_miss_resp_valid_i & ~f0_miss_disable_l0_access_q;
  assign f0_data_array_wr_en_early = f0_l1_miss_resp_early_valid_i;
  assign f0_data_array_wr_data = {f0_l1_miss_resp_l2_err_i, f0_l1_miss_resp_ecc_err_i, f0_l1_miss_resp_data_i};

  icache_micro_data_array #(
    .Width   (IcacheBlockBits + 2),
    .Entries (IcacheMicroCacheSize)
  ) u_micro_data_array (
    .clk_i,
    .f3_rd_en_i         (f3_valid_q),
    .f3_rd_addr_dec_i   (f3_hit_array_q),
    .f3_rd_data_o       (f3_data_array_rd_data),
    .f0_wr_en_i         (f0_data_array_wr_en),
    .f0_wr_en_early_i   (f0_data_array_wr_en_early),
    .f0_wr_addr_dec_i   (f0_miss_victim_q),
    .f0_wr_data_i       (f0_data_array_wr_data),
    .dbg_addr_dec_i     (udata_dbg_mem_addr_dec_q),
    .dbg_write_en_i     (udata_dbg_write_en_q),
    .dbg_write_en_early_i(udata_dbg_write_en_d),
    .dbg_write_data_i   (udata_dbg_write_data_q),
    .dbg_write_ready_o  (udata_dbg_write_ready),
    .dbg_read_en_i      (udata_dbg_read_en_q),
    .dbg_read_data_o    (udata_dbg_read_data),
    .dbg_read_ready_o   (udata_dbg_read_ready)
  );

  assign f3_read_data = f3_data_array_rd_data[IcacheBlockBits-1:0];
  assign f3_read_ecc_err = f3_data_array_rd_data[IcacheBlockBits];
  assign f3_read_l2_err = f3_data_array_rd_data[IcacheBlockBits+1];

  assign f3_resp_valid = f3_valid_q & ~f3_is_prefetch_q;
  assign f3_resp_miss = ~f3_hit_q | f3_tlb_miss_q;

  always_comb begin
    f3_resp = '0;
    f3_resp.data = f3_bypass_icache_q
                 ? bypass_data_q[f3_data_chunk_q * FeFetchReadSize +: FeFetchReadSize]
                 : f3_read_data[f3_data_chunk_q * FeFetchReadSize +: FeFetchReadSize];
    f3_resp.cacheable = f3_cacheable_q;
    f3_resp.page_fault = f3_page_fault_q;
    f3_resp.access_fault = f3_access_fault_q;
    f3_resp.ecc_err = ~f3_resp_miss &
                      (f3_bypass_icache_q ? bypass_ecc_err_q : f3_read_ecc_err);
    f3_resp.bus_err = ~f3_resp_miss &
                      (f3_bypass_icache_q ? bypass_l2_err_q : f3_read_l2_err);
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f4_resp_valid_o <= 1'b0;
      f4_resp_miss_o <= 1'b1;
      f5_resp_fill_done_o <= 1'b0;
    end else begin
      f4_resp_valid_o <= f3_resp_valid;
      if (f3_resp_valid) begin
        f4_resp_miss_o <= f3_resp_miss;
        f4_resp_o <= f3_resp;
      end
      f4_resp_fill_done_q <= f3_resp_fill_done_q;
      f5_resp_fill_done_o <= f4_resp_fill_done_q;
    end
  end

endmodule : icache_micro_cache
