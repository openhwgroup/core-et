// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_traverse vs new rbox_traverse.
//
// Exercises every major path of the triangle rasterization unit:
//   - Reset: verify idle, tr_accept=1, tr_empty=1
//   - Single 4x4 tile (smallest), no MSAA
//   - Single 8x8 tile, partial coverage (scissor clips some quads)
//   - Single 16x16 tile with depth coefficients
//   - Single 32x32 tile (row transitions)
//   - 64x64 tile (many quads)
//   - MSAA 2x mode
//   - MSAA 4x mode
//   - Fully covered tile (all quads inside)
//   - Scissor clipping (min/max X and Y)
//   - Depth clamping
//   - Depth bound testing
//   - Multiple tiles back-to-back
//   - Backpressure (ct_quad_accept/ts_quad_accept deasserted)
//   - Different depth formats (FP32, UN16, UN24)
//   - Random tiles (200+ iterations)

#include "Vcosim_rbox_traverse_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using DUT = Vcosim_rbox_traverse_tb;

// ── Tile size encoding ─────────────────────────────────

enum TileSize : uint8_t {
    TILE_64x64 = 0,
    TILE_64x32 = 1,
    TILE_32x32 = 2,
    TILE_16x16 = 3,
    TILE_8x8   = 4,
    TILE_4x4   = 5,
};

// ── MSAA mode encoding ─────────────────────────────────

enum MsaaMode : uint8_t {
    MSAA_2X  = 0,
    MSAA_4X  = 1,
    MSAA_8X  = 2,
    MSAA_16X = 3,
};

// ── Random helpers ─────────────────────────────────────

static uint32_t xorshift_state = 0xDEADBEEF;

static uint32_t xorshift32() {
    xorshift_state ^= xorshift_state << 13;
    xorshift_state ^= xorshift_state >> 17;
    xorshift_state ^= xorshift_state << 5;
    return xorshift_state;
}

static uint32_t rand_range(uint32_t lo, uint32_t hi) {
    if (lo >= hi) return lo;
    return lo + (xorshift32() % (hi - lo + 1));
}

// ── FP32 helpers ───────────────────────────────────────

static uint32_t make_fp32(int sign, int exp, uint32_t mant) {
    return ((sign & 1) << 31) | ((exp & 0xFF) << 23) | (mant & 0x7FFFFF);
}

// ── Output comparison ──────────────────────────────────

static bool g_msaa_enabled = false;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("tr_accept",          d->orig_tr_accept_o,          d->new_tr_accept_o);
    sim.compare("tr_empty",           d->orig_tr_empty_o,           d->new_tr_empty_o);
    sim.compare("tr_quad_data_valid", d->orig_tr_quad_data_valid_o, d->new_tr_quad_data_valid_o);
    sim.compare("tr_quad_test_valid", d->orig_tr_quad_test_valid_o, d->new_tr_quad_test_valid_o);

    // Compare quad_data when valid
    if (d->orig_tr_quad_data_valid_o || d->new_tr_quad_data_valid_o) {
        sim.compare("qd_x",          d->orig_qd_x_o,          d->new_qd_x_o);
        sim.compare("qd_y",          d->orig_qd_y_o,          d->new_qd_y_o);
        sim.compare("qd_mask",       d->orig_qd_mask_o,       d->new_qd_mask_o);
        sim.compare("qd_sample_idx", d->orig_qd_sample_idx_o, d->new_qd_sample_idx_o);
        sim.compare("qd_i0",         d->orig_qd_i0_o,         d->new_qd_i0_o);
        sim.compare("qd_i1",         d->orig_qd_i1_o,         d->new_qd_i1_o);
        sim.compare("qd_i2",         d->orig_qd_i2_o,         d->new_qd_i2_o);
        sim.compare("qd_i3",         d->orig_qd_i3_o,         d->new_qd_i3_o);
        sim.compare("qd_j0",         d->orig_qd_j0_o,         d->new_qd_j0_o);
        sim.compare("qd_j1",         d->orig_qd_j1_o,         d->new_qd_j1_o);
        sim.compare("qd_j2",         d->orig_qd_j2_o,         d->new_qd_j2_o);
        sim.compare("qd_j3",         d->orig_qd_j3_o,         d->new_qd_j3_o);
        sim.compare("qd_depth0",     d->orig_qd_depth0_o,     d->new_qd_depth0_o);
        sim.compare("qd_depth1",     d->orig_qd_depth1_o,     d->new_qd_depth1_o);
        sim.compare("qd_depth2",     d->orig_qd_depth2_o,     d->new_qd_depth2_o);
        sim.compare("qd_depth3",     d->orig_qd_depth3_o,     d->new_qd_depth3_o);
        // MSAA coverage is only meaningful when MSAA is enabled.
        // In non-MSAA mode the coverage registers hold stale/don't-care values
        // that may differ between original and new (different initialization).
        if (g_msaa_enabled) {
            sim.compare("qd_cov0",       d->orig_qd_cov0_o,       d->new_qd_cov0_o);
            sim.compare("qd_cov1",       d->orig_qd_cov1_o,       d->new_qd_cov1_o);
            sim.compare("qd_cov2",       d->orig_qd_cov2_o,       d->new_qd_cov2_o);
            sim.compare("qd_cov3",       d->orig_qd_cov3_o,       d->new_qd_cov3_o);
        }
    }

    // Compare quad_test when valid
    if (d->orig_tr_quad_test_valid_o || d->new_tr_quad_test_valid_o) {
        sim.compare("qt_x",          d->orig_qt_x_o,          d->new_qt_x_o);
        sim.compare("qt_y",          d->orig_qt_y_o,          d->new_qt_y_o);
        sim.compare("qt_sample_idx", d->orig_qt_sample_idx_o, d->new_qt_sample_idx_o);
        sim.compare("qt_mask",       d->orig_qt_mask_o,       d->new_qt_mask_o);
        sim.compare("qt_depth0",     d->orig_qt_depth0_o,     d->new_qt_depth0_o);
        sim.compare("qt_depth1",     d->orig_qt_depth1_o,     d->new_qt_depth1_o);
        sim.compare("qt_depth2",     d->orig_qt_depth2_o,     d->new_qt_depth2_o);
        sim.compare("qt_depth3",     d->orig_qt_depth3_o,     d->new_qt_depth3_o);
    }
}

