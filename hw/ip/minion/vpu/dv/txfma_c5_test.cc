// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_c5_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

using DUT = Vtxfma_c5_tb;

namespace {

constexpr int kNumInputs = 19;
constexpr int kNumChunks = 1;
constexpr std::array<uint64_t, kNumInputs> kInputMasks = {0xfull, 0x7full, 0x7ffffull, 0x7ull, 0x7full, 0x3full, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull};
const char* const kInputNames[kNumInputs] = {"op_dtype_f5a_h", "cmd_f5a_h", "sigs_f5a_h", "rm_f5a_h", "elxd_f5a_h", "etrz50_f5a_h", "ec_zero_f5a_h", "prod_sign_hi_f5a_h", "s_res_f5a_h", "s_res_check_cadd_a3_f5a_h", "comp_res_sel_f5a_h", "comp_op_f5a_h", "minmax_op_f5a_h", "trans_log_rr_neg_f5a_h", "ediff1_zap_f5a_h", "fa_eq_fc_res_check_hi_f5a_h", "fc_sel_f5a_h", "stky2g_f5a_h", "comb_add_a3_f5a_h"};

struct Inputs {
    uint64_t op_dtype_f5a_h = 0;
    uint64_t cmd_f5a_h = 0;
    uint64_t sigs_f5a_h = 0;
    uint64_t rm_f5a_h = 0;
    uint64_t elxd_f5a_h = 0;
    uint64_t etrz50_f5a_h = 0;
    uint64_t ec_zero_f5a_h = 0;
    uint64_t prod_sign_hi_f5a_h = 0;
    uint64_t s_res_f5a_h = 0;
    uint64_t s_res_check_cadd_a3_f5a_h = 0;
    uint64_t comp_res_sel_f5a_h = 0;
    uint64_t comp_op_f5a_h = 0;
    uint64_t minmax_op_f5a_h = 0;
    uint64_t trans_log_rr_neg_f5a_h = 0;
    uint64_t ediff1_zap_f5a_h = 0;
    uint64_t fa_eq_fc_res_check_hi_f5a_h = 0;
    uint64_t fc_sel_f5a_h = 0;
    uint64_t stky2g_f5a_h = 0;
    uint64_t comb_add_a3_f5a_h = 0;
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
    return {in.op_dtype_f5a_h, in.cmd_f5a_h, in.sigs_f5a_h, in.rm_f5a_h, in.elxd_f5a_h, in.etrz50_f5a_h, in.ec_zero_f5a_h, in.prod_sign_hi_f5a_h, in.s_res_f5a_h, in.s_res_check_cadd_a3_f5a_h, in.comp_res_sel_f5a_h, in.comp_op_f5a_h, in.minmax_op_f5a_h, in.trans_log_rr_neg_f5a_h, in.ediff1_zap_f5a_h, in.fa_eq_fc_res_check_hi_f5a_h, in.fc_sel_f5a_h, in.stky2g_f5a_h, in.comb_add_a3_f5a_h};
}

void apply_inputs(DUT* dut, const Inputs& in) {
    dut->op_dtype_f5a_h = in.op_dtype_f5a_h & 0xfull;
    dut->cmd_f5a_h = in.cmd_f5a_h & 0x7full;
    dut->sigs_f5a_h = in.sigs_f5a_h & 0x7ffffull;
    dut->rm_f5a_h = in.rm_f5a_h & 0x7ull;
    dut->elxd_f5a_h = in.elxd_f5a_h & 0x7full;
    dut->etrz50_f5a_h = in.etrz50_f5a_h & 0x3full;
    dut->ec_zero_f5a_h = in.ec_zero_f5a_h & 0x1ull;
    dut->prod_sign_hi_f5a_h = in.prod_sign_hi_f5a_h & 0x1ull;
    dut->s_res_f5a_h = in.s_res_f5a_h & 0x1ull;
    dut->s_res_check_cadd_a3_f5a_h = in.s_res_check_cadd_a3_f5a_h & 0x1ull;
    dut->comp_res_sel_f5a_h = in.comp_res_sel_f5a_h & 0x1ull;
    dut->comp_op_f5a_h = in.comp_op_f5a_h & 0x1ull;
    dut->minmax_op_f5a_h = in.minmax_op_f5a_h & 0x1ull;
    dut->trans_log_rr_neg_f5a_h = in.trans_log_rr_neg_f5a_h & 0x1ull;
    dut->ediff1_zap_f5a_h = in.ediff1_zap_f5a_h & 0x1ull;
    dut->fa_eq_fc_res_check_hi_f5a_h = in.fa_eq_fc_res_check_hi_f5a_h & 0x1ull;
    dut->fc_sel_f5a_h = in.fc_sel_f5a_h & 0x1ull;
    dut->stky2g_f5a_h = in.stky2g_f5a_h & 0x1ull;
    dut->comb_add_a3_f5a_h = in.comb_add_a3_f5a_h & 0x1ull;
}

Inputs random_inputs(uint32_t& state) {
    return Inputs{(random64(state) & 0xfull), (random64(state) & 0x7full), (random64(state) & 0x7ffffull), (random64(state) & 0x7ull), (random64(state) & 0x7full), (random64(state) & 0x3full), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull)};
}

std::vector<TestCase> directed_cases() {
    return {
        {"zero", Inputs{0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000000000001440ull}},
        {"ones", Inputs{0xfull, 0x7full, 0x7ffffull, 0x7ull, 0x7full, 0x3full, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00000000002b0674ull}},
        {"pattern_a", Inputs{0x1ull, 0x40ull, 0x6f91eull, 0x7ull, 0x3eull, 0x10ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00000000002a4075ull}},
        {"pattern_b", Inputs{0x6ull, 0x6dull, 0x35ee2ull, 0x7ull, 0x3dull, 0xeull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x00000000002b0654ull}},
        {"round_rup_negative", Inputs{0x1ull, 0x7ull, 0x8ull, 0x3ull, 0x30ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x00000000002920c3ull}},
        {"compare_equal", Inputs{0x1ull, 0x20ull, 0x20ull, 0x2ull, 0x0ull, 0x34ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00000000002900c9ull}},
        {"int_cvt_round", Inputs{0x0ull, 0x15ull, 0x100ull, 0x4ull, 0x1full, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00000000002844c0ull}},
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
