// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_reduce
(
  // System signals
  input  logic                                     clock,
  input  logic                                     reset,
  input  logic [`MAX_MINION_ID_R]                  global_min_id,
  // Chicken bit clock gate enable
  input  logic                                     chicken_bit_dcache,
  // Request port for new reduce operation
  output logic                                     reduce_ready,
  input  logic                                     reduce_start,
  output logic                                     tensorstore_busy,
  input  logic                                     tensorstore_start,
  input  logic [`VA_RANGE]                         tensorstore_stride,
  input  reduce_tensorstore_control                reduce_tensorstore_ctrl,
  input  logic                                     thread_id,
  // Port to get messages from sender/receiver
  input  logic                                     l2_resp_valid,
  input  et_link_minion_rsp_info_t                 l2_resp,
  output logic                                     buffer_avail,
  // Request bus to perform a cache operation
  input  logic                                     re_l2_req_ready,
  output logic                                     re_l2_req_valid,
  output et_link_minion_evict_req_info_t           re_l2_req,
  // Port to do Tensor Stores
  input  logic                                     ts_l2_req_ready,
  output logic                                     ts_l2_req_valid,
  output et_link_minion_evict_req_info_t           ts_l2_req,
  // Indication that one operation is ongoing
  output logic                                     re_ts_l2_req_inflight,
  // Control to Dcache pipeline
  output logic                                     pipe_used,
  output logic                                     s1_inst_send_val,
  output logic                                     s2_inst_send_val,
  output logic                                     s3_inst_send_val,
  output logic                                     s1_inst_op_val,
  output logic                                     s2_inst_op_val,
  output logic                                     s3_inst_op_val,
  output logic                                     ts_scp_read,
  output logic [`DCACHE_WAY_RANGE]                 ts_scp_read_way,
  output logic [`DCACHE_LRAM_ADDR_RANGE]           ts_scp_read_addr,
  output logic                                     ts_col_size_is_128b,
  // Control to translate virtual addresses
  input  logic                                     s0_available,
  output logic                                     s0_addr_load,
  output logic [`VA_RANGE]                         s0_addr,
  output logic                                     s0_addr_msb_err,
  output logic                                     s0_thread_id,
  input  logic                                     s1_available,
  output logic                                     s1_addr_load,
  input  logic                                     s1_tlb_wait,
  input  logic                                     s1_tlb_fail,
  input  logic [`PA_RANGE]                         s2_addr,
  // Control of buffer array
  input  logic                                     ba_full,
  output logic                                     ba_alloc,
  output logic                                     ba_write_needed,
  input  logic                                     s2_valid,
  input  logic [`DCACHE_BUFFER_ID_RANGE]           s2_ba_written_entry,
  output logic [`DCACHE_BUFFER_ID_RANGE]           s3_ba_write_addr,
  output logic                                     wait_l2_to_ba_resp,
  output logic                                     ba_entries_required,
  // Buffer array read port
  input  logic                                     s1_valid,
  input  logic                                     ba_read_used,
  input  logic [`DCACHE_DATA_RANGE]                s1_ba_data,
  output logic                                     ba_read_en,
  output logic                                     ba_read_en_spec,
  output logic [`DCACHE_BUFFER_ID_RANGE]           ba_read_addr,
  // Scratchpad configuration bit
  input  logic                                     scp_ctrl_on,
  // Control bits to VPU to execute a reduce
  input  logic                                     vpu_reduce_wait,
  output dcache_vpu_reduce_ctrl                    vpu_ctrl,
  // Management of VM status
  input  minion_vm_status [`CORE_NR_THREADS-1:0]   vm_status,
  output minion_vm_status                          vm_status_ts,
  // For performance counters
  output logic                                     ts_start,
  output logic                                     ts_op,
  //  Indication from MH to avoid collisions using BA
  input  logic                                     mh_going_to_uc_store,
  // Error flags
  output logic [`DCACHE_ERR_FLAG_RANGE][`CORE_NR_THREADS-1:0] err_flags,
  // Bus error
  output logic [`CORE_NR_THREADS-1:0]              bus_err,
  // debug signals
  output logic [`DCACHE_DBG_CSR_TS_SIZE-1:0]       csr_debug_signals,
  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]     sm_match_debug_signals,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]    sm_filter_debug_signals,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]      sm_data_debug_signals
);

  ////////////////////////////////////////////////////////////////////////////////
  // Reduce
  // This instruction reduces the contents of the VPU RF between two minions.
  // The instruction can specify the starting RF entry, the number of RF
  // entries, the reduction operation (float/int, add/sub/max/min) and it also
  // specifies the pairing minion.
  // One minion behaves as sender and another as receiver. Once the receiver
  // minion receives the reduce instruction, it sends a message to the sender
  // minion saying that it is ready to accept packets.
  // The sender minion once sees that the other minion is ready to receive, it
  // will start sending the contents of the requested VPU RF lines.
  // The receiver will get the contents, write it to the buffer array, and then
  // ship it to the VPU which will do the reduce op and write the final results.
  ////////////////////////////////////////////////////////////////////////////////

  // -----------------------------------------------------------------------------

  // Request state
  typedef enum logic [3:0]
  {
    Reduce_Invalid,     // Unit is idle
    Reduce_Wait_Tensor, // Waits for TensorOp dependencies to be cleared
    Reduce_New_Req,     // Decoding a new request
    Reduce_Send_Ready,  // Send that unit is ready to receive data (receiver only)
    Reduce_Wait_Ready,  // Waiting for the ready message from receiver (sender only)
    Reduce_Send_Data,   // Send data to receiver (sender only)
    Reduce_Wait_Data,   // Waits for data from sender (receiver only)
    Reduce_Drain,       // Waits for instruction to drain
    Reduce_Store_Data   // Sends TensorStore data
  } reduce_op_state;

  // Address translation state
  typedef enum logic [2:0]
  {
    Atrans_Idle,       // Address translation engine is Idle
    Atrans_Loop,       // Loop base state to do multiple address translations
    Atrans_S0_req,     // Do request to S0 stage
    Atrans_S1_req,     // Do request to S1 stage
    Atrans_Wait,       // Wait for TLB
    Atrans_Flush,      // Flush available addresses before going to Fail state
    Atrans_Fail,       // Failure or exception in address translation
    Atrans_Save        // Save translated address
  } atrans_state_t;

  // Internal defines
  `define DCACHE_TS_COLS_BITS   3
  `define DCACHE_TS_ROWS_BITS   4
  `define DCACHE_RE_COUNT_BITS  8
  `define DCACHE_RE_ACTION_BITS 2

  // Reduce and Tensor Store internal control structures
  // -----------------------------------------------------------------------------
  typedef struct packed     // Start and end level for reduce auto pair
  {
    logic [4:0] padding1;
    logic [3:0] endl;
    logic [3:0] startl;
  } reduce_level_t;

  typedef struct packed     //  and columns for TensorStores
  {
    logic                            ov;   // Padding bit for union reg_count_t
    logic [`DCACHE_TS_ROWS_BITS-1:0] rows;
    logic [`DCACHE_TS_COLS_BITS-1:0] cols;
  } row_col_count_t;

  typedef union packed      // Shared partner/level registers
  {
    logic [`MAX_MINION_ID_R] partner_id;
    reduce_level_t           level;
  } reduce_pl_info_t;

  typedef union packed      // Shared action/cols registers
  {
    logic [`DCACHE_TS_COLS_BITS-1:0] action;
    logic [`DCACHE_TS_COLS_BITS-1:0] cols;
  } reduce_ac_info_t;

  typedef union packed      // Shared counters registers
  {
    logic [`DCACHE_RE_COUNT_BITS-1:0] cnt;
    row_col_count_t                   rc;
  } reg_count_t;

  typedef struct packed
  {
    reduce_pl_info_t pl_info;    // Mixed partner/level information for Reduce
    reduce_ac_info_t ac_info;    // Mixed action/columns information for Reduce/TensorLoad
    logic            reduce;     // Is a reduce
    logic [1:0]      coop;       // Cooperative bits for TensorStore
    logic            thread_id;  // Thread ID
  } reduce_control_info;


  ////////////////////////////////////////////////////////////////////////////////
  // Signals
  ////////////////////////////////////////////////////////////////////////////////

  reduce_control_info  req_info;        // Request unit is working on
  reduce_control_info  req_info_next;

  reduce_op_state      fsm_state;       // State of the FSM dealing with reduce operations
  reduce_op_state      fsm_state_next;

  logic [`VA_SIZE-1:4] addr;            // Current addr
  logic [`VA_SIZE-1:4] addr_next;       // Next addr
  logic                addr_msb_err;    // Address had an overflow error
  logic                addr_msb_err_next;
  logic [`VA_RANGE]    addr_full;       // Virtual Address in full format

  logic [`VA_SIZE-1:4] stride_addr;     // Stride for tensor store
  logic [`VA_SIZE-1:4] stride_addr_next;

  logic [1:0]          stride_entry;    // Stride for scratchpath entries
  logic [1:0]          stride_entry_next;

  reg_count_t          reg_count;       // Number of regs or columns/rows to send
  reg_count_t          reg_count_next;
  logic                new_req;         // New request is coming in
  logic                reg_count_up;    // Update the reg count
  logic                clk_en;          // Global clock enable for the unit
  logic                addr_up;         // Update the address
  logic                last_packet_row; // Doing the last packet of a row in a tensor store
  logic                stride_up;       // Update the stride

  logic [`DCACHE_TS_RATE_BITS-1:0]         rate;
  logic [`DCACHE_TS_RATE_BITS-1:0]         rate_next;

  logic [`DCACHE_TS_RATE_COUNTER_BITS-1:0] rate_counter;       // Tensor Store rate limit control
  logic [`DCACHE_TS_RATE_COUNTER_BITS-1:0] rate_counter_next;
  logic                                    rate_limit_allow_req;

  logic                                       l2_backoff;
  logic                                       l2_backoff_next;
  logic [`DCACHE_TS_BACKOFF_COUNTER_BITS-1:0] l2_backoff_count;
  logic [`DCACHE_TS_BACKOFF_COUNTER_BITS-1:0] l2_backoff_count_next;

  logic [`DCACHE_TENSOR_CTRL_DEST_RANGE] scp_entry;           // Scratchpad entry
  logic [`DCACHE_TENSOR_CTRL_DEST_RANGE] scp_entry_next;
  logic                                  scp_entry_up;        // Update entry value
  logic                                  scp_idx;             // Index of the cache line half in the scratchpad
  logic                                  scp_idx_next;

  logic [`DCACHE_SET_RANGE]              ts_scp_read_set;     // "set" of the scratchpad entry to be read
  logic                                  ts_is_from_scp;      // Flag indicating that TensorStore is from scratchpad
  logic                                  ts_is_from_scp_next;

  logic                vpu_send_reg_reduce;     // Request VPU to send register in a reduce operation
  logic                vpu_send_reg_tensor;     // Request VPU to send register in a TensorStore operation
  logic                vpu_send_reg;            // Combined request to VPU to send register

  logic                scp_read_reg;            // Request to read from scratchpad

  logic                pipe_used_pre;           // Signal to indicate that pipeline is going to be used in next cycle

  logic                tensorstore_count;      // Counts which L2 message for the same row a tensor store is sending
  logic                tensorstore_count_next;

  logic [`DCACHE_TS_ROWS_BITS:0]  atrans_num_req;        // Number of pending translation requests
  logic [`DCACHE_TS_ROWS_BITS:0]  atrans_num_req_next;

  atrans_state_t       atrans_state;          // Address translation state machine
  atrans_state_t       atrans_state_next;

  logic                                       address_available;     // Indication that there is an available translated address
  logic [`DCACHE_TS_TRANS_ADDR_CNT_BITS-1:0]  address_count;         // Count number of translated addressed
  logic [`DCACHE_TS_TRANS_ADDR_CNT_BITS-1:0]  address_count_next;

  logic                atrans_en;             // Enable address translation engine

  logic                address_save;          // Save a translated address
  logic                address_use;           // Use an already translated address to request data to pipeline
  logic                address_consume;       // Address has been used as many times as possible (1 or 2, depending on number of cols per row)
  logic                address_use_idx;       // Index for the number of usages of an address
  logic                address_use_idx_next;

  logic [`PA_RANGE]    addr_phys;             // Translated physical address to use in L2 transfers for TensorStore
  logic                address_for_l2_read;   // Read an address that was for for L2 transfers
  logic                address_for_l2_ready;  // Indicates that a translated address is available
  logic                address_fifo_full;     // FIFO storing addresses is full

  logic                err_mem;               // Error detected during a memory operation
  logic                err_mem_next;

  logic                clkgt;                 // Gated clock
  logic                clkgt_en;              // Clock enable signal

  logic [3:0]          operation;             // Operation that FMA will have to perform

  logic                ba_available;          // Flag to indicate that there is some buffer available to receive data
  logic                ba_available_next;
  logic                ba_available_up;

  logic                wait_data_from_sender; // Expecting to receive data

  // Common signals for l2 requests
  et_link_minion_evict_req_info_t   l2_req;

  ////////////////////////////////////////////////////////////////////////////////
  // Request control
  ////////////////////////////////////////////////////////////////////////////////

  // Store VM status for memory access
  //       CLK     EN       DOUT          DIN
  `EN_FF ( clkgt,  new_req, vm_status_ts, (thread_id == 1'b1)? vm_status[1] : vm_status[0] )

  // Short name for bit indicating that TensorStore is from scratchpad
  assign ts_is_from_scp_next = reduce_tensorstore_ctrl.tensorstore_scp.scp & ~req_info_next.reduce;

  //     CLK    EN            DOUT            DIN
  `EN_FF(clkgt, new_req,      req_info,       req_info_next)
  `EN_FF(clkgt, new_req,      rate,           rate_next)
  `EN_FF(clkgt, reg_count_up, reg_count,      reg_count_next)
  `EN_FF(clkgt, addr_up,      addr,           addr_next)
  `EN_FF(clkgt, addr_up,      addr_msb_err,   addr_msb_err_next)
  `EN_FF(clkgt, stride_up,    stride_addr,    stride_addr_next)
  `EN_FF(clkgt, stride_up,    stride_entry,   stride_entry_next)
  `EN_FF(clkgt, new_req,      ts_is_from_scp, ts_is_from_scp_next)
  `EN_FF(clkgt, scp_entry_up, scp_entry,      scp_entry_next)
  `EN_FF(clkgt, scp_entry_up, scp_idx,        scp_idx_next)
  `EN_FF(clkgt, new_req,      operation,      reduce_tensorstore_ctrl.reduce.op)

  // Tensor Store is reading scratchpad
  assign ts_scp_read = ts_is_from_scp;

  // Tensor Store indicates that only 128-bit are going to be provided for each column
  assign ts_col_size_is_128b = !req_info.reduce && (req_info.ac_info.cols == `DCACHE_TS_COLS_BITS'd1);

  // For performance counters
  assign ts_start = reduce_ready && tensorstore_start;
  assign ts_op    = !req_info.reduce && (ts_l2_req_valid && ts_l2_req_ready) || (re_l2_req_valid && re_l2_req_ready);

  always_comb begin
    // Ready and new request
    // -------------------------------------------------------------------------
    reduce_ready = (fsm_state == Reduce_Invalid);
    new_req      = (reduce_start || tensorstore_start) && reduce_ready;

    // Counters
    // -------------------------------------------------------------------------

    // Decrement pending entries upon buffer array alloc
    reg_count_up = new_req || ba_alloc;

    reg_count_next = reg_count;
    if (req_info.reduce) begin
      reg_count_next.cnt = reg_count.cnt - `DCACHE_RE_COUNT_BITS'b1;
    end else begin
      // Decrement cols (two columns in each iteration as one column is 128b and a single transfer is 256b)
      if (reg_count.rc.cols < `DCACHE_TS_COLS_BITS'd2) begin
        reg_count_next.rc.cols = `DCACHE_TS_COLS_BITS'b0;
      end else begin
        reg_count_next.rc.cols = reg_count.rc.cols - `DCACHE_TS_COLS_BITS'd2;
      end
      // Col done, move to next row
      if (reg_count_next.rc.cols == `DCACHE_TS_COLS_BITS'b0) begin
        reg_count_next.rc.cols = req_info.ac_info.cols;
        reg_count_next.rc.rows = reg_count.rc.rows - `DCACHE_TS_ROWS_BITS'b1;
        // Saturate at 0
        if (reg_count.rc.rows == `DCACHE_TS_ROWS_BITS'b0) begin
          reg_count_next.rc.cols = `DCACHE_TS_COLS_BITS'b0;
          reg_count_next.rc.rows = `DCACHE_TS_ROWS_BITS'b0;
        end
      end
    end

    // Select initial values for counters
    if (new_req) begin
      if (reduce_start) begin
        reg_count_next.cnt = `ZX(`DCACHE_RE_COUNT_BITS,reduce_tensorstore_ctrl.reduce.num_regs);
      end else begin
        reg_count_next.rc.ov = 1'b0;
        if (ts_is_from_scp_next) begin
          reg_count_next.rc.cols = `DCACHE_TS_COLS_BITS'd4; // 4 columns of 128 bits = full cache line
          reg_count_next.rc.rows = reduce_tensorstore_ctrl.tensorstore_scp.rows;
        end else begin
          reg_count_next.rc.cols = reduce_tensorstore_ctrl.tensorstore.cols + `DCACHE_TS_COLS_BITS'b1;
          reg_count_next.rc.rows = reduce_tensorstore_ctrl.tensorstore.rows;
        end
      end
    end

    // Request information to store
    // -------------------------------------------------------------------------

    // Select request information depending on the request type
    if (reduce_start) begin
      // req_info_next.start_re       = {1'b0,reduce_tensorstore_ctrl.reduce.start_reg}; // Not used
      req_info_next.ac_info.action = {1'b0,reduce_tensorstore_ctrl.reduce.action};
      req_info_next.reduce         = 1'b1;
      req_info_next.coop           = 2'b00;  // Don't care
      rate_next                    = `DCACHE_TS_RATE_BITS'b0; // Don't care
    end else begin
      if (ts_is_from_scp_next) begin
        // req_info_next.start_re     = reduce_tensorstore_ctrl.tensorstore_scp.start_entry; // Not used
        req_info_next.ac_info.cols = `DCACHE_TS_COLS_BITS'd4; // 4 columns of 128 bits = full cache line
        req_info_next.coop         = 2'b00;
        rate_next                  = reduce_tensorstore_ctrl.tensorstore_scp.rate;
      end else begin
        // req_info_next.start_re     = {1'b0,reduce_tensorstore_ctrl.tensorstore.start_reg}; // Not used
        req_info_next.ac_info.cols = reduce_tensorstore_ctrl.tensorstore.cols + `DCACHE_TS_COLS_BITS'b1;
        req_info_next.coop         = reduce_tensorstore_ctrl.tensorstore.coop;
        rate_next                  = reduce_tensorstore_ctrl.tensorstore.rate;
      end
      req_info_next.reduce         = 1'b0;
    end
    req_info_next.thread_id = thread_id;

    // Invariable fields in req_info
    req_info_next.pl_info.partner_id = reduce_tensorstore_ctrl.reduce.partner;

    // Tensor Store information
    // -------------------------------------------------------------------------

    // Save address and stride only in case of TensorStore
    if (new_req & tensorstore_start) begin
      addr_up = 1'b1;
      stride_up = 1'b1;
      addr_msb_err_next = 1'b0;
      if (ts_is_from_scp_next) begin
        addr_next = {reduce_tensorstore_ctrl.tensorstore_scp.addr,2'b0};
      end else begin
        addr_next = reduce_tensorstore_ctrl.tensorstore.addr;
      end
    end else begin
      addr_up = address_save;
      stride_up = 1'b0;
      addr_next = addr + stride_addr;
      addr_msb_err_next = addr[`VA_MSB] & !addr_next[`VA_MSB];
    end

    // Force address stride to be cache aligned when loading from scratchpad
    if (ts_is_from_scp_next) begin
      stride_addr_next = {tensorstore_stride[`VA_SIZE-1:6],2'b0};
    end else begin
      stride_addr_next = tensorstore_stride[`VA_SIZE-1:4];
    end

    // Entry stride has only menaing when doing a TensorStore from scratchpad
    stride_entry_next  = reduce_tensorstore_ctrl.tensorstore_scp.stride_entry;

    // Address with full bits
    addr_full = { addr, 4'b0 };

    // Index in the scratchpad (ensure there is no overflow)
    if (new_req & tensorstore_start & ts_is_from_scp_next) begin
      scp_entry_up = 1'b1;
      scp_idx_next = 1'b0;
      scp_entry_next = reduce_tensorstore_ctrl.tensorstore_scp.start_entry;
    end else begin
      scp_entry_up = ts_is_from_scp & !req_info.reduce & s1_inst_send_val;
      scp_idx_next = ~scp_idx;
      if (scp_idx) begin
        scp_entry_next = scp_entry + stride_entry + `ZX(`DCACHE_TENSOR_CTRL_DEST_BITS,1'b1);
      end else begin
        scp_entry_next = scp_entry;
      end
      if (scp_ctrl_on) begin
        if (scp_entry_next >= `DCACHE_TL_SCP_MAX_IDX) begin
          scp_entry_next = scp_entry_next - `DCACHE_TL_SCP_MAX_IDX;
        end
      end
    end

    // Global clock enable
    // -------------------------------------------------------------------------
    clk_en = !reduce_ready || new_req;
  end

  // Obtain scratchpad access address
  assign ts_scp_read_set = get_set_from_scp_dest(scp_entry); // #xrb#   scp_entry[`DCACHE_SET_RANGE];
  assign ts_scp_read_way = get_way_from_scp_dest(scp_entry); // #xrb#   scp_entry[$clog2(`DCACHE_SETS) +: $clog2(`DCACHE_WAYS)];
  assign ts_scp_read_addr = {ts_scp_read_set, scp_idx, {`CORE_L2_BLOCK_EXT_ADDR_OFFSET_BITS{1'b0}}};

  ////////////////////////////////////////////////////////////////////////////////
  // Engine to obtain translated addresses for TensorStore
  ////////////////////////////////////////////////////////////////////////////////

  // Before using the pipeline, the translated address has to be available
  // The TLB may fail or generate a wait condition, then the pipeline to get data from VPU
  // cannot be started until we have a guarantee that the address is available

  // Enable addres translation engine
  assign atrans_en = (fsm_state == Reduce_New_Req) || (atrans_state != Atrans_Idle);

  // Save translated address
  assign address_save = (atrans_state == Atrans_Save);

  // Address is available to make requests to pipeline
  assign address_available = (address_count != 0);

  // Use and consume translated addresses
  assign address_use     = (scp_read_reg || vpu_send_reg_tensor) && address_available;
  assign address_consume = address_use && address_use_idx;

  always_comb begin

    err_mem_next      = 1'b0;
    // Set the initial value for the number of addresses to translate
    // -------------------------------------------------------------------------
    atrans_num_req_next = atrans_num_req;
    if ((fsm_state == Reduce_New_Req) && !req_info.reduce) begin
      atrans_num_req_next = reg_count.rc.rows + `DCACHE_TS_ROWS_BITS'b1;
    end else if (atrans_state == Atrans_Save) begin
      atrans_num_req_next = atrans_num_req - `DCACHE_TS_ROWS_BITS'b1;
    end

    // Counter of the addresses that have been translated and not consumed
    // -------------------------------------------------------------------------
    address_count_next = address_count;
    if ((fsm_state == Reduce_New_Req) && !req_info.reduce) begin
      address_count_next = `DCACHE_TS_TRANS_ADDR_CNT_BITS'b0;
    end else if (address_save && !address_consume) begin
      address_count_next = address_count + `DCACHE_TS_TRANS_ADDR_CNT_BITS'b1;
    end else if (address_consume && !address_save) begin
      address_count_next = address_count - `DCACHE_TS_TRANS_ADDR_CNT_BITS'b1;
    end

    // Index for the number of uses of an address
    // -------------------------------------------------------------------------
    address_use_idx_next = address_use_idx;
    if (req_info.ac_info.cols <= `DCACHE_TS_COLS_BITS'd2) begin
      address_use_idx_next = 1'b1;
    end else if (!address_available) begin
      address_use_idx_next = 1'b0;
    end else if (address_use) begin
      address_use_idx_next = ~address_use_idx;
    end

    // Translate state transitions
    // -------------------------------------------------------------------------
    atrans_state_next = atrans_state;
    if (atrans_state == Atrans_Idle) begin
      if ((fsm_state == Reduce_New_Req) && !req_info.reduce) begin
        atrans_state_next = Atrans_Loop;
      end
    end else if (atrans_state == Atrans_Loop) begin
      if (atrans_num_req != 5'b0) begin            // Go to request a new address
        atrans_state_next = Atrans_S0_req;
      end else if (atrans_num_req == 5'b0) begin       // All translations done, finish
        atrans_state_next = Atrans_Idle;
      end
    end else if (atrans_state == Atrans_S0_req) begin
      if (s0_available & !address_fifo_full) begin // S0 available and FIFO not full, go to S1
        atrans_state_next = Atrans_S1_req;
      end
    end else if (atrans_state == Atrans_S1_req) begin
      if (s1_available) begin                      // S1 stage available for this engine
        if (s1_tlb_fail) begin
          atrans_state_next = Atrans_Flush;
        end else if (s1_tlb_wait) begin
          atrans_state_next = Atrans_Wait;
        end else begin
          atrans_state_next = Atrans_Save;
        end
      end else begin
        atrans_state_next = Atrans_S0_req;    // S1 stage not available, repeat S0
      end
    end else if (atrans_state == Atrans_Wait) begin
      if (!s1_tlb_wait) begin
        atrans_state_next = Atrans_S0_req;    // Repeat operation that was waiting
      end
    end else if (atrans_state == Atrans_Flush) begin
      if (!address_available) begin
        atrans_state_next = Atrans_Fail;
        err_mem_next = 1'b1;
      end
    end else if (atrans_state == Atrans_Fail) begin
      atrans_state_next = Atrans_Idle;        // Cannot continue, stop operation
    end else if (atrans_state == Atrans_Save) begin
      if (atrans_num_req_next != 5'b0) begin       // Go to request a new one if necessary
        atrans_state_next = Atrans_S0_req;
      end else begin
        atrans_state_next = Atrans_Loop;      // Go to wait for next request
      end
    end
  end

  // Registers for the previous combinational signals
  //         CLK    RST    EN         DOUT               DIN                    DEF
  `RST_EN_FF(clkgt, reset, atrans_en, atrans_state,      atrans_state_next,     Atrans_Idle)
  `EN_FF    (clkgt,        atrans_en, atrans_num_req,    atrans_num_req_next               )
  `EN_FF    (clkgt,        atrans_en, address_count,     address_count_next                )
  `EN_FF    (clkgt,        atrans_en, address_use_idx,   address_use_idx_next              )

  // Connect output ports for address translation
  assign s0_addr_load = (atrans_state == Atrans_S0_req) && s0_available && !address_fifo_full;
  assign s0_thread_id = req_info.thread_id;
  assign s1_addr_load = (atrans_state == Atrans_S1_req);

  // Address to translate
  assign s0_addr = addr_full;
  assign s0_addr_msb_err = addr_msb_err;

  ////////////////////////////////////////////////////////////////////////////////
  // Based on the request, decodes basic info, like type of operation, what
  // action to take, ...
  ////////////////////////////////////////////////////////////////////////////////

  logic [`MAX_MINION_ID_R]            partner_minion;    // ID of the partnering minion
  logic [`MAX_MINION_ID_R]            hartmask;          // Which bits of the hart to look at
  logic [`MAX_MINION_ID_R]            distance;          // Distance based on level
  logic [`DCACHE_REDUCE_LEVEL_RANGE]  level;             // Level of reduce for auto action case
  logic [`DCACHE_REDUCE_ACTION_RANGE] action;            // What action to take: send (0), receive (1), do nothing (2)
  logic                               invert;            // Invert the reduce to a broadcast
  logic                               err_partner_id;    // Wrong partner specified
  logic                               err_operation;     // Wrong operation for fma
  logic                               pairing_auto;      // Auto pairing method selected

  // Error reporting
  //      CLK    RST    DOUT     DIN            DEF
  `RST_FF(clock, reset, err_mem, err_mem_next,  1'b0)

  assign err_flags[`DCACHE_ERR_FLAG_PARTNER_ID]   = {1'b0, err_partner_id | err_operation};
  assign err_flags[`DCACHE_ERR_FLAG_MEM_FAULT]    = {err_mem  && req_info.thread_id, err_mem  && !req_info.thread_id};

  always_comb begin
    // Obtain parameters to compute partner minion and action to implement
    //   Computation of distance and hartmask strictly speaking may overflow as level can do more
    //   shifting than bits in global_min_id bits. Configuration from SW must be correct, otherwise behavior
    //   is "undefined"
    //
    //   Since `MAX_MINION_ID_L is defined as $clog2(`MAX_MINION_ID), it can't be used to specify bitlength
    //   Implicit arithmetic zero extension creates a linter warning but dimensions of operands are well determined
    action         = req_info.ac_info.action[`DCACHE_REDUCE_ACTION_RANGE];
    partner_minion = req_info.pl_info.partner_id;
    level          = req_info.pl_info.level.startl;
    distance       = `ZX(`MAX_MINION_ID_L,1'b1) << level;
    hartmask       = (`ZX(`MAX_MINION_ID_L,1'b1) << (level + `DCACHE_REDUCE_LEVEL_BITS'b1)) - `ZX(`MAX_MINION_ID_L,1'b1);
    invert         = ~action[0];

    // Pairing mode
    pairing_auto = (req_info.ac_info.action[`DCACHE_REDUCE_ACTION_RANGE] != `DCACHE_REDUCE_SEND) &&
                   (req_info.ac_info.action[`DCACHE_REDUCE_ACTION_RANGE] != `DCACHE_REDUCE_RECV);

    // Pairing error
    err_partner_id = 1'b0;

    // Do auto decode of action based on minion ID and level
    if (pairing_auto)
    begin

        // Can't pair to unexisting hierarchy
        if ((fsm_state == Reduce_New_Req) && (level > `DCACHE_REDUCE_MAX_LEVEL) && req_info.reduce)
          err_partner_id = 1'b1;
          
        if((global_min_id & hartmask) == distance)
        begin
            // Reverse the behaviour in case of doing broadcast instead of reduce
            action         = {1'b0, invert}; // 00=>sender, 01=>receiver (invert==1)
            partner_minion = global_min_id - distance;
        end
        else if((global_min_id & hartmask) == 0)
        begin
            // Reverse the behaviour in case of doing broadcast instead of reduce
            action         = {1'b0,~invert}; // 01=>receiver, 00=>sender (invert==1)
            partner_minion = global_min_id + distance;
        end
        else
        begin
            action = `DCACHE_REDUCE_DO_NOTHING; // do nothing
        end
    end
    else 
    begin
        // Cannot pair with myself
        if ((fsm_state == Reduce_New_Req) && (partner_minion == global_min_id) && req_info.reduce)
          err_partner_id = 1'b1;
       
        // Pairing ID out of range
        if ((fsm_state == Reduce_New_Req) && (partner_minion > `DCACHE_REDUCE_MAX_PARTNER_ID) && req_info.reduce)
          err_partner_id = 1'b1;
    end
  end

  // Check operation to be performed
  always_comb begin
    err_operation = (fsm_state == Reduce_New_Req) && req_info.reduce && (action == `DCACHE_REDUCE_RECV)
                 && (operation != 4'b0000) && (operation != 4'b0010) && (operation != 4'b0011) && (operation != 4'b0100)
                 && (operation != 4'b0110) && (operation != 4'b0111) && (operation != 4'b1000);

  end

  ////////////////////////////////////////////////////////////////////////////////
  // This is the FSM that based on the decoded info, takes the needed steps.
  ////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_BUFFER_ID_RANGE] ba_data_send_fifo_data; // Data read out of send data fifo
  logic [`DCACHE_BUFFER_ID_RANGE] ba_op_fifo_data;        // Data read out of operation fifo
  logic                           got_ready_msg;          // Received ready message from receiver
  logic                           got_ready_msg_next;     // Received ready message from receiver
  logic                           s4_send_pending;        // Pending message to be sent
  logic                           s4_send_pending_pre;
  logic                           write_around_throttle;  // Throttle new write arounds
  logic                           write_around_throttle_next;
  logic                           s4_send;                // Sent a send message
  logic                           any_packet;             // Any packet outstanding
  logic                           reduce_wait_done;       // All the conditions for the wait data are met
  logic                           ba_write_needed_rec;    // Buffer array write needed due receiver
  logic                           ba_op_fifo_valid;       // Pending entry to send to VPU to do the reduce
  logic                           ba_alloc_fifo_val;      // Valid entry in the alloc fifo
  logic                           l2_req_inflight;        // Indicates that a double-cycle transfer is ongoing
  logic                           l2_req_inflight_next;
  logic                           s0_inst_send_val;       // Validates that a send operation is in S0 stage

  //         CLK    RST    EN      DOUT              DIN                    DEF
  `RST_EN_FF(clkgt, reset, clk_en, fsm_state,        fsm_state_next,        Reduce_Invalid)
  `RST_FF   (clkgt, reset,         got_ready_msg,    got_ready_msg_next,    1'b0)
  `RST_FF   (clkgt, reset,         l2_req_inflight,  l2_req_inflight_next,  1'b0)

  assign re_ts_l2_req_inflight = l2_req_inflight;
  assign wait_data_from_sender = (fsm_state == Reduce_Wait_Data) || (fsm_state == Reduce_Send_Ready);

  // Indicate that BA entries are going to be allocated
  assign ba_entries_required = ((fsm_state == Reduce_Wait_Data) || (fsm_state == Reduce_Send_Ready))  // Reduce Receive
                            || ((fsm_state == Reduce_Send_Data) || (fsm_state == Reduce_Store_Data)); // Reduce Send or Tensor Store

  // Computes next state
  always_comb begin
    fsm_state_next = fsm_state;

    // Idle and a new message is coming for reduce_start or tensorstore_start
    if ((fsm_state == Reduce_Invalid) && new_req) begin
      fsm_state_next = Reduce_Wait_Tensor;
    end
    // Waiting to have tensor dependencies cleared
    if ((fsm_state == Reduce_Wait_Tensor) && !vpu_reduce_wait) begin
      fsm_state_next = Reduce_New_Req;
    end
    // New tensor store request
    else if ((fsm_state == Reduce_New_Req) && !req_info.reduce) begin
      fsm_state_next = Reduce_Store_Data;
    end
    // Error in request, do nothing
    else if ((fsm_state == Reduce_New_Req) && err_partner_id) begin
      fsm_state_next = Reduce_Invalid;
    end
    // Receiver request with invalid operation
    else if ((fsm_state == Reduce_New_Req) && err_operation) begin
      fsm_state_next = Reduce_Invalid;
    end
    // Nothing to be sent
    else if ((fsm_state == Reduce_New_Req) && (reg_count == 0)) begin
      fsm_state_next = Reduce_Invalid;
    end
    // New sender request
    else if ((fsm_state == Reduce_New_Req) && (action == `DCACHE_REDUCE_SEND)) begin
      fsm_state_next = Reduce_Wait_Ready;
    end
    // New receiver request
    else if ((fsm_state == Reduce_New_Req) && (action == `DCACHE_REDUCE_RECV)) begin
      fsm_state_next = Reduce_Send_Ready;
    end
    // New do nothing request
    else if ((fsm_state == Reduce_New_Req) && (action == `DCACHE_REDUCE_DO_NOTHING)) begin
      fsm_state_next = Reduce_Invalid;
    end
    // If sending ready and accepted
    else if ((fsm_state == Reduce_Send_Ready) && ba_available && re_l2_req_ready) begin
      fsm_state_next = Reduce_Wait_Data;
    end
    // If waiting for ready and got it
    else if ((fsm_state == Reduce_Wait_Ready) && got_ready_msg) begin
      fsm_state_next = Reduce_Send_Data;
    end
    // If an error happened during the address translation, abort
    else if ((fsm_state == Reduce_Store_Data) && (atrans_state == Atrans_Fail)) begin
      fsm_state_next = Reduce_Drain;
    end
    // If sending data, go to invalid when all sends done
    else if ((fsm_state == Reduce_Store_Data) && (reg_count == 0)) begin
      fsm_state_next = Reduce_Drain;
    end
    // If sending data, go to invalid when all sends done
    else if ((fsm_state == Reduce_Send_Data) && (reg_count == 0)) begin
      fsm_state_next = Reduce_Drain;
    end
    // Waiting for pipeline to drain
    else if ((fsm_state == Reduce_Drain) && !any_packet) begin
      fsm_state_next = Reduce_Invalid;
    end
    // If in reduce wait data, wait for its done signal
    else if ((fsm_state == Reduce_Wait_Data) && reduce_wait_done) begin
      fsm_state_next = Reduce_Invalid;
    end
  end

  // Reserves the interface to L2 while requests is not completed
  always_comb begin
    // Activate the protection flag once the first transaction is accepted and until all transcations are done
    l2_req_inflight_next = l2_req_inflight;

    if ((re_l2_req_valid && re_l2_req_ready) || (ts_l2_req_valid && ts_l2_req_ready)) begin
      // Toggle the "inflight" condition when transfer is more than 128b
      if ((l2_req_inflight == 1'b0) && (l2_req.size > ET_LINK_HLine)) begin
        l2_req_inflight_next = 1'b1;
      end else begin
        l2_req_inflight_next = 1'b0;
      end
    end
  end

  // Keeps track of partners ready to receive data from me
  logic [`MAX_MINION_ID_R]              partner_ready_mask;      // Mask of all partners ready to receive from me
  logic [`MAX_MINION_ID_R]              partner_ready_mask_next;
  logic [`DCACHE_REDUCE_MSG_DATA_RANGE] partner_ready_idx;       // Index of the partner that has sent the ready message. MSB is used to flag a peer-to-peer pairing
  logic [`DCACHE_REDUCE_MSG_DATA_RANGE] partner_ready_idx_next;
  logic                                 partner_ready_peer;      // Peer is ready to receive from me
  logic                                 partner_ready_peer_next;
  logic                                 new_partner_ready;       // New partner Ready Message just arrived
  logic                                 new_partner_ready_z1;
  logic                                 current_partner_ready;   // Current partner (the one I want to send data now) is ready
  logic                                 re_l2_req_valid_spec;    // Speculative version

  // Address formatted for Reduce messages
  // Bits[39:32] = 8'h01
// Bits[31:30] = 2'h2
// Bits[29:22] = <shire_id>
// Bits[21:20] = 2'h0
// Bits[19:13] = <shire_min_id>
  // Bits[12]    = <thread_id> (this will be the Id of the sender thread as the "receiver" thread is not known ... probably not relevant bit)
  // Bits[11]    = 1'h1
  // Bits[10:3]  = ET_LINK_Min_Msg_Id_Reduce*
  // Bits[2:0]   = 3'h0
  logic [`PA_RANGE]                      reduce_base_address;
  logic [`PA_RANGE]                      reduce_send_ready_address;
  logic [`PA_RANGE]                      reduce_send_data_address;
  logic [`NUM_SHIRE_IDS_R]               reduce_shire_id;
  logic [`AD_ESR_SHIRE_HART_ID_SIZE-1:0] reduce_hart_id;


assign reduce_shire_id           = partner_minion[`MAX_MINION_ID_L-1:`MIN_PER_N_L+`NUM_NEIGH_L];
assign reduce_hart_id            = `ZX(8,{partner_minion[`MIN_PER_N_L+`NUM_NEIGH_L-1:0],req_info.thread_id});
assign reduce_base_address       = {`AD_ESR_REGION,`AD_ESR_PROT_BITS_MSG,`AD_ESR_AGENT_TYPE_MINION,reduce_hart_id,`AD_ESR_PORT_MSG_COMMON};
assign reduce_send_ready_address = reduce_base_address | (ET_LINK_Min_Msg_Id_Reduce_Ready << `AD_ESR_MESSAGE_ID_START);
assign reduce_send_data_address  = reduce_base_address | (ET_LINK_Min_Msg_Id_Reduce_Data << `AD_ESR_MESSAGE_ID_START);

  assign partner_ready_idx_next    = l2_resp.data[`DCACHE_REDUCE_MSG_DATA_BITS-1:0];
  assign new_partner_ready         = l2_resp_valid && (l2_resp.opcode == ET_LINK_RSP_MsgRcvData) && (et_link_min_msg_id_t'(l2_resp.id) == ET_LINK_Min_Msg_Id_Reduce_Ready);

  // Cannot clock gate this register since asynchronous messages may arrive
  //         CLK    RST    EN                                         DOUT                  DIN                      DEF
  `RST_EN_FF(clock, reset, got_ready_msg || new_partner_ready_z1,     partner_ready_mask,   partner_ready_mask_next, {`MAX_MINION_ID_L{1'b0}})
  `RST_EN_FF(clock, reset, got_ready_msg || new_partner_ready_z1,     partner_ready_peer,   partner_ready_peer_next,                   1'b0  )
  `EN_FF    (clock,        new_partner_ready,                         partner_ready_idx,    partner_ready_idx_next                           )
  `RST_EN_FF(clock, reset, new_partner_ready || new_partner_ready_z1, new_partner_ready_z1, new_partner_ready,                         1'b0  )

  always_comb begin

    partner_ready_mask_next = partner_ready_mask;
    partner_ready_peer_next = partner_ready_peer;

    // Messages may arrive before the reduce module is configured.
    // These may contain information for an auto-pairig or for a point-to-point connection
    if (new_partner_ready_z1) begin
      // MSB of idx indicates whether it is a peer-to-peer message or an auto-pair message
      if (partner_ready_idx[`DCACHE_REDUCE_MSG_DATA_MSB] == 1'b0) begin
        partner_ready_mask_next[partner_ready_idx[`DCACHE_REDUCE_LEVEL_RANGE]] = 1'b1;
      end else begin
        partner_ready_peer_next = 1'b1;
      end
    end

    // Going to send reduce data to current partner, clear mask bit
    if (got_ready_msg) begin
      if (pairing_auto) begin
        partner_ready_mask_next[level] = 1'b0;
      end else begin
        partner_ready_peer_next = 1'b0;
      end
    end
  end

  // Partner readiness is taken from the mask in auto mairing mode or from the
  // peer-to-peer single bit flag otherwise
  always_comb begin
    if (pairing_auto) begin
      current_partner_ready = partner_ready_mask[level];
    end else begin
      current_partner_ready = partner_ready_peer;
    end
  end

  // Check buffer availability before indicating that we are ready to receive.
  // Once we see that BA is not full we are sure that the Core is not going to allocate
  // all resources as we indicate that we need entries with "ba_entries_required" signal
  always_comb begin
    ba_available_next = ba_available;
    if (fsm_state == Reduce_New_Req) begin
      ba_available_next = 1'b0;
    end else if (fsm_state == Reduce_Send_Ready) begin
      ba_available_next = !ba_full | ba_alloc_fifo_val;
    end

    ba_available_up = (fsm_state == Reduce_New_Req) || (fsm_state == Reduce_Send_Ready);
  end

  //         CLK    RST    EN               DOUT          DIN
  `EN_FF    (clkgt,        ba_available_up, ba_available, ba_available_next)

  // Computes outputs based on state
  logic [`DCACHE_REDUCE_MSG_DATA_RANGE] l2_req_msg_data;

  // Group mechanisms that block sending requests to L2
  logic  l2_req_valid_block;
  assign l2_req_valid_block = write_around_throttle || !rate_limit_allow_req || l2_backoff;

  always_comb begin
    //
    // VPU control
    //

    // Can send a new send request if in send request state and
    // it is not full and still pending regs to send
    vpu_send_reg_reduce = (fsm_state == Reduce_Send_Data)                                          && !ba_full && (reg_count != 'b0);
    vpu_send_reg_tensor = (fsm_state == Reduce_Store_Data) && !ts_is_from_scp && address_available && !ba_full && (reg_count != 'b0);
    vpu_send_reg        = vpu_send_reg_reduce || vpu_send_reg_tensor;

    vpu_ctrl.send_reg = vpu_send_reg;
    vpu_ctrl.exec_op  = 1'b0;
    vpu_ctrl.nothing  = (fsm_state == Reduce_New_Req) && req_info.reduce && ((action == `DCACHE_REDUCE_DO_NOTHING) || err_partner_id || err_operation)
                     || (fsm_state == Reduce_Store_Data) && (atrans_state == Atrans_Fail);

    //
    // Scratchpad control
    //
    scp_read_reg = (fsm_state == Reduce_Store_Data) && ts_is_from_scp && address_available && !ba_full && (reg_count != 'b0);

    //
    // Buffer array control
    //

    // Alloc when sending a new data or when allocating storage for incoming data
    ba_alloc = vpu_send_reg || scp_read_reg
             // For receivers, as we are doing the write needed pass at same
             // time, we need to guarantee that S2 stage is available
             || (wait_data_from_sender && !ba_full && !s2_valid && (reg_count != 'b0));

    // Get the entry ID in buffer array in S2 for senders, right at ba_alloc
    // for receivers
    ba_write_needed_rec = (ba_alloc && wait_data_from_sender);
    ba_write_needed     = s2_inst_send_val || ba_write_needed_rec;

    //
    // Requests to L2
    //

    re_l2_req_valid = 1'b0;
    ts_l2_req_valid = 1'b0;
    l2_req          = 'b0;
    ba_read_en      = 1'b0;
    ba_read_addr    = ba_data_send_fifo_data;
    s1_inst_op_val  = 1'b0;

     // Data in message contains the "level" to let partner minion know which minion is ready to receive
     // or just bit DCACHE_REDUCE_MSG_DATA_MSB to '1' to indicate a peer-to-peer connection
    if (pairing_auto) begin
      l2_req_msg_data = `ZX(`DCACHE_REDUCE_MSG_DATA_BITS,level);
    end else begin
      l2_req_msg_data = `DCACHE_REDUCE_MSG_DATA_BITS'b1 << `DCACHE_REDUCE_MSG_DATA_MSB;
    end

    // Sends the ready packet to partnering minion
    if (fsm_state == Reduce_Send_Ready) begin
      re_l2_req_valid                            = ba_available;
      l2_req.id                                  = '0;                            // Unused
      l2_req.source                              = 1'b0;                          // Unused
      l2_req.wdata                               = 1'b1;
      l2_req.opcode                              = ET_LINK_REQ_MsgSendData;       // This is a message
      l2_req.subopcode                           = `ET_LINK_SUBOPCODE_SIZE'b0;
      l2_req.address                             = reduce_send_ready_address;     // Address with message ID
      l2_req.data[`DCACHE_REDUCE_MSG_DATA_RANGE] = l2_req_msg_data;

      l2_req.size                                = ET_LINK_Byte;                  // Byte size
      l2_req.qwen                                = 4'b0001;                       // Byte size
    end

    // Speculative version
    re_l2_req_valid_spec = req_info.reduce
                        // Need to have:
                        //   - both 256b halves ready
                        //   - doing second half
                        //   - 256b or less
                        // otherwise minion can deadlock (RTLMIN-3810)
                        || ((req_info.coop == 2'b00) && (s4_send_pending_pre || l2_req_inflight || (req_info.ac_info.cols <= `DCACHE_TS_COLS_BITS'd2)));

    // If pending to send something and buffer array read port not used and address to send a TensorStore is available
    if (s4_send_pending && !l2_req_valid_block && !ba_read_used && (req_info.reduce || address_for_l2_ready) && (!mh_going_to_uc_store || l2_req_inflight)) begin
      ba_read_en       = 1'b1;                                   // Read the buffer array
      re_l2_req_valid  = re_l2_req_valid_spec;
      ts_l2_req_valid  = !req_info.reduce && (req_info.coop != 2'b00);
      l2_req.id        = re_l2_req_valid ? ET_LINK_Core_Miss_Id_Reduce
                                         : ET_LINK_Core_Miss_Id_Tensor_Store_Coop;
      l2_req.source    = 1'b0;                                   // Unused
      l2_req.wdata     = 1'b1;
      l2_req.opcode    = req_info.reduce ? ET_LINK_REQ_MsgSendData  // Send message for reduce case
                       :                   ET_LINK_REQ_WriteAround; // Write around for tensor store

      l2_req.subopcode = req_info.reduce          ? `ET_LINK_SUBOPCODE_SIZE'd0
                       : (req_info.coop == 2'b00) ? ET_LINK_WriteAround       // 1-Way cooperative
                       : (req_info.coop == 2'b01) ? ET_LINK_WriteAround2Way   // 2-Way cooperative
                       :                            ET_LINK_WriteAround4Way;  // 4-Way cooperative

      l2_req.address   = req_info.reduce                                    ? reduce_send_data_address    // Address with message ID
                       : (req_info.ac_info.cols == `DCACHE_TS_COLS_BITS'd1) ? {addr_phys[`PA_MSB:4],4'b0} // Address for stores forcing alignment
                       : (req_info.ac_info.cols == `DCACHE_TS_COLS_BITS'd2) ? {addr_phys[`PA_MSB:5],5'b0} // 128b x1, x2 or x4
                       :                                                      {addr_phys[`PA_MSB:6],6'b0};
      l2_req.data      = s1_ba_data;
      l2_req.size      = req_info.reduce                                    ? ET_LINK_HLine  // Half line for reduce
                       : (req_info.ac_info.cols == `DCACHE_TS_COLS_BITS'd1) ? ET_LINK_QWord  // QWord size for 128 arounds
                       : (req_info.ac_info.cols == `DCACHE_TS_COLS_BITS'd2) ? ET_LINK_HLine  // Half line for 256 arounds
                       :                                                      ET_LINK_Line;  // Full line otherwise
      l2_req.qwen      = req_info.reduce                                    ? 4'b0011                           // Half line for reduce
                       : (req_info.ac_info.cols == `DCACHE_TS_COLS_BITS'd1) ? 4'(4'b0001 << addr_phys[5:4])     // QWord size for 128 arounds
                       : (req_info.ac_info.cols == `DCACHE_TS_COLS_BITS'd2) ? 4'(4'b0011 << {addr_phys[5],1'b0})// Half line for 256 arounds
                       :                                                      4'b1111;                          // Full line otherwise
    end

    // Have reduces ready to be sent to VPU (S1 need to be empty)
    if (ba_op_fifo_valid && !s1_valid && !ba_read_used) begin
      ba_read_en       = 1'b1;
      ba_read_addr     = ba_op_fifo_data;
      s1_inst_op_val   = 1'b1;
      vpu_ctrl.exec_op = 1'b1;
    end

    s4_send = s4_send_pending && ((re_l2_req_valid && re_l2_req_ready) || (ts_l2_req_valid && ts_l2_req_ready));

    // Checks if last packet (each transfer can do up to 256 bits)
    //   Each column is 128-bit
    //   One or two columns implies single cycle, four implies 2 cycles
    last_packet_row = 1'b0;
    if ((req_info.ac_info.cols == `DCACHE_TS_COLS_BITS'd1)
      || (req_info.ac_info.cols == `DCACHE_TS_COLS_BITS'd2)
      || ((req_info.ac_info.cols == `DCACHE_TS_COLS_BITS'd4) && tensorstore_count)) begin
      last_packet_row = !req_info.reduce && s4_send;
    end

    // Updates packet being sent for same row
    tensorstore_count_next = ~tensorstore_count;
    if (new_req || last_packet_row) begin
      tensorstore_count_next = 1'b0;
    end

    // Speculative buffer array read enable that doesn't depend on miss
    // handler using the buffer array read port. Better timing
    // Don't set it until both 256 halves have allocated buffer array entries
    // otherwise dcache can deadlock with entries in replayQ that have all but
    // one buffer array entries that always fail the replay because of the
    // spec buffer array read (RTLMIN-3810)
    ba_read_en_spec = 1'b0;
    if (s4_send_pending && (re_l2_req_valid_spec || (!req_info.reduce && (req_info.coop != 2'b00))) && !l2_req_valid_block) begin
      ba_read_en_spec = (fsm_state != Reduce_Send_Ready);
    end
    if (ba_op_fifo_valid && !s1_valid) begin
      ba_read_en_spec = (fsm_state != Reduce_Send_Ready);
    end

    //
    // Computes ready message bit
    //
    got_ready_msg_next = got_ready_msg;
    // If incoming ready message, set bit
    if (current_partner_ready && (fsm_state == Reduce_Wait_Ready)) begin
      got_ready_msg_next = 1'b1;
    end
    // If moving from wait ready to another state, clear bit
    if ((fsm_state == Reduce_Wait_Ready) && (fsm_state_next != Reduce_Wait_Ready)) begin
      got_ready_msg_next = 1'b0;
    end

    // Drain condition
    any_packet = s0_inst_send_val || s1_inst_send_val || s2_inst_send_val || s3_inst_send_val || s4_send_pending_pre || s4_send_pending;

    // Reduce wait data done condition
    reduce_wait_done = (reg_count == 'b0) // Have allocated all the registers
                    && !ba_alloc_fifo_val // The alloc FIFO is empty
                    && !ba_op_fifo_valid  // All the ops are out of the FIFO
                    && !s1_inst_op_val    // No operation valid in the dcache pipeline
                    && !s2_inst_op_val
                    && !s3_inst_op_val;
  end

  //         CLK    RST    EN                  DOUT                DIN
  `EN_FF    (clkgt,        new_req || s4_send, tensorstore_count, tensorstore_count_next)

  // Request output is common
  assign re_l2_req = l2_req;
  assign ts_l2_req = l2_req;

  // Request rate control for TensorStore
  always_comb begin
    rate_counter_next = rate_counter;

    if (new_req) begin
      rate_counter_next = `DCACHE_TS_RATE_COUNTER_BITS'h0;
    end else if (last_packet_row) begin
      rate_counter_next = {rate,2'b0}; // Multiply by 4 the provided rate value
    end else if (rate_counter != `DCACHE_TS_RATE_COUNTER_BITS'h0) begin
      rate_counter_next = rate_counter - `DCACHE_TS_RATE_COUNTER_BITS'h1;
    end
  end

  assign rate_limit_allow_req = (rate_counter == `DCACHE_TS_RATE_COUNTER_BITS'h0);

  //      CLK    RST    EN  DOUT          DIN
  `FF    (clkgt,            rate_counter, rate_counter_next)

  // Backoff mechanisms to avoid blocking the pipeline reading the buffer array in case that L2 request cannot be sent
  // Each read request counts one unit, while each cycle in backoff decrements two units
  always_comb begin
    l2_backoff_next = l2_backoff;
    l2_backoff_count_next = l2_backoff_count;

    if (new_req) begin
      l2_backoff_count_next = `DCACHE_TS_BACKOFF_COUNTER_BITS'h0;
    end else if (ba_read_en) begin
      l2_backoff_count_next = l2_backoff_count + `DCACHE_TS_BACKOFF_COUNTER_BITS'h1;
    end else if (l2_backoff) begin
      l2_backoff_count_next = l2_backoff_count - `DCACHE_TS_BACKOFF_COUNTER_BITS'h2;
    end

    if (new_req) begin
      l2_backoff_next = 1'b0;
    end else if (ba_read_en && (l2_backoff_count_next == `DCACHE_TS_BACKOFF_COUNTER_BITS'h8)) begin
      l2_backoff_next = 1'b1;
    end else if (l2_backoff_count_next == `DCACHE_TS_BACKOFF_COUNTER_BITS'h0) begin
      l2_backoff_next = 1'b0;
    end
  end

  //       CLK    RST    EN  DOUT              DIN                    DEF
  `RST_FF (clkgt, reset,     l2_backoff,       l2_backoff_next,       1'b0)
  `FF     (clkgt,            l2_backoff_count, l2_backoff_count_next      )

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // Receiver only section
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // Stores the list of allocated entries in the buffer array. Receiver starts
  // allocating entries while waiting for data. Once data shows up, it uses the
  // head of the FIFO as write pointer to the buffer array and pops the entry.
  ////////////////////////////////////////////////////////////////////////////////

  logic ba_alloc_fifo_pop;     // Pop the head of the alloc fifo
  logic ba_op_fifo_full;       // Op FIFO is full
  logic ba_alloc_fifo_full;    // Ba FIFO is full
  logic ba_alloc_fifo_full_next_unused_ok;
  logic ba_alloc_fifo_valid_next_unused_ok;

  logic wait_l2_to_ba_resp_z1; // Delayed version of the signal forecasting that an L2 response is to come.
  logic wait_l2_to_ba_resp_en;

  gen_fifo
  #(
    .ELEM_SIZE   ( $clog2(`DCACHE_BUFFER_SIZE) ),
    .NUM_ELEMS   ( `DCACHE_BUFFER_SIZE         ),
    .USE_LATCHES ( 0                           )
  )
  ba_alloc_fifo
  (
    // System signals
    .clock       ( clkgt                       ),
    .reset       ( reset                       ),
    // Push data
    .push_early  ( 1'b0                        ), // Only used if implemented with latches
    .push        ( ba_write_needed_rec         ),
    .push_data   ( s2_ba_written_entry         ),
    .full        ( ba_alloc_fifo_full          ),
    .full_next   ( ba_alloc_fifo_full_next_unused_ok ),
    // Pop data
    .pop         ( ba_alloc_fifo_pop           ),
    .pop_data    ( s3_ba_write_addr            ),
    .valid       ( ba_alloc_fifo_val           ),
    .valid_next  ( ba_alloc_fifo_valid_next_unused_ok )
  );

  always_comb begin
    // Pop when the message is accepted
    ba_alloc_fifo_pop = l2_resp_valid
                      && (l2_resp.opcode == ET_LINK_RSP_MsgRcvData)
                      && (et_link_min_msg_id_t'(l2_resp.id) == ET_LINK_Min_Msg_Id_Reduce_Data);

    // Stall incoming data if the Operation FIFO is full. Make sure that the signal
    // forecasting that an L2 response is to come has been activated at least one cycle before,
    // otherwise it won't be possible writing into buffer array
    buffer_avail = ba_alloc_fifo_val && wait_l2_to_ba_resp_z1 && !ba_op_fifo_full;

    // Signal forecasting that an L2 response is to come
    wait_l2_to_ba_resp = ba_alloc_fifo_val;
  end

  assign wait_l2_to_ba_resp_en = wait_l2_to_ba_resp_z1 | wait_l2_to_ba_resp;

  //         CLK    RST    EN                     DOUT                   DIN                 DEF
  `RST_EN_FF(clkgt, reset, wait_l2_to_ba_resp_en, wait_l2_to_ba_resp_z1, wait_l2_to_ba_resp, 1'b0)

  ////////////////////////////////////////////////////////////////////////////////
  // Stores list of operations that are ready to be sent to VPU to do the reduce
  // operation.
  ////////////////////////////////////////////////////////////////////////////////
  logic ba_op_fifo_full_next_unused_ok;
  logic ba_op_fifo_valid_next_unused_ok;

  gen_fifo
  #(
    .ELEM_SIZE   ( $clog2(`DCACHE_BUFFER_SIZE) ),
    .NUM_ELEMS   ( 4                           ),
    .USE_LATCHES ( 0                           )
  )
  ba_op_fifo
  (
    // System signals
    .clock       ( clkgt                       ),
    .reset       ( reset                       ),
    // Push data
    .push_early  ( 1'b0                        ), // Only used if implemented with latches
    .push        ( ba_alloc_fifo_pop           ),
    .push_data   ( s3_ba_write_addr            ),
    .full        ( ba_op_fifo_full             ),
    .full_next   (ba_op_fifo_full_next_unused_ok ),
    // Pop data
    .pop         ( s1_inst_op_val              ),
    .pop_data    ( ba_op_fifo_data             ),
    .valid       ( ba_op_fifo_valid            ),
    .valid_next  ( ba_op_fifo_valid_next_unused_ok )
  );

  ////////////////////////////////////////////////////////////////////////////////
  // S2 Stage
  ////////////////////////////////////////////////////////////////////////////////

  //         CLK    RST    EN      DOUT            DIN             DEF
  `RST_EN_FF(clkgt, reset, clk_en, s2_inst_op_val, s1_inst_op_val, 1'b0)

  ////////////////////////////////////////////////////////////////////////////////
  // S3 Stage
  ////////////////////////////////////////////////////////////////////////////////

  //         CLK    RST    EN      DOUT            DIN             DEF
  `RST_EN_FF(clkgt, reset, clk_en, s3_inst_op_val, s2_inst_op_val, 1'b0)

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // Sender only section
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // Pre-S0 Stage
  // Allocates the Dcache pipeline to let the register data flow through
  ////////////////////////////////////////////////////////////////////////////////

  assign pipe_used_pre = vpu_send_reg | scp_read_reg;

  //         CLK    RST    EN      DOUT       DIN            DEF
  `RST_EN_FF(clkgt, reset, clk_en, pipe_used, pipe_used_pre, 1'b0)

  ////////////////////////////////////////////////////////////////////////////////
  // S0 Stage
  // We have to store the address that will go through TLB
  ////////////////////////////////////////////////////////////////////////////////

  //         CLK    RST    EN      DOUT              DIN        DEF
  `RST_EN_FF(clkgt, reset, clk_en, s0_inst_send_val, pipe_used, 1'b0)

  ////////////////////////////////////////////////////////////////////////////////
  // S1 Stage
  // We need to enable the data FF in S1 stage to capture the store data from
  // VRF
  ////////////////////////////////////////////////////////////////////////////////

  //         CLK    RST    EN      DOUT              DIN               DEF
  `RST_EN_FF(clkgt, reset, clk_en, s1_inst_send_val, s0_inst_send_val, 1'b0)

  ////////////////////////////////////////////////////////////////////////////////
  // S2 Stage
  // Set the write enables to the buffer array and enable clocks to S3 for store
  // data
  ////////////////////////////////////////////////////////////////////////////////

  //         CLK    RST    EN      DOUT              DIN               DEF
  `RST_EN_FF(clkgt, reset, clk_en, s2_inst_send_val, s1_inst_send_val, 1'b0)

  ////////////////////////////////////////////////////////////////////////////////
  // S3 Stage
  // Correctly muxes the data going to buffer array
  ////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_BUFFER_ID_RANGE] s3_ba_written_entry; // Written entry in buffer array

  //         CLK    RST    EN                DOUT                 DIN               DEF
  `RST_EN_FF(clkgt, reset, clk_en,           s3_inst_send_val,    s2_inst_send_val, 1'b0)
  `EN_FF    (clkgt,        s2_inst_send_val, s3_ba_written_entry, s2_ba_written_entry)

  // This FIFO stores the written entries in the buffer array with the data to be sent
  // Make a wrapper with a registered output to reduce delay from FIFO read pointer
  // to the output
  logic ba_data_send_fifo_full_unused_ok;
  logic ba_data_send_fifo_full_next_unused_ok;
  logic ba_data_send_fifo_valid_next_unused_ok;
  logic [`DCACHE_BUFFER_ID_RANGE] ba_data_send_fifo_data_pre;
  logic                           load_send_fifo_data;

  gen_fifo
  #(
    .ELEM_SIZE   ( $clog2(`DCACHE_BUFFER_SIZE) ),
    .NUM_ELEMS   ( `DCACHE_BUFFER_SIZE         ),
    .USE_LATCHES ( 0                           )
  )
  ba_data_send_fifo
  (
    // System signals
    .clock       ( clkgt                       ),
    .reset       ( reset                       ),
    // Push data
    .push_early  ( 1'b0                        ), // Only used if implemented with latches
    .push        ( s3_inst_send_val            ),
    .push_data   ( s3_ba_written_entry         ),
    .full        ( ba_data_send_fifo_full_unused_ok ),
    .full_next   ( ba_data_send_fifo_full_next_unused_ok ),
    // Pop data
    .pop         ( load_send_fifo_data &
                   s4_send_pending_pre         ),
    .pop_data    ( ba_data_send_fifo_data_pre  ),
    .valid       ( s4_send_pending_pre         ),
    .valid_next  ( ba_data_send_fifo_valid_next_unused_ok )
  );

  assign load_send_fifo_data  = s4_send || (s4_send_pending_pre & !s4_send_pending);

  //           CLK    RST    EN                   DOUT                    DIN                         DEF
  `EN_FF      (clkgt,        load_send_fifo_data, ba_data_send_fifo_data, ba_data_send_fifo_data_pre      )
  `RST_EN_FF  (clkgt, reset, load_send_fifo_data, s4_send_pending,        s4_send_pending_pre,        1'b0)


  // This FIFO stores the translated physical addresses to use in L2 transfers
  logic phys_addr_send_fifo_full_next_unused_ok;
  logic phys_addr_send_fifo_valid_next_unused_ok;
  gen_fifo
  #(
    .ELEM_SIZE   ( `PA_SIZE                    ),
    .NUM_ELEMS   ( `DCACHE_TS_PHYS_ADDR_NUM    ),
    .USE_LATCHES ( 0                           )
  )
  phys_addr_send_fifo
  (
    // System signals
    .clock       ( clkgt                       ),
    .reset       ( reset                       ),
    // Push data
    .push_early  ( 1'b0                        ), // Only used if implemented with latches
    .push        ( address_save                ),
    .push_data   ( s2_addr                     ),
    .full        ( address_fifo_full           ),
    .full_next   ( phys_addr_send_fifo_full_next_unused_ok ),
    // Pop data
    .pop         ( address_for_l2_read         ),
    .pop_data    ( addr_phys                   ),
    .valid       ( address_for_l2_ready        ),
    .valid_next  ( phys_addr_send_fifo_valid_next_unused_ok )
  );

  // Move to next row, get new address
  assign address_for_l2_read = last_packet_row;

  ////////////////////////////////////////////////////////////////////////////////
  // Counts the outstanding write arounds, limit it to 8 per minion
  ////////////////////////////////////////////////////////////////////////////////

  logic [3:0] write_around_cnt;      // Number of outstanding write arounds
  logic [3:0] write_around_cnt_next;
  logic       write_around_cnt_up;   // Update the counter
  logic       write_around_cnt_dec;
  logic       opcode_is_mine;

  //         CLK    RST    EN                   DOUT                   DIN                         DEF
  `RST_EN_FF(clkgt, reset, write_around_cnt_up, write_around_cnt,      write_around_cnt_next,      4'b0)
  `RST_EN_FF(clkgt, reset, write_around_cnt_up, write_around_throttle, write_around_throttle_next, 1'b0)

  // Throttle write arounds
  assign write_around_throttle_next = (write_around_cnt_next == 4'b1000);

  // Decrement counter
  assign opcode_is_mine =  (et_link_core_miss_id_t'(l2_resp.id[`ET_CORE_MISS_ID_RANGE])== ET_LINK_Core_Miss_Id_Reduce) ||
                           (et_link_core_miss_id_t'(l2_resp.id[`ET_CORE_MISS_ID_RANGE])== ET_LINK_Core_Miss_Id_Tensor_Store_Coop);
  assign write_around_cnt_dec = l2_resp_valid && opcode_is_mine &&
                               ((l2_resp.opcode == ET_LINK_RSP_Ack) || (l2_resp.opcode == ET_LINK_RSP_Err));

  always_comb begin
    // Updates counter
    write_around_cnt_up   = 1'b0;
    write_around_cnt_next = write_around_cnt;
    if (last_packet_row) begin
      write_around_cnt_up   = 1'b1;
      write_around_cnt_next = write_around_cnt + 4'b1;
    end

    // Write around coming back
    if (write_around_cnt_dec) begin
      write_around_cnt_up = 1'b1;
      if (write_around_cnt_next != 4'b0) begin
        write_around_cnt_next = write_around_cnt_next - 4'b1;
      end
    end

    // Outstanding wite around operations or tensor store still working
    tensorstore_busy = (write_around_cnt != 4'b0) || (!reduce_ready && !req_info.reduce);
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Bus error
  ////////////////////////////////////////////////////////////////////////////////
  logic resp_is_error;

  assign resp_is_error = l2_resp_valid && opcode_is_mine && (l2_resp.opcode == ET_LINK_RSP_Err);
  assign bus_err = {resp_is_error & req_info.thread_id, resp_is_error & ~req_info.thread_id};

  ////////////////////////////////////////////////////////////////////////////////
  // Clock gating
  ////////////////////////////////////////////////////////////////////////////////

  // Activate clock once a new request arrives and until everything is completed
  assign clkgt_en = new_req | !reduce_ready | tensorstore_busy | reset | (|err_flags);

  et_clk_gate cgate_re(
    .enclk ( clkgt                         ),
    .en    ( clkgt_en | chicken_bit_dcache ),
    .clk   ( clock                         ),
    .te    ( 1'b0                          )  // DFT has to connect test signals here
  );


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // Debug signals
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  assign csr_debug_signals = fsm_state;
  /* VERIF-5542
  assign csr_debug_signals = {
             //csr_ts1
             pipe_used_pre,
             tensorstore_count,
             ba_op_fifo_valid,
             ba_op_fifo_full,
             ba_alloc_fifo_val,
             ba_alloc_fifo_full,
             address_fifo_full,
             address_for_l2_ready,
             write_around_throttle,
             s4_send_pending_pre,
             s4_send_pending,
             partner_ready_mask,
             address_use_idx,
             address_count,
             atrans_num_req,
             atrans_state,
             l2_req_inflight,
             got_ready_msg,
             ts_is_from_scp,
             //csr_ts0
             scp_idx,
             scp_entry,
             stride_entry,
             reg_count,
             req_info,
             fsm_state
         };
  */
  assign sm_match_debug_signals = `ZX(`DCACHE_DBG_SM_MATCH_BITS,
    {
      vpu_reduce_wait,
      atrans_en,
      new_partner_ready,
      new_req,
      s4_send,
      load_send_fifo_data,
      write_around_cnt_up,
      reduce_ready,
      reduce_start,
      tensorstore_busy,
      tensorstore_start,
      re_l2_req_valid & re_l2_req_ready,
      ts_l2_req_valid & ts_l2_req_ready,
      l2_resp_valid,
      s3_inst_op_val,
      s2_inst_op_val,
      s1_inst_op_val,
      pipe_used_pre,
      pipe_used,
      s0_inst_send_val,
      s1_inst_send_val,
      s2_inst_send_val,
      s3_inst_send_val,
      ba_read_en,
      ts_scp_read
    });

  assign sm_filter_debug_signals = `ZX(`DCACHE_DBG_SM_FILTER_BITS,
    {
      vpu_reduce_wait,
      vpu_ctrl,
      atrans_en,
      new_partner_ready,
      new_req,
      s4_send,
      re_l2_req_valid,
      ts_l2_req_valid,
      re_l2_req_ready,
      ts_l2_req_ready,
      l2_resp_valid,
      l2_req.opcode,
      l2_req.subopcode,
      l2_req.address,
      l2_req.data[3:0],
      l2_req.size,
      l2_req.qwen,
      reduce_ready,
      reduce_start,
      tensorstore_busy,
      tensorstore_start,
      s3_inst_op_val,
      s2_inst_op_val,
      s1_inst_op_val,
      pipe_used_pre,
      pipe_used,
      s0_inst_send_val,
      s1_inst_send_val,
      s2_inst_send_val,
      s3_inst_send_val,
      ba_read_en,
      fsm_state,
      req_info,
      ts_scp_read
    });

  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(3)]   = `DCACHE_DBG_SM_DATA_GROUP_BITS'b0;

  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(2)]   = `DCACHE_DBG_SM_DATA_GROUP_BITS'b0;

  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(1)]   = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
    {
      ba_op_fifo_valid,
      ba_op_fifo_full,
      ba_alloc_fifo_val,
      ba_alloc_fifo_full,
      address_fifo_full,
      address_fifo_full,
      address_for_l2_ready,
      write_around_throttle,
      s4_send_pending,
      tensorstore_count,
      ts_is_from_scp,
      ts_scp_read_way,
      ts_scp_read_addr,
      ts_col_size_is_128b,
      partner_ready_mask,
      got_ready_msg,
      l2_req_inflight,
      atrans_state,
      atrans_num_req,
      address_count,
      address_use_idx,
      addr,
      req_info,
      reg_count,
      fsm_state
    });
  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(0)]   = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
    {
      stride_addr,
      stride_entry,
      scp_entry,
      scp_idx,
      l2_req.opcode,
      l2_req.subopcode,
      l2_req.address,
      l2_req.data[3:0],
      l2_req.size,
      l2_req.qwen,
      ba_read_addr
    });

endmodule

