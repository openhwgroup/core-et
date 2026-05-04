// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module ptw_top (
  input  logic                    clock,
  input  logic                    reset,
  // Is this in the IOShire
  input  logic                    ioshire,
  // Request port
  input  minion_ptw_req           req_data,
  input  logic                    req_valid,
  output logic                    req_ready,
  input  logic                    invalidate,
  // Neighborhood ESRs
  input  esr_mprot_t              mprot,
  input  tlb_entry_type           vmspagesize,
  // Response port
  output minion_ptw_pte           resp_data,
  output logic                    resp_valid,
  // Memory interface
  // L2 Request bus
  input  logic                    mem_miss_req_disable_next,
  output et_link_neigh_req_info_t mem_miss_req_info,
  output logic                    mem_miss_req_valid,
  input  logic                    mem_miss_req_ready,
  // L2 Fill/Ack bus
  input  et_link_rsp_info_t       mem_fill_req_info,
  input  logic                    mem_fill_req_valid,
  output logic                    mem_fill_req_ready,
  // Output debug signals going to Status Monitor
  output ptw_dbg_sm_t             dbg_sm_signals
);

// INTERNAL PARAMETERS
localparam PTW_PTE_BYTES_LOG2 = $clog2(`PTW_PTE_BYTES);
localparam PTW_CACHE_IDX_SZ   = $clog2(`PTW_CACHE_DEPTH);

// INTERNAL FUNCTIONS
function automatic [$clog2(`PTW_SV48_PG_LEVELS)-1:0] ptw_skip_levels(tlb_entry_type typ);
  case (typ)
    tlb_entry_type_2M: ptw_skip_levels = 'd1;
    tlb_entry_type_1G: ptw_skip_levels = 'd2;
    default:           ptw_skip_levels = 'd0;
  endcase
endfunction

// INTERNAL DECLARATIONS
tlb_entry_type                                                vmspagesize_reg;
et_link_rsp_info_t                                            mem_fill_req_info_reg;
logic                                                         mem_fill_req_valid_reg;
ptw_state                                                     state, state_next;
logic [$clog2(`PTW_SV48_PG_LEVELS)-1:0]                       level_count, level_count_next;
minion_ptw_req                                                req_data_reg, req_data_reg_next;
minion_ptw_pte                                                current_pte_reg, current_pte_reg_next;
minion_ptw_pte                                                in_pte;
logic                                                         in_pte_is_table;
logic [`PA_RANGE]                                             pte_addr;
logic [PTW_CACHE_IDX_SZ-1:0]                                  pte_cache_waddr;
logic [PTW_CACHE_IDX_SZ-1:0]                                  pte_cache_raddr;
logic [`PTW_CACHE_DEPTH-1:0]                                  pte_cache_valid;
//logic [`PTW_CACHE_DEPTH-1:0][`CSR_SATP_ASID_SZ-1:0] pte_cache_asid;  // ASID is 0 bits wide (RTLMIN-350)
logic                                                         pte_cache_hit;
logic [`PTW_CACHE_DEPTH-1:0][`PA_SIZE-PTW_PTE_BYTES_LOG2-1:0] pte_cache_tags;
logic [`PTW_CACHE_DEPTH-1:0][`PA_TRANS_SIZE-1:0]              pte_cache;
logic [`PTW_CACHE_DEPTH-1:0]                                  pte_cache_hits;
logic [`PA_TRANS_SIZE-1:0]                                    pte_cache_data;
logic                                                         pte_addr_invalid;
logic                                                         access_fault_reg, access_fault_reg_next;
logic                                                         ptw_invalidate_reg;
logic                                                         ptw_invalidate_reg_next;

`RST_FF(clock, reset, ptw_invalidate_reg, ptw_invalidate_reg_next, '0)

// Virtual memory Sv48 and Sv39 support. Sv48 just have 1 additional level (4 instend of 3)
logic [$clog2(`PTW_SV48_PG_LEVELS)-1:0] ptw_pg_levels, ptw_pg_levels_next;

