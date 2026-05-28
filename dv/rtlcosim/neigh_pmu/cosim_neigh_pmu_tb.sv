// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_pmu_tb
  import minion_pkg::*;
  import neigh_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [(MinPerN*PmuMinionCountersNum)-1:0] pmu_count_up_bits_i,
  input  logic [(MinPerN*NrThreads*PmuCountersSelectBits)-1:0] pmu_read_sel_bits_i,
  input  logic [(MinPerN*PmuTotalCountersNum)-1:0] pmu_write_en_bits_i,
  input  logic [(MinPerN*XregSize)-1:0] pmu_write_data_bits_i,
  input  logic [(MinPerN*PmuNeighEventCntSelBits)-1:0] pmu_neigh_event_sel_bits_i,
  input  logic [PmuNeighEvents:1] pmu_neigh_events_i,
  input  logic pmu_disable_clock_i,

  output logic [(MinPerN*NrThreads*XregSize)-1:0] orig_pmu_read_data_bits_o,
  output logic [(MinPerN*NrThreads*XregSize)-1:0] new_pmu_read_data_bits_o
);

  logic [MinPerN-1:0][PmuMinionCountersNum-1:0] pmu_count_up;
  logic [MinPerN-1:0][NrThreads-1:0][PmuCountersSelectBits-1:0] pmu_read_sel;
  logic [MinPerN-1:0][PmuTotalCountersNum-1:0] pmu_write_en;
  logic [MinPerN-1:0][XregSize-1:0] pmu_write_data;
  logic [MinPerN-1:0][PmuNeighEventCntSelBits-1:0] pmu_neigh_event_sel;
  logic [MinPerN-1:0][NrThreads-1:0][XregSize-1:0] orig_pmu_read_data;
  logic [MinPerN-1:0][NrThreads-1:0][XregSize-1:0] new_pmu_read_data;
  esr_pkg::esr_pmu_ctrl_t new_pmu_ctrl;
  esr_pmu_ctrl_t orig_pmu_ctrl;

  always_comb begin
    for (int unsigned m = 0; m < MinPerN; m++) begin
      pmu_count_up[m] = pmu_count_up_bits_i[m*PmuMinionCountersNum +: PmuMinionCountersNum];
      pmu_write_en[m] = pmu_write_en_bits_i[m*PmuTotalCountersNum +: PmuTotalCountersNum];
      pmu_write_data[m] = pmu_write_data_bits_i[m*XregSize +: XregSize];
      pmu_neigh_event_sel[m] = pmu_neigh_event_sel_bits_i[m*PmuNeighEventCntSelBits +: PmuNeighEventCntSelBits];
      for (int unsigned t = 0; t < NrThreads; t++) begin
        pmu_read_sel[m][t] = pmu_read_sel_bits_i[(m*NrThreads+t)*PmuCountersSelectBits +: PmuCountersSelectBits];
        orig_pmu_read_data_bits_o[(m*NrThreads+t)*XregSize +: XregSize] = orig_pmu_read_data[m][t];
        new_pmu_read_data_bits_o[(m*NrThreads+t)*XregSize +: XregSize] = new_pmu_read_data[m][t];
      end
    end
    new_pmu_ctrl = '{disable_clock: pmu_disable_clock_i};
    orig_pmu_ctrl = esr_pmu_ctrl_t'(new_pmu_ctrl);
  end

  neigh_pmu_orig u_orig (
    .clock               (clk_i),
    .reset               (!rst_ni),
    .pmu_count_up        (pmu_count_up),
    .pmu_read_data       (orig_pmu_read_data),
    .pmu_read_sel        (pmu_read_sel),
    .pmu_write_en        (pmu_write_en),
    .pmu_write_data      (pmu_write_data),
    .pmu_neigh_event_sel (pmu_neigh_event_sel),
    .pmu_neigh_events    (pmu_neigh_events_i),
    .pmu_ctrl            (orig_pmu_ctrl)
  );

  neigh_pmu u_new (
    .clk_i,
    .rst_ni,
    .pmu_count_up_i        (pmu_count_up),
    .pmu_read_data_o       (new_pmu_read_data),
    .pmu_read_sel_i        (pmu_read_sel),
    .pmu_write_en_i        (pmu_write_en),
    .pmu_write_data_i      (pmu_write_data),
    .pmu_neigh_event_sel_i (pmu_neigh_event_sel),
    .pmu_neigh_events_i    (pmu_neigh_events_i),
    .pmu_ctrl_i            (new_pmu_ctrl)
  );

endmodule : cosim_neigh_pmu_tb
