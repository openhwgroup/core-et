// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_fill_fifo (
  input  logic                                                   clk_i,
  input  logic                                                   rst_ni,
  input  logic                                                   push_i,
  input  etlink_pkg::rsp_t                                       push_data_i,
  input  neigh_pkg::coop_tload_tag_table_data_t                  coop_data_i,
  output logic                                                   full_o,
  input  logic [neigh_pkg::NeighNumRespAgents-1:0]               out_rsp_ready_i,
  output logic [neigh_pkg::NeighNumRespAgents-1:0]               out_rsp_valid_o,
  output etlink_pkg::rsp_t [neigh_pkg::NeighNumRespAgents-1:0]   out_rsp_info_o
);

  import etlink_pkg::*;
  import minion_pkg::et_link_neigh_msg_dest_t;
  import minion_dcache_pkg::*;
  import neigh_pkg::*;

  localparam int unsigned FillDataBits = $bits(fill_fifo_data_t);
  localparam int unsigned FillDataAddrW =
      (NeighFillFifoSize > 1) ? $clog2(NeighFillFifoSize) : 1;
  localparam logic [IdSize-1:0] CoreMissIdCoopBase =
      {{(IdSize-$bits(et_link_core_miss_id_e)){1'b0}}, EtLinkCoreMissIdTensorLoadExt0};

  logic [FillDataBits*NeighFillFifoSize-1:0] fill_data_flat;
  fill_fifo_data_t [NeighFillFifoSize-1:0] fill_data;
  logic [FillDataAddrW-1:0] entry_written;
  logic [NeighFillFifoSize-1:0] entry_ready;
  logic [NeighFillFifoSize-1:0] entry_partial_ready;
  logic [NeighFillFifoSize-1:0] entry_bidding;
  logic [FillDataAddrW-1:0] entry_winner;
  logic valid;
  logic stall;
  logic coop_min_mask_up;
  fill_fifo_data_t coop_min_mask_up_value;
  logic [FillDataBits-1:0] entry_data_bits;
  logic [FillDataBits-1:0] coop_min_mask_up_value_bits;

  fill_fifo_data_t entry_data;
  logic push_data_is_msg;
  logic push_data_is_coop;
  logic [MinPerN-1:0][IdSize-1:0] push_data_coop_min_id;
  neigh_source_e push_data_dest;
  /* verilator lint_off UNUSEDSIGNAL */  // The packed message-dest helper carries a thread bit unused by this wrapper.
  et_link_neigh_msg_dest_t push_data_msg_dest;
  /* verilator lint_on UNUSEDSIGNAL */

  logic [NeighFillFifoSize-1:0] used_entries_q, used_entries_d;
  logic [NeighFillFifoSize-1:0] first_entry_available;
  logic entry_available_found;

  logic [MinPerN-1:0] fill_data_coop_min_mask;
  /* verilator lint_off UNUSEDSIGNAL */  // Some stored shell metadata bits are preserved for equivalence but not consumed locally.
  fill_fifo_data_t out_data_q, out_data_d;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [NeighNumRespAgents-1:0] out_valid_q, out_valid_d;
  logic [NeighNumRespAgents-1:0] out_rsp_next_ready;
  logic out_data_is_ack_q, out_data_is_ack_d;

  always_comb begin
    for (int unsigned i = 0; i < NeighFillFifoSize; i++) begin
      fill_data[i] = fill_fifo_data_t'(fill_data_flat[FillDataBits*i +: FillDataBits]);
    end
  end

  assign push_data_is_msg  = push_data_i.opcode == RspMsgRcvData;
  assign push_data_is_coop = !push_data_is_msg && (neigh_source_e'(push_data_i.dest) == SrcCoop);
  assign push_data_msg_dest = et_link_neigh_msg_dest_t'(push_data_i.dest);
  assign push_data_dest = push_data_is_msg ? neigh_source_e'({1'b0, push_data_msg_dest.min_id})
                                           : neigh_source_e'(push_data_i.dest);

  always_comb begin
    for (int unsigned i = 0; i < MinPerN; i++) begin
      push_data_coop_min_id[i] = push_data_is_coop
          ? (CoreMissIdCoopBase
              | {{(IdSize-ShireCoopMissIdSize){1'b0}}, coop_data_i.min_miss_id[i]})
          : push_data_i.id;
    end
  end

  always_comb begin
    entry_data = '0;
    entry_data.dest = push_data_dest;
    entry_data.is_coop = push_data_is_coop;
    entry_data.coop_min_mask = coop_data_i.coop_min_mask;
    entry_data.coop_min_id = push_data_coop_min_id;
    entry_data.rsp = push_data_i;
  end

  assign entry_data_bits = entry_data;
  assign coop_min_mask_up_value_bits = coop_min_mask_up_value;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      used_entries_q <= '0;
      out_valid_q <= '0;
      out_data_q <= '0;
      out_data_is_ack_q <= 1'b0;
    end else begin
      used_entries_q <= used_entries_d;
      out_valid_q <= out_valid_d;
      if (!stall && valid) begin
        out_data_q <= out_data_d;
        out_data_is_ack_q <= out_data_is_ack_d;
      end
    end
  end

  prim_rf_1r2w_par #(
    .Width (FillDataBits),
    .Depth (NeighFillFifoSize)
  ) u_fill_array (
    .clk_i,
    .rd_data_o          (fill_data_flat),
    .wr_data_a_i        (entry_data_bits),
    .wr_data_a_en_1p_i  (1'b1),
    .wr_addr_a_i        (entry_written),
    .wr_en_a_i          (push_i && !full_o),
    .wr_data_b_i        (coop_min_mask_up_value_bits),
    .wr_data_b_en_1p_i  (1'b1),
    .wr_addr_b_i        (entry_winner),
    .wr_en_b_i          (coop_min_mask_up)
  );

  always_comb begin
    used_entries_d = used_entries_q;
    full_o = &used_entries_q;

    first_entry_available = '0;
    first_entry_available[0] = !used_entries_q[0];
    entry_available_found = !used_entries_q[0];
    for (int unsigned i = 1; i < NeighFillFifoSize; i++) begin
      first_entry_available[i] = !used_entries_q[i] && !entry_available_found;
      entry_available_found |= !used_entries_q[i];
    end

    if (push_i && !full_o) begin
      used_entries_d |= first_entry_available;
    end

    if (valid && !entry_partial_ready[entry_winner] && !stall) begin
      used_entries_d[entry_winner] = 1'b0;
    end
  end

  always_comb begin
    entry_written = '0;
    for (int unsigned i = 1; i < NeighFillFifoSize; i++) begin
      if (first_entry_available[i]) begin
        entry_written = i[FillDataAddrW-1:0];
      end
    end
  end

  assign out_rsp_next_ready =
      out_rsp_ready_i & ~(out_valid_q & {NeighNumRespAgents{!out_data_is_ack_q}});

  always_comb begin
    for (int unsigned i = 0; i < NeighFillFifoSize; i++) begin
      fill_data_coop_min_mask = fill_data[i].coop_min_mask;
      entry_ready[i] = used_entries_q[i]
          && (fill_data[i].is_coop
                  ? &(out_rsp_next_ready[NeighAgentMinion +: MinPerN] | ~fill_data_coop_min_mask)
                  : out_rsp_next_ready[fill_data[i].dest]);

      entry_partial_ready[i] = used_entries_q[i]
          && fill_data[i].is_coop
          && (|(out_rsp_next_ready[NeighAgentMinion +: MinPerN] & fill_data_coop_min_mask))
          && ~(&(out_rsp_next_ready[NeighAgentMinion +: MinPerN] | ~fill_data_coop_min_mask));
    end

    valid = |(entry_ready | entry_partial_ready);
    entry_bidding = entry_ready;
    if (!(|entry_bidding)) begin
      entry_bidding = entry_partial_ready;
    end
  end

  prim_queue_arb_lru #(
    .NumClients (NeighFillFifoSize)
  ) u_entry_ready_arb (
    .clk_i,
    .rst_ni,
    .prio_up_i    (push_i && !full_o),
    .prio_entry_i (entry_written),
    .bid_i        (entry_bidding),
    .winner_o     (entry_winner)
  );

  assign out_data_d = fill_data[entry_winner];
  assign out_data_is_ack_d = out_data_d.rsp.opcode == RspAck;

  always_comb begin
    if (stall) begin
      out_valid_d = out_valid_q & ~out_rsp_ready_i;
    end else begin
      out_valid_d = '0;
      for (int unsigned i = 0; i < MinPerN; i++) begin
        out_valid_d[i] = valid
            && (out_data_d.is_coop
                    ? (out_rsp_next_ready[i] && out_data_d.coop_min_mask[i])
                    : (out_data_d.dest == neigh_source_e'(i[SourceSize-1:0])));
      end

      for (int unsigned i = MinPerN; i < NeighNumRespAgents; i++) begin
        out_valid_d[i] = valid && (out_data_d.dest == neigh_source_e'(i[SourceSize-1:0]));
      end
    end
  end

  always_comb begin
    for (int unsigned i = 0; i < MinPerN; i++) begin
      out_rsp_info_o[i] = out_data_q.rsp;
      out_rsp_info_o[i].id = out_data_q.coop_min_id[i];
    end

    for (int unsigned i = MinPerN; i < NeighNumRespAgents; i++) begin
      out_rsp_info_o[i] = out_data_q.rsp;
    end
  end

  assign out_rsp_valid_o = out_valid_q;
  assign stall = |(~out_rsp_ready_i & out_rsp_valid_o);

  always_comb begin
    coop_min_mask_up = entry_partial_ready[entry_winner] && !stall;
    coop_min_mask_up_value = out_data_d;
    coop_min_mask_up_value.coop_min_mask =
        out_data_d.coop_min_mask & ~out_rsp_next_ready[NeighAgentMinion +: MinPerN];
  end

endmodule : neigh_fill_fifo
