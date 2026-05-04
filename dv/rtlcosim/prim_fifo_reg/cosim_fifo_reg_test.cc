// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: gen_fifo_reg (original) vs prim_fifo_reg (new).

#include "Vcosim_fifo_reg_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_fifo_reg_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("full",  sim.dut->orig_full_o,  sim.dut->new_full_o);
    sim.compare("valid", sim.dut->orig_valid_o, sim.dut->new_valid_o);
    sim.compare("level", sim.dut->orig_level_o, sim.dut->new_level_o);
    if (sim.dut->orig_valid_o || sim.dut->new_valid_o) {
        sim.compare("rdata", sim.dut->orig_rdata_o, sim.dut->new_rdata_o);
    }
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    // Depth=4, Width=8
    const int DEPTH = 4;

    sim.dut->push_i  = 0;
    sim.dut->pop_i   = 0;
    sim.dut->wdata_i = 0;
    sim.reset(10);
    compare_outputs(sim);

    // ── Test 1: Push and pop single items ───────────────
    printf("cosim test 1: single push/pop\n");
    sim.dut->push_i  = 1;
    sim.dut->wdata_i = 0xAA;
    tick_and_compare(sim);
    sim.dut->push_i = 0;
    tick_and_compare(sim);

    sim.dut->pop_i = 1;
    tick_and_compare(sim);
    sim.dut->pop_i = 0;
    tick_and_compare(sim);

    // ── Test 2: Fill to capacity ────────────────────────
    printf("cosim test 2: fill to capacity (%d)\n", DEPTH);
    for (int i = 0; i < DEPTH; i++) {
        sim.dut->push_i  = 1;
        sim.dut->wdata_i = 0x10 + i;
        tick_and_compare(sim);
    }
    sim.dut->push_i = 0;
    tick_and_compare(sim);

    // ── Test 3: Drain in order ──────────────────────────
    printf("cosim test 3: drain in order\n");
    for (int i = 0; i < DEPTH; i++) {
        sim.dut->pop_i = 1;
        tick_and_compare(sim);
    }
    sim.dut->pop_i = 0;
    tick_and_compare(sim);

    // ── Test 4: Simultaneous push+pop at various levels ─
    printf("cosim test 4: simultaneous push+pop\n");
    // Fill 2 items.
    for (int i = 0; i < 2; i++) {
        sim.dut->push_i  = 1;
        sim.dut->wdata_i = 0xB0 + i;
        tick_and_compare(sim);
    }
    sim.dut->push_i = 0;
    tick_and_compare(sim);

    // Simultaneous push+pop for 8 cycles.
    for (int i = 0; i < 8; i++) {
        sim.dut->push_i  = 1;
        sim.dut->wdata_i = 0xC0 + i;
        sim.dut->pop_i   = 1;
        tick_and_compare(sim);
    }
    sim.dut->push_i = 0;
    sim.dut->pop_i  = 0;
    tick_and_compare(sim);

    // Drain remaining.
    for (int i = 0; i < DEPTH; i++) {
        if (!sim.dut->new_valid_o) break;
        sim.dut->pop_i = 1;
        tick_and_compare(sim);
    }
    sim.dut->pop_i = 0;
    tick_and_compare(sim);

    // ── Test 5: Push to empty (bypass path) ─────────────
    printf("cosim test 5: bypass path (push to empty)\n");
    sim.dut->push_i  = 1;
    sim.dut->wdata_i = 0xDD;
    tick_and_compare(sim);
    sim.dut->push_i = 0;
    tick_and_compare(sim);

    // Pop + push simultaneously when only output register occupied.
    sim.dut->push_i  = 1;
    sim.dut->wdata_i = 0xEE;
    sim.dut->pop_i   = 1;
    tick_and_compare(sim);
    sim.dut->push_i = 0;
    sim.dut->pop_i  = 0;
    tick_and_compare(sim);

    // Drain.
    sim.dut->pop_i = 1;
    tick_and_compare(sim);
    sim.dut->pop_i = 0;
    tick_and_compare(sim);

    // ── Test 6: Random stimulus (500 cycles) ────────────
    printf("cosim test 6: random stimulus (500 cycles)\n");
    uint32_t seed = 0xABCD1234;
    int fill = 0;
    for (int i = 0; i < 500; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        int want_push = (seed >> 0) & 1;
        int want_pop  = (seed >> 1) & 1;

        int do_push = want_push && (fill < DEPTH);
        int do_pop  = want_pop  && (fill > 0);

        sim.dut->push_i  = do_push;
        sim.dut->wdata_i = seed & 0xFF;
        sim.dut->pop_i   = do_pop;
        tick_and_compare(sim);

        fill += do_push - do_pop;
    }
    sim.dut->push_i = 0;
    sim.dut->pop_i  = 0;
    tick_and_compare(sim);

    return sim.finish();
}
