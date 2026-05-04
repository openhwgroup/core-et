// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_cache_op_unit_l2 (
  // System signals
  input  logic                                   clock,
  input  logic                                   reset,
  input  logic [`NUM_SHIRE_IDS_R]                shire_id,
  input  logic [`DCACHE_CO_L2_MAX_OPS_BITS-1:0]  cacheop_max,
  input  logic [`DCACHE_CO_L2_REPRATE_BITS-1:0]  cacheop_reprate,
  // Chicken bit clock gate enable
  input  logic                                   chicken_bit_dcache,
  // Request bus to perform a snoop from L2
  output logic                                   busy,
  output logic [`CORE_NR_THREADS-1:0][1:0]       th_id_complete,
  input  logic                                   l2_resp_valid,
  input  et_link_minion_rsp_info_t               l2_resp,
  // Request to perform a cache operation
  output logic                                   cache_op_ready,
  input  logic                                   cache_op_valid,
  input  core_dcache_op_req                      cache_op_req,
  input  core_dcache_op_req_next                 cache_op_req_next,
  // Reports to L2 the taken action
  input  logic                                   l2_req_ready,
  output logic                                   l2_req_valid,
  output et_link_minion_evict_req_info_t         l2_req_data,
  // Direct interface to cache_op unit for operations to L2 and above
  output logic                                   cache_op_ready_byp,
  input  logic                                   cache_op_valid_byp,
  input  core_dcache_op_req                      cache_op_req_byp,
  // Interfaces the pipeline to use the TLB
  input  logic                                   s0_available,
  output logic                                   s0_addr_load,
  output logic [`VA_RANGE]                       s0_addr,
  output logic                                   s0_addr_msb_err,
  output logic                                   s0_thread_id,
  input  logic                                   s1_available,
  output logic                                   s1_addr_load,
  input  logic                                   s1_tlb_wait,
  input  logic                                   s1_tlb_fail,
  input  logic [`PA_TRANS_RANGE]                 s1_tlb_addr,
  // Management of VM status
  input  minion_vm_status [`CORE_NR_THREADS-1:0] vm_status,
  input  minion_vm_status                        vm_status_co,
  output minion_vm_status                        vm_status_l2,
  // Error flags
  output logic [`CORE_NR_THREADS-1:0]            err_flags,
  // Bus error
  output logic [`CORE_NR_THREADS-1:0]            bus_err,
  // Debug signals
  output logic [`DCACHE_DBG_CSR_COUL2_SIZE-1:0]  csr_debug_signals,
  output logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]   sm_match_debug_signals,
  output logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]  sm_filter_debug_signals,
  output logic [`DCACHE_DBG_SM_DATA_BITS-1:0]    sm_data_debug_signals
);

  ////////////////////////////////////////////////////////////////////////////////
  // Computes next state of the cache op FSM
  ////////////////////////////////////////////////////////////////////////////////

  L2_Cache_Op_state state, state_next;    // State of the snoop FSM
  l2_cache_op_req   req, req_next;        // Request FSM is working on

  logic [`VA_RANGE] adder_res;            // Result of adding the address with the stride
  logic [`VA_RANGE] req_addr;             // Tag address of the request
  logic [`VA_RANGE] req_addr_next;
  logic             req_addr_msb_err;     // Address got an overflow error
  logic             req_addr_msb_err_next;
  logic [`VA_RANGE] virtual_addr;         // Composition of the virtual address
  logic [3:0]       req_count;            // Number of operations done
  logic [3:0]       req_count_next;
  logic             new_req;              // New request incoming
  logic             up_addr;              // Update the address
  logic             up_id;                // Update running ID
  logic             cache_op_req_is_l2;   // Cache Op is an L2 request
  logic             cache_op_req_is_l2_next;
  logic             cache_op_req_is_NOP;  // Cache Op is a NOP
  logic             cache_op_req_is_NOP_next;
  logic             en_count;             // Enable the pending count FF
  logic             id_running;           // Value of the running operation ID
  logic             id_running_next;
  logic             err_mem;              // Memory access error
  logic             err_mem_next;

  logic [`DCACHE_L2_SCP_DST_RANGE]  l2_scp_dest;       // Destination in L2 Scp of the L2_ScpFill operations
  logic [`DCACHE_L2_SCP_DST_RANGE]  l2_scp_dest_next;
  logic [`PA_RANGE]                 l2_scp_address;    // Address to use for L2 Scp fills
  logic                             l2_scp_bad_source; // Indication that addres for l2_scp is wrong

  logic [`TENSOR_MASK_BITS-1:0] req_tmask_bits;       // Saved ensor mask bits
  logic [`TENSOR_MASK_BITS-1:0] req_tmask_bits_next;
  logic                         tmask_en;

  logic                        clkgt;     // Gated clock
  logic                        clkgt_en;  // Clock enable signal

  ////////////////////////////////////////////////////////////////////////////////
  // Registered information to process requests
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK       RST       EN        DOUT              DIN                   DEF
  `RST_FF(clkgt,    reset,              state,            state_next,           L2_Cache_Op_Invalid)
  `EN_FF (clkgt,              new_req,  req,              req_next                                 )
  `EN_FF (clkgt,              up_addr,  req_addr,         req_addr_next                            )
  `EN_FF (clkgt,              up_addr,  req_addr_msb_err, req_addr_msb_err_next                    )
  `EN_FF (clkgt,              en_count, req_count,        req_count_next                           )
  `EN_FF (clkgt,              tmask_en, req_tmask_bits,   req_tmask_bits_next                      )
  `EN_FF (clkgt,              up_id,    id_running,       id_running_next                          )
  `EN_FF (clkgt,              up_addr,  l2_scp_dest,      l2_scp_dest_next                         )


  // Store VM status for memory access
  //       CLK    EN       DOUT          DIN
  `EN_FF ( clkgt, new_req, vm_status_l2, cache_op_valid_byp ? vm_status_co : (cache_op_req.thread_id == 1'b1)? vm_status[1] : vm_status[0])

  ////////////////////////////////////////////////////////////////////////////////
  // State Machine
  ////////////////////////////////////////////////////////////////////////////////

  //   CLK    DOUT                 DIN
  `FF (clock, cache_op_req_is_l2,  cache_op_req_is_l2_next)
  `FF (clock, cache_op_req_is_NOP, cache_op_req_is_NOP_next)

always_comb begin

   // Opcode is a cache op providing VA and going straight to L2      
      cache_op_req_is_l2_next =  ((cache_op_req_next.cmd == cache_op_Pref)     && (cache_op_req_next.dest_level  >= cache_op_L2) && (cache_op_req_next.dest_level < cache_op_MEM))                  ||
                                 ((cache_op_req_next.cmd == cache_op_Flush_VA) && (cache_op_req_next.start_level >= cache_op_L2) && (cache_op_req_next.dest_level > cache_op_req_next.start_level)) ||
                                 ((cache_op_req_next.cmd == cache_op_Evict_VA) && (cache_op_req_next.start_level >= cache_op_L2) && (cache_op_req_next.dest_level > cache_op_req_next.start_level));
      
      cache_op_req_is_NOP_next = ((cache_op_req_next.cmd == cache_op_Pref)     && (cache_op_req_next.dest_level  == cache_op_MEM))                                                                   ||
                                 ((cache_op_req_next.cmd == cache_op_Flush_VA) && (cache_op_req_next.start_level >= cache_op_L2) && (cache_op_req_next.dest_level <= cache_op_req_next.start_level)) ||
                                 ((cache_op_req_next.cmd == cache_op_Evict_VA) && (cache_op_req_next.start_level >= cache_op_L2) && (cache_op_req_next.dest_level <= cache_op_req_next.start_level));


    // FSM transition
    state_next = state;
    err_mem_next = 1'b0;

    // -------------- L2_Cache_Op_Invalid --------------------------------------
    if (state == L2_Cache_Op_Invalid) begin
      if (cache_op_valid_byp) begin                       // Requests from CacheOp unit enter here with priority
        state_next = L2_Cache_Op_Request;
      end else if (cache_op_valid && cache_op_req_is_l2) begin  // Regular requests from core enter here
        if (!cache_op_req.use_tmask) begin
          state_next = L2_Cache_Op_Translate;
        end else begin
          state_next = L2_Cache_Op_Wait_Tmask;
        end
      end
    end
    // -------------- L2_Cache_Op_Wait_Tmask -----------------------------------
    else if (state == L2_Cache_Op_Wait_Tmask) begin
      if (cache_op_req.tmask_ready) begin
        if (cache_op_req.tmask_bits[0]) begin             // Once ready arrives if first bit is valid go to translate address
          state_next = L2_Cache_Op_Translate;
        end else if (req_count != req.count) begin
          state_next = L2_Cache_Op_Next_Operation;  // If bit is not valid, jump to next operation, unless we are done
        end else begin
          state_next = L2_Cache_Op_Invalid;
        end
      end
    end
    // -------------- L2_Cache_Op_Translate ------------------------------------
    else if (state == L2_Cache_Op_Translate) begin
      if (s1_addr_load) begin
        if (s1_tlb_fail) begin
          state_next = L2_Cache_Op_Invalid;
          err_mem_next = 1'b1;
        end else if (s1_tlb_wait) begin
          state_next = L2_Cache_Op_Translate_Wait;
        end else begin
          state_next = L2_Cache_Op_Request;
        end
      end
    end else if (state == L2_Cache_Op_Translate_Wait) begin
    // -------------- L2_Cache_Op_Translate_Wait -------------------------------
      if (!s1_tlb_wait) begin
        state_next = L2_Cache_Op_Translate;
      end
    end else if (state == L2_Cache_Op_Request) begin
    // -------------- L2_Cache_Op_Request --------------------------------------
      if ((l2_req_valid && l2_req_ready) || (req.is_l2_scp && l2_scp_bad_source)) begin
        if (req_count == req.count) begin
          state_next = L2_Cache_Op_Invalid;         // L2 cache op commands are done after command sent to L2
        end else begin
          state_next = L2_Cache_Op_Next_Operation;  // L2 cache op done, move to next one
        end
      end
    end else if (state == L2_Cache_Op_Next_Operation) begin
    // -------------- L2_Cache_Op_Next_Operation -------------------------------
      if (!req.use_tmask) begin
        state_next = L2_Cache_Op_Translate;         // When not using mask, simply go to translate a new address
      end else begin
        if (req_tmask_bits[req_count]) begin
          state_next = L2_Cache_Op_Translate;       // Jump to translate if bit is valid
        end else if (req_count == req.count) begin
          state_next = L2_Cache_Op_Invalid;         // When using mask, the last operation can be masked, returned to invalid from here
        end
      end
    end
  end

  // Error reporting
  //      CLK    RST    DOUT     DIN            DEF
  `RST_FF(clkgt, reset, err_mem, err_mem_next,  1'b0)

  assign err_flags = {err_mem  && req.thread_id, err_mem  && !req.thread_id};



  ////////////////////////////////////////////////////////////////////////////////
  // Accepting a new request
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    // When moving from invalid to a different state there's a new request
    new_req = (state == L2_Cache_Op_Invalid) && (state_next != L2_Cache_Op_Invalid);
    up_addr = new_req;

    // Update ID that is running even if operation is a NOP
    up_id = (state == L2_Cache_Op_Invalid) &&
            ((cache_op_valid & cache_op_ready) || (cache_op_valid_byp & cache_op_ready_byp));

    // Request may come from core or from CacheOp unit when the operations
    // either start at L1 or are specified using a set/way. In this case the
    // "bypassed" operation is configured from the CacheOp unit and goes directly
    // to the "Request" state (see FSM above)
    if (cache_op_valid_byp) begin
      req_addr_next        = cache_op_req_byp.addr;

      req_next.thread_id   = cache_op_req_byp.thread_id;
      req_next.cmd         = cache_op_req_byp.cmd;
      req_next.start_level = cache_op_req_byp.start_level;
      req_next.dest_level  = cache_op_req_byp.dest_level;
      req_next.stride      = cache_op_req_byp.stride[`VA_RANGE_CA];
      req_next.use_tmask   = cache_op_req_byp.use_tmask;
      req_next.count       = cache_op_req_byp.count;
      req_next.is_l2_scp   = cache_op_req_byp.l2_scp_fill;
      req_next.is_byp      = 1'b1;

      id_running_next      = cache_op_req_byp.stride[0];
    end else begin
      req_addr_next        = cache_op_req.addr;

      req_next.thread_id   = cache_op_req.thread_id;
      req_next.cmd         = cache_op_req.cmd;
      req_next.start_level = cache_op_req.start_level;
      req_next.dest_level  = cache_op_req.dest_level;
      req_next.stride      = cache_op_req.stride[`VA_RANGE_CA];
      req_next.use_tmask   = cache_op_req.use_tmask;
      req_next.count       = cache_op_req.count;
      req_next.is_l2_scp   = cache_op_req.l2_scp_fill;
      req_next.is_byp      = 1'b0;

      id_running_next      = cache_op_req.stride[0];

    end

    // Bits for the tmask have to be selected once the operation has started
    if (req.is_byp) begin
      req_tmask_bits_next  = cache_op_req_byp.tmask_bits;
    end else begin
      req_tmask_bits_next  = cache_op_req.tmask_bits;
    end

    // No error in address by default
    req_addr_msb_err_next = 1'b0;

    // Saves Tensor Mask bits
    tmask_en = (state == L2_Cache_Op_Wait_Tmask);

    // Increments the virtual address with the stride
    adder_res = virtual_addr + {req.stride,6'b0};

    // If moving to next operation, get adder result
    if (state_next == L2_Cache_Op_Next_Operation) begin
      up_addr               = 1'b1;
      req_addr_next         = adder_res;
      req_addr_msb_err_next = req_addr[`VA_MSB] & !req_addr_next[`VA_MSB];
    end

    // Updates the value of pending elements and L2 Scp destination
    if (new_req) begin
      req_count_next     = 4'b0;                      // Reset counter
      l2_scp_dest_next   = cache_op_req.l2_scp_dest;  // Initial destination
    end else begin
      req_count_next   = req_count + 4'b1;    // Increment otherwise, enable will take care of updating only when needed
      l2_scp_dest_next = l2_scp_dest + `DCACHE_L2_SCP_DST_BITS'b1;
    end

    // Updates pending elements
    en_count = new_req || (state_next == L2_Cache_Op_Next_Operation);
  end


  ////////////////////////////////////////////////////////////////////////////////
  // Address translation control
  ////////////////////////////////////////////////////////////////////////////////
  logic                    s1_addr_load_pre;  // Delayed version of s0_addr_load before being validated by s1_available signal
  logic [`PA_RANGE]        translated_addr;   // Address obtained after TLB translation
  logic [`PA_TRANS_RANGE]  reg_tlb_addr;      // Registered translated address

  //      CLK    RST    EN            DOUT               DIN           DEF
  `RST_FF(clkgt, reset,               s1_addr_load_pre,  s0_addr_load, 1'b0)
  `EN_FF (clkgt,        s1_addr_load, reg_tlb_addr,      s1_tlb_addr       )

  always_comb begin
    s0_addr_load = s0_available && (state == L2_Cache_Op_Translate) && !s1_addr_load_pre;
    s0_thread_id = req.thread_id;
    s1_addr_load = s1_addr_load_pre && s1_available;

    // Virtual address is basically the cache-aligned request address
    virtual_addr = {req_addr[`VA_RANGE_CA],6'b0};
    s0_addr = virtual_addr;
    s0_addr_msb_err = req_addr_msb_err;

    // Address has an untranslated portion that keeps registered here internally
    translated_addr = {reg_tlb_addr,virtual_addr[`PA_UNTRANS_SIZE-1:0]};
  end


  ////////////////////////////////////////////////////////////////////////////////
  // Control the number of outstanding L2 requests
  ////////////////////////////////////////////////////////////////////////////////
  logic [`DCACHE_CO_L2_MAX_OPS_BITS-1:0]       l2_cache_op_cnt;      // Counts outstanding L2 cache ops
  logic [`DCACHE_CO_L2_MAX_OPS_BITS-1:0]       l2_cache_op_cnt_next;
  logic [`DCACHE_CO_L2_MAX_OPS_BITS-1:0]       l2_cache_op_thres;    // Maximum number of L2 cache ops outstanding

  logic                                        l2_resp_is_prefetch;  // Flag indicating that response is for this module

  assign l2_resp_is_prefetch = (et_link_core_miss_id_t'(l2_resp.id) >= ET_LINK_Core_Miss_Id_L2_Prefetch_0) &&
                               (et_link_core_miss_id_t'(l2_resp.id) <= ET_LINK_Core_Miss_Id_L2_Prefetch_3);

  always_comb begin
    // Set limit
    l2_cache_op_thres = cacheop_max;

    // Updates outstanding number of L2 cache ops
    // No need to check the opcode in the response. Whichever it is will be either valid or an error
    //    ((l2_resp.opcode == ET_LINK_RSP_Ack) || (l2_resp.opcode == ET_LINK_RSP_Err))
    l2_cache_op_cnt_next = l2_cache_op_cnt;
    if (l2_req_ready) begin
      l2_cache_op_cnt_next = l2_cache_op_cnt_next + 'b1;
    end
    if (l2_resp_valid && l2_resp_is_prefetch) begin
      l2_cache_op_cnt_next = l2_cache_op_cnt_next - 'b1;
    end
  end

  //      CLK    RST     DOUT              DIN                  DEF
  `RST_FF(clkgt, reset,  l2_cache_op_cnt, l2_cache_op_cnt_next, `DCACHE_CO_L2_MAX_OPS_BITS'b0)

  // Repetition rate control logic [`DCACHE_CO_L2_CYCLE_BITS-1:0] cacheop_reprate_count;
  // When the FSM firstly requests a L2 transfer, a counter starts generating grants.
  // One grant is obtained each time the counter reaches '0'. The FSM starts with one grant.
  // Each time we do a successful request a grant is consumed

  logic [`DCACHE_CO_L2_CYCLE_BITS-1:0] cacheop_reprate_count;      // Counts the number of cycles from request to request
  logic [`DCACHE_CO_L2_CYCLE_BITS-1:0] cacheop_reprate_count_next;
  logic                                cacheop_reprate_count_up;

  logic [`DCACHE_CO_L2_GRANT_BITS-1:0] cacheop_reprate_grant;      // Counts the number of granted requests according to repetition rate counter
  logic [`DCACHE_CO_L2_GRANT_BITS-1:0] cacheop_reprate_grant_next;
  logic                                cacheop_reprate_grant_up;

  logic                                l2_req_valid_started;
  logic                                l2_req_valid_started_next;
  logic                                l2_req_valid_started_up;

  always_comb begin
    // Counter default value
    cacheop_reprate_count_next = cacheop_reprate_count;

    // Next counter value
    if (state == L2_Cache_Op_Invalid) begin
      cacheop_reprate_count_next = (`DCACHE_CO_L2_CYCLE_BITS'b1 << cacheop_reprate) - `DCACHE_CO_L2_CYCLE_BITS'b1;
    end else if (l2_req_valid_started) begin
      if (cacheop_reprate_count != `DCACHE_CO_L2_CYCLE_BITS'b0) begin
        cacheop_reprate_count_next = cacheop_reprate_count - `DCACHE_CO_L2_CYCLE_BITS'b1;
      end else begin
        cacheop_reprate_count_next = (`DCACHE_CO_L2_CYCLE_BITS'b1 << cacheop_reprate) - `DCACHE_CO_L2_CYCLE_BITS'b1;
      end
    end

    // Update counter
    cacheop_reprate_count_up = new_req || (state != L2_Cache_Op_Invalid);
  end

  always_comb begin
    l2_req_valid_started_next = l2_req_valid_started;

    if (state_next == L2_Cache_Op_Request) begin
      l2_req_valid_started_next = 1'b1;
    end else if (state == L2_Cache_Op_Invalid) begin
      l2_req_valid_started_next = 1'b0;
    end

    l2_req_valid_started_up = new_req || (state != L2_Cache_Op_Invalid);
  end

  always_comb begin
    // Grant default value
    cacheop_reprate_grant_next = cacheop_reprate_grant;

    // Next grant value. Start with one grant in the pocket
    if (state == L2_Cache_Op_Invalid) begin
      cacheop_reprate_grant_next = `DCACHE_CO_L2_GRANT_BITS'b1;
    end else begin
      // Add grants, ensure that counter does not overflow
      if (l2_req_valid_started && (cacheop_reprate_count == `DCACHE_CO_L2_CYCLE_BITS'b0)) begin
        if (cacheop_reprate_grant_next != {`DCACHE_CO_L2_GRANT_BITS{1'b1}}) begin
          cacheop_reprate_grant_next = cacheop_reprate_grant_next + `DCACHE_CO_L2_GRANT_BITS'b1;
        end
      end

      // Consume grants. No need to check underflow since grant counter to zero blocks l2_req_valid
      if (l2_req_valid && l2_req_ready) begin
        cacheop_reprate_grant_next = cacheop_reprate_grant_next - `DCACHE_CO_L2_GRANT_BITS'b1;
      end
    end

    // Update grants
    cacheop_reprate_grant_up = new_req || (state != L2_Cache_Op_Invalid);
  end

  //      CLK    EN                        DOUT                   DIN
  `EN_FF (clkgt, cacheop_reprate_count_up, cacheop_reprate_count, cacheop_reprate_count_next)
  `EN_FF (clkgt, cacheop_reprate_grant_up, cacheop_reprate_grant, cacheop_reprate_grant_next)
  `EN_FF (clkgt, l2_req_valid_started_up,  l2_req_valid_started,  l2_req_valid_started_next)

  // Report a bus error when the response is an error
  always_comb begin
    bus_err = {`CORE_NR_THREADS{1'b0}};

    if (l2_resp_valid && l2_resp_is_prefetch && (l2_resp.opcode == ET_LINK_RSP_Err)) begin
      bus_err = {req.thread_id, ~req.thread_id};
    end
  end

  ////////////////////////////////////////////////////////////////////////////////
  // FSM outputs based on current state
  ////////////////////////////////////////////////////////////////////////////////

  // Command to send to upper levels. Map accordingly.

  et_link_req_opcode_t  co_cmd_up;
  always_comb begin
    if ((req.cmd == cache_op_Flush_VA) || (req.cmd == cache_op_Flush_SW)) begin
      co_cmd_up = ET_LINK_REQ_Flush;
    end else if ((req.cmd == cache_op_Evict_VA) || (req.cmd == cache_op_Evict_SW)) begin
      co_cmd_up = ET_LINK_REQ_Evict;
    end else begin
      co_cmd_up = ET_LINK_REQ_Prefetch;
    end
  end

  // Address for L2 Scp fills has an specific format (address is line aligned)
  assign l2_scp_address = {`AD_SCP_REGION,shire_id,l2_scp_dest,6'b0};

  // Check that L2 Scp source address is not the Scp of my shire. This checking dropped from tha spec, so never considering a bad address.
  //assign l2_scp_bad_source = req.is_l2_scp ? ((translated_addr[39:31] == 9'b1) && (translated_addr[29:22] == shire_id)) : 1'b0;
  assign l2_scp_bad_source = 1'b0;

  // Computes actions based on state

  logic l2_req_throttle;

  always_comb begin
    // Ready signal to core
    cache_op_ready = (state == L2_Cache_Op_Invalid) & (cache_op_req_is_l2 | cache_op_req_is_NOP) & !cache_op_valid_byp;

    // Ready signal to CacheOp unit
    cache_op_ready_byp = (state == L2_Cache_Op_Invalid);

    // Throttle requests because of too many outstanding requests or because of too few cycles between requests
    l2_req_throttle = ((l2_cache_op_cnt >= l2_cache_op_thres) && (l2_cache_op_thres != 0)) ||
                      (cacheop_reprate_grant == `DCACHE_CO_L2_GRANT_BITS'b0);

    // Request bus to L2. Generate a request id depending on thread and ID that originated the request
    // to differentiate the response and control the counters for the "ready" signals
    l2_req_valid          = (state == L2_Cache_Op_Request) && !l2_scp_bad_source && !l2_req_throttle;
    l2_req_data.id        = {req.thread_id,id_running} == 2'b00 ? ET_LINK_Core_Miss_Id_L2_Prefetch_0 :
                            {req.thread_id,id_running} == 2'b01 ? ET_LINK_Core_Miss_Id_L2_Prefetch_1 :
                            {req.thread_id,id_running} == 2'b10 ? ET_LINK_Core_Miss_Id_L2_Prefetch_2 :
                                                                  ET_LINK_Core_Miss_Id_L2_Prefetch_3;
    l2_req_data.source    = 1'b0;
    l2_req_data.wdata     = 1'b1;
    l2_req_data.opcode    = req.is_l2_scp ? ET_LINK_REQ_ScpFill : co_cmd_up;
    l2_req_data.subopcode = `ET_LINK_SUBOPCODE_SIZE'b0;
    l2_req_data.address   = req.is_byp    ? virtual_addr[`PA_RANGE] :
                            req.is_l2_scp ? l2_scp_address          :
                                            translated_addr;

    l2_req_data.data              = {`CORE_L2_BLOCK_EXT_SIZE{1'b0}};
    if (req.is_l2_scp) begin
      l2_req_data.data[`PA_RANGE] = translated_addr;
    end else begin
      l2_req_data.data[6:0]       = (req.dest_level << 5) | (req.start_level << 3);
    end

    l2_req_data.size      = req.is_l2_scp ? ET_LINK_DWord : ET_LINK_Byte;
    l2_req_data.qwen      = 4'b0001;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Generates the ready/busy signals
  ////////////////////////////////////////////////////////////////////////////////

  logic [`CORE_NR_THREADS-1:0][1:0][`DCACHE_CO_L2_MAX_OPS_BITS-1:0] ready_counters;
  logic [`CORE_NR_THREADS-1:0][1:0][`DCACHE_CO_L2_MAX_OPS_BITS-1:0] ready_counters_next;
  logic [`CORE_NR_THREADS-1:0][1:0]                                 increment_ready_counters;
  logic [`CORE_NR_THREADS-1:0][1:0]                                 decrement_ready_counters;
  logic                                                             ready_counters_up;

  logic                                                             resp_th;  // Response is for this thread
  logic                                                             resp_id;  // Response if for this ID

  always_comb begin
    // Busy signal
    busy = (state != L2_Cache_Op_Invalid); // Doing an operation

    // Increment the ready counters when doing a new L2 transfer
    increment_ready_counters = {(2*`CORE_NR_THREADS){1'b0}};
    if (l2_req_ready) begin
      increment_ready_counters[req.thread_id][id_running] = 1'b1;
    end

    // Decrement the ready counters when receiving a new L2 response
    decrement_ready_counters = {(2*`CORE_NR_THREADS){1'b0}};
    if (l2_resp_valid && l2_resp_is_prefetch) begin
      case (et_link_core_miss_id_t'(l2_resp.id))
        ET_LINK_Core_Miss_Id_L2_Prefetch_0: {resp_th,resp_id} = 2'b00;
        ET_LINK_Core_Miss_Id_L2_Prefetch_1: {resp_th,resp_id} = 2'b01;
        ET_LINK_Core_Miss_Id_L2_Prefetch_2: {resp_th,resp_id} = 2'b10;
        default:                            {resp_th,resp_id} = 2'b11;
      endcase
      decrement_ready_counters[resp_th][resp_id] = 1'b1;
    end

    // Update counters
    ready_counters_up = (|increment_ready_counters) || (|decrement_ready_counters);

    // Implement counters
    ready_counters_next = ready_counters;
    for (integer th=0; th<`CORE_NR_THREADS; th++) begin
      for (integer id=0; id<=1; id++) begin
        if (increment_ready_counters[th][id] & !decrement_ready_counters[th][id] & !(&ready_counters[th][id])) begin
          ready_counters_next[th][id] = ready_counters[th][id] + `DCACHE_CO_L2_MAX_OPS_BITS'b1;
        // Decrement unless not increment and not at the counter limit (all '0')
        end else if (!increment_ready_counters[th][id] & decrement_ready_counters[th][id] &  (|ready_counters[th][id])) begin
          ready_counters_next[th][id] = ready_counters[th][id] - `DCACHE_CO_L2_MAX_OPS_BITS'b1;
        end
      end
    end

    // Generate ready signals
    for (integer th=0; th<`CORE_NR_THREADS; th++) begin
      for (integer id=0; id<=1; id++) begin
        if ((id_running == id[0]) && (req.thread_id == th[0])) begin
          th_id_complete[th][id] = !busy && (ready_counters[th][id] == 0);
        end else begin
          th_id_complete[th][id] = (ready_counters[th][id] == 0);
        end
      end
    end

  end

  //          CLK    RST    EN                 DOUT            DIN                  DEF
  `RST_EN_FF (clkgt, reset, ready_counters_up, ready_counters, ready_counters_next, 'b0)

  ////////////////////////////////////////////////////////////////////////////////
  // Clock gating
  ////////////////////////////////////////////////////////////////////////////////

  // Activate clock once a new request arrives and until everything is completed
  assign clkgt_en = new_req | busy | !(&th_id_complete) | reset | (|err_flags);

  et_clk_gate cgate_co_l2 (
    .enclk  ( clkgt                         ),
    .en     ( clkgt_en | chicken_bit_dcache ),
    .clk    ( clock                         ),
    .te     ( 1'b0                          )  // DFT has to connect test signals here
  );

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // Debug signals
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  assign csr_debug_signals =  state;
  /* RTLMIN-5542
  assign csr_debug_signals = {                                  // csr 1
                                                                l2_scp_dest,
                                                                id_running,
                                                                // csr 0
                                                                ready_counters,
                                                                l2_cache_op_cnt,
                                                                req_tmask_bits,
                                                                req_count,
                                                                state,
                                                                // req fields
                                                                req.is_l2_scp,
                                                                req.is_byp,
                                                                req.count,
                                                                req.use_tmask,
                                                                req.dest_level,
                                                                req.start_level,
                                                                req.cmd,
                                                                req.thread_id
                                                                };
  */

  assign sm_match_debug_signals = `DCACHE_DBG_SM_MATCH_BITS'b0;

  assign sm_filter_debug_signals = `ZX(`DCACHE_DBG_SM_FILTER_BITS,
          {
            cache_op_valid_byp,
            cache_op_valid,
            ready_counters_up,
            l2_cache_op_cnt,
            err_mem,
            new_req,
            req_addr,
            req,
            state
          });

  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(3)] = `DCACHE_DBG_SM_DATA_GROUP_BITS'b0;
  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(2)] = `DCACHE_DBG_SM_DATA_GROUP_BITS'b0;
  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(1)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
          {
            cache_op_req_is_l2,
            cache_op_req_is_NOP,
            ready_counters,
            l2_cache_op_cnt
          });

  assign sm_data_debug_signals[`DCACHE_DBG_SM_DATA_GROUP(0)] = `ZX(`DCACHE_DBG_SM_DATA_GROUP_BITS,
          {
            l2_scp_dest,
            req_count,
            req_addr,
            req
          });

endmodule

