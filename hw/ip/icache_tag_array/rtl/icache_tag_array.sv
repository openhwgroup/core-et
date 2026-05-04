// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_tag_array
  import icache_geom_pkg::*;
  import icache_pkg::*;
  import minion_pkg::*;
(
  input  logic                              clk_i,
  input  logic                              rst_ni,
  input  logic                              f0_read_valid_i,
  /* verilator lint_off UNUSEDSIGNAL */  // tag match uses only set+tag fields; the block-offset bits are intentionally ignored
  input  logic [PaSize-1:0]                 f0_read_paddr_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output logic [IcacheWays-1:0]             f1_read_tag_hit_o,
  input  logic                              f0_write_early_valid_i,
  input  logic                              f0_write_valid_i,
  input  icache_tag_write_req_t             f0_write_req_i,
  input  logic                              f1_miss_i,
  input  logic [IcacheSetAddrWidth-1:0]     f1_miss_idx_i,
  input  logic [IcacheWayAddrWidth-1:0]     f1_miss_way_i,
  input  icache_miss_state_e                f0_miss_state_i,
  input  logic                              f0_flush_data_i,
  input  logic [IcacheDbgTagAddrWidth-1:0]  apb_paddr_i,
  input  logic                              apb_pwrite_i,
  input  logic                              apb_psel_i,
  input  logic                              apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0]  apb_pwdata_i,
  output logic                              apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0]  apb_prdata_o,
  output logic                              apb_pslverr_o
);

  localparam int unsigned DbgWaysWidth = $clog2(IcacheWays / 2);
  localparam int unsigned HalfApbWidth = BpamShireApbDataWidth / 2;
  localparam int unsigned DbgTagHalfPadWidth = HalfApbWidth - (IcachePaTagSize + 1);

  logic [IcacheSets-1:0][IcacheWays-1:0]      f0_tag_valid_d;
  logic [IcacheSets-1:0][IcacheWays-1:0]      f0_tag_valid_q;
  logic                                       f0_invalidated_d;
  logic                                       f0_invalidated_q;
  logic [IcacheSetAddrWidth-1:0]              f0_read_idx;
  logic [IcacheWays-1:0][IcachePaTagSize-1:0] f0_tag_data;
  logic [IcacheDbgTagAddrWidth-1:0]           dbg_addr_q;
  logic [BpamShireApbDataWidth-1:0]           dbg_write_data_q;
  logic                                       dbg_read_en_d;
  logic                                       dbg_read_en_q;
  logic                                       dbg_write_en_d;
  logic                                       dbg_write_en_q;
  logic [IcacheSetAddrWidth-1:0]              dbg_set;
  logic [DbgWaysWidth-1:0]                    dbg_ways;
  logic [IcacheWays-1:0][IcachePaTagSize-1:0] dbg_tag_data;
  logic [BpamShireApbDataWidth-1:0]           dbg_read_data;
  logic                                       dbg_write_ready;
  logic [IcachePaTagSize-1:0]                 f1_tag_q;
  logic [IcacheWays-1:0][IcachePaTagSize-1:0] f1_tag_data_q;
  logic [IcacheWays-1:0]                      f1_tag_valid_q;
  logic [IcachePaTagSize-1:0]                 f1_tag;

  assign f0_read_idx = f0_read_paddr_i[IcacheBlockAddrSize +: IcacheSetAddrWidth];

  always_comb begin
    f0_tag_valid_d   = f0_tag_valid_q;
    f0_invalidated_d = f0_invalidated_q;

    if (f0_write_valid_i) begin
      if (f0_write_req_i.valid && !f0_invalidated_q) begin
        f0_tag_valid_d[f0_write_req_i.idx][f0_write_req_i.way] = 1'b1;
      end else if (!f0_write_req_i.valid) begin
        f0_tag_valid_d[f0_write_req_i.idx][f0_write_req_i.way] = 1'b0;
      end
    end else if (dbg_write_ready) begin
      f0_tag_valid_d[dbg_set][{dbg_ways, 1'b0}] = dbg_write_data_q[IcachePaTagSize];
      f0_tag_valid_d[dbg_set][{dbg_ways, 1'b1}] = dbg_write_data_q[HalfApbWidth + IcachePaTagSize];
    end

    if (f1_miss_i) begin
      f0_tag_valid_d[f1_miss_idx_i][f1_miss_way_i] = 1'b0;
    end

    if (f0_flush_data_i) begin
      f0_tag_valid_d = '0;
    end

    if (f0_miss_state_i == IcacheMissStateReady) begin
      f0_invalidated_d = 1'b0;
    end
    if (f0_flush_data_i) begin
      f0_invalidated_d = 1'b1;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f0_tag_valid_q <= '0;
    end else begin
      f0_tag_valid_q <= f0_tag_valid_d;
    end
  end

  always_ff @(posedge clk_i) begin
    f0_invalidated_q <= f0_invalidated_d;
  end

  for (genvar g = 0; g < IcacheWays; g++) begin : gen_tag_array_ways
    localparam logic [IcacheWayAddrWidth-1:0] WayIdx = g;
    /* verilator lint_off WIDTHTRUNC */  // compile-time pair index intentionally keeps only the low DbgWaysWidth bits
    localparam logic [DbgWaysWidth-1:0]       PairIdx = g / 2;
    /* verilator lint_on WIDTHTRUNC */
    localparam int unsigned                   DataLo = (g % 2) * HalfApbWidth;

    logic [IcacheSetAddrWidth-1:0] f0_tag_wr_addr;
    logic [IcachePaTagSize-1:0]    f0_tag_wr_data;
    logic                          f0_tag_wr_en;
    logic                          f0_tag_wr_en_early;

    always_comb begin
      if (dbg_write_ready) begin
        f0_tag_wr_en = (dbg_ways == PairIdx);
      end else begin
        f0_tag_wr_en = f0_write_valid_i && f0_write_req_i.valid && (f0_write_req_i.way == WayIdx);
      end
    end

    assign f0_tag_wr_en_early = (apb_psel_i && apb_pwrite_i) || f0_write_early_valid_i;
    assign f0_tag_wr_addr     = dbg_write_ready ? dbg_set : f0_write_req_i.idx;
    assign f0_tag_wr_data     = dbg_write_ready ? dbg_write_data_q[DataLo +: IcachePaTagSize]
                                                : f0_write_req_i.data;

    /* verilator lint_off PINCONNECTEMPTY */  // tag array never uses the optional parallel readback port
    prim_rf_2r1w_preview #(
      .Width   (IcachePaTagSize),
      .Entries (IcacheSets),
      .Parallel('0)
    ) u_rf_tag_data_array (
      .preview_clk_i        (clk_i),
      .rf_clk_i             (clk_i),
      .rd_addr_a_i          (f0_read_idx),
      .rd_data_a_o          (f0_tag_data[g]),
      .rd_addr_b_i          (dbg_set),
      .rd_data_b_o          (dbg_tag_data[g]),
      .rd_par_o             (),
      .wr_en_i              (f0_tag_wr_en),
      .wr_data_en_1p_next_i (f0_tag_wr_en_early),
      .wr_addr_i            (f0_tag_wr_addr),
      .wr_data_i            (f0_tag_wr_data)
    );
    /* verilator lint_on PINCONNECTEMPTY */
  end

  always_ff @(posedge clk_i) begin
    if (apb_psel_i && !apb_penable_i) begin
      dbg_addr_q       <= apb_paddr_i;
      dbg_write_data_q <= apb_pwdata_i;
    end
  end

  always_comb begin
    dbg_read_en_d = dbg_read_en_q;
    if (apb_psel_i && !apb_penable_i && !apb_pwrite_i) begin
      dbg_read_en_d = 1'b1;
    end else if (dbg_read_en_q) begin
      dbg_read_en_d = 1'b0;
    end
  end

  always_comb begin
    dbg_write_en_d = dbg_write_en_q;
    if (apb_psel_i && !apb_penable_i && apb_pwrite_i) begin
      dbg_write_en_d = 1'b1;
    end else if (dbg_write_ready) begin
      dbg_write_en_d = 1'b0;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      dbg_read_en_q  <= 1'b0;
      dbg_write_en_q <= 1'b0;
      apb_pready_o   <= 1'b0;
    end else begin
      dbg_read_en_q  <= dbg_read_en_d;
      dbg_write_en_q <= dbg_write_en_d;
      apb_pready_o   <= dbg_read_en_q || dbg_write_ready;
    end
  end

  always_ff @(posedge clk_i) begin
    if (dbg_read_en_q) begin
      apb_prdata_o <= dbg_read_data;
    end
  end

  assign dbg_set         = dbg_addr_q[DbgWaysWidth +: IcacheSetAddrWidth];
  assign dbg_ways        = dbg_addr_q[DbgWaysWidth-1:0];
  assign dbg_write_ready = dbg_write_en_q && !f0_write_valid_i;
  assign apb_pslverr_o   = 1'b0;

  always_comb begin
    dbg_read_data = '0;
    dbg_read_data[HalfApbWidth-1:0] =
        {{DbgTagHalfPadWidth{1'b0}}, f0_tag_valid_q[dbg_set][{dbg_ways, 1'b0}], dbg_tag_data[{dbg_ways, 1'b0}]};
    dbg_read_data[HalfApbWidth +: HalfApbWidth] =
        {{DbgTagHalfPadWidth{1'b0}}, f0_tag_valid_q[dbg_set][{dbg_ways, 1'b1}], dbg_tag_data[{dbg_ways, 1'b1}]};
  end

  always_ff @(posedge clk_i) begin
    if (f0_read_valid_i) begin
      f1_tag_q        <= f0_read_paddr_i[PaSize-1:IcacheDataAddrSize];
      f1_tag_valid_q  <= f0_tag_valid_q[f0_read_idx];
    end
  end

  for (genvar g = 0; g < IcacheWays; g++) begin : gen_out_ff
    always_ff @(posedge clk_i) begin
      if (f0_read_valid_i) begin
        f1_tag_data_q[g] <= f0_tag_data[g];
      end
    end
  end

  always_comb begin
    f1_tag = f1_tag_q;
    for (int i = 0; i < IcacheWays; i++) begin
      f1_read_tag_hit_o[i] = f1_tag_valid_q[i] && (f1_tag_data_q[i] == f1_tag) && !f0_flush_data_i;
    end
  end

endmodule : icache_tag_array
