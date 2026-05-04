// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_depth_offset vs new rbox_depth_offset.
//
// The module computes depth_offset = depth_coef_a * step_x + depth_coef_b * step_y.
// It has 3 pipeline stages:
//   Stage 0: multiply step by FP coefficient mantissa, normalize (combinational -> registered)
//   Stage 1-2: rbox_fp_adder_2stg adds X and Y offsets (2 registered stages)
//
// Pipeline protocol:
//   Cycle 0: set inputs + valid_stg1=1 (captures multiply result into stage-0 register)
//   Cycle 1: set valid_stg2=1 (captures adder stage-1 register)
//   Cycle 2: output is valid (adder stage-2 completes combinationally)

#include "Vcosim_rbox_depth_offset_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rbox_depth_offset_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    int before = sim.mismatches;
    sim.compare("infinite", sim.dut->orig_infinite, sim.dut->new_infinite);
    sim.compare("nan",      sim.dut->orig_nan,      sim.dut->new_nan);
    sim.compare("zero",     sim.dut->orig_zero,     sim.dut->new_zero);
    sim.compare("subnorm",  sim.dut->orig_subnorm,  sim.dut->new_subnorm);
    sim.compare("e",        sim.dut->orig_e,        sim.dut->new_e);
    sim.compare("s",        sim.dut->orig_s,        sim.dut->new_s);
    sim.compare("m",        sim.dut->orig_m,        sim.dut->new_m);
    sim.compare("e_delta",  sim.dut->orig_e_delta,  sim.dut->new_e_delta);
    sim.compare("m_shift",  sim.dut->orig_m_shift,  sim.dut->new_m_shift);
    if (sim.mismatches > before) {
        printf("  INPUT: coef_a=0x%08x coef_b=0x%08x step_x=%d step_y=%d\n",
               sim.dut->depth_coef_a, sim.dut->depth_coef_b,
               sim.dut->step_x, sim.dut->step_y);
        printf("  ORIG: inf=%d nan=%d zero=%d sub=%d e=0x%02x s=%d m=0x%06x ed=0x%02x ms=0x%02x\n",
               sim.dut->orig_infinite, sim.dut->orig_nan, sim.dut->orig_zero, sim.dut->orig_subnorm,
               sim.dut->orig_e, sim.dut->orig_s, sim.dut->orig_m, sim.dut->orig_e_delta, sim.dut->orig_m_shift);
        printf("  NEW:  inf=%d nan=%d zero=%d sub=%d e=0x%02x s=%d m=0x%06x ed=0x%02x ms=0x%02x\n",
               sim.dut->new_infinite, sim.dut->new_nan, sim.dut->new_zero, sim.dut->new_subnorm,
               sim.dut->new_e, sim.dut->new_s, sim.dut->new_m, sim.dut->new_e_delta, sim.dut->new_m_shift);
    }
}

// Helper: build an IEEE 754 FP32 bit pattern from components.
static uint32_t make_fp32(bool sign, uint8_t exp, uint32_t mant23) {
    return ((uint32_t)sign << 31) | ((uint32_t)exp << 23) | (mant23 & 0x7FFFFF);
}

// Common FP32 constants
static const uint32_t FP32_ZERO      = make_fp32(false, 0,    0);
static const uint32_t FP32_NEG_ZERO  = make_fp32(true,  0,    0);
static const uint32_t FP32_ONE       = make_fp32(false, 127,  0);       // 1.0
static const uint32_t FP32_NEG_ONE   = make_fp32(true,  127,  0);       // -1.0
static const uint32_t FP32_TWO       = make_fp32(false, 128,  0);       // 2.0
static const uint32_t FP32_HALF      = make_fp32(false, 126,  0);       // 0.5
static const uint32_t FP32_NEG_HALF  = make_fp32(true,  126,  0);       // -0.5
static const uint32_t FP32_INF       = make_fp32(false, 255,  0);
static const uint32_t FP32_NEG_INF   = make_fp32(true,  255,  0);
static const uint32_t FP32_NAN       = make_fp32(false, 255,  0x400000);
static const uint32_t FP32_SUBNORM   = make_fp32(false, 0,    1);       // smallest subnormal

