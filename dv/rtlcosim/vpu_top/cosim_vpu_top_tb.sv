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
// original hierarchy elaborates beside the translated package types.
typedef vpu_bypass_force_ctrl vpu_bypass_orig_force_ctrl;
typedef core_vpu_ctrl core_vpu_ctrl_orig;
typedef vpu_ctrl_sigs_t vpu_ctrl_orig_sigs_t;
typedef vpu_ml_load_ctrl vpu_ml_orig_load_ctrl;

typedef struct packed {
    minion_pkg::minion_vpu_id_req      id_core_req;
    minion_pkg::vpu_ctrl_sigs_t        id_vpu_decoder_sigs;
    minion_pkg::minion_vpu_ex_req      ex_core_req;
    logic                              f2_core_kill;
    logic                              f3_core_kill;
    logic                              f4_core_kill;
    minion_pkg::core_vpu_ctrl          f0_core_ctrl;
    logic                              chicken_bit_vpulane;
    logic                              chicken_bit_vputima;
    logic                              chicken_bit_vputrans;
    logic [MINION_MEM_CTRL_BITS-1:0]   mem_ctrl_override;
    logic                              wb_dcache_resp_valid;
    minion_pkg::dcache_vpu_resp        wb_dcache_resp;
    minion_pkg::dcache_vpu_scp_resp    dcache_scp_resp;
    logic [DCACHE_DATA_SIZE-1:0]       dcache_scp_data;
    logic [DCACHE_DATA_SIZE-1:0]       dcache_tenb_data;
    minion_pkg::dcache_vpu_reduce_ctrl dcache_reduce_ctrl;
} vpu_top_inputs_t;

typedef struct packed {
    logic [$bits(minion_pkg::vpu_dcache_ctrl)-1:0]      dcache_ctrl;
    logic [$bits(vpu_defs_pkg::vpu_minion_id_ctrl)-1:0] id_core_ctrl;
    logic [$bits(minion_pkg::vpu_minion_ex_ctrl)-1:0]   ex_core_ctrl;
    logic [$bits(minion_pkg::vpu_minion_tag_ctrl)-1:0]  f2_core_ctrl;
    logic [$bits(minion_pkg::vpu_minion_mem_ctrl)-1:0]  f3_core_ctrl;
    logic [$bits(minion_pkg::vpu_minion_wb_ctrl)-1:0]   wb_core_ctrl;
    logic [CSR_NR_EVENTS_VPU-1:0]                       io_events;
    logic [NEIGH_DEBUG_MATCH_WIDTH-1:0]                 vpu_dbg_match;
    logic [NEIGH_DEBUG_FILTER_WIDTH-1:0]                vpu_dbg_filter;
    logic [4:0][NEIGH_DEBUG_DATA_WIDTH-1:0]             vpu_dbg_data;
} vpu_top_outputs_t;

