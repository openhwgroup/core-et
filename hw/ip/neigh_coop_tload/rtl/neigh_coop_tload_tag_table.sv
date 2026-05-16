// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_coop_tload_tag_table #(
  parameter int unsigned Depth = 8,
  localparam int unsigned AddrWidth = (Depth > 1) ? $clog2(Depth) : 1
) (
  input  logic                                      clk_i,
  input  logic                                      rst_ni,

  input  neigh_pkg::coop_tload_tag_table_req_t [1:0] wr_req_i,
  input  logic                                  [1:0] wr_valid_i,
  output logic                                  [1:0] wr_ready_o,

  input  logic                                        cam_pop_i,
  input  logic [neigh_pkg::ShireCoopLoadIdSize-1:0]  cam_tag_i,
  output neigh_pkg::coop_tload_tag_table_data_t      cam_data_o
);

  logic [1:0] wr_en;
  logic [1:0][AddrWidth-1:0] wr_addr;
  neigh_pkg::coop_tload_tag_table_req_t [Depth-1:0] tag_table_q;
  logic [AddrWidth-1:0] cam_addr;

  logic [Depth-1:0] used_entries_q;
  logic [Depth-1:0] used_entries_d;
  logic             full_q;
  logic             full_d;
  logic [Depth-1:0] first_entry_available;
  logic             first_entry_available_found;
  logic [Depth-1:0] second_entry_available;
  logic             second_entry_available_found;

  always_comb begin
    first_entry_available[0] = ~used_entries_q[0];
    second_entry_available[0] = 1'b0;
    first_entry_available_found = ~used_entries_q[0];
    second_entry_available_found = 1'b0;

    for (int unsigned i = 1; i < Depth; i++) begin
      first_entry_available[i] = ~used_entries_q[i] & ~first_entry_available_found;
      second_entry_available[i] = ~used_entries_q[i] & ~second_entry_available_found &
                                  ~first_entry_available[i];
      first_entry_available_found = first_entry_available_found | ~used_entries_q[i];
      second_entry_available_found = second_entry_available_found |
                                     (~used_entries_q[i] & ~first_entry_available[i]);
    end
  end

  always_comb begin
    used_entries_d = used_entries_q;

    if (|wr_en) begin
      used_entries_d = used_entries_d | first_entry_available;
    end

    if (&wr_en) begin
      used_entries_d = used_entries_d | second_entry_available;
    end

    if (cam_pop_i) begin
      used_entries_d[cam_addr] = 1'b0;
    end
  end

  assign full_d = &used_entries_d;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      used_entries_q <= '0;
      full_q         <= 1'b0;
    end else begin
      used_entries_q <= used_entries_d;
      full_q         <= full_d;
    end
  end

  always_comb begin
    wr_addr = '0;
    for (int unsigned i = 1; i < Depth; i++) begin
      if (first_entry_available[i]) begin
        wr_addr[0] = i[AddrWidth-1:0];
      end
      if (second_entry_available[i]) begin
        wr_addr[1] = i[AddrWidth-1:0];
      end
    end
  end

  logic       wr_arb_stall;
  logic [1:0] wr_arb_grant;
  logic [1:0] wr_arb_grant_d;
  logic       wr_arb_winner_unused;

  prim_arb_lru #(
    .NumClients   (2),
    .NumClientsLog(1)
  ) u_wr_arb (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .bid_i    (wr_valid_i),
    .stall_i  (wr_arb_stall),
    .grant_o  (wr_arb_grant_d),
    .winner_o (wr_arb_winner_unused)
  );

  assign wr_arb_stall = full_q | second_entry_available_found;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      wr_arb_grant <= 2'b01;
    end else if (!wr_arb_stall) begin
      wr_arb_grant <= wr_arb_grant_d;
    end
  end

  assign wr_ready_o = {2{~full_q}} & ({2{second_entry_available_found}} | wr_arb_grant);
  assign wr_en      = wr_valid_i & wr_ready_o;

  // The original tag array has no reset; preserve the unreset storage contract.
  always_ff @(posedge clk_i) begin
    if (|wr_en) begin
      if (wr_en[0]) begin
        tag_table_q[wr_addr[0]] <= wr_req_i[0];
      end else begin
        tag_table_q[wr_addr[0]] <= wr_req_i[1];
      end
    end

    if (&wr_en) begin
      tag_table_q[wr_addr[1]] <= wr_req_i[1];
    end
  end

  logic [Depth-1:0] cam_hit_array;

  always_comb begin
    for (int unsigned i = 0; i < Depth; i++) begin
      cam_hit_array[i] = (cam_tag_i == tag_table_q[i].tag) & used_entries_q[i];
    end
  end

  always_comb begin
    cam_addr = '0;
    for (int unsigned i = 0; i < Depth; i++) begin
      if (cam_hit_array[i]) begin
        cam_addr = cam_addr | i[AddrWidth-1:0];
      end
    end
  end

  assign cam_data_o = tag_table_q[cam_addr].data;

  logic unused_ok;
  assign unused_ok = ^{1'b0, wr_arb_winner_unused};

endmodule : neigh_coop_tload_tag_table
