// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_rst_sync — Verilator test.

#include "Vprim_rst_sync.h"
#include "sim_ctrl.h"

using DUT = Vprim_rst_sync;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 1000;

    sim.dut->dft_i = 0;  // DFT off

    // ── Test 1: Reset asserts immediately ───────────────
    printf("test 1: async assert\n");
    sim.dut->rst_ni = 0;
    sim.dut->clk_i  = 0;
    sim.dut->eval();
    sim.check(sim.dut->rst_no == 0, "rst_no low immediately when rst_ni low");

    // ── Test 2: Reset holds through clock edges ─────────
    printf("test 2: reset holds during clock\n");
    sim.tick();
    sim.check(sim.dut->rst_no == 0, "rst_no stays low during reset");
    sim.tick();
    sim.check(sim.dut->rst_no == 0, "rst_no still low");

    // ── Test 3: Sync deassert ───────────────────────────
    printf("test 3: sync deassert\n");
    sim.dut->rst_ni = 1;
    // Before clock edge, output should still be 0.
    sim.dut->clk_i = 0;
    sim.dut->eval();
    sim.check(sim.dut->rst_no == 0, "rst_no still low before clock edge");

    // After positive clock edge, output deasserts.
    sim.tick();
    sim.check(sim.dut->rst_no == 1, "rst_no high after clock edge (sync deassert)");

    // ── Test 4: Stays deasserted ────────────────────────
    printf("test 4: stays deasserted\n");
    sim.tick();
    sim.check(sim.dut->rst_no == 1, "rst_no remains high");
    sim.tick();
    sim.check(sim.dut->rst_no == 1, "rst_no remains high");

    // ── Test 5: Re-assert ───────────────────────────────
    printf("test 5: re-assert\n");
    sim.dut->rst_ni = 0;
    sim.dut->clk_i = 0;
    sim.dut->eval();
    sim.check(sim.dut->rst_no == 0, "rst_no goes low immediately on re-assert");

    // Release again.
    sim.dut->rst_ni = 1;
    sim.tick();
    sim.check(sim.dut->rst_no == 1, "deasserts again after clock edge");

    return sim.finish();
}
