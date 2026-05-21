// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_channel_tb.h"
#include "sim_ctrl.h"

int main(int argc, char** argv) {
    SimCtrl<Vneigh_channel_tb> sim(argc, argv);
    sim.reset(8);

    sim.check(sim.dut->clk_minion_o == sim.dut->clk_i, "minion clock follows wrapper clock");
    sim.check(sim.dut->clk_tbox_o == sim.dut->clk_i, "tbox clock follows wrapper clock");
    sim.check(sim.dut->any_neigh_sc_req_valid_o == 0, "idle reset state has no shire-cache request valid");
    sim.check(sim.dut->any_l2_dcache_resp_valid_o == 0, "idle reset state has no dcache response valid");
    sim.check(sim.dut->tbox_neigh_rsp_valid_o == 0, "idle reset state has no tbox response valid");

    for (int i = 0; i < 8; ++i) {
        sim.tick();
        sim.check(sim.dut->any_neigh_sc_req_valid_o == 0, "idle cycle has no shire-cache request valid");
        sim.check(sim.dut->any_l2_dcache_resp_valid_o == 0, "idle cycle has no dcache response valid");
    }

    return sim.finish();
}
