// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_ch_dbg
  import esr_pkg::*;
  import minion_pkg::*;
  import neigh_ch_dbg_pkg::*;
  import neigh_hv_logic_pkg::*;
  import neigh_voltage_cross_pkg::*;
#(
  parameter int unsigned NumMinions              = neigh_voltage_cross_pkg::MinPerN,
  parameter int unsigned NumThreads       = minion_pkg::NrThreads,
  parameter int unsigned ThreadCount      = NumMinions * NumThreads,
  parameter int unsigned SmTargetSelWidth = neigh_voltage_cross_pkg::NeighSmTargetSel,
  parameter int unsigned SmGpioWidth      = neigh_voltage_cross_pkg::NeighSmGpioWidth,
  parameter int unsigned SmMuxWidth       = minion_pkg::NeighDebugSmMuxWidth
) (
  input  logic                                                 clk_i,
  input  logic                                                 rst_w_ni,
  input  logic                                                 rst_d_ni,

  output logic [NumMinions-1:0]                                 reset_w_per_minion_o,
  output logic                                                 reset_w_neigh_o,

  input  bpam_run_control_neigh_t                              bpam_run_control_i,
  output tbox_bpam_run_control_t                               tbox_bpam_run_control_o,
  input  bpam_rc_tbox_ack_t                                    tbox_bpam_run_control_ack_i,
  output minion_debug_in_t [NumMinions-1:0]                     debug_minion_req_o,
  input  minion_debug_out_t [NumMinions-1:0]                    debug_minion_status_i,

  /* verilator lint_off UNUSEDSIGNAL */  // Original neigh_ch_dbg ignores ndmreset/clr/set reset DMCTRL fields.
  input  esr_ms_dmctrl_t                                       dmctrl_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output esr_hastatus0_t                                       esr_hastatus0_o,
  output esr_hastatus1_t                                       esr_hastatus1_min_o,
  input  esr_hactrl_t                                          esr_hactrl_i,
  output esr_and_or_tree_l0_t                                  esr_and_or_tree_l0_o,

  input  logic                                                 status_monitor_enable_i,
  input  logic [SmGpioWidth-1:0]                          neigh_sm_gpio_i,
  output logic [NumMinions-1:0]                                 minion_dbg_signals_en_o,
  output logic [NumMinions-1:0][SmMuxWidth-1:0]       minion_dbg_signals_mux_o,
  input  neigh_sm_dbg_monitor_t [NumMinions-1:0]                minion_dbg_signals_i,
  output logic                                                 tbox_dbg_signals_en_o,
  output logic [SmMuxWidth-1:0]                      tbox_dbg_signals_mux_o,
  input  neigh_sm_dbg_monitor_t                                tbox_dbg_signals_i,
  output logic                                                 neigh_ch_dbg_signals_en_o,
  output logic [SmMuxWidth-1:0]                      neigh_ch_dbg_signals_mux_o,
  input  neigh_sm_dbg_monitor_t                                neigh_ch_dbg_signals_i,
  output neigh_sm_dbg_monitor_t                                neigh_sm_signals_o
);

  localparam int unsigned HalfNumMinions       = NumMinions / 2;
  localparam int unsigned MinionSelWidth      = $clog2(NumMinions);
  localparam int unsigned HalfMinionSelWidth  = MinionSelWidth - 1;
  localparam int unsigned HartStatusWidth     = 16;

  logic [ThreadCount-1:0] sel_harts;
  esr_and_or_tree_l0_t               esr_and_or_tree_l0_d;
  esr_and_or_tree_l0_t               esr_and_or_tree_l0_q;
  esr_hastatus0_t                    esr_hastatus0;
  esr_hastatus1_t                    esr_hastatus1_min;

  logic                        tbox_gpio_resume_q;
  bpam_rc_tbox_ack_t           tbox_bpam_run_control_ack_q;
  tbox_bpam_run_control_t      tbox_bpam_run_control_d;
  tbox_bpam_run_control_t      tbox_bpam_run_control_q;

  minion_debug_in_t [NumMinions-1:0] debug_rc_latch;
  minion_debug_in_t [NumMinions-1:0] debug_minion_req_q;

  /* verilator lint_off UNOPTFLAT */  // Packed esr_hastatus0 fields are assigned independently; running feeds the resume FSM while resumeack is a separate output field.
  resume_state_e [NumMinions-1:0][NumThreads-1:0] resume_state_d;
  resume_state_e [NumMinions-1:0][NumThreads-1:0] resume_state_q;
  logic [NumMinions-1:0][NumThreads-1:0]           resume_state_en;
  logic [NumMinions-1:0]                          event_resume_req_level_d;
  logic [NumMinions-1:0]                          event_resume_req_level_q;
  logic [NumMinions-1:0]                          event_resume_req_level_en;
  logic [NumMinions-1:0][NumThreads-1:0]           resume_req_level;
  logic [NumMinions-1:0][NumThreads-1:0]           resume_performed;
  logic [NumMinions-1:0][NumThreads-1:0]           resumeack;
  logic [NumMinions-1:0][NumThreads-1:0]           resumeack_reg_d;
  logic [NumMinions-1:0][NumThreads-1:0]           resumeack_reg_q;
  logic [NumMinions-1:0][NumThreads-1:0]           resumeack_clear;
  logic [NumMinions-1:0][NumThreads-1:0]           resumeack_en;
  logic [NumMinions-1:0][NumThreads-1:0]           resume_to_core;
  /* verilator lint_on UNOPTFLAT */

  logic [SmGpioWidth-1:0]                    neigh_sm_gpio_q;
  logic [SmTargetSelWidth-1:0]               dbg_target_sel;
  logic [31:0]                                    dbg_target_sel_ext;
  logic [SmMuxWidth-1:0]                dbg_mux_sel;
  logic [SmMuxWidth-1:0]                minion_north_dbg_signals_mux_d;
  logic [SmMuxWidth-1:0]                minion_north_dbg_signals_mux_q;
  logic [SmMuxWidth-1:0]                minion_south_dbg_signals_mux_d;
  logic [SmMuxWidth-1:0]                minion_south_dbg_signals_mux_q;
  logic [NumMinions-1:0]                           minion_dbg_signals_en_d;
  logic                                           tbox_dbg_signals_en_d;
  logic                                           neigh_ch_dbg_signals_en_d;
  neigh_sm_dbg_monitor_t [NumMinions-1:0]          minion_dbg_signals_q;
  neigh_sm_dbg_monitor_t                          minion_north_dbg_signals;
  neigh_sm_dbg_monitor_t                          minion_north_dbg_signals_q;
  neigh_sm_dbg_monitor_t                          minion_south_dbg_signals;
  neigh_sm_dbg_monitor_t                          minion_south_dbg_signals_q;
  neigh_sm_dbg_monitor_t                          tbox_dbg_signals_q;
  neigh_sm_dbg_monitor_t                          neigh_ch_dbg_signals_q;

  assign sel_harts = ({ThreadCount{dmctrl_i.hasel}} & esr_hactrl_i.hawindow) |
                     esr_hactrl_i.hartmask;

  assign esr_and_or_tree_l0_o = esr_and_or_tree_l0_q;
  assign esr_hastatus0_o      = esr_hastatus0;
  assign esr_hastatus1_min_o  = esr_hastatus1_min;

  always_comb begin
    esr_and_or_tree_l0_d.anyhalted = |(esr_hastatus0.halted & sel_harts);
    esr_and_or_tree_l0_d.allhalted =
        ((esr_hastatus0.halted & sel_harts) == sel_harts) && |sel_harts;
    esr_and_or_tree_l0_d.anyrunning = |(esr_hastatus0.running & sel_harts);
    esr_and_or_tree_l0_d.allrunning =
        ((esr_hastatus0.running & sel_harts) == sel_harts) && |sel_harts;
    esr_and_or_tree_l0_d.anyresumeack = |(esr_hastatus0.resumeack & sel_harts);
    esr_and_or_tree_l0_d.allresumeack =
        ((esr_hastatus0.resumeack & sel_harts) == sel_harts) && |sel_harts;
    esr_and_or_tree_l0_d.anyhavereset = |(esr_hastatus0.havereset & sel_harts);
    esr_and_or_tree_l0_d.allhavereset =
        ((esr_hastatus0.havereset & sel_harts) == sel_harts) && |sel_harts;
    esr_and_or_tree_l0_d.anyunavailable =
        |((~(esr_hastatus0.halted | esr_hastatus0.running)) & sel_harts);
    esr_and_or_tree_l0_d.anyselected = |sel_harts;
  end

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      esr_and_or_tree_l0_q <= '0;
    end else begin
      esr_and_or_tree_l0_q <= esr_and_or_tree_l0_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      tbox_gpio_resume_q <= 1'b0;
    end else if (tbox_gpio_resume_q != bpam_run_control_i.gpio.resume_req) begin
      // Preserve the original expression: the TBOX GPIO resume edge detector stores
      // the minion GPIO resume request, not the TBOX GPIO resume request.
      tbox_gpio_resume_q <= bpam_run_control_i.gpio.resume_req;
    end
  end

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      tbox_bpam_run_control_ack_q <= '0;
    end else begin
      tbox_bpam_run_control_ack_q <= tbox_bpam_run_control_ack_i;
    end
  end

  always_comb begin
    logic halt_req;
    logic resume_req;

    halt_req = bpam_run_control_i.events.tbox_halt_req ? 1'b1 :
               bpam_run_control_i.gpio.tbox_halt_req ? 1'b1 :
               tbox_bpam_run_control_ack_q.halt_ack ? 1'b0 :
               tbox_bpam_run_control_q.halt_req;

    resume_req = bpam_run_control_i.events.tbox_resume_req ? 1'b1 :
                 (bpam_run_control_i.gpio.tbox_resume_req && !tbox_gpio_resume_q) ? 1'b1 :
                 tbox_bpam_run_control_ack_q.resume_ack ? 1'b0 :
                 tbox_bpam_run_control_q.resume_req;

    tbox_bpam_run_control_d.single_step = bpam_run_control_i.tbox_single_step;
    tbox_bpam_run_control_d.halt_req = halt_req | bpam_run_control_i.gpio.tbox_halt_req;
    tbox_bpam_run_control_d.resume_req = resume_req | bpam_run_control_i.gpio.tbox_resume_req;

    reset_w_neigh_o = !rst_w_ni;
  end

  always_ff @(posedge clk_i) begin
    tbox_bpam_run_control_q <= tbox_bpam_run_control_d;
  end

  assign tbox_bpam_run_control_o = tbox_bpam_run_control_q;

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      debug_minion_req_q <= '0;
    end else begin
      debug_minion_req_q <= debug_rc_latch;
    end
  end

  assign debug_minion_req_o = debug_minion_req_q;

  always_comb begin
    resume_state_d              = resume_state_q;
    resume_state_en             = '0;
    resumeack_reg_d             = resumeack_reg_q;
    resumeack_en                = '0;
    resumeack_clear             = '0;

    for (int unsigned min_idx = 0; min_idx < NumMinions; min_idx++) begin
      for (int unsigned thread_idx = 0; thread_idx < NumThreads; thread_idx++) begin
        unique case (resume_state_q[min_idx][thread_idx])
          ResumeIdle: begin
            if (resume_req_level[min_idx][thread_idx]) begin
              resume_state_d[min_idx][thread_idx] = ResumeResuming;
              resume_state_en[min_idx][thread_idx] = 1'b1;
              resumeack_en[min_idx][thread_idx] = 1'b1;
              resumeack_reg_d[min_idx][thread_idx] = 1'b0;
              resumeack_clear[min_idx][thread_idx] = 1'b1;
            end
          end
          ResumeResuming: begin
            // Preserve the original per-minion indexing bug: every minion's resume FSM
            // observes only the minion-0 running bits for the matching thread.
            if (esr_hastatus0.running[thread_idx]) begin
              resume_state_d[min_idx][thread_idx] = ResumeWait;
              resume_state_en[min_idx][thread_idx] = 1'b1;
            end
          end
          ResumeWait: begin
            if (!(|resume_req_level[min_idx])) begin
              resume_state_d[min_idx][thread_idx] = ResumeIdle;
              resume_state_en[min_idx][thread_idx] = 1'b1;
            end
            resumeack_reg_d[min_idx][thread_idx] = resume_performed[min_idx][thread_idx];
            resumeack_en[min_idx][thread_idx] = 1'b1;
          end
          default: begin
            // Preserve the original no-default case behavior: invalid states hold.
          end
        endcase
      end
    end
  end

  always_comb begin
    for (int unsigned min_idx = 0; min_idx < NumMinions; min_idx++) begin
      for (int unsigned thread_idx = 0; thread_idx < NumThreads; thread_idx++) begin
        resume_performed[min_idx][thread_idx] =
            (resume_state_q[min_idx][thread_idx] == ResumeWait);
        resumeack[min_idx][thread_idx] =
            resumeack_reg_q[min_idx][thread_idx] & ~resumeack_clear[min_idx][thread_idx];
      end
    end
  end

  always_comb begin
    resume_req_level = '0;
    event_resume_req_level_d = event_resume_req_level_q;
    event_resume_req_level_en = '0;

    for (int unsigned min_idx = 0; min_idx < NumMinions; min_idx++) begin
      logic [NumThreads-1:0] hart_slice;

      hart_slice = sel_harts[(NumThreads * min_idx) +: NumThreads];

      resume_req_level[min_idx] =
          ({NumThreads{dmctrl_i.dmactive}} &
           {NumThreads{dmctrl_i.resumereq && dmctrl_i.write_enable}} &
           hart_slice) |
          {NumThreads{bpam_run_control_i.gpio.resume_req}} |
          {NumThreads{event_resume_req_level_q[min_idx]}};

      event_resume_req_level_d[min_idx] = (|debug_minion_status_i[min_idx].running) ? 1'b0 :
          (bpam_run_control_i.events.resume_req | event_resume_req_level_q[min_idx]);
      event_resume_req_level_en[min_idx] =
          (event_resume_req_level_d[min_idx] != event_resume_req_level_q[min_idx]);
    end
  end

  always_comb begin
    esr_hastatus0     = '0;
    esr_hastatus1_min = '0;
    debug_rc_latch    = '0;
    reset_w_per_minion_o = '0;
    resume_to_core    = '0;

    for (int unsigned min_idx = 0; min_idx < NumMinions; min_idx++) begin
      minion_debug_out_t debug_out_struct;
      logic [NumThreads-1:0] hart_slice;

      debug_out_struct = debug_minion_status_i[min_idx];
      hart_slice = sel_harts[(NumThreads * min_idx) +: NumThreads];

      for (int unsigned thread_idx = 0; thread_idx < NumThreads; thread_idx++) begin
        resume_to_core[min_idx][thread_idx] =
            (resume_state_q[min_idx][thread_idx] == ResumeResuming) |
            resume_req_level[min_idx][thread_idx];
      end

      debug_rc_latch[min_idx].halt =
          ~debug_out_struct.halted &
          (({NumThreads{dmctrl_i.dmactive}} & {NumThreads{dmctrl_i.haltreq}} & hart_slice) |
           {NumThreads{bpam_run_control_i.gpio.halt_req}} |
           {NumThreads{bpam_run_control_i.events.halt_req}});

      debug_rc_latch[min_idx].resume = resume_to_core[min_idx] & ~resumeack[min_idx];
      debug_rc_latch[min_idx].resethalt =
          {NumThreads{dmctrl_i.dmactive}} &
          esr_hactrl_i.resethalt[(NumThreads * min_idx) +: NumThreads];
      debug_rc_latch[min_idx].ackhavereset =
          debug_out_struct.have_reset & {NumThreads{dmctrl_i.dmactive}} &
          {NumThreads{dmctrl_i.ackhavereset && dmctrl_i.write_enable}} & hart_slice;

      reset_w_per_minion_o[min_idx] =
          !rst_w_ni |
          bpam_run_control_i.gpio.hart_reset_req |
          (dmctrl_i.dmactive && dmctrl_i.hartreset && |hart_slice);

      esr_hastatus0.halted[(NumThreads * min_idx) +: NumThreads] = debug_out_struct.halted;
      esr_hastatus0.running[(NumThreads * min_idx) +: NumThreads] = debug_out_struct.running;
      esr_hastatus0.resumeack[(NumThreads * min_idx) +: NumThreads] = resumeack[min_idx];
      esr_hastatus0.havereset[(NumThreads * min_idx) +: NumThreads] = debug_out_struct.have_reset;

      esr_hastatus1_min.error[(NumThreads * min_idx) +: NumThreads] = debug_out_struct.error;
      esr_hastatus1_min.exception[(NumThreads * min_idx) +: NumThreads] = debug_out_struct.exception;
      esr_hastatus1_min.busy[(NumThreads * min_idx) +: NumThreads] = debug_out_struct.busy;
    end
  end

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      for (int unsigned min_idx = 0; min_idx < NumMinions; min_idx++) begin
        event_resume_req_level_q[min_idx] <= 1'b0;
        for (int unsigned thread_idx = 0; thread_idx < NumThreads; thread_idx++) begin
          resume_state_q[min_idx][thread_idx] <= ResumeIdle;
          resumeack_reg_q[min_idx][thread_idx] <= 1'b1;
        end
      end
    end else begin
      for (int unsigned min_idx = 0; min_idx < NumMinions; min_idx++) begin
        if (event_resume_req_level_en[min_idx]) begin
          event_resume_req_level_q[min_idx] <= event_resume_req_level_d[min_idx];
        end
        for (int unsigned thread_idx = 0; thread_idx < NumThreads; thread_idx++) begin
          if (resume_state_en[min_idx][thread_idx]) begin
            resume_state_q[min_idx][thread_idx] <= resume_state_d[min_idx][thread_idx];
          end
          if (resumeack_en[min_idx][thread_idx]) begin
            resumeack_reg_q[min_idx][thread_idx] <= resumeack_reg_d[min_idx][thread_idx];
          end
        end
      end
    end
  end

  assign dbg_target_sel = neigh_sm_gpio_q[SmGpioWidth-1 -: SmTargetSelWidth];
  assign dbg_target_sel_ext = {{(32-SmTargetSelWidth){1'b0}}, dbg_target_sel};
  assign dbg_mux_sel = neigh_sm_gpio_q[SmMuxWidth-1:0];
  assign minion_north_dbg_signals_mux_d = dbg_mux_sel;
  assign minion_south_dbg_signals_mux_d = dbg_mux_sel;
  assign tbox_dbg_signals_mux_o = dbg_mux_sel;
  assign neigh_ch_dbg_signals_mux_o = dbg_mux_sel;

  always_comb begin
    for (int unsigned min_idx = 0; min_idx < HalfNumMinions; min_idx++) begin
      minion_dbg_signals_mux_o[min_idx] = minion_north_dbg_signals_mux_q;
    end
    for (int unsigned min_idx = HalfNumMinions; min_idx < NumMinions; min_idx++) begin
      minion_dbg_signals_mux_o[min_idx] = minion_south_dbg_signals_mux_q;
    end
  end

  always_comb begin
    logic [NumMinions-1:0] status_monitor_enable_vec;

    minion_dbg_signals_en_d   = '0;
    tbox_dbg_signals_en_d     = 1'b0;
    neigh_ch_dbg_signals_en_d = 1'b0;
    status_monitor_enable_vec = {{(NumMinions-1){1'b0}}, status_monitor_enable_i};

    if (dbg_target_sel_ext < NumMinions) begin
      minion_dbg_signals_en_d = status_monitor_enable_vec << dbg_target_sel;
    end else if (dbg_target_sel_ext == NumMinions) begin
      tbox_dbg_signals_en_d = status_monitor_enable_i;
    end else begin
      neigh_ch_dbg_signals_en_d = status_monitor_enable_i;
    end
  end

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      neigh_sm_gpio_q <= '0;
      minion_north_dbg_signals_mux_q <= '0;
      minion_south_dbg_signals_mux_q <= '0;
      minion_dbg_signals_en_o <= '0;
      tbox_dbg_signals_en_o <= 1'b0;
      neigh_ch_dbg_signals_en_o <= 1'b0;
      minion_dbg_signals_q <= '0;
      minion_north_dbg_signals_q <= '0;
      minion_south_dbg_signals_q <= '0;
      tbox_dbg_signals_q <= '0;
      neigh_ch_dbg_signals_q <= '0;
    end else if (status_monitor_enable_i) begin
      neigh_sm_gpio_q <= neigh_sm_gpio_i;
      minion_north_dbg_signals_mux_q <= minion_north_dbg_signals_mux_d;
      minion_south_dbg_signals_mux_q <= minion_south_dbg_signals_mux_d;
      minion_dbg_signals_en_o <= minion_dbg_signals_en_d;
      tbox_dbg_signals_en_o <= tbox_dbg_signals_en_d;
      neigh_ch_dbg_signals_en_o <= neigh_ch_dbg_signals_en_d;
      minion_dbg_signals_q <= minion_dbg_signals_i;
      minion_north_dbg_signals_q <= minion_north_dbg_signals;
      minion_south_dbg_signals_q <= minion_south_dbg_signals;
      tbox_dbg_signals_q <= tbox_dbg_signals_i;
      neigh_ch_dbg_signals_q <= neigh_ch_dbg_signals_i;
    end
  end

  always_comb begin
    logic [HalfMinionSelWidth-1:0] half_minion_sel;
    half_minion_sel = dbg_target_sel[0 +: HalfMinionSelWidth];
    minion_north_dbg_signals = minion_dbg_signals_q[{1'b0, half_minion_sel}];
    minion_south_dbg_signals = minion_dbg_signals_q[{1'b1, half_minion_sel}];
  end

  always_comb begin
    if (dbg_target_sel_ext < HalfNumMinions) begin
      neigh_sm_signals_o = minion_north_dbg_signals_q;
    end else if (dbg_target_sel_ext < NumMinions) begin
      neigh_sm_signals_o = minion_south_dbg_signals_q;
    end else if (dbg_target_sel_ext == NumMinions) begin
      neigh_sm_signals_o = tbox_dbg_signals_q;
    end else begin
      neigh_sm_signals_o = neigh_ch_dbg_signals_q;
    end
  end

  // Package/type sanity for the original neighborhood geometry.
  // synthesis translate_off
`ifdef VERILATOR
  always_ff @(posedge clk_i) begin
    if (NumMinions != 8 || NumThreads != 2 || ThreadCount != HartStatusWidth) begin
      $error("neigh_ch_dbg currently preserves the original 8-minion/2-thread neighborhood geometry");
    end
  end
`endif
  // synthesis translate_on

endmodule : neigh_ch_dbg
