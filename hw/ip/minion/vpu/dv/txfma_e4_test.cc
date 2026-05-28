// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_e4_tb.h"
#include "sim_ctrl.h"
#include "txfma_e_stage_model.h"

using DUT = Vtxfma_e4_tb;
using namespace txfma_e_model;

namespace {

void check_case(SimCtrl<DUT>& sim, Coverage& cov, bool s1, bool s2, const char* name) {
    sim.dut->e_sticky1_i = s1;
    sim.dut->e_sticky2_i = s2;
    cov.observe(0, s1);
    cov.observe(1, s2);
    sim.dut->eval();
    sim.check(sim.dut->ph_comb_stiky_o == model_e4(s1, s2), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    cov.add("e_sticky1_i", 1);
    cov.add("e_sticky2_i", 1);

    check_case(sim, cov, false, false, "sticky00");
    check_case(sim, cov, false, true, "sticky01");
    check_case(sim, cov, true, false, "sticky10");
    check_case(sim, cov, true, true, "sticky11");

    cov.check(sim);
    return sim.finish();
}
