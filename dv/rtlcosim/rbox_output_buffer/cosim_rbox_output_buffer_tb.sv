// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_output_buffer vs new rbox_output_buffer.

`include "soc.vh"

module cosim_rbox_output_buffer_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Shared stimulus — quad data as flat bits (matches quad_data_t / rbox_quad_data_t)
  input  logic [467:0]      tr_quad_bits_i,
  input  logic              tr_quad_valid_i,
  input  logic [3:0]        ts_result_mask_i,
  input  logic              ts_result_valid_i,
  input  logic              out_pop_i,
  input  logic [2:0]        out_pending_pcks_i,
  input  logic              out_end_drawcall_i,
  input  logic [1:0]        hart_tile_width_i,
  input  logic [1:0]        hart_tile_height_i,
  input  logic [1:0]        shire_layout_width_i,
  input  logic [1:0]        shire_layout_height_i,
  input  logic              fsh_reads_coverage_i,
  input  logic              fsh_reads_depth_i,
  input  logic              fsh_disabled_i,
  input  logic              early_frag_tests_enable_i,
  input  logic              rbox_halted_i,

  // New module outputs
  output logic              new_ct_quad_accept_o,
  output logic              new_ct_result_accept_o,
  output logic [57:0]       new_out_info_bits_o,
  output logic              new_out_info_valid_o,
  output logic [255:0]      new_out_packet_o,
  output logic              new_out_packet_valid_o,
  output logic              new_empty_o,

  // Original module outputs
  output logic              orig_ct_quad_accept_o,
  output logic              orig_ct_result_accept_o,
  output logic [57:0]       orig_out_info_bits_o,
  output logic              orig_out_info_valid_o,
  output logic [255:0]      orig_out_packet_o,
  output logic              orig_out_packet_valid_o,
  output logic              orig_empty_o
);

  import rbox_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  // Adapt reset
  logic reset;
  assign reset = ~rst_ni;

  // DFT/config = disabled
  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  assign orig_ram_cfg = '0;

  // Cast flat bits to typed structs
  rbox_pkg::quad_data_t  new_tr_quad;
  rbox_quad_data_t       orig_tr_quad;
  assign new_tr_quad  = rbox_pkg::quad_data_t'(tr_quad_bits_i);
  assign orig_tr_quad = rbox_quad_data_t'(tr_quad_bits_i);

  // Output info from both modules — expose as raw bits
  rbox_pkg::quad_info_t [1:0]  new_out_info;
  rbox_quad_info_t      [1:0]  orig_out_info;
  assign new_out_info_bits_o  = new_out_info;
  assign orig_out_info_bits_o = orig_out_info;

  // ── New module ────────────────────────────────────────

  rbox_output_buffer u_new (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_ni),
    .ram_cfg_i                (ram_cfg_t'('0)),
    .dft_i                    (dft_t'('0)),
    .tr_quad_i                (new_tr_quad),
    .tr_quad_valid_i          (tr_quad_valid_i),
    .ct_quad_accept_o         (new_ct_quad_accept_o),
    .ts_result_mask_i         (ts_result_mask_i),
    .ts_result_valid_i        (ts_result_valid_i),
    .ct_result_accept_o       (new_ct_result_accept_o),
    .out_info_o               (new_out_info),
    .out_info_valid_o         (new_out_info_valid_o),
    .out_packet_o             (new_out_packet_o),
    .out_packet_valid_o       (new_out_packet_valid_o),
    .out_pop_i                (out_pop_i),
    .out_pending_pcks_i       (out_pending_pcks_i),
    .out_end_drawcall_i       (out_end_drawcall_i),
    .empty_o                  (new_empty_o),
    .hart_tile_width_i        (hart_tile_width_i),
    .hart_tile_height_i       (hart_tile_height_i),
    .shire_layout_width_i     (shire_layout_width_i),
    .shire_layout_height_i    (shire_layout_height_i),
    .fsh_reads_coverage_i     (fsh_reads_coverage_i),
    .fsh_reads_depth_i        (fsh_reads_depth_i),
    .fsh_disabled_i           (fsh_disabled_i),
    .early_frag_tests_enable_i(early_frag_tests_enable_i),
    .rbox_halted_i            (rbox_halted_i)
  );

  // ── Original module (inlined + renamed) ───────────────

  rbox_output_buffer_orig u_orig (
    .clock                    (clk_i),
    .reset                    (reset),
    .esr_shire_cache_ram_cfg  (orig_ram_cfg),
    .tr_quad                  (orig_tr_quad),
    .tr_quad_valid            (tr_quad_valid_i),
    .ct_quad_accept           (orig_ct_quad_accept_o),
    .ts_result_mask           (ts_result_mask_i),
    .ts_result_valid          (ts_result_valid_i),
    .ct_result_accept         (orig_ct_result_accept_o),
    .out_info                 (orig_out_info),
    .out_info_valid           (orig_out_info_valid_o),
    .out_packet               (orig_out_packet_o),
    .out_packet_valid         (orig_out_packet_valid_o),
    .out_pop                  (out_pop_i),
    .out_pending_pcks         (out_pending_pcks_i),
    .out_end_drawcall         (out_end_drawcall_i),
    .empty                    (orig_empty_o),
    .hart_tile_width          (hart_tile_width_i),
    .hart_tile_height         (hart_tile_height_i),
    .shire_layout_width       (shire_layout_width_i),
    .shire_layout_height      (shire_layout_height_i),
    .fsh_reads_coverage       (fsh_reads_coverage_i),
    .fsh_reads_depth          (fsh_reads_depth_i),
    .fsh_disabled             (fsh_disabled_i),
    .early_frag_tests_enable  (early_frag_tests_enable_i),
    .rbox_halted              (rbox_halted_i)
  );

endmodule

// ════════════════════════════════════════════════════════════
// Renamed copy of original rbox_output_buffer
// ════════════════════════════════════════════════════════════

module rbox_output_buffer_orig
(
    input  logic                           clock,
    input  logic                           reset,
    input  esr_shire_cache_ram_cfg_t       esr_shire_cache_ram_cfg,

    input  rbox_quad_data_t                tr_quad,
    input  logic                           tr_quad_valid,
    output logic                           ct_quad_accept,

    input  logic [3:0]                     ts_result_mask,
    input  logic                           ts_result_valid,
    output logic                           ct_result_accept,

    output rbox_quad_info_t [1:0]          out_info,
    output logic                           out_info_valid,
    output logic [255:0]                   out_packet,
    output logic                           out_packet_valid,
    input  logic                           out_pop,
    input  logic [`RBOX_MAX_QUAD_PCKS_I_R] out_pending_pcks,

    input  logic                           out_end_drawcall,

    output logic                           empty,

    input  logic [1:0]                     hart_tile_width,
    input  logic [1:0]                     hart_tile_height,
    input  logic [1:0]                     shire_layout_width,
    input  logic [1:0]                     shire_layout_height,
    input  logic                           fsh_reads_coverage,
    input  logic                           fsh_reads_depth,
    input  logic                           fsh_disabled,
    input  logic                           early_frag_tests_enable,

    input  logic                           rbox_halted
);

