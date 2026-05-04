// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
// This is a shire-neigh lo voltage crossing
module neigh_lo_voltage_cross #(
`ifdef SIM_ALLOW_STUB
  parameter NUM_ELEMS = 0,
  parameter [`MIN_PER_N-1:0] STUB_MINIONS = '0
`else
  parameter NUM_ELEMS = 0
`endif
  ) (
  // System signals
  input  logic                                  reset_w,
  input  logic                                  reset_d,
  input  logic                                  clock,

  // ESRs
  input  logic                                  esr_icache_prefetch_done,
  output logic                                  esr_icache_prefetch_done_sync,
  input  logic                                  esr_icache_err_detected,
  output logic                                  esr_icache_err_detected_sync,
  input  logic                                  esr_icache_err_logged,
  output logic                                  esr_icache_err_logged_sync,

  // TBOX run control signals
  input   bpam_rc_tbox_ack_t                    bpam_rc_tbox_ack, // from TBOX to neigh channel
  output  bpam_rc_tbox_ack_t                    bpam_rc_tbox_ack_sync, // from neigh channel to shire

  // Debug signals for SM
  input  neigh_sm_dbg_monitor_t                 neigh_sm_signals,
  output neigh_sm_dbg_monitor_t                 neigh_sm_signals_sync,

  // Fast Local Barrier request neigh-L2
  input  logic                                  flb_neigh_l2_req_valid,
  output logic                                  flb_neigh_l2_req_valid_sync,
  input  logic [`CSR_FL_BARRIER_RANGE]          flb_neigh_l2_req_data,
  output logic [`CSR_FL_BARRIER_RANGE]          flb_neigh_l2_req_data_sync,
  // icache SRAM blocks
  input  logic                                  icache_f2_sram_req_write,
  output logic                                  icache_f2_sram_req_write_sync,
  input  logic [`ICACHE_SRAM_ADD_WIDTH-1:0]     icache_f2_sram_req_addr,
  output logic [`ICACHE_SRAM_ADD_WIDTH-1:0]     icache_f2_sram_req_addr_sync,
  input  logic                                  icache_f2_sram_req_valid,
  output logic                                  icache_f2_sram_req_valid_sync,
  input  logic                                  icache_f2_sram_req_ready,
  output logic                                  icache_f2_sram_req_ready_sync,
  // debug status tree
  input  esr_and_or_tree_L0_t                   esr_and_or_tree_L0,
  output esr_and_or_tree_L0_t                   esr_and_or_tree_L0_sync
  );

  localparam ESR_AND_OR_TREE_L0_SZ = $bits(esr_and_or_tree_L0);

`ifndef SIM_ALLOW_STUB
  localparam [`MIN_PER_N-1:0] STUB_MINIONS = '0;
