// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: dcache_store_merge_unit (original) vs
// minion_dcache_store_merge_unit (new).

#include "Vcosim_minion_dcache_store_merge_unit_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>

using DUT = Vcosim_minion_dcache_store_merge_unit_tb;
using U256 = std::array<uint32_t, 8>;

namespace {

constexpr uint8_t kTypes[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0xA, 0xB, 0xC, 0xD, 0xE};

void set_u256(WData* dst, const U256& value) {
    for (int i = 0; i < 8; i++) {
        dst[i] = value[i];
    }
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    for (int i = 0; i < 8; i++) {
        sim.compare("merge_out[" + std::to_string(i) + "]",
                    sim.dut->orig_merge_out_o[i],
                    sim.dut->new_merge_out_o[i]);
    }
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->addr_i = 0;
    sim.dut->typ_i = 0;
    sim.dut->ps_mask_i = 0;
    sim.dut->enable_lo_i = 0;
    sim.dut->enable_hi_i = 0;
    for (int i = 0; i < 8; i++) {
        sim.dut->orig_data_i[i] = 0;
        sim.dut->store_data_i[i] = 0;
    }
    sim.reset(4);
    compare_outputs(sim);

    for (int i = 0; i < 16; i++) {
        U256 orig {};
        U256 store {};
        for (int w = 0; w < 8; w++) {
            orig[w] = 0x10001000U + uint32_t(i * 0x101U) + uint32_t(w * 0x11111111U);
            store[w] = 0xAA00AA00U ^ uint32_t(i * 0x01010101U) ^ uint32_t(w * 0x22222222U);
        }
        sim.dut->addr_i = i & 31;
        sim.dut->typ_i = kTypes[i % (sizeof(kTypes) / sizeof(kTypes[0]))];
        sim.dut->ps_mask_i = uint8_t(1U << (i & 7)) | uint8_t(1U << ((i + 3) & 7));
        set_u256(sim.dut->orig_data_i, orig);
        set_u256(sim.dut->store_data_i, store);
        sim.dut->enable_lo_i = i & 1;
        sim.dut->enable_hi_i = (i >> 1) & 1;
        tick_and_compare(sim);
    }

    uint32_t seed = 0x2468ACE1U;
    for (int i = 0; i < 1024; i++) {
        U256 orig {};
        U256 store {};
        for (int w = 0; w < 8; w++) {
            orig[w] = xorshift32(seed);
            store[w] = xorshift32(seed);
        }
        sim.dut->addr_i = xorshift32(seed) & 31U;
        sim.dut->typ_i = kTypes[xorshift32(seed) % (sizeof(kTypes) / sizeof(kTypes[0]))];
        sim.dut->ps_mask_i = xorshift32(seed) & 0xFFU;
        set_u256(sim.dut->orig_data_i, orig);
        set_u256(sim.dut->store_data_i, store);
        sim.dut->enable_lo_i = xorshift32(seed) & 1U;
        sim.dut->enable_hi_i = xorshift32(seed) & 1U;
        tick_and_compare(sim);
    }

    return sim.finish();
}