// ── Default stimulus (idle state) ──────────────────────

static void set_defaults(DUT* d) {
    g_msaa_enabled = false;
    d->tr_tile_valid_i       = 0;
    d->ct_quad_accept_i      = 1;
    d->ts_quad_accept_i      = 1;
    d->tr_msaa_enable_i      = 0;
    d->tr_msaa_mode_i        = MSAA_2X;
    d->tr_depth_is_fp32_i    = 1;
    d->tr_depth_is_un24_i    = 0;
    d->tr_depth_is_un16_i    = 0;
    d->tr_depth_clamp_enable_i = 0;
    d->tr_depth_min_i        = 0;
    d->tr_depth_max_i        = make_fp32(0, 127, 0); // 1.0
    d->tr_depth_bound_enable_i = 0;
    d->tr_depth_bound_min_i  = 0;
    d->tr_depth_bound_max_i  = make_fp32(0, 127, 0);
    d->tr_scissor_min_x_i    = 0;
    d->tr_scissor_max_x_i    = 0x7FFF;
    d->tr_scissor_min_y_i    = 0;
    d->tr_scissor_max_y_i    = 0x7FFF;

    // Triangle: simple right triangle covering the first quadrant
    // Edge 0: x >= 0  (a=1, b=0)
    // Edge 1: y >= 0  (a=0, b=1)
    // Edge 2: x+y <= large  (a=-1, b=-1)
    d->tri_facing_i       = 0;
    // Edge coefficients are 9.25 fixed point
    // coef = 1.0 = 0x02000000 (1 in 9.25)
    d->tri_edge_eq_a0_i   = 0x02000000ULL; // +1.0
    d->tri_edge_eq_a1_i   = 0;             //  0
    d->tri_edge_eq_a2_i   = 0x3FE000000ULL; // -1.0 in 34-bit 2's complement
    d->tri_edge_eq_b0_i   = 0;
    d->tri_edge_eq_b1_i   = 0x02000000ULL;
    d->tri_edge_eq_b2_i   = 0x3FE000000ULL;
    d->tri_top_or_left_i  = 0x7; // all edges are top-or-left
    d->tri_depth_eq_a_i   = 0;
    d->tri_depth_eq_b_i   = 0;
    d->tri_data_addr_i    = 0;

    // Tile defaults
    d->tile_size_i          = TILE_4x4;
    d->tile_left_i          = 0;
    d->tile_top_i           = 0;
    d->tile_fully_covered_i = 0;
    // Edge samples at tile origin: for a triangle covering everything,
    // use positive values.  edge_sample is 18.25 fixed point = 43 bits.
    // Sample = 1.0 = 0x02000000 (same scale)
    d->tile_edge_sample0_i = 0x02000000ULL;
    d->tile_edge_sample1_i = 0x02000000ULL;
    d->tile_edge_sample2_i = 0x02000000ULL;
    d->tile_depth_sample_i = 0; // FP32 zero
}

// ── Submit a tile and drain ────────────────────────────

