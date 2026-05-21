// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_bus_master #(
  parameter int unsigned SbmInterfaces = shire_sbm_pkg::SbmInterfaces,
  parameter int unsigned MaxConcReq    = shire_sbm_pkg::MaxConcReq,
  parameter int unsigned ApbDataWidth  = shire_sbm_pkg::ApbDataWidth,
  localparam int unsigned IndexWidth   = $clog2(MaxConcReq)
) (
  input  logic                                      clk_i,
  input  logic                                      rst_ni,

  input  logic [shire_sbm_pkg::NumNeigh-1:0]        rst_neigh_ni,
  input  logic                                      rst_shire_cache_ni,
  input  logic                                      rst_rbox_ni,

  input  logic [shire_sbm_pkg::AxiIdWidth-1:0]      shire_virtual_id_i,
  input  logic                                      sys_axi_ar_valid_i,
  input  logic                                      sys_axi_aw_valid_i,
  output logic                                      credit_return_write_o,

  input  axi_pkg::sys_slave_ar_t                    sys_axi_ar_i,
  output logic                                      sys_axi_ar_ready_o,
  input  axi_pkg::sys_slave_aw_t                    sys_axi_aw_i,
  output logic                                      sys_axi_aw_ready_o,
  input  axi_pkg::sys_slave_w_t                     sys_axi_w_i,
  output logic                                      sys_axi_w_ready_o,
  output axi_pkg::sys_slave_b_t                     sys_axi_b_o,
  output logic                                      sys_axi_b_valid_o,
  input  logic                                      sys_axi_b_ready_i,
  output axi_pkg::sys_slave_r_t                     sys_axi_r_o,
  output logic                                      sys_axi_r_valid_o,
  input  logic                                      sys_axi_r_ready_i,

  output shire_sbm_pkg::shire_apb_req_t [SbmInterfaces-1:0] apb_req_o,
  input  shire_sbm_pkg::shire_apb_rsp_t [SbmInterfaces-1:0] apb_rsp_i
);

  localparam int unsigned WriteIfIdx = 1;
  localparam int unsigned ReadIfIdx  = 0;
  localparam logic [3:0] LastNeigh = 4'd3;
  localparam logic [3:0] LastBank  = 4'd3;
  localparam logic [6:0] LastThread = 7'd31;

  shire_sbm_pkg::req_buffer_array_t sbm_buffer_q;
  shire_sbm_pkg::req_buffer_array_t sbm_buffer_d;
  logic possible_overflow_q;
  logic possible_overflow_d;
  logic [1:0] error_return_q;
  logic [1:0] error_return_d;

  logic [SbmInterfaces-1:0] sbm_fifo_req_push;
  logic [SbmInterfaces-1:0] sbm_fifo_req_pop;
  logic [SbmInterfaces-1:0][IndexWidth-1:0] sbm_index_req_push;
  logic [SbmInterfaces-1:0][IndexWidth-1:0] sbm_index_req_pop;
  logic [SbmInterfaces-1:0] sbm_fifo_req_not_empty;
  logic [SbmInterfaces-1:0] sbm_fifo_req_full_unused;
  logic [SbmInterfaces-1:0][MaxConcReq:0] sbm_fifo_req_level_unused;

  logic input_ar_valid;
  logic input_aw_valid;
  assign input_ar_valid = sys_axi_ar_valid_i && !possible_overflow_q && !error_return_q[ReadIfIdx];
  assign input_aw_valid = sys_axi_aw_valid_i && !possible_overflow_q && !error_return_q[WriteIfIdx];

  for (genvar gen_it2 = 0; gen_it2 < SbmInterfaces; gen_it2++) begin : gen_sbm_fifos
    prim_fifo_reg #(
      .Width (IndexWidth),
      .Depth (MaxConcReq)
    ) u_sbm_req_fifo (
      .clk_i,
      .rst_ni,
      .push_i  (sbm_fifo_req_push[gen_it2]),
      .wdata_i (sbm_index_req_push[gen_it2]),
      .full_o  (sbm_fifo_req_full_unused[gen_it2]),
      .pop_i   (sbm_fifo_req_pop[gen_it2]),
      .valid_o (sbm_fifo_req_not_empty[gen_it2]),
      .rdata_o (sbm_index_req_pop[gen_it2]),
      .level_o (sbm_fifo_req_level_unused[gen_it2])
    );
  end

  logic first_free_taken;
  logic [1:0][IndexWidth-1:0] free_pos;

  logic [SbmInterfaces-1:0] read_req_valid;
  logic [SbmInterfaces-1:0] write_req_valid;

  shire_sbm_pkg::sbm_axi_a_t [SbmInterfaces-1:0] mesh_axi_ar;
  logic [SbmInterfaces-1:0] mesh_axi_ar_valid;
  shire_sbm_pkg::sbm_axi_a_t [SbmInterfaces-1:0] mesh_axi_aw;
  logic [SbmInterfaces-1:0] mesh_axi_aw_valid;
  shire_sbm_pkg::sbm_axi_w_t [SbmInterfaces-1:0] mesh_axi_w;

  axi_pkg::sys_slave_b_t [SbmInterfaces-1:0] mesh_axi_b;
  logic [SbmInterfaces-1:0] mesh_axi_b_valid;
  logic [SbmInterfaces-1:0] mesh_axi_b_ready;
  axi_pkg::sys_slave_r_t [SbmInterfaces-1:0] mesh_axi_r;
  logic [SbmInterfaces-1:0] mesh_axi_r_valid;
  logic [SbmInterfaces-1:0] mesh_axi_r_ready;

  logic [SbmInterfaces-1:0] axi_to_apb_ready;

  logic [ApbDataWidth-1:0] sys_axi_wdata;
  assign sys_axi_wdata = sys_axi_w_i.data[sys_axi_aw_i.addr[4:3] * ApbDataWidth +: ApbDataWidth];

  logic unused_sbm_ok;
  assign unused_sbm_ok = ^{
    shire_virtual_id_i[shire_sbm_pkg::AxiIdWidth-1:8],
    sys_axi_ar_i.size,
    sys_axi_ar_i.burst,
    sys_axi_ar_i.lock,
    sys_axi_ar_i.cache,
    sys_axi_ar_i.prot,
    sys_axi_ar_i.qos,
    sys_axi_ar_i.user,
    sys_axi_aw_i.size,
    sys_axi_aw_i.burst,
    sys_axi_aw_i.lock,
    sys_axi_aw_i.cache,
    sys_axi_aw_i.prot,
    sys_axi_aw_i.qos,
    sys_axi_aw_i.user,
    sys_axi_w_i.strb,
    sys_axi_w_i.last,
    sbm_fifo_req_full_unused,
    sbm_fifo_req_level_unused,
    bcast_fifo_full_unused,
    bcast_fifo_level_unused,
    write_rsp_fifo_level_unused,
    read_rsp_fifo_level_unused
  };

  logic [1:0] broadcast_response_q;
  logic [1:0] broadcast_response_d;
  axi_pkg::sys_slave_b_t [1:0] bcast_axi_b_q;
  axi_pkg::sys_slave_b_t [1:0] bcast_axi_b_d;
  logic broadcast_rsp_processing_id_q;
  logic broadcast_rsp_processing_id_d;

  logic bcast_response_q;
  logic bcast_response_d;
  axi_pkg::sys_slave_b_t bcast_response_b_q;
  axi_pkg::sys_slave_b_t bcast_response_b_d;

  logic [SbmInterfaces-1:0] bvalid_rsp_arb;

  shire_sbm_pkg::sbm_axi_a_t [1:0] bcast_axi_aw_pop;
  shire_sbm_pkg::sbm_axi_a_t [1:0] bcast_axi_aw_q;
  logic [1:0][ApbDataWidth-1:0] bcast_axi_w_pop;
  logic [1:0][ApbDataWidth-1:0] bcast_axi_w_q;

  logic [1:0] bcast_perform_q;
  logic [1:0] bcast_perform_d;
  logic [SbmInterfaces-1:0] bcast_active_q;
  logic [SbmInterfaces-1:0] bcast_active_d;
  logic [SbmInterfaces-1:0] bcast_aw_valid_q;
  logic [SbmInterfaces-1:0] bcast_aw_valid_d;

  shire_sbm_pkg::bcast_ctrl_t [1:0] bcast_control_q;
  shire_sbm_pkg::bcast_ctrl_t [1:0] bcast_control_d;

  logic [shire_sbm_pkg::NumNeigh-1:0] bcast_neighs_valid_mask_q;
  logic [shire_sbm_pkg::NumNeigh-1:0] bcast_neighs_valid_mask_d;
  logic [shire_sbm_pkg::NumNeigh-1:0] bcast_neighs_valid_mask_aux;
  logic [shire_sbm_pkg::NumBanks-1:0] bcast_banks_valid_mask_q;
  logic [shire_sbm_pkg::NumBanks-1:0] bcast_banks_valid_mask_d;
  logic [shire_sbm_pkg::NumBanks-1:0] bcast_banks_valid_mask_aux;

  logic [1:0][3:0] count_bvalid;
  logic [1:0] error_search;
  logic [1:0] bcast_push;
  logic [1:0] bcast_fifo_not_empty;
  logic [1:0] bcast_fifo_full_unused;
  logic [1:0][SbmInterfaces:0] bcast_fifo_level_unused;
  logic bcast_rsp_sent;

  assign bvalid_rsp_arb[SbmInterfaces-1:SbmInterfaces-2] = mesh_axi_b_valid[SbmInterfaces-1:SbmInterfaces-2];
  assign bvalid_rsp_arb[shire_sbm_pkg::NeighRangeHi-1:shire_sbm_pkg::NeighRangeLo] =
      bcast_neighs_valid_mask_q & mesh_axi_b_valid[shire_sbm_pkg::NeighRangeHi-1:shire_sbm_pkg::NeighRangeLo];
  assign bvalid_rsp_arb[shire_sbm_pkg::BankRangeHi-1:shire_sbm_pkg::BankRangeLo] =
      bcast_banks_valid_mask_q & mesh_axi_b_valid[shire_sbm_pkg::BankRangeHi-1:shire_sbm_pkg::BankRangeLo];

  always_comb begin
    bcast_perform_d[0] = !bcast_perform_q[0] && !broadcast_response_q[0] && !bcast_control_q[0].valid &&
                         bcast_fifo_not_empty[0] && (&axi_to_apb_ready[shire_sbm_pkg::NeighRangeHi-1:shire_sbm_pkg::NeighRangeLo]);
    bcast_perform_d[1] = !bcast_perform_q[1] && !broadcast_response_q[1] && !bcast_control_q[1].valid &&
                         bcast_fifo_not_empty[1] && (&axi_to_apb_ready[shire_sbm_pkg::BankRangeHi-1:shire_sbm_pkg::BankRangeLo]);

    error_search[0] = search_error_neigh(mesh_axi_b_valid, mesh_axi_b);
    error_search[1] = search_error_bank(mesh_axi_b_valid, mesh_axi_b);

    bcast_neighs_valid_mask_aux = bcast_neighs_valid_mask_q;
    count_bvalid[0] = count_valids_neigh(
        bcast_neighs_valid_mask_q, bcast_control_q[0].axi_id, mesh_axi_b_valid, mesh_axi_b,
        bcast_neighs_valid_mask_aux);

    bcast_banks_valid_mask_aux = bcast_banks_valid_mask_q;
    count_bvalid[1] = count_valids_bank(
        bcast_banks_valid_mask_q, bcast_control_q[1].axi_id, mesh_axi_b_valid, mesh_axi_b,
        bcast_banks_valid_mask_aux);

    bcast_rsp_sent = bcast_response_q && !error_return_q[WriteIfIdx] && sys_axi_b_ready_i;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      bcast_active_q <= '0;
      broadcast_response_q <= '0;
      bcast_control_q <= '0;
      bcast_neighs_valid_mask_q <= '1;
      bcast_banks_valid_mask_q <= '1;
      bcast_aw_valid_q <= '0;
      bcast_response_q <= 1'b0;
      broadcast_rsp_processing_id_q <= 1'b0;
      bcast_perform_q <= '0;
      bcast_axi_aw_q <= '0;
      bcast_axi_w_q <= '0;
      bcast_axi_b_q <= '0;
      bcast_response_b_q <= '0;
    end else begin
      bcast_active_q <= bcast_active_d;
      broadcast_response_q <= broadcast_response_d;
      bcast_control_q <= bcast_control_d;
      bcast_neighs_valid_mask_q <= bcast_neighs_valid_mask_d;
      bcast_banks_valid_mask_q <= bcast_banks_valid_mask_d;
      bcast_aw_valid_q <= bcast_aw_valid_d;
      bcast_response_q <= bcast_response_d;
      broadcast_rsp_processing_id_q <= broadcast_rsp_processing_id_d;
      bcast_perform_q <= bcast_perform_d;
      bcast_axi_b_q <= bcast_axi_b_d;
      bcast_response_b_q <= bcast_response_b_d;
      if (bcast_perform_q[0]) begin
        bcast_axi_aw_q[0] <= bcast_axi_aw_pop[0];
        bcast_axi_w_q[0] <= bcast_axi_w_pop[0];
      end
      if (bcast_perform_q[1]) begin
        bcast_axi_aw_q[1] <= bcast_axi_aw_pop[1];
        bcast_axi_w_q[1] <= bcast_axi_w_pop[1];
      end
    end
  end

  always_comb begin
    bcast_active_d = bcast_active_q;
    broadcast_response_d = broadcast_response_q;
    bcast_control_d = bcast_control_q;
    bcast_neighs_valid_mask_d = bcast_neighs_valid_mask_q;
    bcast_banks_valid_mask_d = bcast_banks_valid_mask_q;
    bcast_aw_valid_d = '0;
    bcast_response_d = 1'b0;
    broadcast_rsp_processing_id_d = broadcast_rsp_processing_id_q;
    bcast_axi_b_d = bcast_axi_b_q;
    bcast_response_b_d = bcast_response_b_q;

    if (!error_return_q[WriteIfIdx] && (broadcast_response_q != 2'b0)) begin
      bcast_response_d = !bcast_rsp_sent;

      if (broadcast_response_q[1]) begin
        broadcast_response_d[1] = !(bcast_rsp_sent && (broadcast_rsp_processing_id_q == 1'b1));
      end
      if (broadcast_response_q[0]) begin
        broadcast_response_d[0] = !(bcast_rsp_sent && (broadcast_rsp_processing_id_q == 1'b0));
      end

      if (broadcast_response_q[1]) begin
        bcast_response_b_d = bcast_axi_b_q[1];
        broadcast_rsp_processing_id_d = 1'b1;
      end else begin
        bcast_response_b_d = bcast_axi_b_q[0];
        broadcast_rsp_processing_id_d = 1'b0;
      end
    end

    if (!bcast_control_q[0].valid && bcast_perform_q[0]) begin
      bcast_aw_valid_d[shire_sbm_pkg::NeighRangeHi-1:shire_sbm_pkg::NeighRangeLo] =
          (bcast_fifo_not_empty[0] && (&axi_to_apb_ready[shire_sbm_pkg::NeighRangeHi-1:shire_sbm_pkg::NeighRangeLo])) ?
          {shire_sbm_pkg::NumNeigh{1'b1}} : '0;
    end

    if (!bcast_control_q[1].valid && bcast_perform_q[1]) begin
      bcast_aw_valid_d[shire_sbm_pkg::BankRangeHi-1:shire_sbm_pkg::BankRangeLo] =
          (bcast_fifo_not_empty[1] && (&axi_to_apb_ready[shire_sbm_pkg::BankRangeHi-1:shire_sbm_pkg::BankRangeLo])) ?
          {shire_sbm_pkg::NumBanks{1'b1}} : '0;
    end

    if (bcast_perform_q[0]) begin
      bcast_control_d[0].axi_id = bcast_axi_aw_pop[0].id;
      bcast_control_d[0].counter = '0;
      bcast_control_d[0].valid = 1'b1;
      bcast_control_d[0].error = 1'b0;
      bcast_active_d[shire_sbm_pkg::NeighRangeHi-1:shire_sbm_pkg::NeighRangeLo] = {shire_sbm_pkg::NumNeigh{1'b1}};
      bcast_neighs_valid_mask_d = '0;
    end

    if (bcast_control_q[0].valid && !broadcast_response_q[0]) begin
      if (|mesh_axi_b_valid[shire_sbm_pkg::NeighRangeHi-1:shire_sbm_pkg::NeighRangeLo]) begin
        bcast_neighs_valid_mask_d = bcast_neighs_valid_mask_aux;
        bcast_control_d[0].counter = bcast_control_q[0].counter + count_bvalid[0];
        bcast_control_d[0].error = bcast_control_q[0].error | error_search[0];
        if (({1'b0, bcast_control_q[0].counter} + {1'b0, count_bvalid[0]}) == 5'd4) begin
          bcast_control_d[0].valid = 1'b0;
          bcast_active_d[shire_sbm_pkg::NeighRangeHi-1:shire_sbm_pkg::NeighRangeLo] = '0;
          bcast_axi_b_d[0].id = bcast_control_q[0].axi_id;
          bcast_axi_b_d[0].resp = (bcast_control_q[0].error | error_search[0]) ? axi_pkg::RespSlvErr : axi_pkg::RespOkay;
          broadcast_response_d[0] = 1'b1;
        end
      end
    end

    if (bcast_perform_q[1]) begin
      bcast_control_d[1].axi_id = bcast_axi_aw_pop[1].id;
      bcast_control_d[1].counter = '0;
      bcast_control_d[1].valid = 1'b1;
      bcast_active_d[shire_sbm_pkg::BankRangeHi-1:shire_sbm_pkg::BankRangeLo] = {shire_sbm_pkg::NumBanks{1'b1}};
      bcast_control_d[1].error = 1'b0;
      bcast_banks_valid_mask_d = '0;
    end

    if (bcast_control_q[1].valid && !broadcast_response_q[1]) begin
      if (|mesh_axi_b_valid[shire_sbm_pkg::BankRangeHi-1:shire_sbm_pkg::BankRangeLo]) begin
        bcast_banks_valid_mask_d = bcast_banks_valid_mask_aux;
        bcast_control_d[1].counter = bcast_control_q[1].counter + count_bvalid[1];
        bcast_control_d[1].error = bcast_control_q[1].error | error_search[1];
        if (({1'b0, bcast_control_q[1].counter} + {1'b0, count_bvalid[1]}) == 5'd4) begin
          bcast_control_d[1].valid = 1'b0;
          bcast_active_d[shire_sbm_pkg::BankRangeHi-1:shire_sbm_pkg::BankRangeLo] = '0;
          bcast_axi_b_d[1].id = bcast_control_q[1].axi_id;
          bcast_axi_b_d[1].resp = (bcast_control_q[1].error | error_search[1]) ? axi_pkg::RespSlvErr : axi_pkg::RespOkay;
          broadcast_response_d[1] = 1'b1;
        end
      end
    end
  end

  for (genvar gen_it0 = 0; gen_it0 < 2; gen_it0++) begin : gen_sbm_broadcast_fifos
    prim_fifo_reg #(
      .Width (shire_sbm_pkg::AxiIdWidth + shire_sbm_pkg::AxiAddrWidth + ApbDataWidth),
      .Depth (SbmInterfaces)
    ) u_broadcast_fifo (
      .clk_i,
      .rst_ni,
      .push_i  (bcast_push[gen_it0]),
      .wdata_i ({sys_axi_aw_i.id, sys_axi_aw_i.addr, sys_axi_wdata}),
      .full_o  (bcast_fifo_full_unused[gen_it0]),
      .pop_i   (bcast_perform_q[gen_it0]),
      .valid_o (bcast_fifo_not_empty[gen_it0]),
      .rdata_o ({bcast_axi_aw_pop[gen_it0].id, bcast_axi_aw_pop[gen_it0].addr, bcast_axi_w_pop[gen_it0]}),
      .level_o (bcast_fifo_level_unused[gen_it0])
    );
  end

  logic decode_control_read_error;
  logic decode_control_write_error;
  axi_pkg::sys_slave_b_t decode_write_error_rsp_q;
  axi_pkg::sys_slave_r_t decode_read_error_rsp_q;
  axi_pkg::sys_slave_b_t decode_write_error_rsp_d;
  axi_pkg::sys_slave_r_t decode_read_error_rsp_d;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      sbm_buffer_q <= '0;
      possible_overflow_q <= 1'b0;
      error_return_q <= '0;
      decode_write_error_rsp_q <= '0;
      decode_read_error_rsp_q <= '0;
    end else begin
      sbm_buffer_q <= sbm_buffer_d;
      possible_overflow_q <= possible_overflow_d;
      error_return_q <= error_return_d;
      if (decode_control_write_error) begin
        decode_write_error_rsp_q <= decode_write_error_rsp_d;
      end
      if (decode_control_read_error) begin
        decode_read_error_rsp_q <= decode_read_error_rsp_d;
      end
    end
  end

  always_comb begin
    bcast_push = '0;
    sbm_fifo_req_push = '0;
    sbm_fifo_req_pop = '0;
    write_req_valid = '0;
    read_req_valid = '0;
    first_free_taken = 1'b0;
    free_pos = first_free_func(sbm_buffer_q);
    sbm_buffer_d = sbm_buffer_q;
    sbm_index_req_push = '0;
    decode_control_write_error = 1'b0;
    decode_control_read_error = 1'b0;
    decode_write_error_rsp_d = '0;
    decode_read_error_rsp_d = '0;

    if (input_aw_valid && sys_axi_aw_ready_o && !error_return_q[WriteIfIdx] && write_decode_error()) begin
      decode_control_write_error = 1'b1;
      decode_write_error_rsp_d.id = sys_axi_aw_i.id;
      decode_write_error_rsp_d.resp = axi_pkg::RespSlvErr;
    end

    if (input_ar_valid && sys_axi_ar_ready_o && !error_return_q[ReadIfIdx] && read_decode_error()) begin
      decode_control_read_error = 1'b1;
      decode_read_error_rsp_d.id = sys_axi_ar_i.id;
      decode_read_error_rsp_d.resp = axi_pkg::RespSlvErr;
      decode_read_error_rsp_d.last = 1'b1;
      decode_read_error_rsp_d.data = '0;
    end

    if (!decode_control_write_error && input_aw_valid && sys_axi_aw_ready_o &&
        !possible_overflow_q && is_neigh_broadcast(sys_axi_aw_i.addr[21:20], sys_axi_aw_i.addr[19:16])) begin
      bcast_push[0] = 1'b1;
    end

    for (int i = shire_sbm_pkg::NeighRangeLo; i < shire_sbm_pkg::NeighRangeHi; i++) begin
      sbm_fifo_req_push[i] = 1'b0;
      sbm_index_req_push[i] = '0;
      sbm_fifo_req_pop[i] = 1'b0;
      write_req_valid[i] = 1'b0;
      read_req_valid[i] = 1'b0;

      if ((bcast_aw_valid_q[i] || sbm_fifo_req_not_empty[i]) && axi_to_apb_ready[i] &&
          !bcast_push[0] && !bcast_fifo_not_empty[0]) begin
        if (!bcast_aw_valid_q[i]) begin
          sbm_fifo_req_pop[i] = 1'b1;
          sbm_buffer_d[sbm_index_req_pop[i]].reserved_pos = 1'b0;
          sbm_buffer_d[sbm_index_req_pop[i]].ar_valid = 1'b0;
          sbm_buffer_d[sbm_index_req_pop[i]].aw_valid = 1'b0;
        end

        if (!possible_overflow_q && (read_to_neigh(i) || write_to_neigh(i))) begin
          sbm_fifo_req_push[i] = 1'b1;
          sbm_index_req_push[i] = first_free_taken ? free_pos[1] : free_pos[0];
          first_free_taken = 1'b1;

          if (read_to_neigh(i)) begin
            sbm_buffer_d[sbm_index_req_push[i]].ar_valid = 1'b1;
            sbm_buffer_d[sbm_index_req_push[i]].ar = '{id: sys_axi_ar_i.id, addr: sys_axi_ar_i.addr};
            sbm_buffer_d[sbm_index_req_push[i]].reserved_pos = 1'b1;
          end else begin
            sbm_buffer_d[sbm_index_req_push[i]].ar_valid = 1'b0;
          end

          if (write_to_neigh(i)) begin
            sbm_buffer_d[sbm_index_req_push[i]].aw_valid = 1'b1;
            sbm_buffer_d[sbm_index_req_push[i]].aw = '{id: sys_axi_aw_i.id, addr: sys_axi_aw_i.addr};
            sbm_buffer_d[sbm_index_req_push[i]].data = sys_axi_wdata;
            sbm_buffer_d[sbm_index_req_push[i]].reserved_pos = 1'b1;
          end else begin
            sbm_buffer_d[sbm_index_req_push[i]].aw_valid = 1'b0;
          end
        end
      end else begin
        if (write_to_neigh(i)) begin
          if (!axi_to_apb_ready[i] || bcast_push[0] || bcast_fifo_not_empty[0]) begin
            sbm_fifo_req_push[i] = 1'b1;
            sbm_index_req_push[i] = first_free_taken ? free_pos[1] : free_pos[0];
            first_free_taken = 1'b1;
            sbm_buffer_d[sbm_index_req_push[i]].aw_valid = 1'b1;
            sbm_buffer_d[sbm_index_req_push[i]].aw = '{id: sys_axi_aw_i.id, addr: sys_axi_aw_i.addr};
            sbm_buffer_d[sbm_index_req_push[i]].data = sys_axi_wdata;
            sbm_buffer_d[sbm_index_req_push[i]].reserved_pos = 1'b1;
          end else begin
            write_req_valid[i] = 1'b1;
          end
        end

        if (read_to_neigh(i)) begin
          read_req_valid[i] = 1'b1;
          if (!axi_to_apb_ready[i] || bcast_push[0] || bcast_fifo_not_empty[0]) begin
            read_req_valid[i] = 1'b0;
            if (sbm_fifo_req_push[i]) begin
              sbm_buffer_d[sbm_index_req_push[i]].ar_valid = 1'b1;
              sbm_buffer_d[sbm_index_req_push[i]].ar = '{id: sys_axi_ar_i.id, addr: sys_axi_ar_i.addr};
              sbm_buffer_d[sbm_index_req_push[i]].reserved_pos = 1'b1;
            end else begin
              sbm_index_req_push[i] = first_free_taken ? free_pos[1] : free_pos[0];
              first_free_taken = 1'b1;
              sbm_buffer_d[sbm_index_req_push[i]].ar_valid = 1'b1;
              sbm_buffer_d[sbm_index_req_push[i]].ar = '{id: sys_axi_ar_i.id, addr: sys_axi_ar_i.addr};
              sbm_buffer_d[sbm_index_req_push[i]].reserved_pos = 1'b1;
            end
            sbm_fifo_req_push[i] = 1'b1;
          end
        end
      end
    end

    if (!decode_control_write_error && input_aw_valid && sys_axi_aw_ready_o && !possible_overflow_q &&
        is_bank_broadcast(sys_axi_aw_i.addr[21:20], sys_axi_aw_i.addr[19:17], sys_axi_aw_i.addr[16:13])) begin
      bcast_push[1] = 1'b1;
    end

    for (int l = shire_sbm_pkg::BankRangeLo; l < shire_sbm_pkg::BankRangeHi; l++) begin
      sbm_fifo_req_push[l] = 1'b0;
      sbm_index_req_push[l] = '0;
      sbm_fifo_req_pop[l] = 1'b0;
      write_req_valid[l] = 1'b0;
      read_req_valid[l] = 1'b0;

      if ((bcast_aw_valid_q[l] || sbm_fifo_req_not_empty[l]) && axi_to_apb_ready[l] &&
          !bcast_push[1] && !bcast_fifo_not_empty[1]) begin
        if (!bcast_aw_valid_q[l]) begin
          sbm_fifo_req_pop[l] = 1'b1;
          sbm_buffer_d[sbm_index_req_pop[l]].reserved_pos = 1'b0;
          sbm_buffer_d[sbm_index_req_pop[l]].ar_valid = 1'b0;
          sbm_buffer_d[sbm_index_req_pop[l]].aw_valid = 1'b0;
        end

        if (!possible_overflow_q && (read_to_bank(l) || write_to_bank(l))) begin
          sbm_fifo_req_push[l] = 1'b1;
          sbm_index_req_push[l] = first_free_taken ? free_pos[1] : free_pos[0];
          first_free_taken = 1'b1;

          if (read_to_bank(l)) begin
            sbm_buffer_d[sbm_index_req_push[l]].ar_valid = 1'b1;
            sbm_buffer_d[sbm_index_req_push[l]].ar = '{id: sys_axi_ar_i.id, addr: sys_axi_ar_i.addr};
            sbm_buffer_d[sbm_index_req_push[l]].reserved_pos = 1'b1;
          end else begin
            sbm_buffer_d[sbm_index_req_push[l]].ar_valid = 1'b0;
          end

          if (write_to_bank(l)) begin
            sbm_buffer_d[sbm_index_req_push[l]].aw_valid = 1'b1;
            sbm_buffer_d[sbm_index_req_push[l]].aw = '{id: sys_axi_aw_i.id, addr: sys_axi_aw_i.addr};
            sbm_buffer_d[sbm_index_req_push[l]].data = sys_axi_wdata;
            sbm_buffer_d[sbm_index_req_push[l]].reserved_pos = 1'b1;
          end else begin
            sbm_buffer_d[sbm_index_req_push[l]].aw_valid = 1'b0;
          end
        end
      end else begin
        if (write_to_bank(l)) begin
          if (!axi_to_apb_ready[l] || bcast_push[1] || bcast_fifo_not_empty[1]) begin
            sbm_fifo_req_push[l] = 1'b1;
            sbm_index_req_push[l] = first_free_taken ? free_pos[1] : free_pos[0];
            first_free_taken = 1'b1;
            sbm_buffer_d[sbm_index_req_push[l]].aw_valid = 1'b1;
            sbm_buffer_d[sbm_index_req_push[l]].aw = '{id: sys_axi_aw_i.id, addr: sys_axi_aw_i.addr};
            sbm_buffer_d[sbm_index_req_push[l]].data = sys_axi_wdata;
            sbm_buffer_d[sbm_index_req_push[l]].reserved_pos = 1'b1;
          end else begin
            write_req_valid[l] = 1'b1;
          end
        end

        if (read_to_bank(l)) begin
          read_req_valid[l] = 1'b1;
          if (!axi_to_apb_ready[l] || bcast_push[1] || bcast_fifo_not_empty[1] || write_to_bank(l)) begin
            read_req_valid[l] = 1'b0;
            if (sbm_fifo_req_push[l]) begin
              sbm_buffer_d[sbm_index_req_push[l]].ar_valid = 1'b1;
              sbm_buffer_d[sbm_index_req_push[l]].ar = '{id: sys_axi_ar_i.id, addr: sys_axi_ar_i.addr};
              sbm_buffer_d[sbm_index_req_push[l]].reserved_pos = 1'b1;
            end else begin
              sbm_index_req_push[l] = first_free_taken ? free_pos[1] : free_pos[0];
              first_free_taken = 1'b1;
              sbm_buffer_d[sbm_index_req_push[l]].ar_valid = 1'b1;
              sbm_buffer_d[sbm_index_req_push[l]].ar = '{id: sys_axi_ar_i.id, addr: sys_axi_ar_i.addr};
              sbm_buffer_d[sbm_index_req_push[l]].reserved_pos = 1'b1;
            end
            sbm_fifo_req_push[l] = 1'b1;
          end
        end
      end
    end

    handle_single_slave(shire_sbm_pkg::ShireRangeLo, 3'b010);
    handle_single_slave(shire_sbm_pkg::RboxRangeLo, 3'b001);
  end

  logic [SbmInterfaces-1:0][IndexWidth-1:0] counter_q;
  logic [SbmInterfaces-1:0][IndexWidth-1:0] counter_d;
  logic [SbmInterfaces-1:0][IndexWidth-1:0] credits_debt_read_q;
  logic [SbmInterfaces-1:0][IndexWidth-1:0] credits_debt_read_d;
  logic credits_debt_write_en;
  logic [IndexWidth-1:0] credits_to_release;
  logic [IndexWidth-1:0] credits_debt_write_q;
  logic [IndexWidth-1:0] credits_debt_write_d;
  logic [$clog2(MaxConcReq):0] sbm_buffer_count;

  logic write_response_arb_pop;
  logic read_response_arb_pop;
  logic write_rsp_fifo_pop;
  logic write_rsp_fifo_push;
  logic write_rsp_fifo_not_empty;
  logic write_rsp_fifo_full;
  axi_pkg::sys_slave_b_t write_rsp_fifo_pop_data;
  logic [SbmInterfaces:0] write_rsp_fifo_level_unused;
  logic read_rsp_fifo_pop;
  logic read_rsp_fifo_push;
  logic read_rsp_fifo_not_empty;
  logic read_rsp_fifo_full;
  axi_pkg::sys_slave_r_t read_rsp_fifo_pop_data;
  logic [SbmInterfaces:0] read_rsp_fifo_level_unused;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      counter_q <= '0;
      credits_debt_read_q <= '0;
      credits_debt_write_q <= '0;
    end else begin
      counter_q <= counter_d;
      credits_debt_read_q <= credits_debt_read_d;
      if (credits_debt_write_en) begin
        credits_debt_write_q <= credits_debt_write_d;
      end
    end
  end

  always_comb begin
    credit_return_write_o = 1'b0;
    credits_debt_read_d = credits_debt_read_q;
    credits_debt_write_en = 1'b0;
    credits_to_release = '0;
    possible_overflow_d = 1'b0;
    sbm_buffer_count = '0;

    for (int iter_req = 0; iter_req < MaxConcReq; iter_req++) begin
      sbm_buffer_count = sbm_buffer_count + sbm_buffer_d[iter_req].reserved_pos;
    end

    counter_d = counter_q;
    for (int iter = 0; iter < SbmInterfaces; iter++) begin
      if (sbm_fifo_req_push[iter] && !sbm_fifo_req_pop[iter]) begin
        counter_d[iter] = counter_q[iter] + {{(IndexWidth-1){1'b0}}, 1'b1};
      end
      if (sbm_fifo_req_pop[iter] && !sbm_fifo_req_push[iter]) begin
        counter_d[iter] = counter_q[iter] - {{(IndexWidth-1){1'b0}}, 1'b1};
      end

      if ((5'd16 <= ({1'b0, counter_q[iter]} + {4'b0, sbm_fifo_req_push[iter]} + 5'd2)) ||
          (5'd16 < (sbm_buffer_count + 5'd2)) || write_rsp_fifo_full || read_rsp_fifo_full) begin
        possible_overflow_d = 1'b1;
      end
    end

    if (input_aw_valid && sys_axi_aw_ready_o) begin
      credits_to_release = {{(IndexWidth-1){1'b0}}, 1'b1};
      credits_debt_write_en = 1'b1;
    end

    if (credits_debt_write_q != '0 && !possible_overflow_q) begin
      credit_return_write_o = 1'b1;
      credits_debt_write_en = 1'b1;
    end

    credits_debt_write_d = credits_debt_write_q - credit_return_write_o + credits_to_release;
  end

  localparam int unsigned WriteCreditCountWidth = $clog2(shire_sbm_pkg::WriteCredits) + 1;
  localparam int unsigned ReadCreditCountWidth  = $clog2(shire_sbm_pkg::ReadCredits) + 1;
  localparam logic [ReadCreditCountWidth-1:0] ReadCreditsInit = 4'd8;
  localparam logic [WriteCreditCountWidth-1:0] WriteCreditsLimit = 4'd8;

  logic [WriteCreditCountWidth-1:0] reserved_wr_pos_q;
  logic [WriteCreditCountWidth-1:0] reserved_wr_pos_d;
  logic [ReadCreditCountWidth-1:0] reserved_rd_pos_q;
  logic [ReadCreditCountWidth-1:0] reserved_rd_pos_d;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      reserved_wr_pos_q <= '0;
      reserved_rd_pos_q <= ReadCreditsInit;
    end else begin
      reserved_wr_pos_q <= reserved_wr_pos_d;
      reserved_rd_pos_q <= reserved_rd_pos_d;
    end
  end

  always_comb begin
    reserved_wr_pos_d = reserved_wr_pos_q;
    if ((input_aw_valid && sys_axi_aw_ready_o) && (sys_axi_b_valid_o && sys_axi_b_ready_i)) begin
      reserved_wr_pos_d = reserved_wr_pos_q;
    end else if (input_aw_valid && sys_axi_aw_ready_o) begin
      reserved_wr_pos_d = reserved_wr_pos_q + 1'b1;
    end else if (sys_axi_b_valid_o && sys_axi_b_ready_i) begin
      reserved_wr_pos_d = reserved_wr_pos_q - 1'b1;
    end

    reserved_rd_pos_d = reserved_rd_pos_q;
    if ((input_ar_valid && sys_axi_ar_ready_o) == (sys_axi_r_valid_o && sys_axi_r_ready_i)) begin
      reserved_rd_pos_d = reserved_rd_pos_q;
    end else if (input_ar_valid && sys_axi_ar_ready_o) begin
      reserved_rd_pos_d = reserved_rd_pos_q - 1'b1;
    end else if (sys_axi_r_valid_o && sys_axi_r_ready_i) begin
      reserved_rd_pos_d = reserved_rd_pos_q + 1'b1;
    end
  end

  assign sys_axi_ar_ready_o = (reserved_rd_pos_q > 0) && !possible_overflow_q && !(|error_return_q);
  assign sys_axi_aw_ready_o = !possible_overflow_q && !(|error_return_q) &&
                              (reserved_wr_pos_q < WriteCreditsLimit);
  assign sys_axi_w_ready_o  = !possible_overflow_q && !(|error_return_q) &&
                              (reserved_wr_pos_q < WriteCreditsLimit);

  axi_pkg::sys_slave_b_t axi_b_arb_grant;
  axi_pkg::sys_slave_r_t axi_r_arb_grant;

  assign write_rsp_fifo_push = !write_rsp_fifo_full &&
      (bcast_response_q || write_rsp_fifo_not_empty || error_return_q[WriteIfIdx] || !sys_axi_b_ready_i) &&
      ((|mesh_axi_b_ready) && write_response_arb_pop);
  assign write_rsp_fifo_pop = !error_return_q[WriteIfIdx] && !bcast_response_q &&
      write_rsp_fifo_not_empty && sys_axi_b_ready_i;

  assign read_rsp_fifo_push = !read_rsp_fifo_full &&
      (read_rsp_fifo_not_empty || error_return_q[ReadIfIdx] || !sys_axi_r_ready_i) &&
      ((|mesh_axi_r_ready) && read_response_arb_pop);
  assign read_rsp_fifo_pop = !error_return_q[ReadIfIdx] && read_rsp_fifo_not_empty && sys_axi_r_ready_i;

  always_comb begin
    error_return_d = error_return_q;
    if (!(|error_return_q) && (decode_control_read_error || decode_control_write_error)) begin
      error_return_d = {decode_control_write_error, decode_control_read_error};
    end

    if ((|error_return_q) && (sys_axi_r_ready_i || sys_axi_b_ready_i)) begin
      error_return_d = {error_return_q[WriteIfIdx] && !sys_axi_b_ready_i,
                        error_return_q[ReadIfIdx] && !sys_axi_r_ready_i};
    end
  end

  assign sys_axi_r_valid_o = error_return_q[ReadIfIdx] ? 1'b1 :
                             read_rsp_fifo_pop ? 1'b1 :
                             read_response_arb_pop ? (|mesh_axi_r_ready) : 1'b0;
  assign sys_axi_r_o = error_return_q[ReadIfIdx] ? decode_read_error_rsp_q :
                       read_rsp_fifo_pop ? read_rsp_fifo_pop_data : axi_r_arb_grant;

  assign sys_axi_b_valid_o = error_return_q[WriteIfIdx] ? 1'b1 :
                             bcast_response_q ? 1'b1 :
                             write_rsp_fifo_not_empty ? 1'b1 :
                             write_response_arb_pop ? (|mesh_axi_b_ready) : 1'b0;
  assign sys_axi_b_o = error_return_q[WriteIfIdx] ? decode_write_error_rsp_q :
                       bcast_response_q ? bcast_response_b_q :
                       write_rsp_fifo_pop ? write_rsp_fifo_pop_data : axi_b_arb_grant;

  assign read_response_arb_pop = !error_return_q[ReadIfIdx] && !read_rsp_fifo_full;

  prim_arb_rr #(
    .NumReqs (SbmInterfaces),
    .Width   ($bits(axi_pkg::sys_slave_r_t))
  ) u_read_response_axi_arb (
    .clk_i,
    .rst_ni,
    .reqs_i   (mesh_axi_r_valid),
    .pop_i    (read_response_arb_pop),
    .grants_o (mesh_axi_r_ready),
    .data_i   (mesh_axi_r),
    .data_o   (axi_r_arb_grant)
  );

  assign write_response_arb_pop = !error_return_q[WriteIfIdx] && (broadcast_response_q == '0) && !write_rsp_fifo_full;

  prim_arb_rr #(
    .NumReqs (SbmInterfaces),
    .Width   ($bits(axi_pkg::sys_slave_b_t))
  ) u_write_response_axi_arb (
    .clk_i,
    .rst_ni,
    .reqs_i   (bvalid_rsp_arb),
    .pop_i    (write_response_arb_pop),
    .grants_o (mesh_axi_b_ready),
    .data_i   (mesh_axi_b),
    .data_o   (axi_b_arb_grant)
  );

  prim_fifo_reg #(
    .Width ($bits(axi_pkg::sys_slave_b_t)),
    .Depth (SbmInterfaces)
  ) u_write_response_fifo (
    .clk_i,
    .rst_ni,
    .push_i  (write_rsp_fifo_push),
    .wdata_i (axi_b_arb_grant),
    .full_o  (write_rsp_fifo_full),
    .pop_i   (write_rsp_fifo_pop),
    .valid_o (write_rsp_fifo_not_empty),
    .rdata_o (write_rsp_fifo_pop_data),
    .level_o (write_rsp_fifo_level_unused)
  );

  prim_fifo_reg #(
    .Width ($bits(axi_pkg::sys_slave_r_t)),
    .Depth (SbmInterfaces)
  ) u_read_response_fifo (
    .clk_i,
    .rst_ni,
    .push_i  (read_rsp_fifo_push),
    .wdata_i (axi_r_arb_grant),
    .full_o  (read_rsp_fifo_full),
    .pop_i   (read_rsp_fifo_pop),
    .valid_o (read_rsp_fifo_not_empty),
    .rdata_o (read_rsp_fifo_pop_data),
    .level_o (read_rsp_fifo_level_unused)
  );

  logic [SbmInterfaces-1:0][shire_sbm_pkg::AxiAddrWidth-1:0] apb_paddr_uncoded;

  for (genvar gen_it1 = 0; gen_it1 < SbmInterfaces; gen_it1++) begin : gen_sbm_axi_to_apb
    always_comb begin
      apb_req_o[gen_it1].paddr = '0;
      if (gen_it1 < shire_sbm_pkg::NeighRangeHi) begin
        apb_req_o[gen_it1].paddr[16:0] = {
          apb_paddr_uncoded[gen_it1][21:20],
          apb_paddr_uncoded[gen_it1][31:30],
          apb_paddr_uncoded[gen_it1][15:3]
        };
      end else if (gen_it1 < shire_sbm_pkg::BankRangeHi) begin
        apb_req_o[gen_it1].paddr[16:0] = {
          5'b0,
          apb_paddr_uncoded[gen_it1][31:30],
          apb_paddr_uncoded[gen_it1][12:3]
        };
      end else begin
        apb_req_o[gen_it1].paddr[16:0] = {
          1'b0,
          apb_paddr_uncoded[gen_it1][31:30],
          apb_paddr_uncoded[gen_it1][16:3]
        };
      end
      apb_req_o[gen_it1].penable = apb_penable;
      apb_req_o[gen_it1].psel = apb_psel;
      apb_req_o[gen_it1].pwdata = apb_pwdata;
      apb_req_o[gen_it1].pwrite = apb_pwrite;

      mesh_axi_ar[gen_it1] = sbm_fifo_req_pop[gen_it1] ? sbm_buffer_q[sbm_index_req_pop[gen_it1]].ar :
          '{id: sys_axi_ar_i.id, addr: sys_axi_ar_i.addr};
      mesh_axi_ar_valid[gen_it1] = sbm_fifo_req_pop[gen_it1] ? sbm_buffer_q[sbm_index_req_pop[gen_it1]].ar_valid :
          read_req_valid[gen_it1];

      mesh_axi_aw[gen_it1] = bcast_aw_valid_q[gen_it1] ? bcast_axi_aw_q[(gen_it1 < shire_sbm_pkg::NeighRangeHi) ? 0 : 1] :
          (sbm_fifo_req_pop[gen_it1] ? sbm_buffer_q[sbm_index_req_pop[gen_it1]].aw :
           '{id: sys_axi_aw_i.id, addr: sys_axi_aw_i.addr});
      mesh_axi_aw_valid[gen_it1] = bcast_aw_valid_q[gen_it1] ? 1'b1 :
          (sbm_fifo_req_pop[gen_it1] ? sbm_buffer_q[sbm_index_req_pop[gen_it1]].aw_valid : write_req_valid[gen_it1]);
      mesh_axi_w[gen_it1].data = bcast_aw_valid_q[gen_it1] ? bcast_axi_w_q[(gen_it1 < shire_sbm_pkg::NeighRangeHi) ? 0 : 1] :
          (sbm_fifo_req_pop[gen_it1] ? sbm_buffer_q[sbm_index_req_pop[gen_it1]].data : sys_axi_wdata);
    end

    logic [shire_sbm_pkg::AxiAddrWidth-1:0] apb_paddr_raw;
    logic apb_penable;
    logic apb_psel;
    logic [ApbDataWidth-1:0] apb_pwdata;
    logic apb_pwrite;

    mshire_axi_to_apb #(
      .ApbAddrWidth  (shire_sbm_pkg::AxiAddrWidth),
      .ApbDataWidth  (ApbDataWidth),
      .ShiftDataEnRd (1'b1),
      .ResponseControl (1'b0)
    ) u_axi_to_apb (
      .clk_i,
      .rst_ni,
      .ready_o (axi_to_apb_ready[gen_it1]),
      .axi_ar_i (mesh_axi_ar[gen_it1]),
      .axi_ar_valid_i (mesh_axi_ar_valid[gen_it1]),
      .axi_aw_i (mesh_axi_aw[gen_it1]),
      .axi_aw_valid_i (mesh_axi_aw_valid[gen_it1]),
      .axi_w_i (mesh_axi_w[gen_it1]),
      .axi_b_o (mesh_axi_b[gen_it1]),
      .axi_b_valid_o (mesh_axi_b_valid[gen_it1]),
      .axi_b_ready_i (bcast_active_q[gen_it1] ? 1'b1 : (write_response_arb_pop && mesh_axi_b_ready[gen_it1])),
      .axi_r_o (mesh_axi_r[gen_it1]),
      .axi_r_valid_o (mesh_axi_r_valid[gen_it1]),
      .axi_r_ready_i (mesh_axi_r_ready[gen_it1] && read_response_arb_pop),
      .apb_paddr_o (apb_paddr_raw),
      .apb_penable_o (apb_penable),
      .apb_prdata_i (apb_rsp_i[gen_it1].prdata),
      .apb_pready_i (apb_rsp_i[gen_it1].pready),
      .apb_psel_o (apb_psel),
      .apb_pslverr_i (apb_rsp_i[gen_it1].pslverr),
      .apb_pwdata_o (apb_pwdata),
      .apb_pwrite_o (apb_pwrite)
    );

    assign apb_paddr_uncoded[gen_it1] = apb_paddr_raw;
  end

  function automatic logic [1:0][IndexWidth-1:0] first_free_func(
      input shire_sbm_pkg::req_buffer_array_t cbuffer);
    logic found1;
    logic found2;
    begin
      first_free_func = '0;
      found1 = 1'b0;
      found2 = 1'b0;
      for (int it = 0; it < MaxConcReq; it++) begin
        if ((!found1 || !found2) && !cbuffer[it].reserved_pos) begin
          if (!found1) begin
            first_free_func[0] = it[IndexWidth-1:0];
            found1 = 1'b1;
          end else begin
            first_free_func[1] = it[IndexWidth-1:0];
            found2 = 1'b1;
          end
        end
      end
    end
  endfunction

  function automatic logic is_neigh_broadcast(input logic [1:0] addr_class, input logic [3:0] neigh_id);
    is_neigh_broadcast = (addr_class == 2'b01) && (neigh_id == 4'hf);
  endfunction

  function automatic logic is_bank_broadcast(input logic [1:0] addr_class, input logic [2:0] block_id,
                                             input logic [3:0] bank_id);
    is_bank_broadcast = (addr_class == 2'b11) && (block_id == 3'b000) && (bank_id == 4'hf);
  endfunction

  function automatic logic write_decode_error;
    logic endpoint_reset;
    begin
      endpoint_reset = (!rst_neigh_ni[sys_axi_aw_i.addr[17:16]]) && (sys_axi_aw_i.addr[21] == 1'b0);
      write_decode_error = (sys_axi_aw_i.addr[39:33] != '0) ||
          (sys_axi_aw_i.addr[32] == 1'b0) ||
          ((sys_axi_aw_i.addr[21:20] == 2'b00) && (sys_axi_aw_i.addr[19:13] > LastThread)) ||
          ((sys_axi_aw_i.addr[21:20] == 2'b01) && (sys_axi_aw_i.addr[19:16] > LastNeigh) &&
           (sys_axi_aw_i.addr[19:16] != 4'hf)) ||
          ((sys_axi_aw_i.addr[21:20] == 2'b01) && (sys_axi_aw_i.addr[19:16] == 4'hf) && (|(~rst_neigh_ni))) ||
          endpoint_reset ||
          ((sys_axi_aw_i.addr[21:17] == 5'b11001) && !rst_rbox_ni) ||
          ((sys_axi_aw_i.addr[21:17] == 5'b11000) && !rst_shire_cache_ni) ||
          ((sys_axi_aw_i.addr[21:20] == 2'b11) &&
           (((sys_axi_aw_i.addr[19:17] == 3'b000) && (sys_axi_aw_i.addr[16:13] > LastBank) &&
             (sys_axi_aw_i.addr[16:13] != 4'hf)) || (sys_axi_aw_i.addr[19:17] > 3'b010))) ||
          (sys_axi_aw_i.addr[29:22] != shire_virtual_id_i[7:0]) ||
          (sys_axi_aw_i.len != 8'b0);
    end
  endfunction

  function automatic logic read_decode_error;
    logic endpoint_reset;
    begin
      endpoint_reset = (!rst_neigh_ni[sys_axi_ar_i.addr[17:16]]) && (sys_axi_ar_i.addr[21] == 1'b0);
      read_decode_error = (sys_axi_ar_i.addr[39:33] != '0) ||
          (sys_axi_ar_i.addr[32] == 1'b0) ||
          ((sys_axi_ar_i.addr[21:20] == 2'b00) && (sys_axi_ar_i.addr[19:13] > LastThread)) ||
          ((sys_axi_ar_i.addr[21:20] == 2'b01) && (sys_axi_ar_i.addr[19:16] > LastNeigh)) ||
          endpoint_reset ||
          ((sys_axi_ar_i.addr[21:17] == 5'b11000) && !rst_shire_cache_ni) ||
          ((sys_axi_ar_i.addr[21:17] == 5'b11001) && !rst_rbox_ni) ||
          ((sys_axi_ar_i.addr[21:20] == 2'b11) &&
           (((sys_axi_ar_i.addr[19:17] == 3'b000) && (sys_axi_ar_i.addr[16:13] > LastBank)) ||
            (sys_axi_ar_i.addr[19:17] > 3'b010))) ||
          (sys_axi_ar_i.addr[29:22] != shire_virtual_id_i[7:0]) ||
          (sys_axi_ar_i.len != 8'b0);
    end
  endfunction

  function automatic logic read_to_neigh(input int unsigned i);
    read_to_neigh = !decode_control_read_error && input_ar_valid && sys_axi_ar_ready_o &&
        (((sys_axi_ar_i.addr[21:20] == 2'b00) && (((i + 1) * shire_sbm_pkg::MinPerNeigh) > sys_axi_ar_i.addr[19:13]) &&
          ((i * shire_sbm_pkg::MinPerNeigh) <= sys_axi_ar_i.addr[19:13])) ||
         ((sys_axi_ar_i.addr[21:20] == 2'b01) && (i[3:0] == sys_axi_ar_i.addr[19:16])));
  endfunction

  function automatic logic write_to_neigh(input int unsigned i);
    write_to_neigh = !decode_control_write_error && input_aw_valid && sys_axi_aw_ready_o && !possible_overflow_q &&
        (((sys_axi_aw_i.addr[21:20] == 2'b00) && (((i + 1) * shire_sbm_pkg::MinPerNeigh) > sys_axi_aw_i.addr[19:13]) &&
          ((i * shire_sbm_pkg::MinPerNeigh) <= sys_axi_aw_i.addr[19:13])) ||
         ((sys_axi_aw_i.addr[21:20] == 2'b01) && (i[3:0] == sys_axi_aw_i.addr[19:16])));
  endfunction

  function automatic logic read_to_bank(input int unsigned l);
    read_to_bank = !decode_control_read_error && input_ar_valid && sys_axi_ar_ready_o &&
        (sys_axi_ar_i.addr[21:20] == 2'b11) && (sys_axi_ar_i.addr[19:17] == 3'b000) &&
        (l == (shire_sbm_pkg::BankRangeLo + int'(sys_axi_ar_i.addr[16:13])));
  endfunction

  function automatic logic write_to_bank(input int unsigned l);
    write_to_bank = !decode_control_write_error && input_aw_valid && sys_axi_aw_ready_o && !possible_overflow_q &&
        (sys_axi_aw_i.addr[21:20] == 2'b11) && (sys_axi_aw_i.addr[19:17] == 3'b000) &&
        (l == (shire_sbm_pkg::BankRangeLo + int'(sys_axi_aw_i.addr[16:13])));
  endfunction

  task automatic handle_single_slave(input int unsigned idx, input logic [2:0] block_id);
    begin
      sbm_fifo_req_push[idx] = 1'b0;
      sbm_index_req_push[idx] = '0;
      sbm_fifo_req_pop[idx] = 1'b0;
      write_req_valid[idx] = 1'b0;
      read_req_valid[idx] = 1'b0;

      if (sbm_fifo_req_not_empty[idx] && axi_to_apb_ready[idx]) begin
        sbm_fifo_req_pop[idx] = 1'b1;
        sbm_buffer_d[sbm_index_req_pop[idx]].reserved_pos = 1'b0;
        sbm_buffer_d[sbm_index_req_pop[idx]].ar_valid = 1'b0;
        sbm_buffer_d[sbm_index_req_pop[idx]].aw_valid = 1'b0;

        if (!possible_overflow_q && (read_to_single(block_id) || write_to_single(block_id))) begin
          sbm_fifo_req_push[idx] = 1'b1;
          sbm_index_req_push[idx] = first_free_taken ? free_pos[1] : free_pos[0];
          first_free_taken = 1'b1;

          if (read_to_single(block_id)) begin
            sbm_buffer_d[sbm_index_req_push[idx]].ar_valid = 1'b1;
            sbm_buffer_d[sbm_index_req_push[idx]].ar = '{id: sys_axi_ar_i.id, addr: sys_axi_ar_i.addr};
            sbm_buffer_d[sbm_index_req_push[idx]].reserved_pos = 1'b1;
          end else begin
            sbm_buffer_d[sbm_index_req_push[idx]].ar_valid = 1'b0;
          end

          if (write_to_single(block_id)) begin
            sbm_buffer_d[sbm_index_req_push[idx]].aw_valid = 1'b1;
            sbm_buffer_d[sbm_index_req_push[idx]].aw = '{id: sys_axi_aw_i.id, addr: sys_axi_aw_i.addr};
            sbm_buffer_d[sbm_index_req_push[idx]].data = sys_axi_wdata;
            sbm_buffer_d[sbm_index_req_push[idx]].reserved_pos = 1'b1;
          end else begin
            sbm_buffer_d[sbm_index_req_push[idx]].aw_valid = 1'b0;
          end
        end
      end else begin
        if (write_to_single(block_id)) begin
          if (!axi_to_apb_ready[idx]) begin
            sbm_fifo_req_push[idx] = 1'b1;
            sbm_index_req_push[idx] = first_free_taken ? free_pos[1] : free_pos[0];
            first_free_taken = 1'b1;
            sbm_buffer_d[sbm_index_req_push[idx]].aw_valid = 1'b1;
            sbm_buffer_d[sbm_index_req_push[idx]].aw = '{id: sys_axi_aw_i.id, addr: sys_axi_aw_i.addr};
            sbm_buffer_d[sbm_index_req_push[idx]].data = sys_axi_wdata;
            sbm_buffer_d[sbm_index_req_push[idx]].reserved_pos = 1'b1;
          end else begin
            write_req_valid[idx] = 1'b1;
          end
        end
        if (read_to_single(block_id)) begin
          read_req_valid[idx] = 1'b1;
          if (!axi_to_apb_ready[idx] || write_to_single(block_id)) begin
            read_req_valid[idx] = 1'b0;
            if (sbm_fifo_req_push[idx]) begin
              sbm_buffer_d[sbm_index_req_push[idx]].ar_valid = 1'b1;
              sbm_buffer_d[sbm_index_req_push[idx]].ar = '{id: sys_axi_ar_i.id, addr: sys_axi_ar_i.addr};
              sbm_buffer_d[sbm_index_req_push[idx]].reserved_pos = 1'b1;
            end else begin
              sbm_index_req_push[idx] = first_free_taken ? free_pos[1] : free_pos[0];
              first_free_taken = 1'b1;
              sbm_buffer_d[sbm_index_req_push[idx]].ar_valid = 1'b1;
              sbm_buffer_d[sbm_index_req_push[idx]].ar = '{id: sys_axi_ar_i.id, addr: sys_axi_ar_i.addr};
              sbm_buffer_d[sbm_index_req_push[idx]].reserved_pos = 1'b1;
            end
            sbm_fifo_req_push[idx] = 1'b1;
          end
        end
      end
    end
  endtask

  function automatic logic read_to_single(input logic [2:0] block_id);
    read_to_single = !decode_control_read_error && input_ar_valid && sys_axi_ar_ready_o && !possible_overflow_q &&
        (sys_axi_ar_i.addr[21:20] == 2'b11) && (sys_axi_ar_i.addr[19:17] == block_id);
  endfunction

  function automatic logic write_to_single(input logic [2:0] block_id);
    write_to_single = !decode_control_write_error && input_aw_valid && sys_axi_aw_ready_o && !possible_overflow_q &&
        (sys_axi_aw_i.addr[21:20] == 2'b11) && (sys_axi_aw_i.addr[19:17] == block_id);
  endfunction

  function automatic logic search_error_neigh(
      input logic [SbmInterfaces-1:0] valid,
      input axi_pkg::sys_slave_b_t [SbmInterfaces-1:0] data);
    begin
      search_error_neigh = 1'b0;
      for (int i = shire_sbm_pkg::NeighRangeLo; i < shire_sbm_pkg::NeighRangeHi; i++) begin
        if (valid[i] && (data[i].resp != axi_pkg::RespOkay)) begin
          search_error_neigh = 1'b1;
        end
      end
    end
  endfunction

  function automatic logic search_error_bank(
      input logic [SbmInterfaces-1:0] valid,
      input axi_pkg::sys_slave_b_t [SbmInterfaces-1:0] data);
    begin
      search_error_bank = 1'b0;
      for (int i = shire_sbm_pkg::BankRangeLo; i < shire_sbm_pkg::BankRangeHi; i++) begin
        if (valid[i] && (data[i].resp != axi_pkg::RespOkay)) begin
          search_error_bank = 1'b1;
        end
      end
    end
  endfunction

  function automatic logic [3:0] count_valids_neigh(
      input logic [shire_sbm_pkg::NumNeigh-1:0] vec_done_i,
      input logic [shire_sbm_pkg::AxiIdWidth-1:0] axi_id,
      input logic [SbmInterfaces-1:0] vector_valid,
      input axi_pkg::sys_slave_b_t [SbmInterfaces-1:0] vector_id,
      output logic [shire_sbm_pkg::NumNeigh-1:0] vec_done_o);
    begin
      count_valids_neigh = '0;
      vec_done_o = vec_done_i;
      for (int i = shire_sbm_pkg::NeighRangeLo; i < shire_sbm_pkg::NeighRangeHi; i++) begin
        if (vector_valid[i] && (vector_id[i].id == axi_id)) begin
          count_valids_neigh = count_valids_neigh + 4'd1;
          vec_done_o[i-shire_sbm_pkg::NeighRangeLo] = 1'b1;
        end
      end
    end
  endfunction

  function automatic logic [3:0] count_valids_bank(
      input logic [shire_sbm_pkg::NumBanks-1:0] vec_done_i,
      input logic [shire_sbm_pkg::AxiIdWidth-1:0] axi_id,
      input logic [SbmInterfaces-1:0] vector_valid,
      input axi_pkg::sys_slave_b_t [SbmInterfaces-1:0] vector_id,
      output logic [shire_sbm_pkg::NumBanks-1:0] vec_done_o);
    begin
      count_valids_bank = '0;
      vec_done_o = vec_done_i;
      for (int i = shire_sbm_pkg::BankRangeLo; i < shire_sbm_pkg::BankRangeHi; i++) begin
        if (vector_valid[i] && (vector_id[i].id == axi_id)) begin
          count_valids_bank = count_valids_bank + 4'd1;
          vec_done_o[i-shire_sbm_pkg::BankRangeLo] = 1'b1;
        end
      end
    end
  endfunction

endmodule : shire_bus_master
