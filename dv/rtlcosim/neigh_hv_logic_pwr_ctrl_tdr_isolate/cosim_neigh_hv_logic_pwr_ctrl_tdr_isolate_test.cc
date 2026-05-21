// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_pwr_ctrl_tdr_isolate_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

namespace {
void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_pwr_ctrl_tdr_isolate_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("pwr_ctrl_glb_isolate", d->orig_pwr_ctrl_glb_isolate_o,
                d->new_pwr_ctrl_glb_isolate_o);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_pwr_ctrl_tdr_isolate_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.reset();

    uint32_t ones = 0;
    uint32_t zeros = 0;
    for (int cycle = 0; cycle < 2048; ++cycle) {
        d->dft_tdr_ctrl_i = (cycle & 1) != 0;
        d->pwr_ctrl_glb_isolate_i = ((cycle >> 1) & 1) != 0;
        ones |= d->dft_tdr_ctrl_i ? 1u : 0u;
        zeros |= d->dft_tdr_ctrl_i ? 0u : 1u;
        ones |= d->pwr_ctrl_glb_isolate_i ? 2u : 0u;
        zeros |= d->pwr_ctrl_glb_isolate_i ? 0u : 2u;
        sim.tick();
        compare_all(sim);
    }
    sim.check((ones & 0x3u) == 0x3u, "all inputs reached 1");
    sim.check((zeros & 0x3u) == 0x3u, "all inputs reached 0");
    return sim.finish();
}
