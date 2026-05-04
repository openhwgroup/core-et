// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_top_exec -- minimal board/demo harness for minion_top.
//
// Reuses the proven minion_top smoke-test integration shape, serves a constant
// NOP I-cache line, and emits a compact UART diagnostic summary after the first
// few retired scalar instructions have been observed.

module minion_top_exec
  import minion_pkg::*;
  import minion_frontend_pkg::*;
#(
  parameter int unsigned ClkHz           = 25_000_000,
  parameter int unsigned BaudRate        = 115200,
  parameter int unsigned BootDelayCycles = 0,
  parameter int unsigned TxFifoDepth     = 64,
  parameter int unsigned TimeoutCycles   = 4096,
  localparam int unsigned TxDepthW       = $clog2(TxFifoDepth + 1),
  localparam int unsigned BootDelayW     = (BootDelayCycles > 0) ? $clog2(BootDelayCycles + 1) : 1,
  localparam int unsigned TimeoutW       = $clog2(TimeoutCycles + 1),
  localparam int unsigned MsgIdxW        = 8,
  localparam int unsigned DiagCountW     = 3
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  output logic       uart_tx_o,
  output logic [7:0] led_o
);

  typedef enum logic [3:0] {
    MsgBoot,
    MsgCounts,
    MsgReq0,
    MsgReq1,
    MsgReq2,
    MsgReq3,
    MsgTrace0,
    MsgTrace1,
    MsgTrace2,
    MsgTrace3,
    MsgStatus
  } msg_kind_e;

  typedef struct packed {
    logic [63:0] pc;
    logic [31:0] inst;
    logic        xcpt;
  } retire_t;

  localparam logic [31:0] NopInst         = 32'h0000_0013;
  localparam logic [63:0] ResetVector     = 64'h0000_0000_0000_1000;
  localparam logic [31:0] BootDelayInitVal = BootDelayCycles;
  localparam logic [31:0] TimeoutInitVal   = TimeoutCycles;
  localparam logic [DiagCountW-1:0] DiagReqsCount    = 3'd4;
  localparam logic [DiagCountW-1:0] DiagTracesCount  = 3'd4;
  localparam logic [DiagCountW-1:0] LastDiagTraceIdx = 3'd3;
  localparam logic [MsgIdxW-1:0] BootMsgLastIdx      = 8'd24;
  localparam logic [MsgIdxW-1:0] CountsMsgLastIdx    = 8'd14;
  localparam logic [MsgIdxW-1:0] ReqMsgLastIdx       = 8'd30;
  localparam logic [MsgIdxW-1:0] TraceMsgLastIdx     = 8'd41;
  localparam logic [MsgIdxW-1:0] StatusMsgLastIdx    = 8'd28;

  logic                        icache_req_ready;
  logic                        icache_resp_valid;
  icache_fe_resp_t             icache_resp;
  /* verilator lint_off UNUSEDSIGNAL */  // The first-cut demo only reports retire/UART status; the remaining top-level observability signals are kept for bring-up expansion.
  logic                        icache_req_valid;
  fe_icache_req_t              icache_req;
  minion_debug_in_t            debug_in;
  minion_debug_out_t           debug_out;
  minion_chicken_bits_t        chicken_bits;
  esr_minion_features_t        esr_features;
  logic                        apb_pready;
  logic [BpamShireApbDataWidth-1:0] apb_prdata;
  logic                        apb_pslverr;
  trace_encoder_signals_t      trace_encoder;
  logic                        nsleepout;
  logic [EtEcoMinionOutputWidth-1:0] eco_o;
  neigh_sm_dbg_monitor_t       minion_dbg_signals;
  minion_satp_info             satp_info_unused;
  minion_satp_info             matp_info_unused;
  logic                        trace_instr_valid;
  logic [InstSize-1:0]         trace_instr_bus;
  logic [ShireTeInstrAddrWidth-1:0] trace_instr_addr;
  logic                        trace_exception;

  logic [TimeoutW-1:0]         timeout_q;
  logic                        done_q;
  logic                        pass_q;
  logic                        fail_q;
  logic [DiagCountW-1:0]       request_count_q;
  logic [DiagCountW-1:0]       trace_count_q;
  logic                        nsleepout_seen_q;
  logic                        all_traces_seen_q;
  logic [63:0]                 req_addr0_q;
  logic [63:0]                 req_addr1_q;
  logic [63:0]                 req_addr2_q;
  logic [63:0]                 req_addr3_q;
  logic                        req_tid0_q;
  logic                        req_tid1_q;
  logic                        req_tid2_q;
  logic                        req_tid3_q;
  retire_t                     trace0_q;
  retire_t                     trace1_q;
  retire_t                     trace2_q;
  retire_t                     trace3_q;

  logic [BootDelayW-1:0]       boot_delay_q;
  logic                        summary_pending_q;
  logic                        msg_active_q;
  msg_kind_e                   msg_kind_q;
  logic [MsgIdxW-1:0]          msg_idx_q;

  logic [7:0]                  tx_wdata;
  logic                        tx_wvalid;
  logic                        tx_wready;
  logic [7:0]                  tx_rdata;
  logic                        tx_rvalid;
  logic                        tx_rready;
  logic                        tx_pop_ready;
  logic [TxDepthW-1:0]         tx_depth;
  logic                        tx_full;
  logic                        tx_empty;
  logic                        tx_busy;
  /* verilator lint_on UNUSEDSIGNAL */

  function automatic logic [FeFetchReadSize-1:0] nop_line();
    logic [FeFetchReadSize-1:0] line;
    line = '0;
    for (int i = 0; i < (FeFetchReadSize / 32); i++) begin
      line[i*32 +: 32] = NopInst;
    end
    return line;
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
  /* verilator lint_on UNUSEDSIGNAL */

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
      8:  boot_msg_byte = 8'h6D;  // m
      9:  boot_msg_byte = 8'h69;  // i
      10: boot_msg_byte = 8'h6E;  // n
      11: boot_msg_byte = 8'h69;  // i
      12: boot_msg_byte = 8'h6F;  // o
      13: boot_msg_byte = 8'h6E;  // n
      14: boot_msg_byte = 8'h5F;  // _
      15: boot_msg_byte = 8'h74;  // t
      16: boot_msg_byte = 8'h6F;  // o
      17: boot_msg_byte = 8'h70;  // p
      18: boot_msg_byte = 8'h5F;  // _
      19: boot_msg_byte = 8'h65;  // e
      20: boot_msg_byte = 8'h78;  // x
      21: boot_msg_byte = 8'h65;  // e
      22: boot_msg_byte = 8'h63;  // c
      23: boot_msg_byte = 8'h0D;  // \r
      default: boot_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] status_msg_byte(
    input logic pass_i,
    input logic [MsgIdxW-1:0] idx
  );
    unique case (idx)
      0:  status_msg_byte = 8'h4D;  // M
      1:  status_msg_byte = 8'h49;  // I
      2:  status_msg_byte = 8'h4E;  // N
      3:  status_msg_byte = 8'h49;  // I
      4:  status_msg_byte = 8'h4F;  // O
      5:  status_msg_byte = 8'h4E;  // N
      6:  status_msg_byte = 8'h20;  // space
      7:  status_msg_byte = 8'h54;  // T
      8:  status_msg_byte = 8'h4F;  // O
      9:  status_msg_byte = 8'h50;  // P
      10: status_msg_byte = 8'h20;  // space
      11: status_msg_byte = 8'h45;  // E
      12: status_msg_byte = 8'h58;  // X
      13: status_msg_byte = 8'h45;  // E
      14: status_msg_byte = 8'h43;  // C
      15: status_msg_byte = 8'h20;  // space
      16: status_msg_byte = 8'h54;  // T
      17: status_msg_byte = 8'h45;  // E
      18: status_msg_byte = 8'h53;  // S
      19: status_msg_byte = 8'h54;  // T
      20: status_msg_byte = 8'h20;  // space
      21: status_msg_byte = pass_i ? 8'h50 : 8'h46;  // P/F
      22: status_msg_byte = pass_i ? 8'h41 : 8'h41;  // A/A
      23: status_msg_byte = pass_i ? 8'h53 : 8'h49;  // S/I
      24: status_msg_byte = pass_i ? 8'h53 : 8'h4C;  // S/L
      25: status_msg_byte = pass_i ? 8'h45 : 8'h45;  // E/E
      26: status_msg_byte = pass_i ? 8'h44 : 8'h44;  // D/D
      27: status_msg_byte = 8'h0D;  // \r
      default: status_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] diag_count_ascii(input logic [DiagCountW-1:0] count);
    logic [3:0] nibble;
    nibble = '0;
    nibble[DiagCountW-1:0] = count;
    diag_count_ascii = hex_nibble_ascii(nibble);
  endfunction

  function automatic logic diag_idx_valid(
    input logic [DiagCountW-1:0] count,
    input logic [1:0] idx
  );
    diag_idx_valid = count > {1'b0, idx};
  endfunction

  function automatic logic [63:0] expected_trace_pc(input logic [1:0] idx);
    unique case (idx)
      2'd0:    expected_trace_pc = ResetVector;
      2'd1:    expected_trace_pc = ResetVector + 64'd4;
      2'd2:    expected_trace_pc = ResetVector + 64'd8;
      default: expected_trace_pc = ResetVector + 64'd12;
    endcase
  endfunction

  function automatic logic trace_entry_pass(
    input retire_t        trace_i,
    input logic [1:0]     idx
  );
    trace_entry_pass = (trace_i.pc == expected_trace_pc(idx)) &&
                       (trace_i.inst == NopInst) &&
                       !trace_i.xcpt;
  endfunction

  function automatic logic [1:0] count_led(input logic [DiagCountW-1:0] count);
    if (count >= 3'd3) begin
      count_led = 2'd3;
    end else begin
      count_led = count[1:0];
    end
  endfunction

  function automatic logic [7:0] counts_msg_byte(
    input logic [DiagCountW-1:0] request_count_i,
    input logic [DiagCountW-1:0] trace_count_i,
    input logic                  nsleepout_seen_i,
    input logic [MsgIdxW-1:0]    idx
  );
    unique case (idx)
      0:  counts_msg_byte = 8'h43;  // C
      1:  counts_msg_byte = 8'h20;  // space
      2:  counts_msg_byte = 8'h51;  // Q
      3:  counts_msg_byte = 8'h3D;  // =
      4:  counts_msg_byte = diag_count_ascii(request_count_i);
      5:  counts_msg_byte = 8'h20;  // space
      6:  counts_msg_byte = 8'h54;  // T
      7:  counts_msg_byte = 8'h3D;  // =
      8:  counts_msg_byte = diag_count_ascii(trace_count_i);
      9:  counts_msg_byte = 8'h20;  // space
      10: counts_msg_byte = 8'h4E;  // N
      11: counts_msg_byte = 8'h3D;  // =
      12: counts_msg_byte = nsleepout_seen_i ? 8'h31 : 8'h30;
      13: counts_msg_byte = 8'h0D;  // \r
      default: counts_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] req_msg_byte(
    input logic [1:0]        which_i,
    input logic              valid_i,
    input logic              thread_i,
    input logic [63:0]       addr_i,
    input logic [MsgIdxW-1:0] idx
  );
    unique case (idx)
      0:  req_msg_byte = 8'h51;  // Q
      1:  req_msg_byte = 8'h30 + {6'b0, which_i};
      2:  req_msg_byte = 8'h20;  // space
      3:  req_msg_byte = 8'h56;  // V
      4:  req_msg_byte = 8'h3D;  // =
      5:  req_msg_byte = valid_i ? 8'h31 : 8'h30;
      6:  req_msg_byte = 8'h20;  // space
      7:  req_msg_byte = 8'h54;  // T
      8:  req_msg_byte = 8'h3D;  // =
      9:  req_msg_byte = thread_i ? 8'h31 : 8'h30;
      10: req_msg_byte = 8'h20;  // space
      11: req_msg_byte = 8'h41;  // A
      12: req_msg_byte = 8'h3D;  // =
      13: req_msg_byte = hex64_digit_ascii(addr_i, 5'd0);
      14: req_msg_byte = hex64_digit_ascii(addr_i, 5'd1);
      15: req_msg_byte = hex64_digit_ascii(addr_i, 5'd2);
      16: req_msg_byte = hex64_digit_ascii(addr_i, 5'd3);
      17: req_msg_byte = hex64_digit_ascii(addr_i, 5'd4);
      18: req_msg_byte = hex64_digit_ascii(addr_i, 5'd5);
      19: req_msg_byte = hex64_digit_ascii(addr_i, 5'd6);
      20: req_msg_byte = hex64_digit_ascii(addr_i, 5'd7);
      21: req_msg_byte = hex64_digit_ascii(addr_i, 5'd8);
      22: req_msg_byte = hex64_digit_ascii(addr_i, 5'd9);
      23: req_msg_byte = hex64_digit_ascii(addr_i, 5'd10);
      24: req_msg_byte = hex64_digit_ascii(addr_i, 5'd11);
      25: req_msg_byte = hex64_digit_ascii(addr_i, 5'd12);
      26: req_msg_byte = hex64_digit_ascii(addr_i, 5'd13);
      27: req_msg_byte = hex64_digit_ascii(addr_i, 5'd14);
      28: req_msg_byte = hex64_digit_ascii(addr_i, 5'd15);
      29: req_msg_byte = 8'h0D;  // \r
      default: req_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] trace_msg_byte(
    input logic [1:0]        which_i,
    input logic              valid_i,
    input retire_t           trace_i,
    input logic [MsgIdxW-1:0] idx
  );
    unique case (idx)
      0:  trace_msg_byte = 8'h54;  // T
      1:  trace_msg_byte = 8'h30 + {6'b0, which_i};
      2:  trace_msg_byte = 8'h20;  // space
      3:  trace_msg_byte = 8'h56;  // V
      4:  trace_msg_byte = 8'h3D;  // =
      5:  trace_msg_byte = valid_i ? 8'h31 : 8'h30;
      6:  trace_msg_byte = 8'h20;  // space
      7:  trace_msg_byte = 8'h50;  // P
      8:  trace_msg_byte = 8'h3D;  // =
      9:  trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd0);
      10: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd1);
      11: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd2);
      12: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd3);
      13: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd4);
      14: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd5);
      15: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd6);
      16: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd7);
      17: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd8);
      18: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd9);
      19: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd10);
      20: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd11);
      21: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd12);
      22: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd13);
      23: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd14);
      24: trace_msg_byte = hex64_digit_ascii(trace_i.pc, 5'd15);
      25: trace_msg_byte = 8'h20;  // space
      26: trace_msg_byte = 8'h49;  // I
      27: trace_msg_byte = 8'h3D;  // =
      28: trace_msg_byte = hex32_digit_ascii(trace_i.inst, 4'd0);
      29: trace_msg_byte = hex32_digit_ascii(trace_i.inst, 4'd1);
      30: trace_msg_byte = hex32_digit_ascii(trace_i.inst, 4'd2);
      31: trace_msg_byte = hex32_digit_ascii(trace_i.inst, 4'd3);
      32: trace_msg_byte = hex32_digit_ascii(trace_i.inst, 4'd4);
      33: trace_msg_byte = hex32_digit_ascii(trace_i.inst, 4'd5);
      34: trace_msg_byte = hex32_digit_ascii(trace_i.inst, 4'd6);
      35: trace_msg_byte = hex32_digit_ascii(trace_i.inst, 4'd7);
      36: trace_msg_byte = 8'h20;  // space
      37: trace_msg_byte = 8'h58;  // X
      38: trace_msg_byte = 8'h3D;  // =
      39: trace_msg_byte = trace_i.xcpt ? 8'h31 : 8'h30;
      40: trace_msg_byte = 8'h0D;  // \r
      default: trace_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  wire req_accept = icache_req_valid && icache_req_ready;
  wire req_capture = !done_q && req_accept && (request_count_q < DiagReqsCount);
  wire trace_capture = !done_q && trace_instr_valid && (trace_count_q < DiagTracesCount);
  wire [63:0] trace_pc_ext = {{(64-ShireTeInstrAddrWidth){1'b0}}, trace_instr_addr};
  wire stored_trace_pass = trace_entry_pass(trace0_q, 2'd0) &&
                           trace_entry_pass(trace1_q, 2'd1) &&
                           trace_entry_pass(trace2_q, 2'd2) &&
                           trace_entry_pass(trace3_q, 2'd3);
  wire boot_delay_done = (boot_delay_q == '0);
  logic summary_done;

  always_comb begin
    unique case (msg_kind_q)
      MsgBoot:   summary_done = (msg_idx_q == BootMsgLastIdx);
      MsgCounts: summary_done = (msg_idx_q == CountsMsgLastIdx);
      MsgReq0,
      MsgReq1,
      MsgReq2,
      MsgReq3:   summary_done = (msg_idx_q == ReqMsgLastIdx);
      MsgTrace0,
      MsgTrace1,
      MsgTrace2,
      MsgTrace3: summary_done = (msg_idx_q == TraceMsgLastIdx);
      default:   summary_done = (msg_idx_q == StatusMsgLastIdx);
    endcase
  end

  always_comb begin
    tx_wvalid = 1'b0;
    tx_wdata  = 8'h00;

    if (msg_active_q && tx_wready) begin
      tx_wvalid = 1'b1;
      unique case (msg_kind_q)
        MsgBoot:   tx_wdata = boot_msg_byte(msg_idx_q);
        MsgCounts: tx_wdata = counts_msg_byte(request_count_q, trace_count_q,
                                              nsleepout_seen_q, msg_idx_q);
        MsgReq0:   tx_wdata = req_msg_byte(2'd0, diag_idx_valid(request_count_q, 2'd0),
                                           req_tid0_q, req_addr0_q, msg_idx_q);
        MsgReq1:   tx_wdata = req_msg_byte(2'd1, diag_idx_valid(request_count_q, 2'd1),
                                           req_tid1_q, req_addr1_q, msg_idx_q);
        MsgReq2:   tx_wdata = req_msg_byte(2'd2, diag_idx_valid(request_count_q, 2'd2),
                                           req_tid2_q, req_addr2_q, msg_idx_q);
        MsgReq3:   tx_wdata = req_msg_byte(2'd3, diag_idx_valid(request_count_q, 2'd3),
                                           req_tid3_q, req_addr3_q, msg_idx_q);
        MsgTrace0: tx_wdata = trace_msg_byte(2'd0, diag_idx_valid(trace_count_q, 2'd0),
                                             trace0_q, msg_idx_q);
        MsgTrace1: tx_wdata = trace_msg_byte(2'd1, diag_idx_valid(trace_count_q, 2'd1),
                                             trace1_q, msg_idx_q);
        MsgTrace2: tx_wdata = trace_msg_byte(2'd2, diag_idx_valid(trace_count_q, 2'd2),
                                             trace2_q, msg_idx_q);
        MsgTrace3: tx_wdata = trace_msg_byte(2'd3, diag_idx_valid(trace_count_q, 2'd3),
                                             trace3_q, msg_idx_q);
        default:   tx_wdata = status_msg_byte(pass_q, msg_idx_q);
      endcase
    end
  end

  always_comb begin
    debug_in = '0;

    chicken_bits = '0;

    esr_features = '0;
    esr_features.trap_on_ml = 1'b1;
    esr_features.trap_on_gfx = 1'b1;
    esr_features.trap_on_u_scp = 1'b1;
    esr_features.trap_on_u_cacheops = 1'b1;
  end

  assign trace_instr_valid = trace_encoder.instr_valid[0];
  assign trace_instr_bus = trace_encoder.instr_bus[InstSize-1:0];
  assign trace_instr_addr = trace_encoder.instr_addr[ShireTeInstrAddrWidth-1:0];
  assign trace_exception = trace_encoder.exception[0];

  /* verilator lint_off PINCONNECTEMPTY */  // The bring-up harness intentionally leaves unrelated SoC-facing ports open while exercising the fetch/retire/UART path.
  minion_top #(
    .DebugApbEn(1'b1),
    .DebugMonEn(1'b1),
    .TraceEn(1'b1),
    .VpuEn  (1'b1)
  ) u_minion (
    .clk_i,
    .rst_c_ni                   (rst_ni),
    .rst_w_ni                   (rst_ni),
    .rst_d_ni                   (rst_ni),
    .dft_i                      ('0),
    .eco_i                      (10'h3a5),
    .eco_o                      (eco_o),
    .ioshire_i                  (1'b0),
    .nsleepin_i                 (1'b1),
    .iso_enable_i               (1'b0),
    .nsleepout_o                (nsleepout),
    .chicken_bits_i             (chicken_bits),
    .l2_dcache_evict_req_ready_i('1),
    .l2_dcache_evict_req_valid_o(),
    .l2_dcache_evict_req_o      (),
    .l2_dcache_miss_req_ready_i ('1),
    .l2_dcache_miss_req_valid_o (),
    .l2_dcache_miss_req_o       (),
    .l2_dcache_resp_ready_o     (),
    .l2_dcache_resp_valid_i     (1'b0),
    .l2_dcache_resp_i           ('0),
    .icache_req_ready_i         (icache_req_ready),
    .icache_req_valid_o         (icache_req_valid),
    .icache_req_o               (icache_req),
    .icache_resp_valid_i        (icache_resp_valid),
    .icache_resp_miss_i         (1'b0),
    .icache_resp_i              (icache_resp),
    .icache_fill_done_i         (1'b0),
    .icache_flush_data_o        (),
    .satp_info_o                (satp_info_unused),
    .matp_info_o                (matp_info_unused),
    .tlb_invalidate_o           (),
    .dc_ptw_req_data_o          (),
    .dc_ptw_req_valid_o         (),
    .dc_ptw_req_ready_i         (1'b1),
    .ptw_dc_resp_data_i         ('0),
    .ptw_dc_resp_valid_i        (1'b0),
    .interrupts_i               ('0),
    .shire_id_i                 ('0),
    .shire_min_id_i             ('0),
    .enabled_i                  (2'b01),
    .reset_vector_i             (ResetVector[VaSize-1:0]),
    .mprot_i                    ('0),
    .vmspagesize_i              ('0),
    .flb_neigh_req_valid_o      (),
    .flb_neigh_req_data_o       (),
    .flb_neigh_resp_valid_i     (1'b0),
    .flb_neigh_resp_data_i      (1'b0),
    .te_thread_sel_i            (1'b0),
    .trace_encoder_o            (trace_encoder),
    .te_enable_i                (1'b1),
    .apb_paddr_i                ('0),
    .apb_penable_i              (1'b0),
    .apb_prdata_o               (apb_prdata),
    .apb_pready_o               (apb_pready),
    .apb_psel_i                 (1'b0),
    .apb_pslverr_o              (apb_pslverr),
    .apb_pwdata_i               ('0),
    .apb_pwrite_i               (1'b0),
    .debug_in_i                 (debug_in),
    .debug_out_o                (debug_out),
    .minion_dbg_signals_o       (minion_dbg_signals),
    .minion_dbg_signals_mux_i   ('0),
    .minion_dbg_sig_enable_i    (1'b0),
    .esr_features_i             (esr_features),
    .esr_bypass_dcache_i        (1'b0),
    .esr_shire_coop_mode_i      (1'b0),
    .esr_minion_mem_override_i  ('0),
    .pmu_count_up_o             (),
    .pmu_read_data_i            ('0),
    .pmu_read_sel_o             (),
    .pmu_write_en_o             (),
    .pmu_write_data_o           (),
    .pmu_neigh_event_sel_o      ()
  );
  /* verilator lint_on PINCONNECTEMPTY */

  always_comb begin
    // Minimal bring-up I-cache model: every request is accepted, and the
    // frontend always sees a cacheable NOP fetch line. This deliberately avoids
    // testing external I-cache latency until the top-level retire path is known
    // good on the board.
    icache_req_ready = 1'b1;
    icache_resp_valid = rst_ni;
    icache_resp = '0;
    icache_resp.data = nop_line();
    icache_resp.cacheable = 1'b1;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      timeout_q       <= TimeoutInitVal[TimeoutW-1:0];
      done_q          <= 1'b0;
      pass_q          <= 1'b0;
      fail_q          <= 1'b0;
      request_count_q <= '0;
      trace_count_q   <= '0;
      nsleepout_seen_q <= 1'b0;
      all_traces_seen_q <= 1'b0;
      req_addr0_q <= '0;
      req_addr1_q <= '0;
      req_addr2_q <= '0;
      req_addr3_q <= '0;
      req_tid0_q <= 1'b0;
      req_tid1_q <= 1'b0;
      req_tid2_q <= 1'b0;
      req_tid3_q <= 1'b0;
      trace0_q <= '0;
      trace1_q <= '0;
      trace2_q <= '0;
      trace3_q <= '0;
    end else begin
      nsleepout_seen_q <= nsleepout_seen_q | nsleepout;

      if (!done_q && timeout_q != '0) begin
        timeout_q <= timeout_q - 1'b1;
      end

      if (req_capture) begin
        unique case (request_count_q[1:0])
          2'd0: begin
            req_addr0_q <= {{(64-VaSizeExt){1'b0}}, icache_req.addr};
            req_tid0_q <= icache_req.thread_id[0];
          end
          2'd1: begin
            req_addr1_q <= {{(64-VaSizeExt){1'b0}}, icache_req.addr};
            req_tid1_q <= icache_req.thread_id[0];
          end
          2'd2: begin
            req_addr2_q <= {{(64-VaSizeExt){1'b0}}, icache_req.addr};
            req_tid2_q <= icache_req.thread_id[0];
          end
          default: begin
            req_addr3_q <= {{(64-VaSizeExt){1'b0}}, icache_req.addr};
            req_tid3_q <= icache_req.thread_id[0];
          end
        endcase
        request_count_q <= request_count_q + 3'd1;
      end

      if (trace_capture) begin
        unique case (trace_count_q[1:0])
          2'd0: begin
            trace0_q.pc   <= trace_pc_ext;
            trace0_q.inst <= trace_instr_bus;
            trace0_q.xcpt <= trace_exception;
          end
          2'd1: begin
            trace1_q.pc   <= trace_pc_ext;
            trace1_q.inst <= trace_instr_bus;
            trace1_q.xcpt <= trace_exception;
          end
          2'd2: begin
            trace2_q.pc   <= trace_pc_ext;
            trace2_q.inst <= trace_instr_bus;
            trace2_q.xcpt <= trace_exception;
          end
          default: begin
            trace3_q.pc   <= trace_pc_ext;
            trace3_q.inst <= trace_instr_bus;
            trace3_q.xcpt <= trace_exception;
          end
        endcase
        trace_count_q <= trace_count_q + 3'd1;

        if (trace_count_q == LastDiagTraceIdx) begin
          all_traces_seen_q <= 1'b1;
        end
      end else if (!done_q && all_traces_seen_q) begin
        done_q <= 1'b1;
        pass_q <= stored_trace_pass;
        fail_q <= !stored_trace_pass;
      end else if (!done_q && timeout_q == '0) begin
        done_q <= 1'b1;
        pass_q <= 1'b0;
        fail_q <= 1'b1;
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      boot_delay_q      <= BootDelayInitVal[BootDelayW-1:0];
      summary_pending_q <= 1'b0;
      msg_active_q      <= 1'b0;
      msg_kind_q        <= MsgBoot;
      msg_idx_q         <= '0;
    end else begin
      if (!boot_delay_done) begin
        boot_delay_q <= boot_delay_q - 1'b1;
      end

      if (done_q && !summary_pending_q && !msg_active_q && msg_kind_q == MsgBoot) begin
        summary_pending_q <= 1'b1;
      end

      if (msg_active_q && tx_wready) begin
        if (summary_done) begin
          msg_active_q <= 1'b0;
          msg_idx_q    <= '0;
          unique case (msg_kind_q)
            MsgBoot: begin
              msg_kind_q <= MsgCounts;
            end
            MsgCounts: begin
              msg_kind_q <= MsgReq0;
            end
            MsgReq0: begin
              msg_kind_q <= MsgReq1;
            end
            MsgReq1: begin
              msg_kind_q <= MsgReq2;
            end
            MsgReq2: begin
              msg_kind_q <= MsgReq3;
            end
            MsgReq3: begin
              msg_kind_q <= MsgTrace0;
            end
            MsgTrace0: begin
              msg_kind_q <= MsgTrace1;
            end
            MsgTrace1: begin
              msg_kind_q <= MsgTrace2;
            end
            MsgTrace2: begin
              msg_kind_q <= MsgTrace3;
            end
            MsgTrace3: begin
              msg_kind_q <= MsgStatus;
            end
            default: begin
              summary_pending_q <= 1'b0;
            end
          endcase
        end else begin
          msg_idx_q <= msg_idx_q + 1'b1;
        end
      end else if (!msg_active_q && summary_pending_q && boot_delay_done) begin
        msg_active_q <= 1'b1;
        msg_idx_q    <= '0;
      end
    end
  end

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

  assign tx_pop_ready = tx_rvalid & tx_rready;

  assign led_o = {
    done_q,
    pass_q,
    fail_q,
    nsleepout_seen_q,
    count_led(request_count_q),
    count_led(trace_count_q)
  };

endmodule