// Submit tile_valid for one cycle, then wait until both accept and empty.
// Returns the number of quads output.
static int submit_tile_and_drain(CosimCtrl<DUT>& sim, int max_cycles = 2000) {
    auto* d = sim.dut.get();

    // Assert tile_valid for one cycle
    d->tr_tile_valid_i = 1;
    sim.tick();
    compare_outputs(sim);
    d->tr_tile_valid_i = 0;

    int quads = 0;
    for (int c = 0; c < max_cycles; c++) {
        sim.tick();
        compare_outputs(sim);
        if (d->orig_tr_quad_data_valid_o && d->orig_tr_accept_o == 0) {
            // Quad accepted
        }
        if (d->new_tr_quad_data_valid_o) quads++;
        if (d->new_tr_accept_o && d->new_tr_empty_o) break;
    }
    return quads;
}

// Wait for pipeline to fully drain (empty=1)
static void drain_pipeline(CosimCtrl<DUT>& sim, int max_cycles = 500) {
    auto* d = sim.dut.get();
    for (int c = 0; c < max_cycles; c++) {
        sim.tick();
        compare_outputs(sim);
        if (d->new_tr_accept_o && d->new_tr_empty_o) break;
    }
}

// ── Test 1: Reset ──────────────────────────────────────

static void test_reset(CosimCtrl<DUT>& sim) {
    printf("  Test 1: Reset\n");
    auto* d = sim.dut.get();
    set_defaults(d);
    sim.tick();
    compare_outputs(sim);

    sim.check(d->new_tr_accept_o == 1, "tr_accept should be 1 after reset");
    sim.check(d->new_tr_empty_o  == 1, "tr_empty should be 1 after reset");
    sim.check(d->new_tr_quad_data_valid_o == 0, "no quad data after reset");
    sim.check(d->new_tr_quad_test_valid_o == 0, "no quad test after reset");
}

// ── Test 2: Single 4x4 tile, no MSAA ──────────────────

