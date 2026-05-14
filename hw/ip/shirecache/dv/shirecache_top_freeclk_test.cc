// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_top free-running clock hookup smoke test.

#include "Vshirecache_top_freeclk_tb.h"
#include "sim_ctrl.h"

int main(int argc, char** argv) {
    SimCtrl<Vshirecache_top_freeclk_tb> sim(argc, argv);
    auto* dut = sim.dut.get();

    dut->clk_free_i = 0;
    dut->status_monitor_bank_sel_i = 0;
    sim.reset();

    for (int i = 0; i < 4; ++i) {
        sim.tick();
        sim.check(dut->sc_trace_valid_o == 0,
                  "main clk_i ticks do not reach bank clk_free_i when external clk_free_i is low");
    }

    dut->clk_free_i = 1;
    sim.tick();
    sim.check(dut->sc_trace_valid_o == 1,
              "external clk_free_i reaches bank trace/perfmon free-clock path");

    dut->clk_free_i = 0;
    sim.tick();
    sim.check(dut->sc_trace_valid_o == 0,
              "bank free-clock path follows external clk_free_i low, not clk_i high phase");

    dut->clk_free_i = 1;
    sim.tick();
    sim.check(dut->sc_trace_valid_o == 1,
              "external clk_free_i can reassert after being independently deasserted");

    return sim.finish();
}
