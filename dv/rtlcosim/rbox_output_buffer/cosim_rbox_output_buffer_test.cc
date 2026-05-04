// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_output_buffer vs new rbox_output_buffer.

#include "Vcosim_rbox_output_buffer_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

using DUT = Vcosim_rbox_output_buffer_tb;

// quad_data_t is 468 bits = 15 x 32-bit words (VlWide<15>).
// out_info is 58 bits = 2 x 32-bit words (VlWide<2>).
// out_packet is 256 bits = 8 x 32-bit words.

static void compare_wide(CosimCtrl<DUT>& sim, const char* name,
                         const uint32_t* orig, const uint32_t* newv, int words) {
    for (int w = 0; w < words; w++) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%s[%d]", name, w);
        sim.compare(buf, orig[w], newv[w]);
    }
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("ct_quad_accept",   sim.dut->orig_ct_quad_accept_o,   sim.dut->new_ct_quad_accept_o);
    sim.compare("ct_result_accept", sim.dut->orig_ct_result_accept_o, sim.dut->new_ct_result_accept_o);
    sim.compare("out_info_valid",   sim.dut->orig_out_info_valid_o,   sim.dut->new_out_info_valid_o);
    sim.compare("out_packet_valid", sim.dut->orig_out_packet_valid_o, sim.dut->new_out_packet_valid_o);
    sim.compare("empty",            sim.dut->orig_empty_o,            sim.dut->new_empty_o);

    // Compare out_info bits when valid on either side (58 bits = QData)
    if (sim.dut->orig_out_info_valid_o || sim.dut->new_out_info_valid_o) {
        sim.compare("out_info_lo", (uint32_t)(sim.dut->orig_out_info_bits_o),
                                   (uint32_t)(sim.dut->new_out_info_bits_o));
        sim.compare("out_info_hi", (uint32_t)(sim.dut->orig_out_info_bits_o >> 32),
                                   (uint32_t)(sim.dut->new_out_info_bits_o >> 32));
    }

    // Compare out_packet when valid on either side (256 bits = 8 words)
    if (sim.dut->orig_out_packet_valid_o || sim.dut->new_out_packet_valid_o) {
        compare_wide(sim, "out_packet", sim.dut->orig_out_packet_o.data(),
                     sim.dut->new_out_packet_o.data(), 8);
    }
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

