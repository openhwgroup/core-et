// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_apb_rsp_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

namespace {
void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_apb_rsp_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("apb_esr_rsp_sync_prdata", d->orig_apb_esr_rsp_sync_prdata_o,
                d->new_apb_esr_rsp_sync_prdata_o);
    sim.compare("apb_esr_rsp_sync_pready", d->orig_apb_esr_rsp_sync_pready_o,
                d->new_apb_esr_rsp_sync_pready_o);
    sim.compare("apb_esr_rsp_sync_pslverr", d->orig_apb_esr_rsp_sync_pslverr_o,
                d->new_apb_esr_rsp_sync_pslverr_o);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_apb_rsp_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->apb_esr_rsp_vcfifo_valid_i = 0;
    d->apb_esr_rsp_pop_prdata_i = 0;
    d->apb_esr_rsp_pop_pready_i = 0;
    d->apb_esr_rsp_pop_pslverr_i = 0;
    sim.reset();

    std::mt19937_64 rng(0x41504202ull);
    uint64_t scalar_ones = 0;
    uint64_t scalar_zeros = 0;
    uint64_t data_ones = 0;
    uint64_t data_zeros = 0;
    for (int cycle = 0; cycle < 2048; ++cycle) {
        const uint64_t data = rng() ^ (uint64_t{cycle} << 32) ^ uint64_t{cycle};
        d->apb_esr_rsp_vcfifo_valid_i = (cycle & 1) != 0;
        d->apb_esr_rsp_pop_prdata_i = data;
        d->apb_esr_rsp_pop_pready_i = ((cycle >> 1) & 1) != 0;
        d->apb_esr_rsp_pop_pslverr_i = ((cycle >> 2) & 1) != 0;
        scalar_ones |= d->apb_esr_rsp_vcfifo_valid_i ? 1ull : 0ull;
        scalar_zeros |= d->apb_esr_rsp_vcfifo_valid_i ? 0ull : 1ull;
        data_ones |= d->apb_esr_rsp_pop_prdata_i;
        data_zeros |= ~d->apb_esr_rsp_pop_prdata_i;
        scalar_ones |= d->apb_esr_rsp_pop_pready_i ? (1ull << 1) : 0ull;
        scalar_zeros |= d->apb_esr_rsp_pop_pready_i ? 0ull : (1ull << 1);
        scalar_ones |= d->apb_esr_rsp_pop_pslverr_i ? (1ull << 2) : 0ull;
        scalar_zeros |= d->apb_esr_rsp_pop_pslverr_i ? 0ull : (1ull << 2);
        sim.tick();
        compare_all(sim);
    }
    sim.check((scalar_ones & 0x7ull) == 0x7ull, "scalar inputs reached 1");
    sim.check((scalar_zeros & 0x7ull) == 0x7ull, "scalar inputs reached 0");
    sim.check(data_ones == ~0ull, "prdata saw every bit high");
    sim.check(data_zeros == ~0ull, "prdata saw every bit low");
    return sim.finish();
}
