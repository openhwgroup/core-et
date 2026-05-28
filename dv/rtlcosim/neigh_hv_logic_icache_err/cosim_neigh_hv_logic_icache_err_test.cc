// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_icache_err_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

namespace {
void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_icache_err_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("ff_dataout", d->orig_ff_dataout_o, d->new_ff_dataout_o);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_icache_err_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->ff_datain_i = 0;
    sim.reset();

    std::mt19937 rng(0x1caceu);
    uint32_t ones = 0;
    uint32_t zeros = 0;
    for (int cycle = 0; cycle < 4096; ++cycle) {
        if (cycle == 73) d->rst_ni = 0;
        if (cycle == 76) d->rst_ni = 1;
        d->ff_datain_i = ((cycle / 3) & 1) ^ ((rng() >> 4) & 1u);
        ones |= d->rst_ni ? 1u : 0u;
        zeros |= d->rst_ni ? 0u : 1u;
        ones |= d->ff_datain_i ? 2u : 0u;
        zeros |= d->ff_datain_i ? 0u : 2u;
        sim.tick();
        compare_all(sim);
    }
    sim.check((ones & 0x3u) == 0x3u, "all inputs reached 1");
    sim.check((zeros & 0x3u) == 0x3u, "all inputs reached 0");
    return sim.finish();
}
