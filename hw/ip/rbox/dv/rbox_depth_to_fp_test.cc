// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_depth_to_fp.
//
// Verifies the internal depth → FP32 conversion with explicit value checks.

#include "Vrbox_depth_to_fp_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <cmath>

using DUT = Vrbox_depth_to_fp_tb;

static void set_depth(DUT* dut, bool inf, bool nan_v, bool zero, bool sub,
                      uint8_t e, bool s, uint32_t m, int8_t e_d, uint8_t m_s) {
    dut->in_infinite = inf;
    dut->in_nan      = nan_v;
    dut->in_zero     = zero;
    dut->in_subnorm  = sub;
    dut->in_e        = e;
    dut->in_s        = s;
    dut->in_m        = m & 0xFFFFFF;
    dut->in_e_delta  = e_d & 0x3F;
    dut->in_m_shift  = m_s & 0x1F;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    set_depth(sim.dut.get(), false, false, true, false, 0, false, 0, 0, 0);
    sim.reset(3);

    // ── Test 1: Zero → 0x00000000 ──────────────────────
    printf("test 1: zero\n");
    set_depth(sim.dut.get(), false, false, true, false, 0, false, 0, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_o == 0x00000000, "zero → 0x00000000");

    // ── Test 2: +inf → 0x7F800000 ──────────────────────
    printf("test 2: +infinity\n");
    set_depth(sim.dut.get(), true, false, false, false, 0xFF, false, 0x800000, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_o == 0x7F800000, "+inf → 0x7F800000");

    // ── Test 3: -inf → 0xFF800000 ──────────────────────
    printf("test 3: -infinity\n");
    set_depth(sim.dut.get(), true, false, false, false, 0xFF, true, 0x800000, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_o == 0xFF800000, "-inf → 0xFF800000");

    // ── Test 4: NaN → sign | 0x7FC00000 ────────────────
    printf("test 4: NaN\n");
    set_depth(sim.dut.get(), false, true, false, false, 0xFF, false, 0xC00000, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_o == 0x7FA00000, "+NaN → 0x7FA00000");

    // ── Test 5: 1.0 (exp=127, m=0x800000) → 0x3F800000 ─
    printf("test 5: 1.0\n");
    set_depth(sim.dut.get(), false, false, false, false, 127, false, 0x800000, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_o == 0x3F800000, "1.0 → 0x3F800000");

    // ── Test 6: -1.0 → 0xBF800000 ──────────────────────
    printf("test 6: -1.0\n");
    set_depth(sim.dut.get(), false, false, false, false, 127, true, 0x800000, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_o == 0xBF800000, "-1.0 → 0xBF800000");

    // ── Test 7: e_delta = +1 shifts exponent up ────────
    printf("test 7: e_delta = +1\n");
    // 1.0 with e_delta=1 → exp=128 → 2.0 = 0x40000000
    set_depth(sim.dut.get(), false, false, false, false, 127, false, 0x800000, 1, 0);
    sim.tick();
    sim.check(sim.dut->out_o == 0x40000000, "1.0 + e_delta=1 → 2.0");

    // ── Test 8: m_shift shifts mantissa left ────────────
    printf("test 8: m_shift\n");
    // m=0x400000 (0.5 in 1.23) with m_shift=1 → m becomes 0x800000 (1.0)
    set_depth(sim.dut.get(), false, false, false, false, 127, false, 0x400000, 0, 1);
    sim.tick();
    // After shift: mant_shifted = 0x800000, mant_final = 0x000000 (implicit 1 stripped)
    sim.check(sim.dut->out_o == 0x3F800000, "m_shift=1 normalizes 0.5 mantissa → 1.0");

    return sim.finish();
}
