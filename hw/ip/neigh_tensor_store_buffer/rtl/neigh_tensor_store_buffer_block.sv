// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_tensor_store_buffer_block #(
  parameter int unsigned NumMinions   = 4,
  parameter int unsigned SourceOffset = 0,
  localparam int unsigned NumMinionsL = (NumMinions > 1) ? $clog2(NumMinions) : 1
) (
  input  logic                                      clk_i,
  input  logic                                      rst_ni,

  input  etlink_pkg::neigh_req_t [NumMinions-1:0]  evict_req_i,
  input  logic [NumMinions-1:0]                    evict_valid_i,
  output logic [NumMinions-1:0]                    evict_ready_o,

  input  logic                                      evict_out_ready_i,
  input  logic                                      evict_out_disable_next_i,
  output logic                                      evict_out_valid_o,
  output logic                                      evict_out_valid_1st_o,
  output logic                                      evict_out_valid_2nd_o,
  output etlink_pkg::neigh_req_t                    evict_out_req_o,

  input  logic [NumMinions-1:0]                    tstore_min_ack_i,

  input  logic [NumMinions-1:0]                    tstore_min_resp_ready_i,
  output logic [NumMinions-1:0]                    tstore_min_resp_valid_o,
  output minion_pkg::et_link_minion_rsp_info_t [NumMinions-1:0] tstore_min_resp_o,

  output neigh_pkg::tstore_block_dbg_sm_t          dbg_sm_signals_o
);

  localparam logic CoopMask2Way = 1'b0;
  localparam logic CoopMask4Way = 1'b1;
  localparam logic [etlink_pkg::SourceSize-1:0] SourceOffsetValue =
      SourceOffset[etlink_pkg::SourceSize-1:0];

  logic [NumMinions-1:0] min_ready_no_coop;
  logic [NumMinions-1:0] min_ready_coop;
  logic [NumMinions-1:0] min_ready_coop_q;
  logic [NumMinions/2-1:0] min_ready_coop_qual;
  logic [NumMinions-1:0] min_can_accept_new_wa;
  logic [NumMinions-1:0] evict_coop_mask;
  logic [NumMinions/2-1:0] evict_out_entry_winner_q;
  logic [NumMinions-1:0] evict_out_entry_winner_ext;
  logic [NumMinions-1:0] evict_out_winner_select;

  always_comb begin : ready_generation
    int unsigned min_id;

    min_id = 0;

    for (int unsigned i = 0; i < NumMinions; i++) begin
      if (evict_req_i[i].subopcode[0]) begin
        evict_coop_mask[i] = CoopMask2Way;
      end else begin
        evict_coop_mask[i] = CoopMask4Way;
      end
      min_ready_no_coop[i] = evict_valid_i[i] && min_can_accept_new_wa[i];
    end

    for (int unsigned i = 0; i < NumMinions; i++) begin
      min_ready_coop[i] = min_ready_no_coop[i]
          && ~(evict_out_valid_o && evict_out_entry_winner_ext[i]);
      if (evict_coop_mask[i] == CoopMask2Way) begin
        min_id = i / 2;
        if ((min_id * 2) != i) begin
          min_ready_coop[i] &= !(evict_coop_mask[i] == evict_coop_mask[min_id * 2]);
        end
        for (int unsigned j = 0; j < 2; j++) begin
          if ((min_id * 2 + j) != i) begin
            min_ready_coop[i] &= min_ready_no_coop[min_id * 2 + j]
                && (evict_coop_mask[i] == evict_coop_mask[min_id * 2 + j]);
          end
        end
      end else begin
        min_id = i / 4;
        if ((min_id * 4) != i) begin
          min_ready_coop[i] &= !(evict_coop_mask[i] == evict_coop_mask[min_id * 4]);
        end
        for (int unsigned j = 0; j < 4; j++) begin
          if ((min_id * 4 + j) != i) begin
            min_ready_coop[i] &= min_ready_no_coop[min_id * 4 + j]
                && (evict_coop_mask[i] == evict_coop_mask[min_id * 4 + j]);
          end
        end
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      min_ready_coop_q <= '0;
    end else begin
      min_ready_coop_q <= min_ready_coop;
    end
  end

  logic [NumMinions-1:0] analyze_done_q;
  logic [NumMinions-1:0] analyze_done_d;

  logic evict_out_available_q;
  logic evict_out_available_d;
  logic evict_out_valid_d;
  logic evict_out_1st_pass_q;
  logic evict_out_1st_pass_d;
  logic evict_out_2nd_pass_q;
  logic evict_out_2nd_pass_d;
  logic evict_pop;
  logic evict_granted_line;
  logic evict_out_up;

  logic [NumMinions-1:0] evict_coop_mask_q;
  logic [NumMinions-1:0] evict_req_hline_q;
  logic [NumMinions-1:0] evict_req_qword_q;
  logic [NumMinions-1:0][etlink_pkg::QwenSize-1:0] evict_req_qwen_q;

  etlink_pkg::neigh_req_t evict_out_req_d;
  etlink_pkg::neigh_req_t evict_out_req_q;

  always_comb begin
    for (int unsigned i = 0; i < NumMinions; i++) begin
      analyze_done_d[i] = !analyze_done_q[i] || (analyze_done_q[i] && !evict_ready_o[i]);
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      analyze_done_q <= '0;
    end else begin
      analyze_done_q <= analyze_done_d;
    end
  end

  always_comb begin
    evict_out_available_d = evict_out_available_q;
    if (!evict_out_available_q || (evict_out_ready_i && !evict_out_1st_pass_q)) begin
      evict_out_available_d = |entry_grant_ext;
    end

    if (evict_out_disable_next_i) begin
      evict_out_valid_d = 1'b0;
    end else begin
      evict_out_valid_d = evict_out_available_d;
    end

    evict_pop = !evict_out_available_q || (evict_out_ready_i && !evict_out_1st_pass_q);

    evict_granted_line = 1'b0;
    for (int unsigned i = 0; i < NumMinions; i++) begin
      if (entry_grant_ext[i]) begin
        if ((evict_req_hline_q[i] && (evict_coop_mask_q[i] == CoopMask2Way)) ||
            (evict_req_qword_q[i] && (evict_coop_mask_q[i] == CoopMask4Way))) begin
          evict_granted_line = 1'b1;
        end
      end
    end

    evict_out_up = |evict_valid_i;
  end

  always_comb begin
    if (!evict_out_1st_pass_q) begin
      evict_out_1st_pass_d = (evict_out_available_d &&
          (!evict_out_available_q || evict_out_ready_i)) &&
          evict_granted_line && !evict_out_2nd_pass_d;
    end else begin
      evict_out_1st_pass_d = !(evict_out_valid_o && evict_out_ready_i);
    end
  end

  always_comb begin
    if (!evict_out_2nd_pass_q) begin
      evict_out_2nd_pass_d = evict_out_1st_pass_q && evict_out_valid_o && evict_out_ready_i;
    end else begin
      evict_out_2nd_pass_d = !(evict_out_valid_o && evict_out_ready_i);
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      evict_out_available_q <= 1'b0;
      evict_out_valid_o <= 1'b0;
      evict_out_1st_pass_q <= 1'b0;
      evict_out_2nd_pass_q <= 1'b0;
    end else begin
      evict_out_available_q <= evict_out_available_d;
      evict_out_valid_o <= evict_out_valid_d;
      evict_out_1st_pass_q <= evict_out_1st_pass_d;
      evict_out_2nd_pass_q <= evict_out_2nd_pass_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (evict_pop) begin
      evict_out_entry_winner_q <= entry_grant_qual;
    end
  end

  assign evict_out_valid_1st_o = evict_out_1st_pass_q;
  assign evict_out_valid_2nd_o = evict_out_2nd_pass_q;

  always_comb begin
    for (int unsigned i = 0; i < NumMinions/2; i++) begin
      min_ready_coop_qual[i] = min_ready_coop_q[2*i] && analyze_done_q[2*i]
          && ~(evict_out_valid_o && evict_out_entry_winner_ext[2*i]);
    end
  end

  logic [NumMinions/2-1:0] entry_grant_qual;
  logic [NumMinions-1:0] entry_grant_ext;

  /* verilator lint_off PINCONNECTEMPTY */  // This site needs only the grant vector of the arb_rr replacement.
  prim_arb_rr #(
    .NumReqs (NumMinions/2),
    .Width   (1)
  ) u_evict_arb (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .reqs_i   (min_ready_coop_qual),
    .pop_i    (evict_pop),
    .grants_o (entry_grant_qual),
    .data_i   ('0),
    .data_o   ()
  );
  /* verilator lint_on PINCONNECTEMPTY */

  always_comb begin
    evict_out_entry_winner_ext = '0;
    entry_grant_ext = '0;
    for (int unsigned i = 0; i < NumMinions/2; i++) begin
      evict_out_entry_winner_ext[2*i] = evict_out_entry_winner_q[i];
      entry_grant_ext[2*i] = entry_grant_qual[i];
    end
  end

  for (genvar gen_min = 0; gen_min < NumMinions; gen_min++) begin : gen_req_info_regs
    always_ff @(posedge clk_i) begin
      if (evict_valid_i[gen_min]) begin
        evict_req_qwen_q[gen_min] <= evict_req_i[gen_min].qwen;
        evict_coop_mask_q[gen_min] <= evict_coop_mask[gen_min];
        evict_req_hline_q[gen_min] <= (evict_req_i[gen_min].size == etlink_pkg::SizeHLine);
        evict_req_qword_q[gen_min] <= (evict_req_i[gen_min].size == etlink_pkg::SizeQWord);
      end
    end
  end

  logic [NumMinions-1:0] evict_req_size_line;
  always_comb begin
    evict_req_size_line = '0;
    for (int unsigned i = 0; i < NumMinions; i += 2) begin
      evict_req_size_line[i] = !((evict_coop_mask_q[i] == CoopMask2Way) && evict_req_qword_q[i]);
    end
  end

  logic [NumMinionsL-1:0] evict_out_mux_sel;
  logic [NumMinionsL-1:0] evict_out_mux_sel_2way;
  logic [NumMinionsL-1:0] evict_out_mux_sel_4way_1;
  logic [NumMinionsL-1:0] evict_out_mux_sel_4way_2;
  logic [NumMinionsL-1:0] evict_out_mux_sel_4way_3;
  logic [etlink_pkg::SourceSize-1:0] evict_out_mux_sel_ext;

  always_comb begin : mux_select_generation
    evict_out_winner_select = evict_pop ? entry_grant_ext : evict_out_entry_winner_ext;
    evict_out_mux_sel = '0;
    for (int unsigned i = 0; i < NumMinions; i += 2) begin
      if (evict_out_winner_select[i]) begin
        evict_out_mux_sel = i[NumMinionsL-1:0];
        evict_out_mux_sel[0] = 1'b0;
      end
    end

    evict_out_mux_sel_ext = {{(etlink_pkg::SourceSize-NumMinionsL){1'b0}}, evict_out_mux_sel};
  end

  generate
    if (NumMinions > 2) begin : gen_2way_partner_multi
      assign evict_out_mux_sel_2way = {evict_out_mux_sel[NumMinionsL-1:1], 1'b1};
    end else begin : gen_2way_partner_two
      assign evict_out_mux_sel_2way = 1'b1;
    end
  endgenerate

  generate
    if (NumMinions > 4) begin : gen_4way_partner_multi
      assign evict_out_mux_sel_4way_1 = {evict_out_mux_sel[NumMinionsL-1:2], 2'b01};
      assign evict_out_mux_sel_4way_2 = {evict_out_mux_sel[NumMinionsL-1:2], 2'b10};
      assign evict_out_mux_sel_4way_3 = {evict_out_mux_sel[NumMinionsL-1:2], 2'b11};
    end else begin : gen_4way_partner_four
      assign evict_out_mux_sel_4way_1 = 2'b01;
      assign evict_out_mux_sel_4way_2 = 2'b10;
      assign evict_out_mux_sel_4way_3 = 2'b11;
    end
  endgenerate

  always_comb begin : output_generation
    int unsigned min_id;

    min_id = 0;

    evict_ready_o = '0;
    for (int unsigned i = 0; i < NumMinions; i++) begin
      evict_ready_o[i] |= evict_out_entry_winner_ext[i] && evict_out_ready_i && !evict_out_1st_pass_q;
      if (!i[0] && (evict_coop_mask_q[i] == CoopMask2Way)) begin
        min_id = i / 2;
        for (int unsigned j = 0; j < 2; j++) begin
          evict_ready_o[min_id * 2 + j] |= evict_ready_o[i]
              && (evict_coop_mask_q[i] == evict_coop_mask_q[min_id * 2 + j]);
        end
      end else if ((i[1:0] == 2'b00) && (evict_coop_mask_q[i] == CoopMask4Way)) begin
        min_id = i / 4;
        for (int unsigned j = 0; j < 4; j++) begin
          evict_ready_o[min_id * 4 + j] |= evict_ready_o[i]
              && (evict_coop_mask_q[i] == evict_coop_mask_q[min_id * 4 + j]);
        end
      end
    end

    evict_out_req_d = evict_req_i[evict_out_mux_sel];
    evict_out_req_d.id = '0;
    evict_out_req_d.id[$bits(minion_dcache_pkg::et_link_core_miss_id_e)-1:0] =
        minion_dcache_pkg::EtLinkCoreMissIdTensorStoreCoop;
    evict_out_req_d.opcode = etlink_pkg::ReqWriteAround;
    evict_out_req_d.subopcode = etlink_pkg::SubWriteAround;
    evict_out_req_d.source = SourceOffsetValue + evict_out_mux_sel_ext;
    evict_out_req_d.address = evict_req_i[evict_out_mux_sel].address;
    evict_out_req_d.size = evict_req_size_line[evict_out_mux_sel]
        ? etlink_pkg::SizeLine : etlink_pkg::SizeHLine;
    evict_out_req_d.qwen = evict_req_qwen_q[evict_out_mux_sel];
    evict_out_req_d.data = evict_req_i[evict_out_mux_sel].data;
    evict_out_req_d.wdata = evict_req_i[evict_out_mux_sel].wdata;

    if (evict_coop_mask_q[evict_out_mux_sel] == CoopMask2Way) begin
      if (!evict_req_size_line[evict_out_mux_sel]) begin
        evict_out_req_d.qwen = evict_req_qwen_q[evict_out_mux_sel]
            | evict_req_qwen_q[evict_out_mux_sel_2way];
        evict_out_req_d.data[1 * 128 +: 128] = evict_req_i[evict_out_mux_sel_2way].data[1 * 128 +: 128];
      end else begin
        evict_out_req_d.qwen = 4'b1111;
        if (evict_out_2nd_pass_d) begin
          evict_out_req_d.data = evict_req_i[evict_out_mux_sel_2way].data;
        end
      end
    end else if (evict_coop_mask_q[evict_out_mux_sel] == CoopMask4Way) begin
      evict_out_req_d.qwen = 4'b1111;
      if (!evict_out_2nd_pass_d) begin
        evict_out_req_d.data[1 * 128 +: 128] = evict_req_i[evict_out_mux_sel_4way_1].data[1 * 128 +: 128];
      end else begin
        evict_out_req_d.data = {
          evict_req_i[evict_out_mux_sel_4way_3].data[1 * 128 +: 128],
          evict_req_i[evict_out_mux_sel_4way_2].data[0 * 128 +: 128]
        };
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (evict_out_up) begin
      evict_out_req_q <= evict_out_req_d;
    end
  end

  assign evict_out_req_o = evict_out_req_q;

  logic [NumMinions-1:0][4:0] min_pending_ack_q;
  logic [NumMinions-1:0][4:0] min_pending_ack_d;
  logic [NumMinions-1:0] min_pending_ack_up;
  logic [NumMinions-1:0][3:0] min_wa_counter_q;
  logic [NumMinions-1:0][3:0] min_wa_counter_d;
  logic [NumMinions-1:0] min_wa_counter_up;
  logic [NumMinions-1:0] min_coop_mask_q;
  logic [NumMinions-1:0] min_ack_back;

  for (genvar gen_min = 0; gen_min < NumMinions; gen_min++) begin : gen_ack_state
    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        min_wa_counter_q[gen_min] <= '0;
      end else if (min_wa_counter_up[gen_min]) begin
        min_wa_counter_q[gen_min] <= min_wa_counter_d[gen_min];
      end
    end

    always_ff @(posedge clk_i) begin
      if (evict_ready_o[gen_min]) begin
        min_coop_mask_q[gen_min] <= evict_coop_mask_q[gen_min];
      end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        min_pending_ack_q[gen_min] <= '0;
      end else if (min_pending_ack_up[gen_min]) begin
        min_pending_ack_q[gen_min] <= min_pending_ack_d[gen_min];
      end
    end
  end

  always_comb begin
    for (int unsigned i = 0; i < NumMinions; i++) begin
      min_ack_back[i] = tstore_min_ack_i[i];
      min_can_accept_new_wa[i] = (min_wa_counter_q[i] == '0)
          || (min_coop_mask_q[i] == evict_coop_mask_q[i])
          || ((min_wa_counter_q[i] + min_pending_ack_q[i]) < 5'd31);

      min_wa_counter_up[i] = evict_ready_o[i] || min_ack_back[i];
      min_wa_counter_d[i] = min_wa_counter_q[i];
      if (evict_ready_o[i]) begin
        min_wa_counter_d[i] = min_wa_counter_d[i] + 4'b1;
      end
      if (min_ack_back[i]) begin
        min_wa_counter_d[i] = min_wa_counter_d[i] - 4'b1;
      end
    end
  end

  always_comb begin : ack_generate
    int unsigned min_id;

    min_id = 0;

    min_pending_ack_d = min_pending_ack_q;
    min_pending_ack_up = '0;

    for (int unsigned i = 0; i < NumMinions; i++) begin
      if (min_ack_back[i] && !i[0] && (min_coop_mask_q[i] == CoopMask2Way)) begin
        min_id = i / 2;
        for (int unsigned j = 1; j < 2; j++) begin
          min_pending_ack_d[min_id * 2 + j] = min_pending_ack_d[min_id * 2 + j] + 5'b1;
          min_pending_ack_up[min_id * 2 + j] = 1'b1;
        end
      end else if (min_ack_back[i] && (i[1:0] == 2'b00) &&
                   (min_coop_mask_q[i] == CoopMask4Way)) begin
        min_id = i / 4;
        for (int unsigned j = 1; j < 4; j++) begin
          min_pending_ack_d[min_id * 4 + j] = min_pending_ack_d[min_id * 4 + j] + 5'b1;
          min_pending_ack_up[min_id * 4 + j] = 1'b1;
        end
      end
    end

    for (int unsigned i = 0; i < NumMinions; i++) begin
      if (tstore_min_resp_ready_i[i] && tstore_min_resp_valid_o[i]) begin
        min_pending_ack_d[i] = min_pending_ack_d[i] - 5'b1;
      end
      min_pending_ack_up[i] |= tstore_min_resp_ready_i[i] && tstore_min_resp_valid_o[i];
    end
  end

  always_comb begin
    tstore_min_resp_o = '0;
    for (int unsigned i = 0; i < NumMinions; i++) begin
      tstore_min_resp_valid_o[i] = (min_pending_ack_q[i] > '0);
      tstore_min_resp_o[i].id = '0;
      tstore_min_resp_o[i].id[$bits(minion_dcache_pkg::et_link_core_miss_id_e)-1:0] =
          minion_dcache_pkg::EtLinkCoreMissIdTensorStoreCoop;
      tstore_min_resp_o[i].dest = 1'b0;
      tstore_min_resp_o[i].wdata = 1'b0;
      tstore_min_resp_o[i].opcode = minion_pkg::EtLinkRspAck;
      tstore_min_resp_o[i].data = '0;
      tstore_min_resp_o[i].size = minion_pkg::EtLinkSz1;
      tstore_min_resp_o[i].qwen = 4'b0000;
    end
  end

  always_comb begin
    dbg_sm_signals_o.tstore_internal_dbg_signals = {
      analyze_done_q,
      entry_grant_qual,
      evict_out_available_q,
      evict_granted_line,
      evict_out_up,
      evict_out_1st_pass_q,
      evict_out_2nd_pass_q,
      evict_pop,
      evict_out_mux_sel,
      min_ack_back,
      min_coop_mask_q
    };
  end

endmodule : neigh_tensor_store_buffer_block
