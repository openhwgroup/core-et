// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for prim_arb_prio (priority arbiter with data hold).

#include "Vprim_arb_prio.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <string>

using DUT = Vprim_arb_prio;

// Default parameters: NumClients=2, Width=32
// Verilator flattens data_i[1:0][31:0] into a single 64-bit:
//   bits [31:0]  = data_i[0]
//   bits [63:32] = data_i[1]
static void set_data(DUT* dut, uint32_t d0, uint32_t d1) {
    dut->data_i = (uint64_t(d1) << 32) | d0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();

    // ── Helper: drive inputs and tick ─────────────────────
    auto drive = [&](uint8_t bid, uint8_t stall,
                     uint32_t d0, uint32_t d1) {
        dut->bid_i   = bid;
        dut->stall_i = stall;
        set_data(dut, d0, d1);
        sim.tick();
    };

    // ── Reset ─────────────────────────────────────────────
    dut->bid_i   = 0;
    dut->stall_i = 0;
    dut->data_i  = 0;
    sim.reset();

    // ── Test 1: Reset clears state ────────────────────────
    printf("test 1: reset clears state\n");
    sim.check(dut->grant_o == 0, "grant_o=0 after reset");
    // After reset, prev_winner_q=0 and no grant, so data_o = data_i[0]
    set_data(dut, 0x11111111, 0x22222222);
    dut->eval();
    sim.check(dut->data_o == 0x11111111,
              "data_o holds data_i[0] after reset (prev_winner=0)");

    // ── Test 2: Single bidder — client 0 ──────────────────
    printf("test 2: single bidder (client 0)\n");
    drive(0x1, 0, 0xAAAAAAAA, 0xBBBBBBBB);
    sim.check(dut->grant_o == 0x1, "grant_o=0x1 when only client 0 bids");
    sim.check(dut->data_o == 0xAAAAAAAA, "data_o selects client 0's data");

    // ── Test 3: Single bidder — client 1 ──────────────────
    printf("test 3: single bidder (client 1)\n");
    drive(0x2, 0, 0xAAAAAAAA, 0xBBBBBBBB);
    sim.check(dut->grant_o == 0x2, "grant_o=0x2 when only client 1 bids");
    sim.check(dut->data_o == 0xBBBBBBBB, "data_o selects client 1's data");

    // ── Test 4: Multiple bidders — highest index wins ─────
    printf("test 4: both bid, highest index wins\n");
    drive(0x3, 0, 0x11111111, 0x22222222);
    sim.check(dut->grant_o == 0x2, "grant_o=0x2 when both bid (client 1 wins)");
    sim.check(dut->data_o == 0x22222222, "data_o selects client 1's data");

    // ── Test 5: Stall blocks all grants ───────────────────
    printf("test 5: stall blocks grants\n");
    drive(0x3, 1, 0xCCCCCCCC, 0xDDDDDDDD);
    sim.check(dut->grant_o == 0x0, "grant_o=0 when stalled");

    // ── Test 6: data_o holds previous winner on stall ─────
    printf("test 6: data_o holds previous winner's data when stalled\n");
    // From test 4, client 1 won. Now stall with new data.
    // prev_winner_q should be 1 (from the grant in test 4).
    // Actually test 5 had no grant (stall), so prev_winner_q is still from test 4.
    // Wait — test 5 ticked with stall=1 and bid=0x3, no grant issued.
    // So prev_winner_q is still 1 from test 4.
    // But data_o = data_i[winner_sel], and winner_sel = prev_winner_q = 1
    // Since data_i changes combinationally, data_o = data_i[1] = 0xDDDDDDDD.
    // The "hold" means the index is held, not the data value.
    sim.check(dut->data_o == 0xDDDDDDDD,
              "data_o uses prev winner index (1) -> data_i[1]");

    // ── Test 7: data_o holds winner index when no bids ────
    printf("test 7: data_o holds winner index when no bids\n");
    // First grant client 0 to set prev_winner.
    drive(0x1, 0, 0x44444444, 0x55555555);
    sim.check(dut->grant_o == 0x1, "client 0 granted");
    sim.check(dut->data_o == 0x44444444, "data_o = client 0 data");

    // Now no bids — data_o should use held index (0).
    drive(0x0, 0, 0x66666666, 0x77777777);
    sim.check(dut->grant_o == 0x0, "no grant when no bids");
    sim.check(dut->data_o == 0x66666666,
              "data_o uses held index (0) -> data_i[0]");

    // ── Test 8: Grant client 1, then no bids, holds index 1
    printf("test 8: hold index across idle cycles\n");
    drive(0x2, 0, 0x88888888, 0x99999999);
    sim.check(dut->grant_o == 0x2, "client 1 granted");
    sim.check(dut->data_o == 0x99999999, "data_o = client 1 data");

    // Two idle cycles — held index stays at 1.
    drive(0x0, 0, 0x10101010, 0x20202020);
    sim.check(dut->data_o == 0x20202020, "hold cycle 1: data_o = data_i[1]");
    drive(0x0, 0, 0x30303030, 0x40404040);
    sim.check(dut->data_o == 0x40404040, "hold cycle 2: data_o = data_i[1]");

    // ── Test 9: Stall then release ────────────────────────
    printf("test 9: stall then release\n");
    drive(0x1, 1, 0xF0F0F0F0, 0x0F0F0F0F);
    sim.check(dut->grant_o == 0x0, "stall: no grant");
    drive(0x1, 0, 0xF0F0F0F0, 0x0F0F0F0F);
    sim.check(dut->grant_o == 0x1, "release: client 0 granted");
    sim.check(dut->data_o == 0xF0F0F0F0, "release: data_o = client 0 data");

    // ── Test 10: Reset in the middle of operation ─────────
    printf("test 10: reset clears held state\n");
    // Grant client 1.
    drive(0x2, 0, 0xAAAAAAAA, 0xBBBBBBBB);
    sim.check(dut->grant_o == 0x2, "client 1 granted before reset");

    // Reset.
    dut->bid_i   = 0;
    dut->stall_i = 0;
    set_data(dut, 0xDEADBEEF, 0xCAFEBABE);
    sim.reset();

    sim.check(dut->grant_o == 0x0, "grant_o=0 after mid-op reset");
    // After reset prev_winner_q=0, so data_o = data_i[0].
    sim.check(dut->data_o == 0xDEADBEEF,
              "data_o = data_i[0] after reset (prev_winner cleared to 0)");

    return sim.finish();
}
