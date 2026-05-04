// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_ram_1p — Verilator test.

#include "Vprim_ram_1p.h"
#include "sim_ctrl.h"

using DUT = Vprim_ram_1p;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->req_i   = 0;
    sim.dut->we_i    = 0;
    sim.dut->addr_i  = 0;
    sim.dut->wdata_i = 0;
    sim.dut->cfg_i   = 0;
    sim.dut->dft_i   = 0;
    sim.reset();

    // Default: Width=32, Depth=256

    // ── Test 1: Write and read back ─────────────────────
    printf("test 1: write and read\n");
    sim.dut->req_i   = 1;
    sim.dut->we_i    = 1;
    sim.dut->addr_i  = 0;
    sim.dut->wdata_i = 0xDEADBEEF;
    sim.tick();
    sim.dut->we_i   = 0;
    sim.dut->addr_i = 0;
    sim.tick();
    sim.check(sim.dut->rdata_o == 0xDEADBEEF, "read back 0xDEADBEEF");

    // ── Test 2: Multiple addresses ──────────────────────
    printf("test 2: multiple addresses\n");
    for (int i = 0; i < 8; i++) {
        sim.dut->req_i   = 1;
        sim.dut->we_i    = 1;
        sim.dut->addr_i  = i;
        sim.dut->wdata_i = 0x100 + i;
        sim.tick();
    }
    for (int i = 0; i < 8; i++) {
        sim.dut->req_i  = 1;
        sim.dut->we_i   = 0;
        sim.dut->addr_i = i;
        sim.tick();
        sim.check(sim.dut->rdata_o == (uint32_t)(0x100 + i),
                  "addr[" + std::to_string(i) + "]");
    }

    // ── Test 3: Output holds when req=0 ─────────────────
    printf("test 3: output holds\n");
    sim.dut->req_i  = 1;
    sim.dut->we_i   = 0;
    sim.dut->addr_i = 3;
    sim.tick();
    uint32_t held = sim.dut->rdata_o;
    sim.dut->req_i = 0;
    sim.tick();
    sim.tick();
    sim.check(sim.dut->rdata_o == held, "output holds when req=0");

    // ── Test 4: Write does not update rdata ────────────
    printf("test 4: write does not update rdata (mutually exclusive)\n");
    // Read addr 3 to set rdata.
    sim.dut->req_i  = 1;
    sim.dut->we_i   = 0;
    sim.dut->addr_i = 3;
    sim.tick();
    uint32_t before_write = sim.dut->rdata_o;
    // Write to addr 10 — rdata should NOT change.
    sim.dut->we_i    = 1;
    sim.dut->addr_i  = 10;
    sim.dut->wdata_i = 0xAAAA;
    sim.tick();
    sim.check(sim.dut->rdata_o == before_write, "rdata unchanged during write");
    // Read back addr 10.
    sim.dut->we_i   = 0;
    sim.dut->addr_i = 10;
    sim.tick();
    sim.check(sim.dut->rdata_o == 0xAAAA, "read back written value");

    return sim.finish();
}
