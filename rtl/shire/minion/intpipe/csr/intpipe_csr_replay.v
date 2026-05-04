// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module intpipe_csr_replay
  (
   input                                                clock,
   input                                                reset,


   // input control signals
   input [`XREG_RANGE]                                  wb_wdata,
   input                                                wb_thread_id,
   input logic [`CORE_NR_THREADS-1:0]                   io_interrupt,
   input logic [`CORE_NR_THREADS-1:0]                   io_redirect,
   // output control signals
   output logic                                         io_replay,
   output logic [`CORE_NR_THREADS-1:0]                  io_replay_stall,
 
   // pulses when accessing a CSR that can trigger a replay
   input                                                reg_co_evict_va_en,
   input                                                reg_co_flush_va_en,
   input                                                reg_co_lock_va_en_qual,
   input                                                reg_co_unlock_va_en_qual,
   input                                                reg_co_lock_sw_en,
   input                                                reg_co_unlock_sw_en,
   input                                                reg_co_prefetch_va_en,
   input                                                reg_co_evict_sw_en,
   input                                                reg_co_flush_sw_en,
   input                                                reg_flb_en,
   input                                                reg_tensor_load_en,
   input                                                reg_tensor_fma_en,
   input                                                reg_tensor_quant_en,
   input                                                reg_tensor_reduce_en,
   input                                                reg_tensor_store_en,
   input [`CORE_NR_THREADS-1:0]                         reg_tensor_conv_ctrl_en,
   input [`CORE_NR_THREADS-1:0]                         reg_tensor_conv_size_en,
   input [`CORE_NR_THREADS-1:0]                         reg_fcc_en,
   input [`CORE_NR_THREADS-1:0]                         reg_tensor_wait_en,
   input                                                reg_tensor_mask_read_invalid,
   input                                                pmu_read_en_wb,
   input [`CORE_NR_THREADS-1:0]                         pmu_read_mem_ready,
   
   // signals to check for conditions
   input                                                dcache_core_ctrl dcache_ctrl_resp,
   input                                                fl_barrier_ready,
   input                                                tensor_fma_replay_condition,
   input                                                tensor_quant_replay_condition,
   input                                                vpu_treduce_enabled,
   input                                                tenb_tl_pend_tfma_en,
   input                                                tenb_restart_process,
   input                                                tload_is_for_tenb,
   input [`CORE_NR_THREADS-1:0]                         conv_can_accept_new_write,
   input [(`CORE_NR_THREADS*`FCC_PER_MIN)-1:0]          fcc_ready,
   input [$clog2(`CORE_NR_THREADS*`FCC_PER_MIN)-1:0]    wb_fcc_addr,
   input [`CORE_NR_THREADS-1:0]                         reg_tensor_mask_ready
   
   );


   logic [`CORE_NR_THREADS-1:0] [CSR_REPLAY_SZ-1:0]  csr_replay_stall;
   logic [`CORE_NR_THREADS-1:0] [CSR_REPLAY_SZ-1:0]  csr_replay_stall_next;
   logic [`CORE_NR_THREADS-1:0] [CSR_REPLAY_SZ-1:0]  clear_stall;
   logic [CSR_REPLAY_SZ-1:0]                         wb_csr_replay;
   logic [`CSR_TENSOR_WAIT_ID_R]                     wb_replay_tensor_wait_cond;
   
   //////////////////////////////////////////////////////////////////////
   // compute replay in current WB cycle ( csr write cannot be handled)
   //////////////////////////////////////////////////////////////////////
   always_comb begin
      
      // no new replays by default
      wb_csr_replay = '0;
      
      //////////////////////////////////////////////////////////////////////      
      // Compute new replays
      //////////////////////////////////////////////////////////////////////
      // Cacheop unit not ready
      `CSR_SET_REPLAY  (CSR_REPLAY_CACHE_OP,  reg_co_evict_va_en,         !dcache_ctrl_resp.cache_op_ready);
      `CSR_SET_REPLAY  (CSR_REPLAY_CACHE_OP,  reg_co_flush_va_en,         !dcache_ctrl_resp.cache_op_ready);
      `CSR_SET_REPLAY  (CSR_REPLAY_CACHE_OP,  reg_co_lock_va_en_qual,     !dcache_ctrl_resp.cache_op_ready);
      `CSR_SET_REPLAY  (CSR_REPLAY_CACHE_OP,  reg_co_unlock_va_en_qual,   !dcache_ctrl_resp.cache_op_ready);
      `CSR_SET_REPLAY  (CSR_REPLAY_CACHE_OP,  reg_co_lock_sw_en,          !dcache_ctrl_resp.cache_op_ready);
      `CSR_SET_REPLAY  (CSR_REPLAY_CACHE_OP,  reg_co_unlock_sw_en,        !dcache_ctrl_resp.cache_op_ready);
      `CSR_SET_REPLAY  (CSR_REPLAY_CACHE_OP,  reg_co_prefetch_va_en,      !dcache_ctrl_resp.cache_op_ready);
      `CSR_SET_REPLAY  (CSR_REPLAY_CACHE_OP,  reg_co_evict_sw_en,         !dcache_ctrl_resp.cache_op_ready);
      `CSR_SET_REPLAY  (CSR_REPLAY_CACHE_OP,  reg_co_flush_sw_en,         !dcache_ctrl_resp.cache_op_ready);
      
      
      //FLB and Barrier and unit not ready
      `CSR_SET_REPLAY  (CSR_REPLAY_FLB,       reg_flb_en,                 !fl_barrier_ready);
      
      //Can't update the tensor load registers if it is still working on previous element
      `CSR_SET_REPLAY  (CSR_REPLAY_TL0,       reg_tensor_load_en,          !dcache_ctrl_resp.tl_ready_0                          && !tload_is_for_tenb);
      `CSR_SET_REPLAY  (CSR_REPLAY_TL1,       reg_tensor_load_en,         (!dcache_ctrl_resp.tl_ready_1 || tenb_tl_pend_tfma_en) &&  tload_is_for_tenb );
      
      // Can't start a tensor fma if still doing one
      `CSR_SET_REPLAY  (CSR_REPLAY_TFMA,      reg_tensor_fma_en,          tensor_fma_replay_condition || tenb_restart_process);
      
      //Can't start a tensor quant if still doing one
      `CSR_SET_REPLAY  (CSR_REPLAY_TQUANT,    reg_tensor_quant_en,        tensor_quant_replay_condition);
      
      //Can't start a new reduce if still doing one
      `CSR_SET_REPLAY  (CSR_REPLAY_TREDUCE,   reg_tensor_reduce_en,       !dcache_ctrl_resp.reduce_ready || vpu_treduce_enabled);
      
      //Can't start a new tensor store if still doing one
      `CSR_SET_REPLAY  (CSR_REPLAY_TS,        reg_tensor_store_en,        !dcache_ctrl_resp.ts_ready  || vpu_treduce_enabled);
      
      //Can't write to Conv CSR if unit can't accept the write because it's busy
      `CSR_SET_REPLAY  (CSR_REPLAY_CONV,      |reg_tensor_conv_ctrl_en,   !conv_can_accept_new_write[wb_thread_id]);
      `CSR_SET_REPLAY  (CSR_REPLAY_CONV,      |reg_tensor_conv_size_en,   !conv_can_accept_new_write[wb_thread_id]);
      
      //Doing an FCC and no credits available
      `CSR_SET_REPLAY  (CSR_REPLAY_FCC0,      |reg_fcc_en,                !fcc_ready[wb_fcc_addr] && wb_wdata[`FCC_PER_MIN_R]  == 1'b0);
      `CSR_SET_REPLAY  (CSR_REPLAY_FCC1,      |reg_fcc_en,                !fcc_ready[wb_fcc_addr] && wb_wdata[`FCC_PER_MIN_R]  == 1'b1);

      // tensor mask
      `CSR_SET_REPLAY  (CSR_REPLAY_TMASK,     reg_tensor_mask_read_invalid,   1'b1);
      
      // Reading from PMU module outside minion
      `CSR_SET_REPLAY ( CSR_REPLAY_PMU_READ,   pmu_read_en_wb,            !pmu_read_mem_ready[wb_thread_id]);
      
      
      //////////////////////////////////////////////////////////////////////
      // tensor wait replays
      //////////////////////////////////////////////////////////////////////
      wb_replay_tensor_wait_cond = wb_wdata[`CSR_TENSOR_WAIT_ID_R];
      
      // tensorLoads (note: no waiting for L2) id 0 and 1
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_TL0,            4'd0,  !dcache_ctrl_resp.tl_complete_0[0] && wb_thread_id == 1'b0);
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_TL1,            4'd1,  !dcache_ctrl_resp.tl_complete_0[1] && wb_thread_id == 1'b0);
      // tensor loads to L2 => mapped the same way as the prefetches
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_PREFETCH0,      4'd2,  !dcache_ctrl_resp.cache_op_complete_l2[wb_thread_id][0]);
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_PREFETCH1,      4'd3,  !dcache_ctrl_resp.cache_op_complete_l2[wb_thread_id][1]);
      // prefetches id 0 and 1
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_PREFETCH0,      4'd4,  !dcache_ctrl_resp.cache_op_complete_l2[wb_thread_id][0]);
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_PREFETCH1,      4'd5,  !dcache_ctrl_resp.cache_op_complete_l2[wb_thread_id][1]);
      // cache ops
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_CACHE_OP,       4'd6,  &dcache_ctrl_resp.cache_op_complete_l2[wb_thread_id] == 1'b0  || &dcache_ctrl_resp.cache_op_complete_l1[wb_thread_id] == 1'b0);
      // tfma
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_TENSOR_FMA,     4'd7,  tensor_fma_replay_condition && wb_thread_id == 1'b0);
      //tensor store
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_TENSOR_STORE,   4'd8,  !dcache_ctrl_resp.ts_ready && wb_thread_id == 1'b0);
      // tensor reduce
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_TENSOR_REDUCE,  4'd9,  !dcache_ctrl_resp.reduce_ready && wb_thread_id == 1'b0);
      `CSR_SET_TENSOR_WAIT_REPLAY(CSR_REPLAY_TW_TENSOR_QUANT,   4'd10, tensor_quant_replay_condition && wb_thread_id == 1'b0);
   end // always_comb

   
   //////////////////////////////////////////////////////////////////////
   // CLEAR STALLS ( when can the CSR be replayed)
   //////////////////////////////////////////////////////////////////////
   // COMPUTE STALL TO FE BECAUSE OF REPLAYS
   always_comb begin
      // no clears by default
      clear_stall = '0;
      
      // clear existing conditions
      for ( int thread = 0; thread < `CORE_NR_THREADS; thread++) begin
         // note: the ones with thread==0 correspond to features that can only happen in thread0
         `CSR_CLEAR_STALL(CSR_REPLAY_CACHE_OP,          dcache_ctrl_resp.cache_op_busy_changed[thread]);
         `CSR_CLEAR_STALL(CSR_REPLAY_FLB,               fl_barrier_ready);
         `CSR_CLEAR_STALL(CSR_REPLAY_TL0,               dcache_ctrl_resp.tl_ready_0);
         `CSR_CLEAR_STALL(CSR_REPLAY_TL1,               dcache_ctrl_resp.tl_ready_1);
         `CSR_CLEAR_STALL(CSR_REPLAY_TFMA,              !tensor_fma_replay_condition);
         `CSR_CLEAR_STALL(CSR_REPLAY_TQUANT,            !tensor_quant_replay_condition);
         `CSR_CLEAR_STALL(CSR_REPLAY_TREDUCE,           dcache_ctrl_resp.reduce_ready && !vpu_treduce_enabled);
         `CSR_CLEAR_STALL(CSR_REPLAY_TS,                dcache_ctrl_resp.ts_ready && !vpu_treduce_enabled);
         `CSR_CLEAR_STALL(CSR_REPLAY_CONV,              conv_can_accept_new_write[thread]);
         `CSR_CLEAR_STALL(CSR_REPLAY_FCC0,              fcc_ready[{thread[0],1'b0}]);
         `CSR_CLEAR_STALL(CSR_REPLAY_FCC1,              fcc_ready[{thread[0],1'b1}]);
         `CSR_CLEAR_STALL(CSR_REPLAY_TMASK,             reg_tensor_mask_ready[thread]);
         `CSR_CLEAR_STALL(CSR_REPLAY_PMU_READ,          pmu_read_mem_ready[thread]);
         `CSR_CLEAR_STALL(CSR_REPLAY_TW_TL0,            dcache_ctrl_resp.tl_complete_0[0] && thread == 1'b0);
         `CSR_CLEAR_STALL(CSR_REPLAY_TW_TL1,            dcache_ctrl_resp.tl_complete_0[1] && thread == 1'b0);
         `CSR_CLEAR_STALL(CSR_REPLAY_TW_PREFETCH0,      dcache_ctrl_resp.cache_op_complete_l2[thread][0]);
         `CSR_CLEAR_STALL(CSR_REPLAY_TW_PREFETCH1,      dcache_ctrl_resp.cache_op_complete_l2[thread][1]);
         `CSR_CLEAR_STALL(CSR_REPLAY_TW_CACHE_OP,       &dcache_ctrl_resp.cache_op_complete_l2[thread] &&  &dcache_ctrl_resp.cache_op_complete_l1[thread]);
         `CSR_CLEAR_STALL(CSR_REPLAY_TW_TENSOR_FMA,     !tensor_fma_replay_condition && thread == 1'b0);
         `CSR_CLEAR_STALL(CSR_REPLAY_TW_TENSOR_STORE,   dcache_ctrl_resp.ts_ready && thread == 1'b0 );
         `CSR_CLEAR_STALL(CSR_REPLAY_TW_TENSOR_REDUCE,  dcache_ctrl_resp.reduce_ready && thread == 1'b0);
         `CSR_CLEAR_STALL(CSR_REPLAY_TW_TENSOR_QUANT,   !tensor_quant_replay_condition && thread == 1'b0);

         // clear stall if we got an interrupt
         if ( io_interrupt[thread] || io_redirect[thread]) clear_stall[thread] = '1;
      end // for ( int thread = 0; thread < `CORE_NR_THREADS; thread++)
   end // always_comb
   

   //////////////////////////////////////////////////////////////////////
   // compute stall per thread
   //////////////////////////////////////////////////////////////////////
   always_comb begin
      // clear stalls
      csr_replay_stall_next = csr_replay_stall & ~clear_stall;
      // set new stalls for the thread in WB
      csr_replay_stall_next[wb_thread_id] |= wb_csr_replay;

      // set replay condition to the core
      io_replay = |wb_csr_replay;

      // set stall condition to FE
      for ( int thread = 0; thread < `CORE_NR_THREADS; thread++) begin
         io_replay_stall[thread] = |csr_replay_stall[thread];
      end

   end // always_comb
   

   localparam self_xor_min_width = 8; 
   genvar i,t;
   generate for (t = 0; t < `CORE_NR_THREADS; t++) begin: CSR_STALL_FF_T
      for (i = 0; i < CSR_REPLAY_SZ; i+=self_xor_min_width) begin: CSR_STALL_FF_I
         logic en; // en will be XOR self gating
         assign en = |(csr_replay_stall     [t][i+:`MIN(CSR_REPLAY_SZ - i, self_xor_min_width)] ^
                       csr_replay_stall_next[t][i+:`MIN(CSR_REPLAY_SZ - i, self_xor_min_width)]);
         `RST_EN_FF(clock, reset, en,
                                      csr_replay_stall     [t][i+:`MIN(CSR_REPLAY_SZ - i, self_xor_min_width)],
                                      csr_replay_stall_next[t][i+:`MIN(CSR_REPLAY_SZ - i, self_xor_min_width)],
                                      '0)
      end
   end
   endgenerate
      
endmodule
