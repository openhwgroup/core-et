// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_clk_gate_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vprim_clk_gate_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

static void eval_check(SimCtrl<DUT>& sim, bool& en_latch_ref, const std::string& msg) {
    if (!sim.dut->clk_i) {
        en_latch_ref = sim.dut->en_i || sim.dut->scanmode_i;
    }
    bool clk_ref = sim.dut->clk_i && en_latch_ref;

    sim.dut->eval();
    sim.check(sim.dut->clk_o == clk_ref, msg);
    sim.sim_time++;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 10000;

    bool en_latch_ref = false;
    uint32_t rng = 0x13579bdfu;

    sim.dut->clk_i = 0;
    sim.dut->en_i = 0;
    sim.dut->scanmode_i = 0;
    eval_check(sim, en_latch_ref, "idle low keeps gate closed");

    sim.dut->en_i = 1;
    eval_check(sim, en_latch_ref, "low phase captures enable immediately");

    sim.dut->clk_i = 1;
    eval_check(sim, en_latch_ref, "captured enable opens high phase");

    sim.dut->en_i = 0;
    eval_check(sim, en_latch_ref, "high phase ignores late disable");

    sim.dut->scanmode_i = 1;
    eval_check(sim, en_latch_ref, "high phase ignores late scan force");

    sim.dut->clk_i = 0;
    eval_check(sim, en_latch_ref, "low phase closes output while sampling scan force");

    sim.dut->en_i = 0;
    sim.dut->scanmode_i = 0;
    eval_check(sim, en_latch_ref, "low phase updates latch when scan is released");

    sim.dut->clk_i = 1;
    eval_check(sim, en_latch_ref, "released latch keeps next high suppressed");

    for (int i = 0; i < 2048; ++i) {
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

        eval_check(sim, en_latch_ref, "random gate step " + std::to_string(i));
    }

    return sim.finish();
}
