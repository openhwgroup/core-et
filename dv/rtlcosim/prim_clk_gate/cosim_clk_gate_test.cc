// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_clk_gate_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_clk_gate_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

static void eval_compare(CosimCtrl<DUT>& sim, const std::string& name) {
    sim.dut->eval();
    sim.compare(name, sim.dut->orig_clk_o, sim.dut->new_clk_o);
    sim.sim_time++;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 10000;

    uint32_t rng = 0x2468ace1u;

    sim.dut->clk_i = 0;
    sim.dut->en_i = 0;
    sim.dut->scanmode_i = 0;
    eval_compare(sim, "init_low");

    sim.dut->en_i = 1;
    eval_compare(sim, "enable_while_low");

    sim.dut->clk_i = 1;
    eval_compare(sim, "rise_after_enable");

    sim.dut->en_i = 0;
    eval_compare(sim, "disable_while_high");

    sim.dut->scanmode_i = 1;
    eval_compare(sim, "scan_force_while_high");

    sim.dut->clk_i = 0;
    eval_compare(sim, "fall_with_scan");

    sim.dut->scanmode_i = 0;
    eval_compare(sim, "release_scan_while_low");

    for (int i = 0; i < 4096; ++i) {
      uint32_t r = next_rand(rng);
      switch (r & 0x7) {
      case 0:
        sim.dut->clk_i = 0;
        break;
      case 1:
        sim.dut->clk_i = 1;
        break;
      case 2:
        sim.dut->en_i = r >> 8 & 0x1;
        break;
      case 3:
        sim.dut->scanmode_i = r >> 9 & 0x1;
        break;
      case 4:
        sim.dut->en_i = r >> 10 & 0x1;
        sim.dut->scanmode_i = r >> 11 & 0x1;
        break;
      case 5:
        sim.dut->clk_i = r >> 12 & 0x1;
        sim.dut->en_i = r >> 13 & 0x1;
        break;
      case 6:
        sim.dut->clk_i = r >> 14 & 0x1;
        sim.dut->scanmode_i = r >> 15 & 0x1;
        break;
      default:
        break;
      }

      eval_compare(sim, "random_step_" + std::to_string(i));
    }

    return sim.finish();
}
