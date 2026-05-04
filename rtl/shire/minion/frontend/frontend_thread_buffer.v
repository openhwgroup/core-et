// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module frontend_thread_buffer #(
  parameter THREAD_ID = 0
)
(
  // System signals
  input logic                                     clock_aon,
  input logic                                     reset,
  input logic                                     reset_debug,
  input logic                                     chicken_bit,
  // Thread enable and reset PC
  input logic                                     f0_thread_enabled,
  input logic [`VA_RANGE]                         f0_reset_vector,
  input logic                                     f0_disable_thread,    // Thread is disabled this cycle
  input logic                                     f0_enable_thread,    // Thread is enabled this cycle
  input logic                                     in_debug_mode,
  // TLB control
  input  minion_vm_status                         vm_status,
  // Icache requests
  input logic                                     f0_icache_req_ready,
  input logic                                     f1_icache_req_ready,
  output logic                                    f0_icache_req_valid,
  output frontend_icache_req                      f0_icache_req,
  // Icache response
  input logic                                     f5_icache_resp_miss,
  input logic                                     f6_icache_resp_valid,
  input logic                                     f6_icache_resp_miss,
  input icache_frontend_resp                      f6_icache_resp,
  input logic                                     f7_icache_fill_done,

  // Mispredict from core
  input logic                                     f0_core_req_valid,
  input minion_fe_req                             f0_core_req,
  // Fetch resp
  input logic                                     f7_inst_ready,
  output logic                                    f7_inst_rvc,
  output logic                                    f7_inst_valid,
  output frontend_operation                       f7_inst_data,
  // interface to write program buffer for debug
  input logic [`MINION_DEBUG_APB_D_WIDTH-1:0]     debug_ffb_wdata,
  input logic [`MINION_DEBUG_FFB_WORDS-1:0]       debug_ffb_en,
  input logic                                     debug_ffb_exec,
  input logic                                     io_halt

);

////////////////////////////////////////////////////////////////////////////////
// This module takes care of fetching the instructions from the icache for one
// thread and it sends the valid instructions in program order to the core.
// There are FE_FETCH_BUFFERS (currently 2) buffers that store
// FE_FETCH_VALID_INST_SIZE instructions (currently 8). When the thread is
// active and one of the buffers has no pending instructions to be sent, the
// thread requests the next fetch PC block to the Icache.
// When the request is accepted by the Icache (it is shared, so this module
// might lose arbitration or it might be busy dealing with misses) the thread
// buffer waits for the data to come back from Icache. When it comes back it
// also sends which of the instructions should be sent to the core.
// In parallel, if there are instructions available to be issued, the module
// tries to send instructions in program order to the core. Again, the
// instruction might not be accepted due the core being busy or other thread
// being selected.
// In case of a mispredict from core, all the buffers are cleared and if
// there's any inflight request to the icache it is killed, and the new PC is
// requested to the icache.
////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // signals
  ////////////////////////////////////////////////////////////////////////////////

  // buffer status
  logic [`FE_FETCH_BUFFERS-1:0]                buffer_empty;        // Available buffer
  logic [`FE_FETCH_BUFFERS-1:0]                buffer_empty_next;
  logic [`FE_FETCH_BUFFERS-1:0][`VA_RANGE_EXT] buffer_pc;                // PC for each instruction buffer
  logic [`FE_FETCH_BUFFERS-1:0]                buffer_page_fault;        // Fetched block had a page fault exception
  logic [`FE_FETCH_BUFFERS-1:0]                buffer_access_fault;      // Fetched block had an access fault exception
  logic [`FE_FETCH_BUFFERS-1:0]                buffer_cacheable;         // Fetched block is cacheable
  logic [`FE_FETCH_BUFFERS-1:0]                buffer_ecc_err;
  logic [`FE_FETCH_BUFFERS-1:0]                buffer_l2_err;
  logic [`FE_FETCH_BUFFERS-1:0][`FE_FETCH_READ_SIZE/32-1:0] buffer_word_en;
  // F0
  logic                         f0_new_inst_req;
  logic [`VA_RANGE_EXT]         f0_pc, f0_pc_next;    // This is the PC of the next instruction to be fetched
  logic [`FE_FETCH_BUFFER_ID]   f0_req_buffer;        // ID of the request buffer to icache
  logic [`FE_FETCH_BUFFERS-1:0] f0_buffer_pc_en;      // Update PC for a buffer when a new fetch req is sent
  logic                         f0_icache_req_acc;    // Request to Icache was accepted
  logic                         f0_pc_update;         // Enable the clock for the PC
  logic                         f0_miss_pending;      // Miss pending, wait for fill to be done
  logic                         f0_miss_pending_next; // Miss pending, wait for fill to be done

  // F1
  logic [`FE_FETCH_BUFFER_ID]                 f1_req_buffer;         // ID of the request buffer to icache
  logic                                       f0_icache_req_kill_f1;
  logic                                       f1_icache_req_kill_f1; // Kill F1 flopped while request is not accepted
  logic                                       f1_req_valid_to_f2;    // Valid request in Icache goint to F2
  logic                                       f1_req_valid;         // Valid request in Icache in F1 stage

  // F2
  logic [`FE_FETCH_BUFFER_ID]                 f2_req_buffer;      // ID of the request buffer to icache
  logic                                       f2_req_valid_to_f3; // Valid request in Icache goint to F3
  logic                                       f2_req_valid;       // Valid request in Icache in F2 stage

  // F3
  logic [`FE_FETCH_BUFFER_ID]                 f3_req_buffer;      // ID of the request buffer to icache
  logic                                       f3_req_valid_to_f4; // Valid request in Icache goint to F4
  logic                                       f3_req_valid;       // Valid request in Icache in F3 stage

  // F4
  logic [`FE_FETCH_BUFFER_ID]                 f4_req_buffer;      // ID of the request buffer to icache
  logic                                       f4_req_valid_to_f5; // Valid request in Icache goint to F5
  logic                                       f4_req_valid;       // Valid request in Icache in F4 stage

  // F5
  logic [`FE_FETCH_BUFFER_ID]                 f5_req_buffer;      // ID of the request buffer to icache
  logic                                       f5_req_valid_to_f6; // Valid request in Icache goint to F6
  logic                                       f5_req_valid;       // Valid request in Icache in F5 stage

  // F6 write
  logic [`FE_FETCH_BUFFERS-1:0]               f6_buffer_en;  // Enable the clock for a buffer to write data
  logic [`FE_FETCH_BUFFERS-1:0]               f6_fault_en;   // Enable the clock for a fetch access/page fault
  logic [`FE_FETCH_BUFFER_ID]                 f6_req_buffer; // ID of the request buffer to icache
  logic [`FE_FETCH_READ_SIZE/32-1:0]          f6_buffer_wr_1p;      // version of f6_buffer_wr from 1/2 cycle before (without qualifies)
  logic [`FE_FETCH_READ_SIZE/32-1:0]          f6_buffer_wr_1p_next;
  logic                                       f6_req_valid;         // Valid request in Icache in F6 stage
  logic                                       f6_req_resp_valid;    // Valid response for a request in Icahce in F6 stage
  logic                                       f6_buffer_wr;         // Doing a write to buffers after qualifies
  logic [`FE_FETCH_READ_SIZE-1:0]             f6_wr_data;
  logic [`FE_FETCH_READ_SIZE/32-1:0]          f6_buffer_wren;
  logic [`FE_FETCH_BUFFER_ID]                 f6_write_ad;
  logic [`FE_FETCH_BUFFERS-1:0]               f6_clear_buffer;

  // F6 read
  logic                                       f6_valid;
  logic                                       f6_stall;
  logic                                       f6_split;
  logic [31:0]                                f6_buffer_inst_data;
  logic [31:0]                                f6_inst_bits;
  logic [31:0]                                f6_inst_bits_expanded;
  logic [`FE_FETCH_PTR_SIZE-1:0]              f6_buffer_ptr;
  logic [`FE_FETCH_PTR_SIZE-1:0]              f6_buffer_ptr_next;
  logic [`FE_BUFFER_POS_SZ-1:0]               f6_buffer_pos;
  logic [`FE_FETCH_BUFFER_ID]                 f6_buffer_id;
  logic [`FE_FETCH_BUFFER_ID]                 f6_buffer_id_next;
  logic                                       f6_last_from_buffer;
  logic                                       f6_inst_rvc;
  logic [`VA_SIZE_EXT-`FE_BUFFER_POS_SZ-2:0]  f6_inst_pc_msb;


  // F7
  logic                                       f7_valid;
  logic                                       f7_stall;
  logic [`FE_FETCH_PTR_SIZE-1:0]              f7_buffer_ptr;
  logic [`FE_BUFFER_POS_SZ-1:0]               f7_buffer_pos;
  logic [`FE_FETCH_BUFFER_ID]                 f7_buffer_id;
  logic [31:0]                                f7_inst_bits;
  logic                                       f7_split;
  logic [`VA_SIZE_EXT-`FE_BUFFER_POS_SZ-2:0]  f7_inst_pc_msb;


  // DEBUG
  logic                                       using_pfb;            // contents are not regular instructions, using pfb for debug
  logic                                       using_pfb_reg;
  logic                                       using_pfb_next;
  logic [`MINION_DEBUG_FFB_WORDS-1:0]         debug_ffb_en_2;
  logic                                       debug_ffb_exec_2;
  logic                                       debug_mode_start;
  logic                                       debug_mode_start_ff;
  logic                                       debug_mode_end;
  logic                                       dbg_clear_f7;
  logic                                       reset_prog_buffer;
  logic                                       reset_debug_ff;

  // gated clock
  logic                                       clock_enable;
  logic                                       clock;

  ////////////////////////////////////////////////////////////////////////////////
  // latches with the buffers
  ////////////////////////////////////////////////////////////////////////////////
  rf_latch_1r_1w_diff_widths #(
    .R_WIDTH(32),
    .R_ALIGNMENT(16),
    .W_WIDTH(`FE_FETCH_READ_SIZE),
    .ENTRIES(`FE_FETCH_BUFFERS),
    .LEVEL2_CLK_GATE(1)
  ) inst_data (
    .clk(clock_aon),
    .test_en(1'b0),
    .rd_data_a(f6_buffer_inst_data), // Read Port A
    .rd_addr_a(f6_buffer_ptr), // Read Address for port A
    .wr_data_a(f6_wr_data),
    .wr_data_a_en_1p(f6_buffer_wr_1p),
    .wr_addr_a(f6_write_ad),
    .wr_en_a(f6_buffer_wren)
  );

  `LATCH_P2(clock, f6_buffer_wr_1p,  f6_buffer_wr_1p_next)

  logic ffb_ff_en_2;
  // because they are typically a pulse... enable when either D or Q are 1
  assign ffb_ff_en_2 = (|debug_ffb_en_2) | (|debug_ffb_en) | debug_ffb_exec_2 | debug_ffb_exec;
  `RST_EN_FF(clock, reset_debug, ffb_ff_en_2, debug_ffb_en_2, debug_ffb_en, '0)
  `RST_EN_FF(clock, reset_debug, ffb_ff_en_2, debug_ffb_exec_2, debug_ffb_exec, 1'b0)

  // mux wr data between icache and debug, and word wr enable
  always_comb begin
    for ( int i = 0; i <`FE_FETCH_BUFFERS; i++)
      buffer_word_en[i] = { (`FE_FETCH_READ_SIZE/32) {1'b1} }
          << buffer_pc[i][2+:$clog2(`FE_FETCH_READ_SIZE/32)];

    f6_buffer_wr_1p_next = f5_req_valid_to_f6 && !f5_icache_resp_miss
        ? buffer_word_en[f5_req_buffer] : '0;

    if (using_pfb) begin
      f6_buffer_wr_1p_next = (debug_mode_start || reset_debug) ? 8'hff : `ZX(`FE_FETCH_READ_SIZE/32, debug_ffb_en);
      f6_buffer_wren = `ZX(`FE_FETCH_READ_SIZE/32, debug_ffb_en_2);
      f6_write_ad = f6_req_buffer;
      if (in_debug_mode || reset_prog_buffer)
        f6_write_ad = '0;
      // When we trap to debug mode, fill the buffer with ebreaks so the break is implicit
      if (reset_prog_buffer)
        f6_buffer_wren = 8'hff;
    end
    else begin
      f6_buffer_wren = f6_buffer_wr? buffer_word_en[f6_req_buffer] : '0;
      f6_write_ad = f6_req_buffer;
    end

    f6_wr_data =  f6_icache_resp.data;
    // On debug mode, store APB data in to the buffer
    if ( using_pfb ) begin
      f6_wr_data[(64*0)+:64] = debug_ffb_wdata;
      f6_wr_data[(64*1)+:32] = debug_ffb_wdata[31:0];
      f6_wr_data[(64*1+32)+:32] = debug_ffb_wdata[31:0];
      if (reset_prog_buffer) begin
        for(int i = 0; i < 5; ++i)
          f6_wr_data[32*i+:32] = `EBREAK;
        for(int i = 5; i < (`FE_FETCH_READ_SIZE/32); ++i)
          f6_wr_data[32*i+:2] = 2'b11; // Reset the lsb to avoid X propagation when checking for  compresed instructions
      end
    end

  end

