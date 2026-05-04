// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_dcache -- direct minion_dcache_top bring-up harness.
//
// Reuses the existing minion_dcache_top_tb wrapper, runs a short built-in
// directed self-check over scalar request / PTW / APB-prefilled hit paths, and
// prints a UART summary followed by PASS/FAIL.

module test_dcache #(
  parameter int unsigned ClkHz         = 25_000_000,
  parameter int unsigned BaudRate      = 115200,
  parameter int unsigned TimeoutCycles = 4096,
  parameter bit          DebugApbEn    = 1'b1,
  parameter bit          DebugMonEn    = 1'b0,
  parameter bit          VpuEn         = 1'b0,
  localparam int unsigned TimeoutW     = $clog2(TimeoutCycles + 1),
  localparam int unsigned MsgIdxW      = 6
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  output logic       uart_tx_o,
  output logic [7:0] led_o
);

  typedef enum logic [5:0] {
    StReset0,
    StWaitReady0,
    StIssuePhys,
    StSamplePhys,
    StCheckPhys,
    StReset1,
    StWaitReady1,
    StProgramSatpPulse,
    StProgramSatpData,
    StIssueVirt,
    StWaitPtw,
    StReset2,
    StWaitReady2,
    StApbWriteMetaLoadSetup,
    StApbWriteMetaLoadAccess,
    StApbReadMetaLoadSetup,
    StApbReadMetaLoadAccess,
    StApbWriteDataLoadSetup,
    StApbWriteDataLoadAccess,
    StApbReadDataLoadSetup,
    StApbReadDataLoadAccess,
    StApbDrainLoad,
    StWaitReadyLoadHit,
    StPreLoadHit,
    StIssueLoadHit,
    StWaitLoadHit,
    StApbReadDataAfterLoadSetup,
    StApbReadDataAfterLoadAccess,
    StApbDrainAfterLoad,
    StReset3,
    StWaitReady3,
    StApbWriteMetaStoreSetup,
    StApbWriteMetaStoreAccess,
    StApbWriteDataStoreSetup,
    StApbWriteDataStoreAccess,
    StApbDrainStore,
    StWaitReadyStoreHit,
    StPreStoreHit,
    StIssueStoreHit,
    StWaitStoreHit,
    StPreLoadBack,
    StIssueLoadBack,
    StWaitLoadBack,
    StPrint,
    StDone
  } test_state_e;

  typedef enum logic [3:0] {
    MsgBoot,
    MsgIdle,
    MsgBp,
    MsgPtw,
    MsgMeta,
    MsgData,
    MsgLoad,
    MsgLoadDiag,
    MsgHitDiag,
    MsgHitRaw,
    MsgHitSync,
    MsgHitGate,
    MsgDataAfterLoad,
    MsgStore,
    MsgStatus
  } msg_kind_e;

  localparam int unsigned WaitReadyCycles = 255;
  localparam int unsigned WaitPtwCycles = 15;
  localparam int unsigned WaitRespCycles = 15;
  localparam int unsigned WaitApbDrainCycles = 8;
  localparam logic [TimeoutW-1:0] WaitOne = {{(TimeoutW-1){1'b0}}, 1'b1};

  localparam logic [3:0]  Sv39Mode = 4'd8;
  localparam logic [4:0]  CmdXrd = 5'd0;
  localparam logic [4:0]  CmdXwr = 5'd1;
  localparam logic [3:0]  TypeW = 4'd2;
  localparam logic [1:0]  LoadStoreWay = 2'd1;

  localparam logic [63:0] PhysAddr = 64'h0000_0000_0000_1234;
  localparam logic [63:0] VirtAddr = 64'h0000_0000_ABCD_E000;
  localparam logic [63:0] ExpectedPtwPpn = 64'h0000_0000_0001_2345;
  localparam logic [63:0] ExpectedPtwAddr = 64'h0000_0000_000A_BCDE;
  // Keep the low set/word bits at 0x200, but put the hit-path address in
  // DRAM-cacheable space. 0x1200 alone is IO-space and therefore uncacheable.
  localparam logic [63:0] LoadStoreAddr = 64'h0000_0080_0100_1200;
  localparam logic [63:0] LoadSeed      = 64'h0123_4567_89AB_CDEF;
  localparam logic [63:0] StoreWrite    = 64'hFEDC_BA98_7654_3210;
  localparam logic [15:0] HitSyncExpected = 16'h2222;

  localparam logic [MsgIdxW-1:0] BootMsgLastIdx   = 6'd20;
  localparam logic [MsgIdxW-1:0] IdleMsgLastIdx   = 6'd26;
  localparam logic [MsgIdxW-1:0] BpMsgLastIdx     = 6'd45;
  localparam logic [MsgIdxW-1:0] PtwMsgLastIdx    = 6'd53;
  localparam logic [MsgIdxW-1:0] RdWrMsgLastIdx   = 6'd41;
  localparam logic [MsgIdxW-1:0] DiagMsgLastIdx   = 6'd47;
  localparam logic [MsgIdxW-1:0] HitDiagMsgLastIdx = 6'd56;
  localparam logic [MsgIdxW-1:0] HitRawMsgLastIdx = 6'd29;
  localparam logic [MsgIdxW-1:0] StatusMsgLastIdx = 6'd19;

  logic                          dut_rst_ni;
  logic                          id_core_alloc_rq_pre;
  logic                          s0_core_alloc_rq_val;
  logic                          id_core_gsc;
  logic                          s0_core_req_valid;
  logic [minion_pkg::VaSize-1:0] req_addr;
  logic                          req_addr_msb_err;
  logic                          req_dest_fp;
  logic [minion_pkg::XregAddrSize-1:0] req_dest_addr;
  logic                          req_thread_id;
  logic [4:0]                    req_cmd;
  logic [3:0]                    req_typ;
  logic [63:0]                   store_data_lo;
  logic [minion_pkg::CoreGscCntBits-1:0] req_gsc_cnt;
  logic [minion_pkg::DmemReqPsMaskSz-1:0] req_ps_mask;
  logic                          req_phys;
  logic                          req_mem_global;
  logic [minion_pkg::CoreGsc32IdxVSize-1:0] req_gsc32_idx;
  logic [1:0]                    vmspagesize;
  logic [1:0]                    vm_prv_t0;
  logic [1:0]                    vm_prv_t1;
  logic [3:0]                    satp_mode;
  logic [minion_pkg::PaTransSize-1:0] satp_ppn;
  logic [3:0]                    matp_mode;
  logic [minion_pkg::PaTransSize-1:0] matp_ppn;
  logic                          satp_info_en;
  logic                          matp_info_en;
  logic                          ptw_req_ready;
  logic                          ptw_resp_canceled_req;
  logic                          ptw_resp_access_fault;
  logic [7:0]                    ptw_resp_rsvd_for_hw;
  logic [minion_pkg::CsrSatpPpnSz-1:0] ptw_resp_ppn;
  logic [1:0]                    ptw_resp_rsvd_for_sw;
  logic                          ptw_resp_d;
  logic                          ptw_resp_a;
  logic                          ptw_resp_g;
  logic                          ptw_resp_u;
  logic                          ptw_resp_x;
  logic                          ptw_resp_w;
  logic                          ptw_resp_r;
  logic                          ptw_resp_v;
  logic                          ptw_resp_valid;
  logic [minion_pkg::DcacheDbgAddrBits-1:0] apb_paddr;
  logic                          apb_pwrite;
  logic                          apb_psel;
  logic                          apb_penable;
  logic [minion_pkg::BpamShireApbDataWidth-1:0] apb_pwdata;
  logic                          apb_pready;
  logic [minion_pkg::BpamShireApbDataWidth-1:0] apb_prdata;
  logic [minion_pkg::DcacheL2MissReqPorts-1:0] l2_miss_req_valid;
  /* verilator lint_off UNUSEDSIGNAL */  // Hit-path harness only checks that no L2 miss is issued; miss payload fields are intentionally unobserved.
  minion_dcache_pkg::et_link_minion_miss_req_info_t l2_miss_req;
  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_off UNUSEDSIGNAL */  // No L2 response is driven in the APB-prefilled hit harness.
  logic                          l2_resp_ready;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                          l2_resp_valid;
  minion_pkg::et_link_minion_rsp_info_t l2_resp;

  logic                          id_core_ready;
  logic                          s1_bp_conf_valid;
  logic [2:0]                    s1_bp_conf_cmd;
  logic                          s1_bp_conf_thread_id;
  logic [minion_pkg::VaSizeExt-1:0] s1_bp_conf_addr;
  logic                          ptw_req_valid;
  logic [3:0]                    ptw_req_satp_mode;
  logic [minion_pkg::PaTransSize-1:0] ptw_req_satp_ppn;
  logic [1:0]                    ptw_req_prv;
  logic [minion_pkg::VaSize-minion_pkg::VaUntransSize-1:0] ptw_req_addr;
  logic                          apb_pslverr;
  logic [minion_pkg::CsrNrEventsDcache-1:0] io_events;
  logic [minion_pkg::NrThreads-1:0] bus_err;
  /* verilator lint_off UNUSEDSIGNAL */  // Replay and S2 response taps are not part of this hit-path UART summary.
  logic                          s1_core_replay_next;
  logic                          s2_core_resp_int_valid;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                          s3_core_resp_valid;
  logic [63:0]                   s3_core_resp_data_lo;
  logic [minion_pkg::VaSizeExt-1:0] s3_core_resp_addr;
  logic [63:0]                   metadata_mask;
  logic [63:0]                   dcache_hit_diag;
  logic [15:0]                   dcache_hit_sync_diag;
  logic [15:0]                   dcache_hit_gate_diag;

  logic [7:0]                    tx_data;
  logic                          tx_valid;
  logic                          tx_ready;
  /* verilator lint_off UNUSEDSIGNAL */  // The harness does not currently surface the transmitter busy flag.
  logic                          tx_busy;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                          tx_fire;

  test_state_e                   state_q;
  msg_kind_e                     msg_kind_q;
  logic [MsgIdxW-1:0]            msg_idx_q;
  logic [TimeoutW-1:0]           timeout_q;
  logic [TimeoutW-1:0]           phase_wait_q;
  logic [7:0]                    error_count_q;

  logic                          idle_ready_q;
  logic                          idle_apb_err_q;
  logic [minion_pkg::CsrNrEventsDcache-1:0] idle_io_events_q;
  logic [7:0]                    idle_bus_err_q;
  logic                          metadata_mask_ok_q;

  logic                          bp_valid_q;
  logic [2:0]                    bp_cmd_q;
  logic                          bp_thread_q;
  logic [minion_pkg::VaSizeExt-1:0] bp_addr_q;
  logic [minion_pkg::CsrNrEventsDcache-1:0] bp_io_events_q;
  logic [7:0]                    bp_bus_err_q;

  logic                          ptw_seen_q;
  logic [3:0]                    ptw_mode_q;
  logic [63:0]                   ptw_ppn_q;
  logic [1:0]                    ptw_prv_q;
  logic [63:0]                   ptw_addr_q;

  logic [63:0]                   load_hit_data_q;
  logic [63:0]                   load_back_data_q;
  logic [63:0]                   meta_prefill_q;
  logic [63:0]                   data_prefill_q;
  logic [63:0]                   data_after_load_q;
  logic                          load_resp_seen_q;
  logic                          load_int_resp_seen_q;
  logic                          load_miss_seen_q;
  logic                          load_replay_seen_q;
  logic                          load_timeout_q;
  logic [7:0]                    load_bus_err_q;
  logic [63:0]                   load_miss_addr_q;
  logic [63:0]                   load_hit_diag_q;
  logic [15:0]                   load_hit_sync_diag_q;
  logic [15:0]                   load_hit_gate_diag_q;

  function automatic logic [minion_pkg::DcacheDbgAddrBits-1:0] apb_md_addr(
    input logic [1:0]  way,
    input logic [3:0]  set_idx
  );
    begin
      apb_md_addr = '0;
      apb_md_addr[0 +: minion_dcache_pkg::DcacheSetIdxWidth] = set_idx;
      apb_md_addr[minion_dcache_pkg::DcacheSetIdxWidth
                  +: minion_dcache_pkg::DcacheWayIdxWidth] = way;
    end
  endfunction

  function automatic logic [minion_pkg::DcacheDbgAddrBits-1:0] apb_da_addr(
    input logic [1:0]  way,
    input logic [6:0]  word_idx
  );
    begin
      apb_da_addr = '0;
      apb_da_addr[minion_pkg::DcacheDbgAddrBits-1] = 1'b1;
      apb_da_addr[7 +: minion_dcache_pkg::DcacheWayIdxWidth] = way;
      apb_da_addr[0 +: 7] = word_idx;
    end
  endfunction

  function automatic logic [63:0] apb_meta_word(
    input minion_dcache_pkg::dcache_line_state_e state,
    input logic [minion_dcache_pkg::PaTagExtSize-1:0] tag
  );
    begin
      apb_meta_word = '0;
      apb_meta_word[0 +: minion_dcache_pkg::PaTagExtSize] = tag;
      apb_meta_word[minion_dcache_pkg::PaTagExtSize +: 2] = state;
    end
  endfunction

  function automatic logic [7:0] ascii_bool(input logic value);
    ascii_bool = value ? 8'h31 : 8'h30;
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
    input int unsigned digit_idx
  );
    begin
      hex64_digit_ascii = hex_nibble_ascii(value[((15 - digit_idx) * 4) +: 4]);
    end
  endfunction

  function automatic logic [7:0] hex16_digit_ascii(
    input logic [15:0] value,
    input int unsigned digit_idx
  );
    begin
      hex16_digit_ascii = hex_nibble_ascii(value[((3 - digit_idx) * 4) +: 4]);
    end
  endfunction

  function automatic logic [7:0] hex12_digit_ascii(
    input logic [11:0] value,
    input int unsigned digit_idx
  );
    begin
      hex12_digit_ascii = hex_nibble_ascii(value[((2 - digit_idx) * 4) +: 4]);
    end
  endfunction

  function automatic logic [7:0] hex8_digit_ascii(
    input logic [7:0] value,
    input int unsigned digit_idx
  );
    begin
      if (digit_idx != 0)
        hex8_digit_ascii = hex_nibble_ascii(value[3:0]);
      else
        hex8_digit_ascii = hex_nibble_ascii(value[7:4]);
    end
  endfunction

  function automatic int unsigned msg_offset(
    input logic [MsgIdxW-1:0] idx,
    input logic [MsgIdxW-1:0] base
  );
    begin
      msg_offset = {{(32-MsgIdxW){1'b0}}, idx} - {{(32-MsgIdxW){1'b0}}, base};
    end
  endfunction

  function automatic logic [7:0] boot_msg_byte(input logic [MsgIdxW-1:0] idx);
    unique case (idx)
      6'd0:    boot_msg_byte = 8'h41;  // A
      6'd1:    boot_msg_byte = 8'h69;  // i
      6'd2:    boot_msg_byte = 8'h6E;  // n
      6'd3:    boot_msg_byte = 8'h65;  // e
      6'd4:    boot_msg_byte = 8'h6B;  // k
      6'd5:    boot_msg_byte = 8'h6B;  // k
      6'd6:    boot_msg_byte = 8'h6F;  // o
      6'd7:    boot_msg_byte = 8'h20;  // space
      6'd8:    boot_msg_byte = 8'h74;  // t
      6'd9:    boot_msg_byte = 8'h65;  // e
      6'd10:   boot_msg_byte = 8'h73;  // s
      6'd11:   boot_msg_byte = 8'h74;  // t
      6'd12:   boot_msg_byte = 8'h5F;  // _
      6'd13:   boot_msg_byte = 8'h64;  // d
      6'd14:   boot_msg_byte = 8'h63;  // c
      6'd15:   boot_msg_byte = 8'h61;  // a
      6'd16:   boot_msg_byte = 8'h63;  // c
      6'd17:   boot_msg_byte = 8'h68;  // h
      6'd18:   boot_msg_byte = 8'h65;  // e
      6'd19:   boot_msg_byte = 8'h0D;  // \r
      default: boot_msg_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] idle_msg_byte(input logic [MsgIdxW-1:0] idx);
    begin
      idle_msg_byte = 8'h20;

      if (idx == 6'd0) begin
        idle_msg_byte = 8'h49;  // I
      end else if (idx == 6'd1) begin
        idle_msg_byte = 8'h44;  // D
      end else if (idx == 6'd2) begin
        idle_msg_byte = 8'h20;
      end else if (idx == 6'd3) begin
        idle_msg_byte = 8'h52;  // R
      end else if (idx == 6'd4) begin
        idle_msg_byte = 8'h3D;  // =
      end else if (idx == 6'd5) begin
        idle_msg_byte = ascii_bool(idle_ready_q);
      end else if (idx == 6'd6) begin
        idle_msg_byte = 8'h20;
      end else if (idx == 6'd7) begin
        idle_msg_byte = 8'h50;  // P
      end else if (idx == 6'd8) begin
        idle_msg_byte = 8'h3D;  // =
      end else if (idx == 6'd9) begin
        idle_msg_byte = ascii_bool(idle_apb_err_q);
      end else if (idx == 6'd10) begin
        idle_msg_byte = 8'h20;
      end else if (idx == 6'd11) begin
        idle_msg_byte = 8'h49;  // I
      end else if (idx == 6'd12) begin
        idle_msg_byte = 8'h3D;  // =
      end else if ((idx >= 6'd13) && (idx <= 6'd15)) begin
        idle_msg_byte = hex12_digit_ascii(idle_io_events_q, msg_offset(idx, 13));
      end else if (idx == 6'd16) begin
        idle_msg_byte = 8'h20;
      end else if (idx == 6'd17) begin
        idle_msg_byte = 8'h42;  // B
      end else if (idx == 6'd18) begin
        idle_msg_byte = 8'h3D;  // =
      end else if ((idx == 6'd19) || (idx == 6'd20)) begin
        idle_msg_byte = hex8_digit_ascii(idle_bus_err_q, msg_offset(idx, 19));
      end else if (idx == 6'd21) begin
        idle_msg_byte = 8'h20;
      end else if (idx == 6'd22) begin
        idle_msg_byte = 8'h4D;  // M
      end else if (idx == 6'd23) begin
        idle_msg_byte = 8'h3D;  // =
      end else if (idx == 6'd24) begin
        idle_msg_byte = ascii_bool(metadata_mask_ok_q);
      end else if (idx == 6'd25) begin
        idle_msg_byte = 8'h0D;
      end else begin
        idle_msg_byte = 8'h0A;
      end
    end
  endfunction

  function automatic logic [7:0] bp_msg_byte(input logic [MsgIdxW-1:0] idx);
    begin
      bp_msg_byte = 8'h20;

      if (idx == 6'd0) begin
        bp_msg_byte = 8'h42;  // B
      end else if (idx == 6'd1) begin
        bp_msg_byte = 8'h50;  // P
      end else if (idx == 6'd2) begin
        bp_msg_byte = 8'h20;
      end else if (idx == 6'd3) begin
        bp_msg_byte = 8'h56;  // V
      end else if (idx == 6'd4) begin
        bp_msg_byte = 8'h3D;
      end else if (idx == 6'd5) begin
        bp_msg_byte = ascii_bool(bp_valid_q);
      end else if (idx == 6'd6) begin
        bp_msg_byte = 8'h20;
      end else if (idx == 6'd7) begin
        bp_msg_byte = 8'h43;  // C
      end else if (idx == 6'd8) begin
        bp_msg_byte = 8'h3D;
      end else if (idx == 6'd9) begin
        bp_msg_byte = hex_nibble_ascii({1'b0, bp_cmd_q});
      end else if (idx == 6'd10) begin
        bp_msg_byte = 8'h20;
      end else if (idx == 6'd11) begin
        bp_msg_byte = 8'h54;  // T
      end else if (idx == 6'd12) begin
        bp_msg_byte = 8'h3D;
      end else if (idx == 6'd13) begin
        bp_msg_byte = ascii_bool(bp_thread_q);
      end else if (idx == 6'd14) begin
        bp_msg_byte = 8'h20;
      end else if (idx == 6'd15) begin
        bp_msg_byte = 8'h41;  // A
      end else if (idx == 6'd16) begin
        bp_msg_byte = 8'h3D;
      end else if ((idx >= 6'd17) && (idx <= 6'd32)) begin
        bp_msg_byte = hex64_digit_ascii({{(64-minion_pkg::VaSizeExt){1'b0}}, bp_addr_q}, msg_offset(idx, 17));
      end else if (idx == 6'd33) begin
        bp_msg_byte = 8'h20;
      end else if (idx == 6'd34) begin
        bp_msg_byte = 8'h45;  // E
      end else if (idx == 6'd35) begin
        bp_msg_byte = 8'h3D;
      end else if ((idx >= 6'd36) && (idx <= 6'd38)) begin
        bp_msg_byte = hex12_digit_ascii(bp_io_events_q, msg_offset(idx, 36));
      end else if (idx == 6'd39) begin
        bp_msg_byte = 8'h20;
      end else if (idx == 6'd40) begin
        bp_msg_byte = 8'h42;  // B
      end else if (idx == 6'd41) begin
        bp_msg_byte = 8'h3D;
      end else if ((idx == 6'd42) || (idx == 6'd43)) begin
        bp_msg_byte = hex8_digit_ascii(bp_bus_err_q, msg_offset(idx, 42));
      end else if (idx == 6'd44) begin
        bp_msg_byte = 8'h0D;
      end else begin
        bp_msg_byte = 8'h0A;
      end
    end
  endfunction

  function automatic logic [7:0] ptw_msg_byte(input logic [MsgIdxW-1:0] idx);
    begin
      ptw_msg_byte = 8'h20;

      if (idx == 6'd0) begin
        ptw_msg_byte = 8'h50;  // P
      end else if (idx == 6'd1) begin
        ptw_msg_byte = 8'h54;  // T
      end else if (idx == 6'd2) begin
        ptw_msg_byte = 8'h20;
      end else if (idx == 6'd3) begin
        ptw_msg_byte = 8'h56;  // V
      end else if (idx == 6'd4) begin
        ptw_msg_byte = 8'h3D;
      end else if (idx == 6'd5) begin
        ptw_msg_byte = ascii_bool(ptw_seen_q);
      end else if (idx == 6'd6) begin
        ptw_msg_byte = 8'h20;
      end else if (idx == 6'd7) begin
        ptw_msg_byte = 8'h4D;  // M
      end else if (idx == 6'd8) begin
        ptw_msg_byte = 8'h3D;
      end else if (idx == 6'd9) begin
        ptw_msg_byte = hex_nibble_ascii(ptw_mode_q);
      end else if (idx == 6'd10) begin
        ptw_msg_byte = 8'h20;
      end else if (idx == 6'd11) begin
        ptw_msg_byte = 8'h50;  // P
      end else if (idx == 6'd12) begin
        ptw_msg_byte = 8'h3D;
      end else if ((idx >= 6'd13) && (idx <= 6'd28)) begin
        ptw_msg_byte = hex64_digit_ascii(ptw_ppn_q, msg_offset(idx, 13));
      end else if (idx == 6'd29) begin
        ptw_msg_byte = 8'h20;
      end else if (idx == 6'd30) begin
        ptw_msg_byte = 8'h52;  // R
      end else if (idx == 6'd31) begin
        ptw_msg_byte = 8'h3D;
      end else if (idx == 6'd32) begin
        ptw_msg_byte = hex_nibble_ascii({2'b00, ptw_prv_q});
      end else if (idx == 6'd33) begin
        ptw_msg_byte = 8'h20;
      end else if (idx == 6'd34) begin
        ptw_msg_byte = 8'h41;  // A
      end else if (idx == 6'd35) begin
        ptw_msg_byte = 8'h3D;
      end else if ((idx >= 6'd36) && (idx <= 6'd51)) begin
        ptw_msg_byte = hex64_digit_ascii(ptw_addr_q, msg_offset(idx, 36));
      end else if (idx == 6'd52) begin
        ptw_msg_byte = 8'h0D;
      end else begin
        ptw_msg_byte = 8'h0A;
      end
    end
  endfunction

  function automatic logic [7:0] rdwr_msg_byte(
    input logic [7:0]          tag0,
    input logic [7:0]          tag1,
    input logic [63:0]         write_data,
    input logic [63:0]         read_data,
    input logic [MsgIdxW-1:0]  idx
  );
    begin
      rdwr_msg_byte = 8'h20;

      if (idx == 6'd0) begin
        rdwr_msg_byte = tag0;
      end else if (idx == 6'd1) begin
        rdwr_msg_byte = tag1;
      end else if (idx == 6'd2) begin
        rdwr_msg_byte = 8'h20;
      end else if (idx == 6'd3) begin
        rdwr_msg_byte = 8'h57;  // W
      end else if (idx == 6'd4) begin
        rdwr_msg_byte = 8'h3D;
      end else if ((idx >= 6'd5) && (idx <= 6'd20)) begin
        rdwr_msg_byte = hex64_digit_ascii(write_data, msg_offset(idx, 5));
      end else if (idx == 6'd21) begin
        rdwr_msg_byte = 8'h20;
      end else if (idx == 6'd22) begin
        rdwr_msg_byte = 8'h52;  // R
      end else if (idx == 6'd23) begin
        rdwr_msg_byte = 8'h3D;
      end else if ((idx >= 6'd24) && (idx <= 6'd39)) begin
        rdwr_msg_byte = hex64_digit_ascii(read_data, msg_offset(idx, 24));
      end else if (idx == 6'd40) begin
        rdwr_msg_byte = 8'h0D;
      end else begin
        rdwr_msg_byte = 8'h0A;
      end
    end
  endfunction

  function automatic logic [7:0] status_msg_byte(input logic [MsgIdxW-1:0] idx);
    logic pass;
    pass = (error_count_q == 0);
    unique case (idx)
      6'd0:    status_msg_byte = 8'h54;  // T
      6'd1:    status_msg_byte = 8'h45;  // E
      6'd2:    status_msg_byte = 8'h53;  // S
      6'd3:    status_msg_byte = 8'h54;  // T
      6'd4:    status_msg_byte = 8'h20;  // space
      6'd5:    status_msg_byte = 8'h44;  // D
      6'd6:    status_msg_byte = 8'h43;  // C
      6'd7:    status_msg_byte = 8'h41;  // A
      6'd8:    status_msg_byte = 8'h43;  // C
      6'd9:    status_msg_byte = 8'h48;  // H
      6'd10:   status_msg_byte = 8'h45;  // E
      6'd11:   status_msg_byte = 8'h20;  // space
      6'd12:   status_msg_byte = pass ? 8'h50 : 8'h46;  // P/F
      6'd13:   status_msg_byte = pass ? 8'h41 : 8'h41;  // A
      6'd14:   status_msg_byte = pass ? 8'h53 : 8'h49;  // S/I
      6'd15:   status_msg_byte = pass ? 8'h53 : 8'h4C;  // S/L
      6'd16:   status_msg_byte = pass ? 8'h45 : 8'h45;  // E
      6'd17:   status_msg_byte = pass ? 8'h44 : 8'h44;  // D
      6'd18:   status_msg_byte = 8'h0D;
      default: status_msg_byte = 8'h0A;
    endcase
  endfunction

  function automatic logic [7:0] load_diag_msg_byte(input logic [MsgIdxW-1:0] idx);
    begin
      load_diag_msg_byte = 8'h20;

      if (idx == 6'd0) begin
        load_diag_msg_byte = 8'h4C;  // L
      end else if (idx == 6'd1) begin
        load_diag_msg_byte = 8'h44;  // D
      end else if (idx == 6'd3) begin
        load_diag_msg_byte = 8'h56;  // V
      end else if (idx == 6'd4) begin
        load_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd5) begin
        load_diag_msg_byte = ascii_bool(load_resp_seen_q);
      end else if (idx == 6'd7) begin
        load_diag_msg_byte = 8'h49;  // I
      end else if (idx == 6'd8) begin
        load_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd9) begin
        load_diag_msg_byte = ascii_bool(load_int_resp_seen_q);
      end else if (idx == 6'd11) begin
        load_diag_msg_byte = 8'h4D;  // M
      end else if (idx == 6'd12) begin
        load_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd13) begin
        load_diag_msg_byte = ascii_bool(load_miss_seen_q);
      end else if (idx == 6'd15) begin
        load_diag_msg_byte = 8'h52;  // R
      end else if (idx == 6'd16) begin
        load_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd17) begin
        load_diag_msg_byte = ascii_bool(load_replay_seen_q);
      end else if (idx == 6'd19) begin
        load_diag_msg_byte = 8'h54;  // T
      end else if (idx == 6'd20) begin
        load_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd21) begin
        load_diag_msg_byte = ascii_bool(load_timeout_q);
      end else if (idx == 6'd23) begin
        load_diag_msg_byte = 8'h42;  // B
      end else if (idx == 6'd24) begin
        load_diag_msg_byte = 8'h3D;
      end else if ((idx == 6'd25) || (idx == 6'd26)) begin
        load_diag_msg_byte = hex8_digit_ascii(load_bus_err_q, msg_offset(idx, 25));
      end else if (idx == 6'd28) begin
        load_diag_msg_byte = 8'h41;  // A
      end else if (idx == 6'd29) begin
        load_diag_msg_byte = 8'h3D;
      end else if ((idx >= 6'd30) && (idx <= 6'd45)) begin
        load_diag_msg_byte = hex64_digit_ascii(load_miss_addr_q, msg_offset(idx, 30));
      end else if (idx == 6'd46) begin
        load_diag_msg_byte = 8'h0D;
      end else if (idx == 6'd47) begin
        load_diag_msg_byte = 8'h0A;
      end
    end
  endfunction

  function automatic logic [7:0] hit_diag_msg_byte(input logic [MsgIdxW-1:0] idx);
    begin
      hit_diag_msg_byte = 8'h20;

      if (idx == 6'd0) begin
        hit_diag_msg_byte = 8'h48;  // H
      end else if (idx == 6'd1) begin
        hit_diag_msg_byte = 8'h58;  // X
      end else if (idx == 6'd3) begin
        hit_diag_msg_byte = 8'h41;  // A
      end else if (idx == 6'd4) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd5) begin
        hit_diag_msg_byte = hex_nibble_ascii(load_hit_diag_q[3:0]);
      end else if (idx == 6'd7) begin
        hit_diag_msg_byte = 8'h42;  // B
      end else if (idx == 6'd8) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd9) begin
        hit_diag_msg_byte = hex_nibble_ascii(load_hit_diag_q[7:4]);
      end else if (idx == 6'd11) begin
        hit_diag_msg_byte = 8'h56;  // V
      end else if (idx == 6'd12) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd13) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[8]);
      end else if (idx == 6'd14) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[9]);
      end else if (idx == 6'd16) begin
        hit_diag_msg_byte = 8'h48;  // H
      end else if (idx == 6'd17) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd18) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[10]);
      end else if (idx == 6'd20) begin
        hit_diag_msg_byte = 8'h51;  // Q
      end else if (idx == 6'd21) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd22) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[11]);
      end else if (idx == 6'd24) begin
        hit_diag_msg_byte = 8'h4E;  // N
      end else if (idx == 6'd25) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd26) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[12]);
      end else if (idx == 6'd28) begin
        hit_diag_msg_byte = 8'h43;  // C
      end else if (idx == 6'd29) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd30) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[13]);
      end else if (idx == 6'd31) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[16]);
      end else if (idx == 6'd33) begin
        hit_diag_msg_byte = 8'h46;  // F
      end else if (idx == 6'd34) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd35) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[14]);
      end else if (idx == 6'd36) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[15]);
      end else if (idx == 6'd38) begin
        hit_diag_msg_byte = 8'h54;  // T
      end else if (idx == 6'd39) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd40) begin
        hit_diag_msg_byte = hex_nibble_ascii(load_hit_diag_q[23:20]);
      end else if (idx == 6'd41) begin
        hit_diag_msg_byte = 8'h20;
      end else if (idx == 6'd42) begin
        hit_diag_msg_byte = 8'h4B;  // K
      end else if (idx == 6'd43) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd44) begin
        hit_diag_msg_byte = hex_nibble_ascii({3'b000, load_hit_diag_q[28]});
      end else if (idx == 6'd45) begin
        hit_diag_msg_byte = hex_nibble_ascii(load_hit_diag_q[27:24]);
      end else if (idx == 6'd46) begin
        hit_diag_msg_byte = 8'h20;
      end else if (idx == 6'd47) begin
        hit_diag_msg_byte = 8'h50;  // P
      end else if (idx == 6'd48) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd49) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[19]);
      end else if (idx == 6'd50) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[18]);
      end else if (idx == 6'd51) begin
        hit_diag_msg_byte = 8'h20;
      end else if (idx == 6'd52) begin
        hit_diag_msg_byte = 8'h55;  // U
      end else if (idx == 6'd53) begin
        hit_diag_msg_byte = 8'h3D;
      end else if (idx == 6'd54) begin
        hit_diag_msg_byte = ascii_bool(load_hit_diag_q[17]);
      end else if (idx == 6'd55) begin
        hit_diag_msg_byte = 8'h0D;
      end else if (idx == 6'd56) begin
        hit_diag_msg_byte = 8'h0A;
      end
    end
  endfunction

  function automatic logic [7:0] hit_sync_msg_byte(input logic [MsgIdxW-1:0] idx);
    begin
      hit_sync_msg_byte = 8'h20;

      if (idx == 6'd0) begin
        hit_sync_msg_byte = 8'h48;  // H
      end else if (idx == 6'd1) begin
        hit_sync_msg_byte = 8'h59;  // Y
      end else if (idx == 6'd3) begin
        hit_sync_msg_byte = 8'h57;  // W
      end else if (idx == 6'd4) begin
        hit_sync_msg_byte = 8'h3D;
      end else if ((idx >= 6'd5) && (idx <= 6'd8)) begin
        hit_sync_msg_byte = hex16_digit_ascii(HitSyncExpected, msg_offset(idx, 5));
      end else if (idx == 6'd10) begin
        hit_sync_msg_byte = 8'h52;  // R
      end else if (idx == 6'd11) begin
        hit_sync_msg_byte = 8'h3D;
      end else if ((idx >= 6'd12) && (idx <= 6'd15)) begin
        hit_sync_msg_byte = hex16_digit_ascii(load_hit_sync_diag_q, msg_offset(idx, 12));
      end else if (idx == 6'd16) begin
        hit_sync_msg_byte = 8'h0D;
      end else if (idx == 6'd17) begin
        hit_sync_msg_byte = 8'h0A;
      end
    end
  endfunction

  function automatic logic [7:0] hit_gate_msg_byte(input logic [MsgIdxW-1:0] idx);
    begin
      hit_gate_msg_byte = 8'h20;

      if (idx == 6'd0) begin
        hit_gate_msg_byte = 8'h48;  // H
      end else if (idx == 6'd1) begin
        hit_gate_msg_byte = 8'h47;  // G
      end else if (idx == 6'd3) begin
        hit_gate_msg_byte = 8'h56;  // V
      end else if (idx == 6'd4) begin
        hit_gate_msg_byte = 8'h3D;
      end else if (idx == 6'd5) begin
        hit_gate_msg_byte = hex_nibble_ascii(load_hit_gate_diag_q[3:0]);
      end else if (idx == 6'd7) begin
        hit_gate_msg_byte = 8'h43;  // C
      end else if (idx == 6'd8) begin
        hit_gate_msg_byte = 8'h3D;
      end else if (idx == 6'd9) begin
        hit_gate_msg_byte = hex_nibble_ascii(load_hit_gate_diag_q[7:4]);
      end else if (idx == 6'd11) begin
        hit_gate_msg_byte = 8'h4D;  // M
      end else if (idx == 6'd12) begin
        hit_gate_msg_byte = 8'h3D;
      end else if (idx == 6'd13) begin
        hit_gate_msg_byte = hex_nibble_ascii(load_hit_gate_diag_q[11:8]);
      end else if (idx == 6'd15) begin
        hit_gate_msg_byte = 8'h44;  // D
      end else if (idx == 6'd16) begin
        hit_gate_msg_byte = 8'h3D;
      end else if (idx == 6'd17) begin
        hit_gate_msg_byte = hex_nibble_ascii(load_hit_gate_diag_q[15:12]);
      end else if (idx == 6'd18) begin
        hit_gate_msg_byte = 8'h0D;
      end else if (idx == 6'd19) begin
        hit_gate_msg_byte = 8'h0A;
      end
    end
  endfunction

  function automatic logic [7:0] hit_raw_msg_byte(input logic [MsgIdxW-1:0] idx);
    begin
      hit_raw_msg_byte = 8'h20;

      if (idx == 6'd0) begin
        hit_raw_msg_byte = 8'h48;  // H
      end else if (idx == 6'd1) begin
        hit_raw_msg_byte = 8'h5A;  // Z
      end else if (idx == 6'd3) begin
        hit_raw_msg_byte = 8'h52;  // R
      end else if (idx == 6'd4) begin
        hit_raw_msg_byte = 8'h3D;
      end else if (idx == 6'd5) begin
        hit_raw_msg_byte = hex_nibble_ascii(load_hit_diag_q[35:32]);
      end else if (idx == 6'd7) begin
        hit_raw_msg_byte = 8'h4B;  // K
      end else if (idx == 6'd8) begin
        hit_raw_msg_byte = 8'h3D;
      end else if (idx == 6'd9) begin
        hit_raw_msg_byte = hex_nibble_ascii(load_hit_diag_q[39:36]);
      end else if (idx == 6'd11) begin
        hit_raw_msg_byte = 8'h53;  // S
      end else if (idx == 6'd12) begin
        hit_raw_msg_byte = 8'h3D;
      end else if (idx == 6'd13) begin
        hit_raw_msg_byte = ascii_bool(load_hit_diag_q[40]);
      end else if (idx == 6'd15) begin
        hit_raw_msg_byte = 8'h59;  // Y
      end else if (idx == 6'd16) begin
        hit_raw_msg_byte = 8'h3D;
      end else if (idx == 6'd17) begin
        hit_raw_msg_byte = hex_nibble_ascii({2'b00, load_hit_diag_q[44:43]});
      end else if (idx == 6'd19) begin
        hit_raw_msg_byte = 8'h56;  // V
      end else if (idx == 6'd20) begin
        hit_raw_msg_byte = 8'h3D;
      end else if (idx == 6'd21) begin
        hit_raw_msg_byte = ascii_bool(load_hit_diag_q[47]);
      end else if (idx == 6'd22) begin
        hit_raw_msg_byte = ascii_bool(load_hit_diag_q[46]);
      end else if (idx == 6'd23) begin
        hit_raw_msg_byte = ascii_bool(load_hit_diag_q[45]);
      end else if (idx == 6'd25) begin
        hit_raw_msg_byte = 8'h4E;  // N
      end else if (idx == 6'd26) begin
        hit_raw_msg_byte = 8'h3D;
      end else if (idx == 6'd27) begin
        hit_raw_msg_byte = ascii_bool(load_hit_diag_q[48]);
      end else if (idx == 6'd28) begin
        hit_raw_msg_byte = 8'h0D;
      end else if (idx == 6'd29) begin
        hit_raw_msg_byte = 8'h0A;
      end
    end
  endfunction

  function automatic logic [MsgIdxW-1:0] msg_last_idx(input msg_kind_e kind);
    unique case (kind)
      MsgBoot:      msg_last_idx = BootMsgLastIdx;
      MsgIdle:      msg_last_idx = IdleMsgLastIdx;
      MsgBp:        msg_last_idx = BpMsgLastIdx;
      MsgPtw:       msg_last_idx = PtwMsgLastIdx;
      MsgMeta:      msg_last_idx = RdWrMsgLastIdx;
      MsgData:      msg_last_idx = RdWrMsgLastIdx;
      MsgLoad:      msg_last_idx = RdWrMsgLastIdx;
      MsgLoadDiag:  msg_last_idx = DiagMsgLastIdx;
      MsgHitDiag:   msg_last_idx = HitDiagMsgLastIdx;
      MsgHitRaw:    msg_last_idx = HitRawMsgLastIdx;
      MsgHitSync:   msg_last_idx = 6'd17;
      MsgHitGate:   msg_last_idx = 6'd19;
      MsgDataAfterLoad: msg_last_idx = RdWrMsgLastIdx;
      MsgStore:     msg_last_idx = RdWrMsgLastIdx;
      default:      msg_last_idx = StatusMsgLastIdx;
    endcase
  endfunction

  function automatic logic [7:0] msg_byte(
    input msg_kind_e          kind,
    input logic [MsgIdxW-1:0] idx
  );
    unique case (kind)
      MsgBoot:      msg_byte = boot_msg_byte(idx);
      MsgIdle:      msg_byte = idle_msg_byte(idx);
      MsgBp:        msg_byte = bp_msg_byte(idx);
      MsgPtw:       msg_byte = ptw_msg_byte(idx);
      MsgMeta:      msg_byte = rdwr_msg_byte(8'h4D, 8'h30,
                          apb_meta_word(minion_dcache_pkg::StateShared, LoadStoreAddr[39:7]),
                          meta_prefill_q, idx);
      MsgData:      msg_byte = rdwr_msg_byte(8'h44, 8'h30, LoadSeed, data_prefill_q, idx);
      MsgLoad:      msg_byte = rdwr_msg_byte(8'h4C, 8'h30, LoadSeed, load_hit_data_q, idx);
      MsgLoadDiag:  msg_byte = load_diag_msg_byte(idx);
      MsgHitDiag:   msg_byte = hit_diag_msg_byte(idx);
      MsgHitRaw:    msg_byte = hit_raw_msg_byte(idx);
      MsgHitSync:   msg_byte = hit_sync_msg_byte(idx);
      MsgHitGate:   msg_byte = hit_gate_msg_byte(idx);
      MsgDataAfterLoad:
                    msg_byte = rdwr_msg_byte(8'h44, 8'h31, LoadSeed, data_after_load_q, idx);
      MsgStore:     msg_byte = rdwr_msg_byte(8'h53, 8'h30, StoreWrite, load_back_data_q, idx);
      default:      msg_byte = status_msg_byte(idx);
    endcase
  endfunction

  function automatic msg_kind_e next_msg_kind(input msg_kind_e kind);
    unique case (kind)
      MsgBoot:      next_msg_kind = MsgIdle;
      MsgIdle:      next_msg_kind = MsgBp;
      MsgBp:        next_msg_kind = MsgPtw;
      MsgPtw:       next_msg_kind = MsgMeta;
      MsgMeta:      next_msg_kind = MsgData;
      MsgData:      next_msg_kind = MsgLoad;
      MsgLoad:      next_msg_kind = MsgLoadDiag;
      MsgLoadDiag:  next_msg_kind = MsgHitDiag;
      MsgHitDiag:   next_msg_kind = MsgHitRaw;
      MsgHitRaw:    next_msg_kind = MsgHitSync;
      MsgHitSync:   next_msg_kind = MsgHitGate;
      MsgHitGate:   next_msg_kind = MsgDataAfterLoad;
      MsgDataAfterLoad:  next_msg_kind = MsgStore;
      MsgStore:     next_msg_kind = MsgStatus;
      default:      next_msg_kind = MsgStatus;
    endcase
  endfunction

  always_comb begin
    dut_rst_ni = rst_ni;

    s0_core_req_valid    = 1'b0;
    id_core_alloc_rq_pre = 1'b0;
    s0_core_alloc_rq_val = 1'b0;
    id_core_gsc          = 1'b0;
    req_addr             = '0;
    req_addr_msb_err     = 1'b0;
    req_dest_fp          = 1'b0;
    req_dest_addr        = '0;
    req_thread_id        = 1'b0;
    req_cmd              = '0;
    req_typ              = '0;
    store_data_lo        = '0;
    req_gsc_cnt          = '0;
    req_ps_mask          = '1;
    req_phys             = 1'b0;
    req_mem_global       = 1'b0;
    req_gsc32_idx        = '0;
    vmspagesize          = '0;
    vm_prv_t0            = minion_pkg::PrvS;
    vm_prv_t1            = minion_pkg::PrvS;
    satp_mode            = '0;
    satp_ppn             = '0;
    matp_mode            = '0;
    matp_ppn             = '0;
    satp_info_en         = 1'b0;
    matp_info_en         = 1'b0;
    ptw_req_ready        = 1'b0;
    ptw_resp_canceled_req = 1'b0;
    ptw_resp_access_fault = 1'b0;
    ptw_resp_rsvd_for_hw  = '0;
    ptw_resp_ppn          = '0;
    ptw_resp_rsvd_for_sw  = '0;
    ptw_resp_d            = 1'b0;
    ptw_resp_a            = 1'b0;
    ptw_resp_g            = 1'b0;
    ptw_resp_u            = 1'b0;
    ptw_resp_x            = 1'b0;
    ptw_resp_w            = 1'b0;
    ptw_resp_r            = 1'b0;
    ptw_resp_v            = 1'b0;
    ptw_resp_valid        = 1'b0;
    apb_paddr            = '0;
    apb_pwrite           = 1'b0;
    apb_psel             = 1'b0;
    apb_penable          = 1'b0;
    apb_pwdata           = '0;
    l2_resp_valid        = 1'b0;
    l2_resp              = '0;
    l2_resp.id           = '0;
    l2_resp.opcode       = minion_pkg::EtLinkRspAckData;
    l2_resp.size         = minion_pkg::EtLinkSz64;

    if ((state_q == StReset0) || (state_q == StReset1) ||
        (state_q == StReset2) || (state_q == StReset3)) begin
      dut_rst_ni = 1'b0;
    end

    unique case (state_q)
      StIssuePhys, StSamplePhys: begin
        s0_core_req_valid = 1'b1;
        req_addr          = PhysAddr[minion_pkg::VaSize-1:0];
        req_dest_addr     = minion_pkg::XregAddrSize'(5'd1);
        req_cmd           = CmdXrd;
        req_typ           = TypeW;
        req_ps_mask       = '1;
        req_phys          = 1'b1;
      end

      StProgramSatpPulse: begin
        satp_info_en = 1'b1;
      end

      StProgramSatpData: begin
        satp_mode = Sv39Mode;
        satp_ppn  = ExpectedPtwPpn[minion_pkg::PaTransSize-1:0];
      end

      StIssueVirt: begin
        s0_core_req_valid = 1'b1;
        req_addr          = VirtAddr[minion_pkg::VaSize-1:0];
        req_dest_addr     = minion_pkg::XregAddrSize'(5'd3);
        req_cmd           = CmdXrd;
        req_typ           = TypeW;
        req_ps_mask       = '1;
        req_phys          = 1'b0;
      end

      StApbWriteMetaLoadSetup, StApbWriteMetaLoadAccess,
      StApbReadMetaLoadSetup, StApbReadMetaLoadAccess: begin
        apb_paddr   = apb_md_addr(LoadStoreWay, LoadStoreAddr[9:6]);
        apb_pwrite  = (state_q == StApbWriteMetaLoadSetup) ||
                      (state_q == StApbWriteMetaLoadAccess);
        apb_psel    = 1'b1;
        apb_penable = (state_q == StApbWriteMetaLoadAccess) ||
                      (state_q == StApbReadMetaLoadAccess);
        apb_pwdata  = apb_meta_word(minion_dcache_pkg::StateShared, LoadStoreAddr[39:7]);
      end

      StApbWriteDataLoadSetup, StApbWriteDataLoadAccess,
      StApbReadDataLoadSetup, StApbReadDataLoadAccess: begin
        apb_paddr   = apb_da_addr(LoadStoreWay, LoadStoreAddr[9:3]);
        apb_pwrite  = (state_q == StApbWriteDataLoadSetup) ||
                      (state_q == StApbWriteDataLoadAccess);
        apb_psel    = 1'b1;
        apb_penable = (state_q == StApbWriteDataLoadAccess) ||
                      (state_q == StApbReadDataLoadAccess);
        apb_pwdata  = LoadSeed;
      end

      StApbReadDataAfterLoadSetup, StApbReadDataAfterLoadAccess: begin
        apb_paddr   = apb_da_addr(LoadStoreWay, LoadStoreAddr[9:3]);
        apb_pwrite  = 1'b0;
        apb_psel    = 1'b1;
        apb_penable = (state_q == StApbReadDataAfterLoadAccess);
      end

      StApbWriteMetaStoreSetup, StApbWriteMetaStoreAccess: begin
        apb_paddr   = apb_md_addr(LoadStoreWay, LoadStoreAddr[9:6]);
        apb_pwrite  = 1'b1;
        apb_psel    = 1'b1;
        apb_penable = (state_q == StApbWriteMetaStoreAccess);
        apb_pwdata  = apb_meta_word(minion_dcache_pkg::StateModified, LoadStoreAddr[39:7]);
      end

      StApbWriteDataStoreSetup, StApbWriteDataStoreAccess: begin
        apb_paddr   = apb_da_addr(LoadStoreWay, LoadStoreAddr[9:3]);
        apb_pwrite  = 1'b1;
        apb_psel    = 1'b1;
        apb_penable = (state_q == StApbWriteDataStoreAccess);
        apb_pwdata  = LoadSeed;
      end

      StPreLoadHit, StIssueLoadHit,
      StPreLoadBack, StIssueLoadBack: begin
        id_core_alloc_rq_pre = (state_q == StPreLoadHit) || (state_q == StPreLoadBack);
        s0_core_alloc_rq_val = (state_q == StIssueLoadHit) || (state_q == StIssueLoadBack);
        s0_core_req_valid = (state_q == StIssueLoadHit) || (state_q == StIssueLoadBack);
        req_addr          = LoadStoreAddr[minion_pkg::VaSize-1:0];
        req_dest_addr     = minion_pkg::XregAddrSize'(5'd4);
        req_cmd           = CmdXrd;
        req_typ           = minion_pkg::DcTypeD;
        req_ps_mask       = '1;
        req_phys          = 1'b1;
      end

      StPreStoreHit, StIssueStoreHit: begin
        id_core_alloc_rq_pre = (state_q == StPreStoreHit);
        s0_core_alloc_rq_val = (state_q == StIssueStoreHit);
        s0_core_req_valid = (state_q == StIssueStoreHit);
        req_addr          = LoadStoreAddr[minion_pkg::VaSize-1:0];
        req_dest_addr     = minion_pkg::XregAddrSize'(5'd5);
        req_cmd           = CmdXwr;
        req_typ           = minion_pkg::DcTypeD;
        store_data_lo     = StoreWrite;
        req_ps_mask       = '1;
        req_phys          = 1'b1;
      end

      StWaitStoreHit: begin
        req_addr          = LoadStoreAddr[minion_pkg::VaSize-1:0];
        req_dest_addr     = minion_pkg::XregAddrSize'(5'd5);
        req_cmd           = CmdXwr;
        req_typ           = minion_pkg::DcTypeD;
        store_data_lo     = StoreWrite;
        req_ps_mask       = '1;
        req_phys          = 1'b1;
      end

      default: begin
      end
    endcase

    tx_data  = msg_byte(msg_kind_q, msg_idx_q);
    tx_valid = (state_q == StPrint);

    if (state_q == StDone) begin
      if (error_count_q == 0)
        led_o = 8'hA5;
      else
        led_o = 8'hF0;
    end else begin
      led_o = (error_count_q == 0) ? 8'h55 : 8'h0F;
    end
  end

  assign tx_fire = tx_valid & tx_ready;

  always_ff @(posedge clk_i or negedge rst_ni) begin : p_test
    int unsigned errs;
    if (!rst_ni) begin
      state_q            <= StReset0;
      msg_kind_q         <= MsgBoot;
      msg_idx_q          <= '0;
      timeout_q          <= '0;
      phase_wait_q       <= '0;
      error_count_q      <= '0;
      idle_ready_q       <= 1'b0;
      idle_apb_err_q     <= 1'b0;
      idle_io_events_q   <= '0;
      idle_bus_err_q     <= '0;
      metadata_mask_ok_q <= 1'b0;
      bp_valid_q         <= 1'b0;
      bp_cmd_q           <= '0;
      bp_thread_q        <= 1'b0;
      bp_addr_q          <= '0;
      bp_io_events_q     <= '0;
      bp_bus_err_q       <= '0;
      ptw_seen_q         <= 1'b0;
      ptw_mode_q         <= '0;
      ptw_ppn_q          <= '0;
      ptw_prv_q          <= '0;
      ptw_addr_q         <= '0;
      load_hit_data_q    <= '0;
      load_back_data_q   <= '0;
      meta_prefill_q     <= '0;
      data_prefill_q     <= '0;
      data_after_load_q  <= '0;
      load_resp_seen_q   <= 1'b0;
      load_int_resp_seen_q <= 1'b0;
      load_miss_seen_q   <= 1'b0;
      load_replay_seen_q <= 1'b0;
      load_timeout_q     <= 1'b0;
      load_bus_err_q     <= '0;
      load_miss_addr_q   <= '0;
      load_hit_diag_q    <= '0;
      load_hit_sync_diag_q <= '0;
      load_hit_gate_diag_q <= '0;
    end else begin
      errs = 0;

      if ((state_q != StDone) && (state_q != StPrint) && (timeout_q != TimeoutCycles[TimeoutW-1:0])) begin
        timeout_q <= timeout_q + {{(TimeoutW-1){1'b0}}, 1'b1};
      end

      if ((state_q != StDone) && (state_q != StPrint) && (timeout_q == TimeoutCycles[TimeoutW-1:0])) begin
        error_count_q <= error_count_q + 8'd1;
        state_q <= StPrint;
        msg_kind_q <= MsgBoot;
        msg_idx_q <= '0;
      end else begin
        unique case (state_q)
          StReset0, StReset1, StReset2, StReset3: begin
            if (phase_wait_q == WaitOne) begin
              phase_wait_q <= '0;
              unique case (state_q)
                StReset0:   state_q <= StWaitReady0;
                StReset1:   state_q <= StWaitReady1;
                StReset2:   state_q <= StWaitReady2;
                default:    state_q <= StWaitReady3;
              endcase
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StWaitReady0: begin
            if (id_core_ready) begin
              idle_ready_q       <= id_core_ready;
              idle_apb_err_q     <= apb_pslverr;
              idle_io_events_q   <= io_events;
              idle_bus_err_q     <= {{(8-minion_pkg::NrThreads){1'b0}}, bus_err};
              metadata_mask_ok_q <= (metadata_mask != 64'd0);
              if (ptw_req_valid != 1'b0) errs++;
              if (apb_pslverr != 1'b0) errs++;
              if (io_events != '0) errs++;
              if (bus_err != '0) errs++;
              if (metadata_mask == 64'd0) errs++;

              error_count_q <= error_count_q + errs[7:0];
              state_q <= StIssuePhys;
              phase_wait_q <= '0;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StIssuePhys: begin
            state_q <= StSamplePhys;
          end

          StSamplePhys: begin
            state_q <= StCheckPhys;
          end

          StCheckPhys: begin
            bp_valid_q     <= s1_bp_conf_valid;
            bp_cmd_q       <= s1_bp_conf_cmd;
            bp_thread_q    <= s1_bp_conf_thread_id;
            bp_addr_q      <= s1_bp_conf_addr;
            bp_io_events_q <= io_events;
            bp_bus_err_q   <= {{(8-minion_pkg::NrThreads){1'b0}}, bus_err};

            if (s1_bp_conf_valid != 1'b1) errs++;
            if (s1_bp_conf_cmd != minion_pkg::DbgBpLoad) errs++;
            if (s1_bp_conf_thread_id != 1'b0) errs++;
            if (s1_bp_conf_addr != PhysAddr[minion_pkg::VaSizeExt-1:0]) errs++;
            if (io_events[11:0] != 12'h001) errs++;
            if (bus_err != '0) errs++;

            error_count_q <= error_count_q + errs[7:0];
            phase_wait_q <= '0;
            state_q <= StReset1;
          end

          StWaitReady1: begin
            if (id_core_ready) begin
              phase_wait_q <= '0;
              state_q <= StProgramSatpPulse;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StProgramSatpPulse: begin
            state_q <= StProgramSatpData;
          end

          StProgramSatpData: begin
            state_q <= StIssueVirt;
          end

          StIssueVirt: begin
            ptw_seen_q   <= 1'b0;
            phase_wait_q <= '0;
            state_q <= StWaitPtw;
          end

          StWaitPtw: begin
            if (ptw_req_valid) begin
              ptw_seen_q <= 1'b1;
              ptw_mode_q <= ptw_req_satp_mode;
              ptw_ppn_q  <= {{(64-minion_pkg::PaTransSize){1'b0}}, ptw_req_satp_ppn};
              ptw_prv_q  <= ptw_req_prv;
              ptw_addr_q <= {{(64-(minion_pkg::VaSize-minion_pkg::VaUntransSize)){1'b0}}, ptw_req_addr};

              if (ptw_req_satp_mode != Sv39Mode) errs++;
              if (ptw_req_satp_ppn != ExpectedPtwPpn[minion_pkg::PaTransSize-1:0]) errs++;
              if (ptw_req_prv != minion_pkg::PrvS) errs++;
              if (ptw_req_addr != ExpectedPtwAddr[minion_pkg::VaSize-minion_pkg::VaUntransSize-1:0]) errs++;

              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StReset2;
            end else if (phase_wait_q == WaitPtwCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StWaitReady2: begin
            if (id_core_ready) begin
              phase_wait_q <= '0;
              state_q <= StApbWriteMetaLoadSetup;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StApbWriteMetaLoadSetup: begin
            phase_wait_q <= '0;
            state_q <= StApbWriteMetaLoadAccess;
          end

          StApbWriteMetaLoadAccess: begin
            if (apb_pready) begin
              if (apb_pslverr) errs++;
              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StApbReadMetaLoadSetup;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StApbReadMetaLoadSetup: begin
            phase_wait_q <= '0;
            state_q <= StApbReadMetaLoadAccess;
          end

          StApbReadMetaLoadAccess: begin
            if (apb_pready) begin
              meta_prefill_q <= apb_prdata;
              if (apb_pslverr) errs++;
              if (apb_prdata != apb_meta_word(minion_dcache_pkg::StateShared, LoadStoreAddr[39:7])) errs++;
              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StApbWriteDataLoadSetup;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StApbWriteDataLoadSetup: begin
            phase_wait_q <= '0;
            state_q <= StApbWriteDataLoadAccess;
          end

          StApbWriteDataLoadAccess: begin
            if (apb_pready) begin
              if (apb_pslverr) errs++;
              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StApbReadDataLoadSetup;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StApbReadDataLoadSetup: begin
            phase_wait_q <= '0;
            state_q <= StApbReadDataLoadAccess;
          end

          StApbReadDataLoadAccess: begin
            if (apb_pready) begin
              data_prefill_q <= apb_prdata;
              if (apb_pslverr) errs++;
              if (apb_prdata != LoadSeed) errs++;
              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StApbDrainLoad;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StApbDrainLoad: begin
            if (phase_wait_q == WaitApbDrainCycles[TimeoutW-1:0]) begin
              phase_wait_q <= '0;
              state_q <= StWaitReadyLoadHit;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StWaitReadyLoadHit: begin
            if (id_core_ready) begin
              phase_wait_q <= '0;
              state_q <= StPreLoadHit;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StPreLoadHit: begin
            state_q <= StIssueLoadHit;
          end

          StIssueLoadHit: begin
            load_resp_seen_q <= 1'b0;
            load_int_resp_seen_q <= 1'b0;
            load_miss_seen_q <= 1'b0;
            load_replay_seen_q <= 1'b0;
            load_timeout_q <= 1'b0;
            load_bus_err_q <= '0;
            load_miss_addr_q <= '0;
            load_hit_diag_q <= '0;
            load_hit_sync_diag_q <= '0;
            load_hit_gate_diag_q <= '0;
            phase_wait_q <= '0;
            state_q <= StWaitLoadHit;
          end

          StWaitLoadHit: begin
            load_hit_diag_q <= load_hit_diag_q | dcache_hit_diag;
            load_hit_sync_diag_q <= load_hit_sync_diag_q | dcache_hit_sync_diag;
            load_hit_gate_diag_q <= load_hit_gate_diag_q | dcache_hit_gate_diag;
            load_resp_seen_q <= load_resp_seen_q | s3_core_resp_valid;
            load_int_resp_seen_q <= load_int_resp_seen_q | s2_core_resp_int_valid;
            load_miss_seen_q <= load_miss_seen_q | (|l2_miss_req_valid);
            load_replay_seen_q <= load_replay_seen_q | s1_core_replay_next;
            load_bus_err_q <= load_bus_err_q | {{(8-minion_pkg::NrThreads){1'b0}}, bus_err};
            if (|l2_miss_req_valid) begin
              load_miss_addr_q <= {{(64-minion_dcache_pkg::PaSize){1'b0}}, l2_miss_req.address};
            end
            if (|l2_miss_req_valid) errs++;
            if (s3_core_resp_valid) begin
              load_hit_data_q <= s3_core_resp_data_lo;

              if (s3_core_resp_data_lo != LoadSeed) errs++;
              if (s3_core_resp_addr != LoadStoreAddr[minion_pkg::VaSizeExt-1:0]) errs++;
              if (bus_err != '0) errs++;

              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StApbReadDataAfterLoadSetup;
            end else if (phase_wait_q == WaitRespCycles[TimeoutW-1:0]) begin
              load_timeout_q <= 1'b1;
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StApbReadDataAfterLoadSetup: begin
            phase_wait_q <= '0;
            state_q <= StApbReadDataAfterLoadAccess;
          end

          StApbReadDataAfterLoadAccess: begin
            if (apb_pready) begin
              data_after_load_q <= apb_prdata;
              if (apb_pslverr) errs++;
              if (apb_prdata != LoadSeed) errs++;
              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StApbDrainAfterLoad;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StApbDrainAfterLoad: begin
            if (phase_wait_q == WaitApbDrainCycles[TimeoutW-1:0]) begin
              phase_wait_q <= '0;
              state_q <= StReset3;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StWaitReady3: begin
            if (id_core_ready) begin
              phase_wait_q <= '0;
              state_q <= StApbWriteMetaStoreSetup;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StApbWriteMetaStoreSetup: begin
            phase_wait_q <= '0;
            state_q <= StApbWriteMetaStoreAccess;
          end

          StApbWriteMetaStoreAccess: begin
            if (apb_pready) begin
              if (apb_pslverr) errs++;
              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StApbWriteDataStoreSetup;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StApbWriteDataStoreSetup: begin
            phase_wait_q <= '0;
            state_q <= StApbWriteDataStoreAccess;
          end

          StApbWriteDataStoreAccess: begin
            if (apb_pready) begin
              if (apb_pslverr) errs++;
              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StApbDrainStore;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StApbDrainStore: begin
            if (phase_wait_q == WaitApbDrainCycles[TimeoutW-1:0]) begin
              phase_wait_q <= '0;
              state_q <= StWaitReadyStoreHit;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StWaitReadyStoreHit: begin
            if (id_core_ready) begin
              phase_wait_q <= '0;
              state_q <= StPreStoreHit;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StPreStoreHit: begin
            state_q <= StIssueStoreHit;
          end

          StIssueStoreHit: begin
            phase_wait_q <= '0;
            state_q <= StWaitStoreHit;
          end

          StWaitStoreHit: begin
            if (|l2_miss_req_valid) errs++;
            if (id_core_ready) begin
              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StPreLoadBack;
            end else if (phase_wait_q == WaitReadyCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StPreLoadBack: begin
            state_q <= StIssueLoadBack;
          end

          StIssueLoadBack: begin
            phase_wait_q <= '0;
            state_q <= StWaitLoadBack;
          end

          StWaitLoadBack: begin
            if (|l2_miss_req_valid) errs++;
            if (s3_core_resp_valid) begin
              load_back_data_q <= s3_core_resp_data_lo;

              if (s3_core_resp_data_lo != StoreWrite) errs++;
              if (s3_core_resp_addr != LoadStoreAddr[minion_pkg::VaSizeExt-1:0]) errs++;
              if (bus_err != '0) errs++;

              error_count_q <= error_count_q + errs[7:0];
              phase_wait_q <= '0;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else if (phase_wait_q == WaitRespCycles[TimeoutW-1:0]) begin
              error_count_q <= error_count_q + 8'd1;
              state_q <= StPrint;
              msg_kind_q <= MsgBoot;
              msg_idx_q <= '0;
            end else begin
              phase_wait_q <= phase_wait_q + WaitOne;
            end
          end

          StPrint: begin
            if (tx_fire) begin
              if (msg_idx_q == msg_last_idx(msg_kind_q)) begin
                if (msg_kind_q == MsgStatus) begin
                  state_q <= StDone;
                end else begin
                  msg_kind_q <= next_msg_kind(msg_kind_q);
                  msg_idx_q <= '0;
                end
              end else begin
                msg_idx_q <= msg_idx_q + 6'd1;
              end
            end
          end

          StDone: begin
          end

          default: begin
            state_q <= StPrint;
            msg_kind_q <= MsgBoot;
            msg_idx_q <= '0;
            error_count_q <= error_count_q + 8'd1;
          end
        endcase
      end
    end
  end

  minion_dcache_top_tb #(
    .DebugApbEn(DebugApbEn),
    .DebugMonEn(DebugMonEn),
    .VpuEn(VpuEn)
  ) u_dcache (
    .clk_i,
    .rst_ni                  (dut_rst_ni),
    .id_core_alloc_rq_pre_i  (id_core_alloc_rq_pre),
    .s0_core_alloc_rq_val_i  (s0_core_alloc_rq_val),
    .id_core_gsc_i           (id_core_gsc),
    .s0_core_req_valid_i     (s0_core_req_valid),
    .req_addr_i              (req_addr),
    .req_addr_msb_err_i      (req_addr_msb_err),
    .req_dest_fp_i           (req_dest_fp),
    .req_dest_addr_i         (req_dest_addr),
    .req_thread_id_i         (req_thread_id),
    .req_cmd_i               (req_cmd),
    .req_typ_i               (req_typ),
    .store_data_lo_i         (store_data_lo),
    .req_gsc_cnt_i           (req_gsc_cnt),
    .req_ps_mask_i           (req_ps_mask),
    .req_phys_i              (req_phys),
    .req_mem_global_i        (req_mem_global),
    .req_gsc32_idx_i         (req_gsc32_idx),
    .vmspagesize_i           (vmspagesize),
    .vm_prv_t0_i             (vm_prv_t0),
    .vm_prv_t1_i             (vm_prv_t1),
    .satp_mode_i             (satp_mode),
    .satp_ppn_i              (satp_ppn),
    .matp_mode_i             (matp_mode),
    .matp_ppn_i              (matp_ppn),
    .satp_info_en_i          (satp_info_en),
    .matp_info_en_i          (matp_info_en),
    .ptw_req_ready_i         (ptw_req_ready),
    .ptw_resp_canceled_req_i (ptw_resp_canceled_req),
    .ptw_resp_access_fault_i (ptw_resp_access_fault),
    .ptw_resp_rsvd_for_hw_i  (ptw_resp_rsvd_for_hw),
    .ptw_resp_ppn_i          (ptw_resp_ppn),
    .ptw_resp_rsvd_for_sw_i  (ptw_resp_rsvd_for_sw),
    .ptw_resp_d_i            (ptw_resp_d),
    .ptw_resp_a_i            (ptw_resp_a),
    .ptw_resp_g_i            (ptw_resp_g),
    .ptw_resp_u_i            (ptw_resp_u),
    .ptw_resp_x_i            (ptw_resp_x),
    .ptw_resp_w_i            (ptw_resp_w),
    .ptw_resp_r_i            (ptw_resp_r),
    .ptw_resp_v_i            (ptw_resp_v),
    .ptw_resp_valid_i        (ptw_resp_valid),
    .apb_paddr_i             (apb_paddr),
    .apb_pwrite_i            (apb_pwrite),
    .apb_psel_i              (apb_psel),
    .apb_penable_i           (apb_penable),
    .apb_pwdata_i            (apb_pwdata),
    .l2_miss_req_valid_o     (l2_miss_req_valid),
    .l2_miss_req_o           (l2_miss_req),
    .l2_resp_ready_o         (l2_resp_ready),
    .l2_resp_valid_i         (l2_resp_valid),
    .l2_resp_i               (l2_resp),
    .id_core_ready_o         (id_core_ready),
    .s1_bp_conf_valid_o      (s1_bp_conf_valid),
    .s1_bp_conf_cmd_o        (s1_bp_conf_cmd),
    .s1_bp_conf_thread_id_o  (s1_bp_conf_thread_id),
    .s1_bp_conf_addr_o       (s1_bp_conf_addr),
    .ptw_req_valid_o         (ptw_req_valid),
    .ptw_req_satp_mode_o     (ptw_req_satp_mode),
    .ptw_req_satp_ppn_o      (ptw_req_satp_ppn),
    .ptw_req_prv_o           (ptw_req_prv),
    .ptw_req_addr_o          (ptw_req_addr),
    .apb_pready_o            (apb_pready),
    .apb_prdata_o            (apb_prdata),
    .apb_pslverr_o           (apb_pslverr),
    .io_events_o             (io_events),
    .bus_err_o               (bus_err),
    .s1_core_replay_next_o   (s1_core_replay_next),
    .s2_core_resp_int_valid_o (s2_core_resp_int_valid),
    .s3_core_resp_valid_o    (s3_core_resp_valid),
    .s3_core_resp_data_lo_o  (s3_core_resp_data_lo),
    .s3_core_resp_addr_o     (s3_core_resp_addr),
    .metadata_mask_o         (metadata_mask),
    .dcache_hit_diag_o       (dcache_hit_diag),
    .dcache_hit_sync_diag_o  (dcache_hit_sync_diag),
    .dcache_hit_gate_diag_o  (dcache_hit_gate_diag)
  );

  ulx3s_uart_tx #(
    .ClkHz    (ClkHz),
    .BaudRate (BaudRate)
  ) u_uart_tx (
    .clk_i,
    .rst_ni,
    .valid_i (tx_valid),
    .data_i  (tx_data),
    .ready_o (tx_ready),
    .tx_o    (uart_tx_o),
    .busy_o  (tx_busy)
  );

endmodule
