// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_buffer_array — Outstanding data buffer for dcache requests.

module minion_dcache_buffer_array
  import minion_dcache_pkg::*;
(
  input  logic                            clk_i,
  input  logic                            rst_ni,
  output logic                            full_o,
  output logic                            reduce_full_o,
  input  logic                            alloc_req_pre_i,
  input  logic                            alloc_req_val_i,
  input  logic                            alloc_gsc_i,
  input  logic                            reduce_alloc_req_i,
  input  logic                            s2_write_needed_core_i,
  input  logic                            s2_write_needed_i,
  input  logic                            s2_write_en_i,
  input  logic                            s2_write_replay_i,
  input  logic [DcacheBufferIdxWidth-1:0] s2_write_replay_entry_i,
  input  logic [DcacheDataSize-1:0]       s3_write_data_i,
  output logic [DcacheBufferIdxWidth-1:0] s2_write_entry_o,
  output logic                            s3_write_en_o,
  input  logic                            s3_write_en_ext_i,
  input  logic                            s2_write_en_ext_prev_i,
  input  logic [DcacheBufferIdxWidth-1:0] s3_write_entry_ext_i,
  input  logic [DcacheDataSize-1:0]       s3_write_data_ext_i,
  input  logic                            s2_dealloc_i,
  input  logic [DcacheBufferIdxWidth-1:0] s2_dealloc_entry_i,
  input  logic                            s1_dealloc_uc_i,
  input  logic [DcacheBufferIdxWidth-1:0] s1_dealloc_uc_entry_i,
  input  logic                            reduce_dealloc_req_i,
  input  logic [DcacheBufferIdxWidth-1:0] reduce_dealloc_entry_i,
  input  logic [DcacheBufferIdxWidth-1:0] s1_read_entry_i,
  output logic [DcacheDataSize-1:0]       s1_read_data_o
);

  localparam int unsigned EntryCountWidth = $clog2(DcacheBufferSize) + 1;
  localparam int unsigned ReduceBufferSize = DcacheBufferSize / 2;
  localparam logic [EntryCountWidth-1:0] EntryCountReset = DcacheBufferSize[EntryCountWidth-1:0];
  localparam logic [EntryCountWidth-1:0] ReduceCountReset = ReduceBufferSize[EntryCountWidth-1:0];

  logic s0_alloc_req;
  logic alloc_clk_en;

  /* verilator lint_off UNOPTFLAT */  // Buffer pre-allocation participates in the preserved replay-queue/backpressure loop and is only cyclic after flattening across modules.
  logic s0_alloc_req_pre_d;
  /* verilator lint_on UNOPTFLAT */
  logic s0_alloc_req_pre_q;
  logic s1_alloc_req_d;
  logic s1_alloc_req_q;
  logic s2_alloc_req_d;
  logic s2_alloc_req_q;
  logic s3_alloc_req_d;
  logic s3_alloc_req_q;
  logic s0_alloc_gsc_d;
  logic s0_alloc_gsc_q;
  logic s1_alloc_gsc_d;
  logic s1_alloc_gsc_q;
  logic s2_alloc_gsc_d;
  logic s2_alloc_gsc_q;

  logic [EntryCountWidth-1:0]       entry_count_available_d;
  logic [EntryCountWidth-1:0]       entry_count_available_q;
  logic [DcacheBufferSize-1:0]      entry_mask_available_d;
  logic [DcacheBufferSize-1:0]      entry_mask_available_q;
  logic                             alloc_req_pre_applied_d;
  logic                             alloc_req_pre_applied_q;
  logic [EntryCountWidth-1:0]       reduce_count_available_d;
  logic [EntryCountWidth-1:0]       reduce_count_available_q;

  logic [DcacheDataSize-1:0]        s3_write_data_mux;
  logic [DcacheBufferIdxWidth-1:0]  s3_write_entry_d;
  logic [DcacheBufferIdxWidth-1:0]  s3_write_entry_q;
  logic [DcacheBufferIdxWidth-1:0]  s3_write_entry_mux;
  logic                             s3_write_en_d;
  logic                             s3_write_en_q;

  assign s0_alloc_req = s0_alloc_req_pre_q && alloc_req_val_i;

  always_comb begin
    s0_alloc_req_pre_d = alloc_req_pre_i;
    s1_alloc_req_d = s0_alloc_req;
    s2_alloc_req_d = s1_alloc_req_q;
    s3_alloc_req_d = s2_alloc_req_q && s2_alloc_gsc_q;
    s0_alloc_gsc_d = alloc_gsc_i;
    s1_alloc_gsc_d = s0_alloc_gsc_q;
    s2_alloc_gsc_d = s1_alloc_gsc_q;

    s2_write_entry_o = '0;
    for (int unsigned i = 1; i < DcacheBufferSize; i++) begin
      if (entry_mask_available_q[i]) begin
        s2_write_entry_o = i[DcacheBufferIdxWidth-1:0];
      end
    end
    if (s2_write_replay_i) begin
      s2_write_entry_o = s2_write_replay_entry_i;
    end

    full_o = (entry_count_available_q == '0);

    entry_count_available_d = entry_count_available_q;
    alloc_req_pre_applied_d = 1'b0;
    if (alloc_req_pre_i || reduce_alloc_req_i) begin
      if (entry_count_available_d != '0) begin
        if (!reduce_alloc_req_i) begin
          alloc_req_pre_applied_d = 1'b1;
        end
        entry_count_available_d = entry_count_available_d - 1'b1;
      end
    end

    if (alloc_req_pre_applied_q && !s0_alloc_req) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end

    if (s2_alloc_req_q && !s2_alloc_gsc_q && !s2_write_needed_core_i) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end
    if (s3_alloc_req_q && !s2_write_needed_core_i) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end
    if (s2_alloc_req_q && !s2_alloc_gsc_q && s3_alloc_req_q && s2_write_needed_core_i) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end
    if (s2_dealloc_i) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end
    if (s1_dealloc_uc_i) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end
    if (reduce_dealloc_req_i) begin
      entry_count_available_d = entry_count_available_d + 1'b1;
    end

    entry_mask_available_d = entry_mask_available_q;
    if (s2_write_needed_i) begin
      entry_mask_available_d[s2_write_entry_o] = 1'b0;
    end
    if (s2_dealloc_i) begin
      entry_mask_available_d[s2_dealloc_entry_i] = 1'b1;
    end
    if (s1_dealloc_uc_i) begin
      entry_mask_available_d[s1_dealloc_uc_entry_i] = 1'b1;
    end
    if (reduce_dealloc_req_i) begin
      entry_mask_available_d[reduce_dealloc_entry_i] = 1'b1;
    end

    reduce_count_available_d = reduce_count_available_q;
    if (reduce_alloc_req_i && (reduce_count_available_q != '0)) begin
      reduce_count_available_d = reduce_count_available_d - 1'b1;
    end
    if (reduce_dealloc_req_i) begin
      reduce_count_available_d = reduce_count_available_d + 1'b1;
    end

    reduce_full_o = (reduce_count_available_q == '0) || full_o;

    alloc_clk_en = alloc_req_pre_i || reduce_alloc_req_i || s0_alloc_req_pre_q
                || s1_alloc_req_q || s2_alloc_req_q || s3_alloc_req_q
                || s2_dealloc_i || s1_dealloc_uc_i || s2_write_needed_i
                || reduce_dealloc_req_i;

    s3_write_en_d = s2_write_en_i;
    s3_write_entry_d = s2_write_entry_o;
    if (s3_write_en_q) begin
      s3_write_entry_mux = s3_write_entry_q;
      s3_write_data_mux = s3_write_data_i;
    end else begin
      s3_write_entry_mux = s3_write_entry_ext_i;
      s3_write_data_mux = s3_write_data_ext_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      s0_alloc_req_pre_q <= 1'b0;
      s1_alloc_req_q <= 1'b0;
      s2_alloc_req_q <= 1'b0;
      s3_alloc_req_q <= 1'b0;
      s0_alloc_gsc_q <= 1'b0;
      s1_alloc_gsc_q <= 1'b0;
      s2_alloc_gsc_q <= 1'b0;
      entry_count_available_q <= EntryCountReset;
      entry_mask_available_q <= '1;
      alloc_req_pre_applied_q <= 1'b0;
      reduce_count_available_q <= ReduceCountReset;
      s3_write_en_q <= 1'b0;
      s3_write_entry_q <= '0;
    end else begin
      if (alloc_clk_en) begin
        s0_alloc_req_pre_q <= s0_alloc_req_pre_d;
        s1_alloc_req_q <= s1_alloc_req_d;
        s2_alloc_req_q <= s2_alloc_req_d;
        s3_alloc_req_q <= s3_alloc_req_d;
        s0_alloc_gsc_q <= s0_alloc_gsc_d;
        s1_alloc_gsc_q <= s1_alloc_gsc_d;
        s2_alloc_gsc_q <= s2_alloc_gsc_d;
        entry_count_available_q <= entry_count_available_d;
        entry_mask_available_q <= entry_mask_available_d;
        alloc_req_pre_applied_q <= alloc_req_pre_applied_d;
        reduce_count_available_q <= reduce_count_available_d;
      end

      s3_write_en_q <= s3_write_en_d;
      s3_write_entry_q <= s3_write_entry_d;
    end
  end

  prim_rf_1r1w_preview #(
    .Width (DcacheDataSize),
    .Depth (DcacheBufferSize)
  ) u_rf_buffer_array (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (s2_write_en_ext_prev_i || s2_write_en_i),
    .wr_en_i              (s3_write_en_q || s3_write_en_ext_i),
    .wr_addr_i            (s3_write_entry_mux),
    .wr_data_i            (s3_write_data_mux),
    .rd_addr_i            (s1_read_entry_i),
    .rd_data_o            (s1_read_data_o)
  );

  assign s3_write_en_o = s3_write_en_q;

endmodule
