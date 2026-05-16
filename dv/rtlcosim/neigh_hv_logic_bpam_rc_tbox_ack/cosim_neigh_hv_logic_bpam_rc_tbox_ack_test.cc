// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_bpam_rc_tbox_ack_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

namespace {
void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_bpam_rc_tbox_ack_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("bpam_rc_tbox_ack_halt", d->orig_bpam_rc_tbox_ack_halt_o,
                d->new_bpam_rc_tbox_ack_halt_o);
    sim.compare("bpam_rc_tbox_ack_resume", d->orig_bpam_rc_tbox_ack_resume_o,
                d->new_bpam_rc_tbox_ack_resume_o);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_bpam_rc_tbox_ack_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->bpam_rc_tbox_ack_halt_i = 0;
    d->bpam_rc_tbox_ack_resume_i = 0;
    sim.reset();

    std::mt19937 rng(0xac701u);
    uint32_t ones = 0;
    uint32_t zeros = 0;
    for (int cycle = 0; cycle < 4096; ++cycle) {
        if (cycle == 256) d->rst_ni = 0;
        if (cycle == 259) d->rst_ni = 1;
        d->bpam_rc_tbox_ack_halt_i = ((rng() >> 0) & 1u) || (cycle % 13 == 0);
        d->bpam_rc_tbox_ack_resume_i = ((rng() >> 1) & 1u) || (cycle % 17 == 0);
        ones |= d->rst_ni ? 1u : 0u;
        zeros |= d->rst_ni ? 0u : 1u;
        ones |= d->bpam_rc_tbox_ack_halt_i ? 2u : 0u;
        zeros |= d->bpam_rc_tbox_ack_halt_i ? 0u : 2u;
        ones |= d->bpam_rc_tbox_ack_resume_i ? 4u : 0u;
        zeros |= d->bpam_rc_tbox_ack_resume_i ? 0u : 4u;
        sim.tick();
        compare_all(sim);
    }
    sim.check((ones & 0x7u) == 0x7u, "all inputs reached 1");
    sim.check((zeros & 0x7u) == 0x7u, "all inputs reached 0");
    return sim.finish();
}
