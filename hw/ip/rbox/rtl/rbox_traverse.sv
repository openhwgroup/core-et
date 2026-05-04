// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX traverse — triangle rasterization.
//
// Walks all 2x2 quads within a tile, evaluating edge equations and
// depth interpolation for each quad. Supports MSAA (2x/4x/8x/16x)
// and scissor testing. Outputs quad data (barycentric, depth, coverage)
// to both the fragment shading path and the depth-test path.
//
// Pipeline stages:
//   S1: MSAA sample index, step computation, input mux
//   S2: Sampler (edge accumulator), coverage/scissor, traverse state
//   S3: Depth offset stage 1, sample-to-FP32 stage 1
//   S4: Depth offset stage 2, sample-to-FP32 stage 2
//   S5: Depth FP adder stage 1
//   S6: Depth FP adder stage 2
//   S7: Depth internal-to-FP32
//   S8: Depth clamp/bound test, depth UNORM conversion
//   S9: Output registers (quad data + quad test)
//
// Differences from original (rbox_traverse.v):
//   - Reset: active-low async-assert/sync-deassert (rst_ni) instead of
//     active-high synchronous (reset)
//   - Naming: lowRISC _i/_o/_d/_q conventions
//   - Types/constants: rbox_pkg:: instead of `include "soc.vh"
//   - Flip-flop macros replaced with explicit always_ff

/* verilator lint_off UNUSEDSIGNAL */  // Packed struct fields (triangle_state_t, tile_state_t, internal_depth_t) partially used
/* verilator lint_off WIDTHTRUNC */    // Bit-select arithmetic on step/sample positions
/* verilator lint_off WIDTHEXPAND */   // Zero-extension of tile coords to render coords
/* verilator lint_off UNOPTFLAT */     // s2_quad_covered: independent fields assigned in same always_comb — acyclic per field
/* verilator lint_off WIDTHCONCAT */   // $bits() in replication — Verilator 5.044 requires explicit sizing

module rbox_traverse
  import rbox_pkg::*;
