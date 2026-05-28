// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_ipi_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

namespace {
void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_ipi_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("esr_to_neigh_ipi_trigger_push",
                d->orig_esr_to_neigh_ipi_trigger_push_o,
                d->new_esr_to_neigh_ipi_trigger_push_o);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_ipi_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->esr_to_neigh_ipi_trigger_i = 0;
    sim.reset();

    std::mt19937 rng(0x1f1001u);
    uint32_t ones = 0;
    uint32_t zeros = 0;
    for (int cycle = 0; cycle < 2048; ++cycle) {
        uint32_t value = (uint32_t(rng()) ^ (uint32_t(cycle) * 0x1021u)) & 0xffffu;
        if (cycle == 0) value = 0;
        if (cycle == 1) value = 0xffffu;
        d->esr_to_neigh_ipi_trigger_i = value;
        ones |= value;
        zeros |= (~value) & 0xffffu;
        sim.tick();
        compare_all(sim);
    }
    sim.check(ones == 0xffffu, "IPI vector saw every bit high");
    sim.check(zeros == 0xffffu, "IPI vector saw every bit low");
    return sim.finish();
}
