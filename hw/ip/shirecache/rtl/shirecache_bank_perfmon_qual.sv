// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache performance monitor qualifier.
//
// Monitors bank signals and uses perf counter qualifier to determine
// what to measure. Resource mode: accumulate/count/max with min/max bounds.
// Event mode: opcode, hit/miss, eviction type, qwen count qualification.
//
// Replaces: shire_cache_bank_perfmon_qual (etcore-soc)

/* verilator lint_off UNUSEDSIGNAL */
module shirecache_bank_perfmon_qual
  import shirecache_pkg::*;
#(
  parameter bit          IsP0             = 1,
  parameter int unsigned PerfmonCovfSize  = 41,
  parameter int unsigned PerfmonModeSize  = 8,
  parameter int unsigned PerfmonQualLsb   = 0,
  parameter int unsigned PerfmonQualSize  = 51
) (
  // Performance monitor request/response
  output perfmon_req_info_t                                perfmon_req_info_o,
  input  perfmon_rsp_info_t                                perfmon_rsp_info_i,

  // Update counter
  output logic                                             perfmon_update_cntr_o,
  output logic [PerfmonCovfSize-1:0]                       esr_sc_perfmon_update_cntr_o,
  input  logic [PerfmonCovfSize-1:0]                       esr_sc_perfmon_cntr_i,

  // ESR qualifier inputs
  input  logic                                             perfmon_enable_i,
  input  logic                                             perfmon_event_i,
  input  logic [PerfmonModeSize-1:0]                       perfmon_mode_i,
  input  logic [PerfmonQualSize-1:PerfmonQualLsb]          perfmon_qual_i
);

  // ══════════════════════════════════════════════════════════
  // Event qualifier bit positions (local constants)
  // ══════════════════════════════════════════════════════════

  localparam int Q_RBUF_L2_HIT        =  0;
  localparam int Q_RBUF_SCP_HIT       =  1;
  localparam int Q_MSGSENDDATA        =  2;
  localparam int Q_TAG_BUBBLE         =  3;
  localparam int Q_TAG_HIT            =  4;
  localparam int Q_TAG_MISS           =  5;
  localparam int Q_EVICT_NONE         =  6;
  localparam int Q_EVICT_DIRTY        =  7;
  localparam int Q_EVICT_WRITE_AROUND =  8;
  localparam int Q_EVICT_0_QWEN       =  9;
  localparam int Q_EVICT_1_QWEN       = 10;
  localparam int Q_EVICT_2_QWEN       = 11;
  localparam int Q_EVICT_3_QWEN       = 12;
  localparam int Q_EVICT_4_QWEN       = 13;
  localparam int Q_L2_READ            = 14;
  localparam int Q_L2_WRITE           = 15;
  localparam int Q_L2_WRITE_AROUND    = 16;
  localparam int Q_L2_LOCK            = 17;
  localparam int Q_L2_UNLOCK          = 18;
  localparam int Q_L2_UNLOCKINV       = 19;
  localparam int Q_L2_FLUSH           = 20;
  localparam int Q_L2_EVICT           = 21;
  localparam int Q_L2_PREFETCH        = 22;
  localparam int Q_L2_ATOMIC          = 23;
  localparam int Q_L2_FILL            = 24;
  localparam int Q_L2_SCRUB           = 25;
  localparam int Q_L3_READ            = 26;
  localparam int Q_L3_WRITE           = 27;
  localparam int Q_L3_FLUSH           = 28;
  localparam int Q_L3_FLUSHWDATA      = 29;
  localparam int Q_L3_EVICT           = 30;
  localparam int Q_L3_EVICTWDATA      = 31;
  localparam int Q_L3_PREFETCH        = 32;
  localparam int Q_L3_ATOMIC          = 33;
  localparam int Q_L3_FILL            = 34;
  localparam int Q_L3_SCRUB           = 35;
  localparam int Q_SCP_READ           = 36;
  localparam int Q_SCP_WRITE          = 37;
  localparam int Q_SCP_FILL           = 38;
  localparam int Q_SCP_SCRUB          = 39;
  localparam int Q_SCP_ZERO           = 40;
  localparam int Q_SCP_ATOMIC         = 41;
  localparam int Q_IDX_READ           = 42;
  localparam int Q_IDX_WRITE          = 43;
  localparam int Q_IDX_ALL_INV        = 44;
  localparam int Q_IDX_L2_INV         = 45;
  localparam int Q_IDX_L2_FLUSH       = 46;
  localparam int Q_IDX_L2_EVICT       = 47;
  localparam int Q_IDX_L3_INV         = 48;
  localparam int Q_IDX_L3_FLUSH       = 49;
  localparam int Q_IDX_L3_EVICT       = 50;

  // Operation and range field positions within perfmon_qual
  localparam int OPER_LSB  = 0;
  localparam int OPER_SIZE = 8;
  localparam int MIN_LSB   = OPER_LSB + OPER_SIZE;
  localparam int MIN_SIZE  = PerfmonIncrSize + 1;
  localparam int MAX_LSB   = MIN_LSB + MIN_SIZE;
  localparam int MAX_SIZE  = PerfmonIncrSize + 1;

  // Operation encodings
  localparam int ACC_OPER = 0;
  localparam int CNT_OPER = 1;
  localparam int MAX_OPER = 2;

  // Mode encodings
  localparam int MODE_0 = 0;
  localparam int MODE_1 = 1;
  localparam int MODE_2 = 2;
  localparam int MODE_3 = 3;
  localparam int MODE_4 = 4;

  // ══════════════════════════════════════════════════════════
  // Resource monitoring
  // ══════════════════════════════════════════════════════════

  logic perfmon_acc_oper, perfmon_cnt_oper, perfmon_max_oper;
  /* verilator lint_off WIDTHEXPAND */
  assign perfmon_acc_oper = (perfmon_qual_i[OPER_LSB +: OPER_SIZE] == ACC_OPER);
  assign perfmon_cnt_oper = (perfmon_qual_i[OPER_LSB +: OPER_SIZE] == CNT_OPER);
  assign perfmon_max_oper = (perfmon_qual_i[OPER_LSB +: OPER_SIZE] == MAX_OPER);
  /* verilator lint_on WIDTHEXPAND */

  logic [MAX_SIZE-1:0] perfmon_max_val;
  logic [MIN_SIZE-1:0] perfmon_min_val;
  assign perfmon_max_val = perfmon_qual_i[MAX_LSB +: MAX_SIZE];
  assign perfmon_min_val = perfmon_qual_i[MIN_LSB +: MIN_SIZE];

  logic [PerfmonIncrSize-1:0] perfmon_rsrc_incr;
  logic                       perfmon_rsrc_update;
  logic [PerfmonIncrSize+1:0] perfmon_rsp_incr;
  logic                       perfmon_max_cntr;

  assign perfmon_rsp_incr = {2'b0, perfmon_rsp_info_i.rsrc_rsp.rsrc_cntr};

  logic perfmon_rsrc_enable;
  assign perfmon_rsrc_enable = ~perfmon_event_i & perfmon_enable_i;

  always_comb begin
    perfmon_rsrc_incr   = '0;
    perfmon_rsrc_update = 1'b0;
    perfmon_max_cntr    = 1'b0;

    if (perfmon_rsrc_enable &
        (perfmon_rsp_incr >= {1'b0, perfmon_min_val}) &
        (perfmon_rsp_incr <= {1'b0, perfmon_max_val})) begin
      if (perfmon_acc_oper) begin
        perfmon_rsrc_incr   = perfmon_rsp_info_i.rsrc_rsp.rsrc_cntr;
        perfmon_rsrc_update = 1'b1;
      end
      if (perfmon_cnt_oper) begin
        perfmon_rsrc_incr   = 1;
        perfmon_rsrc_update = 1'b1;
      end
      if (perfmon_max_oper &
          (perfmon_rsp_incr > {2'b0, esr_sc_perfmon_cntr_i[PerfmonIncrSize-1:0]})) begin
        perfmon_rsrc_incr   = perfmon_rsp_info_i.rsrc_rsp.rsrc_cntr;
        perfmon_rsrc_update = 1'b1;
        perfmon_max_cntr    = 1'b1;
      end
    end
  end

  // ══════════════════════════════════════════════════════════
  // Request info (event mode enables + resource mode select)
  // ══════════════════════════════════════════════════════════

  logic perfmon_ev_0_mode;
  logic perfmon_rbuf_l2_hit_enable, perfmon_rbuf_scp_hit_enable;
  logic perfmon_msgsenddata_enable, perfmon_pipe_tag_enable;

  assign perfmon_ev_0_mode          = perfmon_event_i & perfmon_enable_i;
  assign perfmon_rbuf_l2_hit_enable = perfmon_ev_0_mode & perfmon_qual_i[Q_RBUF_L2_HIT];
  assign perfmon_rbuf_scp_hit_enable= perfmon_ev_0_mode & perfmon_qual_i[Q_RBUF_SCP_HIT];
  assign perfmon_msgsenddata_enable = perfmon_ev_0_mode & perfmon_qual_i[Q_MSGSENDDATA];
  assign perfmon_pipe_tag_enable    = perfmon_ev_0_mode;

  always_comb begin
    perfmon_req_info_o = '0;
    perfmon_req_info_o.ev_mode[PerfmonEvRbuf] = perfmon_rbuf_l2_hit_enable | perfmon_rbuf_scp_hit_enable;
    perfmon_req_info_o.ev_mode[PerfmonEvMsg]  = perfmon_msgsenddata_enable;
    perfmon_req_info_o.ev_mode[PerfmonEvTag]  = perfmon_pipe_tag_enable;

    /* verilator lint_off WIDTHEXPAND */
    if (IsP0) begin
      perfmon_req_info_o.rsrc_mode[PerfmonRsrcL2Reqq]  = perfmon_rsrc_enable & (perfmon_mode_i == MODE_0);
      perfmon_req_info_o.rsrc_mode[PerfmonRsrcL3Busy]  = perfmon_rsrc_enable & (perfmon_mode_i == MODE_1);
      perfmon_req_info_o.rsrc_mode[PerfmonRsrcAnyToL3] = perfmon_rsrc_enable & (perfmon_mode_i == MODE_2);
      perfmon_req_info_o.rsrc_mode[PerfmonRsrcL2ToL3]  = perfmon_rsrc_enable & (perfmon_mode_i == MODE_3);
      perfmon_req_info_o.rsrc_mode[PerfmonRsrcL3ToSys] = perfmon_rsrc_enable & (perfmon_mode_i == MODE_4);
    end else begin
      perfmon_req_info_o.rsrc_mode[PerfmonRsrcL3Reqq]  = perfmon_rsrc_enable & (perfmon_mode_i == MODE_0);
      perfmon_req_info_o.rsrc_mode[PerfmonRsrcL2Busy]  = perfmon_rsrc_enable & (perfmon_mode_i == MODE_1);
      perfmon_req_info_o.rsrc_mode[PerfmonRsrcAnyToSys] = perfmon_rsrc_enable & (perfmon_mode_i == MODE_2);
      perfmon_req_info_o.rsrc_mode[PerfmonRsrcL2ToSys] = perfmon_rsrc_enable & (perfmon_mode_i == MODE_3);
      perfmon_req_info_o.rsrc_mode[PerfmonRsrcCbuf]    = perfmon_rsrc_enable & (perfmon_mode_i == MODE_4);
    end
    /* verilator lint_on WIDTHEXPAND */
  end

  // ══════════════════════════════════════════════════════════
  // Event qualification
  // ══════════════════════════════════════════════════════════

  perfmon_rsp_ev_info_t ev_rsp;
  assign ev_rsp = perfmon_rsp_info_i.ev_rsp;

  // Rbuf hit
  logic rbuf_hit;
  assign rbuf_hit = (ev_rsp.ev_rbuf_l2_hit  & perfmon_rbuf_l2_hit_enable) |
                    (ev_rsp.ev_rbuf_scp_hit & perfmon_rbuf_scp_hit_enable);

  // MsgSendData
  logic msgsenddata;
  assign msgsenddata = ev_rsp.ev_msgsenddata & perfmon_msgsenddata_enable;

  // TC bubble
  logic pipe_tag_bubble;
  assign pipe_tag_bubble = ev_rsp.ev_tag_bubble &
    perfmon_qual_i[Q_TAG_BUBBLE] & perfmon_pipe_tag_enable;

  // TC hit/miss
  logic pipe_tag_hitmiss_pick;
  assign pipe_tag_hitmiss_pick =
    (ev_rsp.ev_tag_hit  & perfmon_qual_i[Q_TAG_HIT]) |
    (ev_rsp.ev_tag_miss & perfmon_qual_i[Q_TAG_MISS]);

  // TC victim type
  logic pipe_evict_pick;
  assign pipe_evict_pick =
    (ev_rsp.ev_tag_evict_dirty        & perfmon_qual_i[Q_EVICT_DIRTY]) |
    (ev_rsp.ev_tag_evict_write_around & perfmon_qual_i[Q_EVICT_WRITE_AROUND]) |
    (ev_rsp.ev_tag_evict_none         & perfmon_qual_i[Q_EVICT_NONE]);

  // TC victim qwen count
  logic pipe_evict_qwen_pick;
  assign pipe_evict_qwen_pick =
    (count_qwen(ev_rsp.ev_tag_victim_qwen, 0) & perfmon_qual_i[Q_EVICT_0_QWEN]) |
    (count_qwen(ev_rsp.ev_tag_victim_qwen, 1) & perfmon_qual_i[Q_EVICT_1_QWEN]) |
    (count_qwen(ev_rsp.ev_tag_victim_qwen, 2) & perfmon_qual_i[Q_EVICT_2_QWEN]) |
    (count_qwen(ev_rsp.ev_tag_victim_qwen, 3) & perfmon_qual_i[Q_EVICT_3_QWEN]) |
    (count_qwen(ev_rsp.ev_tag_victim_qwen, 4) & perfmon_qual_i[Q_EVICT_4_QWEN]);

  // TC opcode match
  logic pipe_opcode_pick;
  assign pipe_opcode_pick = match_opcode(ev_rsp.ev_tag_opcode, perfmon_qual_i);

  // Combined TC event
  logic pipe_tag_oper;
  assign pipe_tag_oper = perfmon_pipe_tag_enable &
    pipe_tag_hitmiss_pick & pipe_evict_pick & pipe_evict_qwen_pick & pipe_opcode_pick;

  // Event update and increment
  logic perfmon_event_0_update;
  logic [PerfmonIncrSize-1:0] perfmon_event_0_incr;
  assign perfmon_event_0_update = |{rbuf_hit, msgsenddata, pipe_tag_bubble, pipe_tag_oper};
  /* verilator lint_off WIDTHEXPAND */
  assign perfmon_event_0_incr   = rbuf_hit + msgsenddata
                                + pipe_tag_bubble + pipe_tag_oper;
  /* verilator lint_on WIDTHEXPAND */

  // ══════════════════════════════════════════════════════════
  // Final counter update
  // ══════════════════════════════════════════════════════════

  /* verilator lint_off WIDTHEXPAND */
  assign esr_sc_perfmon_update_cntr_o =
    perfmon_max_cntr
      ? perfmon_rsrc_incr
      : esr_sc_perfmon_cntr_i + (perfmon_event_0_incr | perfmon_rsrc_incr);
  /* verilator lint_on WIDTHEXPAND */

  assign perfmon_update_cntr_o = perfmon_event_0_update | perfmon_rsrc_update;

  // ══════════════════════════════════════════════════════════
  // Functions
  // ══════════════════════════════════════════════════════════

  // Match opcode against qualifier
  function automatic logic match_opcode(
    input reqq_opcode_e opcode,
    input logic [PerfmonQualSize-1:PerfmonQualLsb] qual
  );
    unique case (opcode)
      L2Read:          match_opcode = qual[Q_L2_READ];
      L2Write:         match_opcode = qual[Q_L2_WRITE];
      L2WriteAround:   match_opcode = qual[Q_L2_WRITE_AROUND];
      L2Lock:          match_opcode = qual[Q_L2_LOCK];
      L2Unlock:        match_opcode = qual[Q_L2_UNLOCK];
      L2UnlockInv:     match_opcode = qual[Q_L2_UNLOCKINV];
      L2Flush:         match_opcode = qual[Q_L2_FLUSH];
      L2Evict:         match_opcode = qual[Q_L2_EVICT];
      L2Prefetch:      match_opcode = qual[Q_L2_PREFETCH];
      L2Atomic:        match_opcode = qual[Q_L2_ATOMIC];
      L2Fill:          match_opcode = qual[Q_L2_FILL];
      L2Scrub:         match_opcode = qual[Q_L2_SCRUB];
      L3Read:          match_opcode = qual[Q_L3_READ];
      L3Write:         match_opcode = qual[Q_L3_WRITE];
      L3Flush:         match_opcode = qual[Q_L3_FLUSH];
      L3FlushWData:    match_opcode = qual[Q_L3_FLUSHWDATA];
      L3Evict:         match_opcode = qual[Q_L3_EVICT];
      L3EvictWData:    match_opcode = qual[Q_L3_EVICTWDATA];
      L3Prefetch:      match_opcode = qual[Q_L3_PREFETCH];
      L3Atomic:        match_opcode = qual[Q_L3_ATOMIC];
      L3Fill:          match_opcode = qual[Q_L3_FILL];
      L3Scrub:         match_opcode = qual[Q_L3_SCRUB];
      ScpRead:         match_opcode = qual[Q_SCP_READ];
      ScpWrite:        match_opcode = qual[Q_SCP_WRITE];
      ScpFill:         match_opcode = qual[Q_SCP_FILL];
      ScpScrub:        match_opcode = qual[Q_SCP_SCRUB];
      ScpZero:         match_opcode = qual[Q_SCP_ZERO];
      ScpAtomic:       match_opcode = qual[Q_SCP_ATOMIC];
      IdxRead:         match_opcode = qual[Q_IDX_READ];
      IdxWrite:        match_opcode = qual[Q_IDX_WRITE];
      IdxAllInv:       match_opcode = qual[Q_IDX_ALL_INV];
      IdxL2Inv:        match_opcode = qual[Q_IDX_L2_INV];
      IdxL2Flush:      match_opcode = qual[Q_IDX_L2_FLUSH];
      IdxL2Evict:      match_opcode = qual[Q_IDX_L2_EVICT];
      IdxL3Inv:        match_opcode = qual[Q_IDX_L3_INV];
      IdxL3Flush:      match_opcode = qual[Q_IDX_L3_FLUSH];
      IdxL3Evict:      match_opcode = qual[Q_IDX_L3_EVICT];
      default:         match_opcode = 1'b0;
    endcase
  endfunction

  // Count popcount of qwen and compare against expected count
  function automatic logic count_qwen(input logic [3:0] qwen, input int cnt);
    int c;
    unique case (qwen)
      4'h0: c = 0;  4'h1: c = 1;  4'h2: c = 1;  4'h3: c = 2;
      4'h4: c = 1;  4'h5: c = 2;  4'h6: c = 2;  4'h7: c = 3;
      4'h8: c = 1;  4'h9: c = 2;  4'ha: c = 2;  4'hb: c = 3;
      4'hc: c = 2;  4'hd: c = 3;  4'he: c = 3;  4'hf: c = 4;
    endcase
    count_qwen = (cnt == c);
  endfunction

  logic unused_ok;
  assign unused_ok = &{1'b0,
    perfmon_rsp_info_i, esr_sc_perfmon_cntr_i,
    perfmon_enable_i, perfmon_event_i, perfmon_mode_i, perfmon_qual_i,
    unused_ok};

  /* verilator lint_on UNUSEDSIGNAL */

endmodule
