// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cpu_etl2axi_trans_table
(
  // System signals
  input  logic clock,
  input  logic reset,
  // Allocate port
  input  etl2axi_tt_req_info_t           alloc_req_info,
  input  logic [`CPU_AXI_DATA_WIDTH-1:0] alloc_req_data,
  input  logic                           alloc_valid,
  output logic [`ETL2AXI_TT_SIZE_L-1:0]  alloc_entry,
  output logic                           alloc_full,
  // Deallocate port
  input  logic [`ETL2AXI_TT_SIZE_L-1:0] dealloc_entry,
  input  logic                          dealloc_valid,
  // Read port
  input  logic [`ETL2AXI_TT_SIZE_L-1:0]  rd_entry,
  output etl2axi_tt_req_info_t           rd_req_info,
  output logic [`CPU_AXI_DATA_WIDTH-1:0] rd_at_rd_data,
  output logic [1:0]                     rd_at_retry_cnt,
  // Conflict checking port
  input  logic [`ETL2AXI_TT_SIZE_L-1:0] chk_entry,
  input  etl2axi_req_info_t             chk_req_info,
  input  logic                          chk_valid,
  output logic                          chk_conflict,
  // Push port
  input  logic [`ETL2AXI_TT_SIZE_L-1:0] push_entry,
  input  logic                          push,
  // Atomic response port
  input  logic [`ETL2AXI_TT_SIZE_L-1:0]  atomic_rsp_entry,
  input  logic                           atomic_rsp_is_rd,
  input  logic [`CPU_AXI_DATA_WIDTH-1:0] atomic_rsp_data,
  input  logic                           atomic_rsp_valid_pre,
  input  logic                           atomic_rsp_valid,
  // Pop port
  output logic [`ETL2AXI_TT_SIZE_L-1:0]  pop_entry,
  output etl2axi_req_info_t              pop_req,
  output logic [`CPU_AXI_DATA_WIDTH-1:0] pop_data,
  output logic                           valid,
  input  logic                           pop
);

//===================================================================
// INTERNAL DECLARATIONS
//===================================================================
// Allocate port
logic [`MIN_PER_N_L-1:0]                            alloc_bank_sel;
logic [`MIN_PER_N-1:0]                              bank_alloc_valid;
logic [`MIN_PER_N-1:0]                              bank_alloc_rd_valid;
logic [`MIN_PER_N-1:0]                              bank_alloc_wr_valid;
logic [`MIN_PER_N-1:0]                              bank_alloc_at_valid;
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE_L-1:0] bank_alloc_rd_entry;
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE_L-1:0] bank_alloc_wr_entry;
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE_L-1:0] bank_alloc_at_entry;
logic [`MIN_PER_N-1:0]                              bank_alloc_rd_full;
logic [`MIN_PER_N-1:0]                              bank_alloc_wr_full;
logic [`MIN_PER_N-1:0]                              bank_alloc_at_full;
// Deallocate port
logic [`MIN_PER_N_L-1:0]                            dealloc_bank_sel;
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE_L-1:0] bank_dealloc_entry;
logic [`MIN_PER_N-1:0]                              bank_dealloc_valid;
// Read port
logic [`MIN_PER_N_L-1:0]                                                   rd_bank_sel;
logic [`ETL2AXI_TT_BANK_SIZE_L-1:0]                                        rd_bank_entry;
etl2axi_tt_req_info_t [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE-1:0]          bank_rd_req_info;
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE-1:0][`CPU_AXI_DATA_WIDTH-1:0] bank_rd_at_rd_data;
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE-1:0][1:0]                     bank_rd_at_retry_cnt;
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE-1:0]                          bank_rd_alloc_entries;
// Conflict checking
logic [`MIN_PER_N_L-1:0]                                                                              chk_bank_sel;
logic [`ETL2AXI_TT_BANK_SIZE_L-1:0]                                                                   chk_bank_entry;
logic [(`MIN_PER_N-1)-1:0][`MIN_PER_N_L-1:0]                                                          bank_other_idx;
logic [`ETL2AXI_TT_BANK_SIZE-1:0]                                                                     bank_own_chk_conflict_mask;
logic [(`MIN_PER_N-1)-1:0][`ETL2AXI_TT_BANK_SIZE-1:`ETL2AXI_TT_BANK_SIZE-`ETL2AXI_TT_BANK_WR_ENTRIES] bank_other_chk_conflict_mask;
logic                                                                                                 bank_own_chk_conflict;
logic                                                                                                 bank_other_chk_conflict;
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE-1:0]                                                     bank_chk_conflict_mask;
logic [`ETL2AXI_TT_SIZE-1:0]                                                                          chk_conflict_mask;
// Conflict mask
logic [`ETL2AXI_TT_SIZE-1:0][`ETL2AXI_TT_SIZE-1:0]                      conflict_mask, conflict_mask_next;
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE-1:0][`ETL2AXI_TT_SIZE-1:0] bank_rd_conflict_mask;
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE-1:0]                       bank_rd_conflict;
// Push port
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE_L-1:0] bank_push_entry;
logic [`MIN_PER_N-1:0]                              bank_push;
// Atomic response port
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE_L-1:0] bank_atomic_rsp_entry;
logic [`MIN_PER_N-1:0]                              bank_atomic_rsp_is_rd;
logic [`MIN_PER_N-1:0][`CPU_AXI_DATA_WIDTH-1:0]     bank_atomic_rsp_data;
logic [`MIN_PER_N-1:0]                              bank_atomic_rsp_valid_pre;
logic [`MIN_PER_N-1:0]                              bank_atomic_rsp_valid;
// Pop port
logic [`MIN_PER_N-1:0][`ETL2AXI_TT_BANK_SIZE_L-1:0] bank_pop_entry;
etl2axi_tt_req_info_t [`MIN_PER_N-1:0]              bank_pop_req;
logic [`MIN_PER_N-1:0]                              bank_pop_is_atomic_wr;
logic [`MIN_PER_N-1:0][`CPU_AXI_DATA_WIDTH-1:0]     bank_pop_data;
logic [`MIN_PER_N-1:0][`CPU_AXI_DATA_WIDTH-1:0]     bank_pop_at_rd_data;
logic [`MIN_PER_N-1:0]                              bank_valid;
logic [`MIN_PER_N-1:0]                              bank_pop_grant;
logic [`MIN_PER_N-1:0]                              bank_pop;

