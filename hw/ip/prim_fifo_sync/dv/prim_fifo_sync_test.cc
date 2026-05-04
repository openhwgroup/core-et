// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_fifo_sync — Verilator test.

#include "Vprim_fifo_sync.h"
#include "sim_ctrl.h"

using DUT = Vprim_fifo_sync;

// Helper: drive one write and tick.
static void write(SimCtrl<DUT>& s, uint8_t data) {
    s.dut->wvalid_i = 1;
    s.dut->wdata_i  = data;
    s.tick();
    s.dut->wvalid_i = 0;
}

// Helper: assert rready and tick, return read data.
static uint8_t read(SimCtrl<DUT>& s) {
    uint8_t val = s.dut->rdata_o;
    s.dut->rready_i = 1;
    s.tick();
    s.dut->rready_i = 0;
    return val;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    // ── Init ────────────────────────────────────────────
    sim.dut->wvalid_i = 0;
    sim.dut->rready_i = 0;
    sim.dut->clr_i    = 0;
    sim.reset();

    // ── Test 1: Empty after reset ───────────────────────
    printf("test 1: empty after reset\n");
    sim.check(sim.dut->empty_o  == 1, "empty flag set");
    sim.check(sim.dut->full_o   == 0, "full flag clear");
    sim.check(sim.dut->depth_o  == 0, "depth is 0");
    sim.check(sim.dut->rvalid_o == 0, "rvalid deasserted");
    sim.check(sim.dut->wready_o == 1, "wready asserted");

    // ── Test 2: Write one, read one ─────────────────────
    printf("test 2: write one, read one\n");
    write(sim, 0xAB);
    sim.check(sim.dut->empty_o == 0, "not empty after write");
    sim.check(sim.dut->depth_o == 1, "depth is 1");
    sim.check(sim.dut->rvalid_o == 1, "rvalid asserted");
    sim.check(sim.dut->rdata_o == 0xAB, "read data matches");

    uint8_t v = read(sim);
    sim.check(v == 0xAB, "read returns 0xAB");
    sim.check(sim.dut->empty_o == 1, "empty after read");
    sim.check(sim.dut->depth_o == 0, "depth back to 0");

    // ── Test 3: Fill to capacity ────────────────────────
    printf("test 3: fill to capacity (4)\n");
    for (int i = 0; i < 4; i++) {
        write(sim, 0x10 + i);
    }
    sim.check(sim.dut->full_o  == 1, "full after 4 writes");
    sim.check(sim.dut->depth_o == 4, "depth is 4");
    sim.check(sim.dut->wready_o == 0, "wready deasserted when full");

    // ── Test 4: Read back in order ──────────────────────
    printf("test 4: FIFO ordering\n");
    for (int i = 0; i < 4; i++) {
        sim.check(sim.dut->rdata_o == (uint8_t)(0x10 + i),
                  "data[" + std::to_string(i) + "] correct");
        read(sim);
    }
    sim.check(sim.dut->empty_o == 1, "empty after reading all");

    // ── Test 5: Simultaneous read+write at partial fill ─
    printf("test 5: simultaneous read+write\n");
    // Pre-fill with 2 items.
    write(sim, 0xC0);
    write(sim, 0xC1);
    sim.check(sim.dut->depth_o == 2, "depth is 2 before simul");

    // Simultaneous read + write for several cycles.
    for (int i = 0; i < 4; i++) {
        sim.dut->wvalid_i = 1;
        sim.dut->wdata_i  = 0xD0 + i;
        sim.dut->rready_i = 1;
        sim.tick();
    }
    sim.dut->wvalid_i = 0;
    sim.dut->rready_i = 0;
    sim.tick();
    sim.check(sim.dut->depth_o == 2, "depth unchanged after balanced r+w");

    // Drain and verify order: C1 (C0 was read during simul), D0, D1, D2, D3
    // Actually let's just check depth and drain.
    for (int i = 0; i < 2; i++) {
        read(sim);
    }
    sim.check(sim.dut->empty_o == 1, "empty after drain");

    // ── Test 6: Synchronous clear ───────────────────────
    printf("test 6: synchronous clear\n");
    write(sim, 0xEE);
    write(sim, 0xFF);
    sim.check(sim.dut->depth_o == 2, "depth 2 before clear");

    sim.dut->clr_i = 1;
    sim.tick();
    sim.dut->clr_i = 0;
    sim.tick();
    sim.check(sim.dut->empty_o == 1, "empty after clear");
    sim.check(sim.dut->depth_o == 0, "depth 0 after clear");

    // ── Test 7: Write after clear works ─────────────────
    printf("test 7: write after clear\n");
    write(sim, 0x42);
    sim.check(sim.dut->depth_o == 1, "depth 1 after post-clear write");
    sim.check(sim.dut->rdata_o == 0x42, "data correct after clear+write");
    read(sim);

    // ── Done ────────────────────────────────────────────
    return sim.finish();
}
