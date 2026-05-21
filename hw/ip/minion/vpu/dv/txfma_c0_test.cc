// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_c0_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

using DUT = Vtxfma_c0_tb;

namespace {

constexpr int kNumInputs = 54;
constexpr int kNumChunks = 3;
constexpr std::array<uint64_t, kNumInputs> kInputMasks = {0x1ull, 0x1fffffffffffull, 0x7ull, 0x7full, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull};
const char* const kInputNames[kNumInputs] = {"in_valid_exa_h", "sigs_in_exa_h_bits", "rm_in_exa_h", "trans_mask_exa_h", "trans_taylor_exa_h", "trans_exp_fma2_exc_exa_h", "use_prev_sigs_exa_h", "sa_hi_f0a_h", "sa_lo_f0a_h", "sb_hi_f0a_h", "sb_lo_f0a_h", "sc_f0a_h", "ea_hi_zero_f0a_l", "ea_hi_all1_f0a_h", "ea_lo_zero_f0a_l", "ea_lo_all1_f0a_h", "eb_hi_zero_f0a_l", "eb_hi_all1_f0a_h", "eb_lo_zero_f0a_l", "eb_lo_all1_f0a_h", "ec_zero_f0a_l", "ec_all1_f0a_h", "eb_eql_127_f0a_h", "eb_eql_126_f0a_h", "eb_eql_125_f0a_h", "fcf10_zero_f0a_l", "fcf11_zero_f0a_l", "fcps_zero_f0a_h", "fcph_zero_f0a_l", "fcnorm_zero_29_8_f0a_h", "fcnorm16_eq_1_f0a_h", "fcnorm8_eq_1_f0a_h", "fcn8_one_30_24_f0a_h", "fcn10_one_30_22_f0a_h", "fcn16_one_30_16_f0a_h", "fcn24_one_30_8_f0a_h", "faps_zero_f0a_l", "fbps_zero_f0a_l", "faph_hi_zero_f0a_l", "fbph_hi_zero_f0a_l", "faph_lo_zero_f0a_l", "fbph_lo_zero_f0a_l", "faph_lo_b0_f0a_h", "fbph_lo_b0_f0a_h", "faps_b0_f0a_h", "fbps_b0_f0a_h", "fc_31_f0a_h", "fcnorm_30_f0a_h", "faph_hi_b0_f0a_h", "fbph_hi_b0_f0a_h", "fcph_b0_f0a_h", "fcf11_b0_f0a_h", "fcf10_b0_f0a_h", "fcps_b0_f0a_h"};

struct Inputs {
    uint64_t in_valid_exa_h = 0;
    uint64_t sigs_in_exa_h_bits = 0;
    uint64_t rm_in_exa_h = 0;
    uint64_t trans_mask_exa_h = 0;
    uint64_t trans_taylor_exa_h = 0;
    uint64_t trans_exp_fma2_exc_exa_h = 0;
    uint64_t use_prev_sigs_exa_h = 0;
    uint64_t sa_hi_f0a_h = 0;
    uint64_t sa_lo_f0a_h = 0;
    uint64_t sb_hi_f0a_h = 0;
    uint64_t sb_lo_f0a_h = 0;
    uint64_t sc_f0a_h = 0;
    uint64_t ea_hi_zero_f0a_l = 0;
    uint64_t ea_hi_all1_f0a_h = 0;
    uint64_t ea_lo_zero_f0a_l = 0;
    uint64_t ea_lo_all1_f0a_h = 0;
    uint64_t eb_hi_zero_f0a_l = 0;
    uint64_t eb_hi_all1_f0a_h = 0;
    uint64_t eb_lo_zero_f0a_l = 0;
    uint64_t eb_lo_all1_f0a_h = 0;
    uint64_t ec_zero_f0a_l = 0;
    uint64_t ec_all1_f0a_h = 0;
    uint64_t eb_eql_127_f0a_h = 0;
    uint64_t eb_eql_126_f0a_h = 0;
    uint64_t eb_eql_125_f0a_h = 0;
    uint64_t fcf10_zero_f0a_l = 0;
    uint64_t fcf11_zero_f0a_l = 0;
    uint64_t fcps_zero_f0a_h = 0;
    uint64_t fcph_zero_f0a_l = 0;
    uint64_t fcnorm_zero_29_8_f0a_h = 0;
    uint64_t fcnorm16_eq_1_f0a_h = 0;
    uint64_t fcnorm8_eq_1_f0a_h = 0;
    uint64_t fcn8_one_30_24_f0a_h = 0;
    uint64_t fcn10_one_30_22_f0a_h = 0;
    uint64_t fcn16_one_30_16_f0a_h = 0;
    uint64_t fcn24_one_30_8_f0a_h = 0;
    uint64_t faps_zero_f0a_l = 0;
    uint64_t fbps_zero_f0a_l = 0;
    uint64_t faph_hi_zero_f0a_l = 0;
    uint64_t fbph_hi_zero_f0a_l = 0;
    uint64_t faph_lo_zero_f0a_l = 0;
    uint64_t fbph_lo_zero_f0a_l = 0;
    uint64_t faph_lo_b0_f0a_h = 0;
    uint64_t fbph_lo_b0_f0a_h = 0;
    uint64_t faps_b0_f0a_h = 0;
    uint64_t fbps_b0_f0a_h = 0;
    uint64_t fc_31_f0a_h = 0;
    uint64_t fcnorm_30_f0a_h = 0;
    uint64_t faph_hi_b0_f0a_h = 0;
    uint64_t fbph_hi_b0_f0a_h = 0;
    uint64_t fcph_b0_f0a_h = 0;
    uint64_t fcf11_b0_f0a_h = 0;
    uint64_t fcf10_b0_f0a_h = 0;
    uint64_t fcps_b0_f0a_h = 0;
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
    return {in.in_valid_exa_h, in.sigs_in_exa_h_bits, in.rm_in_exa_h, in.trans_mask_exa_h, in.trans_taylor_exa_h, in.trans_exp_fma2_exc_exa_h, in.use_prev_sigs_exa_h, in.sa_hi_f0a_h, in.sa_lo_f0a_h, in.sb_hi_f0a_h, in.sb_lo_f0a_h, in.sc_f0a_h, in.ea_hi_zero_f0a_l, in.ea_hi_all1_f0a_h, in.ea_lo_zero_f0a_l, in.ea_lo_all1_f0a_h, in.eb_hi_zero_f0a_l, in.eb_hi_all1_f0a_h, in.eb_lo_zero_f0a_l, in.eb_lo_all1_f0a_h, in.ec_zero_f0a_l, in.ec_all1_f0a_h, in.eb_eql_127_f0a_h, in.eb_eql_126_f0a_h, in.eb_eql_125_f0a_h, in.fcf10_zero_f0a_l, in.fcf11_zero_f0a_l, in.fcps_zero_f0a_h, in.fcph_zero_f0a_l, in.fcnorm_zero_29_8_f0a_h, in.fcnorm16_eq_1_f0a_h, in.fcnorm8_eq_1_f0a_h, in.fcn8_one_30_24_f0a_h, in.fcn10_one_30_22_f0a_h, in.fcn16_one_30_16_f0a_h, in.fcn24_one_30_8_f0a_h, in.faps_zero_f0a_l, in.fbps_zero_f0a_l, in.faph_hi_zero_f0a_l, in.fbph_hi_zero_f0a_l, in.faph_lo_zero_f0a_l, in.fbph_lo_zero_f0a_l, in.faph_lo_b0_f0a_h, in.fbph_lo_b0_f0a_h, in.faps_b0_f0a_h, in.fbps_b0_f0a_h, in.fc_31_f0a_h, in.fcnorm_30_f0a_h, in.faph_hi_b0_f0a_h, in.fbph_hi_b0_f0a_h, in.fcph_b0_f0a_h, in.fcf11_b0_f0a_h, in.fcf10_b0_f0a_h, in.fcps_b0_f0a_h};
}

void apply_inputs(DUT* dut, const Inputs& in) {
    dut->in_valid_exa_h = in.in_valid_exa_h & 0x1ull;
    dut->sigs_in_exa_h_bits = in.sigs_in_exa_h_bits & 0x1fffffffffffull;
    dut->rm_in_exa_h = in.rm_in_exa_h & 0x7ull;
    dut->trans_mask_exa_h = in.trans_mask_exa_h & 0x7full;
    dut->trans_taylor_exa_h = in.trans_taylor_exa_h & 0x1ull;
    dut->trans_exp_fma2_exc_exa_h = in.trans_exp_fma2_exc_exa_h & 0x1ull;
    dut->use_prev_sigs_exa_h = in.use_prev_sigs_exa_h & 0x1ull;
    dut->sa_hi_f0a_h = in.sa_hi_f0a_h & 0x1ull;
    dut->sa_lo_f0a_h = in.sa_lo_f0a_h & 0x1ull;
    dut->sb_hi_f0a_h = in.sb_hi_f0a_h & 0x1ull;
    dut->sb_lo_f0a_h = in.sb_lo_f0a_h & 0x1ull;
    dut->sc_f0a_h = in.sc_f0a_h & 0x1ull;
    dut->ea_hi_zero_f0a_l = in.ea_hi_zero_f0a_l & 0x1ull;
    dut->ea_hi_all1_f0a_h = in.ea_hi_all1_f0a_h & 0x1ull;
    dut->ea_lo_zero_f0a_l = in.ea_lo_zero_f0a_l & 0x1ull;
    dut->ea_lo_all1_f0a_h = in.ea_lo_all1_f0a_h & 0x1ull;
    dut->eb_hi_zero_f0a_l = in.eb_hi_zero_f0a_l & 0x1ull;
    dut->eb_hi_all1_f0a_h = in.eb_hi_all1_f0a_h & 0x1ull;
    dut->eb_lo_zero_f0a_l = in.eb_lo_zero_f0a_l & 0x1ull;
    dut->eb_lo_all1_f0a_h = in.eb_lo_all1_f0a_h & 0x1ull;
    dut->ec_zero_f0a_l = in.ec_zero_f0a_l & 0x1ull;
    dut->ec_all1_f0a_h = in.ec_all1_f0a_h & 0x1ull;
    dut->eb_eql_127_f0a_h = in.eb_eql_127_f0a_h & 0x1ull;
    dut->eb_eql_126_f0a_h = in.eb_eql_126_f0a_h & 0x1ull;
    dut->eb_eql_125_f0a_h = in.eb_eql_125_f0a_h & 0x1ull;
    dut->fcf10_zero_f0a_l = in.fcf10_zero_f0a_l & 0x1ull;
    dut->fcf11_zero_f0a_l = in.fcf11_zero_f0a_l & 0x1ull;
    dut->fcps_zero_f0a_h = in.fcps_zero_f0a_h & 0x1ull;
    dut->fcph_zero_f0a_l = in.fcph_zero_f0a_l & 0x1ull;
    dut->fcnorm_zero_29_8_f0a_h = in.fcnorm_zero_29_8_f0a_h & 0x1ull;
    dut->fcnorm16_eq_1_f0a_h = in.fcnorm16_eq_1_f0a_h & 0x1ull;
    dut->fcnorm8_eq_1_f0a_h = in.fcnorm8_eq_1_f0a_h & 0x1ull;
    dut->fcn8_one_30_24_f0a_h = in.fcn8_one_30_24_f0a_h & 0x1ull;
    dut->fcn10_one_30_22_f0a_h = in.fcn10_one_30_22_f0a_h & 0x1ull;
    dut->fcn16_one_30_16_f0a_h = in.fcn16_one_30_16_f0a_h & 0x1ull;
    dut->fcn24_one_30_8_f0a_h = in.fcn24_one_30_8_f0a_h & 0x1ull;
    dut->faps_zero_f0a_l = in.faps_zero_f0a_l & 0x1ull;
    dut->fbps_zero_f0a_l = in.fbps_zero_f0a_l & 0x1ull;
    dut->faph_hi_zero_f0a_l = in.faph_hi_zero_f0a_l & 0x1ull;
    dut->fbph_hi_zero_f0a_l = in.fbph_hi_zero_f0a_l & 0x1ull;
    dut->faph_lo_zero_f0a_l = in.faph_lo_zero_f0a_l & 0x1ull;
    dut->fbph_lo_zero_f0a_l = in.fbph_lo_zero_f0a_l & 0x1ull;
    dut->faph_lo_b0_f0a_h = in.faph_lo_b0_f0a_h & 0x1ull;
    dut->fbph_lo_b0_f0a_h = in.fbph_lo_b0_f0a_h & 0x1ull;
    dut->faps_b0_f0a_h = in.faps_b0_f0a_h & 0x1ull;
    dut->fbps_b0_f0a_h = in.fbps_b0_f0a_h & 0x1ull;
    dut->fc_31_f0a_h = in.fc_31_f0a_h & 0x1ull;
    dut->fcnorm_30_f0a_h = in.fcnorm_30_f0a_h & 0x1ull;
    dut->faph_hi_b0_f0a_h = in.faph_hi_b0_f0a_h & 0x1ull;
    dut->fbph_hi_b0_f0a_h = in.fbph_hi_b0_f0a_h & 0x1ull;
    dut->fcph_b0_f0a_h = in.fcph_b0_f0a_h & 0x1ull;
    dut->fcf11_b0_f0a_h = in.fcf11_b0_f0a_h & 0x1ull;
    dut->fcf10_b0_f0a_h = in.fcf10_b0_f0a_h & 0x1ull;
    dut->fcps_b0_f0a_h = in.fcps_b0_f0a_h & 0x1ull;
}

Inputs random_inputs(uint32_t& state) {
    return Inputs{(random64(state) & 0x1ull), (random64(state) & 0x1fffffffffffull), (random64(state) & 0x7ull), (random64(state) & 0x7full), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull)};
}

