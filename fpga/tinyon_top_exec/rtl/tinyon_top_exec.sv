// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// tinyon_top_exec -- minimal board/demo harness for minion_top.
//
// Reuses the proven minion_top smoke-test integration shape, serves a constant
// NOP I-cache line, and emits a compact UART summary once two retired scalar
// instructions have been observed.

module tinyon_top_exec
  import minion_pkg::*;
  import minion_frontend_pkg::*;
#(
  parameter int unsigned ClkHz           = 25_000_000,
  parameter int unsigned BaudRate        = 115200,
  parameter int unsigned BootDelayCycles = 0,
  parameter int unsigned TxFifoDepth     = 64,
  parameter int unsigned TimeoutCycles   = 512,
  localparam int unsigned TxDepthW       = $clog2(TxFifoDepth + 1),
  localparam int unsigned BootDelayW     = (BootDelayCycles > 0) ? $clog2(BootDelayCycles + 1) : 1,
  localparam int unsigned TimeoutW       = $clog2(TimeoutCycles + 1),
  localparam int unsigned MsgIdxW        = 8,
  localparam int unsigned NumRetires     = 2
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  output logic       uart_tx_o,
  output logic [7:0] led_o
);

  typedef enum logic [2:0] {
    MsgBoot,
    MsgReq0,
    MsgReq1,
    MsgRet0,
    MsgRet1,
    MsgStatus
  } msg_kind_e;

  typedef struct packed {
    logic        valid;
    logic [63:0] addr;
  } req_t;

  typedef struct packed {
    logic [63:0] pc;
    logic [31:0] inst;
    logic        xcpt;
  } retire_t;

  localparam logic [31:0] NopInst         = 32'h0000_0013;
  localparam logic [63:0] ResetVector     = 64'h0000_0000_0000_1000;
  localparam logic [31:0] BootDelayInitVal = BootDelayCycles;
  localparam logic [31:0] TimeoutInitVal   = TimeoutCycles;
  localparam logic [MsgIdxW-1:0] BootMsgLastIdx   = 8'd24;
  localparam logic [MsgIdxW-1:0] ReqMsgLastIdx    = 8'd26;
  localparam logic [MsgIdxW-1:0] RetMsgLastIdx    = 8'd37;
  localparam logic [MsgIdxW-1:0] StatusMsgLastIdx = 8'd28;

  logic                        icache_req_ready;
  logic                        icache_resp_valid;
  icache_fe_resp_t             icache_resp;
  /* verilator lint_off UNUSEDSIGNAL */  // The minimal demo only checks retire/UART status; unrelated SoC-facing observability ports stay present for integration fidelity.
  logic                        icache_req_valid;
  fe_icache_req_t              icache_req;
  minion_debug_in_t            debug_in;
  minion_debug_out_t           debug_out;
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
  logic                        icache_flush_data;
  logic                        tlb_invalidate;
  minion_ptw_req               dc_ptw_req_data;
  logic                        dc_ptw_req_valid;
  logic                        flb_neigh_req_valid;
  logic [DcacheL2MissReqPorts-1:0]  l2_dcache_miss_req_valid;
  logic [DcacheL2EvictReqPorts-1:0] l2_dcache_evict_req_valid;

  logic                        icache_feed_q;
  logic [TimeoutW-1:0]         timeout_q;
  logic                        done_q;
  logic                        pass_q;
  logic                        fail_q;
  logic [1:0]                  req_count_q;
  req_t                        req_q [NumRetires];
  logic [1:0]                  retire_count_q;
  retire_t                     retire_q [NumRetires];

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
      8:  boot_msg_byte = 8'h74;  // t
      9:  boot_msg_byte = 8'h69;  // i
      10: boot_msg_byte = 8'h6E;  // n
      11: boot_msg_byte = 8'h79;  // y
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
      0:  status_msg_byte = 8'h54;  // T
      1:  status_msg_byte = 8'h49;  // I
      2:  status_msg_byte = 8'h4E;  // N
      3:  status_msg_byte = 8'h59;  // Y
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

  function automatic logic [7:0] retire_msg_byte(
    input logic [0:0]     which_i,
    input retire_t        retire_i,
    input logic [MsgIdxW-1:0] idx
  );
    unique case (idx)
      0:  retire_msg_byte = 8'h52;  // R
      1:  retire_msg_byte = 8'h30 + {{7{1'b0}}, which_i};
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

  function automatic logic [7:0] req_msg_byte(
    input logic [0:0]           which_i,
    input req_t                 req_i,
    input logic [MsgIdxW-1:0]   idx
  );
    unique case (idx)
      0:  req_msg_byte = 8'h51;  // Q
      1:  req_msg_byte = 8'h30 + {{7{1'b0}}, which_i};
      2:  req_msg_byte = 8'h20;  // space
      3:  req_msg_byte = 8'h56;  // V
      4:  req_msg_byte = 8'h3D;  // =
      5:  req_msg_byte = req_i.valid ? 8'h31 : 8'h30;
      6:  req_msg_byte = 8'h20;  // space
      7:  req_msg_byte = 8'h41;  // A
      8:  req_msg_byte = 8'h3D;  // =
      9:  req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd0);
      10: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd1);
      11: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd2);
      12: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd3);
      13: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd4);
      14: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd5);
      15: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd6);
      16: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd7);
      17: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd8);
      18: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd9);
      19: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd10);
      20: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd11);
      21: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd12);
      22: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd13);
      23: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd14);
      24: req_msg_byte = hex64_digit_ascii(req_i.addr, 5'd15);
      25: req_msg_byte = 8'h0D;  // \r
      default: req_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  wire req_accept = icache_req_valid && icache_req_ready;
  wire trace_accept = trace_instr_valid;
  wire boot_delay_done = (boot_delay_q == '0);
  wire summary_done = (msg_kind_q == MsgBoot   && msg_idx_q == BootMsgLastIdx) ||
                      (msg_kind_q == MsgReq0   && msg_idx_q == ReqMsgLastIdx) ||
                      (msg_kind_q == MsgReq1   && msg_idx_q == ReqMsgLastIdx) ||
                      (msg_kind_q == MsgRet0   && msg_idx_q == RetMsgLastIdx) ||
                      (msg_kind_q == MsgRet1   && msg_idx_q == RetMsgLastIdx) ||
                      (msg_kind_q == MsgStatus && msg_idx_q == StatusMsgLastIdx);

  always_comb begin
    tx_wvalid = 1'b0;
    tx_wdata  = 8'h00;

    if (msg_active_q && tx_wready) begin
      tx_wvalid = 1'b1;
      unique case (msg_kind_q)
        MsgBoot:   tx_wdata = boot_msg_byte(msg_idx_q);
        MsgReq0:   tx_wdata = req_msg_byte(1'b0, req_q[0], msg_idx_q);
        MsgReq1:   tx_wdata = req_msg_byte(1'b1, req_q[1], msg_idx_q);
        MsgRet0:   tx_wdata = retire_msg_byte(1'b0, retire_q[0], msg_idx_q);
        MsgRet1:   tx_wdata = retire_msg_byte(1'b1, retire_q[1], msg_idx_q);
        default:   tx_wdata = status_msg_byte(pass_q, msg_idx_q);
      endcase
    end
  end

  always_comb begin
    debug_in = '0;

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
    .DebugApbEn(1'b0),
    .DebugMonEn(1'b0),
    .TraceEn(1'b1),
    .VpuEn  (1'b0)
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
    .chicken_bits_i             ('0),
    .l2_dcache_evict_req_ready_i('1),
    .l2_dcache_evict_req_valid_o(l2_dcache_evict_req_valid),
    .l2_dcache_evict_req_o      (),
    .l2_dcache_miss_req_ready_i ('1),
    .l2_dcache_miss_req_valid_o (l2_dcache_miss_req_valid),
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
    .icache_flush_data_o        (icache_flush_data),
    .satp_info_o                (satp_info_unused),
    .matp_info_o                (matp_info_unused),
    .tlb_invalidate_o           (tlb_invalidate),
    .dc_ptw_req_data_o          (dc_ptw_req_data),
    .dc_ptw_req_valid_o         (dc_ptw_req_valid),
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
    .flb_neigh_req_valid_o      (flb_neigh_req_valid),
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
    icache_req_ready = 1'b1;
    icache_resp_valid = icache_feed_q;
    icache_resp = '0;
    icache_resp.data = nop_line();
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      icache_feed_q <= 1'b0;
    end else begin
      if (req_accept) begin
        icache_feed_q <= 1'b1;
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      timeout_q       <= TimeoutInitVal[TimeoutW-1:0];
      done_q          <= 1'b0;
      pass_q          <= 1'b0;
      fail_q          <= 1'b0;
      req_count_q     <= '0;
      req_q[0]        <= '0;
      req_q[1]        <= '0;
      retire_count_q  <= '0;
      retire_q[0]     <= '0;
      retire_q[1]     <= '0;
    end else begin
      if (!done_q && timeout_q != '0) begin
        timeout_q <= timeout_q - 1'b1;
      end

      if (req_accept && req_count_q < 2'd2) begin
        req_q[req_count_q[0]].valid <= 1'b1;
        req_q[req_count_q[0]].addr  <= {{(64-$bits(icache_req.addr)){1'b0}}, icache_req.addr};
        req_count_q <= req_count_q + 1'b1;
      end

      if (!done_q && trace_accept) begin
        if (retire_count_q < 2'd2) begin
          retire_q[retire_count_q[0]].pc   <= {{(64-ShireTeInstrAddrWidth){1'b0}}, trace_instr_addr};
          retire_q[retire_count_q[0]].inst <= trace_instr_bus;
          retire_q[retire_count_q[0]].xcpt <= trace_exception;
          retire_count_q <= retire_count_q + 1'b1;
        end
      end

      if (!done_q && retire_count_q == 2'd2) begin
        done_q <= 1'b1;
        pass_q <= (retire_q[0].pc == ResetVector) &&
                  (retire_q[0].inst == NopInst) &&
                  !retire_q[0].xcpt &&
                  (retire_q[1].pc == (ResetVector + 64'd4)) &&
                  (retire_q[1].inst == NopInst) &&
                  !retire_q[1].xcpt;
        fail_q <= !((retire_q[0].pc == ResetVector) &&
                    (retire_q[0].inst == NopInst) &&
                    !retire_q[0].xcpt &&
                    (retire_q[1].pc == (ResetVector + 64'd4)) &&
                    (retire_q[1].inst == NopInst) &&
                    !retire_q[1].xcpt);
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
              msg_kind_q <= MsgReq0;
            end
            MsgReq0: begin
              msg_kind_q <= MsgReq1;
            end
            MsgReq1: begin
              msg_kind_q <= MsgRet0;
            end
            MsgRet0: begin
              msg_kind_q <= MsgRet1;
            end
            MsgRet1: begin
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
    icache_feed_q,
    retire_count_q[0],
    retire_count_q[1],
    tx_busy,
    !tx_empty
  };

endmodule
