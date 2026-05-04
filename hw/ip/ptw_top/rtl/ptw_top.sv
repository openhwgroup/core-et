// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module ptw_top (
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  logic                       ioshire_i,
  input  minion_pkg::minion_ptw_req  req_data_i,
  input  logic                       req_valid_i,
  output logic                       req_ready_o,
  input  logic                       invalidate_i,
  /* verilator lint_off UNUSEDSIGNAL */  // PTW PMA checks intentionally ignore ESR fields unrelated to neighborhood/SP/DRAM access.
  input  minion_dcache_pkg::esr_mprot_t mprot_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  minion_pkg::tlb_entry_type  vmspagesize_i,
  output minion_pkg::minion_ptw_pte  resp_data_o,
  output logic                       resp_valid_o,
  input  logic                       mem_miss_req_disable_next_i,
  output etlink_pkg::neigh_req_t     mem_miss_req_info_o,
  output logic                       mem_miss_req_valid_o,
  input  logic                       mem_miss_req_ready_i,
  input  etlink_pkg::rsp_t           mem_fill_req_info_i,
  input  logic                       mem_fill_req_valid_i,
  output logic                       mem_fill_req_ready_o,
  output neigh_pkg::ptw_dbg_sm_t     dbg_sm_signals_o
);
  import etlink_pkg::ReqRead;
  import etlink_pkg::RspAckData;
  import etlink_pkg::SizeLine;
  import minion_pkg::CsrSatpModeSv48;
  import minion_pkg::CsrSatpPpnSz;
  import minion_pkg::PaSize;
  import minion_pkg::PaTransSize;
  import minion_pkg::PrvM;
  import minion_pkg::PtwPgIdxSz;
  import minion_pkg::VaSize;
  import minion_pkg::VaUntransSize;
  import minion_pkg::tlb_entry_type_1G;
  import minion_pkg::tlb_entry_type_2M;
  import minion_dcache_pkg::paddr_is_dram_cacheable;
  import minion_dcache_pkg::paddr_is_dram_mcode;
  import minion_dcache_pkg::paddr_is_dram_mdata;
  import minion_dcache_pkg::paddr_is_dram_osbox;
  import minion_dcache_pkg::paddr_is_dram_other;
  import minion_dcache_pkg::paddr_is_dram_scode;
  import minion_dcache_pkg::paddr_is_dram_sdata;
  import minion_dcache_pkg::paddr_is_sp_sram_data;
  import neigh_pkg::PtwStateDone;
  import neigh_pkg::PtwStateReady;
  import neigh_pkg::PtwStateReq;
  import neigh_pkg::PtwStateWait;
  import shirecache_pkg::LineOffsetIdSize;

  localparam int unsigned PtwCacheDepth    = 8;
  localparam int unsigned PtwPteBytes      = 8;
  /* verilator lint_off UNUSEDPARAM */  // Sv39/Sv48 level counts are kept explicit to mirror the original PTW constants.
  localparam int unsigned PtwSv39PgLevels  = 3;
  localparam int unsigned PtwSv48PgLevels  = 4;
  /* verilator lint_on UNUSEDPARAM */
  localparam int unsigned PtwLevelCountSz  = $clog2(PtwSv48PgLevels);
  localparam int unsigned PtwPteBytesLog2  = $clog2(PtwPteBytes);
  localparam int unsigned PtwCacheIdxSz    = $clog2(PtwCacheDepth);
  localparam int unsigned PtwPpnPadSz      = CsrSatpPpnSz - PaTransSize;
  localparam logic [PtwLevelCountSz-1:0] PtwSv39TopLevel = 2'd2;
  localparam logic [PtwLevelCountSz-1:0] PtwSv48TopLevel = 2'd3;

  function automatic logic [PtwLevelCountSz-1:0] ptw_skip_levels(
      minion_pkg::tlb_entry_type typ
  );
    unique case (typ)
      tlb_entry_type_2M: return 'd1;
      tlb_entry_type_1G: return 'd2;
      default:          return 'd0;
    endcase
  endfunction

  minion_pkg::tlb_entry_type                   vmspagesize_q;
  /* verilator lint_off UNUSEDSIGNAL */  // The original flops the full ET-Link response; PTW only consumes opcode and data here.
  etlink_pkg::rsp_t                            mem_fill_req_info_reg_q;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                                        mem_fill_req_valid_reg_q;
  neigh_pkg::ptw_state_e                       state_q;
  logic [PtwLevelCountSz-1:0]                  level_count_q;
  logic [PtwLevelCountSz-1:0]                  ptw_pg_levels_q;
  /* verilator lint_off UNUSEDSIGNAL */  // satp_ppn is preserved in the request register for parity with the original but consumed through current_pte_q.
  minion_pkg::minion_ptw_req                   req_data_q;
  /* verilator lint_on UNUSEDSIGNAL */
  minion_pkg::minion_ptw_pte                   current_pte_q;
  minion_pkg::minion_ptw_pte                   in_pte;
  logic                                        in_pte_is_table;
  logic [PaSize-1:0]                           pte_addr;
  logic [PtwCacheIdxSz-1:0]                    pte_cache_waddr;
  logic [PtwCacheIdxSz-1:0]                    pte_cache_raddr;
  logic [PtwCacheDepth-1:0]                    pte_cache_valid_q;
  logic [PtwCacheDepth-1:0][PaSize-PtwPteBytesLog2-1:0] pte_cache_tags_q;
  logic [PtwCacheDepth-1:0][PaTransSize-1:0]   pte_cache_q;
  logic [PtwCacheDepth-1:0]                    pte_cache_hits;
  logic                                        pte_cache_hit;
  logic [PaTransSize-1:0]                      pte_cache_data;
  logic                                        pte_addr_invalid;
  logic                                        access_fault_reg_q;
  logic                                        ptw_invalidate_q;
  logic                                        ptw_invalidate_d;

  logic [etlink_pkg::PaMsb-LineOffsetIdSize:0] l1_tag_q;
  logic [etlink_pkg::DataSize-1:0]             l1_cache_q;
  logic                                        l1_valid_q;
  logic                                        l1_hit;
  logic [etlink_pkg::DataSize-1:0]             req_line;

  minion_pkg::minion_ptw_pte                   in_pte_tmp;
  logic                                        plru_access;
  logic [PtwCacheIdxSz-1:0]                    plru_replace_idx;
  logic [PtwSv48PgLevels-1:0][PtwPgIdxSz-1:0]  vpn_idxs;
  logic [PtwLevelCountSz-1:0]                  vpn_index;
  logic [PtwSv48PgLevels-1:0][PaTransSize-1:0] resp_ppns;
  minion_pkg::minion_ptw_pte                   resp_data_d;
  logic                                        resp_valid_d;
  etlink_pkg::neigh_req_t                      mem_miss_req_info_d;
  logic                                        mem_miss_req_valid_d;

  always_comb begin
    ptw_invalidate_d = ptw_invalidate_q;
    if (invalidate_i) begin
      ptw_invalidate_d = 1'b1;
    end else if (ptw_invalidate_q && (state_q inside {PtwStateReady, PtwStateDone})) begin
      ptw_invalidate_d = 1'b0;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      ptw_invalidate_q <= 1'b0;
    end else begin
      ptw_invalidate_q <= ptw_invalidate_d;
    end
  end

  always_ff @(posedge clk_i) begin
    vmspagesize_q <= vmspagesize_i;
  end

  assign mem_fill_req_ready_o = 1'b1;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      mem_fill_req_valid_reg_q <= 1'b0;
    end else if (mem_fill_req_ready_o) begin
      mem_fill_req_valid_reg_q <= mem_fill_req_valid_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (mem_fill_req_ready_o && mem_fill_req_valid_i) begin
      mem_fill_req_info_reg_q <= mem_fill_req_info_i;
    end
  end

  assign l1_hit = (l1_tag_q == pte_addr[etlink_pkg::PaMsb:LineOffsetIdSize]) && l1_valid_q;
  assign req_line = (l1_hit && l1_valid_q) ? l1_cache_q : mem_fill_req_info_reg_q.data;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      l1_valid_q <= 1'b0;
    end else begin
      if (mem_fill_req_valid_reg_q && (mem_fill_req_info_reg_q.opcode == RspAckData)) begin
        l1_cache_q <= mem_fill_req_info_reg_q.data;
        l1_tag_q   <= pte_addr[etlink_pkg::PaMsb:LineOffsetIdSize];
        l1_valid_q <= 1'b1;
      end
      if (ptw_invalidate_q) begin
        l1_valid_q <= 1'b0;
      end
    end
  end

  assign req_ready_o = (state_q == PtwStateReady) && !ptw_invalidate_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      state_q <= PtwStateReady;
    end else begin
      unique case (state_q)
        PtwStateReady: begin
          level_count_q      <= '0;
          access_fault_reg_q <= 1'b0;
          if (req_valid_i && !ptw_invalidate_q) begin
            state_q           <= PtwStateReq;
            req_data_q        <= req_data_i;
            current_pte_q.ppn <= {{PtwPpnPadSz{1'b0}}, req_data_i.satp_ppn};
            ptw_pg_levels_q   <= ((req_data_i.satp_mode == CsrSatpModeSv48) ? PtwSv48TopLevel : PtwSv39TopLevel)
                               - ptw_skip_levels(vmspagesize_q);
          end
        end

        PtwStateReq: begin
          if (ptw_invalidate_q) begin
            state_q <= PtwStateDone;
          end else if (pte_addr_invalid) begin
            access_fault_reg_q <= 1'b1;
            state_q <= PtwStateDone;
          end else if (pte_cache_hit) begin
            state_q           <= PtwStateReq;
            level_count_q     <= level_count_q + 1'b1;
            current_pte_q.ppn <= {{PtwPpnPadSz{1'b0}}, pte_cache_data};
          end else if (mem_miss_req_ready_i || (l1_valid_q && l1_hit)) begin
            state_q <= PtwStateWait;
          end
        end

        PtwStateWait: begin
          if ((mem_fill_req_valid_reg_q && mem_fill_req_ready_o) || (l1_hit && l1_valid_q)) begin
            if (ptw_invalidate_q) begin
              state_q <= PtwStateDone;
            end else if (in_pte_is_table && (level_count_q < ptw_pg_levels_q)) begin
              current_pte_q <= in_pte;
              level_count_q <= level_count_q + 1'b1;
              state_q       <= PtwStateReq;
            end else begin
              current_pte_q <= in_pte;
              state_q       <= PtwStateDone;
            end
          end
        end

        PtwStateDone: begin
          state_q <= PtwStateReady;
        end

        default: begin
          state_q <= PtwStateReady;
        end
      endcase
    end
  end

  always_comb begin
    for (int i = 0; i < PtwSv48PgLevels; i++) begin
      vpn_idxs[i] = req_data_q.addr[VaUntransSize + (PtwSv48PgLevels - 1 - i) * PtwPgIdxSz +: PtwPgIdxSz];
    end
  end

  assign vpn_index = (req_data_q.satp_mode == CsrSatpModeSv48) ? level_count_q : (level_count_q + 1'b1);
  assign pte_addr = {current_pte_q.ppn[PaTransSize-1:0], vpn_idxs[vpn_index], {PtwPteBytesLog2{1'b0}}};

  logic ptw_req_dram_mcode_permitted;
  logic ptw_req_dram_mdata_permitted;
  logic ptw_req_dram_scode_permitted;
  logic ptw_req_dram_sdata_permitted;

  assign ptw_req_dram_mcode_permitted = ioshire_i
                                     || ((req_data_q.prv == PrvM) && !mprot_i.enable_secure_memory);

  assign ptw_req_dram_mdata_permitted = ioshire_i
                                     || (req_data_q.prv == PrvM);

  assign ptw_req_dram_scode_permitted = mprot_i.enable_secure_memory
                                      ? (ioshire_i || (req_data_q.prv == PrvM))
                                      : (ioshire_i || !mprot_i.disable_osbox_access);

  assign ptw_req_dram_sdata_permitted = ioshire_i
                                     || mprot_i.enable_secure_memory
                                     || !mprot_i.disable_osbox_access;

  assign pte_addr_invalid =
      !((paddr_is_sp_sram_data(pte_addr)               && ioshire_i)
     || (paddr_is_dram_mcode(pte_addr)                 && ptw_req_dram_mcode_permitted)
     || (paddr_is_dram_mdata(pte_addr)                 && ptw_req_dram_mdata_permitted)
     || (paddr_is_dram_scode(pte_addr)                 && ptw_req_dram_scode_permitted)
     || (paddr_is_dram_sdata(pte_addr)                 && ptw_req_dram_sdata_permitted)
     || (paddr_is_dram_osbox(pte_addr)                 && (ioshire_i || !mprot_i.disable_osbox_access))
     || (paddr_is_dram_other(pte_addr, mprot_i.dram_size) && (ioshire_i || paddr_is_dram_cacheable(pte_addr))));

  always_comb begin
    mem_miss_req_info_d = '0;
    mem_miss_req_info_d.id        = '0;
    mem_miss_req_info_d.source    = '0;
    mem_miss_req_info_d.wdata     = 1'b0;
    mem_miss_req_info_d.opcode    = ReqRead;
    mem_miss_req_info_d.subopcode = '0;
    mem_miss_req_info_d.address   = {pte_addr[etlink_pkg::PaMsb:LineOffsetIdSize], {LineOffsetIdSize{1'b0}}};
    mem_miss_req_info_d.data      = '0;
    mem_miss_req_info_d.size      = SizeLine;
    mem_miss_req_info_d.qwen      = 4'b0000;

    if (!mem_miss_req_valid_o) begin
      mem_miss_req_valid_d = ((state_q == PtwStateReq) && !pte_cache_hit && !l1_hit) && !mem_miss_req_disable_next_i;
    end else begin
      mem_miss_req_valid_d = !mem_miss_req_ready_i && !mem_miss_req_disable_next_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if ((mem_miss_req_ready_i || !mem_miss_req_valid_o) && mem_miss_req_valid_d) begin
      mem_miss_req_info_o <= mem_miss_req_info_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      mem_miss_req_valid_o <= 1'b0;
    end else begin
      mem_miss_req_valid_o <= mem_miss_req_valid_d;
    end
  end

  assign in_pte_tmp = req_line[8 * pte_addr[LineOffsetIdSize-1:0] +: 8 * PtwPteBytes];

  always_comb begin
    in_pte = in_pte_tmp;
    in_pte.ppn = {{PtwPpnPadSz{1'b0}}, in_pte_tmp.ppn[PaTransSize-1:0]};
    if (in_pte_tmp.ppn[CsrSatpPpnSz-1:PaTransSize] != '0) begin
      in_pte.v = 1'b0;
    end
  end

  assign in_pte_is_table = in_pte.v && !in_pte.r && !in_pte.w && !in_pte.x;

  assign plru_access = pte_cache_hit && (state_q == PtwStateReq);

  pseudo_lru #(
    .NWays(PtwCacheDepth)
  ) u_plru (
    .clk_i,
    .rst_ni,
    .access_i   (plru_access),
    .index_in_i (pte_cache_raddr),
    .index_out_o(plru_replace_idx)
  );

  always_comb begin
    pte_cache_waddr = plru_replace_idx;
    for (int i = PtwCacheDepth - 1; i >= 0; i--) begin
      if (!pte_cache_valid_q[i]) begin
        pte_cache_waddr = i[PtwCacheIdxSz-1:0];
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      pte_cache_valid_q <= '0;
    end else begin
      if (mem_fill_req_valid_reg_q && mem_fill_req_ready_o
          && (mem_fill_req_info_reg_q.opcode == RspAckData)
          && in_pte_is_table && !pte_cache_hit) begin
        pte_cache_valid_q[pte_cache_waddr] <= 1'b1;
        pte_cache_tags_q[pte_cache_waddr]  <= pte_addr[etlink_pkg::PaMsb:PtwPteBytesLog2];
        pte_cache_q[pte_cache_waddr]       <= in_pte.ppn[PaTransSize-1:0];
      end
      if (ptw_invalidate_q) begin
        pte_cache_valid_q <= '0;
      end
    end
  end

  always_comb begin
    for (int i = 0; i < PtwCacheDepth; i++) begin
      pte_cache_hits[i] = (pte_cache_tags_q[i] == pte_addr[etlink_pkg::PaMsb:PtwPteBytesLog2])
                       && pte_cache_valid_q[i];
    end
  end

  assign pte_cache_hit = |pte_cache_hits;

  always_comb begin
    pte_cache_raddr = '0;
    for (int i = PtwCacheDepth - 1; i >= 0; i--) begin
      if (pte_cache_hits[i]) begin
        pte_cache_raddr = i[PtwCacheIdxSz-1:0];
      end
    end
  end

  assign pte_cache_data = pte_cache_q[pte_cache_raddr];

  generate
    for (genvar lvl = 0; lvl < PtwSv48PgLevels - 3; lvl++) begin : gen_resp_ppns
      assign resp_ppns[lvl] = {
        current_pte_q.ppn[PaTransSize-1:PtwPgIdxSz * (PtwSv48PgLevels - lvl - 1)],
        req_data_q.addr[(VaUntransSize + 2 * PtwPgIdxSz) +: PtwPgIdxSz * (PtwSv48PgLevels - lvl - 3)],
        (vmspagesize_q == tlb_entry_type_1G) ? {PtwPgIdxSz{1'b0}}
                                             : req_data_q.addr[(VaUntransSize + PtwPgIdxSz) +: PtwPgIdxSz],
        (vmspagesize_q >= tlb_entry_type_2M) ? {PtwPgIdxSz{1'b0}}
                                             : req_data_q.addr[VaUntransSize +: PtwPgIdxSz]
      };
    end
  endgenerate

  assign resp_ppns[PtwSv48PgLevels-3] = {
    current_pte_q.ppn[PaTransSize-1:PtwPgIdxSz*2],
    (vmspagesize_q == tlb_entry_type_1G) ? {PtwPgIdxSz{1'b0}}
                                         : req_data_q.addr[(VaUntransSize + PtwPgIdxSz) +: PtwPgIdxSz],
    (vmspagesize_q >= tlb_entry_type_2M) ? {PtwPgIdxSz{1'b0}}
                                         : req_data_q.addr[VaUntransSize +: PtwPgIdxSz]
  };

  assign resp_ppns[PtwSv48PgLevels-2] = {
    current_pte_q.ppn[PaTransSize-1:PtwPgIdxSz],
    (vmspagesize_q >= tlb_entry_type_2M) ? {PtwPgIdxSz{1'b0}}
                                         : req_data_q.addr[VaUntransSize +: PtwPgIdxSz]
  };

  assign resp_ppns[PtwSv48PgLevels-1] = current_pte_q.ppn[PaTransSize-1:0];

  always_comb begin
    resp_data_d = current_pte_q;
    resp_data_d.rsvd_for_hw[0] = (req_data_q.prv == PrvM);
    resp_data_d.ppn = {{PtwPpnPadSz{1'b0}}, resp_ppns[vpn_index]};
    resp_data_d.access_fault = access_fault_reg_q;
    resp_data_d.canceled_req = ptw_invalidate_q;
  end

  assign resp_valid_d = (state_q == PtwStateDone);

  always_ff @(posedge clk_i) begin
    if (resp_valid_d) begin
      resp_data_o <= resp_data_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      resp_valid_o <= 1'b0;
    end else begin
      resp_valid_o <= resp_valid_d;
    end
  end

  always_comb begin
    dbg_sm_signals_o = '0;
    dbg_sm_signals_o.state         = state_q;
  end
endmodule
