// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_c0_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using DUT = Vcosim_txfma_c0_tb;

namespace {

constexpr int kNumInputs = 54;
constexpr int kRandomIters = 50000;
constexpr int kTargetComparisons = 50000;
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

std::vector<Inputs> directed_inputs() {
    return {
        Inputs{0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // zero
        Inputs{0x1ull, 0x1fffffffffffull, 0x7ull, 0x7full, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull},  // ones
        Inputs{0x1ull, 0xbd078824901ull, 0x3ull, 0x4cull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull},  // pattern_a
        Inputs{0x1ull, 0xc44660516acull, 0x7ull, 0x4ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull},  // pattern_b
        Inputs{0x1ull, 0x1e0001c0241ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // f32_fma_zero_operands
        Inputs{0x1ull, 0x6000140209ull, 0x2ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull},  // f32_sub_special_signs
        Inputs{0x1ull, 0xa000181c04ull, 0x3ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // f16f32_mul
        Inputs{0x1ull, 0x1e2100000040ull, 0x0ull, 0x55ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull},  // trans_sin_p2
        Inputs{0x1ull, 0x1f6100000240ull, 0x4ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // use_prev_trans_exp
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

void compare_all(CosimCtrl<DUT>& sim) {
    sim.compare("trans_exp_fma2_exc_f0a_h", sim.dut->orig_trans_exp_fma2_exc_f0a_h, sim.dut->new_trans_exp_fma2_exc_f0a_h);
    sim.compare("opa_valid_rfa_h", sim.dut->orig_opa_valid_rfa_h, sim.dut->new_opa_valid_rfa_h);
    sim.compare("opb_valid_rfa_h", sim.dut->orig_opb_valid_rfa_h, sim.dut->new_opb_valid_rfa_h);
    sim.compare("opc_valid_rfa_h", sim.dut->orig_opc_valid_rfa_h, sim.dut->new_opc_valid_rfa_h);
    sim.compare("ctrl_valid_f0a_h", sim.dut->orig_ctrl_valid_f0a_h, sim.dut->new_ctrl_valid_f0a_h);
    sim.compare("is_trans_rfa_h", sim.dut->orig_is_trans_rfa_h, sim.dut->new_is_trans_rfa_h);
    sim.compare("eb_hi_k_exa_h", sim.dut->orig_eb_hi_k_exa_h, sim.dut->new_eb_hi_k_exa_h);
    sim.compare("fa_en_exa_h", sim.dut->orig_fa_en_exa_h, sim.dut->new_fa_en_exa_h);
    sim.compare("fb_en_exa_h", sim.dut->orig_fb_en_exa_h, sim.dut->new_fb_en_exa_h);
    sim.compare("fc_en_exa_h", sim.dut->orig_fc_en_exa_h, sim.dut->new_fc_en_exa_h);
    sim.compare("sa_hi_en_exa_h", sim.dut->orig_sa_hi_en_exa_h, sim.dut->new_sa_hi_en_exa_h);
    sim.compare("sa_lo_en_exa_h", sim.dut->orig_sa_lo_en_exa_h, sim.dut->new_sa_lo_en_exa_h);
    sim.compare("sb_hi_en_exa_h", sim.dut->orig_sb_hi_en_exa_h, sim.dut->new_sb_hi_en_exa_h);
    sim.compare("sb_lo_en_exa_h", sim.dut->orig_sb_lo_en_exa_h, sim.dut->new_sb_lo_en_exa_h);
    sim.compare("ea_hi_zero_clr_f0a_h", sim.dut->orig_ea_hi_zero_clr_f0a_h, sim.dut->new_ea_hi_zero_clr_f0a_h);
    sim.compare("eb_hi_zero_clr_f0a_h", sim.dut->orig_eb_hi_zero_clr_f0a_h, sim.dut->new_eb_hi_zero_clr_f0a_h);
    sim.compare("ea_lo_zero_clr_f0a_h", sim.dut->orig_ea_lo_zero_clr_f0a_h, sim.dut->new_ea_lo_zero_clr_f0a_h);
    sim.compare("eb_lo_zero_clr_f0a_h", sim.dut->orig_eb_lo_zero_clr_f0a_h, sim.dut->new_eb_lo_zero_clr_f0a_h);
    sim.compare("sel_eainmux_exa_h", sim.dut->orig_sel_eainmux_exa_h, sim.dut->new_sel_eainmux_exa_h);
    sim.compare("sel_ealo_ph_exa_h", sim.dut->orig_sel_ealo_ph_exa_h, sim.dut->new_sel_ealo_ph_exa_h);
    sim.compare("ehi_ea_en_exa_h", sim.dut->orig_ehi_ea_en_exa_h, sim.dut->new_ehi_ea_en_exa_h);
    sim.compare("elo_ea_en_exa_h", sim.dut->orig_elo_ea_en_exa_h, sim.dut->new_elo_ea_en_exa_h);
    sim.compare("sel_ebin_hi_exa_h", sim.dut->orig_sel_ebin_hi_exa_h, sim.dut->new_sel_ebin_hi_exa_h);
    sim.compare("sel_eblo_ph_exa_h", sim.dut->orig_sel_eblo_ph_exa_h, sim.dut->new_sel_eblo_ph_exa_h);
    sim.compare("ehi_eb_en_exa_h", sim.dut->orig_ehi_eb_en_exa_h, sim.dut->new_ehi_eb_en_exa_h);
    sim.compare("elo_eb_en_exa_h", sim.dut->orig_elo_eb_en_exa_h, sim.dut->new_elo_eb_en_exa_h);
    sim.compare("sel_ec5b_exa_h", sim.dut->orig_sel_ec5b_exa_h, sim.dut->new_sel_ec5b_exa_h);
    sim.compare("sel_ecin_exa_h", sim.dut->orig_sel_ecin_exa_h, sim.dut->new_sel_ecin_exa_h);
    sim.compare("ec_en_exa_h", sim.dut->orig_ec_en_exa_h, sim.dut->new_ec_en_exa_h);
    sim.compare("sel_ec5bdet_exa_h", sim.dut->orig_sel_ec5bdet_exa_h, sim.dut->new_sel_ec5bdet_exa_h);
    sim.compare("sc_en_exa_h", sim.dut->orig_sc_en_exa_h, sim.dut->new_sc_en_exa_h);
    sim.compare("sa1_nr2_frcpfxp_exa_h", sim.dut->orig_sa1_nr2_frcpfxp_exa_h, sim.dut->new_sa1_nr2_frcpfxp_exa_h);
    sim.compare("sel_sc_exa_h", sim.dut->orig_sel_sc_exa_h, sim.dut->new_sel_sc_exa_h);
    sim.compare("sel_sign_sinp2_exa_h", sim.dut->orig_sel_sign_sinp2_exa_h, sim.dut->new_sel_sign_sinp2_exa_h);
    sim.compare("sa_zap_exa_l", sim.dut->orig_sa_zap_exa_l, sim.dut->new_sa_zap_exa_l);
    sim.compare("sb_zap_exa_l", sim.dut->orig_sb_zap_exa_l, sim.dut->new_sb_zap_exa_l);
    sim.compare("sel_f10_exa_h", sim.dut->orig_sel_f10_exa_h, sim.dut->new_sel_f10_exa_h);
    sim.compare("sel_f11_exa_h", sim.dut->orig_sel_f11_exa_h, sim.dut->new_sel_f11_exa_h);
    sim.compare("sel_opcgfx_exa_h", sim.dut->orig_sel_opcgfx_exa_h, sim.dut->new_sel_opcgfx_exa_h);
    sim.compare("fc_dtps_exa_h", sim.dut->orig_fc_dtps_exa_h, sim.dut->new_fc_dtps_exa_h);
    sim.compare("neg_fmaop_exa_h", sim.dut->orig_neg_fmaop_exa_h, sim.dut->new_neg_fmaop_exa_h);
    sim.compare("trans_op2_exa_l", sim.dut->orig_trans_op2_exa_l, sim.dut->new_trans_op2_exa_l);
    sim.compare("sel_xopc_exa_h", sim.dut->orig_sel_xopc_exa_h, sim.dut->new_sel_xopc_exa_h);
    sim.compare("sel_opcfmt_exa_h", sim.dut->orig_sel_opcfmt_exa_h, sim.dut->new_sel_opcfmt_exa_h);
    sim.compare("sel_snorm_exa_h", sim.dut->orig_sel_snorm_exa_h, sim.dut->new_sel_snorm_exa_h);
    sim.compare("sel_g1sk2_exa_h", sim.dut->orig_sel_g1sk2_exa_h, sim.dut->new_sel_g1sk2_exa_h);
    sim.compare("sel_g1sk8_exa_h", sim.dut->orig_sel_g1sk8_exa_h, sim.dut->new_sel_g1sk8_exa_h);
    sim.compare("sel_g1sk16_exa_h", sim.dut->orig_sel_g1sk16_exa_h, sim.dut->new_sel_g1sk16_exa_h);
    sim.compare("sel_g1sk24_exa_h", sim.dut->orig_sel_g1sk24_exa_h, sim.dut->new_sel_g1sk24_exa_h);
    sim.compare("sel_xexpfrac_exa_h", sim.dut->orig_sel_xexpfrac_exa_h, sim.dut->new_sel_xexpfrac_exa_h);
    sim.compare("sel_transfa_exa_h", sim.dut->orig_sel_transfa_exa_h, sim.dut->new_sel_transfa_exa_h);
    sim.compare("sel_transfb_exa_h", sim.dut->orig_sel_transfb_exa_h, sim.dut->new_sel_transfb_exa_h);
    sim.compare("sel_opafmt_exa_h", sim.dut->orig_sel_opafmt_exa_h, sim.dut->new_sel_opafmt_exa_h);
    sim.compare("sel_opbfmt_exa_h", sim.dut->orig_sel_opbfmt_exa_h, sim.dut->new_sel_opbfmt_exa_h);
    sim.compare("mul_sign_f0a_h", sim.dut->orig_mul_sign_f0a_h, sim.dut->new_mul_sign_f0a_h);
    sim.compare("mul_hp_mode_f0a_h", sim.dut->orig_mul_hp_mode_f0a_h, sim.dut->new_mul_hp_mode_f0a_h);
    sim.compare("res_inf_f0a_h", sim.dut->orig_res_inf_f0a_h, sim.dut->new_res_inf_f0a_h);
    sim.compare("res_snan_f0a_h", sim.dut->orig_res_snan_f0a_h, sim.dut->new_res_snan_f0a_h);
    sim.compare("res_qnan_f0a_h", sim.dut->orig_res_qnan_f0a_h, sim.dut->new_res_qnan_f0a_h);
    sim.compare("classify_f0a_h", sim.dut->orig_classify_f0a_h, sim.dut->new_classify_f0a_h);
    sim.compare("prod_inf_hi_f0a_h", sim.dut->orig_prod_inf_hi_f0a_h, sim.dut->new_prod_inf_hi_f0a_h);
    sim.compare("c_inf_f0a_h", sim.dut->orig_c_inf_f0a_h, sim.dut->new_c_inf_f0a_h);
    sim.compare("a_inf_f0a_h", sim.dut->orig_a_inf_f0a_h, sim.dut->new_a_inf_f0a_h);
    sim.compare("flags_f0a_h", sim.dut->orig_flags_f0a_h, sim.dut->new_flags_f0a_h);
    sim.compare("a_is_qnan_hi_f0a_h", sim.dut->orig_a_is_qnan_hi_f0a_h, sim.dut->new_a_is_qnan_hi_f0a_h);
    sim.compare("c_is_qnan_f0a_h", sim.dut->orig_c_is_qnan_f0a_h, sim.dut->new_c_is_qnan_f0a_h);
    sim.compare("a_is_snan_hi_f0a_h", sim.dut->orig_a_is_snan_hi_f0a_h, sim.dut->new_a_is_snan_hi_f0a_h);
    sim.compare("c_is_snan_f0a_h", sim.dut->orig_c_is_snan_f0a_h, sim.dut->new_c_is_snan_f0a_h);
    sim.compare("res_pos_int_nan_f0a_h", sim.dut->orig_res_pos_int_nan_f0a_h, sim.dut->new_res_pos_int_nan_f0a_h);
    sim.compare("res_neg_int_nan_f0a_h", sim.dut->orig_res_neg_int_nan_f0a_h, sim.dut->new_res_neg_int_nan_f0a_h);
    sim.compare("fc_zero30_8_f0a_h", sim.dut->orig_fc_zero30_8_f0a_h, sim.dut->new_fc_zero30_8_f0a_h);
    sim.compare("res_uint_nan_f0a_h", sim.dut->orig_res_uint_nan_f0a_h, sim.dut->new_res_uint_nan_f0a_h);
    sim.compare("res_zero_f0a_h", sim.dut->orig_res_zero_f0a_h, sim.dut->new_res_zero_f0a_h);
    sim.compare("res_one_f0a_h", sim.dut->orig_res_one_f0a_h, sim.dut->new_res_one_f0a_h);
    sim.compare("prod_inf_lo_f0a_h", sim.dut->orig_prod_inf_lo_f0a_h, sim.dut->new_prod_inf_lo_f0a_h);
    sim.compare("fa_hi_zero_f0a_h", sim.dut->orig_fa_hi_zero_f0a_h, sim.dut->new_fa_hi_zero_f0a_h);
    sim.compare("fb_hi_zero_f0a_h", sim.dut->orig_fb_hi_zero_f0a_h, sim.dut->new_fb_hi_zero_f0a_h);
    sim.compare("res_unorm_one_f0a_h", sim.dut->orig_res_unorm_one_f0a_h, sim.dut->new_res_unorm_one_f0a_h);
    sim.compare("res_snorm_one_f0a_h", sim.dut->orig_res_snorm_one_f0a_h, sim.dut->new_res_snorm_one_f0a_h);
    sim.compare("flags_en_f0a_h", sim.dut->orig_flags_en_f0a_h, sim.dut->new_flags_en_f0a_h);
    sim.compare("dtps_f0a_h", sim.dut->orig_dtps_f0a_h, sim.dut->new_dtps_f0a_h);
    sim.compare("spec_res_en_f0a_h", sim.dut->orig_spec_res_en_f0a_h, sim.dut->new_spec_res_en_f0a_h);
    sim.compare("use_prev_sigs_f0a_h", sim.dut->orig_use_prev_sigs_f0a_h, sim.dut->new_use_prev_sigs_f0a_h);
    sim.compare("rm_f0a_h", sim.dut->orig_rm_f0a_h, sim.dut->new_rm_f0a_h);
    sim.compare("cmd_f0a_h", sim.dut->orig_cmd_f0a_h, sim.dut->new_cmd_f0a_h);
    sim.compare("sigs_f0a_h", sim.dut->orig_sigs_f0a_h, sim.dut->new_sigs_f0a_h);
    sim.compare("op_dtype_f0a_h", sim.dut->orig_op_dtype_f0a_h, sim.dut->new_op_dtype_f0a_h);
    sim.compare("sin_res_shf_idx_f0a_h", sim.dut->orig_sin_res_shf_idx_f0a_h, sim.dut->new_sin_res_shf_idx_f0a_h);
    sim.compare("flog_mul_prod_zero_f0a_h", sim.dut->orig_flog_mul_prod_zero_f0a_h, sim.dut->new_flog_mul_prod_zero_f0a_h);
    sim.compare("ec_zero_clr_f0a_h", sim.dut->orig_ec_zero_clr_f0a_h, sim.dut->new_ec_zero_clr_f0a_h);
    sim.compare("use_mul_f0a_h", sim.dut->orig_use_mul_f0a_h, sim.dut->new_use_mul_f0a_h);
}

void eval_phase(CosimCtrl<DUT>& sim, bool clk, bool do_compare) {
    sim.dut->clk_i = clk;
    sim.dut->eval();
    if (do_compare) compare_all(sim);
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        std::printf("TIMEOUT at sim_time=%lu\n", static_cast<unsigned long>(sim.sim_time));
        std::exit(2);
    }
}

void run_cycle(CosimCtrl<DUT>& sim, const Inputs& in, bool do_compare) {
    apply_inputs(sim.dut.get(), in);
    eval_phase(sim, false, do_compare);
    eval_phase(sim, true, do_compare);
}

void check_coverage(CosimCtrl<DUT>& sim, const InputCoverage& cov) {
    for (int i = 0; i < kNumInputs; ++i) {
        sim.check(cov.ones[i] == kInputMasks[i], std::string(kInputNames[i]) + " drove all bits high");
        sim.check(cov.zeros[i] == kInputMasks[i], std::string(kInputNames[i]) + " drove all bits low");
    }
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    InputCoverage cov;
    auto directed = directed_inputs();
    sim.max_time = 2000000;
    sim.dut->rst_ni = 0;
    run_cycle(sim, Inputs{}, false);
    run_cycle(sim, Inputs{}, false);
    sim.dut->rst_ni = 1;
    Verilated::assertOn(true);
    run_cycle(sim, directed[0], false);
    observe(cov, directed[0]);
    for (const auto& in : directed) {
        observe(cov, in);
        run_cycle(sim, in, true);
    }
    uint32_t state = 0x2468ace0u;
    for (int i = 0; i < kRandomIters; ++i) {
        const Inputs in = random_inputs(state);
        observe(cov, in);
        run_cycle(sim, in, true);
    }
    check_coverage(sim, cov);
    sim.check(sim.comparisons >= kTargetComparisons, "cosim comparison target met");
    return sim.finish();
}
