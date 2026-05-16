// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_local_message_network_tb
  import etlink_pkg::*;
  import minion_pkg::*;
  import neigh_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [(MinPerN*NeighMaxLocalConnect)-1:0] local_message_valid_out_bits_i,
  input  logic [(MinPerN*$bits(neigh_rsp_t))-1:0] local_message_info_out_bits_i,
  input  logic [MinPerN-1:0] local_message_ready_in_i,

  output logic [(MinPerN*NeighMaxLocalConnect)-1:0] orig_local_channel_mask_bits_o,
  output logic [(MinPerN*NeighMaxLocalConnect)-1:0] new_local_channel_mask_bits_o,
  output logic [(MinPerN*NeighMaxLocalConnect*$clog2(MinPerN))-1:0] orig_local_dest_id_bits_o,
  output logic [(MinPerN*NeighMaxLocalConnect*$clog2(MinPerN))-1:0] new_local_dest_id_bits_o,
  output logic [MinPerN-1:0] orig_local_message_ready_out_o,
  output logic [MinPerN-1:0] new_local_message_ready_out_o,
  output logic [MinPerN-1:0] orig_local_message_valid_in_o,
  output logic [MinPerN-1:0] new_local_message_valid_in_o,
  output logic [(MinPerN*$bits(neigh_rsp_t))-1:0] orig_local_message_info_in_bits_o,
  output logic [(MinPerN*$bits(neigh_rsp_t))-1:0] new_local_message_info_in_bits_o,
  output logic [13:0] orig_dbg_local_message_valid_out_o,
  output logic [13:0] new_dbg_local_message_valid_out_o
);

  localparam int unsigned RspBits = $bits(neigh_rsp_t);
  localparam int unsigned MinionIdxWidth = $clog2(MinPerN);

  logic [MinPerN-1:0][NeighMaxLocalConnect-1:0] local_message_valid_out;
  neigh_rsp_t [MinPerN-1:0] new_local_message_info_out;
  neigh_rsp_t [MinPerN-1:0] new_local_message_info_in;
  logic [MinPerN-1:0][NeighMaxLocalConnect-1:0] new_local_channel_mask;
  logic [MinPerN-1:0][NeighMaxLocalConnect-1:0][MinionIdxWidth-1:0] new_local_dest_id;
  fln_dbg_sm_t new_dbg;

  et_link_neigh_rsp_info_t [MinPerN-1:0] orig_local_message_info_out;
  et_link_neigh_rsp_info_t [MinPerN-1:0] orig_local_message_info_in;
  logic [MinPerN-1:0][NeighMaxLocalConnect-1:0] orig_local_channel_mask;
  logic [MinPerN-1:0][NeighMaxLocalConnect-1:0][MinionIdxWidth-1:0] orig_local_dest_id;
  fln_dbg_sm_t orig_dbg;

  always_comb begin
    for (int unsigned m = 0; m < MinPerN; m++) begin
      local_message_valid_out[m] = local_message_valid_out_bits_i[m*NeighMaxLocalConnect +: NeighMaxLocalConnect];
      new_local_message_info_out[m] = neigh_rsp_t'(local_message_info_out_bits_i[m*RspBits +: RspBits]);
      orig_local_message_info_out[m] = et_link_neigh_rsp_info_t'(local_message_info_out_bits_i[m*RspBits +: RspBits]);

      new_local_message_info_in_bits_o[m*RspBits +: RspBits] = new_local_message_info_in[m];
      orig_local_message_info_in_bits_o[m*RspBits +: RspBits] = orig_local_message_info_in[m];
      new_local_channel_mask_bits_o[m*NeighMaxLocalConnect +: NeighMaxLocalConnect] = new_local_channel_mask[m];
      orig_local_channel_mask_bits_o[m*NeighMaxLocalConnect +: NeighMaxLocalConnect] = orig_local_channel_mask[m];
      new_local_dest_id_bits_o[m*NeighMaxLocalConnect*MinionIdxWidth +: NeighMaxLocalConnect*MinionIdxWidth] =
          new_local_dest_id[m];
      orig_local_dest_id_bits_o[m*NeighMaxLocalConnect*MinionIdxWidth +: NeighMaxLocalConnect*MinionIdxWidth] =
          orig_local_dest_id[m];
    end
    new_dbg_local_message_valid_out_o = new_dbg.local_message_valid_out;
    orig_dbg_local_message_valid_out_o = orig_dbg.local_message_valid_out;
  end

  neigh_local_message_network_orig u_orig (
    .clock                   (clk_i),
    .reset                   (!rst_ni),
    .local_channel_mask      (orig_local_channel_mask),
    .local_dest_id           (orig_local_dest_id),
    .local_message_ready_out (orig_local_message_ready_out_o),
    .local_message_valid_out (local_message_valid_out),
    .local_message_info_out  (orig_local_message_info_out),
    .local_message_ready_in  (local_message_ready_in_i),
    .local_message_valid_in  (orig_local_message_valid_in_o),
    .local_message_info_in   (orig_local_message_info_in),
    .dbg_sm_signals          (orig_dbg)
  );

  neigh_local_message_network u_new (
    .clk_i,
    .rst_ni,
    .local_channel_mask_o      (new_local_channel_mask),
    .local_dest_id_o           (new_local_dest_id),
    .local_message_ready_out_o (new_local_message_ready_out_o),
    .local_message_valid_out_i (local_message_valid_out),
    .local_message_info_out_i  (new_local_message_info_out),
    .local_message_ready_in_i  (local_message_ready_in_i),
    .local_message_valid_in_o  (new_local_message_valid_in_o),
    .local_message_info_in_o   (new_local_message_info_in),
    .dbg_sm_signals_o          (new_dbg)
  );

endmodule : cosim_neigh_local_message_network_tb
