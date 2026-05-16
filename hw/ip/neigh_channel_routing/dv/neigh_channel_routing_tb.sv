// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_channel_routing_tb
  import etlink_pkg::*;
  import minion_pkg::*;
  import esr_pkg::*;
  import neigh_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [MinPerN-1:0] flb_req_valid_i,
  input  logic [(MinPerN*CsrFlBarrierSize)-1:0] flb_req_data_bits_i,
  output logic [MinPerN-1:0] flb_resp_valid_o,
  output logic flb_resp_data_o,
  output logic flb_l2_req_valid_o,
  output logic [CsrFlBarrierSize-1:0] flb_l2_req_data_o,
  input  logic flb_l2_resp_valid_i,
  input  logic flb_l2_resp_data_i,

  input  logic [(MinPerN*NeighMaxLocalConnect)-1:0] lmn_valid_out_bits_i,
  input  logic [(MinPerN*$bits(neigh_rsp_t))-1:0] lmn_info_out_bits_i,
  input  logic [MinPerN-1:0] lmn_ready_in_i,
  output logic [MinPerN-1:0] lmn_ready_out_o,
  output logic [MinPerN-1:0] lmn_valid_in_o,
  output logic [(MinPerN*$bits(neigh_rsp_t))-1:0] lmn_info_in_bits_o,
  output logic [(MinPerN*NeighMaxLocalConnect)-1:0] lmn_channel_mask_bits_o,
  output logic [(MinPerN*NeighMaxLocalConnect*$clog2(MinPerN))-1:0] lmn_dest_id_bits_o,
  output logic [13:0] lmn_dbg_valid_out_o,

  input  logic [(MinPerN*PmuMinionCountersNum)-1:0] pmu_count_up_bits_i,
  input  logic [(MinPerN*NrThreads*PmuCountersSelectBits)-1:0] pmu_read_sel_bits_i,
  input  logic [(MinPerN*PmuTotalCountersNum)-1:0] pmu_write_en_bits_i,
  input  logic [(MinPerN*XregSize)-1:0] pmu_write_data_bits_i,
  input  logic [(MinPerN*PmuNeighEventCntSelBits)-1:0] pmu_neigh_event_sel_bits_i,
  input  logic [PmuNeighEvents:1] pmu_neigh_events_i,
  input  logic pmu_disable_clock_i,
  output logic [(MinPerN*NrThreads*XregSize)-1:0] pmu_read_data_bits_o,

  input  logic [$bits(neigh_req_t)-1:0] min_tbox_req_info_bits_i,
  input  logic min_tbox_req_valid_early_i,
  input  logic min_tbox_req_valid_i,
  output logic min_tbox_req_ready_o,
  output logic [$bits(neigh_rsp_t)-1:0] tbox_min_rsp_info_bits_o,
  output logic tbox_min_rsp_valid_o,
  input  logic tbox_min_rsp_ready_i,
  input  logic tbox_sc_req_disable_next_i,
  output logic [$bits(req_nodata_t)-1:0] tbox_sc_req_info_bits_o,
  output logic tbox_sc_req_valid_o,
  input  logic tbox_sc_req_ready_i,
  input  logic [$bits(rsp_t)-1:0] sc_tbox_rsp_info_bits_i,
  input  logic sc_tbox_rsp_valid_i,
  output logic sc_tbox_rsp_ready_o,
  input  logic [$bits(neigh_req_t)-1:0] tbox_neigh_req_info_bits_i,
  input  logic tbox_neigh_req_valid_i,
  output logic tbox_neigh_req_ready_o,
  output logic [$bits(rsp_t)-1:0] tbox_neigh_rsp_info_bits_o,
  output logic tbox_neigh_rsp_valid_o,
  input  logic tbox_neigh_rsp_ready_i
);

  localparam int unsigned FlbWidth = CsrFlBarrierSize;
  localparam int unsigned MinionIdxWidth = $clog2(MinPerN);
  localparam int unsigned NeighRspBits = $bits(neigh_rsp_t);

  logic [MinPerN-1:0][FlbWidth-1:0] flb_req_data;

  logic [MinPerN-1:0][NeighMaxLocalConnect-1:0] local_message_valid_out;
  neigh_rsp_t [MinPerN-1:0] local_message_info_out;
  neigh_rsp_t [MinPerN-1:0] local_message_info_in;
  logic [MinPerN-1:0][NeighMaxLocalConnect-1:0] local_channel_mask;
  logic [MinPerN-1:0][NeighMaxLocalConnect-1:0][MinionIdxWidth-1:0] local_dest_id;
  fln_dbg_sm_t lmn_dbg;

  logic [MinPerN-1:0][PmuMinionCountersNum-1:0] pmu_count_up;
  logic [MinPerN-1:0][NrThreads-1:0][PmuCountersSelectBits-1:0] pmu_read_sel;
  logic [MinPerN-1:0][PmuTotalCountersNum-1:0] pmu_write_en;
  logic [MinPerN-1:0][XregSize-1:0] pmu_write_data;
  logic [MinPerN-1:0][PmuNeighEventCntSelBits-1:0] pmu_neigh_event_sel;
  logic [MinPerN-1:0][NrThreads-1:0][XregSize-1:0] pmu_read_data;
  esr_pmu_ctrl_t pmu_ctrl;

  neigh_req_t min_tbox_req_info;
  neigh_rsp_t tbox_min_rsp_info;
  req_nodata_t tbox_sc_req_info;
  rsp_t sc_tbox_rsp_info;
  neigh_req_t tbox_neigh_req_info;
  rsp_t tbox_neigh_rsp_info;

  always_comb begin
    for (int unsigned m = 0; m < MinPerN; m++) begin
      flb_req_data[m] = flb_req_data_bits_i[m*FlbWidth +: FlbWidth];

      local_message_valid_out[m] = lmn_valid_out_bits_i[m*NeighMaxLocalConnect +: NeighMaxLocalConnect];
      local_message_info_out[m] = neigh_rsp_t'(lmn_info_out_bits_i[m*NeighRspBits +: NeighRspBits]);
      lmn_info_in_bits_o[m*NeighRspBits +: NeighRspBits] = local_message_info_in[m];
      lmn_channel_mask_bits_o[m*NeighMaxLocalConnect +: NeighMaxLocalConnect] = local_channel_mask[m];
      lmn_dest_id_bits_o[m*NeighMaxLocalConnect*MinionIdxWidth +: NeighMaxLocalConnect*MinionIdxWidth] =
          local_dest_id[m];

      pmu_count_up[m] = pmu_count_up_bits_i[m*PmuMinionCountersNum +: PmuMinionCountersNum];
      pmu_write_en[m] = pmu_write_en_bits_i[m*PmuTotalCountersNum +: PmuTotalCountersNum];
      pmu_write_data[m] = pmu_write_data_bits_i[m*XregSize +: XregSize];
      pmu_neigh_event_sel[m] = pmu_neigh_event_sel_bits_i[m*PmuNeighEventCntSelBits +: PmuNeighEventCntSelBits];
      for (int unsigned t = 0; t < NrThreads; t++) begin
        pmu_read_sel[m][t] = pmu_read_sel_bits_i[(m*NrThreads+t)*PmuCountersSelectBits +: PmuCountersSelectBits];
        pmu_read_data_bits_o[(m*NrThreads+t)*XregSize +: XregSize] = pmu_read_data[m][t];
      end
    end

    pmu_ctrl = '{disable_clock: pmu_disable_clock_i};

    min_tbox_req_info = neigh_req_t'(min_tbox_req_info_bits_i);
    tbox_min_rsp_info_bits_o = tbox_min_rsp_info;
    tbox_sc_req_info_bits_o = tbox_sc_req_info;
    sc_tbox_rsp_info = rsp_t'(sc_tbox_rsp_info_bits_i);
    tbox_neigh_req_info = neigh_req_t'(tbox_neigh_req_info_bits_i);
    tbox_neigh_rsp_info_bits_o = tbox_neigh_rsp_info;
    lmn_dbg_valid_out_o = lmn_dbg.local_message_valid_out;
  end

  neigh_fl_barrier u_flb (
    .clk_i,
    .rst_ni,
    .flb_core_neigh_req_valid_i (flb_req_valid_i),
    .flb_core_neigh_req_data_i  (flb_req_data),
    .flb_neigh_core_resp_valid_o(flb_resp_valid_o),
    .flb_neigh_core_resp_data_o (flb_resp_data_o),
    .flb_neigh_l2_req_valid_o   (flb_l2_req_valid_o),
    .flb_neigh_l2_req_data_o    (flb_l2_req_data_o),
    .flb_l2_neigh_resp_valid_i  (flb_l2_resp_valid_i),
    .flb_l2_neigh_resp_data_i   (flb_l2_resp_data_i)
  );

  neigh_local_message_network u_lmn (
    .clk_i,
    .rst_ni,
    .local_channel_mask_o       (local_channel_mask),
    .local_dest_id_o            (local_dest_id),
    .local_message_ready_out_o  (lmn_ready_out_o),
    .local_message_valid_out_i  (local_message_valid_out),
    .local_message_info_out_i   (local_message_info_out),
    .local_message_ready_in_i   (lmn_ready_in_i),
    .local_message_valid_in_o   (lmn_valid_in_o),
    .local_message_info_in_o    (local_message_info_in),
    .dbg_sm_signals_o           (lmn_dbg)
  );

  neigh_pmu u_pmu (
    .clk_i,
    .rst_ni,
    .pmu_count_up_i            (pmu_count_up),
    .pmu_read_data_o           (pmu_read_data),
    .pmu_read_sel_i            (pmu_read_sel),
    .pmu_write_en_i            (pmu_write_en),
    .pmu_write_data_i          (pmu_write_data),
    .pmu_neigh_event_sel_i     (pmu_neigh_event_sel),
    .pmu_neigh_events_i,
    .pmu_ctrl_i                (pmu_ctrl)
  );

  neigh_tbox_router u_tbox (
    .clk_i,
    .rst_ni,
    .min_tbox_req_info_i       (min_tbox_req_info),
    .min_tbox_req_valid_early_i,
    .min_tbox_req_valid_i,
    .min_tbox_req_ready_o,
    .tbox_min_rsp_info_o       (tbox_min_rsp_info),
    .tbox_min_rsp_valid_o,
    .tbox_min_rsp_ready_i,
    .tbox_sc_req_disable_next_i,
    .tbox_sc_req_info_o        (tbox_sc_req_info),
    .tbox_sc_req_valid_o,
    .tbox_sc_req_ready_i,
    .sc_tbox_rsp_info_i        (sc_tbox_rsp_info),
    .sc_tbox_rsp_valid_i,
    .sc_tbox_rsp_ready_o,
    .tbox_neigh_req_info_i     (tbox_neigh_req_info),
    .tbox_neigh_req_valid_i,
    .tbox_neigh_req_ready_o,
    .tbox_neigh_rsp_info_o     (tbox_neigh_rsp_info),
    .tbox_neigh_rsp_valid_o,
    .tbox_neigh_rsp_ready_i
  );

endmodule : neigh_channel_routing_tb