//   CLK    DOUT             DIN
`FF(clock, vmspagesize_reg, vmspagesize)

// Flop L2 response
// ----------------
//          CLK    RST    EN                                                                    DOUT                    DIN                 DEF
`RST_EN_FF (clock, reset, mem_fill_req_ready && (mem_fill_req_valid_reg != mem_fill_req_valid), mem_fill_req_valid_reg, mem_fill_req_valid, 1'b0)
`EN_FF     (clock,        mem_fill_req_ready && mem_fill_req_valid,                             mem_fill_req_info_reg,  mem_fill_req_info)

// L1 CACHE
// --------
// Small L1 cache to store the last received 512b lines
logic [`PA_MSB-`SC_LINE_OFFSET_ID_SIZE:0] l1_tag;
logic [`ET_LINE_DATA_SIZE-1:0]            l1_cache;
logic                                     l1_valid;
logic                                     l1_hit;

assign l1_hit = (l1_tag == pte_addr[`PA_MSB:`SC_LINE_OFFSET_ID_SIZE]) && l1_valid;

// Requested line (L1 or L2)
logic [`ET_LINE_DATA_SIZE-1:0]           req_line;
assign req_line = (l1_hit && l1_valid) ? l1_cache : mem_fill_req_info_reg.data;

always @(posedge clock) begin
  if (reset)
    l1_valid <= '0;
  else begin
    //Invalidates L1  
    if (ptw_invalidate_reg) begin
      l1_valid <= 1'b0;
    end
    // Wait until L2 sends a line
    else if (mem_fill_req_valid_reg && (mem_fill_req_info_reg.opcode == ET_LINK_RSP_AckData)) begin
      // Save L2 line to L1
      l1_cache <= mem_fill_req_info_reg.data;
      l1_tag   <= pte_addr[`PA_MSB:`SC_LINE_OFFSET_ID_SIZE];
      l1_valid <= 1'b1;
    end
  end
end // always @ (posedge clock)

// Manage an invalidate petition
// On invalidation, wait for any outstanding request and send a response to the TLB
// The response should be discarded by the TLB
always_comb begin
  ptw_invalidate_reg_next = ptw_invalidate_reg;
  if (invalidate) begin
    ptw_invalidate_reg_next = 1'b1;
  end else if (ptw_invalidate_reg && (state inside {`PTW_S_READY,`PTW_S_DONE})) begin
    ptw_invalidate_reg_next = 1'b0;
  end
end


// FUNCTIONALITY
assign req_ready = state == `PTW_S_READY && !ptw_invalidate_reg;

// FSM
// ---
//      CLK    RST    DOUT              DIN         DEF
`RST_FF(clock, reset, state,            state_next, `PTW_S_READY)
`FF    (clock,        level_count,      level_count_next)
`FF    (clock,        access_fault_reg, access_fault_reg_next)
`FF    (clock,        req_data_reg,     req_data_reg_next)
`FF    (clock,        current_pte_reg,  current_pte_reg_next)
`FF    (clock,        ptw_pg_levels,    ptw_pg_levels_next)

always_comb begin
  state_next            = state;
  level_count_next      = level_count;
  access_fault_reg_next = access_fault_reg;
  req_data_reg_next     = req_data_reg;
  current_pte_reg_next  = current_pte_reg;
  ptw_pg_levels_next    = ptw_pg_levels;

  case (state) 
    `PTW_S_READY: begin
      level_count_next      = '0;
      access_fault_reg_next = 1'b0;	

      // Register request	
      if (req_valid && !ptw_invalidate_reg) begin
        state_next               = `PTW_S_REQ;
        req_data_reg_next        = req_data;
        // Start with base table address
        current_pte_reg_next.ppn = (`CSR_SATP_PPN_SZ)'(req_data.satp_ppn);
        // Select virtual memory system
        ptw_pg_levels_next       = (req_data.satp_mode == `CSR_SATP_MODE_SV48 ? `PTW_SV48_PG_LEVELS-1 : `PTW_SV39_PG_LEVELS-1)
                                 - ptw_skip_levels(vmspagesize_reg);
      end
    end

    `PTW_S_REQ: begin
      // Go to done stage. Responses will be discarted by TLB
      if (ptw_invalidate_reg) begin
        state_next = `PTW_S_DONE;
      end else if (pte_addr_invalid) begin // PMA check failed. Return access fault.	
        access_fault_reg_next = 1'b1;
        state_next            = `PTW_S_DONE;
      end else if (pte_cache_hit) begin   // Cache hit, PTE is a pointer to a page table .Take cached PTE and go to next level
        state_next               = `PTW_S_REQ;
        level_count_next         = level_count + 1'b1;
        current_pte_reg_next.ppn = (`CSR_SATP_PPN_SZ)'(pte_cache_data);
      end else if (mem_miss_req_ready || (l1_valid && l1_hit)) // Otherwise, wait for L2 to accept except if we hit on L1
        state_next = `PTW_S_WAIT;
    end

    `PTW_S_WAIT: begin
      // Wait for L2 response or take the line from L1
      if ((mem_fill_req_valid_reg && mem_fill_req_ready) || (l1_hit && l1_valid)) begin
        // Jump to done. Respones will be discarted by TLB
        if (ptw_invalidate_reg) begin
          state_next = `PTW_S_DONE;
        end else if (in_pte_is_table && level_count < ptw_pg_levels) begin
          // If the PTE is a pointer to the next level of the page table
          // save PTE and search the next level
          current_pte_reg_next = in_pte;
          level_count_next     = level_count + 1'b1;
          state_next           = `PTW_S_REQ;
        end else begin
          // In any other case, save PTE and finish. This may happen if:
          // - Everything OK in leaf PTE, and no need to set dirty bit
          // - x, w, r combination is invalid
          // - Privilege mode is wrong
          // - Permissions are wrong
          // - Level count overflow (PTE in last level is a pointer to a page table)
          current_pte_reg_next = in_pte;
          state_next           = `PTW_S_DONE;
        end
      end
    end

    `PTW_S_DONE: begin
      // Finish
      state_next = `PTW_S_READY;
    end
  endcase
end


// Request to memory
// -----------------
// VPN page table indexes from request
logic [`PTW_SV48_PG_LEVELS-1:0][`PTW_PG_IDX_SZ-1:0] vpn_idxs; // [ vpnL0 ][ vpnL1 ][ vpnL2 ][ vpnL3 ]
always_comb begin
  for (integer i=0; i<`PTW_SV48_PG_LEVELS; i++)
    vpn_idxs[i] = req_data_reg.addr[`VA_UNTRANS_SIZE + 
                  (`PTW_SV48_PG_LEVELS-1-i)*`PTW_PG_IDX_SZ +: `PTW_PG_IDX_SZ];
