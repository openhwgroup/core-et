// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_depth_to_un vs new rbox_depth_to_un.
//
// Tests the FP32-to-UNORM converter used for depth buffer writes.
// Both modules are combinational, so we compare outputs every cycle.

#include "Vcosim_rbox_depth_to_un_tb.h"
#include "cosim_ctrl.h"

#include <cmath>
#include <cstring>

using DUT = Vcosim_rbox_depth_to_un_tb;

// Helper: reinterpret float bits as uint32_t
static uint32_t f2u(float f) {
    uint32_t u;
    std::memcpy(&u, &f, sizeof(u));
    return u;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("out", sim.dut->orig_out_o, sim.dut->new_out_o);
}

static void set_and_compare(CosimCtrl<DUT>& sim, uint32_t in_val) {
    sim.dut->in_i = in_val;
    sim.tick();
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->in_i = 0;
    sim.reset(5);
    compare_outputs(sim);

    // ── Test 1: Special values ──────────────────────────
    printf("cosim test 1: special values\n");
    set_and_compare(sim, 0x00000000);  // +0
    set_and_compare(sim, 0x80000000);  // -0
    set_and_compare(sim, 0x7F800000);  // +inf
    set_and_compare(sim, 0xFF800000);  // -inf
    set_and_compare(sim, 0x7FC00000);  // +NaN (quiet)
    set_and_compare(sim, 0xFFC00000);  // -NaN (quiet)
    set_and_compare(sim, 0x7F800001);  // +NaN (signaling)
    set_and_compare(sim, 0xFF800001);  // -NaN (signaling)
    set_and_compare(sim, 0x7FFFFFFF);  // +NaN (all mantissa bits set)

    // ── Test 2: Denorms ─────────────────────────────────
    printf("cosim test 2: denormals\n");
    set_and_compare(sim, 0x00000001);  // smallest positive denorm
    set_and_compare(sim, 0x00400000);  // mid denorm
    set_and_compare(sim, 0x007FFFFF);  // largest positive denorm
    set_and_compare(sim, 0x80000001);  // smallest negative denorm
    set_and_compare(sim, 0x807FFFFF);  // largest negative denorm

    // ── Test 3: Clamping (>= 1.0 → all 1s) ─────────────
    printf("cosim test 3: clamping >= 1.0\n");
    set_and_compare(sim, f2u(1.0f));    // exactly 1.0 → all 1s
    set_and_compare(sim, f2u(1.5f));    // 1.5 → all 1s
    set_and_compare(sim, f2u(2.0f));    // 2.0 → all 1s
    set_and_compare(sim, f2u(100.0f));  // 100.0 → all 1s
    set_and_compare(sim, 0x7F7FFFFF);   // FLT_MAX → all 1s

    // ── Test 4: Clamping (negative → 0) ─────────────────
    printf("cosim test 4: clamping negatives\n");
    set_and_compare(sim, f2u(-0.5f));
    set_and_compare(sim, f2u(-1.0f));
    set_and_compare(sim, f2u(-100.0f));
    set_and_compare(sim, 0xFF7FFFFF);   // -FLT_MAX

    // ── Test 5: Normal conversions ──────────────────────
    printf("cosim test 5: normal conversions\n");
    set_and_compare(sim, f2u(0.5f));
    set_and_compare(sim, f2u(0.25f));
    set_and_compare(sim, f2u(0.75f));
    set_and_compare(sim, f2u(0.125f));
    set_and_compare(sim, f2u(0.0625f));
    set_and_compare(sim, f2u(1.0f / 3.0f));
    set_and_compare(sim, f2u(2.0f / 3.0f));
    set_and_compare(sim, f2u(1.0f / 7.0f));
    set_and_compare(sim, f2u(1.0f / 255.0f));
    set_and_compare(sim, f2u(1.0f / 256.0f));
    set_and_compare(sim, f2u(0.999999f));

    // ── Test 6: Boundary near 1.0 ──────────────────────
    printf("cosim test 6: values near 1.0\n");
    // Just below 1.0: 0x3F7FFFFF = 0.999999940395...
    set_and_compare(sim, 0x3F7FFFFF);
    // 0x3F7FFFFE
    set_and_compare(sim, 0x3F7FFFFE);
    // 0x3F7FFFFD
    set_and_compare(sim, 0x3F7FFFFD);
    // Walk down from just below 1.0
    for (uint32_t i = 0; i < 32; i++) {
        set_and_compare(sim, 0x3F7FFFFF - i);
    }

    // ── Test 7: Boundary near 0.0 ──────────────────────
    printf("cosim test 7: values near 0.0\n");
    // Smallest positive normal: 0x00800000 = 2^-126
    set_and_compare(sim, 0x00800000);
    // Walk up from smallest normal
    for (uint32_t i = 0; i < 32; i++) {
        set_and_compare(sim, 0x00800000 + i);
    }

    // ── Test 8: Round-to-nearest-even edge cases ────────
    printf("cosim test 8: round-to-nearest-even edges\n");
    // Powers of two (exact in FP32): 0.5, 0.25, 0.125, etc.
    for (int e = 126; e >= 104; e--) {
        uint32_t val = (uint32_t)e << 23;
        set_and_compare(sim, val);
    }
    // Walk through mantissa bits at exponent 126 (0.5 range)
    for (uint32_t m = 0; m < 256; m++) {
        uint32_t val = (126u << 23) | (m << 15);
        set_and_compare(sim, val);
    }
    // Walk through mantissa bits at exponent 125 (0.25 range)
    for (uint32_t m = 0; m < 256; m++) {
        uint32_t val = (125u << 23) | (m << 15);
        set_and_compare(sim, val);
    }
    // Walk through mantissa bits at exponent 124 (0.125 range)
    for (uint32_t m = 0; m < 256; m++) {
        uint32_t val = (124u << 23) | (m << 15);
        set_and_compare(sim, val);
    }

    // ── Test 9: All exponents in valid range ────────────
    printf("cosim test 9: sweep exponents in [0, 1) range\n");
    // Exponents 1..126 all produce values in (0, 1)
    for (int e = 1; e <= 126; e++) {
        set_and_compare(sim, (uint32_t)e << 23);               // mantissa = 0 (1.0 * 2^(e-127))
        set_and_compare(sim, ((uint32_t)e << 23) | 0x007FFFFF); // max mantissa
        set_and_compare(sim, ((uint32_t)e << 23) | 0x00400000); // mid mantissa
    }

    // ── Test 10: Random FP32 in [0, 1] range (3000+) ───
    printf("cosim test 10: random FP32 in [0,1] (3000 cycles)\n");
    uint32_t seed = 0xCAFEBABE;
    for (int i = 0; i < 3000; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        // Generate exponent in range [1, 126] for values in (0, 1)
        uint8_t exp = (seed & 0x7F);
        if (exp == 0) exp = 1;
        if (exp > 126) exp = 126;
        uint32_t mant = (seed >> 7) & 0x007FFFFF;
        uint32_t val = ((uint32_t)exp << 23) | mant;
        set_and_compare(sim, val);
    }

    // ── Test 11: Random unclamped FP32 (1000+) ──────────
    printf("cosim test 11: random unclamped FP32 (1000 cycles)\n");
    for (int i = 0; i < 1000; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        set_and_compare(sim, seed);
    }

    // ── Test 12: Specific bit patterns ──────────────────
    printf("cosim test 12: specific bit patterns\n");
    set_and_compare(sim, 0x3F000000);  // 0.5
    set_and_compare(sim, 0x3E800000);  // 0.25
    set_and_compare(sim, 0x3F400000);  // 0.75
    set_and_compare(sim, 0x3EAAAAAB);  // 1/3
    set_and_compare(sim, 0x3F2AAAAB);  // 2/3
    set_and_compare(sim, 0x3DCCCCCD);  // 0.1
    set_and_compare(sim, 0x3E4CCCCD);  // 0.2
    set_and_compare(sim, 0x3E99999A);  // 0.3
    set_and_compare(sim, 0x3ECCCCCD);  // 0.4
    set_and_compare(sim, 0x3F19999A);  // 0.6
    set_and_compare(sim, 0x3F333333);  // 0.7
    set_and_compare(sim, 0x3F4CCCCD);  // 0.8
    set_and_compare(sim, 0x3F666666);  // 0.9

    return sim.finish();
}
