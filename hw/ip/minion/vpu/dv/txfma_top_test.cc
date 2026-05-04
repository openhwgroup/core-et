// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_top_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <cstdio>

using DUT = Vtxfma_top_tb;

namespace {

constexpr uint64_t kTransCoeffMask = (1ull << 58) - 1ull;
constexpr unsigned kCmdShift = 38;
constexpr unsigned kDtypeShift = 9;
constexpr unsigned kTxfmaBit = 37;
constexpr unsigned kRen3Bit = 18;
constexpr unsigned kRen1Bit = 20;
constexpr unsigned kAddBit = 4;
constexpr unsigned kWflagsBit = 0;

constexpr uint64_t bit(unsigned pos) {
    return 1ull << pos;
}

uint64_t pack_sigs(uint8_t cmd, uint8_t dtype, uint64_t control_bits) {
    return ((static_cast<uint64_t>(cmd) & 0x7full) << kCmdShift) |
           ((static_cast<uint64_t>(dtype) & 0x0full) << kDtypeShift) |
           control_bits;
}

struct Request {
    bool valid = false;
    bool use_prev_sigs = false;
    uint32_t in1 = 0;
    uint32_t in2 = 0;
    uint32_t in3 = 0;
    uint64_t sigs = 0;
    uint32_t imm = 0;
    uint8_t rm = 0;
    uint8_t typ = 0;
    uint64_t coeff = 0;
};

void drive_request(DUT* dut, const Request& req) {
    dut->in_valid_i = req.valid;
    dut->use_prev_sigs_i = req.use_prev_sigs;
    dut->in1_i = req.in1;
    dut->in2_i = req.in2;
    dut->in3_i = req.in3;
    dut->sigs_i = req.sigs;
    dut->imm_i = req.imm;
    dut->rm_i = req.rm;
    dut->typ_i = req.typ;
    dut->trans_coeff_i = req.coeff;
}

void expect_u32(SimCtrl<DUT>& sim, uint32_t got, uint32_t expected, const char* msg) {
    if (got != expected) {
        std::printf("  detail: %s got=0x%08x expected=0x%08x\n", msg, got, expected);
    }
    sim.check(got == expected, msg);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 4000;

    drive_request(sim.dut.get(), {});
    sim.reset();

    sim.check(sim.dut->out_data_o == 0u, "reset leaves TXFMA data output clear");
    sim.check(sim.dut->out_exc_o == 0u, "reset leaves TXFMA exception flags clear");
    sim.check(sim.dut->out_comp_o == 0u, "reset leaves TXFMA compare output clear");

    Request add_req;
    add_req.valid = true;
    add_req.in1 = 0x3f800000u;  // +1.0f
    add_req.in2 = 0x00000000u;
    add_req.in3 = 0x40000000u;  // +2.0f
    add_req.sigs = pack_sigs(0x00, 0x1,
                             bit(kTxfmaBit) | bit(kRen1Bit) | bit(kRen3Bit) |
                             bit(kAddBit) | bit(kWflagsBit));
    add_req.rm = 0x0;
    add_req.imm = 0x12345u;
    add_req.typ = 0x1;
    add_req.coeff = 0x02aa'5555'89ab'cdefull & kTransCoeffMask;
    drive_request(sim.dut.get(), add_req);
    sim.tick();

    drive_request(sim.dut.get(), {});
    for (int i = 0; i < 6; ++i) {
        sim.tick();
    }

    expect_u32(sim, sim.dut->out_data_o, 0x40400000u,
               "fp32 add request produces +3.0 after six TXFMA stages");
    expect_u32(sim, sim.dut->out_exc_o, 0x00u,
               "fp32 add request produces no exception flags");
    expect_u32(sim, sim.dut->out_comp_o, 0x00u,
               "fp32 add request is not a compare operation");

    sim.tick();
    sim.check(sim.dut->out_data_o == 0u, "TXFMA data output is masked when valid drains");
    sim.check(sim.dut->out_exc_o == 0u, "TXFMA exception output is masked when valid drains");
    sim.check(sim.dut->out_comp_o == 0u, "TXFMA compare output stays clear when valid drains");

    return sim.finish();
}
