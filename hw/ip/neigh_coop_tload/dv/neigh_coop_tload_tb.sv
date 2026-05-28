// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_coop_tload_tb
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import neigh_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [2*ShireCoopLoadIdSize-1:0] tt_wr_tag_bits_i,
  input  logic [2*MinPerN-1:0] tt_wr_min_mask_bits_i,
  input  logic [2*MinPerN*ShireCoopMissIdSize-1:0] tt_wr_miss_id_bits_i,
  input  logic [1:0] tt_wr_valid_i,
  output logic [1:0] tt_wr_ready_o,
  input  logic tt_cam_pop_i,
  input  logic [ShireCoopLoadIdSize-1:0] tt_cam_tag_i,
  output logic [MinPerN-1:0] tt_cam_min_mask_o,
  output logic [MinPerN*ShireCoopMissIdSize-1:0] tt_cam_miss_id_bits_o,

  input  logic [NumNeighWidth+ShireCoopIdSize-1:0] ports_slv_rdy_out_data_lv_bits_i,
  input  logic ports_slv_rdy_out_valid_lv_i,
  output logic [NumNeighWidth+ShireCoopIdSize-1:0] ports_slv_rdy_out_data_bits_o,
  output logic ports_slv_rdy_out_valid_o,
  input  logic [NumOtherNeigh*(NumNeighWidth+ShireCoopIdSize)-1:0] ports_slv_rdy_in_data_bits_i,
  input  logic [NumOtherNeigh-1:0] ports_slv_rdy_in_valid_i,
  output logic [NumOtherNeigh*(NumNeighWidth+ShireCoopIdSize)-1:0] ports_slv_rdy_in_data_lv_bits_o,
  output logic [NumOtherNeigh-1:0] ports_slv_rdy_in_valid_lv_o,
  input  logic [ShireCoopIdSize-1:0] ports_mst_done_out_coop_id_lv_i,
  input  logic [NumOtherNeigh-1:0] ports_mst_done_out_valid_lv_i,
  output logic [ShireCoopIdSize-1:0] ports_mst_done_out_coop_id_o,
  output logic [NumOtherNeigh-1:0] ports_mst_done_out_valid_o,
  input  logic [NumOtherNeigh*ShireCoopIdSize-1:0] ports_mst_done_in_coop_id_bits_i,
  input  logic [NumOtherNeigh-1:0] ports_mst_done_in_valid_i,
  output logic [NumOtherNeigh*ShireCoopIdSize-1:0] ports_mst_done_in_coop_id_lv_bits_o,
  output logic [NumOtherNeigh-1:0] ports_mst_done_in_valid_lv_o,

  input  logic [NumNeighWidth-1:0] tl_neigh_id_i,
  input  logic [MinPerN*ShireCoopMissReqs-1:0] tl_req_valid_bits_i,
  input  logic [MinPerN*ShireCoopMissReqs*5-1:0] tl_req_id_bits_i,
  input  logic [MinPerN*ShireCoopMissReqs*PaSize-1:0] tl_req_addr_bits_i,
  input  logic [MinPerN*ShireCoopMissReqs*ShireCoopDataSize-1:0] tl_req_data_bits_i,
  input  logic [MinPerN*ShireCoopMissReqs*3-1:0] tl_req_size_bits_i,
  input  logic [MinPerN*ShireCoopMissReqs*7-1:0] tl_req_subopcode_bits_i,
  input  logic tl_disable_next_i,
  input  logic tl_req_ready_i,
  input  logic [NumOtherNeigh*(NumNeighWidth+ShireCoopIdSize)-1:0] tl_slv_rdy_in_data_bits_i,
  input  logic [NumOtherNeigh-1:0] tl_slv_rdy_in_valid_i,
  input  logic [NumOtherNeigh*ShireCoopIdSize-1:0] tl_mst_done_in_coop_id_bits_i,
  input  logic [NumOtherNeigh-1:0] tl_mst_done_in_valid_i,
  input  logic tl_tag_cam_pop_i,
  input  logic [ShireCoopLoadIdSize-1:0] tl_tag_cam_tag_i,

  output logic [MinPerN*ShireCoopMissReqs-1:0] tl_req_ready_bits_o,
  output logic tl_req_bid_o,
  output logic [IdSize-1:0] tl_req_id_o,
  output logic tl_req_wdata_o,
  output logic [4:0] tl_req_opcode_o,
  output logic [PaSize-1:0] tl_req_addr_o,
  output logic [ShireCoopDataSize-1:0] tl_req_data_low_o,
  output logic [2:0] tl_req_size_o,
  output logic [3:0] tl_req_qwen_o,
  output logic [6:0] tl_req_subopcode_o,
  output logic tl_slv_rdy_out_valid_o,
  output logic [NumNeighWidth+ShireCoopIdSize-1:0] tl_slv_rdy_out_data_bits_o,
  output logic [NumOtherNeigh-1:0] tl_mst_done_out_valid_o,
  output logic [ShireCoopIdSize-1:0] tl_mst_done_out_coop_id_o,
  output logic [MinPerN-1:0] tl_tag_cam_min_mask_o,
  output logic [MinPerN*ShireCoopMissIdSize-1:0] tl_tag_cam_miss_id_bits_o,
  output logic [MinPerN*ShireCoopMissReqs-1:0] tl_dbg_local_rdy_bits_o,
  output logic [MinPerN*ShireCoopMissReqs-1:0] tl_dbg_slv_creds_bits_o
);

  localparam int unsigned SlvRdyBits = NumNeighWidth + ShireCoopIdSize;
  localparam int unsigned ReqSlots = MinPerN * ShireCoopMissReqs;

  coop_tload_tag_table_req_t [1:0] tt_wr_req;
  coop_tload_tag_table_data_t tt_cam_data;

  always_comb begin
    for (int unsigned p = 0; p < 2; p++) begin
      tt_wr_req[p].tag = tt_wr_tag_bits_i[p*ShireCoopLoadIdSize +: ShireCoopLoadIdSize];
      tt_wr_req[p].data.coop_min_mask = tt_wr_min_mask_bits_i[p*MinPerN +: MinPerN];
      for (int unsigned m = 0; m < MinPerN; m++) begin
        tt_wr_req[p].data.min_miss_id[m] =
            tt_wr_miss_id_bits_i[(p*MinPerN + m)*ShireCoopMissIdSize +: ShireCoopMissIdSize];
      end
    end
    tt_cam_min_mask_o = tt_cam_data.coop_min_mask;
    for (int unsigned m = 0; m < MinPerN; m++) begin
      tt_cam_miss_id_bits_o[m*ShireCoopMissIdSize +: ShireCoopMissIdSize] =
          tt_cam_data.min_miss_id[m];
    end
  end

  neigh_coop_tload_tag_table #(
    .Depth(4)
  ) u_tag_table (
    .clk_i,
    .rst_ni,
    .wr_req_i   (tt_wr_req),
    .wr_valid_i (tt_wr_valid_i),
    .wr_ready_o (tt_wr_ready_o),
    .cam_pop_i  (tt_cam_pop_i),
    .cam_tag_i  (tt_cam_tag_i),
    .cam_data_o (tt_cam_data)
  );

  coop_tload_slv_rdy_req_t ports_slv_rdy_out_data_lv;
  coop_tload_slv_rdy_req_t ports_slv_rdy_out_data;
  coop_tload_slv_rdy_req_t [NumOtherNeigh-1:0] ports_slv_rdy_in_data;
  coop_tload_slv_rdy_req_t [NumOtherNeigh-1:0] ports_slv_rdy_in_data_lv;
  logic [NumOtherNeigh-1:0][ShireCoopIdSize-1:0] ports_mst_done_in_coop_id;
  logic [NumOtherNeigh-1:0][ShireCoopIdSize-1:0] ports_mst_done_in_coop_id_lv;

  always_comb begin
    ports_slv_rdy_out_data_lv = coop_tload_slv_rdy_req_t'(ports_slv_rdy_out_data_lv_bits_i);
    ports_slv_rdy_out_data_bits_o = ports_slv_rdy_out_data;
    for (int unsigned n = 0; n < NumOtherNeigh; n++) begin
      ports_slv_rdy_in_data[n] = coop_tload_slv_rdy_req_t'(
          ports_slv_rdy_in_data_bits_i[n*SlvRdyBits +: SlvRdyBits]);
      ports_slv_rdy_in_data_lv_bits_o[n*SlvRdyBits +: SlvRdyBits] = ports_slv_rdy_in_data_lv[n];
      ports_mst_done_in_coop_id[n] =
          ports_mst_done_in_coop_id_bits_i[n*ShireCoopIdSize +: ShireCoopIdSize];
      ports_mst_done_in_coop_id_lv_bits_o[n*ShireCoopIdSize +: ShireCoopIdSize] =
          ports_mst_done_in_coop_id_lv[n];
    end
  end

  neigh_coop_tload_ports u_ports (
    .clk_i,
    .rst_ni,
    .coop_tload_slv_rdy_out_data_o        (ports_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid_o       (ports_slv_rdy_out_valid_o),
    .coop_tload_slv_rdy_in_data_i         (ports_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid_i        (ports_slv_rdy_in_valid_i),
    .coop_tload_mst_done_out_coop_id_o    (ports_mst_done_out_coop_id_o),
    .coop_tload_mst_done_out_valid_o      (ports_mst_done_out_valid_o),
    .coop_tload_mst_done_in_coop_id_i     (ports_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid_i       (ports_mst_done_in_valid_i),
    .coop_tload_slv_rdy_out_data_lv_i     (ports_slv_rdy_out_data_lv),
    .coop_tload_slv_rdy_out_valid_lv_i    (ports_slv_rdy_out_valid_lv_i),
    .coop_tload_slv_rdy_in_data_lv_o      (ports_slv_rdy_in_data_lv),
    .coop_tload_slv_rdy_in_valid_lv_o     (ports_slv_rdy_in_valid_lv_o),
    .coop_tload_mst_done_out_coop_id_lv_i (ports_mst_done_out_coop_id_lv_i),
    .coop_tload_mst_done_out_valid_lv_i   (ports_mst_done_out_valid_lv_i),
    .coop_tload_mst_done_in_coop_id_lv_o  (ports_mst_done_in_coop_id_lv),
    .coop_tload_mst_done_in_valid_lv_o    (ports_mst_done_in_valid_lv_o)
  );

  et_link_minion_miss_req_info_t [MinPerN-1:0][ShireCoopMissReqs-1:0] tl_min_req;
  coop_tload_slv_rdy_req_t [NumOtherNeigh-1:0] tl_slv_rdy_in_data;
  logic [NumOtherNeigh-1:0][ShireCoopIdSize-1:0] tl_mst_done_in_coop_id;
  etlink_pkg::neigh_req_t tl_req;
  coop_tload_slv_rdy_req_t tl_slv_rdy_out_data;
  coop_tload_tag_table_data_t tl_tag_cam_data;
  coop_tload_dbg_sm_t tl_dbg;
  logic unused_tl_req_fields;
  logic unused_tl_dbg_fields;
  assign unused_tl_req_fields = ^{1'b0, tl_req.source, tl_req.data[NeighDataSize-1:ShireCoopDataSize]};
  assign unused_tl_dbg_fields = ^{1'b0, tl_dbg.mst_ext_is_rdy, tl_dbg.master_neigh};

  always_comb begin
    for (int unsigned slot = 0; slot < ReqSlots; slot++) begin
      logic [2:0] min_idx;
      logic miss_idx;
      min_idx = slot[3:1];
      miss_idx = slot[0];
      tl_min_req[min_idx][miss_idx] = '0;
      tl_min_req[min_idx][miss_idx].id = tl_req_id_bits_i[slot*5 +: 5];
      tl_min_req[min_idx][miss_idx].opcode = minion_dcache_pkg::EtLinkReqRead;
      tl_min_req[min_idx][miss_idx].address = tl_req_addr_bits_i[slot*PaSize +: PaSize];
      tl_min_req[min_idx][miss_idx].data = tl_req_data_bits_i[slot*ShireCoopDataSize +: ShireCoopDataSize];
      tl_min_req[min_idx][miss_idx].size =
          minion_dcache_pkg::et_link_size_e'(tl_req_size_bits_i[slot*3 +: 3]);
      tl_min_req[min_idx][miss_idx].subopcode = tl_req_subopcode_bits_i[slot*7 +: 7];
    end

    for (int unsigned n = 0; n < NumOtherNeigh; n++) begin
      tl_slv_rdy_in_data[n] = coop_tload_slv_rdy_req_t'(
          tl_slv_rdy_in_data_bits_i[n*SlvRdyBits +: SlvRdyBits]);
      tl_mst_done_in_coop_id[n] =
          tl_mst_done_in_coop_id_bits_i[n*ShireCoopIdSize +: ShireCoopIdSize];
    end

    tl_req_id_o = tl_req.id;
    tl_req_wdata_o = tl_req.wdata;
    tl_req_opcode_o = tl_req.opcode;
    tl_req_addr_o = tl_req.address;
    tl_req_data_low_o = tl_req.data[ShireCoopDataSize-1:0];
    tl_req_size_o = tl_req.size;
    tl_req_qwen_o = tl_req.qwen;
    tl_req_subopcode_o = tl_req.subopcode;
    tl_slv_rdy_out_data_bits_o = tl_slv_rdy_out_data;
    tl_tag_cam_min_mask_o = tl_tag_cam_data.coop_min_mask;
    for (int unsigned m = 0; m < MinPerN; m++) begin
      tl_tag_cam_miss_id_bits_o[m*ShireCoopMissIdSize +: ShireCoopMissIdSize] =
          tl_tag_cam_data.min_miss_id[m];
    end

    for (int unsigned slot = 0; slot < ReqSlots; slot++) begin
      logic [2:0] min_idx;
      logic miss_idx;
      min_idx = slot[3:1];
      miss_idx = slot[0];
      tl_dbg_local_rdy_bits_o[slot] = tl_dbg.coop_tload_local_rdy[min_idx][miss_idx];
      tl_dbg_slv_creds_bits_o[slot] = tl_dbg.slv_has_rdy_creds[min_idx][miss_idx];
    end
  end

  neigh_coop_tload u_tload (
    .clk_i,
    .rst_ni,
    .neigh_id_i                     (tl_neigh_id_i),
    .min_coop_miss_req_i            (tl_min_req),
    .min_coop_miss_req_valid_i      (tl_req_valid_bits_i),
    .min_coop_miss_req_ready_o      (tl_req_ready_bits_o),
    .coop_tload_disable_next_i      (tl_disable_next_i),
    .coop_tload_req_o               (tl_req),
    .coop_tload_req_bid_o           (tl_req_bid_o),
    .coop_tload_req_ready_i         (tl_req_ready_i),
    .coop_tload_slv_rdy_out_data_o  (tl_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid_o (tl_slv_rdy_out_valid_o),
    .coop_tload_slv_rdy_in_data_i   (tl_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid_i  (tl_slv_rdy_in_valid_i),
    .coop_tload_mst_done_out_coop_id_o (tl_mst_done_out_coop_id_o),
    .coop_tload_mst_done_out_valid_o   (tl_mst_done_out_valid_o),
    .coop_tload_mst_done_in_coop_id_i  (tl_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid_i    (tl_mst_done_in_valid_i),
    .tag_table_cam_pop_i            (tl_tag_cam_pop_i),
    .tag_table_cam_tag_i            (tl_tag_cam_tag_i),
    .tag_table_cam_data_o           (tl_tag_cam_data),
    .dbg_sm_signals_o               (tl_dbg)
  );

endmodule : neigh_coop_tload_tb
