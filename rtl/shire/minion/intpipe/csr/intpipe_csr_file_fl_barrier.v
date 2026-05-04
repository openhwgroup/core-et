// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module intpipe_csr_file_fl_barrier
(
    // System signals
    input  logic                         clock,
    input  logic                         reset,
    // Trigger a new barrier
    output logic                         start_ready,
    input  logic                         start_valid,
    input  logic [`CSR_FL_BARRIER_RANGE] start_data,
    input  logic [`XREG_ADDR_RANGE]      start_waddr,
    input  logic                         start_thread_id,
    // Request interface with the neigh
    output logic                         neigh_req_valid,
    output logic [`CSR_FL_BARRIER_RANGE] neigh_req_data,
    // Response interface with the neigh
    input  logic                         neigh_resp_valid,
    input  logic                         neigh_resp_data,
    // Interface with the RF write
    input  logic                         rf_wen_ready,
    output logic                         rf_wen_valid,
    output logic                         rf_wen_valid_early,
    output logic                         rf_wen_thread_id,
    output logic [`XREG_ADDR_RANGE]      rf_wen_addr,
    output logic [`XREG_RANGE]           rf_wen_data,
    // Interface with the INT scoreboard
    output logic                         scoreboard_valid,
    output minion_reg_dest               scoreboard_addr
);

////////////////////////////////////////////////////////////////////////////////
// When a new fast local barrier starts, the unit gets busy and no other
// barrier is accepted. It sends the request to the neigh, which eventually
// returns the result of the barrier. Then it is written to the RF and the
// unit finishes and is available again.
////////////////////////////////////////////////////////////////////////////////

// Miss Handler FSM state
typedef enum logic [1:0]
{
    FLB_Invalid   = 2'b00, // Idle
    FLB_Send_Req  = 2'b01, // Sending request to neigh
    FLB_Wait_Req  = 2'b10, // Waiting for response from neigh
    FLB_Write_Res = 2'b11  // Writing result to RF
} fl_barrier_state;

fl_barrier_state state, state_next; // The unit state
logic            clk_en;            // Global clock enable of the unit
logic            start_accepted;    // New request accepted
logic            rf_wen_data_r;
//         CLK    RST    EN                DOUT              DIN                 DEF
`RST_EN_FF(clock, reset, clk_en,           state,            state_next,         FLB_Invalid)
`EN_FF    (clock,        start_accepted,   neigh_req_data,   start_data                     )
`EN_FF    (clock,        start_accepted,   rf_wen_addr,      start_waddr                    )
`RST_EN_FF(clock, reset, start_accepted,   rf_wen_thread_id, start_thread_id,    1'b0       )
`EN_FF    (clock,        neigh_resp_valid, rf_wen_data_r,    neigh_resp_data                )
assign rf_wen_data = `ZX(`XREG_SIZE, rf_wen_data_r);
   
// Next state computation
always_comb
begin
    state_next = state;
    if((state == FLB_Invalid)   && start_valid)      state_next = FLB_Send_Req;
    if(state == FLB_Send_Req)                        state_next = FLB_Wait_Req;
    if((state == FLB_Wait_Req)  && neigh_resp_valid) state_next = FLB_Write_Res;
    if((state == FLB_Write_Res) && rf_wen_ready)     state_next = FLB_Invalid;

    clk_en = start_valid || (state != FLB_Invalid);
end

// Output computation based on state
always_comb
begin
    // New request
    start_ready    = (state == FLB_Invalid);
    start_accepted = start_ready && start_valid;

    // Send request
    neigh_req_valid = (state == FLB_Send_Req);

    // Write
    rf_wen_valid       = (state == FLB_Write_Res);
    rf_wen_valid_early = rf_wen_valid || neigh_resp_valid;

    // Scoreboard
    scoreboard_valid          = (state != FLB_Invalid);
    scoreboard_addr.fp        = 1'b0;
    scoreboard_addr.addr      = rf_wen_addr;
    scoreboard_addr.thread_id = rf_wen_thread_id;
end

endmodule

