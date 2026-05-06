// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_tensorreduce_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vvpu_tensorreduce_tb;

namespace {

constexpr uint32_t kOpcodeMask = 0x7fu;
constexpr uint32_t kRdShift = 7;
constexpr uint32_t kRs1Shift = 15;
constexpr uint32_t kRs2Shift = 20;
constexpr uint32_t kDebugRegPendingShift = 18;
constexpr uint32_t kDebugCurRegShift = 10;
constexpr uint32_t kDebugReduceOpShift = 6;
constexpr uint32_t kDebugIsReduceBit = 5;
constexpr uint32_t kDebugColsPerRowShift = 3;
constexpr uint32_t kDebugScoreboardStickyBit = 2;
constexpr uint32_t kDebugTfmaStickyBit = 1;
constexpr uint32_t kDebugTquantStickyBit = 0;

uint64_t pack_reduce_ctrl(uint8_t start_reg, uint8_t num_regs, uint8_t op,
                          uint16_t partner = 0, uint8_t action = 0) {
  return (uint64_t(start_reg & 0x1fu) << 57) |
         (uint64_t(op & 0x0fu) << 24) |
         (uint64_t(num_regs & 0x7fu) << 16) |
         (uint64_t(partner & 0x1fffu) << 3) |
         uint64_t(action & 0x3u);
}

uint64_t pack_tensorstore_ctrl(uint8_t src_inc, uint8_t start_reg, uint8_t cols,
                               uint8_t rows, bool scp = false, uint8_t coop = 0,
                               uint64_t addr = 0, uint8_t rate = 0) {
  return (uint64_t(src_inc & 0x3u) << 62) |
         (uint64_t(start_reg & 0x1fu) << 57) |
         (uint64_t(cols & 0x3u) << 55) |
         (uint64_t(rows & 0x0fu) << 51) |
         (uint64_t(coop & 0x3u) << 49) |
         (uint64_t(scp ? 1u : 0u) << 48) |
         ((addr & ((uint64_t{1} << 44) - 1)) << 4) |
         uint64_t(rate & 0x0fu);
}

uint8_t pack_dcache_reduce_ctrl(bool send_reg, bool exec_op, bool nothing) {
  return (uint8_t(send_reg) << 2) | (uint8_t(exec_op) << 1) | uint8_t(nothing);
}

uint32_t inst_field(uint32_t inst, uint32_t shift) {
  return (inst >> shift) & 0x1fu;
}

uint32_t debug_reg_pending(uint32_t debug) {
  return (debug >> kDebugRegPendingShift) & 0x7fu;
}

uint32_t debug_cur_reg(uint32_t debug) {
  return (debug >> kDebugCurRegShift) & 0x1fu;
}

uint32_t debug_reduce_op(uint32_t debug) {
  return (debug >> kDebugReduceOpShift) & 0x0fu;
}

void clear_inputs(DUT* dut) {
  dut->reduce_start_i = 0;
  dut->tensorstore_start_i = 0;
  dut->tensorfma_start_i = 0;
  dut->tensorquant_start_i = 0;
  dut->reduce_ctrl_bits_i = 0;
  dut->scoreboard_pend_i = 0;
  dut->tensorfma_pend_i = 0;
  dut->tensorquant_pend_i = 0;
  dut->dcache_reduce_ctrl_bits_i = 0;
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 500;

  clear_inputs(sim.dut.get());
  sim.reset();

  sim.check(sim.dut->enabled_o == 0, "reset leaves tensorreduce disabled");
  sim.check(sim.dut->reduce_wait_o == 0, "reset leaves reduce_wait low");
  sim.check(sim.dut->load_ctrl_bits_o == 0, "reset clears load_ctrl");
  sim.check(sim.dut->reduce_inst_en_next_o == 0, "reset leaves inst enable low");
  sim.check(sim.dut->vpu_tensorreduce_debug_o == 0, "reset clears debug state");

  sim.dut->reduce_ctrl_bits_i = pack_reduce_ctrl(3, 4, 0);
  sim.dut->reduce_start_i = 1;
  sim.dut->scoreboard_pend_i = 1;
  sim.dut->tensorfma_pend_i = 1;
  sim.dut->tensorquant_pend_i = 1;
  sim.tick();
  sim.dut->reduce_start_i = 0;

  sim.check(sim.dut->enabled_o == 1, "reduce request enables the FSM");
  sim.check(sim.dut->reduce_wait_o == 1, "pending hazards raise reduce_wait");
  sim.check(debug_reg_pending(sim.dut->vpu_tensorreduce_debug_o) == 4,
            "reduce request loads pending register count");
  sim.check(debug_cur_reg(sim.dut->vpu_tensorreduce_debug_o) == 3,
            "reduce request loads the start register");
  sim.check(debug_reduce_op(sim.dut->vpu_tensorreduce_debug_o) == 0,
            "reduce request captures the reduce opcode");
  sim.check(((sim.dut->vpu_tensorreduce_debug_o >> kDebugIsReduceBit) & 1u) == 1u,
            "reduce request marks the flow as reduce");
  sim.check(((sim.dut->vpu_tensorreduce_debug_o >> kDebugScoreboardStickyBit) & 1u) == 1u,
            "reduce request latches scoreboard hazard");
  sim.check(((sim.dut->vpu_tensorreduce_debug_o >> kDebugTfmaStickyBit) & 1u) == 1u,
            "reduce request latches tensorfma hazard");
  sim.check(((sim.dut->vpu_tensorreduce_debug_o >> kDebugTquantStickyBit) & 1u) == 1u,
            "reduce request latches tensorquant hazard");

  sim.dut->scoreboard_pend_i = 0;
  sim.dut->tensorfma_pend_i = 0;
  sim.dut->tensorquant_pend_i = 0;
  sim.tick();
  sim.check(sim.dut->reduce_wait_o == 0, "hazards clear after the sticky bits drain");

  sim.dut->dcache_reduce_ctrl_bits_i = pack_dcache_reduce_ctrl(true, false, false);
  sim.dut->eval();
  sim.check(sim.dut->reduce_inst_en_next_o == 1, "send_reg requests a fake store instruction");
  sim.check((sim.dut->reduce_inst_next_o & kOpcodeMask) == 0x0bu,
            "send_reg emits the FSW_PS opcode field");
  sim.check(inst_field(sim.dut->reduce_inst_next_o, kRdShift) == 3,
            "send_reg uses the current destination register field");
  sim.check(inst_field(sim.dut->reduce_inst_next_o, kRs1Shift) == 3,
            "send_reg uses the current rs1 register field");
  sim.check(inst_field(sim.dut->reduce_inst_next_o, kRs2Shift) == 3,
            "send_reg uses the current rs2 register field");
  sim.tick();
  sim.dut->dcache_reduce_ctrl_bits_i = 0;

  sim.dut->dcache_reduce_ctrl_bits_i = pack_dcache_reduce_ctrl(false, true, false);
  sim.tick();
  sim.dut->dcache_reduce_ctrl_bits_i = 0;
  sim.check(sim.dut->reduce_inst_en_next_o == 1, "exec_op produces a reduce instruction one cycle later");
  sim.check((sim.dut->reduce_inst_next_o & kOpcodeMask) == 0x7bu,
            "exec_op emits an arithmetic VPU opcode");
  sim.tick();

  sim.dut->dcache_reduce_ctrl_bits_i = pack_dcache_reduce_ctrl(false, false, true);
  sim.tick();
  sim.dut->dcache_reduce_ctrl_bits_i = 0;
  sim.check(debug_reg_pending(sim.dut->vpu_tensorreduce_debug_o) == 0,
            "nothing input clears the pending register count");
  for (int i = 0; i < 4; ++i) {
    sim.tick();
  }
  sim.check(sim.dut->enabled_o == 0, "enabled drops after the exec countdown drains");

  sim.dut->reduce_ctrl_bits_i = pack_tensorstore_ctrl(2, 9, 1, 2, true, 0, 0x10, 3);
  sim.dut->tensorstore_start_i = 1;
  sim.tick();
  sim.dut->tensorstore_start_i = 0;

  sim.check(sim.dut->enabled_o == 1, "tensorstore request also enables the FSM");
  sim.check(((sim.dut->vpu_tensorreduce_debug_o >> kDebugIsReduceBit) & 1u) == 0u,
            "tensorstore request clears the reduce-mode flag");
  sim.check(((sim.dut->vpu_tensorreduce_debug_o >> kDebugColsPerRowShift) & 0x3u) == 1u,
            "tensorstore request captures the column count");

  return sim.finish();
}
