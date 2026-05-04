// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_mul_div -- direct intpipe_mul_div_top bring-up harness.

module test_mul_div
  import minion_pkg::*;
#(
  parameter int unsigned ClkHz           = 25_000_000,
  parameter int unsigned BaudRate        = 115200,
  parameter int unsigned BootDelayCycles = 0,
  parameter int unsigned TimeoutCycles   = 512,
  localparam int unsigned BootDelayW     = (BootDelayCycles > 0) ? $clog2(BootDelayCycles + 1) : 1,
  localparam int unsigned TimeoutW       = (TimeoutCycles > 0) ? $clog2(TimeoutCycles + 1) : 1,
  localparam int unsigned NumCases       = 22,
  localparam int unsigned CaseIdxW       = $clog2(NumCases + 1),
  localparam int unsigned MsgIdxW        = 7
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  output logic       uart_tx_o,
  output logic [7:0] led_o
);

  typedef enum logic [1:0] {
    ResNormal,
    ResStall,
    ResKill
  } result_kind_e;

  typedef enum logic [3:0] {
    StWaitReady,
    StIssueEarly,
    StIssueValid,
    StWaitResp,
    StStallWaitResp,
    StStallHold,
    StStallConsume,
    StKillRun,
    StKillPulse,
    StKillWaitReady,
    StKillRecoverWaitReady,
    StKillRecoverEarly,
    StKillRecoverValid,
    StKillRecoverResp
  } run_state_e;

  typedef enum logic [1:0] {
    MsgBoot,
    MsgResult,
    MsgStatus
  } msg_kind_e;

  typedef struct packed {
    result_kind_e      kind;
    logic [CaseIdxW-1:0] index;
    logic [4:0]        fn;
    logic              dw;
    logic              pass;
    logic              aux_ok;
    logic [63:0]       got;
    logic [63:0]       exp;
    logic [6:0]        dest;
  } summary_t;

  localparam logic [4:0] FnMul    = 5'b10000;
  localparam logic [4:0] FnMulh   = 5'b10001;
  localparam logic [4:0] FnDiv    = 5'b10100;
  localparam logic [4:0] FnDivu   = 5'b10101;
  localparam logic [4:0] FnRem    = 5'b10110;
  localparam logic [4:0] FnRemu   = 5'b10111;
  localparam logic [4:0] FnMulhsu = 5'b11100;
  localparam logic [4:0] FnMulhu  = 5'b11110;

  localparam logic [4:0] RecoveryFn   = FnMul;
  localparam logic       RecoveryDw   = 1'b1;
  localparam logic [63:0] RecoveryIn1 = 64'd7;
  localparam logic [63:0] RecoveryIn2 = 64'd11;
  localparam logic [63:0] RecoveryExp = 64'd77;
  localparam logic [6:0]  RecoveryDest = 7'h33;

  localparam logic [31:0] BootDelayInitVal = BootDelayCycles;
  localparam logic [31:0] TimeoutInitVal   = TimeoutCycles;
  localparam logic [CaseIdxW-1:0] StallCaseIdxVal = 5'd20;
  localparam logic [CaseIdxW-1:0] KillCaseIdxVal  = 5'd21;
  localparam logic [CaseIdxW-1:0] LastCaseIdxVal  = 5'd21;
  localparam logic [MsgIdxW-1:0] BootMsgLastIdx      = 7'd21;
  localparam logic [MsgIdxW-1:0] StatusMsgLastIdx    = 7'd20;
  localparam logic [MsgIdxW-1:0] ResultMsgLastIdx    = 7'd60;
  localparam logic [MsgIdxW-1:0] ResultAuxLastIdx    = 7'd64;
  localparam logic [2:0]         StallHoldCycles     = 3'd3;
  localparam logic [3:0]         KillDelayCycles     = 4'd5;

  logic [BootDelayW-1:0]         boot_delay_q;
  logic                          boot_sent_q;
  logic                          status_sent_q;
  logic                          msg_active_q;
  msg_kind_e                     msg_kind_q;
  logic [MsgIdxW-1:0]            msg_idx_q;
  logic [CaseIdxW-1:0]           msg_item_idx_q;
  logic [CaseIdxW-1:0]           print_result_idx_q;

  logic                          tx_valid;
  logic [7:0]                    tx_data;
  logic                          tx_ready;

  logic                          req_valid;
  logic                          req_valid_early;
  logic [4:0]                    req_fn_sel;
  logic [4:0]                    req_fn_early_sel;
  logic                          req_dw_sel;
  logic [63:0]                   req_in1_sel;
  logic [63:0]                   req_in2_sel;
  logic [6:0]                    req_dest_sel;
  logic                          resp_ready_sel;
  logic                          kill_sel;

  logic                          req_ready;
  logic                          resp_valid;
  logic [63:0]                   resp_data;
  logic [6:0]                    resp_dest;

  run_state_e                    run_state_q;
  logic [CaseIdxW-1:0]           case_idx_q;
  logic [TimeoutW-1:0]           timeout_q;
  logic [2:0]                    stall_hold_q;
  logic [63:0]                   hold_data_q;
  logic [6:0]                    hold_dest_q;
  logic [3:0]                    kill_delay_q;
  logic                          kill_aux_ok_q;
  logic                          done_q;
  logic                          fail_q;
  logic                          pass_q;
  summary_t                      selected_result;

  logic [CaseIdxW-1:0]           result_count_q;
  summary_t                      results_q [NumCases];

  function automatic result_kind_e case_kind(input logic [CaseIdxW-1:0] idx);
    if (idx == StallCaseIdxVal) return ResStall;
    if (idx == KillCaseIdxVal)  return ResKill;
    return ResNormal;
  endfunction

  function automatic logic [4:0] case_fn(input logic [CaseIdxW-1:0] idx);
    unique case (idx)
      0:       case_fn = FnMul;
      1:       case_fn = FnMulh;
      2:       case_fn = FnMulhu;
      3:       case_fn = FnMulhsu;
      4:       case_fn = FnDiv;
      5:       case_fn = FnDiv;
      6:       case_fn = FnDivu;
      7:       case_fn = FnDivu;
      8:       case_fn = FnRem;
      9:       case_fn = FnRem;
      10:      case_fn = FnRemu;
      11:      case_fn = FnRemu;
      12:      case_fn = FnMul;
      13:      case_fn = FnMulh;
      14:      case_fn = FnMulhu;
      15:      case_fn = FnMulhsu;
      16:      case_fn = FnDiv;
      17:      case_fn = FnDivu;
      18:      case_fn = FnRem;
      19:      case_fn = FnRemu;
      20:      case_fn = FnDivu;
      default: case_fn = RecoveryFn;
    endcase
  endfunction

  function automatic logic case_dw(input logic [CaseIdxW-1:0] idx);
    unique case (idx)
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 20: case_dw = 1'b1;
      default:                                     case_dw = 1'b0;
    endcase
  endfunction

  function automatic logic [63:0] case_in1(input logic [CaseIdxW-1:0] idx);
    unique case (idx)
      0:       case_in1 = 64'h0000_0000_0000_0002;
      1:       case_in1 = 64'hFFFF_FFFF_FFFF_FFFF;
      2:       case_in1 = 64'h1234_5678_9ABC_DEF0;
      3:       case_in1 = 64'hFFFF_FFFF_FFFF_FFFE;
      4:       case_in1 = 64'h0000_0000_0000_0064;
      5:       case_in1 = 64'h8000_0000_0000_0000;
      6:       case_in1 = 64'h1234_5678_9ABC_DEF0;
      7:       case_in1 = 64'h0000_0000_0000_0005;
      8:       case_in1 = 64'h0000_0000_0000_0064;
      9:       case_in1 = 64'h8000_0000_0000_0000;
      10:      case_in1 = 64'h1234_5678_9ABC_DEF0;
      11:      case_in1 = 64'h0000_0000_0000_0005;
      12:      case_in1 = 64'hFFFF_FFFF_FFFF_FFFF;
      13:      case_in1 = 64'hFFFF_FFFF_8000_0000;
      14:      case_in1 = 64'h0000_0000_FFFF_FFFF;
      15:      case_in1 = 64'hFFFF_FFFF_8000_0000;
      16:      case_in1 = 64'hFFFF_FFFF_8000_0000;
      17:      case_in1 = 64'h0000_0000_DEAD_BEEF;
      18:      case_in1 = 64'h0000_0000_0000_0064;
      19:      case_in1 = 64'h0000_0000_DEAD_BEEF;
      20:      case_in1 = 64'h1234_5678_9ABC_DEF0;
      default: case_in1 = 64'h1234_5678_9ABC_DEF0;
    endcase
  endfunction

  function automatic logic [63:0] case_in2(input logic [CaseIdxW-1:0] idx);
    unique case (idx)
      0:       case_in2 = 64'h0000_0000_0000_0003;
      1:       case_in2 = 64'hFFFF_FFFF_FFFF_FFFF;
      2:       case_in2 = 64'hFEDC_BA98_7654_3210;
      3:       case_in2 = 64'h0000_0000_0000_0003;
      4:       case_in2 = 64'h0000_0000_0000_0007;
      5:       case_in2 = 64'hFFFF_FFFF_FFFF_FFFF;
      6:       case_in2 = 64'h0000_0001_0000_0000;
      7:       case_in2 = 64'h0000_0000_0000_0000;
      8:       case_in2 = 64'h0000_0000_0000_0007;
      9:       case_in2 = 64'hFFFF_FFFF_FFFF_FFFF;
      10:      case_in2 = 64'h0000_0001_0000_0000;
      11:      case_in2 = 64'h0000_0000_0000_0000;
      12:      case_in2 = 64'h0000_0000_0000_0002;
      13:      case_in2 = 64'h0000_0000_0000_0002;
      14:      case_in2 = 64'h0000_0000_FFFF_FFFF;
      15:      case_in2 = 64'h0000_0000_FFFF_FFFF;
      16:      case_in2 = 64'hFFFF_FFFF_FFFF_FFFF;
      17:      case_in2 = 64'h0000_0000_0000_1000;
      18:      case_in2 = 64'h0000_0000_0000_0007;
      19:      case_in2 = 64'h0000_0000_0000_1000;
      20:      case_in2 = 64'h0000_0001_0000_0000;
      default: case_in2 = 64'h0000_0000_0000_0003;
    endcase
  endfunction

  function automatic logic [63:0] case_exp(input logic [CaseIdxW-1:0] idx);
    unique case (idx)
      0:       case_exp = 64'h0000_0000_0000_0006;
      1:       case_exp = 64'h0000_0000_0000_0000;
      2:       case_exp = 64'h121F_A00A_D77D_7422;
      3:       case_exp = 64'hFFFF_FFFF_FFFF_FFFF;
      4:       case_exp = 64'h0000_0000_0000_000E;
      5:       case_exp = 64'h8000_0000_0000_0000;
      6:       case_exp = 64'h0000_0000_1234_5678;
      7:       case_exp = 64'hFFFF_FFFF_FFFF_FFFF;
      8:       case_exp = 64'h0000_0000_0000_0002;
      9:       case_exp = 64'h0000_0000_0000_0000;
      10:      case_exp = 64'h0000_0000_9ABC_DEF0;
      11:      case_exp = 64'h0000_0000_0000_0005;
      12:      case_exp = 64'hFFFF_FFFF_FFFF_FFFE;
      13:      case_exp = 64'hFFFF_FFFF_FFFF_FFFF;
      14:      case_exp = 64'hFFFF_FFFF_FFFF_FFFE;
      15:      case_exp = 64'hFFFF_FFFF_8000_0000;
      16:      case_exp = 64'hFFFF_FFFF_8000_0000;
      17:      case_exp = 64'h0000_0000_000D_EADB;
      18:      case_exp = 64'h0000_0000_0000_0002;
      19:      case_exp = 64'h0000_0000_0000_0EEF;
      20:      case_exp = 64'h0000_0000_1234_5678;
      default: case_exp = RecoveryExp;
    endcase
  endfunction

  function automatic logic [6:0] case_dest(input logic [CaseIdxW-1:0] idx);
    if (idx == StallCaseIdxVal) return 7'h55;
    if (idx == KillCaseIdxVal)  return RecoveryDest;
    return {2'b00, idx[4:0]} + 7'h01;
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

  function automatic logic [7:0] hex68_digit_ascii(
    input logic [67:0] value,
    input logic [4:0]  digit_idx
  );
    hex68_digit_ascii = hex_nibble_ascii(value[((5'd16 - digit_idx) * 4) +: 4]);
  endfunction

  function automatic logic [7:0] boot_msg_byte(input logic [MsgIdxW-1:0] idx);
    unique case (idx)
      0:  boot_msg_byte = 8'h41;
      1:  boot_msg_byte = 8'h69;
      2:  boot_msg_byte = 8'h6E;
      3:  boot_msg_byte = 8'h65;
      4:  boot_msg_byte = 8'h6B;
      5:  boot_msg_byte = 8'h6B;
      6:  boot_msg_byte = 8'h6F;
      7:  boot_msg_byte = 8'h20;
      8:  boot_msg_byte = 8'h74;
      9:  boot_msg_byte = 8'h65;
      10: boot_msg_byte = 8'h73;
      11: boot_msg_byte = 8'h74;
      12: boot_msg_byte = 8'h5F;
      13: boot_msg_byte = 8'h6D;
      14: boot_msg_byte = 8'h75;
      15: boot_msg_byte = 8'h6C;
      16: boot_msg_byte = 8'h5F;
      17: boot_msg_byte = 8'h64;
      18: boot_msg_byte = 8'h69;
      19: boot_msg_byte = 8'h76;
      20: boot_msg_byte = 8'h0D;
      default: boot_msg_byte = 8'h0A;
    endcase
  endfunction

  function automatic logic [7:0] status_msg_byte(
    input logic pass_i,
    input logic [MsgIdxW-1:0] idx
  );
    unique case (idx)
      0:  status_msg_byte = 8'h54;
      1:  status_msg_byte = 8'h45;
      2:  status_msg_byte = 8'h53;
      3:  status_msg_byte = 8'h54;
      4:  status_msg_byte = 8'h20;
      5:  status_msg_byte = 8'h4D;
      6:  status_msg_byte = 8'h55;
      7:  status_msg_byte = 8'h4C;
      8:  status_msg_byte = 8'h20;
      9:  status_msg_byte = 8'h44;
      10: status_msg_byte = 8'h49;
      11: status_msg_byte = 8'h56;
      12: status_msg_byte = 8'h20;
      13: status_msg_byte = pass_i ? 8'h50 : 8'h46;
      14: status_msg_byte = pass_i ? 8'h41 : 8'h41;
      15: status_msg_byte = pass_i ? 8'h53 : 8'h49;
      16: status_msg_byte = pass_i ? 8'h53 : 8'h4C;
      17: status_msg_byte = pass_i ? 8'h45 : 8'h45;
      18: status_msg_byte = pass_i ? 8'h44 : 8'h44;
      19: status_msg_byte = 8'h0D;
      default: status_msg_byte = 8'h0A;
    endcase
  endfunction

  function automatic logic [MsgIdxW-1:0] result_msg_last_idx(input result_kind_e kind_i);
    if (kind_i == ResNormal) return ResultMsgLastIdx;
    return ResultAuxLastIdx;
  endfunction

  function automatic logic [7:0] result_msg_byte(
    input summary_t              item,
    input logic [MsgIdxW-1:0]    idx
  );
    unique case (idx)
      0:  result_msg_byte = (item.kind == ResNormal) ? 8'h54 :
                            (item.kind == ResStall)  ? 8'h53 : 8'h4B;
      1:  result_msg_byte = hex_nibble_ascii({3'b000, item.index[4]});
      2:  result_msg_byte = hex_nibble_ascii(item.index[3:0]);
      3:  result_msg_byte = 8'h20;
      4:  result_msg_byte = 8'h46;
      5:  result_msg_byte = 8'h3D;
      6:  result_msg_byte = hex_nibble_ascii({3'b000, item.fn[4]});
      7:  result_msg_byte = hex_nibble_ascii(item.fn[3:0]);
      8:  result_msg_byte = 8'h20;
      9:  result_msg_byte = 8'h57;
      10: result_msg_byte = 8'h3D;
      11: result_msg_byte = item.dw ? 8'h31 : 8'h30;
      12: result_msg_byte = 8'h20;
      13: result_msg_byte = 8'h50;
      14: result_msg_byte = 8'h3D;
      15: result_msg_byte = item.pass ? 8'h31 : 8'h30;
      16: result_msg_byte = 8'h20;
      17: result_msg_byte = 8'h47;
      18: result_msg_byte = 8'h3D;
      19: result_msg_byte = hex64_digit_ascii(item.got, 5'd0);
      20: result_msg_byte = hex64_digit_ascii(item.got, 5'd1);
      21: result_msg_byte = hex64_digit_ascii(item.got, 5'd2);
      22: result_msg_byte = hex64_digit_ascii(item.got, 5'd3);
      23: result_msg_byte = hex64_digit_ascii(item.got, 5'd4);
      24: result_msg_byte = hex64_digit_ascii(item.got, 5'd5);
      25: result_msg_byte = hex64_digit_ascii(item.got, 5'd6);
      26: result_msg_byte = hex64_digit_ascii(item.got, 5'd7);
      27: result_msg_byte = hex64_digit_ascii(item.got, 5'd8);
      28: result_msg_byte = hex64_digit_ascii(item.got, 5'd9);
      29: result_msg_byte = hex64_digit_ascii(item.got, 5'd10);
      30: result_msg_byte = hex64_digit_ascii(item.got, 5'd11);
      31: result_msg_byte = hex64_digit_ascii(item.got, 5'd12);
      32: result_msg_byte = hex64_digit_ascii(item.got, 5'd13);
      33: result_msg_byte = hex64_digit_ascii(item.got, 5'd14);
      34: result_msg_byte = hex64_digit_ascii(item.got, 5'd15);
      35: result_msg_byte = 8'h20;
      36: result_msg_byte = 8'h45;
      37: result_msg_byte = 8'h3D;
      38: result_msg_byte = hex64_digit_ascii(item.exp, 5'd0);
      39: result_msg_byte = hex64_digit_ascii(item.exp, 5'd1);
      40: result_msg_byte = hex64_digit_ascii(item.exp, 5'd2);
      41: result_msg_byte = hex64_digit_ascii(item.exp, 5'd3);
      42: result_msg_byte = hex64_digit_ascii(item.exp, 5'd4);
      43: result_msg_byte = hex64_digit_ascii(item.exp, 5'd5);
      44: result_msg_byte = hex64_digit_ascii(item.exp, 5'd6);
      45: result_msg_byte = hex64_digit_ascii(item.exp, 5'd7);
      46: result_msg_byte = hex64_digit_ascii(item.exp, 5'd8);
      47: result_msg_byte = hex64_digit_ascii(item.exp, 5'd9);
      48: result_msg_byte = hex64_digit_ascii(item.exp, 5'd10);
      49: result_msg_byte = hex64_digit_ascii(item.exp, 5'd11);
      50: result_msg_byte = hex64_digit_ascii(item.exp, 5'd12);
      51: result_msg_byte = hex64_digit_ascii(item.exp, 5'd13);
      52: result_msg_byte = hex64_digit_ascii(item.exp, 5'd14);
      53: result_msg_byte = hex64_digit_ascii(item.exp, 5'd15);
      54: result_msg_byte = 8'h20;
      55: result_msg_byte = 8'h44;
      56: result_msg_byte = 8'h3D;
      57: result_msg_byte = hex_nibble_ascii({1'b0, item.dest[6:4]});
      58: result_msg_byte = hex_nibble_ascii(item.dest[3:0]);
      59: result_msg_byte = (item.kind == ResNormal) ? 8'h0D : 8'h20;
      60: result_msg_byte = (item.kind == ResNormal) ? 8'h0A :
                            (item.kind == ResStall)  ? 8'h48 : 8'h4E;
      61: result_msg_byte = (item.kind == ResStall)  ? 8'h3D : 8'h3D;
      62: result_msg_byte = item.aux_ok ? 8'h31 : 8'h30;
      63: result_msg_byte = 8'h0D;
      default: result_msg_byte = 8'h0A;
    endcase
  endfunction

  logic active_recovery;
  result_kind_e active_kind;
  logic [4:0] active_fn;
  logic active_dw;
  logic [63:0] active_in1;
  logic [63:0] active_in2;
  logic [63:0] active_exp;
  logic [6:0] active_dest;

  assign active_recovery = (run_state_q == StKillRecoverWaitReady) ||
                           (run_state_q == StKillRecoverEarly) ||
                           (run_state_q == StKillRecoverValid) ||
                           (run_state_q == StKillRecoverResp);
  assign active_kind = case_kind(case_idx_q);

  always_comb begin
    selected_result = results_q[msg_item_idx_q];
  end

  always_comb begin
    if (active_recovery) begin
      active_fn   = RecoveryFn;
      active_dw   = RecoveryDw;
      active_in1  = RecoveryIn1;
      active_in2  = RecoveryIn2;
      active_exp  = RecoveryExp;
      active_dest = RecoveryDest;
    end else begin
      active_fn   = case_fn(case_idx_q);
      active_dw   = case_dw(case_idx_q);
      active_in1  = case_in1(case_idx_q);
      active_in2  = case_in2(case_idx_q);
      active_exp  = case_exp(case_idx_q);
      active_dest = case_dest(case_idx_q);
    end
  end

  always_comb begin
    req_valid       = 1'b0;
    req_valid_early = 1'b0;
    req_fn_sel      = active_fn;
    req_fn_early_sel = active_fn;
    req_dw_sel      = active_dw;
    req_in1_sel     = active_in1;
    req_in2_sel     = active_in2;
    req_dest_sel    = active_dest;
    resp_ready_sel  = 1'b1;
    kill_sel        = 1'b0;

    unique case (run_state_q)
      StIssueEarly,
      StKillRecoverEarly: begin
        req_valid_early = 1'b1;
      end

      StIssueValid,
      StKillRecoverValid: begin
        req_valid = 1'b1;
      end

      StStallWaitResp,
      StStallHold: begin
        resp_ready_sel = 1'b0;
      end

      StKillPulse: begin
        kill_sel = 1'b1;
      end

      default: begin
      end
    endcase
  end

  /* verilator lint_off PINCONNECTEMPTY */  // The harness only checks the committed response path and UART TX data stream.
  test_mul_div_tap u_mul_div (
    .clk_i               (clk_i),
    .rst_ni              (rst_ni),
    .dft_i               ('0),
    .req_ready           (req_ready),
    .req_valid           (req_valid),
    .req_valid_early     (req_valid_early),
    .req_fn              (alu_func_e'(req_fn_sel)),
    .req_fn_early        (alu_func_e'(req_fn_early_sel)),
    .req_dw              (req_dw_sel),
    .req_in1             (req_in1_sel),
    .req_in2             (req_in2_sel),
    .req_dest            (minion_reg_dest_t'(req_dest_sel)),
    .kill                (kill_sel),
    .chicken_bit_mul_div (1'b0),
    .resp_ready          (resp_ready_sel),
    .resp_valid          (resp_valid),
    .resp_valid_early    (),
    .resp_data           (resp_data),
    .resp_dest           (resp_dest),
    .debug_clock_enable_o(),
    .debug_start_mul_1p_o(),
    .debug_start_mul_2p_o(),
    .debug_mul_op_1p_o   (),
    .debug_mul_op_2p_o   (),
    .debug_loop_cnt_1p_o (),
    .debug_loop_cnt_2p_o (),
    .debug_ctl_bits_o    (),
    .debug_mpcnd_o       (),
    .debug_mul_lo_dvsr_o (),
    .debug_mul_lo_o      (),
    .debug_acc_v1_o      (),
    .debug_acc_v2_o      (),
    .debug_resp_data_o   ()
  );

  ulx3s_uart_tx #(
    .ClkHz    (ClkHz),
    .BaudRate (BaudRate)
  ) u_uart_tx (
    .clk_i   (clk_i),
    .rst_ni  (rst_ni),
    .valid_i (tx_valid),
    .data_i  (tx_data),
    .ready_o (tx_ready),
    .tx_o    (uart_tx_o),
    .busy_o  ()
  );
  /* verilator lint_on PINCONNECTEMPTY */

  always_comb begin
    tx_valid = 1'b0;
    tx_data  = 8'h00;
    if (msg_active_q && tx_ready) begin
      tx_valid = 1'b1;
      unique case (msg_kind_q)
        MsgBoot:   tx_data = boot_msg_byte(msg_idx_q);
        MsgResult: tx_data = result_msg_byte(selected_result, msg_idx_q);
        default:   tx_data = status_msg_byte(pass_q, msg_idx_q);
      endcase
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      boot_delay_q <= BootDelayInitVal[BootDelayW-1:0];
    end else if (!boot_sent_q && !msg_active_q && (boot_delay_q != '0)) begin
      boot_delay_q <= boot_delay_q - 1'b1;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      msg_active_q      <= 1'b0;
      msg_kind_q        <= MsgBoot;
      msg_idx_q         <= '0;
      msg_item_idx_q    <= '0;
      print_result_idx_q <= '0;
      boot_sent_q       <= 1'b0;
      status_sent_q     <= 1'b0;
    end else begin
      if (msg_active_q && tx_ready) begin
        if (msg_idx_q == ((msg_kind_q == MsgBoot) ? BootMsgLastIdx :
                          (msg_kind_q == MsgStatus) ? StatusMsgLastIdx :
                                                       result_msg_last_idx(selected_result.kind))) begin
          msg_active_q <= 1'b0;
          msg_idx_q    <= '0;
          if (msg_kind_q == MsgBoot) begin
            boot_sent_q <= 1'b1;
          end else if (msg_kind_q == MsgResult) begin
            print_result_idx_q <= print_result_idx_q + 1'b1;
          end else begin
            status_sent_q <= 1'b1;
          end
        end else begin
          msg_idx_q <= msg_idx_q + 1'b1;
        end
      end else if (!msg_active_q) begin
        if (!boot_sent_q && (boot_delay_q == '0)) begin
          msg_active_q   <= 1'b1;
          msg_kind_q     <= MsgBoot;
          msg_idx_q      <= '0;
        end else if (done_q && boot_sent_q && (print_result_idx_q < result_count_q)) begin
          msg_active_q   <= 1'b1;
          msg_kind_q     <= MsgResult;
          msg_idx_q      <= '0;
          msg_item_idx_q <= print_result_idx_q;
        end else if (done_q && boot_sent_q && !status_sent_q && (print_result_idx_q == result_count_q)) begin
          msg_active_q <= 1'b1;
          msg_kind_q   <= MsgStatus;
          msg_idx_q    <= '0;
        end
      end
    end
  end

  /* verilator lint_off SYNCASYNCNET */  // The harness is intentionally async-reset at the top level while it samples multi-cycle DUT outputs in the same clock domain.
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      run_state_q    <= StWaitReady;
      case_idx_q     <= '0;
      timeout_q      <= TimeoutInitVal[TimeoutW-1:0];
      stall_hold_q   <= '0;
      hold_data_q    <= '0;
      hold_dest_q    <= '0;
      kill_delay_q   <= '0;
      kill_aux_ok_q  <= 1'b1;
      done_q         <= 1'b0;
      fail_q         <= 1'b0;
      pass_q         <= 1'b0;
      result_count_q <= '0;
      for (int i = 0; i < NumCases; i++) begin
        results_q[i] <= '0;
      end
    end else if (boot_sent_q && !done_q) begin
      if (timeout_q != '0) begin
        timeout_q <= timeout_q - 1'b1;
      end

      if (timeout_q == '0) begin
        results_q[result_count_q].kind   <= active_kind;
        results_q[result_count_q].index  <= case_idx_q;
        results_q[result_count_q].fn     <= active_fn;
        results_q[result_count_q].dw     <= active_dw;
        results_q[result_count_q].pass   <= 1'b0;
        results_q[result_count_q].aux_ok <= kill_aux_ok_q;
        results_q[result_count_q].got    <= resp_data;
        results_q[result_count_q].exp    <= active_exp;
        results_q[result_count_q].dest   <= active_dest;
        result_count_q <= result_count_q + 1'b1;
        done_q         <= 1'b1;
        fail_q         <= 1'b1;
        pass_q         <= 1'b0;
      end else begin
        unique case (run_state_q)
          StWaitReady: begin
            if (req_ready) begin
              if (active_kind == ResKill) begin
                kill_delay_q  <= KillDelayCycles;
                kill_aux_ok_q <= 1'b1;
              end
              run_state_q <= StIssueEarly;
            end
          end

          StIssueEarly: begin
            run_state_q <= StIssueValid;
          end

          StIssueValid: begin
            timeout_q <= TimeoutInitVal[TimeoutW-1:0];
            unique case (active_kind)
              ResNormal: run_state_q <= StWaitResp;
              ResStall:  run_state_q <= StStallWaitResp;
              default:   run_state_q <= StKillRun;
            endcase
          end

          StWaitResp: begin
            if (resp_valid) begin
              results_q[result_count_q].kind   <= ResNormal;
              results_q[result_count_q].index  <= case_idx_q;
              results_q[result_count_q].fn     <= active_fn;
              results_q[result_count_q].dw     <= active_dw;
              results_q[result_count_q].pass   <= (resp_data == active_exp) && (resp_dest == active_dest);
              results_q[result_count_q].aux_ok <= 1'b1;
              results_q[result_count_q].got    <= resp_data;
              results_q[result_count_q].exp    <= active_exp;
              results_q[result_count_q].dest   <= active_dest;
              result_count_q <= result_count_q + 1'b1;
              if ((resp_data != active_exp) || (resp_dest != active_dest)) begin
                done_q <= 1'b1;
                fail_q <= 1'b1;
                pass_q <= 1'b0;
              end else if (case_idx_q == LastCaseIdxVal) begin
                done_q <= 1'b1;
                fail_q <= 1'b0;
                pass_q <= 1'b1;
              end else begin
                case_idx_q <= case_idx_q + 1'b1;
                timeout_q  <= TimeoutInitVal[TimeoutW-1:0];
                run_state_q <= StWaitReady;
              end
            end
          end

          StStallWaitResp: begin
            if (resp_valid) begin
              hold_data_q   <= resp_data;
              hold_dest_q   <= resp_dest;
              stall_hold_q  <= StallHoldCycles;
              run_state_q   <= StStallHold;
            end
          end

          StStallHold: begin
            if (!resp_valid || (resp_data != hold_data_q) || (resp_dest != hold_dest_q)) begin
              results_q[result_count_q].kind   <= ResStall;
              results_q[result_count_q].index  <= case_idx_q;
              results_q[result_count_q].fn     <= active_fn;
              results_q[result_count_q].dw     <= active_dw;
              results_q[result_count_q].pass   <= 1'b0;
              results_q[result_count_q].aux_ok <= 1'b0;
              results_q[result_count_q].got    <= resp_data;
              results_q[result_count_q].exp    <= active_exp;
              results_q[result_count_q].dest   <= active_dest;
              result_count_q <= result_count_q + 1'b1;
              done_q <= 1'b1;
              fail_q <= 1'b1;
              pass_q <= 1'b0;
            end else if (stall_hold_q == '0) begin
              run_state_q <= StStallConsume;
            end else begin
              stall_hold_q <= stall_hold_q - 1'b1;
            end
          end

          StStallConsume: begin
            if (resp_valid) begin
              results_q[result_count_q].kind   <= ResStall;
              results_q[result_count_q].index  <= case_idx_q;
              results_q[result_count_q].fn     <= active_fn;
              results_q[result_count_q].dw     <= active_dw;
              results_q[result_count_q].pass   <= (resp_data == active_exp) && (resp_dest == active_dest);
              results_q[result_count_q].aux_ok <= 1'b1;
              results_q[result_count_q].got    <= resp_data;
              results_q[result_count_q].exp    <= active_exp;
              results_q[result_count_q].dest   <= active_dest;
              result_count_q <= result_count_q + 1'b1;
              if ((resp_data != active_exp) || (resp_dest != active_dest)) begin
                done_q <= 1'b1;
                fail_q <= 1'b1;
                pass_q <= 1'b0;
              end else begin
                case_idx_q  <= case_idx_q + 1'b1;
                timeout_q   <= TimeoutInitVal[TimeoutW-1:0];
                run_state_q <= StWaitReady;
              end
            end
          end

          StKillRun: begin
            if (resp_valid) begin
              kill_aux_ok_q <= 1'b0;
            end
            if (kill_delay_q == '0) begin
              run_state_q <= StKillPulse;
            end else begin
              kill_delay_q <= kill_delay_q - 1'b1;
            end
          end

          StKillPulse: begin
            if (resp_valid) begin
              kill_aux_ok_q <= 1'b0;
            end
            run_state_q <= StKillWaitReady;
          end

          StKillWaitReady: begin
            if (resp_valid) begin
              kill_aux_ok_q <= 1'b0;
            end
            if (req_ready) begin
              timeout_q   <= TimeoutInitVal[TimeoutW-1:0];
              run_state_q <= StKillRecoverWaitReady;
            end
          end

          StKillRecoverWaitReady: begin
            if (req_ready) begin
              run_state_q <= StKillRecoverEarly;
            end
          end

          StKillRecoverEarly: begin
            run_state_q <= StKillRecoverValid;
          end

          StKillRecoverValid: begin
            run_state_q <= StKillRecoverResp;
          end

          StKillRecoverResp: begin
            if (resp_valid) begin
              results_q[result_count_q].kind   <= ResKill;
              results_q[result_count_q].index  <= case_idx_q;
              results_q[result_count_q].fn     <= RecoveryFn;
              results_q[result_count_q].dw     <= RecoveryDw;
              results_q[result_count_q].pass   <= kill_aux_ok_q && (resp_data == RecoveryExp) && (resp_dest == RecoveryDest);
              results_q[result_count_q].aux_ok <= kill_aux_ok_q;
              results_q[result_count_q].got    <= resp_data;
              results_q[result_count_q].exp    <= RecoveryExp;
              results_q[result_count_q].dest   <= RecoveryDest;
              result_count_q <= result_count_q + 1'b1;
              if (!kill_aux_ok_q || (resp_data != RecoveryExp) || (resp_dest != RecoveryDest)) begin
                done_q <= 1'b1;
                fail_q <= 1'b1;
                pass_q <= 1'b0;
              end else begin
                done_q <= 1'b1;
                fail_q <= 1'b0;
                pass_q <= 1'b1;
              end
            end
          end

          default: begin
            run_state_q <= StWaitReady;
          end
        endcase
      end
    end
  end
  /* verilator lint_on SYNCASYNCNET */

  always_comb begin
    led_o = '0;
    led_o[0] = boot_sent_q;
    led_o[1] = done_q;
    led_o[2] = pass_q;
    led_o[3] = fail_q;
    led_o[7:4] = case_idx_q[3:0];
  end

endmodule
