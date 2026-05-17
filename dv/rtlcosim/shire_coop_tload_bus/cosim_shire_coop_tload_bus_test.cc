// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_coop_tload_bus_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <string>

uint32_t xs(uint32_t& s) { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }

void compare_wide(CosimCtrl<Vcosim_shire_coop_tload_bus_tb>& sim,
                  const char* name, const WData* a, const WData* b, int n) {
    for (int i = 0; i < n; ++i) sim.compare(std::string(name) + std::to_string(i), a[i], b[i]);
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_coop_tload_bus_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->slv_rdy_data_i = 0;
    d->slv_rdy_valid_i = 0;
    d->mst_done_id_i = 0;
    d->mst_done_valid_i = 0;
    sim.reset();
    uint32_t seed = 0xabcdef01u;
    for (int i = 0; i < 2048; ++i) {
        d->slv_rdy_data_i = xs(seed) & 0x0fffffffu;
        d->slv_rdy_valid_i = xs(seed) & 0xfu;
        d->mst_done_id_i = xs(seed) & 0xfffffu;
        d->mst_done_valid_i = xs(seed) & 0xfffu;
        sim.tick();
        compare_wide(sim, "slv_data", d->orig_slv_rdy_data_o, d->new_slv_rdy_data_o, 3);
        sim.compare("slv_valid", d->orig_slv_rdy_valid_o, d->new_slv_rdy_valid_o);
        sim.compare("done_id", d->orig_mst_done_id_o, d->new_mst_done_id_o);
        sim.compare("done_valid", d->orig_mst_done_valid_o, d->new_mst_done_valid_o);
    }
    return sim.finish();
}
