// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_bpam_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

namespace {
void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_bpam_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("bpam_run_control_vcfifo_push",
                d->orig_bpam_run_control_vcfifo_push_o,
                d->new_bpam_run_control_vcfifo_push_o);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_bpam_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->bpam_run_control_i = 0;
    d->dmctrl_write_enable_i = 0;
    sim.reset();

    std::mt19937 rng(0xb0a401u);
    uint32_t rc_ones = 0;
    uint32_t rc_zeros = 0;
    uint32_t scalar_ones = 0;
    uint32_t scalar_zeros = 0;
    for (int cycle = 0; cycle < 4096; ++cycle) {
        if (cycle == 127) d->rst_ni = 0;
        if (cycle == 130) d->rst_ni = 1;
        d->bpam_run_control_i = (cycle ^ rng()) & 0xfu;
        d->dmctrl_write_enable_i = ((cycle % 11) == 0) || ((rng() >> 8) & 1u);
        rc_ones |= d->bpam_run_control_i;
        rc_zeros |= (~d->bpam_run_control_i) & 0xfu;
        scalar_ones |= d->rst_ni ? 1u : 0u;
        scalar_zeros |= d->rst_ni ? 0u : 1u;
        scalar_ones |= d->dmctrl_write_enable_i ? 2u : 0u;
        scalar_zeros |= d->dmctrl_write_enable_i ? 0u : 2u;
        sim.tick();
        compare_all(sim);
    }
    sim.check(rc_ones == 0xfu, "run-control bits reached 1");
    sim.check(rc_zeros == 0xfu, "run-control bits reached 0");
    sim.check((scalar_ones & 0x3u) == 0x3u, "scalar inputs reached 1");
    sim.check((scalar_zeros & 0x3u) == 0x3u, "scalar inputs reached 0");
    return sim.finish();
}
