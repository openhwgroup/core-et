// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module icache_lru_array (
  // System signals
  input  logic                         clock,
  input  logic                         reset,
  // Victim read port
  input  logic                         f0_valid,
  input  logic [`ICACHE_SET_RANGE]     f0_access_set,
  input  logic [`ICACHE_SET_RANGE]     f0_access_set_next,
  input  logic                         f0_set_valid,
  output logic [`ICACHE_WAYS-1:0]      f1_victim,
  // Update port
  input  logic                         f1_update_en,
  input  logic [`ICACHE_SET_RANGE]     f1_update_set,
  input  logic [`ICACHE_WAYS-1:0]      f1_update_access_way
  );

  // INTERNAL DECLARATIONS
  logic [`ICACHE_LRU_SET_RANGE] f1_new_lru_data; // New LRU data from F1

  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // F0 Stage
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_LRU_SET_RANGE] f0_lru_data; // LRU data read from LRU array in F0 stage
  logic [`ICACHE_LRU_SET_RANGE] f0_lru_data_byp; // LRU data after muxing the F1 to F0 bypass
  logic [`ICACHE_WAYS-1:0]      f0_victim; // Victim selected in F0

  always_comb begin
    // Bypass status from F1 if read and write are to the same set
    if (f1_update_en && (f1_update_set == f0_access_set))
      f0_lru_data_byp = f1_new_lru_data;
    else
      f0_lru_data_byp = f0_lru_data;
  end

  // Selects the way to replace based on LRU
  assign f0_victim = icache_lru_victim(f0_lru_data_byp);

  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // F1 Stage
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_LRU_SET_RANGE] f0_lru_data_rf; // LRU data read from RF latches before intialization
  logic [`ICACHE_SETS-1:0]      f1_lru_set_initialized;
  logic [`ICACHE_SETS-1:0]      f1_lru_set_initialized_next;
  logic [`ICACHE_LRU_SET_RANGE] f1_lru_data; // LRU state for the transaction in F1

  // CLK    RST    EN            DOUT                    DIN                          RST
  `RST_EN_FF (clock, reset, f1_update_en, f1_lru_set_initialized, f1_lru_set_initialized_next, '0)
  `EN_FF     (clock,        f0_valid,     f1_lru_data,            f0_lru_data_byp)
  `EN_FF     (clock,        f0_valid,     f1_victim,              f0_victim)

  // Mark written sets as accessed
  always_comb begin
    f1_lru_set_initialized_next = f1_lru_set_initialized;
    f1_lru_set_initialized_next[f1_update_set] = 1'b1;
  end

  // Obtain new LRU status base on LRU and accessed ways
  assign f1_new_lru_data = icache_lru_update(f1_lru_data,f1_update_access_way);
  
  icache_lru_state_array icache_lru_state_array (
    // System signals
    .clk             ( clock                 ),
    // Read port
    .rd_enable       ( f0_set_valid          ),
    .rd_addr         ( f0_access_set_next    ),
    .rd_data         ( f0_lru_data_rf        ),
    // Inputs
    .wr_enable       ( f1_update_en          ),
    .wr_addr         ( f1_update_set         ),
    .wr_data         ( f1_new_lru_data       )
  );

  // While LRU state is not initialized, read zeros
  assign f0_lru_data = f1_lru_set_initialized[f0_access_set] ? f0_lru_data_rf : '0; 

endmodule
