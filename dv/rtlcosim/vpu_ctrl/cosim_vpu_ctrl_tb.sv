// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

// ORIG_RENAME appends _orig inside original packed typedef names that share
// renamed module-name prefixes. Recreate those renamed typedefs so the renamed
// original control subtree elaborates.
typedef core_vpu_ctrl core_vpu_ctrl_orig;
typedef vpu_ctrl_sigs_t vpu_ctrl_orig_sigs_t;
typedef vpu_ml_load_ctrl vpu_ml_orig_load_ctrl;

typedef struct packed {
    logic id_core_valid;
    logic id_core_thread_id;
    logic [CORE_FCSR_RM_SZ-1:0] id_fcsr_rm;
    logic [$bits(minion_pkg::core_vpu_ctrl)-1:0] f0_core_ctrl;
    logic [INST_SIZE-1:0] id_core_inst_bits;
    logic [$bits(minion_pkg::vpu_ctrl_sigs_t)-1:0] id_decoder_sigs;
    logic chicken_bit_vputrans;
    logic ex_thread_id;
    logic ex_core_gscing;
    logic ex_core_kill;
    logic [VPU_LANE_NUM-1:0][1:0][VPU_DATA_S_SZ-1:0] ex_regfile_rdata_bypass;
    logic [$clog2(VPU_LANE_NUM)-1:0] ex_core_gsc_src;
    logic [XREG_SIZE-1:0] ex_core_fromint_data;
    logic f2_core_kill;
    logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] f2_fswizz_rdata;
    logic f3_core_kill;
    logic [VPU_LANE_NUM-1:0] f3_regmask_wdata_lane;
    logic f4_core_kill;
    logic [VPU_DATA_S_SZ-1:0] f8_wdata_lane0;
    logic [VPU_LANE_NUM-1:0][CORE_FCSR_FLAG_BITS_SZ-1:0] f8_wflags;
    logic [VPU_LANE_NUM-1:0] f8_txfma_comp_res;
    logic [VPU_DATA_SZ-1:0] f8_sh_sw_wdata;
    logic wb_dmem_resp_val;
    logic [$bits(minion_pkg::dcache_vpu_resp)-1:0] wb_dmem_resp;
    logic [$bits(minion_pkg::dcache_vpu_scp_resp)-1:0] dcache_scp_resp;
    logic [DCACHE_DATA_SIZE-1:0] dcache_scp_data;
    logic [DCACHE_DATA_SIZE-1:0] dcache_tenb_data;
    logic [$bits(minion_pkg::dcache_vpu_reduce_ctrl)-1:0] dcache_reduce_ctrl;
    logic [VPU_LANE_NUM-1:0][1:0] txfma_trans_dbg;
} vpu_ctrl_inputs_t;

