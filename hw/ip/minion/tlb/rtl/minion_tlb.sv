// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion TLB.
//
// Replaces: `tlb_top` from etcore-soc.

module minion_tlb
  import dft_pkg::*;
  import minion_pkg::*;
  import tlb_pkg::*;
#(
  parameter int unsigned Entries = 8,
  parameter int unsigned NrMinions = 1,
  localparam int unsigned NrMinionsW = (NrMinions == 1) ? 1 : $clog2(NrMinions)
) (
  input  logic                             clk_i,
  input  logic                             rst_ni,
  input  dft_t                             dft_i,
  input  tlb_entry_type                    vmspagesize_i,
  input  logic                             coop_mode_i,
  input  logic [NrMinionsW-1:0]            req_min_id_i,
  input  tlb_req                           req_data_i,
  input  logic                             req_valid_i,
  output tlb_resp                          resp_data_o,
  input  minion_satp_info                   satp_info_i [NrMinions-1:0],
  input  minion_satp_info                   matp_info_i [NrMinions-1:0],
  input  logic [NrMinions-1:0]             tlb_invalidate_i,
  output minion_ptw_req                    ptw_req_data_o,
  output logic                             ptw_req_valid_o,
  input  logic                             ptw_req_ready_i,
  output logic                             ptw_invalidate_o,
  input  minion_ptw_pte                    ptw_resp_data_i,
  input  logic                             ptw_resp_valid_i,
  output logic                             vm_enabled_o
);

  logic unused_dft;
  assign unused_dft = &{1'b0, dft_i.scanmode, dft_i.scan_reset_n,
                        dft_i.sram_clk_override, dft_i.ram_rei, dft_i.ram_wei};

  localparam int unsigned TlbCacheIdxSz = $clog2(Entries);
  localparam int unsigned EntriesPerMin = Entries / NrMinions;
  localparam int unsigned TlbCacheIdxSzPerMin = $clog2(EntriesPerMin);
  localparam int unsigned TagSz = VaTransSize - TlbCacheIdxSzPerMin;
  localparam int unsigned TlbCacheDataWidth = TagSz + $bits(tlb_entry_data_t);

  /* verilator lint_off UNUSED */  // Page-size-dependent tag/index generation intentionally ignores VPN bits for larger pages.
  /* verilator lint_off WIDTHTRUNC */  // Original concatenations over-generate a leading zero/minion-id bit when NrMinions=1; the assigned width intentionally drops it.
  /* verilator lint_off WIDTHEXPAND */  // Original tag concatenations intentionally zero-fill the upper bits for larger page sizes.
  function automatic logic [TagSz-1:0] tlb_tag(
    input logic [VaSize-1:VaUntransSize] vpn_i,
    input tlb_entry_type                 typ_i,
    input logic                          coop_mode_sel_i
  );
    logic [TagSz-1:0] tag;

    tag = '0;
    if (coop_mode_sel_i) begin
      unique case (typ_i)
        tlb_entry_type_2M: begin
          tag = {
            1'b0,
            vpn_i[VaSize-1:(VaUntransSize + PtwPgIdxSz + TlbCacheIdxSz)],
            {PtwPgIdxSz{1'b0}}
          };
        end
        tlb_entry_type_1G: begin
          tag = {
            1'b0,
            vpn_i[VaSize-1:(VaUntransSize + (2 * PtwPgIdxSz) + TlbCacheIdxSz)],
            {(2 * PtwPgIdxSz){1'b0}}
          };
        end
        default: begin
          tag = {1'b0, vpn_i[VaSize-1:(VaUntransSize + TlbCacheIdxSz)]};
        end
      endcase
    end else begin
      unique case (typ_i)
        tlb_entry_type_2M: begin
          tag = {
            vpn_i[VaSize-1:(VaUntransSize + PtwPgIdxSz + TlbCacheIdxSzPerMin)],
            {PtwPgIdxSz{1'b0}}
          };
        end
        tlb_entry_type_1G: begin
          tag = {
            vpn_i[VaSize-1:(VaUntransSize + (2 * PtwPgIdxSz) + TlbCacheIdxSzPerMin)],
            {(2 * PtwPgIdxSz){1'b0}}
          };
        end
        default: begin
          tag = vpn_i[VaSize-1:(VaUntransSize + TlbCacheIdxSzPerMin)];
        end
      endcase
    end

    return tag;
  endfunction

  function automatic logic [TlbCacheIdxSz-1:0] tlb_index(
    input logic [NrMinionsW-1:0]            min_id_i,
    input logic [VaSize-1:VaUntransSize]    vpn_i,
    input tlb_entry_type                    typ_i,
    input logic                             coop_mode_sel_i
  );
    logic [TlbCacheIdxSz-1:0] index;

    if (coop_mode_sel_i) begin
      unique case (typ_i)
        tlb_entry_type_2M: begin
          index = vpn_i[(VaUntransSize + PtwPgIdxSz) +: TlbCacheIdxSz];
        end
        tlb_entry_type_1G: begin
          index = vpn_i[(VaUntransSize + (2 * PtwPgIdxSz)) +: TlbCacheIdxSz];
        end
        default: begin
          index = vpn_i[VaUntransSize +: TlbCacheIdxSz];
        end
      endcase
    end else begin
      unique case (typ_i)
        tlb_entry_type_2M: begin
          index = {min_id_i, vpn_i[(VaUntransSize + PtwPgIdxSz) +: TlbCacheIdxSzPerMin]};
        end
        tlb_entry_type_1G: begin
          index = {min_id_i, vpn_i[(VaUntransSize + (2 * PtwPgIdxSz)) +: TlbCacheIdxSzPerMin]};
        end
        default: begin
          index = {min_id_i, vpn_i[VaUntransSize +: TlbCacheIdxSzPerMin]};
        end
      endcase
    end

    return index;
  endfunction
  /* verilator lint_on WIDTHTRUNC */
  /* verilator lint_on WIDTHEXPAND */
  /* verilator lint_on UNUSED */

  logic                         coop_mode_q;
  logic                         tlb_invalidate_q;
  logic [NrMinions-1:0]         tlb_invalidated_d;
  logic [NrMinions-1:0]         tlb_invalidated_q;
  logic                         ptw_req_invalidated;
  logic [1:0]                   privilege_mode;
  logic [TagSz-1:0]             req_tag;
  logic                         priv_mode_uses_vm;
  logic [CsrSatpModeSz-1:0]     vm_mode;
  logic [PaTransSize-1:0]       vm_ppn;
  logic                         invalid_address;
  tlb_state_e                   state_d;
  /* verilator lint_off UNOPTFLAT */  // Clock-gated FSM enable depends on TLB miss state, matching the original feedback structure.
  tlb_state_e                   state_q;
  /* verilator lint_on UNOPTFLAT */
  logic [NrMinionsW-1:0]        req_min_id_d;
  logic [NrMinionsW-1:0]        req_min_id_q;
  logic [NrMinions-1:0]         req_min_mask;
  logic [VaSize-1:VaUntransSize] req_vpn_d;
  logic [VaSize-1:VaUntransSize] req_vpn_q;
  logic [TagSz-1:0]             req_tag_d;
  logic [TagSz-1:0]             req_tag_q;
  logic                         rf_latch_wr_en;
  logic                         rf_latch_wr_en_early;
  logic [TlbCacheIdxSz-1:0]     tlb_cache_waddr;
  logic                         tlb_cache_sr_rf_wdata;
  logic                         tlb_cache_sw_rf_wdata;
  logic                         tlb_cache_sx_rf_wdata;
  tlb_entry_data_t              tlb_cache_wdata;
  logic [TlbCacheIdxSz-1:0]     tlb_cache_raddr;
  logic [TagSz-1:0]             tlb_cache_rtag;
  tlb_entry_data_t              tlb_cache_rdata;
  logic [TlbCacheDataWidth-1:0] tlb_cache_wdata_flat;
  logic [TlbCacheDataWidth-1:0] tlb_cache_rdata_flat;
  logic [Entries-1:0]           tlb_cache_valid_q;
  logic                         tlb_cache_hit;
  logic                         tlb_miss;
  logic                         priv_ok;
  logic                         w_perm_ok;
  logic                         r_perm_ok;
  logic                         x_perm_ok;
  logic                         tlb_xcpt_accessed;
  logic                         tlb_xcpt_dirty;
  minion_ptw_req                ptw_req_data_d;
  minion_ptw_req                ptw_req_data_q;
  logic                         ptw_req_valid_d;
  logic                         ptw_req_valid_q;
  logic                         pte_is_leaf;
  /* verilator lint_off UNUSED */  // PTW response carries reserved/global bits that belong to other shared consumers.
  minion_ptw_pte                ptw_resp_data_q;
  /* verilator lint_on UNUSED */
  logic                         ptw_resp_valid_q;
  logic                         ff_en;
  logic                         en_clock_fsm;

  assign ff_en = (coop_mode_q ^ coop_mode_i) |
                 (tlb_invalidate_q ^ (|tlb_invalidate_i)) |
                 (|(tlb_invalidated_q ^ tlb_invalidated_d));

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      coop_mode_q       <= 1'b0;
      tlb_invalidate_q  <= 1'b0;
      tlb_invalidated_q <= '0;
    end else if (ff_en) begin
      coop_mode_q       <= coop_mode_i;
      tlb_invalidate_q  <= |tlb_invalidate_i;
      tlb_invalidated_q <= tlb_invalidated_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (ptw_resp_valid_i) begin
      ptw_resp_data_q <= ptw_resp_data_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      ptw_resp_valid_q <= 1'b0;
    end else begin
      ptw_resp_valid_q <= ptw_resp_valid_i;
    end
  end

  assign privilege_mode =
    (req_data_i.status.mprv && !req_data_i.instruction) ? req_data_i.status.mpp : req_data_i.status.prv;
  assign req_tag = tlb_tag(req_data_i.vpn, vmspagesize_i, coop_mode_q);
  assign priv_mode_uses_vm = (req_data_i.status.mprv && !req_data_i.instruction) || !req_data_i.status.debug;
  assign vm_mode = (privilege_mode == PrvM) ? matp_info_i[req_min_id_i].mode : satp_info_i[req_min_id_i].mode;
  assign vm_ppn = (privilege_mode == PrvM) ? matp_info_i[req_min_id_i].ppn  : satp_info_i[req_min_id_i].ppn;
  assign vm_enabled_o = (vm_mode != CsrSatpModeBare) && priv_mode_uses_vm && !req_data_i.passthrough;

  always_comb begin
    if (vm_mode == CsrSatpModeSv48) begin
      invalid_address = req_data_i.msb_err;
    end else if (vm_mode == CsrSatpModeSv39) begin
      invalid_address = req_data_i.msb_err ||
                        !((~|req_data_i.vpn[VaSize-1:Va39Msb]) || (&req_data_i.vpn[VaSize-1:Va39Msb]));
    end else begin
      invalid_address = 1'b0;
    end
  end

  always_comb begin
    tlb_invalidated_d = tlb_invalidated_q;
    if ((state_q == TlbStateReady) || ((state_q == TlbStateWait) && ptw_resp_valid_q)) begin
      tlb_invalidated_d = '0;
    end
    tlb_invalidated_d |= tlb_invalidate_i;
  end

  always_comb begin
    req_min_mask = '0;
    req_min_mask[req_min_id_q] = 1'b1;
  end

  assign ptw_req_invalidated =
    (coop_mode_q & (|tlb_invalidated_q)) |
    (~coop_mode_q & (|(req_min_mask & tlb_invalidated_q)));

  assign en_clock_fsm = (state_q != TlbStateReady) || (req_valid_i && tlb_miss) || !rst_ni;

  always_comb begin
    state_d = state_q;
    req_min_id_d = req_min_id_q;
    req_vpn_d = req_vpn_q;
    req_tag_d = req_tag_q;
    ptw_req_data_d = ptw_req_data_q;
    ptw_req_valid_d = ptw_req_valid_q;

    unique case (state_q)
      TlbStateReady: begin
        if (req_valid_i && tlb_miss && !invalid_address) begin
          state_d = TlbStateRequest;
          req_min_id_d = req_min_id_i;
          req_vpn_d = req_data_i.vpn;
          req_tag_d = req_tag;
          ptw_req_data_d.satp_mode = vm_mode;
          ptw_req_data_d.satp_ppn = vm_ppn;
          ptw_req_data_d.prv = req_data_i.status.prv;
          ptw_req_data_d.addr = req_data_i.vpn;
          ptw_req_valid_d = 1'b1;
        end
      end

      TlbStateRequest: begin
        if (ptw_req_invalidated) begin
          state_d = TlbStateReady;
          ptw_req_valid_d = 1'b0;
        end else if (ptw_req_ready_i) begin
          state_d = TlbStateWait;
          ptw_req_valid_d = 1'b0;
        end
      end

      TlbStateWait: begin
        if (ptw_resp_valid_q) begin
          state_d = TlbStateReady;
        end
      end

      default: begin
        state_d = TlbStateReady;
        ptw_req_valid_d = 1'b0;
      end
    endcase
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      state_q         <= TlbStateReady;
      ptw_req_valid_q <= 1'b0;
    end else if (en_clock_fsm) begin
      state_q         <= state_d;
      req_min_id_q    <= req_min_id_d;
      req_vpn_q       <= req_vpn_d;
      req_tag_q       <= req_tag_d;
      ptw_req_data_q  <= ptw_req_data_d;
      ptw_req_valid_q <= ptw_req_valid_d;
    end
  end

  assign ptw_req_data_o = ptw_req_data_q;
  assign ptw_req_valid_o = ptw_req_valid_q;
  assign ptw_invalidate_o = tlb_invalidate_q;

  assign pte_is_leaf = ptw_resp_data_q.v &&
                       (ptw_resp_data_q.r || (ptw_resp_data_q.x && !ptw_resp_data_q.w)) &&
                       !ptw_resp_data_q.access_fault;
  assign rf_latch_wr_en = !ptw_req_invalidated && ptw_resp_valid_q && !ptw_resp_data_q.canceled_req;
  assign rf_latch_wr_en_early = !(|tlb_invalidated_d) && ptw_resp_valid_i && !ptw_resp_data_i.canceled_req;

  assign tlb_cache_waddr = tlb_index(req_min_id_q, req_vpn_q, vmspagesize_i, coop_mode_q);
  assign tlb_cache_sr_rf_wdata = pte_is_leaf && ptw_resp_data_q.r;
  assign tlb_cache_sw_rf_wdata = pte_is_leaf && ptw_resp_data_q.w;
  assign tlb_cache_sx_rf_wdata = pte_is_leaf && ptw_resp_data_q.x;
  assign tlb_cache_wdata = '{
    ptw_access_fault: ptw_resp_data_q.access_fault,
    expected_ex_mode: ptw_resp_data_q.rsvd_for_hw[0],
    dirty:            ptw_resp_data_q.d,
    access:           ptw_resp_data_q.a,
    sx:               tlb_cache_sx_rf_wdata,
    sw:               tlb_cache_sw_rf_wdata,
    sr:               tlb_cache_sr_rf_wdata,
    u:                ptw_resp_data_q.u,
    ppn:              ptw_resp_data_q.ppn[PaTransSize-1:0]
  };
  assign tlb_cache_wdata_flat = {req_tag_q, tlb_cache_wdata};
  assign {tlb_cache_rtag, tlb_cache_rdata} = tlb_cache_rdata_flat;

  prim_rf_1r1w_preview #(
    .Width (TlbCacheDataWidth),
    .Depth (Entries)
  ) u_tlb_cache_rf (
    .preview_clk_i       (clk_i),
    .rf_clk_i            (clk_i),
    .wr_data_en_1p_next_i(rf_latch_wr_en_early),
    .wr_en_i             (rf_latch_wr_en),
    .wr_addr_i           (tlb_cache_waddr),
    .wr_data_i           (tlb_cache_wdata_flat),
    .rd_addr_i           (tlb_cache_raddr),
    .rd_data_o           (tlb_cache_rdata_flat)
  );

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      tlb_cache_valid_q <= '0;
    end else begin
      if (ptw_resp_valid_q && !ptw_resp_data_q.canceled_req) begin
        tlb_cache_valid_q[tlb_cache_waddr] <= 1'b1;
      end

      if (coop_mode_q && (|tlb_invalidated_q)) begin
        tlb_cache_valid_q <= '0;
      end else if (!coop_mode_q) begin
        for (int unsigned i = 0; i < NrMinions; i++) begin
          if (tlb_invalidated_q[i]) begin
            tlb_cache_valid_q[i * EntriesPerMin +: EntriesPerMin] <= '0;
          end
        end
      end
    end
  end

  assign tlb_cache_raddr = tlb_index(req_min_id_i, req_data_i.vpn, vmspagesize_i, coop_mode_q);
  assign tlb_cache_hit = tlb_cache_valid_q[tlb_cache_raddr] &&
                         vm_enabled_o &&
                         (tlb_cache_rtag == req_tag) &&
                         (tlb_cache_rdata.expected_ex_mode ^ (privilege_mode != PrvM));
  assign tlb_miss = vm_enabled_o && !tlb_cache_hit;

  assign priv_ok =
    (privilege_mode == PrvU) ? tlb_cache_rdata.u :
    req_data_i.status.sum ? 1'b1 :
    !tlb_cache_rdata.u;
  assign w_perm_ok = priv_ok && tlb_cache_rdata.sw;
  assign r_perm_ok = priv_ok && (tlb_cache_rdata.sr || (req_data_i.status.mxr && tlb_cache_rdata.sx));
  assign x_perm_ok = priv_ok && tlb_cache_rdata.sx;

  assign tlb_xcpt_accessed = !tlb_cache_rdata.access;
  assign tlb_xcpt_dirty = !tlb_cache_rdata.dirty && req_data_i.store;

  always_comb begin
    resp_data_o = '0;
    resp_data_o.fill_pending = state_q != TlbStateReady;
    resp_data_o.miss = tlb_miss;
    if (!vm_enabled_o) begin
      resp_data_o.ppn = req_data_i.vpn[PaSize-1:PaUntransSize];
    end else begin
      resp_data_o.ppn = tlb_cache_rdata.ppn;
    end
    resp_data_o.xcpt_ld = (((!r_perm_ok) || tlb_xcpt_accessed) && tlb_cache_hit) || invalid_address;
    resp_data_o.xcpt_st = (((!w_perm_ok) || tlb_xcpt_dirty) && tlb_cache_hit) || invalid_address;
    resp_data_o.xcpt_if = (((!x_perm_ok) || tlb_xcpt_accessed) && tlb_cache_hit) || invalid_address;
    resp_data_o.access_fault = tlb_cache_rdata.ptw_access_fault && tlb_cache_hit;
  end

endmodule : minion_tlb
