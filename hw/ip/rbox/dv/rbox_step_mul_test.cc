// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_step_mul.
//
// Verifies the fixed-point step multiplier with explicit value checks.
// Default parameters: StepIntSz=2, StepFrcSz=4, CoefIntSz=1, CoefFrcSz=40
// StepSz=6, CoefSz=41, ResSz=43

#include "Vrbox_step_mul_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

using DUT = Vrbox_step_mul_tb;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    // Default params: StepIntSz=2, StepFrcSz=4, CoefIntSz=1, CoefFrcSz=40
    // StepSz=6, CoefSz=41, ResSz=43
    // offset = (SX(coef) * step) >> StepFrcSz

    sim.dut->step_i = 0;
    sim.dut->coef_i = 0;
    sim.reset(3);

    // ── Test 1: Zero * Zero = Zero ──────────────────────
    printf("test 1: zero inputs\n");
    sim.dut->step_i = 0;
    sim.dut->coef_i = 0;
    sim.tick();
    sim.check(sim.dut->offset_o == 0, "0 * 0 = 0");

    // ── Test 2: Step = 1.0 (0b01_0000 = 16), Coef = 1 ──
    printf("test 2: step=1.0, coef=1\n");
    sim.dut->step_i = 16;  // 1.0 in 2.4 fixed-point
    sim.dut->coef_i = 1;
    sim.tick();
    // offset = SX(41, 1) * 16 >> 4 = 16 >> 4 = 1
    sim.check(sim.dut->offset_o == 1, "1.0 * 1 = 1");

    // ── Test 3: Step = 0.5 (0b00_1000 = 8), Coef = 2 ───
    printf("test 3: step=0.5, coef=2\n");
    sim.dut->step_i = 8;   // 0.5 in 2.4 fixed-point
    sim.dut->coef_i = 2;
    sim.tick();
    // offset = SX(41, 2) * 8 >> 4 = 16 >> 4 = 1
    sim.check(sim.dut->offset_o == 1, "0.5 * 2 = 1");

    // ── Test 4: Non-zero result ─────────────────────────
    printf("test 4: larger values\n");
    sim.dut->step_i = 16;  // 1.0
    sim.dut->coef_i = 100;
    sim.tick();
    // offset = SX(41, 100) * 16 >> 4 = 1600 >> 4 = 100
    sim.check(sim.dut->offset_o == 100, "1.0 * 100 = 100");

    // ── Test 5: Fractional step ─────────────────────────
    printf("test 5: fractional step 0.25\n");
    sim.dut->step_i = 4;   // 0.25 in 2.4 fixed-point
    sim.dut->coef_i = 100;
    sim.tick();
    // offset = SX(41, 100) * 4 >> 4 = 400 >> 4 = 25
    sim.check(sim.dut->offset_o == 25, "0.25 * 100 = 25");

    return sim.finish();
}