typedef struct packed {
    logic [$bits(minion_pkg::vpu_dcache_ctrl)-1:0] dcache_ctrl;
    logic [$bits(vpu_defs_pkg::vpu_minion_id_ctrl)-1:0] id_core_ctrl;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] id_regfile_raddr1;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] id_regfile_raddr2;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] id_regfile_raddr3;
    logic [2:0] id_regfile_ren;
    logic [2:0] id_regfile_thread_id;
    logic [$bits(vpu_pkg::vpu_input_t)-1:0] ex_req;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] ex_regfile_raddr1;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] ex_regfile_raddr2;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] ex_regfile_raddr3;
    logic [2:0] ex_regfile_thread_id;
    logic [$bits(minion_pkg::vpu_minion_ex_ctrl)-1:0] ex_core_ctrl;
    logic [VPU_LANE_NUM-1:0] ex_mask_in1;
    logic ex_tena_regfile_bypass_en;
    logic ex_tenb_regfile_bypass_en;
    logic [$bits(vpu_pkg::vpu_bypass_force_ctrl_t)-1:0] ex_bypass_force_ctrl;
    logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] ex_tima_valid;
    logic ex_tima_valid_unqual;
    logic ex_tima_sa;
    logic ex_tima_sb;
    logic [VPU_TENB_ADDR_W-1:0] ex_tima_tenb_raddr;
    logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] ex_tena_rf_rdata;
    logic [VPU_DATA_S_SZ-1:0] ex_tena_rf_tima_rdata;
    logic ex_sh_sw_valid;
    logic ex_sh_sw_clock_valid;
    logic [VPU_LANE_NUM-1:0] ex_txfma_valid;
    logic ex_txfma_clock_valid;
    logic ex_rom_valid;
    logic ex_rom_clock_valid;
    logic f2_tima_ren3;
    logic [VPU_TENC_ADDR_MSB:1] f2_tima_tenc_raddr;
    logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f2_tima_tenc_wen;
    logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f2_tima_rf_wen;
    logic [$bits(minion_pkg::vpu_minion_tag_ctrl)-1:0] f2_core_ctrl;
    logic f2_tenb_regfile_wen_l;
    logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] f2_fswizz_rdata1;
    logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] f2_fswizz_rdata2;
    logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] f2_fswizz_rdata3;
    logic f3_regfile_wen_l;
    logic [VPU_DATA_SZ-1:0] f3_tenb_regfile_wdata_l;
    logic f3_tenb_regfile_wen_l;
    logic [VPU_TENB_ADDR_W-1:0] f3_tenb_regfile_waddr_l;
    logic [$bits(minion_pkg::vpu_minion_mem_ctrl)-1:0] f3_core_ctrl;
    logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f3_tima_tenc_wen;
    logic [VPU_TENC_ADDR_MSB:1] f3_tima_tenc_waddr;
    logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f3_tima_rf_wen;
    logic f3_thread_id;
    logic [VPU_LANE_NUM-1:0] f3_regfile_wmask;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f3_regfile_waddr;
    logic f3_bypass_clk_en;
    logic f3_data_vector;
    logic f4_thread_id;
    logic [VPU_LANE_NUM-1:0] f4_regfile_wmask;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f4_regfile_waddr;
    logic f4_regfile_thrid_l;
    logic [VPU_LANE_NUM-1:0][1:0] f4_regfile_wmask_l;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f4_regfile_waddr_l;
    logic [VPU_DATA_SZ-1:0] f4_regfile_wdata_l;
    logic f4_regfile_wen_l;
    logic f4_bypass_clk_en;
    logic f5_thread_id;
    logic [VPU_LANE_NUM-1:0] f5_regfile_wmask;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f5_regfile_waddr;
    logic f5_bypass_clk_en;
    logic f6_thread_id;
    logic [VPU_LANE_NUM-1:0] f6_regfile_wmask;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f6_regfile_waddr;
    logic f6_bypass_clk_en;
    logic f7_thread_id;
    logic [VPU_LANE_NUM-1:0] f7_regfile_wmask;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f7_regfile_waddr;
    logic f7_bypass_clk_en;
    logic [VPU_LANE_NUM-1:0] f7_regfile_wen;
    logic f8_thread_id;
    logic [VPU_LANE_NUM-1:0] f8_regfile_wmask;
    logic [VPU_LANE_NUM-1:0] f8_regfile_wen;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f8_regfile_waddr;
    logic f8_data_vector;
    logic f8_txfma_op;
    logic [$bits(minion_pkg::vpu_minion_wb_ctrl)-1:0] wb_core_ctrl;
    logic [CSR_NR_EVENTS_VPU-1:0] io_events;
    logic [NEIGH_DEBUG_MATCH_WIDTH-1:0] vpu_dbg_match;
    logic [NEIGH_DEBUG_FILTER_WIDTH-1:0] vpu_dbg_filter;
    logic [4:0] [NEIGH_DEBUG_DATA_WIDTH-1:0] vpu_dbg_data;
} vpu_ctrl_outputs_t;

