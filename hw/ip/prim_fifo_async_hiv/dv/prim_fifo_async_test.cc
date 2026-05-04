// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_fifo_async — Verilator test.
//
// Note: Verilator is single-threaded and evaluates both clock domains
// synchronously, so this test cannot truly exercise CDC timing. It
// validates the functional gray-code pointer logic and data integrity.

#include "Vprim_fifo_async_hiv.h"
#include "sim_ctrl.h"

using DUT = Vprim_fifo_async_hiv;

// Custom tick: drive both clocks together (Verilator limitation).
static void tick_both(SimCtrl<DUT>& sim) {
    sim.dut->clk_wr_i = 0;
    sim.dut->clk_rd_i = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->clk_wr_i = 1;
    sim.dut->clk_rd_i = 1;
    sim.dut->eval();
    sim.sim_time++;
}

// Reset both domains.
static void reset_both(SimCtrl<DUT>& sim, int cycles = 5) {
    sim.dut->rst_wr_ni = 0;
    sim.dut->rst_rd_ni = 0;
    for (int i = 0; i < cycles; i++) tick_both(sim);
    sim.dut->rst_wr_ni = 1;
    sim.dut->rst_rd_ni = 1;
    // Extra cycles for synchronizers to settle.
    for (int i = 0; i < 4; i++) tick_both(sim);
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    // Default: Width=32, Depth=8, SyncStages=2
    sim.dut->push_i  = 0;
    sim.dut->pop_i   = 0;
    sim.dut->wdata_i = 0;
    sim.dut->dft_hv_i = 0;  // DFT off
    sim.dut->dft_lv_i = 0;  // DFT off
    reset_both(sim);

    // ── Test 1: Empty after reset ───────────────────────
    printf("test 1: empty after reset\n");
    sim.check(sim.dut->valid_o == 0, "not valid (empty)");
    sim.check(sim.dut->ready_o == 1, "ready (not full)");

    // ── Test 2: Push one, wait for sync, pop ────────────
    printf("test 2: push one, pop after sync latency\n");
    sim.dut->push_i  = 1;
    sim.dut->wdata_i = 0xDEAD;
    tick_both(sim);
    sim.dut->push_i = 0;

    // Wait for write pointer to propagate through synchronizer.
    for (int i = 0; i < 3; i++) tick_both(sim);
    sim.check(sim.dut->valid_o == 1, "valid after sync latency");
    sim.check(sim.dut->rdata_o == 0xDEAD, "rdata = 0xDEAD");

    sim.dut->pop_i = 1;
    tick_both(sim);
    sim.dut->pop_i = 0;
    for (int i = 0; i < 3; i++) tick_both(sim);
    sim.check(sim.dut->valid_o == 0, "empty after pop + sync");

    // ── Test 3: Fill and drain ──────────────────────────
    printf("test 3: fill 8 entries, drain 8\n");
    for (int i = 0; i < 8; i++) {
        sim.dut->push_i  = 1;
        sim.dut->wdata_i = 0x100 + i;
        tick_both(sim);
    }
    sim.dut->push_i = 0;
    tick_both(sim);
    sim.check(sim.dut->ready_o == 0, "full after 8 pushes");

    // Wait for sync.
    for (int i = 0; i < 3; i++) tick_both(sim);

    // Read back in order.
    for (int i = 0; i < 8; i++) {
        sim.check(sim.dut->valid_o == 1,
                  "valid[" + std::to_string(i) + "]");
        sim.check(sim.dut->rdata_o == (uint32_t)(0x100 + i),
                  "data[" + std::to_string(i) + "]");
        sim.dut->pop_i = 1;
        tick_both(sim);
    }
    sim.dut->pop_i = 0;
    for (int i = 0; i < 3; i++) tick_both(sim);
    sim.check(sim.dut->valid_o == 0, "empty after drain");
    // Wait for ready to propagate back.
    for (int i = 0; i < 3; i++) tick_both(sim);
    sim.check(sim.dut->ready_o == 1, "ready after drain + sync");

    // ── Test 4: Interleaved push/pop ────────────────────
    printf("test 4: interleaved push/pop\n");
    // Push 4, pop 2, push 3, pop 5.
    for (int i = 0; i < 4; i++) {
        sim.dut->push_i  = 1;
        sim.dut->wdata_i = 0x200 + i;
        tick_both(sim);
    }
    sim.dut->push_i = 0;
    for (int i = 0; i < 3; i++) tick_both(sim);

    for (int i = 0; i < 2; i++) {
        sim.dut->pop_i = 1;
        tick_both(sim);
    }
    sim.dut->pop_i = 0;
    for (int i = 0; i < 3; i++) tick_both(sim);

    for (int i = 0; i < 3; i++) {
        sim.dut->push_i  = 1;
        sim.dut->wdata_i = 0x300 + i;
        tick_both(sim);
    }
    sim.dut->push_i = 0;
    for (int i = 0; i < 3; i++) tick_both(sim);

    // Drain remaining 5.
    for (int i = 0; i < 5; i++) {
        sim.check(sim.dut->valid_o == 1,
                  "interleave valid[" + std::to_string(i) + "]");
        sim.dut->pop_i = 1;
        tick_both(sim);
    }
    sim.dut->pop_i = 0;
    for (int i = 0; i < 3; i++) tick_both(sim);
    sim.check(sim.dut->valid_o == 0, "empty after interleaved drain");

    return sim.finish();
}
