`include "soc_defines.vh"
`include "monitor_defines.vh"
`include "events/cosim_evt.vh"

module arch_state_monitor
(
    input                    reset,
    input                    clock,
    input [1:0]              enabled,
    input [63:0]             soc_cycle,
    input [`NUM_SHIRE_IDS_R] shire_id,
    input [`MIN_PER_S_R]     shire_min_id
);

`define event_common_assign(EVENT) \
    EVENT.cycle = cycle; \
    EVENT.minion_id = global_min_id;

`define event_trigger(EVENT) trigger_``EVENT``_unpack(EVENT);

`define event_unpack(TARGET, SOURCE, NUM_ELEMS = `VPU_LANE_NUM, ELEM_SIZE = `VPU_DATA_S_SZ) \
   for (int k = 0; k < NUM_ELEMS; k++) begin \
      TARGET[k] = SOURCE[k*ELEM_SIZE+:ELEM_SIZE]; \
   end

`define event_copy(TARGET, SOURCE, NUM_ELEMS = `VPU_LANE_NUM) \
   for (int k = 0; k < NUM_ELEMS; k++) begin \
      TARGET[k] = SOURCE[k]; \
   end


   ////////////////////////////////////////////////////////////////////////////
   // DPI Interface with the CoSim Checker
   ////////////////////////////////////////////////////////////////////////////

   import "DPI-C" context function void inst_retired(input longint unsigned cycle,
                                                     input int unsigned minion_id,
                                                     input bit thread_id,
                                                     input longint unsigned pc,
                                                     input int inst_bits,
                                                     input bit delayed_write,
                                                     input bit load_miss,
                                                     input bit wbx,
                                                     input int wbx_rd,
                                                     input longint unsigned wbx_data,
                                                     input bit wbf,
                                                     input int wbf_rd,
                                                     input bit wbm,
                                                     input bit wbmall,
                                                     input int wbm_rd,
                                                     input longint unsigned wbm_data,
                                                     input bit store,
                                                     input int store_size,
                                                     input longint unsigned store_addr,
                                                     input bit [`VPU_DATA_RANGE] store_data,
                                                     input int store_mask,
                                                     input bit delayed_store,
                                                     input int store_age_id,
                                                     input bit scatter,
                                                     input bit [(`VPU_LANE_NUM-1)*`XREG_SIZE-1:0]     prev_scatter_addr,
                                                     input bit [(`VPU_LANE_NUM-1)*`VPU_DATA_S_SZ-1:0] prev_scatter_data,
                                                     input bit gsc32,
                                                     input longint unsigned gsc32_indices,
                                                     input bit tex_send,
                                                     input bit tex_rcv,
                                                     input bit tensor_load,
                                                     input bit tenb,
                                                     input int unsigned tensor_load_num_lines,
                                                     input bit tensor_fma,
                                                     input bit tensor_quant,
                                                     input bit reduce,
                                                     input bit tensor_store,
                                                     input bit flb_valid,
                                                     input int unsigned flb_dest,      // == flb_idx
                                                     input int unsigned flb_match_idx, // == flb_range
                                                     input bit wflags,
                                                     input int unsigned trans_id,
                                                     input int unsigned prv);


   import "DPI-C" context function void inst_exception(input longint unsigned cycle,
                                                       input int unsigned minion_id,
                                                       input bit thread_id,
                                                       input longint unsigned pc,
                                                       input int inst_bits,
                                                       input int wbf_rd,
                                                       input bit is_store,
                                                       input bit is_atomic,
                                                       input bit is_gsc,
                                                       input longint unsigned scatter_addr,
                                                       input int unsigned scatter_data,
                                                       input bit [(`VPU_LANE_NUM-1)*`XREG_SIZE-1:0]     prev_scatter_addr,
                                                       input bit [(`VPU_LANE_NUM-1)*`VPU_DATA_S_SZ-1:0] prev_scatter_data,
                                                       input int unsigned store_mask,
                                                       input int unsigned gsc_size,
                                                       input int gsc_progress,
                                                       input longint unsigned mstatus,
                                                       input longint unsigned cause,
                                                       input longint unsigned tval,
                                                       input longint unsigned epc,
                                                       input longint unsigned mip,
                                                       input int unsigned prv);

   // DPI Events
   inst_interrupt_t    inst_interrupt;
   warm_reset_t        warm_reset;
   clear_interrupt_t   clear_interrupt;
   delayed_write_int_t delayed_write_int;
   flb_update_t        flb_update;
   load_miss_bus_err_t load_miss_bus_err;
   delayed_store_t     delayed_store;
   update_fcsr_flags_t update_fcsr_flags;
   delayed_write_fp_t  delayed_write_fp;
   delayed_write_fp_t  delayed_write_fp_gsc;
   vpu_rf_update_t     vpu_rf_update;
   delayed_write_rm_t  delayed_write_rm;
   ipi_received_t      ipi_received;
   tensorfma_write_t   tensorfma_write;
   tensorfma_write8_t  tensorfma_write8;
   tensorquant_write_t tensorquant_write;
   reduce_write_t      reduce_write;
   tensorstore_write_t tensorstore_write;
   tensorfma_done_t    tensorfma_done;
   tensorquant_done_t  tensorquant_done;
   reduce_done_t       reduce_done;
   tensorstore_done_t  tensorstore_done;


   // Tensor Load
   import "DPI-C" context function void tensorload_mon(input longint unsigned          cycle,
                                                       input int unsigned minion_id,

                                                       input bit          tl_new_op,
                                                       input int unsigned tl_new_op_sel,
                                                       input int unsigned tl_new_op_transform,
                                                       input int unsigned tl_new_op_num_lines,

                                                       input bit [1:0]    tl_busy_rising,
                                                       input bit [1:0]    tl_busy_falling,

                                                       input bit [63:0]   tl_req_id_idx,

                                                       input int          tl_resp_module,
                                                       input int          tl_resp_id_idx,
                                                       input int          tl_resp_entry,
                                                       input bit [255:0]  tl_resp_data,

                                                       input bit          tl_trans_load_valid,
                                                       input int          tl_trans_entry,
                                                       input int          tl_trans_banks,
                                                       input bit [255:0]  tl_trans_load_data,

                                                       input bit          tl_tenb_flush);


   //
   // configure verbosity level
   //
   logic  verbose;
   int    core_verbose_level;
   int    default_verbose_level;
   int    global_verbose_level;
   logic  global_found;
   logic  display_status;
   wire   [63:0]  cycle;
   logic disable_BEMU_msg_tracking;

// always_ff @(posedge clock) begin
//   if (reset == 1'b1)
//     cycle = 0;
//   else
//     cycle = cycle + 1;
// end

   assign cycle = soc_cycle;


   `ifndef ZEBU
      initial begin
         $timeformat (-9);
         if (!$value$plusargs("GLOBAL_LOG_LEVEL=%d", global_verbose_level)) begin
           global_verbose_level = 0; //DEBUG
         end
         else begin
            global_found = 1'b1;
         end
         if (!$value$plusargs("DEFAULT_LOG_LEVEL=%d", default_verbose_level)) begin
           default_verbose_level = global_found ? global_verbose_level : 1 /*INFO*/;
         end
         if (!$value$plusargs("CORE_RTL_LOG_LEVEL=%d", core_verbose_level) ) begin
           core_verbose_level = default_verbose_level;
         end

         // only show messages if core verbose level = debug, and global verbose level = debug
         verbose = (core_verbose_level == 0) && (global_verbose_level == 0);

         if (!$value$plusargs("DISPLAY_STATUS=%d", display_status)) begin
            display_status = 0;
         end
         if (!$value$plusargs("disableBEMUMsgTracker=%d", disable_BEMU_msg_tracking)) begin
            disable_BEMU_msg_tracking = '0;
         end
      end
   `else // ifndef ZEBU
      initial begin
         global_verbose_level = 0;
         global_found = 1'b0;
         default_verbose_level = global_found ? global_verbose_level : 1;
         core_verbose_level = default_verbose_level;
         verbose = (core_verbose_level == 0) && (global_verbose_level == 0);
         display_status = 0;
         disable_BEMU_msg_tracking = '0;
      end
   `endif // !ifndef ZEBU

   // Global minion ID
   logic [`MAX_MINION_ID_R] global_min_id;

   assign global_min_id = {shire_id,shire_min_id};

   // save inst bits inside the monitor (the actual ones might be clock gated and not all bits propagated through the pipe)

   logic [`INST_SIZE-1:0]      ex_reg_inst, tag_reg_inst, mem_reg_inst, wb_reg_inst;
   logic [`XREG_RANGE]         tag_mem_addr, mem_mem_addr, wb_mem_addr;

   `EN_FF(clock,   `MINION_CORE.mem_pc_valid, { wb_mem_addr, wb_reg_inst},  {mem_mem_addr, mem_reg_inst}               )
   `EN_FF(clock,   `MINION_CORE.tag_pc_valid, {mem_mem_addr, mem_reg_inst}, {tag_mem_addr, tag_reg_inst}               )
   `EN_FF(clock,   `MINION_CORE.ex_pc_valid
                || `MINION_CORE.gsc_pc_valid, {tag_mem_addr, tag_reg_inst}, {`MINION_CORE.ex_alu_add_out, ex_reg_inst} )
   `EN_FF(clock,   `MINION_CORE.id_inst_en,   ex_reg_inst,                   `MINION_CORE.id_inst_bits                 )

   logic delayed_write_valid;
   logic flb_update_valid;
   logic delayed_store_valid;
   logic [`INST_SIZE-1:0]    ex_reg_inst_xcpt;
   logic [`INST_SIZE-1:0]    tag_reg_inst_xcpt;
   logic [`INST_SIZE-1:0]    mem_reg_inst_xcpt;
   logic [`INST_SIZE-1:0]    wb_reg_inst_xcpt;
   `EN_FF(clock, `MINION_CORE.ex_reg_inst_ff.enable  || `MINION_CORE.ex_reg_inst_ff.xcpt,  ex_reg_inst_xcpt, `MINION_CORE.ex_reg_inst_ff.in )
   `EN_FF(clock, `MINION_CORE.tag_reg_inst_ff.enable || `MINION_CORE.tag_reg_inst_ff.xcpt, tag_reg_inst_xcpt, ex_reg_inst_xcpt              )
   `EN_FF(clock, `MINION_CORE.mem_reg_inst_ff.enable || `MINION_CORE.mem_reg_inst_ff.xcpt, mem_reg_inst_xcpt, tag_reg_inst_xcpt             )
   `EN_FF(clock, `MINION_CORE.wb_reg_inst_ff.enable  || `MINION_CORE.wb_reg_inst_ff.xcpt,  wb_reg_inst_xcpt,  mem_reg_inst_xcpt             )


   //regmask signals
   logic                                f8_regmask_thread_id;
   logic [`VPU_REGMASK_ALL_DATA_SZ-1:0] f3_regmask_func_data_packed_masked;
   logic [`VPU_REGMASK_ALL_DATA_SZ-1:0] f4_regmask_func_data_packed_masked;
   logic [`VPU_REGMASK_ALL_DATA_SZ-1:0] f5_regmask_func_data_packed_masked;
   logic [`VPU_REGMASK_ALL_DATA_SZ-1:0] f6_regmask_func_data_packed_masked;
   logic [`VPU_REGMASK_ALL_DATA_SZ-1:0] f7_regmask_func_data_packed_masked;
   logic [`VPU_REGMASK_ALL_DATA_SZ-1:0] f8_regmask_func_data_packed_masked;
   logic [`VPU_LANE_NUM-1:0]            f3_mregdata;
   logic [`VPU_REGMASK_ALL_DATA_SZ-1:0] f8_regmask_wdata;
   logic [`VPU_LANE_NUM-1:0]            f4_regmask_store;
   logic [`VPU_LANE_NUM-1:0]            f5_regmask_store;
   logic [`VPU_LANE_NUM-1:0]            f6_regmask_store;
   logic [`VPU_LANE_NUM-1:0]            f7_regmask_store;
   logic [`VPU_LANE_NUM-1:0]            f8_regmask_store;
   logic                                f4_regmask_store_all;
   logic                                f5_regmask_store_all;
   logic                                f6_regmask_store_all;
   logic                                f7_regmask_store_all;
   logic                                f8_regmask_store_all;
   logic [$clog2(`VPU_REGFILE_NUM)-1:0] f5_waddr;
   logic [$clog2(`VPU_REGFILE_NUM)-1:0] f6_waddr;
   logic [$clog2(`VPU_REGFILE_NUM)-1:0] f7_waddr;
   logic [$clog2(`VPU_REGFILE_NUM)-1:0] f8_waddr;
   logic [$clog2(`VPU_REGFILE_NUM)-1:0] f9_waddr;
   logic [$clog2(`VPU_REGMASK_NUM)-1:0] f4_regmask_waddr;
   logic [$clog2(`VPU_REGMASK_NUM)-1:0] f5_regmask_waddr;
   logic [$clog2(`VPU_REGMASK_NUM)-1:0] f6_regmask_waddr;
   logic [$clog2(`VPU_REGMASK_NUM)-1:0] f7_regmask_waddr;
   logic [$clog2(`VPU_REGMASK_NUM)-1:0] f8_regmask_waddr;
   logic [$clog2(`VPU_REGMASK_NUM)-1:0] f8_regmask_waddr_mux;
   logic                                f8_regmask_write;
   logic                                f8_regmask_write_all;
   logic                                f5_regmask_lanes_off;
   logic                                f6_regmask_lanes_off;
   logic                                f7_regmask_lanes_off;
   logic                                f8_regmask_lanes_off;
   logic                                ex_sigs_mask;
   logic                                tag_sigs_mask;
   logic                                mem_sigs_mask;
   logic                                wb_sigs_mask;
   logic                                f8_trans_tmp_wb;
   logic [`VPU_LANE_NUM-1:0]            regfile_store_multipass;
   logic [`VPU_LANE_NUM-1:0]            regfile_store_multipass_accum;
   logic [`VPU_LANE_NUM-1:0]            regfile_store_multipass_accum_next;
   logic [`VPU_LANE_NUM-1:0]            regfile_store;
   logic [`VPU_DATA_RANGE]              regfile_data_packed;
   logic                                f8_is_multipass;
   logic                                f8_regmask_write_multipass;

   // Multipass signals moved from vpu ctrl to monitor
   logic [3:0]                          ex_multipass;
   logic [3:0]                          f7_multipass;
   logic [3:0]                          f8mp_multipass;
   logic [`VPU_LANE_NUM-1:0]            f8_force_multipass_wdata;
   logic [`VPU_LANE_NUM-1:0]            f8mp_force_wdata;
   logic                                f8mp_ctrl_valid_qual;
   logic                                nsleepin_txfma_27;

    // mirror VPU RF
   `FF(clock, f4_regmask_store_all,          `MINION_VPU_CTRL.mask.f3_regmask_store_all && !`MINION_VPU_CTRL.mask.f3_core_kill             )
   `FF(clock, f5_regmask_store_all,          f4_regmask_store_all                                                                          )
   `FF(clock, f6_regmask_store_all,          f5_regmask_store_all                                                                          )
   `FF(clock, f7_regmask_store_all,          f6_regmask_store_all                                                                          )
   `FF(clock, f8_regmask_store_all,          f7_regmask_store_all                                                                          )
   `FF(clock, f4_regmask_store,              `MINION_VPU_CTRL.mask.f3_regmask_store & {`VPU_LANE_NUM{!`MINION_VPU_CTRL.mask.f3_core_kill}} )
   `FF(clock, f5_regmask_store,              f4_regmask_store                                                                              )
   `FF(clock, f6_regmask_store,              f5_regmask_store                                                                              )
   `FF(clock, f7_regmask_store,              f6_regmask_store                                                                              )
   `FF(clock, f8_regmask_store,              f7_regmask_store                                                                              )
   `FF(clock, f4_regmask_waddr,              `MINION_VPU_CTRL.mask.f3_regmask_waddr                                                        )
   `FF(clock, f5_regmask_waddr,              f4_regmask_waddr                                                                              )
   `FF(clock, f6_regmask_waddr,              f5_regmask_waddr                                                                              )
   `FF(clock, f7_regmask_waddr,              f6_regmask_waddr                                                                              )
   `FF(clock, f8_regmask_waddr,              f7_regmask_waddr                                                                              )
   `FF(clock, regfile_store_multipass_accum, regfile_store_multipass_accum_next                                                            )


   // Accumulates the mask for multipass instructions
   always_comb
   begin
      regfile_store_multipass_accum_next = regfile_store_multipass_accum;

      // Get default data for 1st pass
      if (f8_force_multipass_wdata[0]) begin
         regfile_store_multipass_accum_next = 'b0;
      end

      for (integer i = 0; i < 4; i++) begin
         if (f8mp_multipass[i]) begin
            regfile_store_multipass_accum_next[i*2+:2] = f8mp_force_wdata;
         end
      end
   end

   //vpu signals
   //mask thread id
   assign f8_regmask_thread_id = `MINION_VPU_CTRL.f8_thread_id;

   assign f3_mregdata = `MINION_VPU_CTRL.mask.ex_mask_rf[`MINION_VPU_CTRL.f3_thread_id][`MINION_VPU_CTRL.f3_regfile_waddr];

   // Selects between move all and mask op
   always_comb
   begin
      //f3
      f3_regmask_func_data_packed_masked = {`VPU_REGMASK_ALL_DATA_SZ{1'b0}};

      for (int k = 0; k < `VPU_LANE_NUM; k++) begin
         f3_regmask_func_data_packed_masked[k] = !`MINION_VPU_CTRL.mask.f3_regmask_store[k]                                                     ? f3_mregdata[k]                                : // Not updating bits, keep original
                                                 (`MINION_VPU_CTRL.mask.f3_regmask_store[k] && !`MINION_VPU_CTRL.mask.f3_regmask_from_txfma   ) ? `MINION_VPU_CTRL.mask.f3_regmask_wdata_sel[k] : // Writing bits and not TXFMA, get F3 value
                                                                                                                                                  1'b0;                                           // Zero otherwise
      end

      if (`MINION_VPU_CTRL.mask.f3_regmask_store_all) begin
         f3_regmask_func_data_packed_masked = `MINION_VPU_CTRL.mask.f3_regmask_wdata_sel;
      end
   end


   `FF(clock, f4_regmask_func_data_packed_masked, f3_regmask_func_data_packed_masked)
   `FF(clock, f5_regmask_func_data_packed_masked, f4_regmask_func_data_packed_masked)
   `FF(clock, f6_regmask_func_data_packed_masked, f5_regmask_func_data_packed_masked)
   `FF(clock, f7_regmask_func_data_packed_masked, f6_regmask_func_data_packed_masked)
   `FF(clock, f8_regmask_func_data_packed_masked, f7_regmask_func_data_packed_masked)

   // Select from previous value and mask values coming from TXFMA
   always_comb
   begin
      f8_regmask_wdata = f8_regmask_func_data_packed_masked;
      for (int k = 0; k < `VPU_LANE_NUM; k++) begin
         if (`MINION_VPU_CTRL.mask.f8_regmask_store[k]) begin
            f8_regmask_wdata[k] = `MINION_VPU_CTRL.mask.f8_regmask_comp_res[k];
         end
      end
   end


   // Detect if all 4 lanes are off during wb (vpu_ctrl disabled)
   logic [`VPU_LANE_NUM-1:0] f3_mask_rf0, f4_mask_rf0;
   logic                     f4_vpu_lanes_off;


   //  CLK    DOUT         DIN
   `FF(clock, f3_mask_rf0, `MINION_VPU_CTRL.f2_mask_rf0)
   `FF(clock, f4_mask_rf0, f3_mask_rf0)

   assign f4_vpu_lanes_off = `MINION_VPU_CTRL.f4_ctrl_valid &&
                             (f4_mask_rf0 == `VPU_LANE_NUM'b0) &&
                             !( `MINION_VPU_CTRL.f4_ctrl_sigs.ldst && !`MINION_VPU_CTRL.f4_ctrl_sigs.m0ren) &&  // not FSQ2 or FLQ2, which don't use the mask:
                             (`MINION_VPU_CTRL.f4_ctrl_sigs.vector) &&                                          // Instructions that depend on mask
                             !`MINION_VPU_CTRL.f4_ctrl_sigs.maskop &&                                           // Instructions that do not depend on mask
                             !(`MINION_VPU_CTRL.f4_ctrl_sigs.cvt && (   `MINION_VPU_CTRL.f4_ctrl_sigs.cmd == `VPU_FCMD_MVZ_FX
                                                                     || `MINION_VPU_CTRL.f4_ctrl_sigs.cmd == `VPU_FCMD_MVS_FX
                                                                     || `MINION_VPU_CTRL.f4_ctrl_sigs.cmd == `VPU_FCMD_FCMOVM));


   //regmask control signals
   assign f8_regmask_write_multipass = f8mp_ctrl_valid_qual & f8mp_multipass[1]&`MINION_VPU_CTRL.f8_regmask_from_txfma;
   assign f8_regmask_write           = ((f8_regmask_store != 0) & ~(nsleepin_txfma_27&`MINION_VPU_CTRL.f8_regmask_from_txfma)) | f8_regmask_write_multipass | f8_regmask_lanes_off;
   assign f8_regmask_write_all       = f8_regmask_store_all;
   assign f8_regmask_waddr_mux       = (f8_regmask_store != 0) ? f8_regmask_waddr : f8_waddr[$clog2(`VPU_REGMASK_NUM)-1:0];

   logic                             reset_next;
   // Delayed write FP
   logic                             func_wb_packed;
   logic                             func_wb_thread_id;
   logic                             func_wb_thread_id_reg;
   logic [`VPU_DATA_RANGE]           tima_data_packed;
   logic [`VPU_DATA_RANGE]           tima_data_packed_ext;
   logic                             f8_ctrl_sigs_vector;
   logic                             func_wb;
   logic                             func_wb_reg;
   logic                             func_wb_multipass;
   logic                             func_wb_multipass_reg;
   logic                             f5_vpu_lanes_off;
   logic                             f6_vpu_lanes_off;
   logic                             f7_vpu_lanes_off;
   logic                             f8_vpu_lanes_off;
   logic                             f9_vpu_lanes_off;
   logic                             f5_thread_id;
   logic                             f6_thread_id;
   logic                             f7_thread_id;
   logic                             f8_thread_id;
   logic                             f9_thread_id;
   logic [`FREG_ADDR_RANGE]          func_wb_waddr;
   logic [`FREG_ADDR_RANGE]          func_wb_waddr_reg;
   logic [`FREG_ADDR_RANGE]          func_wb_waddr_reg_z1;
   logic                             is_mem_prefetch;
   logic                             load_wb_double;
   logic                             load_wb_packed;
   logic                             load_wb;
   logic                             load_wb_reg;
   logic                             gather_end;
   logic                             gather_end_reg;
   logic                             gather_end_masked;
   logic                             gather_end_masked_reg;
   dcache_type                       dmem_dtype;
   logic [`FREG_ADDR_RANGE]          dmem_load_addr;
   logic [`FREG_ADDR_RANGE]          dmem_load_addr_reg;
   logic                             dmem_thread_id;
   logic                             dmem_thread_id_reg;
   logic [$clog2(`VPU_LANE_NUM)-1:0] dmem_gather_cnt;
   logic                             dmem_gather_valid;
   logic [`VPU_DATA_RANGE]           packed_load_data;
   logic [`VPU_DATA_RANGE]           masked_packed_gather_data;
   logic [`VPU_LANE_NUM-1:0][1:0]    regfile_store_l;
   logic                             f8mp_tensorfma_val;
   logic                             f8mp_tensorquant_val;
   logic                             tensorfma_val_multipass;
   logic                             tensorquant_val_multipass;
   logic [`VPU_LANE_NUM-1:0]         f2_regmask0;
   logic [`VPU_LANE_NUM-1:0]         f3_regmask0;
   logic [`VPU_LANE_NUM-1:0]         f4_regmask0;
   logic                             tensorstore_working;
   logic                             tensorstore_working_reg;
   logic [3:0]                       tensorstore_size;
   logic [4:0]                       tensorstore_qwen;
   logic                             tensorstore_2nd_pass;
   logic [39:0]                      tensorstore_address;

   // The following wire is used specifically during Zebu runs to force this
   // arch_state_monitor off.
   wire reset_local = 1'b0;

   //      CLK    RST    DOUT                        DIN                                                                                        DEF
   `RST_FF(clock, reset, reset_next,                 reset | reset_local,                                                                       1'b0)
   `RST_FF(clock, reset, func_wb_multipass,          f8mp_multipass[1] && f8mp_ctrl_valid_qual && !f8mp_tensorfma_val && !f8mp_tensorquant_val && !f8_trans_tmp_wb && !f8_regmask_store /*&& (|`MINION_VPU_CTRL.f8mp_regfile_wmask)*/, 1'b0)
   `RST_FF(clock, reset, tensorfma_val_multipass,    f8mp_multipass[1] && f8mp_ctrl_valid_qual && f8mp_tensorfma_val,                           1'b0)
   `RST_FF(clock, reset, tensorquant_val_multipass,  f8mp_multipass[1] && f8mp_ctrl_valid_qual && f8mp_tensorquant_val,                         1'b0)
   `RST_FF(clock, reset, f8_is_multipass,            f8mp_ctrl_valid_qual,                                                                      1'b0)
   `RST_FF(clock, reset, tensorstore_working_reg,    tensorstore_working,                                                                       1'b0)

   // functional writes with all lanes disabled
   always_ff @ (posedge clock)
   begin
      f5_vpu_lanes_off     <= `MINION_CORE.wb_valid & `MINION_CORE.wb_ctrl.wfd & f4_vpu_lanes_off & !wb_sigs_mask;
      f5_waddr             <= `MINION_CORE.wb_waddr;
      f6_waddr             <= f5_waddr;
      f7_waddr             <= f6_waddr;
      f8_waddr             <= f7_waddr;
      f9_waddr             <= f8_waddr;
      f5_thread_id         <= `MINION_CORE.wb_thread_id;
      f6_thread_id         <= f5_thread_id;
      f7_thread_id         <= f6_thread_id;
      f8_thread_id         <= f7_thread_id;
      f9_thread_id         <= f8_thread_id;
      f6_vpu_lanes_off     <= f5_vpu_lanes_off;
      f7_vpu_lanes_off     <= f6_vpu_lanes_off;
      f8_vpu_lanes_off     <= f7_vpu_lanes_off;
      f9_vpu_lanes_off     <= f8_vpu_lanes_off;
      ex_sigs_mask         <= (`MINION_VPU_CTRL.id_core_valid) ? (`MINION_VPU_CTRL.id_decoder_sigs.maskop | `MINION_VPU_CTRL.id_decoder_sigs.tointm)
                                                               : 1'b0;
      /*tag_sigs_mask        <= ex_sigs_mask & `MINION_VPU_CTRL.ex_ctrl_valid_qual;
      mem_sigs_mask        <= tag_sigs_mask & `MINION_VPU_CTRL.f2_ctrl_valid_qual;
      wb_sigs_mask         <= mem_sigs_mask & `MINION_VPU_CTRL.f3_ctrl_valid_qual;
      f5_regmask_lanes_off <= f4_vpu_lanes_off & wb_sigs_mask & `MINION_VPU_CTRL.f4_ctrl_valid_qual;*/

      tag_sigs_mask        <= ex_sigs_mask & !`MINION_VPU_CTRL.ex_core_kill;
      mem_sigs_mask        <= tag_sigs_mask & !`MINION_VPU_CTRL.f2_core_kill;
      wb_sigs_mask         <= mem_sigs_mask & !`MINION_VPU_CTRL.f3_core_kill;
      f5_regmask_lanes_off <= f4_vpu_lanes_off & wb_sigs_mask & !`MINION_VPU_CTRL.f4_core_kill;


      f6_regmask_lanes_off <= f5_regmask_lanes_off;
      f7_regmask_lanes_off <= f6_regmask_lanes_off;
      f8_regmask_lanes_off <= f7_regmask_lanes_off;
   end


   // functional writes
   assign f8_ctrl_sigs_vector = `MINION_VPU_CTRL.f8_ctrl_sigs.vector;


   logic tensorfma_val;
   logic tensorfma_val_int8;
   logic tensorquant_val;
   logic f3_tensorfma_val;
   logic f8_reduce_val;
   logic f8_tensorfma_val;
   logic f8_tensorquant_val;
   logic reduce_val;
   logic reduce_bcst_val;
   logic reduce_get_op;
   logic reduce_get_op_d1;
   logic f3_reduce_bcst_val;
   logic [`VPU_LANE_NUM-1:0] tensorfma_regfile_wmask;
   logic [`VPU_LANE_NUM-1:0] f3_tima0_valid;
   logic [`VPU_LANE_NUM-1:0] f3_tima1_valid;
   logic [`VPU_LANE_NUM-1:0] f3_regfile_wmask_l0;

   assign f3_reduce_bcst_val = `MINION_VPU_REDUCE.exec_op && `MINION_VPU_REDUCE.enabled_int && (`MINION_VPU_REDUCE.reduce_op==8);
   assign reduce_val = (|`MINION_VPU.f8_regfile_wen)&f8_reduce_val;
   assign reduce_bcst_val = (`MINION_VPU_CTRL.f3_regfile_wen_l & f3_reduce_bcst_val);

   genvar ii;
   generate
      for (ii = 0; ii < `VPU_LANE_NUM; ii++) begin
         assign f3_regfile_wmask_l0[ii] = `MINION_VPU_CTRL.f3_regfile_wmask_l[ii][0];
      end
   endgenerate

   assign regfile_store = reduce_bcst_val                 ? f3_regfile_wmask_l0
                        : (|{`MINION_VPU.f8_regfile_wen}) ? `MINION_VPU.f8_regfile_wmask[`VPU_LANE_NUM-1:0]
                        :                                   {(`VPU_LANE_NUM){1'b0}};
   assign regfile_store_multipass = { regfile_store_multipass_accum[7:4], `MINION_VPU.f8_regfile_wen[3:2] & `MINION_VPU.f8_regfile_wmask[3:2], regfile_store_multipass_accum[1:0] };

   assign tensorfma_val = (|`MINION_VPU.f8_regfile_wen)&f8_tensorfma_val & !f8_is_multipass;
   assign tensorfma_val_int8 = `MINION_VPU_CTRL.f3_tima_tenc_wen & (|f3_tima0_valid)
                            || `MINION_VPU_CTRL.f3_tima_rf_wen;

   assign tensorquant_val = (|`MINION_VPU.f8_regfile_wen)&f8_tensorquant_val & !f8_is_multipass;

   assign tensorfma_regfile_wmask = `MINION_VPU_CTRL.f8_regfile_wmask;

   assign func_wb = (regfile_store != 0) && !tensorfma_val && !tensorquant_val && !reduce_val && !reduce_bcst_val & !f8_trans_tmp_wb & !f8_is_multipass;

   generate for (genvar k = 0; k < `VPU_LANE_NUM; k++)
   begin
      assign tima_data_packed     [k*`VPU_DATA_S_SZ+:`VPU_DATA_S_SZ] = `MINION_VPU.VPU_LANES[k].lane.LANE_TIMA[0].lane_tima.tima_out;
      assign tima_data_packed_ext [k*`VPU_DATA_S_SZ+:`VPU_DATA_S_SZ] = `MINION_VPU.VPU_LANES[k].lane.LANE_TIMA[1].lane_tima.tima_out;

      `FF(clock, f3_tima0_valid[k], `MINION_VPU.VPU_LANES[k].lane.LANE_TIMA[0].lane_tima.f2_tima_en)
      `FF(clock, f3_tima1_valid[k], `MINION_VPU.VPU_LANES[k].lane.LANE_TIMA[1].lane_tima.f2_tima_en)
   end
   endgenerate

   assign regfile_data_packed = reduce_bcst_val ? `MINION_VPU_CTRL.f3_regfile_wdata_l
                                                : `MINION_VPU.f8_wdata;

   assign tensorstore_working = `MINION_DCACHE.reduce.tensorstore_busy;

   always_comb
   begin
      for (int k = 0; k < `VPU_LANE_NUM; k++) begin
         func_wb_packed |= (regfile_store[k] & f8_ctrl_sigs_vector);
      end
   end

   assign func_wb_waddr = reduce_bcst_val ? `MINION_VPU_CTRL.f3_regfile_waddr_l
                        :                   `MINION_VPU.f8_regfile_waddr; //txfma
   assign func_wb_thread_id = `MINION_VPU.f8_thread_id; //txfma

   `FF(clock, {func_wb_waddr_reg, func_wb_thread_id_reg}, {func_wb_waddr, func_wb_thread_id})
   `RST_FF(clock, reset, { func_wb_reg, func_wb_multipass_reg},  { func_wb, func_wb_multipass}, '0)


   logic tensorfma_val_reg, tensorquant_val_reg, reduce_val_reg, reduce_bcst_val_reg, reduce_bcst_val_reg_z1, tensorquant_val_multipass_reg;

   logic [`VPU_LANE_NUM-1:0] tensorfma_regfile_wmask_reg;

   `RST_FF(clock, reset,
           { tensorfma_val_reg, tensorquant_val_reg, reduce_val_reg, reduce_bcst_val_reg, tensorquant_val_multipass_reg},
           { tensorfma_val, tensorquant_val, reduce_val, reduce_bcst_val, tensorquant_val_multipass},
           '0)
   `FF(clock, tensorfma_regfile_wmask_reg, tensorfma_regfile_wmask)
   `RST_FF(clock, reset, reduce_bcst_val_reg_z1, reduce_bcst_val_reg, 1'b0)
   `RST_FF(clock, reset, func_wb_waddr_reg_z1, func_wb_waddr_reg, 1'b0)

   // Keeps track of the gather loads
   logic [`XREG_ADDR_RANGE] gsc_gather_waddr, tag_gather_waddr, mem_gather_waddr, wb_gather_waddr, f5_gather_waddr;

   logic [`CORE_GSC_CNT_RANGE] tag_reg_gsc_cnt, mem_reg_gsc_cnt, wb_reg_gsc_cnt; // Gather/Scatter count from WB stage
   `RST_FF(clock, reset, tag_reg_gsc_cnt, `MINION_CORE.gsc_reg_gsc_cnt, '0)
   `RST_FF(clock, reset, mem_reg_gsc_cnt, tag_reg_gsc_cnt, '0)
   `RST_FF(clock, reset, wb_reg_gsc_cnt, mem_reg_gsc_cnt, '0)

   always_ff @(posedge clock)
   begin
      if (reset) begin
         gsc_gather_waddr <= `XREG_ADDR_SIZE'(0);
         tag_gather_waddr <= `XREG_ADDR_SIZE'(0);
         mem_gather_waddr <= `XREG_ADDR_SIZE'(0);
         wb_gather_waddr <= `XREG_ADDR_SIZE'(0);
         f5_gather_waddr <= '0;
      end
      else begin
         gsc_gather_waddr <= `MINION_CORE.ex_waddr;
         tag_gather_waddr <= gsc_gather_waddr;
         mem_gather_waddr <= tag_gather_waddr;
         wb_gather_waddr <= mem_gather_waddr;
         f5_gather_waddr <= wb_gather_waddr;
      end
   end

   logic [`CORE_NR_THREADS-1:0][31:0][`VPU_LANE_NUM-1:0] gather_pending_reg, gather_pending, gather_mask;
   `RST_FF(clock, reset, gather_pending_reg, gather_pending, '{default:0})

   always_comb
   begin
      gather_pending = gather_pending_reg;

      // If doing a gather set the pending bit in the gather mask
      if (`MINION_CORE.tag_reg_valid && !`MINION_CORE.tag_ctrl_kill && `MINION_CORE.tag_ctrl.gsc && ((`MINION_CORE.tag_ctrl.mem_cmd == dcache_cmd_XRD) || (`MINION_CORE.tag_ctrl.mem_cmd == dcache_cmd_XRDB) || dcache_cmd_is_amo(`MINION_CORE.tag_ctrl.mem_cmd)))
      begin
         // Set all the bits starting at the current counter to pending
         for (integer i = 0; i < `VPU_LANE_NUM; i++)
         begin
           // Clear old bits in case an xcpt leave them dirty
           gather_mask[`MINION_CORE.tag_thread_id][`MINION_CORE.tag_reg_inst[11:7]][i] = `MINION_DCACHE.s1_req.ps_mask[i];
            if (i >= tag_reg_gsc_cnt)
            begin
               //gather_mask[`MINION_CORE.tag_thread_id][`MINION_CORE.tag_reg_inst[11:7]][i] = `MINION_DCACHE.s1_req.ps_mask[i];
              gather_pending[`MINION_CORE.tag_thread_id][`MINION_CORE.tag_reg_inst[11:7]][i] = 1'b1;
            end
         end
      end
      // Clear the pending gather bit upon load gather
      if (dmem_gather_valid && gather_mask[dmem_thread_id][`MINION_VPU.f4_regfile_waddr_l][dmem_gather_cnt] == 1'b1) begin
         gather_pending[dmem_thread_id][`MINION_VPU.f4_regfile_waddr_l][dmem_gather_cnt] = 1'b0;
      end

      // Clear pending gather bit for masked bits
      // It is done in WB stage to avoid an early 'delayed write'
      if (`MINION_CORE.wb_pre_valid && `MINION_CORE.wb_ctrl.gsc && (`MINION_CORE.wb_ctrl.mem_cmd inside {dcache_cmd_XRD,dcache_cmd_XRDB} || dcache_cmd_is_amo(`MINION_CORE.wb_ctrl.mem_cmd)) && gather_mask[`MINION_CORE.wb_thread_id][wb_gather_waddr][wb_reg_gsc_cnt] == 1'b0) begin
         gather_pending[`MINION_CORE.wb_thread_id][wb_gather_waddr][wb_reg_gsc_cnt] = 1'b0;
      end
   end

   dcache_type wb_dcache_type;
   logic gather_wb_packed;
   assign wb_dcache_type = dcache_type'(`MINION_CORE.wb_ctrl.mem_type);
   assign gather_wb_packed = dcache_type_is_ps(wb_dcache_type);



   // load and gather writes

  `RST_FF(clock, reset, load_wb, `MINION_VPU.wb_dcache_resp_valid && !dcache_type_is_gsc(`MINION_VPU.wb_dcache_resp.typ) && !`MINION_DCACHE.s3_reduce_op_inst,1'b0)
  `RST_FF(clock, reset, dmem_gather_valid, `MINION_VPU.wb_dcache_resp_valid && dcache_type_is_gsc(`MINION_VPU.wb_dcache_resp.typ),1'b0)
  `RST_FF(clock, reset, dmem_gather_cnt, `MINION_VPU.wb_dcache_resp.gdst,'0)
  `EN_FF(clock, `MINION_VPU.wb_dcache_resp_valid, dmem_dtype, dcache_type'(`MINION_VPU.wb_dcache_resp.typ))
  `EN_FF(clock, `MINION_VPU.wb_dcache_resp_valid, dmem_load_addr,  `MINION_VPU.wb_dcache_resp.addr)
  `EN_FF(clock, `MINION_VPU.wb_dcache_resp_valid, dmem_thread_id,  `MINION_VPU.wb_dcache_resp.thread_id)
  `FF(clock, {dmem_load_addr_reg, dmem_thread_id_reg}, {dmem_load_addr, dmem_thread_id})


   assign gather_end = dmem_gather_valid &&
                       gather_pending[dmem_thread_id][`MINION_VPU.f4_regfile_waddr_l] == {`VPU_LANE_NUM{1'b0}} &&
                       gather_pending_reg[dmem_thread_id][`MINION_VPU.f4_regfile_waddr_l] != {`VPU_LANE_NUM{1'b0}};

   assign gather_end_masked = `MINION_CORE.wb_pre_valid && (!dmem_gather_valid || dmem_thread_id != `MINION_CORE.wb_thread_id)  &&
                              gather_mask[`MINION_CORE.wb_thread_id][wb_gather_waddr] != {`VPU_LANE_NUM{1'b0}} &&
                              gather_pending[`MINION_CORE.wb_thread_id][wb_gather_waddr] == {`VPU_LANE_NUM{1'b0}} &&
                              gather_pending_reg[`MINION_CORE.wb_thread_id][wb_gather_waddr] != {`VPU_LANE_NUM{1'b0}};

   `RST_FF(clock, reset, {gather_end_masked_reg, gather_end_reg, load_wb_reg}, {gather_end_masked, gather_end, load_wb}, '0)

   assign packed_load_data = `MINION_VPU.f4_regfile_wdata_l;
   assign regfile_store_l  = `MINION_VPU.f4_regfile_wmask_l;
   assign load_wb_double   = (dmem_dtype == dcache_type_D);
   assign load_wb_packed   = dcache_type_is_ps(dmem_dtype);
   assign is_mem_prefetch  = `MINION_CORE.wb_ctrl.mem && (`MINION_CORE.wb_waddr == 0) && `MINION_CORE.wb_ctrl.wxd;

   // gather xcpt managment
   localparam int DFP_GSC_XCPT_ID = 0;
   localparam int DFP_VPU_MASKED_ID = 1;
   localparam int DFP_WB_REG_ID = 2;
   localparam int DFP_WB_REG_MULTIPASS_ID = 3;
   localparam int DFP_LOAD_ID = 4;
   localparam int DFP_GSC_ID = 5;

   logic gather_xcpt_with_pending_loads;
   logic   [`CORE_NR_THREADS-1:0][31:0] pending_gsc_xcpt;
   integer expected_gsc_progress [`CORE_NR_THREADS-1:0];
   logic [`XREG_RANGE] gsc_progress_xcpt_reg;
   integer cnt;

   logic wb_reg_xcpt_interrupt;
   logic wb_xcpt_interrupt;

   wire warm_rst;

   reg reg_warm_reset;
   assign warm_rst = `MINION_CORETOP.reset_w;

   // Warm Reset
   always @(posedge clock) begin
      if(reset) begin
         reg_warm_reset <= '0;
      end
      else begin
         if (|(~warm_rst & reg_warm_reset) == 1'b1) begin
            `event_common_assign(warm_reset)
            `event_trigger(warm_reset)
         end
         reg_warm_reset <= warm_rst;
      end
   end

   wire [`CORE_NR_THREADS-1:0][7:0] external_int;

   reg  [`CORE_NR_THREADS-1:0][7:0] reg_external_int;

   genvar iii;
   generate
      for( iii = 0; iii < `CORE_NR_THREADS; iii++ )
      begin
         assign external_int[iii] = { `MINION_CORE.interrupts.mieco[iii],
                                      `MINION_CORE.interrupts.mtip[iii],
                                      `MINION_CORE.interrupts.msip[iii],
                                      `MINION_CORE.interrupts.meip[iii],
                                      `MINION_CORE.interrupts.seip[iii] };
      end
   endgenerate

   always @(posedge clock) begin
      if(reset) begin
         reg_external_int[0] <= '0;
         reg_external_int[1] <= '0;
      end
      else begin
         for( integer thread = 0; thread < `CORE_NR_THREADS; thread++ )
         begin
            if ((~external_int[thread] & reg_external_int[thread]) == 1'b1) begin
               `event_common_assign(clear_interrupt)
               clear_interrupt.thread_id = thread;
               clear_interrupt.mip = ~external_int[thread] & reg_external_int[thread];
               `event_trigger(clear_interrupt)
            end
            reg_external_int[thread] <= external_int[thread];
         end
      end
   end


   `RST_EN_FF(clock, reset, `MINION_CORE.mem_xcpt, wb_reg_xcpt_interrupt, `MINION_CORE.mem_reg_xcpt_interrupt, 0); // just to avoid X
   assign wb_xcpt_interrupt = `MINION_CORE.wb_reg_xcpt ? wb_reg_xcpt_interrupt : 1'b0;
   assign gsc_progress_xcpt_reg = `MINION_CSR.reg_gsc_progress[`MINION_CORE.wb_thread_id];

   always_ff @(posedge clock) begin
      if (reset) begin
         for(int i = 0; i < `CORE_NR_THREADS; ++i) begin
            expected_gsc_progress[i] <= -1;
            pending_gsc_xcpt[i] <= '0;
         end
         cnt <= 0;
      end
      else begin
         // Capture new gsc_progress
         if (`MINION_CORE.wb_xcpt && wb_xcpt_interrupt == 1'b0 && `MINION_CORE.wb_ctrl.gsc && ((`MINION_CORE.wb_ctrl.mem_cmd inside {dcache_cmd_XRD, dcache_cmd_XRDB, dcache_cmd_G32}) || dcache_cmd_is_amo(`MINION_CORE.wb_ctrl.mem_cmd))) begin

            // Skip masked gather scatter if there are no pending access
            gather_xcpt_with_pending_loads = 1'b0;
            for(int i = 0; i < `VPU_LANE_NUM; ++i) begin
               if (i < gsc_progress_xcpt_reg && gather_mask[`MINION_CORE.wb_thread_id][wb_gather_waddr][i]) begin
                  gather_xcpt_with_pending_loads = 1'b1;
               end
            end

            if (gather_xcpt_with_pending_loads) begin
               expected_gsc_progress[`MINION_CORE.wb_thread_id] <= gsc_progress_xcpt_reg;
               pending_gsc_xcpt[`MINION_CORE.wb_thread_id][wb_gather_waddr] <= 1'b1;
            end
         end

         // Wait until all gather accesses have been done and then update the counter.
         for (int j = 0; j < 32; ++j) begin // register
            if (pending_gsc_xcpt[dmem_thread_id_reg][j]) begin
               cnt = 0;
               for (int i = 0; i < `VPU_LANE_NUM; ++i) begin // lane
                  cnt += !gather_pending_reg[dmem_thread_id_reg][j][i];
               end

               if (expected_gsc_progress[dmem_thread_id_reg] == cnt) begin
                  delayed_write_fp_gsc.cycle = cycle;
                  delayed_write_fp_gsc.minion_id = global_min_id;
                  delayed_write_fp_gsc.thread_id = dmem_thread_id_reg;
                  delayed_write_fp_gsc.wbf_rd = dmem_load_addr_reg;
                  delayed_write_fp_gsc.entry_point_id = DFP_GSC_XCPT_ID; // delayed write for xcpt gather. Wait until all the gather have been done.
                  trigger_delayed_write_fp_unpack(delayed_write_fp_gsc);
                  expected_gsc_progress[dmem_thread_id_reg] <= -1;
                  pending_gsc_xcpt[dmem_thread_id_reg][j] <= 1'b0;
               end
            end
         end
      end
   end

   localparam MAX_PREV_ST = 8;

   logic                             s2_tlb_miss;
   logic                             s3_rq_push;
   logic                             s3_uc_load_1st;
   logic                             is_store;
   logic                             is_atomic;
   logic [`DCACHE_REPLAYQ_AGE_ID]    store_age_id;
   logic [`VPU_DATA_RANGE]           store_data;
   logic [`XREG_RANGE]               store_addr;
   logic [31:0]                      store_size;
   logic [`DMEM_REQ_PS_MASK_SZ-1:0]  store_ps_mask, store_ps_mask_next;
   logic [`CORE_GSC32_IDX_V_RANGE]   gsc32_indices;
   logic                             is_gsc32;
   logic                             store_uc;
   logic                             store_delayed;
   logic                             store_misaligned;
   logic                             disable_delayed_stores;

   `ifndef ZEBU
      initial begin
         if (!$value$plusargs("MONITOR_DISABLE_DELAYED_STORES=%d", disable_delayed_stores)) begin
            disable_delayed_stores = 0;
         end
      end
   `else // ifndef ZEBU
      initial begin
         disable_delayed_stores = 0;
      end
   `endif // !ifndef ZEBU


   `FF(clock, s2_tlb_miss,    `MINION_DCACHE.s1_tlb_resp_data.miss)
   `FF(clock, s3_rq_push,     `MINION_DCACHE.s2_rq_push)
   `FF(clock, s3_uc_load_1st, `MINION_DCACHE.s2_uc_load_1st)

   always_comb
   begin
      is_store = `MINION_CORE.wb_ctrl.mem && dcache_cmd_is_write(`MINION_CORE.wb_ctrl.mem_cmd); // Store
      is_atomic = `MINION_CORE.wb_ctrl.mem && dcache_cmd_is_amo(`MINION_CORE.wb_ctrl.mem_cmd); // Atomic

      store_age_id = `MINION_DCACHE.replay_queue.push_age - 'b1;
      // Doing rotate to correctly get the store data for misaligned case
      store_data   = (`MINION_DCACHE.s3_req.cmd == dcache_cmd_SC32) ? { `MINION_DCACHE.s3_store_data }
                                                                    : ({ `MINION_DCACHE.s3_req_to_s4.data, `MINION_DCACHE.s3_req_to_s4.data } >> { `MINION_DCACHE.s3_req_to_s4.addr[4:0], 3'b000 });
      store_addr = wb_mem_addr;

      store_delayed = disable_delayed_stores ? 1'b0 :
                     !`MINION_DCACHE.s3_valid         // Store miss (except for scatters, UCs not missed and masked off)
                     && !`MINION_CORE.wb_ctrl.gsc
                     && !(`MINION_DCACHE.s3_req.cmd == dcache_cmd_SC32)
                     && !store_uc
                     && !store_misaligned
                     && !(`MINION_VPU_CTRL.f4_ctrl_sigs.m0ren & f4_vpu_lanes_off && `MINION_CORE.wb_ctrl.fp);

      is_gsc32 = `MINION_CORE.wb_ctrl.mem_cmd == `M_SC32 ||   // Gather/scatter on 32-byte blocks
                 `MINION_CORE.wb_ctrl.mem_cmd == `M_G32;
   end

   logic [2:0] ex_gsc_start_cnt, gsc_gsc_start_cnt, tag_gsc_start_cnt, mem_gsc_start_cnt,   wb_gsc_start_cnt;
   logic ex_start_cnt_valid;
   logic [`CORE_NR_THREADS-1:0] replay_tlb_miss, replay_tlb_miss_next;
   logic [`CORE_NR_THREADS-1:0][`PC_RANGE_EXT] last_ex_dcache_req_pc, last_ex_dcache_req_pc_next;
   // Using S2 version because S3 might not be valid due clock not enabled for
   // misses and nacks
   //  CLK    DOUT                   DIN
   `FF(clock, store_ps_mask,    store_ps_mask_next)
   `FF(clock, store_uc,         !`MINION_DCACHE.s2_req.cacheable && !s2_tlb_miss)
   `FF(clock, store_misaligned, `MINION_DCACHE.s2_misaligned)
   `FF(clock, store_size,       (`MINION_DCACHE.s2_req.typ[3:2] == 2'b10) ? `VPU_DATA_SZ >> 3 : 4'b0001 << `MINION_DCACHE.s2_req.typ[1:0])
   `FF(clock, gsc32_indices,    `MINION_DCACHE.s2_req.gsc32_idx)

   // save start gsc progress when starting a new instruction (but not one that is replayed because of TLB miss)
   `EN_FF(clock, `MINION_CORE.id_valid_qual, ex_gsc_start_cnt,`MINION_CORE.id_reg_gsc_cnt)
   `EN_FF(clock, ex_start_cnt_valid, gsc_gsc_start_cnt, ex_gsc_start_cnt)
   `FF(clock, tag_gsc_start_cnt, gsc_gsc_start_cnt)
   `FF(clock, mem_gsc_start_cnt, tag_gsc_start_cnt)
   `FF(clock, wb_gsc_start_cnt, mem_gsc_start_cnt)
   `RST_FF(clock, reset, replay_tlb_miss, replay_tlb_miss_next, '0)
   `RST_FF(clock, reset, last_ex_dcache_req_pc, last_ex_dcache_req_pc_next, '0);

   always_comb begin
      replay_tlb_miss_next = replay_tlb_miss;

      if (`MINION_CORE.id_valid_qual) begin
         replay_tlb_miss_next[`MINION_CORE.id_thread_id] = 1'b0;
      end
      if(`MINION_CORE.mem_dcache_flush_pipeline && !`MINION_CORE.mem_ctrl_kill_no_tlb_miss) begin
         replay_tlb_miss_next[`MINION_CORE.mem_thread_id] = 1'b1;
      end

      ex_start_cnt_valid = `MINION_CORE.ex_dcache_req_valid &&
                           `MINION_CORE.ex_reg_pc !== last_ex_dcache_req_pc[`MINION_CORE.ex_thread_id];

      // for gsc, take into account the initial gsc progress counter => set unused lanes as if they were masked
      store_ps_mask_next = `MINION_CORE.mem_ctrl.gsc == 1'b0 ? `MINION_DCACHE.s2_req.ps_mask :
                           `MINION_DCACHE.s2_req.ps_mask  & ~((1 << mem_gsc_start_cnt) -1 );

      last_ex_dcache_req_pc_next = last_ex_dcache_req_pc;
      if( `MINION_CORE.ex_dcache_req_valid) begin
         last_ex_dcache_req_pc_next[`MINION_CORE.ex_thread_id] = `MINION_CORE.ex_reg_pc;
      end
   end // always_comb

   // scatter address and data
   logic [(`VPU_LANE_NUM-1)*`XREG_SIZE-1:0]            prev_scatter_addr;
   logic [(`DMEM_REQ_PS_MASK_SZ-1)*`VPU_DATA_S_SZ-1:0] prev_scatter_data;
   logic [`VPU_DATA_RANGE]                             scatter_store_data;

   // Scatters are not check when they miss, so we send the S2 data directly in
   // the check
   `FF(clock, scatter_store_data, `MINION_DCACHE.s2_req.data)

   always_ff @ (posedge clock)
   begin
      if (`MINION_CORE.wb_pre_valid && `MINION_CORE.wb_reg_gscing) begin
         prev_scatter_addr[wb_reg_gsc_cnt*`XREG_SIZE +: `XREG_SIZE] <= wb_mem_addr;
         prev_scatter_data[wb_reg_gsc_cnt*`VPU_DATA_S_SZ +: `VPU_DATA_S_SZ] <= scatter_store_data[`VPU_DATA_S_SZ-1:0];
      end
   end

   // control end of WFI to allocate rbox buffers to one of the threads
   logic [`CORE_NR_THREADS-1:0] prev_csr_stall;
   `RST_FF(clock, reset, prev_csr_stall, `MINION_CORE.id_csr_stall,'0)


   logic  int_res_delayed;
   logic  fp_res_delayed;
   logic  store_data_select;
   logic  write_delayed;

   // Delays the mask to WB
   `FF(clock, f2_regmask0, `MINION_VPU_CTRL.ex_mask_rf0)
   `FF(clock, f3_regmask0, f2_regmask0)
   `FF(clock, f4_regmask0, f3_regmask0)

   ////////////////////////////////////////////////////////
   // Fflags checking
   ///////////////////////////////////////////////////////
   integer trans_id;
   integer fflags_update_trans_id;
   logic   instr_wflags;
   logic   tensor_quant_wflags;
   logic   tensor_reduce_wflags;

   logic tensorreduce_is_done;
   logic tensorfma_is_done;
   logic tensorquant_is_done;
   logic tensorquant_is_done_scp_err;


   logic [1:0] id_reduce_action_next;
   logic [1:0] id_reduce_action;
   logic [1:0] ex_reduce_action;
   logic [1:0] f2_reduce_action;
   logic [1:0] f3_reduce_action;
   logic [1:0] f4_reduce_action;
   logic [1:0] f5_reduce_action;
   logic [1:0] f6_reduce_action;
   logic [1:0] f7_reduce_action;
   logic [1:0] tensorreduce_action;

   logic [12:0] id_reduce_partner_id_next;
   logic [12:0] id_reduce_partner_id;
   logic [12:0] ex_reduce_partner_id;
   logic [12:0] f2_reduce_partner_id;
   logic [12:0] f3_reduce_partner_id;
   logic [12:0] f4_reduce_partner_id;
   logic [12:0] f5_reduce_partner_id;
   logic [12:0] f6_reduce_partner_id;
   logic [12:0] f7_reduce_partner_id;
   logic [12:0] tensorreduce_partner_id;

   logic [3:0] id_reduce_operation_next;
   logic [3:0] id_reduce_operation;
   logic [3:0] ex_reduce_operation;
   logic [3:0] f2_reduce_operation;
   logic [3:0] f3_reduce_operation;
   logic [3:0] f4_reduce_operation;
   logic [3:0] f5_reduce_operation;
   logic [3:0] f6_reduce_operation;
   logic [3:0] f7_reduce_operation;
   logic [3:0] tensorreduce_operation;

   logic       id_reduce_auto_pair_next;
   logic       id_reduce_auto_pair;
   logic       ex_reduce_auto_pair;
   logic       f2_reduce_auto_pair;
   logic       f3_reduce_auto_pair;
   logic       f4_reduce_auto_pair;
   logic       f5_reduce_auto_pair;
   logic       f6_reduce_auto_pair;
   logic       f7_reduce_auto_pair;
   logic       tensorreduce_auto_pair;

   logic [3:0] id_reduce_level_next;
   logic [3:0] id_reduce_level;
   logic [3:0] ex_reduce_level;
   logic [3:0] f2_reduce_level;
   logic [3:0] f3_reduce_level;
   logic [3:0] f4_reduce_level;
   logic [3:0] f5_reduce_level;
   logic [3:0] f6_reduce_level;
   logic [3:0] f7_reduce_level;
   logic [3:0] tensorreduce_level;

   logic id_zero_reg_reduce_next;
   logic id_zero_reg_reduce;
   logic ex_zero_reg_reduce;
   logic f2_zero_reg_reduce;
   logic f3_zero_reg_reduce;
   logic f4_zero_reg_reduce;
   logic f5_zero_reg_reduce;
   logic f6_zero_reg_reduce;
   logic f7_zero_reg_reduce;
   logic zero_reg_reduce;

   `define FRCP_ID 1
   `define FRSQ_ID 2
   `define FLOG_ID 3
   `define FSIN_ID 4
   `define FEXP_ID 5
   `define FRCP_FIX_RAST_ID 6
   `define NO_TRANS_ID 0


   always_comb
   begin
      int_res_delayed = (`MINION_CORE.wb_ctrl.div)                                                                                                       // Divide instruction
                     || (`MINION_CORE.csr_file.fl_barrier.start_valid)                                                                                   // Fast Local Barrier
                     || (`MINION_CORE.wb_ctrl.wxd && `MINION_CORE.wb_ctrl.fp)                                                                            // FP
                     || (`MINION_CORE.wb_ctrl.wxd && `MINION_CORE.wb_ctrl.mem && !`MINION_CORE.wb_dcache_resp_valid && (`MINION_CORE.wb_waddr != 5'b0)); // Load miss

      fp_res_delayed = `MINION_CORE.wb_ctrl.wfd && ((`MINION_CORE.wb_ctrl.gsc == 1'b0) || ((|f4_regmask0) == 1'b1));  // Result only if mask is not all zeros for gathers

      store_data_select = disable_delayed_stores ? 1'b1 : `MINION_CORE.wb_ctrl.gsc || store_uc || store_misaligned;

      write_delayed =   int_res_delayed              // Delayed write for integer (kill prefetch)
                     || fp_res_delayed               // Delayed write for FP
                     || `MINION_CORE.wb_ctrl.wmd     // Delayed write for mask operations
                     || `MINION_CORE.wb_ctrl.wmad;   // Delayed write for mask operations
   end

   logic  [3:0][63:0]  bcols;

   logic wb_dcache_resp_replay;
   assign wb_dcache_resp_replay = `MINION_CORE.wb_dcache_resp_valid  &&  `MINION_DCACHE.s3_req.replay;

   logic [1:0] prv_mode;
   assign prv_mode = `MINION_CSR.prv[`MINION_CORE.wb_thread_id];

   always_ff @ (negedge clock)
   begin
      if (reset) begin
         tensorstore_2nd_pass = 1'b0;
         bcols <= '0;
      end
      else begin

         // Instruction retired
         if (`MINION_CORE.wb_valid) begin

            casex(wb_reg_inst)
               `FRCP_PS:       trans_id = `FRCP_ID;
               `FRSQ_PS:       trans_id = `FRSQ_ID;
               `FLOG_PS:       trans_id = `FLOG_ID;
               `FSIN_PS:       trans_id = `FSIN_ID;
               `FEXP_PS:       trans_id = `FEXP_ID;
               `FRCP_FIX_RAST: trans_id = `FRCP_FIX_RAST_ID;
               default:        trans_id = `NO_TRANS_ID; // no trans
            endcase

            tensor_quant_wflags  = 1'b0;
            tensor_quant_wflags |= `MINION_CORE.csr_file.io_rw_wb_wdata[4*0+:4] inside { 1, 2, 6, 7 };
            tensor_quant_wflags |= `MINION_CORE.csr_file.io_rw_wb_wdata[4*1+:4] inside { 1, 2, 6, 7 };
            tensor_quant_wflags |= `MINION_CORE.csr_file.io_rw_wb_wdata[4*2+:4] inside { 1, 2, 6, 7 };
            tensor_quant_wflags |= `MINION_CORE.csr_file.io_rw_wb_wdata[4*3+:4] inside { 1, 2, 6, 7 };
            tensor_quant_wflags |= `MINION_CORE.csr_file.io_rw_wb_wdata[4*4+:4] inside { 1, 2, 6, 7 };
            tensor_quant_wflags |= `MINION_CORE.csr_file.io_rw_wb_wdata[4*5+:4] inside { 1, 2, 6, 7 };
            tensor_quant_wflags |= `MINION_CORE.csr_file.io_rw_wb_wdata[4*6+:4] inside { 1, 2, 6, 7 };
            tensor_quant_wflags |= `MINION_CORE.csr_file.io_rw_wb_wdata[4*7+:4] inside { 1, 2, 6, 7 };
            tensor_quant_wflags |= `MINION_CORE.csr_file.io_rw_wb_wdata[4*8+:4] inside { 1, 2, 6, 7 };
            tensor_quant_wflags |= `MINION_CORE.csr_file.io_rw_wb_wdata[4*9+:4] inside { 1, 2, 6, 7 };

            tensor_reduce_wflags = `MINION_CORE.csr_file.vpu_ctrl.reduce_ctrl.reduce.op inside { 0, 2, 3 } &&  // Only trigger when the receiver does FP operations
                                   (`MINION_CORE.csr_file.vpu_ctrl.reduce_ctrl.reduce.action == 2'b01 ||
                                   (tensor_reduce_ap_is_receiver(`MINION_CORE.csr_file.vpu_ctrl.reduce_ctrl.reduce.action,global_min_id,`MINION_CORE.csr_file.io_rw_wb_wdata[6:3])));


            // FP instruction (trans too)
            instr_wflags = (`MINION_VPU_CTRL.f4_ctrl_valid_qual &&
                           (`MINION_VPU_CTRL.f4_ctrl_sigs.wflags || trans_id != 0) && `MINION_CORE.wb_ctrl.fp) ||
                           // TensorFMA for FP
                           (`MINION_CORE.csr_file.tensor_fma_wen_next && !`MINION_CORE.csr_file.io_rw_wb_wdata[2]) ||
                           // TensorQuat for FP
                           (`MINION_CORE.csr_file.reg_tensor_quant_en && tensor_quant_wflags) ||
                           // TensorReduce receiver
                           (`MINION_CORE.csr_file.vpu_ctrl.reduce_start && tensor_reduce_wflags);


            inst_retired(cycle,                                                         // Cycle Count
                         global_min_id,                                                // Minion Nr
                         `MINION_CORE.wb_thread_id,                                    // Thread ID
                         `SX(`XREG_SIZE, `MINION_CORE.wb_reg_pc),                      // PC
                         wb_reg_inst,                                                  // Instruction bits
                         write_delayed,                                                // Delayed Write
                         (`MINION_CORE.wb_ctrl.wxd && `MINION_CORE.wb_ctrl.mem && !`MINION_CORE.wb_dcache_resp_valid && (`MINION_CORE.wb_waddr != 5'b0)), // Load Miss
                         `MINION_CORE.wb_ctrl.wxd,                                     // Integer write
                         `MINION_CORE.wb_waddr,                                        // Integer address
                         `MINION_CORE.wb_rf_wdata,                                     // Integer data
                         `MINION_CORE.wb_ctrl.wfd,                                     // FP write
                         `MINION_CORE.wb_waddr,                                        // FP address
                         `MINION_CORE.wb_ctrl.wmd,                                     // RM write
                         `MINION_CORE.wb_ctrl.wmad,                                    // RM write all
                         `MINION_CORE.wb_waddr,                                        // RM destination
                         {`VPU_REGMASK_ALL_DATA_SZ{1'b0}},                             // RM data is always delayed
                         is_store,                                                     // Store
                         store_size,                                                   // Store Size
                         store_addr,                                                   // Store Addr
                         store_data_select ? scatter_store_data : store_data,          // Store Data (gathers and UC stores get a flopped version of S2 instead of what is in S3 dcache to ease checker)
                         store_ps_mask,                                                // Store Mask
                         store_delayed,                                                // Delayed Store
                         store_age_id,                                                 // Store age id
                         `MINION_CORE.wb_ctrl.gsc,                                     // Gather/Scatter
                         prev_scatter_addr,                                            // first VPU_LANE_NUM-1 scatter addresses
                         prev_scatter_data,                                            // first VPU_LANE_NUM-1 scatter data
                         is_gsc32,                                                     // It's a gather/scatter on 32-byte blocks
                         gsc32_indices,                                                // Indices for gather/scatter on 32-byte blocks operations
                         1'b0,                                                         // Texture sampler request (not used)
                         1'b0,                                                         // Texture sampler receive (not used)
                         `MINION_CORE.csr_file.dcache_ctrl.tensorload_start,           // TensorLoad
                         `MINION_CORE.csr_file.dcache_ctrl.tensorload_ctrl.tl_sel,     // TensorLoadSetupB
                         `MINION_CORE.csr_file.dcache_ctrl.tensorload_ctrl.num_lines,  // TensorLoad Num Lines
                         `MINION_CORE.csr_file.tensor_fma_wen_next,                    // TensorFMA
                         `MINION_CORE.csr_file.reg_tensor_quant_en,                    // TensorQuant
                         `MINION_CORE.csr_file.vpu_ctrl.reduce_start,                  // Reduce
                         `MINION_CORE.csr_file.dcache_ctrl.tensorstore_start,          // Tensor Store
                         `MINION_CORE.csr_file.fl_barrier.start_valid,                 // Fast Local Barrier valid
                         `MINION_CORE.csr_file.fl_barrier.start_waddr,                 // Fast Local Barrier Dest Register
                         `MINION_CORE.csr_file.fl_barrier.start_data,                  // Fast Local Barrier Match, Barrier IDX
                         instr_wflags,                                                 // instr wait fflags
                         trans_id,                                                     // Trans operation id
                         prv_mode                                                      // Privilege mode
            );

         end // if (`MINION_CORE.wb_valid)

         // Instruction took exception
         if (`MINION_CORE.wb_xcpt && wb_xcpt_interrupt == 1'b0) begin
            inst_exception( cycle,
                            global_min_id,                                       // Minion Nr
                            `MINION_CORE.wb_thread_id,                           // Thread ID
                            `SX(`XREG_SIZE, `MINION_CORE.wb_reg_pc),             // PC
                            wb_reg_inst_xcpt,                                    // Instruction bits
                            `MINION_CORE.wb_waddr,                               // FP address
                            is_store,
                            is_atomic,
                            `MINION_CORE.wb_ctrl.gsc,
                            store_addr,
                            scatter_store_data,
                            prev_scatter_addr,
                            prev_scatter_data,
                            store_ps_mask,
                            //{1'b0,`MINION_DCACHE.s2_req.typ[2:0]},
                            store_size,
                            `MINION_CSR.reg_gsc_progress_next[`MINION_CORE.wb_thread_id], // CSR progress
                            `MINION_CSR.reg_mstatus_next[`MINION_CORE.wb_thread_id],
                            64'($signed(`MINION_CSR.delegate ? `MINION_CSR.reg_scause_next[`MINION_CORE.wb_thread_id] : `MINION_CSR.reg_mcause_next[`MINION_CORE.wb_thread_id])),
                            64'($signed(`MINION_CSR.delegate ? `MINION_CSR.reg_stval_next [`MINION_CORE.wb_thread_id] : `MINION_CSR.reg_mtval_next [`MINION_CORE.wb_thread_id])),
                            64'($signed(`MINION_CSR.delegate ? `MINION_CSR.reg_sepc_next  [`MINION_CORE.wb_thread_id] : `MINION_CSR.reg_mepc_next  [`MINION_CORE.wb_thread_id])),
                            `MINION_CSR.reg_mip_next[`MINION_CORE.wb_thread_id],
                            prv_mode
                          );
         end

         // CPU took Interrupt
         if (`MINION_CORE.wb_xcpt && wb_xcpt_interrupt == 1'b1) begin
            `event_common_assign(inst_interrupt)
            inst_interrupt.thread_id      = `MINION_CORE.wb_thread_id;
            inst_interrupt.pc             = `SX(`XREG_SIZE, `MINION_CORE.wb_reg_pc);
            inst_interrupt.inst_bits      = wb_reg_inst;
            inst_interrupt.is_gsc         = `MINION_CORE.wb_ctrl.gsc;
            inst_interrupt.gsc_progress   = `MINION_CSR.reg_gsc_progress_next[`MINION_CORE.wb_thread_id];
            inst_interrupt.mstatus        = `MINION_CSR.reg_mstatus_next[`MINION_CORE.wb_thread_id];
            inst_interrupt.cause          = 64'($signed(`MINION_CSR.delegate ?
                                                        `MINION_CSR.reg_scause_next[`MINION_CORE.wb_thread_id] :
                                                        `MINION_CSR.reg_mcause_next[`MINION_CORE.wb_thread_id]));
            inst_interrupt.tval           = 64'($signed(`MINION_CSR.delegate ?
                                                        `MINION_CSR.reg_stval_next [`MINION_CORE.wb_thread_id] :
                                                        `MINION_CSR.reg_mtval_next [`MINION_CORE.wb_thread_id]));
            inst_interrupt.epc            = 64'($signed(`MINION_CSR.delegate ?
                                                        `MINION_CSR.reg_sepc_next  [`MINION_CORE.wb_thread_id] :
                                                        `MINION_CSR.reg_mepc_next  [`MINION_CORE.wb_thread_id]));
            inst_interrupt.mip            = `MINION_CSR.reg_mip_next[`MINION_CORE.wb_thread_id];
            inst_interrupt.bus_error_addr = `MINION_CSR.reg_mbusaddr_next[`MINION_CORE.wb_thread_id];
            inst_interrupt.bus_error_pc   = `MINION_CSR.dcache_bus_error_pc[`MINION_CORE.wb_thread_id];
            inst_interrupt.prv            = prv_mode;
            `event_trigger(inst_interrupt)
         end

         delayed_write_valid = (`MINION_CORE.wb_dcache_wen && wb_dcache_resp_replay) || (`MINION_CORE.wb_div_wen) || (`MINION_CORE.wb_fp_toint_wen);

         // Delayed write int (DCache misses and Div ops)
         if ( delayed_write_valid )
         begin
             `event_common_assign(delayed_write_int)
             delayed_write_int.thread_id = `MINION_CORE.wb_rf_thread_id;
             delayed_write_int.wbx_rd    = `MINION_CORE.wb_rf_waddr;
             delayed_write_int.wbx_data  = `MINION_CORE.wb_rf_wdata;
             `event_trigger(delayed_write_int)
         end

         flb_update_valid = `MINION_CORE.wb_flb_wen_valid && `MINION_CORE.wb_flb_wen_ready;

         // FLB update
         if ( flb_update_valid )
         begin
            `event_common_assign(flb_update)
            flb_update.thread_id = `MINION_CORE.wb_rf_thread_id;
            flb_update.wbx_rd    = `MINION_CORE.wb_rf_waddr;
            flb_update.wbx_data  = `MINION_CORE.wb_rf_wdata;
            `event_trigger(flb_update)
         end

         // Load Miss with BUS Error and Interrupt not enabled
         for( integer thread = 0; thread < `CORE_NR_THREADS; thread++ ) begin
            if (`MINION_CSR.dcache_bus_error[thread]===1'b1 &&
               (`MINION_CSR.reg_mstatus[thread].mie===1'b0 || `MINION_CSR.reg_mie[thread].bus_err===1'b0)) begin
               `event_common_assign(load_miss_bus_err)
               load_miss_bus_err.thread_id = thread;
               load_miss_bus_err.pc = `MINION_CSR.dcache_bus_error_pc[thread]; // PC required for matching transactions
               load_miss_bus_err.bus_error_addr = `MINION_CSR.dcache_bus_error_addr[thread];
               `event_trigger(load_miss_bus_err)
            end
         end

         delayed_store_valid = `MINION_DCACHE.s3_valid_to_s4 && `MINION_DCACHE.s3_req_to_s4.replay && !dcache_type_is_gsc(`MINION_DCACHE.s3_req_to_s4.typ) && !(`MINION_DCACHE.s3_req_to_s4.misaligned_2nd || store_misaligned) && !disable_delayed_stores ;

         // Delayed stores (not scatters to ease this)
         if ( delayed_store_valid )
         begin
            `event_common_assign(delayed_store)
            delayed_store.thread_id    = `MINION_DCACHE.s3_req_to_s4.dest.thread_id;
            delayed_store.scatter      = dcache_type_is_gsc(`MINION_DCACHE.s3_req_to_s4.typ);
            delayed_store.scatter_cnt  = `MINION_DCACHE.s3_req_to_s4.gsc_cnt;
            delayed_store.store_size   = store_size;
            `event_unpack(delayed_store.store_data, store_data, `VPU_LANE_NUM / 2, `VPU_DATA_S_SZ * 2)
            delayed_store.store_age_id = `MINION_DCACHE.s3_req_to_s4.replay_age;
            `event_trigger(delayed_store)
         end


         case(`MINION_VPU_CTRL.f8_ctrl_sigs.cmd)
            `VPU_TRANS_RCP_FMA2:   fflags_update_trans_id = `FRCP_ID;
            `VPU_TRANS_RSQRT_FMA2: fflags_update_trans_id = `FRSQ_ID;
            `VPU_TRANS_LOG_MUL:    fflags_update_trans_id = `FLOG_ID;
            `VPU_TRANS_SIN_P3:     fflags_update_trans_id = `FSIN_ID;
            `VPU_TRANS_EXP_FMA2:   fflags_update_trans_id = `FEXP_ID;
            `VPU_FCMD_NR2_FRCPFXP: fflags_update_trans_id = `FRCP_FIX_RAST_ID;
            default:               fflags_update_trans_id = `NO_TRANS_ID; // no relevant trans
         endcase


         // Delayed detection when all groups are masked off
         if (f9_vpu_lanes_off) begin
            `event_common_assign(delayed_write_fp)
            delayed_write_fp.thread_id      = f9_thread_id;
            delayed_write_fp.wbf_rd         = f9_waddr;
            delayed_write_fp.entry_point_id = DFP_VPU_MASKED_ID;
            `event_trigger(delayed_write_fp)
         end

         // Delayed write FP from functional units
         if (func_wb_reg) begin
            `event_common_assign(delayed_write_fp)
            delayed_write_fp.thread_id      = func_wb_thread_id_reg;
            delayed_write_fp.wbf_rd         = func_wb_waddr_reg;
            delayed_write_fp.entry_point_id = DFP_WB_REG_ID;
            `event_trigger(delayed_write_fp)
         end

         // Delayed write FP from functional units (multipass)
         if (func_wb_multipass_reg) begin
            `event_common_assign(delayed_write_fp)
            delayed_write_fp.thread_id      = func_wb_thread_id_reg;
            delayed_write_fp.wbf_rd         = func_wb_waddr_reg;
            delayed_write_fp.entry_point_id = DFP_WB_REG_MULTIPASS_ID;
            `event_trigger(delayed_write_fp)
         end

         // Delayed write FP from load instruction
         if (load_wb_reg || gather_end_reg) begin
            `event_common_assign(delayed_write_fp)
            delayed_write_fp.thread_id      = dmem_thread_id_reg;
            delayed_write_fp.wbf_rd         = dmem_load_addr_reg;
            delayed_write_fp.entry_point_id = DFP_LOAD_ID;
            `event_trigger(delayed_write_fp)
         end

         // Delayed write FP from gather instruction that finishes with masked lane
         // in WB stage
         if (gather_end_masked_reg) begin
            `event_common_assign(delayed_write_fp)
            delayed_write_fp.thread_id      = f5_thread_id;
            delayed_write_fp.wbf_rd         = f5_gather_waddr;
            delayed_write_fp.entry_point_id = DFP_GSC_ID;
            `event_trigger(delayed_write_fp)
         end

         // Delayed write mask operation
         if (f8_regmask_write != 0 || f8_regmask_write_all) begin
            `event_common_assign(delayed_write_rm)
            delayed_write_rm.thread_id = f8_regmask_thread_id;
            delayed_write_rm.wbmall    = f8_regmask_write_all;
            delayed_write_rm.wbm_rd    = f8_regmask_waddr_mux;
            delayed_write_rm.wbm_data  = f8_regmask_wdata;
            `event_trigger(delayed_write_rm)
         end

         // Received an IPI with PC
         for (int t = 0; t < `CORE_NR_THREADS; t++) begin
            if (`MINION_CORE.ipi_with_pc[t] && `MINION_CORE.id_redirect_allowed[t]) begin
               `event_common_assign(ipi_received)
               ipi_received.thread_id = t;
               ipi_received.pc        = 64'($signed ({`MINION_CORE.interrupts.ipi_pc, 1'b0}));
               `event_trigger(ipi_received)
            end
         end

         // TensorFMA write
         for (int i = 1; i< 4; i++) begin
            bcols[i] <= bcols[i-1];
         end
         if ( `MINION_CSR.vpu_ctrl.tensorfma_start) begin
            bcols[0] <= (`MINION_CSR.vpu_ctrl.tensorfma_ctrl.cols_b + 1 )*4;
         end

         if (tensorfma_val_reg) begin
            `event_common_assign(tensorfma_write)
            tensorfma_write.entry = func_wb_waddr_reg;
            tensorfma_write.wmask = tensorfma_regfile_wmask_reg;
            `event_trigger(tensorfma_write)
         end

         if (tensorfma_val_int8) begin
            if (bcols[3] == 4 | bcols[3] == 8) begin
               `event_common_assign(tensorfma_write8)
               tensorfma_write8.entry = { `MINION_VPU_CTRL.f3_tima_tenc_waddr, 1'b0 };
               `event_unpack(tensorfma_write8.data, tima_data_packed)
               tensorfma_write8.wmask = f3_tima0_valid;
               `event_trigger(tensorfma_write8)
            end
            else begin
               if(|f3_tima0_valid) begin
                  `event_common_assign(tensorfma_write8)
                  tensorfma_write8.entry = { `MINION_VPU_CTRL.f3_tima_tenc_waddr, 1'b0 };
                  `event_unpack(tensorfma_write8.data, tima_data_packed)
                  tensorfma_write8.wmask = f3_tima0_valid;
                  `event_trigger(tensorfma_write8)
               end
               if(|f3_tima1_valid) begin
                  `event_common_assign(tensorfma_write8)
                  tensorfma_write8.entry = { `MINION_VPU_CTRL.f3_tima_tenc_waddr, 1'b1 };
                  `event_unpack(tensorfma_write8.data, tima_data_packed_ext)
                  tensorfma_write8.wmask = f3_tima1_valid;
                  `event_trigger(tensorfma_write8)
               end
            end
         end

         // TensorQuant write
         if (tensorquant_val_reg) begin
            `event_common_assign(tensorquant_write)
            tensorquant_write.entry = func_wb_waddr_reg;
            tensorquant_write.wmask = {`VPU_LANE_NUM{1'b1}}; // FIXME Why is this hard-coded?
            `event_trigger(tensorquant_write)
            assert(reduce_bcst_val_reg == 1'b0) else $error("check addr sent to tensorquant_write");
         end


         if (tensorquant_val_multipass_reg) begin
            `event_common_assign(tensorquant_write)
            tensorquant_write.entry = func_wb_waddr_reg;
            tensorquant_write.wmask = {`VPU_LANE_NUM{1'b1}};
            `event_trigger(tensorquant_write)
            assert(reduce_bcst_val_reg == 1'b0) else $error("check addr sent to tensorquant_write");
         end

         // Reduce write
         if (reduce_bcst_val_reg_z1) begin
            `event_common_assign(reduce_write)
            reduce_write.entry = func_wb_waddr_reg_z1;
            `event_trigger(reduce_write)
         end else if(reduce_val_reg) begin
            `event_common_assign(reduce_write)
            reduce_write.entry = func_wb_waddr_reg;
            `event_trigger(reduce_write)
         end
         // Tensor Done should be done before flags. This is because the tensor done comes a cycle after the last value is done
         // Reduce
         if (tensorreduce_is_done) begin
            `event_common_assign(reduce_done)
            reduce_done.action            = tensorreduce_action;
            reduce_done.partner_minion_id = tensorreduce_partner_id;
            reduce_done.operation         = tensorreduce_operation;
            reduce_done.zero_reg_reduce   = zero_reg_reduce;
            reduce_done.level             = tensorreduce_level;
            reduce_done.auto_pair         = tensorreduce_auto_pair;
            `event_trigger(reduce_done)
         end
         // FMA
         if (tensorfma_is_done) begin
            `event_common_assign(tensorfma_done)
            `event_trigger(tensorfma_done)
         end
         // Quant
         if (tensorquant_is_done) begin
            `event_common_assign(tensorquant_done)
            `event_trigger(tensorquant_done)
         end
         // Quant w/ SCP Error
         if (tensorquant_is_done_scp_err) begin
            `event_common_assign(tensorquant_done)
            `event_trigger(tensorquant_done)
         end

         // Tensor store: Working signal marks when the store is done
         // Tensor store writes are monitored in the neigh interface for tensor stores that do not go to the local L2 scratchpad
         // and at the DC interface for tensor stores that go to the local L2 scratchpad.
         // The reason is that at the neigh interface the WriteAround has turned into a Write for the local L2 Scratchpad to avoid the coalescing buffer in L2.
         // Also at the Neigh interface the data bus is 512-bits wide and at the DC interface it is 256-bits wide.
         if (tensorstore_working != tensorstore_working_reg && tensorstore_working == 1'b0) begin
            `event_common_assign(tensorstore_done)
            tensorstore_done.coop = `MINION_DCACHE.reduce.req_info.coop;
            `event_trigger(tensorstore_done)
         end

         if (`MINION_DCACHE.l2_evict_req.opcode == 5'd2 && `MINION_DCACHE.l2_evict_req_valid != 2'b0) begin
            tensorstore_size = `MINION_DCACHE.l2_evict_req.size;
            tensorstore_qwen = `MINION_DCACHE.l2_evict_req.qwen;
            if (tensorstore_2nd_pass) begin
               tensorstore_address = `MINION_DCACHE.l2_evict_req.address + 16'h0020;
            end else begin
               tensorstore_address = `MINION_DCACHE.l2_evict_req.address;
            end
            `event_common_assign(tensorstore_write)
            tensorstore_write.is_256_bit = 1'b1;
            tensorstore_write.addr    = tensorstore_address;
            `event_unpack(tensorstore_write.data, `MINION_DCACHE.l2_evict_req.data) // Upper half always 0
            tensorstore_write.size    = tensorstore_size;
            tensorstore_write.qwen    = tensorstore_qwen;
            tensorstore_write.subcode = `MINION_DCACHE.l2_evict_req.subopcode;
            tensorstore_write.coop    = `MINION_DCACHE.reduce.req_info.coop;
            `event_trigger(tensorstore_write)
            if (tensorstore_size == ET_LINK_Line) begin
              tensorstore_2nd_pass = ~tensorstore_2nd_pass;
            end
         end

         // Original code -- it makes all checks excpet for local L2 SCP in the neigh boundary
         // The standalone minion build fails here -- but I am keeping this around until I find the best way to fix this problem
         //if (neigh_inst.neigh_sc_req_info.opcode == 5'd2 && (global_min_id[2:0] == neigh_inst.neigh_sc_req_info.source[2:0])) begin
         //   tensorstore_wr = 1'b1;
         //   tensorstore_size = neigh_inst.neigh_sc_req_info.size;
         //   tensorstore_qwen = neigh_inst.neigh_sc_req_info.qwen;
         //   tensorstore_address = neigh_inst.neigh_sc_req_info.address;
         //   tensorstore_write(cycle, global_min_id, 1'b0, tensorstore_address, neigh_inst.neigh_sc_req_info.data,
         //                      tensorstore_size, tensorstore_qwen, neigh_inst.neigh_sc_req_info.subopcode, `MINION_DCACHE.reduce.req_info.coop);
         //end else if (`MINION_DCACHE.l2_evict_req.opcode == 5'd2 && `MINION_DCACHE.l2_evict_req.address[39:31] == 9'b000000001 && `MINION_DCACHE.l2_evict_req_valid != 2'b0
         //                      && (`MINION_DCACHE.l2_evict_req.address[29:23] == shire_id[6:0] || {`MINION_DCACHE.l2_evict_req.address[30:28],`MINION_DCACHE.l2_evict_req.address[10:6]} == {1'b1,shire_id[6:0]})) begin
         //            tensorstore_wr = 1'b1;
         //   tensorstore_size = `MINION_DCACHE.l2_evict_req.size;
         //   tensorstore_qwen = `MINION_DCACHE.l2_evict_req.qwen;
         //   if (tensorstore_wr & tensorstore_wr_reg) begin
         //      tensorstore_address = `MINION_DCACHE.l2_evict_req.address + 16'h0020;
         //     end else begin
         //      tensorstore_address = `MINION_DCACHE.l2_evict_req.address;
         //   end
         //   tensorstore_write(cycle, global_min_id, 1'b1, tensorstore_address, {256'b0,`MINION_DCACHE.l2_evict_req.data},
         //                      tensorstore_size, tensorstore_qwen, `MINION_DCACHE.l2_evict_req.subopcode, `MINION_DCACHE.reduce.req_info.coop);
         //end
         //else begin
         //   tensorstore_wr = 1'b0;
         //end

         // update fcsr flags
         // Called when there are new flags available
         if (`MINION_CSR.io_fcsr_flags_valid) begin
            `event_common_assign(update_fcsr_flags);
            update_fcsr_flags.thread_id = `MINION_CSR.io_fcsr_thread_id;
            update_fcsr_flags.flags     = `MINION_CSR.io_fcsr_flags_bits;
            update_fcsr_flags.trans_id  = fflags_update_trans_id;
            `event_trigger(update_fcsr_flags)
         end
      end
   end

`ASSERT_VALID_PORTS(ERROR_arch_state_mon_delayed_write_xs,  clock, reset_next,  delayed_write_valid && |enabled, {`MINION_CORE.wb_rf_thread_id, `MINION_CORE.wb_rf_waddr, `MINION_CORE.wb_rf_wdata})
`ASSERT_VALID_PORTS(ERROR_arch_state_mon_flb_update_xs,  clock, reset_next, flb_update_valid && |enabled, {`MINION_CORE.wb_rf_thread_id, `MINION_CORE.wb_rf_waddr, `MINION_CORE.wb_rf_wdata})
`ASSERT_VALID_PORTS(ERROR_arch_state_mon_delayed_store_xs,  clock, reset_next, delayed_store_valid && |enabled, {`MINION_DCACHE.s3_req_to_s4.dest.thread_id, dcache_type_is_gsc(`MINION_DCACHE.s3_req_to_s4.typ) , store_size, store_data, `MINION_DCACHE.s3_req_to_s4.replay_age})
`ASSERT_VALID_PORTS(ERROR_arch_state_mon_fma16_32_write_xs,  clock, reset_next, tensorfma_val_reg && |enabled, {func_wb_waddr_reg, tensorfma_regfile_wmask_reg });
`ASSERT_VALID_PORTS(ERROR_arch_state_mon_fma8_write_xs,  clock, reset_next, tensorfma_val_int8 && |enabled, {`MINION_VPU_CTRL.f3_tima_tenc_waddr, tima_data_packed })
`ASSERT_VALID_PORTS(ERROR_arch_state_mon_ima0_write_xs,  clock, reset_next, |f3_tima0_valid && |enabled, {`MINION_VPU_CTRL.f3_tima_tenc_waddr, tima_data_packed })
`ASSERT_VALID_PORTS(ERROR_arch_state_mon_ima1_write_xs,  clock, reset_next, |f3_tima1_valid && |enabled, {`MINION_VPU_CTRL.f3_tima_tenc_waddr, tima_data_packed_ext })
`ASSERT_VALID_PORTS(ERROR_arch_state_mon_reduce_done_xs,  clock, reset_next, tensorreduce_is_done && |enabled, {tensorreduce_action, tensorreduce_partner_id, zero_reg_reduce})
`ASSERT_VALID_PORTS(ERROR_arch_state_mon_fcsr_flags_update_xs,  clock, reset_next, `MINION_CSR.io_fcsr_flags_valid && |enabled, {`MINION_CSR.io_fcsr_thread_id, `MINION_CSR.io_fcsr_flags_bits, fflags_update_trans_id})
`ASSERT_VALID_PORTS(ERROR_arch_state_mon_rf_write_xs, clock, reset_next, (`MINION_CORE.wb_rf_wen && `MINION_CORE.wb_rf_waddr != 0),   `MINION_CORE.wb_rf_wdata)



   ////////////////////////////////////////////////////////////////////////////////
   // Tensor Load
   ////////////////////////////////////////////////////////////////////////////////
   logic       tl_busy[2],tl_busy_prev[2];
   logic [1:0] tl_busy_rising, tl_busy_falling;
   logic       tl_req_val[2];
   int         tl_req_id[2], tl_req_id_idx[2];
   logic       tl_resp_val;
   int         tl_resp_id,tl_resp_id_idx,tl_resp_module,tl_resp_entry,tl_trans_banks,tl_trans_entry;
   bit [255:0] tl_resp_data;

   `RST_FF(clock, reset, tl_busy_prev[0], tl_busy[0], 1'b0)
   `RST_FF(clock, reset, tl_busy_prev[1], tl_busy[1], 1'b0)

   genvar tli;
   generate
      for (tli=0; tli<2; tli++) begin

        assign tl_busy[tli]         = (tli==0) ? `MINION_DCACHE.tensor_load_0.busy : `MINION_DCACHE.tensor_load_1.busy;
        assign tl_busy_rising[tli]  = ~tl_busy_prev[tli] &  tl_busy[tli];
        assign tl_busy_falling[tli] =  tl_busy_prev[tli] & ~tl_busy[tli];

        assign tl_req_val[tli]   = (tli==0) ? (`MINION_DCACHE.tensor_load_0.l2_req_valid_coop    & `MINION_DCACHE.tensor_load_0.l2_req_ready_coop)    |
                                              (`MINION_DCACHE.tensor_load_0.l2_req_valid_no_coop & `MINION_DCACHE.tensor_load_0.l2_req_ready_no_coop) :
                                              (`MINION_DCACHE.tensor_load_1.l2_req_valid_coop    & `MINION_DCACHE.tensor_load_1.l2_req_ready_coop)    |
                                              (`MINION_DCACHE.tensor_load_1.l2_req_valid_no_coop & `MINION_DCACHE.tensor_load_1.l2_req_ready_no_coop) ;

        assign tl_req_id[tli] = (tli==0) ? `MINION_DCACHE.tensor_load_0.l2_req.id     : `MINION_DCACHE.tensor_load_1.l2_req.id;

        assign tl_req_id_idx[tli] = (tl_req_val[tli]!==1'b1 || tl_req_id[tli]<32'h10 || tl_req_id[tli]>32'h17) ? -1 : (tl_req_id[tli]-32'h10-(4*tli));

      end
   endgenerate


   assign tl_resp_val    = `MINION_DCACHE.l2_resp_valid_ready_int;
   assign tl_resp_data   = `MINION_DCACHE.l2_resp_int.data;
   assign tl_resp_id     = `MINION_DCACHE.l2_resp_int.id;
   assign tl_resp_module = (tl_resp_val!==1'b1 || tl_resp_id<32'h10 || tl_resp_id>32'h17) ? -1 : ((tl_resp_id-32'h10)/4);
   assign tl_resp_id_idx = (tl_resp_id - 32'h10 - (4*tl_resp_module));


   always_ff @(posedge clock) begin

      if (tl_resp_module==0)
         tl_resp_entry = {`MINION_DCACHE.tensor_load_0.tl_load_way[tl_resp_id_idx],`MINION_DCACHE.tensor_load_0.tl_load_set[tl_resp_id_idx]};
      else
         tl_resp_entry = 48 + `MINION_DCACHE.s3_vpu_scp_resp.tenb_line[3:0];

      // tl0 transform
      tl_trans_banks = `MINION_DCACHE.tensor_load_0.load_valid;
      tl_trans_entry = {`DCACHE_TL_SCP_MOD*`MINION_DCACHE.tensor_load_0.load_way+
                        `MINION_DCACHE.tensor_load_0.load_addr[`DCACHE_LRAM_ADDR_SIZE-1:`CORE_L2_BLOCK_EXT_ADDR_OFFSET_BITS+1],
                        `MINION_DCACHE.tensor_load_0.load_addr[`CORE_L2_BLOCK_EXT_ADDR_OFFSET_BITS]};

      if ( (`MINION_DCACHE.tensor_ctrl.start         === 1'b1) ||
           (|tl_busy_rising                          === 1'b1) ||
           (|tl_busy_falling                         === 1'b1) ||
           ( tl_req_id_idx[0]                        !=  -1  ) ||
           ( tl_req_id_idx[1]                        !=  -1  ) ||
           ( tl_resp_module                          !=  -1  ) ||
           (|`MINION_DCACHE.tensor_load_0.load_valid === 1'b1) ||
           (`MINION_DCACHE.tensor_load_1.tenb_flush  === 1'b1)  ) begin
         tensorload_mon(
                        cycle,
                        global_min_id,

                        // process new op start
                        `MINION_DCACHE.tensor_ctrl.start,
                        `MINION_DCACHE.tensor_ctrl.ctrl.tl_sel,
                        `MINION_DCACHE.tensor_ctrl.ctrl.transform,
                        `MINION_DCACHE.tensor_ctrl.ctrl.num_lines,

                        // process busy
                        tl_busy_rising,
                        tl_busy_falling,

                        // process l2 requests
                        {tl_req_id_idx[1], tl_req_id_idx[0]},

                        // process l2 resp / data xfers
                        tl_resp_module,
                        tl_resp_id_idx,
                        tl_resp_entry,
                        `MINION_DCACHE.l2_resp_int.data,

                        // process transform data xfers
                        (|`MINION_DCACHE.tensor_load_0.load_valid),
                        tl_trans_entry,
                        tl_trans_banks,
                        `MINION_DCACHE.tensor_load_0.load_data,

                        // tenb flush
                        `MINION_DCACHE.tensor_load_1.tenb_flush
                       );
      end

    end // always @(posedge clock)



    ////////////////////////////////////////////////////////////////////////////
    // TensorFMA
    ////////////////////////////////////////////////////////////////////////////

    logic ex_tensorfma_val;
    logic f2_tensorfma_val;
    logic f4_tensorfma_val;
    logic f5_tensorfma_val;
    logic f6_tensorfma_val;
    logic f7_tensorfma_val;

    //      CLK    RST    DOUT                DIN                                                           DEF
    `RST_FF(clock, reset, ex_tensorfma_val,   `MINION_VPU_CTRL.id_ml_inst_fma_en,                           1'b0)
    `RST_FF(clock, reset, f2_tensorfma_val,   ex_tensorfma_val,                                             1'b0)
    `RST_FF(clock, reset, f3_tensorfma_val,   f2_tensorfma_val,                                             1'b0)
    `RST_FF(clock, reset, f4_tensorfma_val,   f3_tensorfma_val,                                             1'b0)
    `RST_FF(clock, reset, f5_tensorfma_val,   f4_tensorfma_val,                                             1'b0)
    `RST_FF(clock, reset, f6_tensorfma_val,   f5_tensorfma_val,                                             1'b0)
    `RST_FF(clock, reset, f7_tensorfma_val,   f6_tensorfma_val,                                             1'b0)
    `RST_FF(clock, reset, f8mp_tensorfma_val, f7_tensorfma_val && (|f7_multipass),                          1'b0)
    `RST_FF(clock, reset, f8_tensorfma_val,   f8mp_tensorfma_val || (f7_tensorfma_val && !(|f7_multipass)), 1'b0)

    // Keeps track of the last tensorFMA sent

    logic id_tensorfma_val_last;
    logic ex_tensorfma_val_last;
    logic f2_tensorfma_val_last;
    logic f3_tensorfma_val_last;
    logic f4_tensorfma_val_last;
    logic f5_tensorfma_val_last;
    logic f6_tensorfma_val_last;
    logic f7_tensorfma_val_last;
    logic f8_tensorfma_val_last;

    //      CLK    RST    DOUT              DIN                         DEF
    `RST_FF(clock, reset, ex_tensorfma_val_last, id_tensorfma_val_last, 1'b0)
    `RST_FF(clock, reset, f2_tensorfma_val_last, ex_tensorfma_val_last, 1'b0)
    `RST_FF(clock, reset, f3_tensorfma_val_last, f2_tensorfma_val_last, 1'b0)
    `RST_FF(clock, reset, f4_tensorfma_val_last, f3_tensorfma_val_last, 1'b0)
    `RST_FF(clock, reset, f5_tensorfma_val_last, f4_tensorfma_val_last, 1'b0)
    `RST_FF(clock, reset, f6_tensorfma_val_last, f5_tensorfma_val_last, 1'b0)
    `RST_FF(clock, reset, f7_tensorfma_val_last, f6_tensorfma_val_last, 1'b0)

    always_comb
    begin
       id_tensorfma_val_last = `MINION_VPU_CTRL.ml.tensorfma.enabled_int && ~`MINION_VPU_CTRL.ml.tensorfma.enabled_next;
    end

    assign tensorfma_is_done = f7_tensorfma_val_last;


    ////////////////////////////////////////////////////////////////////////////
    // TensorQuant
    ////////////////////////////////////////////////////////////////////////////

    logic id_tensorquant_val;
    logic id_tensorquant_val_to_ex;
    logic ex_tensorquant_val;
    logic f2_tensorquant_val;
    logic f3_tensorquant_val;
    logic f4_tensorquant_val;
    logic f5_tensorquant_val;
    logic f6_tensorquant_val;
    logic f7_tensorquant_val;


    assign f8mp_multipass = '0;
    assign f7_multipass   = '0;
    assign ex_multipass   = '0;
    assign f8_force_multipass_wdata = '0;
    assign f8mp_force_wdata = '0;
    assign f8mp_ctrl_valid_qual = '0;
    assign nsleepin_txfma_27 = '0;

    assign id_tensorquant_val_to_ex = (ex_tensorquant_val && (ex_multipass[3] | ex_multipass[2] | ex_multipass[0])) ? 1'b1 : id_tensorquant_val;

    //      CLK    RST    DOUT                  DIN                                                                                DEF
    `RST_FF(clock, reset, id_tensorquant_val,   `MINION_VPU_CTRL.ml.tensorquant.quant_en_next,                                     1'b0)
    `RST_FF(clock, reset, ex_tensorquant_val,   id_tensorquant_val_to_ex,                                                          1'b0)
    `RST_FF(clock, reset, f2_tensorquant_val,   ex_tensorquant_val,                                                                1'b0)
    `RST_FF(clock, reset, f3_tensorquant_val,   f2_tensorquant_val,                                                                1'b0)
    `RST_FF(clock, reset, f4_tensorquant_val,   f3_tensorquant_val,                                                                1'b0)
    `RST_FF(clock, reset, f5_tensorquant_val,   f4_tensorquant_val,                                                                1'b0)
    `RST_FF(clock, reset, f6_tensorquant_val,   f5_tensorquant_val,                                                                1'b0)
    `RST_FF(clock, reset, f7_tensorquant_val,   f6_tensorquant_val,                                                                1'b0)
    `RST_FF(clock, reset, f8mp_tensorquant_val, f7_tensorquant_val && (|f7_multipass),                            1'b0)
    `RST_FF(clock, reset, f8_tensorquant_val,   f8mp_tensorquant_val || (f7_tensorquant_val && !(|f7_multipass)), 1'b0)

    // Keeps track of the last tensorQuant sent

    logic        id_tensorquant_val_last;
    logic        id_tensorquant_nop; // first transformation is also last transformation
    logic [8:0]  tensorquant_val_last;

    // Wait for 8 cycles after TensorQuant completes to allow pipe to drain (i.e. guanratee to have RF updated)
    //      CLK    RST    DOUT              DIN                         DEF
    `RST_FF(clock, reset, tensorquant_val_last,{tensorquant_val_last[7:0],id_tensorquant_val_last|id_tensorquant_nop}, 4'b0)

    always_comb
    begin
       id_tensorquant_val_last = `MINION_VPU_CTRL.ml.tensorquant.enabled && ~`MINION_VPU_CTRL.ml.tensorquant.enabled_next;
       id_tensorquant_nop = `MINION_VPU_CTRL.ml.tensorquant.tensorquant_ctrl_en & ~`MINION_VPU_CTRL.ml.tensorquant.new_req & ~`MINION_VPU_CTRL.ml.tensorquant.exec_last_op_trans;
    end

    assign tensorquant_is_done = tensorquant_val_last[8];
    assign tensorquant_is_done_scp_err = `MINION_CSR.scpbit_unset_err_flag_tensor_quant;


   ////////////////////////////////////////////////////////////////////////////
   // Trans
   ////////////////////////////////////////////////////////////////////////////
   //`include "trans_defines.vh"

   logic f2_trans_tmp_wb;
   logic f3_trans_tmp_wb;
   logic f4_trans_tmp_wb;
   logic f5_trans_tmp_wb;
   logic f6_trans_tmp_wb;
   logic f7_trans_tmp_wb;

   `RST_FF(clock, reset, f2_trans_tmp_wb, ((`MINION_VPU_CTRL.trans.push_ex_state.id != 17) & (`MINION_VPU_CTRL.trans.push_ex_state.id != 0)), 1'b0) //TRANS_SIN_RR_STATE, TRANS_EXP_RR_STATE
   `RST_FF(clock, reset, f3_trans_tmp_wb, f2_trans_tmp_wb, 1'b0);
   `RST_FF(clock, reset, f4_trans_tmp_wb, f3_trans_tmp_wb, 1'b0);
   `RST_FF(clock, reset, f5_trans_tmp_wb, f4_trans_tmp_wb, 1'b0);
   `RST_FF(clock, reset, f6_trans_tmp_wb, f5_trans_tmp_wb, 1'b0);
   `RST_FF(clock, reset, f7_trans_tmp_wb, f6_trans_tmp_wb, 1'b0);
   `RST_FF(clock, reset, f8_trans_tmp_wb, f7_trans_tmp_wb, 1'b0);

   ////////////////////////////////////////////////////////////////////////////
   // Reduce
   ////////////////////////////////////////////////////////////////////////////

   logic f0_reduce_val;
   logic f1_reduce_val;
   logic f2_reduce_val;
   logic f3_reduce_val;
   logic f4_reduce_val;
   logic f5_reduce_val;
   logic f6_reduce_val;
   logic f7_reduce_val;


   assign f0_reduce_val = `MINION_VPU_REDUCE.exec_op && `MINION_VPU_REDUCE.enabled_int;

   //      CLK    RST    DOUT              DIN               DEF
   `RST_FF(clock, reset, f1_reduce_val, f0_reduce_val, 1'b0)
   `RST_FF(clock, reset, f2_reduce_val, f1_reduce_val, 1'b0)
   `RST_FF(clock, reset, f3_reduce_val, f2_reduce_val, 1'b0)
   `RST_FF(clock, reset, f4_reduce_val, f3_reduce_val, 1'b0)
   `RST_FF(clock, reset, f5_reduce_val, f4_reduce_val, 1'b0)
   `RST_FF(clock, reset, f6_reduce_val, f5_reduce_val, 1'b0)
   `RST_FF(clock, reset, f7_reduce_val, f6_reduce_val, 1'b0)
   `RST_FF(clock, reset, f8_reduce_val, f7_reduce_val, 1'b0)


   logic doing_reduce;
   logic doing_reduce_next;
   logic id_reduce_done;
   logic id_reduce_done_next;
   logic ex_reduce_done;
   logic f2_reduce_done;
   logic f3_reduce_done;
   logic f4_reduce_done;
   logic f5_reduce_done;
   logic f6_reduce_done;
   logic f7_reduce_done;
   logic f8_reduce_done;

   //      CLK    RST    DOUT                   DIN                        DEF
   `RST_FF(clock, reset, doing_reduce,          doing_reduce_next,         1'b0)
   `RST_FF(clock, reset, id_reduce_done,        id_reduce_done_next,       1'b0)
   `RST_FF(clock, reset, ex_reduce_done,        id_reduce_done,            1'b0)
   `RST_FF(clock, reset, f2_reduce_done,        ex_reduce_done,            1'b0)
   `RST_FF(clock, reset, f3_reduce_done,        f2_reduce_done,            1'b0)
   `RST_FF(clock, reset, f4_reduce_done,        f3_reduce_done,            1'b0)
   `RST_FF(clock, reset, f5_reduce_done,        f4_reduce_done,            1'b0)
   `RST_FF(clock, reset, f6_reduce_done,        f5_reduce_done,            1'b0)
   `RST_FF(clock, reset, f7_reduce_done,        f6_reduce_done,            1'b0)

   `RST_FF(clock, reset, id_reduce_action,      id_reduce_action_next,     '0)
   `RST_FF(clock, reset, ex_reduce_action,      id_reduce_action,          '0)
   `RST_FF(clock, reset, f2_reduce_action,      ex_reduce_action,          '0)
   `RST_FF(clock, reset, f3_reduce_action,      f2_reduce_action,          '0)
   `RST_FF(clock, reset, f4_reduce_action,      f3_reduce_action,          '0)
   `RST_FF(clock, reset, f5_reduce_action,      f4_reduce_action,          '0)
   `RST_FF(clock, reset, f6_reduce_action,      f5_reduce_action,          '0)
   `RST_FF(clock, reset, f7_reduce_action,      f6_reduce_action,          '0)

   `RST_FF(clock, reset, id_reduce_partner_id,  id_reduce_partner_id_next, '0)
   `RST_FF(clock, reset, ex_reduce_partner_id,  id_reduce_partner_id,      '0)
   `RST_FF(clock, reset, f2_reduce_partner_id,  ex_reduce_partner_id,      '0)
   `RST_FF(clock, reset, f3_reduce_partner_id,  f2_reduce_partner_id,      '0)
   `RST_FF(clock, reset, f4_reduce_partner_id,  f3_reduce_partner_id,      '0)
   `RST_FF(clock, reset, f5_reduce_partner_id,  f4_reduce_partner_id,      '0)
   `RST_FF(clock, reset, f6_reduce_partner_id,  f5_reduce_partner_id,      '0)
   `RST_FF(clock, reset, f7_reduce_partner_id,  f6_reduce_partner_id,      '0)

   `RST_FF(clock, reset, id_reduce_operation,  id_reduce_operation_next, '0)
   `RST_FF(clock, reset, ex_reduce_operation,  id_reduce_operation,      '0)
   `RST_FF(clock, reset, f2_reduce_operation,  ex_reduce_operation,      '0)
   `RST_FF(clock, reset, f3_reduce_operation,  f2_reduce_operation,      '0)
   `RST_FF(clock, reset, f4_reduce_operation,  f3_reduce_operation,      '0)
   `RST_FF(clock, reset, f5_reduce_operation,  f4_reduce_operation,      '0)
   `RST_FF(clock, reset, f6_reduce_operation,  f5_reduce_operation,      '0)
   `RST_FF(clock, reset, f7_reduce_operation,  f6_reduce_operation,      '0)

   `RST_FF(clock, reset, id_zero_reg_reduce,    id_zero_reg_reduce_next, '0)
   `RST_FF(clock, reset, ex_zero_reg_reduce,    id_zero_reg_reduce,      '0)
   `RST_FF(clock, reset, f2_zero_reg_reduce,    ex_zero_reg_reduce,      '0)
   `RST_FF(clock, reset, f3_zero_reg_reduce,    f2_zero_reg_reduce,      '0)
   `RST_FF(clock, reset, f4_zero_reg_reduce,    f3_zero_reg_reduce,      '0)
   `RST_FF(clock, reset, f5_zero_reg_reduce,    f4_zero_reg_reduce,      '0)
   `RST_FF(clock, reset, f6_zero_reg_reduce,    f5_zero_reg_reduce,      '0)
   `RST_FF(clock, reset, f7_zero_reg_reduce,    f6_zero_reg_reduce,      '0)

   `RST_FF(clock, reset, id_reduce_auto_pair,    id_reduce_auto_pair_next, '0)
   `RST_FF(clock, reset, ex_reduce_auto_pair,    id_reduce_auto_pair,      '0)
   `RST_FF(clock, reset, f2_reduce_auto_pair,    ex_reduce_auto_pair,      '0)
   `RST_FF(clock, reset, f3_reduce_auto_pair,    f2_reduce_auto_pair,      '0)
   `RST_FF(clock, reset, f4_reduce_auto_pair,    f3_reduce_auto_pair,      '0)
   `RST_FF(clock, reset, f5_reduce_auto_pair,    f4_reduce_auto_pair,      '0)
   `RST_FF(clock, reset, f6_reduce_auto_pair,    f5_reduce_auto_pair,      '0)
   `RST_FF(clock, reset, f7_reduce_auto_pair,    f6_reduce_auto_pair,      '0)

   `RST_FF(clock, reset, id_reduce_level,    id_reduce_level_next, '0)
   `RST_FF(clock, reset, ex_reduce_level,    id_reduce_level,      '0)
   `RST_FF(clock, reset, f2_reduce_level,    ex_reduce_level,      '0)
   `RST_FF(clock, reset, f3_reduce_level,    f2_reduce_level,      '0)
   `RST_FF(clock, reset, f4_reduce_level,    f3_reduce_level,      '0)
   `RST_FF(clock, reset, f5_reduce_level,    f4_reduce_level,      '0)
   `RST_FF(clock, reset, f6_reduce_level,    f5_reduce_level,      '0)
   `RST_FF(clock, reset, f7_reduce_level,    f6_reduce_level,      '0)

   always_comb
   begin
      doing_reduce_next = doing_reduce;
      id_reduce_done_next = 1'b0;
      id_reduce_action_next = id_reduce_action;
      id_reduce_partner_id_next = id_reduce_partner_id;
      id_reduce_operation_next = id_reduce_operation;
      id_zero_reg_reduce_next = id_zero_reg_reduce;
      id_reduce_auto_pair_next = id_reduce_auto_pair;
      id_reduce_level_next = id_reduce_level;
      if (!`MINION_VPU_CTRL.ml.tensorreduce.enabled)
      begin
         if (doing_reduce)
         begin
           doing_reduce_next = 1'b0;
           id_reduce_done_next = 1'b1;
           id_reduce_action_next = `MINION_DCACHE.reduce.action;
           id_reduce_partner_id_next = `MINION_DCACHE.reduce.partner_minion;
           id_reduce_operation_next = `MINION_DCACHE.reduce.operation;
           id_reduce_auto_pair_next = `MINION_DCACHE.reduce.pairing_auto;
           id_reduce_level_next = `MINION_DCACHE.reduce.level;
         end
      end


      if (`MINION_CORE.wb_valid && `MINION_CORE.csr_file.vpu_ctrl.reduce_start)
      begin
         doing_reduce_next = 1'b1;
         id_zero_reg_reduce_next = 1'b0;
         if (`MINION_VPU_CTRL.ml.tensorreduce.reg_pending_next == 0)
         begin
            id_zero_reg_reduce_next = 1'b1;
         end
      end
   end

   assign tensorreduce_action     = f7_reduce_action;
   assign tensorreduce_partner_id = f7_reduce_partner_id;
   assign tensorreduce_operation  = f7_reduce_operation;
   assign tensorreduce_is_done    = f7_reduce_done;
   assign zero_reg_reduce         = f7_zero_reg_reduce;
   assign tensorreduce_level      = f7_reduce_level;
   assign tensorreduce_auto_pair  = f7_reduce_auto_pair;


   ////////////////////////////////////////////////////////////////////////////////
   // Commit log
   ////////////////////////////////////////////////////////////////////////////////

   logic [1:0][`XREG_SIZE*3-1:0] log_reg_data;
   logic [1:0][`XREG_SIZE-1:0]   log_reg_data_wb;
   `FF(clock, log_reg_data[0], {log_reg_data[0][`XREG_SIZE*2-1:0], `MINION_CORE.ex_reg_data[0]})
   `FF(clock, log_reg_data[1], {log_reg_data[1][`XREG_SIZE*2-1:0], `MINION_CORE.ex_reg_data[1]})
   assign log_reg_data_wb = {log_reg_data[1][`XREG_SIZE*3-1:`XREG_SIZE*2], log_reg_data[0][`XREG_SIZE*3-1:`XREG_SIZE*2]};


   `ifndef ZEBU
      always_ff @ (negedge clock)
      begin
          if (verbose && `MINION_CORE.wb_valid)
          begin
             $display("C %4d.%0d: %10d [%d] pc=[%x] W[r%d=%x][%d] R[r%d=%x] R[r%d=%x] inst=[%x] DASM(%x)",
                      global_min_id,`MINION_CORE.wb_thread_id, cycle, `MINION_CORE.wb_valid, `MINION_CORE.wb_reg_pc,
                      `MINION_CORE.wb_rf_wen ? `MINION_CORE.wb_rf_waddr : 5'b0, `MINION_CORE.wb_rf_wdata, `MINION_CORE.wb_rf_wen,
                      wb_reg_inst[19:15], log_reg_data_wb[0],
                      wb_reg_inst[24:20], log_reg_data_wb[1],
                      wb_reg_inst, wb_reg_inst);
          end

          if (verbose && `MINION_VPU_CTRL.f8_ctrl_valid_qual) begin
             $display("VPU %4d.%0d: %10d inst_dec: %x", global_min_id, `MINION_VPU_CTRL.f6_thread_id, cycle, `MINION_VPU_CTRL.f6_ctrl_sigs);
          end

          if (verbose && `MINION_VPU.dcache_ctrl.scp_req.read_en) begin
             $display("VPU->L1 %4d.%0d: %10d address: %x type: Read tfma: %d" , global_min_id, 1, cycle, `MINION_VPU.dcache_ctrl.scp_req.addr, `MINION_VPU.dcache_ctrl.tfma_enabled);
          end
      end
   `endif // ifndef ZEBU

   ////////////////////////////////////////////////////////////////////////////////
   // Status signals
   ////////////////////////////////////////////////////////////////////////////////
   // TODO: Properly report stall reason for every thread in multithreading

   typedef struct packed
   {
       logic [1:0] fma_csr;
       logic gtsc; //gather/scater
       logic l1_miss_prfr1;
       logic l1_miss_prfr0;
       logic l1_miss_store1;
       logic l1_miss_store0;
       logic l1_miss_load1;
       logic l1_miss_load0;
       logic vpu_tensor_st_op;
       logic vpu_tensor_ld_op;
       logic vpu_tensor_ld;
       logic vpu_trans;
       logic vpu_tensor_fma;
       logic tx_rcv;
       logic tx_send;
       logic l1_evict;
       logic l1_miss;
       logic load;
       logic store;
       logic wr_long_f;
       logic busy_long_f;
       logic replay;
       logic mispredict;
       logic ic;
       logic dep_1;
       logic dep_2;
       logic dep_3;
       logic dep_4;
       logic dep_long_i;
       logic dep_long_f;
       logic dep_dcache_f;
       logic dep_tx_rcv_f;
       logic idiv;
       logic fence;
       logic dcache_not_ready;
       logic csr;
       logic gather_scatter;
       logic fp_toint;
       logic mova_mx;
   } status_signals;

   typedef struct packed
   {

     vpu_ctrl_sigs_t vpu_sigs;
     logic vpu_tensorfma_working;
     logic [`VPU_LANE_NUM-1:0]vpu_tensor_txfma;
     logic [`VPU_LANE_NUM-1:0]vpu_tensor_tima;
   } txfma_status_signals;

   txfma_status_signals fma_status_signals;
   status_signals id_status_signals;
   status_signals ex_status_signals;
   status_signals ex_status_signals_mux;
   status_signals gsc_status_signals;
   status_signals gsc_status_signals_mux;
   status_signals tag_status_signals;
   status_signals tag_status_signals_mux;
   status_signals mem_status_signals;
   status_signals mem_status_signals_mux;
   status_signals wb_status_signals;
   status_signals wb_status_signals_mux;

   // Computes the FP registers depending on outstanding TX receives
   logic        id_thread_id;
   logic [31:0] tx_rcv_inflight;
   logic        tx_rcv_dependency;

   always_comb
   begin
        id_thread_id = `MINION_CORE.id_thread_id;

        tx_rcv_inflight = 32'b0;
        tx_rcv_dependency = 1'b0;
   end

   // Computes the FP registers depending on outstanding dcache instructions
   logic [31:0] dcache_inflight;
   logic        dcache_dependency;

   always_comb
   begin
        dcache_inflight = 32'b0;

        // S0
        if (     `MINION_DCACHE.s0_valid_to_s1                         // Valid
            && (`MINION_DCACHE.s0_req.dest.thread_id == id_thread_id) // Same thread
            &&  `MINION_DCACHE.s0_req.dest.fp                         // FP
            && dcache_cmd_is_read(`MINION_DCACHE.s0_req.cmd)          // Load
        )
            dcache_inflight[`MINION_DCACHE.s0_req.dest.addr] = 1'b1;

        // S1
        if (     `MINION_DCACHE.s1_valid                               // Valid
            && (`MINION_DCACHE.s1_req.dest.thread_id == id_thread_id) // Same thread
            &&  `MINION_DCACHE.s1_req.dest.fp                         // FP
            && dcache_cmd_is_read(`MINION_DCACHE.s1_req.cmd)          // Load
        )
            dcache_inflight[`MINION_DCACHE.s1_req.dest.addr] = 1'b1;

        // S2
        if (     `MINION_DCACHE.s2_valid                               // Valid
            && (`MINION_DCACHE.s2_req.dest.thread_id == id_thread_id) // Same thread
            &&  `MINION_DCACHE.s2_req.dest.fp                         // FP
            && dcache_cmd_is_read(`MINION_DCACHE.s2_req.cmd)          // Load
        )
            dcache_inflight[`MINION_DCACHE.s2_req.dest.addr] = 1'b1;

        // ReplayQueue
        for (integer i = 0; i < `DCACHE_REPLAYQ_SIZE; i++)
        begin
            if (     `MINION_DCACHE.replay_queue.replayq_ready_info[i].pending              // Valid
                && (`MINION_DCACHE.replay_queue.replayq[i].dest.thread_id == id_thread_id) // Same thread
                &&  `MINION_DCACHE.replay_queue.replayq[i].dest.fp                         // FP
                && dcache_cmd_is_read(`MINION_DCACHE.replay_queue.replayq[i].cmd)          // Load
            )
                dcache_inflight[`MINION_DCACHE.replay_queue.replayq[i].dest.addr] = 1'b1;
        end

        // Checks if there's a dependency with dcache pending registers
        dcache_dependency = 1'b0;
        for (integer i = 0; i < 4; i++)
        begin
            if (     `MINION_CORE.id_valid                             // Instruction valid
                && dcache_inflight[`MINION_CORE.id_fp_hazard_addr[i]] // Register has a value coming from dcache
                && `MINION_CORE.id_fp_hazard_en[i]                    // Register is used
            )
                dcache_dependency = 1'b1;
        end
   end

   always_comb
   begin
       id_status_signals.wr_long_f        = `MINION_CORE.id_ctrl.fp && !`MINION_CORE.id_ctrl.mem && !`MINION_CORE.id_frontend_vpu_ctrl.is_trans && `MINION_CORE.id_vpu_ctrl.id_trans_insert && !`MINION_CORE.id_keepgsc;
       id_status_signals.busy_long_f      = 1'b0;
       id_status_signals.replay           = `MINION_CORE.wb_take_pc && (`MINION_CORE.wb_thread_id == `MINION_CORE.id_thread_id) && !`MINION_CORE.id_keepgsc;
       id_status_signals.mispredict       = `MINION_CORE.tag_take_pc && (`MINION_CORE.id_thread_id == `MINION_CORE.tag_thread_id) && !`MINION_CORE.id_keepgsc;
       id_status_signals.ic               = !`MINION_CORE.id_valid && !`MINION_CORE.id_keepgsc;
       id_status_signals.dep_1            = `MINION_CORE.id_hazard_ex && !`MINION_CORE.id_keepgsc;
       id_status_signals.dep_2            = `MINION_CORE.id_hazard_tag && !`MINION_CORE.id_keepgsc;
       id_status_signals.dep_3            = `MINION_CORE.id_hazard_mem && !`MINION_CORE.id_keepgsc;
       id_status_signals.dep_4            = `MINION_CORE.id_hazard_wb && !`MINION_CORE.id_keepgsc;
       id_status_signals.dep_long_i       = `MINION_CORE.id_int_sboard_hazard && !`MINION_CORE.id_keepgsc;
       id_status_signals.dep_long_f       = `MINION_CORE.id_ctrl.fp && `MINION_CORE.id_stall_vpu && !`MINION_CORE.id_keepgsc && !tx_rcv_dependency && !dcache_dependency;
       id_status_signals.dep_dcache_f     = dcache_dependency && !tx_rcv_dependency;
       id_status_signals.dep_tx_rcv_f     = tx_rcv_dependency;
       id_status_signals.idiv             = `MINION_CORE.id_ctrl.div && (!`MINION_CORE.ex_div_ready || (`MINION_CORE.wb_div_resp_valid && !`MINION_CORE.wb_div_resp_ready) || `MINION_CORE.ex_div_req);
       id_status_signals.fence            = `MINION_CORE.id_do_fence && !`MINION_CORE.id_keepgsc;
       id_status_signals.dcache_not_ready = !`MINION_CORE.id_dcache_ready && `MINION_CORE.id_ctrl.mem;
       id_status_signals.csr              = (`MINION_CORE.id_csr_stall[`MINION_CORE.id_thread_id] ||
                                            `MINION_CORE.id_csr_en && (`MINION_CORE.ex_reg_valid && (`MINION_CORE.ex_thread_id == `MINION_CORE.id_thread_id) ||
                                                                       `MINION_CORE.tag_reg_valid && (`MINION_CORE.tag_thread_id == `MINION_CORE.id_thread_id) ||
                                                                       `MINION_CORE.mem_reg_valid && (`MINION_CORE.mem_thread_id == `MINION_CORE.id_thread_id) ||
                                                                       `MINION_CORE.wb_reg_valid && (`MINION_CORE.wb_thread_id == `MINION_CORE.id_thread_id)
                                                                      )
                                           ) && !`MINION_CORE.id_keepgsc;
       id_status_signals.gather_scatter   = 1'b0;
       id_status_signals.fp_toint         = `MINION_CORE.id_ctrl.wxd && (  !`MINION_CORE.id_ctrl.fp && `MINION_CORE.ex_reg_valid  && `MINION_CORE.ex_ctrl_fp_toint
                                                                        ||                             `MINION_CORE.tag_reg_valid && `MINION_CORE.tag_ctrl_fp_toint
                                                                        || !`MINION_CORE.id_ctrl.fp && `MINION_CORE.mem_reg_valid && `MINION_CORE.mem_ctrl_fp_toint);
       id_status_signals.mova_mx          = `MINION_CORE.id_vpu_ctrl_mread && `MINION_CORE.wb_vpu_ctrl.mova_mx;
   end

   `FF(clock, ex_status_signals,  id_status_signals)

   always_comb
   begin
       ex_status_signals_mux                 = ex_status_signals;
       ex_status_signals_mux.replay         |= `MINION_CORE.wb_take_pc && (`MINION_CORE.wb_thread_id == `MINION_CORE.ex_thread_id);
       ex_status_signals_mux.mispredict     |= `MINION_CORE.tag_take_pc && (`MINION_CORE.ex_thread_id == `MINION_CORE.tag_thread_id);
       ex_status_signals_mux.gather_scatter |= `MINION_CORE.ex_gscing && !`MINION_CORE.ex_ctrl_kill;
   end

   `FF(clock, gsc_status_signals, ex_status_signals_mux)

   always_comb
   begin
       gsc_status_signals_mux                 = gsc_status_signals;
       gsc_status_signals_mux.gather_scatter |= `MINION_CORE.ex_ctrl.gsc && !`MINION_CORE.ex_ctrl_kill;
   end

   `FF(clock, tag_status_signals, `MINION_CORE.ex_ctrl.gsc ? gsc_status_signals_mux : ex_status_signals_mux)

   always_comb
   begin
       tag_status_signals_mux         = tag_status_signals;
       tag_status_signals_mux.replay |= `MINION_CORE.wb_take_pc && (`MINION_CORE.wb_thread_id == `MINION_CORE.tag_thread_id);
   end

   `FF(clock, mem_status_signals, tag_status_signals_mux)

   always_comb
   begin
       mem_status_signals_mux         = mem_status_signals;
       mem_status_signals_mux.replay |= `MINION_CORE.wb_take_pc && (`MINION_CORE.wb_thread_id == `MINION_CORE.mem_thread_id);
   end

   `FF(clock, wb_status_signals,  mem_status_signals_mux)

  logic dcache_mh0_miss_req, dcache_mh1_miss_req;
   assign dcache_mh0_miss_req = ((`MINION_DCACHE_MH0.req_pri_val && `MINION_DCACHE_MH0.req_pri_rdy) &&
                                (!`MINION_DCACHE_MH0.req_info.tag_match && `MINION_DCACHE_MH0.req_info.cacheable));
   assign dcache_mh1_miss_req = ((`MINION_DCACHE_MH1.req_pri_val && `MINION_DCACHE_MH1.req_pri_rdy) &&
                                (!`MINION_DCACHE_MH1.req_info.tag_match && `MINION_DCACHE_MH1.req_info.cacheable));
   always_comb
   begin
       wb_status_signals_mux                = wb_status_signals;
       wb_status_signals_mux.replay        |= `MINION_CORE.wb_take_pc;
       wb_status_signals_mux.load           = `MINION_CORE.wb_valid && `MINION_CORE.wb_ctrl.mem &&  (`MINION_CORE.wb_ctrl.wfd || `MINION_CORE.wb_ctrl.wxd);
       wb_status_signals_mux.store          = `MINION_CORE.wb_valid && `MINION_CORE.wb_ctrl.mem && !(`MINION_CORE.wb_ctrl.wfd || `MINION_CORE.wb_ctrl.wxd);
       wb_status_signals_mux.l1_miss        = (minion.core.l2_dcache_miss_req_ready[`DCACHE_L2_MISS_IDX_COMM] && minion.core.l2_dcache_miss_req_valid[`DCACHE_L2_MISS_IDX_COMM]) && (minion.core.l2_dcache_miss_req.opcode == ET_LINK_REQ_Read)
                                           && ((et_link_core_miss_id_t'(minion.core.l2_dcache_miss_req.id) == ET_LINK_Core_Miss_Id_Miss_Handler_0) ||
                                               (et_link_core_miss_id_t'(minion.core.l2_dcache_miss_req.id) == ET_LINK_Core_Miss_Id_Miss_Handler_1));

       wb_status_signals_mux.l1_miss_load0  = dcache_mh0_miss_req && (`MINION_DCACHE_MH0.req_info.cmd==dcache_cmd_XRD) | '0;
       wb_status_signals_mux.l1_miss_load1  = dcache_mh1_miss_req && (`MINION_DCACHE_MH1.req_info.cmd==dcache_cmd_XRD) | '0;
       wb_status_signals_mux.l1_miss_store0 = dcache_mh0_miss_req && (`MINION_DCACHE_MH0.req_info.cmd==dcache_cmd_XWR) | '0;
       wb_status_signals_mux.l1_miss_store1 = dcache_mh1_miss_req && (`MINION_DCACHE_MH1.req_info.cmd==dcache_cmd_XWR) | '0;
       wb_status_signals_mux.l1_miss_prfr0  = dcache_mh0_miss_req && (`MINION_DCACHE_MH0.req_info.cmd==dcache_cmd_PFR) | '0;
       wb_status_signals_mux.l1_miss_prfr1  = dcache_mh1_miss_req && (`MINION_DCACHE_MH1.req_info.cmd==dcache_cmd_PFR) | '0;

       wb_status_signals_mux.l1_evict       = !`MINION_DCACHE.writeback_unit.active && `MINION_DCACHE.writeback_unit.active_next;
       wb_status_signals_mux.tx_send        = (minion.core.l2_dcache_evict_req_ready && minion.core.l2_dcache_evict_req_valid) && (minion.core.l2_dcache_evict_req.opcode == ET_LINK_REQ_MsgSendData)
                                              && ((et_link_core_miss_id_t'(minion.core.l2_dcache_miss_req.id) == ET_LINK_Core_Miss_Id_Miss_Handler_0) ||
                                                  (et_link_core_miss_id_t'(minion.core.l2_dcache_miss_req.id) == ET_LINK_Core_Miss_Id_Miss_Handler_1));
       wb_status_signals_mux.tx_rcv           = 1'b0; // TODO, this arrives as a message
       wb_status_signals_mux.vpu_tensor_fma   = `MINION_VPU_CTRL.ex_tima_valid || `MINION_VPU_CTRL.ex_txfma_valid;
       wb_status_signals_mux.vpu_trans        = 1'b0;//`MINION_VPU_CTRL.f2_trans_div;
       wb_status_signals_mux.vpu_tensor_ld    = `MINION_DCACHE.tensor_load_0.busy | `MINION_DCACHE.tensor_load_1.busy;
       wb_status_signals_mux.vpu_tensor_ld_op = `MINION_DCACHE.tensor_load_0.tl_op | `MINION_DCACHE.tensor_load_1.tl_op;
       wb_status_signals_mux.vpu_tensor_st_op = `MINION_DCACHE.reduce.ts_op;
       wb_status_signals_mux.gtsc             = `MINION_CORE.wb_ctrl.mem && `MINION_CORE.wb_ctrl.mem_type inside {`MT_PS_GS8,`MT_PS_GS16,`MT_PS_GS32};
       wb_status_signals_mux.fma_csr          = !`MINION_CORE.csr_file.reg_tensor_fma_en ? 2'b00 : `MINION_CORE.csr_file.reg_tensor_fma[3:1] == 3'b000 ? 2'b10 : `MINION_CORE.csr_file.reg_tensor_fma[2:1];



       fma_status_signals.vpu_sigs              = `MINION_VPU_CTRL.ex_req.sigs;
       fma_status_signals.vpu_tensor_txfma      = `MINION_VPU_CTRL.ex_txfma_valid;
       fma_status_signals.vpu_tensor_tima       = `MINION_VPU_CTRL.ex_tima_valid;
       fma_status_signals.vpu_tensorfma_working = `MINION_VPU_CTRL.ml.tensorfma.working;

   end


   `ifndef ZEBU
      always @(negedge clock) begin
         if (verbose && display_status && !reset && enabled[0]) begin
            $display("C %4d.%0d: %d status=[%x]", global_min_id, `MINION_CORE.wb_thread_id, cycle, wb_status_signals_mux);
            if (`MINION_VPU_CTRL.ex_txfma_valid || `MINION_VPU_CTRL.ex_tima_valid || `MINION_VPU_CTRL.ml.tensorfma.working) begin
               $display("TXFMA %4d: %d status=[%x]", global_min_id, cycle, fma_status_signals);
            end
         end
      end

      always @(`MINION_CSR.io_csr_stall) begin
         if ((default_verbose_level <= 1)&& !reset && enabled[0]) begin
            $display("io_csr_stall%0d=%0d at %0d", global_min_id, `MINION_CSR.io_csr_stall, cycle);
         end
      end

      always @(negedge reset) begin
         if ((default_verbose_level <= 1) && enabled[0]) begin
            $display("Minion %0d out of reset at cycle %0d and time t=%0t", global_min_id, cycle, $time);
         end
      end

      `ifdef ASMON_VPU_DBG
         `include "arch_state_monitor_vpu_dbg.vh"
      `endif
   `endif // ifndef ZEBU

   // mirror VPU RF
   logic [1:0][`VPU_LANE_NUM-1:0]       vpu_rf_we;
   logic [1:0][`VPU_LANE_NUM-1:0][31:0] vpu_rf_data;
   logic [1:0][`VPU_LANE_NUM-1:0][4:0]  vpu_rf_waddr;
   logic [1:0][`VPU_LANE_NUM-1:0]       vpu_rf_tid;

   generate
      for (genvar p = 0; p < 2; p++) begin: VPU_RF_MIRROR_WR_PORTS
         for (genvar k = 0; k < `VPU_LANE_NUM; k++) begin: VPU_RF_MIRROR_LANES
            assign vpu_rf_we[p][k]    = `MINION_VPU.VPU_LANES[k].lane.rf.wr_en[p] & `MINION_VPU.VPU_LANES[k].lane.rf.wr_mask[p];
            assign vpu_rf_data[p][k]  = `MINION_VPU.VPU_LANES[k].lane.rf.wr_data[p];
            assign vpu_rf_waddr[p][k] = `MINION_VPU.VPU_LANES[k].lane.rf.wr_addr[p];
            assign vpu_rf_tid[p][k]   = `MINION_VPU.VPU_LANES[k].lane.rf.wr_thrid[p];
         end
      end
   endgenerate


   always_ff@(posedge clock) begin
      for (int p = 0; p < 2; p++) begin
         if (vpu_rf_we[p] != 0) begin
            `event_common_assign(vpu_rf_update)
            vpu_rf_update.wr_mask = vpu_rf_we[p];
            vpu_rf_update.thread_ids = vpu_rf_tid[p];
            `event_copy(vpu_rf_update.addr, vpu_rf_waddr[p])
            `event_copy(vpu_rf_update.data, vpu_rf_data[p])
            `event_trigger(vpu_rf_update)
         end
      end
   end


`ifndef ZEBU
  ////////////////////////////////////////////////////////////////////////////////
  // ET-POWERFLOW monitors
  ////////////////////////////////////////////////////////////////////////////////


  //Compute timing window generation for the different types of power benchmarks


  //VIRUS70:
  //  - window starts in the first tensor fma when the first tima operation is computed
  //  - window ends in the ET_POWERFLOW_VIRUS70_ITER_NUM'th tensor quant when the last txfma operation is computed

    `define ET_POWERFLOW_VIRUS70_ITER_NUM 7


    logic virus70_first_tensorfma_enabled, virus70_tensorquant_enabled, virus70_txfma_valid_all;
    logic virus70_window_start, virus70_window_end;
    real virus70_iter_cnt, virus70_tima_valid_cnt, virus70_start_cycle;

    //         CLK    RST    EN                                DOUT                             DIN                                                                            DEF
    `RST_EN_FF(clock, reset, !virus70_first_tensorfma_enabled, virus70_first_tensorfma_enabled, `MINION_VPU_CTRL.ml.tensorfma.enabled & `MINION_VPU_CTRL.ex_tima_valid_unqual, 1'b0)
    `RST_FF(   clock, reset,                                   virus70_tensorquant_enabled,     `MINION_VPU_CTRL.ml.tensorquant.enabled,                                       1'b0)
    `RST_FF(   clock, reset,                                   virus70_txfma_valid_all,         `MINION_VPU_CTRL.ex_txfma_valid_all,                                           1'b0)


    always @ (posedge clock) begin
       if (reset) virus70_iter_cnt<=0;
       if(virus70_tensorquant_enabled & ~`MINION_VPU_CTRL.ml.tensorquant.enabled) virus70_iter_cnt<=virus70_iter_cnt+1;

       if (reset) virus70_tima_valid_cnt<=0;
       if(`MINION_VPU_CTRL.ml.tensorfma.enabled & `MINION_VPU_CTRL.ex_tima_valid_unqual) virus70_tima_valid_cnt<=virus70_tima_valid_cnt+1;
    end

    //windows start/end triggers
    assign virus70_window_start = !virus70_first_tensorfma_enabled & (`MINION_VPU_CTRL.ml.tensorfma.enabled & `MINION_VPU_CTRL.ex_tima_valid_unqual);
    assign virus70_window_end = virus70_tensorquant_enabled & virus70_txfma_valid_all & ~`MINION_VPU_CTRL.ex_txfma_valid_all & (virus70_iter_cnt==`ET_POWERFLOW_VIRUS70_ITER_NUM-1);

   //dump et-powerflow messages
   always @ (virus70_window_start, virus70_window_end) begin
      if (global_min_id==0 && virus70_window_start) begin
         $display("ET-POWERFLOW-Virus70:: Window starts at %0d", $realtime);
         virus70_start_cycle <= $realtime;
      end
      if (global_min_id==0 && virus70_window_end) begin
         $display("ET-POWERFLOW-Virus70:: Window ends at %0d", $realtime);
      end
      if (global_min_id==0 && virus70_window_end) begin
         $display("ET-POWERFLOW-Virus70:: TIMA activity is %0.2f %% (%0d tima operations)", ((virus70_tima_valid_cnt/($realtime-virus70_start_cycle))*100),virus70_tima_valid_cnt);
      end
   end
   ////////////////



  //Sparse NN MLP:
  //  - window starts in the first tensor fma when the first txfma operation is computed
  //  - window ends in the ET_POWERFLOW_SPARSE_NN_MLP_ITER_NUM'th tensor fma when the last txfma operation is computed

    `define ET_POWERFLOW_SPARSE_NN_MLP_ITER_NUM 16

    logic sparse_nn_mlp_first_tensorfma_enabled, sparse_nn_mlp_tensorfma_enabled, sparse_nn_mlp_txfma_valid_all;
    logic sparse_nn_mlp_window_start, sparse_nn_mlp_window_end;
    real sparse_nn_mlp_iter_cnt, sparse_nn_mlp_txfma_valid_cnt, sparse_nn_mlp_start_cycle;

    //         CLK    RST    EN                                      DOUT                                   DIN                                                                          DEF
    `RST_EN_FF(clock, reset, !sparse_nn_mlp_first_tensorfma_enabled, sparse_nn_mlp_first_tensorfma_enabled, `MINION_VPU_CTRL.ml.tensorfma.enabled & `MINION_VPU_CTRL.ex_txfma_valid_all, 1'b0)
    `RST_FF(   clock, reset,                                         sparse_nn_mlp_tensorfma_enabled,       `MINION_VPU_CTRL.ml.tensorfma.enabled,                                       1'b0)
    `RST_FF(   clock, reset,                                         sparse_nn_mlp_txfma_valid_all,         `MINION_VPU_CTRL.ex_txfma_valid_all,                                         1'b0)

    always @ (posedge clock) begin
      if (reset) sparse_nn_mlp_iter_cnt<=0;
      if(sparse_nn_mlp_tensorfma_enabled & ~`MINION_VPU_CTRL.ml.tensorfma.enabled) sparse_nn_mlp_iter_cnt<=sparse_nn_mlp_iter_cnt+1;

      if (reset) sparse_nn_mlp_txfma_valid_cnt<=0;
      if(`MINION_VPU_CTRL.ml.tensorfma.enabled & `MINION_VPU_CTRL.ex_txfma_valid_all) sparse_nn_mlp_txfma_valid_cnt<=sparse_nn_mlp_txfma_valid_cnt+1;
    end

      //windows start/end triggers
    assign sparse_nn_mlp_window_start = !sparse_nn_mlp_first_tensorfma_enabled & (`MINION_VPU_CTRL.ml.tensorfma.enabled & `MINION_VPU_CTRL.ex_txfma_valid_all);
    assign sparse_nn_mlp_window_end = sparse_nn_mlp_tensorfma_enabled & sparse_nn_mlp_txfma_valid_all & ~`MINION_VPU_CTRL.ex_txfma_valid_all & (sparse_nn_mlp_iter_cnt==`ET_POWERFLOW_SPARSE_NN_MLP_ITER_NUM-1);

      //dump et-powerflow messages
    always @ (sparse_nn_mlp_window_start, sparse_nn_mlp_window_end) begin
      if (global_min_id==0 && sparse_nn_mlp_window_start) begin
        $display("ET-POWERFLOW-Sparse NN MLP:: Window starts at %0d", $realtime);
        sparse_nn_mlp_start_cycle <= $realtime;
      end
      if (global_min_id==0 && sparse_nn_mlp_window_end)
        $display("ET-POWERFLOW-Sparse NN MLP:: Window ends at %0d", $realtime);

      if (global_min_id==0 && sparse_nn_mlp_window_end)
        $display("ET-POWERFLOW-Sparse NN MLP:: TXFMA activity is %0.2f %% (%0d txfma operations)", ((sparse_nn_mlp_txfma_valid_cnt/($realtime-sparse_nn_mlp_start_cycle))*100),sparse_nn_mlp_txfma_valid_cnt);
    end
  ////////////////



  //Sparse NN EMB:
  //  1- monitor detects sparse nn emb activity when the first TXFMA FMA operation is computed
  //  2- after that, the monitor generates `ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM timing window bins containing txfma activity:
  //     - a new window is generated every `ET_POWERFLOW_SPARSE_NN_EMB_POWER_WINDOW_OFFSET ns
  //     - window length is `ET_POWERFLOW_SPARSE_NN_EMB_POWER_WINDOW_LENGTH ns
  //  3- when the last bin `ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM completes:
  //     - a global txfma activity is calculated
  //     - all bins are processed and select the window bin with txfma activity closest to the global txfma activity
  //  4- monitor dumps the selected timing window bin with


    `define ET_POWERFLOW_SPARSE_NN_EMB_POWER_WINDOW_LENGTH 15000
    `define ET_POWERFLOW_SPARSE_NN_EMB_POWER_WINDOW_OFFSET 2000
    `define ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM 25


    logic   sparse_nn_emb_first_txfma;
    real    sparse_nn_emb_global_time_cnt, sparse_nn_emb_global_txfma_ops_cnt;
    real    sparse_nn_emb_global_window_start, sparse_nn_emb_global_window_end, sparse_nn_emb_global_window_end_pre;
    real    sparse_nn_emb_global_txfma_activity;
    logic   sparse_nn_emb_bin_enable        [`ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM-1:0];
    real    sparse_nn_emb_bin_window_start  [`ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM-1:0];
    real    sparse_nn_emb_bin_window_end    [`ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM-1:0];
    real    sparse_nn_emb_bin_txfma_ops_cnt [`ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM-1:0];
    real    sparse_nn_emb_bin_txfma_activity [`ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM-1:0];
    real    sparse_nn_emb_global_txfma_activity_error;
    integer sparse_nn_emb_global_bin_sel;
    logic   sparse_nn_emb_bin_enable_last;
    logic   sparse_nn_emb_window_start;
    logic   sparse_nn_emb_window_end_pre, sparse_nn_emb_window_end;

    //         CLK    RST    EN                          DOUT                           DIN                                                                            DEF
    `RST_EN_FF(clock, reset, !sparse_nn_emb_first_txfma, sparse_nn_emb_first_txfma,     (`MINION_VPU_CTRL.ex_txfma_valid_all & `MINION_VPU_CTRL.ex_vpu_ctrl.sigs.fma), 1'b0)
    `RST_FF   (clock, reset,                             sparse_nn_emb_bin_enable_last, sparse_nn_emb_bin_enable[`ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM-1],              1'b0)
    `RST_FF   (clock, reset,                             sparse_nn_emb_window_end,      sparse_nn_emb_window_end_pre,                                                  1'b0)

    always @ (posedge clock) begin
      sparse_nn_emb_window_start <=1'b0;

        //ops counter
      if (reset) sparse_nn_emb_global_txfma_ops_cnt<=0;
      if(sparse_nn_emb_first_txfma && `MINION_VPU_CTRL.ex_txfma_valid_all) sparse_nn_emb_global_txfma_ops_cnt<=sparse_nn_emb_global_txfma_ops_cnt+1;

        //global window start
      if (!sparse_nn_emb_first_txfma & (`MINION_VPU_CTRL.ex_txfma_valid_all & `MINION_VPU_CTRL.ex_vpu_ctrl.sigs.fma)) begin
        sparse_nn_emb_global_window_start<=$realtime;
        sparse_nn_emb_window_start<=1'b1;
      end

       //global window end
      if (reset) sparse_nn_emb_global_window_end<=0;
      if (($realtime-sparse_nn_emb_bin_window_start[`ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM-1])>=`ET_POWERFLOW_SPARSE_NN_EMB_POWER_WINDOW_LENGTH-1) begin
        sparse_nn_emb_global_window_end<=$realtime;
      end

        //global time cnt
      if(reset) sparse_nn_emb_global_time_cnt<=0;
      if(sparse_nn_emb_first_txfma) sparse_nn_emb_global_time_cnt<=sparse_nn_emb_global_time_cnt+1;
    end

      //window bins
    genvar b;
    generate
      for (b = 0; b < `ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM; b = b + 1) begin:sparse_nn_emb_bins
        always @(posedge clock) begin
          if (reset) begin
            sparse_nn_emb_bin_txfma_ops_cnt[b]<=0;
            sparse_nn_emb_bin_enable[b]<=0;
            sparse_nn_emb_bin_window_start[b]<=0;
          end

            //create a new bin after `ET_POWERFLOW_SPARSE_NN_EMB_POWER_WINDOW_OFFSET ns
          if (!sparse_nn_emb_bin_enable[b] && sparse_nn_emb_first_txfma && (sparse_nn_emb_global_time_cnt==`ET_POWERFLOW_SPARSE_NN_EMB_POWER_WINDOW_OFFSET*b))
            sparse_nn_emb_bin_enable[b]<=1'b1;

            //store info
          if(sparse_nn_emb_bin_enable[b]) begin
              //count txfma operations for the created bin
            if(`MINION_VPU_CTRL.ex_txfma_valid_all)
              sparse_nn_emb_bin_txfma_ops_cnt[b]<=sparse_nn_emb_bin_txfma_ops_cnt[b]+1;

              //store window start
            if (sparse_nn_emb_bin_window_start[b]==0)
              sparse_nn_emb_bin_window_start[b]<=$realtime;
              //store window end
            else if (($realtime-sparse_nn_emb_bin_window_start[b])>=`ET_POWERFLOW_SPARSE_NN_EMB_POWER_WINDOW_LENGTH-1) begin
              sparse_nn_emb_bin_window_end[b]<=$realtime;
              sparse_nn_emb_bin_enable[b]<=1'b0;
              sparse_nn_emb_bin_txfma_activity[b]<=(sparse_nn_emb_bin_txfma_ops_cnt[b]/($realtime-sparse_nn_emb_bin_window_start[b]));
            end
          end
        end
      end
    endgenerate

      //search for the txfma activity bin closest to the global txfma activity and dump et-powerflow messages
    real err;
    always @ (reset or sparse_nn_emb_global_window_end) begin
      sparse_nn_emb_window_end_pre =1'b0;

      if (global_min_id==0 && (sparse_nn_emb_bin_enable_last & !sparse_nn_emb_bin_enable[`ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM-1])) begin
          //global activity avg
        sparse_nn_emb_global_txfma_activity = sparse_nn_emb_global_txfma_ops_cnt/(sparse_nn_emb_global_window_end-sparse_nn_emb_global_window_start);

        sparse_nn_emb_global_txfma_activity_error=sparse_nn_emb_global_txfma_activity-sparse_nn_emb_bin_txfma_activity[0];
        if (sparse_nn_emb_global_txfma_activity_error<0) sparse_nn_emb_global_txfma_activity_error = -sparse_nn_emb_global_txfma_activity_error;

        for (int i=0;i<`ET_POWERFLOW_SPARSE_NN_EMB_ITER_NUM-1;i++) begin
          err = sparse_nn_emb_global_txfma_activity-sparse_nn_emb_bin_txfma_activity[i];
          if (err<0) err=-err;

          if (err<=sparse_nn_emb_global_txfma_activity_error) begin
             sparse_nn_emb_global_txfma_activity_error=err;
             sparse_nn_emb_global_bin_sel=i;
          end
        end

        $display("ET-POWERFLOW-Sparse NN EMB:: Global Window starts at %0d", sparse_nn_emb_global_window_start);
        $display("ET-POWERFLOW-Sparse NN EMB:: Global Window ends at %0d", sparse_nn_emb_global_window_end);
        $display("ET-POWERFLOW-Sparse NN EMB:: Global TXFMA activity is %0.2f %% (%0d txfma operations)",sparse_nn_emb_global_txfma_activity*100,sparse_nn_emb_global_txfma_ops_cnt);
        $display("ET-POWERFLOW-Sparse NN EMB:: Selected Window starts at %0d", sparse_nn_emb_bin_window_start[sparse_nn_emb_global_bin_sel]);
        $display("ET-POWERFLOW-Sparse NN EMB:: Selected Window ends at %0d", sparse_nn_emb_bin_window_end[sparse_nn_emb_global_bin_sel]);
        $display("ET-POWERFLOW-Sparse NN EMB:: Selected TXFMA activity is %0.2f %% (%0d txfma operations)",(sparse_nn_emb_bin_txfma_activity[sparse_nn_emb_global_bin_sel])*100,sparse_nn_emb_bin_txfma_ops_cnt[sparse_nn_emb_global_bin_sel]);
        sparse_nn_emb_window_end_pre=1'b1;
      end
    end


    ////////////////
`endif // ifdef ZEBU


////////////////////////////////////////////////////////////////////////////
// Msg tracking
////////////////////////////////////////////////////////////////////////////

import "DPI-C" context function void minion_new_msg(input int unsigned shire_id,
                                                    input int unsigned shire_id_dest,
                                                    input int unsigned hart_id_src,
                                                    input int unsigned hart_id_dest,
                                                    input int unsigned port_id,
                                                    input bit          to_localnet);


generate for (genvar k=0; k<2; ++k) begin : msg_tracking

  logic [`MIN_PER_S_R]     msg_minion_dest;
  logic [`MIN_PER_S_R]     msg_minion_src;
  logic [`MAX_HART_ID_R]   msg_hart_id_src;
  logic [`MAX_HART_ID_R]   msg_hart_id_dest;
  logic [`NUM_SHIRE_IDS_R] msg_shire_dest_code;
  logic [`NUM_SHIRE_IDS_R] msg_shire_dest;
  logic                    msg_to_currentneigh;
  logic                    msg_to_currentshire;
  logic                    msg_possible_localpath;
  logic                    msg_to_localnet;

  always @(posedge clock) begin
    msg_minion_dest = `MINION_DCACHE.miss_handler_unit.mh_l2_evict_req[k].address[`AD_ESR_SHIRE_MIN_ID_RANGE]; // Dest minion
    msg_minion_src  = (shire_min_id); // shire_min_id : 0..31
    msg_hart_id_src = {global_min_id, k[0]}; // global min id {shire_id,shire_min_id}

    case (msg_minion_src)
      7'd0: msg_possible_localpath = msg_minion_dest inside {4,2,1};
      7'd1: msg_possible_localpath = msg_minion_dest inside {0};
      7'd2: msg_possible_localpath = msg_minion_dest inside {0,3};
      7'd3: msg_possible_localpath = msg_minion_dest inside {2};
      7'd4: msg_possible_localpath = msg_minion_dest inside {0,6,5};
      7'd5: msg_possible_localpath = msg_minion_dest inside {4};
      7'd6: msg_possible_localpath = msg_minion_dest inside {4,7};
      7'd7: msg_possible_localpath = msg_minion_dest inside {6};
      7'd8: msg_possible_localpath = msg_minion_dest inside {12,10,9};
      7'd9: msg_possible_localpath = msg_minion_dest inside {8};
      7'd10: msg_possible_localpath = msg_minion_dest inside {8,11};
      7'd11: msg_possible_localpath = msg_minion_dest inside {10};
      7'd12: msg_possible_localpath = msg_minion_dest inside {8,14,13};
      7'd13: msg_possible_localpath = msg_minion_dest inside {12};
      7'd14: msg_possible_localpath = msg_minion_dest inside {12,15};
      7'd15: msg_possible_localpath = msg_minion_dest inside {14};
      7'd16: msg_possible_localpath = msg_minion_dest inside {20,18,17};
      7'd17: msg_possible_localpath = msg_minion_dest inside {16};
      7'd18: msg_possible_localpath = msg_minion_dest inside {16,19};
      7'd19: msg_possible_localpath = msg_minion_dest inside {18};
      7'd20: msg_possible_localpath = msg_minion_dest inside {16,22,21};
      7'd21: msg_possible_localpath = msg_minion_dest inside {20};
      7'd22: msg_possible_localpath = msg_minion_dest inside {20,23};
      7'd23: msg_possible_localpath = msg_minion_dest inside {22};
      7'd24: msg_possible_localpath = msg_minion_dest inside {28,26,25};
      7'd25: msg_possible_localpath = msg_minion_dest inside {24};
      7'd26: msg_possible_localpath = msg_minion_dest inside {24,27};
      7'd27: msg_possible_localpath = msg_minion_dest inside {26};
      7'd28: msg_possible_localpath = msg_minion_dest inside {24,30,29};
      7'd29: msg_possible_localpath = msg_minion_dest inside {28};
      7'd30: msg_possible_localpath = msg_minion_dest inside {28,31};
      7'd31: msg_possible_localpath = msg_minion_dest inside {30};
      default: msg_possible_localpath = 1'b0;
    endcase // case (msg_minion_src)

    msg_shire_dest_code = `MINION_DCACHE.miss_handler_unit.mh_l2_evict_req[k].address[`AD_ESR_SHIRE_ID_RANGE];
    msg_shire_dest = (msg_shire_dest_code == `LOCAL_SHIRE_ID) ? shire_id : msg_shire_dest_code;

    msg_to_currentshire = (msg_shire_dest == shire_id);
    msg_to_localnet = msg_to_currentshire && msg_possible_localpath;

    if ( !reset && !disable_BEMU_msg_tracking) begin
      if (`MINION_DCACHE.miss_handler_unit.ma_evict_arb_grant[k] &&
          `MINION_DCACHE.miss_handler_unit.mh_l2_evict_valid[k]  &&
          `MINION_DCACHE.miss_handler_unit.mh_l2_evict_req[k].opcode == ET_LINK_REQ_MsgSendData)
      begin

         msg_hart_id_dest = {msg_shire_dest, msg_minion_dest,`MINION_DCACHE.miss_handler_unit.mh_l2_evict_req[k].address[`AD_ESR_TH_ID_START]};
         minion_new_msg(shire_id,
                        msg_shire_dest,                                                                     // dest shire
                        msg_hart_id_src,                                                                    // src hart id
                        msg_hart_id_dest,                                                                   // dest hart id
                        `MINION_DCACHE.miss_handler_unit.mh_l2_evict_req[k].address[`AD_ESR_PORT_ID_RANGE], // dest port id
                        msg_to_localnet );
      end
    end
  end // always @ (posedge clock)
end
endgenerate

////////////////////////////////////////////////////////////////////////////
// Performance tracking
////////////////////////////////////////////////////////////////////////////

// Detect TEST_START and TEST_END hints

`define TIMER_START_HINT 32'h60002013
`define TIMER_END_HINT   32'h60102013

logic [31:0] pft_cycle_counter;
logic [`CORE_NR_THREADS-1:0][31:0] pft_cycle;
logic [31:0] pft_hid;
logic [31:0] pft_hid_base;

logic pft_wb_valid_mon;
logic pft_wb_thread_id_mon;
logic [31:0] pft_inst_bits_mon_id;
logic [31:0] pft_inst_bits_mon_ex;
logic [31:0] pft_inst_bits_mon_tag;
logic [31:0] pft_inst_bits_mon_mem;
logic [31:0] pft_inst_bits_mon_wb;

logic pft_start;
logic pft_end;

assign pft_inst_bits_mon_id = `MINION_CORE.id_inst_bits;
assign pft_wb_valid_mon = `MINION_CORE.wb_valid;
assign pft_wb_thread_id_mon = `MINION_CORE.wb_thread_id;

assign pft_hid_base = 32'(shire_id)*64 + 32'(shire_min_id)*2;


always @(posedge clock)
begin
  if (reset)
  begin
    pft_cycle_counter <= 0;
    pft_cycle[0] <= 0;
    pft_cycle[1] <= 0;
    pft_inst_bits_mon_ex <= 0;
    pft_inst_bits_mon_tag <= 0;
    pft_inst_bits_mon_mem <= 0;
    pft_inst_bits_mon_wb <= 0;

    pft_start = 0;
    pft_end = 0;
  end else
  begin
    pft_cycle_counter <= pft_cycle_counter + 1;

    pft_inst_bits_mon_ex <= pft_inst_bits_mon_id;
    pft_inst_bits_mon_tag <= pft_inst_bits_mon_ex;
    pft_inst_bits_mon_mem <= pft_inst_bits_mon_tag;
    pft_inst_bits_mon_wb <= pft_inst_bits_mon_mem;

    pft_start = 0;
    pft_end = 0;
    if (pft_wb_valid_mon)
    begin
      if (pft_inst_bits_mon_wb == `TIMER_START_HINT)
      begin
        pft_hid = pft_hid_base | pft_wb_thread_id_mon;
        $display("PERF_TRACK: H%0d: Set start_cycle = %d", pft_hid, pft_cycle_counter);
        pft_cycle[pft_wb_thread_id_mon] <= pft_cycle_counter;
        pft_start = 1;
      end
      else if (pft_inst_bits_mon_wb == `TIMER_END_HINT)
      begin
        pft_hid = pft_hid_base | pft_wb_thread_id_mon;
        $display("PERF_TRACK: H%0d: Set last_cycle = %d", pft_hid, pft_cycle_counter);
        $display("PERF_TRACK: H%0d: Total Runtime Cycles: %d", pft_hid, pft_cycle_counter-pft_cycle[pft_wb_thread_id_mon]);
        pft_end = 1;
      end
    end
  end
end

// Detect operating frequency for the Minion 0
// Assuming all Minions in shire have the same frequency
real pft_period_start;
real pft_period_end;
initial begin
  // Wait until reset is done
  wait (reset == 1);
  wait (reset == 0);
  // Wait until first istruction is retired in Minion 0
  if (shire_min_id == 0)
  begin
    @(posedge pft_wb_valid_mon);
    // Now, measure clock period
    @(posedge clock);
    pft_period_start = $realtime;
    @(posedge clock);
    pft_period_end = $realtime;
    $display("PERF_TRACK: S%0d M%0d: Period: %.3f ns - %.3f = %.3f ns", shire_id, shire_min_id, pft_period_end, pft_period_start, pft_period_end - pft_period_start);
    if (pft_period_end - pft_period_start != 0.0)
      $display("PERF_TRACK: S%0d M%0d: Frequency: %.0f MHz", shire_id, shire_min_id, 1000.0/(pft_period_end - pft_period_start));
    else
      $display("PERF_TRACK: S%0d M%0d: Frequency: ??? MHz", shire_id, shire_min_id);
  end
end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Other checkings
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Add assertion to ensure that a Fill operation never gets blocked more than a few cycles in the L2 resp interface
  // This assertion is removes as the tests generate circumstances in which the number of cycles can be quite long, and are legal situations
  // logic [7:0] count_fill_not_ready;
  // logic l2_receive_data_paused;

  // assign l2_receive_data_paused = `MINION_DCACHE.l2_resp_valid_int & !`MINION_DCACHE.l2_resp_ready_int;

  // always_ff @(posedge clock) begin
  //   if (reset | !l2_receive_data_paused) begin
  //     count_fill_not_ready = 8'h0;
  //   end
  //   else begin
  //     if (l2_receive_data_paused)
  //       count_fill_not_ready = count_fill_not_ready + 1'b1;
  //   end
  // end

  // `ASSERT_ALWAYS_MSG(ERROR_dcache_l2_resp_stalled,clock,reset,(count_fill_not_ready < 200),$sformatf("Pause cycles(%0d) >= 200",count_fill_not_ready))



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////
// tensor_reduce_ap_is_receiver()
///////////////////////////////////
function automatic logic tensor_reduce_ap_is_receiver;
  input logic [`DCACHE_REDUCE_ACTION_RANGE] action;
  input logic [`MAX_MINION_ID_R] minion_id; // global minion id
  input logic [`DCACHE_REDUCE_LEVEL_RANGE] start_level;
  logic  [`MAX_MINION_ID_R] minion_mask;

  minion_mask = ((1 << (start_level+1))-1);
  tensor_reduce_ap_is_receiver = (action == 2'b11 && (minion_id & minion_mask) == '0) || // TensorReduce
                                 (action == 2'b10 && (minion_id & minion_mask) != '0);   // TensorBroadcast
endfunction

endmodule // minion_monitor
