// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: gen_mem2p vs prim_ram_2p.
// Avoids simultaneous read+write to same address on different ports
// (undefined in gen_mem2p with WRITE_THROUGH=0).

#include "Vcosim_ram_2p_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_ram_2p_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("a_rdata", sim.dut->orig_a_rdata_o, sim.dut->new_a_rdata_o);
    sim.compare("b_rdata", sim.dut->orig_b_rdata_o, sim.dut->new_b_rdata_o);
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->a_req_i = 0; sim.dut->a_we_i = 0;
    sim.dut->a_addr_i = 0; sim.dut->a_wdata_i = 0;
    sim.dut->b_req_i = 0; sim.dut->b_we_i = 0;
    sim.dut->b_addr_i = 0; sim.dut->b_wdata_i = 0;
    sim.reset(10);

    // ── Test 1: Write A, read B ─────────────────────────
    printf("cosim test 1: write A, read B\n");
    for (int i = 0; i < 16; i++) {
        sim.dut->a_req_i = 1; sim.dut->a_we_i = 1;
        sim.dut->a_addr_i = i; sim.dut->a_wdata_i = 0xB000 + i;
        tick_and_compare(sim);
    }
    sim.dut->a_req_i = 0;
    tick_and_compare(sim);

    for (int i = 0; i < 16; i++) {
        sim.dut->b_req_i = 1; sim.dut->b_we_i = 0;
        sim.dut->b_addr_i = i;
        tick_and_compare(sim);
    }
    sim.dut->b_req_i = 0;
    tick_and_compare(sim);

    // ── Test 2: Write B, read A ─────────────────────────
    printf("cosim test 2: write B, read A\n");
    for (int i = 0; i < 8; i++) {
        sim.dut->b_req_i = 1; sim.dut->b_we_i = 1;
        sim.dut->b_addr_i = 100 + i; sim.dut->b_wdata_i = 0xD000 + i;
        tick_and_compare(sim);
    }
    sim.dut->b_req_i = 0;
    tick_and_compare(sim);

    for (int i = 0; i < 8; i++) {
        sim.dut->a_req_i = 1; sim.dut->a_we_i = 0;
        sim.dut->a_addr_i = 100 + i;
        tick_and_compare(sim);
    }
    sim.dut->a_req_i = 0;
    tick_and_compare(sim);

    // ── Test 3: Simultaneous A write + B read (diff addr)─
    printf("cosim test 3: simultaneous (different addresses)\n");
    // Pre-fill 50-57.
    for (int i = 0; i < 8; i++) {
        sim.dut->a_req_i = 1; sim.dut->a_we_i = 1;
        sim.dut->a_addr_i = 50 + i; sim.dut->a_wdata_i = 0xE000 + i;
        tick_and_compare(sim);
    }
    sim.dut->a_req_i = 0;
    tick_and_compare(sim);

    // Simultaneous: A writes 60-67, B reads 50-57.
    for (int i = 0; i < 8; i++) {
        sim.dut->a_req_i = 1; sim.dut->a_we_i = 1;
        sim.dut->a_addr_i = 60 + i; sim.dut->a_wdata_i = 0xF000 + i;
        sim.dut->b_req_i = 1; sim.dut->b_we_i = 0;
        sim.dut->b_addr_i = 50 + i;
        tick_and_compare(sim);
    }
    sim.dut->a_req_i = 0; sim.dut->b_req_i = 0;
    tick_and_compare(sim);

    // ── Test 4: Random (no same-addr cross-port r+w) ────
    printf("cosim test 4: random (500 cycles)\n");
    uint32_t seed = 0x12348765;
    for (int i = 0; i < 500; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        uint8_t a_addr = (seed >> 0) & 0x7F;
        uint8_t b_addr = (seed >> 8) & 0x7F;
        int a_req = (seed >> 16) & 1;
        int b_req = (seed >> 17) & 1;
        int a_we  = (seed >> 18) & 1;
        int b_we  = (seed >> 19) & 1;

        // Avoid same-address cross-port collision (undefined in gen_mem2p).
        if (a_req && b_req && a_addr == b_addr && (a_we != b_we)) {
            b_req = 0;
        }

        sim.dut->a_req_i = a_req; sim.dut->a_we_i = a_we;
        sim.dut->a_addr_i = a_addr; sim.dut->a_wdata_i = seed;
        sim.dut->b_req_i = b_req; sim.dut->b_we_i = b_we;
        sim.dut->b_addr_i = b_addr; sim.dut->b_wdata_i = ~seed;
        tick_and_compare(sim);
    }
    sim.dut->a_req_i = 0; sim.dut->b_req_i = 0;
    tick_and_compare(sim);

    return sim.finish();
}