logic [`ETL2AXI_TT_SIZE_L-1:0]                      pop_d0_entry;
etl2axi_tt_req_info_t                               pop_d0_req;
logic                                               pop_d0_is_atomic_wr;
logic [`CPU_AXI_DATA_WIDTH-1:0]                     pop_d0_data;
logic [`CPU_AXI_DATA_WIDTH-1:0]                     pop_d0_at_rd_data;
logic                                               pop_d0_valid;
logic                                               pop_d0_ready;
logic                                               pop_d0_acc;
logic                                               pop_d1_valid;
logic                                               pop_d1_ready;
logic                                               pop_d1_acc;
logic [`ETL2AXI_TT_SIZE_L-1:0]                      pop_d1_entry;
etl2axi_tt_req_info_t                               pop_d1_req;
etl2axi_req_info_t                                  pop_d1_req_to_d2;
logic                                               pop_d1_is_atomic_wr;
logic [`CPU_AXI_DATA_WIDTH-1:0]                     pop_d1_data;
logic [`CPU_AXI_DATA_WIDTH-1:0]                     pop_d1_data_to_d2;
logic [`CPU_AXI_DATA_WIDTH-1:0]                     pop_d1_at_wr_data;
logic                                               pop_d2_valid;
logic                                               pop_d2_ready;
logic [`ETL2AXI_TT_SIZE_L-1:0]                      pop_d2_entry;
etl2axi_req_info_t                                  pop_d2_req;
logic [`CPU_AXI_DATA_WIDTH-1:0]                     pop_d2_data;

// Since transactions for different sources can be independently ordered,
// the Transaction Table is banked to reduce the number of parallel comparisons
// There is one bank per Minion. Requests from other sources will be shuffled
// among the banks according to the accessed address

//===================================================================
// Allocate port
//===================================================================
// Select bank according to source
always_comb
begin
  // If the source is a Minion, just select the corresponding bank
  if (alloc_req_info.source <= ET_LINK_NEIGH_SRC_Minion7)
    alloc_bank_sel = alloc_req_info.source[`MIN_PER_N_L-1:0];
  // Otherwise, select the bank according to the LSBs of the cacheline address
  // to shuffle requests among all the banks
  else
    alloc_bank_sel = alloc_req_info.address[`CPU_AXI_ADDR_OFFSET +: `MIN_PER_N_L];
