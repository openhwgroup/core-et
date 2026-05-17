// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vapb_esr_ff_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

namespace {
void set_defaults(SimCtrl<Vapb_esr_ff_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_ni = 1;
    d->in0_paddr_i = 0;
    d->in0_pwrite_i = 0;
    d->in0_psel_i = 0;
    d->in0_penable_i = 0;
    d->in0_pwdata_i = 0;
    d->out0_pready_i = 0;
    d->out0_prdata_i = 0;
    d->out0_pslverr_i = 0;
    d->in1_paddr_i = 0;
    d->in1_pwrite_i = 0;
    d->in1_psel_i = 0;
    d->in1_penable_i = 0;
    d->in1_pwdata_i = 0;
    d->out1_pready_i = 0;
    d->out1_prdata_i = 0;
    d->out1_pslverr_i = 0;
}

void reset(SimCtrl<Vapb_esr_ff_tb>& sim) {
    sim.dut->rst_ni = 0;
    for (int i = 0; i < 3; ++i) sim.tick();
    sim.dut->rst_ni = 1;
    sim.tick();
}
}  // namespace

int main(int argc, char** argv) {
    SimCtrl<Vapb_esr_ff_tb> sim(argc, argv);
    set_defaults(sim);
    reset(sim);
    auto* d = sim.dut.get();

    sim.check(d->out0_psel_o == 0, "interface 0 idle psel");
    sim.check(d->in0_pready_o == 0, "interface 0 idle pready");
    sim.check(d->out1_psel_o == 0, "interface 1 idle psel");

    d->in0_paddr_i = 0x1234;
    d->in0_pwrite_i = 1;
    d->in0_pwdata_i = 0xdeadbeef12345678ull;
    d->in0_psel_i = 1;
    d->in0_penable_i = 0;
    sim.tick();
    sim.check(d->out0_paddr_o == 0x1234, "setup captures paddr");
    sim.check(d->out0_pwrite_o == 1, "setup captures pwrite");
    sim.check(d->out0_pwdata_o == 0xdeadbeef12345678ull, "setup captures pwdata");
    sim.check(d->out0_psel_o == 1, "setup drives downstream psel");
    sim.check(d->out0_penable_o == 0, "setup does not drive penable");

    d->in0_penable_i = 1;
    sim.tick();
    sim.check(d->out0_psel_o == 1, "exec keeps psel asserted");
    sim.check(d->out0_penable_o == 1, "exec drives penable");
    sim.check(d->in0_pready_o == 0, "exec waits for downstream ready");

    d->out0_prdata_i = 0xfeedfacecafebeefull;
    d->out0_pslverr_i = 1;
    d->out0_pready_i = 1;
    sim.tick();
    sim.check(d->in0_pready_o == 1, "ready cycle returns pready");
    sim.check(d->in0_prdata_o == 0xfeedfacecafebeefull, "ready cycle returns prdata");
    sim.check(d->in0_pslverr_o == 1, "ready cycle returns pslverr");
    sim.check(d->out0_psel_o == 0, "ready cycle deasserts downstream psel");

    d->in0_psel_i = 0;
    d->in0_penable_i = 0;
    d->out0_pready_i = 0;
    sim.tick();
    sim.check(d->in0_pready_o == 0, "done cycle clears pready after one pulse");

    d->in1_paddr_i = 0x00a0;
    d->in1_pwdata_i = 0x1111222233334444ull;
    d->in1_pwrite_i = 1;
    d->in1_psel_i = 1;
    d->in1_penable_i = 0;
    sim.tick();
    d->in1_penable_i = 1;
    d->out1_pready_i = 1;
    d->out1_prdata_i = 0x9999000011112222ull;
    sim.tick();
    sim.tick();
    sim.check(d->in1_pready_o == 1, "interface 1 independent transaction completes");
    sim.check(d->in1_prdata_o == 0x9999000011112222ull, "interface 1 captures response");
    sim.check(d->out0_psel_o == 0, "interface 0 remains idle during interface 1 transaction");

    return sim.finish();
}
