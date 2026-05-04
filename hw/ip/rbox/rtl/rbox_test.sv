// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX depth/stencil test unit.
//
// Receives quads from the traverse unit, fetches depth lines from the
// depth buffer, performs per-sample depth and stencil testing, and
// returns a coverage mask indicating which samples passed.
//
// Pipeline stages (on clk_rbox_i):
//   S1: Address compute — quad position → depth buffer address + offset
//   S2: Fetch — request depth line from rbox_depth_buffer
//   S3: Test — read depth line, compare each quad sample, write back
//   S4: Result — output test mask to control
//
// Supports D16_UNORM, D32_SFLOAT, D24_UNORM_S8_UINT depth-stencil formats.
// Uses prim_fifo_sram for the depth LFIFO (local FIFO tracking pending quads).

module rbox_test
  import rbox_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  // System
  input  logic                       clk_i,
  input  logic                       clk_rbox_i,
  input  logic                       rst_ni,

  // Configuration
  input  ram_cfg_t                   ram_cfg_i,
  input  dft_t                       dft_i,

  // Request to Shire Cache (from depth buffer)
  output etlink_pkg::req_t           db_sc_req_o,
  output logic [ScBanks:0]           db_sc_req_valid_o,
  input  logic                       db_sc_req_ready_i,

  // Response from Shire Cache (to depth buffer)
  input  etlink_pkg::rsp_t           sc_db_rsp_i,
  input  logic                       sc_db_rsp_valid_i,

  // Input from traverse
  input  quad_test_t                 ts_quad_test_i,
  input  logic                       ts_quad_valid_i,
  output logic                       ts_quad_accept_o,

  // Output to control
  output logic [3:0]                 ts_result_mask_o,
  output logic                       ts_result_valid_o,
  input  logic                       ct_result_accept_i,

  // State from control
  input  state_t                     ts_state_i,
  input  logic                       ts_front_triangle_i,

  // Feedback to control
  output logic                       ts_empty_o,

  // Flush from/to control
  input  logic                       db_flush_i,
  output logic                       db_flush_done_o,

  // Debug RAM interface
  input  logic [DbgPtrIdxSz-1:0]    dbg_ram_ptr_i,

  input  logic                       dbg_db_read_valid_i,
  output logic [DepthLineSz-1:0]     dbg_db_read_data_o,

  input  logic [DepthLineSz-1:0]     dbg_db_write_data_i,
  input  logic                       dbg_db_write_valid_i,

  output logic [DbgDbMetaDataSz-1:0] dbg_db_meta_read_data_o,

  input  logic [DbgDbMetaDataSz-1:0] dbg_db_meta_write_data_i,
  input  logic                       dbg_db_meta_write_valid_i
);

  // ── Depth buffer instance ──────────────────────────────

  logic [PaSize-1:0]               db_fetch_address;
  logic [$clog2(QuadBufSz)-1:0]   db_fetch_quad;
  logic                            db_fetch_valid;
  logic [DepthBufIdxSz-1:0]       db_fetch_ptr;
  logic                            db_fetch_done;
  logic [DepthBufIdxSz-1:0]       db_read_ptr;
  logic                            db_read_valid;
  logic [DepthLineSz-1:0]         db_read_data;
  logic                            db_read_data_valid;
  logic [DepthBufIdxSz-1:0]       db_write_ptr;
  logic [DepthLineSz-1:0]         db_write_data;
  logic                            db_write_valid;
  logic                            db_write_done;
  logic [DepthBufIdxSz-1:0]       db_dealloc_ptr;
  logic [$clog2(QuadBufSz)-1:0]   db_dealloc_quad;
  logic                            db_dealloc_valid;

  rbox_depth_buffer u_depth_buffer (
    .clk_i                    (clk_i),
    .clk_rbox_i               (clk_rbox_i),
    .rst_ni                   (rst_ni),
    .ram_cfg_i                (ram_cfg_i),
    .dft_i                    (dft_i),
    .db_sc_req_o              (db_sc_req_o),
    .db_sc_req_valid_o        (db_sc_req_valid_o),
    .db_sc_req_ready_i        (db_sc_req_ready_i),
    .sc_db_rsp_i              (sc_db_rsp_i),
    .sc_db_rsp_valid_i        (sc_db_rsp_valid_i),
    .db_fetch_address_i       (db_fetch_address),
    .db_fetch_quad_i          (db_fetch_quad),
    .db_fetch_valid_i         (db_fetch_valid),
    .db_fetch_ptr_o           (db_fetch_ptr),
    .db_fetch_done_o          (db_fetch_done),
    .db_read_ptr_i            (db_read_ptr),
    .db_read_valid_i          (db_read_valid),
    .db_read_data_o           (db_read_data),
    .db_read_data_valid_o     (db_read_data_valid),
    .db_write_ptr_i           (db_write_ptr),
    .db_write_data_i          (db_write_data),
    .db_write_valid_i         (db_write_valid),
    .db_write_done_o          (db_write_done),
    .db_dealloc_ptr_i         (db_dealloc_ptr),
    .db_dealloc_quad_i        (db_dealloc_quad),
    .db_dealloc_valid_i       (db_dealloc_valid),
    .db_flush_i               (db_flush_i),
    .db_flush_done_o          (db_flush_done_o),
    .dbg_ram_ptr_i            (dbg_ram_ptr_i),
    .dbg_db_read_valid_i      (dbg_db_read_valid_i),
    .dbg_db_read_data_o       (dbg_db_read_data_o),
    .dbg_db_write_data_i      (dbg_db_write_data_i),
    .dbg_db_write_valid_i     (dbg_db_write_valid_i),
    .dbg_db_meta_read_data_o  (dbg_db_meta_read_data_o),
    .dbg_db_meta_write_data_i (dbg_db_meta_write_data_i),
    .dbg_db_meta_write_valid_i(dbg_db_meta_write_valid_i)
  );

  // ── Depth LFIFO (local FIFO for pending quad info) ────

  localparam int unsigned DlfifoSz    = DepthLfifoSz;    // 128
  localparam int unsigned DlfifoIdxSz = $clog2(DlfifoSz); // 7

  depth_lfifo_info_t [DlfifoSz-1:0]  dlfifo_info_q;
  depth_lfifo_info_t [DlfifoSz-1:0]  dlfifo_info_d;
  logic [DlfifoSz-1:0]               dlfifo_info_en;

  logic [DlfifoIdxSz-1:0]  dlfifo_alloc_ptr_q;
  logic [DlfifoIdxSz-1:0]  dlfifo_alloc_ptr_d;
  logic                     dlfifo_alloc_ptr_en;
  logic [DlfifoIdxSz-1:0]  dlfifo_rd_ptr_q;
  logic [DlfifoIdxSz-1:0]  dlfifo_rd_ptr_d;
  logic                     dlfifo_rd_ptr_en;
  logic [DlfifoIdxSz-1:0]  dlfifo_wr_ptr_q;
  logic [DlfifoIdxSz-1:0]  dlfifo_wr_ptr_d;
  logic                     dlfifo_wr_ptr_en;
  logic [DlfifoIdxSz-1:0]  dlfifo_dealloc_ptr_q;
  logic [DlfifoIdxSz-1:0]  dlfifo_dealloc_ptr_d;
  logic                     dlfifo_dealloc_ptr_en;
  logic                     dlfifo_empty_q;
  logic                     dlfifo_empty_d;
  logic                     dlfifo_empty_en;

  always_ff @(posedge clk_rbox_i) begin
    if (dlfifo_alloc_ptr_en)   dlfifo_alloc_ptr_q   <= dlfifo_alloc_ptr_d;
    if (dlfifo_rd_ptr_en)      dlfifo_rd_ptr_q      <= dlfifo_rd_ptr_d;
    if (dlfifo_wr_ptr_en)      dlfifo_wr_ptr_q      <= dlfifo_wr_ptr_d;
    if (dlfifo_dealloc_ptr_en) dlfifo_dealloc_ptr_q <= dlfifo_dealloc_ptr_d;
    if (dlfifo_empty_en)       dlfifo_empty_q       <= dlfifo_empty_d;
  end

  for (genvar g = 0; g < int'(DlfifoSz); g++) begin : gen_dlfifo_info
    always_ff @(posedge clk_rbox_i) begin
      if (dlfifo_info_en[g]) dlfifo_info_q[g] <= dlfifo_info_d[g];
    end
  end

  // Depth quad data FIFO (stores quad depth values pending test)
  logic [DepthQuadDataSz-1:0]  dlfifo_quad_wr_data;
  logic                         dlfifo_quad_wr_en;
  logic                         dlfifo_quad_wr_done;
  logic [DepthQuadDataSz-1:0]  dlfifo_quad_rd_data;
  logic                         dlfifo_quad_rd_en;
  logic                         dlfifo_quad_rd_valid;

  prim_fifo_sram #(
    .Width (DepthQuadDataSz),
    .Depth (DlfifoSz),
    .Ports (2)
  ) u_dlfifo (
    .clk_i     (clk_rbox_i),
    .rst_ni    (rst_ni),
    .wr_en_i   (dlfifo_quad_wr_en),
    .wr_data_i (dlfifo_quad_wr_data),
    .wr_done_o (dlfifo_quad_wr_done),
    .rd_en_i   (dlfifo_quad_rd_en),
    .rd_valid_o(dlfifo_quad_rd_valid),
    .rd_data_o (dlfifo_quad_rd_data),
    .ram_cfg_i (ram_cfg_i),
    .dft_i     (dft_i)
  );

  // ── DLFIFO pointer management ──────────────────────────

  logic rst_pulse;
  assign rst_pulse = !rst_ni;

  always_comb begin
    if (rst_pulse) begin
      dlfifo_alloc_ptr_d = '0;
      dlfifo_wr_ptr_d    = '0;
      dlfifo_empty_d     = '0;

      dlfifo_alloc_ptr_en = 1'b1;
      dlfifo_wr_ptr_en    = 1'b1;
      dlfifo_empty_en     = 1'b1;
    end else begin
      dlfifo_alloc_ptr_d = dlfifo_alloc_ptr_q;
      dlfifo_wr_ptr_d    = dlfifo_wr_ptr_q;

      dlfifo_alloc_ptr_en = 1'b0;
      dlfifo_wr_ptr_en    = 1'b0;

      if (dlfifo_quad_rd_valid) begin
        dlfifo_wr_ptr_d = dlfifo_wr_ptr_q + DlfifoIdxSz'(1);
        dlfifo_wr_ptr_en = 1'b1;
      end

      if (dlfifo_quad_wr_done) begin
        dlfifo_alloc_ptr_d = dlfifo_alloc_ptr_q + DlfifoIdxSz'(1);
        dlfifo_alloc_ptr_en = 1'b1;
      end

      dlfifo_empty_en = dlfifo_alloc_ptr_en || dlfifo_dealloc_ptr_en;
      dlfifo_empty_d  = (dlfifo_alloc_ptr_d == dlfifo_dealloc_ptr_d);
    end
  end

  // ── Stage 1: Address compute ───────────────────────────

  /* verilator lint_off UNUSEDSIGNAL */ // Packed structs carry fields unused by some format/mode paths

  msaa_mode_e                    s1_msaa_mode;
  logic [MaxMsaaDimL-1:0]       s1_quad_sample_x;
  logic [MaxMsaaDimL-1:0]       s1_quad_sample_y;
  logic [PaSize-1:0]            s1_quad_address;
  logic [5:0]                   s1_quad_offset;
  logic                         s1_pipe_en;
  logic                         s2_fetch_quad_valid_q;

  /* verilator lint_off WIDTHEXPAND */  // MSAA sample addressing uses partial bit selects
  /* verilator lint_off WIDTHTRUNC */   // Address arithmetic truncates to PaSize

  always_comb begin
    s1_msaa_mode = convert_msaa_mode(ts_state_i.msaa_samples);
    unique casez ({ts_state_i.msaa_enable, s1_msaa_mode})
      {1'b0, {$bits(s1_msaa_mode){1'b?}}}: {s1_quad_sample_x, s1_quad_sample_y} = {2'b0,                                       ts_quad_test_i.x,
                                                                                     2'b0,                                       ts_quad_test_i.y};
      {1'b1, MsaaMode2x}:                  {s1_quad_sample_x, s1_quad_sample_y} = {1'b0, ts_quad_test_i.sample_idx[0],          ts_quad_test_i.x,
                                                                                     2'b0,                                       ts_quad_test_i.y};
      {1'b1, MsaaMode4x}:                  {s1_quad_sample_x, s1_quad_sample_y} = {1'b0, ts_quad_test_i.sample_idx[0],          ts_quad_test_i.x,
                                                                                     1'b0, ts_quad_test_i.sample_idx[1],          ts_quad_test_i.y};
      {1'b1, MsaaMode8x}:                  {s1_quad_sample_x, s1_quad_sample_y} = {      ts_quad_test_i.sample_idx[1:0],         ts_quad_test_i.x,
                                                                                     1'b0, ts_quad_test_i.sample_idx[2],          ts_quad_test_i.y};
      {1'b1, MsaaMode16x}:                 {s1_quad_sample_x, s1_quad_sample_y} = {      ts_quad_test_i.sample_idx[1:0],         ts_quad_test_i.x,
                                                                                          ts_quad_test_i.sample_idx[3:2],         ts_quad_test_i.y};
      default:                              {s1_quad_sample_x, s1_quad_sample_y} = '0;
    endcase

    if (ts_state_i.ds_buffer_format == DsFormatD16Unorm) begin
      s1_quad_address = ts_state_i.ds_buffer_addr[PaSize-1:0]
                      + PaSize'({s1_quad_sample_x[7], s1_quad_sample_y[6], s1_quad_sample_x[6],   s1_quad_sample_y[5],
                                 s1_quad_sample_x[5], s1_quad_sample_y[4], s1_quad_sample_x[4],   s1_quad_sample_y[3],
                                 s1_quad_sample_x[3], s1_quad_sample_y[2], 6'b0})
                      + PaSize'((  PaSize'(s1_quad_sample_y[MaxRenderDimL-1:7]) * PaSize'(ts_state_i.ds_buffer_row_pitch)
                                 + PaSize'(s1_quad_sample_x[MaxRenderDimL-1:8])) << 16);
    end else begin
      s1_quad_address = ts_state_i.ds_buffer_addr[PaSize-1:0]
                      + PaSize'({s1_quad_sample_x[6], s1_quad_sample_y[6], s1_quad_sample_x[5],   s1_quad_sample_y[5],
                                 s1_quad_sample_x[4], s1_quad_sample_y[4], s1_quad_sample_x[3],   s1_quad_sample_y[3],
                                 s1_quad_sample_x[2], s1_quad_sample_y[2], 6'b0})
                      + PaSize'((  PaSize'(s1_quad_sample_y[MaxRenderDimL-1:7]) * PaSize'(ts_state_i.ds_buffer_row_pitch)
                                 + PaSize'(s1_quad_sample_x[MaxRenderDimL-1:7])) << 16);
    end

    if (ts_state_i.ds_buffer_format == DsFormatD16Unorm)
      s1_quad_offset = 6'(s1_quad_sample_y[1:0]) * 6'd8 + 6'(s1_quad_sample_x[2:0]);
    else
      s1_quad_offset = 6'(s1_quad_sample_y[1:0]) * 6'd4 + 6'(s1_quad_sample_x[1:0]);

    dlfifo_quad_wr_en   = ts_quad_valid_i && (!s2_fetch_quad_valid_q || (s2_fetch_quad_valid_q && db_fetch_done));
    dlfifo_quad_wr_data = ts_quad_test_i.depth;
    ts_quad_accept_o    = dlfifo_quad_wr_done;
    s1_pipe_en          = dlfifo_quad_wr_done;
  end

  /* verilator lint_on WIDTHTRUNC */
  /* verilator lint_on WIDTHEXPAND */

  // ── Stage 2: Fetch depth line ──────────────────────────

  logic [PaSize-1:0]            s2_quad_address_q;
  logic                         s2_fetch_quad_valid_d;
  logic [DlfifoIdxSz-1:0]      s2_dlfifo_fetch_ptr_q;

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s2_fetch_quad_valid_q <= 1'b0;
    else
      s2_fetch_quad_valid_q <= s2_fetch_quad_valid_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s1_pipe_en) begin
      s2_quad_address_q      <= s1_quad_address;
      s2_dlfifo_fetch_ptr_q  <= dlfifo_alloc_ptr_q;
    end
  end

  always_comb begin
    s2_fetch_quad_valid_d =  s2_fetch_quad_valid_q && !db_fetch_done
                          || dlfifo_quad_wr_done;

    db_fetch_address = s2_quad_address_q;
    db_fetch_quad    = s2_dlfifo_fetch_ptr_q;
    db_fetch_valid   = s2_fetch_quad_valid_q;
  end

  // ── Stage 3: Depth/stencil test ────────────────────────

  logic                         s2_read_quad_valid;

  logic                         s3_test_quad_valid_q;
  logic [DepthBufIdxSz-1:0]    s3_depth_buf_ptr_q;
  logic [DepthLineSz-1:0]      s3_depth_data_q;
  logic [DepthLineSz-1:0]      s3_depth_data_d;
  logic [DepthLineBytes-1:0]   s3_depth_data_en;
  logic                         s3_depth_data_dirty_q;
  logic                         s3_depth_data_dirty_d;
  logic                         s3_depth_data_dirty_en;
  logic                         s3_db_read_data_valid_q;

  always_comb begin
    db_read_valid = 1'b0;
    db_read_ptr   = '0;

    db_write_valid = 1'b0;
    db_write_ptr   = '0;
    db_write_data  = '0;

    s2_read_quad_valid = 1'b0;

    dlfifo_rd_ptr_en  = rst_pulse;
    dlfifo_rd_ptr_d   = rst_pulse ? '0 : dlfifo_rd_ptr_q;

    if (dlfifo_info_q[dlfifo_rd_ptr_q].valid) begin
      if (s3_depth_buf_ptr_q == dlfifo_info_q[dlfifo_rd_ptr_q].depth_buf_ptr) begin
        s2_read_quad_valid = 1'b1;
        dlfifo_rd_ptr_d = dlfifo_rd_ptr_q + DlfifoIdxSz'(1);
        dlfifo_rd_ptr_en = 1'b1;
      end else begin
        if (s3_depth_data_dirty_q && !s3_test_quad_valid_q) begin
          db_write_valid = 1'b1;
          db_write_ptr   = s3_depth_buf_ptr_q;
          db_write_data  = s3_depth_data_q;
        end else if (!s3_depth_data_dirty_q) begin
          db_read_valid = 1'b1;
          db_read_ptr   = dlfifo_info_q[dlfifo_rd_ptr_q].depth_buf_ptr;
        end
      end
    end

    dlfifo_quad_rd_en = s2_read_quad_valid;
  end

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s3_test_quad_valid_q    <= 1'b0;
      s3_db_read_data_valid_q <= 1'b0;
    end else begin
      s3_test_quad_valid_q    <= s2_read_quad_valid;
      s3_db_read_data_valid_q <= db_read_data_valid;
    end
  end

  always_ff @(posedge clk_rbox_i) begin
    if (db_read_data_valid) s3_depth_buf_ptr_q <= db_read_ptr;
    if (s3_depth_data_dirty_en) s3_depth_data_dirty_q <= s3_depth_data_dirty_d;
  end

  for (genvar b = 0; b < int'(DepthLineBytes); b++) begin : gen_depth_data
    always_ff @(posedge clk_rbox_i) begin
      if (s3_depth_data_en[b]) s3_depth_data_q[b*8 +: 8] <= s3_depth_data_d[b*8 +: 8];
    end
  end

  // ── Depth/stencil test functions ───────────────────────

  function automatic logic stencil_test_fn(state_t st, logic front_tri, logic [7:0] in_value);
    stencil_state_t ss;
    if (st.stencil_test_enable) begin
      ss = front_tri ? ts_state_i.stencil_front_state : ts_state_i.stencil_back_state;
      unique case (ss.compare_op)
        CompOpNever:          return 1'b0;
        CompOpLess:           return (ss.ref_value & ss.compare_mask) <  (in_value & ss.compare_mask);
        CompOpEqual:          return (ss.ref_value & ss.compare_mask) == (in_value & ss.compare_mask);
        CompOpLessOrEqual:    return (ss.ref_value & ss.compare_mask) <= (in_value & ss.compare_mask);
        CompOpGreater:        return (ss.ref_value & ss.compare_mask) >  (in_value & ss.compare_mask);
        CompOpNotEqual:       return (ss.ref_value & ss.compare_mask) != (in_value & ss.compare_mask);
        CompOpGreaterOrEqual: return (ss.ref_value & ss.compare_mask) >= (in_value & ss.compare_mask);
        CompOpAlways:         return 1'b1;
        default:              return 1'b1;
      endcase
    end else
      return 1'b1;
  endfunction

  function automatic logic depth_u24_compare_fn(comp_op_e op, logic [23:0] frag, logic [23:0] buf_d);
    unique case (op)
      CompOpNever:          return 1'b0;
      CompOpLess:           return frag <  buf_d;
      CompOpEqual:          return frag == buf_d;
      CompOpLessOrEqual:    return frag <= buf_d;
      CompOpGreater:        return frag >  buf_d;
      CompOpNotEqual:       return frag != buf_d;
      CompOpGreaterOrEqual: return frag >= buf_d;
      CompOpAlways:         return 1'b1;
      default:              return 1'b0;
    endcase
  endfunction

  function automatic logic depth_f16_compare_fn(comp_op_e op, logic [15:0] frag, logic [15:0] buf_d);
    logic nan_frag, nan_buf, cmp;
    nan_frag = (frag[14:10] == 5'h1f) && (|frag[9:0]);
    nan_buf  = (buf_d[14:10] == 5'h1f) && (|buf_d[9:0]);
    unique case (op)
      CompOpNever:          cmp = 1'b0;
      CompOpLess:           cmp = frag <  buf_d;
      CompOpEqual:          cmp = frag == buf_d;
      CompOpLessOrEqual:    cmp = frag <= buf_d;
      CompOpGreater:        cmp = frag >  buf_d;
      CompOpNotEqual:       cmp = frag != buf_d;
      CompOpGreaterOrEqual: cmp = frag >= buf_d;
      CompOpAlways:         cmp = 1'b1;
      default:              cmp = 1'b0;
    endcase
    return cmp && !nan_frag && !nan_buf;
  endfunction

  function automatic logic depth_f32_compare_fn(comp_op_e op, logic [31:0] frag, logic [31:0] buf_d);
    logic nan_frag, nan_buf, cmp;
    nan_frag = (frag[31:24] == 8'hff) && (|frag[23:0]);
    nan_buf  = (buf_d[31:24] == 8'hff) && (|buf_d[23:0]);
    unique case (op)
      CompOpNever:          cmp = 1'b0;
      CompOpLess:           cmp = frag <  buf_d;
      CompOpEqual:          cmp = frag == buf_d;
      CompOpLessOrEqual:    cmp = frag <= buf_d;
      CompOpGreater:        cmp = frag >  buf_d;
      CompOpNotEqual:       cmp = frag != buf_d;
      CompOpGreaterOrEqual: cmp = frag >= buf_d;
      CompOpAlways:         cmp = 1'b1;
      default:              cmp = 1'b0;
    endcase
    return cmp && !nan_frag && !nan_buf;
  endfunction

  function automatic logic depth_test_fn(state_t st, logic [31:0] frag, logic [31:0] buf_d);
    if (st.depth_test_enable) begin
      unique case (st.ds_buffer_format)
        DsFormatD16Unorm:        return depth_f16_compare_fn(st.depth_test_compare_op, frag[15:0], buf_d[15:0]);
        DsFormatD32Sfloat:       return depth_f32_compare_fn(st.depth_test_compare_op, frag, buf_d);
        DsFormatD24UnormS8Uint:  return depth_u24_compare_fn(st.depth_test_compare_op, frag[23:0], buf_d[23:0]);
        default:                 return depth_u24_compare_fn(st.depth_test_compare_op, frag[23:0], buf_d[23:0]);
      endcase
    end else
      return 1'b1;
  endfunction

  function automatic logic [7:0] stencil_update_fn(stencil_state_t ss, stencil_op_e op, logic [7:0] val);
    unique case (op)
      StencilOpKeep:     return val;
      StencilOpZero:     return 8'b0;
      StencilOpReplace:  return ss.ref_value                          & ss.write_mask;
      StencilOpIncClamp: return ((val == 8'hff) ? 8'hff : (val + 8'd1)) & ss.write_mask;
      StencilOpDecClamp: return ((val == 8'h00) ? 8'h00 : (val - 8'd1)) & ss.write_mask;
      StencilOpInvert:   return ~val                                   & ss.write_mask;
      StencilOpIncWrap:  return (val + 8'd1)                          & ss.write_mask;
      StencilOpDecWrap:  return (val - 8'd1)                          & ss.write_mask;
      default:           return val;
    endcase
  endfunction

  function automatic logic [31:0] update_depth_stencil_fn(
    state_t st, logic front_tri,
    logic stencil_pass, logic depth_pass,
    logic [31:0] frag, logic [31:0] buf_d
  );
    stencil_state_t ss;
    logic [31:0] result;
    result = buf_d;
    unique case (st.ds_buffer_format)
      DsFormatD16Unorm: begin
        if (stencil_pass && depth_pass && st.depth_test_write_enable)
          result = {16'h0, frag[15:0]};
      end
      DsFormatD32Sfloat: begin
        if (stencil_pass && depth_pass && st.depth_test_write_enable)
          result = frag;
      end
      DsFormatD24UnormS8Uint: begin
        if (stencil_pass && depth_pass && st.depth_test_write_enable)
          result[23:0] = frag[23:0];
        if (st.stencil_test_enable) begin
          ss = front_tri ? ts_state_i.stencil_front_state : ts_state_i.stencil_back_state;
          if (stencil_pass)
            result[31:24] = depth_pass ? stencil_update_fn(ss, ss.pass_op, buf_d[31:24])
                                       : stencil_update_fn(ss, ss.depth_fail_op, buf_d[31:24]);
          else
            result[31:24] = stencil_update_fn(ss, ss.fail_op, buf_d[31:24]);
        end else
          result[31:24] = buf_d[31:24];
      end
      default: result = 32'h0;
    endcase
    return result;
  endfunction

  // ── Stage 3: Test execution ────────────────────────────

  depth_lfifo_info_t  s3_quad_info_in;
  depth_lfifo_info_t  s3_quad_info_out;
  logic               s3_pipe_en;
  logic [3:0][31:0]   s3_quad_in_depth;
  logic [3:0][31:0]   s3_quad_depth;
  logic [3:0]         s3_stencil_test_pass;
  logic [3:0]         s3_depth_test_pass;
  logic [3:0][31:0]   s3_quad_out_depth;
  logic               s4_test_quad_valid_q;

  /* verilator lint_off WIDTHTRUNC */  // Depth data bit-select arithmetic may truncate

  always_comb begin
    s3_quad_info_in = dlfifo_info_q[dlfifo_wr_ptr_q];
    s3_quad_depth   = dlfifo_quad_rd_data;

    s3_pipe_en = dlfifo_quad_rd_valid && (!s4_test_quad_valid_q || s4_test_quad_valid_q && ct_result_accept_i);

    s3_stencil_test_pass = 4'b1111;
    s3_depth_test_pass   = 4'b1111;
    s3_quad_out_depth    = '0;

    s3_depth_data_en       = '0;
    s3_depth_data_dirty_en = rst_pulse;
    s3_depth_data_dirty_d  = rst_pulse ? 1'b0 : s3_depth_data_dirty_q;

    s3_depth_data_d = '0;

    s3_quad_info_out = s3_quad_info_in;

    if (s3_test_quad_valid_q) begin
      for (int quad = 0; quad < 4; quad++) begin
        // Get input data from depth buffer for the quad.
        unique case (ts_state_i.ds_buffer_format)
          DsFormatD16Unorm: s3_quad_in_depth[quad] = {16'b0, s3_depth_data_q[(s3_quad_info_in.quad_offset * 16 + quad[0] * 16 + quad[1] * 8 * 16) +: 16]};
          default:          s3_quad_in_depth[quad] =          s3_depth_data_q[(s3_quad_info_in.quad_offset * 32 + quad[0] * 32 + quad[1] * 4 * 32) +: 32];
        endcase

        s3_stencil_test_pass[quad] = stencil_test_fn(ts_state_i, ts_front_triangle_i, s3_quad_in_depth[quad][31:24]);
        s3_depth_test_pass[quad]   = depth_test_fn(ts_state_i, s3_quad_depth[quad], s3_quad_in_depth[quad]);
        s3_quad_out_depth[quad]    = update_depth_stencil_fn(ts_state_i, ts_front_triangle_i,
                                                             s3_stencil_test_pass[quad], s3_depth_test_pass[quad],
                                                             s3_quad_depth[quad], s3_quad_in_depth[quad]);

        unique case (ts_state_i.ds_buffer_format)
          DsFormatD16Unorm: s3_depth_data_d[(s3_quad_info_in.quad_offset * 16 + quad[0] * 16 + quad[1] * 8 * 16) +: 16] = s3_quad_out_depth[quad][15:0];
          default:          s3_depth_data_d[(s3_quad_info_in.quad_offset * 32 + quad[0] * 32 + quad[1] * 4 * 32) +: 32] = s3_quad_out_depth[quad];
        endcase

        unique case (ts_state_i.ds_buffer_format)
          DsFormatD16Unorm: s3_depth_data_en[(s3_quad_info_in.quad_offset * 2 + quad[0] * 2 + quad[1] * 8 * 2) +: 2] = 2'b11;
          default:          s3_depth_data_en[(s3_quad_info_in.quad_offset * 4 + quad[0] * 4 + quad[1] * 4 * 4) +: 4] = 4'hf;
        endcase

        s3_quad_info_out.mask[quad] = s3_quad_info_out.mask[quad] && s3_stencil_test_pass[quad] && s3_depth_test_pass[quad];
      end

      s3_depth_data_dirty_en = 1'b1;
      s3_depth_data_dirty_d  = s3_depth_data_dirty_d || (|s3_depth_data_en);
    end

    if (s3_db_read_data_valid_q) begin
      s3_depth_data_en = {DepthLineBytes{1'b1}};
      s3_depth_data_d  = db_read_data;
    end

    if (db_write_done) begin
      s3_depth_data_dirty_en = 1'b1;
      s3_depth_data_dirty_d  = 1'b0;
    end
  end

  /* verilator lint_on WIDTHTRUNC */

  // ── Stage 4: Result output ─────────────────────────────

  logic               s4_test_quad_valid_d;
  logic [3:0][31:0]   s4_quad_out_depth_q;
  depth_lfifo_info_t  s4_quad_info_q;

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s4_test_quad_valid_q <= 1'b0;
    else
      s4_test_quad_valid_q <= s4_test_quad_valid_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s3_pipe_en) begin
      s4_quad_out_depth_q <= s3_quad_out_depth;
      s4_quad_info_q      <= s3_quad_info_out;
    end
  end

  always_comb begin
    s4_test_quad_valid_d =  s4_test_quad_valid_q && !ct_result_accept_i
                         || dlfifo_quad_rd_valid;

    db_dealloc_valid = ct_result_accept_i;
    db_dealloc_ptr   = s4_quad_info_q.depth_buf_ptr;
    db_dealloc_quad  = dlfifo_dealloc_ptr_q;

    ts_result_mask_o  = s4_quad_info_q.mask;
    ts_result_valid_o = s4_test_quad_valid_q;

    ts_empty_o = dlfifo_empty_q;

    dlfifo_dealloc_ptr_en  = rst_pulse || ct_result_accept_i;
    dlfifo_dealloc_ptr_d   = rst_pulse ? '0 : dlfifo_dealloc_ptr_q + DlfifoIdxSz'(1);

    dlfifo_info_d  = rst_pulse ? '0                                  : dlfifo_info_q;
    dlfifo_info_en = rst_pulse ? {DlfifoSz{1'b1}}                   : '0;

    if (dlfifo_quad_wr_done) begin
      dlfifo_info_d[dlfifo_alloc_ptr_q].mask        = ts_quad_test_i.mask;
      dlfifo_info_d[dlfifo_alloc_ptr_q].quad_offset  = s1_quad_offset;
      dlfifo_info_en[dlfifo_alloc_ptr_q]             = 1'b1;
    end

    if (db_fetch_done) begin
      dlfifo_info_d[s2_dlfifo_fetch_ptr_q].valid         = 1'b1;
      dlfifo_info_d[s2_dlfifo_fetch_ptr_q].depth_buf_ptr = db_fetch_ptr;
      dlfifo_info_en[s2_dlfifo_fetch_ptr_q]              = 1'b1;
    end

    if (ct_result_accept_i) begin
      dlfifo_info_d[dlfifo_dealloc_ptr_q].valid  = 1'b0;
      dlfifo_info_en[dlfifo_dealloc_ptr_q]       = 1'b1;
    end
  end

  /* verilator lint_on UNUSEDSIGNAL */

  // Unused
  logic unused;
  assign unused = ^{s4_quad_out_depth_q};

endmodule