(
  input  logic                    clk_i,
  input  logic                    rst_ni,

  // Work from control
  input  triangle_state_t         tr_triangle_i,
  input  tile_state_t             tr_tile_i,
  input  logic                    tr_tile_valid_i,
  output logic                    tr_accept_o,
  output logic                    tr_empty_o,

  output quad_data_t              tr_quad_data_o,
  output logic                    tr_quad_data_valid_o,
  output quad_test_t              tr_quad_test_o,
  output logic                    tr_quad_test_valid_o,

  input  logic                    ct_quad_accept_i,

  input  logic                    ts_quad_accept_i,

  // RBOX state from control
  input  logic                    tr_msaa_enable_i,
  input  msaa_mode_e              tr_msaa_mode_i,
  input  logic                    tr_depth_is_fp32_i,
  input  logic                    tr_depth_is_un24_i,
  input  logic                    tr_depth_is_un16_i,
  input  logic [MaxRenderDimL-1:0] tr_scissor_min_x_i,
  input  logic [MaxRenderDimL-1:0] tr_scissor_max_x_i,
  input  logic [MaxRenderDimL-1:0] tr_scissor_min_y_i,
  input  logic [MaxRenderDimL-1:0] tr_scissor_max_y_i,
  input  logic                    tr_depth_clamp_enable_i,
  input  logic [31:0]             tr_depth_min_i,
  input  logic [31:0]             tr_depth_max_i,
  input  logic                    tr_depth_bound_enable_i,
  input  logic [31:0]             tr_depth_bound_min_i,
  input  logic [31:0]             tr_depth_bound_max_i
);

  // ════════════════════════════════════════════════════════════
  // Stage 1: MSAA sample index, step computation, input mux
  // ════════════════════════════════════════════════════════════

  logic                  s1_tr_pipe_en;
  logic                  s1_start_traverse_en;
  logic                  s2_start_traverse_q;
  msaa_sample_idx_t      s1_msaa_sample_idx;
  logic                  s1_msaa_sample_idx_en;
  msaa_sample_idx_t      s2_msaa_sample_idx_q;
  logic                  s1_last_msaa_sample;
  logic                  s2_last_msaa_sample_q;

  logic                  s9_quad_data_valid_q;
  logic                  s9_quad_test_valid_q;

  always_comb begin
    s1_start_traverse_en = tr_tile_valid_i || s2_start_traverse_q;

    s1_tr_pipe_en =
                  // Update pipeline at the start of a tile and when there are no output quads
                     (   !s9_quad_data_valid_q
                      && !s9_quad_test_valid_q
                      && (tr_tile_valid_i || s2_start_traverse_q || !tr_empty_o))
                  // Both Output Buffer and Test pipeline accept the output quad
                  || (    s9_quad_data_valid_q && ct_quad_accept_i
                      &&  s9_quad_test_valid_q && ts_quad_accept_i)
                  // Output Buffer accepts a pending output quad, Test already accepted it
                  || (    s9_quad_data_valid_q && ct_quad_accept_i
                      && !s9_quad_test_valid_q)
                  // Test pipeline accepts a pending output quad, Output Buffer already accepted it
                  || (   !s9_quad_data_valid_q
                      &&  s9_quad_test_valid_q && ts_quad_accept_i);

    s1_msaa_sample_idx = s2_msaa_sample_idx_q + 1;

    case (tr_msaa_mode_i)
      MsaaMode2x  : s1_msaa_sample_idx = s1_msaa_sample_idx & 'd01;
      MsaaMode4x  : s1_msaa_sample_idx = s1_msaa_sample_idx & 'd03;
      MsaaMode8x  : s1_msaa_sample_idx = s1_msaa_sample_idx & 'd07;
      MsaaMode16x : s1_msaa_sample_idx = s1_msaa_sample_idx & 'd16;
    endcase

    s1_last_msaa_sample =  (tr_msaa_mode_i == MsaaMode2x)  &&  s1_msaa_sample_idx[0]
                        || (tr_msaa_mode_i == MsaaMode4x)  && (s1_msaa_sample_idx[1:0] == 2'h3)
                        || (tr_msaa_mode_i == MsaaMode8x)  && (s1_msaa_sample_idx[2:0] == 3'h7)
                        || (tr_msaa_mode_i == MsaaMode16x) && (s1_msaa_sample_idx[3:0] == 4'hf);

    s1_msaa_sample_idx_en = tr_msaa_enable_i && s1_tr_pipe_en;
  end

  step_t [3:0]       s1_row_step_x;
  step_t [3:0]       s1_row_step_y;
  step_t [3:0]       s1_quad_step_x;
  step_t [3:0]       s1_quad_step_y;
  step_t [3:0]       s1_row_base_step_x;
  step_t [3:0]       s1_row_base_step_y;
  step_t [3:0]       s1_quad_base_step_x;
  step_t [3:0]       s1_quad_base_step_y;

  msaa_sample_pos_t  s1_current_sample_pos;
  msaa_sample_pos_t  s1_row_sample_pos;
  msaa_sample_pos_t  s1_quad_sample_pos;

  traverse_state_e   s3_traverse_state_q;

  always_comb begin

    s1_current_sample_pos = {$bits(msaa_sample_pos_t){1'b0}};
    s1_row_sample_pos     = {$bits(msaa_sample_pos_t){1'b0}};
    s1_quad_sample_pos    = {$bits(msaa_sample_pos_t){1'b0}};

    if (!tr_msaa_enable_i)
      s1_current_sample_pos = get_non_msaa_sample_pos();
    else
      s1_current_sample_pos = get_msaa_sample_pos(tr_msaa_mode_i, s2_msaa_sample_idx_q);

    if (tr_msaa_enable_i) begin
      case (tr_msaa_mode_i)
        MsaaMode2x  : s1_row_sample_pos = get_msaa_sample_pos(tr_msaa_mode_i, MsaaSampleIdxSz'(MsaaMode2xRowSample));
        MsaaMode4x  : s1_row_sample_pos = get_msaa_sample_pos(tr_msaa_mode_i, MsaaSampleIdxSz'(MsaaMode4xRowSample));
        MsaaMode8x  : s1_row_sample_pos = get_msaa_sample_pos(tr_msaa_mode_i, MsaaSampleIdxSz'(MsaaMode8xRowSample));
        MsaaMode16x : s1_row_sample_pos = get_msaa_sample_pos(tr_msaa_mode_i, MsaaSampleIdxSz'(MsaaMode16xRowSample));
      endcase
      case (tr_msaa_mode_i)
        MsaaMode2x  : s1_quad_sample_pos = get_msaa_sample_pos(tr_msaa_mode_i, MsaaSampleIdxSz'(MsaaMode2xQuadSample));
        MsaaMode4x  : s1_quad_sample_pos = get_msaa_sample_pos(tr_msaa_mode_i, MsaaSampleIdxSz'(MsaaMode4xQuadSample));
        MsaaMode8x  : s1_quad_sample_pos = get_msaa_sample_pos(tr_msaa_mode_i, MsaaSampleIdxSz'(MsaaMode8xQuadSample));
        MsaaMode16x : s1_quad_sample_pos = get_msaa_sample_pos(tr_msaa_mode_i, MsaaSampleIdxSz'(MsaaMode16xQuadSample));
      endcase
    end

    s1_quad_step_x = '0;
    s1_quad_step_y = '0;
    s1_row_step_x  = '0;
    s1_row_step_y  = '0;
    s1_quad_base_step_x = '0;
    s1_quad_base_step_y = '0;
    s1_row_base_step_x  = '0;
    s1_row_base_step_y  = '0;

    for (integer p = 0; p < 4; p++) begin
             s1_row_step_x[p][MsaaSampPosBits-1:0]  = s1_current_sample_pos[0];
             s1_row_step_y[p][MsaaSampPosBits-1:0]  = s1_current_sample_pos[1];
             s1_quad_step_x[p][MsaaSampPosBits-1:0] = s1_current_sample_pos[0];
             s1_quad_step_y[p][MsaaSampPosBits-1:0] = s1_current_sample_pos[1];

         s1_row_base_step_x[p][MsaaSampPosBits-1:0] = s1_row_sample_pos[0];
         s1_row_base_step_y[p][MsaaSampPosBits-1:0] = s1_row_sample_pos[1];

        s1_quad_base_step_x[p][MsaaSampPosBits-1:0] = s1_quad_sample_pos[0];
        s1_quad_base_step_y[p][MsaaSampPosBits-1:0] = s1_quad_sample_pos[1];
    end

    for (integer p = 0; p < 4; p++) begin
      s1_row_step_x[p] =                                                        s1_row_base_step_x[p] - s1_row_step_x[p];
      s1_row_step_y[p] = {2'b10, {MsaaSampPosBits{1'b0}}} + (s1_row_base_step_y[p] - s1_row_step_y[p]);
    end

    if (tr_tile_valid_i && (s3_traverse_state_q == TraverseEnd)) begin
      s1_quad_step_x[1][StepSz-1 -: StepIntSz] = 'd1;
      s1_quad_step_x[3][StepSz-1 -: StepIntSz] = 'd1;
      s1_quad_step_y[2][StepSz-1 -: StepIntSz] = 'd1;
      s1_quad_step_y[3][StepSz-1 -: StepIntSz] = 'd1;
    end else begin
      for (integer p = 0; p < 4; p++) begin
        s1_quad_step_x[p] = {StepIntSz'(2), {MsaaSampPosBits{1'b0}}} + (s1_quad_base_step_x[p] - s1_quad_step_x[p]);
        s1_quad_step_y[p] =                                              s1_quad_base_step_y[p] - s1_quad_step_y[p];
      end
    end

  end

  traverse_state_e          s2_traverse_state;
  logic                     s2_traverse_state_start;
  logic                     s2_traverse_state_quad;
  logic                     s2_traverse_state_row;
  logic                     s2_traverse_state_end;
  logic                     s2_next_quad;
  logic                     s2_next_row;
  logic                     s2_end_row;
  logic                     s2_sample_row;
  logic                     s1_update_traverse;
  edge_sample_t [3:0][2:0]  s1_input_sample;
  edge_sample_t [3:0][2:0]  s2_input_sample_q;
  edge_sample_t [3:0][2:0]  s2_output_sample;
  edge_sample_t [3:0][2:0]  s3_quad_sample_q;
  edge_sample_t [3:0][2:0]  s3_row_sample_q;
  logic                     s3_fp_accum_bubble_q;

  always_comb begin
    s1_update_traverse = s1_tr_pipe_en && !s3_fp_accum_bubble_q;

    if (tr_tile_valid_i) begin
      for (integer p = 0; p < 4; p++)
        for (integer e = 0; e < 3; e++)
          s1_input_sample[p][e] = tr_tile_i.edge_sample[e];
    end else if (s2_last_msaa_sample_q) begin
      if (s2_end_row)
        s1_input_sample = s3_row_sample_q;
      else
        s1_input_sample = s2_output_sample;
    end else begin
      s1_input_sample = s3_quad_sample_q;
    end
  end

  // ── Samplers (12 instances: 4 pixels x 3 edges) ──────────

  for (genvar pixel = 0; pixel < 4; pixel++) begin : gen_samplers_pixel
    for (genvar edge_eq = 0; edge_eq < 3; edge_eq++) begin : gen_sampler_edge
      rbox_sampler u_sampler (
        .clk_i        (clk_i),
        // Stage 1 inputs
        .coef_x_i     (tr_triangle_i.edge_eq_a[edge_eq]),
        .coef_y_i     (tr_triangle_i.edge_eq_b[edge_eq]),
        .quad_step_x_i(s1_quad_step_x[pixel]),
        .quad_step_y_i(s1_quad_step_y[pixel]),
        .row_step_x_i (s1_row_step_x[pixel]),
        .row_step_y_i (s1_row_step_y[pixel]),
        .valid_i      (s1_tr_pipe_en),
        // Stage 2 input
        .in_sample_i  (s2_input_sample_q[pixel][edge_eq]),
        .sample_row_i (s2_sample_row),
        // Stage 2 output
        .out_sample_o (s2_output_sample[pixel][edge_eq])
      );
    end
  end

  // ── Stage 1→2 flops ──────────────────────────────────────

  step_t [3:0]   s2_row_step_x_q;
  step_t [3:0]   s2_row_step_y_q;
  step_t [3:0]   s2_quad_step_x_q;
  step_t [3:0]   s2_quad_step_y_q;
  depth_coef_t   s2_depth_coef_a_q;
  depth_coef_t   s2_depth_coef_b_q;
  depth_sample_t s2_tile_depth_sample_q;

  always_ff @(posedge clk_i) begin
    if (s1_update_traverse) begin
      s2_row_step_x_q   <= s1_row_step_x;
      s2_row_step_y_q   <= s1_row_step_y;
      s2_quad_step_x_q  <= s1_quad_step_x;
      s2_quad_step_y_q  <= s1_quad_step_y;
      s2_input_sample_q <= s1_input_sample;
    end
  end

  always_ff @(posedge clk_i) begin
    if (tr_tile_valid_i) begin
      s2_depth_coef_a_q      <= tr_triangle_i.depth_eq_a;
      s2_depth_coef_b_q      <= tr_triangle_i.depth_eq_b;
      s2_tile_depth_sample_q <= tr_tile_i.depth_sample;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)                    s2_start_traverse_q <= 1'b0;
    else if (s1_start_traverse_en)  s2_start_traverse_q <= tr_tile_valid_i;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)                    s2_msaa_sample_idx_q <= '0;
    else if (s1_msaa_sample_idx_en) s2_msaa_sample_idx_q <= s1_msaa_sample_idx;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)                    s2_last_msaa_sample_q <= 1'b1;
    else if (s1_msaa_sample_idx_en) s2_last_msaa_sample_q <= s1_last_msaa_sample;
  end

  // ════════════════════════════════════════════════════════════
  // Stage 2: Coverage, scissor, traverse state machine
  // ════════════════════════════════════════════════════════════

  logic [MaxMsaaSamples-1:0][3:0] s2_quad_coverage;
  logic                           s2_quad_covered;
  logic [3:0]                     s2_sample_coverage;
  logic                           s2_quad_in_scissor;
  logic [3:0]                     s2_pixel_in_scissor;
  logic [MaxMsaaSamples-1:0][3:0] s3_quad_coverage_q;
  logic                           s3_quad_covered_q;

  always_comb begin
    s2_quad_coverage = (s2_msaa_sample_idx_q == 0) ? '0 : s3_quad_coverage_q;
    for (integer p = 0; p < 4; p++) begin
      s2_sample_coverage[p] =  sample_inside_triangle(s2_output_sample[p], tr_triangle_i.top_or_left_edge)
                             && s2_pixel_in_scissor[p];
      s2_quad_coverage[s2_msaa_sample_idx_q][p] = s2_sample_coverage[p];
    end

    s2_quad_covered = (s2_msaa_sample_idx_q == 0) ? 1'b1 : s3_quad_covered_q;
    s2_quad_covered = s2_quad_covered && (s2_sample_coverage != 0) && s2_quad_in_scissor;

    s2_sample_row = s2_traverse_state_row;
  end

  logic [MaxRenderDimL-1:0]  s2_current_x;
  logic [MaxRenderDimL-1:0]  s2_current_y;
  logic [MaxRenderDimL-1:0]  s2_current_x_n;
  logic [MaxRenderDimL-1:0]  s2_current_y_n;
  logic [MaxTileDimL-1:0]    s2_quad_x_d;
  logic [MaxTileDimL-1:0]    s2_quad_y_d;
  logic [MaxTileDimL-1:0]    s3_quad_x_q;
  logic [MaxTileDimL-1:0]    s3_quad_y_q;

  logic [MaxTileDimL-1:0]    s2_pending_quads_x;
  logic [MaxTileDimL-1:0]    s2_pending_quads_y;
  logic [MaxTileDimL-1:0]    s3_pending_quads_x_q;
  logic [MaxTileDimL-1:0]    s3_pending_quads_y_q;

  logic                      s3_row_covered_q;
  logic                      s3_row_was_covered_q;

  logic                      s2_update_traverse;
  logic                      s2_fp_accum_bubble;
  logic                      s2_fp_accum_bubble_en;
  logic                      s2_end_tile;
  logic                      s2_next_quad_end_row;
  logic                      s2_next_quad_out_of_scissor;
  logic                      s2_next_row_out_of_scissor;
  logic                      s2_row_covered;
  logic                      s2_row_was_covered;
  logic                      s2_load_row_cov;
  logic                      s3_end_row_q;
  logic                      s3_end_tile_q;

  logic [MaxMsaaSamples-1:0] s2_msaa_sample_idx_mask;

  always_comb begin
    s2_update_traverse =  ((s3_traverse_state_q != TraverseEnd) && s1_tr_pipe_en && !s3_fp_accum_bubble_q)
                       || ((s3_traverse_state_q == TraverseEnd) && s2_start_traverse_q);

    s2_fp_accum_bubble    = rst_ni && s2_update_traverse && !s3_fp_accum_bubble_q;
    s2_fp_accum_bubble_en = !rst_ni || s2_update_traverse ||  s3_fp_accum_bubble_q;

    s2_traverse_state_start =     (s3_traverse_state_q == TraverseEnd)  && s2_start_traverse_q;
    s2_traverse_state_row   =     (s3_traverse_state_q == TraverseQuad) && s3_end_row_q && !s3_end_tile_q;
    s2_traverse_state_end   =     (s3_traverse_state_q == TraverseQuad) && s3_end_tile_q;
    s2_traverse_state_quad  = (   (s3_traverse_state_q == TraverseStart)
                               || (s3_traverse_state_q == TraverseRow)) && s2_last_msaa_sample_q;

    // Priority encoder — one-hot state bits
    priority case (1'b1)
      s2_traverse_state_start : s2_traverse_state = TraverseStart;
      s2_traverse_state_quad  : s2_traverse_state = TraverseQuad;
      s2_traverse_state_row   : s2_traverse_state = TraverseRow;
      s2_traverse_state_end   : s2_traverse_state = TraverseEnd;
      default                 : s2_traverse_state = s3_traverse_state_q;
    endcase
  end

  always_comb begin
    s2_next_quad =   s1_tr_pipe_en
                 && !s3_fp_accum_bubble_q
                 &&  s2_last_msaa_sample_q;

    s2_next_row =      s1_tr_pipe_en
                &&    !s3_fp_accum_bubble_q
                && (   s2_traverse_state_start
                    || s2_traverse_state_row);

    s2_quad_x_d = (s2_traverse_state_start || s2_traverse_state_row) ? get_tile_quad_offset(tr_tile_i.left) : s3_quad_x_q + MaxTileDimL'(2);
    s2_quad_y_d =  s2_traverse_state_start                           ? get_tile_quad_offset(tr_tile_i.top)  : s3_quad_y_q + MaxTileDimL'(2);

    s2_pending_quads_x = (s2_traverse_state_start || s2_traverse_state_row) ? get_quads_in_tile_x(tr_tile_i.size) : s3_pending_quads_x_q - MaxTileDimL'(1);
    s2_pending_quads_y =  s2_traverse_state_start                           ? get_quads_in_tile_y(tr_tile_i.size) : s3_pending_quads_y_q - MaxTileDimL'(1);

    s2_current_x = {tr_tile_i.left[MaxRenderDimL-1:MaxTileDimL], (s2_next_quad ? s2_quad_x_d : s3_quad_x_q)};
    s2_current_y = {tr_tile_i.top[MaxRenderDimL-1:MaxTileDimL],  (s2_next_row  ? s2_quad_y_d : s3_quad_y_q)};

    // Quad coordinates are aligned to 2, so no carry — OR is safe
    s2_current_x_n = s2_current_x | MaxRenderDimL'(1);
    s2_current_y_n = s2_current_y | MaxRenderDimL'(1);

    s2_pixel_in_scissor[0] =  !((s2_current_x   < tr_scissor_min_x_i) || (s2_current_x   > tr_scissor_max_x_i))
                            && !((s2_current_y   < tr_scissor_min_y_i) || (s2_current_y   > tr_scissor_max_y_i));
    s2_pixel_in_scissor[1] =  !((s2_current_x_n < tr_scissor_min_x_i) || (s2_current_x_n > tr_scissor_max_x_i))
                            && !((s2_current_y   < tr_scissor_min_y_i) || (s2_current_y   > tr_scissor_max_y_i));
    s2_pixel_in_scissor[2] =  !((s2_current_x   < tr_scissor_min_x_i) || (s2_current_x   > tr_scissor_max_x_i))
                            && !((s2_current_y_n < tr_scissor_min_y_i) || (s2_current_y_n > tr_scissor_max_y_i));
    s2_pixel_in_scissor[3] =  !((s2_current_x_n < tr_scissor_min_x_i) || (s2_current_x_n > tr_scissor_max_x_i))
                            && !((s2_current_y_n < tr_scissor_min_y_i) || (s2_current_y_n > tr_scissor_max_y_i));

    s2_quad_in_scissor = |s2_pixel_in_scissor;

    s2_next_quad_out_of_scissor = ({tr_tile_i.left[MaxRenderDimL-1:MaxTileDimL], s3_quad_x_q[MaxTileDimL-1:1]} == tr_scissor_max_x_i[MaxRenderDimL-1:1]);
    s2_next_row_out_of_scissor  = ({tr_tile_i.top[MaxRenderDimL-1:MaxTileDimL],  s3_quad_y_q[MaxTileDimL-1:1]} == tr_scissor_max_y_i[MaxRenderDimL-1:1]);

    s2_next_quad_end_row = (s3_pending_quads_x_q == MaxTileDimL'(1)) || s2_next_quad_out_of_scissor || (s3_row_covered_q && !s2_quad_covered);

    s2_end_row =   s2_next_quad
               && (s3_traverse_state_q != TraverseEnd)
               &&  s2_next_quad_end_row;

    s2_end_tile =   s2_next_quad
                && (s3_traverse_state_q != TraverseEnd)
                && (   (s3_pending_quads_y_q == MaxTileDimL'(0))
                    ||  s2_next_row_out_of_scissor
                    || (s3_row_was_covered_q && !s3_row_covered_q))
                &&  s2_next_quad_end_row;

    s2_row_covered = ((s3_traverse_state_q != TraverseEnd) && s3_row_covered_q && !s2_end_row)
                   || (s2_next_quad && s2_quad_covered);

    s2_row_was_covered = s2_end_row && (s3_row_covered_q || (s2_next_quad && s2_quad_covered));

    s2_load_row_cov =   s1_tr_pipe_en
                    && !s3_fp_accum_bubble_q
                    && (s2_traverse_state_start || s2_end_row);

    if (s2_msaa_sample_idx_q == '0)
      s2_msaa_sample_idx_mask = {MaxMsaaSamples{1'b1}};
    else begin
      s2_msaa_sample_idx_mask = '0;
      s2_msaa_sample_idx_mask[s2_msaa_sample_idx_q] = s1_tr_pipe_en;
    end
  end

  step_t [3:0]     s2_step_x;
  step_t [3:0]     s2_step_y;
  internal_depth_t s2_tile_depth_sample_int_fp;

  always_comb begin
    s2_step_x = s2_sample_row ? s2_row_step_x_q : s2_quad_step_x_q;
    s2_step_y = s2_sample_row ? s2_row_step_y_q : s2_quad_step_y_q;

    s2_tile_depth_sample_int_fp = f32_to_depth_internal(s2_tile_depth_sample_q);
  end

  // ── Stage 2→3 flops ──────────────────────────────────────

  logic [3:0]                     s3_sample_coverage_q;
  msaa_sample_idx_t               s3_msaa_sample_idx_q;
  logic [MaxMsaaSamples-1:0]      s3_msaa_sample_idx_mask_q;
  logic                           s3_next_quad_q;
  logic                           s3_next_row_q;
  internal_depth_t                s3_tile_depth_sample_q;
  edge_sample_t [3:0][2:0]        s3_current_sample_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s3_traverse_state_q       <= TraverseEnd;
      s3_msaa_sample_idx_mask_q <= '0;
      s3_end_row_q              <= 1'b0;
      s3_end_tile_q             <= 1'b0;
      s3_quad_covered_q         <= 1'b0;
    end else if (s2_update_traverse) begin
      s3_traverse_state_q       <= s2_traverse_state;
      s3_msaa_sample_idx_mask_q <= s2_msaa_sample_idx_mask;
      s3_end_row_q              <= s2_end_row;
      s3_end_tile_q             <= s2_end_tile;
      s3_quad_covered_q         <= s2_quad_covered;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s2_fp_accum_bubble_en)  s3_fp_accum_bubble_q  <= s2_fp_accum_bubble;
    if (s2_update_traverse) begin
      s3_msaa_sample_idx_q <= s2_msaa_sample_idx_q;
      s3_sample_coverage_q <= s2_sample_coverage;
      s3_next_quad_q       <= s2_next_quad;
      s3_next_row_q        <= s2_next_row;
      s3_row_covered_q     <= s2_row_covered;
    end
    if (s2_load_row_cov)        s3_row_was_covered_q  <= s2_row_was_covered;
    if (s2_next_quad)           s3_quad_x_q           <= s2_quad_x_d;
    if (s2_next_row)            s3_quad_y_q           <= s2_quad_y_d;
    if (s2_next_quad)           s3_pending_quads_x_q  <= s2_pending_quads_x;
    if (s2_next_row)            s3_pending_quads_y_q  <= s2_pending_quads_y;
    if (s2_start_traverse_q)    s3_tile_depth_sample_q <= s2_tile_depth_sample_int_fp;
    if (s2_next_quad)           s3_quad_sample_q      <= s2_output_sample;
    if (s2_next_row)            s3_row_sample_q       <= s2_output_sample;
    if (s1_tr_pipe_en)          s3_current_sample_q   <= s2_output_sample;
  end

  for (genvar sample = 0; sample < MaxMsaaSamples; sample++) begin : gen_s3_quad_coverage
    always_ff @(posedge clk_i) begin
      if (s2_msaa_sample_idx_mask[sample]) s3_quad_coverage_q[sample] <= s2_quad_coverage[sample];
    end
  end

  // ════════════════════════════════════════════════════════════
  // Stages 2–4: Depth offset (spans three stages)
  // ════════════════════════════════════════════════════════════

  logic                       s3_update_traverse;
  internal_depth_t [3:0]      s4_depth_offset;

  for (genvar pixel = 0; pixel < 4; pixel++) begin : gen_depth_offset
    rbox_depth_offset u_depth_offset (
      .clk_i          (clk_i),
      .depth_coef_a_i (s2_depth_coef_a_q),
      .depth_coef_b_i (s2_depth_coef_b_q),
      .step_x_i       (s2_step_x[pixel]),
      .step_y_i       (s2_step_y[pixel]),
      .valid_stg1_i   (s2_update_traverse),
      .valid_stg2_i   (s3_update_traverse),
      .depth_offset_o (s4_depth_offset[pixel])
    );
  end

  // ════════════════════════════════════════════════════════════
  // Stage 3: Sample to FP32, depth offset stage 2
  // ════════════════════════════════════════════════════════════

  logic                           s3_start_traverse;
  logic                           s3_next_quad_upd;
  logic                           s3_next_row_upd;
  logic                           s3_fp_accum_bubble;
  logic                           s3_fp_accum_bubble_en;

  logic [3:0][1:0][31:0]          s4_current_sample_fp32;

  logic                           s4_fp_accum_bubble_q;
  traverse_state_e                s4_traverse_state_q;
  logic                           s4_quad_covered_q;
  logic [3:0]                     s4_sample_coverage_q;
  msaa_sample_idx_t               s4_msaa_sample_idx_q;
  logic [MaxMsaaSamples-1:0]      s4_msaa_sample_idx_mask_q;
  logic                           s4_next_quad_q;
  logic                           s4_next_row_q;
  logic [MaxTileDimL-1:0]         s4_quad_x_q;
  logic [MaxTileDimL-1:0]         s4_quad_y_q;

  always_comb begin
    s3_update_traverse =      (s3_traverse_state_q != TraverseEnd) && s1_tr_pipe_en && !s4_fp_accum_bubble_q
                       || (   (s3_traverse_state_q == TraverseEnd)
                           && (s4_traverse_state_q != TraverseEnd));
    s3_start_traverse  =      (s3_traverse_state_q == TraverseStart);

    s3_next_quad_upd = s3_next_quad_q && s1_tr_pipe_en && !s4_fp_accum_bubble_q;
    s3_next_row_upd  = s3_next_row_q  && s1_tr_pipe_en && !s4_fp_accum_bubble_q;

    s3_fp_accum_bubble    = rst_ni && s3_update_traverse && !s4_fp_accum_bubble_q;
    s3_fp_accum_bubble_en = !rst_ni || s3_update_traverse ||  s4_fp_accum_bubble_q;
  end

  for (genvar pixel = 0; pixel < 4; pixel++) begin : gen_sample_to_fp32
    rbox_sample_to_fp32 #(
      .SampleIntBits (EdgeSampIntBits),
      .SampleFrcBits (EdgeSampFrcBits)
    ) u_barycentric_i (
      .clk_i   (clk_i),
      .in_i    (s3_current_sample_q[pixel][0]),
      .valid_i (s3_update_traverse),
      .out_o   (s4_current_sample_fp32[pixel][0])
    );
    rbox_sample_to_fp32 #(
      .SampleIntBits (EdgeSampIntBits),
      .SampleFrcBits (EdgeSampFrcBits)
    ) u_barycentric_j (
      .clk_i   (clk_i),
      .in_i    (s3_current_sample_q[pixel][1]),
      .valid_i (s3_update_traverse),
      .out_o   (s4_current_sample_fp32[pixel][1])
    );
  end

  // ── Stage 3→4 flops ──────────────────────────────────────

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s4_traverse_state_q       <= TraverseEnd;
      s4_msaa_sample_idx_mask_q <= '0;
    end else if (s3_update_traverse) begin
      s4_traverse_state_q       <= s3_traverse_state_q;
      s4_msaa_sample_idx_mask_q <= s3_msaa_sample_idx_mask_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s3_fp_accum_bubble_en)  s4_fp_accum_bubble_q  <= s3_fp_accum_bubble;
    if (s3_update_traverse) begin
      s4_msaa_sample_idx_q <= s3_msaa_sample_idx_q;
      s4_quad_covered_q    <= s3_quad_covered_q;
      s4_sample_coverage_q <= s3_sample_coverage_q;
      s4_next_quad_q       <= s3_next_quad_q;
      s4_next_row_q        <= s3_next_row_q;
    end
    if (s3_next_quad_upd)  s4_quad_x_q <= s3_quad_x_q;
    if (s3_next_row_upd)   s4_quad_y_q <= s3_quad_y_q;
  end

  // ════════════════════════════════════════════════════════════
  // Stage 4: Depth adder input mux
  // ════════════════════════════════════════════════════════════

  logic                           s4_update_traverse;
  logic                           s4_start_traverse;
  logic                           s4_next_quad_upd;
  logic                           s4_next_row_upd;
  logic                           s4_fp_accum_bubble;
  logic                           s4_fp_accum_bubble_en;

  logic                           s5_fp_accum_bubble_q;
  traverse_state_e                s5_traverse_state_q;
  logic                           s5_quad_covered_q;
  logic [3:0]                     s5_sample_coverage_q;
  msaa_sample_idx_t               s5_msaa_sample_idx_q;
  logic [MaxMsaaSamples-1:0]      s5_msaa_sample_idx_mask_q;
  logic                           s5_next_quad_q;
  logic                           s5_next_row_q;
  logic [MaxTileDimL-1:0]         s5_quad_x_q;
  logic [MaxTileDimL-1:0]         s5_quad_y_q;
  internal_depth_t [3:0]          s5_depth_offset_q;
  internal_depth_t [3:0]          s5_input_sample_depth_q;
  logic [3:0][1:0][31:0]          s5_current_sample_fp32_q;
  internal_depth_t [3:0]          s7_row_sample_depth_q;

  always_comb begin
    s4_update_traverse =      (s4_traverse_state_q != TraverseEnd) && s1_tr_pipe_en && !s5_fp_accum_bubble_q
                       || (   (s4_traverse_state_q == TraverseEnd)
                           && (s5_traverse_state_q != TraverseEnd));
    s4_start_traverse  =      (s4_traverse_state_q == TraverseStart);

    s4_next_quad_upd = s4_next_quad_q && s1_tr_pipe_en && !s5_fp_accum_bubble_q;
    s4_next_row_upd  = s4_next_row_q  && s1_tr_pipe_en && !s5_fp_accum_bubble_q;

    s4_fp_accum_bubble    = rst_ni && s4_update_traverse && !s5_fp_accum_bubble_q;
    s4_fp_accum_bubble_en = !rst_ni || s4_update_traverse ||  s5_fp_accum_bubble_q;
  end

  // ── Stage 4→5 flops ──────────────────────────────────────

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s5_traverse_state_q       <= TraverseEnd;
      s5_msaa_sample_idx_mask_q <= '0;
    end else if (s4_update_traverse) begin
      s5_traverse_state_q       <= s4_traverse_state_q;
      s5_msaa_sample_idx_mask_q <= s4_msaa_sample_idx_mask_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s4_fp_accum_bubble_en)  s5_fp_accum_bubble_q  <= s4_fp_accum_bubble;
    if (s4_update_traverse) begin
      s5_msaa_sample_idx_q    <= s4_msaa_sample_idx_q;
      s5_quad_covered_q       <= s4_quad_covered_q;
      s5_sample_coverage_q    <= s4_sample_coverage_q;
      s5_next_quad_q          <= s4_next_quad_q;
      s5_next_row_q           <= s4_next_row_q;
      s5_depth_offset_q       <= s4_depth_offset;
      s5_current_sample_fp32_q <= s4_current_sample_fp32;
    end
    if (s4_next_quad_upd)  s5_quad_x_q <= s4_quad_x_q;
    if (s4_next_row_upd)   s5_quad_y_q <= s4_quad_y_q;
  end

  // ════════════════════════════════════════════════════════════
  // Stage 5: Depth FP adder (2-stage) — input mux + stage 1
  // ════════════════════════════════════════════════════════════

  logic                       s5_update_traverse;
  internal_depth_t [3:0]      s5_input_sample_depth;
  internal_depth_t [3:0]      s6_output_sample_depth;

  internal_depth_t [3:0]      s7_current_sample_depth_q;

  always_comb begin
    unique case (1'b1)
      s5_traverse_state_q[TraverseIdxStart] : for (integer p = 0; p < 4; p++) s5_input_sample_depth[p] = s3_tile_depth_sample_q;  // Note: reads s3 reg directly (2-stage pipeline latency match)
      s5_traverse_state_q[TraverseIdxQuad]  :                                 s5_input_sample_depth    = s7_current_sample_depth_q;
      s5_traverse_state_q[TraverseIdxRow]   :                                 s5_input_sample_depth    = s7_row_sample_depth_q;
      s5_traverse_state_q[TraverseIdxEnd]   :                                 s5_input_sample_depth    = s7_row_sample_depth_q;
      default:                                                                s5_input_sample_depth    = s7_row_sample_depth_q;  // Pre-reset default
    endcase
  end

  for (genvar pixel = 0; pixel < 4; pixel++) begin : gen_depth_adder
    rbox_fp_adder_2stg u_depth_adder (
      .clk_i   (clk_i),
      .a_i     (s5_input_sample_depth[pixel]),
      .b_i     (s5_depth_offset_q[pixel]),
      .valid_i (s5_update_traverse),
      .out_o   (s6_output_sample_depth[pixel])
    );
  end

  logic                                s5_next_quad_upd;
  logic                                s5_next_row_upd;
  logic                                s5_fp_accum_bubble;
  logic                                s5_fp_accum_bubble_en;

  traverse_state_e                     s6_traverse_state_q;
  logic                                s6_fp_accum_bubble_q;
  logic [3:0][1:0][31:0]               s6_current_sample_fp32_q;
  logic [3:0]                          s6_sample_coverage_q;
  logic                                s6_quad_covered_q;
  msaa_sample_idx_t                    s6_msaa_sample_idx_q;
  logic [MaxMsaaSamples-1:0]           s6_msaa_sample_idx_mask_q;
  logic                                s6_next_quad_q;
  logic                                s6_next_row_q;
  logic [MaxTileDimL-1:0]              s6_quad_x_q;
  logic [MaxTileDimL-1:0]              s6_quad_y_q;

  always_comb begin
    s5_update_traverse =      (s5_traverse_state_q != TraverseEnd) && s1_tr_pipe_en && !s6_fp_accum_bubble_q
                       || (   (s5_traverse_state_q == TraverseEnd)
                           && (s6_traverse_state_q != TraverseEnd));

    s5_next_quad_upd   = s5_next_quad_q && s1_tr_pipe_en && !s6_fp_accum_bubble_q;
    s5_next_row_upd    = s5_next_row_q  && s1_tr_pipe_en && !s6_fp_accum_bubble_q;

    s5_fp_accum_bubble    = rst_ni && s5_update_traverse && !s6_fp_accum_bubble_q;
    s5_fp_accum_bubble_en = !rst_ni || s5_update_traverse ||  s6_fp_accum_bubble_q;
  end

  // ── Stage 5→6 flops ──────────────────────────────────────

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s6_traverse_state_q       <= TraverseEnd;
      s6_msaa_sample_idx_mask_q <= '0;
    end else if (s5_update_traverse) begin
      s6_traverse_state_q       <= s5_traverse_state_q;
      s6_msaa_sample_idx_mask_q <= s5_msaa_sample_idx_mask_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s5_fp_accum_bubble_en)  s6_fp_accum_bubble_q  <= s5_fp_accum_bubble;
    if (s5_update_traverse) begin
      s6_msaa_sample_idx_q    <= s5_msaa_sample_idx_q;
      s6_quad_covered_q       <= s5_quad_covered_q;
      s6_sample_coverage_q    <= s5_sample_coverage_q;
      s6_current_sample_fp32_q <= s5_current_sample_fp32_q;
      s6_next_quad_q          <= s5_next_quad_q;
      s6_next_row_q           <= s5_next_row_q;
    end
    if (s5_next_quad_upd)  s6_quad_x_q <= s5_quad_x_q;
    if (s5_next_row_upd)   s6_quad_y_q <= s5_quad_y_q;
  end

  // ════════════════════════════════════════════════════════════
  // Stage 6: Depth FP adder stage 2
  // ════════════════════════════════════════════════════════════

  logic                                s6_update_traverse;
  logic                                s6_next_quad_upd;
  logic                                s6_next_row_upd;
  logic                                s6_fp_accum_bubble;
  logic                                s6_fp_accum_bubble_en;

  traverse_state_e                     s7_traverse_state_q;
  logic                                s7_fp_accum_bubble_q;
  logic [3:0][1:0][31:0]               s7_current_sample_fp32_q;
  logic [3:0]                          s7_sample_coverage_q;
  logic                                s7_quad_covered_q;
  msaa_sample_idx_t                    s7_msaa_sample_idx_q;
  logic [MaxMsaaSamples-1:0]           s7_msaa_sample_idx_mask_q;
  logic                                s7_next_quad_q;
  logic                                s7_next_row_q;
  logic [MaxTileDimL-1:0]              s7_quad_x_q;
  logic [MaxTileDimL-1:0]              s7_quad_y_q;

  always_comb begin
    s6_update_traverse =      (s6_traverse_state_q != TraverseEnd) && s1_tr_pipe_en && !s7_fp_accum_bubble_q
                       || (   (s6_traverse_state_q == TraverseEnd)
                           && (s7_traverse_state_q != TraverseEnd));

    s6_next_quad_upd   = s6_next_quad_q && s1_tr_pipe_en && !s7_fp_accum_bubble_q;
    s6_next_row_upd    = s6_next_row_q  && s1_tr_pipe_en && !s7_fp_accum_bubble_q;

    s6_fp_accum_bubble    = rst_ni && s6_update_traverse && !s7_fp_accum_bubble_q;
    s6_fp_accum_bubble_en = !rst_ni || s6_update_traverse ||  s7_fp_accum_bubble_q;
  end

  // ── Stage 6→7 flops ──────────────────────────────────────

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s7_traverse_state_q       <= TraverseEnd;
      s7_msaa_sample_idx_mask_q <= '0;
    end else if (s6_update_traverse) begin
      s7_traverse_state_q       <= s6_traverse_state_q;
      s7_msaa_sample_idx_mask_q <= s6_msaa_sample_idx_mask_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s6_fp_accum_bubble_en)  s7_fp_accum_bubble_q  <= s6_fp_accum_bubble;
    if (s6_update_traverse) begin
      s7_msaa_sample_idx_q    <= s6_msaa_sample_idx_q;
      s7_quad_covered_q       <= s6_quad_covered_q;
      s7_sample_coverage_q    <= s6_sample_coverage_q;
      s7_next_quad_q          <= s6_next_quad_q;
      s7_next_row_q           <= s6_next_row_q;
      s7_current_sample_fp32_q <= s6_current_sample_fp32_q;
    end
    if (s6_next_quad_upd)  s7_quad_x_q              <= s6_quad_x_q;
    if (s6_next_row_upd)   s7_quad_y_q              <= s6_quad_y_q;
    if (s6_next_quad_upd)  s7_current_sample_depth_q <= s6_output_sample_depth;
    if (s6_next_row_upd)   s7_row_sample_depth_q     <= s6_output_sample_depth;
  end

  // ════════════════════════════════════════════════════════════
  // Stage 7: Internal depth to FP32
  // ════════════════════════════════════════════════════════════

  logic [3:0][31:0] s7_current_sample_depth_fp32;

  for (genvar pixel = 0; pixel < 4; pixel++) begin : gen_depth_to_fp32
    rbox_depth_to_fp u_depth_to_fp32 (
      .in_i  (s7_current_sample_depth_q[pixel]),
      .out_o (s7_current_sample_depth_fp32[pixel])
    );
  end

  logic                                s7_update_traverse;
  logic                                s7_next_quad_upd;
  logic                                s7_next_row_upd;
  logic                                s7_fp_accum_bubble;
  logic                                s7_fp_accum_bubble_en;

  traverse_state_e                     s8_traverse_state_q;
  logic                                s8_fp_accum_bubble_q;
  logic [3:0][1:0][31:0]               s8_current_sample_fp32_q;
  logic [3:0]                          s8_sample_coverage_q;
  logic                                s8_quad_covered_q;
  msaa_sample_idx_t                    s8_msaa_sample_idx_q;
  logic [MaxMsaaSamples-1:0]           s8_msaa_sample_idx_mask_q;
  logic                                s8_next_quad_q;
  logic                                s8_next_row_q;
  logic [MaxTileDimL-1:0]              s8_quad_x_q;
  logic [MaxTileDimL-1:0]              s8_quad_y_q;
  logic [3:0][31:0]                    s8_current_sample_depth_fp32_q;

  always_comb begin
    s7_update_traverse =      (s7_traverse_state_q != TraverseEnd) && s1_tr_pipe_en && !s8_fp_accum_bubble_q
                       || (   (s7_traverse_state_q == TraverseEnd)
                           && (s8_traverse_state_q != TraverseEnd));

    s7_next_quad_upd   = s7_next_quad_q && s1_tr_pipe_en && !s8_fp_accum_bubble_q;
    s7_next_row_upd    = s7_next_row_q  && s1_tr_pipe_en && !s8_fp_accum_bubble_q;

    s7_fp_accum_bubble    = rst_ni && s7_update_traverse && !s8_fp_accum_bubble_q;
    s7_fp_accum_bubble_en = !rst_ni || s7_update_traverse ||  s8_fp_accum_bubble_q;
  end

  // ── Stage 7→8 flops ──────────────────────────────────────

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s8_traverse_state_q       <= TraverseEnd;
      s8_msaa_sample_idx_mask_q <= '0;
    end else if (s7_update_traverse) begin
      s8_traverse_state_q       <= s7_traverse_state_q;
      s8_msaa_sample_idx_mask_q <= s7_msaa_sample_idx_mask_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s7_fp_accum_bubble_en)  s8_fp_accum_bubble_q  <= s7_fp_accum_bubble;
    if (s7_update_traverse) begin
      s8_msaa_sample_idx_q           <= s7_msaa_sample_idx_q;
      s8_quad_covered_q              <= s7_quad_covered_q;
      s8_sample_coverage_q           <= s7_sample_coverage_q;
      s8_current_sample_fp32_q       <= s7_current_sample_fp32_q;
      s8_current_sample_depth_fp32_q <= s7_current_sample_depth_fp32;
      s8_next_quad_q                 <= s7_next_quad_q;
      s8_next_row_q                  <= s7_next_row_q;
    end
    if (s7_next_quad_upd)  s8_quad_x_q <= s7_quad_x_q;
    if (s7_next_row_upd)   s8_quad_y_q <= s7_quad_y_q;
  end

  // ════════════════════════════════════════════════════════════
  // Stage 8: Depth clamp, depth bound test, UNORM conversion
  // ════════════════════════════════════════════════════════════

  logic [3:0][31:0] s8_current_sample_depth_clamp_fp32;
  logic [3:0]       s8_current_sample_depth_is_inf;
  logic [3:0]       s8_current_sample_depth_is_nan;
  logic [3:0]       s8_clamp_depth_to_min;
  logic [3:0]       s8_clamp_depth_to_max;
  logic [3:0]       s8_depth_bound_test;
  logic [3:0]       s8_sample_coverage;

  always_comb begin
    for (integer p = 0; p < 4; p++) begin
      s8_current_sample_depth_is_inf[p] = (s8_current_sample_depth_fp32_q[p][30:23] == '1) && (s8_current_sample_depth_fp32_q[p][22:20] == 0);
      s8_current_sample_depth_is_nan[p] = (s8_current_sample_depth_fp32_q[p][30:23] == '1) && (s8_current_sample_depth_fp32_q[p][22:20] == 0);
      s8_clamp_depth_to_min[p] =   tr_depth_clamp_enable_i
                               && (    s8_current_sample_depth_is_inf[p]
                                   ||  s8_current_sample_depth_is_nan[p]
                                   ||  s8_current_sample_depth_fp32_q[p][31]
                                   || (s8_current_sample_depth_fp32_q[p] < tr_depth_min_i));
      s8_clamp_depth_to_max[p] =   tr_depth_clamp_enable_i
                               && (s8_current_sample_depth_fp32_q[p] > tr_depth_max_i);

      priority case (1'b1)
        s8_clamp_depth_to_min[p] : s8_current_sample_depth_clamp_fp32[p] = tr_depth_min_i;
        s8_clamp_depth_to_max[p] : s8_current_sample_depth_clamp_fp32[p] = tr_depth_max_i;
        default                  : s8_current_sample_depth_clamp_fp32[p] = s8_current_sample_depth_fp32_q[p];
      endcase

      s8_depth_bound_test[p] =    !tr_depth_bound_enable_i
                              || (   (s8_current_sample_depth_clamp_fp32[p] >= tr_depth_bound_min_i)
                                  && (s8_current_sample_depth_clamp_fp32[p] <= tr_depth_bound_max_i));
    end

    s8_sample_coverage = s8_sample_coverage_q & s8_depth_bound_test;
  end

  logic [3:0][15:0] s8_current_sample_depth_un16;
  logic [3:0][23:0] s8_current_sample_depth_un24;

  for (genvar pixel = 0; pixel < 4; pixel++) begin : gen_depth_unorm
    rbox_depth_to_un #(
      .UnSz (16)
    ) u_depth_round_un16 (
      .in_i  (s8_current_sample_depth_clamp_fp32[pixel]),
      .out_o (s8_current_sample_depth_un16[pixel])
    );

    rbox_depth_to_un #(
      .UnSz (24)
    ) u_depth_round_un24 (
      .in_i  (s8_current_sample_depth_clamp_fp32[pixel]),
      .out_o (s8_current_sample_depth_un24[pixel])
    );
  end

  // ── Stage 8→9 control ────────────────────────────────────

  logic                                s8_update_traverse;
  logic                                s8_next_quad_upd;
  logic                                s8_next_row_upd;
  logic                                s8_fp_accum_bubble;
  logic                                s8_fp_accum_bubble_en;
  logic [MaxMsaaSamples-1:0]           s8_msaa_sample_idx_mask_upd;
  logic                                s8_quad_data_valid;
  logic                                s8_quad_data_valid_en;
  logic                                s8_quad_test_valid;
  logic                                s8_quad_test_valid_en;

  traverse_state_e                     s9_traverse_state_q;
  logic                                s9_fp_accum_bubble_q;
  logic [3:0][1:0][31:0]               s9_current_sample_fp32_q;
  logic [3:0]                          s9_sample_coverage_q;
  logic [MaxMsaaSamples-1:0][3:0]      s9_quad_coverage_q;
  msaa_sample_idx_t                    s9_msaa_sample_idx_q;
  logic [MaxTileDimL-1:0]              s9_quad_x_q;
  logic [MaxTileDimL-1:0]              s9_quad_y_q;
  logic [3:0][31:0]                    s9_current_sample_depth_fp32_q;
  logic [3:0][15:0]                    s9_current_sample_depth_un16_q;
  logic [3:0][23:0]                    s9_current_sample_depth_un24_q;

  always_comb begin
    s8_update_traverse =      (s8_traverse_state_q != TraverseEnd) && s1_tr_pipe_en && !s9_fp_accum_bubble_q
                       || (   (s8_traverse_state_q == TraverseEnd)
                           && (s9_traverse_state_q != TraverseEnd));

    s8_next_quad_upd   = s8_next_quad_q && s1_tr_pipe_en && !s9_fp_accum_bubble_q;
    s8_next_row_upd    = s8_next_row_q  && s1_tr_pipe_en && !s9_fp_accum_bubble_q;

    // NOTE: Original uses concatenation {$bits(...), expr}, NOT replication {$bits(...){expr}}.
    // Match exactly for cycle-accurate behavior.
    s8_msaa_sample_idx_mask_upd = s8_msaa_sample_idx_mask_q & {$bits(s8_msaa_sample_idx_mask_q), (s1_tr_pipe_en && !s9_fp_accum_bubble_q)};

    s8_quad_data_valid =    rst_ni
                       && (   s9_quad_data_valid_q && !ct_quad_accept_i
                           || s8_quad_covered_q    && s1_tr_pipe_en   && !s9_fp_accum_bubble_q);

    s8_quad_data_valid_en = !rst_ni
                          || ((s8_traverse_state_q != TraverseEnd) && s8_quad_covered_q && s1_tr_pipe_en && !s9_fp_accum_bubble_q)
                          || ct_quad_accept_i;

    s8_quad_test_valid =     rst_ni
                       && (   s9_quad_test_valid_q && !ts_quad_accept_i
                           || s8_quad_covered_q    && s1_tr_pipe_en   && !s9_fp_accum_bubble_q);

    s8_quad_test_valid_en = !rst_ni
                         || ((s8_traverse_state_q != TraverseEnd) && s8_quad_covered_q && s1_tr_pipe_en && !s9_fp_accum_bubble_q)
                         || ts_quad_accept_i;

    s8_fp_accum_bubble    = rst_ni && s8_update_traverse && !s9_fp_accum_bubble_q;
    s8_fp_accum_bubble_en = !rst_ni || s8_update_traverse ||  s9_fp_accum_bubble_q;
  end

  // ── Stage 8→9 flops ──────────────────────────────────────

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s9_traverse_state_q <= TraverseEnd;
    end else if (s8_update_traverse) begin
      s9_traverse_state_q <= s8_traverse_state_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (s8_fp_accum_bubble_en)  s9_fp_accum_bubble_q  <= s8_fp_accum_bubble;
    if (s8_update_traverse) begin
      s9_msaa_sample_idx_q           <= s8_msaa_sample_idx_q;
      s9_sample_coverage_q           <= s8_sample_coverage;
      s9_current_sample_fp32_q       <= s8_current_sample_fp32_q;
      s9_current_sample_depth_fp32_q <= s8_current_sample_depth_clamp_fp32;
      s9_current_sample_depth_un16_q <= s8_current_sample_depth_un16;
      s9_current_sample_depth_un24_q <= s8_current_sample_depth_un24;
    end
    if (s8_quad_data_valid_en)  s9_quad_data_valid_q <= s8_quad_data_valid;
    if (s8_quad_test_valid_en)  s9_quad_test_valid_q <= s8_quad_test_valid;
    if (s8_next_quad_upd)       s9_quad_x_q          <= s8_quad_x_q;
    if (s8_next_row_upd)        s9_quad_y_q          <= s8_quad_y_q;
  end

  for (genvar sample = 0; sample < MaxMsaaSamples; sample++) begin : gen_s9_quad_coverage
    always_ff @(posedge clk_i) begin
      if (s8_msaa_sample_idx_mask_upd[sample]) s9_quad_coverage_q[sample] <= s8_sample_coverage;
    end
  end

  // ════════════════════════════════════════════════════════════
  // Stage 9: Output assignments
  // ════════════════════════════════════════════════════════════

  always_comb begin
    tr_quad_data_o.x          = s9_quad_x_q;
    tr_quad_data_o.y          = s9_quad_y_q;
    tr_quad_data_o.mask       = s9_sample_coverage_q;
    tr_quad_data_o.sample_idx = s9_msaa_sample_idx_q;

    for (integer p = 0; p < 4; p++) begin
      tr_quad_data_o.i[p]     = s9_current_sample_fp32_q[p][0];
      tr_quad_data_o.j[p]     = s9_current_sample_fp32_q[p][1];
      tr_quad_data_o.depth[p] = s9_current_sample_depth_fp32_q[p];
      for (integer s = 0; s < MaxMsaaSamples; s++)
        tr_quad_data_o.msaa_coverage[p][s] = s9_quad_coverage_q[s][p];
    end

    tr_quad_data_valid_o = s9_quad_data_valid_q;

    tr_quad_test_o.x          = MaxRenderDimL'(s9_quad_x_q);
    tr_quad_test_o.y          = MaxRenderDimL'(s9_quad_y_q);
    tr_quad_test_o.sample_idx = s9_msaa_sample_idx_q;
    tr_quad_test_o.mask       = s9_sample_coverage_q;

    for (integer p = 0; p < 4; p++) begin
      unique case (1'b1)
        tr_depth_is_fp32_i : tr_quad_test_o.depth[p] =         s9_current_sample_depth_fp32_q[p];
        tr_depth_is_un16_i : tr_quad_test_o.depth[p] = {16'b0, s9_current_sample_depth_un16_q[p]};
        tr_depth_is_un24_i : tr_quad_test_o.depth[p] = { 8'b0, s9_current_sample_depth_un24_q[p]};
        default            : tr_quad_test_o.depth[p] = '0;
      endcase
    end

    tr_quad_test_valid_o = s9_quad_test_valid_q;

    tr_accept_o = (s3_traverse_state_q == TraverseEnd);
    tr_empty_o  = (s3_traverse_state_q == TraverseEnd) && (s9_traverse_state_q == TraverseEnd);
  end

endmodule

/* verilator lint_on UNOPTFLAT */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNUSEDSIGNAL */
