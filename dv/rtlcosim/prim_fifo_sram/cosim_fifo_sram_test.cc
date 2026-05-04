// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: rbox_fifo (original) vs prim_fifo_sram (new).
// Drives identical stimulus to both, compares outputs every cycle.
//
// IMPORTANT: All tests respect FIFO occupancy (no overflow/underflow).
// Both implementations have undefined behavior for same-address
// simultaneous read+write (gen_mem2p produces 'x, ours reads old value),
// so we never exercise that condition.

#include "Vcosim_fifo_sram_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_fifo_sram_tb;

static const int DEPTH = 64;
static int fill = 0;  // tracked fill level

// Compare all outputs between original and new.
static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("wr_done",  sim.dut->orig_wr_done_o,  sim.dut->new_wr_done_o);
    sim.compare("rd_valid", sim.dut->orig_rd_valid_o,  sim.dut->new_rd_valid_o);
    if (sim.dut->orig_rd_valid_o || sim.dut->new_rd_valid_o) {
        sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);
    }
}

// Tick and compare.
static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

// Safe write: only if not full.
static void safe_write(CosimCtrl<DUT>& sim, uint64_t data) {
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_data_i = data;
    sim.dut->rd_en_i   = 0;
    tick_and_compare(sim);
    sim.dut->wr_en_i = 0;
    fill++;
}

// Safe read: only if not empty. Returns after valid cycle.
static void safe_read(CosimCtrl<DUT>& sim) {
    sim.dut->rd_en_i = 1;
    sim.dut->wr_en_i = 0;
    tick_and_compare(sim);
    sim.dut->rd_en_i = 0;
    tick_and_compare(sim);  // data valid this cycle
    fill--;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->wr_en_i   = 0;
    sim.dut->wr_data_i = 0;
    sim.dut->rd_en_i   = 0;
    sim.reset(10);
    compare_outputs(sim);
    fill = 0;

    // ── Test 1: Single write, single read ───────────────
    printf("cosim test 1: single write/read\n");
    safe_write(sim, 0xDEADBEEFULL);
    tick_and_compare(sim);
    safe_read(sim);
    tick_and_compare(sim);

    // ── Test 2: Fill and drain ──────────────────────────
    printf("cosim test 2: fill 8 entries, drain 8\n");
    for (int i = 0; i < 8; i++) {
        safe_write(sim, 0x1000 + i);
    }
    tick_and_compare(sim);
    for (int i = 0; i < 8; i++) {
        safe_read(sim);
    }
    tick_and_compare(sim);

    // ── Test 3: Back-to-back pipelined reads ────────────
    printf("cosim test 3: pipelined reads\n");
    for (int i = 0; i < 6; i++) {
        safe_write(sim, 0xA000 + i);
    }
    tick_and_compare(sim);

    // Pipelined: hold rd_en high for consecutive reads.
    sim.dut->rd_en_i = 1;
    sim.dut->wr_en_i = 0;
    for (int i = 0; i < 6; i++) {
        tick_and_compare(sim);
    }
    sim.dut->rd_en_i = 0;
    tick_and_compare(sim);  // last valid
    tick_and_compare(sim);
    fill -= 6;

    // ── Test 4: Simultaneous read + write (safe) ────────
    // Pre-fill to half so neither overflow nor underflow occurs,
    // and pointers are offset so they never collide.
    printf("cosim test 4: simultaneous r+w (safe)\n");
    for (int i = 0; i < 32; i++) {
        safe_write(sim, 0xB000 + i);
    }
    tick_and_compare(sim);

    // Simultaneous r+w: fill stays constant (balanced).
    for (int i = 0; i < 16; i++) {
        sim.dut->wr_en_i   = 1;
        sim.dut->wr_data_i = 0xC000 + i;
        sim.dut->rd_en_i   = 1;
        tick_and_compare(sim);
        // fill unchanged (one in, one out)
    }
    sim.dut->wr_en_i = 0;
    sim.dut->rd_en_i = 0;
    tick_and_compare(sim);
    tick_and_compare(sim);

    // Drain remaining.
    while (fill > 0) {
        safe_read(sim);
    }
    tick_and_compare(sim);

    // ── Test 5: Fill to capacity and drain ──────────────
    printf("cosim test 5: fill to capacity (%d) and drain\n", DEPTH);
    for (int i = 0; i < DEPTH; i++) {
        safe_write(sim, i);
    }
    tick_and_compare(sim);
    for (int i = 0; i < DEPTH; i++) {
        safe_read(sim);
    }
    tick_and_compare(sim);

    // ── Test 6: Random stimulus (respecting occupancy) ──
    printf("cosim test 6: random stimulus (1000 cycles)\n");
    uint32_t seed = 0x12345678;
    for (int i = 0; i < 1000; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        int want_wr = (seed >> 0) & 1;
        int want_rd = (seed >> 1) & 1;

        // Respect occupancy AND avoid same-address collision:
        // don't do simultaneous r+w when fill==0 (pointers equal).
        int do_wr = want_wr && (fill < DEPTH);
        int do_rd = want_rd && (fill > 0);

        // Avoid simultaneous r+w when pointers might collide.
        // When fill==DEPTH, rptr could equal wptr (full condition).
        // When fill==0, rptr==wptr (empty). Both are corner cases.
        if (do_wr && do_rd && (fill <= 1 || fill >= DEPTH - 1)) {
            // Break tie: prefer read to drain.
            do_wr = 0;
        }

        sim.dut->wr_en_i   = do_wr;
        sim.dut->wr_data_i = seed;
        sim.dut->rd_en_i   = do_rd;
        tick_and_compare(sim);

        fill += do_wr - do_rd;
    }
    sim.dut->wr_en_i = 0;
    sim.dut->rd_en_i = 0;
    tick_and_compare(sim);
    tick_and_compare(sim);

    // Drain any remaining.
    while (fill > 0) {
        safe_read(sim);
    }

    return sim.finish();
}