static void test_4x4_tile(CosimCtrl<DUT>& sim) {
    printf("  Test 2: 4x4 tile, no MSAA\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_4x4;
    d->tile_left_i = 0;
    d->tile_top_i  = 0;

    int quads = submit_tile_and_drain(sim);
    // 4x4 tile = 2 quads x 2 quads = up to 4 quads (but some may be empty)
    printf("    Got %d quads\n", quads);
}

// ── Test 3: 8x8 tile with scissor ─────────────────────

static void test_8x8_scissor(CosimCtrl<DUT>& sim) {
    printf("  Test 3: 8x8 tile with scissor clipping\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_8x8;
    d->tile_left_i = 0;
    d->tile_top_i  = 0;
    // Scissor clips to 4x4 region
    d->tr_scissor_min_x_i = 0;
    d->tr_scissor_max_x_i = 3;
    d->tr_scissor_min_y_i = 0;
    d->tr_scissor_max_y_i = 3;

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads (scissor-clipped)\n", quads);

    // Restore scissor
    d->tr_scissor_max_x_i = 0x7FFF;
    d->tr_scissor_max_y_i = 0x7FFF;
}

// ── Test 4: 16x16 tile with depth ─────────────────────

static void test_16x16_depth(CosimCtrl<DUT>& sim) {
    printf("  Test 4: 16x16 tile with depth coefficients\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_16x16;
    d->tile_left_i = 4; // position at (16,16)
    d->tile_top_i  = 4;

    // Set depth coefficients (small positive FP32 values)
    d->tri_depth_eq_a_i = make_fp32(0, 120, 0); // ~0.0078
    d->tri_depth_eq_b_i = make_fp32(0, 118, 0); // ~0.002
    d->tile_depth_sample_i = make_fp32(0, 126, 0); // 0.5

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads with depth\n", quads);
}

// ── Test 5: 32x32 tile (row transitions) ──────────────

static void test_32x32_tile(CosimCtrl<DUT>& sim) {
    printf("  Test 5: 32x32 tile\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_32x32;
    d->tile_left_i = 0;
    d->tile_top_i  = 0;

    int quads = submit_tile_and_drain(sim, 5000);
    printf("    Got %d quads\n", quads);
}

// ── Test 6: 64x64 tile ────────────────────────────────

static void test_64x64_tile(CosimCtrl<DUT>& sim) {
    printf("  Test 6: 64x64 tile\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_64x64;
    d->tile_left_i = 0;
    d->tile_top_i  = 0;

    int quads = submit_tile_and_drain(sim, 50000);
    printf("    Got %d quads\n", quads);
}

// ── Test 7: MSAA 2x ───────────────────────────────────

static void test_msaa_2x(CosimCtrl<DUT>& sim) {
    printf("  Test 7: MSAA 2x mode\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tr_msaa_enable_i = 1;
    g_msaa_enabled = true;
    d->tr_msaa_mode_i   = MSAA_2X;
    d->tile_size_i       = TILE_4x4;
    d->tile_left_i       = 0;
    d->tile_top_i        = 0;

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads (MSAA 2x)\n", quads);

    d->tr_msaa_enable_i = 0;
    g_msaa_enabled = false;
}

// ── Test 8: MSAA 4x ───────────────────────────────────

static void test_msaa_4x(CosimCtrl<DUT>& sim) {
    printf("  Test 8: MSAA 4x mode\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tr_msaa_enable_i = 1;
    g_msaa_enabled = true;
    d->tr_msaa_mode_i   = MSAA_4X;
    d->tile_size_i       = TILE_4x4;
    d->tile_left_i       = 0;
    d->tile_top_i        = 0;

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads (MSAA 4x)\n", quads);

    d->tr_msaa_enable_i = 0;
    g_msaa_enabled = false;
}

// ── Test 9: Fully covered tile ─────────────────────────

static void test_fully_covered(CosimCtrl<DUT>& sim) {
    printf("  Test 9: Fully covered 4x4 tile\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i          = TILE_4x4;
    d->tile_fully_covered_i = 1;
    // Large positive edge samples ensure all quads are inside
    d->tile_edge_sample0_i = 0x10000000ULL;
    d->tile_edge_sample1_i = 0x10000000ULL;
    d->tile_edge_sample2_i = 0x10000000ULL;

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads (fully covered)\n", quads);

    d->tile_fully_covered_i = 0;
}

// ── Test 10: Depth clamping ────────────────────────────

static void test_depth_clamp(CosimCtrl<DUT>& sim) {
    printf("  Test 10: Depth clamping\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_4x4;
    d->tr_depth_clamp_enable_i = 1;
    d->tr_depth_min_i = make_fp32(0, 124, 0); // ~0.125
    d->tr_depth_max_i = make_fp32(0, 126, 0); // 0.5
    // Depth coefficients that produce values outside the clamp range
    d->tri_depth_eq_a_i = make_fp32(0, 125, 0); // 0.25
    d->tri_depth_eq_b_i = make_fp32(0, 125, 0);
    d->tile_depth_sample_i = make_fp32(0, 126, 0x400000); // 0.75

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads (depth clamped)\n", quads);

    d->tr_depth_clamp_enable_i = 0;
}

// ── Test 11: Depth bound testing ───────────────────────

static void test_depth_bound(CosimCtrl<DUT>& sim) {
    printf("  Test 11: Depth bound testing\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_4x4;
    d->tr_depth_bound_enable_i = 1;
    d->tr_depth_bound_min_i = make_fp32(0, 124, 0); // ~0.125
    d->tr_depth_bound_max_i = make_fp32(0, 126, 0); // 0.5
    d->tile_depth_sample_i = make_fp32(0, 125, 0); // 0.25

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads (depth bound)\n", quads);

    d->tr_depth_bound_enable_i = 0;
}

// ── Test 12: Back-to-back tiles ────────────────────────

static void test_back_to_back(CosimCtrl<DUT>& sim) {
    printf("  Test 12: Back-to-back tiles\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_4x4;

    for (int t = 0; t < 5; t++) {
        d->tile_left_i = t * 1; // different positions
        d->tile_top_i  = t * 1;
        // Large positive edge samples
        d->tile_edge_sample0_i = 0x10000000ULL;
        d->tile_edge_sample1_i = 0x10000000ULL;
        d->tile_edge_sample2_i = 0x10000000ULL;
        submit_tile_and_drain(sim);
    }
    printf("    5 back-to-back tiles done\n");
}

// ── Test 13: Backpressure ──────────────────────────────

static void test_backpressure(CosimCtrl<DUT>& sim) {
    printf("  Test 13: Backpressure (accept deasserted)\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_4x4;
    d->tile_edge_sample0_i = 0x10000000ULL;
    d->tile_edge_sample1_i = 0x10000000ULL;
    d->tile_edge_sample2_i = 0x10000000ULL;

    // Deassert accept to create backpressure
    d->ct_quad_accept_i = 0;
    d->ts_quad_accept_i = 0;

    d->tr_tile_valid_i = 1;
    sim.tick();
    compare_outputs(sim);
    d->tr_tile_valid_i = 0;

    // Run with backpressure for a while
    for (int c = 0; c < 50; c++) {
        sim.tick();
        compare_outputs(sim);
    }

    // Release backpressure and drain
    d->ct_quad_accept_i = 1;
    d->ts_quad_accept_i = 1;
    drain_pipeline(sim);
    printf("    Backpressure test done\n");
}

// ── Test 14: Alternating backpressure ──────────────────

static void test_alternating_backpressure(CosimCtrl<DUT>& sim) {
    printf("  Test 14: Alternating backpressure\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_8x8;
    d->tile_edge_sample0_i = 0x10000000ULL;
    d->tile_edge_sample1_i = 0x10000000ULL;
    d->tile_edge_sample2_i = 0x10000000ULL;

    d->tr_tile_valid_i = 1;
    sim.tick();
    compare_outputs(sim);
    d->tr_tile_valid_i = 0;

    // Alternate backpressure every few cycles
    for (int c = 0; c < 200; c++) {
        d->ct_quad_accept_i = (c % 3 != 0) ? 1 : 0;
        d->ts_quad_accept_i = (c % 5 != 0) ? 1 : 0;
        sim.tick();
        compare_outputs(sim);
        if (d->new_tr_accept_o && d->new_tr_empty_o) break;
    }

    d->ct_quad_accept_i = 1;
    d->ts_quad_accept_i = 1;
    drain_pipeline(sim);
    printf("    Alternating backpressure done\n");
}

// ── Test 15: Depth format UN16 ─────────────────────────

static void test_depth_un16(CosimCtrl<DUT>& sim) {
    printf("  Test 15: Depth format UNORM16\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tr_depth_is_fp32_i = 0;
    d->tr_depth_is_un16_i = 1;
    d->tr_depth_is_un24_i = 0;
    d->tile_size_i = TILE_4x4;
    d->tile_depth_sample_i = make_fp32(0, 126, 0); // 0.5
    d->tile_edge_sample0_i = 0x10000000ULL;
    d->tile_edge_sample1_i = 0x10000000ULL;
    d->tile_edge_sample2_i = 0x10000000ULL;

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads (UN16 format)\n", quads);

    d->tr_depth_is_fp32_i = 1;
    d->tr_depth_is_un16_i = 0;
}

// ── Test 16: Depth format UN24 ─────────────────────────

static void test_depth_un24(CosimCtrl<DUT>& sim) {
    printf("  Test 16: Depth format UNORM24\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tr_depth_is_fp32_i = 0;
    d->tr_depth_is_un16_i = 0;
    d->tr_depth_is_un24_i = 1;
    d->tile_size_i = TILE_4x4;
    d->tile_depth_sample_i = make_fp32(0, 125, 0); // 0.25
    d->tile_edge_sample0_i = 0x10000000ULL;
    d->tile_edge_sample1_i = 0x10000000ULL;
    d->tile_edge_sample2_i = 0x10000000ULL;

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads (UN24 format)\n", quads);

    d->tr_depth_is_fp32_i = 1;
    d->tr_depth_is_un24_i = 0;
}

// ── Test 17: Scissor min/max both X and Y ──────────────

static void test_scissor_all_edges(CosimCtrl<DUT>& sim) {
    printf("  Test 17: Scissor clipping on all edges\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_16x16;
    d->tile_left_i = 0;
    d->tile_top_i  = 0;
    // Scissor to middle 8x8 region
    d->tr_scissor_min_x_i = 4;
    d->tr_scissor_max_x_i = 11;
    d->tr_scissor_min_y_i = 4;
    d->tr_scissor_max_y_i = 11;
    d->tile_edge_sample0_i = 0x10000000ULL;
    d->tile_edge_sample1_i = 0x10000000ULL;
    d->tile_edge_sample2_i = 0x10000000ULL;

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads (scissored all edges)\n", quads);

    d->tr_scissor_min_x_i = 0;
    d->tr_scissor_max_x_i = 0x7FFF;
    d->tr_scissor_min_y_i = 0;
    d->tr_scissor_max_y_i = 0x7FFF;
}

// ── Test 18: Tile at non-zero position ─────────────────

static void test_offset_tile(CosimCtrl<DUT>& sim) {
    printf("  Test 18: Tile at non-zero position\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tile_size_i = TILE_8x8;
    d->tile_left_i = 100; // position (400, 200) in pixel coords
    d->tile_top_i  = 50;
    d->tile_edge_sample0_i = 0x10000000ULL;
    d->tile_edge_sample1_i = 0x10000000ULL;
    d->tile_edge_sample2_i = 0x10000000ULL;
    d->tr_scissor_max_x_i = 500;
    d->tr_scissor_max_y_i = 300;

    int quads = submit_tile_and_drain(sim);
    printf("    Got %d quads (offset tile)\n", quads);

    d->tr_scissor_max_x_i = 0x7FFF;
    d->tr_scissor_max_y_i = 0x7FFF;
}

// ── Test 19: MSAA 2x with 8x8 tile ────────────────────

static void test_msaa_2x_8x8(CosimCtrl<DUT>& sim) {
    printf("  Test 19: MSAA 2x with 8x8 tile\n");
    auto* d = sim.dut.get();
    set_defaults(d);

    d->tr_msaa_enable_i = 1;
    g_msaa_enabled = true;
    d->tr_msaa_mode_i   = MSAA_2X;
    d->tile_size_i       = TILE_8x8;
    d->tile_edge_sample0_i = 0x10000000ULL;
    d->tile_edge_sample1_i = 0x10000000ULL;
    d->tile_edge_sample2_i = 0x10000000ULL;

    int quads = submit_tile_and_drain(sim, 3000);
    printf("    Got %d quads (MSAA 2x, 8x8)\n", quads);

    d->tr_msaa_enable_i = 0;
    g_msaa_enabled = false;
}

// ── Test 20: Random tiles ──────────────────────────────

static void test_random_tiles(CosimCtrl<DUT>& sim, int iterations) {
    printf("  Test 20: Random tiles (%d iterations)\n", iterations);
    auto* d = sim.dut.get();

    static const uint8_t tile_sizes[] = {TILE_4x4, TILE_8x8, TILE_16x16, TILE_4x4, TILE_4x4};

    for (int i = 0; i < iterations; i++) {
        set_defaults(d);

        // Random tile size (bias toward smaller tiles for speed)
        d->tile_size_i = tile_sizes[xorshift32() % 5];

        // Random tile position
        d->tile_left_i = xorshift32() & 0x1FFF;
        d->tile_top_i  = xorshift32() & 0x1FFF;

        // Random edge samples (positive, to get some coverage)
        uint64_t base_samp = 0x02000000ULL + (xorshift32() & 0x0FFFFFFFULL);
        d->tile_edge_sample0_i = base_samp;
        d->tile_edge_sample1_i = base_samp;
        d->tile_edge_sample2_i = base_samp;

        // Random depth sample
        d->tile_depth_sample_i = make_fp32(0, rand_range(120, 127), xorshift32() & 0x7FFFFF);

        // Random depth coefficients
        d->tri_depth_eq_a_i = make_fp32(0, rand_range(100, 120), xorshift32() & 0x7FFFFF);
        d->tri_depth_eq_b_i = make_fp32(0, rand_range(100, 120), xorshift32() & 0x7FFFFF);

        // Randomly enable MSAA (20% chance)
        if ((xorshift32() % 5) == 0) {
            d->tr_msaa_enable_i = 1;
            g_msaa_enabled = true;
            d->tr_msaa_mode_i = xorshift32() & 0x1; // only 2x/4x for speed
        } else {
            d->tr_msaa_enable_i = 0;
            g_msaa_enabled = false;
        }

        // Random depth format (10% UN16, 10% UN24, 80% FP32)
        uint32_t fmt = xorshift32() % 10;
        if (fmt == 0) {
            d->tr_depth_is_fp32_i = 0;
            d->tr_depth_is_un16_i = 1;
            d->tr_depth_is_un24_i = 0;
        } else if (fmt == 1) {
            d->tr_depth_is_fp32_i = 0;
            d->tr_depth_is_un16_i = 0;
            d->tr_depth_is_un24_i = 1;
        } else {
            d->tr_depth_is_fp32_i = 1;
            d->tr_depth_is_un16_i = 0;
            d->tr_depth_is_un24_i = 0;
        }

        // Random depth clamping (20% chance)
        if ((xorshift32() % 5) == 0) {
            d->tr_depth_clamp_enable_i = 1;
            d->tr_depth_min_i = make_fp32(0, 120, 0);
            d->tr_depth_max_i = make_fp32(0, 126, 0);
        } else {
            d->tr_depth_clamp_enable_i = 0;
        }

        // Random depth bound (15% chance)
        if ((xorshift32() % 7) == 0) {
            d->tr_depth_bound_enable_i = 1;
            d->tr_depth_bound_min_i = make_fp32(0, 120, 0);
            d->tr_depth_bound_max_i = make_fp32(0, 127, 0);
        } else {
            d->tr_depth_bound_enable_i = 0;
        }

        // Random backpressure pattern
        int bp_pattern = xorshift32() % 4;

        // Set scissor to encompass the tile (ensures some coverage)
        uint32_t tile_px = (uint32_t)(d->tile_left_i) << 2;
        uint32_t tile_py = (uint32_t)(d->tile_top_i) << 2;
        d->tr_scissor_min_x_i = (tile_px > 2) ? tile_px - 2 : 0;
        d->tr_scissor_min_y_i = (tile_py > 2) ? tile_py - 2 : 0;
        d->tr_scissor_max_x_i = (tile_px + 70 < 0x7FFF) ? tile_px + 70 : 0x7FFF;
        d->tr_scissor_max_y_i = (tile_py + 70 < 0x7FFF) ? tile_py + 70 : 0x7FFF;

        // Submit tile
        int mismatch_before = sim.mismatches;
        d->tr_tile_valid_i = 1;
        sim.tick();
        compare_outputs(sim);
        d->tr_tile_valid_i = 0;

        // Drain with backpressure pattern
        int max_drain = (d->tile_size_i <= TILE_4x4) ? 500 : 5000;
        for (int c = 0; c < max_drain; c++) {
            switch (bp_pattern) {
                case 0: // no backpressure
                    d->ct_quad_accept_i = 1;
                    d->ts_quad_accept_i = 1;
                    break;
                case 1: // periodic ct backpressure
                    d->ct_quad_accept_i = (c % 3 != 0) ? 1 : 0;
                    d->ts_quad_accept_i = 1;
                    break;
                case 2: // periodic ts backpressure
                    d->ct_quad_accept_i = 1;
                    d->ts_quad_accept_i = (c % 4 != 0) ? 1 : 0;
                    break;
                case 3: // both periodic
                    d->ct_quad_accept_i = (c % 3 != 0) ? 1 : 0;
                    d->ts_quad_accept_i = (c % 5 != 0) ? 1 : 0;
                    break;
            }
            sim.tick();
            compare_outputs(sim);
            if (d->new_tr_accept_o && d->new_tr_empty_o) break;
        }

        // Ensure fully drained
        d->ct_quad_accept_i = 1;
        d->ts_quad_accept_i = 1;
        drain_pipeline(sim);

        if (sim.mismatches > mismatch_before) {
            printf("    FAIL iter %d: tile_sz=%d msaa=%d mode=%d left=0x%x top=0x%x\n",
                   i, d->tile_size_i, d->tr_msaa_enable_i, d->tr_msaa_mode_i,
                   d->tile_left_i, d->tile_top_i);
            printf("      edge0=0x%llx edge1=0x%llx edge2=0x%llx\n",
                   (unsigned long long)d->tile_edge_sample0_i,
                   (unsigned long long)d->tile_edge_sample1_i,
                   (unsigned long long)d->tile_edge_sample2_i);
        }

        // Abort early if too many mismatches
        if (sim.mismatches > 100) {
            printf("    Too many mismatches, aborting random tests at iteration %d\n", i);
            return;
        }
    }
    printf("    %d random tiles done\n", iterations);
}

// ── Main ───────────────────────────────────────────────

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000000; // Large timeout for 64x64 tiles + random tests

    set_defaults(sim.dut.get());
    sim.reset();

    test_reset(sim);
    test_4x4_tile(sim);
    test_8x8_scissor(sim);
    test_16x16_depth(sim);
    test_32x32_tile(sim);
    test_64x64_tile(sim);
    test_msaa_2x(sim);
    test_msaa_4x(sim);
    test_fully_covered(sim);
    test_depth_clamp(sim);
    test_depth_bound(sim);
    test_back_to_back(sim);
    test_backpressure(sim);
    test_alternating_backpressure(sim);
    test_depth_un16(sim);
    test_depth_un24(sim);
    test_scissor_all_edges(sim);
    test_offset_tile(sim);
    test_msaa_2x_8x8(sim);
    test_random_tiles(sim, 200);

    // ── Test: Demo triangle (RGB triangle from rbox_demo) ─────
    // Exercises the exact edge equations used in the demo, which produced
    // a gap at rows 48-55.  Verifies original and new produce the same
    // output (same gap pattern = RTL feature, not reimplementation bug).
    //
    // Triangle: v0=(32,4) v1=(4,60) v2=(60,60)
    // Edge convention: edge[i] opposite vertex i.
    //   edge[0] (v1→v2): a=  0, b=-56
    //   edge[1] (v2→v0): a=-56, b= 28
    //   edge[2] (v0→v1): a= 56, b= 28
    // Samples at origin (0,0): E0=3360, E1=1680, E2=-1904
    {
        printf("  Test: Demo triangle (64x64, rows 48-55 gap)\n");
        sim.reset(10);
        auto* d = sim.dut.get();
        set_defaults(d);
        sim.tick(); compare_outputs(sim);  // apply defaults

        d->tile_size_i = TILE_64x64;
        d->tile_left_i = 0;
        d->tile_top_i  = 0;
        d->tri_facing_i = 0;  // front-facing

        // Scissor: full 64x64
        d->tr_scissor_min_x_i = 0;
        d->tr_scissor_max_x_i = 64;
        d->tr_scissor_min_y_i = 0;
        d->tr_scissor_max_y_i = 64;

        // Depth: FP32, always pass (no depth test effect)
        d->tr_depth_is_fp32_i = 1;
        d->tr_depth_is_un16_i = 0;
        d->tr_depth_is_un24_i = 0;

        // Edge coefficients in 9.25 internal format (34-bit signed).
        // Convert from 9.14: shift left by (25-14)=11 bits.
        auto coef_9_25 = [](double val) -> uint64_t {
            int64_t raw = (int64_t)round(val * (1 << 25));
            return (uint64_t)(raw & 0x3FFFFFFFFLL);  // 34-bit mask
        };

        // edge[0]: a=0, b=-56
        d->tri_edge_eq_a0_i = coef_9_25(0.0);
        d->tri_edge_eq_b0_i = coef_9_25(-56.0);
        // edge[1]: a=-56, b=28
        d->tri_edge_eq_a1_i = coef_9_25(-56.0);
        d->tri_edge_eq_b1_i = coef_9_25(28.0);
        // edge[2]: a=56, b=28
        d->tri_edge_eq_a2_i = coef_9_25(56.0);
        d->tri_edge_eq_b2_i = coef_9_25(28.0);

        // Top-or-left edge flags (standard: edge with a>0 or a==0&&b<0)
        d->tri_top_or_left_i = 0;
        // edge[0]: a=0, b=-56 → a==0 && b<0 → top edge → bit 0 = 1
        d->tri_top_or_left_i |= (1 << 0);
        // edge[1]: a=-56 → a<0 → not top-or-left → bit 1 = 0
        // edge[2]: a=56  → a>0 → left edge → bit 2 = 1
        d->tri_top_or_left_i |= (1 << 2);

        // Depth equation: flat depth (a=0, b=0)
        d->tri_depth_eq_a_i = 0;
        d->tri_depth_eq_b_i = 0;

        // Edge samples at (0,0) in 18.25 internal format (43-bit signed).
        // Convert from integer: shift left by 25 fractional bits.
        auto samp_18_25 = [](double val) -> uint64_t {
            int64_t raw = (int64_t)round(val * (1 << 25));
            return (uint64_t)(raw & 0x7FFFFFFFFFFLL);  // 43-bit mask
        };

        d->tile_edge_sample0_i = samp_18_25(3360.0);
        d->tile_edge_sample1_i = samp_18_25(1680.0);
        d->tile_edge_sample2_i = samp_18_25(-1904.0);
        d->tile_depth_sample_i = 0x3F000000;  // 0.5 FP32

        // Submit tile
        d->tr_tile_valid_i = 1;
        sim.tick();
        compare_outputs(sim);
        printf("    After submit: accept=%d empty=%d\n",
               (int)d->new_tr_accept_o, (int)d->new_tr_empty_o);
        d->tr_tile_valid_i = 0;

        int quads = 0;
        for (int c = 0; c < 50000; c++) {
            sim.tick();
            compare_outputs(sim);
            if (d->new_tr_quad_data_valid_o) {
                if (quads < 5 || (int)d->new_qd_y_o >= 60) {
                    printf("    quad[%d] at (%d,%d) mask=0x%x",
                           quads, (int)d->new_qd_x_o, (int)d->new_qd_y_o,
                           (int)d->new_qd_mask_o);
                    if ((int)d->new_qd_y_o >= 60) {
                        // Print edge[0] value (bary_i) for pixel 0 to see if it's positive
                        uint32_t i0 = d->new_qd_i0_o;
                        float fi0; memcpy(&fi0, &i0, 4);
                        uint32_t j0 = d->new_qd_j0_o;
                        float fj0; memcpy(&fj0, &j0, 4);
                        printf(" i0=%.1f j0=%.1f", fi0, fj0);
                    }
                    printf("\n");
                }
                quads++;
            }
            if (d->new_tr_accept_o && d->new_tr_empty_o) {
                printf("    Done at cycle %d\n", c);
                break;
            }
            if (c == 49999) printf("    TIMEOUT after 50000 cycles\n");
        }
        printf("    Got %d quads from demo triangle\n", quads);

        // Verify: with correct edge equations, the triangle covers
        // approximately rows 5-61, expanding from ~1 pixel to ~55 pixels wide.
        // Expected total coverage ~1568 pixels / 4 per quad ≈ 392 quads.
        // If early-row-termination incorrectly fires, we get fewer.
        // The key result: original and new MATCH (0 mismatches in cosim).
    }

    return sim.finish();
}
