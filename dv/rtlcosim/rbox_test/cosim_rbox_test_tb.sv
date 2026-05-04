// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_test vs new rbox_test.
//
// The state_t struct (~457 bits) and quad_test_t (~166 bits) are too large
// for single Verilator signals.  We expose each field individually as a
// port and reconstruct the packed structs internally.

`include "soc.vh"

module cosim_rbox_test_tb (
  input  logic                    clk_i,
  input  logic                    rst_ni,

  // ── Shared stimulus: Shire Cache request ready ────────
  input  logic                    db_sc_req_ready_i,

  // ── Shared stimulus: Shire Cache response ─────────────
  input  logic [533:0]            sc_db_rsp_i,       // etlink_pkg::rsp_t = 534 bits
  input  logic                    sc_db_rsp_valid_i,

  // ── Shared stimulus: quad test (decomposed) ───────────
  input  logic [14:0]             qt_x_i,
  input  logic [14:0]             qt_y_i,
  input  logic [3:0]              qt_sample_idx_i,
  input  logic [3:0]              qt_mask_i,
  input  logic [31:0]             qt_depth0_i,
  input  logic [31:0]             qt_depth1_i,
  input  logic [31:0]             qt_depth2_i,
  input  logic [31:0]             qt_depth3_i,
  input  logic                    ts_quad_valid_i,

  // ── Shared stimulus: result accept ────────────────────
  input  logic                    ct_result_accept_i,

  // ── Shared stimulus: state_t (decomposed) ─────────────
  // We expose only the fields that rbox_test actually uses.
  // The unused fields are tied to zero inside the wrapper.
  input  logic                    st_msaa_enable_i,
  input  logic [3:0]              st_msaa_samples_i,
  input  logic [63:0]             st_ds_buffer_addr_i,
  input  logic [8:0]              st_ds_buffer_format_i,
  input  logic                    st_ds_buffer_tiled_i,
  input  logic [12:0]             st_ds_buffer_row_pitch_i,
  input  logic                    st_depth_clamp_enable_i,
  input  logic                    st_depth_bound_enable_i,
  input  logic                    st_depth_test_enable_i,
  input  logic                    st_depth_test_write_enable_i,
  input  logic [2:0]              st_depth_test_compare_op_i,
  input  logic                    st_stencil_test_enable_i,
  // Stencil front state (36 bits)
  input  logic [7:0]              st_sf_ref_value_i,
  input  logic [7:0]              st_sf_write_mask_i,
  input  logic [7:0]              st_sf_compare_mask_i,
  input  logic [2:0]              st_sf_compare_op_i,
  input  logic [2:0]              st_sf_depth_fail_op_i,
  input  logic [2:0]              st_sf_pass_op_i,
  input  logic [2:0]              st_sf_fail_op_i,
  // Stencil back state (36 bits)
  input  logic [7:0]              st_sb_ref_value_i,
  input  logic [7:0]              st_sb_write_mask_i,
  input  logic [7:0]              st_sb_compare_mask_i,
  input  logic [2:0]              st_sb_compare_op_i,
  input  logic [2:0]              st_sb_depth_fail_op_i,
  input  logic [2:0]              st_sb_pass_op_i,
  input  logic [2:0]              st_sb_fail_op_i,

  // ── Shared stimulus: front triangle ───────────────────
  input  logic                    ts_front_triangle_i,

  // ── Shared stimulus: flush ────────────────────────────
  input  logic                    db_flush_i,

  // ── Shared stimulus: debug ────────────────────────────
  input  logic [6:0]              dbg_ram_ptr_i,
  input  logic                    dbg_db_read_valid_i,
  input  logic [511:0]            dbg_db_write_data_i,
  input  logic                    dbg_db_write_valid_i,
  input  logic [42:0]             dbg_db_meta_write_data_i,
  input  logic                    dbg_db_meta_write_valid_i,

  // ── New module outputs ────────────────────────────────
  output logic [583:0]            new_db_sc_req_o,
  output logic [4:0]              new_db_sc_req_valid_o,
  output logic                    new_ts_quad_accept_o,
  output logic [3:0]              new_ts_result_mask_o,
  output logic                    new_ts_result_valid_o,
  output logic                    new_ts_empty_o,
  output logic                    new_db_flush_done_o,
  output logic [511:0]            new_dbg_db_read_data_o,
  output logic [42:0]             new_dbg_db_meta_read_data_o,

  // ── Original module outputs ───────────────────────────
  output logic [583:0]            orig_db_sc_req_o,
  output logic [4:0]              orig_db_sc_req_valid_o,
  output logic                    orig_ts_quad_accept_o,
  output logic [3:0]              orig_ts_result_mask_o,
  output logic                    orig_ts_result_valid_o,
  output logic                    orig_ts_empty_o,
  output logic                    orig_db_flush_done_o,
  output logic [511:0]            orig_dbg_db_read_data_o,
  output logic [42:0]             orig_dbg_db_meta_read_data_o
);

  import rbox_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  // ── Adapt reset ───────────────────────────────────────
  logic reset;
  assign reset = ~rst_ni;

  // ── Reconstruct quad_test_t ───────────────────────────

  rbox_pkg::quad_test_t          qt_in;
  assign qt_in.x          = qt_x_i;
  assign qt_in.y          = qt_y_i;
  assign qt_in.sample_idx = qt_sample_idx_i;
  assign qt_in.mask       = qt_mask_i;
  assign qt_in.depth[0]   = qt_depth0_i;
  assign qt_in.depth[1]   = qt_depth1_i;
  assign qt_in.depth[2]   = qt_depth2_i;
  assign qt_in.depth[3]   = qt_depth3_i;

  // ── Reconstruct state_t (new) ─────────────────────────

  rbox_pkg::state_t              new_state;

  always_comb begin
    new_state = '0;
    new_state.msaa_enable              = st_msaa_enable_i;
    new_state.msaa_samples             = st_msaa_samples_i;
    new_state.ds_buffer_addr           = st_ds_buffer_addr_i;
    new_state.ds_buffer_format         = rbox_pkg::ds_format_e'(st_ds_buffer_format_i);
    new_state.ds_buffer_tiled          = st_ds_buffer_tiled_i;
    new_state.ds_buffer_row_pitch      = st_ds_buffer_row_pitch_i;
    new_state.depth_clamp_enable       = st_depth_clamp_enable_i;
    new_state.depth_bound_enable       = st_depth_bound_enable_i;
    new_state.depth_test_enable        = st_depth_test_enable_i;
    new_state.depth_test_write_enable  = st_depth_test_write_enable_i;
    new_state.depth_test_compare_op    = rbox_pkg::comp_op_e'(st_depth_test_compare_op_i);
    new_state.stencil_test_enable      = st_stencil_test_enable_i;
    new_state.stencil_front_state.ref_value     = st_sf_ref_value_i;
    new_state.stencil_front_state.write_mask    = st_sf_write_mask_i;
    new_state.stencil_front_state.compare_mask  = st_sf_compare_mask_i;
    new_state.stencil_front_state.compare_op    = rbox_pkg::comp_op_e'(st_sf_compare_op_i);
    new_state.stencil_front_state.depth_fail_op = rbox_pkg::stencil_op_e'(st_sf_depth_fail_op_i);
    new_state.stencil_front_state.pass_op       = rbox_pkg::stencil_op_e'(st_sf_pass_op_i);
    new_state.stencil_front_state.fail_op       = rbox_pkg::stencil_op_e'(st_sf_fail_op_i);
    new_state.stencil_back_state.ref_value      = st_sb_ref_value_i;
    new_state.stencil_back_state.write_mask     = st_sb_write_mask_i;
    new_state.stencil_back_state.compare_mask   = st_sb_compare_mask_i;
    new_state.stencil_back_state.compare_op     = rbox_pkg::comp_op_e'(st_sb_compare_op_i);
    new_state.stencil_back_state.depth_fail_op  = rbox_pkg::stencil_op_e'(st_sb_depth_fail_op_i);
    new_state.stencil_back_state.pass_op        = rbox_pkg::stencil_op_e'(st_sb_pass_op_i);
    new_state.stencil_back_state.fail_op        = rbox_pkg::stencil_op_e'(st_sb_fail_op_i);
  end

  // ── Reconstruct rbox_state_t (original) ───────────────

  rbox_state_t                    orig_state;

  always_comb begin
    orig_state = '0;
    orig_state.msaa_enable              = st_msaa_enable_i;
    orig_state.msaa_samples             = st_msaa_samples_i;
    orig_state.ds_buffer_addr           = st_ds_buffer_addr_i;
    orig_state.ds_buffer_format         = rbox_ds_format_t'(st_ds_buffer_format_i);
    orig_state.ds_buffer_tiled          = st_ds_buffer_tiled_i;
    orig_state.ds_buffer_row_pitch      = st_ds_buffer_row_pitch_i;
    orig_state.depth_clamp_enable       = st_depth_clamp_enable_i;
    orig_state.depth_bound_enable       = st_depth_bound_enable_i;
    orig_state.depth_test_enable        = st_depth_test_enable_i;
    orig_state.depth_test_write_enable  = st_depth_test_write_enable_i;
    orig_state.depth_test_compare_op    = rbox_comp_op_t'(st_depth_test_compare_op_i);
    orig_state.stencil_test_enable      = st_stencil_test_enable_i;
    orig_state.stencil_front_state.ref_value     = st_sf_ref_value_i;
    orig_state.stencil_front_state.write_mask    = st_sf_write_mask_i;
    orig_state.stencil_front_state.compare_mask  = st_sf_compare_mask_i;
    orig_state.stencil_front_state.compare_op    = rbox_comp_op_t'(st_sf_compare_op_i);
    orig_state.stencil_front_state.depth_fail_op = rbox_stencil_op_t'(st_sf_depth_fail_op_i);
    orig_state.stencil_front_state.pass_op       = rbox_stencil_op_t'(st_sf_pass_op_i);
    orig_state.stencil_front_state.fail_op       = rbox_stencil_op_t'(st_sf_fail_op_i);
    orig_state.stencil_back_state.ref_value      = st_sb_ref_value_i;
    orig_state.stencil_back_state.write_mask     = st_sb_write_mask_i;
    orig_state.stencil_back_state.compare_mask   = st_sb_compare_mask_i;
    orig_state.stencil_back_state.compare_op     = rbox_comp_op_t'(st_sb_compare_op_i);
    orig_state.stencil_back_state.depth_fail_op  = rbox_stencil_op_t'(st_sb_depth_fail_op_i);
    orig_state.stencil_back_state.pass_op        = rbox_stencil_op_t'(st_sb_pass_op_i);
    orig_state.stencil_back_state.fail_op        = rbox_stencil_op_t'(st_sb_fail_op_i);
  end

  // ── Reconstruct rbox_quad_test_t (original) ───────────

  rbox_quad_test_t               orig_qt;
  assign orig_qt.x          = qt_x_i;
  assign orig_qt.y          = qt_y_i;
  assign orig_qt.sample_idx = qt_sample_idx_i;
  assign orig_qt.mask       = qt_mask_i;
  assign orig_qt.depth[0]   = qt_depth0_i;
  assign orig_qt.depth[1]   = qt_depth1_i;
  assign orig_qt.depth[2]   = qt_depth2_i;
  assign orig_qt.depth[3]   = qt_depth3_i;

  // ── Original response type wiring ─────────────────────
  et_link_rsp_info_t orig_sc_db_rsp;
  assign orig_sc_db_rsp = et_link_rsp_info_t'(sc_db_rsp_i);

  // ── Original request output ───────────────────────────
  et_link_req_info_t               orig_db_sc_req;
  logic [(`SC_BANKS+1)-1:0]        orig_db_sc_req_valid;
  assign orig_db_sc_req_o       = orig_db_sc_req;
  assign orig_db_sc_req_valid_o = orig_db_sc_req_valid;

  // ── Original debug metadata output (38 bits -> zero-extend to 43) ──
  logic [`RBOX_DBG_DB_META_DATA_R] orig_dbg_db_meta_read_data_raw;
  assign orig_dbg_db_meta_read_data_o = 43'(orig_dbg_db_meta_read_data_raw);

  // ── Original debug metadata write input (truncated to 38 bits) ──
  logic [`RBOX_DBG_DB_META_DATA_R] orig_dbg_db_meta_write_data;
  assign orig_dbg_db_meta_write_data = dbg_db_meta_write_data_i[`RBOX_DBG_DB_META_DATA_R];

  // ── New module response type wiring ───────────────────
  etlink_pkg::rsp_t new_sc_db_rsp;
  assign new_sc_db_rsp = etlink_pkg::rsp_t'(sc_db_rsp_i);

  // ── New request output ────────────────────────────────
  etlink_pkg::req_t new_db_sc_req;
  logic [ScBanks:0]  new_db_sc_req_valid;
  assign new_db_sc_req_o       = new_db_sc_req;
  assign new_db_sc_req_valid_o = new_db_sc_req_valid;

  // ── RAM config (disabled) ─────────────────────────────
  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  assign orig_ram_cfg = '0;

  // ════════════════════════════════════════════════════════════
  // New module instance
  // ════════════════════════════════════════════════════════════

  rbox_test u_new (
    .clk_i                    (clk_i),
    .clk_rbox_i               (clk_i),
    .rst_ni                   (rst_ni),
    .ram_cfg_i                (ram_cfg_t'('0)),
    .dft_i                    (dft_t'('0)),
    .db_sc_req_o              (new_db_sc_req),
    .db_sc_req_valid_o        (new_db_sc_req_valid),
    .db_sc_req_ready_i        (db_sc_req_ready_i),
    .sc_db_rsp_i              (new_sc_db_rsp),
    .sc_db_rsp_valid_i        (sc_db_rsp_valid_i),
    .ts_quad_test_i           (qt_in),
    .ts_quad_valid_i          (ts_quad_valid_i),
    .ts_quad_accept_o         (new_ts_quad_accept_o),
    .ts_result_mask_o         (new_ts_result_mask_o),
    .ts_result_valid_o        (new_ts_result_valid_o),
    .ct_result_accept_i       (ct_result_accept_i),
    .ts_state_i               (new_state),
    .ts_front_triangle_i      (ts_front_triangle_i),
    .ts_empty_o               (new_ts_empty_o),
    .db_flush_i               (db_flush_i),
    .db_flush_done_o          (new_db_flush_done_o),
    .dbg_ram_ptr_i            (dbg_ram_ptr_i),
    .dbg_db_read_valid_i      (dbg_db_read_valid_i),
    .dbg_db_read_data_o       (new_dbg_db_read_data_o),
    .dbg_db_write_data_i      (dbg_db_write_data_i),
    .dbg_db_write_valid_i     (dbg_db_write_valid_i),
    .dbg_db_meta_read_data_o  (new_dbg_db_meta_read_data_o),
    .dbg_db_meta_write_data_i (dbg_db_meta_write_data_i),
    .dbg_db_meta_write_valid_i(dbg_db_meta_write_valid_i)
  );

  // ════════════════════════════════════════════════════════════
  // Original module instance (renamed copy inlined below)
  // ════════════════════════════════════════════════════════════

  rbox_test_orig u_orig (
    .clock                   (clk_i),
    .clock_rbox              (clk_i),
    .reset                   (reset),
    .esr_shire_cache_ram_cfg (orig_ram_cfg),
    .db_sc_req               (orig_db_sc_req),
    .db_sc_req_valid         (orig_db_sc_req_valid),
    .db_sc_req_ready         (db_sc_req_ready_i),
    .sc_db_rsp               (orig_sc_db_rsp),
    .sc_db_rsp_valid         (sc_db_rsp_valid_i),
    .ts_quad_test            (orig_qt),
    .ts_quad_valid           (ts_quad_valid_i),
    .ts_quad_accept          (orig_ts_quad_accept_o),
    .ts_result_mask          (orig_ts_result_mask_o),
    .ts_result_valid         (orig_ts_result_valid_o),
    .ct_result_accept        (ct_result_accept_i),
    .ts_state                (orig_state),
    .ts_front_triangle       (ts_front_triangle_i),
    .ts_empty                (orig_ts_empty_o),
    .db_flush                (db_flush_i),
    .db_flush_done           (orig_db_flush_done_o),
    .dbg_ram_ptr             (dbg_ram_ptr_i),
    .dbg_db_read_valid       (dbg_db_read_valid_i),
    .dbg_db_read_data        (orig_dbg_db_read_data_o),
    .dbg_db_write_data       (dbg_db_write_data_i),
    .dbg_db_write_valid      (dbg_db_write_valid_i),
    .dbg_db_meta_read_data   (orig_dbg_db_meta_read_data_raw),
    .dbg_db_meta_write_data  (orig_dbg_db_meta_write_data),
    .dbg_db_meta_write_valid (dbg_db_meta_write_valid_i)
  );

endmodule

// ════════════════════════════════════════════════════════════
// Renamed copy of original rbox_test
// ════════════════════════════════════════════════════════════

`include "soc.vh"

module rbox_test_orig
(
    // System
    input logic                             clock,
    input logic                             clock_rbox,
    input logic                             reset,

    // Configuration
    input  esr_shire_cache_ram_cfg_t        esr_shire_cache_ram_cfg,

    // Request to Shire Cache
    output et_link_req_info_t               db_sc_req,
    output logic [(`SC_BANKS+1)-1:0]        db_sc_req_valid,
    input  logic                            db_sc_req_ready,

    // Response from Shire Cache
    input  et_link_rsp_info_t               sc_db_rsp,
    input  logic                            sc_db_rsp_valid,

    // Input from traverse
    input  rbox_quad_test_t                 ts_quad_test,
    input  logic                            ts_quad_valid,
    output logic                            ts_quad_accept,

    // Output to control
    output logic [3:0]                      ts_result_mask,
    output logic                            ts_result_valid,
    input  logic                            ct_result_accept,

    // State from Control
    input  rbox_state_t                     ts_state,
    input  logic                            ts_front_triangle,

    // Feedback to Control
    output logic                            ts_empty,

    // Flush from/to Control
    input  logic                            db_flush,
    output logic                            db_flush_done,

    // Debug RAM interface with RBOX RAMs
    input logic [`RBOX_DBG_PTR_R]           dbg_ram_ptr,

    // Debug RAM interface for Depth Buffer
    input  logic                            dbg_db_read_valid,
    output logic [`RBOX_DEPTH_LINE_R]       dbg_db_read_data,

    input  logic [`RBOX_DEPTH_LINE_R]       dbg_db_write_data,
    input  logic                            dbg_db_write_valid,

    output logic [`RBOX_DBG_DB_META_DATA_R] dbg_db_meta_read_data,

    input  logic [`RBOX_DBG_DB_META_DATA_R] dbg_db_meta_write_data,
    input  logic                            dbg_db_meta_write_valid

);

logic [`PA_RANGE]           db_fetch_address;
logic [`RBOX_QUAD_BUF_I_R]  db_fetch_quad;
logic                       db_fetch_valid;
logic [`RBOX_DEPTH_BUF_I_R] db_fetch_ptr;
logic                       db_fetch_done;
logic [`RBOX_DEPTH_BUF_I_R] db_read_ptr;
logic                       db_read_valid;
logic [`RBOX_DEPTH_LINE_R]  db_read_data;
logic                       db_read_data_valid;
logic [`RBOX_DEPTH_BUF_I_R] db_write_ptr;
logic [`RBOX_DEPTH_LINE_R]  db_write_data;
logic                       db_write_valid;
logic                       db_write_done;
logic [`RBOX_DEPTH_BUF_I_R] db_dealloc_ptr;
logic [`RBOX_QUAD_BUF_I_R]  db_dealloc_quad;
logic                       db_dealloc_valid;

rbox_depth_buffer_orig depth_buffer (
                                .clock                       (clock                      ),
                                .clock_rbox                  (clock_rbox                 ),
                                .reset                       (reset                      ),
                                .esr_shire_cache_ram_cfg     (esr_shire_cache_ram_cfg    ),

                                .db_sc_req                   (db_sc_req                  ),
                                .db_sc_req_valid             (db_sc_req_valid            ),
                                .db_sc_req_ready             (db_sc_req_ready            ),

                                .sc_db_rsp                   (sc_db_rsp                  ),
                                .sc_db_rsp_valid             (sc_db_rsp_valid            ),

                                .db_fetch_address            (db_fetch_address           ),
                                .db_fetch_quad               (db_fetch_quad              ),
                                .db_fetch_valid              (db_fetch_valid             ),
                                .db_fetch_ptr                (db_fetch_ptr               ),
                                .db_fetch_done               (db_fetch_done              ),

                                .db_read_ptr                 (db_read_ptr                ),
                                .db_read_valid               (db_read_valid              ),
                                .db_read_data                (db_read_data               ),
                                .db_read_data_valid          (db_read_data_valid         ),

                                .db_write_ptr                (db_write_ptr               ),
                                .db_write_data               (db_write_data              ),
                                .db_write_valid              (db_write_valid             ),
                                .db_write_done               (db_write_done              ),

                                .db_dealloc_ptr              (db_dealloc_ptr             ),
                                .db_dealloc_quad             (db_dealloc_quad            ),
                                .db_dealloc_valid            (db_dealloc_valid           ),

                                .db_flush                    (db_flush                   ),
                                .db_flush_done               (db_flush_done              ),

                                // Debug from/to Depth Buffer
                                .dbg_ram_ptr                 (dbg_ram_ptr                ),
                                .dbg_db_read_valid           (dbg_db_read_valid          ),
                                .dbg_db_read_data            (dbg_db_read_data           ),

                                .dbg_db_write_data           (dbg_db_write_data          ),
                                .dbg_db_write_valid          (dbg_db_write_valid         ),

                                .dbg_db_meta_read_data       (dbg_db_meta_read_data      ),

                                .dbg_db_meta_write_data      (dbg_db_meta_write_data     ),
                                .dbg_db_meta_write_valid     (dbg_db_meta_write_valid    )
                               );


rbox_depth_lfifo_info_t [`RBOX_DEPTH_LFIFO_E_R] dlfifo_info_ff;
rbox_depth_lfifo_info_t [`RBOX_DEPTH_LFIFO_E_R] dlfifo_info_nxt;
logic [`RBOX_DEPTH_LFIFO_E_R]                   dlfifo_info_en;

logic [`RBOX_DEPTH_LFIFO_I_R]                   dlfifo_alloc_ptr_ff;
logic [`RBOX_DEPTH_LFIFO_I_R]                   dlfifo_alloc_ptr_nxt;
logic                                           dlfifo_alloc_ptr_en;
logic [`RBOX_DEPTH_LFIFO_I_R]                   dlfifo_rd_ptr_ff;
logic [`RBOX_DEPTH_LFIFO_I_R]                   dlfifo_rd_ptr_nxt;
logic                                           dlfifo_rd_ptr_en;
logic [`RBOX_DEPTH_LFIFO_I_R]                   dlfifo_wr_ptr_ff;
logic [`RBOX_DEPTH_LFIFO_I_R]                   dlfifo_wr_ptr_nxt;
logic                                           dlfifo_wr_ptr_en;
logic [`RBOX_DEPTH_LFIFO_I_R]                   dlfifo_dealloc_ptr_ff;
logic [`RBOX_DEPTH_LFIFO_I_R]                   dlfifo_dealloc_ptr_nxt;
logic                                           dlfifo_dealloc_ptr_en;
logic                                           dlfifo_empty_ff;
logic                                           dlfifo_empty_nxt;
logic                                           dlfifo_empty_en;

`EN_FF(clock_rbox,        dlfifo_alloc_ptr_en,   dlfifo_alloc_ptr_ff,      dlfifo_alloc_ptr_nxt           );
`EN_FF(clock_rbox,        dlfifo_rd_ptr_en,      dlfifo_rd_ptr_ff,         dlfifo_rd_ptr_nxt              );
`EN_FF(clock_rbox,        dlfifo_wr_ptr_en,      dlfifo_wr_ptr_ff,         dlfifo_wr_ptr_nxt              );
`EN_FF(clock_rbox,        dlfifo_dealloc_ptr_en, dlfifo_dealloc_ptr_ff,    dlfifo_dealloc_ptr_nxt         );
`EN_FF(clock_rbox,        dlfifo_empty_en,       dlfifo_empty_ff,          dlfifo_empty_nxt               );

genvar quad;
generate
    for (quad = 0; quad < `RBOX_DEPTH_LFIFO_SZ; quad++)
        `EN_FF(clock_rbox, dlfifo_info_en[quad], dlfifo_info_ff[quad], dlfifo_info_nxt[quad]);
endgenerate

logic [`RBOX_DEPTH_QUAD_DATA_R] dlfifo_quad_wr_data;
logic                           dlfifo_quad_wr_en;
logic                           dlfifo_quad_wr_done;
logic [`RBOX_DEPTH_QUAD_DATA_R] dlfifo_quad_rd_data;
logic                           dlfifo_quad_rd_en;
logic                           dlfifo_quad_rd_valid;

rbox_fifo
#(
    .WIDTH    (`RBOX_DEPTH_QUAD_DATA_SZ),
    .DEPTH    (`RBOX_DEPTH_LFIFO_SZ    ),
    .PORTS    (2                       )
 )
dlfifo
(
    .clock                   (clock_rbox              ),
    .reset                   (reset                   ),
    .esr_shire_cache_ram_cfg (esr_shire_cache_ram_cfg ),
    .rd_data                 (dlfifo_quad_rd_data     ),
    .rd_en                   (dlfifo_quad_rd_en       ),
    .rd_valid                (dlfifo_quad_rd_valid    ),
    .wr_data                 (dlfifo_quad_wr_data     ),
    .wr_en                   (dlfifo_quad_wr_en       ),
    .wr_done                 (dlfifo_quad_wr_done     )
);

always_comb
begin
    if (reset)
    begin
        dlfifo_alloc_ptr_nxt = '0;
        dlfifo_wr_ptr_nxt    = '0;
        dlfifo_empty_nxt     = '0;

        dlfifo_alloc_ptr_en  = 1'b1;
        dlfifo_wr_ptr_en     = 1'b1;
        dlfifo_empty_en      = 1'b1;
    end
    else
    begin
        dlfifo_alloc_ptr_nxt = dlfifo_alloc_ptr_ff;
        dlfifo_wr_ptr_nxt    = dlfifo_wr_ptr_ff;

        dlfifo_alloc_ptr_en = 1'b0;
        dlfifo_wr_ptr_en    = 1'b0;

        if (dlfifo_quad_rd_valid)
        begin
            dlfifo_wr_ptr_nxt = dlfifo_wr_ptr_ff + 1;
            dlfifo_wr_ptr_en  = 1'b1;
        end

        if (dlfifo_quad_wr_done)
        begin
            dlfifo_alloc_ptr_nxt = dlfifo_alloc_ptr_ff + 1;
            dlfifo_alloc_ptr_en  = 1'b1;
        end

        dlfifo_empty_en  = dlfifo_alloc_ptr_en || dlfifo_dealloc_ptr_en;
        dlfifo_empty_nxt = (dlfifo_alloc_ptr_nxt == dlfifo_dealloc_ptr_nxt);
    end

end

rbox_msaa_mode_t             s1_msaa_mode;
logic [`RBOX_MAX_MSAA_DIM_R] s1_quad_sample_x;
logic [`RBOX_MAX_MSAA_DIM_R] s1_quad_sample_y;
logic [`PA_RANGE]            s1_quad_address;
logic [5:0]                  s1_quad_offset;
logic                        s1_pipe_en;
logic                        s2_fetch_quad_valid_ff;

