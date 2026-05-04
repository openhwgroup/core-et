// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_frontend — deterministic UART trace/demo for minion_frontend.
//
// The design enables thread 0 of minion_frontend, serves requests from a
// tiny built-in fake ICache, and emits one UART trace line per issued
// instruction. The first eight instructions are checked against expected PC,
// instruction word, and a few key decode bits; the harness then prints a
// final PASS/FAIL line.

module test_frontend
  import minion_pkg::*;
  import minion_frontend_pkg::*;
#(
  parameter int unsigned ClkHz           = 25_000_000,
  parameter int unsigned BaudRate        = 115200,
  parameter int unsigned BootDelayCycles = 0,
  parameter int unsigned TxFifoDepth     = 64,
  parameter int unsigned RespLatency     = 4,
  parameter int unsigned TraceCount      = 8,
  localparam int unsigned TxDepthW       = $clog2(TxFifoDepth + 1),
  localparam int unsigned BootDelayW     = (BootDelayCycles > 0) ? $clog2(BootDelayCycles + 1) : 1,
  localparam int unsigned TraceIdxW      = $clog2(TraceCount + 1),
  localparam int unsigned MsgIdxW        = 8
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  output logic       uart_tx_o,
  output logic [7:0] led_o
);

  typedef enum logic [1:0] {
    MsgBoot,
    MsgTrace,
    MsgPass,
    MsgFail
  } msg_kind_e;

  localparam logic [TraceIdxW-1:0] LastTraceIdx    = 4'd7;
  localparam logic [MsgIdxW-1:0]   BootMsgLastIdx   = 8'd22;
  localparam logic [MsgIdxW-1:0]   TraceMsgLastIdx  = 8'd189;
  localparam logic [MsgIdxW-1:0]   StatusMsgLastIdx = 8'd21;
  localparam logic [31:0]          BootDelayInitVal = BootDelayCycles;

  logic [BootDelayW-1:0] boot_delay_q;
  logic                  boot_pending_q;

  logic [7:0]           tx_wdata;
  logic                 tx_wvalid;
  logic                 tx_wready;
  logic [7:0]           tx_rdata;
  logic                 tx_rvalid;
  logic                 tx_rready;
  logic                 tx_pop_ready;
  logic [TxDepthW-1:0]  tx_depth;
  logic                 tx_full;
  logic                 tx_empty;
  logic                 tx_busy;
  logic                 tx_status_xor;

  logic                 msg_active_q;
  msg_kind_e            msg_kind_q;
  logic [MsgIdxW-1:0]   msg_idx_q;

  /* verilator lint_off UNUSEDSIGNAL */  // The harness snapshots a wider frontend debug state than the compact UART line currently prints.
  logic [TraceIdxW-1:0] trace_idx_q;
  logic                 trace_pending_q;
  logic                 status_pending_q;
  logic                 trace_done_q;
  logic                 trace_fail_q;
  logic                 trace_match_q;
  logic [63:0]          trace_pc_q;
  logic [31:0]          trace_inst_q;
  logic                 trace_legal_q;
  logic                 trace_mem_q;
  logic                 trace_br_q;
  logic                 trace_jal_q;
  logic                 trace_jalr_q;
  logic                 trace_redirect_valid_q;
  logic [63:0]          trace_redirect_pc_q;
  logic                 trace_pending_redirect_valid_q;
  logic [63:0]          trace_pending_redirect_pc_q;
  logic                 trace_redirect_pulse_seen_q;
  logic [63:0]          trace_redirect_pulse_pc_q;
  logic                 trace_core_req_valid_q;
  logic [63:0]          trace_core_req_pc_q;
  logic [63:0]          trace_thread_pc_q;
  logic [63:0]          trace_thread_pc_next_q;
  logic                 trace_thread_pc_update_q;
  logic                 trace_clock_enable_q;
  logic                 trace_req_valid_q;
  logic [63:0]          trace_req_addr_q;
  logic [63:0]          trace_last_req_addr_q;
  logic [FeFetchBuffers-1:0] trace_buffer_empty_q;
  logic                 trace_req_buffer_q;
  logic                 trace_f6_buffer_id_q;
  logic                 trace_f7_buffer_id_q;
  logic                 trace_icache_inflight_q;
  logic                 trace_miss_pending_q;
  logic                 trace_f1_req_valid_q;
  logic                 trace_f5_req_valid_q;
  logic                 trace_f6_req_valid_q;
  logic                 trace_f6_buffer_wr_q;
  logic                 trace_f6_valid_q;
  logic                 trace_f6_stall_q;
  logic                 trace_f7_valid_q;
  logic                 trace_f7_stall_q;
  logic                 trace_req_seen_q;
  logic [63:0]          trace_req_snap_addr_q;
  logic                 trace_write_seen_q;
  logic [63:0]          trace_write_pc_q;
  logic [63:0]          trace_write_pc_next_q;
  logic                 trace_write_pc_update_q;
  logic [FeFetchBuffers-1:0] trace_write_buffer_empty_q;
  logic                 trace_write_req_buffer_q;
  logic [63:0]          trace_buffer_pc0_q;
  logic [63:0]          trace_buffer_pc1_q;
  logic                 trace_f6_req_buffer_q;
  logic [7:0]           trace_f6_buffer_ptr_q;
  logic [7:0]           trace_f6_buffer_ptr_next_q;
  logic [7:0]           trace_f7_buffer_ptr_q;
  logic [3:0]           trace_gated_clk_ticks_q;
  logic                 trace_fifo_empty_q;
  logic                 trace_fifo_full_q;
  logic                 trace_tb_inst_valid_q;
  logic                 trace_tb_inst_ready_q;
  /* verilator lint_on UNUSEDSIGNAL */

  logic [1:0]          f0_thread_enabled;
  vm_status_t [1:0]    vm_status;
  logic                f1_icache_req_ready;
  logic                f1_icache_req_valid;
  /* verilator lint_off UNUSEDSIGNAL */  // the harness only consumes the request address field
  fe_icache_req_t      f1_icache_req;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                f5_icache_resp_valid;
  logic                f5_icache_resp_miss;
  icache_fe_resp_t     f5_icache_resp;
  logic                f6_icache_fill_done;
  logic [1:0]          f0_core_req_valid;
  fe_req_t [1:0]       f0_core_req;
  /* verilator lint_off UNUSEDSIGNAL */  // The harness only inspects thread-0 debug state and a subset of the per-thread FIFO flags.
  fe_thread_debug_t [1:0] frontend_debug_thread;
  logic [1:0]          frontend_debug_inst_fifo_empty;
  logic [1:0]          frontend_debug_inst_fifo_full;
  logic [1:0]          frontend_debug_tb_inst_valid;
  logic [1:0]          frontend_debug_tb_inst_ready;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                redirect_arm_q;
  logic                redirect_window_q;
  logic [PcSizeExt-1:0] redirect_pc_q;
  logic                redirect_pulse_seen_q;
  logic [PcSizeExt-1:0] redirect_pulse_pc_q;
  logic [1:0]          id_core_stall;
  logic                id_inst_ready;
  /* verilator lint_off UNOPTFLAT */  // minion_frontend preserves the original combinational scheduling loop through the per-thread issue FIFOs
  logic                id_inst_valid;
  /* verilator lint_on UNOPTFLAT */
  logic                id_inst_thread_id;
  /* verilator lint_off UNUSEDSIGNAL */  // trace harness checks only a subset of the frontend outputs
  fe_core_resp_t       id_inst_data;
  minion_control_t     id_intpipe_ctrl;
  vpu_ctrl_sigs_t      id_vpu_decoder_sigs;
  vpu_id_ctrl_t        id_vpu_core_ctrl;
  /* verilator lint_on UNUSEDSIGNAL */

  /* verilator lint_off UNUSEDSIGNAL */  // Snapshot full frontend output structs even though the compact UART trace checks only selected fields.
  logic                issue_snap_valid_q;
  logic                issue_snap_thread_id_q;
  fe_core_resp_t       issue_snap_data_q;
  minion_control_t     issue_snap_ctrl_q;
  /* verilator lint_on UNUSEDSIGNAL */

  logic [RespLatency-1:0] resp_valid_q;
  icache_fe_resp_t        resp_data_q [RespLatency];
  logic [63:0]            last_req_addr_q;
  logic                   req_seen_q;
  logic [63:0]            req_snap_addr_q;
  logic                   write_seen_q;
  logic [63:0]            write_snap_pc_q;
  logic [63:0]            write_snap_pc_next_q;
  logic                   write_snap_pc_update_q;
  logic [FeFetchBuffers-1:0] write_snap_buffer_empty_q;
  logic                   write_snap_req_buffer_q;

  function automatic logic [7:0] ascii_bool(input logic value);
    ascii_bool = value ? 8'h31 : 8'h30;
  endfunction

  function automatic logic [7:0] ascii_dec_digit(input logic [TraceIdxW-1:0] value);
    ascii_dec_digit = 8'h30 + {{(8-TraceIdxW){1'b0}}, value};
  endfunction

  function automatic logic [7:0] hex_nibble_ascii(input logic [3:0] nibble);
    unique case (nibble)
      4'h0:    hex_nibble_ascii = 8'h30;
      4'h1:    hex_nibble_ascii = 8'h31;
      4'h2:    hex_nibble_ascii = 8'h32;
      4'h3:    hex_nibble_ascii = 8'h33;
      4'h4:    hex_nibble_ascii = 8'h34;
      4'h5:    hex_nibble_ascii = 8'h35;
      4'h6:    hex_nibble_ascii = 8'h36;
      4'h7:    hex_nibble_ascii = 8'h37;
      4'h8:    hex_nibble_ascii = 8'h38;
      4'h9:    hex_nibble_ascii = 8'h39;
      4'hA:    hex_nibble_ascii = 8'h41;
      4'hB:    hex_nibble_ascii = 8'h42;
      4'hC:    hex_nibble_ascii = 8'h43;
      4'hD:    hex_nibble_ascii = 8'h44;
      4'hE:    hex_nibble_ascii = 8'h45;
      default: hex_nibble_ascii = 8'h46;
    endcase
  endfunction

  /* verilator lint_off UNUSEDSIGNAL */  // only the low nibble of the shifted value is consumed
  function automatic logic [7:0] hex64_digit_ascii(
    input logic [63:0] value,
    input logic [4:0]  digit_idx
  );
    logic [63:0] shifted;
    shifted = value >> ((5'd15 - digit_idx) * 4);
    hex64_digit_ascii = hex_nibble_ascii(shifted[3:0]);
  endfunction

  function automatic logic [7:0] hex32_digit_ascii(
    input logic [31:0] value,
    input logic [3:0]  digit_idx
  );
    logic [31:0] shifted;
    shifted = value >> ((4'd7 - digit_idx) * 4);
    hex32_digit_ascii = hex_nibble_ascii(shifted[3:0]);
  endfunction

  function automatic logic [7:0] hex8_digit_ascii(
    input logic [7:0] value,
    input logic       digit_idx
  );
    logic [7:0] shifted;
    if (digit_idx)
      shifted = value;
    else
      shifted = value >> 4;
    hex8_digit_ascii = hex_nibble_ascii(shifted[3:0]);
  endfunction
  /* verilator lint_on UNUSEDSIGNAL */

  function automatic logic [4:0] msg_hex64_idx(
    input logic [4:0] idx,
    input logic [4:0] base
  );
    msg_hex64_idx = idx - base;
  endfunction

  function automatic logic [3:0] msg_hex32_idx(
    input logic [3:0] idx,
    input logic [3:0] base
  );
    msg_hex32_idx = idx - base;
  endfunction

  function automatic logic [7:0] boot_msg_byte(input logic [MsgIdxW-1:0] idx);
    unique case (idx)
      0:  boot_msg_byte = 8'h41;  // A
      1:  boot_msg_byte = 8'h69;  // i
      2:  boot_msg_byte = 8'h6E;  // n
      3:  boot_msg_byte = 8'h65;  // e
      4:  boot_msg_byte = 8'h6B;  // k
      5:  boot_msg_byte = 8'h6B;  // k
      6:  boot_msg_byte = 8'h6F;  // o
      7:  boot_msg_byte = 8'h20;  // space
      8:  boot_msg_byte = 8'h74;  // t
      9:  boot_msg_byte = 8'h65;  // e
      10: boot_msg_byte = 8'h73;  // s
      11: boot_msg_byte = 8'h74;  // t
      12: boot_msg_byte = 8'h5F;  // _
      13: boot_msg_byte = 8'h66;  // f
      14: boot_msg_byte = 8'h72;  // r
      15: boot_msg_byte = 8'h6F;  // o
      16: boot_msg_byte = 8'h6E;  // n
      17: boot_msg_byte = 8'h74;  // t
      18: boot_msg_byte = 8'h65;  // e
      19: boot_msg_byte = 8'h6E;  // n
      20: boot_msg_byte = 8'h64;  // d
      21: boot_msg_byte = 8'h0D;  // \r
      default: boot_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] status_msg_byte(
    input logic             pass_i,
    input logic [MsgIdxW-1:0] idx
  );
    unique case (idx)
      0:  status_msg_byte = 8'h54;  // T
      1:  status_msg_byte = 8'h45;  // E
      2:  status_msg_byte = 8'h53;  // S
      3:  status_msg_byte = 8'h54;  // T
      4:  status_msg_byte = 8'h20;  // space
      5:  status_msg_byte = 8'h46;  // F
      6:  status_msg_byte = 8'h52;  // R
      7:  status_msg_byte = 8'h4F;  // O
      8:  status_msg_byte = 8'h4E;  // N
      9:  status_msg_byte = 8'h54;  // T
      10: status_msg_byte = 8'h45;  // E
      11: status_msg_byte = 8'h4E;  // N
      12: status_msg_byte = 8'h44;  // D
      13: status_msg_byte = 8'h20;  // space
      14: status_msg_byte = pass_i ? 8'h50 : 8'h46;  // P/F
      15: status_msg_byte = pass_i ? 8'h41 : 8'h41;  // A
      16: status_msg_byte = pass_i ? 8'h53 : 8'h49;  // S/I
      17: status_msg_byte = pass_i ? 8'h53 : 8'h4C;  // S/L
      18: status_msg_byte = pass_i ? 8'h45 : 8'h45;  // E/E
      19: status_msg_byte = pass_i ? 8'h44 : 8'h44;  // D/D
      20: status_msg_byte = 8'h0D;  // \r
      default: status_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  /* verilator lint_off UNUSEDSIGNAL */  // The trace formatter accepts a wider debug bundle than the current UART string consumes.
  function automatic logic [7:0] trace_msg_byte(
    input logic [MsgIdxW-1:0]   idx,
    input logic [TraceIdxW-1:0] trace_idx,
    input logic [63:0]          trace_pc,
    input logic [31:0]          trace_inst,
    input logic [63:0]          trace_thread_pc,
    input logic [63:0]          trace_thread_pc_next,
    input logic                 trace_thread_pc_update,
    input logic [FeFetchBuffers-1:0] trace_buffer_empty,
    input logic                 trace_req_seen,
    input logic [63:0]          trace_req_snap_addr,
    input logic                 trace_redirect_valid,
    input logic [63:0]          trace_redirect_pc,
    input logic                 trace_pending_redirect_valid,
    input logic [63:0]          trace_pending_redirect_pc,
    input logic                 trace_redirect_pulse_seen,
    input logic [63:0]          trace_redirect_pulse_pc,
    input logic [3:0]           trace_gated_clk_ticks,
    input logic                 trace_clock_enable
  );
    if (idx == 0) begin
      trace_msg_byte = 8'h54;  // T
    end else if (idx == 1) begin
      trace_msg_byte = ascii_dec_digit(trace_idx);
    end else if (idx == 2) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 3) begin
      trace_msg_byte = 8'h50;  // P
    end else if (idx == 4) begin
      trace_msg_byte = 8'h3D;  // =
    end else if ((idx >= 5) && (idx <= 20)) begin
      trace_msg_byte = hex64_digit_ascii(trace_pc, msg_hex64_idx(idx[4:0], 5'd5));
    end else if (idx == 21) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 22) begin
      trace_msg_byte = 8'h49;  // I
    end else if (idx == 23) begin
      trace_msg_byte = 8'h3D;  // =
    end else if ((idx >= 24) && (idx <= 31)) begin
      trace_msg_byte = hex32_digit_ascii(trace_inst, msg_hex32_idx(idx[3:0], 4'd8));
    end else if (idx == 32) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 33) begin
      trace_msg_byte = 8'h46;  // F
    end else if (idx == 34) begin
      trace_msg_byte = 8'h50;  // P
    end else if (idx == 35) begin
      trace_msg_byte = 8'h3D;  // =
    end else if ((idx >= 36) && (idx <= 51)) begin
      trace_msg_byte = hex64_digit_ascii(trace_thread_pc, msg_hex64_idx(idx[4:0], 5'd4));
    end else if (idx == 52) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 53) begin
      trace_msg_byte = 8'h46;  // F
    end else if (idx == 54) begin
      trace_msg_byte = 8'h4E;  // N
    end else if (idx == 55) begin
      trace_msg_byte = 8'h3D;  // =
    end else if ((idx >= 56) && (idx <= 71)) begin
      trace_msg_byte = hex64_digit_ascii(trace_thread_pc_next, msg_hex64_idx(idx[4:0], 5'd24));
    end else if (idx == 72) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 73) begin
      trace_msg_byte = 8'h50;  // P
    end else if (idx == 74) begin
      trace_msg_byte = 8'h55;  // U
    end else if (idx == 75) begin
      trace_msg_byte = 8'h3D;  // =
    end else if (idx == 76) begin
      trace_msg_byte = ascii_bool(trace_thread_pc_update);
    end else if (idx == 77) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 78) begin
      trace_msg_byte = 8'h42;  // B
    end else if (idx == 79) begin
      trace_msg_byte = 8'h45;  // E
    end else if (idx == 80) begin
      trace_msg_byte = 8'h3D;  // =
    end else if (idx == 81) begin
      trace_msg_byte = ascii_bool(trace_buffer_empty[1]);
    end else if (idx == 82) begin
      trace_msg_byte = ascii_bool(trace_buffer_empty[0]);
    end else if (idx == 83) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 84) begin
      trace_msg_byte = 8'h51;  // Q
    end else if (idx == 85) begin
      trace_msg_byte = 8'h53;  // S
    end else if (idx == 86) begin
      trace_msg_byte = 8'h3D;  // =
    end else if (idx == 87) begin
      trace_msg_byte = ascii_bool(trace_req_seen);
    end else if (idx == 88) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 89) begin
      trace_msg_byte = 8'h51;  // Q
    end else if (idx == 90) begin
      trace_msg_byte = 8'h41;  // A
    end else if (idx == 91) begin
      trace_msg_byte = 8'h3D;  // =
    end else if ((idx >= 92) && (idx <= 107)) begin
      trace_msg_byte = hex64_digit_ascii(trace_req_snap_addr, msg_hex64_idx(idx[4:0], 5'd28));
    end else if (idx == 108) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 109) begin
      trace_msg_byte = 8'h52;  // R
    end else if (idx == 110) begin
      trace_msg_byte = 8'h56;  // V
    end else if (idx == 111) begin
      trace_msg_byte = 8'h3D;  // =
    end else if (idx == 112) begin
      trace_msg_byte = ascii_bool(trace_redirect_valid);
    end else if (idx == 113) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 114) begin
      trace_msg_byte = 8'h52;  // R
    end else if (idx == 115) begin
      trace_msg_byte = 8'h50;  // P
    end else if (idx == 116) begin
      trace_msg_byte = 8'h3D;  // =
    end else if ((idx >= 117) && (idx <= 132)) begin
      trace_msg_byte = hex64_digit_ascii(trace_redirect_pc, msg_hex64_idx(idx[4:0], 5'd21));
    end else if (idx == 133) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 134) begin
      trace_msg_byte = 8'h50;  // P
    end else if (idx == 135) begin
      trace_msg_byte = 8'h56;  // V
    end else if (idx == 136) begin
      trace_msg_byte = 8'h3D;  // =
    end else if (idx == 137) begin
      trace_msg_byte = ascii_bool(trace_pending_redirect_valid);
    end else if (idx == 138) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 139) begin
      trace_msg_byte = 8'h50;  // P
    end else if (idx == 140) begin
      trace_msg_byte = 8'h50;  // P
    end else if (idx == 141) begin
      trace_msg_byte = 8'h3D;  // =
    end else if ((idx >= 142) && (idx <= 157)) begin
      trace_msg_byte = hex64_digit_ascii(trace_pending_redirect_pc, msg_hex64_idx(idx[4:0], 5'd14));
    end else if (idx == 158) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 159) begin
      trace_msg_byte = 8'h52;  // R
    end else if (idx == 160) begin
      trace_msg_byte = 8'h53;  // S
    end else if (idx == 161) begin
      trace_msg_byte = 8'h3D;  // =
    end else if (idx == 162) begin
      trace_msg_byte = ascii_bool(trace_redirect_pulse_seen);
    end else if (idx == 163) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 164) begin
      trace_msg_byte = 8'h53;  // S
    end else if (idx == 165) begin
      trace_msg_byte = 8'h50;  // P
    end else if (idx == 166) begin
      trace_msg_byte = 8'h3D;  // =
    end else if ((idx >= 167) && (idx <= 182)) begin
      trace_msg_byte = hex64_digit_ascii(trace_redirect_pulse_pc, msg_hex64_idx(idx[4:0], 5'd7));
    end else if (idx == 183) begin
      trace_msg_byte = 8'h20;  // space
    end else if (idx == 184) begin
      trace_msg_byte = 8'h47;  // G
    end else if (idx == 185) begin
      trace_msg_byte = 8'h54;  // T
    end else if (idx == 186) begin
      trace_msg_byte = 8'h3D;  // =
    end else if (idx == 187) begin
      trace_msg_byte = hex_nibble_ascii(trace_gated_clk_ticks);
    end else if (idx == 188) begin
      trace_msg_byte = 8'h0D;  // \r
    end else begin
      trace_msg_byte = 8'h0A;  // \n
    end
  endfunction
  /* verilator lint_on UNUSEDSIGNAL */

  function automatic logic [PcSizeExt-1:0] expected_pc(input logic [TraceIdxW-1:0] idx);
    unique case (idx)
      0: expected_pc = 49'h0000_0000_1000;
      1: expected_pc = 49'h0000_0000_1020;
      2: expected_pc = 49'h0000_0000_1040;
      3: expected_pc = 49'h0000_0000_1060;
      4: expected_pc = 49'h0000_0000_1080;
      5: expected_pc = 49'h0000_0000_10A0;
      6: expected_pc = 49'h0000_0000_10C0;
      default: expected_pc = 49'h0000_0000_10E0;
    endcase
  endfunction

  function automatic logic [31:0] expected_inst(input logic [TraceIdxW-1:0] idx);
    unique case (idx)
      0: expected_inst = 32'h0000_0013;
      1: expected_inst = 32'h0010_0093;
      2: expected_inst = 32'h0030_0193;
      3: expected_inst = 32'h0000_A103;
      4: expected_inst = 32'h0020_A223;
      5: expected_inst = 32'h0020_8463;
      6: expected_inst = 32'h0080_006F;
      default: expected_inst = 32'h0000_8067;
    endcase
  endfunction

  function automatic logic expected_mem(input logic [TraceIdxW-1:0] idx);
    unique case (idx)
      3, 4:    expected_mem = 1'b1;
      default: expected_mem = 1'b0;
    endcase
  endfunction

  function automatic logic expected_br(input logic [TraceIdxW-1:0] idx);
    expected_br = (idx == 5);
  endfunction

  function automatic logic expected_jal(input logic [TraceIdxW-1:0] idx);
    expected_jal = (idx == 6);
  endfunction

  function automatic logic expected_jalr(input logic [TraceIdxW-1:0] idx);
    expected_jalr = (idx == 7);
  endfunction

  function automatic logic [255:0] fake_icache_line(input logic [3:0] line_idx);
    logic [31:0] first_inst;

    unique case (line_idx)
      4'd0:    first_inst = 32'h0000_0013;
      4'd1:    first_inst = 32'h0010_0093;
      4'd2:    first_inst = 32'h0030_0193;
      4'd3:    first_inst = 32'h0000_A103;
      4'd4:    first_inst = 32'h0020_A223;
      4'd5:    first_inst = 32'h0020_8463;
      4'd6:    first_inst = 32'h0080_006F;
      4'd7:    first_inst = 32'h0000_8067;
      default: first_inst = 32'h0000_0013;
    endcase

    fake_icache_line = {
      32'h0000_0013,
      32'h0000_0013,
      32'h0000_0013,
      32'h0000_0013,
      32'h0000_0013,
      32'h0000_0013,
      32'h0000_0013,
      first_inst
    };
  endfunction

  wire boot_delay_done = (boot_delay_q == '0);
  wire msg_push        = msg_active_q && tx_wready;
  wire frontend_enable = !boot_pending_q && !trace_done_q;
  wire redirect_active = redirect_arm_q || redirect_window_q;
  wire issue_window    = frontend_enable
                       && !msg_active_q
                       && !trace_pending_q
                       && !status_pending_q
                       && !redirect_active;
  wire issue_sample    = issue_window && !issue_snap_valid_q && id_inst_valid;
  wire inst_accept     = issue_window && issue_snap_valid_q && id_inst_valid;
  wire req_accept      = f1_icache_req_valid && f1_icache_req_ready;
  wire write_event     = frontend_debug_thread[0].f6_buffer_wr;

  always_comb begin
    vm_status = '0;
    vm_status[0].prv = PrvM;
    vm_status[1].prv = PrvM;

    f0_thread_enabled  = frontend_enable ? 2'b01 : 2'b00;
    f1_icache_req_ready = 1'b1;
    f5_icache_resp_valid = resp_valid_q[RespLatency-1];
    f5_icache_resp_miss  = 1'b0;
    f5_icache_resp       = resp_data_q[RespLatency-1];
    f6_icache_fill_done  = 1'b0;
    f0_core_req_valid    = {1'b0, redirect_window_q};
    f0_core_req          = '0;
    f0_core_req[0].pc    = redirect_pc_q;
    id_core_stall        = '0;
    // Match the real core contract: f0_core_req_valid is a core-side redirect
    // pulse, not an acked request held until frontend internals look drained.
    // The FPGA harness asserts this pulse on the falling edge so it is stable
    // before the gated frontend clock's following rising edge.
    // Snapshot first, then acknowledge. Sampling raw id_inst_data on the same
    // edge as ready/pop is simulator-clean but FPGA-timing fragile.
    id_inst_ready        = issue_window && issue_snap_valid_q;
  end

  always_comb begin
    tx_wvalid = msg_active_q && tx_wready;
    tx_wdata  = 8'h00;

    if (msg_active_q) begin
      unique case (msg_kind_q)
        MsgBoot: tx_wdata = boot_msg_byte(msg_idx_q);
        MsgTrace: tx_wdata = trace_msg_byte(
          msg_idx_q,
          trace_idx_q,
          trace_pc_q,
          trace_inst_q,
          trace_thread_pc_q,
          trace_thread_pc_next_q,
          trace_thread_pc_update_q,
          trace_buffer_empty_q,
          trace_req_seen_q,
          trace_req_snap_addr_q,
          trace_redirect_valid_q,
          trace_redirect_pc_q,
          trace_pending_redirect_valid_q,
          trace_pending_redirect_pc_q,
          trace_redirect_pulse_seen_q,
          trace_redirect_pulse_pc_q,
          trace_gated_clk_ticks_q,
          trace_clock_enable_q
        );
        MsgPass: tx_wdata = status_msg_byte(1'b1, msg_idx_q);
        default: tx_wdata = status_msg_byte(1'b0, msg_idx_q);
      endcase
    end
  end

  /* verilator lint_off SYNCASYNCNET */  // The harness reset is async locally and also feeds the synchronous-reset logic inside minion_frontend.
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      boot_delay_q     <= BootDelayInitVal[BootDelayW-1:0];
      boot_pending_q   <= 1'b1;
      msg_active_q     <= 1'b0;
      msg_kind_q       <= MsgBoot;
      msg_idx_q        <= '0;
      trace_idx_q      <= '0;
      trace_pending_q  <= 1'b0;
      status_pending_q <= 1'b0;
      trace_done_q     <= 1'b0;
      trace_fail_q     <= 1'b0;
      trace_match_q    <= 1'b0;
      trace_pc_q       <= '0;
      trace_inst_q     <= '0;
      trace_legal_q    <= 1'b0;
      trace_mem_q      <= 1'b0;
      trace_br_q       <= 1'b0;
      trace_jal_q      <= 1'b0;
      trace_jalr_q     <= 1'b0;
      trace_redirect_valid_q <= 1'b0;
      trace_redirect_pc_q    <= '0;
      trace_pending_redirect_valid_q <= 1'b0;
      trace_pending_redirect_pc_q    <= '0;
      trace_redirect_pulse_seen_q    <= 1'b0;
      trace_redirect_pulse_pc_q      <= '0;
      trace_core_req_valid_q <= 1'b0;
      trace_core_req_pc_q    <= '0;
      trace_thread_pc_q      <= '0;
      trace_thread_pc_next_q <= '0;
      trace_thread_pc_update_q <= 1'b0;
      trace_clock_enable_q   <= 1'b0;
      trace_req_valid_q      <= 1'b0;
      trace_req_addr_q       <= '0;
      trace_last_req_addr_q  <= '0;
      trace_buffer_empty_q   <= '0;
      trace_req_buffer_q     <= 1'b0;
      trace_f6_buffer_id_q   <= 1'b0;
      trace_f7_buffer_id_q   <= 1'b0;
      trace_icache_inflight_q <= 1'b0;
      trace_miss_pending_q   <= 1'b0;
      trace_f1_req_valid_q   <= 1'b0;
      trace_f5_req_valid_q   <= 1'b0;
      trace_f6_req_valid_q   <= 1'b0;
      trace_f6_buffer_wr_q   <= 1'b0;
      trace_f6_valid_q       <= 1'b0;
      trace_f6_stall_q       <= 1'b0;
      trace_f7_valid_q       <= 1'b0;
      trace_f7_stall_q       <= 1'b0;
      trace_req_seen_q       <= 1'b0;
      trace_req_snap_addr_q  <= '0;
      trace_write_seen_q     <= 1'b0;
      trace_write_pc_q       <= '0;
      trace_write_pc_next_q  <= '0;
      trace_write_pc_update_q <= 1'b0;
      trace_write_buffer_empty_q <= '0;
      trace_write_req_buffer_q <= 1'b0;
      trace_buffer_pc0_q     <= '0;
      trace_buffer_pc1_q     <= '0;
      trace_f6_req_buffer_q  <= 1'b0;
      trace_f6_buffer_ptr_q  <= '0;
      trace_f6_buffer_ptr_next_q <= '0;
      trace_f7_buffer_ptr_q  <= '0;
      trace_gated_clk_ticks_q <= '0;
      trace_fifo_empty_q     <= 1'b0;
      trace_fifo_full_q      <= 1'b0;
      trace_tb_inst_valid_q  <= 1'b0;
      trace_tb_inst_ready_q  <= 1'b0;
      issue_snap_valid_q     <= 1'b0;
      issue_snap_thread_id_q <= 1'b0;
      issue_snap_data_q      <= '0;
      issue_snap_ctrl_q      <= '0;
      last_req_addr_q        <= '0;
      req_seen_q            <= 1'b0;
      req_snap_addr_q       <= '0;
      write_seen_q          <= 1'b0;
      write_snap_pc_q       <= '0;
      write_snap_pc_next_q  <= '0;
      write_snap_pc_update_q <= 1'b0;
      write_snap_buffer_empty_q <= '0;
      write_snap_req_buffer_q <= 1'b0;
      redirect_arm_q        <= 1'b0;
      redirect_pc_q         <= '0;
      redirect_pulse_seen_q <= 1'b0;
      redirect_pulse_pc_q   <= '0;
    end else begin
      if (redirect_window_q) begin
        redirect_arm_q        <= 1'b0;
        redirect_pulse_seen_q <= 1'b1;
        redirect_pulse_pc_q   <= redirect_pc_q;
      end

      if (!boot_delay_done) begin
        boot_delay_q <= boot_delay_q - 1'b1;
      end

      if (req_accept) begin
        last_req_addr_q <= {{(64-PcSizeExt){1'b0}}, f1_icache_req.addr};
        req_seen_q      <= 1'b1;
        req_snap_addr_q <= {{(64-PcSizeExt){1'b0}}, f1_icache_req.addr};
      end

      if (write_event) begin
        write_seen_q            <= 1'b1;
        write_snap_pc_q         <= {{(64-VaSizeExt){1'b0}}, frontend_debug_thread[0].f0_pc};
        write_snap_pc_next_q    <= {{(64-VaSizeExt){1'b0}}, frontend_debug_thread[0].f0_pc_next};
        write_snap_pc_update_q  <= frontend_debug_thread[0].f0_pc_update;
        write_snap_buffer_empty_q <= frontend_debug_thread[0].buffer_empty;
        write_snap_req_buffer_q <= frontend_debug_thread[0].f6_req_buffer;
      end

      if (!frontend_enable || redirect_active) begin
        issue_snap_valid_q <= 1'b0;
      end else if (inst_accept) begin
        issue_snap_valid_q <= 1'b0;
      end else if (issue_sample) begin
        issue_snap_valid_q     <= 1'b1;
        issue_snap_thread_id_q <= id_inst_thread_id;
        issue_snap_data_q      <= id_inst_data;
        issue_snap_ctrl_q      <= id_intpipe_ctrl;
      end

      if (inst_accept) begin
        trace_pending_q <= 1'b1;
        trace_pc_q      <= {{15{1'b0}}, issue_snap_data_q.pc};
        trace_inst_q    <= issue_snap_data_q.inst_bits;
        trace_legal_q   <= issue_snap_ctrl_q.legal;
        trace_mem_q     <= issue_snap_ctrl_q.mem;
        trace_br_q      <= issue_snap_ctrl_q.br;
        trace_jal_q     <= issue_snap_ctrl_q.jal;
        trace_jalr_q    <= issue_snap_ctrl_q.jalr;
        trace_redirect_valid_q <= (trace_idx_q != LastTraceIdx);
        if (trace_idx_q != LastTraceIdx)
          trace_redirect_pc_q <= {{15{1'b0}}, expected_pc(trace_idx_q + 1'b1)};
        else
          trace_redirect_pc_q <= '0;
        trace_pending_redirect_valid_q <= redirect_active;
        trace_pending_redirect_pc_q    <= {{(64-PcSizeExt){1'b0}}, redirect_pc_q};
        trace_redirect_pulse_seen_q    <= redirect_pulse_seen_q;
        trace_redirect_pulse_pc_q      <= {{(64-PcSizeExt){1'b0}}, redirect_pulse_pc_q};
        trace_core_req_valid_q <= frontend_debug_thread[0].core_req_valid;
        trace_core_req_pc_q    <= {{(64-PcSizeExt){1'b0}}, frontend_debug_thread[0].core_req_pc};
        trace_thread_pc_q <= {{(64-VaSizeExt){1'b0}}, frontend_debug_thread[0].f0_pc};
        trace_thread_pc_next_q <= {{(64-VaSizeExt){1'b0}}, frontend_debug_thread[0].f0_pc_next};
        trace_thread_pc_update_q <= frontend_debug_thread[0].f0_pc_update;
        trace_clock_enable_q   <= frontend_debug_thread[0].clock_enable;
        trace_req_valid_q <= f1_icache_req_valid;
        if (f1_icache_req_valid)
          trace_req_addr_q <= {{(64-PcSizeExt){1'b0}}, f1_icache_req.addr};
        else
          trace_req_addr_q <= '0;
        if (req_accept)
          trace_last_req_addr_q <= {{(64-PcSizeExt){1'b0}}, f1_icache_req.addr};
        else
          trace_last_req_addr_q <= last_req_addr_q;
        trace_buffer_empty_q    <= frontend_debug_thread[0].buffer_empty;
        trace_req_buffer_q      <= frontend_debug_thread[0].req_buffer;
        trace_f6_buffer_id_q    <= frontend_debug_thread[0].f6_buffer_id;
        trace_f7_buffer_id_q    <= frontend_debug_thread[0].f7_buffer_id;
        trace_icache_inflight_q <= frontend_debug_thread[0].icache_inflight;
        trace_miss_pending_q    <= frontend_debug_thread[0].miss_pending;
        trace_f1_req_valid_q    <= frontend_debug_thread[0].f1_req_valid;
        trace_f5_req_valid_q    <= frontend_debug_thread[0].f5_req_valid;
        trace_f6_req_valid_q    <= frontend_debug_thread[0].f6_req_valid;
        trace_f6_buffer_wr_q    <= frontend_debug_thread[0].f6_buffer_wr;
        trace_f6_valid_q        <= frontend_debug_thread[0].f6_valid;
        trace_f6_stall_q        <= frontend_debug_thread[0].f6_stall;
        trace_f7_valid_q        <= frontend_debug_thread[0].f7_valid;
        trace_f7_stall_q        <= frontend_debug_thread[0].f7_stall;
        trace_req_seen_q <= req_accept ? 1'b1 : req_seen_q;
        trace_req_snap_addr_q <= req_accept ? {{(64-PcSizeExt){1'b0}}, f1_icache_req.addr} : req_snap_addr_q;
        trace_write_seen_q <= write_event ? 1'b1 : write_seen_q;
        trace_write_pc_q <= write_event ? {{(64-VaSizeExt){1'b0}}, frontend_debug_thread[0].f0_pc} : write_snap_pc_q;
        trace_write_pc_next_q <= write_event ? {{(64-VaSizeExt){1'b0}}, frontend_debug_thread[0].f0_pc_next} : write_snap_pc_next_q;
        trace_write_pc_update_q <= write_event ? frontend_debug_thread[0].f0_pc_update : write_snap_pc_update_q;
        trace_write_buffer_empty_q <= write_event ? frontend_debug_thread[0].buffer_empty : write_snap_buffer_empty_q;
        trace_write_req_buffer_q <= write_event ? frontend_debug_thread[0].f6_req_buffer : write_snap_req_buffer_q;
        trace_buffer_pc0_q <= {{(64-VaSizeExt){1'b0}}, frontend_debug_thread[0].buffer_pc0};
        trace_buffer_pc1_q <= {{(64-VaSizeExt){1'b0}}, frontend_debug_thread[0].buffer_pc1};
        trace_f6_req_buffer_q <= frontend_debug_thread[0].f6_req_buffer;
        trace_f6_buffer_ptr_q <= {3'b000, frontend_debug_thread[0].f6_buffer_ptr};
        trace_f6_buffer_ptr_next_q <= {3'b000, frontend_debug_thread[0].f6_buffer_ptr_next};
        trace_f7_buffer_ptr_q <= {3'b000, frontend_debug_thread[0].f7_buffer_ptr};
        trace_gated_clk_ticks_q <= frontend_debug_thread[0].gated_clk_ticks;
        trace_fifo_empty_q      <= frontend_debug_inst_fifo_empty[0];
        trace_fifo_full_q       <= frontend_debug_inst_fifo_full[0];
        trace_tb_inst_valid_q   <= frontend_debug_tb_inst_valid[0];
        trace_tb_inst_ready_q   <= frontend_debug_tb_inst_ready[0];
        trace_match_q   <= (issue_snap_thread_id_q == 1'b0)
                        && ({{15{1'b0}}, issue_snap_data_q.pc} == {{15{1'b0}}, expected_pc(trace_idx_q)})
                        && (issue_snap_data_q.inst_bits == expected_inst(trace_idx_q))
                        && (issue_snap_data_q.rvc == 1'b0)
                        && (issue_snap_ctrl_q.legal == 1'b1)
                        && (issue_snap_ctrl_q.mem == expected_mem(trace_idx_q))
                        && (issue_snap_ctrl_q.br == expected_br(trace_idx_q))
                        && (issue_snap_ctrl_q.jal == expected_jal(trace_idx_q))
                        && (issue_snap_ctrl_q.jalr == expected_jalr(trace_idx_q))
                        && (issue_snap_ctrl_q.fp == 1'b0);

        req_seen_q   <= 1'b0;
        write_seen_q <= 1'b0;
        redirect_pulse_seen_q <= 1'b0;

        if (trace_idx_q != LastTraceIdx) begin
          redirect_arm_q <= 1'b1;
          redirect_pc_q  <= expected_pc(trace_idx_q + 1'b1);
        end
      end

      if (!msg_active_q) begin
        if (boot_pending_q && boot_delay_done) begin
          msg_active_q <= 1'b1;
          msg_kind_q   <= MsgBoot;
          msg_idx_q    <= '0;
        end else if (trace_pending_q) begin
          msg_active_q    <= 1'b1;
          msg_kind_q      <= MsgTrace;
          msg_idx_q       <= '0;
          trace_pending_q <= 1'b0;
        end else if (status_pending_q) begin
          msg_active_q     <= 1'b1;
          msg_kind_q       <= trace_fail_q ? MsgFail : MsgPass;
          msg_idx_q        <= '0;
          status_pending_q <= 1'b0;
        end
      end else if (msg_push) begin
        if (((msg_kind_q == MsgBoot) && (msg_idx_q == BootMsgLastIdx))
         || ((msg_kind_q == MsgTrace) && (msg_idx_q == TraceMsgLastIdx))
         || (((msg_kind_q == MsgPass) || (msg_kind_q == MsgFail)) && (msg_idx_q == StatusMsgLastIdx))) begin
          msg_active_q <= 1'b0;
          msg_idx_q    <= '0;

          if (msg_kind_q == MsgBoot) begin
            boot_pending_q <= 1'b0;
          end else if (msg_kind_q == MsgTrace) begin
            if (trace_match_q && (trace_idx_q != LastTraceIdx)) begin
              trace_idx_q <= trace_idx_q + 1'b1;
            end else begin
              trace_fail_q     <= !trace_match_q;
              status_pending_q <= 1'b1;
            end
          end else begin
            trace_done_q <= 1'b1;
          end
        end else begin
          msg_idx_q <= msg_idx_q + 1'b1;
        end
      end
    end
  end
  /* verilator lint_on SYNCASYNCNET */

  always_ff @(posedge clk_i or negedge rst_ni) begin
    int stage;

    if (!rst_ni) begin
      for (stage = 0; stage < RespLatency; stage++) begin
        resp_valid_q[stage] <= 1'b0;
      end
    end else if (redirect_active) begin
      // The fake ICache has no transaction IDs. A frontend redirect kills any
      // outstanding request in the real pipeline, so stale synthetic responses
      // must be discarded here rather than delivered after the redirected fetch.
      for (stage = 0; stage < RespLatency; stage++) begin
        resp_valid_q[stage] <= 1'b0;
      end
    end else begin
      resp_valid_q[0] <= req_accept;
      if (req_accept) begin
        resp_data_q[0].data         <= fake_icache_line(f1_icache_req.addr[FeFetchReadLsb +: 4]);
        resp_data_q[0].page_fault   <= 1'b0;
        resp_data_q[0].access_fault <= 1'b0;
        resp_data_q[0].cacheable    <= 1'b1;
        resp_data_q[0].bus_err      <= 1'b0;
        resp_data_q[0].ecc_err      <= 1'b0;
      end

      for (stage = 1; stage < RespLatency; stage++) begin
        resp_valid_q[stage] <= resp_valid_q[stage-1];
        if (resp_valid_q[stage-1]) begin
          resp_data_q[stage] <= resp_data_q[stage-1];
        end
      end
    end
  end

  always_ff @(negedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      redirect_window_q <= 1'b0;
    end else begin
      // Present the core redirect for the next rising edge of the gated
      // frontend domain. This models intpipe's combinational redirect path with
      // real FPGA setup/hold instead of a zero-delay root-clock pulse.
      redirect_window_q <= redirect_arm_q;
    end
  end

  minion_frontend #(.NrThreads(2)) u_frontend (
    .clk_i,
    .rst_ni,
    .reset_debug_i         (1'b0),
    .chicken_bit_i         (1'b0),
    .dft_i                 ('0),
    .f0_thread_enabled_i   (f0_thread_enabled),
    .f0_reset_vector_i     (48'h0000_0000_1000),
    .vm_status_i           (vm_status),
    .f1_icache_req_ready_i (f1_icache_req_ready),
    .f1_icache_req_valid_o (f1_icache_req_valid),
    .f1_icache_req_o       (f1_icache_req),
    .f5_icache_resp_valid_i(f5_icache_resp_valid),
    .f5_icache_resp_miss_i (f5_icache_resp_miss),
    .f5_icache_resp_i      (f5_icache_resp),
    .f6_icache_fill_done_i (f6_icache_fill_done),
    .f0_core_req_valid_i   (f0_core_req_valid),
    .f0_core_req_i         (f0_core_req),
    .id_core_stall_i       (id_core_stall),
    .id_inst_ready_i       (id_inst_ready),
    .id_inst_valid_o       (id_inst_valid),
    .id_inst_thread_id_o   (id_inst_thread_id),
    .id_inst_data_o        (id_inst_data),
    .id_intpipe_ctrl_o     (id_intpipe_ctrl),
    .id_vpu_decoder_sigs_o (id_vpu_decoder_sigs),
    .id_vpu_core_ctrl_o    (id_vpu_core_ctrl),
    .debug_thread_o        (frontend_debug_thread),
    .debug_inst_fifo_empty_o(frontend_debug_inst_fifo_empty),
    .debug_inst_fifo_full_o (frontend_debug_inst_fifo_full),
    .debug_tb_inst_valid_o  (frontend_debug_tb_inst_valid),
    .debug_tb_inst_ready_o  (frontend_debug_tb_inst_ready),
    .debug_ffb_wdata_i     ('0),
    .debug_ffb_en_i        ('0),
    .debug_ffb_thread_sel_i(1'b0),
    .debug_ffb_exec_i      ('0),
    .halt_i                ('0),
    .halted_i              ('0)
  );

  prim_fifo_sync #(
    .Width(8),
    .Depth(TxFifoDepth)
  ) u_tx_fifo (
    .clk_i,
    .rst_ni,
    .clr_i    (1'b0),
    .wvalid_i (tx_wvalid),
    .wready_o (tx_wready),
    .wdata_i  (tx_wdata),
    .rvalid_o (tx_rvalid),
    .rready_i (tx_pop_ready),
    .rdata_o  (tx_rdata),
    .depth_o  (tx_depth),
    .full_o   (tx_full),
    .empty_o  (tx_empty)
  );

  ulx3s_uart_tx #(
    .ClkHz(ClkHz),
    .BaudRate(BaudRate)
  ) u_tx (
    .clk_i,
    .rst_ni,
    .valid_i (tx_rvalid),
    .data_i  (tx_rdata),
    .ready_o (tx_rready),
    .tx_o    (uart_tx_o),
    .busy_o  (tx_busy)
  );

  assign tx_pop_ready = tx_rvalid && tx_rready;

  assign tx_status_xor = tx_full ^ tx_empty ^ ^tx_depth;

  assign led_o = {
    boot_pending_q,
    tx_busy,
    msg_active_q,
    trace_done_q,
    trace_fail_q,
    trace_idx_q[2],
    trace_idx_q[1],
    tx_status_xor
  };

endmodule
