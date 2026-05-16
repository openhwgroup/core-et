// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Standalone cosim for prim_hot2bin against CORE-ET hot2bin + onehot_mux.

#include "Vcosim_prim_hot2bin_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vcosim_prim_hot2bin_tb;

static uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

static uint64_t hot_mask(uint32_t hot_w) {
    return (hot_w >= 64) ? ~uint64_t{0} : ((uint64_t{1} << hot_w) - 1u);
}

static void drive(DUT* dut, uint64_t hot2, uint64_t hot3,
                  uint64_t hot8, uint64_t hot33) {
    dut->hot2_i = hot2 & hot_mask(2);
    dut->hot3_i = hot3 & hot_mask(3);
    dut->hot8_i = hot8 & hot_mask(8);
    dut->hot33_i = hot33 & hot_mask(33);
}

static void note_bits(uint64_t hot, uint32_t hot_w,
                      uint64_t& seen_one, uint64_t& seen_zero) {
    const uint64_t mask = hot_mask(hot_w);
    hot &= mask;
    seen_one |= hot;
    seen_zero |= (~hot) & mask;
}

static void compare_all(CosimCtrl<DUT>& sim, const char* phase) {
    auto* dut = sim.dut.get();
    dut->eval();
    sim.compare(std::string(phase) + ":bin2",
                dut->orig_bin2_o, dut->new_bin2_o);
    sim.compare(std::string(phase) + ":bin3",
                dut->orig_bin3_o, dut->new_bin3_o);
    sim.compare(std::string(phase) + ":bin8",
                dut->orig_bin8_o, dut->new_bin8_o);
    sim.compare(std::string(phase) + ":bin33",
                dut->orig_bin33_o, dut->new_bin33_o);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();

    uint64_t seen_one2 = 0;
    uint64_t seen_zero2 = 0;
    uint64_t seen_one3 = 0;
    uint64_t seen_zero3 = 0;
    uint64_t seen_one8 = 0;
    uint64_t seen_zero8 = 0;
    uint64_t seen_one33 = 0;
    uint64_t seen_zero33 = 0;

    auto apply = [&](uint64_t hot2, uint64_t hot3, uint64_t hot8,
                     uint64_t hot33, const char* phase) {
        drive(dut, hot2, hot3, hot8, hot33);
        note_bits(hot2, 2, seen_one2, seen_zero2);
        note_bits(hot3, 3, seen_one3, seen_zero3);
        note_bits(hot8, 8, seen_one8, seen_zero8);
        note_bits(hot33, 33, seen_one33, seen_zero33);
        sim.tick();
        compare_all(sim, phase);
    };

    drive(dut, 0, 0, 0, 0);
    sim.reset();

    printf("phase 1: directed zero and single-hot coverage\n");
    apply(0, 0, 0, 0, "zero");

    for (uint32_t idx = 0; idx < 33; idx++) {
        const uint64_t bit = uint64_t{1} << idx;
        apply(idx < 2 ? bit : 0,
              idx < 3 ? bit : 0,
              idx < 8 ? bit : 0,
              bit,
              "single_hot");
    }

    printf("phase 2: directed multi-hot OR-of-indices coverage\n");
    apply(0x3u, 0x7u, 0xffu, hot_mask(33), "all_ones");
    apply(0x3u, 0x6u, (uint64_t{1} << 2) | (uint64_t{1} << 4),
          (uint64_t{1} << 1) | (uint64_t{1} << 32), "or_not_priority_a");
    apply(0x1u, 0x5u, (uint64_t{1} << 1) | (uint64_t{1} << 2),
          (uint64_t{1} << 15) | (uint64_t{1} << 16), "or_not_priority_b");
    apply(0x2u, 0x3u, (uint64_t{1} << 3) | (uint64_t{1} << 5),
          (uint64_t{1} << 31) | (uint64_t{1} << 32), "or_not_priority_c");
    apply(0x0u, 0x0u, (uint64_t{1} << 0) | (uint64_t{1} << 7),
          (uint64_t{1} << 0) | (uint64_t{1} << 32), "lowest_highest");
    apply(0x2u, 0x4u, (uint64_t{1} << 7),
          (uint64_t{1} << 32), "highest_only");
    apply(0x1u, 0x1u, (uint64_t{1} << 0),
          (uint64_t{1} << 0), "lowest_only");
    apply(0x0u, 0x2u, 0xa5u,
          0x0000000155555555ull, "alternating");

    printf("phase 3: random stimulus\n");
    uint32_t seed = 0x00c0ffeeu;
    for (int i = 0; i < 3000; i++) {
        const uint32_t r0 = xorshift32(seed);
        const uint32_t r1 = xorshift32(seed);
        const uint64_t wide = ((uint64_t{r0} & 1u) << 32) | r1;
        apply(r0 & hot_mask(2),
              (r0 >> 4) & hot_mask(3),
              (r0 >> 12) & hot_mask(8),
              wide & hot_mask(33),
              "random");
    }

    sim.check((seen_one2 & hot_mask(2)) == hot_mask(2),
              "HotW=2 every input bit saw 1");
    sim.check((seen_zero2 & hot_mask(2)) == hot_mask(2),
              "HotW=2 every input bit saw 0");
    sim.check((seen_one3 & hot_mask(3)) == hot_mask(3),
              "HotW=3 every input bit saw 1");
    sim.check((seen_zero3 & hot_mask(3)) == hot_mask(3),
              "HotW=3 every input bit saw 0");
    sim.check((seen_one8 & hot_mask(8)) == hot_mask(8),
              "HotW=8 every input bit saw 1");
    sim.check((seen_zero8 & hot_mask(8)) == hot_mask(8),
              "HotW=8 every input bit saw 0");
    sim.check((seen_one33 & hot_mask(33)) == hot_mask(33),
              "HotW=33 every input bit saw 1");
    sim.check((seen_zero33 & hot_mask(33)) == hot_mask(33),
              "HotW=33 every input bit saw 0");

    return sim.finish();
}
