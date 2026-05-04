// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: arb_prio_data_held (original) vs prim_arb_prio (new).

#include "Vcosim_arb_prio_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_arb_prio_tb;

// Verilator flattens data_i[1:0][31:0] into 64 bits: [31:0]=d0, [63:32]=d1
static void set_data(DUT* d, uint32_t d0, uint32_t d1) {
    d->data_i = (uint64_t(d1) << 32) | d0;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("grant",  sim.dut->orig_grant_o, sim.dut->new_grant_o);
    sim.compare("data_o", sim.dut->orig_data_o,  sim.dut->new_data_o);
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();

    // ── Reset ─────────────────────────────────────────────
    d->bid_i   = 0;
    d->stall_i = 0;
    d->data_i  = 0;
    sim.reset(10);
    compare_outputs(sim);

    // ── Test 1: Single bidder — client 0 ──────────────────
    printf("cosim test 1: single bidder (client 0)\n");
    d->bid_i   = 0x1;
    d->stall_i = 0;
    set_data(d, 0xAAAAAAAA, 0xBBBBBBBB);
    tick_and_compare(sim);

    // ── Test 2: Single bidder — client 1 ──────────────────
    printf("cosim test 2: single bidder (client 1)\n");
    d->bid_i   = 0x2;
    set_data(d, 0x11111111, 0x22222222);
    tick_and_compare(sim);

    // ── Test 3: Both bid — highest wins ───────────────────
    printf("cosim test 3: both bid, highest wins\n");
    d->bid_i   = 0x3;
    set_data(d, 0xDEADBEEF, 0xCAFEBABE);
    tick_and_compare(sim);

    // ── Test 4: Stall blocks grants ───────────────────────
    printf("cosim test 4: stall blocks grants\n");
    d->bid_i   = 0x3;
    d->stall_i = 1;
    set_data(d, 0x12345678, 0x9ABCDEF0);
    tick_and_compare(sim);

    // ── Test 5: Release stall ─────────────────────────────
    printf("cosim test 5: release stall\n");
    d->stall_i = 0;
    tick_and_compare(sim);

    // ── Test 6: No bids — hold previous winner ────────────
    printf("cosim test 6: no bids, output hold\n");
    d->bid_i = 0x0;
    set_data(d, 0xF0F0F0F0, 0x0F0F0F0F);
    tick_and_compare(sim);

    // ── Test 7: Alternating winners ───────────────────────
    printf("cosim test 7: alternating winners\n");
    for (int i = 0; i < 10; i++) {
        uint8_t bid = (i & 1) ? 0x1 : 0x2;
        d->bid_i   = bid;
        d->stall_i = 0;
        set_data(d, 0x10000000 + i, 0x20000000 + i);
        tick_and_compare(sim);
    }

    // ── Test 8: Stall interleaved with grants ─────────────
    printf("cosim test 8: stall interleaved\n");
    for (int i = 0; i < 10; i++) {
        d->bid_i   = 0x3;
        d->stall_i = (i & 1);
        set_data(d, 0xA0000000 + i, 0xB0000000 + i);
        tick_and_compare(sim);
    }
    d->stall_i = 0;

    // ── Test 9: Grant then idle — hold persists ───────────
    printf("cosim test 9: grant then idle hold\n");
    d->bid_i = 0x1;
    set_data(d, 0x44444444, 0x55555555);
    tick_and_compare(sim);

    for (int i = 0; i < 5; i++) {
        d->bid_i = 0x0;
        set_data(d, 0x60000000 + i, 0x70000000 + i);
        tick_and_compare(sim);
    }

    // ── Test 10: Random stimulus (500 cycles) ─────────────
    printf("cosim test 10: random stimulus (500 cycles)\n");
    uint32_t seed = 0xBEEF1234;
    for (int i = 0; i < 500; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        d->bid_i   = seed & 0x3;
        d->stall_i = (seed >> 2) & 1;
        set_data(d, seed, ~seed);
        tick_and_compare(sim);
    }
    d->bid_i   = 0;
    d->stall_i = 0;
    tick_and_compare(sim);

    return sim.finish();
}
