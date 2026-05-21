// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_uc_fcc_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

namespace {
void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_uc_fcc_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("neigh_uc_fcc_vcfifo_push", d->orig_neigh_uc_fcc_vcfifo_push_o,
                d->new_neigh_uc_fcc_vcfifo_push_o);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_uc_fcc_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->uc_to_neigh_fcc_i = 0;
    sim.reset();

    std::mt19937 rng(0xfcc001u);
    uint32_t ones = 0;
    uint32_t zeros = 0;
    for (int cycle = 0; cycle < 2048; ++cycle) {
        uint32_t value = (uint32_t(rng()) ^ (uint32_t(cycle) * 0x21u)) & 0xffu;
        if (cycle == 0) value = 0;
        if (cycle == 1) value = 0xffu;
        d->uc_to_neigh_fcc_i = value;
        ones |= value;
        zeros |= (~value) & 0xffu;
        sim.tick();
        compare_all(sim);
    }
    sim.check(ones == 0xffu, "FCC vector saw every bit high");
    sim.check(zeros == 0xffu, "FCC vector saw every bit low");
    return sim.finish();
}
