`include "soc_defines.vh"
`include "monitor_defines.vh"

module perf_event_monitor
(
    input                    reset,
    input                    clock,
    input [`NUM_SHIRE_IDS_R] shire_id,
    input [`MIN_PER_S_R]     shire_min_id
);

   ////////////////////////////////////////////////////////////////////////////
   // Perf Events
   ////////////////////////////////////////////////////////////////////////////

    import "DPI-C" function void perf_event_initialize(input bit [63:0] shire_mask,
                                                       input bit [63:0] minion_mask,
                                                       input bit [1:0] thread_mask);

    import "DPI-C" function void perf_event_min_retire(input bit [63:0]   cycle,
                                                       input int unsigned thread_id,
                                                       input bit [47:0] pc,
                                                       input bit [31:0] inst_bits);

    import "DPI-C" function void perf_event_icache_req(input bit[63:0] cycle,
                                                       input int unsigned minion_id,
                                                       input bit [47:0] pc,
                                                       input bit miss,
                                                       input bit [15:0] latency);

    import "DPI-C" function void perf_event_tfma_start(input bit [63:0]   cycle,
                                                       input int unsigned minion_id,
                                                       input bit [63:0] fma_control,
                                                       input bit [`TENSOR_MASK_BITS-1:0] mask_bits);

    import "DPI-C" function void perf_event_tfma_finish(input bit [63:0] cycle,
                                                        input int unsigned minion_id);

    import "DPI-C" function void perf_event_tload_start(input bit [63:0] cycle,
                                                        input int unsigned minion_id,
                                                        input int unsigned load_id);

    import "DPI-C" function void perf_event_tload_finish(input bit [63:0] cycle,
                                                         input int unsigned minion_id,
                                                         input int unsigned load_id);

    import "DPI-C" function void perf_event_tquant_start(input bit [63:0] cycle,
                                                         input int 	  unsigned minion_id,
                                                         input bit [63:0] tquant_control);

    import "DPI-C" function void perf_event_tquant_finish(input bit [63:0] cycle,
                                                          input int unsigned minion_id);

    import "DPI-C" function void perf_event_tstore_start(input bit [63:0] cycle,
                                                         input int unsigned minion_id);

    import "DPI-C" function void perf_event_tstore_finish(input bit [63:0] cycle,
                                                          input int unsigned minion_id);




   // Perf checker -- uncomment when build issues are resolved.

   // export "DPI-C" function event_check_count_precise;

   // import "DPI-C" function void perf_checker_event_count_precise(int thread,
   //								 int evt,
   //								 int exp_count);

   // Enable perf analysis
   logic  perf;

   // These are passed to the perf analysis module to intialize structures
   logic [`NUM_MINION_SHIRES-1:0] shire_mask;
   logic [`NUM_NEIGH*`MIN_PER_N-1:0] minion_mask;
   logic [1:0] thread_mask;

   // Global minion ID
   logic [`MAX_MINION_ID_R] global_min_id;

   assign global_min_id = {shire_id,shire_min_id};

   // Signals coming from other monitors
   logic [`INST_SIZE-1:0]       wb_reg_inst;

   logic [48:0] icache_pc;
   logic icache_req_pending;
   logic [15:0] icache_fetch_latency;

   assign wb_reg_inst = `ARCH_STATE_MON.wb_reg_inst;

   logic fma_working;
   logic tquant_enabled;
   logic tstore_working, tstore_working_reg;
   logic [1:0] tl0_active;
   logic [1:0] tl1_active;

   //      CLK    RST    DOUT                 DIN                                             DEF
   `RST_FF(clock, reset, fma_working,         `MINION_VPU_CTRL.ml.tensorfma.enabled,          1'b0)
   `RST_FF(clock, reset, tquant_enabled,      `MINION_VPU_CTRL.ml.tensorquant.enabled,        1'b0)
   `RST_FF(clock, reset, tl0_active,          `MINION_DCACHE.tensor_load_0.l2_operates_in_id, '{default:0})
   `RST_FF(clock, reset, tl1_active,          `MINION_DCACHE.tensor_load_1.l2_operates_in_id, '{default:0})
   `RST_FF(clock, reset, tstore_working_reg,  tstore_working,                                 1'b0)

   // Tensor store is really working if reduce_wait = 0.
   assign tstore_working = `MINION_DCACHE.reduce.tensorstore_busy & ~`MINION_DCACHE.reduce.vpu_reduce_wait;

   `ifndef ZEBU
      initial begin
         if (!$value$plusargs("PERF=%d", perf)) begin
            perf = 0;
         end
         if (!$value$plusargs("SHIRE_MASK=0x%h", shire_mask)) begin
            shire_mask = 'h1; // default value
         end
         if (!$value$plusargs("MINION_MASK=0x%h", minion_mask)) begin
            minion_mask = 'hffffffff; // default value
         end
         if (!$value$plusargs("THREAD_MASK=0x%h", thread_mask)) begin
            thread_mask = 'h1; // default value
         end
         perf_event_initialize(shire_mask, minion_mask, thread_mask);
      end
   `else // ifndef ZEBU
      initial begin
         perf = 0;
         shire_mask = 'h1; // default value
         minion_mask = 'hffffffff; // default value
         thread_mask = 'h1; // default value
         perf_event_initialize(shire_mask, minion_mask, thread_mask);
      end
   `endif // !ifndef ZEBU

   always_ff @ (negedge clock)
   begin
      if (~reset && perf) begin
         if (`MINION_CORE.wb_valid)
            perf_event_min_retire($time, { global_min_id, `MINION_CORE.wb_thread_id }, `MINION_CORE.wb_reg_pc, wb_reg_inst);


         // FMA working signal changed.
         if (`MINION_VPU_CTRL.ml.tensorfma.enabled != fma_working) begin
            if (`MINION_VPU_CTRL.ml.tensorfma.enabled == 1'b1)
               perf_event_tfma_start($time, global_min_id, `MINION_VPU_CTRL.ml.f0_core_ctrl.tensorfma_ctrl, `MINION_VPU_CTRL.ml.f0_core_ctrl.tensorfma_conv_bits);
            else
               perf_event_tfma_finish($time, global_min_id);
         end

         // IC accesses and misses -- works only for thread 0.
         // Need to play to see on which cycle thread 1 sends requests
         if (minion.core.icache_req_ready & minion.core.icache_req_valid) begin
            icache_pc = minion.core.frontend.FRONTEND_THREAD[0].thread_buffer.f0_pc;
            icache_req_pending = 1'b1;
            icache_fetch_latency = 16'b0;
         end

         if (icache_req_pending == 1'b1) begin
            icache_fetch_latency = icache_fetch_latency + 1;
            if (minion.core.icache_resp_valid) begin
               perf_event_icache_req($time, global_min_id, icache_pc, minion.core.icache_resp_miss, icache_fetch_latency);
            end
         end

         // Tensor QUANT signal changed
         if (`MINION_VPU_CTRL.ml.tensorquant.enabled != tquant_enabled) begin
            if (`MINION_VPU_CTRL.ml.tensorquant.enabled == 1'b1)
               perf_event_tquant_start($time, global_min_id, `MINION_VPU_CTRL.ml.tensorquant.tensorquant_ctrl);
            else
               perf_event_tquant_finish($time, global_min_id);
         end

         // Tensor Load activity (start / finish)
         if (tl0_active[0] != `MINION_DCACHE.tensor_load_0.l2_operates_in_id[0]) begin
            if (`MINION_DCACHE.tensor_load_0.l2_operates_in_id[0] == 1'b1)
               perf_event_tload_start($time, global_min_id, 0);
            else
               perf_event_tload_finish($time, global_min_id, 0);
         end
         if (tl1_active != `MINION_DCACHE.tensor_load_1.l2_operates_in_id) begin
            if ((`MINION_DCACHE.tensor_load_1.l2_operates_in_id[0] |
               `MINION_DCACHE.tensor_load_1.l2_operates_in_id[1]) == 1'b1)
               perf_event_tload_start($time, global_min_id, 1);
            else
               perf_event_tload_finish($time, global_min_id, 1);
         end

         // Tensor store activity
         if (tstore_working != tstore_working_reg) begin
            if (tstore_working == 1'b1)
               perf_event_tstore_start($time, global_min_id);
            else
               perf_event_tstore_finish($time, global_min_id);
         end
      end
   end // always_ff @


   // perf checker -- uncomment when test build issues are resolved
   //function void event_check_count_precise(int thread, int evt, int exp_count);
   //   perf_checker_event_count_precise(thread, evt, exp_count);
   //endfunction


endmodule // perf_event_monitor
