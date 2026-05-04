// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_sh_sw_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_vpu_sh_sw_tb;

namespace {

constexpr uint8_t kDtypeInt = 0x0;
constexpr uint8_t kDtypeF32 = 0x1;
constexpr uint8_t kDtypeF16 = 0x2;

constexpr uint8_t kCmdAdd          = 0x00;
constexpr uint8_t kCmdSub          = 0x01;
constexpr uint8_t kCmdMvXf         = 0x14;
constexpr uint8_t kCmdNot          = 0x18;
constexpr uint8_t kCmdAnd          = 0x19;
constexpr uint8_t kCmdOr           = 0x1a;
constexpr uint8_t kCmdXor          = 0x1b;
constexpr uint8_t kCmdSll          = 0x1c;
constexpr uint8_t kCmdSrl          = 0x1d;
constexpr uint8_t kCmdSra          = 0x1e;
constexpr uint8_t kCmdFeq          = 0x1f;
constexpr uint8_t kCmdFle          = 0x20;
constexpr uint8_t kCmdFlt          = 0x21;
constexpr uint8_t kCmdFltu         = 0x22;
constexpr uint8_t kCmdFsetm        = 0x27;
constexpr uint8_t kCmdFcmov        = 0x2d;
constexpr uint8_t kCmdFcmovm       = 0x2e;
constexpr uint8_t kCmdFswizz       = 0x2f;
constexpr uint8_t kCmdMvzFx        = 0x30;
constexpr uint8_t kCmdMvsFx        = 0x31;
constexpr uint8_t kCmdAddi         = 0x4e;
constexpr uint8_t kCmdAndi         = 0x4f;
constexpr uint8_t kCmdSlli         = 0x50;
constexpr uint8_t kCmdFbci         = 0x52;
constexpr uint8_t kCmdSrli         = 0x53;
constexpr uint8_t kCmdSrai         = 0x54;
constexpr uint8_t kCmdFbc          = 0x55;
constexpr uint8_t kCmdFsgnjn       = 0x59;
constexpr uint8_t kCmdFsgnjx       = 0x5a;
constexpr uint8_t kCmdMin          = 0x0f;
constexpr uint8_t kCmdMax          = 0x10;
constexpr uint8_t kCmdMinu         = 0x5f;
constexpr uint8_t kCmdMaxu         = 0x60;
constexpr uint8_t kCmdFsatu8       = 0x61;
constexpr uint8_t kCmdCubeface     = 0x69;
constexpr uint8_t kCmdCubefaceIdx  = 0x6a;
constexpr uint8_t kCmdCubesgnsc    = 0x6b;
constexpr uint8_t kCmdCubesgntc    = 0x6c;
constexpr uint8_t kCmdPackrepb     = 0x6d;
constexpr uint8_t kCmdPackreph     = 0x6e;
constexpr uint8_t kCmdTransLogFma1 = 0x72;
constexpr uint8_t kCmdTransLogFma2 = 0x73;
constexpr uint8_t kCmdTransLogMul  = 0x74;
constexpr uint8_t kCmdFsgnj        = 0x0e;
constexpr uint8_t kCmdFsat8        = 0x7e;
constexpr int kRandomIters = 10000;

struct PortSeen {
    bool saw_zero = false;
    bool saw_nonzero = false;