logic [`RBOX_OUT_BUF_BARY_DATA_R] bary_rd_data;
logic                             bary_rd_en;
logic                             bary_rd_valid;
logic [`RBOX_OUT_BUF_BARY_DATA_R] bary_wr_data;
logic                             bary_wr_en;
logic                             bary_wr_done;

rbox_fifo
#(
    .WIDTH    (`RBOX_OUT_BUF_BARY_DATA_SZ),
    .DEPTH    (`RBOX_OUT_BUF_SZ          ),
    .PORTS    (2                         )
 )
fifo_bary_coord
(
    .clock                   (clock                    ),
    .reset                   (reset                    ),
    .esr_shire_cache_ram_cfg (esr_shire_cache_ram_cfg  ),
    .rd_data                 (bary_rd_data             ),
    .rd_en                   (bary_rd_en               ),
    .rd_valid                (bary_rd_valid            ),
    .wr_data                 (bary_wr_data             ),
    .wr_en                   (bary_wr_en               ),
    .wr_done                 (bary_wr_done             )
);

logic [`RBOX_OUT_BUF_DEPTH_DATA_R] depth_rd_data;
logic                              depth_rd_en;
logic                              depth_rd_valid;
logic [`RBOX_OUT_BUF_DEPTH_DATA_R] depth_wr_data;
logic                              depth_wr_en;
logic                              depth_wr_done;

