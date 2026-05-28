// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module sys_gasket_lov (
  input  logic                              clock,
  input  logic                              clock_fb_ref,
  input  logic                              reset_warm_in,
  input  logic                              gpio_ndmreset_in,
  input  logic                              ndmreset_in,
  input  logic                              dmactive_in,
  input  logic                              reset_cold_in,
  input  logic                              dft__reset_byp,
  input  logic                              dft__reset,
  input  logic                              use_shire_tdr_reset_cntl,
  input  neigh_pkg::shire_tdr_reset_cntl_t  shire_tdr_reset_cntl,
  output logic                              clock_lv,
  output logic                              reset_w,
  output logic                              reset_c,
  output logic                              reset_d,
  output logic                              dft__reset_byp_lv,
  output logic                              dft__reset_lv
);

  localparam int unsigned SyncStages = 2;
  localparam int unsigned DebugRstPulseCntWidth = 5;
  localparam logic [DebugRstPulseCntWidth-1:0] DebugRstPulseCycles = 5'd31;

  logic reset_warm_int;
  logic gpio_ndmreset_int;
  logic ndmreset_int;
  logic dmactive_int;
  logic reset_cold_int;

  logic clock_inv;
  logic reset_warm_hv_sync;
  logic gpio_ndmreset_hv_sync;
  logic ndmreset_hv_sync;
  logic dmactive_hv_sync;
  logic reset_cold_hv_sync;

  logic reset_warm_ls_in;
  logic gpio_ndmreset_ls_in;
  logic ndmreset_ls_in;
  logic dmactive_ls_in;
  logic reset_cold_ls_in;

  logic reset_warm_lv_sync;
  logic gpio_ndmreset_lv_sync;
  logic ndmreset_lv_sync;
  logic dmactive_lv_sync;
  logic reset_cold_lv_sync;

  logic ndmreset_active;
  logic gpio_ndmreset_active;
  logic reset_d_counter_rst;
  logic [DebugRstPulseCntWidth-1:0] dmreset_pulse_counter_q;
  logic dm_reset_pulse_reg_q;
  logic dm_reset_pulse;
  logic dm_reset_pulse_dft;

  assign clock_lv = clock;
  assign clock_inv = !clock_fb_ref;
  assign dft__reset_byp_lv = dft__reset_byp;
  assign dft__reset_lv = dft__reset;

  always_comb begin
    reset_warm_int    = reset_warm_in;
    gpio_ndmreset_int = gpio_ndmreset_in;
    ndmreset_int      = ndmreset_in;
    dmactive_int      = dmactive_in;
    reset_cold_int    = reset_cold_in;
    if (use_shire_tdr_reset_cntl) begin
      reset_warm_int    = shire_tdr_reset_cntl.reset_warm_in;
      gpio_ndmreset_int = shire_tdr_reset_cntl.gpio_ndmreset_in;
      ndmreset_int      = shire_tdr_reset_cntl.ndmreset_in;
      dmactive_int      = shire_tdr_reset_cntl.dmactive_in;
      reset_cold_int    = shire_tdr_reset_cntl.reset_cold_in;
    end
  end

  sys_gasket_lov_sync_rst #(.Stages(SyncStages)) u_sync_reset_warm_hv (
    .clock(clock_inv),
    .reset_in(reset_warm_int),
    .dft__reset_byp(dft__reset_byp),
    .dft__reset(dft__reset),
    .reset_out(reset_warm_hv_sync)
  );

  sys_gasket_lov_sync_rst #(.Stages(SyncStages)) u_sync_gpio_ndmreset_hv (
    .clock(clock_inv),
    .reset_in(gpio_ndmreset_int),
    .dft__reset_byp(dft__reset_byp),
    .dft__reset(dft__reset),
    .reset_out(gpio_ndmreset_hv_sync)
  );

  sys_gasket_lov_sync_rst #(.Stages(SyncStages)) u_sync_ndmreset_hv (
    .clock(clock_inv),
    .reset_in(ndmreset_int),
    .dft__reset_byp(dft__reset_byp),
    .dft__reset(dft__reset),
    .reset_out(ndmreset_hv_sync)
  );

  sys_gasket_lov_sync_rst #(.Stages(SyncStages)) u_sync_dmactive_hv (
    .clock(clock_inv),
    .reset_in(dmactive_int),
    .dft__reset_byp(dft__reset_byp),
    .dft__reset(dft__reset),
    .reset_out(dmactive_hv_sync)
  );

  sys_gasket_lov_sync_rst #(.Stages(SyncStages)) u_sync_reset_cold_hv (
    .clock(clock_inv),
    .reset_in(reset_cold_int),
    .dft__reset_byp(dft__reset_byp),
    .dft__reset(dft__reset),
    .reset_out(reset_cold_hv_sync)
  );

  assign reset_warm_ls_in    = reset_warm_hv_sync;
  assign gpio_ndmreset_ls_in = gpio_ndmreset_hv_sync;
  assign ndmreset_ls_in      = ndmreset_hv_sync;
  assign dmactive_ls_in      = dmactive_hv_sync;
  assign reset_cold_ls_in    = reset_cold_hv_sync;

  rst_repeat u_repeat_reset_warm_lv (
    .reset_out(reset_warm_lv_sync),
    .reset_in(reset_warm_ls_in),
    .clock(clock_lv),
    .dft__reset_byp(dft__reset_byp_lv),
    .dft__reset(dft__reset_lv)
  );

  rst_repeat u_repeat_gpio_ndmreset_lv (
    .reset_out(gpio_ndmreset_lv_sync),
    .reset_in(gpio_ndmreset_ls_in),
    .clock(clock_lv),
    .dft__reset_byp(dft__reset_byp_lv),
    .dft__reset(dft__reset_lv)
  );

  rst_repeat u_repeat_ndmreset_lv (
    .reset_out(ndmreset_lv_sync),
    .reset_in(ndmreset_ls_in),
    .clock(clock_lv),
    .dft__reset_byp(dft__reset_byp_lv),
    .dft__reset(dft__reset_lv)
  );

  rst_repeat u_repeat_dmactive_lv (
    .reset_out(dmactive_lv_sync),
    .reset_in(dmactive_ls_in),
    .clock(clock_lv),
    .dft__reset_byp(dft__reset_byp_lv),
    .dft__reset(dft__reset_lv)
  );

  rst_repeat u_repeat_reset_cold_lv (
    .reset_out(reset_cold_lv_sync),
    .reset_in(reset_cold_ls_in),
    .clock(clock_lv),
    .dft__reset_byp(dft__reset_byp_lv),
    .dft__reset(dft__reset_lv)
  );

  assign ndmreset_active = ndmreset_lv_sync & dmactive_lv_sync;
  assign gpio_ndmreset_active = gpio_ndmreset_lv_sync;
  assign reset_d_counter_rst = dmactive_lv_sync | reset_cold_lv_sync;

  always_ff @(posedge clock_lv or posedge reset_d_counter_rst) begin
    if (reset_d_counter_rst) begin
      dmreset_pulse_counter_q <= '0;
    end else if (dmreset_pulse_counter_q < DebugRstPulseCycles) begin
      dmreset_pulse_counter_q <= dmreset_pulse_counter_q + {{(DebugRstPulseCntWidth-1){1'b0}}, 1'b1};
    end
  end

  always_ff @(posedge clock_lv or posedge reset_d_counter_rst) begin
    if (reset_d_counter_rst) begin
      dm_reset_pulse_reg_q <= 1'b1;
    end else if (dmreset_pulse_counter_q >= DebugRstPulseCycles) begin
      dm_reset_pulse_reg_q <= 1'b0;
    end
  end

  assign dm_reset_pulse = !dmactive_lv_sync & dm_reset_pulse_reg_q;
  assign dm_reset_pulse_dft = dft__reset_byp_lv ? dft__reset_lv : dm_reset_pulse;

  assign reset_w = ndmreset_active |
                   reset_warm_lv_sync |
                   reset_cold_lv_sync |
                   gpio_ndmreset_active;
  assign reset_d = dm_reset_pulse_dft | reset_cold_lv_sync;
  assign reset_c = reset_cold_lv_sync;

  logic unused_ok;
  assign unused_ok = &{1'b0, shire_tdr_reset_cntl.reset_n_system,
                       shire_tdr_reset_cntl.reset_n_system_debug};

endmodule : sys_gasket_lov
