// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_wallace2_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_txfma_wallace2_tb;

namespace {

constexpr uint64_t kMask33 = (1ull << 33) - 1ull;
constexpr int kRandomIters = 12500;

struct WordCoverage32 {
    uint32_t or_mask = 0;
    uint32_t and_mask = 0xffffffffu;
    void observe(uint32_t value) {
        or_mask |= value;
        and_mask &= value;
    }
};

struct WordCoverage33 {
    uint64_t or_mask = 0;
    uint64_t and_mask = kMask33;
    void observe(uint64_t value) {
        const uint64_t masked = value & kMask33;
        or_mask |= masked;
        and_mask &= masked;
    }
};

struct WordCoverage64 {
    uint64_t or_mask = 0;
    uint64_t and_mask = ~0ull;
    void observe(uint64_t value) {
        or_mask |= value;
        and_mask &= value;
    }
};

struct Csa42Outputs {
    uint64_t c = 0;
    uint64_t s = 0;
};

struct Wallace2Outputs {
    uint64_t c41 = 0;
    uint64_t s41 = 0;
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t rand33(uint32_t& state) {
    return ((static_cast<uint64_t>(xorshift32(state)) << 1) | (xorshift32(state) & 1u)) & kMask33;
}

bool majority(bool a, bool b, bool c) {
    return (a && b) || (a && c) || (b && c);
}

Csa42Outputs model_csa42_array(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3) {
    Csa42Outputs out;
    bool carry = false;

    for (int i = 0; i < 64; ++i) {
        const bool x0b = ((x0 >> i) & 1ull) != 0ull;
        const bool x1b = ((x1 >> i) & 1ull) != 0ull;
        const bool x2b = ((x2 >> i) & 1ull) != 0ull;
        const bool x3b = ((x3 >> i) & 1ull) != 0ull;

        const bool s0 = x0b ^ x1b ^ x2b;
        const bool cout = majority(x0b, x1b, x2b);
        const bool s = s0 ^ x3b ^ carry;
        const bool c = majority(s0, x3b, carry);

        out.c |= static_cast<uint64_t>(c) << i;
        out.s |= static_cast<uint64_t>(s) << i;
        carry = cout;
    }

    return out;
}

Wallace2Outputs model(uint32_t sxt, uint32_t cin, uint64_t pp17, uint64_t c21, uint64_t c22, uint64_t s21, uint64_t s22) {
    Wallace2Outputs out;
    const uint64_t pp17_ext = (pp17 & 0xffff'ffffull) << 32;
    const uint64_t sxt_cin = (static_cast<uint64_t>(sxt) << 32) | cin;

    const Csa42Outputs l31 = model_csa42_array(c21 << 1, s21, c22 << 1, s22);
    const Csa42Outputs l41 = model_csa42_array(l31.c << 1, l31.s, pp17_ext, sxt_cin);

    out.c41 = l41.c;
    out.s41 = l41.s;
    return out;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("c41", sim.dut->orig_c41_o, sim.dut->new_c41_o);
    sim.compare("s41", sim.dut->orig_s41_o, sim.dut->new_s41_o);
}

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage32& sxt_cov, WordCoverage32& cin_cov,
                       WordCoverage33& pp17_cov, WordCoverage64& c21_cov, WordCoverage64& c22_cov,
                       WordCoverage64& s21_cov, WordCoverage64& s22_cov,
                       uint32_t sxt, uint32_t cin, uint64_t pp17, uint64_t c21,
                       uint64_t c22, uint64_t s21, uint64_t s22, const char* name) {
    const Wallace2Outputs exp = model(sxt, cin, pp17, c21, c22, s21, s22);
    sim.dut->sxt_i = sxt;
    sim.dut->cin_i = cin;
    sim.dut->pp17_i = pp17;
    sim.dut->c21_i = c21;
    sim.dut->c22_i = c22;
    sim.dut->s21_i = s21;
    sim.dut->s22_i = s22;
    sxt_cov.observe(sxt);
    cin_cov.observe(cin);
    pp17_cov.observe(pp17);
    c21_cov.observe(c21);
    c22_cov.observe(c22);
    s21_cov.observe(s21);
    s22_cov.observe(s22);
    sim.dut->eval();
    compare_outputs(sim);
    sim.check(sim.dut->new_c41_o == exp.c41, name);
    sim.check(sim.dut->new_s41_o == exp.s41, name);
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage32 sxt_cov;
    WordCoverage32 cin_cov;
    WordCoverage33 pp17_cov;
    WordCoverage64 c21_cov;
    WordCoverage64 c22_cov;
    WordCoverage64 s21_cov;
    WordCoverage64 s22_cov;

    drive_and_compare(sim, sxt_cov, cin_cov, pp17_cov, c21_cov, c22_cov, s21_cov, s22_cov,
                      0, 0, 0, 0, 0, 0, 0, "all_zero");
    drive_and_compare(sim, sxt_cov, cin_cov, pp17_cov, c21_cov, c22_cov, s21_cov, s22_cov,
                      0xffff'ffffu, 0xffff'ffffu, kMask33, ~0ull, ~0ull, ~0ull, ~0ull, "all_one");
    drive_and_compare(sim, sxt_cov, cin_cov, pp17_cov, c21_cov, c22_cov, s21_cov, s22_cov,
                      0x1234'5678u, 0x9abc'def0u, 1ull << 32, 0, 0, 0, 0, "pp17_msb_dropped");
    drive_and_compare(sim, sxt_cov, cin_cov, pp17_cov, c21_cov, c22_cov, s21_cov, s22_cov,
                      0x89ab'cdefu, 0x1357'9bdfu, 0x1ffff'ffffull,
                      0x0123'4567'89ab'cdefull, 0xfedc'ba98'7654'3210ull,
                      0x55aa'55aa'55aa'55aaull, 0xaa55'aa55'aa55'aa55ull, "mixed");

    for (uint64_t bit = 0; bit < 33; ++bit) {
        drive_and_compare(sim, sxt_cov, cin_cov, pp17_cov, c21_cov, c22_cov, s21_cov, s22_cov,
                          static_cast<uint32_t>(1ull << (bit & 31)),
                          static_cast<uint32_t>(~(1ull << (bit & 31))),
                          1ull << bit, 0ull, 0ull, 0ull, 0ull, "walk_pp17");
    }

    uint32_t seed = 0x50d7d34bu;
    for (int i = 0; i < kRandomIters; ++i) {
        drive_and_compare(sim, sxt_cov, cin_cov, pp17_cov, c21_cov, c22_cov, s21_cov, s22_cov,
                          xorshift32(seed), xorshift32(seed), rand33(seed),
                          (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed),
                          (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed),
                          (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed),
                          (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed),
                          "random");
    }

    sim.check(sxt_cov.or_mask == 0xffff'ffffu, "sxt_i should drive every bit high");
    sim.check(sxt_cov.and_mask == 0u, "sxt_i should drive every bit low");
    sim.check(cin_cov.or_mask == 0xffff'ffffu, "cin_i should drive every bit high");
    sim.check(cin_cov.and_mask == 0u, "cin_i should drive every bit low");
    sim.check(pp17_cov.or_mask == kMask33, "pp17_i should drive every bit high");
    sim.check(pp17_cov.and_mask == 0ull, "pp17_i should drive every bit low");
    sim.check(c21_cov.or_mask == ~0ull, "c21_i should drive every bit high");
    sim.check(c21_cov.and_mask == 0ull, "c21_i should drive every bit low");
    sim.check(c22_cov.or_mask == ~0ull, "c22_i should drive every bit high");
    sim.check(c22_cov.and_mask == 0ull, "c22_i should drive every bit low");
    sim.check(s21_cov.or_mask == ~0ull, "s21_i should drive every bit high");
    sim.check(s21_cov.and_mask == 0ull, "s21_i should drive every bit low");
    sim.check(s22_cov.or_mask == ~0ull, "s22_i should drive every bit high");
    sim.check(s22_cov.and_mask == 0ull, "s22_i should drive every bit low");
    sim.check(sim.comparisons >= 25000, "txfma_wallace2 cosim should exceed 25k comparisons");
    return sim.finish();
}
