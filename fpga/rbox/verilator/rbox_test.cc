// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// rbox — Verilator smoke test.
//
// Resets the full rbox_top and verifies quiescent state.

#include "Vrbox_top.h"
#include "sim_ctrl.h"

using DUT = Vrbox_top;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    // Tie off inputs.
    sim.dut->shire_id_i = 0;
    sim.dut->rbox_sc_req_ready_i = 0;
    sim.dut->sc_rbox_rsp_valid_i = 0;

    // Reset.
    sim.reset(10);

    // Verify quiescent state after reset.
    sim.check(sim.dut->rbox_sc_req_valid_o == 0, "no cache requests after reset");
    sim.check(sim.dut->sc_rbox_rsp_ready_o == 0, "no response ready after reset");

    // Run a few idle cycles.
    for (int i = 0; i < 20; i++) {
        sim.tick();
        sim.check(sim.dut->rbox_sc_req_valid_o == 0, "idle: no cache requests");
    }

    return sim.finish();
}
