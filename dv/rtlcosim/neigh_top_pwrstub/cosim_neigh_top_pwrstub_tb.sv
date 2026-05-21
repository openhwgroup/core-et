// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off WIDTHTRUNC */  // Deterministic salt patterns intentionally truncate to each translated field width.
/* verilator lint_off WIDTHEXPAND */  // Packed original vectors and translated structs are width-equivalent by construction.
module cosim_neigh_top_pwrstub_tb
  import esr_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,
  output logic [63:0] orig_chunks_o [8],
  output logic [63:0] new_chunks_o [8],
  output logic input_toggle_ok_o
);

  localparam int unsigned OutBits = 512;
  localparam int unsigned OutChunks = 8;
  localparam int unsigned DriveMinSalt = 0;
  localparam int unsigned DriveMaxSalt = 30;
  localparam int unsigned DriveSaltCount = DriveMaxSalt + 1;

  logic [31:0] cycle_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) cycle_q <= '0;
    else         cycle_q <= cycle_q + 32'd1;
  end

  function automatic logic drive_bit(input int unsigned salt);
    logic [31:0] x;
    x = cycle_q ^ (32'h9e3779b9 * (salt + 1));
    drive_bit = x[0] ^ x[3] ^ x[7] ^ x[13] ^ x[21];
  endfunction

  logic [DriveSaltCount-1:0] drive_seen_zero_q;
  logic [DriveSaltCount-1:0] drive_seen_one_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      drive_seen_zero_q <= '0;
      drive_seen_one_q <= '0;
    end else begin
      for (int unsigned salt = DriveMinSalt; salt <= DriveMaxSalt; salt++) begin
        if (drive_bit(salt)) drive_seen_one_q[salt] <= 1'b1;
        else                 drive_seen_zero_q[salt] <= 1'b1;
      end
    end
  end

  logic pwr_ctrl_glb_isolate;
  logic coop_slv_valid_in;
  logic [neigh_pkg::NumOtherNeigh-1:0] coop_mst_valid_in;
  APB_to_neigh_t orig_apb_req_in;
  APB_to_neigh_t orig_apb_req_out;
  APB_from_neigh_t orig_apb_rsp_in;
  APB_from_neigh_t orig_apb_rsp_out;
  neigh_ch_apb_mux_pkg::apb_to_neigh_t new_apb_req_in;
  neigh_ch_apb_mux_pkg::apb_to_neigh_t new_apb_req_out;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t new_apb_rsp_in;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t new_apb_rsp_out;
  logic [shirecache_pkg::Banks:0] req_ready_in;
  logic [shirecache_pkg::Banks:0] orig_req_ready_out;
  logic [shirecache_pkg::Banks:0] new_req_ready_out;
  logic [shirecache_pkg::Banks:0] req_valid_in;
  logic [shirecache_pkg::Banks:0] orig_req_valid_out;
  logic [shirecache_pkg::Banks:0] new_req_valid_out;
  logic rsp_ready_in;
  logic rsp_valid_in;
  logic orig_rsp_ready_out;
  logic orig_rsp_valid_out;
  logic new_rsp_ready_out;
  logic new_rsp_valid_out;
  logic prefetch_done_in;
  logic err_detected_in;
  logic err_logged_in;
  logic orig_prefetch_done_out;
  logic orig_err_detected_out;
  logic orig_err_logged_out;
  logic new_prefetch_done_out;
  logic new_err_detected_out;
  logic new_err_logged_out;
  logic icache_resp_valid_in;
  logic icache_resp_ready_in;
  logic orig_icache_resp_valid_out;
  logic orig_icache_resp_ready_out;
  logic new_icache_resp_valid_out;
  logic new_icache_resp_ready_out;
  esr_and_or_tree_L0_t orig_tree_in;
  esr_and_or_tree_L0_t orig_tree_out;
  esr_and_or_tree_l0_t new_tree_in;
  esr_and_or_tree_l0_t new_tree_out;
  logic flb_req_valid_in;
  logic flb_resp_valid_in;
  logic orig_flb_req_valid_out;
  logic orig_flb_resp_valid_out;
  logic new_flb_req_valid_out;
  logic new_flb_resp_valid_out;
  logic icache_req_write_in;
  logic icache_req_valid_in;
  logic icache_req_ready_in;
  logic orig_icache_req_write_out;
  logic orig_icache_req_valid_out;
  logic orig_icache_req_ready_out;
  logic new_icache_req_write_out;
  logic new_icache_req_valid_out;
  logic new_icache_req_ready_out;
  bpam_rc_tbox_ack_t orig_bpam_in;
  bpam_rc_tbox_ack_t orig_bpam_out;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t new_bpam_in;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t new_bpam_out;
  logic reset_w_icache_in;
  logic orig_reset_w_icache_out;
  logic new_reset_w_icache_out;
  logic [neigh_pkg::EtEcoNeighOutputWidth-1:0] eco_in;
  logic [neigh_pkg::EtEcoNeighOutputWidth-1:0] orig_eco_out;
  logic [neigh_pkg::EtEcoNeighOutputWidth-1:0] new_eco_out;
  logic pwr_glb_nsleepout_in;
  logic [neigh_pkg::MinPerN-1:0] pwr_min_nsleepout_in;
  logic orig_pwr_glb_nsleepout_out;
  logic [neigh_pkg::MinPerN-1:0] orig_pwr_min_nsleepout_out;
  logic new_pwr_glb_nsleepout_out;
  logic [neigh_pkg::MinPerN-1:0] new_pwr_min_nsleepout_out;
  logic orig_coop_slv_valid_out;
  logic [neigh_pkg::NumOtherNeigh-1:0] orig_coop_mst_valid_out;
  logic new_coop_slv_valid_out;
  logic [neigh_pkg::NumOtherNeigh-1:0] new_coop_mst_valid_out;

  always_comb begin
    pwr_ctrl_glb_isolate = drive_bit(0);
    coop_slv_valid_in = drive_bit(1);
    coop_mst_valid_in = drive_bit(2) ? '1 : '0;
    orig_apb_req_in = drive_bit(3) ? '1 : '0;
    orig_apb_req_in.apb_paddr = drive_bit(4) ? '1 : '0;
    orig_apb_req_in.apb_pwdata = drive_bit(5) ? '1 : '0;
    orig_apb_rsp_in = drive_bit(6) ? '1 : '0;
    req_ready_in = drive_bit(7) ? '1 : '0;
    req_valid_in = drive_bit(8) ? '1 : '0;
    rsp_ready_in = drive_bit(9);
    rsp_valid_in = drive_bit(10);
    prefetch_done_in = drive_bit(11);
    err_detected_in = drive_bit(12);
    err_logged_in = drive_bit(13);
    icache_resp_valid_in = drive_bit(14);
    icache_resp_ready_in = drive_bit(15);
    orig_tree_in = drive_bit(16) ? '1 : '0;
    flb_req_valid_in = drive_bit(17);
    flb_resp_valid_in = drive_bit(18);
    icache_req_write_in = drive_bit(19);
    icache_req_valid_in = drive_bit(20);
    icache_req_ready_in = drive_bit(21);
    orig_bpam_in = drive_bit(22) ? '1 : '0;
    reset_w_icache_in = drive_bit(23);
    eco_in = drive_bit(24) ? '1 : '0;
    pwr_glb_nsleepout_in = drive_bit(25);
    pwr_min_nsleepout_in = drive_bit(26) ? '1 : '0;

    new_apb_req_in = orig_apb_req_in;
    new_apb_rsp_in = orig_apb_rsp_in;
    new_tree_in = orig_tree_in;
    new_bpam_in = orig_bpam_in;
  end

  neigh_top_pwrstub_orig u_orig (
    .pwr_ctrl_glb_isolate(pwr_ctrl_glb_isolate),
    .coop_tload_slv_rdy_out_valid__pwrstub_in(coop_slv_valid_in),
    .coop_tload_slv_rdy_out_valid__pwrstub_out(orig_coop_slv_valid_out),
    .coop_tload_mst_done_out_valid__pwrstub_in(coop_mst_valid_in),
    .coop_tload_mst_done_out_valid__pwrstub_out(orig_coop_mst_valid_out),
    .APB_ESR_req__pwrstub_in(orig_apb_req_in),
    .APB_ESR_req__pwrstub_out(orig_apb_req_out),
    .APB_ESR_rsp__pwrstub_in(orig_apb_rsp_in),
    .APB_ESR_rsp__pwrstub_out(orig_apb_rsp_out),
    .neigh_sc_req_ready__pwrstub_in(req_ready_in),
    .neigh_sc_req_ready__pwrstub_out(orig_req_ready_out),
    .neigh_sc_req_valid__pwrstub_out(orig_req_valid_out),
    .neigh_sc_req_valid__pwrstub_in(req_valid_in),
    .neigh_sc_rsp_ready__pwrstub_in(rsp_ready_in),
    .neigh_sc_rsp_valid__pwrstub_in(rsp_valid_in),
    .neigh_sc_rsp_ready__pwrstub_out(orig_rsp_ready_out),
    .neigh_sc_rsp_valid__pwrstub_out(orig_rsp_valid_out),
    .esr_icache_prefetch_done__pwrstub_in(prefetch_done_in),
    .esr_icache_err_detected__pwrstub_in(err_detected_in),
    .esr_icache_err_logged__pwrstub_in(err_logged_in),
    .esr_icache_prefetch_done__pwrstub_out(orig_prefetch_done_out),
    .esr_icache_err_detected__pwrstub_out(orig_err_detected_out),
    .esr_icache_err_logged__pwrstub_out(orig_err_logged_out),
    .icache_f0_sram_resp_valid__pwrstub_in(icache_resp_valid_in),
    .icache_f0_sram_resp_ready__pwrstub_in(icache_resp_ready_in),
    .icache_f0_sram_resp_valid__pwrstub_out(orig_icache_resp_valid_out),
    .icache_f0_sram_resp_ready__pwrstub_out(orig_icache_resp_ready_out),
    .esr_and_or_tree_L0__pwrstub_in(orig_tree_in),
    .esr_and_or_tree_L0__pwrstub_out(orig_tree_out),
    .flb_neigh_l2_req_valid__pwrstub_in(flb_req_valid_in),
    .flb_l2_neigh_resp_valid__pwrstub_in(flb_resp_valid_in),
    .flb_neigh_l2_req_valid__pwrstub_out(orig_flb_req_valid_out),
    .flb_l2_neigh_resp_valid__pwrstub_out(orig_flb_resp_valid_out),
    .icache_f2_sram_req_write__pwrstub_in(icache_req_write_in),
    .icache_f2_sram_req_valid__pwrstub_in(icache_req_valid_in),
    .icache_f2_sram_req_ready__pwrstub_in(icache_req_ready_in),
    .icache_f2_sram_req_write__pwrstub_out(orig_icache_req_write_out),
    .icache_f2_sram_req_valid__pwrstub_out(orig_icache_req_valid_out),
    .icache_f2_sram_req_ready__pwrstub_out(orig_icache_req_ready_out),
    .bpam_rc_tbox_ack_hi__pwrstub_in(orig_bpam_in),
    .bpam_rc_tbox_ack_hi__pwrstub_out(orig_bpam_out),
    .reset_w_icache__pwrstub_in(reset_w_icache_in),
    .reset_w_icache__pwrstub_out(orig_reset_w_icache_out),
    .eco_o__pwrstub_in(eco_in),
    .eco_o__pwrstub_out(orig_eco_out),
    .pwr_ctrl_glb_nsleepout_in(pwr_glb_nsleepout_in),
    .pwr_ctrl_min_nsleepout_in(pwr_min_nsleepout_in),
    .pwr_ctrl_glb_nsleepout_out(orig_pwr_glb_nsleepout_out),
    .pwr_ctrl_min_nsleepout_out(orig_pwr_min_nsleepout_out)
  );

  neigh_top_pwrstub u_new (
    .pwr_ctrl_glb_isolate_i(pwr_ctrl_glb_isolate),
    .coop_tload_slv_rdy_out_valid_i(coop_slv_valid_in),
    .coop_tload_slv_rdy_out_valid_o(new_coop_slv_valid_out),
    .coop_tload_mst_done_out_valid_i(coop_mst_valid_in),
    .coop_tload_mst_done_out_valid_o(new_coop_mst_valid_out),
    .apb_esr_req_i(new_apb_req_in),
    .apb_esr_req_o(new_apb_req_out),
    .apb_esr_rsp_i(new_apb_rsp_in),
    .apb_esr_rsp_o(new_apb_rsp_out),
    .neigh_sc_req_ready_i(req_ready_in),
    .neigh_sc_req_ready_o(new_req_ready_out),
    .neigh_sc_req_valid_i(req_valid_in),
    .neigh_sc_req_valid_o(new_req_valid_out),
    .neigh_sc_rsp_ready_i(rsp_ready_in),
    .neigh_sc_rsp_valid_i(rsp_valid_in),
    .neigh_sc_rsp_ready_o(new_rsp_ready_out),
    .neigh_sc_rsp_valid_o(new_rsp_valid_out),
    .esr_icache_prefetch_done_i(prefetch_done_in),
    .esr_icache_err_detected_i(err_detected_in),
    .esr_icache_err_logged_i(err_logged_in),
    .esr_icache_prefetch_done_o(new_prefetch_done_out),
    .esr_icache_err_detected_o(new_err_detected_out),
    .esr_icache_err_logged_o(new_err_logged_out),
    .icache_f0_sram_resp_valid_i(icache_resp_valid_in),
    .icache_f0_sram_resp_ready_i(icache_resp_ready_in),
    .icache_f0_sram_resp_valid_o(new_icache_resp_valid_out),
    .icache_f0_sram_resp_ready_o(new_icache_resp_ready_out),
    .esr_and_or_tree_l0_i(new_tree_in),
    .esr_and_or_tree_l0_o(new_tree_out),
    .flb_neigh_l2_req_valid_i(flb_req_valid_in),
    .flb_l2_neigh_resp_valid_i(flb_resp_valid_in),
    .flb_neigh_l2_req_valid_o(new_flb_req_valid_out),
    .flb_l2_neigh_resp_valid_o(new_flb_resp_valid_out),
    .icache_f2_sram_req_write_i(icache_req_write_in),
    .icache_f2_sram_req_valid_i(icache_req_valid_in),
    .icache_f2_sram_req_ready_i(icache_req_ready_in),
    .icache_f2_sram_req_write_o(new_icache_req_write_out),
    .icache_f2_sram_req_valid_o(new_icache_req_valid_out),
    .icache_f2_sram_req_ready_o(new_icache_req_ready_out),
    .bpam_rc_tbox_ack_hi_i(new_bpam_in),
    .bpam_rc_tbox_ack_hi_o(new_bpam_out),
    .reset_w_icache_i(reset_w_icache_in),
    .reset_w_icache_o(new_reset_w_icache_out),
    .eco_o_i(eco_in),
    .eco_o_o(new_eco_out),
    .pwr_ctrl_glb_nsleepout_i(pwr_glb_nsleepout_in),
    .pwr_ctrl_min_nsleepout_i(pwr_min_nsleepout_in),
    .pwr_ctrl_glb_nsleepout_o(new_pwr_glb_nsleepout_out),
    .pwr_ctrl_min_nsleepout_o(new_pwr_min_nsleepout_out)
  );

  logic [OutBits-1:0] orig_flat;
  logic [OutBits-1:0] new_flat;
  always_comb begin
    orig_flat = '0;
    new_flat = '0;
    orig_flat[$bits({orig_coop_slv_valid_out, orig_coop_mst_valid_out, orig_apb_req_out,
                     orig_apb_rsp_out, orig_req_ready_out, orig_req_valid_out,
                     orig_rsp_ready_out, orig_rsp_valid_out, orig_prefetch_done_out,
                     orig_err_detected_out, orig_err_logged_out, orig_icache_resp_valid_out,
                     orig_icache_resp_ready_out, orig_tree_out, orig_flb_req_valid_out,
                     orig_flb_resp_valid_out, orig_icache_req_write_out,
                     orig_icache_req_valid_out, orig_icache_req_ready_out, orig_bpam_out,
                     orig_reset_w_icache_out, orig_eco_out, orig_pwr_glb_nsleepout_out,
                     orig_pwr_min_nsleepout_out})-1:0] =
        {orig_coop_slv_valid_out, orig_coop_mst_valid_out, orig_apb_req_out,
         orig_apb_rsp_out, orig_req_ready_out, orig_req_valid_out,
         orig_rsp_ready_out, orig_rsp_valid_out, orig_prefetch_done_out,
         orig_err_detected_out, orig_err_logged_out, orig_icache_resp_valid_out,
         orig_icache_resp_ready_out, orig_tree_out, orig_flb_req_valid_out,
         orig_flb_resp_valid_out, orig_icache_req_write_out,
         orig_icache_req_valid_out, orig_icache_req_ready_out, orig_bpam_out,
         orig_reset_w_icache_out, orig_eco_out, orig_pwr_glb_nsleepout_out,
         orig_pwr_min_nsleepout_out};
    new_flat[$bits({new_coop_slv_valid_out, new_coop_mst_valid_out, new_apb_req_out,
                    new_apb_rsp_out, new_req_ready_out, new_req_valid_out,
                    new_rsp_ready_out, new_rsp_valid_out, new_prefetch_done_out,
                    new_err_detected_out, new_err_logged_out, new_icache_resp_valid_out,
                    new_icache_resp_ready_out, new_tree_out, new_flb_req_valid_out,
                    new_flb_resp_valid_out, new_icache_req_write_out,
                    new_icache_req_valid_out, new_icache_req_ready_out, new_bpam_out,
                    new_reset_w_icache_out, new_eco_out, new_pwr_glb_nsleepout_out,
                    new_pwr_min_nsleepout_out})-1:0] =
        {new_coop_slv_valid_out, new_coop_mst_valid_out, new_apb_req_out,
         new_apb_rsp_out, new_req_ready_out, new_req_valid_out,
         new_rsp_ready_out, new_rsp_valid_out, new_prefetch_done_out,
         new_err_detected_out, new_err_logged_out, new_icache_resp_valid_out,
         new_icache_resp_ready_out, new_tree_out, new_flb_req_valid_out,
         new_flb_resp_valid_out, new_icache_req_write_out,
         new_icache_req_valid_out, new_icache_req_ready_out, new_bpam_out,
         new_reset_w_icache_out, new_eco_out, new_pwr_glb_nsleepout_out,
         new_pwr_min_nsleepout_out};
  end

  for (genvar chunk = 0; chunk < OutChunks; chunk++) begin : gen_chunks
    assign orig_chunks_o[chunk] = orig_flat[chunk*64 +: 64];
    assign new_chunks_o[chunk] = new_flat[chunk*64 +: 64];
  end

  assign input_toggle_ok_o = (cycle_q > 32'd128) &&
                             (&drive_seen_zero_q[DriveMaxSalt:DriveMinSalt]) &&
                             (&drive_seen_one_q[DriveMaxSalt:DriveMinSalt]);

endmodule : cosim_neigh_top_pwrstub_tb
