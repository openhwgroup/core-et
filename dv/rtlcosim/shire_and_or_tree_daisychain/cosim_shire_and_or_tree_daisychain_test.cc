// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_and_or_tree_daisychain_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

uint32_t xs(uint32_t& s) { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_and_or_tree_daisychain_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->shire_id_i = 0;
    d->l2_bits_i = 0;
    d->l0_bits_i = 0;
    sim.reset();
    uint32_t seed = 0x31415926u;
    for (int i = 0; i < 4096; ++i) {
        uint64_t l0 = 0;
        for (int n = 0; n < 4; ++n) l0 |= uint64_t(xs(seed) & 0x3ffu) << (10 * n);
        d->shire_id_i = xs(seed) & 0xffu;
        d->l2_bits_i = xs(seed) & 0x7ffu;
        d->l0_bits_i = l0;
        sim.tick();
        sim.compare("l1", d->orig_l1_bits_o, d->new_l1_bits_o);
        sim.compare("l2", d->orig_l2_bits_o, d->new_l2_bits_o);
    }
    return sim.finish();
}
