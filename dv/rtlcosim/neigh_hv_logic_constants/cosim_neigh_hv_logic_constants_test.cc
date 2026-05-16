// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_constants_tb.h"
#include "cosim_ctrl.h"

namespace {
void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_constants_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("ct_one", d->orig_ct_one_o, d->new_ct_one_o);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_constants_tb> sim(argc, argv);
    sim.reset();
    for (int cycle = 0; cycle < 2048; ++cycle) {
        sim.tick();
        compare_all(sim);
    }
    sim.check(sim.dut->new_ct_one_o == 1, "constant leaf has no functional inputs and drives one");
    return sim.finish();
}