end

// Combine ports from all the banks
always_comb
begin
  // Prepend bank index as the MSBs of the entry ID
  alloc_entry[`ETL2AXI_TT_BANK_SIZE_L +: `MIN_PER_N_L]            = alloc_bank_sel;
  alloc_entry[0                       +: `ETL2AXI_TT_BANK_SIZE_L] = alloc_req_info.op == ETL2AXI_REQ_OP_Write  ? bank_alloc_wr_entry[alloc_bank_sel]
                                                                  : alloc_req_info.op == ETL2AXI_REQ_OP_Read   ? bank_alloc_rd_entry[alloc_bank_sel]
                                                                  : alloc_req_info.op == ETL2AXI_REQ_OP_Atomic ? bank_alloc_at_entry[alloc_bank_sel]
                                                                  :                                              '0;
  alloc_full                                                      = alloc_req_info.op == ETL2AXI_REQ_OP_Write  ? bank_alloc_wr_full[alloc_bank_sel]
                                                                  : alloc_req_info.op == ETL2AXI_REQ_OP_Read   ? bank_alloc_rd_full[alloc_bank_sel]
                                                                  : alloc_req_info.op == ETL2AXI_REQ_OP_Atomic ? bank_alloc_at_full[alloc_bank_sel]
                                                                  :                                              1'b0;
end

//===================================================================
// Deallocate port
//===================================================================
// Select bank according to MSBs of the entry ID
assign dealloc_bank_sel = dealloc_entry[`ETL2AXI_TT_BANK_SIZE_L +: `MIN_PER_N_L];

//===================================================================
// Banks
//===================================================================
for (genvar b = 0; b < `MIN_PER_N; b++)
begin : Bank
  // Allocate port
  assign bank_alloc_valid[b] = alloc_valid & (alloc_bank_sel == b[`MIN_PER_N_L-1:0]);
  assign bank_alloc_rd_valid[b] = bank_alloc_valid[b] & (alloc_req_info.op == ETL2AXI_REQ_OP_Read);
  assign bank_alloc_wr_valid[b] = bank_alloc_valid[b] & (alloc_req_info.op == ETL2AXI_REQ_OP_Write);
  assign bank_alloc_at_valid[b] = bank_alloc_valid[b] & (alloc_req_info.op == ETL2AXI_REQ_OP_Atomic);

  // Dealloc port
  assign bank_dealloc_entry[b] = dealloc_entry[0 +: `ETL2AXI_TT_BANK_SIZE_L];
  assign bank_dealloc_valid[b] = dealloc_valid & (dealloc_bank_sel == b[`MIN_PER_N_L-1:0]);

  // Propage conflict information
  always_comb
  begin
    for (integer unsigned i = 0; i < `ETL2AXI_TT_BANK_SIZE; i++)
      bank_rd_conflict[b][i] = |bank_rd_conflict_mask[b][i];
  end

  // Push port
  assign bank_push_entry[b] = push_entry[`ETL2AXI_TT_BANK_SIZE_L-1:0];
  assign bank_push[b]       = push & (push_entry[`ETL2AXI_TT_BANK_SIZE_L +: `MIN_PER_N_L] == b[`MIN_PER_N_L-1:0]);
  
  // Atomic response port
  assign bank_atomic_rsp_entry[b]     = atomic_rsp_entry[`ETL2AXI_TT_BANK_SIZE_L-1:0];
  assign bank_atomic_rsp_is_rd[b]     = atomic_rsp_is_rd;
  assign bank_atomic_rsp_data[b]      = atomic_rsp_data;
  assign bank_atomic_rsp_valid_pre[b] = atomic_rsp_valid_pre;
  assign bank_atomic_rsp_valid[b]     = atomic_rsp_valid & (atomic_rsp_entry[`ETL2AXI_TT_BANK_SIZE_L +: `MIN_PER_N_L] == b[`MIN_PER_N_L-1:0]);
  
  cpu_etl2axi_trans_table_bank
  bank
  (
    // System signals
    .clock                ( clock                        ),
    .reset                ( reset                        ),
    // Allocate info
    .alloc_req_info       ( alloc_req_info               ),
    .alloc_req_data       ( alloc_req_data               ),
    // Read allocate port
    .alloc_rd_valid       ( bank_alloc_rd_valid[b]       ),
    .alloc_rd_entry       ( bank_alloc_rd_entry[b]       ),
    .alloc_rd_full        ( bank_alloc_rd_full[b]        ),
    // Write allocate port
    .alloc_wr_valid       ( bank_alloc_wr_valid[b]       ),
    .alloc_wr_entry       ( bank_alloc_wr_entry[b]       ),
    .alloc_wr_full        ( bank_alloc_wr_full[b]        ),
    // Atomic allocate port
    .alloc_at_valid       ( bank_alloc_at_valid[b]       ),
    .alloc_at_entry       ( bank_alloc_at_entry[b]       ),
    .alloc_at_full        ( bank_alloc_at_full[b]        ),
    // Deallocate port    
    .dealloc_entry        ( bank_dealloc_entry[b]        ),
    .dealloc_valid        ( bank_dealloc_valid[b]        ),
    // Read port
    .rd_req_info          ( bank_rd_req_info[b]          ),
    .rd_at_rd_data        ( bank_rd_at_rd_data[b]        ),
    .rd_at_retry_cnt      ( bank_rd_at_retry_cnt[b]      ),
    .rd_alloc_entries     ( bank_rd_alloc_entries[b]     ),
    // Conflict information
    .rd_conflict          ( bank_rd_conflict[b]          ),
    // Push port
    .push_entry           ( bank_push_entry[b]           ),
    .push                 ( bank_push[b]                 ),
    // Atomic response port
    .atomic_rsp_entry     ( bank_atomic_rsp_entry[b]     ),
    .atomic_rsp_is_rd     ( bank_atomic_rsp_is_rd[b]     ),
    .atomic_rsp_data      ( bank_atomic_rsp_data[b]      ),
    .atomic_rsp_valid_pre ( bank_atomic_rsp_valid_pre[b] ),
    .atomic_rsp_valid     ( bank_atomic_rsp_valid[b]     ),
    // Pop port
    .pop_entry            ( bank_pop_entry[b]            ),
    .pop_req              ( bank_pop_req[b]              ),
    .pop_is_atomic_wr     ( bank_pop_is_atomic_wr[b]     ),
    .pop_data             ( bank_pop_data[b]             ),
    .pop_at_rd_data       ( bank_pop_at_rd_data[b]       ),
    .valid                ( bank_valid[b]                ),
    .pop                  ( bank_pop[b]                  )
  );

  // Pop port
  assign bank_pop[b] = pop_d0_ready & bank_pop_grant[b];
