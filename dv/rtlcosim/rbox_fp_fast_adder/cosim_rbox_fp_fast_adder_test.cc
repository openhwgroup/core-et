// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_fp_fast_adder vs new rbox_fp_fast_adder.
//
// This module is purely combinational — no pipeline latency. We set inputs,
// call eval() via tick(), and compare outputs on the same cycle.

#include "Vcosim_rbox_fp_fast_adder_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rbox_fp_fast_adder_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("infinite", sim.dut->orig_infinite, sim.dut->new_infinite);
    sim.compare("nan",      sim.dut->orig_nan,      sim.dut->new_nan);
    sim.compare("zero",     sim.dut->orig_zero,     sim.dut->new_zero);
    sim.compare("subnorm",  sim.dut->orig_subnorm,  sim.dut->new_subnorm);
    sim.compare("e",        sim.dut->orig_e,        sim.dut->new_e);
    sim.compare("s",        sim.dut->orig_s,        sim.dut->new_s);
    sim.compare("m",        sim.dut->orig_m,        sim.dut->new_m);
    sim.compare("e_delta",  sim.dut->orig_e_delta,  sim.dut->new_e_delta);
    sim.compare("m_shift",  sim.dut->orig_m_shift,  sim.dut->new_m_shift);
}

// Set inputs, tick (for clock edge), then compare immediately.
static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

struct DepthVal {
    bool infinite, nan, zero, subnorm, s;
    uint8_t e;
    uint32_t m;
    int8_t e_delta;
    uint8_t m_shift;
};

static void set_operands(CosimCtrl<DUT>& sim, const DepthVal& a, const DepthVal& b) {
    sim.dut->a_infinite = a.infinite;
    sim.dut->a_nan      = a.nan;
    sim.dut->a_zero     = a.zero;
    sim.dut->a_subnorm  = a.subnorm;
    sim.dut->a_e        = a.e;
    sim.dut->a_s        = a.s;
    sim.dut->a_m        = a.m & 0xFFFFFF;
    sim.dut->a_e_delta  = a.e_delta & 0x3F;
    sim.dut->a_m_shift  = a.m_shift & 0x1F;

    sim.dut->b_infinite = b.infinite;
    sim.dut->b_nan      = b.nan;
    sim.dut->b_zero     = b.zero;
    sim.dut->b_subnorm  = b.subnorm;
    sim.dut->b_e        = b.e;
    sim.dut->b_s        = b.s;
    sim.dut->b_m        = b.m & 0xFFFFFF;
    sim.dut->b_e_delta  = b.e_delta & 0x3F;
    sim.dut->b_m_shift  = b.m_shift & 0x1F;
}

