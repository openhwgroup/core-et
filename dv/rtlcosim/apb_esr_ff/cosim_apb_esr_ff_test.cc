// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_apb_esr_ff_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

namespace {
void set_defaults(CosimCtrl<Vcosim_apb_esr_ff_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_ni = 1;
    d->in0_paddr_i = 0;
    d->in0_pwrite_i = 0;
    d->in0_psel_i = 0;
    d->in0_penable_i = 0;
    d->in0_pwdata_i = 0;
    d->rsp0_pready_i = 0;
    d->rsp0_prdata_i = 0;
    d->rsp0_pslverr_i = 0;
    d->in1_paddr_i = 0;
    d->in1_pwrite_i = 0;
    d->in1_psel_i = 0;
    d->in1_penable_i = 0;
    d->in1_pwdata_i = 0;
    d->rsp1_pready_i = 0;
    d->rsp1_prdata_i = 0;
    d->rsp1_pslverr_i = 0;
}

void reset(CosimCtrl<Vcosim_apb_esr_ff_tb>& sim) {
    sim.dut->rst_ni = 0;
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.dut->rst_ni = 1;
    sim.tick();
}

void compare_all(CosimCtrl<Vcosim_apb_esr_ff_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("out0_paddr", d->orig_out0_paddr_o, d->new_out0_paddr_o);
    sim.compare("out0_pwrite", d->orig_out0_pwrite_o, d->new_out0_pwrite_o);
    sim.compare("out0_psel", d->orig_out0_psel_o, d->new_out0_psel_o);
    sim.compare("out0_penable", d->orig_out0_penable_o, d->new_out0_penable_o);
    sim.compare("out0_pwdata", d->orig_out0_pwdata_o, d->new_out0_pwdata_o);
    sim.compare("in0_pready", d->orig_in0_pready_o, d->new_in0_pready_o);
    sim.compare("in0_prdata", d->orig_in0_prdata_o, d->new_in0_prdata_o);
    sim.compare("in0_pslverr", d->orig_in0_pslverr_o, d->new_in0_pslverr_o);
    sim.compare("out1_paddr", d->orig_out1_paddr_o, d->new_out1_paddr_o);
    sim.compare("out1_pwrite", d->orig_out1_pwrite_o, d->new_out1_pwrite_o);
    sim.compare("out1_psel", d->orig_out1_psel_o, d->new_out1_psel_o);
    sim.compare("out1_penable", d->orig_out1_penable_o, d->new_out1_penable_o);
    sim.compare("out1_pwdata", d->orig_out1_pwdata_o, d->new_out1_pwdata_o);
    sim.compare("in1_pready", d->orig_in1_pready_o, d->new_in1_pready_o);
    sim.compare("in1_prdata", d->orig_in1_prdata_o, d->new_in1_prdata_o);
    sim.compare("in1_pslverr", d->orig_in1_pslverr_o, d->new_in1_pslverr_o);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_apb_esr_ff_tb> sim(argc, argv);
    set_defaults(sim);
    reset(sim);
    compare_all(sim);

    auto* d = sim.dut.get();
    std::mt19937_64 rng(0x5eed1234);
    for (int cycle = 0; cycle < 3000; ++cycle) {
        const uint64_t r0 = rng();
        const uint64_t r1 = rng();
        d->in0_paddr_i = r0 & 0xffffu;
        d->in0_pwrite_i = (r0 >> 16) & 1u;
        d->in0_psel_i = (r0 >> 17) & 1u;
        d->in0_penable_i = (r0 >> 18) & 1u;
        d->in0_pwdata_i = rng();
        d->rsp0_pready_i = (r0 >> 19) & 1u;
        d->rsp0_prdata_i = rng();
        d->rsp0_pslverr_i = (r0 >> 20) & 1u;
        d->in1_paddr_i = r1 & 0xffffu;
        d->in1_pwrite_i = (r1 >> 16) & 1u;
        d->in1_psel_i = (r1 >> 17) & 1u;
        d->in1_penable_i = (r1 >> 18) & 1u;
        d->in1_pwdata_i = rng();
        d->rsp1_pready_i = (r1 >> 19) & 1u;
        d->rsp1_prdata_i = rng();
        d->rsp1_pslverr_i = (r1 >> 20) & 1u;
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}
