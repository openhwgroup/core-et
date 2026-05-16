// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for prim_hot2bin.

#include "Vprim_hot2bin_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vprim_hot2bin_tb;

static uint32_t ceil_log2(uint32_t value) {
    uint32_t width = 0;
    uint32_t limit = value - 1;
    while (limit != 0) {
        width++;
        limit >>= 1;
    }
    return width;
}

static uint64_t hot_mask(uint32_t hot_w) {
    return (hot_w >= 64) ? ~uint64_t{0} : ((uint64_t{1} << hot_w) - 1u);
}

static uint32_t expected_bin(uint64_t hot, uint32_t hot_w) {
    const uint32_t bin_w = ceil_log2(hot_w);
    const uint32_t bin_mask = (uint32_t{1} << bin_w) - 1u;
    uint32_t bin = 0;
    hot &= hot_mask(hot_w);
    for (uint32_t idx = 0; idx < hot_w; idx++) {
        if (((hot >> idx) & 1u) != 0u) {
            bin |= idx;
        }
    }
    return bin & bin_mask;
}

static std::string msg(const char* width_name, uint64_t hot,
                       uint32_t got, uint32_t exp) {
    return std::string(width_name) + " hot=0x" + std::to_string(hot) +
           " got=" + std::to_string(got) +
           " expected=" + std::to_string(exp);
}

static void clear_inputs(DUT* dut) {
    dut->hot2_i = 0;
    dut->hot3_i = 0;
    dut->hot8_i = 0;
    dut->hot33_i = 0;
}

static void check_hot2(SimCtrl<DUT>& sim, uint64_t hot) {
    auto* dut = sim.dut.get();
    dut->hot2_i = hot & hot_mask(2);
    dut->eval();
    const uint32_t exp = expected_bin(hot, 2);
    sim.check(dut->bin2_o == exp, msg("HotW=2", hot, dut->bin2_o, exp));
}

static void check_hot3(SimCtrl<DUT>& sim, uint64_t hot) {
    auto* dut = sim.dut.get();
    dut->hot3_i = hot & hot_mask(3);
    dut->eval();
    const uint32_t exp = expected_bin(hot, 3);
    sim.check(dut->bin3_o == exp, msg("HotW=3", hot, dut->bin3_o, exp));
}

static void check_hot8(SimCtrl<DUT>& sim, uint64_t hot) {
    auto* dut = sim.dut.get();
    dut->hot8_i = hot & hot_mask(8);
    dut->eval();
    const uint32_t exp = expected_bin(hot, 8);
    sim.check(dut->bin8_o == exp, msg("HotW=8", hot, dut->bin8_o, exp));
}

static void check_hot33(SimCtrl<DUT>& sim, uint64_t hot) {
    auto* dut = sim.dut.get();
    dut->hot33_i = hot & hot_mask(33);
    dut->eval();
    const uint32_t exp = expected_bin(hot, 33);
    sim.check(dut->bin33_o == exp, msg("HotW=33", hot, dut->bin33_o, exp));
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();

    clear_inputs(dut);
    sim.reset();

    printf("test 1: zero input on every supported test width\n");
    check_hot2(sim, 0);
    check_hot3(sim, 0);
    check_hot8(sim, 0);
    check_hot33(sim, 0);

    printf("test 2: exhaustive smallest supported width (HotW=2)\n");
    for (uint64_t hot = 0; hot < (uint64_t{1} << 2); hot++) {
        check_hot2(sim, hot);
    }

    printf("test 3: exhaustive non-power-of-two width (HotW=3)\n");
    for (uint64_t hot = 0; hot < (uint64_t{1} << 3); hot++) {
        check_hot3(sim, hot);
    }

    printf("test 4: exhaustive default width (HotW=8)\n");
    for (uint64_t hot = 0; hot < (uint64_t{1} << 8); hot++) {
        check_hot8(sim, hot);
    }

    printf("test 5: wider edge case (HotW=33)\n");
    check_hot33(sim, 0);
    for (uint32_t idx = 0; idx < 33; idx++) {
        check_hot33(sim, uint64_t{1} << idx);
    }

    // Multi-hot cases that distinguish OR-of-indices from lowest-priority or
    // highest-priority selection.  For example, bits 1 and 32 produce 33,
    // not 1 or 32; bits 31 and 32 produce 63, not any valid source index.
    check_hot33(sim, (uint64_t{1} << 0) | (uint64_t{1} << 32));
    check_hot33(sim, (uint64_t{1} << 1) | (uint64_t{1} << 32));
    check_hot33(sim, (uint64_t{1} << 15) | (uint64_t{1} << 16));
    check_hot33(sim, (uint64_t{1} << 31) | (uint64_t{1} << 32));
    check_hot33(sim, hot_mask(33));

    return sim.finish();
}
