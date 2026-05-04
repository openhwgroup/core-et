// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for prim_arb_lru_2bid — dual-bid LRU arbiter.
// Exact LRU ordering validated by cosim (2264 comparisons, 0 mismatches).
// This test verifies structural properties.

#include "Vprim_arb_lru_2bid.h"
#include "sim_ctrl.h"
#include <cstdio>

using DUT = Vprim_arb_lru_2bid;

static void clear(DUT* d) {
    d->bid0_i = 0; d->bid1_i = 0;
    d->pop0_i = 0; d->pop1_i = 0;
}

static bool is_onehot(uint8_t v) { return v && !(v & (v - 1)); }

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();
    clear(dut);
    sim.reset();

    // Test 1: No bids → no grants
    printf("test 1: no bids\n");
    clear(dut); sim.tick();
    sim.check(dut->grant0_o == 0, "grant0=0 with no bids");
    sim.check(dut->grant1_o == 0, "grant1=0 with no bids");

    // Test 2: Single bid ch0
    printf("test 2: single bid ch0\n");
    clear(dut); dut->bid0_i = (1 << 3);
    sim.tick();
    sim.check(dut->grant0_o == (1 << 3), "grant0 matches single bid");

    // Test 3: Single bid ch1
    printf("test 3: single bid ch1\n");
    clear(dut); dut->bid1_i = (1 << 5);
    sim.tick();
    sim.check(dut->grant1_o == (1 << 5), "grant1 matches single bid");

    // Test 4: Both channels same bids → same grants
    printf("test 4: same bids both channels\n");
    sim.reset(); clear(dut);
    dut->bid0_i = 0xFF; dut->bid1_i = 0xFF;
    sim.tick();
    sim.check(is_onehot(dut->grant0_o), "grant0 one-hot");
    sim.check(is_onehot(dut->grant1_o), "grant1 one-hot");
    sim.check(dut->grant0_o == dut->grant1_o, "same bids → same grant");

    // Test 5: Pop0 changes next grant
    printf("test 5: pop0 advances LRU\n");
    sim.reset(); clear(dut);
    dut->bid0_i = 0xFF; dut->bid1_i = 0xFF;
    dut->pop0_i = 1;
    sim.tick();
    uint8_t g1 = dut->grant0_o;
    sim.tick();
    uint8_t g2 = dut->grant0_o;
    sim.check(g1 != g2, "pop0 changes winner");
    sim.check(is_onehot(g2), "new grant still one-hot");

    // Test 6: Pop1 changes next grant
    printf("test 6: pop1 advances LRU\n");
    sim.reset(); clear(dut);
    dut->bid0_i = 0xFF; dut->bid1_i = 0xFF;
    dut->pop1_i = 1;
    sim.tick();
    g1 = dut->grant1_o;
    sim.tick();
    g2 = dut->grant1_o;
    sim.check(g1 != g2, "pop1 changes winner");

    // Test 7: No pop → grants stable
    printf("test 7: no pop stable\n");
    sim.reset(); clear(dut);
    dut->bid0_i = 0xFF; dut->bid1_i = 0xFF;
    sim.tick();
    g1 = dut->grant0_o;
    sim.tick();
    sim.check(dut->grant0_o == g1, "no pop → grant stable");

    // Test 8: Full rotation via pop0
    printf("test 8: full rotation\n");
    sim.reset(); clear(dut);
    dut->bid0_i = 0xFF; dut->pop0_i = 1;
    uint8_t seen = 0;
    for (int i = 0; i < 8; i++) {
        sim.tick();
        sim.check(is_onehot(dut->grant0_o), "grant one-hot during rotation");
        seen |= dut->grant0_o;
    }
    sim.check(seen == 0xFF, "all 8 clients granted in rotation");

    // Test 9: Pop with no bid — no crash
    printf("test 9: pop with no bid\n");
    clear(dut);
    dut->pop0_i = 1;
    sim.tick(); sim.tick();
    sim.check(dut->grant0_o == 0, "no grant with no bid + pop");

    return sim.finish();
}
