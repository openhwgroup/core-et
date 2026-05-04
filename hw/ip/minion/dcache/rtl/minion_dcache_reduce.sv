// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache reduce/tensor-store controller.
//
// Replaces: dcache_reduce

module minion_dcache_reduce
  import dft_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                               clk_i,
  input  logic                               rst_ni,
  input  logic [MaxMinionIdBits-1:0]         global_min_id_i,
  input  logic                               chicken_bit_dcache_i,
  output logic                               reduce_ready_o,
  input  logic                               reduce_start_i,
  output logic                               tensorstore_busy_o,
  input  logic                               tensorstore_start_i,
  /* verilator lint_off UNUSEDSIGNAL */  // Low address bits are intentionally ignored by the original stride handling.
  input  logic [minion_dcache_pkg::VaSize-1:0]                  tensorstore_stride_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  reduce_tensorstore_control_t        reduce_tensorstore_ctrl_i,
  input  logic                               thread_id_i,
  input  logic                               l2_resp_valid_i,
  /* verilator lint_off UNUSEDSIGNAL */  // Only the message-relevant response fields are consumed in the currently ported reduce logic.
  input  et_link_minion_rsp_info_t           l2_resp_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output logic                               buffer_avail_o,
  input  logic                               re_l2_req_ready_i,
  output logic                               re_l2_req_valid_o,
  output et_link_minion_evict_req_info_t     re_l2_req_o,
  input  logic                               ts_l2_req_ready_i,
  output logic                               ts_l2_req_valid_o,
  output et_link_minion_evict_req_info_t     ts_l2_req_o,
  output logic                               re_ts_l2_req_inflight_o,
  output logic                               pipe_used_o,
  output logic                               s1_inst_send_val_o,
  output logic                               s2_inst_send_val_o,
  output logic                               s3_inst_send_val_o,
  output logic                               s1_inst_op_val_o,
  output logic                               s2_inst_op_val_o,
  output logic                               s3_inst_op_val_o,
  output logic                               ts_scp_read_o,
  output logic [DcacheWayIdxWidth-1:0]       ts_scp_read_way_o,
  output logic [DcacheLramAddrWidth-1:0]     ts_scp_read_addr_o,
  output logic                               ts_col_size_is_128b_o,
  input  logic                               s0_available_i,
  output logic                               s0_addr_load_o,
  output logic [minion_dcache_pkg::VaSize-1:0]                  s0_addr_o,
  output logic                               s0_addr_msb_err_o,
  output logic                               s0_thread_id_o,
  input  logic                               s1_available_i,
  output logic                               s1_addr_load_o,
  input  logic                               s1_tlb_wait_i,
  input  logic                               s1_tlb_fail_i,
  input  logic [minion_dcache_pkg::PaSize-1:0]                  s2_addr_i,
  input  logic                               ba_full_i,
  output logic                               ba_alloc_o,
  output logic                               ba_write_needed_o,
  input  logic                               s2_valid_i,
  input  logic [DcacheBufferIdxWidth-1:0]    s2_ba_written_entry_i,
  output logic [DcacheBufferIdxWidth-1:0]    s3_ba_write_addr_o,
  output logic                               wait_l2_to_ba_resp_o,
  output logic                               ba_entries_required_o,
  input  logic                               s1_valid_i,
  input  logic                               ba_read_used_i,
  input  logic [DcacheDataSize-1:0]          s1_ba_data_i,
  output logic                               ba_read_en_o,
  output logic                               ba_read_en_spec_o,
  output logic [DcacheBufferIdxWidth-1:0]    ba_read_addr_o,
  input  logic                               scp_ctrl_on_i,
  input  logic                               vpu_reduce_wait_i,
  output dcache_vpu_reduce_ctrl_t            vpu_ctrl_o,
  input  minion_dcache_pkg::vm_status_t [NrThreads-1:0] vm_status_i,
  output minion_dcache_pkg::vm_status_t vm_status_ts_o,
  output logic                               ts_start_o,
  output logic                               ts_op_o,
  input  logic                               mh_going_to_uc_store_i,
  output logic [DcacheErrFlagRange:0][NrThreads-1:0] err_flags_o,
  output logic [NrThreads-1:0]               bus_err_o,
  output logic [minion_dcache_pkg::DcacheDbgCsrTsSize-1:0]      csr_debug_signals_o,
  output logic [DcacheDbgSmMatchBits-1:0]    sm_match_debug_signals_o,
  output logic [DcacheDbgSmFilterBits-1:0]   sm_filter_debug_signals_o,
  output logic [DcacheDbgSmDataBits-1:0]     sm_data_debug_signals_o
);

  localparam int unsigned NumNeighBits = 2;
  localparam int unsigned MinPerNBits = AdEsrShireHartIdSize - 1 - NumNeighBits;
  localparam logic [minion_dcache_pkg::PaSize-33:0] AdEsrRegion = 'b1;
  /* verilator lint_off VARHIDDEN */  // The preserved local ESR decode constants intentionally shadow shared package names used elsewhere in minion.
  localparam logic [1:0] AdEsrProtBitsMsg = 2'b10;
  /* verilator lint_on VARHIDDEN */
  localparam logic [1:0] AdEsrAgentTypeMinion = 2'b00;
  localparam logic [11:0] AdEsrPortMsgCommon = 12'h800;
  localparam int unsigned AdEsrMessageIdStart = 3;
  localparam logic [EtLinkSubopcodeSize-1:0] EtLinkWriteAround = '0;
  localparam logic [EtLinkSubopcodeSize-1:0] EtLinkWriteAround2Way =
      {{(EtLinkSubopcodeSize-1){1'b0}}, 1'b1};
  localparam logic [EtLinkSubopcodeSize-1:0] EtLinkWriteAround4Way =
      {{(EtLinkSubopcodeSize-2){1'b0}}, 2'b10};

  typedef enum logic [3:0] {
    ReduceInvalid,
    ReduceWaitTensor,
    ReduceNewReq,
    ReduceSendReady,
    ReduceWaitReady,
    ReduceSendData,
    ReduceWaitData,
    ReduceDrain,
    ReduceStoreData
  } reduce_op_state_e;

  typedef enum logic [2:0] {
    AtransIdle,
    AtransLoop,
    AtransS0Req,
    AtransS1Req,
    AtransWait,
    AtransFlush,
    AtransFail,
    AtransSave
  } atrans_state_e;

  typedef struct packed {
    logic [4:0]                  padding1;
    logic [DcacheReduceLevelBits-1:0] endl;
    logic [DcacheReduceLevelBits-1:0] startl;
  } reduce_level_t;

  typedef struct packed {
    logic                        ov;
    logic [DcacheTsRowsBits-1:0] rows;
    logic [DcacheTsColsBits-1:0] cols;
  } row_col_count_t;

  typedef union packed {
    logic [MaxMinionIdBits-1:0]  partner_id;
    reduce_level_t               level;
  } reduce_pl_info_t;

  typedef union packed {
    logic [DcacheTsColsBits-1:0] action;
    logic [DcacheTsColsBits-1:0] cols;
  } reduce_ac_info_t;

  typedef union packed {
    logic [DcacheReCountBits-1:0] cnt;
    row_col_count_t               rc;
  } reg_count_t;

  typedef struct packed {
    reduce_pl_info_t              pl_info;
    reduce_ac_info_t              ac_info;
    logic                         reduce;
    logic [1:0]                   coop;
    logic                         thread_id;
  } reduce_control_info_t;

  reduce_control_info_t                       req_info_q, req_info_d;
  /* verilator lint_off UNOPTFLAT */  // The reduce completion logic feeds only registered/FIFO state; Verilator incorrectly folds it back into FSM next-state.
  reduce_op_state_e                           fsm_state_q, fsm_state_d;
  /* verilator lint_on UNOPTFLAT */
  logic [minion_dcache_pkg::VaSize-1:4]                          addr_q, addr_d;
  logic                                       addr_msb_err_q, addr_msb_err_d;
  logic [minion_dcache_pkg::VaSize-1:4]                          stride_addr_q, stride_addr_d;
  logic [1:0]                                 stride_entry_q, stride_entry_d;
  reg_count_t                                 reg_count_q, reg_count_d;
  logic [DcacheTsRateBits-1:0]                rate_q, rate_d;
  logic                                       new_req;
  logic                                       reg_count_up;
  /* verilator lint_off SYNCASYNCNET */  // clk_en intentionally tracks the ungated enable while also controlling state on the clkgt domain.
  logic                                       clk_en;
  /* verilator lint_on SYNCASYNCNET */
  logic                                       addr_up;
  logic                                       stride_up;
  logic [minion_dcache_pkg::DcacheTensorCtrlDestBits-1:0]        scp_entry_q, scp_entry_d;
  logic                                       scp_entry_up;
  logic                                       scp_idx_q, scp_idx_d;
  logic [DcacheSetIdxWidth-1:0]               ts_scp_read_set;
  logic                                       ts_is_from_scp_q, ts_is_from_scp_d;
  logic                                       vpu_send_reg_reduce;
  logic                                       vpu_send_reg_tensor;
  logic                                       vpu_send_reg;
  logic                                       scp_read_reg;
  logic                                       pipe_used_pre;
  logic                                       tensorstore_count_q, tensorstore_count_d;
  logic [DcacheTsRowsBits:0]                  atrans_num_req_q, atrans_num_req_d;
  atrans_state_e                              atrans_state_q, atrans_state_d;
  logic                                       address_available;
  logic [DcacheTsTransAddrCntBits-1:0]        address_count_q, address_count_d;
  logic                                       address_save;
  logic                                       address_use;
  logic                                       address_consume;
  logic                                       address_use_idx_q, address_use_idx_d;
  /* verilator lint_off UNUSEDSIGNAL */  // Low alignment bits are intentionally unused by the L2 request formatting logic.
  logic [minion_dcache_pkg::PaSize-1:0]                          addr_phys;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                                       address_for_l2_read;
  logic                                       address_for_l2_ready;
  logic                                       address_fifo_full;
  logic                                       err_mem_q, err_mem_d;
  logic                                       clkgt;
  logic                                       clkgt_en;
  logic [3:0]                                 operation_q;
  logic                                       ba_available_q, ba_available_d;
  logic                                       ba_available_up;
  logic                                       wait_data_from_sender;
  et_link_minion_evict_req_info_t             l2_req;
  logic [DcacheTsRateCounterBits-1:0]         rate_counter_q, rate_counter_d;
  logic                                       rate_limit_allow_req;
  logic                                       l2_backoff_q, l2_backoff_d;
  logic [DcacheTsBackoffCounterBits-1:0]      l2_backoff_count_q, l2_backoff_count_d;
  logic [DcacheBufferIdxWidth-1:0]            ba_data_send_fifo_data_q;
  logic [DcacheBufferIdxWidth-1:0]            ba_op_fifo_data;
  logic                                       got_ready_msg_q, got_ready_msg_d;
  logic                                       s4_send_pending_q;
  logic                                       s4_send_pending_pre;
  logic                                       write_around_throttle_q, write_around_throttle_d;
  logic                                       s4_send;
  logic                                       any_packet;
  /* verilator lint_off UNOPTFLAT */  // reduce_wait_done only depends on FIFO valid/pipeline registers; Verilator falsely back-propagates through the FSM next-state block.
  logic                                       reduce_wait_done;
  /* verilator lint_on UNOPTFLAT */
  logic                                       ba_write_needed_rec;
  logic                                       ba_op_fifo_valid;
  logic                                       ba_alloc_fifo_val;
  logic                                       l2_req_inflight_q, l2_req_inflight_d;
  logic                                       s0_inst_send_val_q;
  logic [MaxMinionIdBits-1:0]                 partner_ready_mask_q, partner_ready_mask_d;
  logic [DcacheReduceMsgDataBits-1:0]         partner_ready_idx_q, partner_ready_idx_d;
  logic                                       partner_ready_peer_q, partner_ready_peer_d;
  logic                                       new_partner_ready;
  logic                                       new_partner_ready_z1_q, new_partner_ready_z1_d;
  logic                                       current_partner_ready;
  logic                                       re_l2_req_valid_spec;
  logic [minion_dcache_pkg::PaSize-1:0]                          reduce_base_address;
  logic [minion_dcache_pkg::PaSize-1:0]                          reduce_send_ready_address;
  logic [minion_dcache_pkg::PaSize-1:0]                          reduce_send_data_address;
  logic [NumShireIdsBits-1:0]                 reduce_shire_id;
  logic [AdEsrShireHartIdSize-1:0]            reduce_hart_id;
  logic                                       ba_alloc_fifo_pop;
  logic                                       ba_alloc_fifo_full;
  logic                                       ba_op_fifo_full;
  logic [DcacheBufferIdxWidth-1:0]            ba_alloc_fifo_rdata;
  logic                                       ba_data_send_fifo_valid;
  logic                                       wait_l2_to_ba_resp_z1_q;
  logic                                       wait_l2_to_ba_resp_en;
  logic [DcacheBufferIdxWidth-1:0]            s3_ba_written_entry_q;
  logic [3:0]                                 write_around_cnt_q, write_around_cnt_d;
  logic                                       write_around_cnt_up;
  logic                                       write_around_cnt_dec;
  logic                                       opcode_is_mine;
  logic                                       resp_is_error;
  logic [MaxMinionIdBits-1:0]                 partner_minion;
  logic [MaxMinionIdBits-1:0]                 hartmask;
  logic [MaxMinionIdBits-1:0]                 distance;
  logic [DcacheReduceLevelBits-1:0]           level;
  logic [DcacheReduceActionBits-1:0]          action;
  logic                                       invert;
  logic                                       err_partner_id;
  logic                                       err_operation;
  logic                                       pairing_auto;
  logic [DcacheReduceMsgDataBits-1:0]         l2_req_msg_data;
  logic                                       l2_req_valid_block;
  logic                                       atrans_en;
  logic                                       last_packet_row;
  logic                                       load_send_fifo_data;
  logic [DcacheBufferIdxWidth-1:0]            ba_data_send_fifo_data_pre;
  logic [DcacheDbgSmDataGroupBits-1:0]        sm_data_group0;
  logic [DcacheDbgSmDataGroupBits-1:0]        sm_data_group1;
  logic [DcacheDbgSmDataGroupBits-1:0]        sm_data_group2;
  logic [DcacheDbgSmDataGroupBits-1:0]        sm_data_group3;
  minion_dcache_pkg::vm_status_t vm_status_ts_q, vm_status_ts_d;
  dft_t                                       dft_zero;

  assign dft_zero = '0;
  /* verilator lint_off UNUSEDSIGNAL */  // Some translated helper/state signals are preserved verbatim even when only control/debug logic consumes them.
  assign ts_scp_read_o = ts_is_from_scp_q;
  assign ts_col_size_is_128b_o = !req_info_q.reduce
      && (req_info_q.ac_info.cols == {{(DcacheTsColsBits-1){1'b0}}, 1'b1});
  assign ts_start_o = reduce_ready_o && tensorstore_start_i;
  assign ts_op_o = (!req_info_q.reduce && (ts_l2_req_valid_o && ts_l2_req_ready_i))
      || (re_l2_req_valid_o && re_l2_req_ready_i);
  assign addr_up = (new_req && tensorstore_start_i) || address_save;
  assign stride_up = new_req && tensorstore_start_i;
  assign ts_scp_read_set = get_set_from_scp_dest(scp_entry_q);
  assign ts_scp_read_way_o = get_way_from_scp_dest(scp_entry_q);
  assign ts_scp_read_addr_o = {ts_scp_read_set, scp_idx_q, {CoreL2BlockExtAddrOffsetBits{1'b0}}};
  assign address_save = (atrans_state_q == AtransSave);
  assign address_available = (address_count_q != '0);
  assign address_use = (scp_read_reg || vpu_send_reg_tensor) && address_available;
  assign address_consume = address_use && address_use_idx_q;
  assign atrans_en = (fsm_state_q == ReduceNewReq) || (atrans_state_q != AtransIdle);
  assign reduce_ready_o = (fsm_state_q == ReduceInvalid);
  assign new_req = (reduce_start_i || tensorstore_start_i) && reduce_ready_o;
  assign reg_count_up = new_req || ba_alloc_o;
  assign clk_en = !reduce_ready_o || new_req;
  assign clkgt_en = clk_en;

  prim_clk_gate u_clk_gate (
    .clk_i (clk_i),
    .en_i  (clkgt_en | chicken_bit_dcache_i),
    .dft_i (dft_zero),
    .clk_o (clkgt)
  );

  always_comb begin
    vm_status_ts_d = thread_id_i ? vm_status_i[1] : vm_status_i[0];

    req_info_d = req_info_q;
    rate_d = rate_q;
    reg_count_d = reg_count_q;
    addr_d = addr_q + stride_addr_q;
    addr_msb_err_d = addr_q[minion_dcache_pkg::VaSize-5] && !addr_d[minion_dcache_pkg::VaSize-5];
    stride_addr_d = stride_addr_q;
    stride_entry_d = stride_entry_q;
    scp_entry_d = scp_entry_q;
    scp_idx_d = scp_idx_q;
    scp_entry_up = 1'b0;

    if (reduce_start_i) begin
      req_info_d.ac_info.action = {1'b0, reduce_tensorstore_ctrl_i.reduce.action};
      req_info_d.reduce = 1'b1;
      req_info_d.coop = 2'b00;
      rate_d = '0;
    end else begin
      req_info_d.reduce = 1'b0;
    end
    ts_is_from_scp_d = reduce_tensorstore_ctrl_i.tensorstore_scp.scp && !req_info_d.reduce;

    if (!reduce_start_i) begin
      if (ts_is_from_scp_d) begin
        req_info_d.ac_info.cols = {{(DcacheTsColsBits-3){1'b0}}, 3'b100};
        req_info_d.coop = 2'b00;
        rate_d = reduce_tensorstore_ctrl_i.tensorstore_scp.rate;
      end else begin
        req_info_d.ac_info.cols = reduce_tensorstore_ctrl_i.tensorstore.cols + {{(DcacheTsColsBits-1){1'b0}}, 1'b1};
        req_info_d.coop = reduce_tensorstore_ctrl_i.tensorstore.coop;
        rate_d = reduce_tensorstore_ctrl_i.tensorstore.rate;
      end
    end
    req_info_d.thread_id = thread_id_i;
    req_info_d.pl_info.partner_id = reduce_tensorstore_ctrl_i.reduce.partner;

    if (req_info_q.reduce) begin
      reg_count_d.cnt = reg_count_q.cnt - {{(DcacheReCountBits-1){1'b0}}, 1'b1};
    end else begin
      reg_count_d = reg_count_q;
      if (reg_count_q.rc.cols < {{(DcacheTsColsBits-2){1'b0}}, 2'b10}) begin
        reg_count_d.rc.cols = '0;
      end else begin
        reg_count_d.rc.cols = reg_count_q.rc.cols - {{(DcacheTsColsBits-2){1'b0}}, 2'b10};
      end
      if (reg_count_d.rc.cols == '0) begin
        reg_count_d.rc.cols = req_info_q.ac_info.cols;
        reg_count_d.rc.rows = reg_count_q.rc.rows - {{(DcacheTsRowsBits-1){1'b0}}, 1'b1};
        if (reg_count_q.rc.rows == '0) begin
          reg_count_d.rc.cols = '0;
          reg_count_d.rc.rows = '0;
        end
      end
    end

    if (new_req) begin
      if (reduce_start_i) begin
        reg_count_d.cnt = {{(DcacheReCountBits-7){1'b0}}, reduce_tensorstore_ctrl_i.reduce.num_regs};
      end else begin
        reg_count_d.rc.ov = 1'b0;
        if (ts_is_from_scp_d) begin
          reg_count_d.rc.cols = {{(DcacheTsColsBits-3){1'b0}}, 3'b100};
          reg_count_d.rc.rows = reduce_tensorstore_ctrl_i.tensorstore_scp.rows;
        end else begin
          reg_count_d.rc.cols = reduce_tensorstore_ctrl_i.tensorstore.cols + {{(DcacheTsColsBits-1){1'b0}}, 1'b1};
          reg_count_d.rc.rows = reduce_tensorstore_ctrl_i.tensorstore.rows;
        end
      end
    end

    if (new_req && tensorstore_start_i) begin
      addr_msb_err_d = 1'b0;
      if (ts_is_from_scp_d) begin
        addr_d = {reduce_tensorstore_ctrl_i.tensorstore_scp.addr, 2'b0};
      end else begin
        addr_d = reduce_tensorstore_ctrl_i.tensorstore.addr;
      end
    end

    if (ts_is_from_scp_d) begin
      stride_addr_d = {tensorstore_stride_i[minion_dcache_pkg::VaSize-1:6], 2'b0};
    end else begin
      stride_addr_d = tensorstore_stride_i[minion_dcache_pkg::VaSize-1:4];
    end
    stride_entry_d = reduce_tensorstore_ctrl_i.tensorstore_scp.stride_entry;

    if (new_req && tensorstore_start_i && ts_is_from_scp_d) begin
      scp_entry_up = 1'b1;
      scp_idx_d = 1'b0;
      scp_entry_d = reduce_tensorstore_ctrl_i.tensorstore_scp.start_entry;
    end else if (ts_is_from_scp_q && !req_info_q.reduce && s1_inst_send_val_o) begin
      scp_entry_up = 1'b1;
      scp_idx_d = ~scp_idx_q;
      if (scp_idx_q) begin
        scp_entry_d = scp_entry_q
            + {{(minion_dcache_pkg::DcacheTensorCtrlDestBits-2){1'b0}}, stride_entry_q}
            + {{(minion_dcache_pkg::DcacheTensorCtrlDestBits-1){1'b0}}, 1'b1};
      end
      if (scp_ctrl_on_i && (scp_entry_d >= 6'd48)) begin
        scp_entry_d = scp_entry_d - 6'd48;
      end
    end

    s0_addr_o = {addr_q, 4'b0};
    s0_addr_msb_err_o = addr_msb_err_q;
  end

  always_comb begin
    atrans_num_req_d = atrans_num_req_q;
    if ((fsm_state_q == ReduceNewReq) && !req_info_q.reduce) begin
      atrans_num_req_d = reg_count_q.rc.rows + {{DcacheTsRowsBits{1'b0}}, 1'b1};
    end else if (atrans_state_q == AtransSave) begin
      atrans_num_req_d = atrans_num_req_q - {{DcacheTsRowsBits{1'b0}}, 1'b1};
    end

    address_count_d = address_count_q;
    if ((fsm_state_q == ReduceNewReq) && !req_info_q.reduce) begin
      address_count_d = '0;
    end else if (address_save && !address_consume) begin
      address_count_d = address_count_q + {{(DcacheTsTransAddrCntBits-1){1'b0}}, 1'b1};
    end else if (address_consume && !address_save) begin
      address_count_d = address_count_q - {{(DcacheTsTransAddrCntBits-1){1'b0}}, 1'b1};
    end

    address_use_idx_d = address_use_idx_q;
    if (req_info_q.ac_info.cols <= {{(DcacheTsColsBits-1){1'b0}}, 1'b1}) begin
      address_use_idx_d = 1'b1;
    end else if (!address_available) begin
      address_use_idx_d = 1'b0;
    end else if (address_use) begin
      address_use_idx_d = ~address_use_idx_q;
    end

    atrans_state_d = atrans_state_q;
    err_mem_d = 1'b0;
    unique case (atrans_state_q)
      AtransIdle: begin
        if ((fsm_state_q == ReduceNewReq) && !req_info_q.reduce) begin
          atrans_state_d = AtransLoop;
        end
      end
      AtransLoop: begin
        if (atrans_num_req_q != '0) begin
          atrans_state_d = AtransS0Req;
        end else begin
          atrans_state_d = AtransIdle;
        end
      end
      AtransS0Req: begin
        if (s0_available_i && !address_fifo_full) begin
          atrans_state_d = AtransS1Req;
        end
      end
      AtransS1Req: begin
        if (s1_available_i) begin
          if (s1_tlb_fail_i) begin
            atrans_state_d = AtransFlush;
          end else if (s1_tlb_wait_i) begin
            atrans_state_d = AtransWait;
          end else begin
            atrans_state_d = AtransSave;
          end
        end else begin
          atrans_state_d = AtransS0Req;
        end
      end
      AtransWait: begin
        if (!s1_tlb_wait_i) begin
          atrans_state_d = AtransS0Req;
        end
      end
      AtransFlush: begin
        if (!address_available) begin
          atrans_state_d = AtransFail;
          err_mem_d = 1'b1;
        end
      end
      AtransFail: begin
        atrans_state_d = AtransIdle;
      end
      AtransSave: begin
        if (atrans_num_req_d != '0) begin
          atrans_state_d = AtransS0Req;
        end else begin
          atrans_state_d = AtransLoop;
        end
      end
      default: begin
        atrans_state_d = AtransIdle;
      end
    endcase
  end

  always_comb begin
    action = req_info_q.ac_info.action[DcacheReduceActionBits-1:0];
    partner_minion = req_info_q.pl_info.partner_id;
    level = req_info_q.pl_info.level.startl;
    distance = {{(MaxMinionIdBits-1){1'b0}}, 1'b1} << level;
    hartmask = ({{(MaxMinionIdBits-1){1'b0}}, 1'b1} << (level + {{(DcacheReduceLevelBits-1){1'b0}}, 1'b1})) - {{(MaxMinionIdBits-1){1'b0}}, 1'b1};
    invert = ~action[0];
    pairing_auto = (req_info_q.ac_info.action[DcacheReduceActionBits-1:0] != DcacheReduceSend)
        && (req_info_q.ac_info.action[DcacheReduceActionBits-1:0] != DcacheReduceRecv);
    err_partner_id = 1'b0;

    if (pairing_auto) begin
      if ((global_min_id_i & hartmask) == distance) begin
        action = {1'b0, invert};
        partner_minion = global_min_id_i - distance;
      end else if ((global_min_id_i & hartmask) == '0) begin
        action = {1'b0, ~invert};
        partner_minion = global_min_id_i + distance;
      end else begin
        action = DcacheReduceDoNothing;
      end
    end else if ((fsm_state_q == ReduceNewReq) && (partner_minion == global_min_id_i) && req_info_q.reduce) begin
      err_partner_id = 1'b1;
    end

    err_operation = (fsm_state_q == ReduceNewReq) && req_info_q.reduce && (action == DcacheReduceRecv)
        && (operation_q != 4'b0000) && (operation_q != 4'b0010) && (operation_q != 4'b0011)
        && (operation_q != 4'b0100) && (operation_q != 4'b0110) && (operation_q != 4'b0111)
        && (operation_q != 4'b1000);
  end

  always_comb begin
    fsm_state_d = fsm_state_q;
    if ((fsm_state_q == ReduceInvalid) && new_req) begin
      fsm_state_d = ReduceWaitTensor;
    end
    if ((fsm_state_q == ReduceWaitTensor) && !vpu_reduce_wait_i) begin
      fsm_state_d = ReduceNewReq;
    end else if ((fsm_state_q == ReduceNewReq) && !req_info_q.reduce) begin
      fsm_state_d = ReduceStoreData;
    end else if ((fsm_state_q == ReduceNewReq) && err_partner_id) begin
      fsm_state_d = ReduceInvalid;
    end else if ((fsm_state_q == ReduceNewReq) && err_operation) begin
      fsm_state_d = ReduceInvalid;
    end else if ((fsm_state_q == ReduceNewReq) && (reg_count_q.cnt == '0)) begin
      fsm_state_d = ReduceInvalid;
    end else if ((fsm_state_q == ReduceNewReq) && (action == DcacheReduceSend)) begin
      fsm_state_d = ReduceWaitReady;
    end else if ((fsm_state_q == ReduceNewReq) && (action == DcacheReduceRecv)) begin
      fsm_state_d = ReduceSendReady;
    end else if ((fsm_state_q == ReduceNewReq) && (action == DcacheReduceDoNothing)) begin
      fsm_state_d = ReduceInvalid;
    end else if ((fsm_state_q == ReduceSendReady) && ba_available_q && re_l2_req_ready_i) begin
      fsm_state_d = ReduceWaitData;
    end else if ((fsm_state_q == ReduceWaitReady) && got_ready_msg_q) begin
      fsm_state_d = ReduceSendData;
    end else if ((fsm_state_q == ReduceStoreData) && (atrans_state_q == AtransFail)) begin
      fsm_state_d = ReduceDrain;
    end else if ((fsm_state_q == ReduceStoreData) && (reg_count_q.cnt == '0) && req_info_q.reduce) begin
      fsm_state_d = ReduceDrain;
    end else if ((fsm_state_q == ReduceSendData) && (reg_count_q.cnt == '0)) begin
      fsm_state_d = ReduceDrain;
    end else if ((fsm_state_q == ReduceDrain) && !any_packet) begin
      fsm_state_d = ReduceInvalid;
    end else if ((fsm_state_q == ReduceWaitData) && reduce_wait_done) begin
      fsm_state_d = ReduceInvalid;
    end
  end

  always_comb begin
    l2_req_inflight_d = l2_req_inflight_q;
    if ((re_l2_req_valid_o && re_l2_req_ready_i) || (ts_l2_req_valid_o && ts_l2_req_ready_i)) begin
      if (!l2_req_inflight_q && (l2_req.size > EtLinkHLine)) begin
        l2_req_inflight_d = 1'b1;
      end else begin
        l2_req_inflight_d = 1'b0;
      end
    end
  end

  assign wait_data_from_sender = (fsm_state_q == ReduceWaitData) || (fsm_state_q == ReduceSendReady);
  assign ba_entries_required_o = ((fsm_state_q == ReduceWaitData) || (fsm_state_q == ReduceSendReady))
      || ((fsm_state_q == ReduceSendData) || (fsm_state_q == ReduceStoreData));

  assign reduce_shire_id = {{(NumShireIdsBits-(MaxMinionIdBits-MinPerNBits-NumNeighBits)){1'b0}},
      partner_minion[MaxMinionIdBits-1:MinPerNBits+NumNeighBits]};
  assign reduce_hart_id = {{(AdEsrShireHartIdSize-MinPerNBits-NumNeighBits-1){1'b0}},
      partner_minion[MinPerNBits+NumNeighBits-1:0], req_info_q.thread_id};
  assign reduce_base_address = {AdEsrRegion, AdEsrProtBitsMsg, reduce_shire_id, AdEsrAgentTypeMinion,
      reduce_hart_id, AdEsrPortMsgCommon};
  assign reduce_send_ready_address = reduce_base_address
      | (minion_dcache_pkg::PaSize'(EtLinkMinMsgIdReduceReady) << AdEsrMessageIdStart);
  assign reduce_send_data_address = reduce_base_address
      | (minion_dcache_pkg::PaSize'(EtLinkMinMsgIdReduceData) << AdEsrMessageIdStart);
  assign partner_ready_idx_d = l2_resp_i.data[DcacheReduceMsgDataBits-1:0];
  assign new_partner_ready = l2_resp_valid_i && (l2_resp_i.opcode == EtLinkRspMsgRcvData)
      && (et_link_min_msg_id_e'(l2_resp_i.id) == EtLinkMinMsgIdReduceReady);

  always_comb begin
    partner_ready_mask_d = partner_ready_mask_q;
    partner_ready_peer_d = partner_ready_peer_q;

    if (new_partner_ready_z1_q) begin
      if (!partner_ready_idx_q[DcacheReduceMsgDataMsb]) begin
        partner_ready_mask_d[partner_ready_idx_q[DcacheReduceLevelBits-1:0]] = 1'b1;
      end else begin
        partner_ready_peer_d = 1'b1;
      end
    end

    if (got_ready_msg_q) begin
      if (pairing_auto) begin
        partner_ready_mask_d[level] = 1'b0;
      end else begin
        partner_ready_peer_d = 1'b0;
      end
    end
  end

  assign current_partner_ready = pairing_auto ? partner_ready_mask_q[level] : partner_ready_peer_q;

  always_comb begin
    ba_available_d = ba_available_q;
    if (fsm_state_q == ReduceNewReq) begin
      ba_available_d = 1'b0;
    end else if (fsm_state_q == ReduceSendReady) begin
      ba_available_d = !ba_full_i || ba_alloc_fifo_val;
    end
    ba_available_up = (fsm_state_q == ReduceNewReq) || (fsm_state_q == ReduceSendReady);
  end

  assign l2_req_valid_block = write_around_throttle_q || !rate_limit_allow_req || l2_backoff_q;

  always_comb begin
    vpu_send_reg_reduce = (fsm_state_q == ReduceSendData) && !ba_full_i && (reg_count_q != '0);
    vpu_send_reg_tensor = (fsm_state_q == ReduceStoreData) && !ts_is_from_scp_q
        && address_available && !ba_full_i && (reg_count_q != '0);
    vpu_send_reg = vpu_send_reg_reduce || vpu_send_reg_tensor;
    scp_read_reg = (fsm_state_q == ReduceStoreData) && ts_is_from_scp_q
        && address_available && !ba_full_i && (reg_count_q != '0);

    vpu_ctrl_o.send_reg = vpu_send_reg;
    vpu_ctrl_o.exec_op = 1'b0;
    vpu_ctrl_o.nothing = ((fsm_state_q == ReduceNewReq) && req_info_q.reduce
        && ((action == DcacheReduceDoNothing) || err_partner_id || err_operation))
        || ((fsm_state_q == ReduceStoreData) && (atrans_state_q == AtransFail));

    ba_alloc_o = vpu_send_reg || scp_read_reg
        || (wait_data_from_sender && !ba_full_i && !s2_valid_i && (reg_count_q != '0));
    ba_write_needed_rec = ba_alloc_o && wait_data_from_sender;

    re_l2_req_valid_o = 1'b0;
    ts_l2_req_valid_o = 1'b0;
    l2_req = '0;
    ba_read_en_o = 1'b0;
    ba_read_addr_o = ba_data_send_fifo_data_q;
    s1_inst_op_val_o = 1'b0;

    if (pairing_auto) begin
      l2_req_msg_data = {{(DcacheReduceMsgDataBits-DcacheReduceLevelBits){1'b0}}, level};
    end else begin
      l2_req_msg_data = {{DcacheReduceMsgDataBits-1{1'b0}}, 1'b1} << DcacheReduceMsgDataMsb;
    end

    if (fsm_state_q == ReduceSendReady) begin
      re_l2_req_valid_o = ba_available_q;
      l2_req.wdata = 1'b1;
      l2_req.opcode = EtLinkReqMsgSendData;
      l2_req.subopcode = '0;
      l2_req.address = reduce_send_ready_address;
      l2_req.data[DcacheReduceMsgDataBits-1:0] = l2_req_msg_data;
      l2_req.size = EtLinkByte;
      l2_req.qwen = 4'b0001;
    end

    re_l2_req_valid_spec = req_info_q.reduce
        || ((req_info_q.coop == 2'b00)
            && (s4_send_pending_pre || l2_req_inflight_q
                || (req_info_q.ac_info.cols <= {{(DcacheTsColsBits-2){1'b0}}, 2'b10})));

    if (s4_send_pending_q && !l2_req_valid_block && !ba_read_used_i
        && (req_info_q.reduce || address_for_l2_ready)
        && (!mh_going_to_uc_store_i || l2_req_inflight_q)) begin
      ba_read_en_o = 1'b1;
      re_l2_req_valid_o = re_l2_req_valid_spec;
      ts_l2_req_valid_o = !req_info_q.reduce && (req_info_q.coop != 2'b00);
      l2_req.id = re_l2_req_valid_o ? EtLinkCoreMissIdReduce : EtLinkCoreMissIdTensorStoreCoop;
      l2_req.wdata = 1'b1;
      l2_req.opcode = req_info_q.reduce ? EtLinkReqMsgSendData : et_link_req_opcode_e'(5'd2);
      l2_req.subopcode = req_info_q.reduce ? '0
          : (req_info_q.coop == 2'b00) ? EtLinkWriteAround
          : (req_info_q.coop == 2'b01) ? EtLinkWriteAround2Way
          : EtLinkWriteAround4Way;
      l2_req.address = req_info_q.reduce ? reduce_send_data_address
          : (req_info_q.ac_info.cols == {{(DcacheTsColsBits-1){1'b0}}, 1'b1}) ? {addr_phys[minion_dcache_pkg::PaSize-1:4], 4'b0}
          : (req_info_q.ac_info.cols == {{(DcacheTsColsBits-2){1'b0}}, 2'b10}) ? {addr_phys[minion_dcache_pkg::PaSize-1:5], 5'b0}
          : {addr_phys[minion_dcache_pkg::PaSize-1:6], 6'b0};
      l2_req.data = s1_ba_data_i;
      l2_req.size = req_info_q.reduce ? EtLinkHLine
          : (req_info_q.ac_info.cols == {{(DcacheTsColsBits-1){1'b0}}, 1'b1}) ? EtLinkQWord
          : (req_info_q.ac_info.cols == {{(DcacheTsColsBits-2){1'b0}}, 2'b10}) ? EtLinkHLine
          : EtLinkLine;
      l2_req.qwen = req_info_q.reduce ? 4'b0011
          : (req_info_q.ac_info.cols == {{(DcacheTsColsBits-1){1'b0}}, 1'b1}) ? (4'b0001 << addr_phys[5:4])
          : (req_info_q.ac_info.cols == {{(DcacheTsColsBits-2){1'b0}}, 2'b10}) ? (4'b0011 << {addr_phys[5], 1'b0})
          : 4'b1111;
    end

    if (ba_op_fifo_valid && !s1_valid_i && !ba_read_used_i) begin
      ba_read_en_o = 1'b1;
      ba_read_addr_o = ba_op_fifo_data;
      s1_inst_op_val_o = 1'b1;
      vpu_ctrl_o.exec_op = 1'b1;
    end

    s4_send = s4_send_pending_q && ((re_l2_req_valid_o && re_l2_req_ready_i)
        || (ts_l2_req_valid_o && ts_l2_req_ready_i));

    last_packet_row = 1'b0;
    if ((req_info_q.ac_info.cols == {{(DcacheTsColsBits-1){1'b0}}, 1'b1})
        || (req_info_q.ac_info.cols == {{(DcacheTsColsBits-2){1'b0}}, 2'b10})
        || ((req_info_q.ac_info.cols == {{(DcacheTsColsBits-3){1'b0}}, 3'b100})
            && tensorstore_count_q)) begin
      last_packet_row = !req_info_q.reduce && s4_send;
    end

    tensorstore_count_d = ~tensorstore_count_q;
    if (new_req || last_packet_row) begin
      tensorstore_count_d = 1'b0;
    end

    ba_read_en_spec_o = 1'b0;
    if (s4_send_pending_q
        && (re_l2_req_valid_spec || (!req_info_q.reduce && (req_info_q.coop != 2'b00)))
        && !l2_req_valid_block) begin
      ba_read_en_spec_o = (fsm_state_q != ReduceSendReady);
    end
    if (ba_op_fifo_valid && !s1_valid_i) begin
      ba_read_en_spec_o = (fsm_state_q != ReduceSendReady);
    end

    got_ready_msg_d = got_ready_msg_q;
    if (current_partner_ready && (fsm_state_q == ReduceWaitReady)) begin
      got_ready_msg_d = 1'b1;
    end
    if ((fsm_state_q == ReduceWaitReady) && (fsm_state_d != ReduceWaitReady)) begin
      got_ready_msg_d = 1'b0;
    end

    any_packet = s0_inst_send_val_q || s1_inst_send_val_o || s2_inst_send_val_o || s3_inst_send_val_o
        || s4_send_pending_pre || s4_send_pending_q;
    reduce_wait_done = (reg_count_q == '0) && !ba_alloc_fifo_val && !ba_op_fifo_valid
        && !s1_inst_op_val_o && !s2_inst_op_val_o && !s3_inst_op_val_o;
  end

  assign ba_write_needed_o = s2_inst_send_val_o || ba_write_needed_rec;
  assign re_l2_req_o = l2_req;
  assign ts_l2_req_o = l2_req;
  assign pipe_used_pre = vpu_send_reg || scp_read_reg;
  assign address_for_l2_read = last_packet_row;
  assign load_send_fifo_data = s4_send || (s4_send_pending_pre && !s4_send_pending_q);

  always_comb begin
    rate_counter_d = rate_counter_q;
    if (new_req) begin
      rate_counter_d = '0;
    end else if (last_packet_row) begin
      rate_counter_d = {rate_q, 2'b0};
    end else if (rate_counter_q != '0) begin
      rate_counter_d = rate_counter_q - {{(DcacheTsRateCounterBits-1){1'b0}}, 1'b1};
    end
  end

  assign rate_limit_allow_req = (rate_counter_q == '0);

  always_comb begin
    l2_backoff_d = l2_backoff_q;
    l2_backoff_count_d = l2_backoff_count_q;

    if (new_req) begin
      l2_backoff_count_d = '0;
    end else if (ba_read_en_o) begin
      l2_backoff_count_d = l2_backoff_count_q + {{(DcacheTsBackoffCounterBits-1){1'b0}}, 1'b1};
    end else if (l2_backoff_q) begin
      l2_backoff_count_d = l2_backoff_count_q - {{(DcacheTsBackoffCounterBits-2){1'b0}}, 2'b10};
    end

    if (new_req) begin
      l2_backoff_d = 1'b0;
    end else if (ba_read_en_o && (l2_backoff_count_d == {{(DcacheTsBackoffCounterBits-4){1'b0}}, 4'h8})) begin
      l2_backoff_d = 1'b1;
    end else if (l2_backoff_count_d == '0) begin
      l2_backoff_d = 1'b0;
    end
  end

  assign write_around_throttle_d = (write_around_cnt_d == 4'b1000);
  assign opcode_is_mine = (et_link_core_miss_id_e'(l2_resp_i.id[4:0]) == EtLinkCoreMissIdReduce)
      || (et_link_core_miss_id_e'(l2_resp_i.id[4:0]) == EtLinkCoreMissIdTensorStoreCoop);
  assign write_around_cnt_dec = l2_resp_valid_i && opcode_is_mine
      && ((l2_resp_i.opcode == EtLinkRspAck) || (l2_resp_i.opcode == EtLinkRspErr));

  always_comb begin
    write_around_cnt_up = 1'b0;
    write_around_cnt_d = write_around_cnt_q;
    if (last_packet_row) begin
      write_around_cnt_up = 1'b1;
      write_around_cnt_d = write_around_cnt_q + 4'b0001;
    end
    if (write_around_cnt_dec) begin
      write_around_cnt_up = 1'b1;
      if (write_around_cnt_d != 4'b0000) begin
        write_around_cnt_d = write_around_cnt_d - 4'b0001;
      end
    end
  end

  assign ba_alloc_fifo_pop = l2_resp_valid_i && (l2_resp_i.opcode == EtLinkRspMsgRcvData)
      && (et_link_min_msg_id_e'(l2_resp_i.id) == EtLinkMinMsgIdReduceData);
  assign wait_l2_to_ba_resp_o = ba_alloc_fifo_val;
  assign wait_l2_to_ba_resp_en = wait_l2_to_ba_resp_z1_q || wait_l2_to_ba_resp_o;
  assign buffer_avail_o = ba_alloc_fifo_val && wait_l2_to_ba_resp_z1_q && !ba_op_fifo_full;
  assign s4_send_pending_pre = ba_data_send_fifo_valid;
  assign s3_ba_write_addr_o = ba_alloc_fifo_rdata;
  assign new_partner_ready_z1_d = new_partner_ready;

  /* verilator lint_off PINCONNECTEMPTY */  // FIFO status outputs are intentionally unused where the original logic does not consume them.
  prim_fifo_sync #(
    .Width (DcacheBufferIdxWidth),
    .Depth (DcacheBufferSize)
  ) u_ba_alloc_fifo (
    .clk_i    (clkgt),
    .rst_ni   (rst_ni),
    .clr_i    (1'b0),
    .wvalid_i (ba_write_needed_rec),
    .wready_o (),
    .wdata_i  (s2_ba_written_entry_i),
    .rvalid_o (ba_alloc_fifo_val),
    .rready_i (ba_alloc_fifo_pop),
    .rdata_o  (ba_alloc_fifo_rdata),
    .depth_o  (),
    .full_o   (ba_alloc_fifo_full),
    .empty_o  ()
  );

  prim_fifo_sync #(
    .Width (DcacheBufferIdxWidth),
    .Depth (4)
  ) u_ba_op_fifo (
    .clk_i    (clkgt),
    .rst_ni   (rst_ni),
    .clr_i    (1'b0),
    .wvalid_i (ba_alloc_fifo_pop),
    .wready_o (),
    .wdata_i  (ba_alloc_fifo_rdata),
    .rvalid_o (ba_op_fifo_valid),
    .rready_i (s1_inst_op_val_o),
    .rdata_o  (ba_op_fifo_data),
    .depth_o  (),
    .full_o   (ba_op_fifo_full),
    .empty_o  ()
  );

  prim_fifo_sync #(
    .Width (DcacheBufferIdxWidth),
    .Depth (DcacheBufferSize)
  ) u_ba_data_send_fifo (
    .clk_i    (clkgt),
    .rst_ni   (rst_ni),
    .clr_i    (1'b0),
    .wvalid_i (s3_inst_send_val_o),
    .wready_o (),
    .wdata_i  (s3_ba_written_entry_q),
    .rvalid_o (ba_data_send_fifo_valid),
    .rready_i (load_send_fifo_data && s4_send_pending_pre),
    .rdata_o  (ba_data_send_fifo_data_pre),
    .depth_o  (),
    .full_o   (),
    .empty_o  ()
  );

  prim_fifo_sync #(
    .Width (minion_dcache_pkg::PaSize),
    .Depth (DcacheTsPhysAddrNum)
  ) u_phys_addr_send_fifo (
    .clk_i    (clkgt),
    .rst_ni   (rst_ni),
    .clr_i    (1'b0),
    .wvalid_i (address_save),
    .wready_o (),
    .wdata_i  (s2_addr_i),
    .rvalid_o (address_for_l2_ready),
    .rready_i (address_for_l2_read),
    .rdata_o  (addr_phys),
    .depth_o  (),
    .full_o   (address_fifo_full),
    .empty_o  ()
  );
  /* verilator lint_on PINCONNECTEMPTY */

  // Original EN_FF/FF state intentionally survives reset.
  always_ff @(posedge clkgt) begin
    if (new_req) begin
      vm_status_ts_q <= vm_status_ts_d;
      req_info_q <= req_info_d;
      rate_q <= rate_d;
      ts_is_from_scp_q <= ts_is_from_scp_d;
      operation_q <= reduce_tensorstore_ctrl_i.reduce.op;
    end
    if (reg_count_up) begin
      reg_count_q <= reg_count_d;
    end
    if (addr_up) begin
      addr_q <= addr_d;
      addr_msb_err_q <= addr_msb_err_d;
    end
    if (stride_up) begin
      stride_addr_q <= stride_addr_d;
      stride_entry_q <= stride_entry_d;
    end
    if (scp_entry_up) begin
      scp_entry_q <= scp_entry_d;
      scp_idx_q <= scp_idx_d;
    end
    if (atrans_en) begin
      atrans_num_req_q <= atrans_num_req_d;
      address_count_q <= address_count_d;
      address_use_idx_q <= address_use_idx_d;
    end
    if (ba_available_up) begin
      ba_available_q <= ba_available_d;
    end
    if (new_req || s4_send) begin
      tensorstore_count_q <= tensorstore_count_d;
    end
    rate_counter_q <= rate_counter_d;
    l2_backoff_count_q <= l2_backoff_count_d;
    if (s2_inst_send_val_o) begin
      s3_ba_written_entry_q <= s2_ba_written_entry_i;
    end
    if (load_send_fifo_data) begin
      ba_data_send_fifo_data_q <= ba_data_send_fifo_data_pre;
    end
  end

  /* verilator lint_off SYNCASYNCNET */  // The translated reduce control keeps async reset and synchronous clk_en gating on the same clkgt-domain state.
  always_ff @(posedge clkgt or negedge rst_ni) begin
    if (!rst_ni) begin
      fsm_state_q <= ReduceInvalid;
      atrans_state_q <= AtransIdle;
      l2_req_inflight_q <= 1'b0;
      got_ready_msg_q <= 1'b0;
      l2_backoff_q <= 1'b0;
      wait_l2_to_ba_resp_z1_q <= 1'b0;
      s2_inst_op_val_o <= 1'b0;
      s3_inst_op_val_o <= 1'b0;
      pipe_used_o <= 1'b0;
      s0_inst_send_val_q <= 1'b0;
      s1_inst_send_val_o <= 1'b0;
      s2_inst_send_val_o <= 1'b0;
      s3_inst_send_val_o <= 1'b0;
      s4_send_pending_q <= 1'b0;
      write_around_cnt_q <= '0;
      write_around_throttle_q <= 1'b0;
    end else begin
      if (clk_en) begin
        fsm_state_q <= fsm_state_d;
        s2_inst_op_val_o <= s1_inst_op_val_o;
        s3_inst_op_val_o <= s2_inst_op_val_o;
        pipe_used_o <= pipe_used_pre;
        s0_inst_send_val_q <= pipe_used_o;
        s1_inst_send_val_o <= s0_inst_send_val_q;
        s2_inst_send_val_o <= s1_inst_send_val_o;
        s3_inst_send_val_o <= s2_inst_send_val_o;
      end
      if (atrans_en) begin
        atrans_state_q <= atrans_state_d;
      end
      l2_req_inflight_q <= l2_req_inflight_d;
      got_ready_msg_q <= got_ready_msg_d;
      l2_backoff_q <= l2_backoff_d;
      if (write_around_cnt_up) begin
        write_around_cnt_q <= write_around_cnt_d;
        write_around_throttle_q <= write_around_throttle_d;
      end
      if (wait_l2_to_ba_resp_en) begin
        wait_l2_to_ba_resp_z1_q <= wait_l2_to_ba_resp_o;
      end
      if (load_send_fifo_data) begin
        s4_send_pending_q <= s4_send_pending_pre;
      end
    end
  end
  /* verilator lint_on SYNCASYNCNET */

  // Partner-ready tracking is clocked on the ungated clock in the original RTL.
  always_ff @(posedge clk_i) begin
    if (new_partner_ready) begin
      partner_ready_idx_q <= partner_ready_idx_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      err_mem_q <= 1'b0;
      partner_ready_mask_q <= '0;
      partner_ready_peer_q <= 1'b0;
      new_partner_ready_z1_q <= 1'b0;
    end else begin
      err_mem_q <= err_mem_d;
      if (got_ready_msg_q || new_partner_ready_z1_q) begin
        partner_ready_mask_q <= partner_ready_mask_d;
        partner_ready_peer_q <= partner_ready_peer_d;
      end
      if (new_partner_ready || new_partner_ready_z1_q) begin
        new_partner_ready_z1_q <= new_partner_ready_z1_d;
      end
    end
  end

  assign s0_addr_load_o = (atrans_state_q == AtransS0Req) && s0_available_i && !address_fifo_full;
  assign s1_addr_load_o = (atrans_state_q == AtransS1Req);
  assign s0_thread_id_o = req_info_q.thread_id;
  assign vm_status_ts_o = vm_status_ts_q;
  assign err_flags_o[DcacheErrFlagCoLock] = '0;
  assign err_flags_o[DcacheErrFlagMemFault] = {
    (err_mem_q && req_info_q.thread_id), (err_mem_q && !req_info_q.thread_id)
  };
  assign re_ts_l2_req_inflight_o = l2_req_inflight_q;
  assign tensorstore_busy_o = (write_around_cnt_q != '0) || (!reduce_ready_o && !req_info_q.reduce);
  assign resp_is_error = l2_resp_valid_i && opcode_is_mine && (l2_resp_i.opcode == EtLinkRspErr);
  assign bus_err_o = {resp_is_error && req_info_q.thread_id, resp_is_error && !req_info_q.thread_id};
  assign csr_debug_signals_o = fsm_state_q[minion_dcache_pkg::DcacheDbgCsrTsSize-1:0];

  always_comb begin
    sm_match_debug_signals_o = '0;
    sm_match_debug_signals_o[$bits({
        vpu_reduce_wait_i, atrans_en, new_partner_ready, new_req, s4_send, load_send_fifo_data,
        write_around_cnt_up, reduce_ready_o, reduce_start_i, tensorstore_busy_o,
        tensorstore_start_i, re_l2_req_valid_o && re_l2_req_ready_i,
        ts_l2_req_valid_o && ts_l2_req_ready_i, l2_resp_valid_i, s3_inst_op_val_o, s2_inst_op_val_o,
        s1_inst_op_val_o, pipe_used_pre, pipe_used_o, s0_inst_send_val_q, s1_inst_send_val_o,
        s2_inst_send_val_o, s3_inst_send_val_o, ba_read_en_o, ts_scp_read_o
      })-1:0] = {
        vpu_reduce_wait_i, atrans_en, new_partner_ready, new_req, s4_send, load_send_fifo_data,
        write_around_cnt_up, reduce_ready_o, reduce_start_i, tensorstore_busy_o,
        tensorstore_start_i, re_l2_req_valid_o && re_l2_req_ready_i,
        ts_l2_req_valid_o && ts_l2_req_ready_i, l2_resp_valid_i, s3_inst_op_val_o, s2_inst_op_val_o,
        s1_inst_op_val_o, pipe_used_pre, pipe_used_o, s0_inst_send_val_q, s1_inst_send_val_o,
        s2_inst_send_val_o, s3_inst_send_val_o, ba_read_en_o, ts_scp_read_o
      };

    sm_filter_debug_signals_o = '0;
    sm_filter_debug_signals_o[$bits({
        vpu_reduce_wait_i, vpu_ctrl_o, atrans_en, new_partner_ready, new_req, s4_send,
        re_l2_req_valid_o, ts_l2_req_valid_o, re_l2_req_ready_i, ts_l2_req_ready_i, l2_resp_valid_i,
        l2_req.opcode, l2_req.subopcode, l2_req.address, l2_req.data[3:0], l2_req.size, l2_req.qwen,
        reduce_ready_o, reduce_start_i, tensorstore_busy_o, tensorstore_start_i, s3_inst_op_val_o,
        s2_inst_op_val_o, s1_inst_op_val_o, pipe_used_pre, pipe_used_o, s0_inst_send_val_q,
        s1_inst_send_val_o, s2_inst_send_val_o, s3_inst_send_val_o, ba_read_en_o, fsm_state_q,
        req_info_q, ts_scp_read_o
      })-1:0] = {
        vpu_reduce_wait_i, vpu_ctrl_o, atrans_en, new_partner_ready, new_req, s4_send,
        re_l2_req_valid_o, ts_l2_req_valid_o, re_l2_req_ready_i, ts_l2_req_ready_i, l2_resp_valid_i,
        l2_req.opcode, l2_req.subopcode, l2_req.address, l2_req.data[3:0], l2_req.size, l2_req.qwen,
        reduce_ready_o, reduce_start_i, tensorstore_busy_o, tensorstore_start_i, s3_inst_op_val_o,
        s2_inst_op_val_o, s1_inst_op_val_o, pipe_used_pre, pipe_used_o, s0_inst_send_val_q,
        s1_inst_send_val_o, s2_inst_send_val_o, s3_inst_send_val_o, ba_read_en_o, fsm_state_q,
        req_info_q, ts_scp_read_o
      };

    sm_data_group3 = '0;
    sm_data_group2 = '0;

    sm_data_group1 = '0;
    sm_data_group1[$bits({
        ba_op_fifo_valid, ba_op_fifo_full, ba_alloc_fifo_val, ba_alloc_fifo_full, address_fifo_full,
        address_fifo_full, address_for_l2_ready, write_around_throttle_q, s4_send_pending_q,
        tensorstore_count_q, ts_is_from_scp_q, ts_scp_read_way_o, ts_scp_read_addr_o,
        ts_col_size_is_128b_o, partner_ready_mask_q, got_ready_msg_q, l2_req_inflight_q,
        atrans_state_q, atrans_num_req_q, address_count_q, address_use_idx_q, addr_q, req_info_q,
        reg_count_q, fsm_state_q
      })-1:0] = {
        ba_op_fifo_valid, ba_op_fifo_full, ba_alloc_fifo_val, ba_alloc_fifo_full, address_fifo_full,
        address_fifo_full, address_for_l2_ready, write_around_throttle_q, s4_send_pending_q,
        tensorstore_count_q, ts_is_from_scp_q, ts_scp_read_way_o, ts_scp_read_addr_o,
        ts_col_size_is_128b_o, partner_ready_mask_q, got_ready_msg_q, l2_req_inflight_q,
        atrans_state_q, atrans_num_req_q, address_count_q, address_use_idx_q, addr_q, req_info_q,
        reg_count_q, fsm_state_q
      };

    sm_data_group0 = '0;
    sm_data_group0[$bits({
        stride_addr_q, stride_entry_q, scp_entry_q, scp_idx_q, l2_req.opcode, l2_req.subopcode,
        l2_req.address, l2_req.data[3:0], l2_req.size, l2_req.qwen, ba_read_addr_o
      })-1:0] = {
        stride_addr_q, stride_entry_q, scp_entry_q, scp_idx_q, l2_req.opcode, l2_req.subopcode,
        l2_req.address, l2_req.data[3:0], l2_req.size, l2_req.qwen, ba_read_addr_o
      };

    sm_data_debug_signals_o = {sm_data_group3, sm_data_group2, sm_data_group1, sm_data_group0};
  end
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : minion_dcache_reduce
