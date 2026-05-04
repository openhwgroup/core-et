// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_metadata_array — DCache metadata/tag array.

module minion_dcache_metadata_array
  import minion_dcache_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic [DcacheSetIdxWidth-1:0] s1_read_set_i,
  input  logic                         s1_write_valid_i,
  input  logic                         s1_write_valid_prev_i,
  input  dcache_meta_write_req_t       s1_write_req_i,
  output logic [DcacheWays-1:0]        s1_read_resp_valid_o,
  output dcache_meta_data_t            s1_read_resp_o [DcacheWays],
  input  logic                         cfg_clear_all_i,
  input  logic                         cfg_clear_low_i
);

  logic                                 up_valid;
  logic [DcacheSets-1:0][DcacheWays-1:0] s1_valid_d;
  logic [DcacheSets-1:0][DcacheWays-1:0] s1_valid_q;
  logic [DcacheWays-1:0]                 s1_write_en;
  logic [DcacheSetIdxWidth-1:0]          s1_write_set;
  logic [$bits(dcache_meta_data_t)-1:0]  s1_write_data;
  logic [$bits(dcache_meta_data_t)-1:0]  s1_read_resp_raw [DcacheWays];

  always_comb begin
    s1_write_set = s1_write_req_i.set_idx;
    s1_write_data = s1_write_req_i.data;
    for (int way = 0; way < DcacheWays; way++) begin
      s1_write_en[way] = s1_write_valid_i && s1_write_req_i.way_en[way];
      s1_read_resp_o[way] = dcache_meta_data_t'(s1_read_resp_raw[way]);
      s1_read_resp_valid_o[way] = s1_valid_q[s1_read_set_i][way];
    end
  end

  for (genvar way = 0; way < DcacheWays; way++) begin : gen_tag_way
    prim_rf_1r1w_preview #(
      .Width ($bits(dcache_meta_data_t)),
      .Depth (DcacheSets)
    ) u_mem_array (
      .preview_clk_i        (clk_i),
      .rf_clk_i             (clk_i),
      .wr_data_en_1p_next_i (s1_write_valid_prev_i),
      .wr_en_i              (s1_write_en[way]),
      .wr_addr_i            (s1_write_set),
      .wr_data_i            (s1_write_data),
      .rd_addr_i            (s1_read_set_i),
      .rd_data_o            (s1_read_resp_raw[way])
    );
  end

  always_comb begin
    s1_valid_d = s1_valid_q;

    if (cfg_clear_all_i) begin
      s1_valid_d = '0;
    end else if (cfg_clear_low_i) begin
      for (int i = 0; i < DcacheSetsScpSplit; i++) begin
        s1_valid_d[i] = '0;
      end
    end else if (s1_write_valid_i) begin
      for (int w = 0; w < DcacheWays; w++) begin
        if (s1_write_req_i.way_en[w]) begin
          s1_valid_d[s1_write_req_i.set_idx][w] = (s1_write_req_i.data.coh_state != StateInvalid);
        end
      end
    end

    up_valid = cfg_clear_all_i || cfg_clear_low_i || s1_write_valid_i;
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      s1_valid_q <= '0;
    end else if (up_valid) begin
      s1_valid_q <= s1_valid_d;
    end
  end

endmodule
