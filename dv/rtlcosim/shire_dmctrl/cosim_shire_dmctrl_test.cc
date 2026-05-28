// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_dmctrl_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

uint32_t xs(uint32_t& s) { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_dmctrl_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->dmctrl_bits_i = 0;
    sim.reset();
    uint32_t seed = 0x12345678u;
    for (int i = 0; i < 2200; ++i) {
        uint32_t r = xs(seed);
        // Force frequent write-enable toggles and randomize all payload bits.
        d->dmctrl_bits_i = ((r >> 3) & 0x3feu) | ((i & 7) == 0 || ((r >> 1) & 1u));
        sim.tick();
        sim.compare("dmctrl", d->orig_dmctrl_bits_o, d->new_dmctrl_bits_o);
    }
    return sim.finish();
}
