`include "soc_defines.vh"
`include "monitor_defines.vh"

module dreams_event_monitor
(
    input                    reset,
    input                    clock,
    input [`NUM_SHIRE_IDS_R] shire_id,
    input [`MIN_PER_S_R]     shire_min_id
);

   ////////////////////////////////////////////////////////////////////////////
   // Events
   ////////////////////////////////////////////////////////////////////////////

    import "DPI-C" function void event_min_retire(real               cycle,
                                                  input int unsigned thread_id,
                                                  input bit [47:0]   pc,
                                                  input bit [31:0]   inst_bits);

    import "DPI-C" function void event_tfma_change(real               cycle,
                                                   input int unsigned thread_id,
                                                   input bit          value);

    import "DPI-C" function void event_tload_req(real               cycle,
                                                 input int unsigned thread_id,
                                                 input bit          unit,
                                                 input bit          ready,
                                                 input bit [39:0]   address,
                                                 input bit [15:0]   id);

    import "DPI-C" function void event_tload_resp(real               cycle,
                                                  input int unsigned thread_id,
                                                  input bit          unit,
                                                  input bit          ready,
                                                  input bit [39:0]   address,
                                                  input bit [15:0]   id);

    import "DPI-C" function void event_tquant_change(real               cycle,
                                                     input int unsigned thread_id,
                                                     input bit          value);

    import "DPI-C" function void event_tstore_req(real               cycle,
                                                  input int unsigned thread_id,
                                                  input bit          ready,
                                                  input bit [39:0]   address);

    import "DPI-C" function void event_cacheop_req(real               cycle,
                                                   input int unsigned thread_id,
                                                   input bit          ready,
                                                   input bit [39:0]   address,
                                                   input bit [15:0]   id);

    import "DPI-C" function void event_cacheop_resp(real               cycle,
                                                    input int unsigned thread_id,
                                                    input bit [15:0]   id);

  // Configure verbosity level
   logic  dreams;

   // Global minion ID
   logic [`MAX_MINION_ID_R] global_min_id;

   assign global_min_id = {shire_id,shire_min_id};

   // Signals coming from other monitors
   logic [`INST_SIZE-1:0]       wb_reg_inst;

   assign wb_reg_inst = `ARCH_STATE_MON.wb_reg_inst;


   logic event_s0_fma_en;
   logic event_quant_en_next;

   logic [255:0][39:0] event_id_to_addr;      // Maps ID to original addr
   logic [255:0][39:0] event_id_to_addr_next;

   // Dcache signals
   logic                           l2_resp_is_for_tensor_0;
   logic                           l2_resp_is_for_tensor_1;


   //      CLK    RST    DOUT                 DIN                                            DEF
   `RST_FF(clock, reset, event_s0_fma_en,     `MINION_VPU_CTRL.ml.tensorfma.s0_fma_en,       1'b0)
   `RST_FF(clock, reset, event_quant_en_next, `MINION_VPU_CTRL.ml.tensorquant.quant_en_next, 1'b0)
   `FF    (clock,        event_id_to_addr,    event_id_to_addr_next)

   initial begin
      if (!$value$plusargs("DREAMS=%d", dreams)) begin
         dreams = 0;
      end
   end

   // Dcache signals connections
   assign l2_resp_is_for_tensor_0  = (`MINION_DCACHE.l2_resp.opcode == ET_LINK_RSP_AckData) &&
                                     ((et_link_core_miss_id_t'(`MINION_DCACHE.l2_resp.id ) >= et_link_core_miss_id_t'(ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0)) &&
                                      (et_link_core_miss_id_t'(`MINION_DCACHE.l2_resp.id ) <= et_link_core_miss_id_t'(ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0+`DCACHE_TL_L2_TRANSFERS-1)));
   assign l2_resp_is_for_tensor_1  = (`MINION_DCACHE.l2_resp.opcode == ET_LINK_RSP_AckData) &&
                                     ((et_link_core_miss_id_t'(`MINION_DCACHE.l2_resp.id ) >= et_link_core_miss_id_t'(ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0+`DCACHE_TL_L2_TRANSFERS)) &&
                                      (et_link_core_miss_id_t'(`MINION_DCACHE.l2_resp.id ) <= et_link_core_miss_id_t'(ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0+2*`DCACHE_TL_L2_TRANSFERS-1)));


   always_ff @ (negedge clock)
   begin
      if (~reset && dreams) begin
         $timeformat(-9, 3, "", 8);
         // Instruction retired
         if (`MINION_CORE.wb_valid)
         begin
            event_min_retire($realtime, { global_min_id, `MINION_CORE.wb_thread_id }, `MINION_CORE.wb_reg_pc, wb_reg_inst);
         end
         // TensorFMA execution change
         if (`MINION_VPU_CTRL.ml.tensorfma.s0_fma_en != event_s0_fma_en)
            event_tfma_change($realtime, { global_min_id, `MINION_CORE.wb_thread_id }, `MINION_VPU_CTRL.ml.tensorfma.s0_fma_en);
         // TensorQuant
         if (`MINION_VPU_CTRL.ml.tensorquant.quant_en_next != event_quant_en_next)
            event_tquant_change($realtime, { global_min_id, `MINION_CORE.wb_thread_id }, `MINION_VPU_CTRL.ml.tensorquant.quant_en_next);
         // TensorStore
         if (`MINION_DCACHE.reduce.ts_l2_req_valid)
            event_tstore_req($realtime, { global_min_id, `MINION_CORE.wb_thread_id }, `MINION_DCACHE.reduce.ts_l2_req_ready, `MINION_DCACHE.reduce.ts_l2_req.address);
         // Tload0 req
         if (`MINION_DCACHE.tensor_load_0.l2_req_valid)
            event_tload_req($realtime, { global_min_id, `MINION_CORE.wb_thread_id }, 0, `MINION_DCACHE.tensor_load_0.l2_req_ready, `MINION_DCACHE.tensor_load_0.l2_req.address, `MINION_DCACHE.tensor_load_0.l2_req.id);
         // Tload1 req
         if (`MINION_DCACHE.tensor_load_1.l2_req_valid)
            event_tload_req($realtime, { global_min_id, `MINION_CORE.wb_thread_id }, 1, `MINION_DCACHE.tensor_load_1.l2_req_ready, `MINION_DCACHE.tensor_load_1.l2_req.address, `MINION_DCACHE.tensor_load_1.l2_req.id);
         // Tload0 fill
         if (`MINION_DCACHE.l2_resp_valid && l2_resp_is_for_tensor_0)
            event_tload_resp($realtime, { global_min_id, `MINION_CORE.wb_thread_id }, 0, `MINION_DCACHE.l2_resp_ready, event_id_to_addr[`MINION_DCACHE.l2_resp.id], `MINION_DCACHE.l2_resp.id);
         // Tload1 fill
         else if (`MINION_DCACHE.l2_resp_valid && l2_resp_is_for_tensor_1)
            event_tload_resp($realtime, { global_min_id, `MINION_CORE.wb_thread_id }, 1, `MINION_DCACHE.l2_resp_ready, event_id_to_addr[`MINION_DCACHE.l2_resp.id], `MINION_DCACHE.l2_resp.id);
         // Other ack
         else if (`MINION_DCACHE.l2_resp_valid)
            event_cacheop_resp($realtime, { global_min_id, `MINION_CORE.wb_thread_id }, `MINION_DCACHE.l2_resp.id);
      end
   end

   // Tracks what original maps to each ID. Helps dreams visual tracking
   always_comb
   begin
       event_id_to_addr_next = event_id_to_addr;
       if(`MINION_DCACHE.tensor_load_0.l2_req_valid && `MINION_DCACHE.tensor_load_0.l2_req_ready)
           event_id_to_addr_next[`MINION_DCACHE.tensor_load_0.l2_req.id] = `MINION_DCACHE.tensor_load_0.l2_req.address;
       if(`MINION_DCACHE.tensor_load_1.l2_req_valid && `MINION_DCACHE.tensor_load_1.l2_req_ready)
           event_id_to_addr_next[`MINION_DCACHE.tensor_load_1.l2_req.id] = `MINION_DCACHE.tensor_load_1.l2_req.address;
   end

`ifndef MINION_MON_NO_EVENTS
   import "DPI-C" function void new_dcache_events(input int unsigned minion_id,
                                                  input int unsigned thread_id,
                                                  input bit [4:0]    dcache_info);
   import "DPI-C" function void new_decode(input int unsigned      minion_id,
                                           input int unsigned      thread_id,
                                           input bit [`PC_RANGE]   pc,
                                           input bit [`INST_RANGE] inst_bits);
   import "DPI-C" function void new_ex(input int unsigned      minion_id,
                                       input int unsigned      thread_id,
                                       input bit [`PC_RANGE]   pc,
                                       input bit [`XREG_RANGE] src1,
                                       input bit [`XREG_RANGE] src2,
                                       input bit [`XREG_RANGE] res,
                                       input bit               gsc);
   import "DPI-C" function void new_gsc(input int unsigned    minion_id,
                                        input int unsigned    thread_id,
                                        input bit [`PC_RANGE] pc);
   import "DPI-C" function void new_tag(input int unsigned    minion_id,
                                        input int unsigned    thread_id,
                                        input bit [`PC_RANGE] pc,
                                        input bit             gsc);
   import "DPI-C" function void new_mem(input int unsigned    minion_id,
                                        input int unsigned    thread_id,
                                        input bit [`PC_RANGE] pc);
   import "DPI-C" function void new_wb(input int unsigned      minion_id,
                                       input int unsigned      thread_id,
                                       input bit [`PC_RANGE]   pc,
                                       input bit               mem,
                                       input bit [`XREG_RANGE] addr,
                                       input bit               vpu_op);
   import "DPI-C" function void new_fpu(input int unsigned            minion_id,
                                        input int unsigned            thread_id,
                                        input bit                     vpu_packed,
                                        input bit [`VPU_LANE_NUM-1:0] vpu_mask,
                                        input bit                     gather,
                                        input bit [`VPU_LANE_NUM-1:0] gather_elem);
   import "DPI-C" function void new_master_a_command(input int unsigned minion_id,
                                                     input bit [2:0]    opcode,
                                                     input bit [3:0]    size,
                                                     input bit          dcache,
                                                     input bit [5:0]    miss_id,
                                                     input bit [31:0]   address);
   import "DPI-C" function void new_master_d_command(input int unsigned minion_id,
                                                     input bit [2:0]    opcode,
                                                     input bit [3:0]    size,
                                                     input bit          dcache,
                                                     input bit [5:0]    miss_id);

   logic wb_is_vpu_op;

   always_comb
   begin
       wb_is_vpu_op = `MINION_CORE.wb_ctrl.fp                                                               // Regular VPU instruction
       || (`MINION_CORE.csr_file.io_rw_wb_cmd != csr_cmd_t'(core_csr_ctrl_N)) && (`MINION_CORE.csr_file.reg_tensor_fma_en); // TensorFMA
   end

   // All events are sampled at negedge to guarantee that they are "stable"
   always_ff @ (negedge clock)
     begin
        if (~reset)
          begin
             // WARNING: the order of the core stages are important!!
             // DO NOT CHANGE THE ORDER!!!

             // DCache info
             new_dcache_events(global_min_id, `MINION_DCACHE.s2_req.dest.thread_id,
                               { `MINION_DCACHE.s2_nack && `MINION_DCACHE.s2_valid,
                                 `MINION_DCACHE.s2_reg_nack,
                                 `MINION_DCACHE.s2_nack_inflight,
                                 `MINION_DCACHE.s2_nack_miss,
                                 `MINION_DCACHE.s2_nack_rq_conflict });

             // WB
             if (`MINION_CORE.wb_valid)
               new_wb(global_min_id, `MINION_CORE.wb_thread_id,
                      `MINION_CORE.wb_reg_pc,
                      `MINION_CORE.wb_ctrl.mem, // Is mem
                      `MINION_CORE.wb_rf_wdata, // Addr
                      wb_is_vpu_op);            // Is VPU

             // VPU
             if (`MINION_VPU_CTRL.f4_ctrl_valid_qual)
               new_fpu(global_min_id, `MINION_VPU.f4_thread_id,
                      `MINION_VPU_CTRL.f4_ctrl_sigs.dtype[1], // Is PS/PI
                      `ARCH_STATE_MON.f4_regmask0,            // VPU Mask
                      `MINION_CORE.wb_ctrl.gsc,
                      `ARCH_STATE_MON.wb_reg_gsc_cnt);
             // MEM
             if (`MINION_CORE.mem_reg_valid && ~`MINION_CORE.mem_ctrl_kill)
               new_mem(global_min_id, `MINION_CORE.mem_thread_id,
                       `MINION_CORE.mem_reg_pc);

             // TAG
             if (`MINION_CORE.tag_reg_valid && ~`MINION_CORE.tag_ctrl_kill)
               new_tag(global_min_id, `MINION_CORE.tag_thread_id,
                       `MINION_CORE.tag_reg_pc,
                       `MINION_CORE.tag_ctrl.gsc);

             // GSC
             if (`MINION_CORE.gsc_reg_valid && ~`MINION_CORE.ex_ctrl_kill)
               new_gsc(global_min_id, `MINION_CORE.ex_thread_id,
                       `MINION_CORE.ex_reg_pc);

             // EX
             if (`MINION_CORE.ex_reg_valid && ~`MINION_CORE.ex_ctrl_kill)
               new_ex(global_min_id, `MINION_CORE.ex_thread_id,
                      `MINION_CORE.ex_reg_pc,
                      `MINION_CORE.ex_alu_out,
                      `MINION_CORE.ex_op1,
                      `MINION_CORE.ex_op2,
                      `MINION_CORE.ex_ctrl.gsc);

             // Decode
             if (`MINION_CORE.id_valid && ~`MINION_CORE.id_ctrl_kill)
               new_decode(global_min_id, `MINION_CORE.id_thread_id,
                          `MINION_CORE.id_pc,
                          `MINION_CORE.id_inst_bits);

          end // if (~reset)
     end // always_ff @

   // dcache
      always_ff @ (negedge clock)
        begin
           if (~reset)
             begin
                // New Minion to L2
                if (minion.core.l2_dcache_evict_req_ready && minion.core.l2_dcache_evict_req_valid)
                  new_master_a_command(global_min_id,
                                       minion.core.l2_dcache_evict_req.opcode,
                                       minion.core.l2_dcache_evict_req.size,
                                       1'b1,
                                       minion.core.l2_dcache_evict_req.id,
                                       minion.core.l2_dcache_evict_req.address);

                if (minion.core.l2_dcache_miss_req_ready[`DCACHE_L2_MISS_IDX_COMM] && minion.core.l2_dcache_miss_req_valid[`DCACHE_L2_MISS_IDX_COMM])
                  new_master_a_command(global_min_id,
                                       minion.core.l2_dcache_miss_req.opcode,
                                       minion.core.l2_dcache_miss_req.size,
                                       1'b1,
                                       minion.core.l2_dcache_miss_req.id,
                                       minion.core.l2_dcache_miss_req.address);

                // New L2 to Minion
                if (minion.core.l2_dcache_resp_ready && minion.core.l2_dcache_resp_valid)
                  new_master_d_command(global_min_id,
                                       minion.core.l2_dcache_resp.opcode,
                                       minion.core.l2_dcache_resp.size,
                                       1'b1,
                                       minion.core.l2_dcache_resp.id);
             end // if (~reset)
        end // always_ff @

  `endif

endmodule // dreams_event_monitor