always_comb
begin

    s1_msaa_mode = f_convert_msaa_mode(ts_state.msaa_samples);
    casez ({ts_state.msaa_enable, s1_msaa_mode})
        {1'b0, {$bits(s1_msaa_mode){1'b?}}} : {s1_quad_sample_x, s1_quad_sample_y} = {2'b0,                               ts_quad_test.x,
                                                                                      2'b0,                               ts_quad_test.y};
        {1'b1, RBOX_MSAA_MODE_2X}           : {s1_quad_sample_x, s1_quad_sample_y} = {1'b0, ts_quad_test.sample_idx[0],   ts_quad_test.x,
                                                                                      2'b0,                               ts_quad_test.y};
        {1'b1, RBOX_MSAA_MODE_4X}           : {s1_quad_sample_x, s1_quad_sample_y} = {1'b0, ts_quad_test.sample_idx[0],   ts_quad_test.x,
                                                                                      1'b0, ts_quad_test.sample_idx[1],   ts_quad_test.y};
        {1'b1, RBOX_MSAA_MODE_8X}           : {s1_quad_sample_x, s1_quad_sample_y} = {      ts_quad_test.sample_idx[1:0], ts_quad_test.x,
                                                                                      1'b0, ts_quad_test.sample_idx[2],   ts_quad_test.y};
        {1'b1, RBOX_MSAA_MODE_16X}          : {s1_quad_sample_x, s1_quad_sample_y} = {      ts_quad_test.sample_idx[1:0], ts_quad_test.x,
                                                                                            ts_quad_test.sample_idx[3:2], ts_quad_test.y};
        default                             : {s1_quad_sample_x, s1_quad_sample_y} = '0;
    endcase

    if (ts_state.ds_buffer_format == RBOX_DS_FORMAT_D16_UNORM)
    begin
        s1_quad_address = ts_state.ds_buffer_addr[`PA_RANGE]
                        + `ZX(`PA_SIZE, {s1_quad_sample_x[7], s1_quad_sample_y[6], s1_quad_sample_x[6],   s1_quad_sample_y[5],
                                         s1_quad_sample_x[5], s1_quad_sample_y[4], s1_quad_sample_x[4],   s1_quad_sample_y[3],
                                         s1_quad_sample_x[3], s1_quad_sample_y[2], 6'b0})
                        + ((  s1_quad_sample_y[`RBOX_MAX_RENDER_DIM_L-1:7] * ts_state.ds_buffer_row_pitch
                            + s1_quad_sample_x[`RBOX_MAX_RENDER_DIM_L-1:8]) << 16);
    end
    else
    begin
        s1_quad_address = ts_state.ds_buffer_addr[`PA_RANGE]
                        + `ZX(`PA_SIZE, {s1_quad_sample_x[6], s1_quad_sample_y[6], s1_quad_sample_x[5],   s1_quad_sample_y[5],
                                         s1_quad_sample_x[4], s1_quad_sample_y[4], s1_quad_sample_x[3],   s1_quad_sample_y[3],
                                         s1_quad_sample_x[2], s1_quad_sample_y[2], 6'b0})
                        + ((  s1_quad_sample_y[`RBOX_MAX_RENDER_DIM_L-1:7] * ts_state.ds_buffer_row_pitch
                            + s1_quad_sample_x[`RBOX_MAX_RENDER_DIM_L-1:7]) << 16);
    end

    if (ts_state.ds_buffer_format == RBOX_DS_FORMAT_D16_UNORM)
        s1_quad_offset = s1_quad_sample_y[1:0] * 8 + s1_quad_sample_x[2:0];
    else
        s1_quad_offset = s1_quad_sample_y[1:0] * 4 + s1_quad_sample_x[1:0];

    dlfifo_quad_wr_en   = ts_quad_valid && (!s2_fetch_quad_valid_ff || (s2_fetch_quad_valid_ff && db_fetch_done));
    dlfifo_quad_wr_data = ts_quad_test.depth;
    ts_quad_accept      = dlfifo_quad_wr_done;
    s1_pipe_en          = dlfifo_quad_wr_done;
end

logic [`PA_RANGE]             s2_quad_address_ff;
logic                         s2_fetch_quad_valid_nxt;
logic [`RBOX_DEPTH_LFIFO_I_R] s2_dlfifo_fetch_ptr_ff;

   `RST_FF(clock_rbox, reset,             s2_fetch_quad_valid_ff, s2_fetch_quad_valid_nxt, 1'b0);
    `EN_FF(clock_rbox,        s1_pipe_en, s2_quad_address_ff,     s1_quad_address              );
    `EN_FF(clock_rbox,        s1_pipe_en, s2_dlfifo_fetch_ptr_ff, dlfifo_alloc_ptr_ff          );

always_comb
begin
    s2_fetch_quad_valid_nxt =  s2_fetch_quad_valid_ff && !db_fetch_done
                            || dlfifo_quad_wr_done;

    db_fetch_address = s2_quad_address_ff;
    db_fetch_quad    = s2_dlfifo_fetch_ptr_ff;
    db_fetch_valid   = s2_fetch_quad_valid_ff;

end

logic                            s2_read_quad_valid;

logic                            s3_test_quad_valid_ff;
logic [`RBOX_DEPTH_BUF_I_R]      s3_depth_buf_ptr_ff;
logic [`RBOX_DEPTH_LINE_R]       s3_depth_data_ff;
logic [`RBOX_DEPTH_LINE_R]       s3_depth_data_nxt;
logic [`RBOX_DEPTH_LINE_BYTES_R] s3_depth_data_en;
logic                            s3_depth_data_dirty_ff;
logic                            s3_depth_data_dirty_nxt;
logic                            s3_depth_data_dirty_en;
logic                            s3_db_read_data_valid_ff;

always_comb
begin
    db_read_valid = 1'b0;
    db_read_ptr   = '0;

    db_write_valid = 1'b0;
    db_write_ptr   = '0;
    db_write_data  = '0;

    s2_read_quad_valid = 1'b0;

    dlfifo_rd_ptr_en = reset;
    dlfifo_rd_ptr_nxt = reset ? '0 : dlfifo_rd_ptr_ff;

    if (dlfifo_info_ff[dlfifo_rd_ptr_ff].valid)
    begin
        if (s3_depth_buf_ptr_ff == dlfifo_info_ff[dlfifo_rd_ptr_ff].depth_buf_ptr)
        begin
            s2_read_quad_valid = 1'b1;
            dlfifo_rd_ptr_nxt = dlfifo_rd_ptr_ff + 1;
            dlfifo_rd_ptr_en  = 1'b1;
        end
        else
        begin
            if (s3_depth_data_dirty_ff && !s3_test_quad_valid_ff)
            begin
                db_write_valid = 1'b1;
                db_write_ptr   = s3_depth_buf_ptr_ff;
                db_write_data  = s3_depth_data_ff;
            end
            else if (!s3_depth_data_dirty_ff)
            begin
                db_read_valid = 1'b1;
                db_read_ptr   = dlfifo_info_ff[dlfifo_rd_ptr_ff].depth_buf_ptr;
            end
        end
    end

    dlfifo_quad_rd_en = s2_read_quad_valid;
end


`RST_FF(clock_rbox, reset, s3_test_quad_valid_ff,    s2_read_quad_valid, 1'b0);
`RST_FF(clock_rbox, reset, s3_db_read_data_valid_ff, db_read_data_valid, 1'b0);

`EN_FF(clock_rbox, db_read_data_valid,     s3_depth_buf_ptr_ff,    db_read_ptr            );
`EN_FF(clock_rbox, s3_depth_data_dirty_en, s3_depth_data_dirty_ff, s3_depth_data_dirty_nxt);

genvar b;
generate
    for (b = 0; b < `RBOX_DEPTH_LINE_BYTES; b++)
        `EN_FF(clock_rbox, s3_depth_data_en[b], s3_depth_data_ff[b*8 +: 8], s3_depth_data_nxt[b*8 +: 8]);
endgenerate

function automatic logic stencil_test_f;
    input rbox_state_t state;
    input logic        front_triangle;
    input logic [7:0]  in_value;

    rbox_stencil_state_t stencil_state;

    // Stencil test.
    if (state.stencil_test_enable)
    begin
        stencil_state = front_triangle ? ts_state.stencil_front_state : ts_state.stencil_back_state;

        unique casez (stencil_state.compare_op)
            RBOX_COMP_OP_NEVER            : stencil_test_f = 1'b0;
            RBOX_COMP_OP_LESS             : stencil_test_f = (stencil_state.ref_value & stencil_state.compare_mask) <  (in_value & stencil_state.compare_mask);
            RBOX_COMP_OP_EQUAL            : stencil_test_f = (stencil_state.ref_value & stencil_state.compare_mask) == (in_value & stencil_state.compare_mask);
            RBOX_COMP_OP_LESS_OR_EQUAL    : stencil_test_f = (stencil_state.ref_value & stencil_state.compare_mask) <= (in_value & stencil_state.compare_mask);
            RBOX_COMP_OP_GREATER          : stencil_test_f = (stencil_state.ref_value & stencil_state.compare_mask) >  (in_value & stencil_state.compare_mask);
            RBOX_COMP_OP_NOT_EQUAL        : stencil_test_f = (stencil_state.ref_value & stencil_state.compare_mask) != (in_value & stencil_state.compare_mask);
            RBOX_COMP_OP_GREATER_OR_EQUAL : stencil_test_f = (stencil_state.ref_value & stencil_state.compare_mask) >= (in_value & stencil_state.compare_mask);
            RBOX_COMP_OP_ALWAYS           : stencil_test_f =  1'b1;
        endcase
    end
    else
        stencil_test_f = 1'b1;

endfunction

function automatic logic depth_u24_compare_f;
    input rbox_comp_op_t comp_op;
    input logic [23:0] frag_depth;
    input logic [23:0] buf_depth;

    unique case (comp_op)
        RBOX_COMP_OP_NEVER            : depth_u24_compare_f = 1'b0;
        RBOX_COMP_OP_LESS             : depth_u24_compare_f = frag_depth <  buf_depth;
        RBOX_COMP_OP_EQUAL            : depth_u24_compare_f = frag_depth == buf_depth;
        RBOX_COMP_OP_LESS_OR_EQUAL    : depth_u24_compare_f = frag_depth <= buf_depth;
        RBOX_COMP_OP_GREATER          : depth_u24_compare_f = frag_depth >  buf_depth;
        RBOX_COMP_OP_NOT_EQUAL        : depth_u24_compare_f = frag_depth != buf_depth;
        RBOX_COMP_OP_GREATER_OR_EQUAL : depth_u24_compare_f = frag_depth >= buf_depth;
        RBOX_COMP_OP_ALWAYS           : depth_u24_compare_f = 1'b1;
    endcase

endfunction

function automatic logic depth_f16_compare_f;
    input rbox_comp_op_t comp_op;
    input logic [15:0] frag_depth;
    input logic [15:0] buf_depth;

    logic is_nan_frag;
    logic is_nan_buf;

    is_nan_frag = (frag_depth[14:10] == 5'h1f) && (|frag_depth[9:0]);
    is_nan_buf  = ( buf_depth[14:10] == 5'h1f) &&  (|buf_depth[9:0]);

    unique case (comp_op)
        RBOX_COMP_OP_NEVER            : depth_f16_compare_f = 1'b0;
        RBOX_COMP_OP_LESS             : depth_f16_compare_f = frag_depth <  buf_depth;
        RBOX_COMP_OP_EQUAL            : depth_f16_compare_f = frag_depth == buf_depth;
        RBOX_COMP_OP_LESS_OR_EQUAL    : depth_f16_compare_f = frag_depth <= buf_depth;
        RBOX_COMP_OP_GREATER          : depth_f16_compare_f = frag_depth >  buf_depth;
        RBOX_COMP_OP_NOT_EQUAL        : depth_f16_compare_f = frag_depth != buf_depth;
        RBOX_COMP_OP_GREATER_OR_EQUAL : depth_f16_compare_f = frag_depth >= buf_depth;
        RBOX_COMP_OP_ALWAYS           : depth_f16_compare_f = 1'b1;
    endcase

    depth_f16_compare_f = depth_f16_compare_f && !is_nan_frag && !is_nan_buf;
endfunction

function automatic logic depth_f32_compare_f;
    input rbox_comp_op_t comp_op;
    input logic [31:0] frag_depth;
    input logic [31:0] buf_depth;

    logic is_nan_frag;
    logic is_nan_buf;

    is_nan_frag = (frag_depth[31:24] == 8'hff) && (|frag_depth[23:0]);
    is_nan_buf  = ( buf_depth[31:24] == 8'hff) &&  (|buf_depth[23:0]);

    unique case (comp_op)
        RBOX_COMP_OP_NEVER            : depth_f32_compare_f = 1'b0;
        RBOX_COMP_OP_LESS             : depth_f32_compare_f = frag_depth <  buf_depth;
        RBOX_COMP_OP_EQUAL            : depth_f32_compare_f = frag_depth == buf_depth;
        RBOX_COMP_OP_LESS_OR_EQUAL    : depth_f32_compare_f = frag_depth <= buf_depth;
        RBOX_COMP_OP_GREATER          : depth_f32_compare_f = frag_depth >  buf_depth;
        RBOX_COMP_OP_NOT_EQUAL        : depth_f32_compare_f = frag_depth != buf_depth;
        RBOX_COMP_OP_GREATER_OR_EQUAL : depth_f32_compare_f = frag_depth >= buf_depth;
        RBOX_COMP_OP_ALWAYS           : depth_f32_compare_f = 1'b1;
    endcase

    depth_f32_compare_f = depth_f32_compare_f && !is_nan_frag && !is_nan_buf;
endfunction

function automatic logic depth_test_f;
    input rbox_state_t state;
    input logic [31:0] frag_depth;
    input logic [31:0] buf_depth;

    if (state.depth_test_enable)
    begin
        case (state.ds_buffer_format)
            RBOX_DS_FORMAT_D16_UNORM         : depth_test_f = depth_f16_compare_f(state.depth_test_compare_op, frag_depth[15:0], buf_depth[15:0]);
            RBOX_DS_FORMAT_D32_SFLOAT        : depth_test_f = depth_f32_compare_f(state.depth_test_compare_op, frag_depth,       buf_depth      );
            RBOX_DS_FORMAT_D24_UNORM_S8_UINT : depth_test_f = depth_u24_compare_f(state.depth_test_compare_op, frag_depth[23:0], buf_depth[23:0]);
            default                          : depth_test_f = depth_u24_compare_f(state.depth_test_compare_op, frag_depth[23:0], buf_depth[23:0]);
        endcase
    end
    else
        depth_test_f = 1'b1;
endfunction

function automatic logic [7:0] stencil_update_f;
    input rbox_stencil_state_t state;
    input rbox_stencil_op_t    update_op;
    input [7:0]                value;

    unique casez (update_op)
        RBOX_STENCIL_OP_KEEP      : stencil_update_f = value;
        RBOX_STENCIL_OP_ZERO      : stencil_update_f = 7'b0;
        RBOX_STENCIL_OP_REPLACE   : stencil_update_f = state.ref_value                          & state.write_mask;
        RBOX_STENCIL_OP_INC_CLAMP : stencil_update_f = ((value == 8'hff) ? 8'hff : (value + 1)) & state.write_mask;
        RBOX_STENCIL_OP_DEC_CLAMP : stencil_update_f = ((value == 8'h00) ? 8'h00 : (value - 1)) & state.write_mask;
        RBOX_STENCIL_OP_INVERT    : stencil_update_f = ~value                                   & state.write_mask;
        RBOX_STENCIL_OP_INC_WRAP  : stencil_update_f = (value + 1)                              & state.write_mask;
        RBOX_STENCIL_OP_DEC_WRAP  : stencil_update_f = (value - 1)                              & state.write_mask;
    endcase
endfunction


function automatic logic [31:0] update_depth_stencil_f;
    input rbox_state_t state;
    input logic        front_triangle;
    input logic        stencil_test_pass;
    input logic        depth_test_pass;
    input logic [31:0] frag_depth;
    input logic [31:0] buf_depth;

    rbox_stencil_state_t stencil_state;

    update_depth_stencil_f = buf_depth;
    unique casez (state.ds_buffer_format)
        RBOX_DS_FORMAT_D16_UNORM :
            begin
                if (stencil_test_pass && depth_test_pass && state.depth_test_write_enable)
                    update_depth_stencil_f = {16'h0, frag_depth[15:0]};
            end
        RBOX_DS_FORMAT_D32_SFLOAT :
            begin
                if (stencil_test_pass && depth_test_pass && state.depth_test_write_enable)
                    update_depth_stencil_f = frag_depth;
            end
        RBOX_DS_FORMAT_D24_UNORM_S8_UINT :
            begin
                if (stencil_test_pass && depth_test_pass && state.depth_test_write_enable)
                    update_depth_stencil_f[23:0] = frag_depth[23:0];

                // Stencil test.
                if (state.stencil_test_enable)
                begin
                    stencil_state = front_triangle ? ts_state.stencil_front_state : ts_state.stencil_back_state;

                    if (stencil_test_pass)
                        if (depth_test_pass)
                            update_depth_stencil_f[31:24] = stencil_update_f(stencil_state, stencil_state.pass_op, buf_depth[31:24]);
                        else
                            update_depth_stencil_f[31:24] = stencil_update_f(stencil_state, stencil_state.depth_fail_op, buf_depth[31:24]);
                    else
                        update_depth_stencil_f[31:24] = stencil_update_f(stencil_state, stencil_state.fail_op, buf_depth[31:24]);
                end
                else
                    update_depth_stencil_f[31:24] = buf_depth[31:24];
            end
        default :
            begin
                update_depth_stencil_f = 32'h0;
            end
    endcase

endfunction

rbox_depth_lfifo_info_t s3_quad_info_in;
rbox_depth_lfifo_info_t s3_quad_info_out;
logic                   s3_pipe_en;
logic [3:0][31:0]       s3_quad_in_depth;   // Depth from depth buffer
logic [3:0][31:0]       s3_quad_depth;      // Depth from fragment
logic [3:0]             s3_stencil_test_pass;
logic [3:0]             s3_depth_test_pass;
logic [3:0][31:0]       s3_quad_out_depth;
logic                   s4_test_quad_valid_ff;


always_comb
begin
    s3_quad_info_in = dlfifo_info_ff[dlfifo_wr_ptr_ff];
    s3_quad_depth   = dlfifo_quad_rd_data;

    s3_pipe_en = dlfifo_quad_rd_valid && (!s4_test_quad_valid_ff || s4_test_quad_valid_ff && ct_result_accept);

    s3_stencil_test_pass = 4'b1111;
    s3_depth_test_pass   = 4'b1111;
    s3_quad_out_depth    = '0;

    s3_depth_data_en        = '0;
    s3_depth_data_dirty_en  = reset;
    s3_depth_data_dirty_nxt = reset ? 1'b0 : s3_depth_data_dirty_ff;

    s3_depth_data_nxt = '0;

    s3_quad_info_out = s3_quad_info_in;

    if (s3_test_quad_valid_ff)
    begin
        for (integer quad_i = 0; quad_i < 4; quad_i++)
        begin
            // Get input data from depth buffer for the quad.
            unique casez (ts_state.ds_buffer_format)
                RBOX_DS_FORMAT_D16_UNORM : s3_quad_in_depth[quad_i] = {16'b0, s3_depth_data_ff[(s3_quad_info_in.quad_offset * 16 + quad_i[0] * 16 + quad_i[1] * 8 * 16)  +: 16]};
                default                  : s3_quad_in_depth[quad_i] = s3_depth_data_ff[(s3_quad_info_in.quad_offset * 32 + quad_i[0] * 32 + quad_i[1] * 4 * 32) +: 32];
            endcase

            s3_stencil_test_pass[quad_i] = stencil_test_f(ts_state, ts_front_triangle, s3_quad_in_depth[quad_i][31:24]);
              s3_depth_test_pass[quad_i] =   depth_test_f(ts_state, s3_quad_depth[quad_i], s3_quad_in_depth[quad_i]);
              s3_quad_out_depth[quad_i] = update_depth_stencil_f(ts_state, ts_front_triangle,
                                                               s3_stencil_test_pass[quad_i], s3_depth_test_pass[quad_i],
                                                               s3_quad_depth[quad_i], s3_quad_in_depth[quad_i]);

            unique casez (ts_state.ds_buffer_format)
                RBOX_DS_FORMAT_D16_UNORM : s3_depth_data_nxt[(s3_quad_info_in.quad_offset * 16 + quad_i[0] * 16 + quad_i[1] * 8 * 16) +: 16] = s3_quad_out_depth[quad_i][15:0];
                default                  : s3_depth_data_nxt[(s3_quad_info_in.quad_offset * 32 + quad_i[0] * 32 + quad_i[1] * 4 * 32) +: 32] = s3_quad_out_depth[quad_i];
            endcase

            unique casez (ts_state.ds_buffer_format)
                RBOX_DS_FORMAT_D16_UNORM : s3_depth_data_en[(s3_quad_info_in.quad_offset * 2 + quad_i[0] * 2 + quad_i[1] * 8 * 2) +: 2] = 2'b11;
                default                  : s3_depth_data_en[(s3_quad_info_in.quad_offset * 4 + quad_i[0] * 4 + quad_i[1] * 4 * 4) +: 4] = 4'hf;
            endcase

            s3_quad_info_out.mask[quad_i] = s3_quad_info_out.mask[quad_i] && s3_stencil_test_pass[quad_i] && s3_depth_test_pass[quad_i];
        end

        s3_depth_data_dirty_en = 1'b1;
        s3_depth_data_dirty_nxt = s3_depth_data_dirty_nxt || (|s3_depth_data_en);

    end

    if (s3_db_read_data_valid_ff)
    begin
        s3_depth_data_en = {$bits(s3_depth_data_en){1'b1}};
        s3_depth_data_nxt = db_read_data;
    end

    if (db_write_done)
    begin
        s3_depth_data_dirty_en  = 1'b1;
        s3_depth_data_dirty_nxt = 1'b0;
    end
end

logic                   s4_test_quad_valid_nxt;
logic [3:0][31:0]       s4_quad_out_depth_ff;
rbox_depth_lfifo_info_t s4_quad_info_ff;

`RST_FF(clock_rbox, reset, s4_test_quad_valid_ff, s4_test_quad_valid_nxt, 1'b0);

`EN_FF(clock_rbox, s3_pipe_en, s4_quad_out_depth_ff, s3_quad_out_depth);
`EN_FF(clock_rbox, s3_pipe_en, s4_quad_info_ff,      s3_quad_info_out );

always_comb
begin

    s4_test_quad_valid_nxt =  s4_test_quad_valid_ff && !ct_result_accept
                           || dlfifo_quad_rd_valid;

    db_dealloc_valid = ct_result_accept;
    db_dealloc_ptr   = s4_quad_info_ff.depth_buf_ptr;
    db_dealloc_quad  = dlfifo_dealloc_ptr_ff;

    ts_result_mask  = s4_quad_info_ff.mask;
    ts_result_valid = s4_test_quad_valid_ff;

    ts_empty = dlfifo_empty_ff;

    dlfifo_dealloc_ptr_en  = reset || ct_result_accept;
    dlfifo_dealloc_ptr_nxt = reset ? '0 : dlfifo_dealloc_ptr_ff + 1;

    dlfifo_info_nxt = reset ? '0                            : dlfifo_info_ff;
    dlfifo_info_en  = reset ? {$bits(dlfifo_info_en){1'b1}} : '0;

    if (dlfifo_quad_wr_done)
    begin
        dlfifo_info_nxt[dlfifo_alloc_ptr_ff].mask        = ts_quad_test.mask;
        dlfifo_info_nxt[dlfifo_alloc_ptr_ff].quad_offset = s1_quad_offset;
         dlfifo_info_en[dlfifo_alloc_ptr_ff]             = 1'b1;
    end

    if (db_fetch_done)
    begin
        dlfifo_info_nxt[s2_dlfifo_fetch_ptr_ff].valid         = 1'b1;
        dlfifo_info_nxt[s2_dlfifo_fetch_ptr_ff].depth_buf_ptr = db_fetch_ptr;
         dlfifo_info_en[s2_dlfifo_fetch_ptr_ff]               = 1'b1;
    end

    if (ct_result_accept)
    begin
        dlfifo_info_nxt[dlfifo_dealloc_ptr_ff].valid = 1'b0;
         dlfifo_info_en[dlfifo_dealloc_ptr_ff] = 1'b1;
    end
end

endmodule

// ════════════════════════════════════════════════════════════
// Renamed copy of original rbox_depth_buffer (for rbox_test_orig)
// Identical to the one in cosim_rbox_depth_buffer_tb.sv.
// ════════════════════════════════════════════════════════════

module rbox_depth_buffer_orig
(
    input  logic                            clock,
    input  logic                            clock_rbox,
    input  logic                            reset,
    input  esr_shire_cache_ram_cfg_t        esr_shire_cache_ram_cfg,
    output et_link_req_info_t               db_sc_req,
    output logic [(`SC_BANKS+1)-1:0]        db_sc_req_valid,
    input  logic                            db_sc_req_ready,
    input  et_link_rsp_info_t               sc_db_rsp,
    input  logic                            sc_db_rsp_valid,
    input  logic [`PA_RANGE]                db_fetch_address,
    input  logic [`RBOX_QUAD_BUF_I_R]       db_fetch_quad,
    input  logic                            db_fetch_valid,
    output logic [`RBOX_DEPTH_BUF_I_R]      db_fetch_ptr,
    output logic                            db_fetch_done,
    input  logic [`RBOX_DEPTH_BUF_I_R]      db_read_ptr,
    input  logic                            db_read_valid,
    output logic [`RBOX_DEPTH_LINE_R]       db_read_data,
    output logic                            db_read_data_valid,
    input  logic [`RBOX_DEPTH_BUF_I_R]      db_write_ptr,
    input  logic [`RBOX_DEPTH_LINE_R]       db_write_data,
    input  logic                            db_write_valid,
    output logic                            db_write_done,
    input  logic [`RBOX_DEPTH_BUF_I_R]      db_dealloc_ptr,
    input  logic [`RBOX_QUAD_BUF_I_R]       db_dealloc_quad,
    input  logic                            db_dealloc_valid,
    input  logic                            db_flush,
    output logic                            db_flush_done,
    input  logic [`RBOX_DBG_PTR_R]          dbg_ram_ptr,
    input  logic                            dbg_db_read_valid,
    output logic [`RBOX_DEPTH_LINE_R]       dbg_db_read_data,
    input  logic [`RBOX_DEPTH_LINE_R]       dbg_db_write_data,
    input  logic                            dbg_db_write_valid,
    output logic [`RBOX_DBG_DB_META_DATA_R] dbg_db_meta_read_data,
    input  logic [`RBOX_DBG_DB_META_DATA_R] dbg_db_meta_write_data,
    input  logic                            dbg_db_meta_write_valid
);

logic spill_pending_ff, spill_pending_nxt, spill_pending_en;
logic fetch_accept, fetch_fill_done;
logic fetch_fill_pending_ff, fetch_fill_pending_nxt, fetch_fill_pending_en;

`RST_EN_FF(clock_rbox, reset, spill_pending_en,      spill_pending_ff,      spill_pending_nxt,      1'b0)
`RST_EN_FF(clock_rbox, reset, fetch_fill_pending_en, fetch_fill_pending_ff, fetch_fill_pending_nxt, 1'b0)

logic [`RBOX_DEPTH_BUF_E_R][`RBOX_DEPTH_BUF_I_R] lru_array_ff, lru_array_nxt;
logic [`RBOX_DEPTH_BUF_E_R]                       lru_array_en;
logic [`RBOX_DEPTH_BUF_I_R] lru_ptr, update_ptr;
logic allocate;
logic [`RBOX_DEPTH_BUF_E_R] lru_match, update_lru, tag_match;
logic [`RBOX_DEPTH_BUF_I_R] match_ptr;
logic [`RBOX_DEPTH_BUF_E_R][`RBOX_DEPTH_BUF_TAG_R] tags_ff, tags_nxt;
logic [`RBOX_DEPTH_BUF_E_R] tags_en, valid_ff, valid_nxt, valid_en;
logic [`RBOX_DEPTH_BUF_E_R] dirty_ff, dirty_nxt, dirty_en;
logic [`RBOX_DEPTH_BUF_E_R] reserved_ff, reserved_nxt, reserved_en;
logic [`RBOX_DEPTH_BUF_E_R][`RBOX_QUAD_BUF_I_R] dealloc_quad_ff, dealloc_quad_nxt;
logic [`RBOX_DEPTH_BUF_E_R] dealloc_quad_en;
logic [`RBOX_DEPTH_BUF_I_R] spill_ptr_ff, spill_ptr_nxt;
logic spill_ptr_en;
logic [`RBOX_DEPTH_BUF_I_R] flush_ptr_ff, flush_ptr_nxt;
logic flush_ptr_en, flush_done_ff, flush_done_nxt, flush_done_en;
logic flush_spill_done_ff, flush_spill_done_nxt, flush_spill_done_en;
logic [`RBOX_DEPTH_BUF_I_R] spill_pending_acks_ff, spill_pending_acks_nxt;
logic spill_pending_acks_en;

    `EN_FF(clock,             tags_en,               tags_ff,               tags_nxt                             )
`RST_EN_FF(clock,      reset, valid_en,              valid_ff,              valid_nxt       ,       '0           )
`RST_EN_FF(clock,      reset, dirty_en,              dirty_ff,              dirty_nxt       ,       '0           )
`RST_EN_FF(clock,      reset, reserved_en,           reserved_ff,           reserved_nxt    ,       '0           )
    `EN_FF(clock,             dealloc_quad_en,       dealloc_quad_ff,       dealloc_quad_nxt                     )
`RST_EN_FF(clock_rbox, reset, lru_array_en,          lru_array_ff,          lru_array_nxt,          lru_array_nxt)
    `EN_FF(clock_rbox,        spill_ptr_en,          spill_ptr_ff,          spill_ptr_nxt                        )
    `EN_FF(clock_rbox,        flush_ptr_en,          flush_ptr_ff,          flush_ptr_nxt                        )
    `EN_FF(clock_rbox,        spill_pending_acks_en, spill_pending_acks_ff, spill_pending_acks_nxt               )
    `EN_FF(clock_rbox,        flush_done_en,         flush_done_ff,         flush_done_nxt                       )
    `EN_FF(clock_rbox,        flush_spill_done_en,   flush_spill_done_ff,   flush_spill_done_nxt                 )

always_comb begin
    lru_ptr = lru_array_ff[`RBOX_DEPTH_BUF_SZ - 1];
    db_fetch_ptr = (|tag_match) ? match_ptr : lru_ptr;
    fetch_accept = db_fetch_valid && ((|tag_match) || (!reserved_ff[lru_ptr] && fetch_fill_done));
    for (integer i = 0; i < `RBOX_DEPTH_BUF_SZ; i++)
        lru_match[i] = (lru_array_ff[i] == db_fetch_ptr);
end

genvar entry;
generate
    for (entry = 0; entry < `RBOX_DEPTH_BUF_SZ; entry++)
        always_comb
            update_lru[entry] = (|lru_match[(`RBOX_DEPTH_BUF_SZ-1):entry]) && fetch_accept;
endgenerate

always_comb begin
    lru_array_en = '0; lru_array_nxt = lru_array_ff;
    for (integer i = (`RBOX_DEPTH_BUF_SZ - 1); i >= 0; i--) begin
        if (reset) begin lru_array_en[i] = 1'b1; lru_array_nxt[i] = i; end
        else if (update_lru[i]) begin lru_array_en[i] = 1'b1; lru_array_nxt[i] = (i == 0) ? db_fetch_ptr : lru_array_ff[i - 1]; end
    end
end

always_comb begin
    match_ptr = '0;
    for (integer i = 0; i < `RBOX_DEPTH_BUF_SZ; i++) begin
        tag_match[i] = (reserved_ff[i] || valid_ff[i]) && (db_fetch_address[`RBOX_DEPTH_BUF_TAG_R] == tags_ff[i]);
        match_ptr = tag_match[i] ? i : match_ptr;
    end
end

logic fetch_fill_received;
logic [`RBOX_DEPTH_BUF_I_R] fetch_fill_ptr;
logic spill_ack_received, spill_done;

always_comb begin
    tags_en = '0; valid_en = '0; dirty_en = '0; reserved_en = '0; dealloc_quad_en = '0;
    spill_pending_acks_en = 1'b0; db_fetch_done = 1'b0;
    tags_nxt = tags_ff; valid_nxt = valid_ff; dirty_nxt = dirty_ff;
    reserved_nxt = reserved_ff; dealloc_quad_nxt = dealloc_quad_ff;
    spill_pending_acks_nxt = spill_pending_acks_ff;

    if (reset) begin spill_pending_acks_en = 1'b1; spill_pending_acks_nxt = '0; end
    if (fetch_fill_done) begin tags_en[db_fetch_ptr] = 1'b1; tags_nxt[db_fetch_ptr] = db_fetch_address[`RBOX_DEPTH_BUF_TAG_R]; end
    if (fetch_accept) begin
        reserved_en[db_fetch_ptr] = 1'b1; dealloc_quad_en[db_fetch_ptr] = 1'b1;
        reserved_nxt[db_fetch_ptr] = 1'b1; dealloc_quad_nxt[db_fetch_ptr] = db_fetch_quad;
        db_fetch_done = 1'b1;
    end
    if (db_dealloc_valid && (dealloc_quad_ff[db_dealloc_ptr] == db_dealloc_quad))
    begin reserved_en[db_dealloc_ptr] = 1'b1; reserved_nxt[db_dealloc_ptr] = 1'b0; end
    if (db_write_done) begin dirty_en[db_write_ptr] = 1'b1; dirty_nxt[db_write_ptr] = 1'b1; end
    if (fetch_fill_received) begin
        valid_en[fetch_fill_ptr] = 1'b1; valid_nxt[fetch_fill_ptr] = 1'b1;
        dirty_en[fetch_fill_ptr] = 1'b1; dirty_nxt[fetch_fill_ptr] = 1'b0;
    end
    if (spill_done) begin
        valid_en[spill_ptr_ff] = 1'b1; valid_nxt[spill_ptr_ff] = 1'b0;
        dirty_en[spill_ptr_ff] = 1'b0; dirty_nxt[spill_ptr_ff] = 1'b0;
        spill_pending_acks_en = 1'b1; spill_pending_acks_nxt++;
    end
    if (spill_ack_received) begin spill_pending_acks_en = 1'b1; spill_pending_acks_nxt--; end
    if (dbg_db_meta_write_valid) begin
        tags_en[dbg_ram_ptr] = 1'b1; dirty_en[dbg_ram_ptr] = 1'b1; valid_en[dbg_ram_ptr] = 1'b1;
        reserved_en[dbg_ram_ptr] = 1'b1; dealloc_quad_en[dbg_ram_ptr] = 1'b1;
        {tags_nxt[dbg_ram_ptr], dirty_nxt[dbg_ram_ptr], valid_nxt[dbg_ram_ptr],
         reserved_nxt[dbg_ram_ptr], dealloc_quad_nxt[dbg_ram_ptr]} = dbg_db_meta_write_data;
    end
end

et_link_req_info_t db_sc_req_ff, db_sc_req_nxt;
logic [(`SC_BANKS+1)-1:0] db_sc_req_valid_ff, db_sc_req_valid_nxt;
logic db_sc_req_en;

`RST_EN_FF(clock_rbox, reset, db_sc_req_en, db_sc_req_valid_ff, db_sc_req_valid_nxt, '0)
    `EN_FF(clock_rbox,        db_sc_req_en, db_sc_req_ff,       db_sc_req_nxt          )

logic fetch_spill_read, flush_spill_read, spill_read;
logic [`PA_RANGE] spill_address, flush_address;
logic fetch_fill_write;
logic [`RBOX_DEPTH_LINE_R] fetch_fill_data, write_data;
logic [`RBOX_DEPTH_BUF_I_R] write_ptr;
logic write_valid;
logic [`RBOX_DEPTH_BUF_I_R] read_ptr;
logic [`RBOX_DEPTH_LINE_R] read_data;
logic read_valid, db_sc_req_accepted;

always_comb begin
    db_sc_req_accepted = db_sc_req_ready || !(|db_sc_req_valid_ff);
    fetch_spill_read = db_fetch_valid && !dbg_db_read_valid && !dbg_db_write_valid
                     && !(|tag_match) && valid_ff[db_fetch_ptr] && dirty_ff[db_fetch_ptr];
    flush_spill_read = db_flush && !dbg_db_read_valid && !dbg_db_write_valid
                     && valid_ff[flush_ptr_ff] && dirty_ff[flush_ptr_ff];
    spill_read = fetch_spill_read || flush_spill_read;
    spill_ptr_en = spill_read; spill_ptr_nxt = db_flush ? flush_ptr_ff : db_fetch_ptr;
    spill_done = spill_pending_ff && db_sc_req_accepted;
    fetch_fill_write = db_fetch_valid && !dbg_db_read_valid && !dbg_db_write_valid
                     && !(|tag_match) && (spill_done || !fetch_spill_read);
    fetch_fill_done = fetch_fill_pending_ff && db_sc_req_accepted
                    || fetch_fill_write && db_sc_req_accepted;
    spill_pending_en = !spill_pending_ff && spill_read || spill_done;
    spill_pending_nxt = !spill_pending_ff && spill_read;
    fetch_fill_pending_en = !fetch_fill_pending_ff && !fetch_fill_done
                          || fetch_fill_pending_ff && db_sc_req_accepted;
    fetch_fill_pending_nxt = !fetch_fill_pending_ff && fetch_fill_write && !fetch_fill_done;
    db_sc_req_en = db_sc_req_ready;
    db_sc_req_valid_nxt = db_sc_req_ready ? '0 : db_sc_req_valid_ff;
    db_sc_req_nxt = '0; spill_address = '0;

    if (spill_pending_ff && db_sc_req_accepted) begin
        spill_address = {tags_ff[spill_ptr_ff], 6'b0};
        db_sc_req_nxt.source = 0; db_sc_req_nxt.wdata = 1; db_sc_req_nxt.size = ET_LINK_Line;
        db_sc_req_nxt.qwen = 4'b1111; db_sc_req_nxt.opcode = ET_LINK_REQ_Write;
        db_sc_req_nxt.subopcode = '0;
        db_sc_req_nxt.id = `ZX(`ET_LINK_ID_SIZE, spill_ptr_ff) + 8'h80;
        db_sc_req_nxt.address = spill_address; db_sc_req_nxt.data = read_data;
        db_sc_req_valid_nxt[spill_address[`SC_BANK_SEL_ADDR_RANGE]] = 1'b1;
        db_sc_req_en = 1'b1;
    end
    if (!fetch_fill_pending_ff && fetch_fill_write) begin
        db_sc_req_nxt.source = 0; db_sc_req_nxt.wdata = 0; db_sc_req_nxt.size = ET_LINK_Line;
        db_sc_req_nxt.qwen = 4'b0000; db_sc_req_nxt.opcode = ET_LINK_REQ_Read;
        db_sc_req_nxt.subopcode = '0;
        db_sc_req_nxt.id = `ZX(`ET_LINK_ID_SIZE, db_fetch_ptr) + 8'h80;
        db_sc_req_nxt.address = {db_fetch_address[`RBOX_DEPTH_BUF_TAG_R], 6'b0};
        db_sc_req_valid_nxt[db_fetch_address[`SC_BANK_SEL_ADDR_RANGE]] = 1'b1;
        db_sc_req_en = 1'b1;
    end
    db_sc_req = db_sc_req_ff; db_sc_req_valid = db_sc_req_valid_ff;
end

always_comb begin
    read_valid = spill_read || db_read_valid && valid_ff[db_read_ptr] || dbg_db_read_valid;
    read_ptr = dbg_db_read_valid ? dbg_ram_ptr : (spill_read ? spill_ptr_nxt : db_read_ptr);
    fetch_fill_received = sc_db_rsp_valid && (sc_db_rsp.opcode == ET_LINK_RSP_AckData) && sc_db_rsp.id[`ET_LINK_ID_SIZE-1];
    fetch_fill_ptr = sc_db_rsp.id[`RBOX_DEPTH_BUF_I_R];
    fetch_fill_data = sc_db_rsp.data;
    spill_ack_received = sc_db_rsp_valid && (sc_db_rsp.opcode == ET_LINK_RSP_Ack) && sc_db_rsp.id[`ET_LINK_ID_SIZE-1];
    write_valid = fetch_fill_received || db_write_valid;
    write_ptr = dbg_db_write_valid ? dbg_ram_ptr : (fetch_fill_received ? fetch_fill_ptr : db_write_ptr);
    write_data = dbg_db_write_valid ? dbg_db_write_data : (fetch_fill_received ? fetch_fill_data : db_write_data);
    db_write_done = db_write_valid && !fetch_fill_received;
    db_read_data = read_data;
    db_read_data_valid = db_read_valid && !dbg_db_read_valid && !spill_read && valid_ff[db_read_ptr];
end

always_comb begin
    flush_ptr_en = 1'b0; flush_done_en = 1'b0; flush_spill_done_en = 1'b0;
    flush_ptr_nxt = '0; flush_spill_done_nxt = 1'b0; flush_done_nxt = 1'b0;
    if (reset) begin flush_ptr_en = 1'b1; flush_ptr_nxt = '0; end
    if ((db_flush && !flush_spill_done_ff)
        && ((valid_ff[flush_ptr_ff] && dirty_ff[flush_ptr_ff] && spill_done)
            || (!valid_ff[flush_ptr_ff] || !dirty_ff[flush_ptr_ff])))
    begin
        flush_ptr_en = 1'b1; flush_ptr_nxt = flush_ptr_ff + 'd1;
        flush_spill_done_en = (flush_ptr_ff == '1); flush_spill_done_nxt = (flush_ptr_ff == '1);
    end
    flush_spill_done_en = reset || flush_spill_done_en || flush_done_ff;
    flush_spill_done_nxt = !reset && flush_spill_done_nxt;
    flush_done_en = reset || !flush_done_ff && flush_spill_done_ff && (spill_pending_acks_ff == '0) || flush_done_ff;
    flush_done_nxt = !reset && flush_spill_done_ff && (spill_pending_acks_ff == '0) && !flush_done_ff;
    db_flush_done = flush_done_ff;
end

always_comb begin
    dbg_db_read_data = read_data;
    dbg_db_meta_read_data = {valid_ff[dbg_ram_ptr[`RBOX_DEPTH_BUF_I_R]], dirty_ff[dbg_ram_ptr[`RBOX_DEPTH_BUF_I_R]],
                             reserved_ff[dbg_ram_ptr[`RBOX_DEPTH_BUF_I_R]], dealloc_quad_ff[dbg_ram_ptr[`RBOX_DEPTH_BUF_I_R]],
                             tags_ff[dbg_ram_ptr[`RBOX_DEPTH_BUF_I_R]]};
end

gen_mem2p #(.WIDTH(`RBOX_DEPTH_LINE_SZ), .WORDS(`RBOX_DEPTH_BUF_SZ))
depth_buffer_data (
    .clock(clock), .addressA(read_ptr), .enA(read_valid), .weA(1'b0), .dinA('0), .doutA(read_data),
    .addressB(write_ptr), .enB(write_valid), .weB(1'b1 && !esr_shire_cache_ram_cfg.dft__ram_wei),
    .dinB(write_data), .doutB(/*unused*/)
);

wire unused_ok = &{1'b0, esr_shire_cache_ram_cfg, esr_shire_cache_ram_cfg};

endmodule
