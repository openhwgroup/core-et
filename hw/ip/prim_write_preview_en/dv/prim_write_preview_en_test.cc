// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_write_preview_en_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vprim_write_preview_en_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

static void eval_step(SimCtrl<DUT>& sim) {
    sim.dut->eval();
    sim.sim_time++;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 12000;

    uint8_t q_ref = 0;
    bool en_2p_ref = false;
    uint32_t rng = 0x1234abcdu;

    auto model = [&]() {
        if (sim.dut->clk_i) {
            en_2p_ref = sim.dut->en_i;
        }
        if (!sim.dut->clk_i && en_2p_ref) {
            q_ref = sim.dut->d_i;
        }
    };

    sim.dut->clk_i = 1;
    sim.dut->en_i = 0;
    sim.dut->d_i = 0x12;
    model();
    eval_step(sim);
    sim.check(sim.dut->q_o == q_ref, "high phase holds preview output");

    sim.dut->clk_i = 0;
    sim.dut->en_i = 1;
    sim.dut->d_i = 0x34;
    model();
    eval_step(sim);
    sim.check(sim.dut->q_o == q_ref, "low phase captures preview data when enabled");

    sim.dut->d_i = 0x56;
    model();
    eval_step(sim);
    sim.check(sim.dut->q_o == q_ref, "low phase tracks new preview data while open");

    for (int i = 0; i < 512; ++i) {
        sim.dut->clk_i = 1;
        sim.dut->en_i = (next_rand(rng) >> 1) & 0x1;
        sim.dut->d_i = next_rand(rng) & 0xffu;
        model();
        eval_step(sim);
        sim.check(sim.dut->q_o == q_ref,
                  "high phase output matches model on iteration " + std::to_string(i));

        sim.dut->clk_i = 0;
        sim.dut->en_i = (next_rand(rng) >> 5) & 0x1;
        sim.dut->d_i = next_rand(rng) & 0xffu;
        model();
        eval_step(sim);
        sim.check(sim.dut->q_o == q_ref,
                  "low phase capture matches model on iteration " + std::to_string(i));

        sim.dut->d_i = next_rand(rng) & 0xffu;
        model();
        eval_step(sim);
        sim.check(sim.dut->q_o == q_ref,
                  "low phase retrack matches model on iteration " + std::to_string(i));
    }

    return sim.finish();
}