end

logic [$clog2(`PTW_SV48_PG_LEVELS)-1:0]       vpn_index;
assign vpn_index = req_data_reg.satp_mode == `CSR_SATP_MODE_SV48 ? level_count : 
                   level_count + 1'b1;

// Address: Take base table address and add VPN to index the PTE
assign pte_addr = {current_pte_reg.ppn[`PA_TRANS_SIZE-1:0], vpn_idxs[vpn_index], PTW_PTE_BYTES_LOG2'(0)};

// PMA checker
assign pte_addr_invalid = !( paddr_is_sram_space(pte_addr)    ||
                             paddr_is_mram_space(pte_addr)    ||
                             paddr_is_bootrom_space(pte_addr) );

// Memory miss request
et_link_neigh_req_info_t mem_miss_req_info_next;
logic                    mem_miss_req_valid_next;

//       CLK    RST    EN                                                                    DOUT                DIN                      DEF
`EN_FF  (clock,        (mem_miss_req_ready | ~mem_miss_req_valid) & mem_miss_req_valid_next, mem_miss_req_info,  mem_miss_req_info_next)
`RST_FF (clock, reset,                                                                       mem_miss_req_valid, mem_miss_req_valid_next, 1'b0)

always_comb begin
  mem_miss_req_info_next.id        = '0; // Unused
  mem_miss_req_info_next.source    = '0; // Unused (assigned at neighborhood channel)
  mem_miss_req_info_next.wdata     = 1'b0;
  mem_miss_req_info_next.opcode    = ET_LINK_REQ_Read;
  mem_miss_req_info_next.subopcode = `ET_LINK_SUBOPCODE_SIZE'(ET_LINK_Read_L2);
  mem_miss_req_info_next.address   = {pte_addr[`PA_MSB:`SC_LINE_OFFSET_ID_SIZE],(`SC_LINE_OFFSET_ID_SIZE)'(0)};
  mem_miss_req_info_next.data      = '0;
  mem_miss_req_info_next.size      = ET_LINK_Line;
  mem_miss_req_info_next.qwen      = 4'b0000;

  if (!mem_miss_req_valid)
    mem_miss_req_valid_next = ((state == `PTW_S_REQ) && !pte_cache_hit && !l1_hit) && !mem_miss_req_disable_next;
  else
    mem_miss_req_valid_next = !mem_miss_req_ready && !mem_miss_req_disable_next;  
end

// Response from memory
// --------------------
minion_ptw_pte in_pte_tmp;

// Select entry from 8-entry cache line
assign in_pte_tmp = req_line[8*pte_addr[`SC_LINE_OFFSET_ID_SIZE-1:0] +: 8*`PTW_PTE_BYTES];

always_comb begin
  // Copy all input bits
  in_pte = in_pte_tmp;
  // Only `PA_TRANS_SIZE bits of PPN are used
  in_pte.ppn = (`CSR_SATP_PPN_SZ)'(in_pte_tmp.ppn[`PA_TRANS_SIZE-1:0]);
  // Check that unused MSBs from PPN are 0, otherwise input PTE is not valid
  if (in_pte_tmp.ppn[`CSR_SATP_PPN_SZ-1:`PA_TRANS_SIZE] != 0)
    in_pte.v = 1'b0;
end

assign in_pte_is_table = in_pte.v && !in_pte.r && !in_pte.w && !in_pte.x;

assign mem_fill_req_ready = 1'b1;

// PTE cache
// ---------
// Pseudo-LRU
logic plru_access;
logic [PTW_CACHE_IDX_SZ-1:0] plru_replace_idx;

// When hit occurs and entry is used, update PLRU state
assign plru_access = pte_cache_hit && state == `PTW_S_REQ;

pseudo_lru #(.N_WAYS(`PTW_CACHE_DEPTH))
plru (.*,
      .access(plru_access),
      .index_in(pte_cache_raddr),
      .index_out(plru_replace_idx)
     );

// Select cache write address
always_comb begin
  // If all entries are valid, use PLRU to select one
  pte_cache_waddr = plru_replace_idx;
  // Otherwise, select one of the free entries
  for (integer i = `PTW_CACHE_DEPTH-1; i >= 0; i--) begin
    if (pte_cache_valid[i] == 1'b0)
      pte_cache_waddr = i[PTW_CACHE_IDX_SZ-1:0];
  end
end

// Cache
always @(posedge clock) begin
  if (reset)
    pte_cache_valid <= '0;
  else begin
    // Store received PTE if it was not stored yet (only non-leaf PTEs)
    if (mem_fill_req_valid_reg && mem_fill_req_ready && 
    (mem_fill_req_info_reg.opcode == ET_LINK_RSP_AckData) && in_pte_is_table && !pte_cache_hit) begin
      pte_cache_valid[pte_cache_waddr] <= 1'b1;
      pte_cache_tags[pte_cache_waddr] <= pte_addr[`PA_MSB:PTW_PTE_BYTES_LOG2];
      pte_cache[pte_cache_waddr] <= in_pte.ppn[`PA_TRANS_SIZE-1:0];
    end
    // Invalidate cache entries
    if (ptw_invalidate_reg)
      pte_cache_valid <= '0;
  end
end

always_comb begin
  for (integer i = 0; i < `PTW_CACHE_DEPTH; i++)
    pte_cache_hits[i] = (pte_cache_tags[i] == pte_addr[`PA_MSB:PTW_PTE_BYTES_LOG2]) &&
                        pte_cache_valid[i];
end
assign pte_cache_hit = |pte_cache_hits;

// Select cache read address
always_comb begin
  pte_cache_raddr = '0;
  // Select entry that made hit
  for (integer i = `PTW_CACHE_DEPTH-1; i >= 0; i--) begin
    if (pte_cache_hits[i])
      pte_cache_raddr = i[PTW_CACHE_IDX_SZ-1:0];
  end
end

// Read cache data
assign pte_cache_data = pte_cache[pte_cache_raddr];

// Response to requestors
// ----------------------
logic [`PTW_SV48_PG_LEVELS-1:0][`PA_TRANS_SIZE-1:0] resp_ppns;
minion_ptw_pte                                      resp_data_next;
logic                                               resp_valid_next;

//       CLK    RST    EN               DOUT        DIN              DEF
`EN_FF  (clock,        resp_valid_next, resp_data,  resp_data_next)
`RST_FF (clock, reset,                  resp_valid, resp_valid_next, 1'b0)

// Pages greater than the minimum size are fractured:
// Response PPN is generated by concatenating PTE's most significant PPN indexes
// (which define page boundary according to table level)
// and least significant VPN indexes (in-page offset)
 
for (genvar lvl = 0; lvl < `PTW_SV48_PG_LEVELS-3; lvl++)
  //                      { PPN               [PTW_SV48_PG_LEVELS-1:             PTW_SV48_PG_LEVELS-lvl-1 ], VPN [PTW_SV48_PG_LEVELS-lvl-2:0]}
  assign resp_ppns[lvl] = {current_pte_reg.ppn[`PA_TRANS_SIZE-1:`PTW_PG_IDX_SZ*(`PTW_SV48_PG_LEVELS-lvl-1)],
                          req_data_reg.addr[(`VA_UNTRANS_SIZE + 2*`PTW_PG_IDX_SZ) +: `PTW_PG_IDX_SZ*(`PTW_SV48_PG_LEVELS-lvl-3)],
                          vmspagesize_reg == tlb_entry_type_1G ? {`PTW_PG_IDX_SZ{1'b0}} : req_data_reg.addr[(`VA_UNTRANS_SIZE + `PTW_PG_IDX_SZ) +: `PTW_PG_IDX_SZ],
                          vmspagesize_reg >= tlb_entry_type_2M ? {`PTW_PG_IDX_SZ{1'b0}} : req_data_reg.addr[ `VA_UNTRANS_SIZE                   +: `PTW_PG_IDX_SZ]
                          };

assign resp_ppns[`PTW_SV48_PG_LEVELS-3] = {current_pte_reg.ppn[`PA_TRANS_SIZE-1:`PTW_PG_IDX_SZ*2],
                                           vmspagesize_reg == tlb_entry_type_1G ? {`PTW_PG_IDX_SZ{1'b0}} : req_data_reg.addr[(`VA_UNTRANS_SIZE + `PTW_PG_IDX_SZ) +: `PTW_PG_IDX_SZ],
                                           vmspagesize_reg >= tlb_entry_type_2M ? {`PTW_PG_IDX_SZ{1'b0}} : req_data_reg.addr[ `VA_UNTRANS_SIZE                   +: `PTW_PG_IDX_SZ]
                                          };
assign resp_ppns[`PTW_SV48_PG_LEVELS-2] = {current_pte_reg.ppn[`PA_TRANS_SIZE-1:`PTW_PG_IDX_SZ],
                                           vmspagesize_reg >= tlb_entry_type_2M ? {`PTW_PG_IDX_SZ{1'b0}} : req_data_reg.addr[ `VA_UNTRANS_SIZE                   +: `PTW_PG_IDX_SZ]
                                          };
assign resp_ppns[`PTW_SV48_PG_LEVELS-1] = current_pte_reg.ppn[`PA_TRANS_SIZE-1:0];

always_comb begin
  resp_data_next = current_pte_reg;
  resp_data_next.rsvd_for_hw[0] = (req_data_reg.prv == `CSR_PRV_M);
  resp_data_next.ppn = (`CSR_SATP_PPN_SZ)'(resp_ppns[vpn_index]);
  resp_data_next.access_fault = access_fault_reg;
  resp_data_next.canceled_req = ptw_invalidate_reg; // This petition was canceled by an invalidation request. TLB should not cache this petition
end
assign resp_valid_next = (state == `PTW_S_DONE);

////////////////////////////////////////////////////////////////////////////////
// Connect output debug signals
////////////////////////////////////////////////////////////////////////////////
always_comb begin
  dbg_sm_signals = '0;
  // Connect PTW signals
  dbg_sm_signals.state = state;
end

endmodule