`endif

  ////////////////////////////////////////////////////////////////////////////////
  // icache sram req fifo wr
  ////////////////////////////////////////////////////////////////////////////////
  assign icache_f2_sram_req_ready_sync = icache_f2_sram_req_ready | ~icache_f2_sram_req_valid_sync;

  //          CLK    RST      EN                                                                                      DOUT                           DIN                       DEF
  `RST_EN_FF (clock, reset_w, (icache_f2_sram_req_valid != icache_f2_sram_req_valid_sync) & icache_f2_sram_req_ready, icache_f2_sram_req_valid_sync, icache_f2_sram_req_valid, 1'b0)
  `EN_FF     (clock,          icache_f2_sram_req_valid & icache_f2_sram_req_ready,                                    icache_f2_sram_req_addr_sync,  icache_f2_sram_req_addr)
  `EN_FF     (clock,          icache_f2_sram_req_valid & icache_f2_sram_req_ready,                                    icache_f2_sram_req_write_sync, icache_f2_sram_req_write)

  ////////////////////////////////////////////////////////////////////////////////
  // tbox us monitor
  ////////////////////////////////////////////////////////////////////////////////
  generate
    if ( &STUB_MINIONS[`MIN_PER_N-1:0] == 1'b0 ) begin: bpam_rc_tbox_ack_vxsing
      `FF(clock, bpam_rc_tbox_ack_sync, bpam_rc_tbox_ack)
    end else begin
       assign bpam_rc_tbox_ack_sync = '0;
    end
  endgenerate

  ////////////////////////////////////////////////////////////////////////////////
  // flb l2 neigh req fifo wr
  ////////////////////////////////////////////////////////////////////////////////
  logic flb_neigh_l2_req_vcfifo_wr_lov_full_unused_ok;
  logic flb_neigh_l2_req_vcfifo_wr_lov_full_next_unused_ok;
  logic flb_neigh_l2_req_vcfifo_wr_lov_valid_next_unused_ok;
  gen_fifo #(
   .ELEM_SIZE ( $bits( flb_neigh_l2_req_data )),
   .NUM_ELEMS ( NUM_ELEMS                     )
  ) flb_neigh_l2_req_vcfifo_wr_lov (
   // System signals
   .clock      ( clock                       ),
   .reset      ( reset_w                     ),
   // Push data
   .push_early ( 1'b0                        ), // Unused
   .push       ( flb_neigh_l2_req_valid      ),
   .push_data  ( flb_neigh_l2_req_data       ),
   .full       ( flb_neigh_l2_req_vcfifo_wr_lov_full_unused_ok ), // Unused
   .full_next  ( flb_neigh_l2_req_vcfifo_wr_lov_full_next_unused_ok ), // Unused
   // Pop data
   .pop        ( flb_neigh_l2_req_valid_sync ),
   .pop_data   ( flb_neigh_l2_req_data_sync  ),
   .valid      ( flb_neigh_l2_req_valid_sync ),
   .valid_next ( flb_neigh_l2_req_vcfifo_wr_lov_valid_next_unused_ok )  // Unused
  );

  ////////////////////////////////////////////////////////////////////////////////
  // status monitor fifo wr
  ////////////////////////////////////////////////////////////////////////////////
  //       CLK    RST      DOUT                   DIN               DEF
  `RST_FF (clock, reset_d, neigh_sm_signals_sync, neigh_sm_signals, '0)

  ////////////////////////////////////////////////////////////////////////////////
  // debug status and/or tree fifo wr
  ////////////////////////////////////////////////////////////////////////////////
  //          CLK    RST                EN                                             DOUT                     DIN                 DEF
  `RST_EN_FF (clock, reset_w | reset_d, esr_and_or_tree_L0 != esr_and_or_tree_L0_sync, esr_and_or_tree_L0_sync, esr_and_or_tree_L0, '0)

  ////////////////////////////////////////////////////////////////////////////////
  // esr icache prefetch done reg
  ////////////////////////////////////////////////////////////////////////////////
  //          CLK    RST      EN                                                      DOUT                           DIN                       DEF
  `RST_EN_FF (clock, reset_w, esr_icache_prefetch_done^esr_icache_prefetch_done_sync, esr_icache_prefetch_done_sync, esr_icache_prefetch_done, 1'b1)

  ////////////////////////////////////////////////////////////////////////////////
  // esr icache err detected reg
  ////////////////////////////////////////////////////////////////////////////////
  //          CLK    RST      EN                                                    DOUT                          DIN                      DEF
  `RST_EN_FF (clock, reset_w, esr_icache_err_detected^esr_icache_err_detected_sync, esr_icache_err_detected_sync, esr_icache_err_detected, '0)

  ////////////////////////////////////////////////////////////////////////////////
  // esr icache err logged reg
  ////////////////////////////////////////////////////////////////////////////////
  //          CLK    RST      EN                                                DOUT                        DIN                    DEF
  `RST_EN_FF (clock, reset_w, esr_icache_err_logged^esr_icache_err_logged_sync, esr_icache_err_logged_sync, esr_icache_err_logged, '0)

endmodule
