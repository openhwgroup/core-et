// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_c4_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

using DUT = Vtxfma_c4_tb;

namespace {

constexpr int kNumInputs = 10;
constexpr int kNumChunks = 1;
constexpr std::array<uint64_t, kNumInputs> kInputMasks = {0xfull, 0x7full, 0x7ffffull, 0x7full, 0x7ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull};
const char* const kInputNames[kNumInputs] = {"op_dtype_f4a_h", "cmd_f4a_h", "sigs_f4a_h", "elxd_f4a_h", "sin_res_shf_idx_f4a_h", "ph_comb_stiky_f4a_h", "eff_sub1_f4a_h", "eff_sub2_f4a_h", "s_res_f4a_h", "sc_f4a_h"};

struct Inputs {
    uint64_t op_dtype_f4a_h = 0;
    uint64_t cmd_f4a_h = 0;
    uint64_t sigs_f4a_h = 0;
    uint64_t elxd_f4a_h = 0;
    uint64_t sin_res_shf_idx_f4a_h = 0;
    uint64_t ph_comb_stiky_f4a_h = 0;
    uint64_t eff_sub1_f4a_h = 0;
    uint64_t eff_sub2_f4a_h = 0;
    uint64_t s_res_f4a_h = 0;
    uint64_t sc_f4a_h = 0;
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
    return {in.op_dtype_f4a_h, in.cmd_f4a_h, in.sigs_f4a_h, in.elxd_f4a_h, in.sin_res_shf_idx_f4a_h, in.ph_comb_stiky_f4a_h, in.eff_sub1_f4a_h, in.eff_sub2_f4a_h, in.s_res_f4a_h, in.sc_f4a_h};
}

void apply_inputs(DUT* dut, const Inputs& in) {
    dut->op_dtype_f4a_h = in.op_dtype_f4a_h & 0xfull;
    dut->cmd_f4a_h = in.cmd_f4a_h & 0x7full;
    dut->sigs_f4a_h = in.sigs_f4a_h & 0x7ffffull;
    dut->elxd_f4a_h = in.elxd_f4a_h & 0x7full;
    dut->sin_res_shf_idx_f4a_h = in.sin_res_shf_idx_f4a_h & 0x7ull;
    dut->ph_comb_stiky_f4a_h = in.ph_comb_stiky_f4a_h & 0x1ull;
    dut->eff_sub1_f4a_h = in.eff_sub1_f4a_h & 0x1ull;
    dut->eff_sub2_f4a_h = in.eff_sub2_f4a_h & 0x1ull;
    dut->s_res_f4a_h = in.s_res_f4a_h & 0x1ull;
    dut->sc_f4a_h = in.sc_f4a_h & 0x1ull;
}

Inputs random_inputs(uint32_t& state) {
    return Inputs{(random64(state) & 0xfull), (random64(state) & 0x7full), (random64(state) & 0x7ffffull), (random64(state) & 0x7full), (random64(state) & 0x7ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull)};
}

std::vector<TestCase> directed_cases() {
    return {
        {"zero", Inputs{0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000000000000600ull}},
        {"ones", Inputs{0xfull, 0x7full, 0x7ffffull, 0x7full, 0x7ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00000000000037fcull}},
        {"pattern_a", Inputs{0x0ull, 0x5ull, 0x7eecfull, 0x35ull, 0x3ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00000000000036d4ull}},
        {"pattern_b", Inputs{0x7ull, 0x28ull, 0x24933ull, 0x7dull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x0000000000002600ull}},
        {"f32_elxd_effsub", Inputs{0x1ull, 0x7ull, 0x8ull, 0xcull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000000000002631ull}},
        {"sin_shift_idx3", Inputs{0x0ull, 0x77ull, 0x10000ull, 0x0ull, 0x3ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x000000000000160cull}},
        {"f16f32_clear", Inputs{0xeull, 0x7bull, 0x100ull, 0x1full, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000000000002c01ull}},
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
