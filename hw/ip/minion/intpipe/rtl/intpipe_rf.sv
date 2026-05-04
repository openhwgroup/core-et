// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_rf — Integer register file wrapper.
//
// 2 read ports, 1 write port. Uses prim_rf_2r1w with hardwired-zero
// entries for x0 (per-thread) and parallel read for x31 (per-thread).
//
// Line-by-line translation of: intpipe_rf (etcore-soc)

module intpipe_rf
  import minion_pkg::*;
#(
  parameter int unsigned ReadPorts  = 2,
  parameter int unsigned WritePorts = 1
) (
  // System signals
  input  logic                                           clk_i,
  // Read ports
  /* verilator lint_off UNUSEDSIGNAL */  // rd_en present for interface compatibility but RF reads are always combinational
  input  logic [ReadPorts-1:0]                           rd_en,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic                                           rd_thread_id,
  input  logic [ReadPorts-1:0][XregAddrSize-1:0]         rd_addr,
  output logic [ReadPorts-1:0][XregSize-1:0]             rd_data,
  output logic [NrThreadsDefault-1:0][XregSize-1:0]      wb_x31_reg,
  // Write ports
  input  logic [WritePorts-1:0]                          wr_en,
  input  logic [WritePorts-1:0]                          wr_en_early,
  input  logic                                           wr_thread_id,
  input  logic [WritePorts-1:0][XregAddrSize-1:0]        wr_addr,
  input  logic [WritePorts-1:0][XregSize-1:0]            wr_data
);

  ////////////////////////////////////////////////////////////////////////////////
  // Preview the early write-enable signal for the RF's staged write-data capture.
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // Data storage
  // Bootrom should write a 0 to entry 0
  ////////////////////////////////////////////////////////////////////////////////

  // ZERO: entries 0 and 32 are hardwired to zero (x0 per thread)
  // PARALLEL: entries 31 and 63 are parallel-read (x31 per thread)
  localparam int unsigned RfEntries = NrThreadsDefault * 32;
  localparam logic [RfEntries-1:0] RfZero     = {NrThreadsDefault{32'b1}};
  localparam logic [RfEntries-1:0] RfParallel = {NrThreadsDefault{32'h80000000}};
  localparam int unsigned RfParallelW = XregSize * NrThreadsDefault;

  prim_rf_2r1w_preview #(
    .Width       (XregSize),
    .Entries     (RfEntries),
    .Zero        (RfZero),
    .Parallel    (RfParallel),
    .ParallelW   (RfParallelW),
    .Level2CkGate(1)
  ) u_rf (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    // Read port A
    .rd_addr_a_i          ({rd_thread_id, rd_addr[0]}),
    .rd_data_a_o          (rd_data[0]),
    // Read port B
    .rd_addr_b_i          ({rd_thread_id, rd_addr[1]}),
    .rd_data_b_o          (rd_data[1]),
    // Parallel read
    .rd_par_o             (wb_x31_reg),
    // Write port
    .wr_en_i              (wr_en[0]),
    .wr_data_en_1p_next_i (wr_en_early[0]),
    .wr_addr_i            ({wr_thread_id, wr_addr[0]}),
    .wr_data_i            (wr_data[0])
  );

endmodule
