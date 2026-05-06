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
  output logic [$bits(vpu_top_outputs_t)-1:0] new_outputs_o,
  output logic [63:0] orig_protocol_o,
  output logic [63:0] new_protocol_o
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
  localparam logic [7:0] ScenarioKill             = 8'd9;
  localparam logic [7:0] ScenarioChicken          = 8'd10;
  localparam logic [7:0] ScenarioThread           = 8'd11;
  localparam logic [7:0] ScenarioCmdSub           = 8'd12;
  localparam logic [7:0] ScenarioCompareMinMax    = 8'd13;
  localparam logic [7:0] ScenarioConvert          = 8'd14;
  localparam logic [7:0] ScenarioMoveFromInt      = 8'd15;
  localparam logic [7:0] ScenarioMoveToInt        = 8'd16;
  localparam logic [7:0] ScenarioLogicMask        = 8'd17;
  localparam logic [7:0] ScenarioShiftImm         = 8'd18;
  localparam logic [7:0] ScenarioDtypeF16         = 8'd19;
  localparam logic [7:0] ScenarioDtypeF11         = 8'd20;
  localparam logic [7:0] ScenarioDtypeF10         = 8'd21;
  localparam logic [7:0] ScenarioDtypeUn8         = 8'd22;
  localparam logic [7:0] ScenarioDtypeSn8         = 8'd23;
  localparam logic [7:0] ScenarioRoundRne         = 8'd24;
  localparam logic [7:0] ScenarioRoundRtz         = 8'd25;
  localparam logic [7:0] ScenarioRoundRdn         = 8'd26;
  localparam logic [7:0] ScenarioRoundRup         = 8'd27;
  localparam logic [7:0] ScenarioRoundRmm         = 8'd28;
  localparam logic [7:0] ScenarioRoundDyn         = 8'd29;
  localparam logic [7:0] ScenarioHazardWrite      = 8'd30;
  localparam logic [7:0] ScenarioHazardRead       = 8'd31;
  localparam logic [7:0] ScenarioInterleaveT0     = 8'd32;
  localparam logic [7:0] ScenarioInterleaveT1     = 8'd33;
  localparam logic [7:0] ScenarioKillDcacheTensor = 8'd34;
  localparam logic [7:0] ScenarioDcacheSideband   = 8'd35;
  localparam logic [7:0] ScenarioTensorStore      = 8'd36;
  localparam logic [7:0] ScenarioDebugBanks       = 8'd37;
  localparam logic [7:0] ScenarioRandom           = 8'hff;

  localparam int unsigned ProtocolShortAlu        = 0;
  localparam int unsigned ProtocolAddSub          = 1;
  localparam int unsigned ProtocolMulTima         = 2;
  localparam int unsigned ProtocolTxfma           = 3;
  localparam int unsigned ProtocolCompareMinMax   = 4;
  localparam int unsigned ProtocolConvert         = 5;
  localparam int unsigned ProtocolMove            = 6;
  localparam int unsigned ProtocolLogicMask       = 7;
  localparam int unsigned ProtocolShiftImm        = 8;
  localparam int unsigned ProtocolRomTrans        = 9;
  localparam int unsigned ProtocolDtypeInt        = 10;
  localparam int unsigned ProtocolDtypeF32        = 11;
  localparam int unsigned ProtocolDtypeF16        = 12;
  localparam int unsigned ProtocolDtypeF11        = 13;
  localparam int unsigned ProtocolDtypeF10        = 14;
  localparam int unsigned ProtocolDtypeUn8        = 15;
  localparam int unsigned ProtocolDtypeSn8        = 16;
  localparam int unsigned ProtocolRoundRne        = 17;
  localparam int unsigned ProtocolRoundRtz        = 18;
  localparam int unsigned ProtocolRoundRdn        = 19;
  localparam int unsigned ProtocolRoundRup        = 20;
  localparam int unsigned ProtocolRoundRmm        = 21;
  localparam int unsigned ProtocolRoundDyn        = 22;
  localparam int unsigned ProtocolThread0         = 23;
  localparam int unsigned ProtocolThread1         = 24;
  localparam int unsigned ProtocolDestSrcOverlap  = 25;
  localparam int unsigned ProtocolBackToBackHaz   = 26;
  localparam int unsigned ProtocolInterleave      = 27;
  localparam int unsigned ProtocolKillEx          = 28;
  localparam int unsigned ProtocolKillF2          = 29;
  localparam int unsigned ProtocolKillF3          = 30;
  localparam int unsigned ProtocolKillF4          = 31;
  localparam int unsigned ProtocolDcacheWb        = 32;
  localparam int unsigned ProtocolDcacheScpTenb   = 33;
  localparam int unsigned ProtocolReduceSend      = 34;
  localparam int unsigned ProtocolReduceExec      = 35;
  localparam int unsigned ProtocolReduceNothing   = 36;
  localparam int unsigned ProtocolMemOverride     = 37;
  localparam int unsigned ProtocolDbgMatch        = 38;
  localparam int unsigned ProtocolDbgFilter       = 39;
  localparam int unsigned ProtocolDbgData0        = 40;
  localparam int unsigned ProtocolDbgData1        = 41;
  localparam int unsigned ProtocolDbgData2        = 42;
  localparam int unsigned ProtocolDbgData3        = 43;
  localparam int unsigned ProtocolDbgData4        = 44;
  localparam int unsigned ProtocolChickenLaneDis  = 45;
  localparam int unsigned ProtocolChickenTimaDis  = 46;
  localparam int unsigned ProtocolChickenTransDis = 47;
  localparam int unsigned ProtocolTensorFma       = 48;
  localparam int unsigned ProtocolTensorReduce    = 49;
  localparam int unsigned ProtocolTensorQuant     = 50;
  localparam int unsigned ProtocolTensorStore     = 51;
  localparam int unsigned ProtocolRealTxfma       = 52;

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
    tmp.id_core_req.inst_bits[VPU_INST_TYP_SEL_MSB:VPU_INST_TYP_SEL_LSB] = 2'd1;
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

  function automatic logic [63:0] protocol_bits(
      input logic [7:0] observed_scenario,
      input vpu_top_inputs_t observed_in,
      input logic id_wen,
      input logic id_ren_any,
      input logic id_mren_any,
      input logic id_fromint,
      input logic id_tfma_enabled,
      input logic id_tquant_enabled,
      input logic id_reduce_enabled,
      input logic ex_tointm,
      input logic f2_fma,
      input logic f3_fma,
      input logic wb_fma,
      input logic wb_tointm,
      input logic dcache_read,
      input logic dcache_tenb_credit,
      input logic dcache_tfma_enabled,
      input logic dcache_reduce_wait,
      input logic dcache_tfma_rf_write,
      input logic [CSR_NR_EVENTS_VPU-1:0] events,
      input logic [NEIGH_DEBUG_MATCH_WIDTH-1:0] dbg_match,
      input logic [NEIGH_DEBUG_FILTER_WIDTH-1:0] dbg_filter,
      input logic [4:0][NEIGH_DEBUG_DATA_WIDTH-1:0] dbg_data,
      input logic ex_sh_sw_valid,
      input logic ex_tima_valid_any,
      input logic ex_txfma_valid_any,
      input logic ex_txfma_clock_valid,
      input logic ex_sh_sw_clock_valid,
      input logic ex_rom_clock_valid,
      input logic ex_rom_valid,
      input logic [CORE_FCSR_RM_SZ-1:0] effective_rm
  );
    logic [63:0] bits;
    logic valid;
    logic id_activity;
    logic family_activity;
    logic debug_activity;
    logic [6:0] cmd;
    logic [3:0] dtype;
    logic [FREG_ADDR_SIZE-1:0] ra1;
    logic [FREG_ADDR_SIZE-1:0] ra2;
    logic [FREG_ADDR_SIZE-1:0] ra3;
    logic [FREG_ADDR_SIZE-1:0] rd;
    logic [CORE_FCSR_RM_SZ-1:0] inst_rm;
    logic dest_src_overlap;

    bits = '0;
    valid = observed_in.id_core_req.valid;
    id_activity = id_wen | id_ren_any | id_mren_any | id_fromint;
    debug_activity = (|dbg_match) | (|dbg_filter) | (|dbg_data);
    family_activity = id_activity | ex_sh_sw_valid | ex_tima_valid_any |
                      ex_txfma_valid_any | ex_rom_valid | f2_fma |
                      f3_fma | wb_fma | ex_tointm | wb_tointm |
                      debug_activity;
    cmd = observed_in.id_vpu_decoder_sigs.cmd;
    dtype = observed_in.id_vpu_decoder_sigs.dtype;
    ra1 = observed_in.id_core_req.inst_bits[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB];
    ra2 = observed_in.id_core_req.inst_bits[VPU_INST_REN2_RA_SEL_MSB:VPU_INST_REN2_RA_SEL_LSB];
    ra3 = observed_in.id_core_req.inst_bits[VPU_INST_REN3_RA_SEL_MSB:VPU_INST_REN3_RA_SEL_LSB];
    rd = observed_in.id_core_req.inst_bits[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB];
    inst_rm = observed_in.id_core_req.inst_bits[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB];
    dest_src_overlap = valid & observed_in.id_vpu_decoder_sigs.wen &
        ((observed_in.id_vpu_decoder_sigs.ren1 & (rd == ra1)) |
         (observed_in.id_vpu_decoder_sigs.ren2 & (rd == ra2)) |
         (observed_in.id_vpu_decoder_sigs.ren3 & (rd == ra3)));

    bits[ProtocolShortAlu] = valid & observed_in.id_vpu_decoder_sigs.shsw & ex_sh_sw_clock_valid;
    bits[ProtocolAddSub] = valid & observed_in.id_vpu_decoder_sigs.shsw &
        ((cmd == VpuCmdAdd) | (cmd == VpuCmdSub) | (cmd == VpuCmdAddi)) &
        (id_activity | ex_sh_sw_clock_valid | events[VPU_EVENT_SHORT_OPS] | debug_activity);
    bits[ProtocolMulTima] = valid &
        ((cmd == VpuCmdMul) | (cmd == VpuCmdMull) | (cmd == VpuCmdMullu) |
         (cmd == VpuCmdMulh) | (cmd == VpuCmdMulhu)) &
        (ex_tima_valid_any | events[VPU_EVENT_TIMA_OPS] | debug_activity);
    bits[ProtocolTxfma] = valid & observed_in.id_vpu_decoder_sigs.txfma &
        ex_txfma_valid_any & ex_txfma_clock_valid;
    bits[ProtocolCompareMinMax] = valid &
        ((cmd == VpuCmdMin) | (cmd == VpuCmdMax) | (cmd == VpuCmdMinu) |
         (cmd == VpuCmdMaxu) | (cmd == VpuCmdCmp) | (cmd == VpuCmdFeq) |
         (cmd == VpuCmdFle) | (cmd == VpuCmdFlt) | (cmd == VpuCmdFltu)) &
        (id_activity | ex_sh_sw_clock_valid);
    bits[ProtocolConvert] = valid &
        (observed_in.id_vpu_decoder_sigs.cvt | observed_in.id_vpu_decoder_sigs.gcvt |
         (cmd == VpuCmdCvtFf) | (cmd == VpuCmdCvtF32int) | (cmd == VpuCmdCvtIntf32) |
         (cmd == VpuCmdCvtF16f32) | (cmd == VpuCmdCvtF11f32) | (cmd == VpuCmdCvtF10f32) |
         (cmd == VpuCmdCvtF32f16) | (cmd == VpuCmdCvtF32f11) | (cmd == VpuCmdCvtF32f10) |
         (cmd == VpuCmdCvtF32un8) | (cmd == VpuCmdCvtF32sn8) |
         (cmd == VpuCmdCvtUn8f32) | (cmd == VpuCmdCvtSn8f32)) & family_activity;
    bits[ProtocolMove] = valid &
        (observed_in.id_vpu_decoder_sigs.fromint | observed_in.id_vpu_decoder_sigs.toint |
         observed_in.id_vpu_decoder_sigs.tointm | (cmd == VpuCmdMvXf) | (cmd == VpuCmdMvFx) |
         (cmd == VpuCmdMovaXM) | (cmd == VpuCmdMovaMX) | (cmd == VpuCmdMovMX)) &
        (id_fromint | ex_tointm | wb_tointm | family_activity);
    bits[ProtocolLogicMask] = valid &
        (observed_in.id_vpu_decoder_sigs.maskop | (cmd == VpuCmdAnd) | (cmd == VpuCmdOr) |
         (cmd == VpuCmdXor) | (cmd == VpuCmdNot) | (cmd == VpuCmdMand) |
         (cmd == VpuCmdMor) | (cmd == VpuCmdMxor) | (cmd == VpuCmdMnot) |
         (cmd == VpuCmdFsetm) | (cmd == VpuCmdAndi)) &
        (id_activity | id_mren_any | events[VPU_EVENT_MASK_OPS]);
    bits[ProtocolShiftImm] = valid &
        ((cmd == VpuCmdSlli) | (cmd == VpuCmdSrli) | (cmd == VpuCmdSrai) |
         (cmd == VpuCmdAddi) | (cmd == VpuCmdAndi) | (cmd == VpuCmdFbci)) &
        (id_activity | ex_sh_sw_clock_valid | debug_activity);
    bits[ProtocolRomTrans] = valid &
        (observed_in.id_vpu_decoder_sigs.rom | observed_in.id_vpu_decoder_sigs.trans) &
        ex_rom_valid & ex_rom_clock_valid;

    bits[ProtocolDtypeInt] = valid & (dtype == VpuDtypeInt) & family_activity;
    bits[ProtocolDtypeF32] = valid & (dtype == VpuDtypeF32) & family_activity;
    bits[ProtocolDtypeF16] = valid & (dtype == VpuDtypeF16) & family_activity;
    bits[ProtocolDtypeF11] = valid & (dtype == VpuDtypeF11) & family_activity;
    bits[ProtocolDtypeF10] = valid & (dtype == VpuDtypeF10) & family_activity;
    bits[ProtocolDtypeUn8] = valid & (dtype == VpuDtypeUn8) & family_activity;
    bits[ProtocolDtypeSn8] = valid & (dtype == VpuDtypeSn8) & family_activity;

    bits[ProtocolRoundRne] = valid & observed_in.id_vpu_decoder_sigs.round & (effective_rm == RNE) & family_activity;
    bits[ProtocolRoundRtz] = valid & observed_in.id_vpu_decoder_sigs.round & (effective_rm == RTZ) & family_activity;
    bits[ProtocolRoundRdn] = valid & observed_in.id_vpu_decoder_sigs.round & (effective_rm == RDN) & family_activity;
    bits[ProtocolRoundRup] = valid & observed_in.id_vpu_decoder_sigs.round & (effective_rm == RUP) & family_activity;
    bits[ProtocolRoundRmm] = valid & observed_in.id_vpu_decoder_sigs.round & (effective_rm == RMM) & family_activity;
    bits[ProtocolRoundDyn] = valid & observed_in.id_vpu_decoder_sigs.round &
        (inst_rm == VPU_RM_DYN) & (effective_rm == observed_in.id_core_req.fcsr_rm) & family_activity;

    bits[ProtocolThread0] = valid & !observed_in.id_core_req.thread_id & family_activity;
    bits[ProtocolThread1] = valid & observed_in.id_core_req.thread_id & family_activity;
    bits[ProtocolDestSrcOverlap] = dest_src_overlap & family_activity;
    bits[ProtocolBackToBackHaz] = (observed_scenario == ScenarioHazardRead) & dest_src_overlap & family_activity;
    bits[ProtocolInterleave] = ((observed_scenario == ScenarioInterleaveT0) |
                                (observed_scenario == ScenarioInterleaveT1)) & valid & family_activity;

    bits[ProtocolKillEx] = observed_in.ex_core_req.kill & dbg_match[51];
    bits[ProtocolKillF2] = observed_in.f2_core_kill & dbg_match[52];
    bits[ProtocolKillF3] = observed_in.f3_core_kill & dbg_match[53];
    bits[ProtocolKillF4] = observed_in.f4_core_kill & dbg_match[54];

    bits[ProtocolDcacheWb] = observed_in.wb_dcache_resp_valid & dbg_match[57];
    bits[ProtocolDcacheScpTenb] = observed_in.dcache_scp_resp.fill_is_tenb &
        (dcache_tenb_credit | (|dbg_data[2]));
    bits[ProtocolReduceSend] = observed_in.dcache_reduce_ctrl.send_reg &
        (dcache_reduce_wait | events[VPU_EVENT_TREDUCE_INST] | (|dbg_data[4]));
    bits[ProtocolReduceExec] = observed_in.dcache_reduce_ctrl.exec_op & debug_activity;
    bits[ProtocolReduceNothing] = observed_in.dcache_reduce_ctrl.nothing & !dcache_reduce_wait;
    bits[ProtocolMemOverride] = (observed_in.mem_ctrl_override != '0) &
        (dcache_read | dcache_tfma_rf_write | dcache_tfma_enabled | debug_activity);

    bits[ProtocolDbgMatch] = |dbg_match;
    bits[ProtocolDbgFilter] = |dbg_filter;
    bits[ProtocolDbgData0] = |dbg_data[0];
    bits[ProtocolDbgData1] = |dbg_data[1];
    bits[ProtocolDbgData2] = |dbg_data[2];
    bits[ProtocolDbgData3] = |dbg_data[3];
    bits[ProtocolDbgData4] = |dbg_data[4];

    bits[ProtocolChickenLaneDis] = !observed_in.chicken_bit_vpulane & valid & debug_activity;
    bits[ProtocolChickenTimaDis] = !observed_in.chicken_bit_vputima & valid & debug_activity;
    bits[ProtocolChickenTransDis] = !observed_in.chicken_bit_vputrans & valid & debug_activity;

    bits[ProtocolTensorFma] = observed_in.f0_core_ctrl.tensorfma_start &
        (id_tfma_enabled | dcache_tfma_enabled | events[VPU_EVENT_TFMA_INST] | (|dbg_data[3]));
    bits[ProtocolTensorReduce] = observed_in.f0_core_ctrl.reduce_start &
        (id_reduce_enabled | events[VPU_EVENT_TREDUCE_INST] | (|dbg_data[3]) | (|dbg_data[4]));
    bits[ProtocolTensorQuant] = observed_in.f0_core_ctrl.tensorquant_start &
        (id_tquant_enabled | events[VPU_EVENT_TQUANT_INST] | (|dbg_data[4]));
    bits[ProtocolTensorStore] = observed_in.f0_core_ctrl.tensorstore_start &
        (dcache_read | dcache_tfma_rf_write | (|dbg_data[3]) | (|dbg_data[4]));
    bits[ProtocolRealTxfma] = bits[ProtocolTxfma];

    return bits;
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
      ScenarioCmdSub: begin
        in = decode_inputs(1'b0, VpuCmdSub, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_vpu_decoder_sigs.add = 1'b0;
        in.id_vpu_decoder_sigs.sub = 1'b1;
        in.id_vpu_decoder_sigs.round = 1'b1;
        in.id_core_req.inst_bits[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB] = RDN;
      end
      ScenarioCompareMinMax: begin
        in = decode_inputs(1'b0, VpuCmdMax, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b011, 1'b1);
        in.id_vpu_decoder_sigs.add = 1'b0;
        in.id_vpu_decoder_sigs.wflags = 1'b1;
      end
      ScenarioConvert: begin
        in = decode_inputs(1'b0, VpuCmdCvtF16f32, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b011, 1'b1);
        in.id_vpu_decoder_sigs.dtype = VpuDtypeF16;
        in.id_vpu_decoder_sigs.cvt = 1'b1;
        in.id_vpu_decoder_sigs.round = 1'b1;
        in.id_core_req.inst_bits[VPU_INST_TYP_SEL_MSB:VPU_INST_TYP_SEL_LSB] = 2'd2;
        in.id_core_req.inst_bits[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB] = RMM;
      end
      ScenarioMoveFromInt: begin
        in = decode_inputs(1'b1, VpuCmdMvXf, 1'b1, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0, 3'b001, 1'b1);
        in.ex_core_req.fromint_data = 64'hfeed_beef_0123_4567;
      end
      ScenarioMoveToInt: begin
        in = decode_inputs(1'b0, VpuCmdMvFx, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b001, 1'b1);
        in.id_vpu_decoder_sigs.toint = 1'b1;
        in.id_vpu_decoder_sigs.tointm = 1'b1;
      end
      ScenarioLogicMask: begin
        in = decode_inputs(1'b0, VpuCmdMand, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 3'b000, 1'b1);
        in.id_vpu_decoder_sigs.dtype = VpuDtypeInt;
        in.id_vpu_decoder_sigs.m0ren = 1'b1;
        in.id_vpu_decoder_sigs.mallren = 1'b1;
        in.id_vpu_decoder_sigs.mren1 = 1'b1;
        in.id_vpu_decoder_sigs.mren2 = 1'b1;
      end
      ScenarioShiftImm: begin
        in = decode_inputs(1'b0, VpuCmdSlli, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b001, 1'b1);
        in.id_vpu_decoder_sigs.dtype = VpuDtypeInt;
        in.id_core_req.inst_bits[VPU_INST_TYP_SEL_MSB:VPU_INST_TYP_SEL_LSB] = 2'd0;
      end
      ScenarioDtypeF16: begin
        in = decode_inputs(1'b1, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_vpu_decoder_sigs.dtype = VpuDtypeF16;
        in.id_core_req.inst_bits[VPU_INST_TYP_SEL_MSB:VPU_INST_TYP_SEL_LSB] = 2'd2;
      end
      ScenarioDtypeF11: begin
        in = decode_inputs(1'b0, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_vpu_decoder_sigs.dtype = VpuDtypeF11;
        in.id_core_req.inst_bits[VPU_INST_TYP_SEL_MSB:VPU_INST_TYP_SEL_LSB] = 2'd3;
      end
      ScenarioDtypeF10: begin
        in = decode_inputs(1'b1, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_vpu_decoder_sigs.dtype = VpuDtypeF10;
        in.id_core_req.inst_bits[VPU_INST_TYP_SEL_MSB:VPU_INST_TYP_SEL_LSB] = 2'd0;
      end
      ScenarioDtypeUn8: begin
        in = decode_inputs(1'b0, VpuCmdCvtF32un8, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b011, 1'b1);
        in.id_vpu_decoder_sigs.dtype = VpuDtypeUn8;
        in.id_vpu_decoder_sigs.cvt = 1'b1;
        in.id_vpu_decoder_sigs.round = 1'b1;
      end
      ScenarioDtypeSn8: begin
        in = decode_inputs(1'b1, VpuCmdCvtF32sn8, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b011, 1'b1);
        in.id_vpu_decoder_sigs.dtype = VpuDtypeSn8;
        in.id_vpu_decoder_sigs.cvt = 1'b1;
        in.id_vpu_decoder_sigs.round = 1'b1;
      end
      ScenarioRoundRne: begin
        in = decode_inputs(1'b0, VpuCmdMadd, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_vpu_decoder_sigs.round = 1'b1;
        in.id_core_req.inst_bits[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB] = RNE;
      end
      ScenarioRoundRtz: begin
        in = decode_inputs(1'b1, VpuCmdMadd, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_vpu_decoder_sigs.round = 1'b1;
        in.id_core_req.inst_bits[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB] = RTZ;
      end
      ScenarioRoundRdn: begin
        in = decode_inputs(1'b0, VpuCmdMadd, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_vpu_decoder_sigs.round = 1'b1;
        in.id_core_req.inst_bits[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB] = RDN;
      end
      ScenarioRoundRup: begin
        in = decode_inputs(1'b1, VpuCmdMadd, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_vpu_decoder_sigs.round = 1'b1;
        in.id_core_req.inst_bits[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB] = RUP;
      end
      ScenarioRoundRmm: begin
        in = decode_inputs(1'b0, VpuCmdMadd, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_vpu_decoder_sigs.round = 1'b1;
        in.id_core_req.inst_bits[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB] = RMM;
      end
      ScenarioRoundDyn: begin
        in = decode_inputs(1'b1, VpuCmdMadd, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_vpu_decoder_sigs.round = 1'b1;
        in.id_core_req.fcsr_rm = RDN;
        in.id_core_req.inst_bits[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB] = VPU_RM_DYN;
      end
      ScenarioHazardWrite: begin
        in = decode_inputs(1'b0, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_core_req.inst_bits[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB] = 5'd13;
        in.id_core_req.inst_bits[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = 5'd2;
      end
      ScenarioHazardRead: begin
        in = decode_inputs(1'b0, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.id_core_req.inst_bits[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB] = 5'd13;
        in.id_core_req.inst_bits[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = 5'd13;
      end
      ScenarioInterleaveT0: begin
        in = decode_inputs(1'b0, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b011, 1'b1);
        in.id_core_req.inst_bits[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB] = 5'd8;
      end
      ScenarioInterleaveT1: begin
        in = decode_inputs(1'b1, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 3'b011, 1'b1);
        in.id_core_req.inst_bits[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB] = 5'd9;
      end
      ScenarioKillDcacheTensor: begin
        in = decode_inputs(1'b1, VpuCmdMadd, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.f0_core_ctrl.tensorfma_start = 1'b1;
        in.f0_core_ctrl.tensorfma_ctrl.start_a = 4'd3;
        in.f0_core_ctrl.tensorfma_ctrl.rows_a = 4'd1;
        in.f0_core_ctrl.tensorfma_ctrl.cols_a = 4'd1;
        in.f0_core_ctrl.tensorfma_ctrl.cols_b = 2'd1;
        in.f0_core_ctrl.tensorfma_ctrl.scp_a = 6'd4;
        in.f0_core_ctrl.tensorfma_ctrl.scp_b = 6'd5;
        in.f0_core_ctrl.tensorfma_ctrl.mode = 3'd2;
        in.f0_core_ctrl.tensorfma_ctrl.first_pass = 1'b1;
        in.dcache_scp_resp.fill_is_tenb_early = 1'b1;
        in.dcache_scp_resp.fill_is_tenb = 1'b1;
        in.wb_dcache_resp_valid = 1'b1;
        in.ex_core_req.kill = 1'b1;
        in.f2_core_kill = 1'b1;
        in.f3_core_kill = 1'b1;
        in.f4_core_kill = 1'b1;
      end
      ScenarioDcacheSideband: begin
        in = decode_inputs(1'b0, VpuCmdAdd, 1'b1, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0, 3'b111, 1'b1);
        in.wb_dcache_resp_valid = 1'b1;
        in.wb_dcache_resp.thread_id = 1'b0;
        in.wb_dcache_resp.gdst = 3'd4;
        in.wb_dcache_resp.addr = 5'd10;
        in.dcache_scp_resp.fill_is_tenb_early = 1'b1;
        in.dcache_scp_resp.fill_is_tenb = 1'b1;
        in.dcache_scp_resp.tenb_line = 4'd7;
        in.dcache_reduce_ctrl.exec_op = 1'b1;
        in.dcache_reduce_ctrl.nothing = 1'b0;
        in.mem_ctrl_override = '1;
      end
      ScenarioTensorStore: begin
        in = base_inputs();
        in.f0_core_ctrl.tensorstore_start = 1'b1;
        in.f0_core_ctrl.reduce_ctrl.tensorstore.src_inc = 2'd1;
        in.f0_core_ctrl.reduce_ctrl.tensorstore.start_reg = 5'd6;
        in.f0_core_ctrl.reduce_ctrl.tensorstore.cols = 2'd2;
        in.f0_core_ctrl.reduce_ctrl.tensorstore.rows = 4'd2;
        in.f0_core_ctrl.reduce_ctrl.tensorstore.coop = 2'd1;
        in.f0_core_ctrl.reduce_ctrl.tensorstore.scp = 1'b1;
        in.f0_core_ctrl.reduce_ctrl.tensorstore.rate = 4'd3;
      end
      ScenarioDebugBanks: begin
        in = decode_inputs(1'b1, VpuCmdMadd, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 3'b111, 1'b1);
        in.f0_core_ctrl.tensorfma_start = 1'b1;
        in.f0_core_ctrl.tensorfma_ctrl.start_a = 4'd2;
        in.f0_core_ctrl.tensorfma_ctrl.rows_a = 4'd2;
        in.f0_core_ctrl.tensorfma_ctrl.cols_a = 4'd1;
        in.f0_core_ctrl.tensorfma_ctrl.cols_b = 2'd1;
        in.f0_core_ctrl.tensorfma_ctrl.scp_a = 6'd6;
        in.f0_core_ctrl.tensorfma_ctrl.scp_b = 6'd7;
        in.f0_core_ctrl.tensorfma_ctrl.mode = 3'd3;
        in.f0_core_ctrl.tensorquant_start = 1'b1;
        in.f0_core_ctrl.tensorquant_ctrl.start_reg = 5'd9;
        in.f0_core_ctrl.tensorquant_ctrl.rows = 4'd2;
        in.f0_core_ctrl.tensorquant_ctrl.cols = 2'd1;
        in.f0_core_ctrl.tensorquant_ctrl.scp_src = 6'd8;
        in.f0_core_ctrl.reduce_start = 1'b1;
        in.f0_core_ctrl.reduce_ctrl.reduce.start_reg = 5'd10;
        in.f0_core_ctrl.reduce_ctrl.reduce.num_regs = 7'd2;
        in.f0_core_ctrl.reduce_ctrl.reduce.action = 2'd2;
        in.wb_dcache_resp_valid = 1'b1;
        in.dcache_scp_resp.fill_is_tenb = 1'b1;
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

  assign orig_protocol_o = protocol_bits(
      scenario_i,
      in,
      orig_id_core_ctrl.wen,
      orig_id_core_ctrl.ren1 | orig_id_core_ctrl.ren2 | orig_id_core_ctrl.ren3 | orig_id_core_ctrl.rend,
      orig_id_core_ctrl.m0ren | orig_id_core_ctrl.mallren | orig_id_core_ctrl.mren1 | orig_id_core_ctrl.mren2,
      orig_id_core_ctrl.fromint,
      orig_id_core_ctrl.tfma_enabled,
      orig_id_core_ctrl.tquant_enabled,
      orig_id_core_ctrl.reduce_enabled,
      orig_ex_core_ctrl.tointm,
      orig_f2_core_ctrl.fma,
      orig_f3_core_ctrl.fma,
      orig_wb_core_ctrl.fma,
      orig_wb_core_ctrl.tointm,
      orig_dcache_ctrl.scp_req.read_en,
      orig_dcache_ctrl.tenb_credit,
      orig_dcache_ctrl.tfma_enabled,
      orig_dcache_ctrl.reduce_wait,
      orig_dcache_ctrl.tfma_rf_write,
      orig_io_events,
      orig_vpu_dbg_match,
      orig_vpu_dbg_filter,
      orig_vpu_dbg_data,
      u_orig.ex_sh_sw_valid,
      |u_orig.ex_tima_valid,
      |u_orig.ex_txfma_valid,
      u_orig.ex_txfma_clock_valid,
      u_orig.ex_sh_sw_clock_valid,
      u_orig.ex_rom_clock_valid,
      u_orig.ex_rom_valid,
      u_orig.ctrl.id_vpu_ctrl.rm
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

  assign new_protocol_o = protocol_bits(
      scenario_i,
      in,
      new_id_core_ctrl.wen,
      new_id_core_ctrl.ren1 | new_id_core_ctrl.ren2 | new_id_core_ctrl.ren3 | new_id_core_ctrl.rend,
      new_id_core_ctrl.m0ren | new_id_core_ctrl.mallren | new_id_core_ctrl.mren1 | new_id_core_ctrl.mren2,
      new_id_core_ctrl.fromint,
      new_id_core_ctrl.tfma_enabled,
      new_id_core_ctrl.tquant_enabled,
      new_id_core_ctrl.reduce_enabled,
      new_ex_core_ctrl.tointm,
      new_f2_core_ctrl.fma,
      new_f3_core_ctrl.fma,
      new_wb_core_ctrl.fma,
      new_wb_core_ctrl.tointm,
      new_dcache_ctrl.scp_req.read_en,
      new_dcache_ctrl.tenb_credit,
      new_dcache_ctrl.tfma_enabled,
      new_dcache_ctrl.reduce_wait,
      new_dcache_ctrl.tfma_rf_write,
      new_io_events,
      new_vpu_dbg_match,
      new_vpu_dbg_filter,
      new_vpu_dbg_data,
      u_new.ex_sh_sw_valid,
      |u_new.ex_tima_valid,
      |u_new.ex_txfma_valid,
      u_new.ex_txfma_clock_valid,
      u_new.ex_sh_sw_clock_valid,
      u_new.ex_rom_clock_valid,
      u_new.ex_rom_valid,
      u_new.ctrl.id_vpu_ctrl.rm
  );

endmodule