rbox_fifo
#(
    .WIDTH    (`RBOX_OUT_BUF_DEPTH_DATA_SZ),
    .DEPTH    (`RBOX_OUT_BUF_SZ           ),
    .PORTS    (2                          )
 )
fifo_depth
(
    .clock                   (clock                   ),
    .reset                   (reset                   ),
    .esr_shire_cache_ram_cfg (esr_shire_cache_ram_cfg ),
    .rd_data                 (depth_rd_data           ),
    .rd_en                   (depth_rd_en             ),
    .rd_valid                (depth_rd_valid          ),
    .wr_data                 (depth_wr_data           ),
    .wr_en                   (depth_wr_en             ),
    .wr_done                 (depth_wr_done           )
);

rbox_quad_info_t [`RBOX_OUT_BUF_E_R]                    quad_info_nxt;
rbox_quad_info_t [`RBOX_OUT_BUF_E_R]                    quad_info_ff;
logic            [`RBOX_OUT_BUF_E_R]                    quad_info_en;

logic [`RBOX_OUT_BUF_I_R]                               quad_wr_ptr_ff;
logic [`RBOX_OUT_BUF_I_R]                               quad_wr_ptr_nxt;
logic                                                   quad_wr_ptr_en;

logic [`RBOX_OUT_BUF_I_R]                               depth_wr_ptr_ff;
logic [`RBOX_OUT_BUF_I_R]                               depth_wr_ptr_nxt;
logic                                                   depth_wr_ptr_en;

logic [`RBOX_OUT_BUF_I_R]                               rd_ptr_ff;
logic [`RBOX_OUT_BUF_I_R]                               rd_ptr_nxt;
logic [`RBOX_OUT_BUF_I_R]                               rd_ptr_nxt_ff;
logic [`RBOX_OUT_BUF_I_R]                               rd_ptr_nxt_nxt;
logic                                                   rd_ptr_en;

logic                                                   empty_ff;
logic                                                   empty_nxt;
logic                                                   empty_en;

logic [`RBOX_OUT_BUF_E_R][3:0][`RBOX_MAX_MSAA_SAMP_RNG] quad_msaa_cov_ff;
logic [`RBOX_OUT_BUF_E_R][3:0][`RBOX_MAX_MSAA_SAMP_RNG] quad_msaa_cov_nxt;
logic [`RBOX_OUT_BUF_E_R]                               quad_msaa_cov_en;

logic [`RBOX_MAX_TILE_DIM_R]                            last_col_ff;
logic [`RBOX_MAX_TILE_DIM_R]                            last_col_nxt;
logic                                                   last_col_en;
logic [`RBOX_MAX_TILE_DIM_R]                            last_row_ff;
logic [`RBOX_MAX_TILE_DIM_R]                            last_row_nxt;
logic                                                   last_row_en;

logic                                                   bary_wr_done_hold_ff;
logic                                                   bary_wr_done_hold_nxt;
logic                                                   bary_wr_done_hold_en;
logic                                                   depth_wr_done_hold_ff;
logic                                                   depth_wr_done_hold_nxt;
logic                                                   depth_wr_done_hold_en;

logic                                                   bary_rd_done;
logic                                                   bary_rd_valid_en;
logic [1:0]                                             bary_rd_valid_ff;
logic [1:0]                                             bary_rd_valid_nxt;
logic [1:0][`RBOX_OUT_BUF_BARY_DATA_R]                  bary_rd_data_ff;
logic      [`RBOX_OUT_BUF_BARY_DATA_R]                  bary_rd_data_nxt;
logic [1:0]                                             bary_rd_data_en;
logic                                                   depth_rd_valid_en;
logic [1:0]                                             depth_rd_valid_ff;
logic                                                   depth_rd_done;
logic [1:0]                                             depth_rd_valid_nxt;
logic [1:0][`RBOX_OUT_BUF_DEPTH_DATA_R]                 depth_rd_data_ff;
logic      [`RBOX_OUT_BUF_DEPTH_DATA_R]                 depth_rd_data_nxt;
logic [1:0]                                             depth_rd_data_en;

