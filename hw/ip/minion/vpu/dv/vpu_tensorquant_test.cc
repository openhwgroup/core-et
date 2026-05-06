// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_tensorquant_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vvpu_tensorquant_tb;

namespace {

constexpr int kCoreTensorquantCtrlPos = 1;
constexpr int kCoreTensorquantStartPos = 65;
constexpr int kCoreTensorstoreStartPos = 130;
constexpr int kCoreTensorfmaStartPos = 213;
constexpr int kCorePendingScoreboardPos = 0;

constexpr int kTqTransPos = 0;
constexpr int kTqScpSrcPos = 45;
constexpr int kTqRowsPos = 51;
constexpr int kTqColsPos = 55;
constexpr int kTqStartRegPos = 57;

constexpr uint8_t TQ_LAST = 0;
constexpr uint8_t TQ_I32_TO_F32 = 1;
constexpr uint8_t TQ_F32_TO_I32 = 2;
constexpr uint8_t TQ_RELU = 3;
constexpr uint8_t TQ_ADD_ROW = 4;
constexpr uint8_t TQ_ADD_COL = 5;
constexpr uint8_t TQ_MUL_ROW = 6;
constexpr uint8_t TQ_MUL_COL = 7;
constexpr uint8_t TQ_SAT_I8 = 8;
constexpr uint8_t TQ_SAT_U8 = 9;

constexpr uint32_t kOpcodeMask = 0x7f;
constexpr uint32_t kVpuOpcode = 0x7b;
constexpr uint32_t kDebugEnabledBit = 10;
constexpr uint32_t kDebugScoreboardStickyBit = 9;
constexpr uint32_t kDebugTfmaStickyBit = 8;
constexpr uint32_t kDebugTstoreStickyBit = 7;
constexpr uint32_t kDebugTransformShift = 1;
constexpr uint32_t kScpReqReadEnBit = 15;

void set_bits(WData* words, int pos, int width, uint64_t value) {
  for (int bit = 0; bit < width; ++bit) {
    const int idx = pos + bit;
    const uint32_t mask = uint32_t{1} << (idx & 31);
    if ((value >> bit) & 1ull) words[idx >> 5] |= mask;
    else words[idx >> 5] &= ~mask;
  }
}

void clear_core(DUT* dut) {
  for (int i = 0; i < 7; ++i) dut->core_ctrl_bits_i[i] = 0;
}

void clear_data(DUT* dut) {
  for (int i = 0; i < 8; ++i) dut->dcache_scp_data_i[i] = 0;
}

void clear_inputs(DUT* dut) {
  clear_core(dut);
  clear_data(dut);
  dut->tensorfma_pend_i = 0;
  dut->tensorstore_pend_i = 0;
  dut->dcache_scp_available_i = 0;
}

uint64_t trans_list(std::initializer_list<uint8_t> ops) {
  uint64_t trans = 0;
  int pos = 0;
  for (uint8_t op : ops) {
    trans |= uint64_t(op & 0x0fu) << pos;
    pos += 4;
  }
  return trans;
}

uint64_t pack_quant_ctrl(uint8_t start_reg, uint8_t cols, uint8_t rows,
                         uint8_t scp_src, uint64_t trans) {
  return (uint64_t(start_reg & 0x1fu) << kTqStartRegPos) |
         (uint64_t(cols & 0x3u) << kTqColsPos) |
         (uint64_t(rows & 0x0fu) << kTqRowsPos) |
         (uint64_t(scp_src & 0x3fu) << kTqScpSrcPos) |
         ((trans & ((uint64_t{1} << 40) - 1)) << kTqTransPos);
}

void drive_quant_start(DUT* dut, uint64_t ctrl, bool scoreboard = false) {
  clear_core(dut);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorquantStartPos, 1, 1);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorquantCtrlPos, 64, ctrl);
  set_bits(dut->core_ctrl_bits_i, kCorePendingScoreboardPos, 1, scoreboard);
}

