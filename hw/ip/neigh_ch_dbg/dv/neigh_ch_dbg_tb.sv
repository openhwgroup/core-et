// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_ch_dbg_tb
  import esr_pkg::*;
  import minion_pkg::*;
  import neigh_ch_dbg_pkg::*;
  import neigh_hv_logic_pkg::*;
  import neigh_voltage_cross_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,
  input  logic rst_w_force_i,
  input  logic rst_d_force_i,

  input  logic bpam_gpio_halt_req_i,
  input  logic bpam_gpio_resume_req_i,
  input  logic bpam_gpio_hart_reset_req_i,
  input  logic bpam_gpio_tbox_halt_req_i,
  input  logic bpam_gpio_tbox_resume_req_i,
  input  logic bpam_events_halt_req_i,
  input  logic bpam_events_resume_req_i,
  input  logic bpam_events_tbox_halt_req_i,
  input  logic bpam_events_tbox_resume_req_i,
  input  logic bpam_tbox_single_step_i,
  input  logic tbox_ack_halt_i,
  input  logic tbox_ack_resume_i,

  input  logic dmctrl_dmactive_i,
  input  logic dmctrl_hasel_i,
  input  logic dmctrl_ackhavereset_i,
  input  logic dmctrl_hartreset_i,
  input  logic dmctrl_resumereq_i,
  input  logic dmctrl_haltreq_i,
  input  logic dmctrl_write_enable_i,
  input  logic [15:0] hactrl_resethalt_i,
  input  logic [15:0] hactrl_hartmask_i,
  input  logic [15:0] hactrl_hawindow_i,

  input  logic [15:0] dbg_halted_i,
  input  logic [15:0] dbg_running_i,
  input  logic [15:0] dbg_have_reset_i,
  input  logic [15:0] dbg_busy_i,
  input  logic [15:0] dbg_exception_i,
  input  logic [15:0] dbg_error_i,

  input  logic        status_monitor_enable_i,
  input  logic [NeighSmGpioWidth-1:0] neigh_sm_gpio_i,
  input  logic [31:0] minion_mon_seed0_i,
  input  logic [31:0] minion_mon_seed1_i,
  input  logic [31:0] minion_mon_seed2_i,
  input  logic [31:0] minion_mon_seed3_i,
  input  logic [31:0] minion_mon_seed4_i,
  input  logic [31:0] minion_mon_seed5_i,
  input  logic [31:0] minion_mon_seed6_i,
  input  logic [31:0] minion_mon_seed7_i,
  input  logic [31:0] tbox_mon_seed_i,
  input  logic [31:0] neigh_ch_mon_seed_i,

  output logic [7:0]  reset_w_per_minion_o,
  output logic        reset_w_neigh_o,
  output logic        tbox_halt_req_o,
  output logic        tbox_resume_req_o,
  output logic        tbox_single_step_o,
  output logic [15:0] debug_halt_o,
  output logic [15:0] debug_resume_o,
  output logic [15:0] debug_resethalt_o,
  output logic [15:0] debug_ackhavereset_o,
  output logic [15:0] hastatus0_halted_o,
  output logic [15:0] hastatus0_running_o,
  output logic [15:0] hastatus0_resumeack_o,
  output logic [15:0] hastatus0_havereset_o,
  output logic [15:0] hastatus1_error_o,
  output logic [15:0] hastatus1_exception_o,
  output logic [15:0] hastatus1_busy_o,
  output logic        and_anyhalted_o,
  output logic        and_allhalted_o,
  output logic        and_anyrunning_o,
  output logic        and_allrunning_o,
  output logic        and_anyresumeack_o,
  output logic        and_allresumeack_o,
  output logic        and_anyhavereset_o,
  output logic        and_allhavereset_o,
  output logic        and_anyunavailable_o,
  output logic        and_anyselected_o,
  output logic [7:0]  minion_dbg_signals_en_o,
  output logic [55:0] minion_dbg_signals_mux_flat_o,
  output logic        tbox_dbg_signals_en_o,
  output logic [6:0]  tbox_dbg_signals_mux_o,
  output logic        neigh_ch_dbg_signals_en_o,
  output logic [6:0]  neigh_ch_dbg_signals_mux_o,
  output logic [31:0] neigh_sm_filter_low_o,
  output logic        neigh_sm_filter_valid_o,
  output logic [31:0] neigh_sm_match_low_o,
  output logic        neigh_sm_match_valid_o,
  output logic [31:0] neigh_sm_data_low_o,
  output logic        neigh_sm_data_valid_o
);

  localparam int unsigned NumMinions = neigh_voltage_cross_pkg::MinPerN;
  localparam int unsigned NumThreads = minion_pkg::NrThreads;

  logic rst_w_ni;
  logic rst_d_ni;
  bpam_run_control_neigh_t bpam_run_control;
  tbox_bpam_run_control_t tbox_bpam_run_control;
  bpam_rc_tbox_ack_t tbox_bpam_run_control_ack;
  esr_ms_dmctrl_t dmctrl;
  esr_hactrl_t hactrl;
  esr_hastatus0_t hastatus0;
  esr_hastatus1_t hastatus1;
  esr_and_or_tree_l0_t and_or_tree;
  minion_debug_in_t [NumMinions-1:0] debug_minion_req;
  minion_debug_out_t [NumMinions-1:0] debug_minion_status;
  neigh_sm_dbg_monitor_t [NumMinions-1:0] minion_dbg_signals;
  neigh_sm_dbg_monitor_t tbox_dbg_signals;
  neigh_sm_dbg_monitor_t neigh_ch_dbg_signals;
  neigh_sm_dbg_monitor_t neigh_sm_signals;
  logic [NumMinions-1:0][NeighDebugSmMuxWidth-1:0] minion_dbg_signals_mux;

  function automatic neigh_sm_dbg_monitor_t make_monitor(input logic [31:0] seed);
    neigh_sm_dbg_monitor_t mon;
    mon = '0;
    mon.filter_signals[31:0] = seed;
    mon.filter_valid = seed[0];
    mon.match_signals = {~seed, seed};
    mon.match_valid = seed[1];
    mon.data_signals = {seed ^ 32'hc0de_0003, seed ^ 32'hc0de_0002,
                        seed ^ 32'hc0de_0001, seed};
    mon.data_valid = seed[2];
    return mon;
  endfunction

  assign rst_w_ni = rst_ni & ~rst_w_force_i;
  assign rst_d_ni = rst_ni & ~rst_d_force_i;

  always_comb begin
    bpam_run_control = '0;
    bpam_run_control.gpio.halt_req = bpam_gpio_halt_req_i;
    bpam_run_control.gpio.resume_req = bpam_gpio_resume_req_i;
    bpam_run_control.gpio.hart_reset_req = bpam_gpio_hart_reset_req_i;
    bpam_run_control.gpio.tbox_halt_req = bpam_gpio_tbox_halt_req_i;
    bpam_run_control.gpio.tbox_resume_req = bpam_gpio_tbox_resume_req_i;
    bpam_run_control.events.halt_req = bpam_events_halt_req_i;
    bpam_run_control.events.resume_req = bpam_events_resume_req_i;
    bpam_run_control.events.tbox_halt_req = bpam_events_tbox_halt_req_i;
    bpam_run_control.events.tbox_resume_req = bpam_events_tbox_resume_req_i;
    bpam_run_control.tbox_single_step = bpam_tbox_single_step_i;

    tbox_bpam_run_control_ack.halt_ack = tbox_ack_halt_i;
    tbox_bpam_run_control_ack.resume_ack = tbox_ack_resume_i;

    dmctrl = '0;
    dmctrl.dmactive = dmctrl_dmactive_i;
    dmctrl.hasel = dmctrl_hasel_i;
    dmctrl.ackhavereset = dmctrl_ackhavereset_i;
    dmctrl.hartreset = dmctrl_hartreset_i;
    dmctrl.resumereq = dmctrl_resumereq_i;
    dmctrl.haltreq = dmctrl_haltreq_i;
    dmctrl.write_enable = dmctrl_write_enable_i;

    hactrl.resethalt = hactrl_resethalt_i;
    hactrl.hartmask = hactrl_hartmask_i;
    hactrl.hawindow = hactrl_hawindow_i;

    for (int unsigned min_idx = 0; min_idx < NumMinions; min_idx++) begin
      debug_minion_status[min_idx].halted = dbg_halted_i[(NumThreads * min_idx) +: NumThreads];
      debug_minion_status[min_idx].running = dbg_running_i[(NumThreads * min_idx) +: NumThreads];
      debug_minion_status[min_idx].have_reset = dbg_have_reset_i[(NumThreads * min_idx) +: NumThreads];
      debug_minion_status[min_idx].busy = dbg_busy_i[(NumThreads * min_idx) +: NumThreads];
      debug_minion_status[min_idx].exception = dbg_exception_i[(NumThreads * min_idx) +: NumThreads];
      debug_minion_status[min_idx].error = dbg_error_i[(NumThreads * min_idx) +: NumThreads];
    end

    minion_dbg_signals[0] = make_monitor(minion_mon_seed0_i);
    minion_dbg_signals[1] = make_monitor(minion_mon_seed1_i);
    minion_dbg_signals[2] = make_monitor(minion_mon_seed2_i);
    minion_dbg_signals[3] = make_monitor(minion_mon_seed3_i);
    minion_dbg_signals[4] = make_monitor(minion_mon_seed4_i);
    minion_dbg_signals[5] = make_monitor(minion_mon_seed5_i);
    minion_dbg_signals[6] = make_monitor(minion_mon_seed6_i);
    minion_dbg_signals[7] = make_monitor(minion_mon_seed7_i);
    tbox_dbg_signals = make_monitor(tbox_mon_seed_i);
    neigh_ch_dbg_signals = make_monitor(neigh_ch_mon_seed_i);
  end

  always_comb begin
    debug_halt_o = '0;
    debug_resume_o = '0;
    debug_resethalt_o = '0;
    debug_ackhavereset_o = '0;
    minion_dbg_signals_mux_flat_o = '0;
    for (int unsigned min_idx = 0; min_idx < NumMinions; min_idx++) begin
      debug_halt_o[(NumThreads * min_idx) +: NumThreads] = debug_minion_req[min_idx].halt;
      debug_resume_o[(NumThreads * min_idx) +: NumThreads] = debug_minion_req[min_idx].resume;
      debug_resethalt_o[(NumThreads * min_idx) +: NumThreads] = debug_minion_req[min_idx].resethalt;
      debug_ackhavereset_o[(NumThreads * min_idx) +: NumThreads] = debug_minion_req[min_idx].ackhavereset;
      minion_dbg_signals_mux_flat_o[(NeighDebugSmMuxWidth * min_idx) +: NeighDebugSmMuxWidth] =
          minion_dbg_signals_mux[min_idx];
    end
  end

  assign tbox_halt_req_o = tbox_bpam_run_control.halt_req;
  assign tbox_resume_req_o = tbox_bpam_run_control.resume_req;
  assign tbox_single_step_o = tbox_bpam_run_control.single_step;
  assign hastatus0_halted_o = hastatus0.halted;
  assign hastatus0_running_o = hastatus0.running;
  assign hastatus0_resumeack_o = hastatus0.resumeack;
  assign hastatus0_havereset_o = hastatus0.havereset;
  assign hastatus1_error_o = hastatus1.error;
  assign hastatus1_exception_o = hastatus1.exception;
  assign hastatus1_busy_o = hastatus1.busy;
  assign and_anyhalted_o = and_or_tree.anyhalted;
  assign and_allhalted_o = and_or_tree.allhalted;
  assign and_anyrunning_o = and_or_tree.anyrunning;
  assign and_allrunning_o = and_or_tree.allrunning;
  assign and_anyresumeack_o = and_or_tree.anyresumeack;
  assign and_allresumeack_o = and_or_tree.allresumeack;
  assign and_anyhavereset_o = and_or_tree.anyhavereset;
  assign and_allhavereset_o = and_or_tree.allhavereset;
  assign and_anyunavailable_o = and_or_tree.anyunavailable;
  assign and_anyselected_o = and_or_tree.anyselected;
  assign neigh_sm_filter_low_o = neigh_sm_signals.filter_signals[31:0];
  assign neigh_sm_filter_valid_o = neigh_sm_signals.filter_valid;
  assign neigh_sm_match_low_o = neigh_sm_signals.match_signals[31:0];
  assign neigh_sm_match_valid_o = neigh_sm_signals.match_valid;
  assign neigh_sm_data_low_o = neigh_sm_signals.data_signals[31:0];
  assign neigh_sm_data_valid_o = neigh_sm_signals.data_valid;

  neigh_ch_dbg u_dut (
    .clk_i,
    .rst_w_ni,
    .rst_d_ni,
    .reset_w_per_minion_o,
    .reset_w_neigh_o,
    .bpam_run_control_i             (bpam_run_control),
    .tbox_bpam_run_control_o        (tbox_bpam_run_control),
    .tbox_bpam_run_control_ack_i    (tbox_bpam_run_control_ack),
    .debug_minion_req_o             (debug_minion_req),
    .debug_minion_status_i          (debug_minion_status),
    .dmctrl_i                       (dmctrl),
    .esr_hastatus0_o                (hastatus0),
    .esr_hastatus1_min_o            (hastatus1),
    .esr_hactrl_i                   (hactrl),
    .esr_and_or_tree_l0_o           (and_or_tree),
    .status_monitor_enable_i,
    .neigh_sm_gpio_i,
    .minion_dbg_signals_en_o,
    .minion_dbg_signals_mux_o       (minion_dbg_signals_mux),
    .minion_dbg_signals_i           (minion_dbg_signals),
    .tbox_dbg_signals_en_o,
    .tbox_dbg_signals_mux_o,
    .tbox_dbg_signals_i             (tbox_dbg_signals),
    .neigh_ch_dbg_signals_en_o,
    .neigh_ch_dbg_signals_mux_o,
    .neigh_ch_dbg_signals_i         (neigh_ch_dbg_signals),
    .neigh_sm_signals_o             (neigh_sm_signals)
  );

endmodule : neigh_ch_dbg_tb
