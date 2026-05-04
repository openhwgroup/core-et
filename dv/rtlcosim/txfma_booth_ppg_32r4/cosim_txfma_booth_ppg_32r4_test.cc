// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_booth_ppg_32r4_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_booth_ppg_32r4_tb;

namespace {

constexpr uint64_t kMask33 = (1ull << 33) - 1ull;
constexpr uint64_t kMaskLow13 = 0x1fffull;
constexpr uint64_t kMaskHigh15 = 0x7fffull << 18;

struct BoolCoverage {
    bool seen0 = false;
    bool seen1 = false;
    void observe(bool value) { value ? seen1 = true : seen0 = true; }
    bool full() const { return seen0 && seen1; }
};

struct WordCoverage {
    uint32_t or_mask = 0;
    uint32_t and_mask = 0xffffffffu;
    void observe(uint32_t value) {
        or_mask |= value;
        and_mask &= value;
    }
};

struct ModelOutputs {
    uint32_t sxt = 0;
    uint32_t cin = 0;
    std::array<uint64_t, 17> pp{};
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t sign_extend33(uint32_t value, bool sign) {
    if (!sign) return static_cast<uint64_t>(value);
    return (static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(value))) & kMask33);
}

uint64_t model_norm_pp(uint32_t mulcand, uint32_t r4input, bool sign) {
    const uint64_t signed_mulcand = sign_extend33(mulcand, sign);
    switch (r4input & 0x7u) {
      case 0u:
      case 7u:
        return 0;
      case 1u:
      case 2u:
        return signed_mulcand;
      case 5u:
      case 6u:
        return (~signed_mulcand) & kMask33;
      case 3u:
        return (static_cast<uint64_t>(mulcand) << 1) & kMask33;
      case 4u:
        return (~((static_cast<uint64_t>(mulcand) << 1) & kMask33)) & kMask33;
      default:
        return 0;
    }
}

uint64_t model_msb_pp(uint32_t mulcand, bool msb, bool sign) {
    return (!sign && msb) ? static_cast<uint64_t>(mulcand) : 0ull;
}

uint32_t extract_bits(uint64_t value, int lsb, int width) {
    return static_cast<uint32_t>((value >> lsb) & ((uint64_t{1} << width) - 1u));
}

