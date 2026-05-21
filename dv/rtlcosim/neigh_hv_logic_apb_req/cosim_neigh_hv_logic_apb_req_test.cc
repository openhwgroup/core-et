// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_apb_req_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>
#include <string>

namespace {

struct BitCov {
    uint64_t ones = 0;
    uint64_t zeros = 0;
    void sample(uint64_t v, unsigned bit) {
        const uint64_t m = 1ull << bit;
        ones |= v ? m : 0ull;
        zeros |= v ? 0ull : m;
    }
};

void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_apb_req_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("apb_esr_req_vcfifo_push", d->orig_apb_esr_req_vcfifo_push_o,
                d->new_apb_esr_req_vcfifo_push_o);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_apb_req_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->apb_pready_neigh_i = 0;
    d->apb_esr_req_apb_psel_i = 0;
    d->apb_esr_req_apb_penable_i = 0;
    sim.reset();

    BitCov cov;
    std::mt19937 rng(0x41504201u);
    for (int cycle = 0; cycle < 4096; ++cycle) {
        if (cycle == 64) d->rst_ni = 0;
        if (cycle == 66) d->rst_ni = 1;
        d->apb_pready_neigh_i = (cycle % 7 == 0) || ((rng() >> 0) & 1u);
        d->apb_esr_req_apb_psel_i = (cycle % 5 == 0) || ((rng() >> 1) & 1u);
        d->apb_esr_req_apb_penable_i = (cycle % 3 == 0) || ((rng() >> 2) & 1u);
        cov.sample(d->rst_ni, 0);
        cov.sample(d->apb_pready_neigh_i, 1);
        cov.sample(d->apb_esr_req_apb_psel_i, 2);
        cov.sample(d->apb_esr_req_apb_penable_i, 3);
        sim.tick();
        compare_all(sim);
    }

    sim.check((cov.ones & 0xfu) == 0xfu, "all inputs reached 1");
    sim.check((cov.zeros & 0xfu) == 0xfu, "all inputs reached 0");
    return sim.finish();
}
