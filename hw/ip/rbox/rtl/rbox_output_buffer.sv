// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX output buffer.
//
// Collects quads from the traversal stage and depth test results, packs them
// into output packets (barycentric i/j, depth, MSAA coverage), and presents
// them for transfer to the cache via ET-Link.
//
// Uses two prim_fifo_sram instances (barycentric coords, depth data) and
// per-entry register arrays for quad info and MSAA coverage.

/* verilator lint_off SYNCASYNCNET */  // rst_ni intentionally drives child async-reset FIFOs and the preserved local reset/init pulse logic in this wrapper.
module rbox_output_buffer
  import rbox_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  ram_cfg_t                   ram_cfg_i,
  input  dft_t                       dft_i,

  input  quad_data_t                 tr_quad_i,
  input  logic                       tr_quad_valid_i,
  output logic                       ct_quad_accept_o,

  input  logic [3:0]                 ts_result_mask_i,
  input  logic                       ts_result_valid_i,
  output logic                       ct_result_accept_o,

  output quad_info_t [1:0]           out_info_o,
  output logic                       out_info_valid_o,
  output logic [255:0]               out_packet_o,
  output logic                       out_packet_valid_o,
  input  logic                       out_pop_i,
  input  logic [MaxQuadPcksL-1:0]    out_pending_pcks_i,

  input  logic                       out_end_drawcall_i,

  output logic                       empty_o,

  input  logic [1:0]                 hart_tile_width_i,
  input  logic [1:0]                 hart_tile_height_i,
  input  logic [1:0]                 shire_layout_width_i,
  input  logic [1:0]                 shire_layout_height_i,
  input  logic                       fsh_reads_coverage_i,
  input  logic                       fsh_reads_depth_i,
  input  logic                       fsh_disabled_i,
  input  logic                       early_frag_tests_enable_i,

  input  logic                       rbox_halted_i
);

  localparam int unsigned BufSz      = OutBufSz;            // 128
  localparam int unsigned BufIdxSz   = $clog2(BufSz);       // 7
  localparam int unsigned BaryDataSz = OutBufBaryDataSz;     // 256
  localparam int unsigned DepthDataSz = OutBufDepthDataSz;   // 128

  // ── FIFO interfaces ────────────────────────────────────

  logic [BaryDataSz-1:0]  bary_rd_data;
  logic                    bary_rd_en;
  logic                    bary_rd_valid;
  logic [BaryDataSz-1:0]  bary_wr_data;
  logic                    bary_wr_en;
  logic                    bary_wr_done;

  prim_fifo_sram #(
    .Width (BaryDataSz),
    .Depth (BufSz),
    .Ports (2)
  ) u_fifo_bary_coord (
    .clk_i     (clk_i),
    .rst_ni    (rst_ni),
    .wr_en_i   (bary_wr_en),
    .wr_data_i (bary_wr_data),
    .wr_done_o (bary_wr_done),
    .rd_en_i   (bary_rd_en),
    .rd_valid_o(bary_rd_valid),
    .rd_data_o (bary_rd_data),
    .ram_cfg_i (ram_cfg_i),
    .dft_i     (dft_i)
  );

  logic [DepthDataSz-1:0] depth_rd_data;
  logic                    depth_rd_en;
  logic                    depth_rd_valid;
  logic [DepthDataSz-1:0] depth_wr_data;
  logic                    depth_wr_en;
  logic                    depth_wr_done;

  prim_fifo_sram #(
    .Width (DepthDataSz),
    .Depth (BufSz),
    .Ports (2)
  ) u_fifo_depth (
    .clk_i     (clk_i),
    .rst_ni    (rst_ni),
    .wr_en_i   (depth_wr_en),
    .wr_data_i (depth_wr_data),
    .wr_done_o (depth_wr_done),
    .rd_en_i   (depth_rd_en),
    .rd_valid_o(depth_rd_valid),
    .rd_data_o (depth_rd_data),
    .ram_cfg_i (ram_cfg_i),
    .dft_i     (dft_i)
  );

  // ── Per-entry registers ────────────────────────────────

  quad_info_t [BufSz-1:0]                         quad_info_d;
  quad_info_t [BufSz-1:0]                         quad_info_q;
  logic       [BufSz-1:0]                         quad_info_en;

  logic [BufIdxSz-1:0]                            quad_wr_ptr_q;
  logic [BufIdxSz-1:0]                            quad_wr_ptr_d;
  logic                                            quad_wr_ptr_en;

  logic [BufIdxSz-1:0]                            depth_wr_ptr_q;
  logic [BufIdxSz-1:0]                            depth_wr_ptr_d;
  logic                                            depth_wr_ptr_en;

  logic [BufIdxSz-1:0]                            rd_ptr_q;
  logic [BufIdxSz-1:0]                            rd_ptr_d;
  logic [BufIdxSz-1:0]                            rd_ptr_next_q;
  logic [BufIdxSz-1:0]                            rd_ptr_next_d;
  logic                                            rd_ptr_en;

  logic                                            empty_q;
  logic                                            empty_d;
  logic                                            empty_en;

  /* verilator lint_off UNUSEDSIGNAL */ // Only entries at rd_ptr consumed; full array needed for indexed write
  logic [BufSz-1:0][3:0][MaxMsaaSamples-1:0]      quad_msaa_cov_q;
  logic [BufSz-1:0][3:0][MaxMsaaSamples-1:0]      quad_msaa_cov_d;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [BufSz-1:0]                                quad_msaa_cov_en;

  logic [MaxTileDimL-1:0]                          last_col_q;
  logic [MaxTileDimL-1:0]                          last_col_d;
  logic                                            last_col_en;
  logic [MaxTileDimL-1:0]                          last_row_q;
  logic [MaxTileDimL-1:0]                          last_row_d;
  logic                                            last_row_en;

  logic                                            bary_wr_done_hold_q;
  logic                                            bary_wr_done_hold_d;
  logic                                            bary_wr_done_hold_en;
  logic                                            depth_wr_done_hold_q;
  logic                                            depth_wr_done_hold_d;
  logic                                            depth_wr_done_hold_en;

  logic                                            bary_rd_done;
  logic                                            bary_rd_valid_en;
  logic [1:0]                                      bary_rd_valid_q;
  logic [1:0]                                      bary_rd_valid_d;
  logic [1:0][BaryDataSz-1:0]                     bary_rd_data_q;
  logic      [BaryDataSz-1:0]                     bary_rd_data_d;
  logic [1:0]                                      bary_rd_data_en;
  logic                                            depth_rd_valid_en;
  logic [1:0]                                      depth_rd_valid_q;
  logic                                            depth_rd_done;
  logic [1:0]                                      depth_rd_valid_d;
  logic [1:0][DepthDataSz-1:0]                    depth_rd_data_q;
  logic      [DepthDataSz-1:0]                    depth_rd_data_d;
  logic [1:0]                                      depth_rd_data_en;

  logic                                            out_info_en;
  quad_info_t [1:0]                                out_info_d;
  quad_info_t [1:0]                                out_info_q;
  logic                                            out_info_valid_en;
  logic                                            out_info_valid_d;
  logic                                            out_info_valid_q;

  logic                                            out_depth_enabled;

  logic                                            out_pck_bary_i_sel;
  logic                                            out_pck_bary_j_sel;
  logic                                            out_pck_depth_sel;
  logic                                            out_pck_cov_sel;

  logic                                            force_empty_quad;

  // ── Sequential logic ───────────────────────────────────

  always_ff @(posedge clk_i) begin
    if (quad_wr_ptr_en)        quad_wr_ptr_q        <= quad_wr_ptr_d;
    if (depth_wr_ptr_en)       depth_wr_ptr_q       <= depth_wr_ptr_d;
    if (rd_ptr_en)             rd_ptr_q              <= rd_ptr_d;
    if (rd_ptr_en)             rd_ptr_next_q         <= rd_ptr_next_d;
    if (empty_en)              empty_q               <= empty_d;
    if (bary_wr_done_hold_en)  bary_wr_done_hold_q  <= bary_wr_done_hold_d;
    if (depth_wr_done_hold_en) depth_wr_done_hold_q <= depth_wr_done_hold_d;
    if (last_col_en)           last_col_q           <= last_col_d;
    if (last_row_en)           last_row_q           <= last_row_d;
    if (out_info_en)           out_info_q           <= out_info_d;
    if (out_info_valid_en)     out_info_valid_q     <= out_info_valid_d;
    if (bary_rd_valid_en)      bary_rd_valid_q      <= bary_rd_valid_d;
    if (depth_rd_valid_en)     depth_rd_valid_q     <= depth_rd_valid_d;
  end

  for (genvar g = 0; g < BufSz; g++) begin : gen_quad_regs
    always_ff @(posedge clk_i) begin
      if (quad_info_en[g])     quad_info_q[g]     <= quad_info_d[g];
      if (quad_msaa_cov_en[g]) quad_msaa_cov_q[g] <= quad_msaa_cov_d[g];
    end
  end

  for (genvar g = 0; g < 2; g++) begin : gen_rd_data_regs
    always_ff @(posedge clk_i) begin
      if (bary_rd_data_en[g])  bary_rd_data_q[g]  <= bary_rd_data_d;
      if (depth_rd_data_en[g]) depth_rd_data_q[g] <= depth_rd_data_d;
    end
  end

  // ── Combinational logic ────────────────────────────────

  /* verilator lint_off SYNCASYNCNET */  // rst_ni intentionally drives child async-reset FIFOs and the preserved local reset pulse/init muxing in this wrapper.
  logic rst_pulse;
  assign rst_pulse = !rst_ni;  // Active-high pulse for init logic (matching original reset behavior)

  /* verilator lint_off UNUSEDSIGNAL */ // Not all quad_msaa_cov_q bits consumed when coverage disabled

  always_comb begin

    quad_wr_ptr_en    = rst_pulse;
    depth_wr_ptr_en   = rst_pulse;
    rd_ptr_en         = rst_pulse;
    quad_info_en      = {BufSz{rst_pulse}};
    last_col_en       = rst_pulse;
    last_row_en       = rst_pulse;
    quad_msaa_cov_en  = '0;

    quad_info_d       = '0;
    quad_msaa_cov_d   = '0;
    last_col_d        = '0;
    last_row_d        = '0;

    bary_wr_data      = '0;
    bary_wr_en        = 1'b0;
    bary_rd_en        = 1'b0;

    depth_wr_data     = '0;
    depth_wr_en       = 1'b0;
    depth_rd_en       = 1'b0;

    bary_wr_done_hold_en  = rst_pulse;
    depth_wr_done_hold_en = rst_pulse;

    ct_quad_accept_o   = 1'b0;
    ct_result_accept_o = 1'b0;

    quad_wr_ptr_d  = rst_pulse ? '0 : quad_wr_ptr_q;
    depth_wr_ptr_d = rst_pulse ? '0 : depth_wr_ptr_q;
    rd_ptr_d       = rst_pulse ? '0 : rd_ptr_q;
    rd_ptr_next_d  = rst_pulse ? BufIdxSz'(1) : rd_ptr_next_q;

    out_depth_enabled = !early_frag_tests_enable_i || fsh_reads_depth_i;

    if (tr_quad_valid_i && !rbox_halted_i) begin
       quad_info_en[quad_wr_ptr_q] = 1'b1;
      quad_info_d[quad_wr_ptr_q].x          = tr_quad_i.x;
      quad_info_d[quad_wr_ptr_q].y          = tr_quad_i.y;
      quad_info_d[quad_wr_ptr_q].mask       = early_frag_tests_enable_i ? 4'hf : tr_quad_i.mask;
      quad_info_d[quad_wr_ptr_q].sample_idx = tr_quad_i.sample_idx;
      quad_info_d[quad_wr_ptr_q].valid      = 1'b1;
      quad_info_d[quad_wr_ptr_q].start_pack =
           ((tr_quad_i.x >> (hart_tile_width_i  + 2'(1))) != (last_col_q >> (hart_tile_width_i  + 2'(1))))
        || ((tr_quad_i.y >> (hart_tile_height_i + 2'(1))) != (last_row_q >> (hart_tile_height_i + 2'(1))));
      quad_info_d[quad_wr_ptr_q].test_done  = !early_frag_tests_enable_i;
      quad_info_d[quad_wr_ptr_q].hart_id    = quad_to_hart(
        hart_tile_width_i, hart_tile_height_i,
        shire_layout_width_i, shire_layout_height_i,
        tr_quad_i.x, tr_quad_i.y
      );

      quad_msaa_cov_d[quad_wr_ptr_q]  = tr_quad_i.msaa_coverage;
      quad_msaa_cov_en[quad_wr_ptr_q] = fsh_reads_coverage_i && !early_frag_tests_enable_i;

      last_col_d  = tr_quad_i.x;
      last_col_en = 1'b1;
      last_row_d  = tr_quad_i.y;
      last_row_en = 1'b1;

      if (!fsh_disabled_i) begin
        bary_wr_en   = !bary_wr_done_hold_q;
        bary_wr_data = {tr_quad_i.j, tr_quad_i.i};
      end

      if (out_depth_enabled) begin
        depth_wr_en   = !depth_wr_done_hold_q;
        depth_wr_data = tr_quad_i.depth;
      end

      ct_quad_accept_o = ( fsh_disabled_i     ||  bary_wr_done ||  bary_wr_done_hold_q)
                      && (!out_depth_enabled  || depth_wr_done || depth_wr_done_hold_q)
                      && !rbox_halted_i;

      quad_wr_ptr_en = ct_quad_accept_o;
      quad_wr_ptr_d  = quad_wr_ptr_q + BufIdxSz'(1);
    end

    if (ts_result_valid_i && !rbox_halted_i) begin
       quad_info_en[depth_wr_ptr_q]           = 1'b1;
      quad_info_d[depth_wr_ptr_q]             = quad_info_q[depth_wr_ptr_q];
      quad_info_d[depth_wr_ptr_q].test_done   = 1'b1;
      quad_info_d[depth_wr_ptr_q].mask        = quad_info_d[depth_wr_ptr_q].mask & ts_result_mask_i;

      for (int q = 0; q < 4; q++)
        quad_msaa_cov_d[depth_wr_ptr_q][q][quad_info_q[depth_wr_ptr_q].sample_idx] = ts_result_mask_i[q];

      quad_msaa_cov_en[depth_wr_ptr_q] = fsh_reads_coverage_i && early_frag_tests_enable_i;

      depth_wr_ptr_d  = depth_wr_ptr_q + BufIdxSz'(1);
      depth_wr_ptr_en = 1'b1;

      ct_result_accept_o = 1'b1;
    end

    bary_wr_done_hold_d  = rst_pulse ? 1'b0 : (   bary_wr_done_hold_q && !ct_quad_accept_o
                                                || bary_wr_done && !rst_pulse);
    depth_wr_done_hold_d = rst_pulse ? 1'b0 : (   depth_wr_done_hold_q && !ct_quad_accept_o
                                                || depth_wr_done);

    force_empty_quad =    quad_info_q[rd_ptr_next_q].start_pack
                     || (!quad_info_q[rd_ptr_next_q].valid && out_end_drawcall_i);

    out_info_d[0] = quad_info_q[rd_ptr_q];
    out_info_d[1] = quad_info_q[rd_ptr_next_q];
    out_info_d[1].start_pack = force_empty_quad;

    out_info_valid_d = !rst_pulse
                    && (       quad_info_q[rd_ptr_q    ].valid && quad_info_q[rd_ptr_q    ].test_done
                        && (  (quad_info_q[rd_ptr_next_q].valid && quad_info_q[rd_ptr_next_q].test_done)
                            || force_empty_quad));

    out_info_en =  out_info_valid_d && !out_info_valid_q
                || out_info_valid_d &&  out_pop_i;

    out_info_valid_en =  out_info_valid_d && !out_info_valid_q
                      || out_pop_i
                      || rst_pulse;

     bary_rd_done      = 1'b0;
    depth_rd_done      = 1'b0;
     bary_rd_valid_d   = '0;
    depth_rd_valid_d   = '0;
     bary_rd_data_en   = '0;
    depth_rd_data_en   = '0;
     bary_rd_valid_en  = 1'b0;
    depth_rd_valid_en  = 1'b0;

     bary_rd_data_d    =  bary_rd_data;
    depth_rd_data_d    = depth_rd_data;

    out_pck_bary_i_sel = 1'b0;
    out_pck_bary_j_sel = 1'b0;
    out_pck_depth_sel  = 1'b0;
    out_pck_cov_sel    = 1'b0;

    out_packet_valid_o = 1'b0;
    out_packet_o       = '0;

    if (out_info_valid_q) begin
       bary_rd_done =  bary_rd_valid ||  fsh_disabled_i;
      depth_rd_done = depth_rd_valid || !fsh_reads_depth_i;

       bary_rd_valid_d = { bary_rd_valid_q[1] ||  (bary_rd_valid_q[0] &&  bary_rd_done) || out_info_q[1].start_pack,
                           bary_rd_valid_q[0] ||                          bary_rd_done};
      depth_rd_valid_d = {depth_rd_valid_q[1] || (depth_rd_valid_q[0] && depth_rd_done) || out_info_q[1].start_pack,
                          depth_rd_valid_q[0] ||                         depth_rd_done};

       bary_rd_valid_en = 1'b1;
      depth_rd_valid_en = 1'b1;

       bary_rd_en = ( bary_rd_valid_d != 2'b11) && !fsh_disabled_i;
      depth_rd_en = (depth_rd_valid_d != 2'b11) &&  out_depth_enabled;

       bary_rd_data_en[0] =  !bary_rd_valid_q[0] &&  bary_rd_done;
      depth_rd_data_en[0] = !depth_rd_valid_q[0] && depth_rd_done;

       bary_rd_data_en[1] =   bary_rd_valid_q[0] &&  bary_rd_done;
      depth_rd_data_en[1] =  depth_rd_valid_q[0] && depth_rd_done;

      out_pck_bary_i_sel = !fsh_disabled_i && !out_depth_enabled && !fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(2))
                        || !fsh_disabled_i &&  out_depth_enabled && !fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(3))
                        || !fsh_disabled_i && !out_depth_enabled &&  fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(3))
                        || !fsh_disabled_i &&  out_depth_enabled &&  fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(4));
      out_pck_bary_j_sel = !fsh_disabled_i && !out_depth_enabled && !fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(1))
                        || !fsh_disabled_i &&  out_depth_enabled && !fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(2))
                        || !fsh_disabled_i && !out_depth_enabled &&  fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(2))
                        || !fsh_disabled_i &&  out_depth_enabled &&  fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(3));
      out_pck_depth_sel  =                     out_depth_enabled && !fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(1))
                        ||                     out_depth_enabled &&  fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(2));
      out_pck_cov_sel    =                                           fsh_reads_coverage_i && (out_pending_pcks_i == MaxQuadPcksL'(1));

      unique case (1'b1)
        out_pck_bary_i_sel: {out_packet_valid_o, out_packet_o} = {!rbox_halted_i && ( bary_rd_valid_q == 2'b11), bary_rd_data_q[1][127:0],   bary_rd_data_q[0][127:0]};
        out_pck_bary_j_sel: {out_packet_valid_o, out_packet_o} = {!rbox_halted_i && ( bary_rd_valid_q == 2'b11), bary_rd_data_q[1][255:128], bary_rd_data_q[0][255:128]};
        out_pck_depth_sel:  {out_packet_valid_o, out_packet_o} = {!rbox_halted_i && (depth_rd_valid_q == 2'b11), depth_rd_data_q};
        out_pck_cov_sel:    {out_packet_valid_o, out_packet_o} = {!rbox_halted_i,                                256'(quad_msaa_cov_q)};
        default:            {out_packet_valid_o, out_packet_o} = '0;
      endcase
    end

    if (out_pop_i) begin
       bary_rd_valid_d = '0;
      depth_rd_valid_d = '0;
       bary_rd_valid_en = 1'b1;
      depth_rd_valid_en = 1'b1;
    end

    // Move the read pointer when the next pair of quads is loaded.
    if (out_info_en) begin
      rd_ptr_d      = force_empty_quad ? (rd_ptr_d      + BufIdxSz'(1)) : (rd_ptr_d      + BufIdxSz'(2));
      rd_ptr_next_d = force_empty_quad ? (rd_ptr_next_d + BufIdxSz'(1)) : (rd_ptr_next_d + BufIdxSz'(2));
      rd_ptr_en = 1'b1;

      // Clear valid bit of popped entries.
       quad_info_en[rd_ptr_q]           = 1'b1;
      quad_info_d[rd_ptr_q].valid       = 1'b0;
       quad_info_en[rd_ptr_next_q]      = !quad_info_q[rd_ptr_next_q].start_pack;
      quad_info_d[rd_ptr_next_q].valid   = 1'b0;
    end

     bary_rd_valid_en =  bary_rd_valid_en | rst_pulse;
    depth_rd_valid_en = depth_rd_valid_en | rst_pulse;

    out_info_o       = out_info_q;
    out_info_valid_o = out_info_valid_q;

    empty_en  = rst_pulse || quad_wr_ptr_en || rd_ptr_en;
    empty_d   = rst_pulse ? 1'b0 : (quad_wr_ptr_d == rd_ptr_d);
    empty_o   = empty_q;
  end
  /* verilator lint_on SYNCASYNCNET */

  /* verilator lint_on UNUSEDSIGNAL */

  // ── Assertions ─────────────────────────────────────────

  // synthesis translate_off
  `ifdef VERILATOR
    /* verilator lint_off SYNCASYNCNET */
    always_ff @(posedge clk_i) begin
      if (rst_ni && ((1 << $clog2(BufSz)) != BufSz))
        $error("RBOX Output Buffer Size must be a power of 2: %0d", BufSz);
      if (rst_ni && out_pop_i && !(out_pending_pcks_i == MaxQuadPcksL'(1)))
        $error("RBOX Output Buffer expected pending packets is 1 for out_pop, found: %0d", out_pending_pcks_i);
    end
    /* verilator lint_on SYNCASYNCNET */
  `else
    assert property (@(posedge clk_i) disable iff (!rst_ni)
      (1 << $clog2(BufSz)) == BufSz)
    else $error("RBOX Output Buffer Size must be a power of 2: %0d", BufSz);

    assert property (@(posedge clk_i) disable iff (!rst_ni)
      !(out_pop_i && !(out_pending_pcks_i == MaxQuadPcksL'(1))))
    else $error("RBOX Output Buffer expected pending packets is 1 for out_pop, found: %0d", out_pending_pcks_i);
  `endif
  // synthesis translate_on

endmodule
/* verilator lint_on SYNCASYNCNET */