////////////////////////////////////////////////////////////////////////////////
// Controls for clock gating
////////////////////////////////////////////////////////////////////////////////
  logic icache_inflight;
  logic icache_inflight_next;

  always_comb begin
    icache_inflight_next = icache_inflight;
    if (f0_icache_req_valid) icache_inflight_next = 1'b1;
    else if (f0_core_req_valid || f0_icache_req_kill_f1 || f6_req_valid)
      icache_inflight_next = 1'b0;


    clock_enable = ( f0_core_req_valid       ||                                // request from core
                     f0_enable_thread        ||                                // Thread is being enabled
                     f0_icache_req_acc       ||                                // Sending a request to icache
                     icache_inflight         ||                                // pending req to the icache
                     f0_thread_enabled && !f0_miss_pending && |buffer_empty || //buffers not full => there might be new request to icache
                     f6_valid && !f6_stall   ||                                // pipe advances
                     f7_valid && !f7_stall   ||                                // pipe advances
                     reset || reset_debug    ||                                // not resetting
                     in_debug_mode           ||                                // in debug mode, always on
                     io_halt                 ||                                // must send valid rsp to intpipe to trigger a halt interrupt
                     chicken_bit);
  end
  `RST_FF(clock_aon, reset, icache_inflight, icache_inflight_next, 1'b0)
  et_clk_gate ck_gate(.enclk(clock), .en(clock_enable), .clk(clock_aon), .te(1'b0));


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F0 Stage
////////////////////////////////////////////////////////////////////////////////
  //         CLK        RST      EN                 DOUT             DIN                   DEF
  `EN_FF     (clock,             f0_pc_update,      f0_pc,           f0_pc_next)
  `RST_FF    (clock_aon, reset,                     f0_miss_pending, f0_miss_pending_next, 1'b0)

  always_comb begin

    // Sends valid request when
    f0_new_inst_req =  f0_thread_enabled // Thread is enabled
                        && !f0_enable_thread  // Do not request during the cycle the thread is enabled, as reset PC hasn't been flopped yet
                        && !(f0_miss_pending && !f7_icache_fill_done )  // There's no miss pending to be dealt with

                        && !f0_core_req_valid // Core is not having a mispredict
                        && !f1_req_valid      // No request in-flight already
                        && !f2_req_valid
                        && !f3_req_valid
                        && !f4_req_valid
                        && !f5_req_valid
                        && !f6_req_valid
                        && ( (|buffer_empty) || (|f6_clear_buffer) ) // Buffer available
                        && !reset              // Wait until we are out of reset
                        && !using_pfb;         // buffer contents are the 'program fetch buffer' for debug

    f0_icache_req_valid = f0_new_inst_req && !io_halt; //don't send requests to the ICACHE if we are halting

    // Sends the request to the Icache
    f0_icache_req.thread_id = THREAD_ID[`CORE_NR_THREADS_R];
    f0_icache_req.vm_status = vm_status;
    f0_icache_req.addr      = f0_pc;

    // Selects which is the request buffer that will be used to store the
    // insts
    f0_req_buffer = '0;
    for(integer unsigned i = 0; i < `FE_FETCH_BUFFERS; i++)
      if(buffer_empty[i] ||                       // buffer is empty
         f6_clear_buffer[i] && !f6_buffer_en[i])  // buffer is about to be empty
                                                  // the !f6_buffer_en is the case when icache bypass and
                                                  //  reading the last instruction from the 256 chunk
        f0_req_buffer = i;

    // Generates the next PC
    f0_pc_next = `SX(`PC_SIZE_EXT, f0_reset_vector);
    // Update to the PC when data coming back from icache
    if(f6_buffer_wr) begin
      f0_pc_next = '0;
      f0_pc_next[`VA_EXT_MSB:`FE_FETCH_READ_LSB] = buffer_pc[f6_req_buffer][`VA_EXT_MSB:`FE_FETCH_READ_LSB] + 1'b1;
    end
    // If core has a mispredict, get this PC (highest prio)
    if(f0_core_req_valid)
      f0_pc_next = f0_core_req.pc;

    // Update PC when
    f0_pc_update = reset              // Doing reset
                   || f0_enable_thread   // Thread is being enabled
                   || f6_buffer_wr       // New data coming (PC of next block to fetch comes from I$)
                   || f0_core_req_valid; // Mispredict

    // Updates the miss pending
    f0_miss_pending_next = f0_miss_pending;
    // Received a miss of a non-killed request, then set the bit
    if(f6_req_valid && f6_icache_resp_miss && !f0_core_req_valid)
      f0_miss_pending_next = 1'b1;
    // Upon mispredict, thread disable or fill done, clear the pending miss
    // bit
    if(f0_core_req_valid || f0_disable_thread || f7_icache_fill_done || io_halt)
      f0_miss_pending_next = 1'b0;
  end

  always_comb begin
    // Request is accepted
    f0_icache_req_acc = (f0_icache_req_valid && f0_icache_req_ready) || (f0_new_inst_req && io_halt);

    // Update PC for buffer
    for(integer i = 0; i < `FE_FETCH_BUFFERS; i++)
      f0_buffer_pc_en[i] = f0_icache_req_acc && (i == f0_req_buffer);
  end

////////////////////////////////////////////////////////////////////////////////
// F1 Stage
////////////////////////////////////////////////////////////////////////////////


  //         CLK    RST    EN                   DOUT                   DIN                DEF
  `RST_EN_FF(clock, reset, f1_icache_req_ready, f1_req_valid,          f0_icache_req_acc, 1'b0)
  `EN_FF    (clock,        f1_icache_req_ready, f1_req_buffer,         f0_req_buffer)
  `FF       (clock,                             f1_icache_req_kill_f1, f1_icache_req_ready ? 1'b0 : f0_icache_req_kill_f1)

  always_comb begin
    f0_icache_req_kill_f1 = (f1_req_valid && f0_core_req_valid) || f1_icache_req_kill_f1;
    f1_req_valid_to_f2    = f1_req_valid && f1_icache_req_ready && !f0_icache_req_kill_f1;
  end

////////////////////////////////////////////////////////////////////////////////
// F2 Stage
////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    DOUT           DIN                DEF
  `RST_FF(clock, reset, f2_req_valid,  f1_req_valid_to_f2, 1'b0)
  `FF    (clock,        f2_req_buffer, f1_req_buffer)

  assign f2_req_valid_to_f3 = f2_req_valid && !f0_core_req_valid;

////////////////////////////////////////////////////////////////////////////////
// F3 Stage
////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    DOUT           DIN                DEF
  `RST_FF(clock, reset, f3_req_valid,  f2_req_valid_to_f3, 1'b0)
  `FF    (clock,        f3_req_buffer, f2_req_buffer)

  assign f3_req_valid_to_f4 = f3_req_valid && !f0_core_req_valid;

////////////////////////////////////////////////////////////////////////////////
// F4 Stage
////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    DOUT           DIN                DEF
  `RST_FF(clock, reset, f4_req_valid,  f3_req_valid_to_f4, 1'b0)
  `FF    (clock,        f4_req_buffer, f3_req_buffer)

  assign f4_req_valid_to_f5 = f4_req_valid && !f0_core_req_valid;

////////////////////////////////////////////////////////////////////////////////
// F5 Stage
////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    DOUT           DIN                DEF
  `RST_FF(clock, reset, f5_req_valid,  f4_req_valid_to_f5, 1'b0)
  `FF    (clock,        f5_req_buffer, f4_req_buffer)

  assign f5_req_valid_to_f6 = f5_req_valid && !f0_core_req_valid;

////////////////////////////////////////////////////////////////////////////////
// F6 Stage - writing
////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    DOUT           DIN                 DEF
  `RST_FF(clock, reset, f6_req_valid,  f5_req_valid_to_f6, 1'b0)
  `FF    (clock,        f6_req_buffer, f5_req_buffer)

  always_comb begin
    f6_buffer_wr      = f6_req_valid && !f0_core_req_valid &&
        ((f6_icache_resp_valid && !f6_icache_resp_miss) || io_halt);
    f6_req_resp_valid = f6_req_valid && !f0_core_req_valid && ((f6_icache_resp_valid) || io_halt);

    // Generates the enable
    for(integer i = 0; i < `FE_FETCH_BUFFERS; i++) begin
      f6_buffer_en[i] = f6_buffer_wr && (i == f6_req_buffer);
      f6_fault_en[i]  = f6_req_resp_valid && (i == f6_req_buffer);
    end
  end

  for( genvar buffer = 0; buffer < `FE_FETCH_BUFFERS; buffer++) begin
    //     CLK    EN                       DOUT                            DIN
    `EN_FF(clock, f6_fault_en[buffer],     buffer_page_fault[buffer],   f6_icache_resp.page_fault && !io_halt)
    `EN_FF(clock, f6_fault_en[buffer],     buffer_access_fault[buffer], f6_icache_resp.access_fault && !io_halt)
    `EN_FF(clock, f6_fault_en[buffer],     buffer_l2_err[buffer],       f6_icache_resp.bus_err && !io_halt)
    `EN_FF(clock, f6_fault_en[buffer],     buffer_ecc_err[buffer],      f6_icache_resp.ecc_err && !io_halt)
    `EN_FF(clock, f6_buffer_en[buffer],    buffer_cacheable[buffer],    f6_icache_resp.cacheable)
    `EN_FF(clock, f0_buffer_pc_en[buffer], buffer_pc[buffer],           f0_pc)
  end

////////////////////////////////////////////////////////////////////////////////
// Computes the buffer available bit based on fetches coming from icache and
// instruction consumption from core
////////////////////////////////////////////////////////////////////////////////
  `RST_FF(clock_aon, reset,  buffer_empty,     buffer_empty_next,   {`FE_FETCH_BUFFERS{1'b1}})

  always_comb begin
    // Computes next valid bit
    buffer_empty_next = buffer_empty;

    for(integer i = 0; i < `FE_FETCH_BUFFERS; i++) begin
      f6_clear_buffer[i] = f6_last_from_buffer && f6_valid && !f6_stall && f6_buffer_id == i;

      // If new data coming update the avilable to the ones coming from
      // ICache
      // If last instruction, make buffer available again
      if(f6_clear_buffer[i])
        buffer_empty_next[i] = 1'b1;
      // writing new data => buffer not empty
      else if(f6_buffer_en[i])
        buffer_empty_next[i] = 1'b0;
      // there was a fault.. not writing data to buffer, but set not empty so that the fault is sent to the intpipe
      else if (f6_fault_en[i] && (f6_icache_resp.page_fault || f6_icache_resp.access_fault || f6_icache_resp.ecc_err || f6_icache_resp.bus_err))
        buffer_empty_next[i] = 1'b0;
      // Mispredict or disabling thread, makes all buffers available again
      else if(f0_core_req_valid || f0_disable_thread)
        buffer_empty_next[i] = 1'b1;
    end // for (integer i = 0; i < `FE_FETCH_BUFFERS; i++)


    // When entering debug-mode, set program buffer to empty
    dbg_clear_f7 = 1'b0;
    if(debug_mode_start_ff) begin
      buffer_empty_next = 2'b11;
      dbg_clear_f7 = 1'b1;
    end

    // if writing program fetch buffer, setting empty
    if (debug_ffb_exec_2)
      buffer_empty_next = 2'b10;
  end // always_comb


////////////////////////////////////////////////////////////////////////////////
// F6 Stage - reading
////////////////////////////////////////////////////////////////////////////////
  logic ffb_update_read_pointer;

  //         CLK | RST |  EN                                                                                | REG           |  NEXT                                             | RST_VALUE
  `RST_EN_FF(clock, reset, f6_valid && !f6_stall || &buffer_empty && f5_req_valid || ffb_update_read_pointer,  f6_buffer_ptr, (ffb_update_read_pointer) ? '0 : f6_buffer_ptr_next, '0)

  assign ffb_update_read_pointer = debug_ffb_exec;


  always_comb begin
    f6_inst_bits = f6_buffer_inst_data;

    {f6_buffer_id, f6_buffer_pos } = f6_buffer_ptr;

    if (&buffer_empty && f5_req_valid) begin
      f6_buffer_ptr_next  = {f5_req_buffer, buffer_pc[f5_req_buffer][`FE_BUFFER_POS_SZ:1]};
    end
    else begin
      f6_buffer_ptr_next = f6_buffer_ptr + (f6_inst_rvc ? 2'b01 : 2'b10);
    end
    f6_buffer_id_next = f6_buffer_ptr_next[`FE_FETCH_PTR_SIZE-1:`FE_BUFFER_POS_SZ];

    // is data from 2 buffers required? => 32b instruction, at read_pos=all 1's
    f6_split = !f6_inst_rvc && &f6_buffer_pos;

    // determine valid
    if ( !f6_split) begin
      f6_valid = !buffer_empty[f6_buffer_id];
      // maybe bypass data directly from icache
      if ( ! f6_valid && f6_buffer_wr) begin
        f6_valid = 1'b1;
        f6_inst_bits = f6_icache_resp.data[f6_buffer_pos*16 +: 32];
      end
    end else begin
      f6_valid = !buffer_empty[f6_buffer_id] && !buffer_empty[f6_buffer_id_next];
      // maybe bypass data directly from icache
      if ( !f6_valid &&  !buffer_empty[f6_buffer_id] && buffer_empty[f6_buffer_id_next] && f6_buffer_wr) begin
        f6_valid = 1'b1;
        f6_inst_bits[31:16] = f6_icache_resp.data[15:0];
      end
    end // else: !if( ! f6_split)
    f6_valid = f6_valid && !dbg_clear_f7;

    f6_last_from_buffer = ( f6_buffer_id_next !=  f6_buffer_id );

    f6_inst_pc_msb = buffer_pc[f6_buffer_id][`VA_SIZE_EXT-1:`FE_FETCH_READ_LSB];
  end

  // instruction is compressed => take either from buffer, or from icache data if bypassing
  // in case of halt, force it to a non rvc instruction
  assign f6_inst_rvc =  (!io_halt && (!buffer_empty[f6_buffer_id] ? f6_buffer_inst_data[1:0] : f6_icache_resp.data[f6_buffer_pos*16 +: 2]) != 2'b11);


  // RVC expander
  frontend_rvc_expander rvc_expander (
    // Input data
    .in_bits    ( f6_inst_bits ),
    // Expanded output data
    .out_bits   ( f6_inst_bits_expanded )
  );

////////////////////////////////////////////////////////////////////////////////
// F7 Stage
////////////////////////////////////////////////////////////////////////////////

  `RST_FF(clock, reset, f7_valid, (f0_core_req_valid || dbg_clear_f7) ? 1'b0 : f6_valid? 1'b1 : f7_inst_ready? 1'b0 : f7_valid, 1'b0)
  `EN_FF(clock, f6_valid && !f6_stall, f7_inst_bits, f6_inst_bits_expanded)
  `EN_FF(clock, f6_valid && !f6_stall, f7_inst_rvc, f6_inst_rvc)
  `EN_FF(clock, f6_valid && !f6_stall, f7_buffer_ptr, f6_buffer_ptr)
  `EN_FF(clock, f6_valid && !f6_stall, f7_split, f6_split)
  `EN_FF(clock, f6_valid && !f6_stall, f7_inst_pc_msb, f6_inst_pc_msb)

  always_comb begin
    f7_stall = !f7_inst_ready;
    f6_stall = f7_valid && f7_stall;

    // fill structure to next stage
    {f7_buffer_id, f7_buffer_pos} = f7_buffer_ptr;

    f7_inst_data.pc = { f7_inst_pc_msb, f7_buffer_pos, 1'b0};

    // Exception from TLB and replay
    f7_inst_data.page_fault0   = buffer_page_fault[f7_buffer_id] && !using_pfb_reg;
    f7_inst_data.access_fault0 = buffer_access_fault[f7_buffer_id] && !using_pfb_reg;
    // page faults in case instruction has data from 2 buffers
    if (!f7_inst_rvc && f7_split) begin
      f7_inst_data.page_fault1   = buffer_page_fault[f6_buffer_id] && !using_pfb_reg;
      f7_inst_data.access_fault1 = buffer_access_fault[f6_buffer_id] && !using_pfb_reg;
      // output errors => or of both buffers if instruction is half in each buffer
      f7_inst_data.bus_err = |buffer_l2_err && !using_pfb_reg;
      f7_inst_data.ecc_err = |buffer_ecc_err && !using_pfb_reg;
    end else begin
      f7_inst_data.page_fault1   = 1'b0;
      f7_inst_data.access_fault1 = 1'b0;
      // and output errors
      f7_inst_data.bus_err = buffer_l2_err[f7_buffer_id]  && !using_pfb_reg;
      f7_inst_data.ecc_err = buffer_ecc_err[f7_buffer_id] && !using_pfb_reg;
    end



    f7_inst_data.replay       = !buffer_cacheable[f7_buffer_id] && f0_core_req.speculative; // Replay if not cacheable and speculative instruction

    if ( using_pfb ) begin
      f7_inst_data.page_fault0 = '0;
      f7_inst_data.page_fault1 = '0;
      f7_inst_data.access_fault0 = '0;
      f7_inst_data.access_fault1 = '0;
      f7_inst_data.bus_err = '0;
      f7_inst_data.ecc_err = '0;
      f7_inst_data.replay = '0;
    end

    f7_inst_data.inst_bits = f7_inst_bits;

    f7_inst_valid = f7_valid && !dbg_clear_f7;

  end // always_comb


////////////////////////////////////////////////////////////////////////////////
// DEBUG control
////////////////////////////////////////////////////////////////////////////////
  logic using_pfb_en;

  `FF(clock, reset_debug_ff, reset_debug)
  `RST_EN_FF(clock, reset, using_pfb_en , using_pfb_reg, using_pfb_next, 1'b0)

  `FF(clock, debug_mode_start_ff, debug_mode_start)
  assign debug_mode_start = f0_core_req.debug_info.halt && f0_core_req_valid;
  assign debug_mode_end   = f0_core_req.debug_info.resume && f0_core_req_valid;

  always_comb begin
    // if writing program fetch buffer from APB, update read_id and pos_id (will be reset to 0)
    using_pfb_en = debug_mode_start || debug_mode_end;

    using_pfb_next = using_pfb_reg;
    if (debug_mode_start) begin
      using_pfb_next = 1'b1;
    end else if (debug_mode_end) begin
      using_pfb_next = 1'b0;
    end
  end

  assign using_pfb = using_pfb_reg || using_pfb_next;

  assign reset_prog_buffer = (in_debug_mode && reset_debug_ff) || debug_mode_start_ff;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Asserts
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// synopsys translate_off
// synopsys translate_on

endmodule
