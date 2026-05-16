// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_ch_dbg_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic rst_w_force_i,
  input  logic rst_d_force_i,

  input  logic bpam_gpio_ndmreset_i,
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
  input  logic dmctrl_ndmreset_i,
  input  logic dmctrl_clresethaltreq_i,
  input  logic dmctrl_setresethaltreq_i,
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

  input  logic status_monitor_enable_i,
  input  logic [neigh_voltage_cross_pkg::NeighSmGpioWidth-1:0] neigh_sm_gpio_i,
  input  logic [8*395-1:0] minion_dbg_signals_bits_i,
  input  logic [395-1:0]   tbox_dbg_signals_bits_i,
  input  logic [395-1:0]   neigh_ch_dbg_signals_bits_i,

  output logic [7:0]  orig_reset_w_per_minion_o,
  output logic [7:0]  new_reset_w_per_minion_o,
  output logic        orig_reset_w_neigh_o,
  output logic        new_reset_w_neigh_o,
  output logic [2:0]  orig_tbox_run_control_bits_o,
  output logic [2:0]  new_tbox_run_control_bits_o,
  output logic [63:0] orig_debug_minion_req_bits_o,
  output logic [63:0] new_debug_minion_req_bits_o,
  output logic [63:0] orig_esr_hastatus0_bits_o,
  output logic [63:0] new_esr_hastatus0_bits_o,
  output logic [47:0] orig_esr_hastatus1_bits_o,
  output logic [47:0] new_esr_hastatus1_bits_o,
  output logic [9:0]  orig_esr_and_or_tree_l0_bits_o,
  output logic [9:0]  new_esr_and_or_tree_l0_bits_o,
  output logic [7:0]  orig_minion_dbg_signals_en_o,
  output logic [7:0]  new_minion_dbg_signals_en_o,
  output logic [55:0] orig_minion_dbg_signals_mux_bits_o,
  output logic [55:0] new_minion_dbg_signals_mux_bits_o,
  output logic        orig_tbox_dbg_signals_en_o,
  output logic        new_tbox_dbg_signals_en_o,
  output logic [6:0]  orig_tbox_dbg_signals_mux_o,
  output logic [6:0]  new_tbox_dbg_signals_mux_o,
  output logic        orig_neigh_ch_dbg_signals_en_o,
  output logic        new_neigh_ch_dbg_signals_en_o,
  output logic [6:0]  orig_neigh_ch_dbg_signals_mux_o,
  output logic [6:0]  new_neigh_ch_dbg_signals_mux_o,
  output logic [394:0] orig_neigh_sm_signals_bits_o,
  output logic [394:0] new_neigh_sm_signals_bits_o
);

  localparam int unsigned NumMinions = neigh_voltage_cross_pkg::MinPerN;
  localparam int unsigned NumThreads = minion_pkg::NrThreads;
  localparam int unsigned MonitorWidth = $bits(neigh_sm_dbg_monitor_t);
  localparam int unsigned DebugReqWidth = $bits(minion_debug_in_t);

  logic reset_w;
  logic reset_d;
  logic rst_w_ni;
  logic rst_d_ni;

  bpam_run_control_neigh_t orig_bpam_run_control;
  tbox_bpam_run_control_t orig_tbox_bpam_run_control;
  bpam_rc_tbox_ack_t orig_tbox_bpam_run_control_ack;
  minion_debug_in_t [NumMinions-1:0] orig_debug_minion_req;
  minion_debug_out_t [NumMinions-1:0] orig_debug_minion_status;
  esr_ms_dmctrl_t orig_dmctrl;
  esr_hactrl_t orig_hactrl;
  esr_hastatus0_t orig_esr_hastatus0;
  esr_hastatus1_t orig_esr_hastatus1_min;
  esr_and_or_tree_L0_t orig_esr_and_or_tree_l0;
  neigh_sm_dbg_monitor_t [NumMinions-1:0] orig_minion_dbg_signals;
  logic [NumMinions-1:0][minion_pkg::NeighDebugSmMuxWidth-1:0]
      orig_minion_dbg_signals_mux;
  neigh_sm_dbg_monitor_t orig_tbox_dbg_signals;
  neigh_sm_dbg_monitor_t orig_neigh_ch_dbg_signals;
  neigh_sm_dbg_monitor_t orig_neigh_sm_signals;

  neigh_voltage_cross_pkg::bpam_run_control_neigh_t new_bpam_run_control;
  neigh_ch_dbg_pkg::tbox_bpam_run_control_t new_tbox_bpam_run_control;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t new_tbox_bpam_run_control_ack;
  minion_pkg::minion_debug_in_t [NumMinions-1:0] new_debug_minion_req;
  minion_pkg::minion_debug_out_t [NumMinions-1:0] new_debug_minion_status;
  esr_pkg::esr_ms_dmctrl_t new_dmctrl;
  esr_pkg::esr_hactrl_t new_hactrl;
  esr_pkg::esr_hastatus0_t new_esr_hastatus0;
  esr_pkg::esr_hastatus1_t new_esr_hastatus1_min;
  esr_pkg::esr_and_or_tree_l0_t new_esr_and_or_tree_l0;
  minion_pkg::neigh_sm_dbg_monitor_t [NumMinions-1:0] new_minion_dbg_signals;
  logic [NumMinions-1:0][minion_pkg::NeighDebugSmMuxWidth-1:0]
      new_minion_dbg_signals_mux;
  minion_pkg::neigh_sm_dbg_monitor_t new_tbox_dbg_signals;
  minion_pkg::neigh_sm_dbg_monitor_t new_neigh_ch_dbg_signals;
  minion_pkg::neigh_sm_dbg_monitor_t new_neigh_sm_signals;

  assign reset_w = !rst_ni | rst_w_force_i;
  assign reset_d = !rst_ni | rst_d_force_i;
  assign rst_w_ni = rst_ni & ~rst_w_force_i;
  assign rst_d_ni = rst_ni & ~rst_d_force_i;

  always_comb begin
    orig_bpam_run_control = '0;
    orig_bpam_run_control.gpio.ndmreset = bpam_gpio_ndmreset_i;
    orig_bpam_run_control.gpio.halt_req = bpam_gpio_halt_req_i;
    orig_bpam_run_control.gpio.resume_req = bpam_gpio_resume_req_i;
    orig_bpam_run_control.gpio.hartReset_req = bpam_gpio_hart_reset_req_i;
    orig_bpam_run_control.gpio.tbox_halt_req = bpam_gpio_tbox_halt_req_i;
    orig_bpam_run_control.gpio.tbox_resume_req = bpam_gpio_tbox_resume_req_i;
    orig_bpam_run_control.events.halt_req = bpam_events_halt_req_i;
    orig_bpam_run_control.events.resume_req = bpam_events_resume_req_i;
    orig_bpam_run_control.events.tbox_halt_req = bpam_events_tbox_halt_req_i;
    orig_bpam_run_control.events.tbox_resume_req = bpam_events_tbox_resume_req_i;
    orig_bpam_run_control.tbox_single_step = bpam_tbox_single_step_i;

    new_bpam_run_control = '0;
    new_bpam_run_control.gpio.ndmreset = bpam_gpio_ndmreset_i;
    new_bpam_run_control.gpio.halt_req = bpam_gpio_halt_req_i;
    new_bpam_run_control.gpio.resume_req = bpam_gpio_resume_req_i;
    new_bpam_run_control.gpio.hart_reset_req = bpam_gpio_hart_reset_req_i;
    new_bpam_run_control.gpio.tbox_halt_req = bpam_gpio_tbox_halt_req_i;
    new_bpam_run_control.gpio.tbox_resume_req = bpam_gpio_tbox_resume_req_i;
    new_bpam_run_control.events.halt_req = bpam_events_halt_req_i;
    new_bpam_run_control.events.resume_req = bpam_events_resume_req_i;
    new_bpam_run_control.events.tbox_halt_req = bpam_events_tbox_halt_req_i;
    new_bpam_run_control.events.tbox_resume_req = bpam_events_tbox_resume_req_i;
    new_bpam_run_control.tbox_single_step = bpam_tbox_single_step_i;

    orig_tbox_bpam_run_control_ack.halt_ack = tbox_ack_halt_i;
    orig_tbox_bpam_run_control_ack.resume_ack = tbox_ack_resume_i;
    new_tbox_bpam_run_control_ack.halt_ack = tbox_ack_halt_i;
    new_tbox_bpam_run_control_ack.resume_ack = tbox_ack_resume_i;

    orig_dmctrl.dmactive = dmctrl_dmactive_i;
    orig_dmctrl.ndmreset = dmctrl_ndmreset_i;
    orig_dmctrl.clresethaltreq = dmctrl_clresethaltreq_i;
    orig_dmctrl.setresethaltreq = dmctrl_setresethaltreq_i;
    orig_dmctrl.hasel = dmctrl_hasel_i;
    orig_dmctrl.ackhavereset = dmctrl_ackhavereset_i;
    orig_dmctrl.hartreset = dmctrl_hartreset_i;
    orig_dmctrl.resumereq = dmctrl_resumereq_i;
    orig_dmctrl.haltreq = dmctrl_haltreq_i;
    orig_dmctrl.write_enable = dmctrl_write_enable_i;
    new_dmctrl.dmactive = dmctrl_dmactive_i;
    new_dmctrl.ndmreset = dmctrl_ndmreset_i;
    new_dmctrl.clresethaltreq = dmctrl_clresethaltreq_i;
    new_dmctrl.setresethaltreq = dmctrl_setresethaltreq_i;
    new_dmctrl.hasel = dmctrl_hasel_i;
    new_dmctrl.ackhavereset = dmctrl_ackhavereset_i;
    new_dmctrl.hartreset = dmctrl_hartreset_i;
    new_dmctrl.resumereq = dmctrl_resumereq_i;
    new_dmctrl.haltreq = dmctrl_haltreq_i;
    new_dmctrl.write_enable = dmctrl_write_enable_i;

    orig_hactrl.resethalt = hactrl_resethalt_i;
    orig_hactrl.hartmask = hactrl_hartmask_i;
    orig_hactrl.hawindow = hactrl_hawindow_i;
    new_hactrl.resethalt = hactrl_resethalt_i;
    new_hactrl.hartmask = hactrl_hartmask_i;
    new_hactrl.hawindow = hactrl_hawindow_i;

    for (int unsigned min_idx = 0; min_idx < NumMinions; min_idx++) begin
      orig_debug_minion_status[min_idx].halted = dbg_halted_i[(NumThreads * min_idx) +: NumThreads];
      orig_debug_minion_status[min_idx].running = dbg_running_i[(NumThreads * min_idx) +: NumThreads];
      orig_debug_minion_status[min_idx].have_reset = dbg_have_reset_i[(NumThreads * min_idx) +: NumThreads];
      orig_debug_minion_status[min_idx].busy = dbg_busy_i[(NumThreads * min_idx) +: NumThreads];
      orig_debug_minion_status[min_idx].exception = dbg_exception_i[(NumThreads * min_idx) +: NumThreads];
      orig_debug_minion_status[min_idx].error = dbg_error_i[(NumThreads * min_idx) +: NumThreads];
      new_debug_minion_status[min_idx].halted = dbg_halted_i[(NumThreads * min_idx) +: NumThreads];
      new_debug_minion_status[min_idx].running = dbg_running_i[(NumThreads * min_idx) +: NumThreads];
      new_debug_minion_status[min_idx].have_reset = dbg_have_reset_i[(NumThreads * min_idx) +: NumThreads];
      new_debug_minion_status[min_idx].busy = dbg_busy_i[(NumThreads * min_idx) +: NumThreads];
      new_debug_minion_status[min_idx].exception = dbg_exception_i[(NumThreads * min_idx) +: NumThreads];
      new_debug_minion_status[min_idx].error = dbg_error_i[(NumThreads * min_idx) +: NumThreads];
      orig_minion_dbg_signals[min_idx] =
          minion_dbg_signals_bits_i[(MonitorWidth * min_idx) +: MonitorWidth];
      new_minion_dbg_signals[min_idx] =
          minion_dbg_signals_bits_i[(MonitorWidth * min_idx) +: MonitorWidth];
    end

    orig_tbox_dbg_signals = tbox_dbg_signals_bits_i;
    new_tbox_dbg_signals = tbox_dbg_signals_bits_i;
    orig_neigh_ch_dbg_signals = neigh_ch_dbg_signals_bits_i;
    new_neigh_ch_dbg_signals = neigh_ch_dbg_signals_bits_i;
  end

  always_comb begin
    orig_debug_minion_req_bits_o = '0;
    new_debug_minion_req_bits_o = '0;
    orig_minion_dbg_signals_mux_bits_o = '0;
    new_minion_dbg_signals_mux_bits_o = '0;
    for (int unsigned min_idx = 0; min_idx < NumMinions; min_idx++) begin
      orig_debug_minion_req_bits_o[(DebugReqWidth * min_idx) +: DebugReqWidth] =
          orig_debug_minion_req[min_idx];
      new_debug_minion_req_bits_o[(DebugReqWidth * min_idx) +: DebugReqWidth] =
          new_debug_minion_req[min_idx];
      orig_minion_dbg_signals_mux_bits_o[(minion_pkg::NeighDebugSmMuxWidth * min_idx) +: minion_pkg::NeighDebugSmMuxWidth] =
          orig_minion_dbg_signals_mux[min_idx];
      new_minion_dbg_signals_mux_bits_o[(minion_pkg::NeighDebugSmMuxWidth * min_idx) +: minion_pkg::NeighDebugSmMuxWidth] =
          new_minion_dbg_signals_mux[min_idx];
    end
  end

  assign orig_tbox_run_control_bits_o = orig_tbox_bpam_run_control;
  assign new_tbox_run_control_bits_o = new_tbox_bpam_run_control;
  assign orig_esr_hastatus0_bits_o = orig_esr_hastatus0;
  assign new_esr_hastatus0_bits_o = new_esr_hastatus0;
  assign orig_esr_hastatus1_bits_o = orig_esr_hastatus1_min;
  assign new_esr_hastatus1_bits_o = new_esr_hastatus1_min;
  assign orig_esr_and_or_tree_l0_bits_o = orig_esr_and_or_tree_l0;
  assign new_esr_and_or_tree_l0_bits_o = new_esr_and_or_tree_l0;
  assign orig_neigh_sm_signals_bits_o = orig_neigh_sm_signals;
  assign new_neigh_sm_signals_bits_o = new_neigh_sm_signals;

  neigh_ch_dbg_orig u_orig (
    .clock                         (clk_i),
    .reset_w                       (reset_w),
    .reset_d                       (reset_d),
    .reset_w_per_minion            (orig_reset_w_per_minion_o),
    .reset_w_neigh                 (orig_reset_w_neigh_o),
    .bpam_run_control              (orig_bpam_run_control),
    .tbox_bpam_run_control         (orig_tbox_bpam_run_control),
    .tbox_bpam_run_control_ack     (orig_tbox_bpam_run_control_ack),
    .debug_minion_req              (orig_debug_minion_req),
    .debug_minion_status           (orig_debug_minion_status),
    .dmctrl                        (orig_dmctrl),
    .esr_hastatus0                 (orig_esr_hastatus0),
    .esr_hastatus1_min             (orig_esr_hastatus1_min),
    .esr_hactrl                    (orig_hactrl),
    .esr_and_or_tree_L0            (orig_esr_and_or_tree_l0),
    .status_monitor_enable         (status_monitor_enable_i),
    .neigh_sm_gpio                 (neigh_sm_gpio_i),
    .minion_dbg_signals_en         (orig_minion_dbg_signals_en_o),
    .minion_dbg_signals_mux        (orig_minion_dbg_signals_mux),
    .minion_dbg_signals            (orig_minion_dbg_signals),
    .tbox_dbg_signals_en           (orig_tbox_dbg_signals_en_o),
    .tbox_dbg_signals_mux          (orig_tbox_dbg_signals_mux_o),
    .tbox_dbg_signals              (orig_tbox_dbg_signals),
    .neigh_ch_dbg_signals_en       (orig_neigh_ch_dbg_signals_en_o),
    .neigh_ch_dbg_signals_mux      (orig_neigh_ch_dbg_signals_mux_o),
    .neigh_ch_dbg_signals          (orig_neigh_ch_dbg_signals),
    .neigh_sm_signals              (orig_neigh_sm_signals)
  );

  neigh_ch_dbg u_new (
    .clk_i,
    .rst_w_ni,
    .rst_d_ni,
    .reset_w_per_minion_o          (new_reset_w_per_minion_o),
    .reset_w_neigh_o               (new_reset_w_neigh_o),
    .bpam_run_control_i            (new_bpam_run_control),
    .tbox_bpam_run_control_o       (new_tbox_bpam_run_control),
    .tbox_bpam_run_control_ack_i   (new_tbox_bpam_run_control_ack),
    .debug_minion_req_o            (new_debug_minion_req),
    .debug_minion_status_i         (new_debug_minion_status),
    .dmctrl_i                      (new_dmctrl),
    .esr_hastatus0_o               (new_esr_hastatus0),
    .esr_hastatus1_min_o           (new_esr_hastatus1_min),
    .esr_hactrl_i                  (new_hactrl),
    .esr_and_or_tree_l0_o          (new_esr_and_or_tree_l0),
    .status_monitor_enable_i,
    .neigh_sm_gpio_i,
    .minion_dbg_signals_en_o       (new_minion_dbg_signals_en_o),
    .minion_dbg_signals_mux_o      (new_minion_dbg_signals_mux),
    .minion_dbg_signals_i          (new_minion_dbg_signals),
    .tbox_dbg_signals_en_o         (new_tbox_dbg_signals_en_o),
    .tbox_dbg_signals_mux_o        (new_tbox_dbg_signals_mux_o),
    .tbox_dbg_signals_i            (new_tbox_dbg_signals),
    .neigh_ch_dbg_signals_en_o     (new_neigh_ch_dbg_signals_en_o),
    .neigh_ch_dbg_signals_mux_o    (new_neigh_ch_dbg_signals_mux_o),
    .neigh_ch_dbg_signals_i        (new_neigh_ch_dbg_signals),
    .neigh_sm_signals_o            (new_neigh_sm_signals)
  );

endmodule : cosim_neigh_ch_dbg_tb
