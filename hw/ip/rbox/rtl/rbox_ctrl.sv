// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX controller — master controller for the render box.
//
// Manages the input packet buffer, output packet generation, and all
// communication with the Shire Cache. Arbitrates SC requests between
// the depth buffer, input buffer, output message port, and output
// packet writes. Drives traverse and test pipeline control signals.
//
// Pipeline stages:
//   S0: ESR configuration registers (clock domain: clk_i)
//   S1: Input buffer request generation, response handling
//   S2: Input FIFO read, packet decode, state loading
//   S3: Registered state (triangle, tile, rbox, fsh)
//   S4: Output buffer, packet/message generation, SC request
//
// Differences from original (rbox_ctrl.v):
//   - Reset: active-low async-assert/sync-deassert (rst_ni) instead of
//     active-high synchronous (reset)
//   - Naming: lowRISC _i/_o/_d/_q conventions
//   - Types/constants: rbox_pkg/etlink_pkg instead of `include "soc.vh"
//   - Flip-flop macros replaced with explicit always_ff
//   - DFT/RAM config via separate dft_t and ram_cfg_t ports

/* verilator lint_off UNUSEDSIGNAL */  // Packed struct fields partially used (ESR registers, input packets, ET-Link responses)
/* verilator lint_off WIDTHEXPAND */   // Zero-extension in address arithmetic and hart ID computations
/* verilator lint_off WIDTHTRUNC */    // Bit-select arithmetic on buffer pointers and address fields
/* verilator lint_off UNOPTFLAT */     // s4_hart_*_nxt: independent per-hart fields assigned in same always_comb — acyclic per element