// Build a quad_data_t bit pattern from small integers.
// Layout (MSB first in packed struct):
//   x[5:0]  y[5:0]  mask[3:0]  i[127:0]  j[127:0]  depth[127:0]
//   sample_idx[3:0]  msaa_coverage[63:0]
// Total: 6+6+4+128+128+128+4+64 = 468 bits = VlWide<15>
static void make_quad(DUT* dut, uint8_t x, uint8_t y, uint8_t mask,
                      uint32_t i_val, uint32_t j_val, uint32_t depth_val,
                      uint8_t sample_idx, uint32_t msaa_cov) {
    // Zero out all 15 words first
    for (int w = 0; w < 15; w++)
        dut->tr_quad_bits_i[w] = 0;

    // Pack from LSB: msaa_coverage[63:0] at bits [63:0]
    dut->tr_quad_bits_i[0] = msaa_cov;
    dut->tr_quad_bits_i[1] = 0;  // upper msaa_coverage

    // sample_idx[3:0] at bits [67:64]
    dut->tr_quad_bits_i[2] = (uint32_t)sample_idx;

    // depth[127:0] at bits [195:68]
    //   depth is 4x32 = 128 bits. Put depth_val in each of the 4 lanes.
    //   bit 68 is in word 2, bit 4
    uint64_t depth_start = 68;
    for (int lane = 0; lane < 4; lane++) {
        uint64_t bit_pos = depth_start + lane * 32;
        int word = bit_pos / 32;
        int shift = bit_pos % 32;
        if (shift == 0) {
            dut->tr_quad_bits_i[word] |= depth_val;
        } else {
            dut->tr_quad_bits_i[word] |= (depth_val << shift);
            dut->tr_quad_bits_i[word + 1] |= (depth_val >> (32 - shift));
        }
    }

    // j[127:0] at bits [323:196]
    uint64_t j_start = 196;
    for (int lane = 0; lane < 4; lane++) {
        uint64_t bit_pos = j_start + lane * 32;
        int word = bit_pos / 32;
        int shift = bit_pos % 32;
        if (shift == 0) {
            dut->tr_quad_bits_i[word] |= j_val;
        } else {
            dut->tr_quad_bits_i[word] |= (j_val << shift);
            dut->tr_quad_bits_i[word + 1] |= (j_val >> (32 - shift));
        }
    }

    // i[127:0] at bits [451:324]
    uint64_t i_start = 324;
    for (int lane = 0; lane < 4; lane++) {
        uint64_t bit_pos = i_start + lane * 32;
        int word = bit_pos / 32;
        int shift = bit_pos % 32;
        if (shift == 0) {
            dut->tr_quad_bits_i[word] |= i_val;
        } else {
            dut->tr_quad_bits_i[word] |= (i_val << shift);
            if (word + 1 < 15)
                dut->tr_quad_bits_i[word + 1] |= (i_val >> (32 - shift));
        }
    }

    // mask[3:0] at bits [455:452]
    uint64_t mask_start = 452;
    {
        int word = mask_start / 32;
        int shift = mask_start % 32;
        dut->tr_quad_bits_i[word] |= ((uint32_t)(mask & 0xF) << shift);
    }

    // y[5:0] at bits [461:456]
    uint64_t y_start = 456;
    {
        int word = y_start / 32;
        int shift = y_start % 32;
        dut->tr_quad_bits_i[word] |= ((uint32_t)(y & 0x3F) << shift);
    }

    // x[5:0] at bits [467:462]
    uint64_t x_start = 462;
    {
        int word = x_start / 32;
        int shift = x_start % 32;
        dut->tr_quad_bits_i[word] |= ((uint32_t)(x & 0x3F) << shift);
        if (shift + 6 > 32 && word + 1 < 15) {
            dut->tr_quad_bits_i[word + 1] |= ((uint32_t)(x & 0x3F) >> (32 - shift));
        }
    }
}

