// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_csr_file_fl_barrier — Fast Local Barrier unit.
//
// When a new fast local barrier starts, the unit gets busy and no other
// barrier is accepted. It sends the request to the neigh, which eventually
// returns the result of the barrier. Then it is written to the RF and the
// unit finishes and is available again.
//
// Replaces: intpipe_csr_file_fl_barrier (etcore-soc)

module intpipe_csr_file_fl_barrier
  import minion_pkg::*;
(
  input  logic                             clk_i,
  input  logic                             rst_ni,

  // Trigger a new barrier
  output logic                             start_ready,
  input  logic                             start_valid,
  input  logic [CsrFlBarrierSize-1:0]      start_data,
  input  logic [XregAddrSize-1:0]          start_waddr,
  input  logic                             start_thread_id,

  // Request interface with the neigh
  output logic                             neigh_req_valid,
  output logic [CsrFlBarrierSize-1:0]      neigh_req_data,

  // Response interface with the neigh
  input  logic                             neigh_resp_valid,
  input  logic                             neigh_resp_data,

  // Interface with the RF write
  input  logic                             rf_wen_ready,
  output logic                             rf_wen_valid,
  output logic                             rf_wen_valid_early,
  output logic                             rf_wen_thread_id,
  output logic [XregAddrSize-1:0]          rf_wen_addr,
  output logic [XregSize-1:0]              rf_wen_data,

  // Interface with the INT scoreboard
  output logic                             scoreboard_valid,
  output minion_reg_dest_t                 scoreboard_addr
);

  // FSM state
  typedef enum logic [1:0] {
    FLB_Invalid   = 2'b00,
    FLB_Send_Req  = 2'b01,
    FLB_Wait_Req  = 2'b10,
    FLB_Write_Res = 2'b11
  } fl_barrier_state_e;

  fl_barrier_state_e state_q, state_d;
  logic              clk_en;
  logic              start_accepted;
  logic              rf_wen_data_r;

  // State register
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      state_q <= FLB_Invalid;
    else if (clk_en)
      state_q <= state_d;
  end

  // neigh_req_data register
  always_ff @(posedge clk_i) begin
    if (start_accepted)
      neigh_req_data <= start_data;
  end

  // rf_wen_addr register
  always_ff @(posedge clk_i) begin
    if (start_accepted)
      rf_wen_addr <= start_waddr;
  end

  // rf_wen_thread_id register
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      rf_wen_thread_id <= 1'b0;
    else if (start_accepted)
      rf_wen_thread_id <= start_thread_id;
  end

  // rf_wen_data_r register
  always_ff @(posedge clk_i) begin
    if (neigh_resp_valid)
      rf_wen_data_r <= neigh_resp_data;
  end

  assign rf_wen_data = {{(XregSize-1){1'b0}}, rf_wen_data_r};

  // Next state computation
  always_comb begin
    state_d = state_q;
    if ((state_q == FLB_Invalid)   && start_valid)      state_d = FLB_Send_Req;
    if  (state_q == FLB_Send_Req)                       state_d = FLB_Wait_Req;
    if ((state_q == FLB_Wait_Req)  && neigh_resp_valid)  state_d = FLB_Write_Res;
    if ((state_q == FLB_Write_Res) && rf_wen_ready)      state_d = FLB_Invalid;

    clk_en = start_valid || (state_q != FLB_Invalid);
  end

  // Output computation based on state
  always_comb begin
    // New request
    start_ready    = (state_q == FLB_Invalid);
    start_accepted = start_ready && start_valid;

    // Send request
    neigh_req_valid = (state_q == FLB_Send_Req);

    // Write
    rf_wen_valid       = (state_q == FLB_Write_Res);
    rf_wen_valid_early = rf_wen_valid || neigh_resp_valid;

    // Scoreboard
    scoreboard_valid          = (state_q != FLB_Invalid);
    scoreboard_addr.fp        = 1'b0;
    scoreboard_addr.addr      = rf_wen_addr;
    scoreboard_addr.thread_id = rf_wen_thread_id;
  end

endmodule