end

//===================================================================
// Read port
//===================================================================
// Select bank according to MSBs of the entry ID
assign rd_bank_sel     = rd_entry[`ETL2AXI_TT_BANK_SIZE_L +: `MIN_PER_N_L];
// Select bank entry according to LSBs of the entry ID
assign rd_bank_entry   = rd_entry[0                       +: `ETL2AXI_TT_BANK_SIZE_L];
assign rd_req_info     = bank_rd_req_info[rd_bank_sel][rd_bank_entry];
assign rd_at_rd_data   = bank_rd_at_rd_data[rd_bank_sel][rd_bank_entry];
assign rd_at_retry_cnt = bank_rd_at_retry_cnt[rd_bank_sel][rd_bank_entry];

//===================================================================
// Conflict checking
//===================================================================
// Select bank according to MSBs of the entry ID
assign chk_bank_sel   = chk_entry[`ETL2AXI_TT_BANK_SIZE_L +: `MIN_PER_N_L];
// Select bank entry according to LSBs of the entry ID
assign chk_bank_entry = chk_entry[0                       +: `ETL2AXI_TT_BANK_SIZE_L];

always_comb
begin
  bank_own_chk_conflict_mask   = '0;
  bank_other_chk_conflict_mask = '0;

  // Compare against requests of the same bank
  for (integer unsigned i = 0; i < `ETL2AXI_TT_BANK_SIZE; i++)
                                     // Entry is allocated
    bank_own_chk_conflict_mask[i] =  bank_rd_alloc_entries[chk_bank_sel][i]
                                     // It is a different entry (cannot conflict with itself)
                                  && (i[0 +: `ETL2AXI_TT_BANK_SIZE_L] != chk_bank_entry)
                                     // The same cacheline is accessed
                                  && chk_req_info.address[`CPU_AXI_ADDR_WIDTH-1:`CPU_AXI_ADDR_OFFSET] == bank_rd_req_info[chk_bank_sel][i].address[`CPU_AXI_ADDR_WIDTH-1:`CPU_AXI_ADDR_OFFSET];

  for (integer unsigned b = 0; b < `MIN_PER_N-1; b++)
  begin
    // Obtain indices of the other banks
    if (b < chk_bank_sel)
      bank_other_idx[b] = b[`MIN_PER_N_L-1:0];
    else
      bank_other_idx[b] = b[`MIN_PER_N_L-1:0] + 1'b1;

    // Compare against write/atomic requests of the other banks
    for (integer unsigned i = `ETL2AXI_TT_BANK_SIZE-`ETL2AXI_TT_BANK_WR_ENTRIES; i < `ETL2AXI_TT_BANK_SIZE; i++)
                                             // Entry is allocated
      bank_other_chk_conflict_mask[b][i] =  bank_rd_alloc_entries[bank_other_idx[b]][i]
                                             // The same cacheline is accessed
                                          && (chk_req_info.address[`CPU_AXI_ADDR_WIDTH-1:`CPU_AXI_ADDR_OFFSET] == bank_rd_req_info[bank_other_idx[b]][i].address[`CPU_AXI_ADDR_WIDTH-1:`CPU_AXI_ADDR_OFFSET])
                                             // It is either a write or an atomic
                                          && (bank_rd_req_info[bank_other_idx[b]][i].op != ETL2AXI_REQ_OP_Read)
                                             // Reads are not compared against requests of other banks
                                          && (chk_req_info.op != ETL2AXI_REQ_OP_Read)
                                             // Writes are only compared against atomic requests of other banks
                                          && ((chk_req_info.op != ETL2AXI_REQ_OP_Write) || (bank_rd_req_info[bank_other_idx[b]][i].op == ETL2AXI_REQ_OP_Atomic));
  end
