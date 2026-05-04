// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vintpipe_decode_tb.h"
#include "sim_ctrl.h"

#include <cstdio>
#include <string>

using DUT = Vintpipe_decode_tb;

namespace {

struct Expect {
    bool legal;
    bool mcode;
    bool fp;
};

void check_decode(SimCtrl<DUT>& sim, uint32_t inst, Expect expect, const char* name) {
    sim.dut->inst_i = inst;
    sim.dut->eval();

    char msg[256];
    std::snprintf(
        msg, sizeof(msg),
        "%s: inst=0x%08x legal=%d/%d mcode=%d/%d fp=%d/%d extra=%d",
        name, inst,
        sim.dut->legal_o, expect.legal,
        sim.dut->mcode_o, expect.mcode,
        sim.dut->fp_o, expect.fp,
        sim.dut->enable_extra_trans_o
    );

    sim.check(
        sim.dut->legal_o == expect.legal &&
        sim.dut->mcode_o == expect.mcode &&
        sim.dut->fp_o == expect.fp,
        msg
    );
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.reset();

    const bool extra = sim.dut->enable_extra_trans_o;

    check_decode(sim, 0x00000013, {true, false, false}, "NOP");
    check_decode(sim, 0x5870007B, {true, false, true},  "FRCP_PS");
    check_decode(sim, 0x5830007B, {true, false, true},  "FLOG_PS");
    check_decode(sim, 0x5840007B, {true, false, true},  "FEXP_PS");
    check_decode(sim, 0x5880007B,
                 extra ? Expect{true, false, true} : Expect{true, true, false},
                 "FRSQ_PS");
    check_decode(sim, 0x5860007B,
                 extra ? Expect{true, false, true} : Expect{true, true, false},
                 "FSIN_PS");

    return sim.finish();
}