ModelOutputs model(uint32_t mpcnd, uint32_t mplr, bool mode, bool sign) {
    ModelOutputs out;
    const uint64_t ext_mplr = static_cast<uint64_t>(mplr) << 1;
    uint64_t ext_sxt = 0;
    uint64_t ext_cin = 0;

    for (int i = 0; i < 17; ++i) {
        const bool b0 = ((ext_mplr >> (2 * i)) & 1ull) != 0ull;
        const bool b1 = ((ext_mplr >> (2 * i + 1)) & 1ull) != 0ull;
        const bool b2 = ((ext_mplr >> (2 * i + 2)) & 1ull) != 0ull;
        const bool all_same = (!b0 && !b1 && !b2) || (b0 && b1 && b2);
        const bool cin_even = b2 && !(b2 && b1 && b0);
        const bool sxt_even = !((b2 ^ ((((mpcnd >> 31) & 1u) != 0u) && sign)) && !all_same);

        ext_cin |= static_cast<uint64_t>(cin_even) << (2 * i);
        ext_sxt |= static_cast<uint64_t>(sxt_even) << (2 * i);
        ext_sxt |= uint64_t{1} << (2 * i + 1);
    }

    out.sxt = (extract_bits(ext_sxt, 0, 31) << 1) | extract_bits(ext_cin, 32, 1);
    out.cin = extract_bits(ext_cin, 0, 32);

    if (mode) {
      out.sxt = (extract_bits(ext_sxt, 20, 11) << 21) | (extract_bits(ext_cin, 20, 13) << 6);
      out.cin = (extract_bits(ext_sxt, 0, 12) << 13) | extract_bits(ext_cin, 0, 13);
    }

    out.pp[0] = model_norm_pp(mpcnd, (mplr & 0x3u) << 1, sign);
    for (int i = 1; i < 16; ++i) {
        out.pp[i] = model_norm_pp(mpcnd, (mplr >> (2 * i - 1)) & 0x7u, sign);
    }
    out.pp[16] = model_msb_pp(mpcnd, ((mplr >> 31) & 1u) != 0u, sign);

    for (int i = 0; i < 7; ++i) {
        out.pp[i] &= mode ? kMaskLow13 : kMask33;
    }
    for (int i = 10; i < 17; ++i) {
        out.pp[i] &= mode ? kMaskHigh15 : kMask33;
    }

    return out;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("sxt", sim.dut->orig_sxt_o, sim.dut->new_sxt_o);
    sim.compare("cin", sim.dut->orig_cin_o, sim.dut->new_cin_o);
    sim.compare("pp1", static_cast<uint64_t>(sim.dut->orig_pp1_o), static_cast<uint64_t>(sim.dut->new_pp1_o));
    sim.compare("pp2", static_cast<uint64_t>(sim.dut->orig_pp2_o), static_cast<uint64_t>(sim.dut->new_pp2_o));
    sim.compare("pp3", static_cast<uint64_t>(sim.dut->orig_pp3_o), static_cast<uint64_t>(sim.dut->new_pp3_o));
    sim.compare("pp4", static_cast<uint64_t>(sim.dut->orig_pp4_o), static_cast<uint64_t>(sim.dut->new_pp4_o));
    sim.compare("pp5", static_cast<uint64_t>(sim.dut->orig_pp5_o), static_cast<uint64_t>(sim.dut->new_pp5_o));
    sim.compare("pp6", static_cast<uint64_t>(sim.dut->orig_pp6_o), static_cast<uint64_t>(sim.dut->new_pp6_o));
    sim.compare("pp7", static_cast<uint64_t>(sim.dut->orig_pp7_o), static_cast<uint64_t>(sim.dut->new_pp7_o));
    sim.compare("pp8", static_cast<uint64_t>(sim.dut->orig_pp8_o), static_cast<uint64_t>(sim.dut->new_pp8_o));
    sim.compare("pp9", static_cast<uint64_t>(sim.dut->orig_pp9_o), static_cast<uint64_t>(sim.dut->new_pp9_o));
    sim.compare("pp10", static_cast<uint64_t>(sim.dut->orig_pp10_o), static_cast<uint64_t>(sim.dut->new_pp10_o));
    sim.compare("pp11", static_cast<uint64_t>(sim.dut->orig_pp11_o), static_cast<uint64_t>(sim.dut->new_pp11_o));
    sim.compare("pp12", static_cast<uint64_t>(sim.dut->orig_pp12_o), static_cast<uint64_t>(sim.dut->new_pp12_o));
    sim.compare("pp13", static_cast<uint64_t>(sim.dut->orig_pp13_o), static_cast<uint64_t>(sim.dut->new_pp13_o));
    sim.compare("pp14", static_cast<uint64_t>(sim.dut->orig_pp14_o), static_cast<uint64_t>(sim.dut->new_pp14_o));
    sim.compare("pp15", static_cast<uint64_t>(sim.dut->orig_pp15_o), static_cast<uint64_t>(sim.dut->new_pp15_o));
    sim.compare("pp16", static_cast<uint64_t>(sim.dut->orig_pp16_o), static_cast<uint64_t>(sim.dut->new_pp16_o));
    sim.compare("pp17", static_cast<uint64_t>(sim.dut->orig_pp17_o), static_cast<uint64_t>(sim.dut->new_pp17_o));
}

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& mpcnd_cov, WordCoverage& mplr_cov,
                       BoolCoverage& mode_cov, BoolCoverage& sign_cov, uint32_t mpcnd,
                       uint32_t mplr, bool mode, bool sign, const char* name) {
    const ModelOutputs exp = model(mpcnd, mplr, mode, sign);
    sim.dut->mpcnd_i = mpcnd;
    sim.dut->mplr_i = mplr;
    sim.dut->mode_i = mode;
    sim.dut->sign_i = sign;
    mpcnd_cov.observe(mpcnd);
    mplr_cov.observe(mplr);
    mode_cov.observe(mode);
    sign_cov.observe(sign);
    sim.dut->eval();
    compare_outputs(sim);
    sim.check(sim.dut->new_sxt_o == exp.sxt, name);
    sim.check(sim.dut->new_cin_o == exp.cin, name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp1_o) == exp.pp[0], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp2_o) == exp.pp[1], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp3_o) == exp.pp[2], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp4_o) == exp.pp[3], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp5_o) == exp.pp[4], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp6_o) == exp.pp[5], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp7_o) == exp.pp[6], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp8_o) == exp.pp[7], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp9_o) == exp.pp[8], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp10_o) == exp.pp[9], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp11_o) == exp.pp[10], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp12_o) == exp.pp[11], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp13_o) == exp.pp[12], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp14_o) == exp.pp[13], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp15_o) == exp.pp[14], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp16_o) == exp.pp[15], name);
    sim.check(static_cast<uint64_t>(sim.dut->new_pp17_o) == exp.pp[16], name);
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage mpcnd_cov;
    WordCoverage mplr_cov;
    BoolCoverage mode_cov;
    BoolCoverage sign_cov;

    drive_and_compare(sim, mpcnd_cov, mplr_cov, mode_cov, sign_cov, 0x00000000u, 0x00000000u, false, false, "zero_unsigned");
    drive_and_compare(sim, mpcnd_cov, mplr_cov, mode_cov, sign_cov, 0xffffffffu, 0xaaaaaaaau, false, false, "full_unsigned");
    drive_and_compare(sim, mpcnd_cov, mplr_cov, mode_cov, sign_cov, 0x80000000u, 0x55555555u, false, true, "full_signed");
    drive_and_compare(sim, mpcnd_cov, mplr_cov, mode_cov, sign_cov, 0x12345678u, 0x87654321u, true, false, "ph_unsigned");
    drive_and_compare(sim, mpcnd_cov, mplr_cov, mode_cov, sign_cov, 0x89abcdefu, 0x13579bdfu, true, true, "ph_signed");
    drive_and_compare(sim, mpcnd_cov, mplr_cov, mode_cov, sign_cov, 0x7fffffffu, 0x80000001u, false, true, "msb_signed_mix");

    uint32_t seed = 0x6483d2b1u;
    for (int i = 0; i < 8192; ++i) {
        drive_and_compare(sim, mpcnd_cov, mplr_cov, mode_cov, sign_cov,
                          xorshift32(seed), xorshift32(seed),
                          (xorshift32(seed) & 1u) != 0u,
                          (xorshift32(seed) & 1u) != 0u, "random");
    }

    sim.check(mpcnd_cov.or_mask == 0xffffffffu, "mpcnd_i should drive every bit high");
    sim.check(mpcnd_cov.and_mask == 0x00000000u, "mpcnd_i should drive every bit low");
    sim.check(mplr_cov.or_mask == 0xffffffffu, "mplr_i should drive every bit high");
    sim.check(mplr_cov.and_mask == 0x00000000u, "mplr_i should drive every bit low");
    sim.check(mode_cov.full(), "mode_i should be driven to both 0 and 1");
    sim.check(sign_cov.full(), "sign_i should be driven to both 0 and 1");
    sim.check(sim.comparisons >= 150000, "txfma_booth_ppg_32r4 cosim should exceed 150k comparisons");
    return sim.finish();
}
