// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_intpipe -- direct intpipe_top bring-up harness.
//
// Drives intpipe_top from a tiny scripted issued-instruction source, logs
// accepted issue records, redirect requests, and retired instructions over
// UART, and emits a final PASS/FAIL status line once the expected sequence
// completes or times out.

module test_intpipe
  import minion_pkg::*;
#(
  parameter int unsigned ClkHz           = 25_000_000,
  parameter int unsigned BaudRate        = 115200,
  parameter int unsigned BootDelayCycles = 0,
  parameter int unsigned TxFifoDepth     = 64,
  parameter int unsigned TimeoutCycles   = 4096,
  localparam int unsigned BootDelayW     = (BootDelayCycles > 0) ? $clog2(BootDelayCycles + 1) : 1,
  localparam int unsigned TimeoutW       = $clog2(TimeoutCycles + 1),
  localparam int unsigned MsgIdxW        = 8,
  localparam int unsigned NumIssues      = 16,
  localparam int unsigned NumRedirects   = 3,
  localparam int unsigned NumRetires     = 16,
  localparam int unsigned IssueCountW    = $clog2(NumIssues + 1),
  localparam int unsigned RedirectCountW = $clog2(NumRedirects + 1),
  localparam int unsigned RetireCountW   = $clog2(NumRetires + 1),
  localparam int unsigned SummaryIdxW    = $clog2(NumRetires + 1)
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  output logic       uart_tx_o,
  output logic [7:0] led_o
);

  typedef enum logic [2:0] {
    MsgBoot,
    MsgIssue,
    MsgRedirect,
    MsgRetire,
    MsgStatus
  } msg_kind_e;

  typedef struct packed {
    logic [63:0] pc;
    logic [31:0] inst;
  } issue_t;

  typedef struct packed {
    logic [63:0] pc;
    logic [31:0] inst;
    logic        xcpt;
  } retire_t;

  localparam logic [63:0] ResetVector      = 64'h0000_0000_0000_1000;
  localparam logic [31:0] BootDelayInitVal = BootDelayCycles;
  localparam logic [31:0] TimeoutInitVal   = TimeoutCycles;
  localparam logic [PcSizeExt-1:0] PcStride = {{(PcSizeExt-3){1'b0}}, 3'b100};
  localparam logic [MsgIdxW-1:0] BootMsgLastIdx     = 8'd21;
  localparam logic [MsgIdxW-1:0] IssueMsgLastIdx    = 8'd33;
  localparam logic [MsgIdxW-1:0] RedirectMsgLastIdx = 8'd26;
  localparam logic [MsgIdxW-1:0] RetireMsgLastIdx   = 8'd37;
  localparam logic [MsgIdxW-1:0] StatusMsgLastIdx   = 8'd20;
  localparam logic [IssueCountW-1:0] NumIssuesCount = 5'd16;
  localparam logic [IssueCountW-1:0] LastIssueIdxCount = 5'd15;
  localparam logic [RedirectCountW-1:0] NumRedirectsCount = 2'd3;
  localparam logic [RetireCountW-1:0] NumRetiresCount = 5'd16;

  logic [BootDelayW-1:0]    boot_delay_q;
  logic                     boot_pending_q;
  logic                     summary_pending_q;
  logic                     summary_started_q;
  logic                     msg_active_q;
  msg_kind_e                msg_kind_q;
  logic [MsgIdxW-1:0]       msg_idx_q;
  logic [3:0]               msg_item_idx_q;
  logic [SummaryIdxW-1:0]   print_issue_idx_q;
  logic [SummaryIdxW-1:0]   print_redirect_idx_q;
  logic [SummaryIdxW-1:0]   print_retire_idx_q;

  logic [7:0]               tx_wdata;
  logic                     tx_wvalid;
  logic                     tx_wready;
  logic [7:0]               tx_rdata;
  logic                     tx_rvalid;
  logic                     tx_rready;
  logic                     tx_pop_ready;
  logic                     tx_empty;
  logic                     tx_busy;

  logic                     src_active_q;
  logic                     src_active_d;
  logic [PcSizeExt-1:0]     src_pc_q;
  logic [PcSizeExt-1:0]     src_pc_d;
  logic                     ctrl_wait_q;
  logic                     ctrl_wait_d;
  logic [PcSizeExt-1:0]     ctrl_pc_q;
  logic [PcSizeExt-1:0]     ctrl_pc_d;
  logic [PcSizeExt-1:0]     ctrl_fallthrough_pc_q;
  logic [PcSizeExt-1:0]     ctrl_fallthrough_pc_d;
  logic                     redirect_prev_valid_q;
  logic [PcSizeExt-1:0]     redirect_prev_pc_q;

  logic [TimeoutW-1:0]      timeout_q;
  logic                     done_q;
  logic                     pass_q;
  logic                     fail_q;
  logic                     issue_overflow_q;
  logic                     redirect_overflow_q;
  logic                     retire_overflow_q;
  logic                     bad_thread_req_q;
  logic                     bad_thread_retire_q;

  logic [IssueCountW-1:0]   issue_count_q;
  issue_t                   issue_q [NumIssues];
  logic [RedirectCountW-1:0] redirect_count_q;
  logic [NumRedirects-1:0]  redirect_valid_q;
  logic [63:0]              redirect_pc0_q;
  logic [63:0]              redirect_pc1_q;
  logic [63:0]              redirect_pc2_q;
  logic [RetireCountW-1:0]  retire_count_q;
  retire_t                  retire_q [NumRetires];

  logic [NrThreads-1:0]     id_fe_req_valid;
  minion_fe_req [NrThreads-1:0] id_fe_req;
  logic                     id_fe_resp_ready;
  logic                     id_fe_resp_valid;
  logic                     id_fe_resp_thread_id;
  frontend_core_resp        id_fe_resp;
  minion_control_t          id_ctrl;

  logic                     wb_valid;
  logic                     wb_thread_id;
  logic [PcSizeExt-1:0]     wb_reg_pc;
  logic [InstSize-1:0]      te_wb_reg_inst;
  logic                     te_exception;

  logic                     issue_fire;
  logic                     issue_is_ctrl_flow;
  logic                     redirect_valid_t0;
  logic                     redirect_new_t0;
  logic                     retire_fire_t0;
  logic                     control_retire_release;
  logic                     boot_delay_done;
  logic                     summary_done;
  logic                     summary_pass;
  logic                     redirect_print_valid;
  logic [63:0]              redirect_print_pc;

  function automatic logic [31:0] program_inst(input logic [63:0] pc);
    unique case (pc)
      64'h0000_0000_0000_1000: program_inst = 32'h0010_0093;  // addi x1,  x0, 1
      64'h0000_0000_0000_1004: program_inst = 32'h0020_0113;  // addi x2,  x0, 2
      64'h0000_0000_0000_1008: program_inst = 32'h0020_8463;  // beq  x1,  x2, +8  (not taken)
      64'h0000_0000_0000_100C: program_inst = 32'h0030_0193;  // addi x3,  x0, 3
      64'h0000_0000_0000_1010: program_inst = 32'h0010_8463;  // beq  x1,  x1, +8  (taken)
      64'h0000_0000_0000_1014: program_inst = 32'h0070_0393;  // skipped on taken branch
      64'h0000_0000_0000_1018: program_inst = 32'h0080_006F;  // jal  x0, +8
      64'h0000_0000_0000_101C: program_inst = 32'h0080_0413;  // skipped on jal
      64'h0000_0000_0000_1020: program_inst = 32'h0220_8233;  // mul  x4, x1, x2
      64'h0000_0000_0000_1024: program_inst = 32'h0212_42B3;  // div  x5, x4, x1
      64'h0000_0000_0000_1028: program_inst = 32'h3401_9073;  // csrrw x0, mscratch, x3
      64'h0000_0000_0000_102C: program_inst = 32'h0000_0393;  // addi x7, x0, 0  (CSR write/read bubble 1)
      64'h0000_0000_0000_1030: program_inst = 32'h0000_0413;  // addi x8, x0, 0  (CSR write/read bubble 2)
      64'h0000_0000_0000_1034: program_inst = 32'h0000_0493;  // addi x9,  x0, 0  (CSR write/read bubble 3)
      64'h0000_0000_0000_1038: program_inst = 32'h3400_2373;  // csrrs x6, mscratch, x0
      64'h0000_0000_0000_103C: program_inst = 32'h0000_0693;  // addi x13, x0, 0  (CSR read/branch bubble)
      64'h0000_0000_0000_1040: program_inst = 32'h0033_0463;  // beq  x6, x3, +8  (CSR p2 data check)
      64'h0000_0000_0000_1044: program_inst = 32'h0090_0593;  // fail path if CSR readback is wrong
      64'h0000_0000_0000_1048: program_inst = 32'h00A0_0613;  // addi x12, x0, 10
      default:                 program_inst = 32'h0000_0013;  // nop
    endcase
  endfunction

  function automatic logic [63:0] expected_issue_pc(input int unsigned idx);
    unique case (idx)
      0:       expected_issue_pc = 64'h0000_0000_0000_1000;
      1:       expected_issue_pc = 64'h0000_0000_0000_1004;
      2:       expected_issue_pc = 64'h0000_0000_0000_1008;
      3:       expected_issue_pc = 64'h0000_0000_0000_100C;
      4:       expected_issue_pc = 64'h0000_0000_0000_1010;
      5:       expected_issue_pc = 64'h0000_0000_0000_1018;
      6:       expected_issue_pc = 64'h0000_0000_0000_1020;
      7:       expected_issue_pc = 64'h0000_0000_0000_1024;
      8:       expected_issue_pc = 64'h0000_0000_0000_1028;
      9:       expected_issue_pc = 64'h0000_0000_0000_102C;
      10:      expected_issue_pc = 64'h0000_0000_0000_1030;
      11:      expected_issue_pc = 64'h0000_0000_0000_1034;
      12:      expected_issue_pc = 64'h0000_0000_0000_1038;
      13:      expected_issue_pc = 64'h0000_0000_0000_103C;
      14:      expected_issue_pc = 64'h0000_0000_0000_1040;
      default: expected_issue_pc = 64'h0000_0000_0000_1048;
    endcase
  endfunction

  function automatic logic [63:0] expected_redirect_pc(input int unsigned idx);
    unique case (idx)
      0:       expected_redirect_pc = 64'h0000_0000_0000_1018;
      1:       expected_redirect_pc = 64'h0000_0000_0000_1020;
      default: expected_redirect_pc = 64'h0000_0000_0000_1048;
    endcase
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

  function automatic logic [7:0] hex64_digit_ascii(
    input logic [63:0] value,
    input logic [4:0]  digit_idx
  );
    hex64_digit_ascii = hex_nibble_ascii(value[((5'd15 - digit_idx) * 4) +: 4]);
  endfunction

  function automatic logic [7:0] hex32_digit_ascii(
    input logic [31:0] value,
    input logic [3:0]  digit_idx
  );
    hex32_digit_ascii = hex_nibble_ascii(value[((4'd7 - digit_idx) * 4) +: 4]);
  endfunction

  function automatic logic [7:0] summary_idx_ascii(input logic [3:0] idx);
    summary_idx_ascii = hex_nibble_ascii(idx[3:0]);
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
      13: boot_msg_byte = 8'h69;  // i
      14: boot_msg_byte = 8'h6E;  // n
      15: boot_msg_byte = 8'h74;  // t
      16: boot_msg_byte = 8'h70;  // p
      17: boot_msg_byte = 8'h69;  // i
      18: boot_msg_byte = 8'h70;  // p
      19: boot_msg_byte = 8'h65;  // e
      20: boot_msg_byte = 8'h0D;  // \r
      default: boot_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] status_msg_byte(
    input logic pass_i,
    input logic [MsgIdxW-1:0] idx
  );
    unique case (idx)
      0:  status_msg_byte = 8'h54;  // T
      1:  status_msg_byte = 8'h45;  // E
      2:  status_msg_byte = 8'h53;  // S
      3:  status_msg_byte = 8'h54;  // T
      4:  status_msg_byte = 8'h20;  // space
      5:  status_msg_byte = 8'h49;  // I
      6:  status_msg_byte = 8'h4E;  // N
      7:  status_msg_byte = 8'h54;  // T
      8:  status_msg_byte = 8'h50;  // P
      9:  status_msg_byte = 8'h49;  // I
      10: status_msg_byte = 8'h50;  // P
      11: status_msg_byte = 8'h45;  // E
      12: status_msg_byte = 8'h20;  // space
      13: status_msg_byte = pass_i ? 8'h50 : 8'h46;  // P/F
      14: status_msg_byte = pass_i ? 8'h41 : 8'h41;  // A/A
      15: status_msg_byte = pass_i ? 8'h53 : 8'h49;  // S/I
      16: status_msg_byte = pass_i ? 8'h53 : 8'h4C;  // S/L
      17: status_msg_byte = pass_i ? 8'h45 : 8'h45;  // E/E
      18: status_msg_byte = pass_i ? 8'h44 : 8'h44;  // D/D
      19: status_msg_byte = 8'h0D;  // \r
      default: status_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] issue_msg_byte(
    input logic [3:0]             which_i,
    input issue_t                 issue_i,
    input logic [MsgIdxW-1:0]     idx
  );
    unique case (idx)
      0:  issue_msg_byte = 8'h49;  // I
      1:  issue_msg_byte = summary_idx_ascii(which_i[3:0]);
      2:  issue_msg_byte = 8'h20;  // space
      3:  issue_msg_byte = 8'h50;  // P
      4:  issue_msg_byte = 8'h3D;  // =
      5:  issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd0);
      6:  issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd1);
      7:  issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd2);
      8:  issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd3);
      9:  issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd4);
      10: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd5);
      11: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd6);
      12: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd7);
      13: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd8);
      14: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd9);
      15: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd10);
      16: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd11);
      17: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd12);
      18: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd13);
      19: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd14);
      20: issue_msg_byte = hex64_digit_ascii(issue_i.pc, 5'd15);
      21: issue_msg_byte = 8'h20;  // space
      22: issue_msg_byte = 8'h49;  // I
      23: issue_msg_byte = 8'h3D;  // =
      24: issue_msg_byte = hex32_digit_ascii(issue_i.inst, 4'd0);
      25: issue_msg_byte = hex32_digit_ascii(issue_i.inst, 4'd1);
      26: issue_msg_byte = hex32_digit_ascii(issue_i.inst, 4'd2);
      27: issue_msg_byte = hex32_digit_ascii(issue_i.inst, 4'd3);
      28: issue_msg_byte = hex32_digit_ascii(issue_i.inst, 4'd4);
      29: issue_msg_byte = hex32_digit_ascii(issue_i.inst, 4'd5);
      30: issue_msg_byte = hex32_digit_ascii(issue_i.inst, 4'd6);
      31: issue_msg_byte = hex32_digit_ascii(issue_i.inst, 4'd7);
      32: issue_msg_byte = 8'h0D;  // \r
      default: issue_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] redirect_msg_byte(
    input logic [3:0]             which_i,
    input logic                   redirect_valid_i,
    input logic [63:0]            redirect_pc_i,
    input logic [MsgIdxW-1:0]     idx
  );
    unique case (idx)
      0:  redirect_msg_byte = 8'h4A;  // J
      1:  redirect_msg_byte = summary_idx_ascii(which_i[3:0]);
      2:  redirect_msg_byte = 8'h20;  // space
      3:  redirect_msg_byte = 8'h56;  // V
      4:  redirect_msg_byte = 8'h3D;  // =
      5:  redirect_msg_byte = redirect_valid_i ? 8'h31 : 8'h30;
      6:  redirect_msg_byte = 8'h20;  // space
      7:  redirect_msg_byte = 8'h50;  // P
      8:  redirect_msg_byte = 8'h3D;  // =
      9:  redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd0);
      10: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd1);
      11: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd2);
      12: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd3);
      13: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd4);
      14: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd5);
      15: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd6);
      16: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd7);
      17: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd8);
      18: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd9);
      19: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd10);
      20: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd11);
      21: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd12);
      22: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd13);
      23: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd14);
      24: redirect_msg_byte = hex64_digit_ascii(redirect_pc_i, 5'd15);
      25: redirect_msg_byte = 8'h0D;  // \r
      default: redirect_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] retire_msg_byte(
    input logic [3:0]             which_i,
    input retire_t                retire_i,
    input logic [MsgIdxW-1:0]     idx
  );
    unique case (idx)
      0:  retire_msg_byte = 8'h52;  // R
      1:  retire_msg_byte = summary_idx_ascii(which_i[3:0]);
      2:  retire_msg_byte = 8'h20;  // space
      3:  retire_msg_byte = 8'h50;  // P
      4:  retire_msg_byte = 8'h3D;  // =
      5:  retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd0);
      6:  retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd1);
      7:  retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd2);
      8:  retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd3);
      9:  retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd4);
      10: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd5);
      11: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd6);
      12: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd7);
      13: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd8);
      14: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd9);
      15: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd10);
      16: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd11);
      17: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd12);
      18: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd13);
      19: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd14);
      20: retire_msg_byte = hex64_digit_ascii(retire_i.pc, 5'd15);
      21: retire_msg_byte = 8'h20;  // space
      22: retire_msg_byte = 8'h49;  // I
      23: retire_msg_byte = 8'h3D;  // =
      24: retire_msg_byte = hex32_digit_ascii(retire_i.inst, 4'd0);
      25: retire_msg_byte = hex32_digit_ascii(retire_i.inst, 4'd1);
      26: retire_msg_byte = hex32_digit_ascii(retire_i.inst, 4'd2);
      27: retire_msg_byte = hex32_digit_ascii(retire_i.inst, 4'd3);
      28: retire_msg_byte = hex32_digit_ascii(retire_i.inst, 4'd4);
      29: retire_msg_byte = hex32_digit_ascii(retire_i.inst, 4'd5);
      30: retire_msg_byte = hex32_digit_ascii(retire_i.inst, 4'd6);
      31: retire_msg_byte = hex32_digit_ascii(retire_i.inst, 4'd7);
      32: retire_msg_byte = 8'h20;  // space
      33: retire_msg_byte = 8'h58;  // X
      34: retire_msg_byte = 8'h3D;  // =
      35: retire_msg_byte = retire_i.xcpt ? 8'h31 : 8'h30;
      36: retire_msg_byte = 8'h0D;  // \r
      default: retire_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  assign id_fe_resp_valid = src_active_q;
  assign id_fe_resp_thread_id = 1'b0;
  assign issue_fire = id_fe_resp_valid && id_fe_resp_ready;
  assign issue_is_ctrl_flow = id_ctrl.br || id_ctrl.jal || id_ctrl.jalr;
  assign redirect_valid_t0 = id_fe_req_valid[0];
  assign redirect_new_t0 = redirect_valid_t0 &&
                           (!redirect_prev_valid_q || (redirect_prev_pc_q != id_fe_req[0].pc));
  assign retire_fire_t0 = wb_valid && !wb_thread_id;
  assign control_retire_release = ctrl_wait_q && retire_fire_t0 && (wb_reg_pc == ctrl_pc_q);
  assign boot_delay_done = (boot_delay_q == '0);
  assign summary_done = (msg_kind_q == MsgBoot     && msg_idx_q == BootMsgLastIdx)     ||
                        (msg_kind_q == MsgIssue    && msg_idx_q == IssueMsgLastIdx)    ||
                        (msg_kind_q == MsgRedirect && msg_idx_q == RedirectMsgLastIdx) ||
                        (msg_kind_q == MsgRetire   && msg_idx_q == RetireMsgLastIdx)   ||
                        (msg_kind_q == MsgStatus   && msg_idx_q == StatusMsgLastIdx);

  always_comb begin
    src_active_d          = src_active_q;
    src_pc_d              = src_pc_q;
    ctrl_wait_d           = ctrl_wait_q;
    ctrl_pc_d             = ctrl_pc_q;
    ctrl_fallthrough_pc_d = ctrl_fallthrough_pc_q;

    if (redirect_new_t0) begin
      if (issue_count_q < NumIssuesCount) begin
        src_active_d = 1'b1;
        src_pc_d = id_fe_req[0].pc;
      end else begin
        src_active_d = 1'b0;
      end
      ctrl_wait_d = 1'b0;
    end

    if (issue_fire) begin
      if (issue_count_q == LastIssueIdxCount) begin
        src_active_d = 1'b0;
        ctrl_wait_d = 1'b0;
      end else if (issue_is_ctrl_flow) begin
        src_active_d = 1'b0;
        ctrl_wait_d = 1'b1;
        ctrl_pc_d = src_pc_q;
        ctrl_fallthrough_pc_d = src_pc_q + PcStride;
      end else if (!redirect_new_t0) begin
        src_active_d = 1'b1;
        src_pc_d = src_pc_q + PcStride;
      end
    end

    if (control_retire_release && !redirect_new_t0) begin
      ctrl_wait_d = 1'b0;
      if (issue_count_q < NumIssuesCount) begin
        src_active_d = 1'b1;
        src_pc_d = ctrl_fallthrough_pc_q;
      end else begin
        src_active_d = 1'b0;
      end
    end
  end

  intpipe_decode u_decode (
    .inst_bits (id_fe_resp.inst_bits),
    .inst_ctrl (id_ctrl)
  );

  always_comb begin
    id_fe_resp = '0;
    id_fe_resp.pc = src_pc_q;
    id_fe_resp.inst_bits = program_inst({{(64-PcSizeExt){1'b0}}, src_pc_q});
  end

  always_comb begin
    summary_pass = !issue_overflow_q &&
                   !redirect_overflow_q &&
                   !retire_overflow_q &&
                   !bad_thread_req_q &&
                   !bad_thread_retire_q &&
                   (issue_count_q == NumIssuesCount) &&
                   (redirect_count_q == NumRedirectsCount) &&
                   (retire_count_q == NumRetiresCount);

    for (int i = 0; i < NumIssues; i++) begin
      summary_pass &= (issue_q[i].pc == expected_issue_pc(i)) &&
                      (issue_q[i].inst == program_inst(expected_issue_pc(i)));
    end

    summary_pass &= (&redirect_valid_q) &&
                    (redirect_pc0_q == expected_redirect_pc(0)) &&
                    (redirect_pc1_q == expected_redirect_pc(1)) &&
                    (redirect_pc2_q == expected_redirect_pc(2));

    for (int i = 0; i < NumRetires; i++) begin
      summary_pass &= (retire_q[i].pc == expected_issue_pc(i)) &&
                      (retire_q[i].inst == program_inst(expected_issue_pc(i))) &&
                      !retire_q[i].xcpt;
    end
  end

  always_comb begin
    redirect_print_valid = 1'b0;
    redirect_print_pc = '0;
    unique case (msg_item_idx_q[1:0])
      2'd0: begin
        redirect_print_valid = redirect_valid_q[0];
        redirect_print_pc = redirect_pc0_q;
      end
      2'd1: begin
        redirect_print_valid = redirect_valid_q[1];
        redirect_print_pc = redirect_pc1_q;
      end
      default: begin
        redirect_print_valid = redirect_valid_q[2];
        redirect_print_pc = redirect_pc2_q;
      end
    endcase
  end

  always_comb begin
    tx_wvalid = 1'b0;
    tx_wdata  = 8'h00;

    if (msg_active_q && tx_wready) begin
      tx_wvalid = 1'b1;
      unique case (msg_kind_q)
        MsgBoot:     tx_wdata = boot_msg_byte(msg_idx_q);
        MsgIssue:    tx_wdata = issue_msg_byte(msg_item_idx_q[3:0], issue_q[msg_item_idx_q[3:0]], msg_idx_q);
        MsgRedirect: tx_wdata = redirect_msg_byte(msg_item_idx_q[3:0],
                                                  redirect_print_valid,
                                                  redirect_print_pc,
                                                  msg_idx_q);
        MsgRetire:   tx_wdata = retire_msg_byte(msg_item_idx_q[3:0], retire_q[msg_item_idx_q[3:0]], msg_idx_q);
        default:     tx_wdata = status_msg_byte(pass_q, msg_idx_q);
      endcase
    end
  end

  /* verilator lint_off PINCONNECTEMPTY */  // The focused harness drives only the intpipe-facing interfaces needed for scalar/control/CSR bring-up and leaves unrelated SoC outputs open.
  intpipe_top #(
    .VpuEn(1'b0)
  ) u_intpipe (
    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),
    .rst_w_ni               (rst_ni),
    .rst_d_ni               (rst_ni),
    .te_enable              (1'b1),
    .dft__reset_byp         (1'b0),
    .dft__reset             (1'b0),
    .interrupts             ('0),
    .shire_id               ('0),
    .shire_min_id           ('0),
    .chicken_bit_intpipe    (1'b0),
    .id_fe_req_valid        (id_fe_req_valid),
    .id_fe_req              (id_fe_req),
    .id_fe_stall            (),
    .id_fe_resp_ready       (id_fe_resp_ready),
    .id_fe_resp_valid       (id_fe_resp_valid),
    .id_fe_resp_thread_id   (id_fe_resp_thread_id),
    .id_fe_resp             (id_fe_resp),
    .icache_invalidate      (),
    .dcache_ctrl            (),
    .dcache_ctrl_resp       ('0),
    .vpu_ctrl               (),
    .id_dcache_alloc_rq_pre (),
    .ex_dcache_alloc_rq_val (),
    .id_dcache_gsc          (),
    .id_dcache_ready        (1'b1),
    .ex_dcache_req_valid    (),
    .ex_dcache_req          (),
    .ex_dcache_gsc          (),
    .tag_dcache_kill        (),
    .tag_dcache_store_data  (),
    .mem_dcache_kill        (),
    .wb_dcache_x31          (),
    .mem_dcache_resp_int_valid(1'b0),
    .wb_dcache_resp_valid   (1'b0),
    .wb_dcache_resp         ('0),
    .tag_dcache_bp          ('0),
    .tag_dcache_bp_valid    (1'b0),
    .id_ctrl                (id_ctrl),
    .id_dcache_ordered      ('1),
    .id_dcache_idle         (1'b1),
    .id_dcache_scoreboard   ('0),
    .id_dcache_sb_int_dealloc('0),
    .id_dcache_sb_fp_dealloc('0),
    .tag_dcache_xcpt        ('0),
    .tag_dcache_replay_next (1'b0),
    .mem_dcache_flush_pipeline(1'b0),
    .wb_dcache_invalidate_lr(),
    .wb_dcache_fp_toint     (),
    .dcache_bus_error       ('0),
    .dcache_bus_error_addr  ('0),
`ifdef DCACHE_REPORT_PC
    .dcache_bus_error_pc    ('0),
`endif
    .satp_info              (),
    .matp_info              (),
    .satp_info_en           (),
    .matp_info_en           (),
    .vm_status              (),
    .tlb_invalidate         (),
    .l2_resp_valid          (1'b0),
    .l2_resp_ready          (1'b1),
    .l2_resp                ('0),
    .id_vpu_req             (),
    .ex_vpu_req             (),
    .tag_vpu_kill           (),
    .mem_vpu_kill           (),
    .wb_vpu_kill            (),
    .wb_vpu_dcache_resp_valid(),
    .wb_vpu_dcache_resp     (),
    .thread0_enable         (1'b1),
    .thread1_enable         (1'b0),
    .id_frontend_vpu_ctrl   ('0),
    .id_vpu_ctrl            ('0),
    .ex_vpu_ctrl            ('0),
    .tag_vpu_ctrl           ('0),
    .mem_vpu_ctrl           ('0),
    .wb_vpu_ctrl            ('0),
    .vpu_tfma_tenb_working  (1'b0),
    .flb_neigh_req_valid    (),
    .flb_neigh_req_data     (),
    .flb_neigh_resp_valid   (1'b0),
    .flb_neigh_resp_data    (1'b0),
    .TE_wb_reg_inst         (te_wb_reg_inst),
    .TE_wb_reg_cause_ie     (),
    .TE_wb_reg_context      (),
    .TE_tval                (),
    .TE_prv                 (),
    .TE_exception           (te_exception),
    .te_thread_sel          (1'b0),
    .update_ddata0          (),
    .ddata0_wdata           (),
    .read_ddata0            ('0),
    .debug_ex_program_buffer('0),
    .halt                   ('0),
    .resume                 ('0),
    .in_debug_mode          (),
    .debug_busy             (),
    .debug_exception        (),
    .debug_monitor_out      (),
    .wb_valid               (wb_valid),
    .wb_thread_id           (wb_thread_id),
    .wb_reg_pc              (wb_reg_pc),
    .esr_shire_coop_mode    (1'b0),
    .esr_features           ('0),
    .tensor_load_err_flags  ('0),
    .cache_ops_err_flags    ('0),
    .tensor_reduce_err_flags(1'b0),
    .pmu_count_up           (),
    .pmu_read_data          ('0),
    .pmu_read_sel           (),
    .pmu_write_en           (),
    .pmu_write_data         (),
    .pmu_neigh_event_sel    (),
    .io_events_dcache_vpu   ('0),
    .dcache_debug_signals   ('0)
  );
  /* verilator lint_on PINCONNECTEMPTY */

  /* verilator lint_off SYNCASYNCNET */  // The harness intentionally uses async-assert top reset in local state flops while submodules also observe the synchronized functional clock domain.
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      timeout_q            <= TimeoutInitVal[TimeoutW-1:0];
      done_q               <= 1'b0;
      pass_q               <= 1'b0;
      fail_q               <= 1'b0;
      src_active_q         <= 1'b1;
      src_pc_q             <= ResetVector[PcSizeExt-1:0];
      ctrl_wait_q          <= 1'b0;
      ctrl_pc_q            <= '0;
      ctrl_fallthrough_pc_q <= '0;
      redirect_prev_valid_q <= 1'b0;
      redirect_prev_pc_q   <= '0;
      issue_overflow_q     <= 1'b0;
      redirect_overflow_q  <= 1'b0;
      retire_overflow_q    <= 1'b0;
      bad_thread_req_q     <= 1'b0;
      bad_thread_retire_q  <= 1'b0;
      issue_count_q        <= '0;
      redirect_count_q     <= '0;
      retire_count_q       <= '0;
      for (int i = 0; i < NumIssues; i++) begin
        issue_q[i] <= '0;
      end
      redirect_valid_q <= '0;
      redirect_pc0_q <= '0;
      redirect_pc1_q <= '0;
      redirect_pc2_q <= '0;
      for (int i = 0; i < NumRetires; i++) begin
        retire_q[i] <= '0;
      end
    end else begin
      if (!done_q && timeout_q != '0) begin
        timeout_q <= timeout_q - 1'b1;
      end

      if (id_fe_req_valid[1] ||
          (|id_fe_req[1].pc) ||
          id_fe_req[1].speculative ||
          id_fe_req[1].debug_info.halt ||
          id_fe_req[1].debug_info.resume) begin
        bad_thread_req_q <= 1'b1;
      end

      if (redirect_new_t0) begin
        if (redirect_count_q < NumRedirectsCount) begin
          unique case (redirect_count_q)
            2'd0: begin
              redirect_valid_q[0] <= 1'b1;
              redirect_pc0_q <= {{(64-PcSizeExt){1'b0}}, id_fe_req[0].pc};
            end
            2'd1: begin
              redirect_valid_q[1] <= 1'b1;
              redirect_pc1_q <= {{(64-PcSizeExt){1'b0}}, id_fe_req[0].pc};
            end
            default: begin
              redirect_valid_q[2] <= 1'b1;
              redirect_pc2_q <= {{(64-PcSizeExt){1'b0}}, id_fe_req[0].pc};
            end
          endcase
          redirect_count_q <= redirect_count_q + 1'b1;
        end else begin
          redirect_overflow_q <= 1'b1;
        end
      end

      if (issue_fire) begin
        if (issue_count_q < NumIssuesCount) begin
          issue_q[issue_count_q[3:0]].pc <= {{(64-PcSizeExt){1'b0}}, src_pc_q};
          issue_q[issue_count_q[3:0]].inst <= id_fe_resp.inst_bits;
          issue_count_q <= issue_count_q + 1'b1;
        end else begin
          issue_overflow_q <= 1'b1;
        end
      end

      if (wb_valid && wb_thread_id) begin
        bad_thread_retire_q <= 1'b1;
      end

      if (retire_fire_t0) begin
        if (retire_count_q < NumRetiresCount) begin
          retire_q[retire_count_q[3:0]].pc <= {{(64-PcSizeExt){1'b0}}, wb_reg_pc};
          retire_q[retire_count_q[3:0]].inst <= te_wb_reg_inst;
          retire_q[retire_count_q[3:0]].xcpt <= te_exception;
          retire_count_q <= retire_count_q + 1'b1;
        end else begin
          retire_overflow_q <= 1'b1;
        end
      end

      src_active_q <= src_active_d;
      src_pc_q <= src_pc_d;
      ctrl_wait_q <= ctrl_wait_d;
      ctrl_pc_q <= ctrl_pc_d;
      ctrl_fallthrough_pc_q <= ctrl_fallthrough_pc_d;
      redirect_prev_valid_q <= redirect_valid_t0;
      if (redirect_valid_t0) begin
        redirect_prev_pc_q <= id_fe_req[0].pc;
      end

      if (!done_q && (
            timeout_q == '0 ||
            issue_overflow_q ||
            redirect_overflow_q ||
            retire_overflow_q ||
            bad_thread_req_q ||
            bad_thread_retire_q ||
            (!src_active_d && (retire_count_q == NumRetiresCount))
          )) begin
        done_q <= 1'b1;
        pass_q <= summary_pass;
        fail_q <= !summary_pass;
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      boot_delay_q      <= BootDelayInitVal[BootDelayW-1:0];
      boot_pending_q    <= 1'b1;
      summary_pending_q <= 1'b0;
      summary_started_q <= 1'b0;
      msg_active_q      <= 1'b0;
      msg_kind_q        <= MsgBoot;
      msg_idx_q         <= '0;
      msg_item_idx_q    <= '0;
      print_issue_idx_q <= '0;
      print_redirect_idx_q <= '0;
      print_retire_idx_q <= '0;
    end else begin
      if (!boot_delay_done) begin
        boot_delay_q <= boot_delay_q - 1'b1;
      end

      if (done_q && !summary_started_q) begin
        summary_pending_q <= 1'b1;
        summary_started_q <= 1'b1;
      end

      if (msg_active_q && tx_wready) begin
        if (summary_done) begin
          msg_active_q <= 1'b0;
          msg_idx_q <= '0;
          unique case (msg_kind_q)
            MsgBoot: begin
              boot_pending_q <= 1'b0;
            end
            MsgIssue: begin
              print_issue_idx_q <= print_issue_idx_q + 1'b1;
            end
            MsgRedirect: begin
              print_redirect_idx_q <= print_redirect_idx_q + 1'b1;
            end
            MsgRetire: begin
              print_retire_idx_q <= print_retire_idx_q + 1'b1;
            end
            default: begin
              summary_pending_q <= 1'b0;
            end
          endcase
        end else begin
          msg_idx_q <= msg_idx_q + 1'b1;
        end
      end else if (!msg_active_q && boot_delay_done) begin
        if (boot_pending_q) begin
          msg_active_q <= 1'b1;
          msg_kind_q <= MsgBoot;
          msg_idx_q <= '0;
          msg_item_idx_q <= '0;
        end else if (summary_pending_q) begin
          if (print_issue_idx_q < issue_count_q) begin
            msg_active_q <= 1'b1;
            msg_kind_q <= MsgIssue;
            msg_idx_q <= '0;
            msg_item_idx_q <= print_issue_idx_q[3:0];
          end else if (print_redirect_idx_q[RedirectCountW-1:0] < redirect_count_q) begin
            msg_active_q <= 1'b1;
            msg_kind_q <= MsgRedirect;
            msg_idx_q <= '0;
            msg_item_idx_q <= {{2{1'b0}}, print_redirect_idx_q[RedirectCountW-1:0]};
          end else if (print_retire_idx_q < retire_count_q) begin
            msg_active_q <= 1'b1;
            msg_kind_q <= MsgRetire;
            msg_idx_q <= '0;
            msg_item_idx_q <= print_retire_idx_q[3:0];
          end else begin
            msg_active_q <= 1'b1;
            msg_kind_q <= MsgStatus;
            msg_idx_q <= '0;
            msg_item_idx_q <= '0;
          end
        end
      end
    end
  end
  /* verilator lint_on SYNCASYNCNET */

  /* verilator lint_off PINCONNECTEMPTY */  // The harness only consumes FIFO emptiness and ready/valid behavior; depth/full are intentionally unobserved.
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
    .depth_o  (),
    .full_o   (),
    .empty_o  (tx_empty)
  );
  /* verilator lint_on PINCONNECTEMPTY */

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

  assign tx_pop_ready = tx_rvalid & tx_rready;

  assign led_o = {
    done_q,
    pass_q,
    fail_q,
    src_active_q,
    ctrl_wait_q,
    wb_valid,
    tx_busy,
    !tx_empty
  };

endmodule