module rbox_ctrl
  import rbox_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          clk_rbox_i,
  input  logic                          rst_ni,

  // Configuration
  input  ram_cfg_t                      ram_cfg_i,
  input  dft_t                          dft_i,

  // Global enable for RBOX clock, ESR driven
  output logic                          rbox_enable_o,

  // Request to Shire Cache
  output req_t                          rbox_sc_req_o,
  output logic [ScBanks:0]              rbox_sc_req_valid_o,
  input  logic [ScBanks:0]              rbox_sc_req_ready_i,

  // Response from Shire Cache
  input  rsp_t                          sc_rbox_rsp_i,
  input  logic                          sc_rbox_rsp_valid_i,
  output logic                          sc_rbox_rsp_ready_o,

  // Request from Depth Buffer to Shire Cache
  input  req_t                          db_sc_req_i,
  input  logic [ScBanks:0]              db_sc_req_valid_i,
  output logic                          db_sc_req_ready_o,

  // Response from Shire Cache to Depth Buffer
  output rsp_t                          sc_db_rsp_o,
  output logic                          sc_db_rsp_valid_o,

  // RBOX ESRs
  input  logic [63:0]                   esr_config_i,
  input  logic                          esr_config_en_i,
  input  logic [63:0]                   esr_in_buf_pg_i,
  input  logic                          esr_in_buf_pg_en_i,
  input  logic [63:0]                   esr_in_buf_cfg_i,
  input  logic                          esr_in_buf_cfg_en_i,
  input  logic [31:0]                   esr_out_buf_pg_i,
  input  logic                          esr_out_buf_pg_en_i,
  input  logic [63:0]                   esr_out_buf_cfg_i,
  input  logic                          esr_out_buf_cfg_en_i,
  input  logic [31:0]                   esr_start_i,
  input  logic                          esr_start_en_i,
  input  logic [31:0]                   esr_consume_i,
  input  logic                          esr_consume_en_i,
  output logic [31:0]                   esr_status_o,

  // Control to traverse
  output logic                          tr_msaa_enable_o,
  output msaa_mode_e                    tr_msaa_mode_o,
  output logic                          tr_depth_is_fp32_o,
  output logic                          tr_depth_is_un16_o,
  output logic                          tr_depth_is_un24_o,
  output logic [MaxRenderDimL-1:0]      tr_scissor_min_x_o,
  output logic [MaxRenderDimL-1:0]      tr_scissor_max_x_o,
  output logic [MaxRenderDimL-1:0]      tr_scissor_min_y_o,
  output logic [MaxRenderDimL-1:0]      tr_scissor_max_y_o,
  output logic                          tr_depth_clamp_enable_o,
  output logic [31:0]                   tr_depth_min_o,
  output logic [31:0]                   tr_depth_max_o,
  output logic                          tr_depth_bound_enable_o,
  output logic [31:0]                   tr_depth_bound_min_o,
  output logic [31:0]                   tr_depth_bound_max_o,

  // Work to Traverse
  output triangle_state_t               tr_triangle_o,
  output tile_state_t                   tr_tile_o,
  output logic                          tr_tile_valid_o,
  input  logic                          tr_accept_i,
  input  logic                          tr_empty_i,

  // Output from Traverse
  input  quad_data_t                    tr_quad_i,
  input  logic                          tr_quad_valid_i,
  output logic                          ct_quad_accept_o,

  // Control to Test
  output state_t                        ts_state_o,
  output logic                          ts_front_triangle_o,

  // Test to Control
  input  logic                          ts_empty_i,

  // Output from Test
  input  logic [3:0]                    ts_result_mask_i,
  input  logic                          ts_result_valid_i,
  output logic                          ct_result_accept_o,

  // Flush Depth Buffer to/from Test
  output logic                          db_flush_o,
  input  logic                          db_flush_done_i,

  // Signals to Status Monitor
  output sm_ctrl_t                      ct_sm_data_o,

  // Debug run control
  input  logic                          rbox_halted_i
);

  // ════════════════════════════════════════════════════════════
  // Local parameters
  // ════════════════════════════════════════════════════════════

  localparam int unsigned BankLsb    = 6;
  localparam int unsigned BankIdSize = $clog2(ScBanks);

  // ════════════════════════════════════════════════════════════
  // S0: ESR Configuration Registers
  // ════════════════════════════════════════════════════════════

  esr_config_t      s0_esr_config_q;
  esr_in_buf_pg_t   s0_esr_in_buf_pg_q;
  esr_in_buf_cfg_t  s0_esr_in_buf_cfg_q;
  esr_out_buf_pg_t  s0_esr_out_buf_pg_q;
  esr_out_buf_cfg_t s0_esr_out_buf_cfg_q;
  esr_consume_t     s0_esr_consume_q;
  esr_start_t       s0_esr_start_q;

  esr_status_t      s0_esr_status_q;
  esr_status_t      s0_esr_status_d;
  logic             s0_esr_status_en;

  // ESR config register (clk_i domain)
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      s0_esr_config_q <= esr_config_t'(EsrConfigReset);
    else if (esr_config_en_i)
      s0_esr_config_q <= esr_config_t'(esr_config_i);
  end

  // ESR registers (clk_rbox_i domain)
  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s0_esr_in_buf_pg_q <= esr_in_buf_pg_t'(EsrInBufPgReset);
    else if (esr_in_buf_pg_en_i)
      s0_esr_in_buf_pg_q <= esr_in_buf_pg_t'(esr_in_buf_pg_i);
  end

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s0_esr_in_buf_cfg_q <= esr_in_buf_cfg_t'(EsrInBufCfgReset);
    else if (esr_in_buf_cfg_en_i)
      s0_esr_in_buf_cfg_q <= esr_in_buf_cfg_t'(esr_in_buf_cfg_i);
  end

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s0_esr_out_buf_pg_q <= esr_out_buf_pg_t'(EsrOutBufPgReset);
    else if (esr_out_buf_pg_en_i)
      s0_esr_out_buf_pg_q <= esr_out_buf_pg_t'(esr_out_buf_pg_i);
  end

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s0_esr_out_buf_cfg_q <= esr_out_buf_cfg_t'(EsrOutBufCfgReset);
    else if (esr_out_buf_cfg_en_i)
      s0_esr_out_buf_cfg_q <= esr_out_buf_cfg_t'(esr_out_buf_cfg_i);
  end

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s0_esr_start_q <= esr_start_t'(EsrStartReset);
    else if (esr_start_en_i)
      s0_esr_start_q <= esr_start_t'(esr_start_i);
  end

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s0_esr_status_q <= esr_status_t'(EsrStatusReset);
    else if (s0_esr_status_en)
      s0_esr_status_q <= s0_esr_status_d;
  end

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s0_esr_consume_q <= esr_consume_t'(EsrConsumeReset);
    else if (esr_consume_en_i)
      s0_esr_consume_q <= esr_consume_t'(esr_consume_i);
  end

  // ════════════════════════════════════════════════════════════
  // S1: Input buffer control, start/done logic
  // ════════════════════════════════════════════════════════════

  logic                      s1_start_trigger;
  logic [InBufLineSz-1:0]    s1_in_buf_off_q;
  logic [InBufLineSz-1:0]    s1_in_buf_off_d;
  logic [InBufLineSz-1:0]    s1_in_pending_lines_q;
  logic [InBufLineSz-1:0]    s1_in_pending_lines_d;
  logic                      s1_in_req_line_en;
  logic [InBufLineSz-1:0]    s1_in_received_lines_q;
  logic [InBufLineSz-1:0]    s1_in_received_lines_d;
  logic                      s1_in_received_lines_en;
  logic [InBufLineSz-1:0]    s1_in_process_pckts_q;
  logic [InBufLineSz-1:0]    s1_in_process_pckts_d;
  logic                      s1_in_process_pckts_en;
  logic                      s1_started_q;
  logic                      s1_started_d;
  logic                      s1_done_q;
  logic                      s1_done_d;
  logic                      s1_db_flush_pending_en;
  logic                      s1_db_flush_pending_q;
  logic                      s1_db_flush_pending_d;
  logic                      s1_db_flush_done_en;
  logic                      s1_db_flush_done_q;
  logic                      s1_db_flush_done_d;
  logic                      s1_in_update;
  logic                      s1_config_error;
  logic                      s1_in_new_req;
  logic                      s1_in_buffer_end;
  logic                      s1_sc_rbox_rsp_is_read_data;
  logic                      s1_in_sc_req_ready;
  logic [ScBanks:0]          s1_in_sc_req_valid_q;
  logic                      s2_in_fifo_pop_done;
  logic                      s3_end_of_buffer_q;
  logic                      s4_all_sent_end_buf_q;
  logic                      s4_all_sent_msg_q;

  always_comb begin
    s1_start_trigger = esr_start_en_i && esr_start_i[0];

    s1_config_error =   !s0_esr_in_buf_pg_q.page_active0 && !s0_esr_in_buf_pg_q.page_active1
                    ||  !s0_esr_in_buf_pg_q.page_active0 &&  s0_esr_in_buf_cfg_q.start_offset[OutBufLineSz-1]
                    ||  !s0_esr_in_buf_pg_q.page_active1 &&  s0_esr_in_buf_cfg_q.start_offset[OutBufLineSz-1]
                    || !(s0_esr_in_buf_pg_q.page_active1 &&  s0_esr_in_buf_pg_q.page_active0) && s0_esr_in_buf_cfg_q.line_count[OutBufLineSz-1]
                    ||  !s0_esr_out_buf_pg_q.page_active;

    s1_started_d = (s1_started_q && ~s1_done_q) || s1_start_trigger;

    s1_done_d = s1_started_q
              && (   s1_config_error
                  || (   s3_end_of_buffer_q                    // RTLMIN-4655: probably redundant as s4_all_sent_end_buf_q should be false otherwise
                      && s4_all_sent_end_buf_q                 // Wait for all Minion HARTs to receive their End Phase packet
                      && s4_all_sent_msg_q                     // Wait for all Minion HARTs to receive the last Message
                      && (   !s0_esr_start_q.flush             // Flush of the Depth Buffer not required at the end.
                          ||  s1_db_flush_done_q)));           // Wait for the Depth Buffer to be flushed.

    s1_db_flush_pending_en  =  !rst_ni
                            || db_flush_done_i
                            || (    s1_started_q
                                && !s1_config_error
                                &&  s0_esr_start_q.flush
                                &&  s3_end_of_buffer_q
                                &&  tr_empty_i
                                &&  ts_empty_i
                                && !s1_db_flush_done_q);

    s1_db_flush_pending_d =  rst_ni
                          &&  s1_started_q
                          && !s1_config_error
                          &&  s0_esr_start_q.flush
                          &&  s3_end_of_buffer_q
                          &&  tr_empty_i
                          &&  ts_empty_i
                          && !db_flush_done_i;

    db_flush_o = s1_db_flush_pending_q;

    s1_db_flush_done_en =  !rst_ni
                        || s1_done_q
                        || db_flush_done_i;

    s1_db_flush_done_d = rst_ni && !s1_done_q && s1_db_flush_pending_q && db_flush_done_i;

    s1_in_req_line_en       = 1'b0;
    s1_in_received_lines_en = 1'b0;
    s1_in_process_pckts_en  = 1'b0;

    s1_in_pending_lines_d  = s1_in_pending_lines_q;
    s1_in_received_lines_d = s1_in_received_lines_q;
    s1_in_process_pckts_d  = s1_in_process_pckts_q;
    s1_in_buf_off_d        = s1_in_buf_off_q;

    if (s1_start_trigger && ~s1_started_q && !s1_config_error) begin
      s1_in_req_line_en       = 1'b1;
      s1_in_received_lines_en = 1'b1;
      s1_in_process_pckts_en  = 1'b1;

      s1_in_pending_lines_d  = s0_esr_in_buf_cfg_q.line_count;
      s1_in_received_lines_d = '0;
      s1_in_process_pckts_d  = '0;
      s1_in_buf_off_d        = '0;
    end

    if (s1_in_new_req && (!(|s1_in_sc_req_valid_q) || s1_in_sc_req_ready)) begin
      s1_in_req_line_en = 1'b1;
      s1_in_pending_lines_d--;
      s1_in_buf_off_d++;
    end

    if (s2_in_fifo_pop_done) begin
      s1_in_process_pckts_en = 1'b1;
      s1_in_process_pckts_d++;
    end

    if (s1_sc_rbox_rsp_is_read_data) begin
      s1_in_received_lines_en = 1'b1;
      s1_in_received_lines_d++;
    end

    s1_in_buffer_end = (s1_in_received_lines_q == s0_esr_in_buf_cfg_q.line_count);

    s0_esr_status_d = '0;
    if (s1_start_trigger && s1_config_error)
      s0_esr_status_d.status = StatusError;
    else if (s1_started_d)
      s0_esr_status_d.status = StatusWorking;
    else if (s1_done_q)
      s0_esr_status_d.status = StatusFinished;
    else
      s0_esr_status_d.status = StatusReady;

    s0_esr_status_en = !rst_ni || s1_start_trigger || (s1_started_q && s1_done_q);

    esr_status_o = s0_esr_status_q;

    rbox_enable_o = s0_esr_config_q.enable;
  end

  // ════════════════════════════════════════════════════════════
  // S1: SC response handling
  // ════════════════════════════════════════════════════════════

  logic [FifoPtrAddrSz-1:0]  s1_sc_rbox_rsp_buf_ptr;
  logic [511:0]               s1_sc_rbox_rsp_data;
  logic                       s4_hart_ack_received;
  logic [HartsL-1:0]          s4_hart_id_for_ack;

  always_comb begin
    sc_rbox_rsp_ready_o = sc_rbox_rsp_valid_i;

    // Data response for the Input Buffer : MSB of the response ID (bit 7 unless changed later) is 0.
    s1_sc_rbox_rsp_is_read_data =  sc_rbox_rsp_valid_i
                                && (sc_rbox_rsp_i.opcode == RspAckData)
                                && !sc_rbox_rsp_i.id[IdSize-1];
    s1_sc_rbox_rsp_buf_ptr = sc_rbox_rsp_i.id[FifoPtrAddrSz-1:0];
    s1_sc_rbox_rsp_data    = sc_rbox_rsp_i.data;

    // Write Ack response for the Output Buffer : MSB of the response ID (bit 7 unless changed later) is 0.
    s4_hart_ack_received =  sc_rbox_rsp_valid_i
                         && (sc_rbox_rsp_i.opcode == RspAck)
                         && !sc_rbox_rsp_i.id[IdSize-1];
    s4_hart_id_for_ack = sc_rbox_rsp_i.id[HartsL-1:0];

    // Response for the Depth Buffer : MSB of the response ID (bit 7 unless changed later) is 1.
    sc_db_rsp_valid_o =  sc_rbox_rsp_valid_i
                      && sc_rbox_rsp_i.id[IdSize-1];

    sc_db_rsp_o = sc_rbox_rsp_i;
  end

  // ════════════════════════════════════════════════════════════
  // S1: Flip-flops (clk_rbox_i domain)
  // ════════════════════════════════════════════════════════════

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s1_started_q <= 1'b0;
      s1_done_q    <= 1'b0;
    end else begin
      s1_started_q <= s1_started_d;
      s1_done_q    <= s1_done_d;
    end
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s1_db_flush_pending_en)
      s1_db_flush_pending_q <= s1_db_flush_pending_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s1_db_flush_done_en)
      s1_db_flush_done_q <= s1_db_flush_done_d;
  end

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni) begin
      s1_in_pending_lines_q  <= '0;
      s1_in_buf_off_q        <= '0;
    end else if (s1_in_req_line_en) begin
      s1_in_pending_lines_q  <= s1_in_pending_lines_d;
      s1_in_buf_off_q        <= s1_in_buf_off_d;
    end
  end

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s1_in_received_lines_q <= '0;
    else if (s1_in_received_lines_en)
      s1_in_received_lines_q <= s1_in_received_lines_d;
  end

  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s1_in_process_pckts_q <= '0;
    else if (s1_in_process_pckts_en)
      s1_in_process_pckts_q <= s1_in_process_pckts_d;
  end

  // ════════════════════════════════════════════════════════════
  // Input FIFO
  // ════════════════════════════════════════════════════════════

  logic [FifoPtrAddrSz-1:0]  s1_in_fifo_alloc_ptr;
  logic                       s1_in_fifo_alloc;

  logic [511:0]               s1_in_fifo_in_data;
  logic [FifoPtrAddrSz-1:0]  s1_in_fifo_push_ptr;
  logic                       s1_in_fifo_push;
  logic                       s1_in_fifo_push_done;

  logic [511:0]               s2_in_fifo_out_data;
  logic                       s2_in_fifo_out_valid;
  inpck_type_e                s2_in_fifo_out_pck_type;
  logic                       s2_in_fifo_pop;

  logic                       s1_in_fifo_full;
  logic                       s1_in_fifo_empty;

  rbox_input_fifo u_input_fifo (
    .clk_i       (clk_rbox_i           ),
    .rst_ni      (rst_ni               ),
    .ram_cfg_i   (ram_cfg_i            ),
    .dft_i       (dft_i                ),

    .alloc_ptr_o (s1_in_fifo_alloc_ptr ),
    .alloc_i     (s1_in_fifo_alloc     ),

    .in_data_i   (s1_in_fifo_in_data   ),
    .push_ptr_i  (s1_in_fifo_push_ptr  ),
    .push_i      (s1_in_fifo_push      ),
    .push_done_o (s1_in_fifo_push_done ),

    .out_data_o     (s2_in_fifo_out_data    ),
    .out_pck_type_o (s2_in_fifo_out_pck_type),
    .out_valid_o    (s2_in_fifo_out_valid   ),
    .pop_i          (s2_in_fifo_pop         ),
    .pop_done_o     (s2_in_fifo_pop_done    ),

    .buffer_end_i(s1_in_buffer_end     ),

    .full_o      (s1_in_fifo_full      ),
    .empty_o     (s1_in_fifo_empty     )
  );

  // ════════════════════════════════════════════════════════════
  // S1: SC request generation for input buffer reads
  // ════════════════════════════════════════════════════════════

  logic [ScBanks:0]          s1_in_sc_req_valid_d;
  logic                      s1_in_sc_req_valid_en;
  req_t                      s1_in_sc_req_d;
  req_t                      s1_in_sc_req_q;
  logic                      s1_in_sc_req_en;
  logic [PaSize-1:0]         s1_req_pa;
  logic [InBufLineSz-1:0]    s1_req_line;

  always_comb begin
    s1_req_line = s0_esr_in_buf_cfg_q.start_offset + s1_in_buf_off_q;
    s1_req_pa = '0;
    if (s0_esr_in_buf_pg_q.page_active0 && s0_esr_in_buf_pg_q.page_active1)
      s1_req_pa = {s1_req_line[InBufLineSz-1] ? s0_esr_in_buf_pg_q.page1 : s0_esr_in_buf_pg_q.page0, s1_req_line[InBufLineSz-2:0], 6'b0};
    else if (s0_esr_in_buf_pg_q.page_active1)
      s1_req_pa = {s0_esr_in_buf_pg_q.page1, s1_req_line[InBufLineSz-2:0], 6'b0};
    else if (s0_esr_in_buf_pg_q.page_active0)
      s1_req_pa = {s0_esr_in_buf_pg_q.page0, s1_req_line[InBufLineSz-2:0], 6'b0};

    s1_in_new_req = s1_started_q
                  && (s1_in_pending_lines_q != '0)
                  && !s1_in_fifo_full;

    s1_in_sc_req_d           = '0;
    s1_in_sc_req_d.id        = IdSize'(s1_in_fifo_alloc_ptr);
    s1_in_sc_req_d.source    = '0;
    s1_in_sc_req_d.opcode    = ReqRead;
    s1_in_sc_req_d.subopcode = '0;
    s1_in_sc_req_d.address   = s1_req_pa;
    s1_in_sc_req_d.wdata     = 1'b0;
    s1_in_sc_req_d.data      = '0;
    s1_in_sc_req_d.size      = SizeLine;
    s1_in_sc_req_d.qwen      = 4'b0000;

    s1_in_sc_req_valid_d = '0;

    s1_in_sc_req_valid_d[s1_req_pa[BankLsb +: BankIdSize]] = rst_ni && s1_in_new_req;

    s1_in_sc_req_valid_en =  !rst_ni
                          || (|s1_in_sc_req_valid_d) && !(|s1_in_sc_req_valid_q)
                          ||  s1_in_sc_req_ready;

    s1_in_sc_req_en =     (|s1_in_sc_req_valid_d)
                    && ( !(|s1_in_sc_req_valid_q) || s1_in_sc_req_ready);

    s1_in_fifo_alloc = s1_in_sc_req_en;

    s1_in_fifo_in_data  = s1_sc_rbox_rsp_data;
    s1_in_fifo_push     = s1_sc_rbox_rsp_is_read_data;
    s1_in_fifo_push_ptr = s1_sc_rbox_rsp_buf_ptr;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s1_in_sc_req_en)
      s1_in_sc_req_q <= s1_in_sc_req_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s1_in_sc_req_valid_en)
      s1_in_sc_req_valid_q <= s1_in_sc_req_valid_d;
  end

  // ════════════════════════════════════════════════════════════
  // SC request arbiter
  // ════════════════════════════════════════════════════════════

  req_t                      s4_out_msg_sc_req_q;
  logic [ScBanks:0]          s4_out_msg_sc_req_valid_q;
  logic                      s4_out_msg_sc_req_ready;

  req_t                      s4_out_pck_sc_req_q;
  logic [ScBanks:0]          s4_out_pck_sc_req_valid_q;
  logic                      s4_out_pck_sc_req_ready;

  always_comb begin
    //
    //  Arbiter request to Shire Cache
    //
    //  Priority :
    //
    //   - 1st : depth buffer
    //   - 2nd : input buffer
    //   - 3rd : output message port write
    //   - 4th : output packet write
    //

    rbox_sc_req_o       = '0;
    rbox_sc_req_valid_o = '0;

    priority case (1'b1)
      (|db_sc_req_valid_i)          : {rbox_sc_req_valid_o, rbox_sc_req_o} = {        db_sc_req_valid_i,          db_sc_req_i         };
      (|s1_in_sc_req_valid_q)       : {rbox_sc_req_valid_o, rbox_sc_req_o} = {     s1_in_sc_req_valid_q,      s1_in_sc_req_q      };
      (|s4_out_msg_sc_req_valid_q)  : {rbox_sc_req_valid_o, rbox_sc_req_o} = {s4_out_msg_sc_req_valid_q, s4_out_msg_sc_req_q };
      (|s4_out_pck_sc_req_valid_q)  : {rbox_sc_req_valid_o, rbox_sc_req_o} = {s4_out_pck_sc_req_valid_q, s4_out_pck_sc_req_q };
      default                       : ;
    endcase

    db_sc_req_ready_o       = 1'b0;
    s1_in_sc_req_ready      = 1'b0;
    s4_out_msg_sc_req_ready = 1'b0;
    s4_out_pck_sc_req_ready = 1'b0;

    priority case (1'b1)
      (|db_sc_req_valid_i)          : db_sc_req_ready_o       = |(        db_sc_req_valid_i          & rbox_sc_req_ready_i);
      (|s1_in_sc_req_valid_q)       : s1_in_sc_req_ready      = |(     s1_in_sc_req_valid_q       & rbox_sc_req_ready_i);
      (|s4_out_msg_sc_req_valid_q)  : s4_out_msg_sc_req_ready = |(s4_out_msg_sc_req_valid_q  & rbox_sc_req_ready_i);
      (|s4_out_pck_sc_req_valid_q)  : s4_out_pck_sc_req_ready = |(s4_out_pck_sc_req_valid_q  & rbox_sc_req_ready_i);
      default                       : ;
    endcase
  end

  // ════════════════════════════════════════════════════════════
  // S2: Input packet decode
  // ════════════════════════════════════════════════════════════

  logic [InpckDataSz-1:0]              s2_read_pck_data;
  inpck_rbox_state_t                   s2_inpck_rbox_state;
  inpck_fsh_state_t                    s2_inpck_fsh_state;
  inpck_large_triangle_t               s2_inpck_rbox_large_triangle;
  inpck_tri_with_tile_128x128_t        s2_inpck_rbox_tri_w_128t;
  inpck_tri_with_tile_64x64_t          s2_inpck_rbox_tri_w_64t;
  inpck_large_triangle_tile_t          s2_inpck_rbox_ltt;
  inpck_fully_covered_tile_t           s2_inpck_rbox_fct;
  state_t                              s2_rbox_state;
  fsh_state_t                          s2_fsh_state;
  triangle_state_t                     s2_triangle_state;
  tile_state_t                         s2_tile_state;
  logic [MaxRenderDimL-1:0]            s2_scissor_min_x;
  logic [MaxRenderDimL-1:0]            s2_scissor_max_x;
  logic [MaxRenderDimL-1:0]            s2_scissor_min_y;
  logic [MaxRenderDimL-1:0]            s2_scissor_max_y;
  logic [HartsL-1:0]                   s2_last_active_hart;
  logic [HartsPerShire-1:0]            s2_hart_active_mask;
  logic                                s2_hart_active_en;
  logic                                s2_load_rbox_state;
  logic                                s2_load_fsh_state;
  logic                                s2_load_triangle_state;
  logic                                s2_load_tile_state;
  logic                                s2_input_pck_accept;
  logic                                s2_drawcall_accept;
  logic                                s2_new_tile;
  logic                                s2_new_drawcall;
  logic                                s2_tile_valid_d;
  logic                                s2_tile_valid_en;
  logic                                s2_end_of_buffer;
  logic                                s2_end_of_buffer_en;
  logic                                s3_tile_valid_q;
  logic                                s4_drawcall_done_q;

  always_comb begin
    s2_input_pck_accept = s2_in_fifo_out_valid && !s3_tile_valid_q && tr_accept_i;
    s2_drawcall_accept  = s2_in_fifo_out_valid && s4_drawcall_done_q;

    s2_read_pck_data              = s2_in_fifo_out_data;
    s2_inpck_rbox_state           = inpck_rbox_state_t'(s2_read_pck_data[$bits(inpck_rbox_state_t)-1:0]);
    s2_inpck_fsh_state            = inpck_fsh_state_t'(s2_read_pck_data[$bits(inpck_fsh_state_t)-1:0]);
    s2_inpck_rbox_large_triangle  = inpck_large_triangle_t'(s2_read_pck_data[$bits(inpck_large_triangle_t)-1:0]);
    s2_inpck_rbox_tri_w_128t     = inpck_tri_with_tile_128x128_t'(s2_read_pck_data[$bits(inpck_tri_with_tile_128x128_t)-1:0]);
    s2_inpck_rbox_tri_w_64t      = inpck_tri_with_tile_64x64_t'(s2_read_pck_data[$bits(inpck_tri_with_tile_64x64_t)-1:0]);
    s2_inpck_rbox_ltt             = inpck_large_triangle_tile_t'(s2_read_pck_data[$bits(inpck_large_triangle_tile_t)-1:0]);
    s2_inpck_rbox_fct             = inpck_fully_covered_tile_t'(s2_read_pck_data[$bits(inpck_fully_covered_tile_t)-1:0]);

    s2_end_of_buffer_en = s1_start_trigger;
    s2_end_of_buffer    = s3_end_of_buffer_q && !s1_start_trigger;

    s2_load_triangle_state = 1'b0;
    s2_load_tile_state     = 1'b0;
    s2_load_rbox_state     = 1'b0;
    s2_load_fsh_state      = 1'b0;

    s2_triangle_state      = '0;
    s2_tile_state          = '0;
    s2_rbox_state          = '0;
    s2_fsh_state           = '0;

    s2_new_drawcall        = 1'b0;
    s2_new_tile            = 1'b0;

    s2_last_active_hart    = {HartsL{1'b1}}; // all-ones = 63

    s2_hart_active_mask = '1;

    unique casez (s2_in_fifo_out_pck_type)
      InpckTriWithTile64x64:   begin
                                 s2_new_tile            = s2_in_fifo_out_valid;
                                 s2_load_triangle_state = s2_input_pck_accept;
                                 s2_load_tile_state     = s2_input_pck_accept;
                                 s2_triangle_state      = t64_triangle_state(s2_inpck_rbox_tri_w_64t);
                                 s2_tile_state          = t64_tile_state(s2_inpck_rbox_tri_w_64t);
                               end
      InpckTriWithTile128x128: begin
                                 s2_new_tile            = s2_in_fifo_out_valid;
                                 s2_load_triangle_state = s2_input_pck_accept;
                                 s2_load_tile_state     = s2_input_pck_accept;
                                 s2_triangle_state      = t128_triangle_state(s2_inpck_rbox_tri_w_128t);
                                 s2_tile_state          = t128_tile_state(s2_inpck_rbox_tri_w_128t);
                               end
      InpckLargeTriangle:      begin
                                 s2_load_triangle_state = s2_input_pck_accept;
                                 s2_triangle_state      = lt_triangle_state(s2_inpck_rbox_large_triangle);
                               end
      InpckFullyCoveredTile:   begin
                                 s2_new_tile        = s2_in_fifo_out_valid;
                                 s2_load_tile_state = s2_input_pck_accept;
                                 s2_tile_state      = fct_tile_state(s2_inpck_rbox_fct);
                               end
      InpckLargeTriangleTile:  begin
                                 s2_new_tile        = s2_in_fifo_out_valid;
                                 s2_load_tile_state = s2_input_pck_accept;
                                 s2_tile_state      = fct_tile_state(s2_inpck_rbox_fct);
                               end
      InpckRboxState:          begin
                                 s2_load_rbox_state  = s2_input_pck_accept;
                                 s2_rbox_state       = state_from_inpck(s2_inpck_rbox_state.state);
                                 s2_last_active_hart =
                                   (('d1 << (  {{HartsL-2{1'b0}}, s2_rbox_state.shire_layout_height}
                                             + {{HartsL-2{1'b0}}, s2_rbox_state.shire_layout_width}))
                                   - 'd1);
                                 for (int m = 0; m < HartsPerShire; m++)
                                   s2_hart_active_mask[m] = (m[HartsL-1:0] <= s2_last_active_hart);
                               end
      InpckFragShadingState:   begin
                                 s2_new_drawcall   = s2_in_fifo_out_valid;
                                 s2_load_fsh_state = s2_drawcall_accept;
                                 s2_fsh_state      = fsh_state_from_inpck(s2_inpck_fsh_state);
                               end
      InpckEndOfBuffer:        begin
                                 s2_end_of_buffer_en = s2_in_fifo_out_valid;
                                 s2_end_of_buffer    = s2_in_fifo_out_valid;
                               end
      default: ;
    endcase

    // Scissor window is in range [min_x, max_x] for x and range [min_y, max_y] for y.
    s2_scissor_min_x = s2_rbox_state.scissor_start_x;
    s2_scissor_min_y = s2_rbox_state.scissor_start_y;
    s2_scissor_max_x = s2_rbox_state.scissor_start_x + s2_rbox_state.scissor_width;
    s2_scissor_max_y = s2_rbox_state.scissor_start_y + s2_rbox_state.scissor_height;

    s2_hart_active_en = !rst_ni || s2_load_rbox_state;

    s2_tile_valid_d = rst_ni && s2_new_tile && tr_accept_i && !s3_tile_valid_q;
    s2_tile_valid_en = !rst_ni || (s2_new_tile && tr_accept_i) || s3_tile_valid_q;

    s2_in_fifo_pop =    s1_started_q
                   && (   ( s2_new_drawcall && s2_drawcall_accept)
                       || (!s2_new_drawcall && s2_input_pck_accept)
                       ||   s2_end_of_buffer);
  end

  // ════════════════════════════════════════════════════════════
  // S3: Registered state
  // ════════════════════════════════════════════════════════════

  state_t                    s3_rbox_state_q;
  logic [MaxRenderDimL-1:0]  s3_scissor_min_x_q;
  logic [MaxRenderDimL-1:0]  s3_scissor_max_x_q;
  logic [MaxRenderDimL-1:0]  s3_scissor_min_y_q;
  logic [MaxRenderDimL-1:0]  s3_scissor_max_y_q;
  logic [HartsL-1:0]         s3_last_active_hart_q;
  logic [HartsPerShire-1:0]  s3_hart_active_mask_q;
  fsh_state_t                s3_fsh_state_q;
  triangle_state_t           s3_triangle_state_q;
  tile_state_t               s3_tile_state_q;

  always_ff @(posedge clk_rbox_i) begin
    if (s2_tile_valid_en)
      s3_tile_valid_q <= s2_tile_valid_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s2_load_rbox_state) begin
      s3_rbox_state_q    <= s2_rbox_state;
      s3_scissor_min_x_q <= s2_scissor_min_x;
      s3_scissor_max_x_q <= s2_scissor_max_x;
      s3_scissor_min_y_q <= s2_scissor_min_y;
      s3_scissor_max_y_q <= s2_scissor_max_y;
    end
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s2_hart_active_en) begin
      s3_last_active_hart_q <= s2_last_active_hart;
      s3_hart_active_mask_q <= s2_hart_active_mask;
    end
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s2_load_fsh_state)
      s3_fsh_state_q <= s2_fsh_state;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s2_load_triangle_state)
      s3_triangle_state_q <= s2_triangle_state;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s2_load_tile_state)
      s3_tile_state_q <= s2_tile_state;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s2_end_of_buffer_en)
      s3_end_of_buffer_q <= s2_end_of_buffer;
  end

  // ════════════════════════════════════════════════════════════
  // S4: Per-hart tracking arrays
  // ════════════════════════════════════════════════════════════

  logic [HartsPerShire-1:0][HartBufOffSz-1:0]    s4_hart_buf_off_q;
  logic [HartsPerShire-1:0][HartBufOffSz-1:0]    s4_hart_buf_off_d;
  logic [HartsPerShire-1:0]                       s4_hart_buf_off_en;
  logic [HartsPerShire-1:0][HartPcktCreditSz-1:0] s4_hart_pckt_credits_q;
  logic [HartsPerShire-1:0][HartPcktCreditSz-1:0] s4_hart_pckt_credits_d;
  logic [HartsPerShire-1:0][MaxHartMsgL:0]        s4_hart_msg_credits_q;
  logic [HartsPerShire-1:0][MaxHartMsgL:0]        s4_hart_msg_credits_d;
  logic [HartsPerShire-1:0]                       s4_hart_credits_en;
  logic [HartsPerShire-1:0][HartPcktCreditSz-1:0] s4_hart_pending_acks_q;
  logic [HartsPerShire-1:0][HartPcktCreditSz-1:0] s4_hart_pending_acks_d;
  logic [HartsPerShire-1:0]                       s4_hart_pending_acks_en;
  logic [HartsPerShire-1:0][HartPcktCreditSz-1:0] s4_hart_sent_pckts_q;
  logic [HartsPerShire-1:0][HartPcktCreditSz-1:0] s4_hart_sent_pckts_d;
  logic [HartsPerShire-1:0]                       s4_hart_sent_pckts_en;
  logic [HartsPerShire-1:0]                       s4_hart_send_fsh_state_q;
  logic [HartsPerShire-1:0]                       s4_hart_send_fsh_state_d;
  logic [HartsPerShire-1:0]                       s4_hart_send_fsh_state_en;
  logic [HartsPerShire-1:0]                       s4_hart_sent_fsh_state_q;
  logic [HartsPerShire-1:0]                       s4_hart_sent_fsh_state_d;
  logic [HartsPerShire-1:0]                       s4_hart_sent_fsh_state_en;
  logic [HartsPerShire-1:0]                       s4_hart_sent_end_buf_q;
  logic [HartsPerShire-1:0]                       s4_hart_sent_end_buf_d;
  logic [HartsPerShire-1:0]                       s4_hart_sent_end_buf_en;
  logic                                           s4_all_sent_end_buf_d;
  logic                                           s4_all_sent_end_buf_en;
  logic [HartsPerShire-1:0]                       s4_hart_send_msg_q;
  logic [HartsPerShire-1:0]                       s4_hart_send_msg_d;
  logic [HartsPerShire-1:0]                       s4_hart_send_msg_en;
  logic                                           s4_all_sent_msg_d;
  logic                                           s4_all_sent_msg_en;

  logic [HartsL-1:0]                              s4_hart_id;
  logic [HartsL-1:0]                              s4_pckt_send_hart_id;
  logic [HartsL-1:0]                              s4_msg_send_hart_id_q;
  logic [HartsL-1:0]                              s4_msg_send_hart_id_d;
  logic                                           s4_msg_send_hart_id_en;
  logic                                           s4_msg_send_hart_id_sel;
  logic [HartsL-1:0]                              s4_msg_sent_hart_id;
  logic [HartsL-1:0]                              s4_end_buf_hart_id_q;
  logic [HartsL-1:0]                              s4_end_buf_hart_id_d;
  logic                                           s4_end_buf_hart_id_en;
  logic                                           s4_end_buf_hart_id_sel;

  logic                                           s4_hart_packet_sent;
  logic                                           s4_hart_fsh_state_sent;
  logic                                           s4_hart_msg_sent;
  logic                                           s4_hart_msg_send_set;
  logic                                           s4_hart_end_buf_set;

  logic                                           s4_hart_credits_released_q;
  logic                                           s4_hart_credits_released_d;
  logic                                           s4_hart_credits_released_en;

  always_comb begin
    s4_hart_buf_off_d        = s4_hart_buf_off_q;
    s4_hart_pckt_credits_d   = s4_hart_pckt_credits_q;
    s4_hart_msg_credits_d    = s4_hart_msg_credits_q;
    s4_hart_sent_pckts_d     = s4_hart_sent_pckts_q;
    s4_hart_pending_acks_d   = s4_hart_pending_acks_q;
    s4_hart_send_fsh_state_d = s4_hart_send_fsh_state_q;
    s4_hart_sent_fsh_state_d = s4_hart_sent_fsh_state_q;
    s4_hart_sent_end_buf_d   = s4_hart_sent_end_buf_q;
    s4_hart_send_msg_d       = s4_hart_send_msg_q;

    s4_hart_buf_off_en        =   '0;
    s4_hart_credits_en        =   '0;
    s4_hart_sent_pckts_en     =   '0;
    s4_hart_pending_acks_en   =   '0;
    s4_hart_send_fsh_state_en =   '0;
    s4_hart_sent_fsh_state_en =   '0;
    s4_hart_sent_end_buf_en   =   '0;
    s4_end_buf_hart_id_en     = 1'b0;
    s4_all_sent_end_buf_en    = 1'b0;
    s4_hart_send_msg_en       =   '0;
    s4_msg_send_hart_id_en    = 1'b0;
    s4_all_sent_msg_en        = 1'b0;

    if (s1_start_trigger) begin
      s4_hart_buf_off_en        =   '1;
      s4_hart_credits_en        =   '1;
      s4_hart_sent_pckts_en     =   '1;
      s4_hart_pending_acks_en   =   '1;
      s4_hart_send_fsh_state_en =   '1;
      s4_hart_sent_fsh_state_en =   '1;
      s4_hart_sent_end_buf_en   =   '1;
      s4_end_buf_hart_id_en     = 1'b1;
      s4_all_sent_end_buf_en    = 1'b1;
      s4_hart_send_msg_en       =   '1;
      s4_msg_send_hart_id_en    = 1'b1;
      s4_all_sent_msg_en        = 1'b1;

      s4_hart_buf_off_d        = '0;
      s4_hart_sent_pckts_d     = '0;
      s4_hart_pending_acks_d   = '0;
      s4_hart_sent_fsh_state_d = '0;
      s4_hart_sent_end_buf_d   = '0;
      s4_hart_send_msg_d       = '0;
      for (int m = 0; m < HartsPerShire; m++) begin
        s4_hart_pckt_credits_d[m]   = HartPcktCreditSz'(1 << s0_esr_out_buf_cfg_q.hart_buf_lines);
        s4_hart_msg_credits_d[m]    = (MaxHartMsgL+1)'(s0_esr_out_buf_cfg_q.hart_max_msg + 1);
        s4_hart_send_fsh_state_d[m] = 1'b1;
      end
    end

    s4_pckt_send_hart_id = s4_hart_end_buf_set ? s4_end_buf_hart_id_q : s4_hart_id;

    if (s4_hart_packet_sent) begin
      if (s4_hart_buf_off_d[s4_pckt_send_hart_id] == HartBufOffSz'(1 << s0_esr_out_buf_cfg_q.hart_buf_lines))
        s4_hart_buf_off_d[s4_pckt_send_hart_id] = '0;
      else
        s4_hart_buf_off_d[s4_pckt_send_hart_id]++;
      s4_hart_buf_off_en[s4_pckt_send_hart_id] = 1'b1;

      s4_hart_pckt_credits_d[s4_pckt_send_hart_id]--;
      s4_hart_credits_en[s4_pckt_send_hart_id] = 1'b1;

      s4_hart_pending_acks_d[s4_pckt_send_hart_id]++;
      s4_hart_pending_acks_en[s4_pckt_send_hart_id] = 1'b1;

      s4_hart_sent_pckts_d[s4_pckt_send_hart_id]++;
      s4_hart_sent_pckts_en[s4_pckt_send_hart_id] = 1'b1;
    end

    if (s4_hart_fsh_state_sent) begin
      s4_hart_send_fsh_state_d[s4_hart_id] = 1'b0;
      s4_hart_send_fsh_state_en[s4_hart_id] = 1'b1;

      s4_hart_sent_fsh_state_d[s4_hart_id] = 1'b1;
      s4_hart_sent_fsh_state_en[s4_hart_id] = 1'b1;
    end

    if (s4_hart_ack_received) begin
      s4_hart_pending_acks_d[s4_hart_id_for_ack]--;
      s4_hart_pending_acks_en[s4_hart_id_for_ack] = 1'b1;
    end

    if (s4_hart_credits_released_q) begin
      s4_hart_pckt_credits_d[s0_esr_consume_q.hart_id] += HartPcktCreditSz'(s0_esr_consume_q.pckt_credits);
      s4_hart_msg_credits_d[s0_esr_consume_q.hart_id]  += (MaxHartMsgL+1)'(s0_esr_consume_q.msg_credits);
      s4_hart_credits_en[s0_esr_consume_q.hart_id] = 1'b1;
    end

    if (s2_load_fsh_state) begin
      s4_hart_send_fsh_state_d = '1;
      s4_hart_send_fsh_state_en = '1;

      s4_hart_sent_fsh_state_d = '0;
      s4_hart_sent_fsh_state_en = '1;
    end

    if (s4_hart_end_buf_set) begin
      s4_hart_sent_end_buf_en[s4_end_buf_hart_id_q] = 1'b1;
      s4_hart_sent_end_buf_d[s4_end_buf_hart_id_q]  = 1'b1;

      s4_all_sent_end_buf_en = 1'b1;
      s4_end_buf_hart_id_en  = 1'b1;
    end

    s4_msg_sent_hart_id = s4_out_msg_sc_req_q.id[HartsL-1:0];

    if (s4_hart_msg_sent) begin
      s4_hart_send_msg_d[s4_msg_sent_hart_id] = 1'b0;
      s4_hart_send_msg_en[s4_msg_sent_hart_id] = 1'b1;

      s4_hart_msg_credits_d[s4_msg_sent_hart_id]--;
      s4_hart_credits_en[s4_msg_sent_hart_id] = 1'b1;

      s4_hart_sent_pckts_d[s4_msg_sent_hart_id] = '0;
      s4_hart_sent_pckts_en[s4_msg_sent_hart_id] = 1'b1;

      s4_msg_send_hart_id_en = 1'b1;
      s4_all_sent_msg_en     = 1'b1;
    end

    if (s4_hart_msg_send_set) begin
      s4_hart_send_msg_d[s4_pckt_send_hart_id] = 1'b1;
      s4_hart_send_msg_en[s4_pckt_send_hart_id] = 1'b1;

      s4_msg_send_hart_id_en = 1'b1;
      s4_all_sent_msg_en     = 1'b1;
    end

    s4_hart_credits_released_d  = esr_consume_en_i;
    s4_hart_credits_released_en = esr_consume_en_i || s4_hart_credits_released_q;

    s4_all_sent_msg_d     = s1_start_trigger ? 1'b0 : !(|(s4_hart_send_msg_d & s3_hart_active_mask_q));
    s4_all_sent_end_buf_d = s1_start_trigger ? 1'b0 : &(s4_hart_sent_end_buf_d | (~s3_hart_active_mask_q));

    // RTLMIN-4770: Should start from the last HART ID to implement Round Robin?
    s4_msg_send_hart_id_d = '0;
    s4_msg_send_hart_id_sel = rst_ni;

    for (int m = 0; m < HartsPerShire; m++) begin
      if ((m[HartsL-1:0] <= s3_last_active_hart_q) && s4_msg_send_hart_id_sel) begin
        s4_msg_send_hart_id_sel = !s4_hart_send_msg_d[m];
        s4_msg_send_hart_id_d   =  s4_hart_send_msg_d[m] ? m[HartsL-1:0] : '0;
      end
    end

    // RTLMIN-4770: Should start from the last HART ID to implement Round Robin?
    s4_end_buf_hart_id_d = '0;
    s4_end_buf_hart_id_sel = rst_ni;

    for (int m = 0; m < HartsPerShire; m++) begin
      if ((m[HartsL-1:0] <= s3_last_active_hart_q) && s4_end_buf_hart_id_sel) begin
        s4_end_buf_hart_id_sel =  s4_hart_sent_end_buf_d[m];
        s4_end_buf_hart_id_d   = !s4_hart_sent_end_buf_d[m] ? m[HartsL-1:0] : '0;
      end
    end
  end

  // S4: Hart credits released flop
  always_ff @(posedge clk_rbox_i or negedge rst_ni) begin
    if (!rst_ni)
      s4_hart_credits_released_q <= 1'b0;
    else if (s4_hart_credits_released_en)
      s4_hart_credits_released_q <= s4_hart_credits_released_d;
  end

  // S4: Per-hart flop arrays
  for (genvar m = 0; m < HartsPerShire; m++) begin : gen_hart_flops
    always_ff @(posedge clk_rbox_i) begin
      if (s4_hart_buf_off_en[m])
        s4_hart_buf_off_q[m] <= s4_hart_buf_off_d[m];
    end

    always_ff @(posedge clk_rbox_i) begin
      if (s4_hart_credits_en[m]) begin
        s4_hart_pckt_credits_q[m] <= s4_hart_pckt_credits_d[m];
        s4_hart_msg_credits_q[m]  <= s4_hart_msg_credits_d[m];
      end
    end

    always_ff @(posedge clk_rbox_i) begin
      if (s4_hart_sent_pckts_en[m])
        s4_hart_sent_pckts_q[m] <= s4_hart_sent_pckts_d[m];
    end

    always_ff @(posedge clk_rbox_i) begin
      if (s4_hart_pending_acks_en[m])
        s4_hart_pending_acks_q[m] <= s4_hart_pending_acks_d[m];
    end

    always_ff @(posedge clk_rbox_i) begin
      if (s4_hart_send_fsh_state_en[m])
        s4_hart_send_fsh_state_q[m] <= s4_hart_send_fsh_state_d[m];
    end

    always_ff @(posedge clk_rbox_i) begin
      if (s4_hart_sent_fsh_state_en[m])
        s4_hart_sent_fsh_state_q[m] <= s4_hart_sent_fsh_state_d[m];
    end

    always_ff @(posedge clk_rbox_i) begin
      if (s4_hart_sent_end_buf_en[m])
        s4_hart_sent_end_buf_q[m] <= s4_hart_sent_end_buf_d[m];
    end

    always_ff @(posedge clk_rbox_i) begin
      if (s4_hart_send_msg_en[m])
        s4_hart_send_msg_q[m] <= s4_hart_send_msg_d[m];
    end
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_all_sent_msg_en)
      s4_all_sent_msg_q <= s4_all_sent_msg_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_all_sent_end_buf_en)
      s4_all_sent_end_buf_q <= s4_all_sent_end_buf_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_msg_send_hart_id_en)
      s4_msg_send_hart_id_q <= s4_msg_send_hart_id_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_end_buf_hart_id_en)
      s4_end_buf_hart_id_q <= s4_end_buf_hart_id_d;
  end

  // ════════════════════════════════════════════════════════════
  // S4: Drawcall done / end drawcall
  // ════════════════════════════════════════════════════════════

  logic s4_drawcall_done_d;
  logic s4_drawcall_done_en;
  logic s4_end_drawcall_d;
  logic s4_end_drawcall_en;
  logic s4_end_drawcall_q;
  logic s4_out_buf_empty;

  always_comb begin
    s4_drawcall_done_d = !s2_load_fsh_state;  // Clear when a new drawcall starts until it finishes
    s4_drawcall_done_en =  !rst_ni
                        || s2_load_fsh_state
                        || (tr_empty_i && ts_empty_i && s4_out_buf_empty);

    s4_end_drawcall_d =    rst_ni
                       && ((s2_new_drawcall && !s2_drawcall_accept) || s3_end_of_buffer_q)
                       &&  tr_empty_i
                       &&  ts_empty_i;
    s4_end_drawcall_en =  !rst_ni
                       || (!s4_end_drawcall_q && s4_end_drawcall_d)
                       ||  s2_drawcall_accept;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_drawcall_done_en)
      s4_drawcall_done_q <= s4_drawcall_done_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_end_drawcall_en)
      s4_end_drawcall_q <= s4_end_drawcall_d;
  end

  // ════════════════════════════════════════════════════════════
  // S4: Quad packet count, rbox state load delay
  // ════════════════════════════════════════════════════════════

  logic [MaxQuadPcksL-1:0] s4_num_quad_pcks_q;
  logic [MaxQuadPcksL-1:0] s4_num_quad_pcks_d;
  logic                    s4_load_rbox_state_q;
  logic                    s4_load_rbox_state_d;
  logic                    s4_load_rbox_state_en;

  always_comb begin
    s4_load_rbox_state_en  = s2_load_rbox_state || s4_load_rbox_state_q;
    s4_load_rbox_state_d   = s2_load_rbox_state;

    s4_num_quad_pcks_d = '0;

    if (!s3_rbox_state_q.fragment_shader_disabled) begin
      if (s3_rbox_state_q.fragment_shader_reads_bary)
        s4_num_quad_pcks_d += MaxQuadPcksL'(2);
      if (s3_rbox_state_q.fragment_shader_reads_depth)
        s4_num_quad_pcks_d++;
      if (s3_rbox_state_q.fragment_shader_reads_coverage)
        s4_num_quad_pcks_d++;
    end
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s2_load_rbox_state)
      s4_load_rbox_state_q <= s4_load_rbox_state_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_load_rbox_state_q)
      s4_num_quad_pcks_q <= s4_num_quad_pcks_d;
  end

  // ════════════════════════════════════════════════════════════
  // Output Buffer
  // ════════════════════════════════════════════════════════════

  quad_info_t [1:0]          s4_out_buf_info;
  logic                      s4_out_buf_info_valid;
  logic [255:0]              s4_out_buf_packet;
  logic                      s4_out_buf_packet_valid;
  logic [MaxQuadPcksL-1:0]   s4_pending_quad_pcks_q;
  logic                      s4_out_buf_pop;

  rbox_output_buffer u_output_buffer (
    .clk_i                (clk_rbox_i                                        ),
    .rst_ni               (rst_ni                                            ),
    .ram_cfg_i            (ram_cfg_i                                         ),
    .dft_i                (dft_i                                             ),

    .tr_quad_i            (tr_quad_i                                         ),
    .tr_quad_valid_i      (tr_quad_valid_i                                   ),
    .ct_quad_accept_o     (ct_quad_accept_o                                  ),

    .ts_result_mask_i     (ts_result_mask_i                                  ),
    .ts_result_valid_i    (ts_result_valid_i                                 ),
    .ct_result_accept_o   (ct_result_accept_o                                ),

    .out_info_o           (s4_out_buf_info                                   ),
    .out_info_valid_o     (s4_out_buf_info_valid                             ),
    .out_packet_o         (s4_out_buf_packet                                 ),
    .out_packet_valid_o   (s4_out_buf_packet_valid                           ),
    .out_pop_i            (s4_out_buf_pop                                    ),
    .out_pending_pcks_i   (s4_pending_quad_pcks_q                            ),

    .out_end_drawcall_i   (s4_end_drawcall_q                                 ),

    .empty_o              (s4_out_buf_empty                                   ),

    .hart_tile_width_i    (s3_rbox_state_q.hart_tile_width                   ),
    .hart_tile_height_i   (s3_rbox_state_q.hart_tile_height                  ),
    .shire_layout_width_i (s3_rbox_state_q.shire_layout_width                ),
    .shire_layout_height_i(s3_rbox_state_q.shire_layout_height               ),
    .fsh_reads_coverage_i (s3_rbox_state_q.fragment_shader_reads_coverage     ),
    .fsh_reads_depth_i    (s3_rbox_state_q.fragment_shader_reads_depth        ),
    .fsh_disabled_i       (s3_rbox_state_q.fragment_shader_disabled           ),
    .early_frag_tests_enable_i(s3_rbox_state_q.early_frag_tests_enable       ),

    .rbox_halted_i        (rbox_halted_i                                     )
  );

  // ════════════════════════════════════════════════════════════
  // S4: Output packet / message generation
  // ════════════════════════════════════════════════════════════

  logic [MaxQuadPcksL-1:0]   s4_pending_quad_pcks_d;
  logic                      s4_pending_quad_pcks_en;
  outpck_fsh_state_t         s4_fsh_state_pckt;
  outpck_quad_info_t         s4_quad_info_pckt;
  outpck_end_phase_t         s4_end_phase_pckt;
  req_t                      s4_out_msg_sc_req_d;
  logic                      s4_out_msg_sc_req_en;
  logic [ScBanks:0]          s4_out_msg_sc_req_valid_d;
  logic                      s4_out_msg_sc_req_valid_en;
  req_t                      s4_out_pck_sc_req_d;
  logic                      s4_out_pck_sc_req_en;
  logic [ScBanks:0]          s4_out_pck_sc_req_valid_d;
  logic                      s4_out_pck_sc_req_valid_en;
  logic [PaSize-1:0]         s4_out_pck_req_pa;
  logic [PaSize-1:0]         s4_out_pck_end_req_pa;
  logic [PaSize-1:0]         s4_out_msg_req_pa;
  logic [OutBufLineSz-1:0]   s4_hart_buf_ptr;
  logic [OutBufLineSz-1:0]   s4_hart_buf_end_ptr;

  always_comb begin
    s4_hart_id = s4_out_buf_info[0].hart_id;

    s4_hart_buf_ptr = OutBufLineSz'({s0_esr_out_buf_cfg_q.start_offset, 1'b0})
                    + OutBufLineSz'(OutBufLineSz'(1 << s0_esr_out_buf_cfg_q.hart_buf_lines) * OutBufLineSz'(s4_hart_id))
                    + OutBufLineSz'(s4_hart_buf_off_q[s4_hart_id]);

    s4_out_pck_req_pa = {s0_esr_out_buf_pg_q.page, s4_hart_buf_ptr, 5'b0};

    s4_pending_quad_pcks_d = '0;
    s4_pending_quad_pcks_en = !rst_ni;

    s4_out_pck_sc_req_valid_d = '0;
    s4_out_pck_sc_req_d       = '0;

    s4_out_pck_sc_req_valid_en = !rst_ni || s4_out_pck_sc_req_ready;
    s4_out_pck_sc_req_en       = 1'b0;

    s4_fsh_state_pckt  = '0;
    s4_quad_info_pckt  = '0;
    s4_end_phase_pckt  = '0;

    s4_hart_packet_sent    = 1'b0;
    s4_hart_fsh_state_sent = 1'b0;
    s4_hart_msg_send_set   = 1'b0;
    s4_hart_msg_sent       = 1'b0;

    s4_out_buf_pop = 1'b0;

    s4_out_msg_req_pa = '0;

    // Send Drawcall State packet to the target Minion HART.
    //
    // Conditions :
    //
    //  - Output Buffer reports there is a new pack of two quads ready to be written
    //  - Previous quads finished sending all packets
    //  - The Drawcall State packet was NOT sent for the target Minion HART
    //  - Packet credits available for the target Minion HART
    //  - No message port write pending for the target Minion HART
    //  - No pending requests to Shire Cache.
    //
    if (    s4_out_buf_info_valid
        && (s4_pending_quad_pcks_q == '0)
        &&  s4_hart_send_fsh_state_q[s4_hart_id]
        && (s4_hart_pckt_credits_q[s4_hart_id] > HartPcktCreditSz'(1))
        && !s4_hart_send_msg_q[s4_hart_id]
        && (!(|s4_out_pck_sc_req_valid_q) || s4_out_pck_sc_req_ready))
    begin
      s4_fsh_state_pckt.outpck_type           = OutpckStateInfo;
      s4_fsh_state_pckt.fshader_data_addr     = s3_fsh_state_q.fshader_data_addr;
      s4_fsh_state_pckt.fshader_function_addr = s3_fsh_state_q.fshader_function_addr;

      s4_out_pck_sc_req_d.source    = '0;
      s4_out_pck_sc_req_d.wdata     = 1'b1;
      s4_out_pck_sc_req_d.size      = SizeHLine;
      s4_out_pck_sc_req_d.qwen      = s4_out_pck_req_pa[5] ? 4'b1100 : 4'b0011;
      s4_out_pck_sc_req_d.opcode    = ReqWrite;
      s4_out_pck_sc_req_d.subopcode = '0;
      s4_out_pck_sc_req_d.id        = IdSize'(s4_hart_id);
      s4_out_pck_sc_req_d.address   = s4_out_pck_req_pa;
      s4_out_pck_sc_req_d.data      = {s4_fsh_state_pckt, s4_fsh_state_pckt};

      s4_out_pck_sc_req_valid_d[s4_out_pck_req_pa[BankLsb +: BankIdSize]] = 1'b1;

      s4_out_pck_sc_req_valid_en = 1'b1;
      s4_out_pck_sc_req_en       = 1'b1;

      s4_hart_packet_sent    = 1'b1;
      s4_hart_fsh_state_sent = 1'b1;

      // Check if need to force a message write for the target Minion HART
      // before the next pack of two quads can be written to Shire Cache.
      s4_hart_msg_send_set = ((s4_hart_sent_pckts_q[s4_hart_id] + HartPcktCreditSz'(1 + s4_num_quad_pcks_q)) >= HartPcktCreditSz'(1 << s0_esr_out_buf_cfg_q.max_pckts_msg));
    end

    // Start writing packets for a new pack of two quads.
    //
    // Conditions :
    //
    //  - Output Buffer reports there is a new pack of two quads ready to be written
    //  - Previous quads finished sending all packets
    //  - The Drawcall State packet was already sent for the target Minion HART
    //  - Packet credits available for the target Minion HART
    //  - No message port write pending for the target Minion HART
    //  - No pending requests to Shire Cache.
    //
    if (    s4_out_buf_info_valid
        && (s4_pending_quad_pcks_q == '0)
        && !s4_hart_send_fsh_state_q[s4_hart_id]
        && (s4_hart_pckt_credits_q[s4_hart_id] > HartPcktCreditSz'(s4_num_quad_pcks_q))
        && !s4_hart_send_msg_q[s4_hart_id]
        && (!(|s4_out_pck_sc_req_valid_q) || s4_out_pck_sc_req_ready))
    begin
      s4_pending_quad_pcks_en  = 1'b1;
      s4_pending_quad_pcks_d   = s4_num_quad_pcks_q;

      s4_quad_info_pckt.outpck_type = OutpckQuadInfo;
      s4_quad_info_pckt.quad0_x     = {s3_tile_state_q.left[MaxRenderDimL-1:MaxTileDimL], s4_out_buf_info[0].x};
      s4_quad_info_pckt.quad0_y     = {s3_tile_state_q.top[MaxRenderDimL-1:MaxTileDimL],  s4_out_buf_info[0].y};
      s4_quad_info_pckt.quad1_x     = s4_out_buf_info[1].start_pack ? '0 : {s3_tile_state_q.left[MaxRenderDimL-1:MaxTileDimL], s4_out_buf_info[1].x};
      s4_quad_info_pckt.quad1_y     = s4_out_buf_info[1].start_pack ? '0 : {s3_tile_state_q.top[MaxRenderDimL-1:MaxTileDimL],  s4_out_buf_info[1].y};
      s4_quad_info_pckt.sample_idx  = s4_out_buf_info[0].sample_idx;
      s4_quad_info_pckt.mask        = {(s4_out_buf_info[1].start_pack ? 4'b0 : s4_out_buf_info[1].mask), s4_out_buf_info[0].mask};
      s4_quad_info_pckt.quad0_triangle_ptr = s3_triangle_state_q.triangle_data_addr;
      s4_quad_info_pckt.quad1_triangle_ptr = s4_out_buf_info[1].start_pack ? '0 : s3_triangle_state_q.triangle_data_addr;

      s4_out_pck_sc_req_d.source    = '0;
      s4_out_pck_sc_req_d.wdata     = 1'b1;
      s4_out_pck_sc_req_d.size      = SizeHLine;
      s4_out_pck_sc_req_d.qwen      = s4_out_pck_req_pa[5] ? 4'b1100 : 4'b0011;
      s4_out_pck_sc_req_d.opcode    = ReqWrite;
      s4_out_pck_sc_req_d.subopcode = '0;
      s4_out_pck_sc_req_d.id        = IdSize'(s4_hart_id);
      s4_out_pck_sc_req_d.address   = s4_out_pck_req_pa;
      s4_out_pck_sc_req_d.data      = {s4_quad_info_pckt, s4_quad_info_pckt};

      s4_out_pck_sc_req_valid_d[s4_out_pck_req_pa[BankLsb +: BankIdSize]] = 1'b1;

      s4_out_pck_sc_req_valid_en = 1'b1;
      s4_out_pck_sc_req_en       = 1'b1;

      s4_hart_packet_sent = 1'b1;

      s4_out_buf_pop = (s4_pending_quad_pcks_q == MaxQuadPcksL'(1));

      // Check if need to force a message write for the target Minion HART
      // before the next pack of two quads can be written to Shire Cache.
      s4_hart_msg_send_set = ((s4_hart_sent_pckts_q[s4_hart_id] + HartPcktCreditSz'(2 * s4_num_quad_pcks_q)) >= HartPcktCreditSz'(1 << s0_esr_out_buf_cfg_q.max_pckts_msg));
    end

    // Send data packets for the current pack of two quads.
    //
    // Conditions :
    //
    //  - Valid packet data
    //  - There are pending data packets for the pack of two quads.
    //  - No pending requests to Shire Cache.
    //
    if (   s4_out_buf_packet_valid
        && (s4_pending_quad_pcks_q > '0)
        && (!(|s4_out_pck_sc_req_valid_q) || s4_out_pck_sc_req_ready))
    begin
      s4_pending_quad_pcks_en  = 1'b1;
      s4_pending_quad_pcks_d   = s4_pending_quad_pcks_q - MaxQuadPcksL'(1);

      s4_out_pck_sc_req_d.source    = '0;
      s4_out_pck_sc_req_d.wdata     = 1'b1;
      s4_out_pck_sc_req_d.size      = SizeHLine;
      s4_out_pck_sc_req_d.qwen      = s4_out_pck_req_pa[5] ? 4'b1100 : 4'b0011;
      s4_out_pck_sc_req_d.opcode    = ReqWrite;
      s4_out_pck_sc_req_d.subopcode = '0;
      s4_out_pck_sc_req_d.id        = IdSize'(s4_hart_id);
      s4_out_pck_sc_req_d.address   = s4_out_pck_req_pa;
      s4_out_pck_sc_req_d.data      = {(s4_out_buf_info[1].start_pack ? 128'b0 : s4_out_buf_packet[255:128]), s4_out_buf_packet[127:0],
                                       (s4_out_buf_info[1].start_pack ? 128'b0 : s4_out_buf_packet[255:128]), s4_out_buf_packet[127:0]};

      s4_out_pck_sc_req_valid_d[s4_out_pck_req_pa[BankLsb +: BankIdSize]] = 1'b1;

      s4_out_pck_sc_req_valid_en = 1'b1;
      s4_out_pck_sc_req_en       = 1'b1;

      s4_hart_packet_sent = 1'b1;

      s4_out_buf_pop = (s4_pending_quad_pcks_q == MaxQuadPcksL'(1));
    end

    s4_out_msg_sc_req_valid_d = '0;
    s4_out_msg_sc_req_d       = '0;

    s4_out_msg_sc_req_valid_en = !rst_ni || s4_out_msg_sc_req_ready;
    s4_out_msg_sc_req_en       = 1'b0;

    s4_hart_buf_end_ptr   = '0;
    s4_out_pck_end_req_pa = '0;

    s4_hart_end_buf_set = 1'b0;

    if (    !s4_hart_sent_end_buf_q[s4_end_buf_hart_id_q]
        &&  s3_end_of_buffer_q && tr_empty_i && ts_empty_i && s4_out_buf_empty
        && !(|s4_out_pck_sc_req_valid_d)
        && (!(|s4_out_pck_sc_req_valid_q) || s4_out_pck_sc_req_ready))
    begin
      s4_end_phase_pckt = '0;
      s4_end_phase_pckt.outpck_type = OutpckEndPhase;

      s4_hart_buf_end_ptr = OutBufLineSz'({s0_esr_out_buf_cfg_q.start_offset, 1'b0})
                          + OutBufLineSz'(OutBufLineSz'(1 << s0_esr_out_buf_cfg_q.hart_buf_lines) * OutBufLineSz'(s4_end_buf_hart_id_q))
                          + OutBufLineSz'(s4_hart_buf_off_q[s4_end_buf_hart_id_q]);

      s4_out_pck_end_req_pa = {s0_esr_out_buf_pg_q.page, s4_hart_buf_end_ptr, 5'b0};

      s4_out_pck_sc_req_d.source    = '0;
      s4_out_pck_sc_req_d.wdata     = 1'b1;
      s4_out_pck_sc_req_d.size      = SizeHLine;
      s4_out_pck_sc_req_d.qwen      = s4_out_pck_end_req_pa[5] ? 4'b1100 : 4'b0011;
      s4_out_pck_sc_req_d.opcode    = ReqWrite;
      s4_out_pck_sc_req_d.subopcode = '0;
      s4_out_pck_sc_req_d.id        = IdSize'(s4_end_buf_hart_id_q);
      s4_out_pck_sc_req_d.address   = s4_out_pck_end_req_pa;
      s4_out_pck_sc_req_d.data      = {s4_end_phase_pckt, s4_end_phase_pckt};

      s4_out_pck_sc_req_valid_d[s4_out_pck_end_req_pa[BankLsb +: BankIdSize]] = 1'b1;

      s4_out_pck_sc_req_valid_en = 1'b1;
      s4_out_pck_sc_req_en       = 1'b1;

      s4_hart_packet_sent = 1'b1;

      s4_hart_end_buf_set = 1'b1;

      s4_hart_msg_send_set = 1'b1;
    end

    s4_out_msg_req_pa = '0;

    if (   s4_hart_send_msg_q[s4_msg_send_hart_id_q]
        && (s4_hart_pending_acks_q[s4_msg_send_hart_id_q] == '0)
        && (!(|s4_out_msg_sc_req_valid_q) || s4_out_pck_sc_req_ready))
    begin
      s4_out_msg_req_pa = hart_msg_port_pa(s4_msg_send_hart_id_q, s0_esr_out_buf_cfg_q.hart_port_id);

      s4_out_msg_sc_req_d.source    = '0;
      s4_out_msg_sc_req_d.wdata     = 1'b1;
      s4_out_msg_sc_req_d.size      = SizeQWord;
      s4_out_msg_sc_req_d.qwen      = 4'b0001;
      s4_out_msg_sc_req_d.opcode    = ReqMsgSendData;
      s4_out_msg_sc_req_d.subopcode = '0;
      s4_out_msg_sc_req_d.id        = IdSize'(s4_msg_send_hart_id_q) + IdSize'(8'h80);
      s4_out_msg_sc_req_d.address   = s4_out_msg_req_pa;
      // RTLMIN-4771: Remove HART ID for debug in [63:32]
      s4_out_msg_sc_req_d.data      = 512'(64'(s4_hart_sent_pckts_q[s4_msg_send_hart_id_q])) | (512'(64'(s4_msg_send_hart_id_q)) << 32);

      s4_out_msg_sc_req_valid_d[s4_out_msg_req_pa[BankLsb +: BankIdSize]] = 1'b1;

      s4_out_msg_sc_req_valid_en = 1'b1;
      s4_out_msg_sc_req_en       = 1'b1;
    end

    s4_out_msg_sc_req_valid_d = !rst_ni ? '0 : s4_out_msg_sc_req_valid_d;

    s4_hart_msg_sent = (|s4_out_msg_sc_req_valid_q) && (|s4_out_msg_sc_req_ready);
  end

  // S4: Output packet/message flops
  always_ff @(posedge clk_rbox_i) begin
    if (s4_pending_quad_pcks_en)
      s4_pending_quad_pcks_q <= s4_pending_quad_pcks_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_out_pck_sc_req_en)
      s4_out_pck_sc_req_q <= s4_out_pck_sc_req_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_out_pck_sc_req_valid_en)
      s4_out_pck_sc_req_valid_q <= s4_out_pck_sc_req_valid_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_out_msg_sc_req_en)
      s4_out_msg_sc_req_q <= s4_out_msg_sc_req_d;
  end

  always_ff @(posedge clk_rbox_i) begin
    if (s4_out_msg_sc_req_valid_en)
      s4_out_msg_sc_req_valid_q <= s4_out_msg_sc_req_valid_d;
  end

  // ════════════════════════════════════════════════════════════
  // Output to Traverse and Test
  // ════════════════════════════════════════════════════════════

  always_comb begin
    tr_triangle_o     = s3_triangle_state_q;
    tr_tile_o         = s3_tile_state_q;
    tr_tile_valid_o   = s3_tile_valid_q;

    tr_msaa_enable_o        = s3_rbox_state_q.msaa_enable && msaa_mode_is_valid(s3_rbox_state_q.msaa_samples);
    tr_msaa_mode_o          = convert_msaa_mode(s3_rbox_state_q.msaa_samples);
    tr_depth_is_fp32_o      =  (s3_rbox_state_q.ds_buffer_format == DsFormatD32Sfloat);
    tr_depth_is_un16_o      =  (s3_rbox_state_q.ds_buffer_format == DsFormatD16Unorm);
    tr_depth_is_un24_o      =  (s3_rbox_state_q.ds_buffer_format == DsFormatX8D24UnormPack32)
                            || (s3_rbox_state_q.ds_buffer_format == DsFormatD24UnormS8Uint);
    tr_scissor_min_x_o      = s3_scissor_min_x_q;
    tr_scissor_max_x_o      = s3_scissor_max_x_q;
    tr_scissor_min_y_o      = s3_scissor_min_y_q;
    tr_scissor_max_y_o      = s3_scissor_max_y_q;
    tr_depth_clamp_enable_o = s3_rbox_state_q.depth_clamp_enable;
    tr_depth_min_o          = s3_rbox_state_q.depth_min;
    tr_depth_max_o          = s3_rbox_state_q.depth_max;
    tr_depth_bound_enable_o = s3_rbox_state_q.depth_bound_enable;
    tr_depth_bound_min_o    = s3_rbox_state_q.depth_bound_min;
    tr_depth_bound_max_o    = s3_rbox_state_q.depth_bound_max;

    ts_state_o          = s3_rbox_state_q;
    ts_front_triangle_o = (s3_triangle_state_q.tri_facing == TriFacingFront);
  end

  // ════════════════════════════════════════════════════════════
  // Status Monitor
  // ════════════════════════════════════════════════════════════

  always_comb begin
    ct_sm_data_o.s1_config_error             = s1_config_error;
    ct_sm_data_o.s1_started                  = s1_started_q;
    ct_sm_data_o.s1_done                     = s1_done_q;
    ct_sm_data_o.s1_sc_rbox_rsp_is_read_data = s1_sc_rbox_rsp_is_read_data;
    ct_sm_data_o.s1_in_pending_lines_ff      = s1_in_pending_lines_q;
    ct_sm_data_o.s1_in_process_pckts_ff      = s1_in_process_pckts_q;
    ct_sm_data_o.s1_in_buf_off               = s1_in_buf_off_q;
    ct_sm_data_o.s1_in_sc_req_valid_ff       = s1_in_sc_req_valid_q;
    ct_sm_data_o.s1_req_pa                   = s1_req_pa;
    ct_sm_data_o.s1_in_fifo_full             = s1_in_fifo_full;
    ct_sm_data_o.s1_in_fifo_empty            = s1_in_fifo_empty;
    ct_sm_data_o.s1_in_fifo_alloc            = s1_in_fifo_alloc;
    ct_sm_data_o.s1_in_fifo_alloc_ptr        = s1_in_fifo_alloc_ptr;
    ct_sm_data_o.s1_in_fifo_push_ptr         = s1_in_fifo_push_ptr;
    ct_sm_data_o.s1_in_fifo_push             = s1_in_fifo_push;
    ct_sm_data_o.s1_in_fifo_push_done        = s1_in_fifo_push_done;

    ct_sm_data_o.s2_load_rbox_state          = s2_load_rbox_state;
    ct_sm_data_o.s2_load_fsh_state           = s2_load_fsh_state;
    ct_sm_data_o.s2_load_triangle_state      = s2_load_triangle_state;
    ct_sm_data_o.s2_load_tile_state          = s2_load_tile_state;
    ct_sm_data_o.s2_input_pck_accept         = s2_input_pck_accept;

    ct_sm_data_o.s3_end_of_buffer_ff         = s3_end_of_buffer_q;
    ct_sm_data_o.s3_tile_valid_ff            = s3_tile_valid_q;

    ct_sm_data_o.s4_out_buf_info_valid       = s4_out_buf_info_valid;
    ct_sm_data_o.s4_out_buf_packet_valid     = s4_out_buf_packet_valid;
    ct_sm_data_o.s4_out_buf_info             = s4_out_buf_info;
    ct_sm_data_o.s4_hart_id                  = s4_hart_id;
    ct_sm_data_o.s4_out_msg_sc_req_valid_ff  = s4_out_msg_sc_req_valid_q;
    ct_sm_data_o.s4_out_pck_sc_req_valid_ff  = s4_out_pck_sc_req_valid_q;
    ct_sm_data_o.s4_out_msg_req_pa           = s4_out_msg_req_pa;
    ct_sm_data_o.s4_out_pck_req_pa           = s4_out_pck_req_pa;
  end

  // ════════════════════════════════════════════════════════════
  // Assertions
  // ════════════════════════════════════════════════════════════

  // synthesis translate_off
  `ifdef VERILATOR
    /* verilator lint_off SYNCASYNCNET */
    always_ff @(posedge clk_rbox_i) begin
      if (rst_ni && !$onehot0(rbox_sc_req_valid_o))
        $error("rbox_ctrl: multi-bit valid/ready handshake should only send 1-hot ready. rbox_sc_req_valid_o = %b",
               rbox_sc_req_valid_o);
    end
    /* verilator lint_on SYNCASYNCNET */
  `else
    assert property (@(posedge clk_rbox_i) disable iff (!rst_ni)
      $onehot0(rbox_sc_req_valid_o))
    else $error("rbox_ctrl: multi-bit valid/ready handshake should only send 1-hot ready. rbox_sc_req_valid_o = %b",
                $sampled(rbox_sc_req_valid_o));
  `endif
  // synthesis translate_on

/* verilator lint_on UNOPTFLAT */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on UNUSEDSIGNAL */

endmodule
