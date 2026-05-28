// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_ioshire_noc_ints_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

uint32_t xs(uint32_t& s) { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_ioshire_noc_ints_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->noc_err_int_srcs_i = 0;
    sim.reset();
    uint32_t seed = 0x13579bdfu;
    for (int i = 0; i < 2048; ++i) {
        uint32_t r = xs(seed);
        d->noc_err_int_srcs_i = r & 0x7ffffu;
        sim.tick();
        sim.compare("noc_normal_status", d->orig_noc_all_err_int_srcs_o,
                    d->new_noc_all_err_int_srcs_o);
        sim.compare("ioshire_noc_err_int", d->orig_ioshire_noc_err_int_o,
                    d->new_ioshire_noc_err_int_o);
    }
    return sim.finish();
}
