// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vpseudo_lru_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <random>

using DUT = Vpseudo_lru_tb;

namespace {

constexpr int kNWays = 8;
constexpr int kIdxWidth = 3;

using PlruState = std::array<uint8_t, kNWays>;

static void model_access(PlruState& state, uint32_t index) {
  state[1] = ((index >> (kIdxWidth - 1)) & 1U) ? 0U : 1U;

  for (int st_idx = 2; st_idx < kNWays; ++st_idx) {
    int ptr_idx_bits = 0;
    for (int x = st_idx; x > 0; x >>= 1) {
      ++ptr_idx_bits;
    }

    uint32_t ptr_idx = 1U;
    for (int bit = 0; bit < ptr_idx_bits - 1; ++bit) {
      int src_bit = kIdxWidth - 1 - bit;
      ptr_idx = (ptr_idx << 1) | ((index >> src_bit) & 1U);
    }

    if (ptr_idx == static_cast<uint32_t>(st_idx)) {
      int update_bit = kIdxWidth - ptr_idx_bits;
      state[st_idx] = ((index >> update_bit) & 1U) ? 0U : 1U;
    }
  }
}

static uint32_t model_index_out(const PlruState& state) {
  uint32_t index = 0;
  index |= static_cast<uint32_t>(state[1]) << (kIdxWidth - 1);

  for (int out_bit = kIdxWidth - 2; out_bit >= 0; --out_bit) {
    int ptr_idx_bits = kIdxWidth - out_bit;
    uint32_t ptr_idx = 1U;
    for (int bit = 0; bit < ptr_idx_bits - 1; ++bit) {
      int src_bit = kIdxWidth - 1 - bit;
      ptr_idx = (ptr_idx << 1) | ((index >> src_bit) & 1U);
    }
    index |= static_cast<uint32_t>(state[ptr_idx]) << out_bit;
  }

  return index;
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();

  dut->clk_i = 0;
  dut->rst_ni = 1;
  dut->access_i = 0;
  dut->index_in_i = 0;

  PlruState model_state{};

  auto check_index = [&](const char* msg) {
    sim.check(dut->index_out_o == model_index_out(model_state), msg);
  };

  printf("test 1: reset drives zero replacement index\n");
  sim.reset();
  check_index("replacement index is zero after reset");

  printf("test 2: hold state when access is low\n");
  dut->access_i = 0;
  dut->index_in_i = 5;
  sim.tick();
  check_index("replacement index is held when access is low");

  printf("test 3: directed accesses update the tree\n");
  const uint32_t directed[] = {0, 4, 2, 6, 1, 5, 3, 7};
  for (uint32_t index : directed) {
    dut->access_i = 1;
    dut->index_in_i = index;
    sim.tick();
    model_access(model_state, index);
    check_index("replacement index matches directed software model");
  }

  printf("test 4: randomized accesses track software model\n");
  std::mt19937 rng(0x5eed1234);
  for (int cycle = 0; cycle < 256; ++cycle) {
    bool access = (rng() & 1U) != 0;
    uint32_t index = rng() & ((1U << kIdxWidth) - 1U);

    dut->access_i = access ? 1 : 0;
    dut->index_in_i = index;
    sim.tick();

    if (access) {
      model_access(model_state, index);
    }
    check_index("replacement index matches randomized software model");
  }

  return sim.finish();
}
