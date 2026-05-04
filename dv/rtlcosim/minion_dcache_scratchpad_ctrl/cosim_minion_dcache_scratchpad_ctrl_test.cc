// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_scratchpad_ctrl vs minion_dcache_scratchpad_ctrl.

#include "Vcosim_minion_dcache_scratchpad_ctrl_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_minion_dcache_scratchpad_ctrl_tb;

namespace {

constexpr uint8_t kModeShared = 0;
constexpr uint8_t kModeSplit = 1;
constexpr uint8_t kModeScratchpad = 3;

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
  BoolCoverage dcache_idle;
  BoolCoverage da_clear_rdy;
  bool seen_mode_shared = false;
  bool seen_mode_split = false;
  bool seen_mode_scratchpad = false;
};

void clear_inputs(DUT* dut) {
  dut->scratchpad_ctrl_i = kModeShared;
  dut->dcache_idle_i = 0;
  dut->da_clear_rdy_i = 0;
}

void sample_coverage(DUT* dut, Coverage& cov) {
  cov.dcache_idle.sample(dut->dcache_idle_i);
  cov.da_clear_rdy.sample(dut->da_clear_rdy_i);
  if (dut->scratchpad_ctrl_i == kModeShared) cov.seen_mode_shared = true;
  if (dut->scratchpad_ctrl_i == kModeSplit) cov.seen_mode_split = true;
  if (dut->scratchpad_ctrl_i == kModeScratchpad) cov.seen_mode_scratchpad = true;
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(cov.dcache_idle.full(), "coverage: dcache_idle exercised 0/1");
  sim.check(cov.da_clear_rdy.full(), "coverage: da_clear_rdy exercised 0/1");
  sim.check(cov.seen_mode_shared && cov.seen_mode_split && cov.seen_mode_scratchpad,
            "coverage: scratchpad_ctrl exercised shared/split/scratchpad");
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("dcache_cfg_stall", sim.dut->orig_dcache_cfg_stall_o, sim.dut->new_dcache_cfg_stall_o);
  sim.compare("dcache_cfg_clear_all", sim.dut->orig_dcache_cfg_clear_all_o, sim.dut->new_dcache_cfg_clear_all_o);
  sim.compare("dcache_cfg_clear_low", sim.dut->orig_dcache_cfg_clear_low_o, sim.dut->new_dcache_cfg_clear_low_o);
  sim.compare("dcache_cfg_alloc_scp", sim.dut->orig_dcache_cfg_alloc_scp_o, sim.dut->new_dcache_cfg_alloc_scp_o);
  sim.compare("da_clear_en", sim.dut->orig_da_clear_en_o, sim.dut->new_da_clear_en_o);
  sim.compare("da_clear_val", sim.dut->orig_da_clear_val_o, sim.dut->new_da_clear_val_o);
  sim.compare("da_clear_set", sim.dut->orig_da_clear_set_o, sim.dut->new_da_clear_set_o);
  sim.compare("da_clear_way", sim.dut->orig_da_clear_way_o, sim.dut->new_da_clear_way_o);
  sim.compare("da_clear_idx", sim.dut->orig_da_clear_idx_o, sim.dut->new_da_clear_idx_o);
  sim.compare("dcache_cfg_mode", sim.dut->orig_dcache_cfg_mode_o, sim.dut->new_dcache_cfg_mode_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim, Coverage* cov = nullptr) {
  if (cov != nullptr) sample_coverage(sim.dut.get(), *cov);
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
  compare_outputs(sim);
}

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  Coverage coverage;

  clear_inputs(sim.dut.get());
  sample_coverage(sim.dut.get(), coverage);
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
  compare_outputs(sim);

  sim.dut->scratchpad_ctrl_i = kModeScratchpad;
  tick_and_compare(sim, &coverage);
  sim.dut->dcache_idle_i = 1;
  tick_and_compare(sim, &coverage);
  sim.dut->dcache_idle_i = 0;
  sim.dut->da_clear_rdy_i = 1;
  for (int i = 0; i < 140; ++i) {
    tick_and_compare(sim, &coverage);
  }

  sim.dut->scratchpad_ctrl_i = kModeSplit;
  sim.dut->dcache_idle_i = 1;
  sim.dut->da_clear_rdy_i = 0;
  tick_and_compare(sim, &coverage);
  tick_and_compare(sim, &coverage);
  sim.dut->dcache_idle_i = 0;
  sim.dut->da_clear_rdy_i = 1;
  for (int i = 0; i < 128; ++i) {
    tick_and_compare(sim, &coverage);
  }

  constexpr uint32_t kSeeds[] = {0x13579BDFu, 0xC001CAFEu, 0x5F3759DFu};
  for (uint32_t seed_init : kSeeds) {
    uint32_t seed = seed_init;
    clear_inputs(sim.dut.get());
    for (int cycle = 0; cycle < 512; ++cycle) {
      const uint32_t mode_sel = xorshift32(seed) % 3u;
      sim.dut->scratchpad_ctrl_i =
          (mode_sel == 0) ? kModeShared : ((mode_sel == 1) ? kModeSplit : kModeScratchpad);
      sim.dut->dcache_idle_i = xorshift32(seed) & 1u;
      sim.dut->da_clear_rdy_i = xorshift32(seed) & 1u;
      tick_and_compare(sim, &coverage);
    }
  }

  check_coverage(sim, coverage);
  return sim.finish();
}
