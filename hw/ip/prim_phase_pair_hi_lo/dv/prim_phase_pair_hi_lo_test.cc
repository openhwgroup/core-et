// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_phase_pair_hi_lo_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vprim_phase_pair_hi_lo_tb;

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

    uint8_t q_hi_ref = 0;
    uint8_t q_lo_ref = 0;
    bool hi_gate_ref = false;
    bool lo_gate_ref = false;
    uint32_t rng = 0x2468ace1u;

    auto model = [&]() {
        if (!sim.dut->clk_i) {
            hi_gate_ref = sim.dut->hi_en_i;
        }
        if (sim.dut->clk_i) {
            lo_gate_ref = sim.dut->lo_en_i;
        }
        if (sim.dut->clk_i && hi_gate_ref) {
            q_hi_ref = sim.dut->d_i;
        }
        if (!sim.dut->clk_i && lo_gate_ref) {
            q_lo_ref = q_hi_ref;
        }
    };

    sim.dut->clk_i = 0;
    sim.dut->hi_en_i = 0;
    sim.dut->lo_en_i = 1;
    sim.dut->d_i = 0x12;
    model();
    eval_step(sim);
    sim.check(sim.dut->q_hi_o == q_hi_ref, "initial low phase holds q_hi");
    sim.check(sim.dut->q_lo_o == q_lo_ref, "initial low phase updates q_lo");

    sim.dut->clk_i = 1;
    sim.dut->hi_en_i = 1;
    sim.dut->d_i = 0x34;
    model();
    eval_step(sim);
    sim.check(sim.dut->q_hi_o == q_hi_ref, "high phase capture updates q_hi");
    sim.check(sim.dut->q_lo_o == q_lo_ref, "high phase holds q_lo");

    sim.dut->d_i = 0x56;
    model();
    eval_step(sim);
    sim.check(sim.dut->q_hi_o == q_hi_ref, "high phase tracks q_hi while enabled");
    sim.check(sim.dut->q_lo_o == q_lo_ref, "high phase still holds q_lo");

    sim.dut->clk_i = 0;
    sim.dut->lo_en_i = 0;
    model();
    eval_step(sim);
    sim.check(sim.dut->q_lo_o == q_lo_ref, "disabled low phase holds q_lo");

    for (int i = 0; i < 512; ++i) {
        sim.dut->clk_i = 1;
        sim.dut->hi_en_i = (next_rand(rng) >> 3) & 0x1;
        sim.dut->lo_en_i = (next_rand(rng) >> 5) & 0x1;
        sim.dut->d_i = next_rand(rng) & 0xffu;
        model();
        eval_step(sim);
        sim.check(sim.dut->q_hi_o == q_hi_ref,
                  "high phase q_hi matches model on iteration " + std::to_string(i));
        sim.check(sim.dut->q_lo_o == q_lo_ref,
                  "high phase q_lo holds on iteration " + std::to_string(i));

        sim.dut->d_i = next_rand(rng) & 0xffu;
        model();
        eval_step(sim);
        sim.check(sim.dut->q_hi_o == q_hi_ref,
                  "high phase retrack matches model on iteration " + std::to_string(i));
        sim.check(sim.dut->q_lo_o == q_lo_ref,
                  "high phase retrack holds q_lo on iteration " + std::to_string(i));

        sim.dut->clk_i = 0;
        sim.dut->hi_en_i = (next_rand(rng) >> 7) & 0x1;
        sim.dut->lo_en_i = (next_rand(rng) >> 11) & 0x1;
        sim.dut->d_i = next_rand(rng) & 0xffu;
        model();
        eval_step(sim);
        sim.check(sim.dut->q_hi_o == q_hi_ref,
                  "low phase q_hi holds on iteration " + std::to_string(i));
        sim.check(sim.dut->q_lo_o == q_lo_ref,
                  "low phase q_lo matches model on iteration " + std::to_string(i));
    }

    return sim.finish();
}
