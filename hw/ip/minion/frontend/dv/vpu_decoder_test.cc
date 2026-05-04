// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_decoder_tb.h"
#include "sim_ctrl.h"

#include <cstdio>

using DUT = Vvpu_decoder_tb;

namespace {

constexpr uint8_t kCmdNone          = 0x00;
constexpr uint8_t kCmdFfrcSin       = 0x51;
constexpr uint8_t kCmdTransRcpRr    = 0x62;
constexpr uint8_t kCmdTransRsqrtRr  = 0x65;
constexpr uint8_t kCmdTransLogRr    = 0x71;
constexpr uint8_t kCmdTransExpFrac  = 0x7A;

void check_decode(SimCtrl<DUT>& sim, uint32_t inst, bool expect_vpu,
                  uint8_t expect_cmd, const char* name) {
    sim.dut->inst_i = inst;
    sim.dut->eval();

    char msg[256];
    std::snprintf(
        msg, sizeof(msg),
        "%s: inst=0x%08x vpu=%d/%d cmd=0x%02x/0x%02x extra=%d",
        name, inst,
        sim.dut->is_vpu_o, expect_vpu,
        sim.dut->cmd_o, expect_cmd,
        sim.dut->enable_extra_trans_o
    );

    sim.check(
        sim.dut->is_vpu_o == expect_vpu &&
        sim.dut->cmd_o == expect_cmd,
        msg
    );
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.reset();

    const bool extra = sim.dut->enable_extra_trans_o;

    check_decode(sim, 0x00000013, false, kCmdNone,         "NOP");
    check_decode(sim, 0x5870007B, true,  kCmdTransRcpRr,   "FRCP_PS");
    check_decode(sim, 0x5830007B, true,  kCmdTransLogRr,   "FLOG_PS");
    check_decode(sim, 0x5840007B, true,  kCmdTransExpFrac, "FEXP_PS");
    check_decode(sim, 0x5880007B,
                 extra,
                 extra ? kCmdTransRsqrtRr : kCmdNone,
                 "FRSQ_PS");
    check_decode(sim, 0x5860007B,
                 extra,
                 extra ? kCmdFfrcSin : kCmdNone,
                 "FSIN_PS");

    return sim.finish();
}