std::vector<TestCase> directed_cases() {
    return {
        {"zero", Inputs{0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000600000000014ull, 0x0000181000000000ull, 0x0000000000100000ull}},
        {"ones", Inputs{0x1ull, 0x1fffffffffffull, 0x7ull, 0x7full, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x0000008000000010ull, 0x420a181000000000ull, 0x0000000000f81e80ull}},
        {"pattern_a", Inputs{0x1ull, 0xbd078824901ull, 0x3ull, 0x4cull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000208000000014ull, 0x0000c01600000000ull, 0x0000000000100000ull}},
        {"pattern_b", Inputs{0x1ull, 0xc44660516acull, 0x7ull, 0x4ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00000076200352d3ull, 0x000a181000000000ull, 0x0000000001300400ull}},
        {"f32_fma_zero_operands", Inputs{0x1ull, 0x1e0001c0241ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x00080300f0003c51ull, 0x4a1a185e00140200ull, 0x0000000000f01e81ull}},
        {"f32_sub_special_signs", Inputs{0x1ull, 0x6000140209ull, 0x2ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x8100672030011456ull, 0x481a185e00143025ull, 0x0000000000b01621ull}},
        {"f16f32_mul", Inputs{0x1ull, 0xa000181c04ull, 0x3ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x8000643040020f95ull, 0xf710185e00290000ull, 0x0000000000d01bffull}},
        {"trans_sin_p2", Inputs{0x1ull, 0x1e2100000040ull, 0x0ull, 0x55ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x0000600f00003c17ull, 0x422af81200cc0000ull, 0x0000000000fc9eb0ull}},
        {"use_prev_trans_exp", Inputs{0x1ull, 0x1f6100000240ull, 0x4ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000608f00003c17ull, 0x4a1a184e001c0000ull, 0x0000000000f81eb1ull}},
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
    return {dut->out_chunk0_o, dut->out_chunk1_o, dut->out_chunk2_o};
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
    apply_inputs(sim.dut.get(), Inputs{});
    sim.reset();
    for (const auto& tc : cases) {
        apply_inputs(sim.dut.get(), tc.in);
        observe(cov, tc.in);
        sim.tick();
        check_outputs(sim, tc);
    }

    uint32_t state = 0x13579bdfu;
    for (int i = 0; i < 256; ++i) {
        const Inputs in = random_inputs(state);
        apply_inputs(sim.dut.get(), in);
        observe(cov, in);
        sim.tick();
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