logic                                                   out_info_en;
rbox_quad_info_t [1:0]                                  out_info_nxt;
rbox_quad_info_t [1:0]                                  out_info_ff;
logic                                                   out_info_valid_en;
logic                                                   out_info_valid_nxt;
logic                                                   out_info_valid_ff;

logic                                                   out_depth_enabled;

logic                                                   out_pck_bary_i;
logic                                                   out_pck_bary_j;
logic                                                   out_pck_depth ;
logic                                                   out_pck_cov;

logic                                                   force_empty_quad;

`EN_FF(clock, quad_wr_ptr_en,        quad_wr_ptr_ff,        quad_wr_ptr_nxt       )
`EN_FF(clock, depth_wr_ptr_en,       depth_wr_ptr_ff,       depth_wr_ptr_nxt      )
`EN_FF(clock, rd_ptr_en,             rd_ptr_ff,             rd_ptr_nxt            )
`EN_FF(clock, rd_ptr_en,             rd_ptr_nxt_ff,         rd_ptr_nxt_nxt        )
`EN_FF(clock, empty_en,              empty_ff,              empty_nxt             )
`EN_FF(clock, bary_wr_done_hold_en,  bary_wr_done_hold_ff,  bary_wr_done_hold_nxt )
`EN_FF(clock, depth_wr_done_hold_en, depth_wr_done_hold_ff, depth_wr_done_hold_nxt)
`EN_FF(clock, last_col_en,           last_col_ff,           last_col_nxt          )
`EN_FF(clock, last_row_en,           last_row_ff,           last_row_nxt          )
`EN_FF(clock, out_info_en,           out_info_ff,           out_info_nxt          )
`EN_FF(clock, out_info_valid_en,     out_info_valid_ff,     out_info_valid_nxt    )
`EN_FF(clock, bary_rd_valid_en,      bary_rd_valid_ff,      bary_rd_valid_nxt     )
`EN_FF(clock, depth_rd_valid_en,     depth_rd_valid_ff,     depth_rd_valid_nxt    )


genvar quad;
generate
    for (quad = 0; quad < `RBOX_OUT_BUF_SZ; quad++)
    begin
        `EN_FF(clock,     quad_info_en[quad],     quad_info_ff[quad],     quad_info_nxt[quad])
        `EN_FF(clock, quad_msaa_cov_en[quad], quad_msaa_cov_ff[quad], quad_msaa_cov_nxt[quad])
    end
    for (quad = 0; quad < 2; quad++)
    begin
        `EN_FF(clock,  bary_rd_data_en[quad],  bary_rd_data_ff[quad],  bary_rd_data_nxt);
        `EN_FF(clock, depth_rd_data_en[quad], depth_rd_data_ff[quad], depth_rd_data_nxt);
    end
endgenerate


