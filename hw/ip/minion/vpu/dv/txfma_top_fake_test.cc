// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_top_fake_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>

#include "svdpi.h"

using DUT = Vtxfma_top_fake_tb;

namespace {

constexpr uint64_t kTransCoeffMask = (1ull << 58) - 1ull;
constexpr unsigned kCmdShift = 38;
constexpr unsigned kDtypeShift = 9;
constexpr unsigned kTxfmaBit = 37;
constexpr unsigned kRen3Bit = 18;
constexpr unsigned kRen2Bit = 19;
constexpr unsigned kRen1Bit = 20;
constexpr unsigned kFmaBit = 6;
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

uint8_t sigs_cmd(uint64_t sigs) {
    return static_cast<uint8_t>((sigs >> kCmdShift) & 0x7full);
}

uint8_t sigs_dtype(uint64_t sigs) {
    return static_cast<uint8_t>((sigs >> kDtypeShift) & 0x0full);
}

uint32_t txfma_emu_model(uint32_t src1, uint32_t src2, uint32_t src3,
                         uint8_t cmd, uint8_t dtype, uint8_t rm,
                         uint8_t* out_flags) {
    uint32_t result = 0;
    switch (cmd) {
    case 0x00:
        result = src1 + src3;
        break;
    case 0x01:
        result = src1 - src3;
        break;
    case 0x02:
        result = src1 ^ ((src2 << 1) | (src2 >> 31));
        break;
    case 0x07:
        result = src1 + (src2 ^ 0x9e3779b9u) + src3;
        break;
    default: {
        const uint32_t rot = ((src2 << (rm & 31u)) | (src2 >> ((32u - rm) & 31u)));
        result = src1 ^ rot ^ (src3 + static_cast<uint32_t>(cmd)) ^
                 (static_cast<uint32_t>(dtype) << 28);
        break;
    }
    }

    const uint8_t flags = static_cast<uint8_t>(
        (cmd ^ dtype ^ rm ^
         static_cast<uint8_t>(src1) ^
         static_cast<uint8_t>(src2 >> 8) ^
         static_cast<uint8_t>(src3 >> 16) ^
         static_cast<uint8_t>(result >> 5)) &
        0x3fu);

    if (out_flags) {
        *out_flags = flags;
    }
    return result;
}

}  // namespace

extern "C" int dpi_TXFMA_EMU(svBitVecVal* src1, svBitVecVal* src2, svBitVecVal* src3,
                             svBitVecVal* cmd, svBitVecVal* dtype, svBitVecVal* rm,
                             svBitVecVal* out_flags) {
    uint8_t flags = 0;
    const uint32_t result = txfma_emu_model(src1[0], src2[0], src3[0],
                                            static_cast<uint8_t>(cmd[0] & 0x7fu),
                                            static_cast<uint8_t>(dtype[0] & 0x0fu),
                                            static_cast<uint8_t>(rm[0] & 0x07u),
                                            &flags);
    out_flags[0] = flags;
    return static_cast<int32_t>(result);
}

namespace {

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

struct Model {
    std::array<Request, 7> pipe{};
    uint32_t out_data = 0;
    uint8_t out_exc = 0;

    void step(const Request& in) {
        if (pipe.back().valid) {
            out_data = txfma_emu_model(pipe.back().in1, pipe.back().in2, pipe.back().in3,
                                       sigs_cmd(pipe.back().sigs), sigs_dtype(pipe.back().sigs),
                                       pipe.back().rm, &out_exc);
        }
        for (size_t i = pipe.size() - 1; i > 0; --i) {
            pipe[i] = pipe[i - 1];
        }
        pipe[0] = in;
    }
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

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000;

    drive_request(sim.dut.get(), {});
    sim.reset();

    Model model;

    Request req;
    req.valid = true;
    req.in1 = 0x01020304u;
    req.in2 = 0x11121314u;
    req.in3 = 0x21222324u;
    req.sigs = pack_sigs(0x00, 0x1,
                         bit(kTxfmaBit) | bit(kRen1Bit) | bit(kRen3Bit) |
                         bit(kAddBit) | bit(kWflagsBit));
    req.rm = 0x2;
    req.imm = 0xabcdeu;
    req.typ = 0x1;
    req.coeff = kTransCoeffMask;
    drive_request(sim.dut.get(), req);
    sim.tick();
    model.step(req);
    sim.check(sim.dut->out_comp_o == 0, "out_comp stays low on first request");

    drive_request(sim.dut.get(), {});
    for (int i = 0; i < 7; ++i) {
        sim.tick();
        model.step({});
    }

    sim.check(sim.dut->out_data_o == model.out_data, "add-class request appears after seven stages");
    sim.check(sim.dut->out_exc_o == model.out_exc, "flags follow add-class request");
    sim.check(sim.dut->out_comp_o == 0, "out_comp remains hardwired low");

    req = {};
    req.valid = true;
    req.in1 = 0x89abcdefu;
    req.in2 = 0x10203040u;
    req.in3 = 0x55667788u;
    req.sigs = pack_sigs(0x07, 0xe,
                         bit(kTxfmaBit) | bit(kRen1Bit) | bit(kRen2Bit) |
                         bit(kRen3Bit) | bit(kFmaBit) | bit(kWflagsBit));
    req.rm = 0x5;
    req.imm = 0x54321u;
    req.typ = 0x2;
    req.coeff = 0x0155'aaaa'1234'5678ull & kTransCoeffMask;
    drive_request(sim.dut.get(), req);
    sim.tick();
    model.step(req);

    drive_request(sim.dut.get(), {});
    for (int i = 0; i < 7; ++i) {
        sim.tick();
        model.step({});
    }

    sim.check(sim.dut->out_data_o == model.out_data, "fma-class request appears after seven stages");
    sim.check(sim.dut->out_exc_o == model.out_exc, "flags follow fma-class request");

    const uint32_t held_data = sim.dut->out_data_o;
    const uint32_t held_exc = sim.dut->out_exc_o;
    for (int i = 0; i < 4; ++i) {
        sim.tick();
        model.step({});
        sim.check(sim.dut->out_data_o == held_data, "output data holds with no new valid request");
        sim.check(sim.dut->out_exc_o == held_exc, "output flags hold with no new valid request");
        sim.check(sim.dut->out_comp_o == 0, "out_comp stays low while idle");
    }

    return sim.finish();
}
