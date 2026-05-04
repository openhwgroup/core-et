// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_step_mul vs new rbox_step_mul.
//
// Tests the parameterized fixed-point step multiplier used by rbox_sampler
// for edge function sampling. Both modules are combinational, so we compare
// outputs on every cycle after setting inputs.

#include "Vcosim_rbox_step_mul_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rbox_step_mul_tb;

// Verilator represents 33-64 bit signals as uint64_t (QData).
// StepSz=6 → uint8_t, CoefSz=34 → uint64_t, ResSz=36 → uint64_t

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("offset", sim.dut->orig_offset_o, sim.dut->new_offset_o);
}

static void set_and_compare(CosimCtrl<DUT>& sim, uint8_t step, uint64_t coef) {
    sim.dut->step_i = step;
    sim.dut->coef_i = coef;
    sim.tick();
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    // StepSz = 6, CoefSz = 34, ResSz = 36
    sim.dut->step_i = 0;
    sim.dut->coef_i = 0;
    sim.reset(5);
    compare_outputs(sim);

    // ── Test 1: Zero inputs ─────────────────────────────
    printf("cosim test 1: zero inputs\n");
    set_and_compare(sim, 0, 0);

    // ── Test 2: Step = 1.0 (integer 1, frac 0) ─────────
    printf("cosim test 2: step = 1.0, various coefficients\n");
    // step = 1.0 = 0b01_0000 = 0x10 (int=1, frac=0)
    set_and_compare(sim, 0x10, 1);
    set_and_compare(sim, 0x10, 0x1FFFFFFFFULL);   // max positive 34-bit
    set_and_compare(sim, 0x10, 0x200000000ULL);    // min negative (MSB set)
    set_and_compare(sim, 0x10, 0x3FFFFFFFFULL);    // -1 in 34-bit 2's complement

    // ── Test 3: Coefficient = 1.0 (int=1, frac=0) ──────
    printf("cosim test 3: coef = 1.0, various steps\n");
    // coef = 1.0 in 9.25 format = 0x02000000 (bit 25 set)
    uint64_t coef_one = 1ULL << 25;
    for (int s = 0; s < 64; s++) {  // all 6-bit step values
        set_and_compare(sim, s, coef_one);
    }

    // ── Test 4: Negative coefficients ───────────────────
    printf("cosim test 4: negative coefficients\n");
    // coef = -1.0 in 9.25 format (sign extend)
    uint64_t coef_neg_one = 0x3FE000000ULL;  // -1.0 in 34-bit signed 9.25
    for (int s = 0; s < 64; s++) {
        set_and_compare(sim, s, coef_neg_one);
    }

    // ── Test 5: Random stimulus (2000 cycles) ───────────
    printf("cosim test 5: random stimulus (2000 cycles)\n");
    uint32_t seed = 0xDEADBEEF;
    for (int i = 0; i < 2000; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        uint8_t step = seed & 0x3F;  // 6 bits
        uint64_t coef = ((uint64_t)(seed) << 2) & 0x3FFFFFFFFULL;  // 34 bits

        set_and_compare(sim, step, coef);
    }

    // ── Test 6: Edge cases ──────────────────────────────
    printf("cosim test 6: edge cases\n");
    set_and_compare(sim, 0x3F, 0x3FFFFFFFFULL);  // max step * max coef (negative)
    set_and_compare(sim, 0x3F, 0x1FFFFFFFFULL);  // max step * max positive coef
    set_and_compare(sim, 1, 0x200000000ULL);       // min step * min negative coef

    return sim.finish();
}
