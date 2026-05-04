// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: gen_mem1p vs prim_ram_1p.

#include "Vcosim_ram_1p_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_ram_1p_tb;

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    sim.compare("rdata", sim.dut->orig_rdata_o, sim.dut->new_rdata_o);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->req_i   = 0;
    sim.dut->we_i    = 0;
    sim.dut->addr_i  = 0;
    sim.dut->wdata_i = 0;
    sim.reset(10);

    // ── Test 1: Write then read ─────────────────────────
    printf("cosim test 1: write then read\n");
    for (int i = 0; i < 16; i++) {
        sim.dut->req_i   = 1;
        sim.dut->we_i    = 1;
        sim.dut->addr_i  = i;
        sim.dut->wdata_i = 0xA000 + i;
        tick_and_compare(sim);
    }
    sim.dut->we_i = 0;
    for (int i = 0; i < 16; i++) {
        sim.dut->addr_i = i;
        tick_and_compare(sim);
    }

    // ── Test 2: Output holds ────────────────────────────
    printf("cosim test 2: output holds\n");
    sim.dut->req_i = 0;
    tick_and_compare(sim);
    tick_and_compare(sim);

    // ── Test 3: Random ──────────────────────────────────
    printf("cosim test 3: random (500 cycles)\n");
    uint32_t seed = 0xFACE1234;
    for (int i = 0; i < 500; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        sim.dut->req_i   = (seed >> 0) & 1;
        sim.dut->we_i    = (seed >> 1) & 1;
        sim.dut->addr_i  = (seed >> 2) & 0xFF;
        sim.dut->wdata_i = seed;
        tick_and_compare(sim);
    }

    return sim.finish();
}