void idle_core(DUT* dut) {
  set_bits(dut->core_ctrl_bits_i, kCoreTensorquantStartPos, 1, 0);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaStartPos, 1, 0);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorstoreStartPos, 1, 0);
}

bool is_fcvt_pw_ps(uint32_t inst) { return ((inst >> 20) == 0xc00u) && ((inst & kOpcodeMask) == kVpuOpcode); }
bool is_fcvt_ps_pw(uint32_t inst) { return ((inst >> 20) == 0xd00u) && ((inst & kOpcodeMask) == kVpuOpcode); }
bool is_fmax_pi(uint32_t inst) { return ((inst >> 25) == 0x17u) && (((inst >> 12) & 0x7u) == 1u); }
bool is_fadd_pi(uint32_t inst) { return ((inst >> 25) == 0x03u) && (((inst >> 12) & 0x7u) == 0u); }
bool is_fmul_ps(uint32_t inst) { return ((inst >> 25) == 0x08u) && ((inst & kOpcodeMask) == kVpuOpcode); }
bool is_fsat8_pi(uint32_t inst) { return ((inst >> 20) == 0x060u) && (((inst >> 12) & 0x7u) == 3u); }
bool is_fsatu8_pi(uint32_t inst) { return ((inst >> 20) == 0x061u) && (((inst >> 12) & 0x7u) == 3u); }

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 1500;

  clear_inputs(sim.dut.get());
  sim.reset();

  sim.check(sim.dut->enabled_o == 0, "reset leaves TensorQuant disabled");
  sim.check(((sim.dut->dcache_scp_req_bits_o >> kScpReqReadEnBit) & 1u) == 0,
            "reset leaves scratchpad request low");
  sim.check(sim.dut->quant_en_next_o == 0, "reset clears quant_en_next");
  sim.check(sim.dut->quant_tena_en_o == 0, "reset clears quant_tena_en");
  sim.check(sim.dut->ex_ren_o == 0, "reset clears ex_ren");

  drive_quant_start(sim.dut.get(), pack_quant_ctrl(0, 0, 0, 0, trans_list({TQ_LAST})));
  sim.tick();
  idle_core(sim.dut.get());
  sim.check(sim.dut->enabled_o == 0, "TQUANT_LAST-only transform list is ignored");

  clear_inputs(sim.dut.get());
  sim.reset();
  drive_quant_start(sim.dut.get(), pack_quant_ctrl(2, 1, 0, 4,
      trans_list({TQ_F32_TO_I32, TQ_RELU, TQ_ADD_ROW, TQ_ADD_COL, TQ_MUL_ROW,
                  TQ_MUL_COL, TQ_SAT_I8, TQ_SAT_U8, TQ_LAST})), true);
  sim.dut->tensorfma_pend_i = 1;
  sim.dut->tensorstore_pend_i = 1;
  sim.dut->dcache_scp_available_i = 0;
  sim.tick();
  idle_core(sim.dut.get());
  sim.check(sim.dut->enabled_o == 1, "non-empty transform list enables TensorQuant");
  sim.check(((sim.dut->vpu_tensorquant_debug_o >> kDebugScoreboardStickyBit) & 1u) == 1,
            "TensorQuant latches scoreboard hazard");
  sim.check(((sim.dut->vpu_tensorquant_debug_o >> kDebugTfmaStickyBit) & 1u) == 1,
            "TensorQuant latches TensorFMA hazard");
  sim.check(((sim.dut->vpu_tensorquant_debug_o >> kDebugTstoreStickyBit) & 1u) == 1,
            "TensorQuant latches TensorStore hazard");
  sim.check(sim.dut->quant_en_next_o == 0, "hazards block quant instruction issue");

  set_bits(sim.dut->core_ctrl_bits_i, kCorePendingScoreboardPos, 1, 0);
  sim.dut->tensorfma_pend_i = 0;
  sim.dut->tensorstore_pend_i = 0;
  bool saw_fcvt = false;
  bool saw_relu = false;
  bool saw_fadd = false;
  bool saw_fmul = false;
  bool saw_sat_i8 = false;
  bool saw_sat_u8 = false;
  bool saw_scp_req = false;
  bool saw_tena = false;
  bool saw_ex_ren = false;
  bool saw_ex_data = false;
  for (int i = 0; i < 180; ++i) {
    sim.dut->dcache_scp_available_i = (i > 2);
    for (int w = 0; w < 8; ++w) sim.dut->dcache_scp_data_i[w] = 0x11110000u + uint32_t(i * 8 + w);
    sim.tick();
    saw_scp_req |= ((sim.dut->dcache_scp_req_bits_o >> kScpReqReadEnBit) & 1u) != 0;
    saw_tena |= sim.dut->quant_tena_en_o;
    saw_ex_ren |= sim.dut->ex_ren_o;
    saw_ex_data |= sim.dut->ex_rdata_o[0] != 0;
    if (sim.dut->quant_en_next_o) {
      const uint32_t inst = sim.dut->quant_inst_next_o;
      saw_fcvt |= is_fcvt_pw_ps(inst) || is_fcvt_ps_pw(inst);
      saw_relu |= is_fmax_pi(inst);
      saw_fadd |= is_fadd_pi(inst);
      saw_fmul |= is_fmul_ps(inst);
      saw_sat_i8 |= is_fsat8_pi(inst);
      saw_sat_u8 |= is_fsatu8_pi(inst);
      sim.check((inst & kOpcodeMask) == kVpuOpcode, "issued TensorQuant instruction uses VPU opcode");
      sim.check(sim.dut->quant_mask_next_o != 0, "issued TensorQuant instruction has a non-zero mask");
    }
  }
  sim.check(saw_fcvt, "transform sequence emits FP32/int32 convert instruction");
  sim.check(saw_relu, "transform sequence emits ReLU max instruction");
  sim.check(saw_fadd, "transform sequence emits int32 add row/col instruction");
  sim.check(saw_fmul, "transform sequence emits FP32 multiply row/col instruction");
  sim.check(saw_sat_i8, "transform sequence emits signed int8 saturation instruction");
  sim.check(saw_sat_u8, "transform sequence emits unsigned int8 saturation instruction");
  sim.check(saw_scp_req, "row/column transforms request scratchpad data");
  sim.check(saw_tena, "scratchpad/zero transforms enable TENA read data");
  sim.check(saw_ex_ren, "TENA enable pipelines to ex_ren");
  sim.check(saw_ex_data, "scratchpad data reaches ex_rdata");
  sim.check(((sim.dut->vpu_tensorquant_debug_o >> kDebugEnabledBit) & 1u) == 0,
            "TensorQuant drains after transform list reaches TQUANT_LAST");

  clear_inputs(sim.dut.get());
  sim.reset();
  drive_quant_start(sim.dut.get(), pack_quant_ctrl(1, 0, 0, 6, trans_list({TQ_ADD_ROW, TQ_LAST})));
  sim.dut->dcache_scp_available_i = 0;
  sim.tick();
  idle_core(sim.dut.get());
  sim.check(((sim.dut->dcache_scp_req_bits_o >> kScpReqReadEnBit) & 1u) == 1,
            "TensorQuant keeps scratchpad request pending while unavailable");
  sim.check(sim.dut->quant_en_next_o == 0,
            "scratchpad-dependent transform waits until SCP data is available");
  sim.dut->dcache_scp_available_i = 1;
  for (int w = 0; w < 8; ++w) sim.dut->dcache_scp_data_i[w] = 0xa5a50000u + w;
  bool released_scp_wait = false;
  for (int i = 0; i < 16; ++i) {
    sim.tick();
    released_scp_wait |= sim.dut->quant_en_next_o || sim.dut->quant_tena_en_o || sim.dut->ex_ren_o;
  }
  sim.check(released_scp_wait,
            "available scratchpad data releases TENA/ex read path");

  return sim.finish();
}