    void sample(uint64_t value) {
        if (value == 0) {
            saw_zero = true;
        } else {
            saw_nonzero = true;
        }
    }
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

constexpr std::array<uint8_t, 46> kStimulusCmds = {{
    kCmdSll, kCmdSlli, kCmdSrl, kCmdSra, kCmdSrli, kCmdSrai,
    kCmdFeq, kCmdFle, kCmdFlt, kCmdFltu,
    kCmdFbci, kCmdFbci,
    kCmdMin, kCmdMax, kCmdMinu, kCmdMaxu,
    kCmdAnd, kCmdAndi, kCmdOr, kCmdXor, kCmdNot,
    kCmdFcmov, kCmdFcmovm, kCmdFswizz, kCmdPackrepb, kCmdPackreph,
    kCmdFsetm, kCmdCubeface, kCmdCubefaceIdx, kCmdCubesgnsc, kCmdCubesgntc,
    kCmdMvzFx, kCmdMvsFx, kCmdFbc,
    kCmdMvXf, kCmdTransLogFma1, kCmdTransLogFma2, kCmdTransLogMul,
    kCmdFsgnj, kCmdFsgnjn, kCmdFsgnjx,
    kCmdFsat8, kCmdFsatu8,
    kCmdAdd, kCmdAddi, kCmdSub
}};

constexpr std::array<uint32_t, 4> kPatterns = {{
    0x00000000u,
    0xffffffffu,
    0xaaaaaaaau,
    0x55555555u,
}};

void set_defaults(DUT* dut) {
    dut->lane_id_i = 0;
    dut->ex_in_valid_i = 0;
    dut->ex_cmd_i = 0;
    dut->ex_in1_i = 0;
    dut->ex_in2_i = 0;
    dut->ex_in3_i = 0;
    dut->ex_imm_i = 0;
    dut->ex_dtype_i = kDtypeInt;
    dut->ex_mask_in1_i = 0;
    dut->f2_swizz_in1_i = 0;
    dut->f2_swizz_in2_i = 0;
    dut->f2_swizz_in3_i = 0;
}

void compare_all(CosimCtrl<DUT>& sim) {
    sim.compare("f2_fswizz_rdata", sim.dut->orig_f2_fswizz_rdata_o, sim.dut->new_f2_fswizz_rdata_o);
    sim.compare("f3_out_data", sim.dut->orig_f3_out_data_o, sim.dut->new_f3_out_data_o);
    sim.compare("f3_out_exc", sim.dut->orig_f3_out_exc_o, sim.dut->new_f3_out_exc_o);
}

void sample_inputs(const DUT* dut,
                   PortSeen& lane_id_seen,
                   PortSeen& ex_in_valid_seen,
                   PortSeen& ex_cmd_seen,
                   PortSeen& ex_in1_seen,
                   PortSeen& ex_in2_seen,
                   PortSeen& ex_in3_seen,
                   PortSeen& ex_imm_seen,
                   PortSeen& ex_dtype_seen,
                   PortSeen& ex_mask_seen,
                   PortSeen& sw1_seen,
                   PortSeen& sw2_seen,
                   PortSeen& sw3_seen) {
    lane_id_seen.sample(dut->lane_id_i);
    ex_in_valid_seen.sample(dut->ex_in_valid_i);
    ex_cmd_seen.sample(dut->ex_cmd_i);
    ex_in1_seen.sample(dut->ex_in1_i);
    ex_in2_seen.sample(dut->ex_in2_i);
    ex_in3_seen.sample(dut->ex_in3_i);
    ex_imm_seen.sample(dut->ex_imm_i);
    ex_dtype_seen.sample(dut->ex_dtype_i);
    ex_mask_seen.sample(dut->ex_mask_in1_i);
    sw1_seen.sample(dut->f2_swizz_in1_i);
    sw2_seen.sample(dut->f2_swizz_in2_i);
    sw3_seen.sample(dut->f2_swizz_in3_i);
}

void drive_stimulus(DUT* dut, uint32_t& seed, uint8_t cmd, bool valid, bool fbci_f32) {
    dut->lane_id_i = xorshift32(seed) & 0x7u;
    dut->ex_in_valid_i = valid;
    dut->ex_cmd_i = cmd;
    dut->ex_in1_i = xorshift32(seed);
    dut->ex_in2_i = xorshift32(seed);
    dut->ex_in3_i = xorshift32(seed);
    dut->ex_imm_i = xorshift32(seed) & 0xfffffu;
    dut->ex_dtype_i = fbci_f32 ? kDtypeF32 : (xorshift32(seed) % 3u == 0 ? kDtypeF16 : kDtypeInt);
    dut->ex_mask_in1_i = xorshift32(seed) & 0x1u;
    dut->f2_swizz_in1_i = xorshift32(seed);
    dut->f2_swizz_in2_i = xorshift32(seed);
    dut->f2_swizz_in3_i = xorshift32(seed);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    set_defaults(sim.dut.get());
    sim.reset(4);

    PortSeen lane_id_seen;
    PortSeen ex_in_valid_seen;
    PortSeen ex_cmd_seen;
    PortSeen ex_in1_seen;
    PortSeen ex_in2_seen;
    PortSeen ex_in3_seen;
    PortSeen ex_imm_seen;
    PortSeen ex_dtype_seen;
    PortSeen ex_mask_seen;
    PortSeen sw1_seen;
    PortSeen sw2_seen;
    PortSeen sw3_seen;

    std::array<bool, kStimulusCmds.size()> saw_cmd = {};
    uint32_t seed = 0x51ee1234u;

    drive_stimulus(sim.dut.get(), seed, kCmdAdd, true, false);
    sample_inputs(sim.dut.get(), lane_id_seen, ex_in_valid_seen, ex_cmd_seen, ex_in1_seen, ex_in2_seen,
                  ex_in3_seen, ex_imm_seen, ex_dtype_seen, ex_mask_seen, sw1_seen, sw2_seen, sw3_seen);
    sim.tick();
    sim.compare("f2_fswizz_rdata", sim.dut->orig_f2_fswizz_rdata_o, sim.dut->new_f2_fswizz_rdata_o);

    set_defaults(sim.dut.get());
    sim.dut->ex_in_valid_i = 0;
    sample_inputs(sim.dut.get(), lane_id_seen, ex_in_valid_seen, ex_cmd_seen, ex_in1_seen, ex_in2_seen,
                  ex_in3_seen, ex_imm_seen, ex_dtype_seen, ex_mask_seen, sw1_seen, sw2_seen, sw3_seen);
    sim.tick();
    compare_all(sim);

    for (size_t i = 0; i < kStimulusCmds.size(); ++i) {
        const uint8_t cmd = kStimulusCmds[i];
        const bool fbci_f32 = (cmd == kCmdFbci) && (i & 1u);
        drive_stimulus(sim.dut.get(), seed, cmd, true, fbci_f32);
        sample_inputs(sim.dut.get(), lane_id_seen, ex_in_valid_seen, ex_cmd_seen, ex_in1_seen, ex_in2_seen,
                      ex_in3_seen, ex_imm_seen, ex_dtype_seen, ex_mask_seen, sw1_seen, sw2_seen, sw3_seen);
        saw_cmd[i] = true;
        sim.tick();
        compare_all(sim);
    }

    for (size_t i = 0; i < kStimulusCmds.size(); ++i) {
        const uint8_t cmd = kStimulusCmds[i];
        for (size_t p = 0; p < kPatterns.size(); ++p) {
            drive_stimulus(sim.dut.get(), seed, cmd, true, false);
            sim.dut->lane_id_i = (i + p) & 0x7u;
            sim.dut->ex_in1_i = kPatterns[p];
            sim.dut->ex_in2_i = kPatterns[(p + 1) % kPatterns.size()];
            sim.dut->ex_in3_i = kPatterns[(p + 2) % kPatterns.size()];
            sim.dut->ex_imm_i = kPatterns[(p + 3) % kPatterns.size()] & 0xfffffu;
            sim.dut->ex_dtype_i = (p & 1u) ? kDtypeF16 : kDtypeInt;
            sim.dut->ex_mask_in1_i = p & 1u;
            sim.dut->f2_swizz_in1_i = kPatterns[(p + 2) % kPatterns.size()];
            sim.dut->f2_swizz_in2_i = kPatterns[(p + 3) % kPatterns.size()];
            sim.dut->f2_swizz_in3_i = kPatterns[p];
            sample_inputs(sim.dut.get(), lane_id_seen, ex_in_valid_seen, ex_cmd_seen, ex_in1_seen, ex_in2_seen,
                          ex_in3_seen, ex_imm_seen, ex_dtype_seen, ex_mask_seen, sw1_seen, sw2_seen, sw3_seen);
            sim.tick();
            compare_all(sim);
        }
    }

    for (int i = 0; i < kRandomIters; ++i) {
        const size_t idx = xorshift32(seed) % kStimulusCmds.size();
        const uint8_t cmd = kStimulusCmds[idx];
        const bool valid = (xorshift32(seed) & 0x3u) != 0;
        const bool fbci_f32 = (cmd == kCmdFbci) && (xorshift32(seed) & 0x1u);
        drive_stimulus(sim.dut.get(), seed, cmd, valid, fbci_f32);
        sample_inputs(sim.dut.get(), lane_id_seen, ex_in_valid_seen, ex_cmd_seen, ex_in1_seen, ex_in2_seen,
                      ex_in3_seen, ex_imm_seen, ex_dtype_seen, ex_mask_seen, sw1_seen, sw2_seen, sw3_seen);
        sim.tick();
        compare_all(sim);
    }

    set_defaults(sim.dut.get());
    sim.dut->ex_in_valid_i = 0;
    sample_inputs(sim.dut.get(), lane_id_seen, ex_in_valid_seen, ex_cmd_seen, ex_in1_seen, ex_in2_seen,
                  ex_in3_seen, ex_imm_seen, ex_dtype_seen, ex_mask_seen, sw1_seen, sw2_seen, sw3_seen);
    sim.tick();
    compare_all(sim);

    for (size_t i = 0; i < saw_cmd.size(); ++i) {
        sim.check(saw_cmd[i], std::string("directed phase should cover command index ") + std::to_string(i));
    }

    sim.check(lane_id_seen.saw_zero && lane_id_seen.saw_nonzero, "lane_id_i should see zero and non-zero values");
    sim.check(ex_in_valid_seen.saw_zero && ex_in_valid_seen.saw_nonzero, "ex_in_valid_i should toggle");
    sim.check(ex_cmd_seen.saw_zero && ex_cmd_seen.saw_nonzero, "ex_cmd_i should see zero and non-zero values");
    sim.check(ex_in1_seen.saw_zero && ex_in1_seen.saw_nonzero, "ex_in1_i should see zero and non-zero values");
    sim.check(ex_in2_seen.saw_zero && ex_in2_seen.saw_nonzero, "ex_in2_i should see zero and non-zero values");
    sim.check(ex_in3_seen.saw_zero && ex_in3_seen.saw_nonzero, "ex_in3_i should see zero and non-zero values");
    sim.check(ex_imm_seen.saw_zero && ex_imm_seen.saw_nonzero, "ex_imm_i should see zero and non-zero values");
    sim.check(ex_dtype_seen.saw_zero && ex_dtype_seen.saw_nonzero, "ex_dtype_i should see zero and non-zero values");
    sim.check(ex_mask_seen.saw_zero && ex_mask_seen.saw_nonzero, "ex_mask_in1_i should toggle");
    sim.check(sw1_seen.saw_zero && sw1_seen.saw_nonzero, "f2_swizz_in1_i should see zero and non-zero values");
    sim.check(sw2_seen.saw_zero && sw2_seen.saw_nonzero, "f2_swizz_in2_i should see zero and non-zero values");
    sim.check(sw3_seen.saw_zero && sw3_seen.saw_nonzero, "f2_swizz_in3_i should see zero and non-zero values");
    sim.check(sim.comparisons >= 30000, "vpu_sh_sw cosim should exceed 30k comparisons");

    return sim.finish();
}
