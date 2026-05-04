// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_sampler vs new rbox_sampler.
//
// The sampler has a 1-cycle pipeline: stage 1 computes quad/row offsets
// via four rbox_step_mul instances and registers them when valid is high.
// Stage 2 selects quad or row offset based on sample_row and adds it to
// in_sample. We drive both modules with identical stimulus and compare
// out_sample each cycle.

#include "Vcosim_rbox_sampler_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rbox_sampler_tb;

// Types:
//   edge_coef_t     = 34 bits → uint64_t
//   step_t          = 6 bits  → uint8_t
//   edge_sample_t   = 43 bits → uint64_t

static constexpr uint64_t COEF_MASK   = (1ULL << 34) - 1;  // 0x3FFFFFFFF
static constexpr uint8_t  STEP_MASK   = 0x3F;               // 6 bits
static constexpr uint64_t SAMPLE_MASK = (1ULL << 43) - 1;   // 0x7FFFFFFFFFF

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("out_sample", sim.dut->orig_out_sample_o, sim.dut->new_out_sample_o);
}

// Load stage 1 inputs and tick with valid=1 to register offsets.
static void load_stage1(CosimCtrl<DUT>& sim,
                        uint64_t coef_x, uint64_t coef_y,
                        uint8_t qsx, uint8_t qsy,
                        uint8_t rsx, uint8_t rsy) {
    sim.dut->coef_x_i     = coef_x & COEF_MASK;
    sim.dut->coef_y_i     = coef_y & COEF_MASK;
    sim.dut->quad_step_x_i = qsx & STEP_MASK;
    sim.dut->quad_step_y_i = qsy & STEP_MASK;
    sim.dut->row_step_x_i  = rsx & STEP_MASK;
    sim.dut->row_step_y_i  = rsy & STEP_MASK;
    sim.dut->valid_i       = 1;
    sim.tick();
    sim.dut->valid_i       = 0;
}

