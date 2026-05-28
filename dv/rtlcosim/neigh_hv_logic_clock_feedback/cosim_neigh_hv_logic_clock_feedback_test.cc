// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_clock_feedback_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

namespace {
void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_clock_feedback_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("dll_feedback_shire", d->orig_dll_feedback_shire_o,
                d->new_dll_feedback_shire_o);
    sim.compare("dll_feedback_neigh", d->orig_dll_feedback_neigh_o,
                d->new_dll_feedback_neigh_o);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_clock_feedback_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.reset();

    uint32_t ones = 0;
    uint32_t zeros = 0;
    for (int cycle = 0; cycle < 2048; ++cycle) {
        d->clock_shire_i = (cycle & 1) != 0;
        d->clock_neigh_i = ((cycle >> 1) & 1) != 0;
        ones |= d->clock_shire_i ? 1u : 0u;
        zeros |= d->clock_shire_i ? 0u : 1u;
        ones |= d->clock_neigh_i ? 2u : 0u;
        zeros |= d->clock_neigh_i ? 0u : 2u;
        sim.tick();
        compare_all(sim);
    }
    sim.check((ones & 0x3u) == 0x3u, "clock feedback inputs reached 1");
    sim.check((zeros & 0x3u) == 0x3u, "clock feedback inputs reached 0");
    return sim.finish();
}