module cosim_vpu_top_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [7:0] scenario_i,
  input  logic [$bits(vpu_top_inputs_t)-1:0] random_inputs_i,
  output logic [$bits(vpu_top_inputs_t)-1:0] applied_inputs_o,
  output logic [$bits(vpu_top_outputs_t)-1:0] orig_outputs_o,
  output logic [$bits(vpu_top_outputs_t)-1:0] new_outputs_o
);

  localparam logic [7:0] ScenarioIdle        = 8'd0;
  localparam logic [7:0] ScenarioAllOnes     = 8'd1;
  localparam logic [7:0] ScenarioShSw        = 8'd2;
  localparam logic [7:0] ScenarioTxfma       = 8'd3;
  localparam logic [7:0] ScenarioRomTrans    = 8'd4;
  localparam logic [7:0] ScenarioTima        = 8'd5;
  localparam logic [7:0] ScenarioTensorQuant = 8'd6;
  localparam logic [7:0] ScenarioReduce      = 8'd7;
  localparam logic [7:0] ScenarioDcache      = 8'd8;
  localparam logic [7:0] ScenarioKill        = 8'd9;
  localparam logic [7:0] ScenarioChicken     = 8'd10;
  localparam logic [7:0] ScenarioThread      = 8'd11;
  localparam logic [7:0] ScenarioRandom      = 8'hff;

  vpu_top_inputs_t random_in;
  vpu_top_inputs_t in;
  vpu_top_outputs_t orig_out;
  vpu_top_outputs_t new_out;

  assign random_in = vpu_top_inputs_t'(random_inputs_i);
  assign applied_inputs_o = in;
  assign orig_outputs_o = orig_out;
  assign new_outputs_o = new_out;

  function automatic vpu_top_inputs_t base_inputs();
    vpu_top_inputs_t tmp;
    tmp = '0;
    tmp.chicken_bit_vpulane = 1'b1;
    tmp.chicken_bit_vputima = 1'b1;
    tmp.chicken_bit_vputrans = 1'b1;
    tmp.id_core_req.fcsr_rm = RNE;
    tmp.dcache_reduce_ctrl.nothing = 1'b1;
    tmp.wb_dcache_resp.ps_mask = '1;
    tmp.wb_dcache_resp.data = 256'h0123_4567_89ab_cdef_0011_2233_4455_6677_8899_aabb_ccdd_eeff_1357_2468_dead_beef;
    tmp.dcache_scp_data = 256'h89ab_cdef_0123_4567_dead_beef_1357_2468_0011_2233_4455_6677_8899_aabb_ccdd_eeff;
    tmp.dcache_tenb_data = 256'hfedc_ba98_7654_3210_cafe_babe_0bad_f00d_ffeeddcc_bbaa9988_77665544_33221100;
    return tmp;
  endfunction

  function automatic vpu_top_inputs_t decode_inputs(
      input logic thread_id,
      input minion_pkg::vpu_cmd_e cmd,
      input logic shsw,
      input logic txfma,
      input logic rom,
      input logic trans,
      input logic fromint,
      input logic maskop,
      input logic [2:0] ren_mask,
      input logic wen
  );
    vpu_top_inputs_t tmp;
    tmp = base_inputs();
    tmp.id_core_req.valid = 1'b1;
    tmp.id_core_req.thread_id = thread_id;
    tmp.id_core_req.inst_bits[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = 5'd3;
    tmp.id_core_req.inst_bits[VPU_INST_REN2_RA_SEL_MSB:VPU_INST_REN2_RA_SEL_LSB] = 5'd11;
    tmp.id_core_req.inst_bits[VPU_INST_REN3_RA_SEL_MSB:VPU_INST_REN3_RA_SEL_LSB] = 5'd19;
    tmp.id_core_req.inst_bits[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB] = 5'd7;
    tmp.id_vpu_decoder_sigs.cmd = cmd;
    tmp.id_vpu_decoder_sigs.dtype = VpuDtypeF32;
    tmp.id_vpu_decoder_sigs.ren1 = ren_mask[0];
    tmp.id_vpu_decoder_sigs.ren2 = ren_mask[1];
    tmp.id_vpu_decoder_sigs.ren3 = ren_mask[2];
    tmp.id_vpu_decoder_sigs.rend = wen;
    tmp.id_vpu_decoder_sigs.wen = wen;
    tmp.id_vpu_decoder_sigs.shsw = shsw;
    tmp.id_vpu_decoder_sigs.txfma = txfma;
    tmp.id_vpu_decoder_sigs.rom = rom;
    tmp.id_vpu_decoder_sigs.trans = trans;
    tmp.id_vpu_decoder_sigs.fromint = fromint;
    tmp.id_vpu_decoder_sigs.maskop = maskop;
    tmp.id_vpu_decoder_sigs.vector = 1'b1;
    tmp.id_vpu_decoder_sigs.fma = txfma;
    tmp.id_vpu_decoder_sigs.add = shsw;
    tmp.id_vpu_decoder_sigs.wflags = txfma;
    tmp.ex_core_req.thread_id = thread_id;
    tmp.ex_core_req.gsc_src = 3'd2;
    tmp.ex_core_req.fromint_data = 64'h1234_5678_9abc_def0;
    return tmp;
  endfunction

  always_comb begin
    in = random_in;
    unique case (scenario_i)
      ScenarioIdle: begin
        in = base_inputs();
      end
      ScenarioAllOnes: begin
        in = '1;
      end
      ScenarioShSw: begin
        in = decode_inputs(1'b0, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0, 3'b111, 1'b1);
      end
      ScenarioTxfma: begin
        in = decode_inputs(1'b1, VpuCmdMadd, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
      end
      ScenarioRomTrans: begin
        in = decode_inputs(1'b0, VpuTransRcpRr, 1'b0, 1'b0, 1'b1, 1'b1, 1'b0, 1'b0, 3'b111, 1'b1);
      end
      ScenarioTima: begin
        in = decode_inputs(1'b1, VpuCmdMul, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.f0_core_ctrl.tensorfma_start = 1'b1;
        in.f0_core_ctrl.tensorfma_ctrl.start_a = 4'd1;
        in.f0_core_ctrl.tensorfma_ctrl.rows_a = 4'd1;
        in.f0_core_ctrl.tensorfma_ctrl.cols_a = 4'd1;
        in.f0_core_ctrl.tensorfma_ctrl.cols_b = 2'd1;
        in.f0_core_ctrl.tensorfma_ctrl.scp_a = 6'd1;
        in.f0_core_ctrl.tensorfma_ctrl.scp_b = 6'd2;
        in.f0_core_ctrl.tensorfma_ctrl.mode = 3'd1;
        in.f0_core_ctrl.tensorfma_ctrl.first_pass = 1'b1;
        in.f0_core_ctrl.tensorfma_ctrl.to_vrf = 1'b1;
      end
      ScenarioTensorQuant: begin
        in = decode_inputs(1'b0, VpuCmdCvtF32int, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b011, 1'b1);
        in.f0_core_ctrl.tensorquant_start = 1'b1;
        in.f0_core_ctrl.tensorquant_ctrl.start_reg = 5'd4;
        in.f0_core_ctrl.tensorquant_ctrl.rows = 4'd1;
        in.f0_core_ctrl.tensorquant_ctrl.cols = 2'd1;
        in.f0_core_ctrl.tensorquant_ctrl.scp_src = 6'd3;
        in.f0_core_ctrl.tensorquant_ctrl.trans[0] = 1'b1;
      end
      ScenarioReduce: begin
        in = decode_inputs(1'b1, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b001, 1'b1);
        in.f0_core_ctrl.reduce_start = 1'b1;
        in.f0_core_ctrl.reduce_ctrl.reduce.start_reg = 5'd5;
        in.f0_core_ctrl.reduce_ctrl.reduce.num_regs = 7'd1;
        in.f0_core_ctrl.reduce_ctrl.reduce.action = 2'd1;
        in.dcache_reduce_ctrl.send_reg = 1'b1;
        in.dcache_reduce_ctrl.nothing = 1'b0;
      end
      ScenarioDcache: begin
        in = decode_inputs(1'b0, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0, 3'b111, 1'b1);
        in.wb_dcache_resp_valid = 1'b1;
        in.wb_dcache_resp.thread_id = 1'b0;
        in.wb_dcache_resp.gdst = 3'd1;
        in.wb_dcache_resp.addr = 5'd6;
        in.dcache_scp_resp.fill_is_tenb_early = 1'b1;
        in.dcache_scp_resp.fill_is_tenb = 1'b1;
        in.dcache_scp_resp.tenb_line = 4'd2;
      end
      ScenarioKill: begin
        in = decode_inputs(1'b1, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.ex_core_req.kill = 1'b1;
        in.f2_core_kill = 1'b1;
        in.f3_core_kill = 1'b1;
        in.f4_core_kill = 1'b1;
      end
      ScenarioChicken: begin
        in = decode_inputs(1'b0, VpuCmdAdd, 1'b1, 1'b1, 1'b1, 1'b1, 1'b1, 1'b1, 3'b111, 1'b1);
        in.chicken_bit_vpulane = 1'b0;
        in.chicken_bit_vputima = 1'b0;
        in.chicken_bit_vputrans = 1'b0;
      end
      ScenarioThread: begin
        in = decode_inputs(1'b1, VpuCmdMvXf, 1'b1, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0, 3'b101, 1'b1);
        in.id_core_req.inst_bits[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = 5'd9;
        in.id_core_req.inst_bits[VPU_INST_REN2_RA_SEL_MSB:VPU_INST_REN2_RA_SEL_LSB] = 5'd17;
        in.id_core_req.inst_bits[VPU_INST_REN3_RA_SEL_MSB:VPU_INST_REN3_RA_SEL_LSB] = 5'd25;
        in.ex_core_req.gscing = 1'b1;
        in.ex_core_req.gsc_src = 3'd5;
      end
      ScenarioRandom: begin
        in = random_in;
      end
      default: begin
        in = random_in;
      end
    endcase
  end

  minion_vpu_id_req      orig_id_core_req;
  vpu_ctrl_sigs_t        orig_id_vpu_decoder_sigs;
  minion_vpu_ex_req      orig_ex_core_req;
  core_vpu_ctrl          orig_f0_core_ctrl;
  dcache_vpu_resp        orig_wb_dcache_resp;
  dcache_vpu_scp_resp    orig_dcache_scp_resp;
  dcache_vpu_reduce_ctrl orig_dcache_reduce_ctrl;

  assign orig_id_core_req = minion_vpu_id_req'(in.id_core_req);
  assign orig_id_vpu_decoder_sigs = vpu_ctrl_sigs_t'(in.id_vpu_decoder_sigs);
  assign orig_ex_core_req = minion_vpu_ex_req'(in.ex_core_req);
  assign orig_f0_core_ctrl = core_vpu_ctrl'(in.f0_core_ctrl);
  assign orig_wb_dcache_resp = dcache_vpu_resp'(in.wb_dcache_resp);
  assign orig_dcache_scp_resp = dcache_vpu_scp_resp'(in.dcache_scp_resp);
  assign orig_dcache_reduce_ctrl = dcache_vpu_reduce_ctrl'(in.dcache_reduce_ctrl);

  minion_pkg::minion_vpu_id_req      new_id_core_req;
  minion_pkg::vpu_ctrl_sigs_t        new_id_vpu_decoder_sigs;
  minion_pkg::minion_vpu_ex_req      new_ex_core_req;
  minion_pkg::core_vpu_ctrl          new_f0_core_ctrl;
  minion_pkg::dcache_vpu_resp        new_wb_dcache_resp;
  minion_pkg::dcache_vpu_scp_resp    new_dcache_scp_resp;
  minion_pkg::dcache_vpu_reduce_ctrl new_dcache_reduce_ctrl;

  assign new_id_core_req = in.id_core_req;
  assign new_id_vpu_decoder_sigs = in.id_vpu_decoder_sigs;
  assign new_ex_core_req = in.ex_core_req;
  assign new_f0_core_ctrl = in.f0_core_ctrl;
  assign new_wb_dcache_resp = in.wb_dcache_resp;
  assign new_dcache_scp_resp = in.dcache_scp_resp;
  assign new_dcache_reduce_ctrl = in.dcache_reduce_ctrl;

  vpu_dcache_ctrl orig_dcache_ctrl;
  vpu_minion_id_ctrl orig_id_core_ctrl;
  vpu_minion_ex_ctrl orig_ex_core_ctrl;
  vpu_minion_tag_ctrl orig_f2_core_ctrl;
  vpu_minion_mem_ctrl orig_f3_core_ctrl;
  vpu_minion_wb_ctrl orig_wb_core_ctrl;
  logic [CSR_NR_EVENTS_VPU-1:0] orig_io_events;
  logic [NEIGH_DEBUG_MATCH_WIDTH-1:0] orig_vpu_dbg_match;
  logic [NEIGH_DEBUG_FILTER_WIDTH-1:0] orig_vpu_dbg_filter;
  logic [4:0][NEIGH_DEBUG_DATA_WIDTH-1:0] orig_vpu_dbg_data;

  minion_pkg::vpu_dcache_ctrl new_dcache_ctrl;
  vpu_defs_pkg::vpu_minion_id_ctrl new_id_core_ctrl;
  minion_pkg::vpu_minion_ex_ctrl new_ex_core_ctrl;
  minion_pkg::vpu_minion_tag_ctrl new_f2_core_ctrl;
  minion_pkg::vpu_minion_mem_ctrl new_f3_core_ctrl;
  minion_pkg::vpu_minion_wb_ctrl new_wb_core_ctrl;
  logic [CSR_NR_EVENTS_VPU-1:0] new_io_events;
  logic [NEIGH_DEBUG_MATCH_WIDTH-1:0] new_vpu_dbg_match;
  logic [NEIGH_DEBUG_FILTER_WIDTH-1:0] new_vpu_dbg_filter;
  logic [4:0][NEIGH_DEBUG_DATA_WIDTH-1:0] new_vpu_dbg_data;

  assign orig_out.dcache_ctrl = orig_dcache_ctrl;
  assign orig_out.id_core_ctrl = orig_id_core_ctrl;
  assign orig_out.ex_core_ctrl = orig_ex_core_ctrl;
  assign orig_out.f2_core_ctrl = orig_f2_core_ctrl;
  assign orig_out.f3_core_ctrl = orig_f3_core_ctrl;
  assign orig_out.wb_core_ctrl = orig_wb_core_ctrl;
  assign orig_out.io_events = orig_io_events;
  assign orig_out.vpu_dbg_match = orig_vpu_dbg_match;
  assign orig_out.vpu_dbg_filter = orig_vpu_dbg_filter;
  assign orig_out.vpu_dbg_data = orig_vpu_dbg_data;

  assign new_out.dcache_ctrl = new_dcache_ctrl;
  assign new_out.id_core_ctrl = new_id_core_ctrl;
  assign new_out.ex_core_ctrl = new_ex_core_ctrl;
  assign new_out.f2_core_ctrl = new_f2_core_ctrl;
  assign new_out.f3_core_ctrl = new_f3_core_ctrl;
  assign new_out.wb_core_ctrl = new_wb_core_ctrl;
  assign new_out.io_events = new_io_events;
  assign new_out.vpu_dbg_match = new_vpu_dbg_match;
  assign new_out.vpu_dbg_filter = new_vpu_dbg_filter;
  assign new_out.vpu_dbg_data = new_vpu_dbg_data;

  vpu_top_orig #(
    .USE_MMI (1'b0)
  ) u_orig (
    .clock                 (clk_i),
    .reset                 (~rst_ni),
    .id_core_req           (orig_id_core_req),
    .id_vpu_decoder_sigs   (orig_id_vpu_decoder_sigs),
    .ex_core_req           (orig_ex_core_req),
    .f2_core_kill          (in.f2_core_kill),
    .f3_core_kill          (in.f3_core_kill),
    .f4_core_kill          (in.f4_core_kill),
    .f0_core_ctrl          (orig_f0_core_ctrl),
    .chicken_bit_vpulane   (in.chicken_bit_vpulane),
    .chicken_bit_vputima   (in.chicken_bit_vputima),
    .chicken_bit_vputrans  (in.chicken_bit_vputrans),
    .mem_ctrl_override     (in.mem_ctrl_override),
    .wb_dcache_resp_valid  (in.wb_dcache_resp_valid),
    .wb_dcache_resp        (orig_wb_dcache_resp),
    .dcache_ctrl           (orig_dcache_ctrl),
    .dcache_scp_resp       (orig_dcache_scp_resp),
    .dcache_scp_data       (in.dcache_scp_data),
    .dcache_tenb_data      (in.dcache_tenb_data),
    .dcache_reduce_ctrl    (orig_dcache_reduce_ctrl),
    .id_core_ctrl          (orig_id_core_ctrl),
    .ex_core_ctrl          (orig_ex_core_ctrl),
    .f2_core_ctrl          (orig_f2_core_ctrl),
    .f3_core_ctrl          (orig_f3_core_ctrl),
    .wb_core_ctrl          (orig_wb_core_ctrl),
    .io_events             (orig_io_events),
    .vpu_dbg_match         (orig_vpu_dbg_match),
    .vpu_dbg_filter        (orig_vpu_dbg_filter),
    .vpu_dbg_data          (orig_vpu_dbg_data)
  );

  vpu_top #(
    .UseFakeTxfma (1'b0)
  ) u_new (
    .clock                 (clk_i),
    .reset                 (~rst_ni),
    .id_core_req           (new_id_core_req),
    .id_vpu_decoder_sigs   (new_id_vpu_decoder_sigs),
    .ex_core_req           (new_ex_core_req),
    .f2_core_kill          (in.f2_core_kill),
    .f3_core_kill          (in.f3_core_kill),
    .f4_core_kill          (in.f4_core_kill),
    .f0_core_ctrl          (new_f0_core_ctrl),
    .chicken_bit_vpulane   (in.chicken_bit_vpulane),
    .chicken_bit_vputima   (in.chicken_bit_vputima),
    .chicken_bit_vputrans  (in.chicken_bit_vputrans),
    .mem_ctrl_override     (in.mem_ctrl_override),
    .wb_dcache_resp_valid  (in.wb_dcache_resp_valid),
    .wb_dcache_resp        (new_wb_dcache_resp),
    .dcache_ctrl           (new_dcache_ctrl),
    .dcache_scp_resp       (new_dcache_scp_resp),
    .dcache_scp_data       (in.dcache_scp_data),
    .dcache_tenb_data      (in.dcache_tenb_data),
    .dcache_reduce_ctrl    (new_dcache_reduce_ctrl),
    .id_core_ctrl          (new_id_core_ctrl),
    .ex_core_ctrl          (new_ex_core_ctrl),
    .f2_core_ctrl          (new_f2_core_ctrl),
    .f3_core_ctrl          (new_f3_core_ctrl),
    .wb_core_ctrl          (new_wb_core_ctrl),
    .io_events             (new_io_events),
    .vpu_dbg_match         (new_vpu_dbg_match),
    .vpu_dbg_filter        (new_vpu_dbg_filter),
    .vpu_dbg_data          (new_vpu_dbg_data)
  );

endmodule
