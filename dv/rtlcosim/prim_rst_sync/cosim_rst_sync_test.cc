// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: rst_repeat (original) vs prim_rst_sync (new).

#include "Vcosim_rst_sync_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rst_sync_tb;

static void tick(CosimCtrl<DUT>& sim) {
    sim.dut->clk_i = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->clk_i = 1;
    sim.dut->eval();
    sim.sim_time++;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 1000;

    // ── Test 1: Assert reset ────────────────────────────
    // Note: At t=0 before any clock edge, Verilator does not fire
    // the original's async reset (negedge int_reset_in_n) because the
    // signal starts at 0 with no edge transition. Our module correctly
    // asserts reset immediately via the always_ff sensitivity list.
    // In real hardware both assert at power-on. We start comparing
    // after the first clock edge when Verilator has processed the
    // async reset for both.
    printf("cosim test 1: assert reset\n");
    sim.dut->rst_ni = 0;
    tick(sim);
    sim.compare("rst (asserted)", sim.dut->orig_rst_no, sim.dut->new_rst_no);
    tick(sim);
    sim.compare("rst (held)", sim.dut->orig_rst_no, sim.dut->new_rst_no);
    tick(sim);
    sim.compare("rst (held 2)", sim.dut->orig_rst_no, sim.dut->new_rst_no);

    // ── Test 2: Deassert reset ──────────────────────────
    printf("cosim test 2: deassert reset\n");
    sim.dut->rst_ni = 1;
    tick(sim);
    sim.compare("rst (deasserted)", sim.dut->orig_rst_no, sim.dut->new_rst_no);
    tick(sim);
    sim.compare("rst (stable)", sim.dut->orig_rst_no, sim.dut->new_rst_no);

    // ── Test 3: Re-assert and deassert ──────────────────
    printf("cosim test 3: toggle reset\n");
    for (int i = 0; i < 5; i++) {
        sim.dut->rst_ni = 0;
        sim.dut->eval();
        sim.compare("rst (re-assert " + std::to_string(i) + ")",
                     sim.dut->orig_rst_no, sim.dut->new_rst_no);
        tick(sim);
        sim.compare("rst (held " + std::to_string(i) + ")",
                     sim.dut->orig_rst_no, sim.dut->new_rst_no);

        sim.dut->rst_ni = 1;
        tick(sim);
        sim.compare("rst (deassert " + std::to_string(i) + ")",
                     sim.dut->orig_rst_no, sim.dut->new_rst_no);
        tick(sim);
        sim.compare("rst (stable " + std::to_string(i) + ")",
                     sim.dut->orig_rst_no, sim.dut->new_rst_no);
    }

    return sim.finish();
}
