// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_e4_tb.h"
#include "cosim_ctrl.h"
#include "../../../hw/ip/minion/vpu/dv/txfma_e_stage_model.h"

using DUT = Vcosim_txfma_e4_tb;
using namespace txfma_e_model;

namespace {

void drive_and_compare(CosimCtrl<DUT>& sim, Coverage& cov, bool s1, bool s2) {
    sim.dut->e_sticky1_i = s1;
    sim.dut->e_sticky2_i = s2;
    cov.observe(0, s1);
    cov.observe(1, s2);
    sim.dut->eval();
    sim.compare("ph_comb_stiky", sim.dut->orig_ph_comb_stiky_o, sim.dut->new_ph_comb_stiky_o);
    sim.check(sim.dut->new_ph_comb_stiky_o == model_e4(s1, s2), "model ph_comb_stiky");
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    cov.add("e_sticky1_i", 1);
    cov.add("e_sticky2_i", 1);

    for (int i = 0; i < 512; ++i) {
        drive_and_compare(sim, cov, false, false);
        drive_and_compare(sim, cov, false, true);
        drive_and_compare(sim, cov, true, false);
        drive_and_compare(sim, cov, true, true);
    }

    cov.check(sim);
    sim.check(sim.comparisons >= 2048, "txfma_e4 should run at least 2048 output comparisons");
    return sim.finish();
}
