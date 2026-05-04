// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_prim_write_commit_rst_en_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_prim_write_commit_rst_en_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

static void eval_compare(CosimCtrl<DUT>& sim, const std::string& name) {
    sim.dut->eval();
    sim.compare(name, sim.dut->orig_q_o, sim.dut->new_q_o);
    sim.sim_time++;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 16000;

    uint32_t rng = 0x2468ace1u;

    for (int i = 0; i < 1024; ++i) {
        sim.dut->clk_i = (i & 0x1);
        sim.dut->rst_i = (next_rand(rng) >> 2) & 0x1;
        sim.dut->en_i = (next_rand(rng) >> 5) & 0x1;
        sim.dut->d_i = next_rand(rng) & 0xffu;
        eval_compare(sim, "step_" + std::to_string(i));

        sim.dut->d_i = next_rand(rng) & 0xffu;
        eval_compare(sim, "retrack_" + std::to_string(i));
    }

    return sim.finish();
}
