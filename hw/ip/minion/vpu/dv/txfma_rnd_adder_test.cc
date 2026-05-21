// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_rnd_adder_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_rnd_adder_tb;

namespace {

constexpr uint64_t kMask33 = (1ull << 33) - 1ull;

uint64_t xorshift64(uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

uint32_t model(uint64_t in, uint32_t rnd_ctl, bool cin) {
    uint64_t round_vector = 0;
    round_vector |= static_cast<uint64_t>((rnd_ctl >> 3) & 0x3u) << 21;  // PH high L/R
    round_vector |= static_cast<uint64_t>((rnd_ctl >> 1) & 0x3u) << 8;   // PS L/R
    round_vector |= static_cast<uint64_t>(rnd_ctl & 0x1u) << 1;          // INT L
    const uint64_t sum = (in & kMask33) + round_vector + static_cast<uint64_t>(cin);
    return static_cast<uint32_t>((sum >> 1) & 0xffffffffu);
}

void check_case(SimCtrl<DUT>& sim, uint64_t in, uint32_t rnd_ctl, bool cin, const char* name) {
    sim.dut->in = in & kMask33;
    sim.dut->rnd_ctl = rnd_ctl & 0x1fu;
    sim.dut->cin = cin;
    sim.dut->eval();
    sim.check(sim.dut->out == model(in, rnd_ctl, cin), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0, 0, false, "zero_no_round");
    check_case(sim, 0, 0x01, false, "integer_l_round_bit");
    check_case(sim, 0, 0x06, false, "ps_lr_round_bits");
    check_case(sim, 0, 0x18, false, "ph_lr_round_bits");
    check_case(sim, kMask33, 0, true, "carry_out_wraps_to_zero_output");
    check_case(sim, 0x1fffffff0ull, 0x1f, true, "mixed_round_controls_and_cin");

    for (uint32_t ctl = 0; ctl < 32; ++ctl) {
        check_case(sim, 0, ctl, false, "control_sweep_zero");
        check_case(sim, kMask33, ctl, true, "control_sweep_all_ones");
    }

    uint64_t seed = 0x51f15eeda55a5eedull;
    for (int i = 0; i < 4096; ++i) {
        const uint64_t in = xorshift64(seed) & kMask33;
        const uint32_t ctl = xorshift64(seed) & 0x1fu;
        const bool cin = (xorshift64(seed) & 1ull) != 0;
        check_case(sim, in, ctl, cin, "random_round_adder");
    }

    return sim.finish();
}