// Set stage 2 inputs and compare output.
static void sample_and_compare(CosimCtrl<DUT>& sim,
                               uint64_t in_sample, bool sample_row) {
    sim.dut->in_sample_i  = in_sample & SAMPLE_MASK;
    sim.dut->sample_row_i = sample_row ? 1 : 0;
    sim.tick();
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    // Initialize all inputs
    sim.dut->coef_x_i      = 0;
    sim.dut->coef_y_i      = 0;
    sim.dut->quad_step_x_i = 0;
    sim.dut->quad_step_y_i = 0;
    sim.dut->row_step_x_i  = 0;
    sim.dut->row_step_y_i  = 0;
    sim.dut->valid_i        = 0;
    sim.dut->in_sample_i   = 0;
    sim.dut->sample_row_i  = 0;
    sim.reset(5);

    // ── Test 1: Zero coefficients, zero sample ──────────
    printf("cosim test 1: zero coefficients, zero sample\n");
    load_stage1(sim, 0, 0, 0, 0, 0, 0);
    sample_and_compare(sim, 0, false);
    sample_and_compare(sim, 0, true);

    // ── Test 2: Unit step (1.0), positive coefficient ───
    printf("cosim test 2: unit step, positive coefficient\n");
    // step = 1.0 = 0b01_0000 = 0x10 (2-bit int, 4-bit frac)
    // coef = 1.0 in 9.25 format = 1 << 25 = 0x02000000
    uint64_t coef_one = 1ULL << 25;
    load_stage1(sim, coef_one, coef_one, 0x10, 0x10, 0x10, 0x10);
    // Quad mode: out = in + offset_x + offset_y where offset = step * coef
    sample_and_compare(sim, 0, false);
    sample_and_compare(sim, 1000, false);
    // Row mode
    sample_and_compare(sim, 0, true);
    sample_and_compare(sim, 1000, true);

    // ── Test 3: Different quad and row steps ────────────
    printf("cosim test 3: different quad and row steps\n");
    // quad_step_x=2.0 (0x20), quad_step_y=0.5 (0x08)
    // row_step_x=1.5 (0x18), row_step_y=0.25 (0x04)
    uint64_t coef_two = 2ULL << 25;
    load_stage1(sim, coef_two, coef_one, 0x20, 0x08, 0x18, 0x04);
    sample_and_compare(sim, 0, false);      // quad mode
    sample_and_compare(sim, 0, true);       // row mode
    sample_and_compare(sim, 0x1000, false); // quad with nonzero sample
    sample_and_compare(sim, 0x1000, true);  // row with nonzero sample

    // ── Test 4: Negative coefficients ───────────────────
    printf("cosim test 4: negative coefficients\n");
    // coef = -1.0 in 34-bit signed 9.25 format
    uint64_t coef_neg_one = 0x3FE000000ULL;  // -1.0
    load_stage1(sim, coef_neg_one, coef_neg_one, 0x10, 0x10, 0x10, 0x10);
    sample_and_compare(sim, 0, false);
    sample_and_compare(sim, 0x7FFFFFFFFFFULL, false);  // large positive sample
    sample_and_compare(sim, 0, true);
    sample_and_compare(sim, 0x40000000000ULL, true);   // negative sample (MSB set)

    // ── Test 5: Mixed positive/negative coefficients ────
    printf("cosim test 5: mixed positive/negative coefficients\n");
    uint64_t coef_neg_half = 0x3FF000000ULL;  // -0.5 in 9.25
    load_stage1(sim, coef_one, coef_neg_half, 0x10, 0x10, 0x08, 0x20);
    sample_and_compare(sim, 0x100000, false);
    sample_and_compare(sim, 0x100000, true);

    // ── Test 6: Valid gating (pipeline register enable) ─
    printf("cosim test 6: valid gating\n");
    // Load some offsets first
    load_stage1(sim, coef_one, coef_one, 0x10, 0x10, 0x10, 0x10);
    sample_and_compare(sim, 0, false);  // capture current output

    // Now set different coefficients WITHOUT valid — offsets should not change
    sim.dut->coef_x_i      = coef_neg_one & COEF_MASK;
    sim.dut->coef_y_i      = coef_neg_one & COEF_MASK;
    sim.dut->quad_step_x_i = 0x20;
    sim.dut->quad_step_y_i = 0x20;
    sim.dut->row_step_x_i  = 0x20;
    sim.dut->row_step_y_i  = 0x20;
    sim.dut->valid_i        = 0;
    sim.tick();
    // Stage 2 output should still use the old registered offsets
    sample_and_compare(sim, 0, false);
    sample_and_compare(sim, 0, true);

    // Now pulse valid — offsets should update
    sim.dut->valid_i = 1;
    sim.tick();
    sim.dut->valid_i = 0;
    sample_and_compare(sim, 0, false);
    sample_and_compare(sim, 0, true);

    // ── Test 7: Maximum values ──────────────────────────
    printf("cosim test 7: maximum values\n");
    uint64_t max_pos_coef = 0x1FFFFFFFFULL;  // max positive 34-bit
    uint64_t min_neg_coef = 0x200000000ULL;  // min negative 34-bit
    load_stage1(sim, max_pos_coef, min_neg_coef, 0x3F, 0x3F, 0x3F, 0x3F);
    sample_and_compare(sim, 0, false);
    sample_and_compare(sim, SAMPLE_MASK, false);
    sample_and_compare(sim, 0, true);
    sample_and_compare(sim, SAMPLE_MASK, true);

    // ── Test 8: All step values sweep ───────────────────
    printf("cosim test 8: step value sweep\n");
    for (int s = 0; s < 64; s++) {
        uint8_t step = s & STEP_MASK;
        load_stage1(sim, coef_one, coef_neg_one, step, step, step, step);
        sample_and_compare(sim, 0x10000000ULL, false);
        sample_and_compare(sim, 0x10000000ULL, true);
    }

    // ── Test 9: Random stimulus (3000+ cycles) ──────────
    printf("cosim test 9: random stimulus (3500 cycles)\n");
    uint32_t seed = 0xDEADBEEF;
    for (int i = 0; i < 3500; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        uint32_t s2 = seed * 2654435761u;

        uint64_t cx = ((uint64_t)seed << 2) & COEF_MASK;
        uint64_t cy = ((uint64_t)s2   << 2) & COEF_MASK;
        uint8_t qsx = seed & STEP_MASK;
        uint8_t qsy = (seed >> 6) & STEP_MASK;
        uint8_t rsx = (seed >> 12) & STEP_MASK;
        uint8_t rsy = (seed >> 18) & STEP_MASK;

        load_stage1(sim, cx, cy, qsx, qsy, rsx, rsy);

        // Generate random sample and sample_row
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        uint64_t samp = ((uint64_t)seed << 11) & SAMPLE_MASK;
        bool row = (seed >> 1) & 1;

        sample_and_compare(sim, samp, row);
    }

    // ── Test 10: Alternating quad/row with same offsets ─
    printf("cosim test 10: alternating quad/row\n");
    uint64_t coef_three = 3ULL << 25;  // 3.0
    load_stage1(sim, coef_three, coef_one, 0x10, 0x08, 0x20, 0x04);
    for (int i = 0; i < 50; i++) {
        sample_and_compare(sim, (uint64_t)i * 0x1000, i & 1);
    }

    // ── Test 11: Back-to-back valid pulses ──────────────
    printf("cosim test 11: back-to-back valid pulses\n");
    for (int i = 0; i < 100; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        uint64_t cx = ((uint64_t)seed) & COEF_MASK;
        uint64_t cy = ((uint64_t)(seed * 7)) & COEF_MASK;
        uint8_t qsx = (seed >> 2) & STEP_MASK;
        uint8_t qsy = (seed >> 8) & STEP_MASK;
        uint8_t rsx = (seed >> 14) & STEP_MASK;
        uint8_t rsy = (seed >> 20) & STEP_MASK;

        // Load offsets (this ticks once with valid=1)
        load_stage1(sim, cx, cy, qsx, qsy, rsx, rsy);
        // Immediately sample
        uint64_t samp = ((uint64_t)(seed >> 3) << 5) & SAMPLE_MASK;
        sample_and_compare(sim, samp, seed & 1);
    }

    return sim.finish();
}
