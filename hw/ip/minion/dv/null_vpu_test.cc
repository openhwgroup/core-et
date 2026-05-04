// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Smoke test for null_vpu.

#include "Vnull_vpu_tb.h"
#include "sim_ctrl.h"

using DUT = Vnull_vpu_tb;

static void check_outputs(SimCtrl<DUT>& sim, bool expect_nonzero_pattern) {
    sim.check(sim.dut->input_pattern_nonzero_o == (expect_nonzero_pattern ? 1 : 0),
              expect_nonzero_pattern ? "selected pattern drives non-zero null_vpu inputs"
                                     : "zero pattern keeps null_vpu inputs idle");
    sim.check(sim.dut->id_core_ctrl_zero_o == 1, "ID-stage return bundle stays idle");
    sim.check(sim.dut->ex_core_ctrl_fail_closed_o == 1,
              "EX-stage return bundle stays fail-closed with illegal_rm asserted");
    sim.check(sim.dut->f2_core_ctrl_zero_o == 1, "TAG-stage return bundle stays idle");
    sim.check(sim.dut->f3_core_ctrl_zero_o == 1, "MEM-stage return bundle stays idle");
    sim.check(sim.dut->wb_core_ctrl_zero_o == 1, "WB-stage return bundle stays idle");
    sim.check(sim.dut->io_events_zero_o == 1, "performance counter events stay idle");
    sim.check(sim.dut->vpu_dbg_match_zero_o == 1, "debug match bus stays idle");
    sim.check(sim.dut->vpu_dbg_filter_zero_o == 1, "debug filter bus stays idle");
    sim.check(sim.dut->vpu_dbg_data_zero_o == 1, "debug data bus stays idle");
    sim.check(sim.dut->dcache_ctrl_zero_o == 1, "dcache direct-control bus stays idle");
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000;

    sim.dut->pattern_sel_i = 0;
    sim.reset();

    sim.dut->pattern_sel_i = 0;
    sim.tick();
    check_outputs(sim, false);

    sim.dut->pattern_sel_i = 1;
    sim.tick();
    check_outputs(sim, true);

    sim.dut->pattern_sel_i = 2;
    sim.tick();
    check_outputs(sim, true);

    sim.dut->pattern_sel_i = 3;
    sim.tick();
    check_outputs(sim, true);

    sim.dut->pattern_sel_i = 0;
    sim.tick();
    check_outputs(sim, false);

    return sim.finish();
}
