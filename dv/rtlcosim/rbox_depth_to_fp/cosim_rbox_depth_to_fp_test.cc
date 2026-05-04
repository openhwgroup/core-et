// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_depth_to_fp vs new rbox_depth_to_fp.

#include "Vcosim_rbox_depth_to_fp_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rbox_depth_to_fp_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("out", sim.dut->orig_out_o, sim.dut->new_out_o);
}

static void set_depth(CosimCtrl<DUT>& sim,
                      bool infinite, bool nan, bool zero, bool subnorm,
                      uint8_t e, bool s, uint32_t m,
                      int8_t e_delta, uint8_t m_shift) {
    sim.dut->in_infinite = infinite;
    sim.dut->in_nan      = nan;
    sim.dut->in_zero     = zero;
    sim.dut->in_subnorm  = subnorm;
    sim.dut->in_e        = e;
    sim.dut->in_s        = s;
    sim.dut->in_m        = m & 0xFFFFFF;
    sim.dut->in_e_delta  = e_delta & 0x3F;
    sim.dut->in_m_shift  = m_shift & 0x1F;
    sim.tick();
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->in_infinite = 0;
    sim.dut->in_nan = 0;
    sim.dut->in_zero = 0;
    sim.dut->in_subnorm = 0;
    sim.dut->in_e = 0;
    sim.dut->in_s = 0;
    sim.dut->in_m = 0;
    sim.dut->in_e_delta = 0;
    sim.dut->in_m_shift = 0;
    sim.reset(5);
    compare_outputs(sim);

    // ── Test 1: Special values ──────────────────────────
    printf("cosim test 1: special values\n");
    set_depth(sim, false, false, true,  false, 0,    false, 0,        0, 0);  // +zero
    set_depth(sim, false, false, true,  false, 0,    true,  0,        0, 0);  // -zero → +0
    set_depth(sim, true,  false, false, false, 0xFF, false, 0x800000, 0, 0);  // +inf
    set_depth(sim, true,  false, false, false, 0xFF, true,  0x800000, 0, 0);  // -inf
    set_depth(sim, false, true,  false, false, 0xFF, false, 0xC00000, 0, 0);  // +NaN
    set_depth(sim, false, true,  false, false, 0xFF, true,  0xC00000, 0, 0);  // -NaN

    // ── Test 2: Normal values with no renormalization ───
    printf("cosim test 2: normal values, no renorm\n");
    // 1.0 = exp=127, mantissa=1.0 (m=0x800000)
    set_depth(sim, false, false, false, false, 127, false, 0x800000, 0, 0);
    // -1.0
    set_depth(sim, false, false, false, false, 127, true,  0x800000, 0, 0);
    // 0.5 = exp=126
    set_depth(sim, false, false, false, false, 126, false, 0x800000, 0, 0);
    // Large exponent
    set_depth(sim, false, false, false, false, 254, false, 0xFFFFFF, 0, 0);

    // ── Test 3: Values with e_delta ─────────────────────
    printf("cosim test 3: values with e_delta\n");
    set_depth(sim, false, false, false, false, 127, false, 0x800000,  1, 0);  // e_delta=+1
    set_depth(sim, false, false, false, false, 127, false, 0x800000, -1, 0);  // e_delta=-1
    set_depth(sim, false, false, false, false, 100, false, 0xABCDEF, -5, 0);

    // ── Test 4: Values with m_shift ─────────────────────
    printf("cosim test 4: values with m_shift\n");
    for (uint8_t shift = 0; shift < 24; shift++) {
        set_depth(sim, false, false, false, false, 127, false, 0xFFFFFF, 0, shift);
    }

    // ── Test 5: Combined e_delta and m_shift ────────────
    printf("cosim test 5: combined e_delta + m_shift\n");
    for (int d = -5; d <= 1; d++) {
        for (uint8_t s = 0; s < 10; s++) {
            set_depth(sim, false, false, false, false, 127, false, 0xC5A3B7, d, s);
        }
    }

    // ── Test 6: Random stimulus (3000 cycles) ───────────
    printf("cosim test 6: random stimulus (3000 cycles)\n");
    uint32_t seed = 0x12345678;
    for (int i = 0; i < 3000; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        bool inf   = (seed & 0x100) != 0;
        bool nan_v = !inf && ((seed & 0x200) != 0);
        bool zero  = !inf && !nan_v && ((seed & 0x400) != 0);
        uint8_t e     = (seed >> 11) & 0xFF;
        bool s        = (seed >> 19) & 1;
        uint32_t m    = (seed >> 0) & 0xFFFFFF;
        int8_t e_d    = ((seed >> 24) & 0x3F);
        if (e_d > 31) e_d -= 64;  // sign extend 6 bits
        uint8_t m_s   = (seed >> 20) & 0x1F;

        if (zero) { e = 0; m = 0; }
        if (inf)  { e = 0xFF; m = 0x800000; }
        if (nan_v){ e = 0xFF; m = 0xC00000; }

        set_depth(sim, inf, nan_v, zero, false, e, s, m, e_d, m_s);
    }

    return sim.finish();
}
