// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for prim_arb_rr (round-robin arbiter with data mux).

#include "Vprim_arb_rr.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <string>

using DUT = Vprim_arb_rr;

// Default parameters: NumReqs=4, Width=32
// Ports: clk_i, rst_ni, reqs_i[3:0], pop_i, grants_o[3:0],
//        data_i[3:0][31:0] (Verilator: data_i[0..3]), data_o[31:0]

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();

    dut->reqs_i = 0;
    dut->pop_i  = 0;
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0;
    sim.reset();

    // ── Test 1: No requests → grants_o = 0 ─────────────────
    printf("test 1: no requests\n");
    dut->reqs_i = 0;
    dut->pop_i  = 0;
    sim.tick();
    sim.check(dut->grants_o == 0, "grants_o = 0 when no reqs");

    // ── Test 2: Single request → grants that request ────────
    printf("test 2: single request\n");
    for (int r = 0; r < 4; r++) {
        dut->reqs_i = (1 << r);
        dut->pop_i  = 0;
        for (int i = 0; i < 4; i++) dut->data_i[i] = 0x100 * (i + 1);
        sim.tick();
        sim.check(dut->grants_o == (1 << r),
                  "single req[" + std::to_string(r) + "] granted");
        sim.check(dut->data_o == (uint32_t)(0x100 * (r + 1)),
                  "data_o matches req[" + std::to_string(r) + "]");
    }

    // ── Test 3: Reset clears RR state ───────────────────────
    printf("test 3: reset clears RR state\n");
    // Advance the RR pointer so req 0 is not highest priority.
    dut->reqs_i = 0xF;  // all four
    dut->pop_i  = 1;
    sim.tick();  // grants req 0, advances mask
    sim.tick();  // grants req 1, advances mask
    dut->reqs_i = 0;
    dut->pop_i  = 0;
    sim.tick();

    // Reset and verify req 0 wins again (mask = 0 → lowest index wins).
    sim.reset();
    dut->reqs_i = 0xF;
    dut->pop_i  = 0;
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0xA0 + i;
    sim.tick();
    sim.check(dut->grants_o == 0x1, "after reset, req 0 wins");
    sim.check(dut->data_o == 0xA0, "data_o = data_i[0] after reset");

    // ── Test 4: pop_i advances the RR pointer ───────────────
    printf("test 4: pop advances RR pointer\n");
    // All four request. After reset, req 0 wins.
    dut->reqs_i = 0xF;
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0x10 * (i + 1);

    // Without pop, same winner each cycle.
    dut->pop_i = 0;
    sim.tick();
    uint8_t g1 = dut->grants_o;
    sim.tick();
    sim.check(dut->grants_o == g1, "no pop → same grant");

    // With pop, winner rotates.
    uint8_t seen = 0;
    for (int cycle = 0; cycle < 4; cycle++) {
        dut->pop_i = 1;
        sim.tick();
        uint8_t g = dut->grants_o;
        // grant should be one-hot
        sim.check(g != 0 && (g & (g - 1)) == 0,
                  "grant is one-hot, cycle " + std::to_string(cycle));
        seen |= g;
    }
    sim.check(seen == 0xF, "all 4 requestors granted over 4 pops");
    dut->pop_i = 0;

    // ── Test 5: Round-robin fairness with subset of reqs ────
    printf("test 5: round-robin fairness (subset)\n");
    sim.reset();
    dut->reqs_i = 0b1010;  // reqs 1 and 3
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0xD0 + i;

    int count[4] = {};
    for (int cycle = 0; cycle < 8; cycle++) {
        dut->pop_i = 1;
        sim.tick();
        uint8_t g = dut->grants_o;
        for (int b = 0; b < 4; b++) {
            if (g & (1 << b)) count[b]++;
        }
    }
    sim.check(count[0] == 0, "req 0 not granted (not requesting)");
    sim.check(count[2] == 0, "req 2 not granted (not requesting)");
    sim.check(count[1] == 4, "req 1 granted 4 times");
    sim.check(count[3] == 4, "req 3 granted 4 times");
    dut->pop_i = 0;

    // ── Test 6: Data mux selects winner's data ──────────────
    printf("test 6: data mux\n");
    sim.reset();
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0xCAFE0000 + i;

    // Request each one individually and verify data_o.
    for (int r = 0; r < 4; r++) {
        dut->reqs_i = (1 << r);
        dut->pop_i  = 1;
        sim.tick();
        sim.check(dut->data_o == (uint32_t)(0xCAFE0000 + r),
                  "data mux selects req " + std::to_string(r));
    }
    dut->pop_i = 0;

    // ── Test 7: Data held when no reqs ──────────────────────
    printf("test 7: data held when no reqs\n");
    // Last winner was req 3. De-assert all reqs.
    dut->reqs_i = 0;
    sim.tick();
    sim.check(dut->grants_o == 0, "no grants when no reqs");
    sim.check(dut->data_o == (uint32_t)(0xCAFE0003),
              "data_o holds last winner's data");

    // Change data_i[3] and check data_o tracks it (since grants_held
    // still points at req 3).
    dut->data_i[3] = 0xBEEF0003;
    sim.tick();
    sim.check(dut->data_o == (uint32_t)(0xBEEF0003),
              "data_o tracks held winner's data_i");

    // ── Test 8: pop_i ignored when no reqs ──────────────────
    printf("test 8: pop ignored when no reqs\n");
    dut->reqs_i = 0;
    dut->pop_i  = 1;
    sim.tick();
    sim.tick();
    dut->pop_i = 0;
    // Re-assert all reqs: the RR pointer should not have advanced.
    dut->reqs_i = 0xF;
    for (int i = 0; i < 4; i++) dut->data_i[i] = 0x50 + i;
    sim.tick();
    // The last winner before de-assertion was req 3, and mask should
    // still point past req 3 (wrapping). So req 0 should win.
    sim.check(dut->grants_o == 0x1,
              "RR pointer unchanged by pop with no reqs");

    return sim.finish();
}
