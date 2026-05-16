// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Level-shifter wrapper for the neighborhood cooperative TLoad sideband buses.
// The original inserts explicit voltage-level cells around one flop stage; this
// technology-neutral translation preserves the flop timing and models the level
// shifters as direct combinational connections.
module neigh_coop_tload_ports (
  input  logic clk_i,
  input  logic rst_ni,

  output neigh_pkg::coop_tload_slv_rdy_req_t                         coop_tload_slv_rdy_out_data_o,
  output logic                                                        coop_tload_slv_rdy_out_valid_o,

  input  neigh_pkg::coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] coop_tload_slv_rdy_in_data_i,
  input  logic [neigh_pkg::NumOtherNeigh-1:0]                         coop_tload_slv_rdy_in_valid_i,

  output logic [neigh_pkg::ShireCoopIdSize-1:0]                       coop_tload_mst_done_out_coop_id_o,
  output logic [neigh_pkg::NumOtherNeigh-1:0]                         coop_tload_mst_done_out_valid_o,

  input  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id_i,
  input  logic [neigh_pkg::NumOtherNeigh-1:0]                         coop_tload_mst_done_in_valid_i,

  input  neigh_pkg::coop_tload_slv_rdy_req_t                         coop_tload_slv_rdy_out_data_lv_i,
  input  logic                                                        coop_tload_slv_rdy_out_valid_lv_i,

  output neigh_pkg::coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] coop_tload_slv_rdy_in_data_lv_o,
  output logic [neigh_pkg::NumOtherNeigh-1:0]                         coop_tload_slv_rdy_in_valid_lv_o,

  input  logic [neigh_pkg::ShireCoopIdSize-1:0]                       coop_tload_mst_done_out_coop_id_lv_i,
  input  logic [neigh_pkg::NumOtherNeigh-1:0]                         coop_tload_mst_done_out_valid_lv_i,

  output logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id_lv_o,
  output logic [neigh_pkg::NumOtherNeigh-1:0]                         coop_tload_mst_done_in_valid_lv_o
);

  neigh_pkg::coop_tload_slv_rdy_req_t                         coop_tload_slv_rdy_out_data_hv;
  logic                                                        coop_tload_slv_rdy_out_valid_hv;
  neigh_pkg::coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] coop_tload_slv_rdy_in_data_hv;
  logic [neigh_pkg::NumOtherNeigh-1:0]                         coop_tload_slv_rdy_in_valid_hv;
  logic [neigh_pkg::ShireCoopIdSize-1:0]                       coop_tload_mst_done_out_coop_id_hv;
  logic [neigh_pkg::NumOtherNeigh-1:0]                         coop_tload_mst_done_out_valid_hv;
  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id_hv;
  logic [neigh_pkg::NumOtherNeigh-1:0]                         coop_tload_mst_done_in_valid_hv;

  assign coop_tload_slv_rdy_out_data_hv      = coop_tload_slv_rdy_out_data_lv_i;
  assign coop_tload_slv_rdy_out_valid_hv     = coop_tload_slv_rdy_out_valid_lv_i;
  assign coop_tload_mst_done_out_coop_id_hv  = coop_tload_mst_done_out_coop_id_lv_i;
  assign coop_tload_mst_done_out_valid_hv    = coop_tload_mst_done_out_valid_lv_i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      coop_tload_slv_rdy_out_valid_o  <= 1'b0;
      coop_tload_mst_done_out_valid_o <= '0;
    end else begin
      coop_tload_slv_rdy_out_valid_o  <= coop_tload_slv_rdy_out_valid_hv;
      coop_tload_mst_done_out_valid_o <= coop_tload_mst_done_out_valid_hv;
    end
  end

  // Original EN_FF storage has no reset; preserve the unreset data registers.
  always_ff @(posedge clk_i) begin
    if (coop_tload_slv_rdy_out_valid_hv) begin
      coop_tload_slv_rdy_out_data_o <= coop_tload_slv_rdy_out_data_hv;
    end
    if (|coop_tload_mst_done_out_valid_hv) begin
      coop_tload_mst_done_out_coop_id_o <= coop_tload_mst_done_out_coop_id_hv;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      coop_tload_slv_rdy_in_valid_hv  <= '0;
      coop_tload_mst_done_in_valid_hv <= '0;
    end else begin
      coop_tload_slv_rdy_in_valid_hv  <= coop_tload_slv_rdy_in_valid_i;
      coop_tload_mst_done_in_valid_hv <= coop_tload_mst_done_in_valid_i;
    end
  end

  // Original EN_FF storage has no reset; preserve the unreset data registers.
  always_ff @(posedge clk_i) begin
    if (|coop_tload_slv_rdy_in_valid_i) begin
      coop_tload_slv_rdy_in_data_hv <= coop_tload_slv_rdy_in_data_i;
    end
    if (|coop_tload_mst_done_in_valid_i) begin
      coop_tload_mst_done_in_coop_id_hv <= coop_tload_mst_done_in_coop_id_i;
    end
  end

  assign coop_tload_slv_rdy_in_data_lv_o      = coop_tload_slv_rdy_in_data_hv;
  assign coop_tload_slv_rdy_in_valid_lv_o     = coop_tload_slv_rdy_in_valid_hv;
  assign coop_tload_mst_done_in_coop_id_lv_o  = coop_tload_mst_done_in_coop_id_hv;
  assign coop_tload_mst_done_in_valid_lv_o    = coop_tload_mst_done_in_valid_hv;

endmodule : neigh_coop_tload_ports