end

// Obtain conflict flags
assign bank_own_chk_conflict   = |bank_own_chk_conflict_mask;
assign bank_other_chk_conflict = |bank_other_chk_conflict_mask;

// Send to output
assign chk_conflict = bank_own_chk_conflict | bank_other_chk_conflict;

//===================================================================
// Flop to store conflict_mask
//===================================================================
// Assign conflicts to appropriate banks and store into flop
always_comb
begin
  bank_chk_conflict_mask = '0;

  // Assign conflicts for bank 0
  if (chk_bank_sel == (`MIN_PER_N_L)'(0))
    // If selected bank is bank 0, assign comparisons against the same bank
    bank_chk_conflict_mask[0] = bank_own_chk_conflict_mask;
  else
    // Otherwise, assign appropriate comparisons against other banks
    bank_chk_conflict_mask[0][`ETL2AXI_TT_BANK_SIZE-1 -: `ETL2AXI_TT_BANK_WR_ENTRIES] = bank_other_chk_conflict_mask[0];

  // Assign conflicts for banks 1-6
  for (integer unsigned b = 1; b < `MIN_PER_N; b++)
  begin
    if (b < chk_bank_sel)
      bank_chk_conflict_mask[b][`ETL2AXI_TT_BANK_SIZE-1 -: `ETL2AXI_TT_BANK_WR_ENTRIES] = bank_other_chk_conflict_mask[b];
    else if (b == chk_bank_sel)
      bank_chk_conflict_mask[b] = bank_own_chk_conflict_mask;
    else
      bank_chk_conflict_mask[b][`ETL2AXI_TT_BANK_SIZE-1 -: `ETL2AXI_TT_BANK_WR_ENTRIES] = bank_other_chk_conflict_mask[b-1];
  end

  // Assign conflicts for bank 7
  if (chk_bank_sel == (`MIN_PER_N_L)'(7))
    // If selected bank is bank 7, assign comparisons against the same bank
    bank_chk_conflict_mask[7] = bank_own_chk_conflict_mask;
  else
    // Otherwise, assign appropriate comparisons against other banks
    bank_chk_conflict_mask[7][`ETL2AXI_TT_BANK_SIZE-1 -: `ETL2AXI_TT_BANK_WR_ENTRIES] = bank_other_chk_conflict_mask[6];
end

assign chk_conflict_mask = bank_chk_conflict_mask;

//         CLK    RST    EN                          DOUT           DIN                 DEF
`RST_EN_FF(clock, reset, chk_valid || dealloc_valid, conflict_mask, conflict_mask_next, '0)

always_comb
begin
  conflict_mask_next = conflict_mask;
  
  if (chk_valid)
    conflict_mask_next[chk_entry] = chk_conflict_mask;

  // When an entry is deallocated, the conflict masks of all the other entries
  // must be updated, and the corresponding bit cleared in all of them.
  // This avoids that a new request allocates the same entry and the older
  // request conflicts with the newer one going to that entry.
  if (dealloc_valid)
  begin
    for (integer i = 0; i < `ETL2AXI_TT_SIZE; i++)
      conflict_mask_next[i][dealloc_entry] = 1'b0;
  end
end

// Map conflict mask to each bank
assign bank_rd_conflict_mask = conflict_mask;

//===================================================================
// Pop port
//===================================================================
// RR arbiter to select bank
arb_rr
#(
  .NUM_REQS ( `MIN_PER_N )
)
pop_arb
(
  .clock  ( clock          ),
  .reset  ( reset          ),
  .grants ( bank_pop_grant ),
  .pop    ( pop_d0_ready   ),
  .reqs   ( bank_valid     )
);

// Mux pop outputs from all the banks
always_comb
begin
  pop_d0_entry        = '0;
  pop_d0_req          = '0;
  pop_d0_is_atomic_wr = '0;
  pop_d0_data         = '0;
  pop_d0_at_rd_data   = '0;

  for (integer unsigned b = 0; b < `MIN_PER_N; b++)
  begin
    if (bank_pop_grant[b])
    begin
      // Prepend bank index as the MSBs of the entry ID
      pop_d0_entry        |= {b[`MIN_PER_N_L-1:0],bank_pop_entry[b]};
      pop_d0_req          |= bank_pop_req[b];
      pop_d0_is_atomic_wr |= bank_pop_is_atomic_wr[b];
      pop_d0_data         |= bank_pop_data[b];
      pop_d0_at_rd_data   |= bank_pop_at_rd_data[b];
    end
  end
end
assign pop_d0_valid = |bank_valid;
assign pop_d0_ready = pop_d1_ready | ~pop_d1_valid;
assign pop_d0_acc   = pop_d0_valid & pop_d0_ready;

//-------------------------------------------------------------------
// Atomic module
// - Combine pop_d0_data (operands) and pop_d0_at_rd_data (read data)
//   on atomic's exclusive write through the atomic module to
//   generate pop_d1_at_wr_data (updated data) to be written
//-------------------------------------------------------------------
cpu_etl2axi_atomic
atomic
(
  // System signals
  .clock          ( clock                                        ),
  // Input atomic request
  .d0_conf        ( pop_d0_req.amo_conf                          ),
  .d0_addr_offset ( pop_d0_req.address[`CPU_AXI_ADDR_OFFSET-1:0] ),
  .d0_rd_data     ( pop_d0_at_rd_data                            ),
  .d0_operands    ( pop_d0_data                                  ),
  .d0_valid       ( pop_d0_acc & pop_d0_is_atomic_wr             ),
  // Output atomic result
  .d1_wr_data     ( pop_d1_at_wr_data                            )
);

//-------------------------------------------------------------------
// Pop D1 stage
// - Register pop output from banks to wait for atomic module latency
//-------------------------------------------------------------------
//         CLK    RST    EN                                              DOUT                 DIN           RST
`RST_EN_FF(clock, reset, pop_d0_ready && (pop_d1_valid != pop_d0_valid), pop_d1_valid,        pop_d0_valid, '0)
`EN_FF    (clock,        pop_d0_acc,                                     pop_d1_entry,        pop_d0_entry)
`EN_FF    (clock,        pop_d0_acc,                                     pop_d1_req,          pop_d0_req)
`EN_FF    (clock,        pop_d0_acc,                                     pop_d1_is_atomic_wr, pop_d0_is_atomic_wr)
`EN_FF    (clock,        pop_d0_acc,                                     pop_d1_data,         pop_d0_data)

// Generate internal pop request
assign pop_d1_req_to_d2 = '{id      : pop_d1_req.id,
                            source  : pop_d1_req.source,
                            op      : pop_d1_req.op,
                            address : pop_d1_req.address,
                            size    : pop_d1_req.size,
                            strb    : pop_d1_req.strb,
                            qwen    : 4'b0, // Unused (only used for messages)
                            exop    : etl2axi_tt_amoexop_t'(pop_d1_is_atomic_wr),
                            op_err  : 1'b0  // Unused (only used for invalid op. codes)
                           };

// If it is an atomic's exclusive write, select output data from the atomic module
assign pop_d1_data_to_d2 = pop_d1_is_atomic_wr ? pop_d1_at_wr_data : pop_d1_data;

assign pop_d1_ready = pop_d2_ready | ~pop_d2_valid;
assign pop_d1_acc   = pop_d1_valid & pop_d1_ready;

//-------------------------------------------------------------------
// Pop D2 stage
// - Register atomic module output
//-------------------------------------------------------------------
//         CLK    RST    EN                                              DOUT          DIN           RST
`RST_EN_FF(clock, reset, pop_d1_ready && (pop_d2_valid != pop_d1_valid), pop_d2_valid, pop_d1_valid, '0)
`EN_FF    (clock,        pop_d1_acc,                                     pop_d2_entry, pop_d1_entry)
`EN_FF    (clock,        pop_d1_acc,                                     pop_d2_req,   pop_d1_req_to_d2)
`EN_FF    (clock,        pop_d1_acc,                                     pop_d2_data,  pop_d1_data_to_d2)

// Assign to output
assign pop_entry    = pop_d2_entry;
assign pop_req      = pop_d2_req;
assign pop_data     = pop_d2_data;
assign valid        = pop_d2_valid;
assign pop_d2_ready = pop;

//===================================================================
// LINT: Unused signals
//===================================================================
wire unused_ok = &{
`ifndef ET_ASCENT_LINT
   1'b0,
`endif
   pop_d1_req.amo_conf
};

endmodule
