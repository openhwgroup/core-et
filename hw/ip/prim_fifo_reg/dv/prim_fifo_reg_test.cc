// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_fifo_reg — Verilator test.

#include "Vprim_fifo_reg.h"
#include "sim_ctrl.h"

using DUT = Vprim_fifo_reg;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->push_i = 0;
    sim.dut->pop_i  = 0;
    sim.dut->wdata_i = 0;
    sim.reset();

    // Default parameters: Width=8, Depth=4

    // ── Test 1: Empty after reset ───────────────────────
    printf("test 1: empty after reset\n");
    sim.check(sim.dut->valid_o == 0, "not valid when empty");
    sim.check(sim.dut->full_o  == 0, "not full when empty");
    sim.check(sim.dut->level_o == 1, "level[0]=1 (empty)");

    // ── Test 2: Push one item ───────────────────────────
    printf("test 2: push one item\n");
    sim.dut->push_i  = 1;
    sim.dut->wdata_i = 0xAA;
    sim.tick();
    sim.dut->push_i = 0;
    sim.check(sim.dut->valid_o == 1, "valid after push");
    sim.check(sim.dut->full_o  == 0, "not full after 1 push");
    sim.check(sim.dut->rdata_o == 0xAA, "rdata = 0xAA");
    sim.tick();

    // ── Test 3: Pop one item ────────────────────────────
    printf("test 3: pop one item\n");
    sim.dut->pop_i = 1;
    sim.tick();
    sim.dut->pop_i = 0;
    sim.check(sim.dut->valid_o == 0, "empty after pop");
    sim.tick();

    // ── Test 4: Fill to capacity (4) ────────────────────
    printf("test 4: fill to capacity\n");
    for (int i = 0; i < 4; i++) {
        sim.dut->push_i  = 1;
        sim.dut->wdata_i = 0x10 + i;
        sim.tick();
    }
    sim.dut->push_i = 0;
    sim.check(sim.dut->full_o  == 1, "full after 4 pushes");
    sim.check(sim.dut->valid_o == 1, "valid when full");
    sim.check(sim.dut->rdata_o == 0x10, "head = first pushed (0x10)");
    sim.tick();

    // ── Test 5: FIFO ordering ───────────────────────────
    printf("test 5: FIFO ordering\n");
    for (int i = 0; i < 4; i++) {
        sim.check(sim.dut->rdata_o == (uint8_t)(0x10 + i),
                  "data[" + std::to_string(i) + "] correct");
        sim.dut->pop_i = 1;
        sim.tick();
    }
    sim.dut->pop_i = 0;
    sim.check(sim.dut->valid_o == 0, "empty after draining");
    sim.tick();

    // ── Test 6: Push and pop same cycle ─────────────────
    printf("test 6: simultaneous push+pop\n");
    // Fill with 2 items first.
    for (int i = 0; i < 2; i++) {
        sim.dut->push_i  = 1;
        sim.dut->wdata_i = 0xC0 + i;
        sim.tick();
    }
    sim.dut->push_i = 0;
    sim.tick();

    // Push new item while popping.
    sim.dut->push_i  = 1;
    sim.dut->wdata_i = 0xDD;
    sim.dut->pop_i   = 1;
    sim.tick();
    sim.dut->push_i = 0;
    sim.dut->pop_i  = 0;
    // Should have popped 0xC0, pushed 0xDD. Head should now be 0xC1.
    sim.check(sim.dut->rdata_o == 0xC1, "head = 0xC1 after simul push+pop");
    sim.tick();

    // Drain remaining: 0xC1, 0xDD.
    sim.dut->pop_i = 1;
    sim.tick();
    sim.check(sim.dut->rdata_o == 0xDD, "next = 0xDD");
    sim.tick();
    sim.dut->pop_i = 0;
    sim.check(sim.dut->valid_o == 0, "empty after drain");
    sim.tick();

    // ── Test 7: Push directly to output (bypass) ────────
    printf("test 7: push to empty FIFO (direct bypass)\n");
    sim.dut->push_i  = 1;
    sim.dut->wdata_i = 0xFF;
    sim.tick();
    sim.dut->push_i = 0;
    sim.check(sim.dut->valid_o == 1, "valid immediately");
    sim.check(sim.dut->rdata_o == 0xFF, "data bypassed to output");
    // Pop it.
    sim.dut->pop_i = 1;
    sim.tick();
    sim.dut->pop_i = 0;
    sim.check(sim.dut->valid_o == 0, "empty again");
    sim.tick();

    // ── Test 8: Push while popping from empty ───────────
    printf("test 8: push while popping last item (mem empty bypass)\n");
    // Push one item.
    sim.dut->push_i  = 1;
    sim.dut->wdata_i = 0xE0;
    sim.tick();
    sim.dut->push_i = 0;
    sim.tick();

    // Simultaneous push+pop when only output register has data.
    sim.dut->push_i  = 1;
    sim.dut->wdata_i = 0xE1;
    sim.dut->pop_i   = 1;
    sim.tick();
    sim.dut->push_i = 0;
    sim.dut->pop_i  = 0;
    // Should have popped 0xE0, pushed 0xE1 directly to output.
    sim.check(sim.dut->valid_o == 1, "still valid (bypass)");
    sim.check(sim.dut->rdata_o == 0xE1, "data = 0xE1 (bypassed)");
    sim.dut->pop_i = 1;
    sim.tick();
    sim.dut->pop_i = 0;
    sim.check(sim.dut->valid_o == 0, "empty");
    sim.tick();

    return sim.finish();
}
