// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

module vpu_ctrl_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic id_core_valid_i,
  input  logic id_core_thread_id_i,
  input  logic [2:0] ren_mask_i,
  input  logic swap12_i,
  input  logic swap13_i,
  input  logic swap23_i,
  input  logic txfma_i,
  input  logic rom_i,
  input  logic fromint_i,
  input  logic maskop_i,
  input  logic wen_i,
  input  logic wb_dmem_resp_val_i,
  input  logic [FREG_ADDR_SIZE-1:0] ra1_i,
  input  logic [FREG_ADDR_SIZE-1:0] ra2_i,
  input  logic [FREG_ADDR_SIZE-1:0] ra3_i,
  output logic [2:0] id_regfile_ren_o,
  output logic [2:0] id_regfile_thread_id_o,
  output logic [FREG_ADDR_SIZE-1:0] id_regfile_raddr1_o,
  output logic [FREG_ADDR_SIZE-1:0] id_regfile_raddr2_o,
  output logic [FREG_ADDR_SIZE-1:0] id_regfile_raddr3_o,
  output logic ex_sh_sw_valid_o,
  output logic ex_sh_sw_clock_valid_o,
  output logic ex_txfma_clock_valid_o,
  output logic ex_txfma_valid_any_o,
  output logic ex_rom_valid_o,
  output logic ex_rom_clock_valid_o,
  output logic ex_req_fromint_o,
  output logic f3_regfile_wen_l_o,
  output logic [CSR_NR_EVENTS_VPU-1:0] io_events_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // Full child/control structs are connected to preserve the real boundary; only selected probes are checked here.
  core_vpu_ctrl f0_core_ctrl;
  vpu_ctrl_sigs_t id_decoder_sigs;
  logic [INST_SIZE-1:0] id_core_inst_bits;
  dcache_vpu_resp wb_dmem_resp;
  dcache_vpu_scp_resp dcache_scp_resp;
  dcache_vpu_reduce_ctrl dcache_reduce_ctrl;
  vpu_dcache_ctrl dcache_ctrl;
  vpu_minion_id_ctrl id_core_ctrl;
  vpu_input_t ex_req;
  vpu_minion_ex_ctrl ex_core_ctrl;
  vpu_bypass_force_ctrl ex_bypass_force_ctrl;
  vpu_minion_tag_ctrl f2_core_ctrl;
  vpu_minion_mem_ctrl f3_core_ctrl;
  vpu_minion_wb_ctrl wb_core_ctrl;
  logic [VPU_LANE_NUM-1:0] ex_txfma_valid_by_lane;

  assign ex_txfma_valid_any_o = |ex_txfma_valid_by_lane;
  assign ex_req_fromint_o = ex_req.sigs.fromint;

  always_comb begin
    f0_core_ctrl = '0;
    id_decoder_sigs = '0;
    id_decoder_sigs.cmd = VpuCmdAdd;
    id_decoder_sigs.dtype = VpuDtypeF32;
    id_decoder_sigs.ren1 = ren_mask_i[0];
    id_decoder_sigs.ren2 = ren_mask_i[1];
    id_decoder_sigs.ren3 = ren_mask_i[2];
    id_decoder_sigs.swap12 = swap12_i;
    id_decoder_sigs.swap13 = swap13_i;
    id_decoder_sigs.swap23 = swap23_i;
    id_decoder_sigs.txfma = txfma_i;
    id_decoder_sigs.rom = rom_i;
    id_decoder_sigs.fromint = fromint_i;
    id_decoder_sigs.maskop = maskop_i;
    id_decoder_sigs.wen = wen_i;
    id_decoder_sigs.vector = 1'b0;
    id_decoder_sigs.shsw = 1'b1;

    id_core_inst_bits = '0;
    id_core_inst_bits[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = ra1_i;
    id_core_inst_bits[VPU_INST_REN2_RA_SEL_MSB:VPU_INST_REN2_RA_SEL_LSB] = ra2_i;
    id_core_inst_bits[VPU_INST_REN3_RA_SEL_MSB:VPU_INST_REN3_RA_SEL_LSB] = ra3_i;

    wb_dmem_resp = '0;
    wb_dmem_resp.ps_mask = '1;
    wb_dmem_resp.data = 256'h0123_4567_89ab_cdef_0011_2233_4455_6677_8899_aabb_ccdd_eeff_1357_2468_dead_beef;
    dcache_scp_resp = '0;
    dcache_reduce_ctrl = '0;
  end

  /* verilator lint_off PINCONNECTEMPTY */  // Unit TB observes selected VPU-control outputs and leaves the rest intentionally unconnected.
  vpu_ctrl dut (
    .clock_aon                 (clk_i),
    .clock_sec                 (clk_i),
    .reset                     (~rst_ni),
    .id_core_valid             (id_core_valid_i),
    .id_core_thread_id         (id_core_thread_id_i),
    .id_fcsr_rm                ('0),
    .f0_core_ctrl              (f0_core_ctrl),
    .id_core_inst_bits         (id_core_inst_bits),
    .id_decoder_sigs           (id_decoder_sigs),
    .chicken_bit_vputrans      (1'b1),
    .ex_thread_id              (1'b0),
    .ex_core_gscing            (1'b0),
    .ex_core_kill              (1'b0),
    .ex_regfile_rdata_bypass   ('0),
    .ex_core_gsc_src           ('0),
    .ex_core_fromint_data      ('0),
    .f2_core_kill              (1'b0),
    .f2_fswizz_rdata           ('0),
    .f3_core_kill              (1'b0),
    .f3_regmask_wdata_lane     ('0),
    .f4_core_kill              (1'b0),
    .f8_wdata_lane0            ('0),
    .f8_wflags                 ('0),
    .f8_txfma_comp_res         ('0),
    .f8_sh_sw_wdata            ('0),
    .wb_dmem_resp_val          (wb_dmem_resp_val_i),
    .wb_dmem_resp              (wb_dmem_resp),
    .dcache_scp_resp           (dcache_scp_resp),
    .dcache_scp_data           ('0),
    .dcache_tenb_data          ('0),
    .dcache_reduce_ctrl        (dcache_reduce_ctrl),
    .txfma_trans_dbg           ('0),
    .dcache_ctrl               (dcache_ctrl),
    .id_core_ctrl              (id_core_ctrl),
    .id_regfile_raddr1         (id_regfile_raddr1_o),
    .id_regfile_raddr2         (id_regfile_raddr2_o),
    .id_regfile_raddr3         (id_regfile_raddr3_o),
    .id_regfile_ren            (id_regfile_ren_o),
    .id_regfile_thread_id      (id_regfile_thread_id_o),
    .ex_req                    (ex_req),
    .ex_regfile_raddr1         (),
    .ex_regfile_raddr2         (),
    .ex_regfile_raddr3         (),
    .ex_regfile_thread_id      (),
    .ex_core_ctrl              (ex_core_ctrl),
    .ex_mask_in1               (),
    .ex_tena_regfile_bypass_en (),
    .ex_tenb_regfile_bypass_en (),
    .ex_bypass_force_ctrl      (ex_bypass_force_ctrl),
    .ex_tima_valid             (),
    .ex_tima_valid_unqual      (),
    .ex_tima_sa                (),
    .ex_tima_sb                (),
    .ex_tima_tenb_raddr        (),
    .ex_tena_rf_rdata          (),
    .ex_tena_rf_tima_rdata     (),
    .ex_sh_sw_valid            (ex_sh_sw_valid_o),
    .ex_sh_sw_clock_valid      (ex_sh_sw_clock_valid_o),
    .ex_txfma_valid            (ex_txfma_valid_by_lane),
    .ex_txfma_clock_valid      (ex_txfma_clock_valid_o),
    .ex_rom_valid              (ex_rom_valid_o),
    .ex_rom_clock_valid        (ex_rom_clock_valid_o),
    .f2_tima_ren3              (),
    .f2_tima_tenc_raddr        (),
    .f2_tima_tenc_wen          (),
    .f2_tima_rf_wen            (),
    .f2_core_ctrl              (f2_core_ctrl),
    .f2_tenb_regfile_wen_l     (),
    .f2_fswizz_rdata1          (),
    .f2_fswizz_rdata2          (),
    .f2_fswizz_rdata3          (),
    .f3_regfile_wen_l          (f3_regfile_wen_l_o),
    .f3_tenb_regfile_wdata_l   (),
    .f3_tenb_regfile_wen_l     (),
    .f3_tenb_regfile_waddr_l   (),
    .f3_core_ctrl              (f3_core_ctrl),
    .f3_tima_tenc_wen          (),
    .f3_tima_tenc_waddr        (),
    .f3_tima_rf_wen            (),
    .f3_thread_id              (),
    .f3_regfile_wmask          (),
    .f3_regfile_waddr          (),
    .f3_bypass_clk_en          (),
    .f3_data_vector            (),
    .f4_thread_id              (),
    .f4_regfile_wmask          (),
    .f4_regfile_waddr          (),
    .f4_regfile_thrid_l        (),
    .f4_regfile_wmask_l        (),
    .f4_regfile_waddr_l        (),
    .f4_regfile_wdata_l        (),
    .f4_regfile_wen_l          (),
    .f4_bypass_clk_en          (),
    .f5_thread_id              (),
    .f5_regfile_wmask          (),
    .f5_regfile_waddr          (),
    .f5_bypass_clk_en          (),
    .f6_thread_id              (),
    .f6_regfile_wmask          (),
    .f6_regfile_waddr          (),
    .f6_bypass_clk_en          (),
    .f7_thread_id              (),
    .f7_regfile_wmask          (),
    .f7_regfile_waddr          (),
    .f7_bypass_clk_en          (),
    .f7_regfile_wen            (),
    .f8_thread_id              (),
    .f8_regfile_wmask          (),
    .f8_regfile_wen            (),
    .f8_regfile_waddr          (),
    .f8_data_vector            (),
    .f8_txfma_op               (),
    .wb_core_ctrl              (wb_core_ctrl),
    .io_events                 (io_events_o),
    .vpu_dbg_match             (),
    .vpu_dbg_filter            (),
    .vpu_dbg_data              ()
  );
  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_on PINCONNECTEMPTY */

endmodule
