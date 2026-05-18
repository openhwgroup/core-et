// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module uncached_lo_voltage_cross_lv_logic
  import shire_uncached_pkg::*;
(
  input  logic       from_axi_sys_sbm_aw_vcfifo_valid_i,
  input  logic       from_axi_sys_sbm_w_vcfifo_valid_i,
  input  logic       from_axi_sys_aw_valid_lo_i,
  input  logic       from_axi_sys_w_valid_lo_i,
  input  logic [1:0] from_axi_sys_aw_vcvalid_lo_i,
  input  logic [1:0] from_axi_sys_w_vcvalid_lo_i,
  input  logic       from_axi_sys_sbm_aw_ready_i,
  input  logic       from_axi_sys_sbm_w_ready_i,
  input  logic       from_axi_sys_uc_aw_vcfifo_valid_i,
  input  logic       from_axi_sys_uc_w_vcfifo_valid_i,
  input  logic       from_axi_sys_uc_aw_ready_i,
  input  logic       from_axi_sys_uc_w_ready_i,

  input  logic       from_axi_sys_sbm_aw_vcfifo_ready_i,
  input  logic       from_axi_sys_uc_aw_vcfifo_ready_i,
  input  logic       from_axi_sys_sbm_w_vcfifo_ready_i,
  input  logic       from_axi_sys_uc_w_vcfifo_ready_i,

  output logic       from_axi_sys_sbm_aw_valid_o,
  output logic       from_axi_sys_sbm_aw_vcfifo_push_o,
  output logic       from_axi_sys_sbm_aw_vcfifo_pop_o,
  output logic       from_axi_sys_uc_aw_valid_o,
  output logic       from_axi_sys_uc_aw_vcfifo_push_o,
  output logic       from_axi_sys_uc_aw_vcfifo_pop_o,

  output logic       from_axi_sys_sbm_w_valid_o,
  output logic       from_axi_sys_sbm_w_vcfifo_push_o,
  output logic       from_axi_sys_sbm_w_vcfifo_pop_o,
  output logic       from_axi_sys_uc_w_valid_o,
  output logic       from_axi_sys_uc_w_vcfifo_push_o,
  output logic       from_axi_sys_uc_w_vcfifo_pop_o,

  output logic       from_axi_sys_aw_ready_lo_o,
  output logic       from_axi_sys_w_ready_lo_o
);

  localparam int unsigned SbmSysSlaveVsiId = 1;
  localparam int unsigned UcSysSlaveVsiId  = 0;

  assign from_axi_sys_sbm_aw_valid_o = from_axi_sys_sbm_aw_vcfifo_valid_i &
                                       from_axi_sys_sbm_w_vcfifo_valid_i;
  assign from_axi_sys_sbm_aw_vcfifo_push_o = from_axi_sys_aw_valid_lo_i &
                                             from_axi_sys_aw_vcvalid_lo_i[SbmSysSlaveVsiId];
  assign from_axi_sys_sbm_aw_vcfifo_pop_o = from_axi_sys_sbm_aw_valid_o &
                                            from_axi_sys_sbm_w_valid_o &
                                            from_axi_sys_sbm_aw_ready_i &
                                            from_axi_sys_sbm_w_ready_i;
  assign from_axi_sys_uc_aw_valid_o = from_axi_sys_uc_aw_vcfifo_valid_i &
                                      from_axi_sys_uc_w_vcfifo_valid_i;
  assign from_axi_sys_uc_aw_vcfifo_push_o = from_axi_sys_aw_ready_lo_o &
                                            from_axi_sys_w_ready_lo_o &
                                            from_axi_sys_aw_valid_lo_i &
                                            from_axi_sys_aw_vcvalid_lo_i[UcSysSlaveVsiId];
  assign from_axi_sys_uc_aw_vcfifo_pop_o = from_axi_sys_uc_aw_ready_i & from_axi_sys_uc_w_ready_i;

  assign from_axi_sys_sbm_w_valid_o = from_axi_sys_sbm_aw_vcfifo_valid_i &
                                      from_axi_sys_sbm_w_vcfifo_valid_i;
  assign from_axi_sys_sbm_w_vcfifo_push_o = from_axi_sys_w_valid_lo_i &
                                            from_axi_sys_w_vcvalid_lo_i[SbmSysSlaveVsiId];
  assign from_axi_sys_sbm_w_vcfifo_pop_o = from_axi_sys_sbm_aw_valid_o &
                                           from_axi_sys_sbm_w_valid_o &
                                           from_axi_sys_sbm_aw_ready_i &
                                           from_axi_sys_sbm_w_ready_i;
  assign from_axi_sys_uc_w_valid_o = from_axi_sys_uc_aw_vcfifo_valid_i &
                                     from_axi_sys_uc_w_vcfifo_valid_i;
  assign from_axi_sys_uc_w_vcfifo_push_o = from_axi_sys_aw_ready_lo_o &
                                           from_axi_sys_w_ready_lo_o &
                                           from_axi_sys_w_valid_lo_i &
                                           from_axi_sys_w_vcvalid_lo_i[UcSysSlaveVsiId];
  assign from_axi_sys_uc_w_vcfifo_pop_o = from_axi_sys_uc_aw_ready_i & from_axi_sys_uc_w_ready_i;

  assign from_axi_sys_aw_ready_lo_o = from_axi_sys_sbm_aw_vcfifo_ready_i |
                                      from_axi_sys_uc_aw_vcfifo_ready_i;
  assign from_axi_sys_w_ready_lo_o = from_axi_sys_sbm_w_vcfifo_ready_i |
                                     from_axi_sys_uc_w_vcfifo_ready_i;

endmodule : uncached_lo_voltage_cross_lv_logic
