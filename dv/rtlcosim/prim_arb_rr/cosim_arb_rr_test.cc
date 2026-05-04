// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: arb_rr_data (original) vs prim_arb_rr (new).
// Drives identical stimulus and compares grants + data each cycle.

#include "Vcosim_arb_rr_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>

using DUT = Vcosim_arb_rr_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("grants", sim.dut->orig_grants_o, sim.dut->new_grants_o);
    sim.compare("data",   sim.dut->orig_data_o,   sim.dut->new_data_o);
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();

    // NUM_REQS=4, WIDTH=32

    dut->reqs_i = 0;
    dut->pop_i  = 0;
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0;
    sim.reset(10);
    compare_outputs(sim);

    // ── Test 1: Single requests ─────────────────────────────
    printf("cosim test 1: single requests\n");
    for (int r = 0; r < 4; r++) {
        dut->reqs_i = (1 << r);
        dut->pop_i  = 1;
        for (int i = 0; i < 4; i++) dut->data_i[i] = 0x1000 * (i + 1) + r;
        tick_and_compare(sim);
    }
    dut->pop_i = 0;
    tick_and_compare(sim);

    // ── Test 2: All requests, round-robin with pop ──────────
    printf("cosim test 2: all requests with pop\n");
    sim.reset(10);
    dut->reqs_i = 0xF;
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0xA000 + i;

    for (int cycle = 0; cycle < 16; cycle++) {
        dut->pop_i = 1;
        tick_and_compare(sim);
    }
    dut->pop_i = 0;
    tick_and_compare(sim);

    // ── Test 3: All requests without pop (no rotation) ──────
    printf("cosim test 3: all requests without pop\n");
    sim.reset(10);
    dut->reqs_i = 0xF;
    dut->pop_i  = 0;
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0xB000 + i;

    for (int cycle = 0; cycle < 8; cycle++) {
        tick_and_compare(sim);
    }

    // ── Test 4: Subset of requests ──────────────────────────
    printf("cosim test 4: subset of requests\n");
    sim.reset(10);
    dut->reqs_i = 0b1010;  // reqs 1 and 3
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0xC000 + i;

    for (int cycle = 0; cycle < 12; cycle++) {
        dut->pop_i = 1;
        tick_and_compare(sim);
    }
    dut->pop_i = 0;
    tick_and_compare(sim);

    // ── Test 5: De-assert reqs (data held) ──────────────────
    printf("cosim test 5: de-assert reqs\n");
    // After the previous test, both modules should hold the same data.
    dut->reqs_i = 0;
    dut->pop_i  = 0;
    for (int cycle = 0; cycle < 4; cycle++) {
        tick_and_compare(sim);
    }

    // ── Test 6: Pop with no reqs (pointer should not advance) ─
    printf("cosim test 6: pop with no reqs\n");
    dut->reqs_i = 0;
    dut->pop_i  = 1;
    for (int cycle = 0; cycle < 4; cycle++) {
        tick_and_compare(sim);
    }
    dut->pop_i = 0;

    // Re-assert all and verify both agree.
    dut->reqs_i = 0xF;
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0xD000 + i;
    tick_and_compare(sim);

    // ── Test 7: Alternating reqs ────────────────────────────
    printf("cosim test 7: alternating reqs\n");
    sim.reset(10);
    for (int cycle = 0; cycle < 20; cycle++) {
        dut->reqs_i = (cycle & 1) ? 0b0101 : 0b1010;
        dut->pop_i  = 1;
        for (int i = 0; i < 4; i++) dut->data_i[i] = 0xE000 + cycle * 0x10 + i;
        tick_and_compare(sim);
    }
    dut->pop_i = 0;
    dut->reqs_i = 0;
    tick_and_compare(sim);

    // ── Test 8: Random stimulus (500 cycles) ────────────────
    printf("cosim test 8: random stimulus (500 cycles)\n");
    sim.reset(10);
    uint32_t seed = 0xDEAD1234;
    for (int cycle = 0; cycle < 500; cycle++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        dut->reqs_i = seed & 0xF;
        dut->pop_i  = (seed >> 4) & 1;
        for (int i = 0; i < 4; i++) {
            dut->data_i[i] = seed + (static_cast<uint32_t>(i) * 0x11111111u);
        }
        tick_and_compare(sim);
    }
    dut->reqs_i = 0;
    dut->pop_i  = 0;
    tick_and_compare(sim);

    return sim.finish();
}
