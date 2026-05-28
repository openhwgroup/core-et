// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_c2_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

using DUT = Vtxfma_c2_tb;

namespace {

constexpr int kNumInputs = 24;
constexpr int kNumChunks = 1;
constexpr std::array<uint64_t, kNumInputs> kInputMasks = {0xfull, 0x7full, 0x7ffffull, 0x7ull, 0x3ffull, 0x3ffull, 0x3ffull, 0x1full, 0x1full, 0x1full, 0x1full, 0x1full, 0xffull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull};
const char* const kInputNames[kNumInputs] = {"op_dtype_f2a_h", "cmd_f2a_h", "sigs_f2a_h", "rm_f2a_h", "ediff1_f2a_h", "ediff1_abs_f2a_h", "ediff2_abs_f2a_h", "ediff1_sel_f2a_h", "ediff2_sel_f2a_h", "align1_mux_sel_f2a_h", "align2_mux_sel_f2a_h", "opd3_mux_sel_f2a_h", "ec_f2a_h", "ec_zero_f2a_h", "eprod_hi_zero_f2a_h", "fc_b0_f2a_h", "e_sticky1_f2a_h", "e_sticky2_f2a_h", "sc_f2a_h", "sel_fcmask_f2a_h", "prod_sign_hi_f2a_h", "prod_sign_lo_f2a_h", "fc_zero30_8_f2a_h", "minmax_op_f2a_h"};

struct Inputs {
    uint64_t op_dtype_f2a_h = 0;
    uint64_t cmd_f2a_h = 0;
    uint64_t sigs_f2a_h = 0;
    uint64_t rm_f2a_h = 0;
    uint64_t ediff1_f2a_h = 0;
    uint64_t ediff1_abs_f2a_h = 0;
    uint64_t ediff2_abs_f2a_h = 0;
    uint64_t ediff1_sel_f2a_h = 0;
    uint64_t ediff2_sel_f2a_h = 0;
    uint64_t align1_mux_sel_f2a_h = 0;
    uint64_t align2_mux_sel_f2a_h = 0;
    uint64_t opd3_mux_sel_f2a_h = 0;
    uint64_t ec_f2a_h = 0;
    uint64_t ec_zero_f2a_h = 0;
    uint64_t eprod_hi_zero_f2a_h = 0;
    uint64_t fc_b0_f2a_h = 0;
    uint64_t e_sticky1_f2a_h = 0;
    uint64_t e_sticky2_f2a_h = 0;
    uint64_t sc_f2a_h = 0;
    uint64_t sel_fcmask_f2a_h = 0;
    uint64_t prod_sign_hi_f2a_h = 0;
    uint64_t prod_sign_lo_f2a_h = 0;
    uint64_t fc_zero30_8_f2a_h = 0;
    uint64_t minmax_op_f2a_h = 0;
};

struct TestCase {
    const char* name;
    Inputs in;
    std::array<uint64_t, kNumChunks> expected;
};

struct InputCoverage {
    std::array<uint64_t, kNumInputs> ones{};
    std::array<uint64_t, kNumInputs> zeros{};
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t random64(uint32_t& state) {
    const uint64_t lo = xorshift32(state);
    const uint64_t hi = xorshift32(state);
    return (hi << 32) | lo;
}

std::array<uint64_t, kNumInputs> input_values(const Inputs& in) {
    return {in.op_dtype_f2a_h, in.cmd_f2a_h, in.sigs_f2a_h, in.rm_f2a_h, in.ediff1_f2a_h, in.ediff1_abs_f2a_h, in.ediff2_abs_f2a_h, in.ediff1_sel_f2a_h, in.ediff2_sel_f2a_h, in.align1_mux_sel_f2a_h, in.align2_mux_sel_f2a_h, in.opd3_mux_sel_f2a_h, in.ec_f2a_h, in.ec_zero_f2a_h, in.eprod_hi_zero_f2a_h, in.fc_b0_f2a_h, in.e_sticky1_f2a_h, in.e_sticky2_f2a_h, in.sc_f2a_h, in.sel_fcmask_f2a_h, in.prod_sign_hi_f2a_h, in.prod_sign_lo_f2a_h, in.fc_zero30_8_f2a_h, in.minmax_op_f2a_h};
}

void apply_inputs(DUT* dut, const Inputs& in) {
    dut->op_dtype_f2a_h = in.op_dtype_f2a_h & 0xfull;
    dut->cmd_f2a_h = in.cmd_f2a_h & 0x7full;
    dut->sigs_f2a_h = in.sigs_f2a_h & 0x7ffffull;
    dut->rm_f2a_h = in.rm_f2a_h & 0x7ull;
    dut->ediff1_f2a_h = in.ediff1_f2a_h & 0x3ffull;
    dut->ediff1_abs_f2a_h = in.ediff1_abs_f2a_h & 0x3ffull;
    dut->ediff2_abs_f2a_h = in.ediff2_abs_f2a_h & 0x3ffull;
    dut->ediff1_sel_f2a_h = in.ediff1_sel_f2a_h & 0x1full;
    dut->ediff2_sel_f2a_h = in.ediff2_sel_f2a_h & 0x1full;
    dut->align1_mux_sel_f2a_h = in.align1_mux_sel_f2a_h & 0x1full;
    dut->align2_mux_sel_f2a_h = in.align2_mux_sel_f2a_h & 0x1full;
    dut->opd3_mux_sel_f2a_h = in.opd3_mux_sel_f2a_h & 0x1full;
    dut->ec_f2a_h = in.ec_f2a_h & 0xffull;
    dut->ec_zero_f2a_h = in.ec_zero_f2a_h & 0x1ull;
    dut->eprod_hi_zero_f2a_h = in.eprod_hi_zero_f2a_h & 0x1ull;
    dut->fc_b0_f2a_h = in.fc_b0_f2a_h & 0x1ull;
    dut->e_sticky1_f2a_h = in.e_sticky1_f2a_h & 0x1ull;
    dut->e_sticky2_f2a_h = in.e_sticky2_f2a_h & 0x1ull;
    dut->sc_f2a_h = in.sc_f2a_h & 0x1ull;
    dut->sel_fcmask_f2a_h = in.sel_fcmask_f2a_h & 0x1ull;
    dut->prod_sign_hi_f2a_h = in.prod_sign_hi_f2a_h & 0x1ull;
    dut->prod_sign_lo_f2a_h = in.prod_sign_lo_f2a_h & 0x1ull;
    dut->fc_zero30_8_f2a_h = in.fc_zero30_8_f2a_h & 0x1ull;
    dut->minmax_op_f2a_h = in.minmax_op_f2a_h & 0x1ull;
}

Inputs random_inputs(uint32_t& state) {
    return Inputs{(random64(state) & 0xfull), (random64(state) & 0x7full), (random64(state) & 0x7ffffull), (random64(state) & 0x7ull), (random64(state) & 0x3ffull), (random64(state) & 0x3ffull), (random64(state) & 0x3ffull), (random64(state) & 0x1full), (random64(state) & 0x1full), (random64(state) & 0x1full), (random64(state) & 0x1full), (random64(state) & 0x1full), (random64(state) & 0xffull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull)};
}

std::vector<TestCase> directed_cases() {
    return {
        {"zero", Inputs{0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x00140304c0180f00ull}},
        {"ones", Inputs{0xfull, 0x7full, 0x7ffffull, 0x7ull, 0x3ffull, 0x3ffull, 0x3ffull, 0x1full, 0x1full, 0x1full, 0x1full, 0x1full, 0xffull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00040004c0100000ull}},
        {"pattern_a", Inputs{0x6ull, 0x3ull, 0x796e1ull, 0x5ull, 0x311ull, 0x36dull, 0x2dcull, 0x7ull, 0x2ull, 0x14ull, 0x18ull, 0x11ull, 0x12ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00060004c0100800ull}},
        {"pattern_b", Inputs{0x1ull, 0x2eull, 0x2311dull, 0x5ull, 0x392ull, 0x373ull, 0x2a5ull, 0x19ull, 0x9ull, 0x10ull, 0x6ull, 0x15ull, 0x3ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00820004c2001000ull}},
        {"f32_sub_ediff_neg1", Inputs{0x1ull, 0x1ull, 0x40ull, 0x0ull, 0x3ffull, 0x1ull, 0x2ull, 0x6ull, 0x1ull, 0x0ull, 0x1ull, 0x2ull, 0x7full, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x00c40100c2c01500ull}},
        {"int_cvt_signed", Inputs{0x0ull, 0x15ull, 0x2100ull, 0x0ull, 0x1ull, 0x1ull, 0x3ull, 0x2ull, 0x7ull, 0x8ull, 0x9ull, 0xaull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x00c86500c2001001ull}},
        {"sin_transform_negative", Inputs{0x1ull, 0x75ull, 0x10000ull, 0x0ull, 0x3ffull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x7eull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x000403008a181710ull}},
        {"ffrc_mask", Inputs{0x1ull, 0x4cull, 0x20ull, 0x3ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x00140240c0180700ull}},
    };
}

void observe(InputCoverage& cov, const Inputs& in) {
    const auto vals = input_values(in);
    for (int i = 0; i < kNumInputs; ++i) {
        const uint64_t masked = vals[i] & kInputMasks[i];
        cov.ones[i] |= masked;
        cov.zeros[i] |= (~masked) & kInputMasks[i];
    }
}

std::array<uint64_t, kNumChunks> capture_outputs(DUT* dut) {
    return {dut->out_chunk0_o};
}

void check_outputs(SimCtrl<DUT>& sim, const TestCase& tc) {
    const auto got = capture_outputs(sim.dut.get());
    for (int i = 0; i < kNumChunks; ++i) {
        sim.check(got[i] == tc.expected[i], std::string(tc.name) + " output chunk " + std::to_string(i));
    }
}

void check_coverage(SimCtrl<DUT>& sim, const InputCoverage& cov) {
    for (int i = 0; i < kNumInputs; ++i) {
        sim.check(cov.ones[i] == kInputMasks[i], std::string(kInputNames[i]) + " drove all bits high");
        sim.check(cov.zeros[i] == kInputMasks[i], std::string(kInputNames[i]) + " drove all bits low");
    }
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    InputCoverage cov;
    auto cases = directed_cases();
    for (const auto& tc : cases) {
        apply_inputs(sim.dut.get(), tc.in);
        observe(cov, tc.in);
        sim.dut->eval();
        sim.sim_time++;
        check_outputs(sim, tc);
    }

    uint32_t state = 0x13579bdfu;
    for (int i = 0; i < 256; ++i) {
        const Inputs in = random_inputs(state);
        apply_inputs(sim.dut.get(), in);
        observe(cov, in);
        sim.dut->eval();
        const auto first = capture_outputs(sim.dut.get());
        sim.dut->eval();
        const auto second = capture_outputs(sim.dut.get());
        for (int chunk = 0; chunk < kNumChunks; ++chunk) {
            sim.check(first[chunk] == second[chunk], "random deterministic output chunk " + std::to_string(chunk));
        }
    }

    check_coverage(sim, cov);
    return sim.finish();
}
