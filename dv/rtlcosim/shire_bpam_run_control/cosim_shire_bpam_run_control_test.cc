// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_bpam_run_control_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

uint32_t xs(uint32_t& s) { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_bpam_run_control_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->bpam_bits_i = 0;
    d->tbox_single_step_i = 0;
    d->rbox_single_step_i = 0;
    sim.reset();
    uint32_t seed = 0x9e3779b9u;
    for (int i = 0; i < 2048; ++i) {
        uint32_t r = xs(seed);
        d->bpam_bits_i = r & 0x7fffu;
        d->tbox_single_step_i = (r >> 16) & 0xfu;
        d->rbox_single_step_i = (r >> 20) & 1u;
        sim.tick();
        sim.compare("neigh", d->orig_neigh_bits_o, d->new_neigh_bits_o);
        sim.compare("rbox", d->orig_rbox_bits_o, d->new_rbox_bits_o);
    }
    return sim.finish();
}