static DepthVal make_normal(uint8_t e, bool s, uint32_t m, int8_t e_d = 0, uint8_t m_s = 0) {
    return {false, false, false, false, s, e, m, e_d, m_s};
}
static DepthVal make_zero()     { return {false, false, true,  false, false, 0,    0,        0, 0}; }
static DepthVal make_inf(bool s){ return {true,  false, false, false, s,     0xFF, 0x800000, 0, 0}; }
static DepthVal make_nan()      { return {false, true,  false, false, false, 0xFF, 0xC00000, 0, 0}; }

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    // Initialize
    set_operands(sim, make_zero(), make_zero());
    sim.reset(5);

    // Combinational: set inputs, tick, compare on same cycle.

    // ── Test 1: Zero + Zero ─────────────────────────────
    printf("cosim test 1: zero + zero\n");
    set_operands(sim, make_zero(), make_zero());
    tick_and_compare(sim);

    // ── Test 2: Normal + Zero ───────────────────────────
    printf("cosim test 2: normal + zero\n");
    set_operands(sim, make_normal(127, false, 0x800000), make_zero());
    tick_and_compare(sim);

    // ── Test 3: Zero + Normal ───────────────────────────
    printf("cosim test 3: zero + normal\n");
    set_operands(sim, make_zero(), make_normal(127, false, 0xC00000));
    tick_and_compare(sim);

    // ── Test 4: Same sign addition ──────────────────────
    printf("cosim test 4: same sign addition\n");
    // 1.0 + 1.0 = 2.0
    set_operands(sim, make_normal(127, false, 0x800000), make_normal(127, false, 0x800000));
    tick_and_compare(sim);

    // 0.5 + 0.25
    set_operands(sim, make_normal(126, false, 0x800000), make_normal(125, false, 0x800000));
    tick_and_compare(sim);

    // Large + small same sign
    set_operands(sim, make_normal(200, false, 0xFFFFFF), make_normal(180, false, 0x800000));
    tick_and_compare(sim);

    // ── Test 5: Subtraction (different signs) ───────────
    printf("cosim test 5: subtraction\n");
    // 1.0 + (-0.5) = 0.5
    set_operands(sim, make_normal(127, false, 0x800000), make_normal(126, true, 0x800000));
    tick_and_compare(sim);

    // 1.0 + (-1.0) = 0.0
    set_operands(sim, make_normal(127, false, 0x800000), make_normal(127, true, 0x800000));
    tick_and_compare(sim);

    // (-2.0) + 1.0 = -1.0
    set_operands(sim, make_normal(128, true, 0x800000), make_normal(127, false, 0x800000));
    tick_and_compare(sim);

    // Nearly equal subtraction (cancellation)
    set_operands(sim, make_normal(127, false, 0x800001), make_normal(127, true, 0x800000));
    tick_and_compare(sim);

    // ── Test 6: Infinity cases ──────────────────────────
    printf("cosim test 6: infinity\n");
    // +inf + normal
    set_operands(sim, make_inf(false), make_normal(127, false, 0x800000));
    tick_and_compare(sim);

    // normal + (-inf)
    set_operands(sim, make_normal(127, false, 0x800000), make_inf(true));
    tick_and_compare(sim);

    // +inf + +inf
    set_operands(sim, make_inf(false), make_inf(false));
    tick_and_compare(sim);

    // +inf + zero
    set_operands(sim, make_inf(false), make_zero());
    tick_and_compare(sim);

    // ── Test 7: NaN cases ───────────────────────────────
    printf("cosim test 7: NaN\n");
    // NaN + normal
    set_operands(sim, make_nan(), make_normal(127, false, 0x800000));
    tick_and_compare(sim);

    // normal + NaN
    set_operands(sim, make_normal(127, false, 0x800000), make_nan());
    tick_and_compare(sim);

    // NaN + NaN
    set_operands(sim, make_nan(), make_nan());
    tick_and_compare(sim);

    // NaN + zero
    set_operands(sim, make_nan(), make_zero());
    tick_and_compare(sim);

    // NaN + inf
    set_operands(sim, make_nan(), make_inf(false));
    tick_and_compare(sim);

    // ── Test 8: Values with renormalization ──────────────
    printf("cosim test 8: renormalization (e_delta, m_shift)\n");
    set_operands(sim, make_normal(127, false, 0xABCDEF, -3, 5),
                      make_normal(126, false, 0x800000, 1, 2));
    tick_and_compare(sim);

    set_operands(sim, make_normal(100, true, 0xF01234, -2, 3),
                      make_normal(100, false, 0xE56789, -1, 1));
    tick_and_compare(sim);

    // Large m_shift values
    set_operands(sim, make_normal(127, false, 0x800000, 0, 15),
                      make_normal(127, false, 0x800000, 0, 10));
    tick_and_compare(sim);

    // Negative e_delta
    set_operands(sim, make_normal(50, false, 0x900000, -5, 8),
                      make_normal(50, true, 0xA00000, -3, 4));
    tick_and_compare(sim);

    // Positive e_delta
    set_operands(sim, make_normal(200, false, 0xC00000, 1, 0),
                      make_normal(199, false, 0xF00000, 1, 0));
    tick_and_compare(sim);

    // ── Test 9: Random stimulus (5000 pairs) ────────────
    printf("cosim test 9: random stimulus (5000 pairs)\n");
    uint32_t seed = 0xCAFEBABE;
    for (int i = 0; i < 5000; i++) {
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
        uint32_t s2 = seed * 2654435761u;

        DepthVal a, b;

        // Occasionally generate special values
        if ((seed & 0xF) == 0) {
            a = make_zero();
        } else if ((seed & 0xF) == 1) {
            a = make_inf(seed & 0x10);
        } else if ((seed & 0xF) == 2) {
            a = make_nan();
        } else {
            a = make_normal((seed >> 4) & 0xFF, seed & 0x10,
                           0x800000 | (seed & 0x7FFFFF),
                           (int8_t)((seed >> 12) & 0x7) - 3,
                           (seed >> 15) & 0xF);
        }

        if ((s2 & 0xF) == 0) {
            b = make_zero();
        } else if ((s2 & 0xF) == 1) {
            b = make_inf(s2 & 0x10);
        } else if ((s2 & 0xF) == 2) {
            b = make_nan();
        } else {
            b = make_normal((s2 >> 4) & 0xFF, s2 & 0x10,
                           0x800000 | (s2 & 0x7FFFFF),
                           (int8_t)((s2 >> 12) & 0x7) - 3,
                           (s2 >> 15) & 0xF);
        }

        set_operands(sim, a, b);
        tick_and_compare(sim);
    }

    return sim.finish();
}
