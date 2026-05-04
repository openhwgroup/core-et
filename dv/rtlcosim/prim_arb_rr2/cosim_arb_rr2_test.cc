// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: arb_rr2 (original) vs prim_arb_rr2 (new).
// Drives identical stimulus and compares all outputs each cycle.

#include "Vcosim_arb_rr2_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>

using DUT = Vcosim_arb_rr2_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* dut = sim.dut.get();
    sim.compare("valid",      dut->orig_valid_o,      dut->new_valid_o);
    sim.compare("valid2",     dut->orig_valid2_o,     dut->new_valid2_o);
    sim.compare("grants",     dut->orig_grants_o,     dut->new_grants_o);
    sim.compare("grants2",    dut->orig_grants2_o,    dut->new_grants2_o);
    sim.compare("grants_id",  dut->orig_grants_id_o,  dut->new_grants_id_o);
    sim.compare("grants2_id", dut->orig_grants2_id_o, dut->new_grants2_id_o);
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000000;
    auto* dut = sim.dut.get();

    // NUM_REQS=64

    dut->reqs_i   = 0;
    dut->alloc_i  = 0;
    dut->esr_wr_i = 0;
    sim.reset(10);
    compare_outputs(sim);

    // ── Test 1: Reset + idle ────────────────────────────────
    printf("cosim test 1: reset + idle\n");
    for (int i = 0; i < 5; i++) {
        tick_and_compare(sim);
    }

    // ── Test 2: Sequential alloc through all 64 entries ─────
    printf("cosim test 2: sequential alloc through 64\n");
    sim.reset(10);
    dut->reqs_i   = ~0ULL;  // all 64 free
    dut->alloc_i  = 0;
    dut->esr_wr_i = 0;
    tick_and_compare(sim);

    for (int i = 0; i < 64; i++) {
        dut->alloc_i = 1;
        tick_and_compare(sim);
    }
    dut->alloc_i = 0;
    tick_and_compare(sim);

    // ── Test 3: Random reqs pattern (200 cycles) ────────────
    printf("cosim test 3: random reqs (200 cycles)\n");
    sim.reset(10);
    uint64_t seed = 0xCAFEBABE12345678ULL;
    for (int cycle = 0; cycle < 200; cycle++) {
        // xorshift64
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;

        dut->reqs_i   = seed;
        dut->alloc_i  = (seed >> 3) & 1;
        dut->esr_wr_i = 0;
        tick_and_compare(sim);
    }
    dut->reqs_i   = 0;
    dut->alloc_i  = 0;
    tick_and_compare(sim);

    // ── Test 4: Rapid alloc/dealloc ─────────────────────────
    printf("cosim test 4: rapid alloc/dealloc\n");
    sim.reset(10);
    seed = 0xDEADBEEF01020304ULL;
    uint64_t reqs = ~0ULL;
    for (int cycle = 0; cycle < 150; cycle++) {
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;

        // Randomly flip some bits (simulating alloc/dealloc)
        uint64_t flip = seed & (seed >> 32);  // sparse flips
        reqs ^= flip;
        dut->reqs_i   = reqs;
        dut->alloc_i  = (cycle & 1);  // alloc every other cycle
        dut->esr_wr_i = 0;
        tick_and_compare(sim);
    }
    dut->alloc_i = 0;
    tick_and_compare(sim);

    // ── Test 5: Edge cases: 0 reqs, 1 req, 2 reqs, all reqs ─
    printf("cosim test 5: edge cases\n");
    sim.reset(10);

    // 0 reqs
    dut->reqs_i   = 0;
    dut->alloc_i  = 0;
    dut->esr_wr_i = 0;
    tick_and_compare(sim);
    tick_and_compare(sim);

    // 1 req (bit 42)
    dut->reqs_i = (1ULL << 42);
    tick_and_compare(sim);
    dut->alloc_i = 1;
    tick_and_compare(sim);
    dut->alloc_i = 0;
    tick_and_compare(sim);

    // 2 reqs (bits 0 and 63)
    dut->reqs_i = (1ULL << 0) | (1ULL << 63);
    tick_and_compare(sim);
    dut->alloc_i = 1;
    tick_and_compare(sim);
    tick_and_compare(sim);
    dut->alloc_i = 0;
    tick_and_compare(sim);

    // All reqs
    dut->reqs_i = ~0ULL;
    tick_and_compare(sim);
    for (int i = 0; i < 10; i++) {
        dut->alloc_i = 1;
        tick_and_compare(sim);
    }
    dut->alloc_i = 0;
    tick_and_compare(sim);

    // ── Test 6: ESR write during operation ──────────────────
    printf("cosim test 6: esr_wr during operation\n");
    sim.reset(10);
    dut->reqs_i   = 0x00FF00FF00FF00FFULL;
    dut->alloc_i  = 0;
    dut->esr_wr_i = 0;
    tick_and_compare(sim);

    // A few alloc cycles
    for (int i = 0; i < 5; i++) {
        dut->alloc_i = 1;
        tick_and_compare(sim);
    }
    dut->alloc_i = 0;

    // Change reqs and pulse esr_wr
    dut->reqs_i   = 0xFF00FF00FF00FF00ULL;
    dut->esr_wr_i = 1;
    tick_and_compare(sim);
    dut->esr_wr_i = 0;
    tick_and_compare(sim);

    // Continue with alloc
    for (int i = 0; i < 10; i++) {
        dut->alloc_i = 1;
        tick_and_compare(sim);
    }
    dut->alloc_i = 0;
    tick_and_compare(sim);

    // ── Test 7: Alternating patterns ────────────────────────
    printf("cosim test 7: alternating patterns\n");
    sim.reset(10);
    for (int cycle = 0; cycle < 40; cycle++) {
        dut->reqs_i   = (cycle & 1) ? 0xAAAAAAAAAAAAAAAAULL : 0x5555555555555555ULL;
        dut->alloc_i  = 1;
        dut->esr_wr_i = 0;
        tick_and_compare(sim);
    }
    dut->alloc_i = 0;
    dut->reqs_i  = 0;
    tick_and_compare(sim);

    // ── Test 8: Extended random (500 cycles) ────────────────
    printf("cosim test 8: extended random (500 cycles)\n");
    sim.reset(10);
    seed = 0x0123456789ABCDEFULL;
    for (int cycle = 0; cycle < 500; cycle++) {
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;

        dut->reqs_i   = seed;
        dut->alloc_i  = (seed >> 5) & 1;
        dut->esr_wr_i = ((seed >> 10) & 0xF) == 0;  // occasional esr_wr
        tick_and_compare(sim);
    }
    dut->reqs_i   = 0;
    dut->alloc_i  = 0;
    dut->esr_wr_i = 0;
    tick_and_compare(sim);

    return sim.finish();
}
