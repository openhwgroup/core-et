// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_top_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>
#include <vector>

using DUT = Vcosim_vpu_top_tb;

namespace {

constexpr uint8_t kScenarioIdle        = 0;
constexpr uint8_t kScenarioAllOnes     = 1;
constexpr uint8_t kScenarioShSw        = 2;
constexpr uint8_t kScenarioTxfma       = 3;
constexpr uint8_t kScenarioRomTrans    = 4;
constexpr uint8_t kScenarioTima        = 5;
constexpr uint8_t kScenarioTensorQuant = 6;
constexpr uint8_t kScenarioReduce      = 7;
constexpr uint8_t kScenarioDcache      = 8;
constexpr uint8_t kScenarioKill        = 9;
constexpr uint8_t kScenarioChicken     = 10;
constexpr uint8_t kScenarioThread      = 11;
constexpr uint8_t kScenarioRandom      = 0xff;

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

template <typename Wide>
void fill_wide(Wide& words, uint32_t value) {
  for (size_t i = 0; i < words.size(); ++i) words[i] = value;
}

template <typename Wide>
void randomize_wide(Wide& words, uint32_t& seed) {
  for (size_t i = 0; i < words.size(); ++i) words[i] = xorshift32(seed);
}

template <typename Wide>
void sample_wide(const Wide& words, std::vector<uint32_t>& ones, std::vector<uint32_t>& zeros) {
  if (ones.empty()) {
    ones.assign(words.size(), 0);
    zeros.assign(words.size(), 0);
  }
  for (size_t i = 0; i < words.size(); ++i) {
    ones[i] |= words[i];
    zeros[i] |= ~words[i];
  }
}

bool full_coverage(const std::vector<uint32_t>& ones, const std::vector<uint32_t>& zeros) {
  if (ones.empty() || ones.size() != zeros.size()) return false;
  for (size_t i = 0; i < ones.size(); ++i) {
    if (ones[i] != 0xffffffffu || zeros[i] != 0xffffffffu) return false;
  }
  return true;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  for (size_t i = 0; i < sim.dut->orig_outputs_o.size(); ++i) {
    sim.compare("outputs[" + std::to_string(i) + "]",
                sim.dut->orig_outputs_o[i], sim.dut->new_outputs_o[i]);
  }
}

void tick_compare_sample(CosimCtrl<DUT>& sim,
                         std::vector<uint32_t>& input_ones,
                         std::vector<uint32_t>& input_zeros) {
  sim.tick();
  sample_wide(sim.dut->applied_inputs_o, input_ones, input_zeros);
  compare_outputs(sim);
}

void run_scenario(CosimCtrl<DUT>& sim,
                  uint8_t scenario,
                  int cycles,
                  std::vector<uint32_t>& input_ones,
                  std::vector<uint32_t>& input_zeros) {
  sim.dut->scenario_i = scenario;
  for (int i = 0; i < cycles; ++i) {
    tick_compare_sample(sim, input_ones, input_zeros);
  }
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 350000;

  std::vector<uint32_t> input_ones;
  std::vector<uint32_t> input_zeros;

  fill_wide(sim.dut->random_inputs_i, 0);
  sim.dut->scenario_i = kScenarioIdle;
  sim.reset();
  sample_wide(sim.dut->applied_inputs_o, input_ones, input_zeros);
  compare_outputs(sim);

  run_scenario(sim, kScenarioIdle, 12, input_ones, input_zeros);
  run_scenario(sim, kScenarioAllOnes, 2, input_ones, input_zeros);
  run_scenario(sim, kScenarioIdle, 12, input_ones, input_zeros);

  // Directed semantic phases before random activity. The SV wrapper maps these
  // scenario IDs onto the real top-level input structs so the original and new
  // tops see identical no-op, sh/sw, TXFMA, ROM/trans, TIMA/tensor-FMA,
  // tensor-quant, reduce/store, dcache, kill/stall, chicken-bit, and mixed
  // thread-ID transactions.
  run_scenario(sim, kScenarioShSw, 24, input_ones, input_zeros);
  run_scenario(sim, kScenarioIdle, 12, input_ones, input_zeros);
  run_scenario(sim, kScenarioTxfma, 32, input_ones, input_zeros);
  run_scenario(sim, kScenarioIdle, 16, input_ones, input_zeros);
  run_scenario(sim, kScenarioRomTrans, 32, input_ones, input_zeros);
  run_scenario(sim, kScenarioIdle, 16, input_ones, input_zeros);
  run_scenario(sim, kScenarioTima, 40, input_ones, input_zeros);
  run_scenario(sim, kScenarioIdle, 16, input_ones, input_zeros);
  run_scenario(sim, kScenarioTensorQuant, 40, input_ones, input_zeros);
  run_scenario(sim, kScenarioIdle, 16, input_ones, input_zeros);
  run_scenario(sim, kScenarioReduce, 40, input_ones, input_zeros);
  run_scenario(sim, kScenarioDcache, 32, input_ones, input_zeros);
  run_scenario(sim, kScenarioKill, 24, input_ones, input_zeros);
  run_scenario(sim, kScenarioChicken, 24, input_ones, input_zeros);
  run_scenario(sim, kScenarioThread, 32, input_ones, input_zeros);
  run_scenario(sim, kScenarioIdle, 32, input_ones, input_zeros);

  uint32_t seed = 0x7055a117u;
  sim.dut->scenario_i = kScenarioRandom;
  for (int cycle = 0; cycle < 100000; ++cycle) {
    randomize_wide(sim.dut->random_inputs_i, seed);
    tick_compare_sample(sim, input_ones, input_zeros);
  }

  run_scenario(sim, kScenarioIdle, 32, input_ones, input_zeros);

  sim.check(full_coverage(input_ones, input_zeros),
            "vpu_top applied input bus drives every bit to both 0 and 1");

  return sim.finish();
}
