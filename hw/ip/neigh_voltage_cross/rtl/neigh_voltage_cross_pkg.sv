// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package neigh_voltage_cross_pkg;

  import minion_pkg::*;

  /* verilator lint_off UNUSEDPARAM */  // Crossing geometry constants are consumed by the wrappers and their DV adapters.
  localparam int unsigned NvcNumShireIdsBits    = minion_pkg::NumShireIdsBits;
  localparam int unsigned NumNeighBits          = 2;
  localparam int unsigned TboxPerShireBits      = 2;
  localparam int unsigned MinPerN               = minion_pkg::ShireCoopMinMaskSize;
  localparam int unsigned ThreadsPerN           = MinPerN * minion_pkg::NrThreadsDefault;
  localparam int unsigned UcFccRegisters        = 4;
  localparam int unsigned UcFccTargetSize       = $clog2(UcFccRegisters);
  localparam int unsigned NeighSmTargetSel      = 4;
  localparam int unsigned NeighSmGpioWidth      = minion_pkg::NeighDebugSmMuxWidth + NeighSmTargetSel;
  localparam int unsigned NvcCsrFlBarrierSize   = 13;
  localparam int unsigned NeighDmApbAddrWidth   = 17;
  localparam int unsigned EsrApbDataWidth       = 64;
  localparam int unsigned DefaultFifoDepth      = 3;
  /* verilator lint_on UNUSEDPARAM */

  // Mirrors original APB_to_neigh_t field names/order from shire_types.vh.
  typedef struct packed {
    logic [NeighDmApbAddrWidth-1:0] apb_paddr;
    logic                           apb_penable;
    logic                           apb_psel;
    logic [EsrApbDataWidth-1:0]     apb_pwdata;
    logic                           apb_pwrite;
  } apb_to_neigh_t;

  // Mirrors original bpam_run_control_neigh_t from debug_types.vh.
  typedef struct packed {
    logic ndmreset;
    logic halt_req;
    logic resume_req;
    logic hart_reset_req;
    logic tbox_halt_req;
    logic tbox_resume_req;
  } bpam_neigh_rc_gpio_t;

  typedef struct packed {
    logic halt_req;
    logic resume_req;
    logic tbox_halt_req;
    logic tbox_resume_req;
  } bpam_neigh_rc_event_t;

  typedef struct packed {
    bpam_neigh_rc_gpio_t  gpio;
    bpam_neigh_rc_event_t events;
    logic                 tbox_single_step;
  } bpam_run_control_neigh_t;

  // Original voltage-crossing chicken-bits payload excludes the two reserved
  // bits present in minion_pkg::minion_chicken_bits_t.
  typedef struct packed {
    logic min_frontend_clock_gate_disable;
    logic min_dcache_clock_gate_disable;
    logic min_vputrans_clock_gate_disable;
    logic min_vputima_clock_gate_disable;
    logic min_vpulane_clock_gate_disable;
    logic min_intpipe_clock_gate_disable;
  } minion_chicken_bits_vc_t;

endpackage : neigh_voltage_cross_pkg
