// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_ctrl_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>
#include <vector>

using DUT = Vcosim_vpu_ctrl_tb;

namespace {

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

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 70000;

  std::vector<uint32_t> input_ones;
  std::vector<uint32_t> input_zeros;

  fill_wide(sim.dut->inputs_i, 0);
  sample_wide(sim.dut->inputs_i, input_ones, input_zeros);
  sim.reset();
  compare_outputs(sim);

  fill_wide(sim.dut->inputs_i, 0xffffffffu);
  sample_wide(sim.dut->inputs_i, input_ones, input_zeros);
  sim.tick();
  compare_outputs(sim);

  fill_wide(sim.dut->inputs_i, 0);
  sample_wide(sim.dut->inputs_i, input_ones, input_zeros);
  for (int i = 0; i < 32; ++i) {
    sim.tick();
    compare_outputs(sim);
  }

  uint32_t seed = 0xc7171525u;
  for (int cycle = 0; cycle < 18000; ++cycle) {
    randomize_wide(sim.dut->inputs_i, seed);
    sample_wide(sim.dut->inputs_i, input_ones, input_zeros);
    sim.tick();
    compare_outputs(sim);
  }

  sim.check(full_coverage(input_ones, input_zeros),
            "vpu_ctrl aggregate input bus drives every bit to both 0 and 1");

  return sim.finish();
}
