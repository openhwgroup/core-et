// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_scratchpad_ctrl.

#include "Vminion_dcache_scratchpad_ctrl_tb.h"
#include "sim_ctrl.h"

using DUT = Vminion_dcache_scratchpad_ctrl_tb;

namespace {

constexpr uint8_t kModeShared = 0;
constexpr uint8_t kModeSplit = 1;
constexpr uint8_t kModeScratchpad = 3;

void clear_inputs(DUT* dut) {
  dut->scratchpad_ctrl_i = kModeShared;
  dut->dcache_idle_i = 0;
  dut->da_clear_rdy_i = 0;
}

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void reset_and_clear(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  reset_and_clear(sim);
  sim.check(sim.dut->dcache_cfg_stall_o == 1, "reset starts in stalled configuration state");
  sim.check(sim.dut->dcache_cfg_mode_o == kModeShared, "reset mode is shared");
  sim.check(sim.dut->da_clear_en_o == 0, "reset does not clear data array yet");

  sim.dut->scratchpad_ctrl_i = kModeScratchpad;
  sim.dut->dcache_idle_i = 0;
  tick_and_eval(sim);
  sim.check(sim.dut->dcache_cfg_stall_o == 1, "controller stays stalled while dcache is busy");
  sim.check(sim.dut->dcache_cfg_clear_all_o == 0, "busy dcache does not start clear phase");

  sim.dut->dcache_idle_i = 1;
  tick_and_eval(sim);
  sim.check(sim.dut->dcache_cfg_clear_all_o == 1, "shared-to-scratchpad transition clears all lines");
  sim.check(sim.dut->dcache_cfg_clear_low_o == 0, "shared transition does not use low-only clear");
  sim.check(sim.dut->dcache_cfg_alloc_scp_o == 0, "shared transition does not allocate scratchpad half");

  sim.dut->dcache_idle_i = 0;
  sim.dut->da_clear_rdy_i = 1;
  tick_and_eval(sim);
  sim.check(sim.dut->da_clear_en_o == 1, "zeroing phase enables clear path");
  sim.check(sim.dut->da_clear_val_o == 1, "zeroing phase issues clear transactions");
  sim.check(sim.dut->da_clear_set_o == 0 && sim.dut->da_clear_idx_o == 0 && sim.dut->da_clear_way_o == 0,
            "zeroing starts from first row");

  for (int i = 0; i < 127; ++i) {
    tick_and_eval(sim);
  }
  tick_and_eval(sim);
  sim.check(sim.dut->dcache_cfg_stall_o == 0, "full shared clear completes after all rows");
  sim.check(sim.dut->dcache_cfg_mode_o == kModeScratchpad, "completed clear updates active mode");

  sim.dut->scratchpad_ctrl_i = kModeSplit;
  sim.dut->dcache_idle_i = 1;
  sim.dut->da_clear_rdy_i = 0;
  tick_and_eval(sim);
  sim.check(sim.dut->dcache_cfg_stall_o == 1, "mode change restarts controller");
  bool saw_clear_low = false;
  for (int i = 0; i < 4; ++i) {
    tick_and_eval(sim);
    if (sim.dut->dcache_cfg_clear_low_o) {
      saw_clear_low = true;
      break;
    }
  }
  sim.check(saw_clear_low, "non-shared transition clears only low partition");
  sim.check(sim.dut->dcache_cfg_clear_all_o == 0, "non-shared transition avoids full clear");

  sim.dut->dcache_idle_i = 0;
  sim.dut->da_clear_rdy_i = 1;
  tick_and_eval(sim);
  bool saw_split_mode = false;
  for (int i = 0; i < 128; ++i) {
    tick_and_eval(sim);
    if (sim.dut->dcache_cfg_mode_o == kModeSplit) {
      saw_split_mode = true;
      break;
    }
  }
  sim.check(saw_split_mode, "split transition updates active mode");

  sim.dut->scratchpad_ctrl_i = kModeScratchpad;
  sim.dut->dcache_idle_i = 1;
  sim.dut->da_clear_rdy_i = 0;
  tick_and_eval(sim);
  bool saw_alloc_scp = false;
  for (int i = 0; i < 4; ++i) {
    tick_and_eval(sim);
    if (sim.dut->dcache_cfg_alloc_scp_o) {
      saw_alloc_scp = true;
      break;
    }
  }
  sim.check(saw_alloc_scp, "split-to-scratchpad transition allocates scratchpad partition");

  return sim.finish();
}
