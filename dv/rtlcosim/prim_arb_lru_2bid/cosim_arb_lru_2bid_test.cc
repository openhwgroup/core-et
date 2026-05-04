// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: arb_lru_2_bid (original) vs prim_arb_lru_2bid (new).
// Drives identical stimulus and compares grant0 + grant1 each cycle.

#include "Vcosim_arb_lru_2bid_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>

using DUT = Vcosim_arb_lru_2bid_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("grant0", sim.dut->orig_grant0_o, sim.dut->new_grant0_o);
    sim.compare("grant1", sim.dut->orig_grant1_o, sim.dut->new_grant1_o);
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();

    // NUM_CLIENTS=5
    const int NC = 5;
    const int MASK = (1 << NC) - 1;

    dut->bid0_i = 0;
    dut->bid1_i = 0;
    dut->pop0_i = 0;
    dut->pop1_i = 0;
    sim.reset(10);
    compare_outputs(sim);

    // ── Test 1: Single bid on channel 0 ─────────────────────
    printf("cosim test 1: single bid channel 0\n");
    for (int r = 0; r < NC; r++) {
        dut->bid0_i = (1 << r);
        dut->bid1_i = 0;
        dut->pop0_i = 0;
        dut->pop1_i = 0;
        tick_and_compare(sim);
    }

    // ── Test 2: All bids on channel 0, pop0 ─────────────────
    printf("cosim test 2: all bids ch0 with pop0\n");
    sim.reset(10);
    dut->bid0_i = MASK;
    dut->bid1_i = 0;
    dut->pop1_i = 0;
    for (int cycle = 0; cycle < 20; cycle++) {
        dut->pop0_i = 1;
        tick_and_compare(sim);
    }
    dut->pop0_i = 0;
    tick_and_compare(sim);

    // ── Test 3: All bids on channel 1, pop1 ─────────────────
    printf("cosim test 3: all bids ch1 with pop1\n");
    sim.reset(10);
    dut->bid0_i = 0;
    dut->bid1_i = MASK;
    dut->pop0_i = 0;
    for (int cycle = 0; cycle < 20; cycle++) {
        dut->pop1_i = 1;
        tick_and_compare(sim);
    }
    dut->pop1_i = 0;
    tick_and_compare(sim);

    // ── Test 4: Both channels bid, pop1 only ────────────────
    printf("cosim test 4: both channels, pop1 only\n");
    sim.reset(10);
    dut->bid0_i = MASK;
    dut->bid1_i = MASK;
    dut->pop0_i = 0;
    for (int cycle = 0; cycle < 20; cycle++) {
        dut->pop1_i = 1;
        tick_and_compare(sim);
    }
    dut->pop1_i = 0;
    tick_and_compare(sim);

    // ── Test 5: Subset bids, alternating pop ────────────────
    printf("cosim test 5: subset bids, alternating pop\n");
    sim.reset(10);
    dut->bid0_i = 0b10101;
    dut->bid1_i = 0b01010;
    for (int cycle = 0; cycle < 20; cycle++) {
        // Only one pop at a time
        dut->pop0_i = (cycle & 1) ? 1 : 0;
        dut->pop1_i = (cycle & 1) ? 0 : 1;
        tick_and_compare(sim);
    }
    dut->pop0_i = 0;
    dut->pop1_i = 0;
    tick_and_compare(sim);

    // ── Test 6: No pop (LRU should not update) ──────────────
    printf("cosim test 6: no pop\n");
    sim.reset(10);
    dut->bid0_i = MASK;
    dut->bid1_i = MASK;
    dut->pop0_i = 0;
    dut->pop1_i = 0;
    for (int cycle = 0; cycle < 10; cycle++) {
        tick_and_compare(sim);
    }

    // ── Test 7: bid1 is subset of bid0 (priority vs opportunistic) ──
    printf("cosim test 7: bid1 subset of bid0\n");
    sim.reset(10);
    dut->bid0_i = MASK;
    dut->pop0_i = 0;
    for (int cycle = 0; cycle < 30; cycle++) {
        // bid1 is a varying subset
        dut->bid1_i = (cycle % 3 == 0) ? 0b00011 :
                      (cycle % 3 == 1) ? 0b01100 :
                                         0b10001;
        dut->pop1_i = (dut->bid1_i != 0) ? 1 : 0;
        tick_and_compare(sim);
    }
    dut->pop1_i = 0;
    tick_and_compare(sim);

    // ── Test 8: Random stimulus (1000 cycles) ───────────────
    printf("cosim test 8: random stimulus (1000 cycles)\n");
    sim.reset(10);
    uint32_t seed = 0xCAFE2B1D;
    for (int cycle = 0; cycle < 1000; cycle++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        dut->bid0_i = seed & MASK;
        dut->bid1_i = (seed >> 5) & MASK;
        // Ensure only one pop at a time
        bool do_pop0 = ((seed >> 10) & 1) && !((seed >> 11) & 1);
        bool do_pop1 = !((seed >> 10) & 1) && ((seed >> 11) & 1);
        dut->pop0_i = do_pop0 ? 1 : 0;
        dut->pop1_i = do_pop1 ? 1 : 0;
        tick_and_compare(sim);
    }
    dut->bid0_i = 0;
    dut->bid1_i = 0;
    dut->pop0_i = 0;
    dut->pop1_i = 0;
    tick_and_compare(sim);

    return sim.finish();
}