static void clear_inputs(DUT* dut) {
    for (int w = 0; w < 15; w++) dut->tr_quad_bits_i[w] = 0;
    dut->tr_quad_valid_i         = 0;
    dut->ts_result_mask_i        = 0;
    dut->ts_result_valid_i       = 0;
    dut->out_pop_i               = 0;
    dut->out_pending_pcks_i      = 0;
    dut->out_end_drawcall_i      = 0;
    dut->hart_tile_width_i       = 0;
    dut->hart_tile_height_i      = 0;
    dut->shire_layout_width_i    = 0;
    dut->shire_layout_height_i   = 0;
    dut->fsh_reads_coverage_i    = 0;
    dut->fsh_reads_depth_i       = 0;
    dut->fsh_disabled_i          = 0;
    dut->early_frag_tests_enable_i = 0;
    dut->rbox_halted_i           = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;

    clear_inputs(sim.dut.get());
    sim.reset(10);
    compare_outputs(sim);

    // ── Test 1: Empty after reset ─────────────────────────
    printf("cosim test 1: empty after reset\n");
    tick_and_compare(sim);

    // ── Test 2: Single quad (fsh_disabled, no early frag) ─
    printf("cosim test 2: single quad (fsh_disabled)\n");
    sim.dut->fsh_disabled_i = 1;

    // Push one quad
    make_quad(sim.dut.get(), 2, 4, 0xF, 0x11111111, 0x22222222, 0x33333333, 0, 0);
    sim.dut->tr_quad_valid_i = 1;
    tick_and_compare(sim);
    sim.dut->tr_quad_valid_i = 0;
    for (int w = 0; w < 15; w++) sim.dut->tr_quad_bits_i[w] = 0;

    // Push a second quad at a different position to form a pair
    make_quad(sim.dut.get(), 2, 6, 0xA, 0x44444444, 0x55555555, 0x66666666, 0, 0);
    sim.dut->tr_quad_valid_i = 1;
    tick_and_compare(sim);
    sim.dut->tr_quad_valid_i = 0;
    for (int w = 0; w < 15; w++) sim.dut->tr_quad_bits_i[w] = 0;

    // Wait for out_info_valid
    for (int i = 0; i < 5; i++)
        tick_and_compare(sim);

    // Read packet with pending_pcks=1 (fsh_disabled: only 1 packet needed)
    // With fsh_disabled: no bary, no depth unless out_depth_enabled
    // out_depth_enabled = !early_frag_tests_enable || fsh_reads_depth = 1 || 0 = 1
    // So we need depth packet. pending_pcks=1 => out_pck_depth.
    sim.dut->out_pending_pcks_i = 1;
    for (int i = 0; i < 5; i++)
        tick_and_compare(sim);

    // Pop
    sim.dut->out_pop_i = 1;
    sim.dut->out_pending_pcks_i = 1;
    tick_and_compare(sim);
    sim.dut->out_pop_i = 0;
    sim.dut->out_pending_pcks_i = 0;
    for (int i = 0; i < 3; i++)
        tick_and_compare(sim);

    sim.dut->fsh_disabled_i = 0;

    // ── Test 3: Early fragment test mode ──────────────────
    printf("cosim test 3: early fragment test mode\n");
    sim.dut->early_frag_tests_enable_i = 1;
    sim.dut->fsh_reads_depth_i = 1;

    // Push a quad
    make_quad(sim.dut.get(), 4, 4, 0xF, 0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0, 0);
    sim.dut->tr_quad_valid_i = 1;
    tick_and_compare(sim);
    sim.dut->tr_quad_valid_i = 0;
    for (int w = 0; w < 15; w++) sim.dut->tr_quad_bits_i[w] = 0;

    // Provide test result
    sim.dut->ts_result_mask_i = 0xF;
    sim.dut->ts_result_valid_i = 1;
    tick_and_compare(sim);
    sim.dut->ts_result_valid_i = 0;
    sim.dut->ts_result_mask_i = 0;

    // Push another quad to form a pair
    make_quad(sim.dut.get(), 4, 6, 0xC, 0xDDDDDDDD, 0xEEEEEEEE, 0xFFFFFFFF, 0, 0);
    sim.dut->tr_quad_valid_i = 1;
    tick_and_compare(sim);
    sim.dut->tr_quad_valid_i = 0;
    for (int w = 0; w < 15; w++) sim.dut->tr_quad_bits_i[w] = 0;

    // Provide test result for second quad
    sim.dut->ts_result_mask_i = 0xA;
    sim.dut->ts_result_valid_i = 1;
    tick_and_compare(sim);
    sim.dut->ts_result_valid_i = 0;
    sim.dut->ts_result_mask_i = 0;

    // Wait and read packets
    for (int i = 0; i < 10; i++)
        tick_and_compare(sim);

    // Try reading various pending_pcks values to exercise output mux
    for (int p = 4; p >= 1; p--) {
        sim.dut->out_pending_pcks_i = p;
        for (int i = 0; i < 3; i++)
            tick_and_compare(sim);
    }

    // Pop
    sim.dut->out_pop_i = 1;
    sim.dut->out_pending_pcks_i = 1;
    tick_and_compare(sim);
    sim.dut->out_pop_i = 0;
    sim.dut->out_pending_pcks_i = 0;
    for (int i = 0; i < 5; i++)
        tick_and_compare(sim);

    sim.dut->early_frag_tests_enable_i = 0;
    sim.dut->fsh_reads_depth_i = 0;

    // ── Test 4: Depth write back ──────────────────────────
    printf("cosim test 4: depth write back\n");
    sim.dut->fsh_reads_depth_i = 1;

    for (int q = 0; q < 4; q++) {
        make_quad(sim.dut.get(), q * 2, 0, 0xF,
                  0x10000000 + q, 0x20000000 + q, 0x30000000 + q, 0, 0);
        sim.dut->tr_quad_valid_i = 1;
        tick_and_compare(sim);
        sim.dut->tr_quad_valid_i = 0;
        for (int w = 0; w < 15; w++) sim.dut->tr_quad_bits_i[w] = 0;
        tick_and_compare(sim);
    }

    // Wait for output, try reading packets with various pending_pcks
    for (int i = 0; i < 8; i++)
        tick_and_compare(sim);

    for (int p = 3; p >= 1; p--) {
        sim.dut->out_pending_pcks_i = p;
        for (int i = 0; i < 3; i++)
            tick_and_compare(sim);
    }

    // Pop first pair
    sim.dut->out_pop_i = 1;
    sim.dut->out_pending_pcks_i = 1;
    tick_and_compare(sim);
    sim.dut->out_pop_i = 0;
    sim.dut->out_pending_pcks_i = 0;
    for (int i = 0; i < 5; i++)
        tick_and_compare(sim);

    // Pop second pair
    for (int p = 3; p >= 1; p--) {
        sim.dut->out_pending_pcks_i = p;
        for (int i = 0; i < 3; i++)
            tick_and_compare(sim);
    }
    sim.dut->out_pop_i = 1;
    sim.dut->out_pending_pcks_i = 1;
    tick_and_compare(sim);
    sim.dut->out_pop_i = 0;
    sim.dut->out_pending_pcks_i = 0;
    for (int i = 0; i < 3; i++)
        tick_and_compare(sim);

    sim.dut->fsh_reads_depth_i = 0;

    // ── Test 5: Simultaneous quad + result ────────────────
    printf("cosim test 5: simultaneous quad + result\n");
    sim.dut->early_frag_tests_enable_i = 1;

    // Push first quad
    make_quad(sim.dut.get(), 8, 8, 0xF, 0x11, 0x22, 0x33, 0, 0);
    sim.dut->tr_quad_valid_i = 1;
    tick_and_compare(sim);

    // Push second quad AND provide result for first simultaneously
    make_quad(sim.dut.get(), 8, 10, 0xF, 0x44, 0x55, 0x66, 0, 0);
    sim.dut->tr_quad_valid_i = 1;
    sim.dut->ts_result_mask_i = 0xF;
    sim.dut->ts_result_valid_i = 1;
    tick_and_compare(sim);

    sim.dut->tr_quad_valid_i = 0;
    for (int w = 0; w < 15; w++) sim.dut->tr_quad_bits_i[w] = 0;

    // Provide result for second quad
    sim.dut->ts_result_mask_i = 0x5;
    sim.dut->ts_result_valid_i = 1;
    tick_and_compare(sim);
    sim.dut->ts_result_valid_i = 0;
    sim.dut->ts_result_mask_i = 0;

    for (int i = 0; i < 10; i++)
        tick_and_compare(sim);

    // Pop
    sim.dut->out_pop_i = 1;
    sim.dut->out_pending_pcks_i = 1;
    tick_and_compare(sim);
    sim.dut->out_pop_i = 0;
    sim.dut->out_pending_pcks_i = 0;
    for (int i = 0; i < 5; i++)
        tick_and_compare(sim);

    sim.dut->early_frag_tests_enable_i = 0;

    // ── Test 6: Drawcall end ──────────────────────────────
    printf("cosim test 6: drawcall end\n");

    // Push a single quad (odd number - no natural pair)
    make_quad(sim.dut.get(), 0, 0, 0xF, 0x77, 0x88, 0x99, 0, 0);
    sim.dut->tr_quad_valid_i = 1;
    tick_and_compare(sim);
    sim.dut->tr_quad_valid_i = 0;
    for (int w = 0; w < 15; w++) sim.dut->tr_quad_bits_i[w] = 0;

    for (int i = 0; i < 3; i++)
        tick_and_compare(sim);

    // Signal end of drawcall to flush the unpaired quad
    sim.dut->out_end_drawcall_i = 1;
    for (int i = 0; i < 5; i++)
        tick_and_compare(sim);
    sim.dut->out_end_drawcall_i = 0;

    // Read packets
    for (int p = 2; p >= 1; p--) {
        sim.dut->out_pending_pcks_i = p;
        for (int i = 0; i < 4; i++)
            tick_and_compare(sim);
    }

    // Pop
    sim.dut->out_pop_i = 1;
    sim.dut->out_pending_pcks_i = 1;
    tick_and_compare(sim);
    sim.dut->out_pop_i = 0;
    sim.dut->out_pending_pcks_i = 0;
    for (int i = 0; i < 5; i++)
        tick_and_compare(sim);

    // ── Test 7: Random stimulus (300+ cycles) ─────────────
    printf("cosim test 7: random stimulus (350 cycles)\n");
    uint32_t seed = 0xDEADBEEF;
    int quads_in_flight = 0;
    int results_pending = 0;

    // Use a fixed configuration for random testing
    sim.dut->fsh_disabled_i = 0;
    sim.dut->fsh_reads_depth_i = 1;
    sim.dut->fsh_reads_coverage_i = 0;
    sim.dut->early_frag_tests_enable_i = 0;
    sim.dut->hart_tile_width_i  = 1;
    sim.dut->hart_tile_height_i = 1;
    sim.dut->shire_layout_width_i  = 1;
    sim.dut->shire_layout_height_i = 1;

    for (int i = 0; i < 350; i++) {
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;

        // Random quad push (throttle to avoid overfilling)
        int want_push = ((seed & 0xF) < 5) && (quads_in_flight < 60);
        if (want_push) {
            uint8_t x = (seed >> 4) & 0x3F;
            uint8_t y = (seed >> 10) & 0x3F;
            uint8_t mask = (seed >> 16) & 0xF;
            if (mask == 0) mask = 1;  // at least one fragment
            make_quad(sim.dut.get(), x, y, mask,
                      seed, seed ^ 0xFF, seed ^ 0xAA, (seed >> 20) & 0xF, seed);
            sim.dut->tr_quad_valid_i = 1;
        } else {
            sim.dut->tr_quad_valid_i = 0;
        }

        // Random result (only meaningful in early frag test, but exercise anyway)
        int want_result = ((seed >> 8) & 0xF) < 3;
        if (want_result && results_pending > 0) {
            sim.dut->ts_result_mask_i = (seed >> 12) & 0xF;
            sim.dut->ts_result_valid_i = 1;
        } else {
            sim.dut->ts_result_valid_i = 0;
        }

        // Random pending_pcks (1-4)
        sim.dut->out_pending_pcks_i = 1 + ((seed >> 20) & 0x3);

        // Random pop — only when pending_pcks==1 (assertion requirement)
        int want_pop = ((seed >> 16) & 0xF) < 3;
        if (want_pop && sim.dut->out_pending_pcks_i == 1) {
            sim.dut->out_pop_i = 1;
        } else {
            sim.dut->out_pop_i = 0;
        }

        // Random drawcall end
        sim.dut->out_end_drawcall_i = ((seed >> 24) & 0x3F) == 0;

        // Random halted
        sim.dut->rbox_halted_i = ((seed >> 28) & 0xF) == 0;

        tick_and_compare(sim);

        // Track quad accept
        if (sim.dut->new_ct_quad_accept_o && want_push)
            quads_in_flight++;
        if (sim.dut->new_ct_result_accept_o)
            results_pending--;
        if (want_push && sim.dut->new_ct_quad_accept_o &&
            sim.dut->early_frag_tests_enable_i)
            results_pending++;
    }

    // Drain
    clear_inputs(sim.dut.get());
    for (int i = 0; i < 20; i++)
        tick_and_compare(sim);

    return sim.finish();
}