always_comb
begin

    quad_wr_ptr_en    = reset;
    depth_wr_ptr_en   = reset;
    rd_ptr_en         = reset;
    quad_info_en      = {$bits(quad_info_en){reset}};
    last_col_en       = reset;
    last_row_en       = reset;
    quad_msaa_cov_en  = '0;

    quad_info_nxt     = '0;
    quad_msaa_cov_nxt = '0;
    last_col_nxt      = '0;
    last_row_nxt      = '0;

    bary_wr_data      = '0;
    bary_wr_en        = 1'b0;
    bary_rd_en        = 1'b0;

    depth_wr_data     = '0;
    depth_wr_en       = 1'b0;
    depth_rd_en       = 1'b0;

    bary_wr_done_hold_en  = reset;
    depth_wr_done_hold_en = reset;

    ct_quad_accept   = 1'b0;
    ct_result_accept = 1'b0;

    quad_wr_ptr_nxt  = reset ? '0  : quad_wr_ptr_ff;
    depth_wr_ptr_nxt = reset ? '0  : depth_wr_ptr_ff;
    rd_ptr_nxt       = reset ? 'd0 : rd_ptr_ff;
    rd_ptr_nxt_nxt   = reset ? 'd1 : rd_ptr_nxt_ff;

    out_depth_enabled  = !early_frag_tests_enable || fsh_reads_depth;

    if (tr_quad_valid && !rbox_halted)
    begin
         quad_info_en[quad_wr_ptr_ff] = 1'b1;
        quad_info_nxt[quad_wr_ptr_ff].x          = tr_quad.x;
        quad_info_nxt[quad_wr_ptr_ff].y          = tr_quad.y;
        quad_info_nxt[quad_wr_ptr_ff].mask       = early_frag_tests_enable ? '1 : tr_quad.mask;
        quad_info_nxt[quad_wr_ptr_ff].sample_idx = tr_quad.sample_idx;
        quad_info_nxt[quad_wr_ptr_ff].valid      = 1'b1;
        quad_info_nxt[quad_wr_ptr_ff].start_pack =  ((tr_quad.x >> (hart_tile_width  + 1)) != (last_col_ff >> (hart_tile_width  + 1)))
                                                 || ((tr_quad.y >> (hart_tile_height + 1)) != (last_row_ff >> (hart_tile_height + 1)));
        quad_info_nxt[quad_wr_ptr_ff].test_done  = !early_frag_tests_enable;
        quad_info_nxt[quad_wr_ptr_ff].hart_id =  f_quad_to_hart(hart_tile_width,
                                                                hart_tile_height,
                                                                shire_layout_width,
                                                                shire_layout_height,
                                                                tr_quad.x,
                                                                tr_quad.y);

        quad_msaa_cov_nxt[quad_wr_ptr_ff] = tr_quad.msaa_coverage;
         quad_msaa_cov_en[quad_wr_ptr_ff] = fsh_reads_coverage && !early_frag_tests_enable;

        last_col_nxt = tr_quad.x;
        last_col_en  = 1'b1;
        last_row_nxt = tr_quad.y;
        last_row_en  = 1'b1;

        if (!fsh_disabled)
        begin
            bary_wr_en   = !bary_wr_done_hold_ff;
            bary_wr_data = {tr_quad.j, tr_quad.i};
        end

        if (out_depth_enabled)
        begin
            depth_wr_en   = !depth_wr_done_hold_ff;
            depth_wr_data = tr_quad.depth;
        end

        ct_quad_accept = ( fsh_disabled      ||  bary_wr_done ||  bary_wr_done_hold_ff)
                      && (!out_depth_enabled || depth_wr_done || depth_wr_done_hold_ff)
                      && !rbox_halted;

        quad_wr_ptr_en = ct_quad_accept;

        // Increment works as long as the number of entries is a power of 2.
        quad_wr_ptr_nxt++;
    end

    if (ts_result_valid && !rbox_halted)
    begin
         quad_info_en[depth_wr_ptr_ff]           = 1'b1;
        quad_info_nxt[depth_wr_ptr_ff]           = quad_info_ff[depth_wr_ptr_ff];
        quad_info_nxt[depth_wr_ptr_ff].test_done = 1'b1;

        quad_info_nxt[depth_wr_ptr_ff].mask      = quad_info_nxt[depth_wr_ptr_ff].mask & ts_result_mask;

        for (integer q = 0; q < 4; q++)
            quad_msaa_cov_nxt[depth_wr_ptr_ff][q][quad_info_ff[depth_wr_ptr_ff].sample_idx] = ts_result_mask[q];

        quad_msaa_cov_en[depth_wr_ptr_ff] = fsh_reads_coverage && early_frag_tests_enable;

        // Increment works as long as the number of entries is a power of 2.
        depth_wr_ptr_nxt++;
        depth_wr_ptr_en = 1'b1;

        ct_result_accept = 1'b1;
    end

    bary_wr_done_hold_nxt  = reset ? '0 : (   bary_wr_done_hold_ff && !ct_quad_accept
                                           || bary_wr_done && !reset);

    depth_wr_done_hold_nxt = reset ? '0 : (   depth_wr_done_hold_ff && !ct_quad_accept
                                           || depth_wr_done);


    force_empty_quad =    quad_info_ff[rd_ptr_nxt_ff].start_pack
                     || (!quad_info_ff[rd_ptr_nxt_ff].valid && out_end_drawcall);

    out_info_nxt[0] = quad_info_ff[rd_ptr_ff    ];
    out_info_nxt[1] = quad_info_ff[rd_ptr_nxt_ff];

    // Force when to flush draw call.
    out_info_nxt[1].start_pack = force_empty_quad;

    out_info_valid_nxt = !reset
                      && (       quad_info_ff[rd_ptr_ff    ].valid && quad_info_ff[rd_ptr_ff    ].test_done
                          && (  (quad_info_ff[rd_ptr_nxt_ff].valid && quad_info_ff[rd_ptr_nxt_ff].test_done)
                              || force_empty_quad));

    // Compute when there is a new pair of quads that need to be loaded into the flop.
    out_info_en =  out_info_valid_nxt && !out_info_valid_ff
                || out_info_valid_nxt &&  out_pop;

    out_info_valid_en =  out_info_valid_nxt && !out_info_valid_ff
                      || out_pop
                      || reset;

     bary_rd_done      = 1'b0;
    depth_rd_done      = 1'b0;
     bary_rd_valid_nxt = '0;
    depth_rd_valid_nxt = '0;
     bary_rd_data_en   = '0;
    depth_rd_data_en   = '0;
     bary_rd_valid_en  = 1'b0;
    depth_rd_valid_en  = 1'b0;

     bary_rd_data_nxt  =  bary_rd_data;
    depth_rd_data_nxt  = depth_rd_data;

    out_pck_bary_i     = 1'b0;
    out_pck_bary_j     = 1'b0;
    out_pck_depth      = 1'b0;
    out_pck_cov        = 1'b0;

    out_packet_valid   = 1'b0;
    out_packet         = '0;

    if (out_info_valid_ff)
    begin
         bary_rd_done =  bary_rd_valid ||  fsh_disabled;
        depth_rd_done = depth_rd_valid || !fsh_reads_depth;

         bary_rd_valid_nxt = { bary_rd_valid_ff[1] ||  (bary_rd_valid_ff[0] &&  bary_rd_done) || out_info_ff[1].start_pack,
                               bary_rd_valid_ff[0] ||                           bary_rd_done};
        depth_rd_valid_nxt = {depth_rd_valid_ff[1] || (depth_rd_valid_ff[0] && depth_rd_done) || out_info_ff[1].start_pack,
                              depth_rd_valid_ff[0] ||                          depth_rd_done};

         bary_rd_valid_en = 1'b1;
        depth_rd_valid_en = 1'b1;

         bary_rd_en = ( bary_rd_valid_nxt != 2'b11) && !fsh_disabled;
        depth_rd_en = (depth_rd_valid_nxt != 2'b11) &&  out_depth_enabled;

         bary_rd_data_en[0] =  !bary_rd_valid_ff[0] &&  bary_rd_done;
        depth_rd_data_en[0] = !depth_rd_valid_ff[0] && depth_rd_done;

         bary_rd_data_en[1] =   bary_rd_valid_ff[0] &&  bary_rd_done;
        depth_rd_data_en[1] =  depth_rd_valid_ff[0] && depth_rd_done;

        out_pck_bary_i =  !fsh_disabled && !out_depth_enabled && !fsh_reads_coverage && (out_pending_pcks == 'd2)
                       || !fsh_disabled &&  out_depth_enabled && !fsh_reads_coverage && (out_pending_pcks == 'd3)
                       || !fsh_disabled && !out_depth_enabled &&  fsh_reads_coverage && (out_pending_pcks == 'd3)
                       || !fsh_disabled &&  out_depth_enabled &&  fsh_reads_coverage && (out_pending_pcks == 'd4);
        out_pck_bary_j =  !fsh_disabled && !out_depth_enabled && !fsh_reads_coverage && (out_pending_pcks == 'd1)
                       || !fsh_disabled &&  out_depth_enabled && !fsh_reads_coverage && (out_pending_pcks == 'd2)
                       || !fsh_disabled && !out_depth_enabled &&  fsh_reads_coverage && (out_pending_pcks == 'd2)
                       || !fsh_disabled &&  out_depth_enabled &&  fsh_reads_coverage && (out_pending_pcks == 'd3);
        out_pck_depth  =                    out_depth_enabled && !fsh_reads_coverage && (out_pending_pcks == 'd1)
                       ||                   out_depth_enabled &&  fsh_reads_coverage && (out_pending_pcks == 'd2);
        out_pck_cov    =                                          fsh_reads_coverage && (out_pending_pcks == 'd1);


        case (1'b1)
            out_pck_bary_i : {out_packet_valid, out_packet} = {!rbox_halted && ( bary_rd_valid_ff == 2'b11), bary_rd_data_ff[1][127:  0], bary_rd_data_ff[0][127:  0]};
            out_pck_bary_j : {out_packet_valid, out_packet} = {!rbox_halted && ( bary_rd_valid_ff == 2'b11), bary_rd_data_ff[1][255:128], bary_rd_data_ff[0][255:128]};
            out_pck_depth  : {out_packet_valid, out_packet} = {!rbox_halted && (depth_rd_valid_ff == 2'b11), depth_rd_data_ff};
            out_pck_cov    : {out_packet_valid, out_packet} = {!rbox_halted,                                 quad_msaa_cov_ff};
        endcase

    end

    if (out_pop)
    begin
         bary_rd_valid_nxt = '0;
        depth_rd_valid_nxt = '0;

         bary_rd_valid_en = 1'b1;
        depth_rd_valid_en = 1'b1;
    end

    // Move the read pointer when the next pair of quads is loaded into the output flop.
    if (out_info_en)
    begin
        // Increment works as long as the number of entries is a power of 2.
        rd_ptr_nxt     = force_empty_quad ? (rd_ptr_nxt     + 'd1) : (rd_ptr_nxt     + 'd2);
        rd_ptr_nxt_nxt = force_empty_quad ? (rd_ptr_nxt_nxt + 'd1) : (rd_ptr_nxt_nxt + 'd2);
        rd_ptr_en = 1'b1;

        // Clear valid bit of poped entry.
         quad_info_en[rd_ptr_ff]           = 1'b1;
        quad_info_nxt[rd_ptr_ff].valid     = 1'b0;
         quad_info_en[rd_ptr_nxt_ff]       = !quad_info_ff[rd_ptr_nxt_ff].start_pack;
        quad_info_nxt[rd_ptr_nxt_ff].valid = 1'b0;
    end

     bary_rd_valid_en =  bary_rd_valid_en | reset;
    depth_rd_valid_en = depth_rd_valid_en | reset;

    out_info       = out_info_ff;
    out_info_valid = out_info_valid_ff;

    empty_en = reset || quad_wr_ptr_en || rd_ptr_en;

    empty_nxt = reset ? 1'b0 : (quad_wr_ptr_nxt == rd_ptr_nxt);

    empty = empty_ff;
end

// Assertions commented out for cosim (incompatible with Verilator without ASSERT_ALWAYS_MSG macro)
// `ASSERT_ALWAYS_MSG(ERROR_rbox_out_buf_sz_2n, clock, reset,
//                    (1 << $clog2(`RBOX_OUT_BUF_SZ)) == `RBOX_OUT_BUF_SZ,
//                    $sformatf("RBOX Output Buffer Size must be a power of 2 value : %d", `RBOX_OUT_BUF_SZ))
//
// `ASSERT_ALWAYS_MSG(ERROR_rbox_out_buf_pend_pcks, clock, reset,
//                    (!(out_pop && !(out_pending_pcks == 'd1))),
//                    $sformatf("RBOX Output Buffer expected pending packets is 1 for out_pop, found : %d",
//                    $sampled(out_pending_pcks)))

endmodule
