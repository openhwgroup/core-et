// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: rf_latch_1r_1w vs prim_rf_1r1w.

#include "Vcosim_rf_1r1w_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rf_1r1w_tb;

static void compare_rd(CosimCtrl<DUT>& sim) {
    sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_rd(sim);
}

static void tick_only(CosimCtrl<DUT>& sim) {
    sim.tick();
}

static void eval_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->eval();
    compare_rd(sim);
}

static void step_half(CosimCtrl<DUT>& sim, int clk_level) {
    sim.dut->clk_i = clk_level;
    sim.dut->eval();
    compare_rd(sim);
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
        std::exit(2);
    }
}

static void init_rf(CosimCtrl<DUT>& sim) {
    for (int i = 0; i < 32; i++) {
        sim.dut->wr_data_en_i = 1;
        sim.dut->wr_en_i      = 0;
        sim.dut->wr_data_i    = 0x10000000u + i;
        tick_only(sim);

        sim.dut->wr_data_en_i = 0;
        sim.dut->wr_en_i      = 1;
        sim.dut->wr_addr_i    = i;
        tick_only(sim);
    }
    sim.dut->wr_en_i = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    // No rst_ni on this module -- just initialize inputs.
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_en_i      = 0;
    sim.dut->wr_addr_i    = 0;
    sim.dut->wr_data_i    = 0;
    sim.dut->rd_addr_i    = 0;

    // Establish clock phase and seed the RF to a deterministic state.
    tick_only(sim);
    init_rf(sim);

    // -- Test 1: 2-phase write then read ----------------------------
    printf("cosim test 1: 2-phase write then read\n");
    for (int i = 0; i < 5; i++) {
        sim.dut->rd_addr_i = i;
        tick_and_compare(sim);
    }
    for (int i = 0; i < 16; i++) {
        // Phase 1: capture data
        sim.dut->wr_data_en_i = 1;
        sim.dut->wr_en_i      = 0;
        sim.dut->wr_data_i    = 0xA000 + i;
        tick_and_compare(sim);

        // Phase 2: commit
        sim.dut->wr_data_en_i = 0;
        sim.dut->wr_en_i      = 1;
        sim.dut->wr_addr_i    = i;
        tick_and_compare(sim);
    }
    sim.dut->wr_en_i = 0;

    // Read back all entries.
    for (int i = 0; i < 16; i++) {
        sim.dut->rd_addr_i = i;
        tick_and_compare(sim);
    }

    // -- Test 2: Write with enables deasserted ----------------------
    printf("cosim test 2: enables deasserted\n");
    // wr_data_en_i=0: should not capture.
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_data_i    = 0xFFFFFFFF;
    tick_and_compare(sim);
    // wr_en_i=0: should not write.
    sim.dut->wr_en_i   = 0;
    sim.dut->wr_addr_i = 0;
    tick_and_compare(sim);
    // Check entry 0 unchanged.
    sim.dut->rd_addr_i = 0;
    tick_and_compare(sim);

    // -- Test 3: Single-cycle both-enables (stale data artifact) ----
    printf("cosim test 3: single-cycle both-enables (timing artifact)\n");
    // Both modules should exhibit the same stale-data behavior.
    for (int i = 0; i < 8; i++) {
        sim.dut->wr_data_en_i = 1;
        sim.dut->wr_en_i      = 1;
        sim.dut->wr_addr_i    = 16 + i;
        sim.dut->wr_data_i    = 0xB000 + i;
        tick_and_compare(sim);
    }
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_en_i      = 0;
    for (int i = 0; i < 8; i++) {
        sim.dut->rd_addr_i = 16 + i;
        tick_and_compare(sim);
    }

    // -- Test 4: split capture/commit preserves captured value ------
    printf("cosim test 4: split capture/commit preserves captured value\n");
    sim.dut->wr_data_en_i = 1;
    sim.dut->wr_en_i      = 0;
    sim.dut->wr_data_i    = 0xCAFEBABE;
    tick_and_compare(sim);
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_en_i      = 1;
    sim.dut->wr_addr_i    = 24;
    sim.dut->wr_data_i    = 0x13579BDF;
    tick_and_compare(sim);
    sim.dut->wr_en_i      = 0;
    sim.dut->rd_addr_i    = 24;
    tick_and_compare(sim);

    // -- Test 5: same-cycle capture+commit uses previous capture ----
    printf("cosim test 5: same-cycle capture+commit uses previous capture\n");
    sim.dut->wr_data_en_i = 1;
    sim.dut->wr_en_i      = 0;
    sim.dut->wr_data_i    = 0x2468ACE0;
    tick_and_compare(sim);
    sim.dut->wr_data_en_i = 1;
    sim.dut->wr_en_i      = 1;
    sim.dut->wr_addr_i    = 25;
    sim.dut->wr_data_i    = 0x0BADF00D;
    tick_and_compare(sim);
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_en_i      = 0;
    sim.dut->rd_addr_i    = 25;
    tick_and_compare(sim);

    // -- Test 6: Half-cycle timing ----------------------------------
    printf("cosim test 6: half-cycle timing\n");
    sim.dut->wr_addr_i = 26;
    sim.dut->wr_data_en_i = 1;
    sim.dut->wr_en_i = 0;
    sim.dut->wr_data_i = 0xAA55CC33;
    step_half(sim, 0);
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_data_i = 0x55AA33CC;
    sim.dut->rd_addr_i = 26;
    eval_and_compare(sim);
    sim.dut->wr_en_i = 1;
    step_half(sim, 1);
    sim.dut->wr_en_i = 0;
    sim.dut->rd_addr_i = 26;
    eval_and_compare(sim);

    sim.dut->wr_addr_i = 27;
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_en_i = 0;
    step_half(sim, 0);
    sim.dut->wr_data_en_i = 1;
    sim.dut->wr_data_i = 0x0F0F5A5A;
    eval_and_compare(sim);
    sim.dut->wr_en_i = 1;
    step_half(sim, 1);
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_en_i = 0;
    sim.dut->rd_addr_i = 27;
    eval_and_compare(sim);

    // -- Test 7: Half-cycle random stimulus -------------------------
    printf("cosim test 7: half-cycle random (512 iterations)\n");
    uint32_t half_seed = 0xA55AC33C;
    for (int i = 0; i < 512; i++) {
        half_seed ^= half_seed << 13;
        half_seed ^= half_seed >> 17;
        half_seed ^= half_seed << 5;
        sim.dut->rd_addr_i = (half_seed >> 7) & 0x1F;
        sim.dut->wr_addr_i = (half_seed >> 2) & 0x1F;
        sim.dut->wr_data_i = half_seed;
        sim.dut->wr_data_en_i = (half_seed >> 0) & 1;
        sim.dut->wr_en_i = 0;
        step_half(sim, 0);

        half_seed ^= half_seed << 13;
        half_seed ^= half_seed >> 17;
        half_seed ^= half_seed << 5;
        sim.dut->rd_addr_i = (half_seed >> 8) & 0x1F;
        sim.dut->wr_data_i = ~half_seed;
        sim.dut->wr_data_en_i = (half_seed >> 1) & 1;
        eval_and_compare(sim);
        sim.dut->wr_en_i = (half_seed >> 2) & 1;
        step_half(sim, 1);
        sim.dut->wr_en_i = 0;
    }

    // -- Test 8: Random stimulus ------------------------------------
    printf("cosim test 8: random (500 cycles)\n");
    uint32_t seed = 0xFACE1234;
    for (int i = 0; i < 500; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        sim.dut->wr_data_en_i = (seed >> 0) & 1;
        sim.dut->wr_en_i      = (seed >> 1) & 1;
        sim.dut->wr_addr_i    = (seed >> 2) & 0x1F;  // 5-bit for Depth=32
        sim.dut->wr_data_i    = seed;
        sim.dut->rd_addr_i    = (seed >> 7) & 0x1F;
        tick_and_compare(sim);
    }

    return sim.finish();
}
