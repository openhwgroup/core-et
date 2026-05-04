// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_ctrl (
  // System signals
  input  logic                                                  clock_aon,
  input  logic                                                  clock_sec,
  input  logic                                                  reset,
  // ID/F0 inputs
  input  logic                                                  id_core_valid,
  input  logic                                                  id_core_thread_id,
  input  logic [`CORE_FCSR_RM_SZ-1:0]                           id_fcsr_rm,
  input  core_vpu_ctrl                                          f0_core_ctrl,
  input  logic [`INST_RANGE]                                    id_core_inst_bits,
  input  vpu_ctrl_sigs_t                                        id_decoder_sigs,
  // Chicken bit clock gate enable
  input  logic                                                  chicken_bit_vputrans,
  // EX/F1 inputs
  input  logic                                                  ex_thread_id,
  input  logic                                                  ex_core_gscing,
  input  logic                                                  ex_core_kill,
  input  [`VPU_LANE_NUM-1:0][1:0][`VPU_DATA_S_SZ-1:0]           ex_regfile_rdata_bypass,
  input  logic [$clog2(`VPU_LANE_NUM)-1:0]                      ex_core_gsc_src,
  input  logic [`XREG_RANGE]                                    ex_core_fromint_data,
  // F2 inputs
  input  logic                                                  f2_core_kill,
  input  logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]          f2_fswizz_rdata,
  // F3 inputs
  input  logic                                                  f3_core_kill,
  input  logic [`VPU_LANE_NUM-1:0]                              f3_regmask_wdata_lane,
  // F4 inputs
  input  logic                                                  f4_core_kill,
  // F8 inputs
  input  logic [`VPU_DATA_S_SZ-1:0]                             f8_wdata_lane0,
  input  logic [`VPU_LANE_NUM-1:0][`CORE_FCSR_FLAG_BITS_SZ-1:0] f8_wflags,
  input  logic [`VPU_LANE_NUM-1:0]                              f8_txfma_comp_res,
  input  logic [`VPU_DATA_RANGE]                                f8_sh_sw_wdata,
  // WB DMEM inputs
  input  logic                                                  wb_dmem_resp_val,
  input  dcache_vpu_resp                                        wb_dmem_resp,
  // ML override inputs
  input  dcache_vpu_scp_resp                                    dcache_scp_resp,
  input  logic [`DCACHE_DATA_RANGE]                             dcache_scp_data,
  input  logic [`DCACHE_DATA_RANGE]                             dcache_tenb_data,
  // Dcache reduce control
  input  dcache_vpu_reduce_ctrl                                 dcache_reduce_ctrl,
  //
  input  logic [`VPU_LANE_NUM-1:0][1:0]                         txfma_trans_dbg,
  // ML override outputs
  output vpu_dcache_ctrl                                        dcache_ctrl,
  // ID outputs
  output vpu_minion_id_ctrl                                     id_core_ctrl,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   id_regfile_raddr1,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   id_regfile_raddr2,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   id_regfile_raddr3,
  output logic [2:0]                                            id_regfile_ren,
  output logic [2:0]                                            id_regfile_thread_id,
  // EX/F1 outputs

  output vpu_input_t                                            ex_req,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   ex_regfile_raddr1,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   ex_regfile_raddr2,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   ex_regfile_raddr3,
  output logic [2:0]                                            ex_regfile_thread_id,
  output vpu_minion_ex_ctrl                                     ex_core_ctrl,
  output logic [`VPU_LANE_NUM-1:0]                              ex_mask_in1,
  output logic                                                  ex_tena_regfile_bypass_en,
  output logic                                                  ex_tenb_regfile_bypass_en,
  output vpu_bypass_force_ctrl                                  ex_bypass_force_ctrl,
  output logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]           ex_tima_valid,
  output logic                                                  ex_tima_valid_unqual,
  output logic                                                  ex_tima_sa,
  output logic                                                  ex_tima_sb,
  output logic [`VPU_TENB_ADDR]                                 ex_tima_tenb_raddr,
  output logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]          ex_tena_rf_rdata,
  output logic [`VPU_DATA_S_SZ-1:0]                             ex_tena_rf_tima_rdata,
  output logic                                                  ex_sh_sw_valid,
  output logic                                                  ex_sh_sw_clock_valid,
  output logic [`VPU_LANE_NUM-1:0]                              ex_txfma_valid,
  output logic                                                  ex_txfma_clock_valid,
  output logic                                                  ex_rom_valid,
  output logic                                                  ex_rom_clock_valid,
  // F2 outputs
  output logic                                                  f2_tima_ren3,
  output logic [`VPU_TENC_ADDR]                                 f2_tima_tenc_raddr,
  output vpu_minion_tag_ctrl                                    f2_core_ctrl,
  output logic                                                  f2_tenb_regfile_wen_l,
  output logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]          f2_fswizz_rdata1,
  output logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]          f2_fswizz_rdata2,
  output logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]          f2_fswizz_rdata3,
  // F3 outputs
  output logic [`VPU_DATA_SZ-1:0]                               f3_tenb_regfile_wdata_l,
  output logic                                                  f3_tenb_regfile_wen_l,
  output logic [`VPU_TENB_ADDR]                                 f3_tenb_regfile_waddr_l,
  output vpu_minion_mem_ctrl                                    f3_core_ctrl,
  output logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]           f3_tima_tenc_wen,
  output logic [`VPU_TENC_ADDR]                                 f3_tima_tenc_waddr,
  output logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]           f3_tima_rf_wen,
  output logic                                                  f3_thread_id,
  output logic [`VPU_LANE_NUM-1:0]                              f3_regfile_wmask,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f3_regfile_waddr,
  output logic                                                  f3_bypass_clk_en,
  output logic                                                  f3_data_vector,
  // F4 outputs
  output logic                                                  f4_thread_id,
  output logic [`VPU_LANE_NUM-1:0]                              f4_regfile_wmask,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f4_regfile_waddr,
  output logic                                                  f4_regfile_thrid_l,
  output logic [`VPU_LANE_NUM-1:0][1:0]                         f4_regfile_wmask_l,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f4_regfile_waddr_l,
  output logic [`VPU_DATA_SZ-1:0]                               f4_regfile_wdata_l,
  output logic                                                  f4_regfile_wen_l,
  output logic                                                  f4_bypass_clk_en,
  // F5 outputs
  output logic                                                  f5_thread_id,
  output logic [`VPU_LANE_NUM-1:0]                              f5_regfile_wmask,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f5_regfile_waddr,
  output logic                                                  f5_bypass_clk_en,
  // F6 outputs
  output logic                                                  f6_thread_id,
  output logic [`VPU_LANE_NUM-1:0]                              f6_regfile_wmask,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f6_regfile_waddr,
  output logic                                                  f6_bypass_clk_en,
  // F7 outputs
  output logic                                                  f7_thread_id,
  output logic [`VPU_LANE_NUM-1:0]                              f7_regfile_wmask,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f7_regfile_waddr,
  output logic                                                  f7_bypass_clk_en,
  // F8 outputs
  output logic                                                  f8_thread_id,
  output logic [`VPU_LANE_NUM-1:0]                              f8_regfile_wmask,
  output logic [`VPU_LANE_NUM-1:0]                              f8_regfile_wen,
  output logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f8_regfile_waddr,
  output logic                                                  f8_data_vector,
  output logic                                                  f8_txfma_op,
  // wb outputs
  output vpu_minion_wb_ctrl                                     wb_core_ctrl,
  // Performance counters
  output logic [`CSR_NR_EVENTS_VPU-1:0]                         io_events,
  // Signals going to debug monitor
  output logic [`NEIGH_DEBUG_MATCH_WIDTH-1:0]                   vpu_dbg_match,
  output logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]                  vpu_dbg_filter,
  output logic [4:0] [`NEIGH_DEBUG_DATA_WIDTH-1:0]              vpu_dbg_data
);



  ////////////////////////////////////////////////////////////////////////////////
  // Internal logic
  ////////////////////////////////////////////////////////////////////////////////

  logic [`INST_RANGE]                                               id_core_inst;
  logic [`INST_RANGE]                                               id_core_inst_next;
  logic                                                             id_core_valid_int;
  logic [`FREG_ADDR_RANGE]                                          id_ra1;
  logic [`FREG_ADDR_RANGE]                                          id_ra2;
  logic [`FREG_ADDR_RANGE]                                          id_ra3;
  logic                                                             id_tena_regfile_bypass_en;
  logic                                                             id_tenb_regfile_bypass_en;
  vpu_input_t                                                       id_vpu_ctrl;
  vpu_input_t                                                       ex_vpu_ctrl;
  logic                                                             id_core_thread_id_int;
  logic                                                             id_trans_insert;
  vpu_minion_scoreboard                                             id_scoreboard;
  logic                                                             id_trans_insert_en;
  logic                                                             id_trans_insert_en_next;
  logic [`INST_RANGE]                                               id_trans_insert_inst_next;
  logic                                                             id_trans_thread_id;
  logic                                                             id_trans_busy;
  trans_state                                                       trans_st;
  logic [`INST_RANGE]                                               id_core_inst_int;
  vpu_ctrl_sigs_t                                                   id_decoder_sigs_int;
  vpu_ctrl_sigs_t                                                   id_decoder_sigs_sel;
  logic                                                             id_tfma_enabled;
  logic                                                             id_tfma_wrrf_enabled;
  logic                                                             id_tquant_enabled;
  logic                                                             id_reduce_enabled;
  vpu_ml_load_ctrl                                                  id_tensorfma_load_ctrl;
  vpu_ml_load_ctrl                                                  id_reduce_load_ctrl;
  logic                                                             id_ml_load_ctrl_pre_tena_wen;
  logic                                                             id_ml_load_ctrl_pre_tenb_wen;
  logic                                                             id_ml_inst_en;
  logic                                                             id_ml_inst_en_next;
  logic                                                             id_ml_inst_fma_en;
  logic                                                             id_ml_tena_quant_en;
  logic [`INST_RANGE]                                               id_ml_inst_next;
  logic [`VPU_LANE_NUM-1:0]                                         id_ml_mask_next;
  logic                                                             id_use_prev_sigs;
  logic                                                             id_core_inst_en; // Enable next instruction update
  trans_scoreboard_slot [`TRANS_SCOREBOARD_SLOTS-1:0]               id_trans_scoreboard_slots;
  vpu_bypass_force_ctrl                                             id_bypass_force_ctrl;
  logic [`VPU_LANE_NUM-1:0]                                         id_ml_mask;
  logic                                                             id_inst_from_core;
  logic [`VPU_LANE_NUM-1:0]                                         ex_mask_rf0;
  logic [`FREG_ADDR_RANGE]                                          ex_ra1;
  logic [`FREG_ADDR_RANGE]                                          ex_ra2;
  logic [`FREG_ADDR_RANGE]                                          ex_ra3;
  logic [`VPU_TENA_ADDR]                                            ex_ra1_ten;
  logic [`VPU_TENA_ADDR]                                            ex_tima_tena_raddr;
  logic [`VPU_TENB_ADDR]                                            ex_ra2_ten;
  logic                                                             ex_ctrl_valid_kill;
  logic                                                             ex_ctrl_valid;
  logic                                                             ex_ctrl_valid_qual;
  logic                                                             ex_ctrl_killed;
  logic [$clog2(`VPU_LANE_NUM)-1:0]                                 ex_lane_idx;
  logic                                                             ex_wen;
  logic                                                             ex_thread_id_int;
  logic                                                             ex_trans_thread_id;
  logic                                                             ex_illegal_rm;
  logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]                     ex_store_data_lane;
  logic                                                             ex_mask_valid;
  logic [`INST_RANGE]                                               ex_core_inst;
  logic                                                             ex_tensorfma_en;
  logic                                                             ex_mova_mx;
  logic                                                             ex_ml_inst_val;
  logic                                                             ex_use_prev_sigs;
  logic [`VPU_LANE_NUM-1:0]                                         ex_fma_gate_mask;
  logic [`VPU_LANE_NUM-1:0]                                         ex_ml_mask;
  logic [`VPU_LANE_NUM-1:0]                                         ex_ml_gate_mask;
  logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]                      ex_tima_mask;
  logic [`VPU_LANE_NUM-1:0]                                         ex_ml_mask_to_f2;
  logic [`VPU_LANE_NUM-1:0]                                         ex_tensor_zero_mask;
  logic [`VPU_LANE_NUM-1:0]                                         ex_tensorb_zero_mask_tima0;
  logic [`VPU_LANE_NUM-1:0]                                         ex_tensorb_zero_mask_tima1;
  logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]                      ex_tensor_zero_mask_ext;
  logic [`VPU_LANE_NUM-1:0]                                         ex_tensora_zero_mask_mux;
  logic [`VPU_TENA_SIZE-1:0][`VPU_LANE_NUM-1:0]                     ex_tensora_zero_mask;
  logic [`VPU_TENTMP_SIZE-1:0][`VPU_LANE_NUM-1:0]                   ex_tensortmp_zero_mask;
  logic [`VPU_TENB_SIZE-1:0][`VPU_LANE_NUM-1:0]                     ex_tensorb_zero_mask;
  logic [`VPU_TENA_SIZE-1:0][`VPU_LANE_NUM-1:0]                     ex_tensora_zero_mask_next;
  logic [`VPU_TENTMP_SIZE-1:0][`VPU_LANE_NUM-1:0]                   ex_tensortmp_zero_mask_next;
  logic [`VPU_TENB_SIZE-1:0][`VPU_LANE_NUM-1:0]                     ex_tensorb_zero_mask_next;
  logic                                                             ex_txfma_valid_all;
  logic                                                             ex_store_data_lane_en;
  logic [`VPU_TENA_ADDR]                                            ex_tena_raddr;
  logic                                                             ex_tima_ren3;
  logic                                                             ex_tima_last_pass;
  logic                                                             ex_tentmp_ren;
  logic                                                             ex_tquant_ren;
  logic [`VPU_TENTMP_ADDR]                                          ex_tentmp_raddr;
  logic [`VPU_DATA_S_SZ-1:0]                                        ex_tena_rf_rdata_pre;
  logic [`VPU_DATA_S_SZ-1:0]                                        ex_tentmp_rf_rdata;
  logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]                     ex_tquant_rdata;
  logic                                                             wb_dmem_resp_val_masked;
  logic                                                             wb_dmem_resp_is_gather;
  logic [`VPU_CTRL_SIGS_DTYPE_SZ-1:0]                               wb_regfile_dtype_l;
  logic                                                             f2_ctrl_valid_qual;
  logic                                                             f2_ctrl_killed;
  logic                                                             f2_ctrl_killed_prev;
  logic                                                             f2_thread_id;
  logic [$clog2(`VPU_LANE_NUM)-1:0]                                 f2_lane_idx;
  logic                                                             f2_wen;
  logic [`FREG_ADDR_RANGE]                                          f2_regfile_waddr;
  logic                                                             f2_ml_inst_val;   // Doing an TensorFMA generated instruction
  logic                                                             f2_kill;
  logic                                                             f3_kill;
  logic                                                             f4_kill;
  logic [$clog2(`VPU_LANE_NUM)-1:0]                                 f2_gsc_src;
  logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]                     f2_store_data_lane;
  logic [`VPU_DATA_S_SZ-1:0]                                        f2_scatter_data;
  logic [`VPU_CORE_VPU_STORE_DATA_SZ-1:0]                           f2_store_data;
  logic [`VPU_LANE_NUM-1:0]                                         f2_mask_rf0;
  logic                                                             f2_ctrl_valid;
  logic                                                             f2_mask_wen;
  logic                                                             f2_mova_mx;
  logic                                                             f2_ml_use_prev_sigs;
  logic [`VPU_LANE_NUM-1:0]                                         f2_ml_mask;
  logic                                                             f2_tena_regfile_wen_l;
  vpu_input_t                                                       f2_req;
  logic [`VPU_DATA_S_SZ-1:0]                                        f2_packreph0;
  logic [`VPU_DATA_S_SZ-1:0]                                        f2_packreph1;
  logic [`VPU_DATA_S_SZ-1:0]                                        f2_packreph2;
  logic [`VPU_DATA_S_SZ-1:0]                                        f2_packreph3;
  logic [`VPU_DATA_S_SZ-1:0]                                        f2_packrepb0;
  logic [`VPU_DATA_S_SZ-1:0]                                        f2_packrepb1;
  logic                                                             f2_tentmp_wen_early;
  logic [`VPU_LANE_NUM-1:0]                                         f2_regfile_wmask;
  logic                                                             f3_ctrl_valid_qual;
  logic                                                             f3_ctrl_killed;
  logic                                                             f3_ctrl_killed_prev;
  logic                                                             f3_wen;
  logic [$clog2(`VPU_LANE_NUM)-1:0]                                 f3_lane_idx;
  logic                                                             f3_regfile_wen_l;
  logic [`VPU_LANE_NUM-1:0][1:0]                                    f3_regfile_wmask_l;
  logic [$clog2(`VPU_REGFILE_NUM)-1:0]                              f3_regfile_waddr_l;
  logic [`VPU_DATA_SZ-1:0]                                          f3_regfile_wdata_l;
  logic                                                             f3_regfile_thrid_l;
  vpu_ctrl_sigs_t                                                   f3_ctrl_sigs_to_f4;
  vpu_ctrl_sigs_t                                                   f3_ctrl_sigs;
  logic                                                             f3_ctrl_valid;
  logic                                                             f3_ml_inst_val;
  logic                                                             f3_mova_mx;
  logic [`VPU_LANE_NUM-1:0]                                         f2_regfile_wmask_to_f3;
  logic [`VPU_LANE_NUM-1:0]                                         f3_regfile_wmask_to_f4;
  logic [`VPU_LANE_NUM-1:0]                                         f4_regfile_wmask_to_f5;
  logic                                                             f3_ml_use_prev_sigs;
  logic [`VPU_LANE_NUM-1:0]                                         f3_zero_data_a;
  logic [`VPU_LANE_NUM-1:0]                                         f3_zero_data_tmp;
  logic [`VPU_LANE_NUM-1:0]                                         f3_zero_data_b;
  logic                                                             f3_tenb_pass;
  logic                                                             f3_tena_regfile_wen_l;
  logic                                                             f3_tena_regfile_waddr_l;
  logic [`VPU_DATA_S_SZ-1:0]                                        f3_tena_regfile_wdata_l;
  logic [`VPU_DATA_S_SZ*2-1:0]                                      f3_tentmp_regfile_wdata_l;
  logic [`VPU_DATA_S_SZ-1:0]                                        f3_tentmp_msb_regfile_wdata_l;
  logic                                                             f3_tentmp_wen;
  logic [`VPU_TENTMP_ADDR]                                          f3_tentmp_waddr;
  logic                                                             f4_wen;
  logic [$clog2(`VPU_LANE_NUM)-1:0]                                 f4_lane_idx;
  logic                                                             f4_ctrl_valid_qual;
  logic                                                             f4_ctrl_killed;
  logic                                                             f4_ctrl_killed_prev;
  logic                                                             f4_ctrl_valid;
  vpu_ctrl_sigs_t                                                   f4_ctrl_sigs;
  logic                                                             f4_ml_inst_val;
  logic                                                             f4_mova_mx;
  logic                                                             f4_ml_use_prev_sigs;
  logic                                                             f5_ml_inst_val;
  logic                                                             f5_ctrl_killed;
  logic                                                             f5_core_kill;
  vpu_ctrl_sigs_t                                                   f5_ctrl_sigs;
  logic                                                             f5_ctrl_valid_qual;
  logic                                                             f5_wen;
  logic [$clog2(`VPU_LANE_NUM)-1:0]                                 f5_lane_idx;
  logic                                                             f5_mova_mx;
  logic                                                             f5_ml_use_prev_sigs;
  logic                                                             f6_thread_id_to_f7;
  logic                                                             f6_ctrl_killed;
  logic [`VPU_LANE_NUM-1:0]                                         f6_regfile_wmask_to_f7;
  logic                                                             f6_ml_inst_val;
  logic                                                             f6_ml_use_prev_sigs;
  logic                                                             f6_wen;
  logic [$clog2(`VPU_LANE_NUM)-1:0]                                 f6_lane_idx;
  logic                                                             f6_ctrl_valid_qual;
  vpu_ctrl_sigs_t                                                   f6_ctrl_sigs;
  logic                                                             f6_core_kill;
  logic                                                             f6_trans_thread_id;
  logic [`VPU_LANE_NUM-1:0]                                         f6_trans_wmask;
  logic                                                             f6_trans_wen_ctrl_en;
  logic [`VPU_LANE_NUM-1:0]                                         f7_regfile_wen;
  logic                                                             f7_ml_use_prev_sigs;
  logic                                                             f7_wen;
  logic [$clog2(`VPU_LANE_NUM)-1:0]                                 f7_lane_idx;
  logic                                                             f7_ctrl_valid_qual;
  logic                                                             f7_ctrl_killed;
  vpu_ctrl_sigs_t                                                   f7_ctrl_sigs;
  vpu_ctrl_sigs_t                                                   f8_ctrl_sigs;
  logic                                                             f8_ctrl_valid_qual;
  logic                                                             f8_ctrl_killed;
  logic                                                             f8_wen;
  logic [$clog2(`VPU_LANE_NUM)-1:0]                                 f8_lane_idx;
  logic [`CORE_FCSR_FLAG_BITS_SZ-1:0]                               f8_fcsr_flags;
  logic [`VPU_MASK_SCOREBOARD_SIZE-1:0]                             id_scoreboard_mask_valid;
  minion_mreg_dest [`VPU_MASK_SCOREBOARD_SIZE-1:0]                  id_scoreboard_mask_dest;
  logic [`VPU_DATA_S_SZ-1:0]                                        f8_toint_data;
  logic [`XREG_RANGE]                                               f8_toint_mask_rf;
  logic [`VPU_LANE_NUM-1:0]                                         f8_regmask_store;
  logic [`VPU_LANE_NUM-1:0]                                         f8_regmask_comp_res;
  logic                                                             f8_regmask_from_txfma;
  logic                                                             f8_trans_wen_ctrl_en;
  logic                                                             f8_trans_fcsr_en;
  logic [`CORE_FCSR_FLAG_BITS_SZ-1:0]                               f8_wflags_all_lanes;
  logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]                     ex_gsc_fs;
  logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]                     ex_gsc_fs_next;
  logic                                                             ex_tena_rdata_mask;
  logic [10:0]                                                      vpu_tensorquant_debug;
  logic [24:0]                                                      vpu_tensorreduce_debug;
  logic [24:0]                                                      vpu_tensorfma_debug;
  logic                                                             ex_txfma_valid_all_en;
  logic                                                             f2_txfma_valid_all_en;



  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // ID/F0 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // Deals with multicycle instructions: TXFMA lanes 2..7 are disabled and
  // a TXFMA operation shows in the pipeline.
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK        RST    EN                              DOUT                 DIN                                                                           DEF
  `EN_FF(clock_aon, id_core_inst_en, id_core_inst, id_core_inst_next)
  `FF(clock_aon, id_inst_from_core, !(id_ml_inst_en_next | id_trans_insert_en_next))
  `EN_FF(clock_aon, id_ml_inst_en_next, id_ml_mask, id_ml_mask_next)



  always_comb begin
    // Selects the instruction for the next ID pipeline
    if (id_ml_inst_en_next)
      id_core_inst_next = id_ml_inst_next;
    else if (id_trans_insert_en_next)
      id_core_inst_next = id_trans_insert_inst_next;
    else
      id_core_inst_next = id_core_inst_int;

    //inject u-instruct during ml and trans ops
    id_core_inst_en = id_ml_inst_en_next || id_trans_insert_en_next;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // VPU decoder
  ////////////////////////////////////////////////////////////////////////////////
  vpu_uinst_decoder uinst_decoder (
    .id_inst(id_core_inst),
    .id_sigs(id_decoder_sigs_int)
  );


  ////////////////////////////////////////////////////////////////////////////////
  // F0 control logic
  ////////////////////////////////////////////////////////////////////////////////

  // regfile address extracted from instruction fields ra1,ra2,ra3
  always_comb begin
    id_vpu_ctrl = '0;

    //select decoder sigs from frontend, reg or internal
    if (id_inst_from_core)
      id_decoder_sigs_sel = id_decoder_sigs;            //frontend decoder
    else
      id_decoder_sigs_sel = id_decoder_sigs_int;        // internal decoder

    //assign default control sigs coming from decoder
    id_vpu_ctrl.sigs = id_decoder_sigs_sel;

    //assign ex ra id read address when there is no gsc in progress
    id_ra1 = ex_ra1;
    id_ra2 = ex_ra2;
    id_ra3 = ex_ra3;

    //assign id read address when there is no gsc in progress
    if (!ex_core_gscing) begin
      // ren1
      if (id_decoder_sigs_sel.ren1) begin
        if (!id_decoder_sigs_sel.swap12) begin
          id_ra1 = id_core_inst_int[`VPU_INST_REN1_RA_SEL];
        end

        if (id_decoder_sigs_sel.swap12) begin
          id_ra2 = id_core_inst_int[`VPU_INST_REN1_RA_SEL];
          id_vpu_ctrl.sigs.ren1 = 0;
          id_vpu_ctrl.sigs.ren2 = 1;
        end

        if (id_decoder_sigs_sel.swap13) begin
          id_ra3 = id_core_inst_int[`VPU_INST_REN1_RA_SEL];
          id_vpu_ctrl.sigs.ren1 = (id_decoder_sigs_sel.cmd == `VPU_TRANS_EXP_FRAC);
          id_vpu_ctrl.sigs.ren3 = 1;
        end
      end

      // ren2
      if (id_decoder_sigs_sel.ren2) begin
        if (id_decoder_sigs_sel.swap12) begin
          id_ra1 = id_core_inst_int[`VPU_INST_REN2_RA_SEL];
          id_vpu_ctrl.sigs.ren1 = 1;
          id_vpu_ctrl.sigs.ren2 = 0;
        end

        if (id_decoder_sigs_sel.swap23) begin
          id_ra3 = id_core_inst_int[`VPU_INST_REN2_RA_SEL];
          id_vpu_ctrl.sigs.ren2 = 0;
          id_vpu_ctrl.sigs.ren3 = 1;
        end

        if (!id_decoder_sigs_sel.swap12 && !id_decoder_sigs_sel.swap23)
          id_ra2 = id_core_inst_int[`VPU_INST_REN2_RA_SEL];
      end

      // ren3
      if (id_decoder_sigs_sel.ren3) begin
        id_ra3 = id_core_inst_int[`VPU_INST_REN3_RA_SEL];
      end

      // fsetm
      if (id_decoder_sigs_sel.cvt && (id_decoder_sigs_sel.cmd == `VPU_FCMD_FSETM)) begin
        id_ra1 = id_core_inst_int[`VPU_INST_MASK_FRA_SEL];
      end

      // NR2
      if (id_decoder_sigs_sel.cmd == `VPU_FCMD_NR2_FRCPFXP) begin
        id_ra3 = id_core_inst_int[`VPU_INST_REN2_RA_SEL];
      end

      // regmask extension
      if (id_decoder_sigs_sel.maskop) begin
        id_ra1 = {2'b0, id_core_inst_int[`VPU_INST_MASK_MRA1_SEL]};
        id_ra2 = {2'b0, id_core_inst_int[`VPU_INST_MASK_MRA2_SEL]};
      end

      // use rd as ra1 (scatter and scatter on 32-byte blocks) or ra3
      if (id_decoder_sigs_sel.rend) begin
        if (id_decoder_sigs_sel.ren3)
          id_ra3 = id_core_inst_int[`VPU_INST_RD_SEL];
        else
          id_ra1 = id_core_inst_int[`VPU_INST_RD_SEL];
      end

    end

    // Type
    id_vpu_ctrl.typ = id_use_prev_sigs ? '0
                    :                    id_core_inst_int[`VPU_INST_TYP_SEL];



    // Rounding mode, sel rm from CSR when ex_core_inst[`VPU_INST_RM_SEL] == '7'
    id_vpu_ctrl.rm = !id_vpu_ctrl.sigs.round ?                                                     RTZ                                 // Round Towards Zero if no round flag
                   : (id_core_inst_int[`VPU_INST_RM_SEL] == `VPU_RM_DYN | id_vpu_ctrl.sigs.gcvt) ? id_fcsr_rm                          // Dynamic rm
                   :                                                                               id_core_inst_int[`VPU_INST_RM_SEL]; //instruction

    // Immediate operand
    id_vpu_ctrl.imm = id_use_prev_sigs                                                     ? '0
                    : (id_vpu_ctrl.sigs.cmd == `VPU_FCMD_FBCI)                             ? id_core_inst_int[31:12]
                    : (id_vpu_ctrl.sigs.cmd == `VPU_FCMD_MPOPC_RAST)                       ? {16'b0, id_core_inst_int[24:23], id_core_inst_int[19:18]}
                    : (id_vpu_ctrl.sigs.cmd == `VPU_FCMD_FSWIZZ | id_vpu_ctrl.sigs.maskop) ? {12'b0, id_core_inst_int[24:20], id_core_inst_int[14:12]}
                    :                                                                        {10'b0, id_core_inst_int[31:27], id_core_inst_int[24:20]};
  end

  assign id_regfile_raddr1 = id_ra1;
  assign id_regfile_raddr2 = id_ra2;
  assign id_regfile_raddr3 = id_ra3;

  //force to bypass op1/op2 operands from tensor RF's if the opertion is not a reduce
  assign id_tena_regfile_bypass_en = id_ml_inst_fma_en & (id_vpu_ctrl.sigs.cmd != `VPU_FCMD_XOR & id_vpu_ctrl.sigs.cmd != `VPU_FCMD_SETZ)
                                   | id_ml_tena_quant_en;
  assign id_tenb_regfile_bypass_en = id_ml_inst_fma_en & (id_vpu_ctrl.sigs.cmd != `VPU_FCMD_XOR & id_vpu_ctrl.sigs.cmd != `VPU_FCMD_SETZ);

  //tensor fma read data from tenc
  assign id_regfile_ren[0] = (id_core_valid_int & (id_vpu_ctrl.sigs.ren1 | id_vpu_ctrl.sigs.rend) & !id_tena_regfile_bypass_en) // Regular accesses (TFMA disables it)
                           | ex_core_gscing;                                                                                    // Gather/Scatter
  assign id_regfile_ren[1] = (id_core_valid_int & id_vpu_ctrl.sigs.ren2 & !id_tenb_regfile_bypass_en)                           // Regular accesses (TFMA disables it)
                           | ex_core_gscing;                                                                                    // Gather Scatter
  assign id_regfile_ren[2] = (id_core_valid_int & id_vpu_ctrl.sigs.ren3)                                                        // Regular accesses (TFMA disables it)
                           | ex_core_gscing;                                                                                    // Gather Scatter

  // id core valid, override inputs from core by the TensorFMA if enabled
  always_comb begin
    id_core_valid_int     = id_core_valid;
    id_core_inst_int      = id_inst_from_core ? id_core_inst_bits : id_core_inst;
    id_core_thread_id_int = id_core_thread_id;

    if (id_ml_inst_en)
      id_core_valid_int = 1'b1;
    else if (id_trans_insert_en) begin
      id_core_valid_int     = 1'b1;
      id_core_thread_id_int = id_trans_thread_id;
    end
  end

  // Override thread ID per RF read port
  assign id_regfile_thread_id = id_ml_inst_fma_en                                                       ? {1'b0, 1'b0, 1'b0} // TFMA (always from thread0)
                              : id_ml_inst_en                                                           ? {1'b0, 1'b0, 1'b0} // Reduce (always from thread0)
                              : ex_core_gscing                                                          ? {3{ex_thread_id_int}}
                              :                                                                           {3{id_core_thread_id_int}};


  // Use previous configuration if current one is the same and all lanes received a first valid
  assign id_use_prev_sigs = id_ml_inst_fma_en & (id_vpu_ctrl.sigs == ex_req.sigs) && (ex_req.rm == id_vpu_ctrl.rm);


  ////////////////////////////////////////////////////////////////////////////////
  // f0 core control signals
  ////////////////////////////////////////////////////////////////////////////////
  always_comb begin
    id_trans_insert = id_trans_insert_en;

    //trans scoreboard
    for (integer i = 0; i < `TRANS_SCOREBOARD_SLOTS; i++) begin
      id_scoreboard.fp_valid[2 * i]    = id_trans_scoreboard_slots[i].rs_valid;
      id_scoreboard.fp_dest[2 * i]     = id_trans_scoreboard_slots[i].rs;

      id_scoreboard.fp_valid[2 * i + 1]  = id_trans_scoreboard_slots[i].rd_valid;
      id_scoreboard.fp_dest[2 * i + 1]   = id_trans_scoreboard_slots[i].rd;
    end

    //txfma operation requires scoreboard at f5 and f6
    //the entries happen just after trans scoreboard
    //f5
    id_scoreboard.fp_valid[`VPU_TRANS_SCOREBOARD_SIZE]            = f5_ctrl_valid_qual & f5_ctrl_sigs.txfma;
    id_scoreboard.fp_dest[`VPU_TRANS_SCOREBOARD_SIZE].fp          = 1'b1;
    id_scoreboard.fp_dest[`VPU_TRANS_SCOREBOARD_SIZE].addr        = f5_regfile_waddr & {($clog2(`VPU_REGFILE_NUM)) {f5_ctrl_sigs.txfma}};
    id_scoreboard.fp_dest[`VPU_TRANS_SCOREBOARD_SIZE].thread_id   = f5_thread_id & f5_ctrl_sigs.txfma;
    //f6
    id_scoreboard.fp_valid[`VPU_TRANS_SCOREBOARD_SIZE + 1]          = f6_ctrl_valid_qual & f6_ctrl_sigs.txfma;
    id_scoreboard.fp_dest[`VPU_TRANS_SCOREBOARD_SIZE + 1].fp        = 1'b1;
    id_scoreboard.fp_dest[`VPU_TRANS_SCOREBOARD_SIZE + 1].addr      = f6_regfile_waddr & {($clog2(`VPU_REGFILE_NUM)) {f6_ctrl_sigs.txfma}};
    id_scoreboard.fp_dest[`VPU_TRANS_SCOREBOARD_SIZE + 1].thread_id = f6_thread_id & f6_ctrl_sigs.txfma;
  end


  // toint scoreboard
  always_comb begin
    //f5
    id_scoreboard.toint_valid[0]          = f5_wen & f5_ctrl_sigs.toint;
    id_scoreboard.toint_dest[0].fp        = 1'b0;
    id_scoreboard.toint_dest[0].addr      = f5_regfile_waddr & {($clog2(`VPU_REGFILE_NUM)) {f5_ctrl_sigs.toint}};
    id_scoreboard.toint_dest[0].thread_id = f5_thread_id;
    //f6
    id_scoreboard.toint_valid[1]          = f6_wen & f6_ctrl_sigs.toint;
    id_scoreboard.toint_dest[1].fp        = 1'b0;
    id_scoreboard.toint_dest[1].addr      = f6_regfile_waddr & {($clog2(`VPU_REGFILE_NUM)) {f6_ctrl_sigs.toint}};
    id_scoreboard.toint_dest[1].thread_id = f6_thread_id;
    //f7
    id_scoreboard.toint_valid[2]          = f7_wen & f7_ctrl_sigs.toint;
    id_scoreboard.toint_dest[2].fp        = 1'b0;
    id_scoreboard.toint_dest[2].addr      = f7_regfile_waddr & {($clog2(`VPU_REGFILE_NUM)) {f7_ctrl_sigs.toint}};
    id_scoreboard.toint_dest[2].thread_id = f7_thread_id;
    //f8
    id_scoreboard.toint_valid[3]          = f8_wen & f8_ctrl_sigs.toint;
    id_scoreboard.toint_dest[3].fp        = 1'b0;
    id_scoreboard.toint_dest[3].addr      = f8_regfile_waddr & {($clog2(`VPU_REGFILE_NUM)) {f8_ctrl_sigs.toint}};
    id_scoreboard.toint_dest[3].thread_id = f8_thread_id;
  end

  // mask scoreboard
  always_comb begin
    id_scoreboard.mask_valid = id_scoreboard_mask_valid;
    id_scoreboard.mask_dest  = id_scoreboard_mask_dest;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // id core ctrl
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    id_core_ctrl = '0;
    id_core_ctrl.tfma_wrrf_enabled = id_tfma_wrrf_enabled;
    id_core_ctrl.tfma_enabled      = id_tfma_enabled;
    id_core_ctrl.tquant_enabled    = id_tquant_enabled;
    id_core_ctrl.reduce_enabled    = id_reduce_enabled;
    id_core_ctrl.trans_busy        = id_trans_busy;
    id_core_ctrl.id_trans_insert   = id_trans_insert;
    id_core_ctrl.scoreboard        = id_scoreboard;
    id_core_ctrl.fflags_stall      = f2_ctrl_valid | f3_ctrl_valid | f4_ctrl_valid;
  end


  ////////////////////////////////////////////////////////////////////////////////
  // bypass force control
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    case (id_vpu_ctrl.sigs.cmd)
      //                                                  txfma->in0
      //                                                  |   txfma->in1
      //                                                  |   |   txfma->in2
      //                                                  |   |   |   shsw->in1
      //                                                  |   |   |   |   txfma->in2
      //                                                  |   |   |   |   |
      //                                                  |   |   |   |   |
      //                                                  |   |   |   |   |
      `VPU_TRANS_RCP_FMA2:      id_bypass_force_ctrl = {`Y, `N, `N, `N, `N};
`ifdef ENABLE_EXTRA_TRANS
      `VPU_TRANS_RSQRT_FMA2:    id_bypass_force_ctrl = {`Y, `N, `N, `N, `N};
`endif
      `VPU_TRANS_LOG_FMA1:      id_bypass_force_ctrl = {`N, `N, `Y, `N, `N};
      `VPU_TRANS_LOG_FMA2:      id_bypass_force_ctrl = {`Y, `N, `N, `N, `Y};
      `VPU_TRANS_LOG_MUL:       id_bypass_force_ctrl = {`Y, `N, `N, `Y, `N};
      `VPU_TRANS_EXP_RR:        id_bypass_force_ctrl = {`N, `Y, `N, `N, `N};
      `VPU_TRANS_EXP_FMA1:      id_bypass_force_ctrl = {`N, `N, `N, `N, `N};
      `VPU_TRANS_EXP_FMA2:      id_bypass_force_ctrl = {`Y, `N, `N, `N, `N};
`ifdef ENABLE_EXTRA_TRANS
      `VPU_TRANS_SIN_TRANSFORM: id_bypass_force_ctrl = {`Y, `N, `Y, `N, `N};
      `VPU_TRANS_SIN_RR:        id_bypass_force_ctrl = {`N, `Y, `N, `N, `N};
      `VPU_TRANS_SIN_P2:        id_bypass_force_ctrl = {`Y, `N, `N, `N, `N};
      `VPU_TRANS_SIN_P3:        id_bypass_force_ctrl = {`Y, `N, `N, `N, `N};
`endif
      `VPU_FCMD_NR2_FRCPFXP:    id_bypass_force_ctrl = {`Y, `N, `N, `N, `N};
      default:                  id_bypass_force_ctrl = {`N, `N, `N, `N, `N};
    endcase
  end




  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // EX/F1 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  rst_en_ff #(
    .width($bits({id_ml_inst_en, id_core_valid_int}))
  ) F1_CTRL_RST_EN_FF (
    .clock(clock_aon),
    .reset(reset),
    .en   (!ex_core_gscing),
    .D    ({id_ml_inst_en, id_core_valid_int}),
    .Q    ({ex_ml_inst_val, ex_ctrl_valid})
  );

  en_ff #(
    .width($bits({id_ra1, id_ra2, id_ra3, id_core_inst_int, id_ml_mask, id_ml_inst_fma_en,
         id_tena_regfile_bypass_en, id_tenb_regfile_bypass_en}))
  ) F1_CTRL_EN_FF (
    .clock(clock_aon),
    .en   (id_core_valid_int),
    .D    ({id_ra1, id_ra2, id_ra3, id_core_inst_int, id_ml_mask, id_ml_inst_fma_en, id_tena_regfile_bypass_en, id_tenb_regfile_bypass_en}),
    .Q    ({ex_ra1, ex_ra2, ex_ra3, ex_core_inst, ex_ml_mask, ex_tensorfma_en, ex_tena_regfile_bypass_en, ex_tenb_regfile_bypass_en})
  );

  // Some signals in "f2_req.sigs" are used to stall, so ensure that previous stages provide a clean signal
  rst_en_ff #(
    .width($bits({id_vpu_ctrl, id_bypass_force_ctrl}))
  ) F1_CTRL_RST_EN_FF_2 (
    .clock(clock_aon),
    .reset(reset),
    .en   (id_core_valid_int & !id_use_prev_sigs),
    .D    ({id_vpu_ctrl, id_bypass_force_ctrl}),
    .Q    ({ex_vpu_ctrl, ex_bypass_force_ctrl})
  );

  ff #(
    .width($bits({id_regfile_thread_id, id_core_valid_int &
         (id_vpu_ctrl.sigs.cmd == `VPU_FCMD_MOVA_M_X)}))
  ) F1_CTRL_FF (
    .clock(clock_aon),
    .D    ({id_regfile_thread_id, id_core_valid_int & (id_vpu_ctrl.sigs.cmd == `VPU_FCMD_MOVA_M_X)}),
    .Q    ({ex_regfile_thread_id, ex_mova_mx})
  );


  // zero mask info
  //         CLK        RST    EN                                                    DOUT                    DIN            DEF
  `EN_FF(clock_sec, f3_tena_regfile_wen_l, ex_tensora_zero_mask, ex_tensora_zero_mask_next)
  `EN_FF(clock_sec, f3_tentmp_wen, ex_tensortmp_zero_mask, ex_tensortmp_zero_mask_next)
  `EN_FF(clock_sec, f3_tenb_regfile_wen_l | dcache_scp_resp.fill_is_tenb, ex_tensorb_zero_mask, ex_tensorb_zero_mask_next)
  `RST_EN_FF(clock_aon, reset, dcache_scp_resp.fill_is_tenb, f3_tenb_pass, ~f3_tenb_pass, 1'b0)


  ////////////////////////////////////////////////////////////////////////////////
  // F1 control logic
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    ex_tensora_zero_mask_next = ex_tensora_zero_mask;
    ex_tensora_zero_mask_next[f3_tena_regfile_waddr_l] = f3_zero_data_a;

    ex_tensortmp_zero_mask_next = ex_tensortmp_zero_mask;
    ex_tensortmp_zero_mask_next[f3_tentmp_waddr] = f3_zero_data_tmp;

    ex_tensorb_zero_mask_next = ex_tensorb_zero_mask;
    ex_tensorb_zero_mask_next[dcache_scp_resp.fill_is_tenb ? { dcache_scp_resp.tenb_line[$clog2(`VPU_TENB_SIZE/2)-1:0], f3_tenb_pass } : f3_tenb_regfile_waddr_l] = f3_zero_data_b;
  end

  //ex request
always_comb begin
  ex_req = ex_vpu_ctrl; 
  ex_req.use_prev_sigs = ex_use_prev_sigs;

    //fromint ren
  if (ex_vpu_ctrl.sigs.fromint) begin
    ex_req.sigs.ren2 = ex_vpu_ctrl.sigs.swap12;
    ex_req.sigs.ren3 = ex_vpu_ctrl.sigs.swap13;
  end
end


// Valid flag after checking that not all masks are 0
assign ex_ctrl_valid_qual = ex_ctrl_valid &&
                       // Instructions that do not depend on mask
                       (!ex_req.sigs.vector || ex_req.sigs.maskop || ex_ml_inst_val ||
                        (ex_req.sigs.cmd==`VPU_FCMD_MVZ_FX || ex_req.sigs.cmd==`VPU_FCMD_MVS_FX || ex_req.sigs.cmd==`VPU_FCMD_FCMOVM) ||
                        // If an instruction that depends on mask has all
                        // 8 lanes masked, vpu_ctrl unit is not enabled
                        ex_req.sigs.vector && (|ex_mask_rf0 | ex_req.sigs.trans)
                       );

  // Instruction control signals
  assign ex_lane_idx = ex_req.sigs.vector ? ex_core_inst[22:20]         //vector
                     :                      '0;                         //scalar

  // Instructions that write to register (except load)
  assign ex_wen = ex_ctrl_valid_qual & ex_req.sigs.wen & !ex_req.sigs.ldst;


  assign ex_illegal_rm = (ex_req.rm >= 5) && ex_req.sigs.round;


  // regfile read signals
  assign ex_regfile_raddr1    = ex_ra1;
  assign ex_regfile_raddr2    = ex_ra2;
  assign ex_regfile_raddr3    = ex_ra3;

  assign ex_ra1_ten = ex_ra1[`VPU_TENA_ADDR];
  assign ex_ra2_ten = ex_ra2[`VPU_TENB_ADDR];

  // forming zero mask for tensor gating
  always_comb begin

    // tenA
    ex_tensora_zero_mask_mux = ex_tentmp_ren ? ex_tensortmp_zero_mask[ex_tentmp_raddr]                                       // TENTMP
                             :                 ex_tensora_zero_mask[ex_tima_valid_unqual ? ex_tima_tena_raddr : ex_ra1_ten]; // TENA

    //tenB-tima0
    ex_tensorb_zero_mask_tima0 = ex_tensorb_zero_mask[ex_tima_valid_unqual ? ex_tima_tenb_raddr : ex_ra2_ten];

    //tenB-tima1
    ex_tensorb_zero_mask_tima1 = (ex_tensorb_zero_mask[ex_tima_valid_unqual ? (ex_tima_tenb_raddr | 'b1) : ex_ra2_ten]);
    //disable zero mask when tima1 is not used
    for (integer l = 0; l < `VPU_LANE_NUM; l++)
      ex_tensorb_zero_mask_tima1[l] |= ~(ex_tima_mask[l][1]);

    //tensor mask from tenA or tenB
    ex_tensor_zero_mask = ex_tensora_zero_mask_mux                                 // TENA
                        | ex_tensorb_zero_mask_tima0 & ex_tensorb_zero_mask_tima1; // TENB
  end


  // tensor gate for fma units. it gets inactive if operation is not fma type
  always_comb begin
    for (integer l = 0; l < `VPU_LANE_NUM; l++) begin
      for (integer t = 0; t < `VPU_NUM_TIMA; t++) begin
        //extend zero mask to tima lanes
        ex_tensor_zero_mask_ext[l][t] = ((ex_tensor_zero_mask >> l) & 1'b1) & (ex_tima_ren3 && ~ex_tima_last_pass);

        //tima valid
        ex_tima_valid[l][t] = ex_tima_valid_unqual & ~ex_tensor_zero_mask_ext[l][t] & ex_tima_mask[l][t];
      end
    end
  end

  always_comb begin
    //txfma
    ex_fma_gate_mask = ex_tensor_zero_mask & {`VPU_LANE_NUM{(ex_req.sigs.cmd == `VPU_FCMD_MADD) && ex_tensorfma_en}};
    ex_ml_gate_mask  = ex_ml_inst_val ? ex_ml_mask : {`VPU_LANE_NUM{1'b1}};
    ex_txfma_valid   = {`VPU_LANE_NUM{ex_txfma_valid_all}} & ~ex_fma_gate_mask & ex_ml_gate_mask;

    //sh sw
    ex_sh_sw_valid = ex_ctrl_valid_qual && ex_req.sigs.shsw;

    ex_ml_mask_to_f2 = ex_txfma_valid_all ? ~ex_fma_gate_mask & ex_ml_mask : ex_ml_mask;
  end

  //detect when all lanes are on to enable use previous sigs
  always_comb begin
    ex_txfma_valid_all_en = f2_txfma_valid_all_en;

    if (f0_core_ctrl.tensorfma_start || (ex_tensorfma_en && (ex_req.sigs != f2_req.sigs)))
      ex_txfma_valid_all_en = 1'b0;

    if (ex_tensorfma_en && (ex_txfma_valid == `VPU_LANE_NUM'hff)) begin
      ex_txfma_valid_all_en = 1'b1;
    end
  end


  // Use previous configuration if current one is the same
  assign ex_use_prev_sigs = ex_tensorfma_en & (ex_req.sigs == f2_req.sigs) & f2_txfma_valid_all_en;


  //store data lane enable ff
  assign ex_store_data_lane_en = ((ex_req.sigs.cmd == `VPU_FCMD_MVZ_FX) | (ex_req.sigs.cmd == `VPU_FCMD_MVS_FX) | ex_req.sigs.ldst) & ex_ctrl_valid_qual;

  // Move RF to int data
  always_comb begin
    for (integer i = 0; i < `VPU_LANE_NUM; i++)
      ex_store_data_lane[i] = ex_regfile_rdata_bypass[i][0];
  end

  //ex ctrl valid kill
  assign ex_ctrl_valid_kill = ex_ctrl_valid && (!ex_core_kill | ex_ml_inst_val | ex_req.sigs.trans);
  assign ex_ctrl_killed     = ex_ctrl_valid && ex_core_kill && !ex_ml_inst_val && !ex_req.sigs.trans;

  //thread if
  assign ex_thread_id_int = ex_req.sigs.trans ? ex_trans_thread_id : ex_thread_id;

  //trans rom valid
  assign ex_rom_valid = ex_ctrl_valid_qual && ex_req.sigs.rom;

  //clock gates en
  assign ex_txfma_clock_valid = ex_ctrl_valid && ex_req.sigs.txfma;
  assign ex_sh_sw_clock_valid = ex_ctrl_valid && ex_req.sigs.shsw;
  assign ex_rom_clock_valid   = ex_ctrl_valid && ex_req.sigs.rom;

  // Functional units enable signals
  assign ex_txfma_valid_all = ex_ctrl_valid_qual && ex_req.sigs.txfma;
  assign ex_mask_valid      = ex_ctrl_valid_qual && ex_req.sigs.maskop;

  ////////////////////////////////////////////////////////////////////////////////
  // ex core control
  ////////////////////////////////////////////////////////////////////////////////

  //     CLK        EN              DOUT       DIN
  `EN_FF(clock_aon, ex_core_gscing, ex_gsc_fs, ex_gsc_fs_next)

  always_comb begin
    for (integer i = 0; i < `VPU_LANE_NUM; i++)
      ex_gsc_fs_next[i] = ex_regfile_rdata_bypass[i][1];
  end

  assign ex_core_ctrl.tointm     = ex_req.sigs.tointm & !ex_req.sigs.shsw;
  // flq/fsq ignore the mask
  assign ex_core_ctrl.ps_mask    = ex_mask_rf0;
  assign ex_core_ctrl.gsc_fs     = ex_gsc_fs[ex_core_gsc_src];
  assign ex_core_ctrl.illegal_rm = ex_illegal_rm;


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F2 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  rst_ff #(
    .width($bits({(ex_ctrl_valid_qual && (!ex_core_kill || ex_ml_inst_val)), ex_ctrl_valid_kill, ex_ctrl_killed, ex_txfma_valid_all_en, (ex_wen && (!ex_core_kill || ex_ml_inst_val))}))
  ) F2_CTRL_RST_FF (
    .clock(clock_sec),
    .reset(reset),
    .D    ({(ex_ctrl_valid_qual && (!ex_core_kill || ex_ml_inst_val || ex_req.sigs.trans)), ex_ctrl_valid_kill, ex_ctrl_killed, ex_txfma_valid_all_en, (ex_wen && (!ex_core_kill || ex_ml_inst_val || ex_req.sigs.trans))}),
    .Q    ({f2_ctrl_valid_qual, f2_ctrl_valid, f2_ctrl_killed_prev, f2_txfma_valid_all_en, f2_wen})
  );

  ff #(
    .width($bits({ex_ml_inst_val, ex_mova_mx}))
  ) F2_CTRL_FF (
    .clock(clock_sec),
    .D    ({ex_ml_inst_val, ex_mova_mx}),
    .Q    ({f2_ml_inst_val, f2_mova_mx})
  );

  en_ff #(
    .width($bits({ex_core_inst[`VPU_INST_RD_SEL], ex_ml_mask_to_f2, (ex_thread_id_int && !ex_ml_inst_val), ex_use_prev_sigs, ex_lane_idx}))
  ) F2_CTRL_EN_FF (
    .clock(clock_sec),
    .en   (ex_ctrl_valid_kill),
    .D    ({ex_core_inst[`VPU_INST_RD_SEL], ex_ml_mask_to_f2, (ex_thread_id_int && !ex_ml_inst_val), ex_use_prev_sigs, ex_lane_idx}),
    .Q    ({f2_regfile_waddr, f2_ml_mask, f2_thread_id, f2_ml_use_prev_sigs, f2_lane_idx})
  );

  // Some signals in "f2_req.sigs" are used to stall, so ensure they are properly reset here and in previus stages
  rst_en_ff #(
    .width($bits(ex_req))
  ) F2_CTRL_RST_EN_FF (
    .clock(clock_sec),
    .reset(reset),
    .en   (ex_ctrl_valid_kill & !ex_use_prev_sigs),
    .D    (ex_req),
    .Q    (f2_req)
  );


  en_ff #(
    .width($bits({ex_core_gsc_src, ex_store_data_lane}))
  ) F2_CTRL2_EN_FF (
    .clock(clock_sec),
    .en   (ex_ctrl_valid_kill & !ex_use_prev_sigs & ex_store_data_lane_en),
    .D    ({ex_core_gsc_src, ex_store_data_lane}),
    .Q    ({f2_gsc_src, f2_store_data_lane})
  );


  ////////////////////////////////////////////////////////////////////////////////
  // F2 control logic
  ////////////////////////////////////////////////////////////////////////////////

  // control writing to regmask registers
  assign f2_mask_wen = f2_wen & !f2_req.sigs.toint;

  // control internal pipe
  always_comb begin
    f2_kill = f2_core_kill && !f2_req.sigs.trans && !f2_ml_inst_val; // Ignore kill from core when doing generated operations
    f2_ctrl_killed = f2_kill | f2_ctrl_killed_prev;
    f2_regfile_wmask_to_f3 = f2_regfile_wmask & {`VPU_LANE_NUM{f2_ctrl_valid_qual & !f2_kill}};
  end

  // Store data to DCache
  always_comb begin
    // Scatter data
    f2_scatter_data = f2_store_data_lane[f2_gsc_src];
    f2_store_data   = f2_store_data_lane; // PS regular (scalar sends everything as dcache ignores the MSBs)
  end


  // signals to control writing to regfile registers (func)
  always_comb begin
    f2_regfile_wmask = `VPU_LANE_NUM'b0;

    if (f2_wen && !f2_req.sigs.maskop && !f2_req.sigs.tointm && !f2_req.sigs.toint) begin
      if ((f2_ctrl_valid_qual && (!f2_req.sigs.vector)) || (f2_req.sigs.cmd == `VPU_FCMD_FCMOVM))
        f2_regfile_wmask = {`VPU_LANE_NUM{1'b1}};
      else
        f2_regfile_wmask = f2_mask_rf0;
    end


    if (f2_wen && f2_ml_inst_val)
      f2_regfile_wmask = f2_ml_mask;

  end


  // swizz / pacrep connections between lanes
  always_comb begin
    //f2_packreph
    f2_packreph0 = {f2_fswizz_rdata[1][15:0], f2_fswizz_rdata[0][15:0]};
    f2_packreph1 = {f2_fswizz_rdata[3][15:0], f2_fswizz_rdata[2][15:0]};
    f2_packreph2 = {f2_fswizz_rdata[5][15:0], f2_fswizz_rdata[4][15:0]};
    f2_packreph3 = {f2_fswizz_rdata[7][15:0], f2_fswizz_rdata[6][15:0]};

    //f2_packrepb
    f2_packrepb0 = {f2_fswizz_rdata[3][7:0], f2_fswizz_rdata[2][7:0], f2_fswizz_rdata[1][7:0], f2_fswizz_rdata[0][7:0]};
    f2_packrepb1 = {f2_fswizz_rdata[7][7:0], f2_fswizz_rdata[6][7:0], f2_fswizz_rdata[5][7:0], f2_fswizz_rdata[4][7:0]};


    //lane 0
    f2_fswizz_rdata1[0] = f2_fswizz_rdata[1];
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPH)
      f2_fswizz_rdata1[0] = f2_packreph0;
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPB)
      f2_fswizz_rdata1[0] = f2_packrepb0;
    f2_fswizz_rdata2[0] = f2_fswizz_rdata[2];
    f2_fswizz_rdata3[0] = f2_fswizz_rdata[3];
    //lane 1
    f2_fswizz_rdata1[1] = f2_fswizz_rdata[0];
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPH)
      f2_fswizz_rdata1[1] = f2_packreph1;
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPB)
      f2_fswizz_rdata1[1] = f2_packrepb1;
    f2_fswizz_rdata2[1] = f2_fswizz_rdata[2];
    f2_fswizz_rdata3[1] = f2_fswizz_rdata[3];
    //lane 2
    f2_fswizz_rdata1[2] = f2_fswizz_rdata[0];
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPH)
      f2_fswizz_rdata1[2] = f2_packreph2;
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPB)
      f2_fswizz_rdata1[2] = f2_packrepb0;
    f2_fswizz_rdata2[2] = f2_fswizz_rdata[1];
    f2_fswizz_rdata3[2] = f2_fswizz_rdata[3];
    //lane 3
    f2_fswizz_rdata1[3] = f2_fswizz_rdata[0];
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPH)
      f2_fswizz_rdata1[3] = f2_packreph3;
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPB)
      f2_fswizz_rdata1[3] = f2_packrepb1;
    f2_fswizz_rdata2[3] = f2_fswizz_rdata[1];
    f2_fswizz_rdata3[3] = f2_fswizz_rdata[2];
    //lane 4
    f2_fswizz_rdata1[4] = f2_fswizz_rdata[5];
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPH)
      f2_fswizz_rdata1[4] = f2_packreph0;
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPB)
      f2_fswizz_rdata1[4] = f2_packrepb0;
    f2_fswizz_rdata2[4] = f2_fswizz_rdata[6];
    f2_fswizz_rdata3[4] = f2_fswizz_rdata[7];
    //lane 5
    f2_fswizz_rdata1[5] = f2_fswizz_rdata[4];
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPH)
      f2_fswizz_rdata1[5] = f2_packreph1;
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPB)
      f2_fswizz_rdata1[5] = f2_packrepb1;
    f2_fswizz_rdata2[5] = f2_fswizz_rdata[6];
    f2_fswizz_rdata3[5] = f2_fswizz_rdata[7];
    //lane 6
    f2_fswizz_rdata1[6] = f2_fswizz_rdata[4];
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPH)
      f2_fswizz_rdata1[6] = f2_packreph2;
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPB)
      f2_fswizz_rdata1[6] = f2_packrepb0;
    f2_fswizz_rdata2[6] = f2_fswizz_rdata[5];
    f2_fswizz_rdata3[6] = f2_fswizz_rdata[7];
    //lane 7
    f2_fswizz_rdata1[7] = f2_fswizz_rdata[4];
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPH)
      f2_fswizz_rdata1[7] = f2_packreph3;
    if (f2_req.sigs.cmd == `VPU_FCMD_PACKREPB)
      f2_fswizz_rdata1[7] = f2_packrepb1;
    f2_fswizz_rdata2[7] = f2_fswizz_rdata[5];
    f2_fswizz_rdata3[7] = f2_fswizz_rdata[6];
  end

  //ten a/n regfile wen
  assign f2_tena_regfile_wen_l = id_ml_load_ctrl_pre_tena_wen;
  assign f2_tenb_regfile_wen_l = id_ml_load_ctrl_pre_tenb_wen;



  ////////////////////////////////////////////////////////////////////////////////
  // f2 core control
  ////////////////////////////////////////////////////////////////////////////////
  assign f2_core_ctrl.fma          = f2_req.sigs.txfma;
  assign f2_core_ctrl.store_data   = f2_store_data;
  assign f2_core_ctrl.scatter_data = f2_scatter_data;
  assign f2_core_ctrl.tointm       = f2_req.sigs.tointm & !f2_req.sigs.shsw;



  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F3 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  rst_ff #(
    .width($bits({(f2_ctrl_valid_qual & !f2_kill), f2_ctrl_valid, f2_ctrl_killed, f2_ml_inst_val, (f2_wen & !f2_kill)}))
  ) F3_CTRL_RST_FF (
    .clock(clock_sec),
    .reset(reset),
    .D    ({(f2_ctrl_valid_qual & !f2_kill), f2_ctrl_valid, f2_ctrl_killed, f2_ml_inst_val, (f2_wen & !f2_kill)}),
    .Q    ({f3_ctrl_valid_qual, f3_ctrl_valid, f3_ctrl_killed_prev, f3_ml_inst_val, f3_wen})
  );

  ff #(
    .width($bits({f2_thread_id, f2_mova_mx}))
  ) F3_CTRL_FF (
    .clock(clock_sec),
    .D    ({f2_thread_id, f2_mova_mx}),
    .Q    ({f3_thread_id, f3_mova_mx})
  );


  ff #(
    .width($bits(f2_regfile_wmask))
  ) F3_CTRL2_FF (
    .clock(clock_sec),
    .D    (f2_regfile_wmask_to_f3),
    .Q    (f3_regfile_wmask)
  );

  en_ff #(
    .width($bits({f2_ml_use_prev_sigs, f2_lane_idx}))
  ) F3_CTRL_EN_FF (
    .clock(clock_sec),
    .en   (f2_ctrl_valid_qual),
    .D    ({f2_ml_use_prev_sigs, f2_lane_idx}),
    .Q    ({f3_ml_use_prev_sigs, f3_lane_idx})
  );


  en_ff #(
    .width($bits(f2_regfile_waddr))
  ) F3_CTRL4_EN_FF (
    .clock(clock_sec),
    .en   (f2_ctrl_valid),
    .D    (f2_regfile_waddr),
    .Q    (f3_regfile_waddr)
  );


  rst_en_ff #(
    .width($bits(f2_req.sigs))
  ) F3_CTRL_RST_EN_FF (
    .clock(clock_sec),
    .reset(reset),
    .en   (f2_ctrl_valid & !f2_ml_use_prev_sigs),
    .D    (f2_req.sigs),
    .Q    (f3_ctrl_sigs)
  );



  ////////////////////////////////////////////////////////////////////////////////
  // F3 control logic
  ////////////////////////////////////////////////////////////////////////////////



  // signals to control writing to regfile registers (load)
  always_comb begin
    f3_regfile_wmask_l = {`VPU_LANE_NUM{2'b0}};
    f3_regfile_thrid_l = id_reduce_load_ctrl.wen ? id_reduce_load_ctrl.thread_id : wb_dmem_resp.thread_id; // Override with Tensor reduce thread id
    f3_regfile_waddr_l = id_reduce_load_ctrl.wen ? id_reduce_load_ctrl.waddr     : wb_dmem_resp.addr;      // Override with Tensor reduce write address
    f3_regfile_wdata_l = '0;

    if (wb_dmem_resp_val_masked) begin
      if (wb_dmem_resp_is_gather) begin
        f3_regfile_wmask_l[wb_dmem_resp.gdst] = 2'b11; // Bypass to all sources
        f3_regfile_wdata_l[wb_dmem_resp.gdst * `VPU_DATA_S_SZ +: `VPU_DATA_S_SZ] = wb_dmem_resp.data[`VPU_DATA_S_SZ - 1:0];
      end else begin
        for (integer i = 0; i < `VPU_LANE_NUM; i++)
          f3_regfile_wmask_l[i] = id_reduce_load_ctrl.wen                  ? {2'b01}                      // DO NOT bypass to source 2/3 for reduce
                                : (wb_regfile_dtype_l == `VPU_LOAD_DTYPE_PS) ? {2{wb_dmem_resp.ps_mask[i]}} // PS load
                                :                                              2'b11;                      // Bypass to all sources
        f3_regfile_wdata_l = (wb_dmem_resp.typ == dcache_type_PS_BR) ? {`VPU_LANE_NUM{wb_dmem_resp.data[`VPU_DATA_S_SZ - 1:0]}} : wb_dmem_resp.data;
      end

      // zero extension for type S/D for upper 64b
      // (other bits are already cleared by Dcache
      if ((wb_regfile_dtype_l == `VPU_LOAD_DTYPE_S) || (wb_regfile_dtype_l == `VPU_LOAD_DTYPE_D))
        f3_regfile_wdata_l[`VPU_DATA_SZ - 1:64] = {`VPU_DATA_SZ - 64{1'b0}};
    end

    //wen
    f3_regfile_wen_l = |f3_regfile_wmask_l;
  end


  // signals to control writing to tensor rf
  always_comb begin
    f3_tena_regfile_waddr_l = '0;
    f3_tenb_regfile_waddr_l = '0;

    //wen
    f3_tena_regfile_wen_l = id_tensorfma_load_ctrl.wen & ~id_tensorfma_load_ctrl.waddr[3];
    f3_tenb_regfile_wen_l = id_tensorfma_load_ctrl.wen & id_tensorfma_load_ctrl.waddr[3];

    //waddr
    if (f3_tena_regfile_wen_l)
      f3_tena_regfile_waddr_l = id_tensorfma_load_ctrl.waddr[`VPU_TENA_ADDR];

    if (f3_tenb_regfile_wen_l)
      f3_tenb_regfile_waddr_l = id_tensorfma_load_ctrl.waddr[`VPU_TENB_ADDR];

    // TENA always broadcasts from SCP port
    f3_tena_regfile_wdata_l       = dcache_scp_data[`VPU_DATA_S_SZ * id_tensorfma_load_ctrl.broadcast_sel +: `VPU_DATA_S_SZ];
    f3_tentmp_regfile_wdata_l     = dcache_scp_data[`VPU_DATA_S_SZ * 2 * (id_tensorfma_load_ctrl.broadcast_sel >> 1) +: `VPU_DATA_S_SZ * 2];
    f3_tentmp_msb_regfile_wdata_l = f3_tentmp_regfile_wdata_l[`VPU_DATA_S_SZ * 2 - 1:`VPU_DATA_S_SZ];
    // TENB full cacheline from SCP or TENB
    f3_tenb_regfile_wdata_l = dcache_scp_resp.fill_is_tenb ? dcache_tenb_data : dcache_scp_data;
  end

  // zero data detection
  always_comb begin
    f3_zero_data_a   = {`VPU_LANE_NUM{(f3_tena_regfile_wdata_l == `VPU_DATA_S_SZ'b0)}};
    f3_zero_data_tmp = {`VPU_LANE_NUM{(f3_tentmp_msb_regfile_wdata_l == `VPU_DATA_S_SZ'b0)}};
    for (int i = 0; i < `VPU_LANE_NUM; i = i + 1) begin
      f3_zero_data_b[i] = (f3_tenb_regfile_wdata_l[`VPU_DATA_S_SZ * i +: `VPU_DATA_S_SZ] == `VPU_DATA_S_SZ'b0);
    end
  end

  //overwrite control signals
  always_comb begin
    f3_ctrl_sigs_to_f4        = f3_ctrl_sigs;
    f3_ctrl_sigs_to_f4.vector = f3_ctrl_sigs.vector;
  end

  //f3 bypass clock enable
  assign f3_bypass_clk_en = (f3_wen & !f3_ctrl_sigs.txfma)
                          | f3_ctrl_sigs.trans
                          | (f2_req.sigs.cmd == `VPU_FCMD_MVS_FX)
                          | (f2_req.sigs.cmd == `VPU_FCMD_MVZ_FX);

  assign f3_data_vector = f3_ctrl_sigs.vector;

  assign f3_ctrl_killed = (f3_core_kill & !f3_ctrl_sigs.trans) | f3_ctrl_killed_prev;


  ////////////////////////////////////////////////////////////////////////////////
  // f3 core control
  ////////////////////////////////////////////////////////////////////////////////
  //core ctrl signals
  assign f3_core_ctrl.fma    = f3_ctrl_sigs.txfma;
  assign f3_core_ctrl.tointm = f3_ctrl_sigs.tointm & !f3_ctrl_sigs.shsw;



  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F4 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    f3_kill = f3_core_kill && !f3_ctrl_sigs.trans && !f3_ml_inst_val; // Ignore kill from core when doing generated operations
    f3_regfile_wmask_to_f4 = f3_regfile_wmask & {`VPU_LANE_NUM{f3_ctrl_valid_qual & !f3_kill}};
  end

  rst_en_ff #(
    .width($bits(f3_ctrl_sigs_to_f4))
  ) F4_CTRL_RST_EN_FF (
    .clock(clock_sec),
    .reset(reset),
    .en   (f3_ctrl_valid & !f3_ml_use_prev_sigs),
    .D    (f3_ctrl_sigs_to_f4),
    .Q    (f4_ctrl_sigs)
  );

  en_ff #(
    .width($bits({f3_ml_use_prev_sigs, f3_lane_idx}))
  ) F4_CTRL2_EN_FF (
    .clock(clock_sec),
    .en   (f3_ctrl_valid_qual),
    .D    ({f3_ml_use_prev_sigs, f3_lane_idx}),
    .Q    ({f4_ml_use_prev_sigs, f4_lane_idx})
  );

  en_ff #(
    .width($bits(f3_regfile_waddr))
  ) F4_CTRL3_EN_FF (
    .clock(clock_sec),
    .en   (f3_wen),
    .D    (f3_regfile_waddr),
    .Q    (f4_regfile_waddr)
  );


  en_ff #(
    .width($bits({f3_regfile_thrid_l, f3_regfile_waddr_l, f3_regfile_wdata_l}))
  ) F4_CTRL4_EN_FF (
    .clock(clock_sec),
    .en   (|f3_regfile_wmask_l),
    .D    ({f3_regfile_thrid_l, f3_regfile_waddr_l, f3_regfile_wdata_l}),
    .Q    ({f4_regfile_thrid_l, f4_regfile_waddr_l, f4_regfile_wdata_l})
  );



  rst_ff #(
    .width($bits({(f3_ctrl_valid_qual & (!f3_core_kill | f3_ml_inst_val)), (f3_wen & (!f3_core_kill | f3_ml_inst_val)), f3_ctrl_killed, f3_ctrl_valid, f3_ml_inst_val}))
  ) F4_CTRL_RST_FF (
    .clock(clock_sec),
    .reset(reset),
    .D    ({(f3_ctrl_valid_qual & (!f3_core_kill | f3_ml_inst_val | f3_ctrl_sigs_to_f4.trans)), (f3_wen & (!f3_core_kill | f3_ml_inst_val | f3_ctrl_sigs_to_f4.trans)), f3_ctrl_killed, f3_ctrl_valid, f3_ml_inst_val}),
    .Q    ({f4_ctrl_valid_qual, f4_wen, f4_ctrl_killed_prev, f4_ctrl_valid, f4_ml_inst_val})
  );

  ff #(
    .width($bits({f3_regfile_wen_l, f3_regfile_wmask_l, f3_regfile_wmask_to_f4, f3_thread_id, f3_mova_mx}))
  ) F4_CTRL_FF (
    .clock(clock_sec),
    .D    ({f3_regfile_wen_l, f3_regfile_wmask_l, f3_regfile_wmask_to_f4, f3_thread_id, f3_mova_mx}),
    .Q    ({f4_regfile_wen_l, f4_regfile_wmask_l, f4_regfile_wmask, f4_thread_id, f4_mova_mx})
  );

  //f4 bypass clock enable
  assign f4_bypass_clk_en = (f4_wen & !f4_ctrl_sigs.txfma)
                          | f4_ctrl_sigs.trans
                          | (f4_ctrl_sigs.cmd == `VPU_FCMD_MVS_FX)
                          | (f4_ctrl_sigs.cmd == `VPU_FCMD_MVZ_FX);

  assign f4_ctrl_killed = (f4_core_kill & !f4_ctrl_sigs.trans) | f4_ctrl_killed_prev;

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F5 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    f4_kill = f4_core_kill && !f4_ctrl_sigs.trans && !f4_ml_inst_val; // Ignore kill from core when doing generated operations
    f4_regfile_wmask_to_f5 = f4_regfile_wmask & {`VPU_LANE_NUM{f4_ctrl_valid_qual & !f4_kill}};
  end


  rst_ff #(
    .width($bits({f4_ctrl_valid_qual, f4_ctrl_killed, (f4_wen & (!f4_core_kill | f4_ml_inst_val)), f4_ml_inst_val}))
  ) F5_CTRL_RST_FF (
    .clock(clock_sec),
    .reset(reset),
    .D    ({f4_ctrl_valid_qual, f4_ctrl_killed, (f4_wen & (!f4_core_kill | f4_ml_inst_val | f4_ctrl_sigs.trans)), f4_ml_inst_val}),
    .Q    ({f5_ctrl_valid_qual, f5_ctrl_killed, f5_wen, f5_ml_inst_val})
  );


  ff #(
    .width($bits({f4_regfile_wmask_to_f5, f4_thread_id, f4_mova_mx}))
  ) F5_CTRL_FF (
    .clock(clock_sec),
    .D    ({f4_regfile_wmask_to_f5, f4_thread_id, f4_mova_mx}),
    .Q    ({f5_regfile_wmask, f5_thread_id, f5_mova_mx})
  );

  rst_en_ff #(
    .width($bits(f4_ctrl_sigs))
  ) F5_CTRL_RST_EN_FF (
    .clock(clock_sec),
    .reset(reset),
    .en   (f4_ctrl_valid_qual & !f4_ml_use_prev_sigs),
    .D    (f4_ctrl_sigs),
    .Q    (f5_ctrl_sigs)
  );

  en_ff #(
    .width($bits({f4_ml_use_prev_sigs, f4_lane_idx, f4_core_kill}))
  ) F5_CTRL_EN_FF (
    .clock(clock_sec),
    .en   (f4_ctrl_valid_qual),
    .D    ({f4_ml_use_prev_sigs, f4_lane_idx, f4_core_kill}),
    .Q    ({f5_ml_use_prev_sigs, f5_lane_idx, f5_core_kill})
  );


  en_ff #(
    .width($bits(f4_regfile_waddr))
  ) F5_CTRL3_EN_FF (
    .clock(clock_sec),
    .en   (f4_wen),
    .D    (f4_regfile_waddr),
    .Q    (f5_regfile_waddr)
  );

  //f5 bypass clock enable
  assign f5_bypass_clk_en = (f5_wen & !f5_ctrl_sigs.txfma)
                          | f5_ctrl_sigs.trans
                          | (f5_ctrl_sigs.cmd == `VPU_FCMD_MVS_FX)
                          | (f5_ctrl_sigs.cmd == `VPU_FCMD_MVZ_FX);


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F6 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  rst_ff #(
    .width($bits({f5_ctrl_valid_qual, f5_ctrl_killed, (f5_wen & (!f5_core_kill | f5_ml_inst_val)), f5_ml_inst_val}))
  ) F6_CTRL_RST_FF (
    .clock(clock_sec),
    .reset(reset),
    .D    ({f5_ctrl_valid_qual, f5_ctrl_killed, (f5_wen & (!f5_core_kill | f5_ml_inst_val | f5_ctrl_sigs.trans)), f5_ml_inst_val}),
    .Q    ({f6_ctrl_valid_qual, f6_ctrl_killed, f6_wen, f6_ml_inst_val})
  );

  rst_en_ff #(
    .width($bits(f5_ctrl_sigs))
  ) F6_CTRL_RST_EN_FF (
    .clock(clock_sec),
    .reset(reset),
    .en   (f5_ctrl_valid_qual & !f5_ml_use_prev_sigs),
    .D    (f5_ctrl_sigs),
    .Q    (f6_ctrl_sigs)
  );

  ff #(
    .width($bits({f5_regfile_wmask, f5_thread_id}))
  ) F6_CTRL_FF (
    .clock(clock_sec),
    .D    ({f5_regfile_wmask, f5_thread_id}),
    .Q    ({f6_regfile_wmask, f6_thread_id})
  );

  en_ff #(
    .width($bits({f5_ml_use_prev_sigs, f5_lane_idx, f5_core_kill}))
  ) F6_CTRL_EN_FF (
    .clock(clock_sec),
    .en   (f5_ctrl_valid_qual),
    .D    ({f5_ml_use_prev_sigs, f5_lane_idx, f5_core_kill}),
    .Q    ({f6_ml_use_prev_sigs, f6_lane_idx, f6_core_kill})
  );

  en_ff #(
    .width($bits(f5_regfile_waddr))
  ) F6_CTRL2_EN_FF (
    .clock(clock_sec),
    .en   (f5_wen),
    .D    (f5_regfile_waddr),
    .Q    (f6_regfile_waddr)
  );

  //f6 bypass clock enable
  assign f6_bypass_clk_en = (f6_wen & !f6_ctrl_sigs.txfma)
                          | f6_ctrl_sigs.trans
                          | (f6_ctrl_sigs.cmd == `VPU_FCMD_MVS_FX)
                          | (f6_ctrl_sigs.cmd == `VPU_FCMD_MVZ_FX);


  //Trans wmask overwrite
  always_comb begin
    f6_thread_id_to_f7     = f6_thread_id;
    f6_regfile_wmask_to_f7 = f6_regfile_wmask;

    if (f6_trans_wen_ctrl_en) begin
      f6_thread_id_to_f7     = f6_trans_thread_id;
      f6_regfile_wmask_to_f7 = f6_trans_wmask;
    end
  end


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F7 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  rst_ff #(
    .width($bits({f6_ctrl_valid_qual, f6_ctrl_killed, (f6_wen & (!f6_core_kill | f6_ml_inst_val))}))
  ) F7_CTRL_RST_FF (
    .clock(clock_sec),
    .reset(reset),
    .D    ({f6_ctrl_valid_qual, f6_ctrl_killed, (f6_wen & (!f6_core_kill | f6_ml_inst_val | f6_ctrl_sigs.trans))}),
    .Q    ({f7_ctrl_valid_qual, f7_ctrl_killed, f7_wen})
  );

  rst_en_ff #(
    .width($bits(f6_ctrl_sigs))
  ) F7_CTRL_RST_EN_FF (
    .clock(clock_sec),
    .reset(reset),
    .en   (f6_ctrl_valid_qual & !f6_ml_use_prev_sigs),
    .D    (f6_ctrl_sigs),
    .Q    (f7_ctrl_sigs)
  );

  ff #(
    .width($bits({f6_regfile_wmask, f6_thread_id}))
  ) F7_CTRL_FF (
    .clock(clock_sec),
    .D    ({f6_regfile_wmask_to_f7, f6_thread_id_to_f7}),
    .Q    ({f7_regfile_wmask, f7_thread_id})
  );

  en_ff #(
    .width($bits({f6_ml_use_prev_sigs, f6_lane_idx}))
  ) F7_CTRL2_EN_FF (
    .clock(clock_sec),
    .en   (f6_ctrl_valid_qual),
    .D    ({f6_ml_use_prev_sigs, f6_lane_idx}),
    .Q    ({f7_ml_use_prev_sigs, f7_lane_idx})
  );

  en_ff #(
    .width($bits(f6_regfile_waddr))
  ) F7_CTRL3_EN_FF (
    .clock(clock_sec),
    .en   (f6_wen),
    .D    (f6_regfile_waddr),
    .Q    (f7_regfile_waddr)
  );

  assign f7_regfile_wen = f7_regfile_wmask & {`VPU_LANE_NUM{f7_ctrl_valid_qual}};


  //f7 bypass clock enable
  assign f7_bypass_clk_en = (f7_wen & !f7_ctrl_sigs.txfma)
                          | f7_ctrl_sigs.trans
                          | (f7_ctrl_sigs.cmd == `VPU_FCMD_MVS_FX)
                          | (f7_ctrl_sigs.cmd == `VPU_FCMD_MVZ_FX);



  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F8 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  rst_en_ff #(
    .width($bits(f7_ctrl_sigs))
  ) F8_CTRL_RST_EN_FF (
    .clock(clock_sec),
    .reset(reset),
    .en   (f7_ctrl_valid_qual & !f7_ml_use_prev_sigs),
    .D    (f7_ctrl_sigs),
    .Q    (f8_ctrl_sigs)
  );

  rst_ff #(
    .width($bits({f7_ctrl_valid_qual, f7_ctrl_killed, f7_wen, f7_regfile_wen, f7_thread_id}))
  ) F8_CTRL_RST_FF (
    .clock(clock_sec),
    .reset(reset),
    .D    ({f7_ctrl_valid_qual, f7_ctrl_killed, f7_wen, f7_regfile_wen, f7_thread_id}),
    .Q    ({f8_ctrl_valid_qual, f8_ctrl_killed, f8_wen, f8_regfile_wmask, f8_thread_id})
  );


  en_ff #(
    .width($bits({f7_regfile_waddr, f7_lane_idx}))
  ) F8_CTRL_EN_FF (
    .clock(clock_sec),
    .en   (f7_ctrl_valid_qual),
    .D    ({f7_regfile_waddr, f7_lane_idx}),
    .Q    ({f8_regfile_waddr, f8_lane_idx})
  );

  assign f8_regfile_wen = f8_regfile_wmask;

  //ctrl sent to f8 bypass
  assign f8_txfma_op    = f8_ctrl_sigs.txfma;
  assign f8_data_vector = f8_ctrl_sigs.vector;



  ////////////////////////////////////////////////////////////////////////////////
  // mask value coming from txfma compare
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    f8_regmask_from_txfma = f8_ctrl_sigs.tointm & f8_ctrl_sigs.txfma;

    for (integer i = 0; i < `VPU_LANE_NUM; i++)
      f8_regmask_comp_res[i] = (f8_txfma_comp_res[i] & f8_regmask_from_txfma);
  end



  ////////////////////////////////////////////////////////////////////////////////
  // Flags sent to core at stage F8
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    f8_wflags_all_lanes = '0;

    for (integer i = 0; i < `VPU_LANE_NUM; i++)
      f8_wflags_all_lanes |= f8_wflags[i] & {`CORE_FCSR_FLAG_BITS_SZ{f8_regfile_wmask[i] || (f8_regmask_store[i] & f8_ctrl_sigs.tointm)}};
  end

  assign f8_fcsr_flags = (!f8_ctrl_sigs.vector) ? f8_wflags[0]
                       :                          f8_wflags_all_lanes;


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // Delayed writes
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // wb trans div control logic
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // wb dmem control signals
  ////////////////////////////////////////////////////////////////////////////////
  // load dmem response
  assign wb_dmem_resp_val_masked = wb_dmem_resp_val && (wb_dmem_resp.ps_mask != 0 || !wb_dmem_resp.typ[3]);
  assign wb_dmem_resp_is_gather  = (dcache_type_is_gsc(wb_dmem_resp.typ));
  assign wb_regfile_dtype_l      = {2'b0, wb_dmem_resp.typ[3], wb_dmem_resp.typ[0] && !wb_dmem_resp.typ[3]};




  ////////////////////////////////////////////////////////////////////////////////
  // wb core control signals
  ////////////////////////////////////////////////////////////////////////////////
  // core to int data
  always_comb begin
    f8_toint_data = f8_sh_sw_wdata[f8_lane_idx * `VPU_DATA_S_SZ +: `VPU_DATA_S_SZ];

    //sel data from lane 0 when writting into int rf
    if (f8_lane_idx == 0)
      f8_toint_data = f8_wdata_lane0;
  end

  // core ctrl signals
  always_comb begin

    wb_core_ctrl.toint_data = {32'b0, f8_toint_data} & {`XREG_SIZE{f8_ctrl_sigs.toint}};

    if (f8_ctrl_sigs.cmd == `VPU_FCMD_FEQ || f8_ctrl_sigs.cmd == `VPU_FCMD_FLT || f8_ctrl_sigs.cmd == `VPU_FCMD_FLE)
      wb_core_ctrl.toint_data = {65'b0, f8_toint_data[0]};
    if (f8_ctrl_sigs.cmd == `VPU_FCMD_MVS_FX || f8_ctrl_sigs.cmd == `VPU_FCMD_CVT_INTF32 || f8_ctrl_sigs.cmd == `VPU_FCMD_CVT_UINTF32)
      wb_core_ctrl.toint_data = {{32{f8_toint_data[`VPU_DATA_S_SZ - 1]}}, f8_toint_data};
    if (f8_ctrl_sigs.maskop)
      wb_core_ctrl.toint_data = f8_toint_mask_rf;
  end


  assign wb_core_ctrl.tointm           = f4_ctrl_sigs.tointm & !f4_ctrl_sigs.shsw;
  assign wb_core_ctrl.fma              = f4_ctrl_sigs.txfma;
  assign wb_core_ctrl.fcsr_flags       = f8_fcsr_flags;
  assign wb_core_ctrl.fcsr_flags_valid = f8_trans_wen_ctrl_en ? f8_trans_fcsr_en : (f8_ctrl_valid_qual && f8_ctrl_sigs.wflags);
  assign wb_core_ctrl.thread_id        = f8_thread_id;
  assign wb_core_ctrl.mova_mx          = f5_mova_mx;
                            

  ////////////////////////////////////////////////////////////////////////////////
  // Performance counters
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    // TIMA unit works independently without intrunctions. Raise when the unit activates
    io_events[`VPU_EVENT_TIMA_OPS] = |ex_tima_valid;
    // Raise when an instruction use the TXFMA with float16 and it's retired
    io_events[`VPU_EVENT_TXFMA_3216_OPS] = f8_ctrl_valid_qual && f8_ctrl_sigs.dtype == `VPU_DTYPE_F16_F32 && f8_ctrl_sigs.txfma;
    // Raise when an instruction use the TXFMA with float32/int and it's retired
    io_events[`VPU_EVENT_TXFMA_32_OPS] = f8_ctrl_valid_qual && f8_ctrl_sigs.txfma && !f8_ctrl_sigs.trans;
    // Raise when an instruction use the TXFMA with int and it's retired
    io_events[`VPU_EVENT_TXFMA_INT_OPS] = f8_ctrl_valid_qual && f8_ctrl_sigs.dtype == `VPU_DTYPE_INT && f8_ctrl_sigs.txfma;
    // Raise when an instruction is TRANS (doesn't count micro-operations, just the instruction)
    io_events[`VPU_EVENT_TRANS_OPS] = f8_ctrl_valid_qual && f8_trans_wen_ctrl_en && !f8_ctrl_sigs.trans;
    // Raise when an instruction use SHORT unit and its retired
    io_events[`VPU_EVENT_SHORT_OPS] = f8_ctrl_valid_qual && f8_ctrl_sigs.shsw;
    // Raise when an instruction use MASK unit and its retired
    io_events[`VPU_EVENT_MASK_OPS] = f8_ctrl_valid_qual && f8_ctrl_sigs.maskop;
    // TFMA_INST = csr pseudoinstruction
    io_events[`VPU_EVENT_TFMA_INST] = f0_core_ctrl.tensorfma_start;
    // REDUCE_INST = csr pseudoinstruction
    io_events[`VPU_EVENT_TREDUCE_INST] = f0_core_ctrl.reduce_start;
    // TSTORE_INST = csr pseudoinstruction
    io_events[`VPU_EVENT_TQUANT_INST] = f0_core_ctrl.tensorquant_start;  

  end


////////////////////////////////////////////////////////////////////////////////
// Machine learning
// This unit implements the TensorFMA instruction used for machine learning.
////////////////////////////////////////////////////////////////////////////////
  vpu_ml ml (
      // System signals
    .clock                    ( clock_sec                            ),
    .reset                    ( reset                                ),
    // Control port from core to start new TensorFMA
    .f0_core_ctrl             ( f0_core_ctrl                         ),
    .tfma_enabled             ( id_tfma_enabled                      ),
    .tfma_wrrf_enabled        ( id_tfma_wrrf_enabled                 ),
    .tquant_enabled           ( id_tquant_enabled                    ),
    .tstore_reduce_enabled    ( id_reduce_enabled                    ),
    // Requests to read data to dacache
    .dcache_scp_resp          ( dcache_scp_resp                      ),
    .dcache_scp_data          ( dcache_scp_data                      ),
    .dcache_ctrl              ( dcache_ctrl                          ),
    // Dcache reduce control
    .dcache_reduce_ctrl       ( dcache_reduce_ctrl                   ),
    // Signals going to VPU/SCP control
    .tensorfma_load_ctrl      ( id_tensorfma_load_ctrl               ),
    .reduce_load_ctrl         ( id_reduce_load_ctrl                  ),
    .load_ctrl_pre_tena_wen   ( id_ml_load_ctrl_pre_tena_wen         ),
    .load_ctrl_pre_tenb_wen   ( id_ml_load_ctrl_pre_tenb_wen         ),
    .id_inst_en_next          ( id_ml_inst_en_next                   ),
    .id_inst_next             ( id_ml_inst_next                      ),
    .id_mask_next             ( id_ml_mask_next                      ),
    .id_inst_en               ( id_ml_inst_en                        ),
    .id_inst                  ( id_core_inst_int                     ),
    .id_inst_fma_en           ( id_ml_inst_fma_en                    ),
    .id_inst_tena_quant_en    ( id_ml_tena_quant_en                  ),
    .ex_tena_rdata_mask       ( ex_tena_rdata_mask                   ),  
    // TensorTMP control
    .f2_tentmp_wen_early      ( f2_tentmp_wen_early                  ),
    .f3_tentmp_wen            ( f3_tentmp_wen                        ),
    .f3_tentmp_waddr          ( f3_tentmp_waddr                      ),
    .ex_tentmp_ren            ( ex_tentmp_ren                        ),
    .ex_tentmp_raddr          ( ex_tentmp_raddr                      ),
    // TensorQuant RF control
    .ex_tquant_ren            ( ex_tquant_ren                        ),
    .ex_tquant_rdata          ( ex_tquant_rdata                      ),
    // Control straight to TIMA (bypasses VPU CTRL)
    .ex_tima_valid            ( ex_tima_valid_unqual                 ),
    .ex_tima_mask             ( ex_tima_mask                         ),
    .ex_tima_tena_raddr       ( ex_tima_tena_raddr                   ),
    .ex_tima_tenb_raddr       ( ex_tima_tenb_raddr                   ),
    .ex_tima_last_pass        ( ex_tima_last_pass                    ),
    .ex_tima_ren3             ( ex_tima_ren3                         ),
    .ex_tima_sa               ( ex_tima_sa                           ),
    .ex_tima_sb               ( ex_tima_sb                           ),
    .f2_tima_ren3             ( f2_tima_ren3                         ),
    .f2_tima_tenc_raddr       ( f2_tima_tenc_raddr                   ),
    .f3_tima_tenc_wen         ( f3_tima_tenc_wen                     ),
    .f3_tima_tenc_waddr       ( f3_tima_tenc_waddr                   ),
    .f3_tima_rf_wen           ( f3_tima_rf_wen                       ),
    .io_events_wait_tenb      ( io_events[`VPU_EVENT_TFMA_WAIT_TENB] ),
    .vpu_tensorquant_debug    ( vpu_tensorquant_debug                ),
    .vpu_tensorreduce_debug   ( vpu_tensorreduce_debug               ),
    .vpu_tensorfma_debug      ( vpu_tensorfma_debug                  )
  );

////////////////////////////////////////////////////////////////////////////////
// Trans control unit
////////////////////////////////////////////////////////////////////////////////
  vpu_trans trans(
    // System signals
    .clock_aon                 ( clock_aon                         ),
    .reset                     ( reset                             ),
    // Control port from core to start a new trans
    .id_valid                  ( id_core_valid_int                 ),
    .id_core_thread_id         ( id_core_thread_id                 ),
    .id_instruction            ( id_core_inst_int                  ),
    .ex_valid                  ( ex_ctrl_valid                     ),
    .ex_core_thread_id         ( ex_thread_id                      ),
    .ex_instruction            ( ex_core_inst                      ),
    .ex_mask_rf0               ( ex_mask_rf0                       ),
    // Chicken bit vputrans clock gate enable
    .chicken_bit_vputrans      ( chicken_bit_vputrans              ),
    // Kill
    .f6_killed                 ( f6_ctrl_killed                    ),
    .f7_killed                 ( f7_ctrl_killed                    ),
    .f8_killed                 ( f8_ctrl_killed                    ),
    // Output control
    .id_insert_en_next         ( id_trans_insert_en_next           ),
    .id_insert_inst_next       ( id_trans_insert_inst_next         ),
    .id_insert_en              ( id_trans_insert_en                ),
    .id_trans_thread_id        ( id_trans_thread_id                ),
    .ex_trans_thread_id        ( ex_trans_thread_id                ),
    .f6_trans_ctrl_en          ( f6_trans_wen_ctrl_en              ),
    .f6_trans_wmask            ( f6_trans_wmask                    ),
    .f6_trans_thread_id        ( f6_trans_thread_id                ),
    .f8_trans_fcsr_en          ( f8_trans_fcsr_en                  ),
    .f8_trans_ctrl_en          ( f8_trans_wen_ctrl_en              ),      
    .id_trans_busy             ( id_trans_busy                     ),
    .id_trans_scoreboard       ( id_trans_scoreboard_slots         ),
    .state                     ( trans_st                          )
  );

////////////////////////////////////////////////////////////////////////////////
// TensorA RF
////////////////////////////////////////////////////////////////////////////////
  vpu_tensora_rf tena_rf (
    // System signal
    .clock       (clock_sec),
    // Read ports
    .rd_addr     (ex_tena_raddr),
    .rd_data     (ex_tena_rf_rdata_pre),
    // Write ports
    .wr_en_early (f2_tena_regfile_wen_l),
    .wr_en       (f3_tena_regfile_wen_l),
    .wr_addr     (f3_tena_regfile_waddr_l),
    .wr_data     (f3_tena_regfile_wdata_l)
  );

  always_comb begin
    ex_tena_raddr = ex_tima_valid_unqual ? ex_tima_tena_raddr : ex_regfile_raddr1[0];
  end

////////////////////////////////////////////////////////////////////////////////
// TensorTmp temporal RF
// In INT8 mode, the FSM uses all the SCP slots while it operates. It is
// needed some slots to be freed for other purposes (FCCs, loads, stores, ...)
// In INT8 mode, the A matrix is read sometimes in 64b granularity. This way,
// on the next time that the same A matrix row is read, there's no need to use
// the SCP. 8 temporal values are stored in this RF. This way 8 out of 32
// cycles are available for the dcache.
////////////////////////////////////////////////////////////////////////////////
  vpu_tensortmp_rf tentmp_rf (
    // System signal
    .clock       (clock_sec),
    // Read ports
    .rd_addr     (ex_tentmp_raddr),
    .rd_data     (ex_tentmp_rf_rdata),
    // Write ports
    .wr_en_early (f2_tentmp_wen_early),
    .wr_en       (f3_tentmp_wen),
    .wr_addr     (f3_tentmp_waddr),
    .wr_data     (f3_tentmp_msb_regfile_wdata_l)  // Always write 32MSB of 64b chunk
  );

  // If reading from ten tmp, forward the data from this buffer
  always_comb begin
    // TIMA selects between Tensor Temporal and Tensor A
    ex_tena_rf_tima_rdata = ex_tentmp_ren ? ex_tentmp_rf_rdata    // Read from Temporal RF
                          :                 ex_tena_rf_rdata_pre; // Read from Tensor A
    for (integer i = 0; i < `VPU_LANE_NUM; i++)
      ex_tena_rf_rdata[i] = ex_tquant_ren ? ex_tquant_rdata[i]     // Read from TensorQuant
                          :                 ex_tena_rf_tima_rdata; // Read from Tensor A or Tensor Temporal

    //tena rdata mask to force data to '0' during skip convolutional operation
    ex_tena_rf_tima_rdata &= {`VPU_DATA_S_SZ{ex_tena_rdata_mask}};
  end

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Multi-stage functional unit
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// VPU mask
// This unit implements the mask instructions and the mask rf
////////////////////////////////////////////////////////////////////////////////
  logic ex_ignore_mask;
  assign ex_ignore_mask = (ex_req.sigs.ldst & !ex_req.sigs.m0ren) | ex_req.sigs.cmd == `VPU_FCMD_FCMOVM;

  vpu_mask mask (
    // System signals
    .clock                 (clock_sec),
    .reset                 (reset),
    // EX/F1 data and control inputs
    .ex_in_valid           (ex_mask_valid),
    .ex_inst_valid         (ex_ctrl_valid_kill),
    .ex_regfile_raddr1     (ex_regfile_raddr1[$clog2(`VPU_REGMASK_NUM) - 1:0]),
    .ex_regfile_raddr2     (ex_regfile_raddr2[$clog2(`VPU_REGMASK_NUM) - 1:0]),
    .ex_cmd                (ex_req.sigs.cmd),
    .ex_imm                (ex_req.imm[`VPU_LANE_NUM - 1:0]),
    .ex_fromint            (ex_req.sigs.fromint),
    .ex_fromint_data       (ex_core_fromint_data),
    .ex_thread_id          (ex_thread_id_int && !ex_ml_inst_val),
    .ex_ignore_mask        (ex_ignore_mask),
    // F2 inputs
    .f2_thread_id          (f2_thread_id),
    .f2_wen                (f2_mask_wen),
    .f2_core_kill          (f2_core_kill),
    .f2_waddr              (f2_regfile_waddr[$clog2(`VPU_REGMASK_NUM) - 1:0]),
    .f2_maskop             (f2_req.sigs.maskop),
    .f2_tointm             (f2_req.sigs.tointm),
    // F3 inputs
    .f3_thread_id          (f3_thread_id),
    .f3_regmask_from_short (f3_ctrl_sigs.tointm & f3_ctrl_sigs.shsw),
    .f3_regmask_from_txfma (f3_ctrl_sigs.tointm & f3_ctrl_sigs.txfma),
    .f3_core_kill          (f3_core_kill),
    .f3_regmask_wdata_lane (f3_regmask_wdata_lane),
    // F4 inputs
    .f4_regmask_from_txfma (f4_ctrl_sigs.tointm & f4_ctrl_sigs.txfma),
    .f4_core_kill          (f4_core_kill),
    // F5 inputs
    .f5_thread_id          (f5_thread_id),
    // F6 inputs
    .f6_thread_id          (f6_thread_id),
    // F7 inputs
    .f7_thread_id          (f7_thread_id),
    // F8 inputs
    .f8_thread_id          (f8_thread_id),
    .f8_regmask_comp_res   (f8_regmask_comp_res),
    // EX/F1 outputs
    .ex_mask_rf0           (ex_mask_rf0),
    .ex_mask_in1           (ex_mask_in1),
    // F2 ouputs
    .f2_mask_rf0           (f2_mask_rf0),
    // F8 outputs
    .f8_toint_data         (f8_toint_mask_rf),
    .f8_regmask_store      (f8_regmask_store),
    // WB outputs
    .wb_mask_valid         (id_scoreboard_mask_valid),
    .wb_mask_dest          (id_scoreboard_mask_dest)
  );


////////////////////////////////////////////////////////////////////////////////
// Debug signals
////////////////////////////////////////////////////////////////////////////////

//`define NEIGH_DEBUG_MATCH_WIDTH   64
//`define NEIGH_DEBUG_FILTER_WIDTH 200
//`define NEIGH_DEBUG_DATA_WIDTH   128


  always_comb begin

    vpu_dbg_data   = '0;
    vpu_dbg_match  = '0;
    vpu_dbg_filter = '0;

    ////match
    vpu_dbg_match[0]     = io_events[`VPU_EVENT_TFMA_WAIT_TENB];     //tfma wait tenb
    vpu_dbg_match[1]     = io_events[`VPU_EVENT_TIMA_OPS];           //tima op
    vpu_dbg_match[2]     = io_events[`VPU_EVENT_TXFMA_3216_OPS];     //txfma fp32 fp16
    vpu_dbg_match[3]     = io_events[`VPU_EVENT_TXFMA_32_OPS];       //txfma fp32
    vpu_dbg_match[4]     = io_events[`VPU_EVENT_TXFMA_INT_OPS];      //txfma fp32
    vpu_dbg_match[5]     = io_events[`VPU_EVENT_TRANS_OPS];          //trans ops
    vpu_dbg_match[6]     = io_events[`VPU_EVENT_SHORT_OPS];          //short ops
    vpu_dbg_match[7]     = io_events[`VPU_EVENT_MASK_OPS];           //mask ops
    vpu_dbg_match[8]     = io_events[`VPU_EVENT_TFMA_INST];          //tensor fma instruction
    vpu_dbg_match[9]     = io_events[`VPU_EVENT_TREDUCE_INST];       //tensor reduce instruction
    vpu_dbg_match[10]    = io_events[`VPU_EVENT_TQUANT_INST];        //tensor quant instruction
    vpu_dbg_match[11]    = 1'b0;
    vpu_dbg_match[27:12] = f4_regfile_wmask_l;                       //rf wen load
    vpu_dbg_match[35:28] = f8_regfile_wmask;                         //rf wen func
    vpu_dbg_match[38:36] = id_regfile_ren;                           //rf ren
    vpu_dbg_match[39]    = f3_tima_tenc_wen[0][0];                   //tenc rf wen
    vpu_dbg_match[40]    = f3_tenb_regfile_wen_l;                    //tenb rf wen_l
    vpu_dbg_match[41]    = f3_tena_regfile_wen_l;                    //tena rf wen_l
    vpu_dbg_match[42]    = id_core_valid;                            //id core valid qual
    vpu_dbg_match[43]    = ex_ctrl_valid_qual;                       //ex core valid qual
    vpu_dbg_match[44]    = f2_ctrl_valid_qual;                       //f2 core valid qual
    vpu_dbg_match[45]    = f3_ctrl_valid_qual;                       //f3 core valid qual
    vpu_dbg_match[46]    = f4_ctrl_valid_qual;                       //f4 core valid qual
    vpu_dbg_match[47]    = f5_ctrl_valid_qual;                       //f5 core valid qual
    vpu_dbg_match[48]    = f6_ctrl_valid_qual;                       //f6 core valid qual
    vpu_dbg_match[49]    = f7_ctrl_valid_qual;                       //f7 core valid qual
    vpu_dbg_match[50]    = f8_ctrl_valid_qual;                       //f8 core valid qual
    vpu_dbg_match[51]    = ex_core_kill;                             //ex core kill
    vpu_dbg_match[52]    = f2_core_kill;                             //f2 core kill
    vpu_dbg_match[53]    = f3_core_kill;                             //f3 core kill
    vpu_dbg_match[54]    = f4_core_kill;                             //f4 core kill
    vpu_dbg_match[55]    = ex_core_gscing;                           //ex core gather/scatter
    vpu_dbg_match[56]    = ex_rom_valid;                             //rom valid
    vpu_dbg_match[57]    = wb_dmem_resp_val;                         //dmem resp valid
    vpu_dbg_match[58]    = f8_ctrl_sigs.wflags;                      //flags valid generation
    vpu_dbg_match[59]    = id_trans_insert_en_next;                  //trans instruct en
    vpu_dbg_match[60]    = id_ml_inst_en_next;                       //ml instruct en


    ////data
    //data[0]: input core/decoder control
    vpu_dbg_data[0] = {
      1'b0,
      ex_mask_rf0,               //8
      ex_regfile_raddr3,         //5
      ex_regfile_raddr2,         //5
      ex_regfile_raddr1,         //5
      id_core_thread_id,         //1
      id_core_inst_bits,         //32
      id_vpu_ctrl.use_prev_sigs, //1
      id_vpu_ctrl.sigs,          //46
      id_vpu_ctrl.imm,           //20
      id_vpu_ctrl.rm,            //3
      id_vpu_ctrl.typ            //2
    };


    //data[1]: output core control and trans
    vpu_dbg_data[1][127:110] = '0;                             //Not used
    vpu_dbg_data[1][109:0]   = {
      ex_bypass_force_ctrl,          //5
      trans_st,                       //25
      txfma_trans_dbg,                //16
      id_core_ctrl.tfma_wrrf_enabled, //1
      id_core_ctrl.tfma_enabled,      //1
      id_core_ctrl.tquant_enabled,    //1
      id_core_ctrl.reduce_enabled,    //1
      1'b0,                           //1
      id_core_ctrl.trans_busy,        //1
      id_core_ctrl.id_trans_insert,   //1
      ex_core_ctrl,                   //42
      f2_core_ctrl.fma,               //1
      f2_core_ctrl.tointm,            //1
      f3_core_ctrl,                   //2
      wb_core_ctrl.tointm,            //1
      wb_core_ctrl.fma,               //1
      wb_core_ctrl.fcsr_flags,        //6
      wb_core_ctrl.fcsr_flags_valid,  //1
      wb_core_ctrl.thread_id,         //1
      wb_core_ctrl.mova_mx            //1
    };

    //data[2]: fp/toint/mask scoreboard + dcache resp
    vpu_dbg_data[2][127]   = 1'b0;                                 //Not used
    vpu_dbg_data[2][126:0] = {
      dcache_scp_resp,                     //7
      id_core_ctrl.scoreboard.mask_valid,  //4
      id_core_ctrl.scoreboard.toint_valid, //4
      id_core_ctrl.scoreboard.fp_valid,    //14
      id_core_ctrl.scoreboard.fp_dest      //98
    };

    //data[3]: tensorfma + reduce + tensorquant + tensorstore
    vpu_dbg_data[3][127:112] = '0;
    vpu_dbg_data[3][111:0]   = {
      f0_core_ctrl.reduce_ctrl.tensorstore.src_inc,   //2
      f0_core_ctrl.reduce_ctrl.tensorstore.start_reg, //5
      f0_core_ctrl.reduce_ctrl.tensorstore.cols,      //2
      f0_core_ctrl.reduce_ctrl.tensorstore.rows,      //4
      f0_core_ctrl.reduce_ctrl.tensorstore.coop,      //2
      f0_core_ctrl.reduce_ctrl.tensorstore.scp,       //1
      f0_core_ctrl.tensorfma_ctrl.is_conv,            //1
      f0_core_ctrl.tensorfma_ctrl.cols_b,             //2
      f0_core_ctrl.tensorfma_ctrl.rows_a,             //4
      f0_core_ctrl.tensorfma_ctrl.cols_a,             //4
      f0_core_ctrl.tensorfma_ctrl.start_a,            //4
      f0_core_ctrl.tensorfma_ctrl.to_vrf,             //1
      f0_core_ctrl.tensorfma_ctrl.u_b,                //1
      f0_core_ctrl.tensorfma_ctrl.u_a,                //1
      f0_core_ctrl.tensorfma_ctrl.ten_b,              //1
      f0_core_ctrl.tensorfma_ctrl.scp_b,              //6
      f0_core_ctrl.tensorfma_ctrl.scp_a,              //6
      f0_core_ctrl.tensorfma_ctrl.mode,               //3
      f0_core_ctrl.tensorfma_ctrl.first_pass,         //1
      vpu_tensorquant_debug,                          //11
      vpu_tensorreduce_debug,                         //25
      vpu_tensorfma_debug                             //25
    };


    vpu_dbg_data[4][127:101] = '0;                                                   //Not used
    vpu_dbg_data[4][100:0]   = {
      f0_core_ctrl.reduce_ctrl.tensorstore_scp.stride_entry, //2
      f0_core_ctrl.reduce_ctrl.tensorstore_scp.start_entry,  //7
      f0_core_ctrl.reduce_ctrl.tensorstore_scp.rows,         //4
      f0_core_ctrl.reduce_ctrl.tensorstore_scp.scp,          //1
      f0_core_ctrl.reduce_ctrl.reduce.start_reg,             //5
      f0_core_ctrl.reduce_ctrl.reduce.op,                    //4
      f0_core_ctrl.reduce_ctrl.reduce.num_regs,              //8
      f0_core_ctrl.reduce_ctrl.reduce.partner,               //13
      f0_core_ctrl.reduce_ctrl.reduce.action,                //2
      f0_core_ctrl.tensorquant_ctrl.start_reg,               //5
      f0_core_ctrl.tensorquant_ctrl.cols,                    //2
      f0_core_ctrl.tensorquant_ctrl.rows,                    //4
      f0_core_ctrl.tensorquant_ctrl.scp_src,                 //6
      f0_core_ctrl.tensorquant_ctrl.trans                    //40
    };




    ////filter
    vpu_dbg_filter[199:152] = '0;

    ////match
    vpu_dbg_filter[0]     = io_events[`VPU_EVENT_TFMA_WAIT_TENB];     //tfma wait tenb
    vpu_dbg_filter[1]     = io_events[`VPU_EVENT_TIMA_OPS];           //tima op
    vpu_dbg_filter[2]     = io_events[`VPU_EVENT_TXFMA_3216_OPS];     //txfma fp32 fp16
    vpu_dbg_filter[3]     = io_events[`VPU_EVENT_TXFMA_32_OPS];       //txfma fp32
    vpu_dbg_filter[4]     = io_events[`VPU_EVENT_TXFMA_INT_OPS];      //txfma fp32
    vpu_dbg_filter[5]     = io_events[`VPU_EVENT_TRANS_OPS];          //trans ops
    vpu_dbg_filter[6]     = io_events[`VPU_EVENT_SHORT_OPS];          //short ops
    vpu_dbg_filter[7]     = io_events[`VPU_EVENT_MASK_OPS];           //mask ops
    vpu_dbg_filter[8]     = io_events[`VPU_EVENT_TFMA_INST];          //tensor fma instruction
    vpu_dbg_filter[9]     = io_events[`VPU_EVENT_TREDUCE_INST];       //tensor reduce instruction
    vpu_dbg_filter[10]    = io_events[`VPU_EVENT_TQUANT_INST];        //tensor quant instruction
    vpu_dbg_filter[11]    = 1'b0;
    vpu_dbg_filter[27:12] = f4_regfile_wmask_l;                       //rf wen load
    vpu_dbg_filter[35:28] = f8_regfile_wmask;                         //rf wen func
    vpu_dbg_filter[38:36] = id_regfile_ren;                           //rf ren
    vpu_dbg_filter[39]    = f3_tima_tenc_wen[0][0];                   //tenc rf wen
    vpu_dbg_filter[40]    = f3_tenb_regfile_wen_l;                    //tenb rf wen_l
    vpu_dbg_filter[41]    = f3_tena_regfile_wen_l;                    //tena rf wen_l
    vpu_dbg_filter[42]    = id_core_valid;                            //id core valid qual
    vpu_dbg_filter[43]    = ex_ctrl_valid_qual;                       //ex core valid qual
    vpu_dbg_filter[44]    = f2_ctrl_valid_qual;                       //f2 core valid qual
    vpu_dbg_filter[45]    = f3_ctrl_valid_qual;                       //f3 core valid qual
    vpu_dbg_filter[46]    = f4_ctrl_valid_qual;                       //f4 core valid qual
    vpu_dbg_filter[47]    = f5_ctrl_valid_qual;                       //f5 core valid qual
    vpu_dbg_filter[48]    = f6_ctrl_valid_qual;                       //f6 core valid qual
    vpu_dbg_filter[49]    = f7_ctrl_valid_qual;                       //f7 core valid qual
    vpu_dbg_filter[50]    = f8_ctrl_valid_qual;                       //f8 core valid qual
    vpu_dbg_filter[51]    = ex_core_kill;                             //ex core kill
    vpu_dbg_filter[52]    = f2_core_kill;                             //f2 core kill
    vpu_dbg_filter[53]    = f3_core_kill;                             //f3 core kill
    vpu_dbg_filter[54]    = f4_core_kill;                             //f4 core kill
    vpu_dbg_filter[55]    = ex_core_gscing;                           //ex core gather/scatter
    vpu_dbg_filter[56]    = ex_rom_valid;                             //rom valid
    vpu_dbg_filter[57]    = wb_dmem_resp_val;                         //dmem resp valid
    vpu_dbg_filter[58]    = f8_ctrl_sigs.wflags;                      //flags valid generation
    vpu_dbg_filter[59]    = id_trans_insert_en_next;                  //trans instruct en
    vpu_dbg_filter[60]    = id_ml_inst_en_next;                       //ml instruct en


    vpu_dbg_filter[151:64] = {
      wb_core_ctrl.fcsr_flags, //6
      f4_regfile_waddr_l,      //5
      f3_regfile_waddr,        //5
      f4_regfile_waddr,        //5
      f5_regfile_waddr,        //5
      f6_regfile_waddr,        //5
      f7_regfile_waddr,        //5
      f8_regfile_waddr,        //5
      ex_regfile_raddr3,       //5
      ex_regfile_raddr2,       //5
      ex_regfile_raddr1,       //5
      id_core_inst_bits        //32
    };


  end

endmodule