static void set_inputs(CosimCtrl<DUT>& sim, uint32_t coef_a, uint32_t coef_b,
                        uint8_t sx, uint8_t sy) {
    sim.dut->depth_coef_a = coef_a;
    sim.dut->depth_coef_b = coef_b;
    sim.dut->step_x       = sx & 0x3F;
    sim.dut->step_y       = sy & 0x3F;
}

// Push one datum through the 3-stage pipeline and compare.
// Protocol: set inputs + valid_stg1 on cycle 0, valid_stg2 on cycle 1, compare on cycle 2.
static void single_shot(CosimCtrl<DUT>& sim, uint32_t coef_a, uint32_t coef_b,
                         uint8_t sx, uint8_t sy) {
    set_inputs(sim, coef_a, coef_b, sx, sy);
    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 0;
    sim.tick();  // cycle 0: multiply stage captured

    sim.dut->valid_stg1 = 0;
    sim.dut->valid_stg2 = 1;
    sim.tick();  // cycle 1: adder stage 1 captured

    sim.dut->valid_stg1 = 0;
    sim.dut->valid_stg2 = 0;
    sim.tick();  // cycle 2: adder stage 2 output valid (combinational from stage-1 FFs)
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    // Initialize
    set_inputs(sim, FP32_ZERO, FP32_ZERO, 0, 0);
    sim.dut->valid_stg1 = 0;
    sim.dut->valid_stg2 = 0;
    sim.reset(10);

    // ── Test 1: Zero steps, zero coefficients ──────────────
    printf("cosim test 1: zero steps, zero coefficients\n");
    single_shot(sim, FP32_ZERO, FP32_ZERO, 0, 0);

    // ── Test 2: Non-zero steps, zero coefficients ──────────
    printf("cosim test 2: non-zero steps, zero coefficients\n");
    single_shot(sim, FP32_ZERO, FP32_ZERO, 1, 1);
    single_shot(sim, FP32_ZERO, FP32_ZERO, 0x3F, 0x3F);

    // ── Test 3: Zero steps, non-zero coefficients ──────────
    printf("cosim test 3: zero steps, non-zero coefficients\n");
    single_shot(sim, FP32_ONE, FP32_TWO, 0, 0);

    // ── Test 4: Unit step_x=1, step_y=0, coef_a=1.0 ───────
    printf("cosim test 4: step_x=1 coef_a=1.0, step_y=0\n");
    single_shot(sim, FP32_ONE, FP32_ZERO, 1, 0);

    // ── Test 5: Unit step_y=1, step_x=0, coef_b=1.0 ───────
    printf("cosim test 5: step_x=0, step_y=1 coef_b=1.0\n");
    single_shot(sim, FP32_ZERO, FP32_ONE, 0, 1);

    // ── Test 6: Both steps=1, both coefs=1.0 ──────────────
    printf("cosim test 6: both steps=1, both coefs=1.0 => 1.0+1.0=2.0\n");
    single_shot(sim, FP32_ONE, FP32_ONE, 1, 1);

    // ── Test 7: step_x=2, coef_a=0.5, step_y=1, coef_b=1.0
    printf("cosim test 7: step_x=2 coef_a=0.5, step_y=1 coef_b=1.0\n");
    single_shot(sim, FP32_HALF, FP32_ONE, 2, 1);

    // ── Test 8: Negative coefficients ──────────────────────
    printf("cosim test 8: negative coefficients\n");
    single_shot(sim, FP32_NEG_ONE, FP32_ONE, 1, 1);     // -1 + 1 = 0
    single_shot(sim, FP32_NEG_HALF, FP32_NEG_ONE, 1, 1); // -0.5 + -1 = -1.5
    single_shot(sim, FP32_NEG_ONE, FP32_NEG_ONE, 3, 2);  // -3 + -2 = -5

    // ── Test 9: Large step values (max 6-bit = 63) ────────
    printf("cosim test 9: large step values\n");
    single_shot(sim, FP32_ONE, FP32_ONE, 0x3F, 0x3F);
    single_shot(sim, FP32_TWO, FP32_HALF, 0x3F, 0x3F);

    // ── Test 10: Mixed step sizes with various coefficients ─
    printf("cosim test 10: mixed step sizes\n");
    single_shot(sim, make_fp32(false, 130, 0x400000), make_fp32(true, 125, 0x200000), 5, 17);
    single_shot(sim, make_fp32(false, 100, 0x123456), make_fp32(false, 100, 0x654321), 0x20, 0x10);

    // ── Test 11: Subnormal coefficients (should be flushed to zero) ─
    printf("cosim test 11: subnormal coefficients\n");
    single_shot(sim, FP32_SUBNORM, FP32_ZERO, 1, 0);
    single_shot(sim, FP32_ZERO, FP32_SUBNORM, 0, 1);
    single_shot(sim, FP32_SUBNORM, FP32_SUBNORM, 1, 1);

    // ── Test 12: Infinity coefficients ─────────────────────
    printf("cosim test 12: infinity coefficients\n");
    single_shot(sim, FP32_INF, FP32_ZERO, 1, 0);
    single_shot(sim, FP32_ZERO, FP32_NEG_INF, 0, 1);
    single_shot(sim, FP32_INF, FP32_INF, 1, 1);

    // ── Test 13: NaN coefficients ──────────────────────────
    printf("cosim test 13: NaN coefficients\n");
    single_shot(sim, FP32_NAN, FP32_ZERO, 1, 0);
    single_shot(sim, FP32_ZERO, FP32_NAN, 0, 1);
    single_shot(sim, FP32_NAN, FP32_NAN, 1, 1);
    single_shot(sim, FP32_NAN, FP32_ONE, 0x3F, 0x3F);

    // ── Test 14: Negative zero coefficient ─────────────────
    printf("cosim test 14: negative zero coefficient\n");
    single_shot(sim, FP32_NEG_ZERO, FP32_ONE, 1, 1);
    single_shot(sim, FP32_NEG_ZERO, FP32_NEG_ZERO, 1, 1);

    // ── Test 15: Streaming pipeline (both valids high) ─────
    printf("cosim test 15: streaming pipeline (continuous)\n");
    // Prime the pipeline: 2-cycle latency before first valid output.
    // Cycle 0: load first datum into multiply stage
    set_inputs(sim, FP32_ONE, FP32_ONE, 1, 1);
    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 1;
    sim.tick();

    // Cycle 1: first datum enters adder stage 1, second datum enters multiply stage
    set_inputs(sim, FP32_TWO, FP32_HALF, 2, 3);
    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 1;
    sim.tick();

    // Cycle 2+: compare output each cycle (output reflects data from 2 cycles ago)
    set_inputs(sim, FP32_NEG_ONE, FP32_ONE, 5, 10);
    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 1;
    sim.tick();
    compare_outputs(sim);  // output from (1.0*1 + 1.0*1)

    set_inputs(sim, FP32_HALF, FP32_HALF, 0x3F, 0x3F);
    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 1;
    sim.tick();
    compare_outputs(sim);  // output from (2.0*2 + 0.5*3)

    set_inputs(sim, FP32_ZERO, FP32_ZERO, 0, 0);
    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 1;
    sim.tick();
    compare_outputs(sim);  // output from (-1.0*5 + 1.0*10)

    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 1;
    sim.tick();
    compare_outputs(sim);  // output from (0.5*63 + 0.5*63)

    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 1;
    sim.tick();
    compare_outputs(sim);  // output from (0+0)

    // ── Test 16: Random stimulus (3000+ pairs) ─────────────
    printf("cosim test 16: random stimulus (3500 single-shot)\n");
    uint32_t seed = 0xDEADBEEF;
    for (int i = 0; i < 3500; i++) {
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
        uint32_t s2 = seed * 2654435761u;

        uint32_t coef_a, coef_b;
        uint8_t sx, sy;

        // Occasionally generate special FP32 values
        switch (seed & 0xF) {
            case 0:  coef_a = FP32_ZERO; break;
            case 1:  coef_a = FP32_INF; break;
            case 2:  coef_a = FP32_NEG_INF; break;
            case 3:  coef_a = FP32_NAN; break;
            case 4:  coef_a = FP32_SUBNORM; break;
            case 5:  coef_a = FP32_NEG_ZERO; break;
            default: coef_a = seed; break;  // random FP32 bit pattern
        }

        switch (s2 & 0xF) {
            case 0:  coef_b = FP32_ZERO; break;
            case 1:  coef_b = FP32_INF; break;
            case 2:  coef_b = FP32_NEG_INF; break;
            case 3:  coef_b = FP32_NAN; break;
            case 4:  coef_b = FP32_SUBNORM; break;
            case 5:  coef_b = FP32_NEG_ZERO; break;
            default: coef_b = s2; break;    // random FP32 bit pattern
        }

        sx = (seed >> 8) & 0x3F;
        sy = (s2 >> 8) & 0x3F;

        single_shot(sim, coef_a, coef_b, sx, sy);
    }

    // ── Test 16b: Targeted inputs from streaming failures ──
    printf("cosim test 16b: targeted failing inputs (single-shot)\n");
    single_shot(sim, 0x3a817770, 0x6e078470, 55, 4);
    single_shot(sim, 0x39034c7a, 0xb4a38a5a, 12, 10);
    single_shot(sim, 0xbc7fb30d, 0x2db6f0fd, 51, 48);
    single_shot(sim, 0x82700c8c, 0x3f7ad8cc, 12, 24);
    single_shot(sim, 0xeebd270b, 0xa599319b, 39, 49);

    // ── Test 17: Random streaming (1000 cycles) ────────────
    printf("cosim test 17: random streaming (1000 cycles)\n");
    seed = 0xCAFEBABE;
    // Prime the pipeline
    seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
    uint32_t s2 = seed * 2654435761u;
    set_inputs(sim, seed, s2, (seed >> 8) & 0x3F, (s2 >> 8) & 0x3F);
    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 1;
    sim.tick();

    seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
    s2 = seed * 2654435761u;
    set_inputs(sim, seed, s2, (seed >> 8) & 0x3F, (s2 >> 8) & 0x3F);
    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 1;
    sim.tick();

    // Now compare every cycle — track 2-cycle-ago inputs for debug
    uint32_t hist_a[3] = {}, hist_b[3] = {};
    uint8_t  hist_sx[3] = {}, hist_sy[3] = {};
    for (int i = 0; i < 1000; i++) {
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
        s2 = seed * 2654435761u;
        // Shift history
        hist_a[2] = hist_a[1]; hist_a[1] = hist_a[0]; hist_a[0] = seed;
        hist_b[2] = hist_b[1]; hist_b[1] = hist_b[0]; hist_b[0] = s2;
        hist_sx[2] = hist_sx[1]; hist_sx[1] = hist_sx[0]; hist_sx[0] = (seed >> 8) & 0x3F;
        hist_sy[2] = hist_sy[1]; hist_sy[1] = hist_sy[0]; hist_sy[0] = (s2 >> 8) & 0x3F;
        set_inputs(sim, seed, s2, (seed >> 8) & 0x3F, (s2 >> 8) & 0x3F);
        sim.dut->valid_stg1 = 1;
        sim.dut->valid_stg2 = 1;
        int before = sim.mismatches;
        sim.tick();
        compare_outputs(sim);
        if (sim.mismatches > before) {
            printf("  INPUT(T-2): coef_a=0x%08x coef_b=0x%08x step_x=%d step_y=%d\n",
                   hist_a[2], hist_b[2], hist_sx[2], hist_sy[2]);
        }
    }

    // Flush pipeline
    set_inputs(sim, FP32_ZERO, FP32_ZERO, 0, 0);
    sim.dut->valid_stg1 = 1;
    sim.dut->valid_stg2 = 1;
    sim.tick();
    compare_outputs(sim);
    sim.tick();
    compare_outputs(sim);

    return sim.finish();
}
