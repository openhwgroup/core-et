// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cpu_etl2axi_trans_table_bank
(
  // System signals
  input  logic clock,
  input  logic reset,
  // Allocate info
  input  etl2axi_tt_req_info_t           alloc_req_info,
  input  logic [`CPU_AXI_DATA_WIDTH-1:0] alloc_req_data,
  // Read allocate port
  input  logic                               alloc_rd_valid,
  output logic [`ETL2AXI_TT_BANK_SIZE_L-1:0] alloc_rd_entry,
  output logic                               alloc_rd_full,
  // Write allocate port
  input  logic                               alloc_wr_valid,
  output logic [`ETL2AXI_TT_BANK_SIZE_L-1:0] alloc_wr_entry,
  output logic                               alloc_wr_full,
  // Atomic allocate port
  input  logic                               alloc_at_valid,
  output logic [`ETL2AXI_TT_BANK_SIZE_L-1:0] alloc_at_entry,
  output logic                               alloc_at_full,
  // Deallocate port
  input  logic [`ETL2AXI_TT_BANK_SIZE_L-1:0] dealloc_entry, 
  input  logic                               dealloc_valid,
  // Read port
  output etl2axi_tt_req_info_t [`ETL2AXI_TT_BANK_SIZE-1:0]          rd_req_info,
  output logic [`ETL2AXI_TT_BANK_SIZE-1:0][`CPU_AXI_DATA_WIDTH-1:0] rd_at_rd_data,
  output logic [`ETL2AXI_TT_BANK_SIZE-1:0][1:0]                     rd_at_retry_cnt,
  output logic [`ETL2AXI_TT_BANK_SIZE-1:0]                          rd_alloc_entries,
  // Conflict information
  input  logic [`ETL2AXI_TT_BANK_SIZE-1:0] rd_conflict,
  // Push port
  input  logic [`ETL2AXI_TT_BANK_SIZE_L-1:0] push_entry,
  input  logic                               push,
  // Atomic response port
  input  logic [`ETL2AXI_TT_BANK_SIZE_L-1:0] atomic_rsp_entry,
  input  logic                               atomic_rsp_is_rd,
  input  logic [`CPU_AXI_DATA_WIDTH-1:0]     atomic_rsp_data,
  input  logic                               atomic_rsp_valid_pre,
  input  logic                               atomic_rsp_valid,
  // Pop port
  output logic [`ETL2AXI_TT_BANK_SIZE_L-1:0] pop_entry,
  output etl2axi_tt_req_info_t               pop_req,
  output logic                               pop_is_atomic_wr,
  output logic [`CPU_AXI_DATA_WIDTH-1:0]     pop_data,
  output logic [`CPU_AXI_DATA_WIDTH-1:0]     pop_at_rd_data,
  output logic                               valid,
  input  logic                               pop
);

//===================================================================
// INTERNAL DECLARATIONS
//===================================================================
// Allocated request entries
logic [`ETL2AXI_TT_BANK_SIZE-1:0] alloc_req_entries, alloc_req_entries_next;
logic                             rd_entry_available_found;
logic [`ETL2AXI_TT_BANK_SIZE-1:0] alloc_rd_mask;
logic                             wr_entry_available_found;
logic [`ETL2AXI_TT_BANK_SIZE-1:0] alloc_wr_mask;
logic                             at_entry_available_found;
logic [`ETL2AXI_TT_BANK_SIZE-1:0] alloc_at_mask;
// States
etl2axi_tt_state_t [`ETL2AXI_TT_BANK_SIZE-1:0] entry_states, entry_states_next;
// Atomic retry counters
logic [`ETL2AXI_TT_BANK_SIZE-1:`ETL2AXI_TT_BANK_SIZE-`ETL2AXI_TT_BANK_AT_ENTRIES][1:0] atomic_retry_cnt, atomic_retry_cnt_next;
// Request Buffer
etl2axi_tt_entry_t [`ETL2AXI_TT_BANK_SIZE-1:0] req_rd_data_a;
logic                                          req_wr_en_a;
logic [`ETL2AXI_TT_BANK_SIZE_L-1:0]            req_wr_addr_a;
etl2axi_tt_entry_t                             req_wr_data_a;
// Pop port
logic [`ETL2AXI_TT_BANK_SIZE-1:0]   ready_entries;
logic                               entry_prio_up;
logic [`ETL2AXI_TT_BANK_SIZE-1:0]   entry_prio_mask;
logic [`ETL2AXI_TT_BANK_SIZE_L-1:0] entry_winner;
// Allocated data entries
logic [`ETL2AXI_TT_BANK_DATA_ENTRIES-1:0]        alloc_data_entries, alloc_data_entries_next;
logic                                            first_data_entry_available_found;
logic                                            second_data_entry_available_found;
logic [1:0][`ETL2AXI_TT_BANK_DATA_ENTRIES-1:0]   alloc_data_mask;
logic [1:0][`ETL2AXI_TT_BANK_DATA_ENTRIES_L-1:0] alloc_data_entry;
// Data Buffer
logic [`ETL2AXI_TT_BANK_DATA_ENTRIES-1:0][`CPU_AXI_DATA_WIDTH-1:0] data_rd_data;
logic [`ETL2AXI_TT_BANK_DATA_ENTRIES_L-1:0]                        data_rd_addr_a;
logic [`CPU_AXI_DATA_WIDTH-1:0]                                    data_rd_data_a;
logic [`ETL2AXI_TT_BANK_DATA_ENTRIES_L-1:0]                        data_rd_addr_b;
logic [`CPU_AXI_DATA_WIDTH-1:0]                                    data_rd_data_b;
logic                                                              data_wr_en_a;
logic [`ETL2AXI_TT_BANK_DATA_ENTRIES_L-1:0]                        data_wr_addr_a;
logic [`CPU_AXI_DATA_WIDTH-1:0]                                    data_wr_data_a;
logic                                                              data_wr_en_early_b;
logic                                                              data_wr_en_early_p2_b;
logic                                                              data_wr_en_b;
logic [`ETL2AXI_TT_BANK_DATA_ENTRIES_L-1:0]                        data_wr_addr_b;
logic [`CPU_AXI_DATA_WIDTH-1:0]                                    data_wr_data_b;

// For read allocation, we don't need any available data entries
assign alloc_rd_full = &alloc_req_entries;
// For write allocation, we need one available data entry
assign alloc_wr_full = &alloc_req_entries[`ETL2AXI_TT_BANK_SIZE-1 -: `ETL2AXI_TT_BANK_WR_ENTRIES] || ~first_data_entry_available_found;
// For atomic allocation, we need two available data entries
assign alloc_at_full = &alloc_req_entries[`ETL2AXI_TT_BANK_SIZE-1 -: `ETL2AXI_TT_BANK_AT_ENTRIES] || ~second_data_entry_available_found;

//===================================================================
// REQUEST BUFFER
//===================================================================

//-------------------------------------------------------------------
// Allocated request entries
//-------------------------------------------------------------------
//         CLK    RST    EN                                                                   DOUT               DIN                     DEF
`RST_EN_FF(clock, reset, alloc_rd_valid || alloc_wr_valid || alloc_at_valid || dealloc_valid, alloc_req_entries, alloc_req_entries_next, '0)

always_comb
begin
  alloc_req_entries_next = alloc_req_entries;

  // Set if allocation is requested and available entry
  if (alloc_rd_valid)
    alloc_req_entries_next |= alloc_rd_mask;
  else if (alloc_wr_valid)
    alloc_req_entries_next |= alloc_wr_mask;
  else if (alloc_at_valid)
    alloc_req_entries_next |= alloc_at_mask;

  // Clear entries when de-allocated
  if (dealloc_valid)
    alloc_req_entries_next[dealloc_entry] = 1'b0;
end

// Obtain first entry available for each type of allocation
always_comb
begin
  rd_entry_available_found = 1'b0;
  alloc_rd_mask            = '0;
  alloc_rd_entry           = '0;
  wr_entry_available_found = 1'b0;
  alloc_wr_mask            = '0;
  alloc_wr_entry           = '0;
  at_entry_available_found = 1'b0;
  alloc_at_mask            = '0;
  alloc_at_entry           = '0;

  // The bank is divided into three sections:
  // - The first section can only be allocated by read requests
  // - The second section can be allocated by read and write requests
  // - The third section can be allocated by all type of requests (reads, writes and atomics)

  // Check for availability of read entries through all the sections
  for (integer unsigned i = 0; i < `ETL2AXI_TT_BANK_SIZE; i++)
  begin
    if (!alloc_req_entries[i] && !rd_entry_available_found)
    begin
      alloc_rd_mask[i] = 1'b1;
      alloc_rd_entry  |= i[`ETL2AXI_TT_BANK_SIZE_L-1:0];
    end

    rd_entry_available_found |= ~alloc_req_entries[i];
  end

  // Check for availability of write entries through the second and third sections
  for (integer unsigned i = `ETL2AXI_TT_BANK_SIZE-`ETL2AXI_TT_BANK_WR_ENTRIES; i < `ETL2AXI_TT_BANK_SIZE; i++)
  begin
     if (!alloc_req_entries[i] && !wr_entry_available_found)
     begin
        alloc_wr_mask[i] = 1'b1;
        alloc_wr_entry  |= i[`ETL2AXI_TT_BANK_SIZE_L-1:0];
     end

     wr_entry_available_found |= ~alloc_req_entries[i];
  end

  // Check for availability of atomic entries only through the third section
  for (integer unsigned i = `ETL2AXI_TT_BANK_SIZE-`ETL2AXI_TT_BANK_AT_ENTRIES; i < `ETL2AXI_TT_BANK_SIZE; i++)
  begin
     if (!alloc_req_entries[i] && !at_entry_available_found)
     begin
        alloc_at_mask[i] = 1'b1;
        alloc_at_entry  |= i[`ETL2AXI_TT_BANK_SIZE_L-1:0];
     end

     at_entry_available_found |= ~alloc_req_entries[i];
  end
end

//-------------------------------------------------------------------
// States
//-------------------------------------------------------------------
//      CLK    RST    DOUT          DIN                DEF
`RST_FF(clock, reset, entry_states, entry_states_next, ETL2AXI_TT_ST_Pending)

always_comb
begin
  entry_states_next = entry_states;

  if (alloc_rd_valid)
    entry_states_next[alloc_rd_entry] = ETL2AXI_TT_ST_Pending;
  if (alloc_wr_valid)
    entry_states_next[alloc_wr_entry] = ETL2AXI_TT_ST_Pending;
  if (alloc_at_valid)
    entry_states_next[alloc_at_entry] = ETL2AXI_TT_ST_Pending;

  // When a request is pushed into the Transaction Table,
  // it is marked as Conflict
  if (push)
    entry_states_next[push_entry] = ETL2AXI_TT_ST_Conflict;

  // If the entry had a conflict, set it to ready to be replayed
  // when all the conflicting entries have been deallocated
  for (integer unsigned i = 0; i < `ETL2AXI_TT_BANK_SIZE; i++)
  begin
    if ((entry_states[i] == ETL2AXI_TT_ST_Conflict) && !rd_conflict[i])
      entry_states_next[i] = ETL2AXI_TT_ST_Ready;
  end

  if (atomic_rsp_valid)
  begin
    // Set state to AtomicWriteReady when the exclusive read response is received
    if (atomic_rsp_is_rd)
      entry_states_next[atomic_rsp_entry] = ETL2AXI_TT_ST_AtomicWriteReady;
    // Set state to ready when a failed exclusive write response is received,
    // so that the whole atomic operation (exclusive read+write) is retried
    else
      entry_states_next[atomic_rsp_entry] = ETL2AXI_TT_ST_Ready;
  end

  // Set state to Pending when request is replayed
  if (pop && valid)
    entry_states_next[pop_entry] = ETL2AXI_TT_ST_Pending;
end

//-------------------------------------------------------------------
// Atomic retry counters
//-------------------------------------------------------------------
//         CLK    RST    EN                                  DOUT              DIN                    DEF
`RST_EN_FF(clock, reset, alloc_at_valid || atomic_rsp_valid, atomic_retry_cnt, atomic_retry_cnt_next, '0)

always_comb
begin
  atomic_retry_cnt_next = atomic_retry_cnt;

  // On allocation, reset counters to 0
  if (alloc_at_valid)
    atomic_retry_cnt_next[alloc_at_entry] = '0;

  // On a failed atomic's exclusive write response,
  // increment the corresponding retry counter
  if (atomic_rsp_valid && !atomic_rsp_is_rd)
    atomic_retry_cnt_next[atomic_rsp_entry] = atomic_retry_cnt[atomic_rsp_entry] + 1'b1;
end

//-------------------------------------------------------------------
// Request buffer
//-------------------------------------------------------------------

// Write interface
assign req_wr_en_a   = alloc_rd_valid | alloc_wr_valid | alloc_at_valid;
assign req_wr_addr_a = alloc_rd_valid ? alloc_rd_entry
                     : alloc_wr_valid ? alloc_wr_entry
                     :                  alloc_at_entry;
assign req_wr_data_a = '{req        : alloc_req_info,
                         data_entry : alloc_data_entry
                        };

rf_latch_1r_1w_par
#(
  .WIDTH           ( $bits(etl2axi_tt_entry_t) ),
  .ENTRIES         ( `ETL2AXI_TT_BANK_SIZE     ),
  .LEVEL2_CLK_GATE ( 1                         )
)
tt_req_buffer
(
  // System signals
  .clk             ( clock         ),
  .test_en         ( 1'b0          ),
  // Read port
  .rd_data_a       ( req_rd_data_a ),
  // Write port
  .wr_en_a         ( req_wr_en_a   ),
  .wr_data_a_en_1p ( 1'b1          ),
  .wr_addr_a       ( req_wr_addr_a ),
  .wr_data_a       ( req_wr_data_a )
);

//-------------------------------------------------------------------
// Read port
//-------------------------------------------------------------------
// Expose internal contents of the Transaction Table bank
always_comb
begin
  for (integer unsigned i = 0; i < `ETL2AXI_TT_BANK_SIZE; i++)
    rd_req_info[i] = req_rd_data_a[i].req;
end;
// Expose original read cacheline for atomics
always_comb
begin
  for (integer unsigned i = 0; i < `ETL2AXI_TT_BANK_SIZE; i++)
    rd_at_rd_data[i] = data_rd_data[req_rd_data_a[i].data_entry[1]];
end
// Expose atomic retry counters
always_comb
begin
  // Fill non-existent counter outputs for non-atomic entries with zeros
  for (integer unsigned i = 0; i < `ETL2AXI_TT_BANK_SIZE-`ETL2AXI_TT_BANK_AT_ENTRIES; i++)
    rd_at_retry_cnt[i] = '0;
  // Connect actual retry counters for atomic entries
  for (integer unsigned i = `ETL2AXI_TT_BANK_SIZE-`ETL2AXI_TT_BANK_AT_ENTRIES; i < `ETL2AXI_TT_BANK_SIZE; i++)
    rd_at_retry_cnt[i] = atomic_retry_cnt[i];
end

// Expose array of allocated request entries
assign rd_alloc_entries = alloc_req_entries;

//-------------------------------------------------------------------
// Pop port
//-------------------------------------------------------------------
logic [`ETL2AXI_TT_BANK_SIZE-1:0] grant_unused_ok;

// Update priority when an entry is allocated
assign entry_prio_up = alloc_rd_valid | alloc_wr_valid | alloc_at_valid;
always_comb
begin
  entry_prio_mask = '0;

  if (alloc_rd_valid)
    entry_prio_mask[alloc_rd_entry] = 1'b1;
  if (alloc_wr_valid)
    entry_prio_mask[alloc_wr_entry] = 1'b1;
  if (alloc_at_valid)
    entry_prio_mask[alloc_at_entry] = 1'b1;
end

// Check which entries are ready to be replayed
always_comb
begin
  for (integer unsigned i = 0; i < `ETL2AXI_TT_BANK_SIZE; i++)
    ready_entries[i] = (entry_states[i] == ETL2AXI_TT_ST_Ready)
                    || (entry_states[i] == ETL2AXI_TT_ST_AtomicWriteReady);
end

// Select the oldest one
arb_lru_delayed
#(
   .NUM_CLIENTS ( `ETL2AXI_TT_BANK_SIZE )
)
entry_ready_arb
(
  // System signals
  .clock         ( clock           ),
  .reset         ( reset           ),
  // Update priority
  .update_prio   ( entry_prio_up   ),
  .grant_delayed ( entry_prio_mask ),
  // Bidding
  .bid           ( ready_entries   ),
  .stall         ( 1'b0            ),
  // Grant
  .grant         ( grant_unused_ok ),
  .winner        ( entry_winner    )
);

// Send to output
assign pop_entry        = entry_winner;
assign pop_req          = req_rd_data_a[entry_winner].req;
assign pop_is_atomic_wr = entry_states[entry_winner] == ETL2AXI_TT_ST_AtomicWriteReady;
assign pop_data         = data_rd_data_a;
assign pop_at_rd_data   = data_rd_data_b;
assign valid            = |ready_entries;

//===================================================================
// DATA BUFFER
//===================================================================

//-------------------------------------------------------------------
// Allocated data entries
//-------------------------------------------------------------------
//         CLK    RST    EN                                                                   DOUT                DIN                      DEF
`RST_EN_FF(clock, reset, alloc_rd_valid || alloc_wr_valid || alloc_at_valid || dealloc_valid, alloc_data_entries, alloc_data_entries_next, '0)

always_comb
begin
  alloc_data_entries_next = alloc_data_entries;

  // If write allocation is requested, allocate one data entry
  if (alloc_wr_valid || alloc_at_valid)
    alloc_data_entries_next |= alloc_data_mask[0];
  // If atomic allocation is requested, allocate two data entries
  if (alloc_at_valid)
    alloc_data_entries_next |= alloc_data_mask[1];

  // When a request entry is deallocated
  // also deallocate the corresponding data entries
  if (dealloc_valid)
  begin
    // For writes, deallocate one data entry
    if (req_rd_data_a[dealloc_entry].req.op != ETL2AXI_REQ_OP_Read)
      alloc_data_entries_next[req_rd_data_a[dealloc_entry].data_entry[0]] = 1'b0;
    // For atomics, deallocate two data entries
    if (req_rd_data_a[dealloc_entry].req.op == ETL2AXI_REQ_OP_Atomic)
      alloc_data_entries_next[req_rd_data_a[dealloc_entry].data_entry[1]] = 1'b0;
  end
end

// Obtain first two available entries
always_comb
begin
  first_data_entry_available_found  = 1'b0;
  second_data_entry_available_found = 1'b0;
  alloc_data_mask                   = '0;
  alloc_data_entry                  = '0;

  for (integer unsigned i = 0; i < `ETL2AXI_TT_BANK_DATA_ENTRIES; i++)
  begin
    // Find first available entry
    if (!alloc_data_entries[i] && !first_data_entry_available_found)
    begin
      alloc_data_mask[0][i] = 1'b1;
      alloc_data_entry[0]  |= i[`ETL2AXI_TT_BANK_DATA_ENTRIES_L-1:0];
    end

    // Find second available entry
    if (!alloc_data_entries[i] && first_data_entry_available_found && !second_data_entry_available_found)
    begin
      alloc_data_mask[1][i] = 1'b1;
      alloc_data_entry[1]  |= i[`ETL2AXI_TT_BANK_DATA_ENTRIES_L-1:0];
    end

    second_data_entry_available_found |= first_data_entry_available_found & ~alloc_data_entries[i];
    first_data_entry_available_found  |= ~alloc_data_entries[i];
  end
end

//-------------------------------------------------------------------
// Data buffer
//-------------------------------------------------------------------

// Write interface A
// Used for allocation
assign data_wr_en_a   = alloc_wr_valid | alloc_at_valid;
assign data_wr_addr_a = alloc_data_entry[0];
assign data_wr_data_a = alloc_req_data;

// Write interface B
// Used for atomics to store original read cacheline
assign data_wr_en_early_b = atomic_rsp_valid_pre;
assign data_wr_en_b       = atomic_rsp_valid & atomic_rsp_is_rd;
assign data_wr_addr_b     = req_rd_data_a[atomic_rsp_entry].data_entry[1];
assign data_wr_data_b     = atomic_rsp_data;

`LATCH_P2 (clock, data_wr_en_early_p2_b, data_wr_en_early_b)

rf_latch_1r_2w_par
#(
  .WIDTH           ( `CPU_AXI_DATA_WIDTH           ),
  .ENTRIES         ( `ETL2AXI_TT_BANK_DATA_ENTRIES ),
  .LEVEL2_CLK_GATE ( 1                             )
)
tt_data_buffer
(
  // System signals
  .clk             ( clock                 ),
  .test_en         ( 1'b0                  ),
  // Read port
  .rd_data_a       ( data_rd_data          ),
  // Write port A
  .wr_en_a         ( data_wr_en_a          ),
  .wr_data_a_en_1p ( 1'b1                  ),
  .wr_addr_a       ( data_wr_addr_a        ),
  .wr_data_a       ( data_wr_data_a        ),
  // Write port B
  .wr_en_b         ( data_wr_en_b          ),
  .wr_data_b_en_1p ( data_wr_en_early_p2_b ),
  .wr_addr_b       ( data_wr_addr_b        ),
  .wr_data_b       ( data_wr_data_b        )
);

// Read interface A
assign data_rd_addr_a = req_rd_data_a[entry_winner].data_entry[0];
assign data_rd_data_a = data_rd_data[data_rd_addr_a];

// Read interface B
assign data_rd_addr_b = req_rd_data_a[entry_winner].data_entry[1];
assign data_rd_data_b = data_rd_data[data_rd_addr_b];

endmodule
