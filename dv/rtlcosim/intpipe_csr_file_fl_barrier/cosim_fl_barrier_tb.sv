// Co-simulation testbench: intpipe_csr_file_fl_barrier (new) vs original.

`include "soc.vh"

module cosim_fl_barrier_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stimulus
  input  logic        stim_start_valid,
  input  logic [12:0] stim_start_data,
  input  logic [4:0]  stim_start_waddr,
  input  logic        stim_start_thread_id,
  input  logic        stim_neigh_resp_valid,
  input  logic        stim_neigh_resp_data,
  input  logic        stim_rf_wen_ready,

  // New outputs
  output logic        new_start_ready,
  output logic        new_neigh_req_valid,
  output logic [12:0] new_neigh_req_data,
  output logic        new_rf_wen_valid,
  output logic        new_rf_wen_valid_early,
  output logic        new_rf_wen_thread_id,
  output logic [4:0]  new_rf_wen_addr,
  output logic [63:0] new_rf_wen_data,
  output logic        new_scoreboard_valid,
  output logic        new_scoreboard_fp,
  output logic [4:0]  new_scoreboard_addr,
  output logic        new_scoreboard_thread_id,

  // Original outputs
  output logic        orig_start_ready,
  output logic        orig_neigh_req_valid,
  output logic [12:0] orig_neigh_req_data,
  output logic        orig_rf_wen_valid,
  output logic        orig_rf_wen_valid_early,
  output logic        orig_rf_wen_thread_id,
  output logic [4:0]  orig_rf_wen_addr,
  output logic [63:0] orig_rf_wen_data,
  output logic        orig_scoreboard_valid,
  output logic        orig_scoreboard_fp,
  output logic [4:0]  orig_scoreboard_addr,
  output logic        orig_scoreboard_thread_id
);

  import minion_pkg::*;

  // -- New module --
  minion_reg_dest_t new_scoreboard_addr_s;

  intpipe_csr_file_fl_barrier u_new (
    .clk_i               (clk_i),
    .rst_ni              (rst_ni),
    .start_ready         (new_start_ready),
    .start_valid         (stim_start_valid),
    .start_data          (stim_start_data),
    .start_waddr         (stim_start_waddr),
    .start_thread_id     (stim_start_thread_id),
    .neigh_req_valid     (new_neigh_req_valid),
    .neigh_req_data      (new_neigh_req_data),
    .neigh_resp_valid    (stim_neigh_resp_valid),
    .neigh_resp_data     (stim_neigh_resp_data),
    .rf_wen_ready        (stim_rf_wen_ready),
    .rf_wen_valid        (new_rf_wen_valid),
    .rf_wen_valid_early  (new_rf_wen_valid_early),
    .rf_wen_thread_id    (new_rf_wen_thread_id),
    .rf_wen_addr         (new_rf_wen_addr),
    .rf_wen_data         (new_rf_wen_data),
    .scoreboard_valid    (new_scoreboard_valid),
    .scoreboard_addr     (new_scoreboard_addr_s)
  );
  assign new_scoreboard_fp        = new_scoreboard_addr_s.fp;
  assign new_scoreboard_addr      = new_scoreboard_addr_s.addr;
  assign new_scoreboard_thread_id = new_scoreboard_addr_s.thread_id;

  // -- Original module --
  minion_reg_dest orig_scoreboard_addr_s;

  intpipe_csr_file_fl_barrier_orig u_orig (
    .clock              (clk_i),
    .reset              (~rst_ni),
    .start_ready        (orig_start_ready),
    .start_valid        (stim_start_valid),
    .start_data         (stim_start_data),
    .start_waddr        (stim_start_waddr),
    .start_thread_id    (stim_start_thread_id),
    .neigh_req_valid    (orig_neigh_req_valid),
    .neigh_req_data     (orig_neigh_req_data),
    .neigh_resp_valid   (stim_neigh_resp_valid),
    .neigh_resp_data    (stim_neigh_resp_data),
    .rf_wen_ready       (stim_rf_wen_ready),
    .rf_wen_valid       (orig_rf_wen_valid),
    .rf_wen_valid_early (orig_rf_wen_valid_early),
    .rf_wen_thread_id   (orig_rf_wen_thread_id),
    .rf_wen_addr        (orig_rf_wen_addr),
    .rf_wen_data        (orig_rf_wen_data),
    .scoreboard_valid   (orig_scoreboard_valid),
    .scoreboard_addr    (orig_scoreboard_addr_s)
  );
  assign orig_scoreboard_fp        = orig_scoreboard_addr_s.fp;
  assign orig_scoreboard_addr      = orig_scoreboard_addr_s.addr;
  assign orig_scoreboard_thread_id = orig_scoreboard_addr_s.thread_id;

endmodule
