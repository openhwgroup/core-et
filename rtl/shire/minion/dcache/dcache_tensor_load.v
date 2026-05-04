// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_tensor_load #(
  parameter MODULE_IDX = 0
) (
  // System signals
  input  logic                                      clock,
  input  logic                                      reset,
  input  logic [`MIN_PER_S_R]                       shire_min_id,
  // Chicken bit clock gate enable
  input  logic                                      chicken_bit_dcache,
  // Request port for new prefetch
  output logic                                      busy,
  output logic [1:0]                                id_complete,
  input  core_dcache_tensor_ctrl                    tensor_ctrl,
  // Signals to control s0 to s2 addr to do prefetches and override pipeline
  // value with ML value.
  input  logic                                      s0_available,
  output logic                                      s0_addr_load,
  output logic [`VA_RANGE]                          s0_addr,
  output logic                                      s0_addr_msb_err,
  output logic                                      s0_thread_id,
  input  logic                                      s1_available,
  output logic                                      s1_addr_load,
  input  logic                                      s1_tlb_wait,
  input  logic                                      s1_tlb_fail,
  input  logic [`PA_RANGE]                          s2_addr,
  // Port to request cacheline directly to L2 (cooperative TL) or to L2 common arbiter (non cooperative TL, MH, TEX)
  input  logic                                      l2_req_ready_coop,
  output logic                                      l2_req_valid_coop,
  input  logic                                      l2_req_ready_no_coop,
  output logic                                      l2_req_valid_no_coop,
  output et_link_minion_miss_req_info_t             l2_req,
  // Answer from L2
  input  logic                                      l2_resp_valid,
  input  et_link_minion_rsp_info_t                  l2_resp,
  // Refill control
  output logic [`DCACHE_WAY_RANGE]                  load_way,
  output logic [`DCACHE_LRAM_ADDR_RANGE]            load_addr,
  output logic [`CORE_L2_BLOCK_EXT_RANGE]           load_data,
  output logic [`DCACHE_LRAM_BANK_RANGE]            load_valid,
  output logic                                      load_valid_pre,
  input  logic                                      load_ready,
  output logic                                      load_capture,
  // TENB logic
  input  logic                                      tfma_enabled,
  input  logic                                      tenb_tstore_busy,
  input  logic                                      tenb_credit,
  input  logic [`DCACHE_TENB_R]                     tenb_credit_entry,
  input  logic                                      tenb_written,
  output logic                                      tenb_ready,
  output logic                                      tenb_flush,
  // Management of VM status
  input  minion_vm_status [`CORE_NR_THREADS-1:0]    vm_status,
  output minion_vm_status                           vm_status_tl,
  // Error output
  output logic [`DCACHE_TL_ERROR_BITS-1:0]          err_flags,
  // For performance counters
  output logic                                      tl_start,
  output logic                                      tl_op,
  // Bus error
  output logic [`CORE_NR_THREADS-1:0]               bus_err,
  // For debug signals
  output logic [`DCACHE_DBG_CSR_TL_SIZE-1:0]        csr_debug_signals,
  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]      sm_match_debug_signals,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]     sm_filter_debug_signals,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]       sm_data_debug_signals
);

  ////////////////////////////////////////////////////////////////////////////////
  // MMPrefetch
  // This instruction prefetches the specified number of L2 lines starting at
  // a given address, increased some specified "stride" length, and writing
  // into a given scratchpad (dcache LRAM) destination.
  // Three operation modes are accepted:
  // 1) Without transformation:
  //      L2 lines are written directly into scratchpad
  // 2) With interleaving
  //      Some parts of the requested L2 lines are stored internally and then
  //      data are interleaved and finally stored into memory
  // 3) With transposition
  //      Similar to interleaving but with a different data ordering (matrix
  //      transpose)
  ////////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////////////
  // Control frontend
  ////////////////////////////////////////////////////////////////////////////////
  // This part uses the "Request control", "Interleave control" and "Transpose
  // control" parts (see below) to store the requested information into scratchpad
  // If no transformation is required, only the "Request control" part is used
  ////////////////////////////////////////////////////////////////////////////////

  `define TRANS_SPLIT_COUNT_BITS 3
  `define TRANS_SPLIT_CYCLES     8
  `define TRANS_SPLIT_TERM_COUNT (`TRANS_SPLIT_CYCLES-1)

  // 16 x 8 = 128 bytes (1k bit) = 2 full 512b L2 line, 8 x 64b each
  `define DCACHE_TL_TMP_DATA_BLOCKS 16
  `define DCACHE_TL_INT_DATA_BLOCKS 8
  `define DCACHE_TL_TRP_DATA_BLOCKS 16

  typedef enum logic [2:0]
  {
    ML_CTRL_Idle             = 3'b000,    // Default idle state
    ML_CTRL_Wait_Start       = 3'b001,    // Wait for convolutional bits to be ready or for previous operation to finish
    ML_CTRL_NoT_Req          = 3'b010,    // State for no transformation, plain, writes
    ML_CTRL_Int_Trans_Flush  = 3'b011,    // Base state to implement interleaves for MODULE_IDX 0 / Flush state for MODULE_IDX 1
    ML_CTRL_Int_Req          = 3'b100,    // Interleave makes request to download lines
    ML_CTRL_Int_Int          = 3'b101,    // Interleave makes data interleaving and writes to scratchpad
    ML_CTRL_Tra_Req          = 3'b110,    // Transpose makes request to download lines
    ML_CTRL_Tra_Tra          = 3'b111     // Transpose makes data transposition and writes to scratchpad
  }
  dcache_tl_ctrl_state;

  logic                                         tensor_ctrl_start;
  logic                                         tensor_ctrl_restart;
  logic                                         start_pulse;                 // Delayed version of tensor_ctrl_start masked by not restart
  logic                                         start_pulse_pre;             // Delayed version of tensor_ctrl_start

  logic                                         request_control_start;       // Signals from main control to request control
  logic                                         request_control_store_addr;
  logic [`VA_RANGE]                             request_control_stride;
  logic                                         request_control_use_tmask;
  logic                                         request_control_use_coop;
  logic [`TENSOR_MASK_BITS-1:0]                 request_control_tmask_bits;
  logic [`DCACHE_TENSOR_CTRL_NLINES_RANGE]      request_control_num_lines;
  logic [`VA_RANGE]                             request_control_address;
  logic [`DCACHE_TENSOR_CTRL_DEST_RANGE]        request_control_scp_dest;    // Scratchpad destination
  logic                                         request_control_ending;      // Flag from "Request control" to indicate end of operation
  logic                                         request_control_ending_l2;   // Flag from "Request control" to indicate end of operation including L2 operations
  logic                                         request_control_flush;       // Request to finish operations till pipeline is flushed
  logic                                         request_control_flush_done;  // Flush complete
  logic [`DCACHE_SET_RANGE]                     request_control_clear_set;
  logic [`DCACHE_WAY_RANGE]                     request_control_clear_way;
  logic [`DCACHE_WAY_RANGE]                     request_control_load_way;    // Refill control from request control
  logic [`DCACHE_LRAM_ADDR_RANGE]               request_control_load_addr;
  logic                                         request_control_l2_group_id; // Identifier to group request to L2
  logic                                         request_control_l2_op_idx;   // Operation idx, changes in every new operation
  logic                                         request_control_thread_id;   // Thread that initiated the operation

  logic                                         save_tmask_pending;
  logic                                         save_tmask_pending_next;

  logic                                         save_tmask_bits;             // Clock enables
  logic                                         ctrl_en;

  dcache_tl_ctrl_state                          ctrl_state;                  // State machine states
  dcache_tl_ctrl_state                          ctrl_state_next;
  logic                                         ctrl_state_changed;

  logic [`VA_RANGE_CA]                          tensor_load_stride;          // Stride for tensor load operations (cache aligned)

  logic                                         trans_none;                  // No transformation
  logic                                         trans_int8;                  // Interleave 8 bits
  logic                                         trans_int16;                 // Interleave 16 bits
  logic                                         trans_trp8;                  // Transpose 8 bits
  logic                                         trans_trp16;                 // Transpose 16 bits
  logic                                         trans_trp32;                 // Transpose 32 bits
  tensorload_offset                             trans_offset;                // Offset in multiples of 16 bytes (0, 16, 32 or 48)
  logic                                         trans_offset_valid;          // Detects validity of provided offset
  logic                                         trans_size_valid;            // Detects validity of provided size

  logic                                         trans_size8;                 // Transformation size is 8 bits
  logic                                         trans_size16;                // Transformation size is 16 bits

  logic [`DCACHE_TENSOR_CTRL_NLINES_RANGE]      trans_nlines;                // Number of lines (-1) to transform
  logic [`VA_RANGE]                             trans_stride;                // Address increment stride
  logic [`DCACHE_TENSOR_CTRL_DEST_RANGE]        trans_scp_dest;              // Initial scratchpad destination of downloaded data
  logic                                         trans_use_tmask;             // Use tensor mask
  logic                                         trans_use_coop;              // Use cooperative load
  logic [`TENSOR_MASK_BITS-1:0]                 trans_tmask_bits;            // Tensor mask bits (which loads are done and which are skipped)

  logic [`DCACHE_TENSOR_CTRL_NLINES_RANGE]      shared_count;                // Counter for interleave loops or transposed lines
  logic [`DCACHE_TENSOR_CTRL_NLINES_RANGE]      shared_count_next;
  logic [`TMASK_COUNT_RANGE]                    shared_tmask_idx;            // Index for tensor mask bits in the counter
  logic [`DCACHE_TENSOR_CTRL_DEST_RANGE]        shared_dest;                 // Destination into scratchpad for interleave operations
  logic [`DCACHE_TENSOR_CTRL_DEST_RANGE]        shared_dest_next;
  logic                                         shared_load_idx;             // Index for first or second pass of data fill
  logic                                         shared_load_idx_next;
  logic [`DCACHE_SET_RANGE]                     shared_clear_set;            // Tag interface data
  logic [`DCACHE_WAY_RANGE]                     shared_clear_way;
  logic [`DCACHE_WAY_RANGE]                     shared_load_way;             // Refill data
  logic [`DCACHE_LRAM_ADDR_RANGE]               shared_load_addr;
  logic [`CORE_L2_BLOCK_EXT_RANGE]              shared_load_data;            // Data to write into LRAM (256b)

  logic                                         interleave_clear_valid;      // Tags interface for the interleave control block
  logic                                         interleave_clear_valid_next;
  logic                                         interleave_clear_valid_cken;
  logic [`DCACHE_LRAM_BANK_RANGE]               interleave_load_valid;       // Valid signal to write data into LRAM

  logic [`DCACHE_LRAM_BANK_RANGE]               transpose_load_valid;        // Valid signal to write data into LRAM, with masking
  logic                                         transpose_load_valid_single;
  logic [`DCACHE_LRAM_BANK_RANGE]               transpose_load_valid_mask;

  logic [`TRANS_SPLIT_COUNT_BITS-1:0]           trans_split_count;           // Counter to split operations in multiple stages
  logic [`TRANS_SPLIT_COUNT_BITS-1:0]           trans_split_count_next;

  logic                                         drain_load_capture;          // Indication that there are outstanding L2 requests that need to go into tmp buffer
  logic                                         drain_load_capture_next;     // once a Transpose or Interleave operation has been cancelled because of an error

  logic                                         l2_resp_is_for_me;           // Flag indicating that l2 response goes to this module
  logic                                         l2_req_ready;                // Flag to signal that neigh is ready for l2 requests
  logic                                         l2_req_valid;                // Flag to signal that neigh can receive the l2 request

  logic                                         err_unlock_flag;             // Error flags and their clear/set/update controls
  logic                                         err_transf_flag;
  logic                                         err_transf_set;
  logic                                         err_offset_flag;
  logic                                         err_offset_set;
  logic                                         err_mem;                     // Error detected during a memory operation
  logic                                         err_mem_next;
  logic                                         err_mem_sticky;              // Keep a sticky bit to ensure main control also ends upon error
  logic                                         err_mem_sticky_next;

  logic [`DCACHE_TL_TMP_DATA_BLOCKS-1:0][`DCACHE_LRAM_DATA_RANGE] tmp_data;
  logic [`DCACHE_TL_TMP_DATA_BLOCKS-1:0][`DCACHE_LRAM_DATA_RANGE] tmp_data_next;

  logic [`SHIRE_COOP_DATA_RANGE]                tensor_coop_data;            // Saved cooperative data when starting operation
  logic [`SHIRE_COOP_DATA_RANGE]                tensor_coop_data_next;
  logic [`SHIRE_COOP_DATA_RANGE]                tensor_coop_data_sequenced;  // Cooperative data to deliver to L2 request with sequence number

  logic                                         id_running;                  // Identifier of the running tensor operation
  logic                                         thread_id;                   // Thread that started the operation

  logic [`MIN_PER_N_R]                          minion_id;
  logic [`NUM_NEIGH_R]                          neigh_id;

  logic                                         clkgt;                       // Gated clock
  logic                                         clkgt_en;                    // Clock enable signal

  logic [(`VPU_TENB_SIZE/2)-1:0]                tenb_avail_entry;            // Per TENB entry is available to be written
  logic [(`VPU_TENB_SIZE/2)-1:0]                tenb_avail_entry_next;
  logic                                         tenb_avail_all;              // All the entries are available (VPU buffer empty)
  logic                                         tenb_avail_all_next;
  logic                                         tfma_started;                // The TFMA for this TENB tensorload started
  logic                                         tfma_started_next;
  logic                                         tfma_active_start;           // The TFMA was active when TENB tensorload started
  logic                                         tfma_active_start_next;

  logic                                         l2_interface_busy;           // Indicates there are outstanding requests to L2 not completed
  logic                                         l2_interface_busy_prev;      // Delayed version of the previous signal

  // -----------------------------------------------------------------------------

  // Local information storage for static settings

  tensorload_transform                          tensor_ctrl_transform;
  tensorload_offset                             tensor_ctrl_offset;
  logic                                         tensor_ctrl_use_tmask;
  logic                                         tensor_ctrl_use_coop;
  logic [`DCACHE_TENSOR_CTRL_NLINES_RANGE]      tensor_ctrl_num_lines;
  logic                                         tensor_ctrl_tenb;
  logic [`DCACHE_TENSOR_CTRL_DEST_RANGE]        tensor_ctrl_scp_dest;
  logic                                         tenb_no_extra_credits_cond1;
  logic                                         tenb_no_extra_credits_cond3;

  // Ready signal for cooperative and non cooperative tensor loads

  assign l2_req_ready = tensor_ctrl_use_coop ? l2_req_ready_coop: l2_req_ready_no_coop;

  // Mask start signal depending on MODULE_IDX and input mode bit
  assign tensor_ctrl_start = tensor_ctrl.start && (MODULE_IDX == tensor_ctrl.ctrl.tl_sel);
  assign tensor_ctrl_restart = tensor_ctrl.restart_b && (MODULE_IDX == 1);

  // In cooperative loads, send cooperative ID and minion mask in the data field
  assign minion_id = shire_min_id[`MIN_PER_N_R];
  assign neigh_id  = shire_min_id[`MIN_PER_N_L+:`NUM_NEIGH_L];

  assign tensor_coop_data_next = ((`MIN_PER_N'b1 << neigh_id) << `SHIRE_COOP_NEIGH_MASK_START)  | // Own Neigh mask
                                  (tensor_ctrl.coop.neigh_mask << `SHIRE_COOP_NEIGH_MASK_START) | // Provided Neigh mask
                                  (tensor_ctrl.coop.id << `SHIRE_COOP_ID_START)                 | // Provided ID
                                  (`MIN_PER_N'b1 << minion_id)                                  | // Own Minion mask
                                  tensor_ctrl.coop.minion_mask;                                   // Provided Minion mask

  // Store necessary static data for tensor load

  //       CLK     EN                 DOUT                    DIN
  generate
    if (MODULE_IDX == 0) begin
      `EN_FF ( clkgt,  tensor_ctrl_start, tensor_ctrl_use_tmask,  tensor_ctrl.ctrl.use_tmask       )
      assign tenb_no_extra_credits_cond1 = 1'b0;
      assign tenb_no_extra_credits_cond3 = 1'b0;
    end else begin
      assign tensor_ctrl_use_tmask = 1'b0;
      `EN_FF ( clkgt,  tensor_ctrl_start, tenb_no_extra_credits_cond1,  tensor_ctrl.stride[1]      )
      `EN_FF ( clkgt,  tensor_ctrl_start, tenb_no_extra_credits_cond3,  tensor_ctrl.stride[2]      )
    end
  endgenerate
  `EN_FF ( clkgt,  tensor_ctrl_start, tensor_ctrl_use_coop,   tensor_ctrl.ctrl.use_coop        )
  `EN_FF ( clkgt,  tensor_ctrl_start, tensor_ctrl_num_lines,  tensor_ctrl.ctrl.num_lines       )
  `EN_FF ( clkgt,  tensor_ctrl_start, tensor_ctrl_scp_dest,   tensor_ctrl.ctrl.scp_dest        )
  `EN_FF ( clkgt,  tensor_ctrl_start, tensor_load_stride,     tensor_ctrl.stride[`VA_RANGE_CA] )
  `EN_FF ( clkgt,  tensor_ctrl_start, id_running,             tensor_ctrl.stride[0]            )
  `EN_FF ( clkgt,  tensor_ctrl_start, thread_id,              tensor_ctrl.thread_id            )
  `EN_FF ( clkgt,  tensor_ctrl_start, tensor_coop_data,       tensor_coop_data_next            )

  // Parameter MODULE_IDX defines whether tensor can go to TENB or not
  // Operations that go to TENB:
  //   - do not accept transformation
  //   - do not accept any offset
  generate
    if (MODULE_IDX == 0) begin
      assign tensor_ctrl_tenb = 1'b0;
      //       CLK     EN                 DOUT                    DIN
      `EN_FF ( clkgt,  tensor_ctrl_start, tensor_ctrl_offset,     tensor_ctrl.ctrl.offset   )
      `EN_FF ( clkgt,  tensor_ctrl_start, tensor_ctrl_transform,  tensor_ctrl.ctrl.transform)
    end else begin
      assign tensor_ctrl_tenb = 1'b1;
      assign tensor_ctrl_offset = TransOffset0;
      assign tensor_ctrl_transform = TensorTrans_None;
    end
  endgenerate

  // Store address information in the "request control" registers
  assign request_control_store_addr = tensor_ctrl_start;
  assign request_control_address    = {tensor_ctrl.ctrl.addr, 6'b0};

  // Start pulse once data are stored into registers

  //       CLK    RST    EN  DOUT           DIN                 DEF
  `RST_FF (clkgt, reset,     start_pulse_pre,   tensor_ctrl_start,  1'b0)

  assign start_pulse = start_pulse_pre & ~tensor_ctrl_restart;

  // Store VM status for memory access
  //       CLK     EN           DOUT          DIN
  `EN_FF ( clkgt,  start_pulse, vm_status_tl, (thread_id == 1'b1)? vm_status[1] : vm_status[0] )

  // For performance counters
  assign tl_start = start_pulse;
  assign tl_op    = l2_req_valid && l2_req_ready;

  // -----------------------------------------------------------------------------

  // Put information in meaningful short variables

  // Decode type of transformation
  assign trans_none   = (tensor_ctrl_transform == TensorTrans_None );
  assign trans_int8   = (tensor_ctrl_transform == TensorTrans_Int8 );
  assign trans_int16  = (tensor_ctrl_transform == TensorTrans_Int16);
  assign trans_trp8   = (tensor_ctrl_transform == TensorTrans_Trp8 );
  assign trans_trp16  = (tensor_ctrl_transform == TensorTrans_Trp16);
  assign trans_trp32  = (tensor_ctrl_transform == TensorTrans_Trp32);
  assign trans_offset = (trans_int8  | trans_trp8 ) ? tensor_ctrl_offset :                             // Any of the 4 values (0, 16, 32, 48) is fine
                        (trans_int16 | trans_trp16) ? tensorload_offset'(tensor_ctrl_offset & 2'b10) : // Only accept 0 and 32, 16->0, 48->32
                                                      TransOffset0;                                    // Only 0 is a valid value for size 32

  // Detect valid transformation parameters
  always_comb begin
    trans_offset_valid = 1'b1;
    trans_size_valid = (trans_none | trans_int8 | trans_int16 | trans_trp8 | trans_trp16 | trans_trp32);
  end

  // Decode transformation size
  assign trans_size8  = trans_int8  || trans_trp8;
  assign trans_size16 = trans_int16 || trans_trp16;

  // Decode other relevant information
  assign trans_nlines    = tensor_ctrl_num_lines;      // Internal number of lines (-1) ... can't be more than external request
  assign trans_stride    = {tensor_load_stride,6'b0};  // Cache aligned stride
  assign trans_scp_dest  = tensor_ctrl_scp_dest;       // Destination in L1 Scp (16 sets and 4 ways)
  assign trans_use_tmask = tensor_ctrl_use_tmask;
  assign trans_use_coop  = tensor_ctrl_use_coop;

  // Select values for shared operation

  // Pick index to tensor mask index from counters
  assign shared_tmask_idx = shared_count;

  // *****************************************************************************
  // State machine
  // *****************************************************************************
  // Aux signals
  logic  ctrl_start;
  logic  ctrl_start_tmask_ready;
  logic  ctrl_start_tmask_wait;
  logic  ctrl_start_previous_ready;
  logic  ctrl_start_previous_wait;

  logic  ctrl_wait_tmask_ready;
  logic  ctrl_wait_previous_ready;

  // Conditions to keep in a wait state while Tmask is not ready (if used) or
  // we need to wait for the previous operation to have completed the L2 operations
  assign ctrl_start                = start_pulse && trans_size_valid && trans_offset_valid;
  assign ctrl_start_tmask_ready    = ctrl_start && (tensor_ctrl.tmask_ready || !tensor_ctrl_use_tmask);
  assign ctrl_start_tmask_wait     = ctrl_start && !tensor_ctrl.tmask_ready &&  tensor_ctrl_use_tmask;
  assign ctrl_start_previous_ready = ctrl_start && (trans_none || !l2_interface_busy || (MODULE_IDX == 1));

  // For TL0, any operation that requires transformation has to wait if there are outstanding L2 operations
  // For TL1, any new operation has to wait if TFMA is not enabled yet and there is an outstanding TStore or TReduce
  assign ctrl_wait_previous_ready  = (MODULE_IDX == 0) ? (trans_none || !l2_interface_busy) :
                                                         ((tenb_avail_all && !l2_interface_busy) || !tenb_tstore_busy || tfma_enabled);
  assign ctrl_start_previous_wait  = ctrl_start && !ctrl_wait_previous_ready;

  assign ctrl_wait_tmask_ready     = !save_tmask_pending;

// State machine
  // State machine
  always_comb begin
    // Defaults depend on MODULE_IDX
    ctrl_state_next = ctrl_state;
    if (MODULE_IDX == 0) begin
      shared_count_next = shared_count;
      shared_dest_next = shared_dest;
      shared_load_idx_next = shared_load_idx;
      trans_split_count_next = trans_split_count;
      drain_load_capture_next = drain_load_capture;
    end else begin
      shared_count_next = {`DCACHE_TENSOR_CTRL_NLINES_BITS{1'b0}};
      shared_dest_next = {(`DCACHE_TENSOR_CTRL_DEST_BITS){1'b0}};
      shared_load_idx_next = 1'b0;
      trans_split_count_next = `TRANS_SPLIT_COUNT_BITS'd0;
      drain_load_capture_next = 1'b0;
    end

    // Main idle/wait states
    if ((ctrl_state == ML_CTRL_Idle) && (ctrl_start_tmask_wait || ctrl_start_previous_wait)) begin
      ctrl_state_next = ML_CTRL_Wait_Start;
    end else if (((ctrl_state == ML_CTRL_Idle)       && ctrl_start_tmask_ready && ctrl_start_previous_ready)
              || ((ctrl_state == ML_CTRL_Wait_Start) && ctrl_wait_tmask_ready  && ctrl_wait_previous_ready))
    begin
      if (trans_none) begin
        ctrl_state_next = ML_CTRL_NoT_Req;
      end else if ((trans_int8 || trans_int16) && (MODULE_IDX == 0)) begin
        ctrl_state_next = ML_CTRL_Int_Trans_Flush;
        shared_count_next = `DCACHE_TENSOR_CTRL_NLINES_BITS'b0;
      end else if ((trans_trp8 || trans_trp16 || trans_trp32) && (MODULE_IDX == 0)) begin
        ctrl_state_next = ML_CTRL_Tra_Req;
        trans_split_count_next = `TRANS_SPLIT_COUNT_BITS'd0;
      end
    end

    // Plain tensor load state with no transformation
    if (ctrl_state == ML_CTRL_NoT_Req) begin
      if (request_control_ending) begin
        ctrl_state_next = ML_CTRL_Idle;
      end
    end

    // Interleave and transpose operations only valid for MODULE_IDX 0
    if (MODULE_IDX == 0) begin
      // Interleave operations

      // State used as base to apply interleaving transformations
      if (ctrl_state == ML_CTRL_Int_Trans_Flush) begin
        if (shared_count <= trans_nlines) begin
          ctrl_state_next = ML_CTRL_Int_Req;
        end else begin
          ctrl_state_next = ML_CTRL_Idle; // Cannot be excluded with Verdi. DVCEX032. VERIF-1344
        end

        if (shared_count == 0) begin
          shared_dest_next = trans_scp_dest;
        end else begin
          shared_dest_next = shared_dest + 1'b1;
        end

        shared_load_idx_next = 1'b0;
      end

      // State to download lines to interleave
      if (ctrl_state == ML_CTRL_Int_Req) begin
        if (drain_load_capture) begin // Wait to go to IDLE intil L2 interface is drained
          if (!l2_interface_busy) begin
            ctrl_state_next = ML_CTRL_Idle;
            drain_load_capture_next = 1'b0;
          end
        end else if (request_control_ending_l2) begin // This is a single pulse indication
          // In case of memory error during memory transaction, just remember to drain L2 operations
          if (err_mem_sticky | err_mem_sticky_next) begin
            drain_load_capture_next = 1'b1;
          end
          // Unmasked lines, go to interleave data state
          else if (!trans_use_tmask || trans_tmask_bits[shared_tmask_idx]) begin
            ctrl_state_next = ML_CTRL_Int_Int;
          end
          // Otherwise check if additional lines have to be processed
          else begin
            if (shared_count == trans_nlines) begin
              ctrl_state_next = ML_CTRL_Idle;
            end else begin
              ctrl_state_next = ML_CTRL_Int_Trans_Flush;
              shared_count_next = shared_count + `DCACHE_TENSOR_CTRL_NLINES_BITS'b1;
            end
          end
        end
      end

      // State to interleave data and write it into LRAM
      if (ctrl_state == ML_CTRL_Int_Int) begin
        if (load_ready) begin
          shared_load_idx_next = ~shared_load_idx;
          if (shared_load_idx) begin
            // Check if more lines have to beprocessed
            if (shared_count == trans_nlines) begin
              ctrl_state_next = ML_CTRL_Idle;
            end else begin
              ctrl_state_next = ML_CTRL_Int_Trans_Flush;
              shared_count_next = shared_count + `DCACHE_TENSOR_CTRL_NLINES_BITS'b1;
            end
          end
        end
      end

      // Transpose operations

      if (ctrl_state == ML_CTRL_Tra_Req) begin
        if (drain_load_capture) begin // Wait to go to IDLE intil L2 interface is drained
          if (!l2_interface_busy) begin
            ctrl_state_next = ML_CTRL_Idle;
            drain_load_capture_next = 1'b0;
          end
        end else if (request_control_ending_l2) begin // This is a single pulse indication
          // In case of memory error during memory transaction, just remember to drain L2 operations
          if (err_mem_sticky | err_mem_sticky_next) begin
            drain_load_capture_next = 1'b1;
          end else begin
            ctrl_state_next = ML_CTRL_Tra_Tra;
          end
        end

        shared_count_next = `DCACHE_TENSOR_CTRL_NLINES_BITS'b0;
        shared_dest_next = trans_scp_dest;
        shared_load_idx_next = trans_split_count[`TRANS_SPLIT_COUNT_BITS-1];
      end

      if (ctrl_state == ML_CTRL_Tra_Tra) begin
        if (load_ready) begin
          shared_count_next = shared_count + `DCACHE_TENSOR_CTRL_NLINES_BITS'b1;
          shared_dest_next = shared_dest + 1'b1;
        end

        if (load_ready && (shared_count == trans_nlines)) begin
          if (trans_split_count == `TRANS_SPLIT_TERM_COUNT) begin
            ctrl_state_next = ML_CTRL_Idle;
          end else begin
            ctrl_state_next = ML_CTRL_Tra_Req;
          end

          trans_split_count_next = trans_split_count + `TRANS_SPLIT_COUNT_BITS'd1;
        end
      end

    end // MODULE_IDX == 0

    // Re-start states only available for MODULE_IDX 1
    if (MODULE_IDX == 1) begin
      // These transitions overwrite any previous ones.
      if (tensor_ctrl_restart) begin
        ctrl_state_next = ML_CTRL_Int_Trans_Flush;
      end

      if (ctrl_state == ML_CTRL_Int_Trans_Flush) begin
        if (request_control_flush_done) begin
          ctrl_state_next = ML_CTRL_Idle;
        end
      end

    end

    // Ensure that destination is always within scratchpad region
    if (shared_dest_next >= `DCACHE_TL_SCP_MAX_IDX) begin
      shared_dest_next = shared_dest_next - `DCACHE_TL_SCP_MAX_IDX;
    end

  end

  // -----------------------------------------------------------------------------

  // Detect writing into an unlocked cache line, wrong transformation value or wrong
  // offset for a given transformation value
  always_comb begin
    err_transf_set   = start_pulse & !trans_size_valid;

    err_offset_set   = start_pulse & !trans_offset_valid;
  end

  //       CLK    RST    DOUT             DIN             DEF
  `RST_FF (clkgt, reset, err_mem,         err_mem_next,  1'b0)
  `RST_FF (clkgt, reset, err_transf_flag, err_transf_set, 1'b0)
  `RST_FF (clkgt, reset, err_offset_flag, err_offset_set, 1'b0)

  // Externalize error flags
  always_comb begin
    err_unlock_flag   = 1'b0;
    err_flags = `DCACHE_TL_ERROR_BITS'b0;
    err_flags[`DCACHE_TL_ERROR_UNLOCK_LINE]       = err_unlock_flag;
    err_flags[`DCACHE_TL_ERROR_TRANS_VALID]       = err_transf_flag;
    err_flags[`DCACHE_TL_ERROR_WRONG_OFFST]       = err_offset_flag;
    err_flags[`DCACHE_TL_ERROR_ACCESS_FAULT_0]    = err_mem & ~thread_id;
    err_flags[`DCACHE_TL_ERROR_ACCESS_FAULT_1]    = err_mem & thread_id;
  end

  // Sticky error bit to ensure that state machines all finish operation
  // Clear bit when main state machine goes to Idle and set it when an error
  // is detected
  always_comb begin
    if (ctrl_state == ML_CTRL_Idle) begin
      err_mem_sticky_next = 1'b0;
    end else if (err_mem_next) begin
      err_mem_sticky_next = 1'b1;
    end else begin
      err_mem_sticky_next = err_mem_sticky;
    end
  end

  //       CLK    RST    DOUT             DIN                   DEF
  `RST_FF (clkgt, reset, err_mem_sticky,  err_mem_sticky_next,  1'b0)

  // *****************************************************************************
  // Muxing of signals in and out
  // *****************************************************************************

  // Send a start pulse to "Request control" when going to any of the *_Req states
  assign request_control_start     = ctrl_state_changed && ((ctrl_state == ML_CTRL_NoT_Req) ||
                                                            (ctrl_state == ML_CTRL_Int_Req) ||
                                                            (ctrl_state == ML_CTRL_Tra_Req));

  // Send a flush request when entering the flush state
  assign request_control_flush = (MODULE_IDX == 1) ? (ctrl_state == ML_CTRL_Int_Trans_Flush) : 1'b0;

  // Information to "request control" depends on the requesting state
  always_comb begin
    if (ctrl_state == ML_CTRL_Int_Req) begin
      request_control_num_lines  = trans_size8 ?                        // 4 or 2 lines depending on size
                                   `DCACHE_TENSOR_CTRL_NLINES_BITS'd3 :
                                   `DCACHE_TENSOR_CTRL_NLINES_BITS'd1;
      request_control_use_tmask  = trans_use_tmask;                     // Using tmask, but single mask bit expands for all lines
      request_control_use_coop   = trans_use_coop;
      request_control_tmask_bits = {`TENSOR_MASK_BITS{trans_tmask_bits[shared_tmask_idx]}};
      request_control_stride     = trans_stride;                        // Default stride
    end else if (ctrl_state == ML_CTRL_Tra_Req) begin
      request_control_num_lines  = trans_size8 ?                        // 64, 32 or 16 lines depending on size, divided by split factor of 8
                                   `DCACHE_TENSOR_CTRL_NLINES_BITS'd7 :
                                   trans_size16?
                                   `DCACHE_TENSOR_CTRL_NLINES_BITS'd3 :
                                   `DCACHE_TENSOR_CTRL_NLINES_BITS'd1;
      request_control_use_tmask  = 1'b0;                                // Not using tmask
      request_control_use_coop   = trans_use_coop;
      request_control_tmask_bits = `TENSOR_MASK_BITS'b0;
      request_control_stride     = trans_stride;                        // Default stride
    end else begin // default for plain downloads (no transformation)
      request_control_num_lines  = trans_nlines;                    // All parameters are passed as they come in the command
      request_control_use_tmask  = trans_use_tmask;
      request_control_use_coop   = trans_use_coop;
      request_control_tmask_bits = trans_tmask_bits;
      request_control_stride     = trans_stride;
    end
  end

  // Common static information for all operations
  assign request_control_scp_dest = tensor_ctrl_scp_dest;

  // Group identifier for requests to L2. Use the ID of the running tensor
  assign request_control_l2_group_id = id_running;

  // Operation identifier changes in every new request
  logic update_l2_operation_id;
  assign update_l2_operation_id = ((ctrl_state == ML_CTRL_NoT_Req) && request_control_ending && (!request_control_use_tmask || (|request_control_tmask_bits)))    ||
                                  ((ctrl_state == ML_CTRL_Int_Req) && request_control_ending_l2 && (!request_control_use_tmask || (|request_control_tmask_bits))) ||
                                  ((ctrl_state == ML_CTRL_Tra_Req) && request_control_ending_l2)                                                                  ||
                                  ((ctrl_state == ML_CTRL_Int_Trans_Flush) && request_control_ending && (MODULE_IDX == 1));
  `RST_EN_FF (clkgt, reset, update_l2_operation_id, request_control_l2_op_idx, ~request_control_l2_op_idx, 1'b0)

  // Thread for the ongoing operations
  assign request_control_thread_id = thread_id;

  // -----------------------------------------------------------------------------

  // Interleave signals to tag interface
  assign interleave_clear_valid_next = (MODULE_IDX == 0) ? ((ctrl_state == ML_CTRL_Int_Trans_Flush) && (shared_count <= trans_nlines))  : 1'b0;
  assign interleave_clear_valid_cken = (MODULE_IDX == 0) ? ((ctrl_state == ML_CTRL_Int_Trans_Flush) || (ctrl_state == ML_CTRL_Int_Req)) : 1'b0;

  //          CLK    RST    EN                           DOUT                    DIN                          DEF
  `RST_EN_FF (clkgt, reset, interleave_clear_valid_cken, interleave_clear_valid, interleave_clear_valid_next, 1'b0)

  // Shared signals to tag interface.
  // Get set and way with limited range in case of scratchpad being enabled, or full-range otherwise
  assign shared_clear_set = get_set_from_scp_dest(shared_dest);
  assign shared_clear_way = get_way_from_scp_dest(shared_dest);

  // -----------------------------------------------------------------------------

  // Mux outpus to refill interface
  assign load_way     = (MODULE_IDX == 0) ? ((ctrl_state >= ML_CTRL_Int_Trans_Flush) ? shared_load_way  : request_control_load_way)
                                          : request_control_load_way;

  assign load_addr    = (MODULE_IDX == 0) ? ((ctrl_state >= ML_CTRL_Int_Trans_Flush) ? shared_load_addr : request_control_load_addr)
                                          : request_control_load_addr;

  assign load_data    = shared_load_data;
  assign load_valid   = interleave_load_valid | transpose_load_valid;
  assign load_capture = (ctrl_state == ML_CTRL_Int_Req) || (ctrl_state == ML_CTRL_Tra_Req);

  // Interleave signals to refill interface (write into all banks)
  assign interleave_load_valid  = {`DCACHE_LRAM_NUM_BANKS{(ctrl_state == ML_CTRL_Int_Int)}};

  // Transpose signals to refill interface
  assign transpose_load_valid_single = (ctrl_state == ML_CTRL_Tra_Tra) && (trans_tmask_bits[shared_tmask_idx] | !trans_use_tmask);
  assign transpose_load_valid_mask = `DCACHE_LRAM_NUM_BANKS'b1 << trans_split_count[1:0];
  assign transpose_load_valid = {`DCACHE_LRAM_NUM_BANKS{transpose_load_valid_single}} & transpose_load_valid_mask;

  // Pre-load to activate writing to memory in interleave and transpose operations
  assign load_valid_pre = (ctrl_state_next == ML_CTRL_Int_Int) || (ctrl_state_next == ML_CTRL_Tra_Tra);

  // Shared signals to refill interface
  assign shared_load_way  = shared_clear_way;
  assign shared_load_addr = {shared_clear_set, shared_load_idx, {`CORE_L2_BLOCK_EXT_ADDR_OFFSET_BITS{1'b0}}};

  // -----------------------------------------------------------------------------

  // Flag to indicate that tmask has to be saved
  always_comb begin
    save_tmask_pending_next = save_tmask_pending;

    // Set flag when going to Wait start and mask not ready
    if ((ctrl_state == ML_CTRL_Idle) && ctrl_start_tmask_wait) begin
      save_tmask_pending_next = 1'b1;
    end

    // Undet flag when waiting and tmask bits have already been saved
    if (save_tmask_bits) begin
      save_tmask_pending_next = 1'b0;
    end
  end

  // Save tmask bits when ready
  assign save_tmask_bits =   ((ctrl_state == ML_CTRL_Idle) && tensor_ctrl.tmask_ready && start_pulse)
                           ||((ctrl_state == ML_CTRL_Wait_Start) && save_tmask_pending && tensor_ctrl_use_tmask && tensor_ctrl.tmask_ready);

  // Enable control state machine while operating
  assign ctrl_en = (ctrl_state != ML_CTRL_Idle) | start_pulse | tensor_ctrl_restart;

  //          CLK    RST    EN               DOUT                 DIN                               DEF
  `RST_FF    (clkgt, reset,                  save_tmask_pending,  save_tmask_pending_next,          1'b0        )
  `EN_FF     (clkgt,        save_tmask_bits, trans_tmask_bits,    tensor_ctrl.tmask_bits                        )

  `RST_EN_FF (clkgt, reset, ctrl_en,         ctrl_state,          ctrl_state_next,                  ML_CTRL_Idle)
  `EN_FF     (clkgt,        ctrl_en,         ctrl_state_changed,  (ctrl_state_next != ctrl_state)               )

  `EN_FF     (clkgt,        ctrl_en,         shared_count,        shared_count_next                             )
  `RST_EN_FF (clkgt, reset, ctrl_en,         shared_dest,         shared_dest_next,                 'b0         )
  `EN_FF     (clkgt,        ctrl_en,         shared_load_idx,     shared_load_idx_next                          )

  `EN_FF     (clkgt,        ctrl_en,         trans_split_count,   trans_split_count_next                        )

  `RST_FF    (clkgt, reset,                  drain_load_capture,  drain_load_capture_next,          1'b0        )

  // Signals for shared l2 operation
  // -----------------------------------------------------------------------------

  logic [`DCACHE_TL_L2_TRANSFERS_RANGE]      new_l2_req;             // Ask for a new request to L2
  logic [`DCACHE_TL_L2_TRANSFERS_RANGE]      tl_load_idx;            // Stores lram address bit modified in each l2_resp_valid cycle (1-bit counter)
  logic [`DCACHE_TL_L2_TRANSFERS_RANGE]      tl_load_idx_next;
  logic                                      shared_l2_use_p2;       // Data is in L2 second pass
  logic [`DCACHE_TL_TRP_DATA_BLOCKS-1:0]     shared_l2_store;        // Store signal for used tmp_data blocks
  logic [`DCACHE_TL_L2_TRANSFERS_RANGE][3:0] l2i_stg_addr;           // Address where the line should be stored inside the buffers
  logic [`DCACHE_TL_L2_TRANSFERS_SEL_RANGE]  shared_stg_idx;         // Index for the array where we find the storage address
  logic [3:0]                                shared_l2_counter;      // Conter for L2 transfers, up to 8 lines (max fro transpose), split in two halfs each --> 4 bits
  logic [3:0]                                shared_l2_counter_next;
  logic                                      shared_l2_count;        // Increment/update counter
logic [3:0][`DCACHE_LRAM_DATA_RANGE]       shared_l2_sel_data;     // Data selected/muxed to be stored in the current L2 cycle

assign shared_l2_use_p2 = (trans_offset == TransOffset32) || (trans_offset == TransOffset48);
assign shared_l2_count  = (MODULE_IDX == 0) ? (((ctrl_state == ML_CTRL_Int_Trans_Flush) || ((ctrl_state == ML_CTRL_Int_Req) && |new_l2_req)) ||
                                               ((ctrl_state == ML_CTRL_Tra_Req) && (|new_l2_req || ctrl_state_changed)))
                                            : 1'b0;

// Select data to be stored. For every L2 data chunk (256b, 32 bytes), data index 0 and 1 are selected depending on offset
//  Size = 1 (byte)
//    l2_resp_data  (32 bytes) 0...............16............... (pass 1 or 2)
//    sel_data[0]   ( 8 bytes) xxxxxxxx                          (offset  0 or 32)
//    sel_data[1]   ( 8 bytes)         xxxxxxxx                  (offset  0 or 32)
//    sel_data[0]   ( 8 bytes)                  xxxxxxxx         (offset 16 or 48)
//    sel_data[1]   ( 8 bytes)                          xxxxxxxx (offset 16 or 48)
//  Size = 2 (word)
//    l2_resp_data  (32 bytes) 0...............16............... (pass 1 or 2)
//    sel_data[0]   ( 8 bytes) xxxxxxxx                          (offset  0 or 32)
//    sel_data[1]   ( 8 bytes)         xxxxxxxx                  (offset  0 or 32)
//    sel_data[2]   ( 8 bytes)                  xxxxxxxx         (offset  0 or 32)
//    sel_data[3]   ( 8 bytes)                          xxxxxxxx (offset  0 or 32)

assign shared_l2_sel_data[0] = ((trans_offset == TransOffset0) ||
                                (trans_offset == TransOffset32)) ? l2_resp.data[0*`DCACHE_LRAM_DATA_SIZE+:`DCACHE_LRAM_DATA_SIZE] :
                                                                   l2_resp.data[2*`DCACHE_LRAM_DATA_SIZE+:`DCACHE_LRAM_DATA_SIZE];
assign shared_l2_sel_data[1] = ((trans_offset == TransOffset0) ||
                                (trans_offset == TransOffset32)) ? l2_resp.data[1*`DCACHE_LRAM_DATA_SIZE+:`DCACHE_LRAM_DATA_SIZE] :
                                                                   l2_resp.data[3*`DCACHE_LRAM_DATA_SIZE+:`DCACHE_LRAM_DATA_SIZE];

assign shared_l2_sel_data[2] = l2_resp.data[2*`DCACHE_LRAM_DATA_SIZE+:`DCACHE_LRAM_DATA_SIZE];
assign shared_l2_sel_data[3] = l2_resp.data[3*`DCACHE_LRAM_DATA_SIZE+:`DCACHE_LRAM_DATA_SIZE];

always_comb begin
  if (MODULE_IDX == 0) begin
    if ((ctrl_state == ML_CTRL_Int_Trans_Flush) ||
        ((ctrl_state == ML_CTRL_Tra_Req) && ctrl_state_changed)) begin
      shared_l2_counter_next = 7'b0;
    end else begin
      shared_l2_counter_next = shared_l2_counter + 7'h2;
    end
  end else begin
    shared_l2_counter_next = 7'b0;
  end

  shared_stg_idx = l2_resp.id[`DCACHE_TL_L2_TRANSFERS_SEL_RANGE];

  // Signal to store data when we are in appropriate L2 resp phase
  // The store condition is generated in blocks of 16 bytes (i and i+1 indexes are equal)

  shared_l2_store = `DCACHE_TL_TMP_DATA_BLOCKS'b0;

  if (trans_int8 || trans_int16) begin
    // Bear in mind that shared_l2_counter can count at most for 4 L2 lines (0...7 L2 passes)
    for (integer i=0; i<`DCACHE_TL_INT_DATA_BLOCKS; i=i+2) begin // Counts 0, 2, 4, 6
      if ((shared_l2_use_p2 == tl_load_idx[shared_stg_idx]) && l2_resp_is_for_me) begin
        if (trans_int8) begin
          shared_l2_store[i] = (l2i_stg_addr[shared_stg_idx][2:1] == i[2:1]);    // groups of 2 (16 bytes)
        end else begin
          shared_l2_store[i] = (l2i_stg_addr[shared_stg_idx][1] == i[2]);        // groups of 4 (32 bytes)
        end
      end

      // Next block of 8 bytes
      shared_l2_store[i+1] = shared_l2_store[i];
    end
  end else begin
    // Bear in mind that shared_l2_counter can count at most for 8 L2 lines (0...15 L2 passes)
    for (integer i=0; i<`DCACHE_TL_TRP_DATA_BLOCKS; i=i+2) begin // Counts 0, 2, 4, 6, 8, 10, 12, 14
      if (trans_trp32 && l2_resp_is_for_me) begin
        shared_l2_store[i] = ((l2i_stg_addr[shared_stg_idx][1:0] | {1'b0,tl_load_idx[shared_stg_idx]}) == i[3:2]);    // groups of 8 (64 bytes), split in two passes
      end else if ((shared_l2_use_p2 == tl_load_idx[shared_stg_idx]) && l2_resp_is_for_me) begin
        if (trans_trp8) begin
          shared_l2_store[i] = (l2i_stg_addr[shared_stg_idx][3:1] == i[3:1]);    // groups of 2 (16 bytes)
        end else if (trans_trp16) begin
          shared_l2_store[i] = (l2i_stg_addr[shared_stg_idx][2:1] == i[3:2]);    // groups of 4 (32 bytes)
        end
      end else begin
        shared_l2_store[i]   = 1'b0;
      end

      // Next block of 8 bytes
      shared_l2_store[i+1] = shared_l2_store[i];
    end
  end
end

//      CLK    RST    EN               DOUT                   DIN
`EN_FF (clkgt,        shared_l2_count, shared_l2_counter, shared_l2_counter_next)


// Input to TMP DATA array
// -----------------------------------------------------------------------------

// Mux data into tmp_data
always_comb begin
  // Modify input depending on the transformation type
  if (trans_int8 | trans_trp8) begin
    for (integer i=0; i<`DCACHE_TL_TMP_DATA_BLOCKS; i=i+2) begin
      tmp_data_next[i+:2] = shared_l2_sel_data[1:0];     // 2 x 8 bytes = 16 bytes (half l2 response)
    end
  end else begin
    for (integer i=0; i<`DCACHE_TL_TMP_DATA_BLOCKS; i=i+4) begin
      tmp_data_next[i+:4] = shared_l2_sel_data;          // 4 x 8 bytes = 32 bytes (full l2 response)
    end
  end
end

// *****************************************************************************
// Data out muxing
// *****************************************************************************

// Data interleaved from tmp buffer
//   Size = 1 (byte wise)
//     * During first pass it gets data from buffers 0,2,4 and 6
//       8 bytes from each buffer, in total 32 bytes
//     * During second pass it gets data from buffers 1,3,5 and 7
//       8 bytes from each buffer, in total 32 bytes
//  Size = 2 (word wise)
//     * During first pass it gets data from buffers [0,1] and [4,5]
//       16 bytes from each buffer pair, in total 32 bytes
//     * During second pass it gets data from buffers [2,3] and [6,7]
//       16 bytes from each buffer pair, in total 32 bytes
//
// Data transposition from tmp buffer (full buffer description)
//   Size = 1 (1 byte)
//     * During first pass and first 8 columns it gets data from 32 buffers 0,2,4..62
//       1 bytes per buffer in each column, in total 32 bytes per column
//     * During second pass and first 8 columns it gets data from 32 buffers (0,2,4..62)+64
//       1 bytes per buffer in each column, in total 32 bytes per column
//     * From columns 8 to 15, buffers are 1,3,5,..31 and (1,3,5,..31)+64
//   Size = 2 (2 bytes)
//     * During first pass and first 4 columns it gets data from 16 buffers 0,4,8..60
//       2 bytes per buffer in each column, in total 32 bytes per column
//     * During second pass and first 4 columns it gets data from 16 buffers (0,4,8..60)+64
//       2 bytes per buffer in each column, in total 32 bytes per column
//     * From columns  4 to  7 buffers are 1,5, 9,..61 and (1,5, 9,..61)+64
//     * From columns  8 to 15 buffers are 2,6,10,..62 and (2,6,10,..62)+64
//     * From columns 16 to 31 buffers are 3,7,11,..63 and (3,7,11,..63)+64
//   Size = 3 (4 bytes)
//     * During first pass and first 2 columns it gets data from 8 buffers 0,8,16..56
//       4 bytes per buffer in each column, in total 32 bytes per column
//     * During second pass and first 2 columns it gets data from 8 buffers (0,8,16..56)+64
//       4 bytes per buffer in each column, in total 32 bytes per column
//     * From columns  2 to  3 buffers are  1, 9,17,..57 and ( 1, 9,17,..57)+64
//     * From columns  4 to  6 buffers are  2,10,18,..58 and ( 2,10,18,..58)+64
//      ...
//     * From columns 14 to 15 buffers are 15,23,31,..63 and (15,23,31,..63)+64
//
// Data transposition from tmp buffer (limited buffer to 16 x 8 bytes = 128 bytes)
//     * Single pass always
//     * Used buffers are limited to index 0 to 15
//     * Behavior is as described for the full buffer operation but data are
//       only valid in one section of 8 bytes of the l2 data
//     * Data validity is determined by the index of the "transpose_split" counter.
//       Lower 2 bits indicate the LRAM column (64b, 8 bytes)
//       Upper bit indicates whether the access is for the first half of the cache line
//       or for the second

// Note: for transposition, idx_buf and idx_byte are computed as in the full buffer operation (128),
// but buffer index is then wrapped to the limited buffer index (16)
logic [6:0] idx_buf;
logic [3:0] idx_bufl;
logic [2:0] idx_byte;
always_comb begin
  if (trans_int8) begin
    for (int i=0; i<`CORE_L2_BLOCK_EXT_SIZE/8; i++) begin // increment one byte each time
      idx_buf  = {4'b0,i[1:0],shared_load_idx}; // (i%4)*2 + shared_load_idx
      idx_byte = i[4:2];                        // (i/4)
      idx_bufl = idx_buf[3:0];
      shared_load_data[i*8+:8] = tmp_data[idx_bufl][idx_byte*8+:8];
    end
  end else if (trans_int16) begin
    for (int i=0; i<`CORE_L2_BLOCK_EXT_SIZE/8; i=i+2) begin // increment two bytes each time
      idx_buf  = {i[1:0],1'b0} + i[4] + {shared_load_idx,1'b0}; // (i%4)*2 + (i/16) + shared_load_idx*2
      idx_byte = {i[3:2],1'b0};                                 // ((i%16)/4)*2
      idx_bufl = idx_buf[3:0];
      shared_load_data[i*8+:16] = tmp_data[idx_bufl][idx_byte*8+:16];
    end
  end else if (trans_trp8) begin
    for (int i=0; i<`CORE_L2_BLOCK_EXT_SIZE/8; i++) begin // increment one byte each time
      idx_buf  = {i[4:0],1'b0} + {shared_load_idx,6'b0} + shared_count[`DCACHE_TENSOR_CTRL_NLINES_BITS-1:3]; // i*2 + shared_load_idx*64 + col/8
      idx_byte = shared_count[2:0];                                                                          // col%8
      idx_bufl = idx_buf[3:0];
      shared_load_data[i*8+:8] = tmp_data[idx_bufl][idx_byte*8+:8];
    end
  end else if (trans_trp16) begin
    for (int i=0; i<`CORE_L2_BLOCK_EXT_SIZE/8; i=i+2) begin // increment two bytes each time
      idx_buf  = {i[4:0],1'b0} + {shared_load_idx,6'b0} + shared_count[`DCACHE_TENSOR_CTRL_NLINES_BITS-1:2]; // i*2 + shared_load_idx*64 + (col*2)/8
      idx_byte = {shared_count[1:0],1'b0};                                                                   // (col*2)%8
      idx_bufl = idx_buf[3:0];
      shared_load_data[i*8+:16] = tmp_data[idx_bufl][idx_byte*8+:16];
    end
  end else begin // trans_trp32
    for (int i=0; i<`CORE_L2_BLOCK_EXT_SIZE/8; i=i+4) begin // increment four bytes each time
      idx_buf  = {i[4:0],1'b0} + {shared_load_idx,6'b0} + shared_count[`DCACHE_TENSOR_CTRL_NLINES_BITS-1:1]; // i*2 + shared_load_idx*64 + (col*4)/8
      idx_byte = {shared_count[0],2'b0};                                                                     // (col*4)%8
      idx_bufl = idx_buf[3:0];
      shared_load_data[i*8+:32] = tmp_data[idx_bufl][idx_byte*8+:32];
    end
  end
end

// *****************************************************************************
// Temporary data array to store the downloaded data
// *****************************************************************************

// Ensure that in case of MODULE_IDX = 1 the array is not used/instantiated
generate
  if (MODULE_IDX == 0) begin : TMP_DATA_TL

    // Write enable validate one cycle in advance and latched in phase 2
    logic rf_write_en_prev_p2;
    logic rf_write_en_prev;

    assign rf_write_en_prev = (ctrl_state_next == ML_CTRL_Int_Req) || (ctrl_state == ML_CTRL_Tra_Req);

    `LATCH_P2(clkgt, rf_write_en_prev_p2, rf_write_en_prev)

    // Generate latches
    genvar tmprf;
    for(tmprf = 0; tmprf < `DCACHE_TL_TMP_DATA_BLOCKS; tmprf++) begin : TMP_DATA_RF

      rf_latch_single_1r_1w_par #(
        .WIDTH   ( `DCACHE_LRAM_DATA_SIZE )
      ) tmp_data_reg (
        // System signals
        .clk             ( clkgt                  ),
        .test_en         ( 1'b0                   ),
        // Output port
        .rd_data_a       ( tmp_data[tmprf]        ),
        // Write port
        .wr_data_a       ( tmp_data_next[tmprf]   ),
        .wr_en_a         ( shared_l2_store[tmprf] ),
        .wr_data_a_en_1p ( rf_write_en_prev_p2    )
      );
    end
  end else begin : TMP_DATA_RF_0

    // Force tmp_data to zero
    genvar tmprf;
    for(tmprf = 0; tmprf < `DCACHE_TL_TMP_DATA_BLOCKS; tmprf++) begin
      assign tmp_data[tmprf] = `DCACHE_LRAM_DATA_SIZE'b0;
    end

  end
endgenerate

////////////////////////////////////////////////////////////////////////////////
// Request control
////////////////////////////////////////////////////////////////////////////////
// This part controls the request of L2 lines at the appropriate address, which
// is inserted to Dcache pipeline for proper translation from VA to PA. Once
// address comes back after translation in s2, a request to the L2 is triggered
// using the "L2 Interface control".
// Data returns with a L2 response that is either handled at Dcache top to write
// into LRAM or handled by the local memory for further transformation.
////////////////////////////////////////////////////////////////////////////////

logic [`DCACHE_TENSOR_CTRL_DEST_RANGE]        l1_scp_dest;            // Cacheline destination for L1
logic [`DCACHE_TENSOR_CTRL_DEST_BITS:0]       l1_scp_dest_ext;

logic [`VA_RANGE]                             s0_addr_next;           // Next address to prefetch
logic                                         s0_addr_msb_err_next;   // Next address has an overflow

logic [`DCACHE_TENSOR_CTRL_NLINES_RANGE]      count;                  // Counts what prefetch we are doing.
logic [`DCACHE_TENSOR_CTRL_NLINES_RANGE]      count_next;

logic [`DCACHE_TENSOR_CTRL_NLINES_RANGE]      tenb_count;             // Counter for the TENB entries being requested
logic [`DCACHE_TENSOR_CTRL_NLINES_RANGE]      tenb_count_next;
logic [`DCACHE_TENB_R]                        tenb_idx;               // Index of the current entry (LSB of tenb_count)
logic [`DCACHE_TENB_R]                        tenb_idx_last;          // Saved index of the previous operation
logic                                         tenb_idx_save;

logic [`TMASK_COUNT_RANGE]                    tmask_idx;              // Tensor mask index
logic [`TMASK_COUNT_RANGE]                    tmask_idx_next;

logic [`VPU_TENB_CREDIT_RANGE]                tenb_credits;           // Number of TENB credits available
logic [`VPU_TENB_CREDIT_RANGE]                tenb_credits_next;
logic                                         tenb_credits_ok;        // The condition regarding TENB credits is met

logic [`VPU_TENB_RCV_ENTRY_RANGE]             tenb_rcv_entry;         // Per TENB entry is back, so it can be sent again (prevents problems with two outstanding loads to same TENB entry)
logic [`VPU_TENB_RCV_ENTRY_RANGE]             tenb_rcv_entry_next;
logic                                         tenb_rcv_ok;            // Entries have been received and we ca do a new request

logic [`DCACHE_TENB_R]                        tenb_written_entry;     // Index to the written entry

logic                                         s1_addr_load_next;
logic                                         s0_addr_load_skip_mask; // Skip current load pass due to tesnsor mask bit being '0'
logic                                         enabled, enabled_next;  // Module is enabled doing prefetches
logic                                         enabled_req;            // New request makes state machine going to enable state
logic                                         addr_up;                // Update the address FF
logic                                         s2_req_valid_next;      // Request valid for S2 stage
logic                                         s2_req_valid;           // Request in S2 stage before qualify
logic                                         s2_req_ready;           // Internal ready to accept a new address to L2
logic                                         waiting_tlb;            // Flag to indicate that we are waiting for TLB to resolve the address
logic                                         waiting_tlb_next;
logic                                         l2_seq_wait;            // Wait for the sequence to be ready

logic                                         s0_available_x;         // Internal versions of pipeline control signals
logic                                         s0_addr_load_x;         // These signals are used to shadow the interface to pipeline
logic                                         s1_available_x;
logic                                         s1_addr_load_x;
logic                                         s1_tlb_wait_x;
logic                                         s1_tlb_fail_x;
logic [`PA_RANGE]                             s2_addr_x;




//      CLK    RST    EN        DOUT             DIN                    DEF
`RST_FF(clkgt, reset,           enabled,         enabled_next,          1'b0)
`RST_FF(clkgt, reset,           s1_addr_load_x,  s1_addr_load_next,     1'b0)
`RST_FF(clkgt, reset,           s2_req_valid,    s2_req_valid_next,     1'b0)
`RST_FF(clkgt, reset,           waiting_tlb,     waiting_tlb_next,      1'b0)
`EN_FF (clkgt,        addr_up,  count,           count_next                 )
`EN_FF (clkgt,        addr_up,  s0_addr,         s0_addr_next               )
`EN_FF (clkgt,        addr_up,  s0_addr_msb_err, s0_addr_msb_err_next       )
`EN_FF (clkgt,        addr_up,  tmask_idx,       tmask_idx_next             )

// MODULE_IDX 0 does not do any TENB transfer
generate
  if (MODULE_IDX == 0) begin
    assign tenb_credits = {`VPU_TENB_CREDIT_BITS{1'b0}};
    assign tenb_rcv_entry = {`VPU_TENB_RCV_ENTRY_BITS{1'b0}};
    assign tenb_count = {`DCACHE_TENSOR_CTRL_NLINES_BITS{1'b0}};
    assign tenb_idx_last = {`DCACHE_TENB_BITS{1'b0}};
  end else begin
    //         CLK    RST    EN               DOUT             DIN                  DEF
    `RST_FF   (clock, reset,                  tenb_credits,    tenb_credits_next,   `DCACHE_TL_TENB_CREDITS         )
    `RST_FF   (clock, reset,                  tenb_rcv_entry,  tenb_rcv_entry_next, {`VPU_TENB_RCV_ENTRY_BITS{1'b1}}        )
    `EN_FF    (clkgt,        addr_up,         tenb_count,      tenb_count_next                                      )
    `RST_EN_FF(clkgt, reset, tenb_idx_save,   tenb_idx_last,   tenb_idx,            {`DCACHE_TENB_BITS{1'b0}})
  end
endgenerate

// Flush the tenb status.
assign tenb_flush = request_control_flush_done;

// Index of TENB entry from tenb_count
assign tenb_idx = tenb_count[`DCACHE_TENB_R];

// Ready to do a new request as previous requests have already been received
// When a new operation starts and we jump from a "last_idx" to "idx=0", we
// need to check that the reception for the skipped indexes has already happened
//   Normal operation, after at least one request: tenb_rcv_ok = tenb_rcv_entry[tenb_idx]
//     tenb_idx_last = 0, 1, 2 => tenb_idx = 1, 2, 3 => (tenb_idx_last >= tenb_idx) = False
//     tenb_idx_last = 3       => tenb_idx = 0       => (i > tenb_idx_last) = False
//   First request of any operation after first operation: tenb_idx = 0
//     tenb_idx_last = 0 => tenb_rcv_ok = tenb_rcv_entry[0] & (&tenb_rcv_entry[3:1])
//     tenb_idx_last = 1 => tenb_rcv_ok = tenb_rcv_entry[0] & (&tenb_rcv_entry[3:2])
//     tenb_idx_last = 2 => tenb_rcv_ok = tenb_rcv_entry[0] & tenb_rcv_entry[3]
//     tenb_idx_last = 3 => tenb_rcv_ok = tenb_rcv_entry[0]
always_comb begin
  tenb_rcv_ok = tenb_rcv_entry[tenb_idx];
  for (integer i=0; i<(`VPU_TENB_SIZE/2); i++) begin
    if ((i > tenb_idx_last) && (tenb_idx_last >= tenb_idx)) begin
      tenb_rcv_ok &= tenb_rcv_entry[i];
    end
  end
end

always_comb begin
  enabled_next                 = enabled;
  addr_up                      = 1'b0;
  err_mem_next                 = 1'b0;

  // Increment counters and addresses by default
  s0_addr_next         = s0_addr + request_control_stride;       // Next address to prefetch
  s0_addr_msb_err_next = s0_addr[`VA_MSB] & !s0_addr_next[`VA_MSB];

  count_next     = count + `DCACHE_TENSOR_CTRL_NLINES_BITS'b1;
  tmask_idx_next = tmask_idx + `TMASK_COUNT_BITS'b1;
  tenb_idx_save  = 1'b0;
  if (MODULE_IDX == 0) begin
    tenb_credits_next   = 'b0;
    tenb_rcv_entry_next = 'b0;
    tenb_count_next     = 'b0;
  end else begin
    tenb_credits_next   = tenb_credits;
    tenb_rcv_entry_next = tenb_rcv_entry;
    tenb_count_next     = tenb_count;
  end

  // Initialize addresses and keep other counters
  if (!enabled && request_control_store_addr) begin
    s0_addr_next         = request_control_address;
    s0_addr_msb_err_next = 1'b0;
    count_next           = count;
    tmask_idx_next       = tmask_idx;
    addr_up              = 1'b1;
    if (MODULE_IDX == 1) begin
      tenb_count_next    = tenb_count;
    end
  end

  // New prefetch starts: ensure counters and destination are set
  enabled_req = 1'b0;
  if (!enabled && request_control_start) begin
    s0_addr_next         = s0_addr;
    s0_addr_msb_err_next = 1'b0;
    count_next           = `DCACHE_TENSOR_CTRL_NLINES_BITS'b0;
        tmask_idx_next       = `TMASK_COUNT_BITS'b0;
        addr_up              = 1'b1;
        enabled_next         = 1'b1;
        enabled_req          = 1'b1;
        if (MODULE_IDX == 1)
          tenb_count_next    = 'b0;
    end

    // Request accepted or skipped because of the mask
    if ((s2_req_valid && s2_req_ready) || s0_addr_load_skip_mask)
    begin
        // Moves to next request
        addr_up = 1'b1;
        
        // Disable when doing last request
        if (count == request_control_num_lines)
            enabled_next = 1'b0;
    end

    // Decrement TENB credits and advance pointer
    if ((MODULE_IDX == 1) && s2_req_valid && s2_req_ready)
    begin
      tenb_credits_next = tenb_credits - tensor_ctrl_tenb;
      tenb_rcv_entry_next[tenb_idx] = 1'b0; // Clear the received bit upon sending the TLoad
      tenb_count_next = tenb_count + 'b1;
      tenb_idx_save = 1'b1;
    end

    // Increment number of TENB credits
    if ((MODULE_IDX == 1) && tenb_credit) begin
      tenb_credits_next = tenb_credits_next + 1'b1;
    end

    // Set received bit when it is written in TENB. We need this to prevent
    // sending two times a request going to same TENB entry and writing out of
    // order due the first request coming later than the second
    if (tenb_written) begin
      tenb_rcv_entry_next[tenb_written_entry] = 1'b1;
    end

    // TLB failure for ML request
    if (enabled && s1_addr_load_x && s1_tlb_fail_x) begin
      enabled_next = 1'b0;
      err_mem_next = 1'b1;
    end

    // Terminate pending operations and initialize tenb registers
    if (request_control_flush) begin
      enabled_next = 1'b0;
    end

    if (tenb_flush) begin
      tenb_credits_next = `DCACHE_TL_TENB_CREDITS;
      tenb_rcv_entry_next = {`VPU_TENB_SIZE/2{1'b1}};
    end

    // Cacheline destination. Ensure that in L1 operations index does not go outside of the defined limits.
    l1_scp_dest_ext = request_control_scp_dest + count;
    if (l1_scp_dest_ext >= `DCACHE_TL_SCP_MAX_IDX) begin
      l1_scp_dest_ext = l1_scp_dest_ext - `DCACHE_TL_SCP_MAX_IDX;
    end
    l1_scp_dest = l1_scp_dest_ext[`DCACHE_TENSOR_CTRL_DEST_BITS-1:0];

    // Clear the used set and way: only used to detect errors
    // For TENB, store the idx of the destination in the set and the entry index in the way
    request_control_clear_set = tensor_ctrl_tenb ? count : get_set_from_scp_dest(l1_scp_dest);
    request_control_clear_way = tensor_ctrl_tenb ? tenb_idx : get_way_from_scp_dest(l1_scp_dest);

    // Signals to control s0 to s2 addr in pipeline. When address has not to be translated,
    // the pipeline is "free" and not disturbed. This is achieved by shadowing pipeline control signals
    // and keeping same state machine
    s0_addr_load_x = s0_available_x && enabled && !s1_addr_load_x && !s2_req_valid
                   && (!request_control_use_tmask || request_control_tmask_bits[tmask_idx]) // Tensor mask bit is set or it is not a tensor load using tensor mask
                   && !waiting_tlb                                                          // Not waiting for TLB
                   && (!tensor_ctrl_tenb || (tenb_credits_ok && tenb_rcv_ok));              // Not writing to TENB or TENB credits available and entry available
    s1_addr_load_next = s0_addr_load_x && !request_control_flush;
    s2_req_valid_next = (s1_addr_load_x && !s1_tlb_wait_x && !s1_tlb_fail_x && !request_control_flush)  // New request successfull
                     || (s2_req_valid && !s2_req_ready && s1_available_x && !request_control_flush);    // Previous request not accepted, not overwritten by regular instruction, retry
    waiting_tlb_next = (s1_addr_load_x || waiting_tlb) && s1_tlb_wait_x;                                // TLB miss or waiting for fill to complete

    // Indicate when operation is finishing: all L2 transfers are complete (none has to be done if tensor mask bits are all zero)
    request_control_ending_l2 = ((!enabled && l2_interface_busy_prev && !l2_interface_busy) ||
                                  (enabled && !enabled_next && !(|request_control_tmask_bits) && request_control_use_tmask) ||
                                  (enabled && !enabled_next && s1_addr_load_x && s1_tlb_fail_x));
    // For Non-Transformation operations simply check that all requests to L2 have been issued
    request_control_ending = !(enabled || enabled_next || l2_req_valid || l2_seq_wait);
    // Flush is complete
    request_control_flush_done = request_control_flush && !enabled && !l2_interface_busy;
  end

  always_comb begin
    // We are doing a tensorload using the tensor mask CSR info. If bits are
    // not pending to be computed and tensor mask bit says this pass is
    // disabled, skip it
    s0_addr_load_skip_mask =    s0_available_x && enabled  && !s1_addr_load_x                        && !s2_req_valid
                             && request_control_use_tmask  && !request_control_tmask_bits[tmask_idx] && !waiting_tlb;
  end

  // Connect to pipeline control ports. Shadowed if not translating using TLB
  // This could be removed, but it may be useful sometime ...
  logic  shadow_tlb_translate;
  assign shadow_tlb_translate = 1'b0;

  assign s0_addr_load = s0_addr_load_x & !shadow_tlb_translate;
  assign s0_thread_id = request_control_thread_id;
  assign s1_addr_load = s1_addr_load_x & !shadow_tlb_translate;
  assign s0_available_x = s0_available | shadow_tlb_translate;
  assign s1_available_x = s1_available | shadow_tlb_translate;
  assign s1_tlb_wait_x = s1_tlb_wait & !shadow_tlb_translate;
  assign s1_tlb_fail_x = s1_tlb_fail & !shadow_tlb_translate;
  assign s2_addr_x = shadow_tlb_translate? s0_addr[`PA_RANGE] : s2_addr;

  ////////////////////////////////////////////////////////////////////////////////
  // L2 Interface control
  ////////////////////////////////////////////////////////////////////////////////
  // Sends miss requests to L2 to obtain the data according to the provided
  // address. Two miss requests can be inflight. After sending the request the
  // each control waits until an ACK is received to
  ////////////////////////////////////////////////////////////////////////////////

  `define DCACHE_TL_SEQUENCE_ENTRIES    (1 << (`SHIRE_COOP_SEQ_ID_SIZE+1))

  typedef enum logic [1:0] {
    ML_L2I_Wait_Req = 2'b00, // Wait for new request
    ML_L2I_L2_Req   = 2'b01, // Do a request to L2
    ML_L2I_Wait_Ack = 2'b10, // Wait Ack from L2
    ML_L2I_L2_Seq   = 2'b11  // Wait sequence to do a L2 request
  } dcache_tl_l2i_state;

  dcache_tl_l2i_state     [`DCACHE_TL_L2_TRANSFERS_RANGE]            l2i_state;        // Stores state of the transactions to L2
  dcache_tl_l2i_state     [`DCACHE_TL_L2_TRANSFERS_RANGE]            l2i_state_next;
  logic                   [`DCACHE_TL_L2_TRANSFERS_RANGE]            l2i_state_up;     // Update state
  et_link_core_miss_id_t  [`DCACHE_TL_L2_TRANSFERS_RANGE]            my_miss_id;       // miss_id sent to L2 request

  logic                   [`DCACHE_TL_L2_TRANSFERS_RANGE]            waiting_for_req;  // Flag to indicate that instances are in Wait_Req state
  logic                   [`DCACHE_TL_L2_TRANSFERS_RANGE]            requesting_to_l2; // Flag to indicate that instances are in L2_Req state
  logic                   [`DCACHE_TL_L2_TRANSFERS_RANGE]            accepted_to_l2;   // Flag to indicate that request has been granted
  logic                   [`DCACHE_TL_L2_TRANSFERS_RANGE]            sequencing_to_l2; // Flag to indicate that instances are in L2_Seq state


  logic                                                              req_up;           // Update necessary information for request

  logic [`PA_CLINE_RANGE]                                            req_addr_cline;   // Address aligned to cache line to request to L2

  logic [`DCACHE_TL_L2_TRANSFERS_RANGE][`DCACHE_WAY_RANGE]           tl_load_way;      // Stores lram way_en for the current transaction
  logic [`DCACHE_TL_L2_TRANSFERS_RANGE][`DCACHE_SET_RANGE]           tl_load_set;      // Stores lram set for the current transaction


  logic [`DCACHE_TL_L2_TRANSFERS_RANGE]                              new_data_chunk;   // Received a new data chunk from L2 response
  logic [`DCACHE_TL_L2_TRANSFERS_RANGE]                              received_error;   // Received an error notification as response
  logic [`DCACHE_TL_L2_TRANSFERS_RANGE]                              received_answer;  // Received any kind of answer

  logic [`DCACHE_TL_L2_TRANSFERS_RANGE]                              l2i_group;        // Group to which an L2 request belongs to
  logic [`DCACHE_TL_L2_TRANSFERS_RANGE]                              l2i_op_idx;       // Operation index to which an L2 request belongs to
  logic [`DCACHE_TL_L2_TRANSFERS_RANGE]                              l2i_op_is_coop;   // Flag indicating that the request was for a cooperative load

  logic [1:0]                                                        l2i_op_in_idx;    // Flag for every Op Idx indicating that there are outstanding operations
  logic                                                              l2i_current_idx;




  logic [`DCACHE_TL_L2_TRANSFERS_RANGE][`SHIRE_COOP_SEQ_ID_SIZE:0]   l2i_seq_saved;    // Saved sequence value
  logic [`DCACHE_TL_SEQUENCE_ENTRIES-1:0]                            sequence_data;    // Information for the cooperative sequence
  logic [`DCACHE_TL_SEQUENCE_ENTRIES-1:0]                            sequence_data_next;
  logic [`SHIRE_COOP_SEQ_ID_SIZE-1:0]                                sequence_count;   // Sequence counter
  logic [`SHIRE_COOP_SEQ_ID_SIZE-1:0]                                sequence_count_next;
  logic                                                              sequence_usable;
  logic                                                              l2i_seq_data_up;  // Update sequence data
  logic                                                              l2i_seq_cnt_up;   // Update sequence counter
  logic                                                              l2i_seq_idx;
  logic                                                              l2i_seq_idx_next;

  // Say ready when not doing a request or when any of the control instances waits for a request
  assign s2_req_ready = !(l2_req_valid | l2_seq_wait) && (|waiting_for_req) && (!l2i_op_in_idx[request_control_l2_op_idx] || (l2i_current_idx == request_control_l2_op_idx));

  // Keep ML busy while there are pending transfers
  assign l2_interface_busy = !(&waiting_for_req);

  // Create instances of control to handle L2 transactions
  
  for (genvar i=0; i<`DCACHE_TL_L2_TRANSFERS; i++) begin : l2_interface
    // Constant miss_id for every instance
    assign my_miss_id[i] = et_link_core_miss_id_t'(ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0 + et_link_core_miss_id_t'(i) + MODULE_IDX*4);
    // Send a new request to the first available control instance
    if (i == 0) begin
      assign new_l2_req[i] = (l2i_state[i] == ML_L2I_Wait_Req) && !(l2_req_valid | l2_seq_wait) && s2_req_valid && s2_req_ready;
    end else begin
      assign new_l2_req[i] = (l2i_state[i] == ML_L2I_Wait_Req) && !(l2_req_valid | l2_seq_wait) && s2_req_valid && s2_req_ready && ((|new_l2_req[i-1:0]) == 1'b0);
    end
    // Control to change state
    always_comb begin
      l2i_state_next[i] = l2i_state[i];
      // New internal request
      if ((l2i_state[i] == ML_L2I_Wait_Req) && new_l2_req[i] && !request_control_use_coop) begin
        l2i_state_next[i] = ML_L2I_L2_Req;
      end
      if ((l2i_state[i] == ML_L2I_Wait_Req) && new_l2_req[i] &&  request_control_use_coop) begin
        l2i_state_next[i] = ML_L2I_L2_Seq;
      end
      // Wait here until sequence is validated
      if ((l2i_state[i] == ML_L2I_L2_Seq) && sequence_usable) begin
        l2i_state_next[i] = ML_L2I_L2_Req;
      end
      // Accepted requested to L2
      if ((l2i_state[i] == ML_L2I_L2_Req) && l2_req_ready) begin
        l2i_state_next[i] = ML_L2I_Wait_Ack;
      end
      // Last data chunk received (load index == 1 or operating over L2 Scp)
      if (new_data_chunk[i] && tl_load_idx[i]) begin
        l2i_state_next[i] = ML_L2I_Wait_Req; // Cannot be excluded with Verdi. DVCEX032. VERIF-1344
      end
      // In case of error, finish current request immediatelly
      if (received_error[i]) begin
        l2i_state_next[i] = ML_L2I_Wait_Req;
      end
    end
    // Enable state machine clock
    assign l2i_state_up[i] = (l2i_state[i] != ML_L2I_Wait_Req) | enabled;
    // Say that the control instance is ready for new request
    assign waiting_for_req[i] = (l2i_state[i] == ML_L2I_Wait_Req);
    // Say that the control instance is doing a new L2 request
    assign requesting_to_l2[i] = (l2i_state[i] == ML_L2I_L2_Req);
    // Say that a L2 request has been accepted for this instance
    assign accepted_to_l2[i] = requesting_to_l2[i] & l2_req_ready;
    // Say that the control instance is waiting the sequence to do a new L2 request
    assign sequencing_to_l2[i] = (l2i_state[i] == ML_L2I_L2_Seq);
    // Receiving a new data chunk
    assign new_data_chunk[i] = (l2i_state[i] == ML_L2I_Wait_Ack) && ((l2_resp.opcode == ET_LINK_RSP_AckData) || (l2_resp.opcode == ET_LINK_RSP_Ack))
                            && (et_link_core_miss_id_t'(l2_resp.id) == my_miss_id[i]) && l2_resp_valid;
    // Receiving and error
    assign received_error[i] = (l2i_state[i] == ML_L2I_Wait_Ack) && (l2_resp.opcode == ET_LINK_RSP_Err)
                            && (et_link_core_miss_id_t'(l2_resp.id) == my_miss_id[i]) && l2_resp_valid;
    // Receiving any answer
    assign received_answer[i] = (l2i_state[i] == ML_L2I_Wait_Ack)
                            && (et_link_core_miss_id_t'(l2_resp.id) == my_miss_id[i]) && l2_resp_valid;
    // Set the address index bit (this is basically a counter from 0 to 1)
    assign tl_load_idx_next[i] = (l2i_state[i] == ML_L2I_Wait_Req) ? 1'b0 :
                                 (new_data_chunk[i])               ? ~tl_load_idx[i] :
                                                                     tl_load_idx[i];
    //           CLK    RST    EN                 DOUT                   DIN                           DEF
    `RST_EN_FF(clkgt, reset, l2i_state_up[i],   l2i_state[i],          l2i_state_next[i],            ML_L2I_Wait_Req)
    `EN_FF    (clkgt,        new_l2_req[i],     tl_load_way[i],        request_control_clear_way                    )
    `EN_FF    (clkgt,        new_l2_req[i],     tl_load_set[i],        request_control_clear_set                    )
    `EN_FF    (clkgt,        l2i_state_up[i],   tl_load_idx[i],        tl_load_idx_next[i]                          )
    `EN_FF    (clkgt,        new_l2_req[i],     l2i_group[i],          request_control_l2_group_id                  )
    `EN_FF    (clkgt,        new_l2_req[i],     l2i_op_idx[i],         request_control_l2_op_idx                    )
    `EN_FF    (clkgt,        new_l2_req[i],     l2i_op_is_coop[i],     request_control_use_coop                     )
    `EN_FF    (clkgt,        accepted_to_l2[i], l2i_seq_saved[i],      {l2i_seq_idx,sequence_count}                 )
    `EN_FF    (clkgt,        new_l2_req[i],     l2i_stg_addr[i],        shared_l2_counter                           )
  end
 

  // Record the "current" operation index. When the first request enters and we are not operating
  // in that operation index, save the operation index as the current one
  // Notice that s2_req_ready is not set if we are operating in request_control_l2_op_idx and current_idx != request_control_l2_op_idx
  logic l2i_current_idx_up;

  assign l2i_current_idx_up = s2_req_valid && s2_req_ready && !l2i_op_in_idx[request_control_l2_op_idx] && (l2i_current_idx != request_control_l2_op_idx);

  //         CLK    RST    EN                  DOUT             DIN                        DEF
  `RST_EN_FF(clkgt, reset, l2i_current_idx_up, l2i_current_idx, request_control_l2_op_idx, 1'b1)

  // Create a delayed version of "l2_interface_busy"
  //          CLK    RST    EN                                          DOUT                    DIN                DEF
  `RST_EN_FF (clkgt, reset, l2_interface_busy | l2_interface_busy_prev, l2_interface_busy_prev, l2_interface_busy, 1'b0)

  // Determine which control instance is requesting to L2 (only one can be in ML_L2I_L2_Req state)
  // Give priority to use lower index control entities if they are available
  logic [`DCACHE_TL_L2_TRANSFERS_SEL_RANGE] l2_req_idx;

  always_comb begin
    l2_req_idx = 'b0;
    for (integer i=`DCACHE_TL_L2_TRANSFERS-1; i>=0; i--) begin
      if (l2i_state[i] == ML_L2I_L2_Req) begin
        l2_req_idx = i[`DCACHE_TL_L2_TRANSFERS_SEL_RANGE];
      end
    end
  end

  // Determine if there are operations pending for a given operation index
  always_comb begin
    l2i_op_in_idx = 2'b0;
    for (integer idx=0; idx<2; idx++) begin
      for (integer i=0; i<`DCACHE_TL_L2_TRANSFERS; i++) begin
        l2i_op_in_idx[idx] |= (l2i_op_idx[i] == idx[0]) && (l2i_state[i] != ML_L2I_Wait_Req);
      end
    end
  end

  // Sequence information and control
  logic [`SHIRE_COOP_ID_SIZE-1:0]  tensor_coop_id_curr;        // Current cooperation ID
  logic [`SHIRE_COOP_ID_SIZE-1:0]  tensor_coop_id_prev;        // Previous cooperation ID
  logic                            request_different_coop_id;  //
  logic                            tensor_coop_id_prev_up;

  always_comb begin
    // Sequence counter and data default
    sequence_count_next = sequence_count;
    sequence_data_next  = sequence_data;

    // When changing operation, reset sequence counter
    if (enabled_req) begin
      sequence_count_next = `SHIRE_COOP_SEQ_ID_SIZE'b0;
    end else if (l2_req_valid & l2_req_ready) begin
      sequence_count_next = sequence_count + `SHIRE_COOP_SEQ_ID_SIZE'b1;
    end

    // Check if the slot pointed by the current sequence counter is usable
    sequence_usable = (sequence_data[{l2i_seq_idx,sequence_count}] == 1'b0);

    // Update counter
    l2i_seq_cnt_up = (enabled_req || (l2_req_valid & l2_req_ready)) && request_control_use_coop;

    // Update previous tensor coop_id
    tensor_coop_id_prev_up = l2i_current_idx_up & request_control_use_coop;

    // Decide if the current mask is different to the previous
    tensor_coop_id_curr = tensor_coop_data[`SHIRE_COOP_ID_START+:`SHIRE_COOP_ID_SIZE];
    request_different_coop_id = (tensor_coop_id_curr != tensor_coop_id_prev);
    l2i_seq_idx_next = (request_different_coop_id && request_control_use_coop) ? ~l2i_seq_idx : l2i_seq_idx;

    // Update sequence data.
    //   Put flag to '1' when request is accepted
    //   Put flag to '0' when request is acknowledged
    l2i_seq_data_up = ((l2_req_valid & l2_req_ready & request_control_use_coop) || // Update during requests
                       (|(received_answer & l2i_op_is_coop)));                     // Update during responses
    if (l2_req_valid & l2_req_ready & request_control_use_coop) begin
      sequence_data_next[{l2i_seq_idx,sequence_count}] = 1'b1;
    end

    for (integer i=0; i<`DCACHE_TL_L2_TRANSFERS; i++) begin
      if (received_answer[i] & l2i_op_is_coop[i]) begin
        sequence_data_next[l2i_seq_saved[i]] = 1'b0;
      end
    end

    // Cooperative data used in the current operation
    tensor_coop_data_sequenced = tensor_coop_data | (sequence_count << `SHIRE_COOP_SEQ_ID_START);
  end

  //         CLK    RST    EN                      DOUT                 DIN                  DEF
  `RST_EN_FF(clkgt, reset, l2i_current_idx_up,     l2i_seq_idx,         l2i_seq_idx_next,    1'b0 )
  `RST_EN_FF(clkgt, reset, tensor_coop_id_prev_up, tensor_coop_id_prev, tensor_coop_id_curr, '0   )
  `RST_EN_FF(clkgt, reset, l2i_seq_data_up,        sequence_data,       sequence_data_next,  '0   )
  `RST_EN_FF(clkgt, reset, l2i_seq_cnt_up,         sequence_count,      sequence_count_next, '0   )

  // Full L2 request address, cache aligned
  logic [`PA_RANGE] req_l2_addr_full;
  assign req_l2_addr_full = {req_addr_cline, 6'b0};

  // Request to L2
  always_comb begin
    l2_req_valid         = |requesting_to_l2; // Only one of the controls requests at a given time
    l2_req_valid_coop    = l2_req_valid & request_control_use_coop;
    l2_req_valid_no_coop = l2_req_valid & ~request_control_use_coop;

    l2_seq_wait      = |sequencing_to_l2;    // Only one of the controls "sequences" at a given time


    l2_req.id        = my_miss_id[l2_req_idx];
    l2_req.source    = 1'b0; // Unused
    l2_req.wdata     = request_control_use_coop ? 1'b1 : 1'b0;
    l2_req.opcode    = request_control_use_coop ? ET_LINK_REQ_ReadCoop :
                                                  ET_LINK_REQ_Read;
    l2_req.subopcode = `ET_LINK_SUBOPCODE_SIZE'b0;
    l2_req.address   = req_l2_addr_full;                                           // Put source address

    l2_req.data      = request_control_use_coop ? tensor_coop_data_sequenced       // In cooperative loads, send cooperative ID and minion mask in the data field
                                                : (`MIN_PER_N'b1 << minion_id);    // In normal loads, just send minion mask for this minion
    l2_req.size      = ET_LINK_Line;
    l2_req.qwen      = request_control_use_coop ? 4'b0001 : 4'b0000;
  end

  // Detect new data is being received
  assign l2_resp_is_for_me = (|new_data_chunk) | (|received_error);

  // Update registers for L2 request
  assign req_up = |new_l2_req;

  // Register information required for the accepted request
  //     CLK    RST    EN      DOUT            DIN
  `EN_FF(clkgt,        req_up, req_addr_cline, s2_addr_x[`PA_CLINE_RANGE])

  // Report bus error
  logic bus_err_local;

  assign bus_err_local = |received_error;
  assign bus_err = {bus_err_local & thread_id, bus_err_local & ~thread_id};


  ////////////////////////////////////////////////////////////////////////////////
  // Selects the load control to LRAM
  ////////////////////////////////////////////////////////////////////////////////
  logic  [`DCACHE_TL_L2_TRANSFERS_SEL_RANGE] sel_t;

  // Select the outstanding transfer that is being answered
  assign sel_t = l2_resp.id[`DCACHE_TL_L2_TRANSFERS_SEL_RANGE];

  always_comb begin
    request_control_load_way    = tl_load_way[sel_t];
    request_control_load_addr   = {tl_load_set[sel_t], tl_load_idx[sel_t], {`CORE_L2_BLOCK_EXT_ADDR_OFFSET_BITS{1'b0}}};
  end

// The entry index for every transfer has been stored in the way field for TENB operations
assign tenb_written_entry = request_control_load_way[`DCACHE_TENB_R];

////////////////////////////////////////////////////////////////////////////////
// Checks what TENB entries can be written when a fill comes back
////////////////////////////////////////////////////////////////////////////////

// These flops do not use gated clock since some external pulses from FMA may arrive asynchronously
//      CLK    RST    DOUT               DIN                    DEF
`RST_FF(clock, reset, tenb_avail_entry,  tenb_avail_entry_next, {`VPU_TENB_SIZE/2{1'b1}})
`FF    (clock,        tfma_started,      tfma_started_next)
`FF    (clock,        tfma_active_start, tfma_active_start_next)
`RST_FF(clock, reset, tenb_avail_all,    tenb_avail_all_next,   1'b0)

// Computes the available entries to be written in TENB
always_comb begin
  tenb_avail_entry_next = tenb_avail_entry;
  tenb_avail_all_next = &tenb_avail_entry;

  // Upon a TENB write, it is no longer available
  if (tenb_written) begin
    tenb_avail_entry_next[tenb_written_entry] = 1'b0;
  end

  // Credit coming back
  if (tenb_credit) begin
    tenb_avail_entry_next[tenb_credit_entry] = 1'b1;
  end

  // Reset when flushing state
  if (tenb_flush) begin
    tenb_avail_entry_next = {`VPU_TENB_SIZE/2{1'b1}};
  end
end

// Computes if the TFMA started
always_comb begin
  tfma_started_next = tfma_started;

  // TensorFMA must be always executed after TensorLoad to TENB
  // When the TLoad starts, its tfma hasn't started
  if (tensor_ctrl_start) begin
    tfma_started_next = 1'b0;
  end

  // When TFMA is enabled again
  if (tfma_enabled && !tensor_ctrl_start && !tfma_active_start) begin
    tfma_started_next = 1'b1;
  end

  // It is possible that when TLoad starts TFMA was active doing
  // other work. Need to keep track of this
  tfma_active_start_next = tfma_active_start;
  if (tensor_ctrl_start) begin
    tfma_active_start_next = tfma_enabled;
  end

  if (!tfma_enabled) begin
    tfma_active_start_next = 1'b0;
  end
end

always_comb begin
  tenb_ready = tenb_avail_entry[tenb_written_entry];

  // 1) When TFMA for this TLoad started, it is ok to use look ahead credits
  //    unless operations in the tensor store/reduce are ongoing
  // 2) When TFMA for this TLoad hasn't started we can only use the TENB
  //    credits available, otherwise we could deadlock the dcache by preventing
  //    look ahead TENB writes to be done
  // 3) The first TENB lines are save and can use all tenb credits (overlaps
  //    with still outstanding requests of previous tload to tenb)
  tenb_credits_ok = (tenb_credits > 0) && tfma_started                      && !tenb_no_extra_credits_cond1  // 1)
                 || (tenb_credits > `DCACHE_TL_TENB_LOOK_AHEAD)                                              // 2)
                 || (tenb_credits > 0) && (tenb_count < (`VPU_TENB_SIZE/2)) && !tenb_no_extra_credits_cond3; // 3)
end

////////////////////////////////////////////////////////////////////////////////
// Ready and busy signals
////////////////////////////////////////////////////////////////////////////////

logic [1:0]  l2_operates_in_id;  // Flags to know whether L2 operations are still pending for each ID

// Say ready when no activity at all
assign busy = (ctrl_state != ML_CTRL_Idle) | start_pulse | tensor_ctrl.restart_b;

// Say "complete" when not busy with the last programmed operation
always_comb begin

  l2_operates_in_id = 2'b0;
  for (integer id=0; id<2; id++) begin
    for (integer i=0; i<`DCACHE_TL_L2_TRANSFERS; i++) begin
      l2_operates_in_id[id] |= (l2i_state[i] != ML_L2I_Wait_Req) &&
                               (l2i_group[i] == id[0]);
    end
  end

  for (integer id=0; id<=1; id++) begin
    if (thread_id == 1'b0) begin
      if (id_running == id[0]) begin
        id_complete[id] = !busy && (l2_operates_in_id[id] == 0);
      end else begin
        id_complete[id] = (l2_operates_in_id[id] == 0);
      end
    end else begin // thread_id == 1 always "complete" (can't execute TL)
      id_complete[id] = 1'b1;
    end
  end

end

////////////////////////////////////////////////////////////////////////////////
// Clock gating
////////////////////////////////////////////////////////////////////////////////

// Activate clock once a new request arrives and until everything is completed
assign clkgt_en = tensor_ctrl_start | busy | !(&id_complete) | reset | (|err_flags);

et_clk_gate cgate_tl(
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

// RTLMIN-5542
assign csr_debug_signals = ctrl_state;
/*
assign csr_debug_signals = {  //csr_debug_tl1
          l2i_op_idx,
          l2i_group,
          l2i_state,
          //csr_debug_tl0
          tfma_active_start,
          tfma_started,
          tenb_avail_entry,
          s2_req_valid,
          tenb_rcv_entry,
          tenb_credits,
          tmask_idx,
          count,
          waiting_tlb,
          enabled,
          tensor_ctrl_num_lines,
          tensor_coop_data,
          tensor_ctrl_use_coop,
          tensor_ctrl_use_tmask,
          ctrl_state
        };
*/

assign sm_match_debug_signals = `ZX(`DCACHE_DBG_SM_MATCH_BITS,
        {
          l2i_state_up,
          new_l2_req,
          tfma_active_start,
          s0_addr_load & s0_available,
          s2_req_valid & s2_req_ready,
          ctrl_state_changed,
          interleave_clear_valid,
          l2_req_valid & l2_req_ready,
          shared_l2_count,
          err_unlock_flag,
          err_transf_flag,
          err_offset_flag,
          load_valid,
          save_tmask_bits,
          addr_up,
          ctrl_en,
          start_pulse,
          request_control_start,
          tensor_ctrl_start
        });

assign sm_filter_debug_signals = `ZX(`DCACHE_DBG_SM_FILTER_BITS,
        {
          l2i_state_up,
          new_l2_req,
          tfma_active_start,
          s0_addr_load,
          s0_available,
          s2_req_valid,
          s2_req_ready,
          ctrl_state_changed,
          interleave_clear_valid,
          l2_req_valid,
          l2_req_ready,
          shared_l2_count,
          err_unlock_flag,
          err_transf_flag,
          err_offset_flag,
          load_valid,
          save_tmask_bits,
          addr_up,
          ctrl_en,
          start_pulse,
          request_control_start,
          tensor_ctrl_start,
          l2_req.id,
          l2_req.opcode,
          l2_req.address,
          waiting_tlb,
          l2i_op_idx,
          l2i_group,
          l2i_state,
          ctrl_state
        });

assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(3)] = `DCACHE_DBG_SM_DATA_GROUP_BITS'b0;

assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(2)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
        {
          count,
          tmask_idx,
          shared_l2_counter,
          shared_count,
          shared_dest,
          shared_load_idx,
          trans_split_count,
          load_way,
          load_addr,
          tl_load_way,
          tl_load_set,
          tl_load_idx,
          tenb_credits,
          tenb_rcv_entry,
          tenb_avail_entry,
          tfma_started,
          s2_addr
        });
assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(1)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
        {
          l2_req.id,
          l2_req.opcode,
          l2_req.address,
          l2_req.data,
          s0_addr
        });
assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(0)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
        {
          vm_status_tl,
          id_running,
          thread_id,
          trans_tmask_bits,
          tensor_load_stride,
          tensor_ctrl_use_tmask,
          tensor_ctrl_use_coop,
          tensor_coop_data,
          tensor_ctrl_scp_dest,
          tensor_ctrl_offset,
          tensor_ctrl_transform,
          tensor_ctrl_num_lines
        });

endmodule
