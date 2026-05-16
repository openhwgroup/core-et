// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package neigh_ch_dbg_pkg;

  // Mirrors original tbox_bpam_run_control_t from debug_types.vh.
  typedef struct packed {
    logic halt_req;
    logic resume_req;
    logic single_step;
  } tbox_bpam_run_control_t;

  // Mirrors original resume_state_type enum values from debug_types.vh.
  typedef enum logic [1:0] {
    ResumeIdle     = 2'b00,
    ResumeResuming = 2'b01,
    ResumeWait     = 2'b10
  } resume_state_e;

endpackage : neigh_ch_dbg_pkg
