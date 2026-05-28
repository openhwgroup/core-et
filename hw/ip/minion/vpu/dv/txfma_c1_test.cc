// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_c1_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

using DUT = Vtxfma_c1_tb;

namespace {

constexpr int kNumInputs = 16;
constexpr int kNumChunks = 1;
constexpr std::array<uint64_t, kNumInputs> kInputMasks = {0x1ull, 0x7ffffull, 0x7full, 0xfull, 0xffull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull};
const char* const kInputNames[kNumInputs] = {"ctrl_valid_f1a_h", "sigs_f1a_h", "cmd_in_f1a_h", "op_dtype_f1a_h", "ea_hi_f1a_h", "fa_hi_zero_f1a_h", "sa_hi_f1a_h", "sb_hi_f1a_h", "sc_f1a_h", "sa_lo_f1a_h", "sb_lo_f1a_h", "ea_hi_zero_f1a_h", "eb_hi_zero_f1a_h", "ea_lo_zero_f1a_h", "eb_lo_zero_f1a_h", "flog_mul_prod_zero_f1a_h"};

struct Inputs {
    uint64_t ctrl_valid_f1a_h = 0;
    uint64_t sigs_f1a_h = 0;
    uint64_t cmd_in_f1a_h = 0;
    uint64_t op_dtype_f1a_h = 0;
    uint64_t ea_hi_f1a_h = 0;
    uint64_t fa_hi_zero_f1a_h = 0;
    uint64_t sa_hi_f1a_h = 0;
    uint64_t sb_hi_f1a_h = 0;
    uint64_t sc_f1a_h = 0;
    uint64_t sa_lo_f1a_h = 0;
    uint64_t sb_lo_f1a_h = 0;
    uint64_t ea_hi_zero_f1a_h = 0;
    uint64_t eb_hi_zero_f1a_h = 0;
    uint64_t ea_lo_zero_f1a_h = 0;
    uint64_t eb_lo_zero_f1a_h = 0;
    uint64_t flog_mul_prod_zero_f1a_h = 0;
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
    return {in.ctrl_valid_f1a_h, in.sigs_f1a_h, in.cmd_in_f1a_h, in.op_dtype_f1a_h, in.ea_hi_f1a_h, in.fa_hi_zero_f1a_h, in.sa_hi_f1a_h, in.sb_hi_f1a_h, in.sc_f1a_h, in.sa_lo_f1a_h, in.sb_lo_f1a_h, in.ea_hi_zero_f1a_h, in.eb_hi_zero_f1a_h, in.ea_lo_zero_f1a_h, in.eb_lo_zero_f1a_h, in.flog_mul_prod_zero_f1a_h};
}

void apply_inputs(DUT* dut, const Inputs& in) {
    dut->ctrl_valid_f1a_h = in.ctrl_valid_f1a_h & 0x1ull;
    dut->sigs_f1a_h = in.sigs_f1a_h & 0x7ffffull;
    dut->cmd_in_f1a_h = in.cmd_in_f1a_h & 0x7full;
    dut->op_dtype_f1a_h = in.op_dtype_f1a_h & 0xfull;
    dut->ea_hi_f1a_h = in.ea_hi_f1a_h & 0xffull;
    dut->fa_hi_zero_f1a_h = in.fa_hi_zero_f1a_h & 0x1ull;
    dut->sa_hi_f1a_h = in.sa_hi_f1a_h & 0x1ull;
    dut->sb_hi_f1a_h = in.sb_hi_f1a_h & 0x1ull;
    dut->sc_f1a_h = in.sc_f1a_h & 0x1ull;
    dut->sa_lo_f1a_h = in.sa_lo_f1a_h & 0x1ull;
    dut->sb_lo_f1a_h = in.sb_lo_f1a_h & 0x1ull;
    dut->ea_hi_zero_f1a_h = in.ea_hi_zero_f1a_h & 0x1ull;
    dut->eb_hi_zero_f1a_h = in.eb_hi_zero_f1a_h & 0x1ull;
    dut->ea_lo_zero_f1a_h = in.ea_lo_zero_f1a_h & 0x1ull;
    dut->eb_lo_zero_f1a_h = in.eb_lo_zero_f1a_h & 0x1ull;
    dut->flog_mul_prod_zero_f1a_h = in.flog_mul_prod_zero_f1a_h & 0x1ull;
}

Inputs random_inputs(uint32_t& state) {
    return Inputs{(random64(state) & 0x1ull), (random64(state) & 0x7ffffull), (random64(state) & 0x7full), (random64(state) & 0xfull), (random64(state) & 0xffull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull)};
}

std::vector<TestCase> directed_cases() {
    return {
        {"zero", Inputs{0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000000000000100ull}},
        {"ones", Inputs{0x1ull, 0x7ffffull, 0x7full, 0xfull, 0xffull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00000000000779ffull}},
        {"pattern_a", Inputs{0x1ull, 0x6e1c4ull, 0x2aull, 0x6ull, 0x1eull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00000000001e6155ull}},
        {"pattern_b", Inputs{0x0ull, 0x1be69ull, 0x56ull, 0xeull, 0x9dull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00000000000b78acull}},
        {"fma_sign_norm", Inputs{0x1ull, 0xaull, 0x7ull, 0x1ull, 0x7full, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x000000000001610full}},
        {"log_mul_zero", Inputs{0x1ull, 0x88ull, 0x74ull, 0xeull, 0x7full, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x000000000004e0e9ull}},
        {"compare_sgcvt", Inputs{0x1ull, 0x4800ull, 0x21ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000000000001942ull}},
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
