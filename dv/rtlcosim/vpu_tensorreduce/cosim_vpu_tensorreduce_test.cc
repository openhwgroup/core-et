// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_tensorreduce_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_vpu_tensorreduce_tb;

namespace {

struct BoolCoverage {
  bool seen0 = false;
  bool seen1 = false;
  void sample(bool v) {
    if (v) seen1 = true;
    else seen0 = true;
  }
  bool full() const { return seen0 && seen1; }
};

struct Coverage {
  BoolCoverage reduce_start, tensorstore_start, tensorfma_start, tensorquant_start;
  BoolCoverage scoreboard_pend, tensorfma_pend, tensorquant_pend;
  BoolCoverage send_reg, exec_op, nothing;
  BoolCoverage reduce_case, tensorstore_case;
  bool meaningful_odd_reduce_req = false;
  uint64_t reduce_ctrl_ones = 0;
  uint64_t reduce_ctrl_zeros = 0;
  uint8_t dcache_ctrl_ones = 0;
  uint8_t dcache_ctrl_zeros = 0;
};

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

uint64_t pack_reduce_ctrl(uint8_t start_reg, uint8_t num_regs, uint8_t op,
                          uint16_t partner = 0, uint8_t action = 0) {
  return (uint64_t(start_reg & 0x1fu) << 57) |
         (uint64_t(op & 0x0fu) << 24) |
         (uint64_t(num_regs & 0x7fu) << 16) |
         (uint64_t(partner & 0x1fffu) << 3) |
         uint64_t(action & 0x3u);
}

uint64_t pack_tensorstore_ctrl(uint8_t src_inc, uint8_t start_reg, uint8_t cols,
                               uint8_t rows, bool scp, uint8_t coop,
                               uint64_t addr, uint8_t rate) {
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

void sample_coverage(DUT* dut, Coverage& cov) {
  cov.reduce_start.sample(dut->reduce_start_i);
  cov.tensorstore_start.sample(dut->tensorstore_start_i);
  cov.tensorfma_start.sample(dut->tensorfma_start_i);
  cov.tensorquant_start.sample(dut->tensorquant_start_i);
  cov.scoreboard_pend.sample(dut->scoreboard_pend_i);
  cov.tensorfma_pend.sample(dut->tensorfma_pend_i);
  cov.tensorquant_pend.sample(dut->tensorquant_pend_i);
  cov.send_reg.sample((dut->dcache_reduce_ctrl_bits_i >> 2) & 1u);
  cov.exec_op.sample((dut->dcache_reduce_ctrl_bits_i >> 1) & 1u);
  cov.nothing.sample(dut->dcache_reduce_ctrl_bits_i & 1u);
  cov.reduce_case.sample(dut->reduce_start_i);
  cov.tensorstore_case.sample(dut->tensorstore_start_i);
  const uint8_t reduce_num_regs = static_cast<uint8_t>((dut->reduce_ctrl_bits_i >> 16) & 0x7fu);
  cov.meaningful_odd_reduce_req |= dut->reduce_start_i && !dut->tensorstore_start_i &&
                                   ((dut->dcache_reduce_ctrl_bits_i & 1u) == 0u) &&
                                   ((reduce_num_regs & 1u) != 0u) && (reduce_num_regs != 0u);
  cov.reduce_ctrl_ones |= dut->reduce_ctrl_bits_i;
  cov.reduce_ctrl_zeros |= ~dut->reduce_ctrl_bits_i;
  cov.dcache_ctrl_ones |= dut->dcache_reduce_ctrl_bits_i & 0x7u;
  cov.dcache_ctrl_zeros |= (~dut->dcache_reduce_ctrl_bits_i) & 0x7u;
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(cov.reduce_start.full(), "coverage: reduce_start exercised 0/1");
  sim.check(cov.tensorstore_start.full(), "coverage: tensorstore_start exercised 0/1");
  sim.check(cov.tensorfma_start.full(), "coverage: tensorfma_start exercised 0/1");
  sim.check(cov.tensorquant_start.full(), "coverage: tensorquant_start exercised 0/1");
  sim.check(cov.scoreboard_pend.full(), "coverage: scoreboard_pend exercised 0/1");
  sim.check(cov.tensorfma_pend.full(), "coverage: tensorfma_pend exercised 0/1");
  sim.check(cov.tensorquant_pend.full(), "coverage: tensorquant_pend exercised 0/1");
  sim.check(cov.send_reg.full(), "coverage: dcache_reduce_ctrl.send_reg exercised 0/1");
  sim.check(cov.exec_op.full(), "coverage: dcache_reduce_ctrl.exec_op exercised 0/1");
  sim.check(cov.nothing.full(), "coverage: dcache_reduce_ctrl.nothing exercised 0/1");
  sim.check(cov.reduce_case.full(), "coverage: reduce request path exercised");
  sim.check(cov.tensorstore_case.full(), "coverage: tensorstore request path exercised");
  sim.check(cov.meaningful_odd_reduce_req,
            "coverage: meaningful reduce request exercised odd num_regs");
  sim.check(cov.reduce_ctrl_ones == 0xffffffffffffffffull &&
                cov.reduce_ctrl_zeros == 0xffffffffffffffffull,
            "coverage: every reduce_ctrl_bits input bit exercised 0/1");
  sim.check(cov.dcache_ctrl_ones == 0x7u && cov.dcache_ctrl_zeros == 0x7u,
            "coverage: every dcache_reduce_ctrl_bits input bit exercised 0/1");
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("enabled", sim.dut->orig_enabled_o, sim.dut->new_enabled_o);
  sim.compare("reduce_wait", sim.dut->orig_reduce_wait_o, sim.dut->new_reduce_wait_o);
  sim.compare("load_ctrl_bits", sim.dut->orig_load_ctrl_bits_o, sim.dut->new_load_ctrl_bits_o);
  sim.compare("reduce_inst_en_next", sim.dut->orig_reduce_inst_en_next_o,
              sim.dut->new_reduce_inst_en_next_o);
  sim.compare("reduce_inst_next", sim.dut->orig_reduce_inst_next_o,
              sim.dut->new_reduce_inst_next_o);
  sim.compare("vpu_tensorreduce_debug", sim.dut->orig_vpu_tensorreduce_debug_o,
              sim.dut->new_vpu_tensorreduce_debug_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim, Coverage* cov = nullptr) {
  if (cov != nullptr) sample_coverage(sim.dut.get(), *cov);
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
  compare_outputs(sim);
}

void run_directed_odd_reduce(CosimCtrl<DUT>& sim, Coverage& cov) {
  clear_inputs(sim.dut.get());
  sim.dut->reduce_start_i = 1;
  sim.dut->reduce_ctrl_bits_i = pack_reduce_ctrl(
      5,   // start_reg
      3,   // odd num_regs, must occupy reduce_control_t bits [22:16]
      0,   // FADD reduce op
      0x15,
      1);
  sim.dut->dcache_reduce_ctrl_bits_i = pack_dcache_reduce_ctrl(false, false, false);
  tick_and_compare(sim, &cov);
  sim.check(sim.dut->new_enabled_o,
            "directed odd-num_regs reduce request enters enabled state");

  clear_inputs(sim.dut.get());
  sim.dut->dcache_reduce_ctrl_bits_i = pack_dcache_reduce_ctrl(true, false, false);
  tick_and_compare(sim, &cov);
  sim.check(sim.dut->new_reduce_inst_en_next_o,
            "directed odd-num_regs reduce request emits send-register instruction");

  clear_inputs(sim.dut.get());
  sim.dut->dcache_reduce_ctrl_bits_i = pack_dcache_reduce_ctrl(false, true, false);
  tick_and_compare(sim, &cov);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 60000;

  Coverage cov;
  clear_inputs(sim.dut.get());
  sim.reset();

  tick_and_compare(sim, &cov);

  sim.dut->reduce_start_i = 1;
  sim.dut->tensorstore_start_i = 1;
  sim.dut->tensorfma_start_i = 1;
  sim.dut->tensorquant_start_i = 1;
  sim.dut->reduce_ctrl_bits_i = 0xffffffffffffffffull;
  sim.dut->scoreboard_pend_i = 1;
  sim.dut->tensorfma_pend_i = 1;
  sim.dut->tensorquant_pend_i = 1;
  sim.dut->dcache_reduce_ctrl_bits_i = 0x7u;
  tick_and_compare(sim, &cov);

  run_directed_odd_reduce(sim, cov);

  uint32_t seed = 0x62f1c3b9u;
  for (int cycle = 0; cycle < 2500; ++cycle) {
    clear_inputs(sim.dut.get());

    const uint32_t mode = xorshift32(seed) % 3u;
    if (mode == 1u) {
      sim.dut->reduce_start_i = 1;
      sim.dut->reduce_ctrl_bits_i = pack_reduce_ctrl(
          xorshift32(seed) & 0x1fu,
          (xorshift32(seed) & 0x3fu) + 1u,
          xorshift32(seed) & 0x0fu,
          xorshift32(seed) & 0x1fffu,
          xorshift32(seed) & 0x3u);
    } else if (mode == 2u) {
      sim.dut->tensorstore_start_i = 1;
      sim.dut->reduce_ctrl_bits_i = pack_tensorstore_ctrl(
          xorshift32(seed) & 0x3u,
          xorshift32(seed) & 0x1fu,
          xorshift32(seed) & 0x3u,
          xorshift32(seed) & 0x0fu,
          (xorshift32(seed) & 1u) != 0u,
          xorshift32(seed) & 0x3u,
          xorshift32(seed) & ((uint32_t{1} << 20) - 1u),
          xorshift32(seed) & 0x0fu);
    } else {
      sim.dut->reduce_ctrl_bits_i = 0;
    }

    sim.dut->tensorfma_start_i = (xorshift32(seed) & 1u) != 0u;
    sim.dut->tensorquant_start_i = (xorshift32(seed) & 1u) != 0u;
    sim.dut->scoreboard_pend_i = (xorshift32(seed) & 1u) != 0u;
    sim.dut->tensorfma_pend_i = (xorshift32(seed) & 1u) != 0u;
    sim.dut->tensorquant_pend_i = (xorshift32(seed) & 1u) != 0u;
    sim.dut->dcache_reduce_ctrl_bits_i = pack_dcache_reduce_ctrl(
        (xorshift32(seed) & 1u) != 0u,
        (xorshift32(seed) & 1u) != 0u,
        (xorshift32(seed) & 1u) != 0u);

    tick_and_compare(sim, &cov);
  }

  check_coverage(sim, cov);
  sim.check(sim.comparisons >= 12000,
            "vpu_tensorreduce cosim should generate at least 12000 comparisons");
  return sim.finish();
}
