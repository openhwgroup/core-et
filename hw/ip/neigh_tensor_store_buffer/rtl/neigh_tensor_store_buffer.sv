// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_tensor_store_buffer (
  input  logic                                     clk_i,
  input  logic                                     rst_ni,

  input  etlink_pkg::neigh_req_t [neigh_pkg::MinPerN-1:0] evict_req_i,
  input  logic [neigh_pkg::MinPerN-1:0]           evict_valid_i,
  output logic [neigh_pkg::MinPerN-1:0]           evict_ready_o,

  input  logic                                     evict_out_ready_i,
  input  logic                                     evict_out_disable_next_i,
  output logic                                     evict_out_valid_o,
  output etlink_pkg::neigh_req_t                   evict_out_req_o,

  input  logic [neigh_pkg::MinPerN-1:0]           tstore_min_ack_i,

  input  logic [neigh_pkg::MinPerN-1:0]           tstore_min_resp_ready_i,
  output logic [neigh_pkg::MinPerN-1:0]           tstore_min_resp_valid_o,
  output minion_pkg::et_link_minion_rsp_info_t [neigh_pkg::MinPerN-1:0] tstore_min_resp_o,

  output neigh_pkg::tstore_dbg_sm_t                dbg_sm_signals_o
);

  localparam int unsigned NumMinions = neigh_pkg::MinPerN;
  localparam int unsigned HalfMinions = NumMinions / 2;
  localparam int unsigned ReqWidth = $bits(etlink_pkg::neigh_req_t);

  logic evict_out_ready_north;
  logic evict_out_valid_north;
  logic evict_out_valid_1st_north;
  logic evict_out_valid_2nd_north;
  etlink_pkg::neigh_req_t evict_out_req_north;

  logic evict_out_ready_south;
  logic evict_out_valid_south;
  logic evict_out_valid_1st_south;
  logic evict_out_valid_2nd_south;
  etlink_pkg::neigh_req_t evict_out_req_south;

  logic evict_out_disable_q;
  logic evict_out_disable_d;

  neigh_pkg::tstore_block_dbg_sm_t dbg_sm_signals_ts_buffer_north;
  neigh_pkg::tstore_block_dbg_sm_t dbg_sm_signals_ts_buffer_south;

  neigh_tensor_store_buffer_block #(
    .NumMinions   (HalfMinions),
    .SourceOffset (0)
  ) u_neigh_tensor_store_buffer_north (
    .clk_i,
    .rst_ni,
    .evict_req_i              (evict_req_i[HalfMinions-1:0]),
    .evict_valid_i            (evict_valid_i[HalfMinions-1:0]),
    .evict_ready_o            (evict_ready_o[HalfMinions-1:0]),
    .evict_out_ready_i        (evict_out_ready_north),
    .evict_out_disable_next_i (1'b0),
    .evict_out_valid_o        (evict_out_valid_north),
    .evict_out_valid_1st_o    (evict_out_valid_1st_north),
    .evict_out_valid_2nd_o    (evict_out_valid_2nd_north),
    .evict_out_req_o          (evict_out_req_north),
    .tstore_min_ack_i         (tstore_min_ack_i[HalfMinions-1:0]),
    .tstore_min_resp_ready_i  (tstore_min_resp_ready_i[HalfMinions-1:0]),
    .tstore_min_resp_valid_o  (tstore_min_resp_valid_o[HalfMinions-1:0]),
    .tstore_min_resp_o        (tstore_min_resp_o[HalfMinions-1:0]),
    .dbg_sm_signals_o         (dbg_sm_signals_ts_buffer_north)
  );

  neigh_tensor_store_buffer_block #(
    .NumMinions   (HalfMinions),
    .SourceOffset (HalfMinions)
  ) u_neigh_tensor_store_buffer_south (
    .clk_i,
    .rst_ni,
    .evict_req_i              (evict_req_i[HalfMinions +: HalfMinions]),
    .evict_valid_i            (evict_valid_i[HalfMinions +: HalfMinions]),
    .evict_ready_o            (evict_ready_o[HalfMinions +: HalfMinions]),
    .evict_out_ready_i        (evict_out_ready_south),
    .evict_out_disable_next_i (1'b0),
    .evict_out_valid_o        (evict_out_valid_south),
    .evict_out_valid_1st_o    (evict_out_valid_1st_south),
    .evict_out_valid_2nd_o    (evict_out_valid_2nd_south),
    .evict_out_req_o          (evict_out_req_south),
    .tstore_min_ack_i         (tstore_min_ack_i[HalfMinions +: HalfMinions]),
    .tstore_min_resp_ready_i  (tstore_min_resp_ready_i[HalfMinions +: HalfMinions]),
    .tstore_min_resp_valid_o  (tstore_min_resp_valid_o[HalfMinions +: HalfMinions]),
    .tstore_min_resp_o        (tstore_min_resp_o[HalfMinions +: HalfMinions]),
    .dbg_sm_signals_o         (dbg_sm_signals_ts_buffer_south)
  );

  logic fifo_write;
  logic fifo_pop;
  logic fifo_valid;
  logic fifo_full;
  etlink_pkg::neigh_req_t fifo_data_in;
  etlink_pkg::neigh_req_t fifo_data_out;

  /* verilator lint_off PINCONNECTEMPTY */  // full_next/valid_next are unused by this exact original FIFO site.
  prim_fifo #(
    .Width      (ReqWidth),
    .Depth      (neigh_pkg::TstoreReqFifoDepth),
    .UseLatches (1'b0)
  ) u_tensor_store_req_fifo (
    .clk_i,
    .rst_ni,
    .push_early_i (1'b0),
    .push_i       (fifo_write),
    .push_data_i  (fifo_data_in),
    .full_o       (fifo_full),
    .full_next_o  (),
    .pop_i        (fifo_pop),
    .pop_data_o   (fifo_data_out),
    .valid_o      (fifo_valid),
    .valid_next_o ()
  );
  /* verilator lint_on PINCONNECTEMPTY */

  logic grant_north_q;
  logic grant_north_d;

  always_comb begin
    grant_north_d = (grant_north_q && evict_out_valid_north && evict_out_valid_1st_north &&
                     evict_out_ready_north) ? 1'b1 :
                    (grant_north_q && evict_out_valid_north && evict_out_valid_2nd_north &&
                     !evict_out_ready_north) ? 1'b1 :
                    (grant_north_q && evict_out_valid_south &&
                     (!evict_out_valid_north || evict_out_ready_north)) ? 1'b0 :
                    (!grant_north_q && evict_out_valid_south && evict_out_valid_1st_south &&
                     evict_out_ready_south) ? 1'b0 :
                    (!grant_north_q && evict_out_valid_south && evict_out_valid_2nd_south &&
                     !evict_out_ready_south) ? 1'b0 :
                    (!grant_north_q && evict_out_valid_north &&
                     (!evict_out_valid_south || evict_out_ready_south)) ? 1'b1 :
                    grant_north_q;

    fifo_write = !fifo_full && ((grant_north_q && evict_out_valid_north) ||
                                (!grant_north_q && evict_out_valid_south));
    fifo_data_in = grant_north_q ? evict_out_req_north : evict_out_req_south;
  end

  always_comb begin
    evict_out_ready_north = grant_north_q && !fifo_full;
    evict_out_ready_south = !grant_north_q && !fifo_full;
  end

  assign evict_out_disable_d = evict_out_disable_next_i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      grant_north_q <= 1'b1;
      evict_out_disable_q <= 1'b0;
    end else begin
      grant_north_q <= grant_north_d;
      evict_out_disable_q <= evict_out_disable_d;
    end
  end

  always_comb begin
    evict_out_valid_o = fifo_valid && !evict_out_disable_q;
    evict_out_req_o = fifo_data_out;
    fifo_pop = fifo_valid && evict_out_ready_i;
  end

  always_comb begin
    dbg_sm_signals_o.tstore_north_dbg_sm_signals = dbg_sm_signals_ts_buffer_north;
    dbg_sm_signals_o.tstore_south_dbg_sm_signals = dbg_sm_signals_ts_buffer_south;
  end

endmodule : neigh_tensor_store_buffer
