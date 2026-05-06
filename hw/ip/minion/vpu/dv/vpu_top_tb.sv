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

module vpu_top_tb (
  input  logic                              clk_i,
  input  logic                              rst_ni,
  input  logic                              id_core_valid_i,
  input  logic                              id_core_thread_id_i,
  input  logic [2:0]                        ren_mask_i,
  input  logic                              shsw_i,
  input  logic                              txfma_i,
  input  logic                              rom_i,
  input  logic                              trans_i,
  input  logic                              fromint_i,
  input  logic                              maskop_i,
  input  logic                              wen_i,
  input  logic                              swap12_i,
  input  logic                              tensorfma_start_i,
  input  logic                              tensorquant_start_i,
  input  logic                              reduce_start_i,
  input  logic                              tensorstore_start_i,
  input  logic                              wb_dcache_resp_valid_i,
  input  logic                              dcache_scp_fill_tenb_i,
  input  logic                              dcache_reduce_send_reg_i,
  input  logic                              dcache_reduce_exec_op_i,
  input  logic                              ex_core_kill_i,
  input  logic                              f2_core_kill_i,
  input  logic                              f3_core_kill_i,
  input  logic                              f4_core_kill_i,
  input  logic                              chicken_vpulane_i,
  input  logic                              chicken_vputima_i,
  input  logic                              chicken_vputrans_i,
  input  logic [FREG_ADDR_SIZE-1:0]          ra1_i,
  input  logic [FREG_ADDR_SIZE-1:0]          ra2_i,
  input  logic [FREG_ADDR_SIZE-1:0]          ra3_i,
  input  logic [FREG_ADDR_SIZE-1:0]          rd_i,
  input  logic [XREG_SIZE-1:0]               fromint_data_i,
  output logic [2:0]                        id_core_ctrl_ren_o,
  output logic                              id_core_ctrl_wen_o,
  output logic                              id_core_ctrl_fromint_o,
  output logic                              id_core_ctrl_tfma_enabled_o,
  output logic                              id_core_ctrl_tquant_enabled_o,
  output logic                              id_core_ctrl_reduce_enabled_o,
  output logic                              id_core_ctrl_fflags_stall_o,
  output logic                              ex_core_ctrl_tointm_o,
  output logic                              f2_core_ctrl_fma_o,
  output logic                              f3_core_ctrl_fma_o,
  output logic                              wb_core_ctrl_fma_o,
  output logic                              wb_core_ctrl_fcsr_flags_valid_o,
  output logic                              dcache_ctrl_read_en_o,
  output logic                              dcache_ctrl_tenb_credit_o,
  output logic                              dcache_ctrl_tfma_enabled_o,
  output logic                              dcache_ctrl_reduce_wait_o,
  output logic                              dcache_ctrl_tfma_rf_write_o,
  output logic                              ex_sh_sw_valid_o,
  output logic                              ex_sh_sw_clock_valid_o,
  output logic                              ex_txfma_valid_any_o,
  output logic                              ex_txfma_clock_valid_o,
  output logic                              ex_rom_valid_o,
  output logic                              ex_rom_clock_valid_o,
  output logic                              ex_tima_valid_any_o,
  output logic [CSR_NR_EVENTS_VPU-1:0]       io_events_o,
  output logic [NEIGH_DEBUG_MATCH_WIDTH-1:0] vpu_dbg_match_o,
  output logic [NEIGH_DEBUG_FILTER_WIDTH-1:0] vpu_dbg_filter_o,
  output logic                              vpu_dbg_data_nonzero_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // Unit TB checks selected top-level probes while preserving the complete VPU boundary structs.
  minion_vpu_id_req      id_core_req;
  vpu_ctrl_sigs_t        id_vpu_decoder_sigs;
  minion_vpu_ex_req      ex_core_req;
  core_vpu_ctrl          f0_core_ctrl;
  dcache_vpu_resp        wb_dcache_resp;
  dcache_vpu_scp_resp    dcache_scp_resp;
  dcache_vpu_reduce_ctrl dcache_reduce_ctrl;

  vpu_dcache_ctrl        dcache_ctrl;
  vpu_minion_id_ctrl     id_core_ctrl;
  vpu_minion_ex_ctrl     ex_core_ctrl;
  vpu_minion_tag_ctrl    f2_core_ctrl;
  vpu_minion_mem_ctrl    f3_core_ctrl;
  vpu_minion_wb_ctrl     wb_core_ctrl;
  logic [4:0][NEIGH_DEBUG_DATA_WIDTH-1:0] vpu_dbg_data;

  assign id_core_ctrl_ren_o              = {id_core_ctrl.ren3, id_core_ctrl.ren2, id_core_ctrl.ren1};
  assign id_core_ctrl_wen_o              = id_core_ctrl.wen;
  assign id_core_ctrl_fromint_o          = id_core_ctrl.fromint;
  assign id_core_ctrl_tfma_enabled_o     = id_core_ctrl.tfma_enabled;
  assign id_core_ctrl_tquant_enabled_o   = id_core_ctrl.tquant_enabled;
  assign id_core_ctrl_reduce_enabled_o   = id_core_ctrl.reduce_enabled;
  assign id_core_ctrl_fflags_stall_o     = id_core_ctrl.fflags_stall;
  assign ex_core_ctrl_tointm_o           = ex_core_ctrl.tointm;
  assign f2_core_ctrl_fma_o              = f2_core_ctrl.fma;
  assign f3_core_ctrl_fma_o              = f3_core_ctrl.fma;
  assign wb_core_ctrl_fma_o              = wb_core_ctrl.fma;
  assign wb_core_ctrl_fcsr_flags_valid_o = wb_core_ctrl.fcsr_flags_valid;
  assign dcache_ctrl_read_en_o           = dcache_ctrl.scp_req.read_en;
  assign dcache_ctrl_tenb_credit_o       = dcache_ctrl.tenb_credit;
  assign dcache_ctrl_tfma_enabled_o      = dcache_ctrl.tfma_enabled;
  assign dcache_ctrl_reduce_wait_o       = dcache_ctrl.reduce_wait;
  assign dcache_ctrl_tfma_rf_write_o     = dcache_ctrl.tfma_rf_write;
  assign vpu_dbg_data_nonzero_o          = |vpu_dbg_data;

  always_comb begin
    id_core_req = '0;
    id_core_req.valid = id_core_valid_i;
    id_core_req.thread_id = id_core_thread_id_i;
    id_core_req.fcsr_rm = RNE;
    id_core_req.inst_bits[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = ra1_i;
    id_core_req.inst_bits[VPU_INST_REN2_RA_SEL_MSB:VPU_INST_REN2_RA_SEL_LSB] = ra2_i;
    id_core_req.inst_bits[VPU_INST_REN3_RA_SEL_MSB:VPU_INST_REN3_RA_SEL_LSB] = ra3_i;
    id_core_req.inst_bits[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB] = rd_i;

    id_vpu_decoder_sigs = '0;
    id_vpu_decoder_sigs.cmd = txfma_i ? VpuCmdMadd : (rom_i ? VpuTransRcpRr : VpuCmdAdd);
    id_vpu_decoder_sigs.dtype = VpuDtypeF32;
    id_vpu_decoder_sigs.ren1 = ren_mask_i[0];
    id_vpu_decoder_sigs.ren2 = ren_mask_i[1];
    id_vpu_decoder_sigs.ren3 = ren_mask_i[2];
    id_vpu_decoder_sigs.rend = wen_i;
    id_vpu_decoder_sigs.wen = wen_i;
    id_vpu_decoder_sigs.shsw = shsw_i;
    id_vpu_decoder_sigs.txfma = txfma_i;
    id_vpu_decoder_sigs.rom = rom_i;
    id_vpu_decoder_sigs.trans = trans_i;
    id_vpu_decoder_sigs.fromint = fromint_i;
    id_vpu_decoder_sigs.maskop = maskop_i;
    id_vpu_decoder_sigs.swap12 = swap12_i;
    id_vpu_decoder_sigs.vector = 1'b1;
    id_vpu_decoder_sigs.fma = txfma_i;
    id_vpu_decoder_sigs.add = shsw_i;
    id_vpu_decoder_sigs.wflags = txfma_i;

    ex_core_req = '0;
    ex_core_req.thread_id = id_core_thread_id_i;
    ex_core_req.gscing = tensorfma_start_i | tensorquant_start_i | reduce_start_i | tensorstore_start_i;
    ex_core_req.gsc_src = 3'd2;
    ex_core_req.kill = ex_core_kill_i;
    ex_core_req.fromint_data = fromint_data_i;

    f0_core_ctrl = '0;
    f0_core_ctrl.tensorfma_start = tensorfma_start_i;
    f0_core_ctrl.tensorfma_ctrl.start_a = rd_i[3:0];
    f0_core_ctrl.tensorfma_ctrl.rows_a = 4'd1;
    f0_core_ctrl.tensorfma_ctrl.cols_a = 4'd1;
    f0_core_ctrl.tensorfma_ctrl.cols_b = 2'd1;
    f0_core_ctrl.tensorfma_ctrl.scp_a = 6'd1;
    f0_core_ctrl.tensorfma_ctrl.scp_b = 6'd2;
    f0_core_ctrl.tensorfma_ctrl.mode = 3'd1;
    f0_core_ctrl.tensorfma_ctrl.first_pass = 1'b1;
    f0_core_ctrl.tensorfma_ctrl.to_vrf = 1'b1;
    f0_core_ctrl.tensorquant_start = tensorquant_start_i;
    f0_core_ctrl.tensorquant_ctrl.start_reg = rd_i;
    f0_core_ctrl.tensorquant_ctrl.rows = 4'd1;
    f0_core_ctrl.tensorquant_ctrl.cols = 2'd1;
    f0_core_ctrl.tensorquant_ctrl.scp_src = 6'd3;
    f0_core_ctrl.reduce_start = reduce_start_i;
    f0_core_ctrl.tensorstore_start = tensorstore_start_i;
    f0_core_ctrl.reduce_ctrl.reduce.start_reg = rd_i;
    f0_core_ctrl.reduce_ctrl.reduce.num_regs = 7'd1;
    f0_core_ctrl.reduce_ctrl.reduce.action = 2'd1;
    f0_core_ctrl.pending_scoreboard_t0 = 1'b0;

    wb_dcache_resp = '0;
    wb_dcache_resp.thread_id = id_core_thread_id_i;
    wb_dcache_resp.gdst = 3'd1;
    wb_dcache_resp.addr = rd_i;
    wb_dcache_resp.data = 256'h0123_4567_89ab_cdef_0011_2233_4455_6677_8899_aabb_ccdd_eeff_1357_2468_dead_beef;
    wb_dcache_resp.ps_mask = '1;

    dcache_scp_resp = '0;
    dcache_scp_resp.fill_is_tenb_early = dcache_scp_fill_tenb_i;
    dcache_scp_resp.fill_is_tenb = dcache_scp_fill_tenb_i;
    dcache_scp_resp.tenb_line = 4'd3;

    dcache_reduce_ctrl = '0;
    dcache_reduce_ctrl.send_reg = dcache_reduce_send_reg_i;
    dcache_reduce_ctrl.exec_op = dcache_reduce_exec_op_i;
    dcache_reduce_ctrl.nothing = ~(dcache_reduce_send_reg_i | dcache_reduce_exec_op_i);
  end

  vpu_top dut (
    .clock                 (clk_i),
    .reset                 (~rst_ni),
    .id_core_req           (id_core_req),
    .id_vpu_decoder_sigs   (id_vpu_decoder_sigs),
    .ex_core_req           (ex_core_req),
    .f2_core_kill          (f2_core_kill_i),
    .f3_core_kill          (f3_core_kill_i),
    .f4_core_kill          (f4_core_kill_i),
    .f0_core_ctrl          (f0_core_ctrl),
    .chicken_bit_vpulane   (chicken_vpulane_i),
    .chicken_bit_vputima   (chicken_vputima_i),
    .chicken_bit_vputrans  (chicken_vputrans_i),
    .mem_ctrl_override     ('0),
    .wb_dcache_resp_valid  (wb_dcache_resp_valid_i),
    .wb_dcache_resp        (wb_dcache_resp),
    .dcache_ctrl           (dcache_ctrl),
    .dcache_scp_resp       (dcache_scp_resp),
    .dcache_scp_data       (256'h89ab_cdef_0123_4567_dead_beef_1357_2468_0011_2233_4455_6677_8899_aabb_ccdd_eeff),
    .dcache_tenb_data      (256'hfedc_ba98_7654_3210_cafe_babe_0bad_f00d_ffeeddcc_bbaa9988_77665544_33221100),
    .dcache_reduce_ctrl    (dcache_reduce_ctrl),
    .id_core_ctrl          (id_core_ctrl),
    .ex_core_ctrl          (ex_core_ctrl),
    .f2_core_ctrl          (f2_core_ctrl),
    .f3_core_ctrl          (f3_core_ctrl),
    .wb_core_ctrl          (wb_core_ctrl),
    .io_events             (io_events_o),
    .vpu_dbg_match         (vpu_dbg_match_o),
    .vpu_dbg_filter        (vpu_dbg_filter_o),
    .vpu_dbg_data          (vpu_dbg_data)
  );

  assign ex_sh_sw_valid_o        = dut.ex_sh_sw_valid;
  assign ex_sh_sw_clock_valid_o  = dut.ex_sh_sw_clock_valid;
  assign ex_txfma_valid_any_o    = |dut.ex_txfma_valid;
  assign ex_txfma_clock_valid_o  = dut.ex_txfma_clock_valid;
  assign ex_rom_valid_o          = dut.ex_rom_valid;
  assign ex_rom_clock_valid_o    = dut.ex_rom_clock_valid;
  assign ex_tima_valid_any_o     = |dut.ex_tima_valid;
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