module cosim_vpu_ctrl_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(vpu_ctrl_inputs_t)-1:0] inputs_i,
  output logic [$bits(vpu_ctrl_outputs_t)-1:0] orig_outputs_o,
  output logic [$bits(vpu_ctrl_outputs_t)-1:0] new_outputs_o
);

  vpu_ctrl_inputs_t in;
  vpu_ctrl_outputs_t orig_out;
  vpu_ctrl_outputs_t new_out;

  assign in = vpu_ctrl_inputs_t'(inputs_i);
  assign orig_outputs_o = orig_out;
  assign new_outputs_o = new_out;
  core_vpu_ctrl orig_f0_core_ctrl;
  minion_pkg::core_vpu_ctrl new_f0_core_ctrl;
  assign orig_f0_core_ctrl = core_vpu_ctrl'(in.f0_core_ctrl);
  assign new_f0_core_ctrl = minion_pkg::core_vpu_ctrl'(in.f0_core_ctrl);
  vpu_ctrl_sigs_t orig_id_decoder_sigs;
  minion_pkg::vpu_ctrl_sigs_t new_id_decoder_sigs;
  assign orig_id_decoder_sigs = vpu_ctrl_sigs_t'(in.id_decoder_sigs);
  assign new_id_decoder_sigs = minion_pkg::vpu_ctrl_sigs_t'(in.id_decoder_sigs);
  dcache_vpu_resp orig_wb_dmem_resp;
  minion_pkg::dcache_vpu_resp new_wb_dmem_resp;
  assign orig_wb_dmem_resp = dcache_vpu_resp'(in.wb_dmem_resp);
  assign new_wb_dmem_resp = minion_pkg::dcache_vpu_resp'(in.wb_dmem_resp);
  dcache_vpu_scp_resp orig_dcache_scp_resp;
  minion_pkg::dcache_vpu_scp_resp new_dcache_scp_resp;
  assign orig_dcache_scp_resp = dcache_vpu_scp_resp'(in.dcache_scp_resp);
  assign new_dcache_scp_resp = minion_pkg::dcache_vpu_scp_resp'(in.dcache_scp_resp);
  dcache_vpu_reduce_ctrl orig_dcache_reduce_ctrl;
  minion_pkg::dcache_vpu_reduce_ctrl new_dcache_reduce_ctrl;
  assign orig_dcache_reduce_ctrl = dcache_vpu_reduce_ctrl'(in.dcache_reduce_ctrl);
  assign new_dcache_reduce_ctrl = minion_pkg::dcache_vpu_reduce_ctrl'(in.dcache_reduce_ctrl);
  vpu_dcache_ctrl orig_dcache_ctrl;
  minion_pkg::vpu_dcache_ctrl new_dcache_ctrl;
  assign orig_out.dcache_ctrl = orig_dcache_ctrl;
  assign new_out.dcache_ctrl = new_dcache_ctrl;
  vpu_minion_id_ctrl orig_id_core_ctrl;
  vpu_defs_pkg::vpu_minion_id_ctrl new_id_core_ctrl;
  assign orig_out.id_core_ctrl = orig_id_core_ctrl;
  assign new_out.id_core_ctrl = new_id_core_ctrl;
  vpu_input_t orig_ex_req;
  vpu_pkg::vpu_input_t new_ex_req;
  assign orig_out.ex_req = orig_ex_req;
  assign new_out.ex_req = new_ex_req;
  vpu_minion_ex_ctrl orig_ex_core_ctrl;
  minion_pkg::vpu_minion_ex_ctrl new_ex_core_ctrl;
  assign orig_out.ex_core_ctrl = orig_ex_core_ctrl;
  assign new_out.ex_core_ctrl = new_ex_core_ctrl;
  vpu_bypass_force_ctrl orig_ex_bypass_force_ctrl;
  vpu_pkg::vpu_bypass_force_ctrl_t new_ex_bypass_force_ctrl;
  assign orig_out.ex_bypass_force_ctrl = orig_ex_bypass_force_ctrl;
  assign new_out.ex_bypass_force_ctrl = new_ex_bypass_force_ctrl;
  vpu_minion_tag_ctrl orig_f2_core_ctrl;
  minion_pkg::vpu_minion_tag_ctrl new_f2_core_ctrl;
  assign orig_out.f2_core_ctrl = orig_f2_core_ctrl;
  assign new_out.f2_core_ctrl = new_f2_core_ctrl;
  vpu_minion_mem_ctrl orig_f3_core_ctrl;
  minion_pkg::vpu_minion_mem_ctrl new_f3_core_ctrl;
  assign orig_out.f3_core_ctrl = orig_f3_core_ctrl;
  assign new_out.f3_core_ctrl = new_f3_core_ctrl;
  vpu_minion_wb_ctrl orig_wb_core_ctrl;
  minion_pkg::vpu_minion_wb_ctrl new_wb_core_ctrl;
  assign orig_out.wb_core_ctrl = orig_wb_core_ctrl;
  assign new_out.wb_core_ctrl = new_wb_core_ctrl;

  vpu_ctrl_orig u_orig (
    .clock_aon                    (clk_i),
    .clock_sec                    (clk_i),
    .reset                        (~rst_ni),
    .id_core_valid                (in.id_core_valid),
    .id_core_thread_id            (in.id_core_thread_id),
    .id_fcsr_rm                   (in.id_fcsr_rm),
    .f0_core_ctrl                 (orig_f0_core_ctrl),
    .id_core_inst_bits            (in.id_core_inst_bits),
    .id_decoder_sigs              (orig_id_decoder_sigs),
    .chicken_bit_vputrans         (in.chicken_bit_vputrans),
    .ex_thread_id                 (in.ex_thread_id),
    .ex_core_gscing               (in.ex_core_gscing),
    .ex_core_kill                 (in.ex_core_kill),
    .ex_regfile_rdata_bypass      (in.ex_regfile_rdata_bypass),
    .ex_core_gsc_src              (in.ex_core_gsc_src),
    .ex_core_fromint_data         (in.ex_core_fromint_data),
    .f2_core_kill                 (in.f2_core_kill),
    .f2_fswizz_rdata              (in.f2_fswizz_rdata),
    .f3_core_kill                 (in.f3_core_kill),
    .f3_regmask_wdata_lane        (in.f3_regmask_wdata_lane),
    .f4_core_kill                 (in.f4_core_kill),
    .f8_wdata_lane0               (in.f8_wdata_lane0),
    .f8_wflags                    (in.f8_wflags),
    .f8_txfma_comp_res            (in.f8_txfma_comp_res),
    .f8_sh_sw_wdata               (in.f8_sh_sw_wdata),
    .wb_dmem_resp_val             (in.wb_dmem_resp_val),
    .wb_dmem_resp                 (orig_wb_dmem_resp),
    .dcache_scp_resp              (orig_dcache_scp_resp),
    .dcache_scp_data              (in.dcache_scp_data),
    .dcache_tenb_data             (in.dcache_tenb_data),
    .dcache_reduce_ctrl           (orig_dcache_reduce_ctrl),
    .txfma_trans_dbg              (in.txfma_trans_dbg),
    .dcache_ctrl                  (orig_dcache_ctrl),
    .id_core_ctrl                 (orig_id_core_ctrl),
    .id_regfile_raddr1            (orig_out.id_regfile_raddr1),
    .id_regfile_raddr2            (orig_out.id_regfile_raddr2),
    .id_regfile_raddr3            (orig_out.id_regfile_raddr3),
    .id_regfile_ren               (orig_out.id_regfile_ren),
    .id_regfile_thread_id         (orig_out.id_regfile_thread_id),
    .ex_req                       (orig_ex_req),
    .ex_regfile_raddr1            (orig_out.ex_regfile_raddr1),
    .ex_regfile_raddr2            (orig_out.ex_regfile_raddr2),
    .ex_regfile_raddr3            (orig_out.ex_regfile_raddr3),
    .ex_regfile_thread_id         (orig_out.ex_regfile_thread_id),
    .ex_core_ctrl                 (orig_ex_core_ctrl),
    .ex_mask_in1                  (orig_out.ex_mask_in1),
    .ex_tena_regfile_bypass_en    (orig_out.ex_tena_regfile_bypass_en),
    .ex_tenb_regfile_bypass_en    (orig_out.ex_tenb_regfile_bypass_en),
    .ex_bypass_force_ctrl         (orig_ex_bypass_force_ctrl),
    .ex_tima_valid                (orig_out.ex_tima_valid),
    .ex_tima_valid_unqual         (orig_out.ex_tima_valid_unqual),
    .ex_tima_sa                   (orig_out.ex_tima_sa),
    .ex_tima_sb                   (orig_out.ex_tima_sb),
    .ex_tima_tenb_raddr           (orig_out.ex_tima_tenb_raddr),
    .ex_tena_rf_rdata             (orig_out.ex_tena_rf_rdata),
    .ex_tena_rf_tima_rdata        (orig_out.ex_tena_rf_tima_rdata),
    .ex_sh_sw_valid               (orig_out.ex_sh_sw_valid),
    .ex_sh_sw_clock_valid         (orig_out.ex_sh_sw_clock_valid),
    .ex_txfma_valid               (orig_out.ex_txfma_valid),
    .ex_txfma_clock_valid         (orig_out.ex_txfma_clock_valid),
    .ex_rom_valid                 (orig_out.ex_rom_valid),
    .ex_rom_clock_valid           (orig_out.ex_rom_clock_valid),
    .f2_tima_ren3                 (orig_out.f2_tima_ren3),
    .f2_tima_tenc_raddr           (orig_out.f2_tima_tenc_raddr),
    .f2_tima_tenc_wen             (orig_out.f2_tima_tenc_wen),
    .f2_tima_rf_wen               (orig_out.f2_tima_rf_wen),
    .f2_core_ctrl                 (orig_f2_core_ctrl),
    .f2_tenb_regfile_wen_l        (orig_out.f2_tenb_regfile_wen_l),
    .f2_fswizz_rdata1             (orig_out.f2_fswizz_rdata1),
    .f2_fswizz_rdata2             (orig_out.f2_fswizz_rdata2),
    .f2_fswizz_rdata3             (orig_out.f2_fswizz_rdata3),
    .f3_regfile_wen_l             (orig_out.f3_regfile_wen_l),
    .f3_tenb_regfile_wdata_l      (orig_out.f3_tenb_regfile_wdata_l),
    .f3_tenb_regfile_wen_l        (orig_out.f3_tenb_regfile_wen_l),
    .f3_tenb_regfile_waddr_l      (orig_out.f3_tenb_regfile_waddr_l),
    .f3_core_ctrl                 (orig_f3_core_ctrl),
    .f3_tima_tenc_wen             (orig_out.f3_tima_tenc_wen),
    .f3_tima_tenc_waddr           (orig_out.f3_tima_tenc_waddr),
    .f3_tima_rf_wen               (orig_out.f3_tima_rf_wen),
    .f3_thread_id                 (orig_out.f3_thread_id),
    .f3_regfile_wmask             (orig_out.f3_regfile_wmask),
    .f3_regfile_waddr             (orig_out.f3_regfile_waddr),
    .f3_bypass_clk_en             (orig_out.f3_bypass_clk_en),
    .f3_data_vector               (orig_out.f3_data_vector),
    .f4_thread_id                 (orig_out.f4_thread_id),
    .f4_regfile_wmask             (orig_out.f4_regfile_wmask),
    .f4_regfile_waddr             (orig_out.f4_regfile_waddr),
    .f4_regfile_thrid_l           (orig_out.f4_regfile_thrid_l),
    .f4_regfile_wmask_l           (orig_out.f4_regfile_wmask_l),
    .f4_regfile_waddr_l           (orig_out.f4_regfile_waddr_l),
    .f4_regfile_wdata_l           (orig_out.f4_regfile_wdata_l),
    .f4_regfile_wen_l             (orig_out.f4_regfile_wen_l),
    .f4_bypass_clk_en             (orig_out.f4_bypass_clk_en),
    .f5_thread_id                 (orig_out.f5_thread_id),
    .f5_regfile_wmask             (orig_out.f5_regfile_wmask),
    .f5_regfile_waddr             (orig_out.f5_regfile_waddr),
    .f5_bypass_clk_en             (orig_out.f5_bypass_clk_en),
    .f6_thread_id                 (orig_out.f6_thread_id),
    .f6_regfile_wmask             (orig_out.f6_regfile_wmask),
    .f6_regfile_waddr             (orig_out.f6_regfile_waddr),
    .f6_bypass_clk_en             (orig_out.f6_bypass_clk_en),
    .f7_thread_id                 (orig_out.f7_thread_id),
    .f7_regfile_wmask             (orig_out.f7_regfile_wmask),
    .f7_regfile_waddr             (orig_out.f7_regfile_waddr),
    .f7_bypass_clk_en             (orig_out.f7_bypass_clk_en),
    .f7_regfile_wen               (orig_out.f7_regfile_wen),
    .f8_thread_id                 (orig_out.f8_thread_id),
    .f8_regfile_wmask             (orig_out.f8_regfile_wmask),
    .f8_regfile_wen               (orig_out.f8_regfile_wen),
    .f8_regfile_waddr             (orig_out.f8_regfile_waddr),
    .f8_data_vector               (orig_out.f8_data_vector),
    .f8_txfma_op                  (orig_out.f8_txfma_op),
    .wb_core_ctrl                 (orig_wb_core_ctrl),
    .io_events                    (orig_out.io_events),
    .vpu_dbg_match                (orig_out.vpu_dbg_match),
    .vpu_dbg_filter               (orig_out.vpu_dbg_filter),
    .vpu_dbg_data                 (orig_out.vpu_dbg_data)
  );

  vpu_ctrl u_new (
    .clock_aon                    (clk_i),
    .clock_sec                    (clk_i),
    .reset                        (~rst_ni),
    .id_core_valid                (in.id_core_valid),
    .id_core_thread_id            (in.id_core_thread_id),
    .id_fcsr_rm                   (in.id_fcsr_rm),
    .f0_core_ctrl                 (new_f0_core_ctrl),
    .id_core_inst_bits            (in.id_core_inst_bits),
    .id_decoder_sigs              (new_id_decoder_sigs),
    .chicken_bit_vputrans         (in.chicken_bit_vputrans),
    .ex_thread_id                 (in.ex_thread_id),
    .ex_core_gscing               (in.ex_core_gscing),
    .ex_core_kill                 (in.ex_core_kill),
    .ex_regfile_rdata_bypass      (in.ex_regfile_rdata_bypass),
    .ex_core_gsc_src              (in.ex_core_gsc_src),
    .ex_core_fromint_data         (in.ex_core_fromint_data),
    .f2_core_kill                 (in.f2_core_kill),
    .f2_fswizz_rdata              (in.f2_fswizz_rdata),
    .f3_core_kill                 (in.f3_core_kill),
    .f3_regmask_wdata_lane        (in.f3_regmask_wdata_lane),
    .f4_core_kill                 (in.f4_core_kill),
    .f8_wdata_lane0               (in.f8_wdata_lane0),
    .f8_wflags                    (in.f8_wflags),
    .f8_txfma_comp_res            (in.f8_txfma_comp_res),
    .f8_sh_sw_wdata               (in.f8_sh_sw_wdata),
    .wb_dmem_resp_val             (in.wb_dmem_resp_val),
    .wb_dmem_resp                 (new_wb_dmem_resp),
    .dcache_scp_resp              (new_dcache_scp_resp),
    .dcache_scp_data              (in.dcache_scp_data),
    .dcache_tenb_data             (in.dcache_tenb_data),
    .dcache_reduce_ctrl           (new_dcache_reduce_ctrl),
    .txfma_trans_dbg              (in.txfma_trans_dbg),
    .dcache_ctrl                  (new_dcache_ctrl),
    .id_core_ctrl                 (new_id_core_ctrl),
    .id_regfile_raddr1            (new_out.id_regfile_raddr1),
    .id_regfile_raddr2            (new_out.id_regfile_raddr2),
    .id_regfile_raddr3            (new_out.id_regfile_raddr3),
    .id_regfile_ren               (new_out.id_regfile_ren),
    .id_regfile_thread_id         (new_out.id_regfile_thread_id),
    .ex_req                       (new_ex_req),
    .ex_regfile_raddr1            (new_out.ex_regfile_raddr1),
    .ex_regfile_raddr2            (new_out.ex_regfile_raddr2),
    .ex_regfile_raddr3            (new_out.ex_regfile_raddr3),
    .ex_regfile_thread_id         (new_out.ex_regfile_thread_id),
    .ex_core_ctrl                 (new_ex_core_ctrl),
    .ex_mask_in1                  (new_out.ex_mask_in1),
    .ex_tena_regfile_bypass_en    (new_out.ex_tena_regfile_bypass_en),
    .ex_tenb_regfile_bypass_en    (new_out.ex_tenb_regfile_bypass_en),
    .ex_bypass_force_ctrl         (new_ex_bypass_force_ctrl),
    .ex_tima_valid                (new_out.ex_tima_valid),
    .ex_tima_valid_unqual         (new_out.ex_tima_valid_unqual),
    .ex_tima_sa                   (new_out.ex_tima_sa),
    .ex_tima_sb                   (new_out.ex_tima_sb),
    .ex_tima_tenb_raddr           (new_out.ex_tima_tenb_raddr),
    .ex_tena_rf_rdata             (new_out.ex_tena_rf_rdata),
    .ex_tena_rf_tima_rdata        (new_out.ex_tena_rf_tima_rdata),
    .ex_sh_sw_valid               (new_out.ex_sh_sw_valid),
    .ex_sh_sw_clock_valid         (new_out.ex_sh_sw_clock_valid),
    .ex_txfma_valid               (new_out.ex_txfma_valid),
    .ex_txfma_clock_valid         (new_out.ex_txfma_clock_valid),
    .ex_rom_valid                 (new_out.ex_rom_valid),
    .ex_rom_clock_valid           (new_out.ex_rom_clock_valid),
    .f2_tima_ren3                 (new_out.f2_tima_ren3),
    .f2_tima_tenc_raddr           (new_out.f2_tima_tenc_raddr),
    .f2_tima_tenc_wen             (new_out.f2_tima_tenc_wen),
    .f2_tima_rf_wen               (new_out.f2_tima_rf_wen),
    .f2_core_ctrl                 (new_f2_core_ctrl),
    .f2_tenb_regfile_wen_l        (new_out.f2_tenb_regfile_wen_l),
    .f2_fswizz_rdata1             (new_out.f2_fswizz_rdata1),
    .f2_fswizz_rdata2             (new_out.f2_fswizz_rdata2),
    .f2_fswizz_rdata3             (new_out.f2_fswizz_rdata3),
    .f3_regfile_wen_l             (new_out.f3_regfile_wen_l),
    .f3_tenb_regfile_wdata_l      (new_out.f3_tenb_regfile_wdata_l),
    .f3_tenb_regfile_wen_l        (new_out.f3_tenb_regfile_wen_l),
    .f3_tenb_regfile_waddr_l      (new_out.f3_tenb_regfile_waddr_l),
    .f3_core_ctrl                 (new_f3_core_ctrl),
    .f3_tima_tenc_wen             (new_out.f3_tima_tenc_wen),
    .f3_tima_tenc_waddr           (new_out.f3_tima_tenc_waddr),
    .f3_tima_rf_wen               (new_out.f3_tima_rf_wen),
    .f3_thread_id                 (new_out.f3_thread_id),
    .f3_regfile_wmask             (new_out.f3_regfile_wmask),
    .f3_regfile_waddr             (new_out.f3_regfile_waddr),
    .f3_bypass_clk_en             (new_out.f3_bypass_clk_en),
    .f3_data_vector               (new_out.f3_data_vector),
    .f4_thread_id                 (new_out.f4_thread_id),
    .f4_regfile_wmask             (new_out.f4_regfile_wmask),
    .f4_regfile_waddr             (new_out.f4_regfile_waddr),
    .f4_regfile_thrid_l           (new_out.f4_regfile_thrid_l),
    .f4_regfile_wmask_l           (new_out.f4_regfile_wmask_l),
    .f4_regfile_waddr_l           (new_out.f4_regfile_waddr_l),
    .f4_regfile_wdata_l           (new_out.f4_regfile_wdata_l),
    .f4_regfile_wen_l             (new_out.f4_regfile_wen_l),
    .f4_bypass_clk_en             (new_out.f4_bypass_clk_en),
    .f5_thread_id                 (new_out.f5_thread_id),
    .f5_regfile_wmask             (new_out.f5_regfile_wmask),
    .f5_regfile_waddr             (new_out.f5_regfile_waddr),
    .f5_bypass_clk_en             (new_out.f5_bypass_clk_en),
    .f6_thread_id                 (new_out.f6_thread_id),
    .f6_regfile_wmask             (new_out.f6_regfile_wmask),
    .f6_regfile_waddr             (new_out.f6_regfile_waddr),
    .f6_bypass_clk_en             (new_out.f6_bypass_clk_en),
    .f7_thread_id                 (new_out.f7_thread_id),
    .f7_regfile_wmask             (new_out.f7_regfile_wmask),
    .f7_regfile_waddr             (new_out.f7_regfile_waddr),
    .f7_bypass_clk_en             (new_out.f7_bypass_clk_en),
    .f7_regfile_wen               (new_out.f7_regfile_wen),
    .f8_thread_id                 (new_out.f8_thread_id),
    .f8_regfile_wmask             (new_out.f8_regfile_wmask),
    .f8_regfile_wen               (new_out.f8_regfile_wen),
    .f8_regfile_waddr             (new_out.f8_regfile_waddr),
    .f8_data_vector               (new_out.f8_data_vector),
    .f8_txfma_op                  (new_out.f8_txfma_op),
    .wb_core_ctrl                 (new_wb_core_ctrl),
    .io_events                    (new_out.io_events),
    .vpu_dbg_match                (new_out.vpu_dbg_match),
    .vpu_dbg_filter               (new_out.vpu_dbg_filter),
    .vpu_dbg_data                 (new_out.vpu_dbg_data)
  );

endmodule
