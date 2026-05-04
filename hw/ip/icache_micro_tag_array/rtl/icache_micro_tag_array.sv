// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_micro_tag_array
  import icache_geom_pkg::*;
  import icache_pkg::*;
  import minion_pkg::*;
(
  input  logic                               clk_i,
  input  logic                               rst_ni,
  input  logic [IcacheMicroTagSize-1:0]      f2_rd_tag_i,
  output logic [IcacheMicroCacheSize-1:0]    f2_rd_hit_array_o,
  input  logic                               f0_wr_early_valid_i,
  input  logic                               f0_wr_valid_i,
  input  logic [IcacheMicroCacheSize-1:0]    f0_wr_victim_i,
  input  logic [IcacheMicroTagSize-1:0]      f0_wr_tag_i,
  input  logic                               f2_miss_i,
  input  logic [IcacheMicroCacheSize-1:0]    f2_miss_victim_i,
  input  icache_miss_state_e                 f0_miss_state_i,
  input  logic                               f0_flush_data_i,
  input  logic [IcacheDbgUtagAddrWidth-1:0]  apb_paddr_i,
  input  logic                               apb_pwrite_i,
  input  logic                               apb_psel_i,
  input  logic                               apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0]   apb_pwdata_i,
  output logic                               apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0]   apb_prdata_o,
  output logic                               apb_pslverr_o
);

  localparam int unsigned DbgReadPadWidth = BpamShireApbDataWidth - (IcacheMicroTagSize + 1);

  logic [IcacheDbgUtagAddrWidth-1:0]          dbg_addr_q;
  logic                                       dbg_write_en_d;
  logic                                       dbg_write_en_q;
  logic [BpamShireApbDataWidth-1:0]           dbg_write_data_q;
  logic                                       dbg_write_ready;
  logic                                       dbg_read_en_d;
  logic                                       dbg_read_en_q;
  logic [BpamShireApbDataWidth-1:0]           dbg_read_data;
  logic [IcacheMicroCacheSize-1:0][IcacheMicroTagSize-1:0] f2_tags;
  logic [IcacheMicroCacheSize-1:0]            f2_valid_entries_d;
  logic [IcacheMicroCacheSize-1:0]            f2_valid_entries_q;
  logic                                       f2_invalidated_d;
  logic                                       f2_invalidated_q;
  logic [IcacheMicroCacheAdWidth-1:0]         f0_wr_entry;
  logic                                       f0_tag_wr_en;
  logic                                       f0_tag_wr_en_early;
  logic [IcacheMicroCacheAdWidth-1:0]         f0_tag_wr_addr;
  logic [IcacheMicroTagSize-1:0]              f0_tag_wr_data;
  logic [IcacheMicroCacheSize-1:0]            f2_hit_array;
  logic [IcacheMicroCacheSize-1:0]            f2_hit_array_from_fill;

  always_comb begin
    f0_wr_entry = '0;
    for (int i = 0; i < IcacheMicroCacheSize; i++) begin
      if (f0_wr_victim_i[i]) begin
        f0_wr_entry |= i[IcacheMicroCacheAdWidth-1:0];
      end
    end
  end

  assign f0_tag_wr_en       = dbg_write_ready || f0_wr_valid_i;
  assign f0_tag_wr_en_early = (apb_psel_i && apb_pwrite_i) || f0_wr_early_valid_i;
  assign f0_tag_wr_addr     = dbg_write_ready ? dbg_addr_q : f0_wr_entry;
  assign f0_tag_wr_data     = dbg_write_ready ? dbg_write_data_q[IcacheMicroTagSize-1:0] : f0_wr_tag_i;

  /* verilator lint_off PINCONNECTEMPTY */  // micro tag array does not use the optional test-mode port carried by the original seam
  prim_rf_1r1w_par_preview #(
    .Width(IcacheMicroTagSize),
    .Depth(IcacheMicroCacheSize)
  ) u_rf_tag_data_array (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (f0_tag_wr_en_early),
    .rd_data_o            (f2_tags),
    .wr_data_i            (f0_tag_wr_data),
    .wr_addr_i            (f0_tag_wr_addr),
    .wr_en_i              (f0_tag_wr_en)
  );
  /* verilator lint_on PINCONNECTEMPTY */

  always_comb begin
    f2_valid_entries_d = f2_valid_entries_q;
    f2_invalidated_d   = f2_invalidated_q;

    for (int i = 0; i < IcacheMicroCacheSize; i++) begin
      if (f0_wr_valid_i) begin
        if (f0_wr_victim_i[i] && !f2_invalidated_q) begin
          f2_valid_entries_d[i] = 1'b1;
        end
      end else if (dbg_write_ready) begin
        f2_valid_entries_d[dbg_addr_q] = dbg_write_data_q[IcacheMicroTagSize];
      end

      if (f2_miss_i && f2_miss_victim_i[i]) begin
        f2_valid_entries_d[i] = 1'b0;
      end

      if (f0_flush_data_i) begin
        f2_valid_entries_d[i] = 1'b0;
      end
    end

    if (f0_miss_state_i == IcacheMissStateReady) begin
      f2_invalidated_d = 1'b0;
    end
    if (f0_flush_data_i) begin
      f2_invalidated_d = 1'b1;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f2_valid_entries_q <= '0;
    end else begin
      f2_valid_entries_q <= f2_valid_entries_d;
    end
  end

  always_ff @(posedge clk_i) begin
    f2_invalidated_q <= f2_invalidated_d;
  end

  always_comb begin
    for (int i = 0; i < IcacheMicroCacheSize; i++) begin
      f2_hit_array[i] = f2_valid_entries_q[i]
                      && (f2_tags[i] == f2_rd_tag_i)
                      && !(f0_tag_wr_en && (f0_tag_wr_addr == i[IcacheMicroCacheAdWidth-1:0]))
                      && !f0_flush_data_i;

      f2_hit_array_from_fill[i] = f0_tag_wr_en
                                && (f0_tag_wr_data == f2_rd_tag_i)
                                && (f0_tag_wr_addr == i[IcacheMicroCacheAdWidth-1:0]);
    end

    f2_rd_hit_array_o = f2_hit_array | f2_hit_array_from_fill;
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

  assign dbg_write_ready = dbg_write_en_q && !f0_wr_valid_i;
  assign dbg_read_data   = {{DbgReadPadWidth{1'b0}}, f2_valid_entries_q[dbg_addr_q], f2_tags[dbg_addr_q]};
  assign apb_pslverr_o   = 1'b0;

endmodule : icache_micro_tag_array
