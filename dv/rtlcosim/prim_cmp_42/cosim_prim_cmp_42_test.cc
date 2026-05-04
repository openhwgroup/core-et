// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: prim_cmp_42 (new) vs r42cmp (original).
//
// Tests 4:2 compressor at Width=1, Width=8, Width=68, Width=71 with
// 2M random vectors per width variant.

#include "Vcosim_prim_cmp_42_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_prim_cmp_42_tb;

// Simple xorshift32 PRNG for reproducible results.
static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static uint64_t xorshift64() {
    uint64_t hi = xorshift32();
    uint64_t lo = xorshift32();
    return (hi << 32) | lo;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;

    sim.dut->rst_ni = 1;

    // ================================================================
    // Width=1: exhaustive (32 combinations) then 2M random
    // ================================================================
    printf("phase 1: Width=1 exhaustive\n");
    for (uint8_t a = 0; a <= 1; a++) {
        for (uint8_t b = 0; b <= 1; b++) {
            for (uint8_t c = 0; c <= 1; c++) {
                for (uint8_t d = 0; d <= 1; d++) {
                    for (uint8_t cin = 0; cin <= 1; cin++) {
                        sim.dut->stim_w1_a   = a;
                        sim.dut->stim_w1_b   = b;
                        sim.dut->stim_w1_c   = c;
                        sim.dut->stim_w1_d   = d;
                        sim.dut->stim_w1_cin = cin;
                        sim.dut->eval();
                        sim.compare("w1_sv", (uint8_t)sim.dut->orig_w1_sv,
                                             (uint8_t)sim.dut->new_w1_sv);
                        sim.compare("w1_cv", (uint8_t)sim.dut->orig_w1_cv,
                                             (uint8_t)sim.dut->new_w1_cv);
                    }
                }
            }
        }
    }

    printf("phase 2: Width=1 random (2M)\n");
    for (int i = 0; i < 2000000; i++) {
        uint32_t r = xorshift32();
        sim.dut->stim_w1_a   = r & 1;
        sim.dut->stim_w1_b   = (r >> 1) & 1;
        sim.dut->stim_w1_c   = (r >> 2) & 1;
        sim.dut->stim_w1_d   = (r >> 3) & 1;
        sim.dut->stim_w1_cin = (r >> 4) & 1;
        sim.dut->eval();
        sim.compare("w1_sv", (uint8_t)sim.dut->orig_w1_sv,
                             (uint8_t)sim.dut->new_w1_sv);
        sim.compare("w1_cv", (uint8_t)sim.dut->orig_w1_cv,
                             (uint8_t)sim.dut->new_w1_cv);
        if (sim.mismatches > 50) break;
    }

    // ================================================================
    // Width=8: 2M random
    // ================================================================
    printf("phase 3: Width=8 random (2M)\n");
    for (int i = 0; i < 2000000; i++) {
        sim.dut->stim_w8_a   = xorshift32() & 0xFF;
        sim.dut->stim_w8_b   = xorshift32() & 0xFF;
        sim.dut->stim_w8_c   = xorshift32() & 0xFF;
        sim.dut->stim_w8_d   = xorshift32() & 0xFF;
        sim.dut->stim_w8_cin = xorshift32() & 1;
        sim.dut->eval();
        sim.compare("w8_sv", (uint8_t)sim.dut->orig_w8_sv,
                             (uint8_t)sim.dut->new_w8_sv);
        sim.compare("w8_cv", (uint8_t)sim.dut->orig_w8_cv,
                             (uint8_t)sim.dut->new_w8_cv);
        if (sim.mismatches > 50) break;
    }

    // ================================================================
    // Width=68: 2M random
    // ================================================================
    printf("phase 4: Width=68 random (2M)\n");
    for (int i = 0; i < 2000000; i++) {
        sim.dut->stim_w68_a_lo = xorshift64();
        sim.dut->stim_w68_a_hi = xorshift32() & 0xF;
        sim.dut->stim_w68_b_lo = xorshift64();
        sim.dut->stim_w68_b_hi = xorshift32() & 0xF;
        sim.dut->stim_w68_c_lo = xorshift64();
        sim.dut->stim_w68_c_hi = xorshift32() & 0xF;
        sim.dut->stim_w68_d_lo = xorshift64();
        sim.dut->stim_w68_d_hi = xorshift32() & 0xF;
        sim.dut->stim_w68_cin  = xorshift32() & 1;
        sim.dut->eval();

        sim.compare("w68_sv_lo", (uint64_t)sim.dut->orig_w68_sv_lo,
                                 (uint64_t)sim.dut->new_w68_sv_lo);
        sim.compare("w68_sv_hi", (uint8_t)sim.dut->orig_w68_sv_hi,
                                 (uint8_t)sim.dut->new_w68_sv_hi);
        sim.compare("w68_cv_lo", (uint64_t)sim.dut->orig_w68_cv_lo,
                                 (uint64_t)sim.dut->new_w68_cv_lo);
        sim.compare("w68_cv_hi", (uint8_t)sim.dut->orig_w68_cv_hi,
                                 (uint8_t)sim.dut->new_w68_cv_hi);
        if (sim.mismatches > 50) break;
    }

    // ================================================================
    // Width=71: 2M random
    // ================================================================
    printf("phase 5: Width=71 random (2M)\n");
    for (int i = 0; i < 2000000; i++) {
        sim.dut->stim_w71_a_lo = xorshift64();
        sim.dut->stim_w71_a_hi = xorshift32() & 0x7F;
        sim.dut->stim_w71_b_lo = xorshift64();
        sim.dut->stim_w71_b_hi = xorshift32() & 0x7F;
        sim.dut->stim_w71_c_lo = xorshift64();
        sim.dut->stim_w71_c_hi = xorshift32() & 0x7F;
        sim.dut->stim_w71_d_lo = xorshift64();
        sim.dut->stim_w71_d_hi = xorshift32() & 0x7F;
        sim.dut->stim_w71_cin  = xorshift32() & 1;
        sim.dut->eval();

        sim.compare("w71_sv_lo", (uint64_t)sim.dut->orig_w71_sv_lo,
                                 (uint64_t)sim.dut->new_w71_sv_lo);
        sim.compare("w71_sv_hi", (uint8_t)sim.dut->orig_w71_sv_hi,
                                 (uint8_t)sim.dut->new_w71_sv_hi);
        sim.compare("w71_cv_lo", (uint64_t)sim.dut->orig_w71_cv_lo,
                                 (uint64_t)sim.dut->new_w71_cv_lo);
        sim.compare("w71_cv_hi", (uint8_t)sim.dut->orig_w71_cv_hi,
                                 (uint8_t)sim.dut->new_w71_cv_hi);
        if (sim.mismatches > 50) break;
    }

    return sim.finish();
}
