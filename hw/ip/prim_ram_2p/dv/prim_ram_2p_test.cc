// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_ram_2p — Verilator test.

#include "Vprim_ram_2p.h"
#include "sim_ctrl.h"

using DUT = Vprim_ram_2p;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->a_req_i   = 0; sim.dut->a_we_i = 0;
    sim.dut->a_addr_i  = 0; sim.dut->a_wdata_i = 0;
    sim.dut->b_req_i   = 0; sim.dut->b_we_i = 0;
    sim.dut->b_addr_i  = 0; sim.dut->b_wdata_i = 0;
    sim.dut->cfg_i     = 0; sim.dut->dft_i = 0;
    sim.reset();

    // Default: Width=32, Depth=256

    // ── Test 1: Write port A, read port B ───────────────
    printf("test 1: write A, read B\n");
    sim.dut->a_req_i   = 1;
    sim.dut->a_we_i    = 1;
    sim.dut->a_addr_i  = 5;
    sim.dut->a_wdata_i = 0xCAFE;
    sim.tick();
    sim.dut->a_req_i = 0;

    sim.dut->b_req_i  = 1;
    sim.dut->b_we_i   = 0;
    sim.dut->b_addr_i = 5;
    sim.tick();
    sim.check(sim.dut->b_rdata_o == 0xCAFE, "port B reads 0xCAFE");
    sim.dut->b_req_i = 0;
    sim.tick();

    // ── Test 2: Write port B, read port A ───────────────
    printf("test 2: write B, read A\n");
    sim.dut->b_req_i   = 1;
    sim.dut->b_we_i    = 1;
    sim.dut->b_addr_i  = 10;
    sim.dut->b_wdata_i = 0xBEEF;
    sim.tick();
    sim.dut->b_req_i = 0;

    sim.dut->a_req_i  = 1;
    sim.dut->a_we_i   = 0;
    sim.dut->a_addr_i = 10;
    sim.tick();
    sim.check(sim.dut->a_rdata_o == 0xBEEF, "port A reads 0xBEEF");
    sim.dut->a_req_i = 0;
    sim.tick();

    // ── Test 3: Simultaneous write A + read B (diff addr)─
    printf("test 3: simultaneous A write + B read (different addresses)\n");
    // Pre-write addr 20.
    sim.dut->a_req_i = 1; sim.dut->a_we_i = 1;
    sim.dut->a_addr_i = 20; sim.dut->a_wdata_i = 0x1111;
    sim.tick();
    sim.dut->a_req_i = 0;
    sim.tick();

    // Simultaneous: write addr 21 on A, read addr 20 on B.
    sim.dut->a_req_i = 1; sim.dut->a_we_i = 1;
    sim.dut->a_addr_i = 21; sim.dut->a_wdata_i = 0x2222;
    sim.dut->b_req_i = 1; sim.dut->b_we_i = 0;
    sim.dut->b_addr_i = 20;
    sim.tick();
    sim.check(sim.dut->b_rdata_o == 0x1111, "B reads 0x1111 while A writes elsewhere");
    sim.dut->a_req_i = 0; sim.dut->b_req_i = 0;
    sim.tick();

    // ── Test 4: Output holds when req=0 ─────────────────
    printf("test 4: output holds\n");
    uint32_t a_held = sim.dut->a_rdata_o;
    uint32_t b_held = sim.dut->b_rdata_o;
    sim.tick();
    sim.tick();
    sim.check(sim.dut->a_rdata_o == a_held, "A output holds");
    sim.check(sim.dut->b_rdata_o == b_held, "B output holds");

    // ── Test 5: Multiple addresses ──────────────────────
    printf("test 5: bulk write A, bulk read B\n");
    for (int i = 0; i < 16; i++) {
        sim.dut->a_req_i = 1; sim.dut->a_we_i = 1;
        sim.dut->a_addr_i = 100 + i;
        sim.dut->a_wdata_i = 0xF000 + i;
        sim.tick();
    }
    sim.dut->a_req_i = 0;
    sim.tick();

    for (int i = 0; i < 16; i++) {
        sim.dut->b_req_i = 1; sim.dut->b_we_i = 0;
        sim.dut->b_addr_i = 100 + i;
        sim.tick();
        sim.check(sim.dut->b_rdata_o == (uint32_t)(0xF000 + i),
                  "bulk[" + std::to_string(i) + "]");
    }
    sim.dut->b_req_i = 0;
    sim.tick();

    return sim.finish();
}
