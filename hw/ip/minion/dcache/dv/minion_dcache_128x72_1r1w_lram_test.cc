// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_128x72_1r1w_lram — Verilator test.

#include "Vminion_dcache_128x72_1r1w_lram_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vminion_dcache_128x72_1r1w_lram_tb;

namespace {

constexpr uint64_t kHalfMask36 = (1ULL << 36) - 1ULL;

struct HalfModel {
    std::array<uint64_t, 128> mem {};
    uint64_t wr_data_del = 0;
    uint8_t rd_addr_reg = 0;
};

struct Model {
    HalfModel lo;
    HalfModel hi;
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void half_step(HalfModel& model, bool wr_pre, bool wr_en, uint8_t wr_addr, uint64_t wr_data,
               bool rd_en, uint8_t rd_addr) {
    // Transparent latch: at negedge, current wr_pre gates the data capture.
    if (wr_pre) model.wr_data_del = wr_data & kHalfMask36;
    if (wr_en) model.mem[wr_addr & 0x7f] = model.wr_data_del;
    if (rd_en) model.rd_addr_reg = rd_addr & 0x7f;
}

void model_step(Model& model, DUT* dut) {
    half_step(model.lo,
              dut->wr_enablelo_pre_i,
              dut->wr_enablelo_i,
              dut->wr_addr_i,
              uint64_t(dut->wr_data_lo_i),
              dut->rd_enable_i && !dut->rd_enablelob_i,
              dut->rd_addr_i);
    half_step(model.hi,
              dut->wr_enablehi_pre_i,
              dut->wr_enablehi_i,
              dut->wr_addr_i,
              uint64_t(dut->wr_data_hi_i),
              dut->rd_enable_i && !dut->rd_enablehib_i,
              dut->rd_addr_i);
}

void tick_only(SimCtrl<DUT>& sim, Model& model) {
    model_step(model, sim.dut.get());
    sim.dut->eval();
    sim.tick();
}

void tick_and_check(SimCtrl<DUT>& sim, Model& model, const std::string& label) {
    model_step(model, sim.dut.get());
    sim.dut->eval();
    sim.tick();
    const uint64_t expect_lo = model.lo.mem[model.lo.rd_addr_reg];
    const uint64_t expect_hi = model.hi.mem[model.hi.rd_addr_reg];
    sim.check(uint64_t(sim.dut->rd_data_lo_o) == expect_lo,
              label + " lo got=" + std::to_string(uint64_t(sim.dut->rd_data_lo_o)) +
              " expect=" + std::to_string(expect_lo));
    sim.check(uint64_t(sim.dut->rd_data_hi_o) == expect_hi,
              label + " hi got=" + std::to_string(uint64_t(sim.dut->rd_data_hi_o)) +
              " expect=" + std::to_string(expect_hi));
}

void init_array(SimCtrl<DUT>& sim, Model& model) {
    sim.dut->rd_enable_i = 0;
    sim.dut->rd_enablelob_i = 0;
    sim.dut->rd_enablehib_i = 0;
    for (uint32_t i = 0; i < 128; ++i) {
        sim.dut->wr_enablelo_pre_i = 1;
        sim.dut->wr_enablehi_pre_i = 1;
        sim.dut->wr_enablelo_i = 0;
        sim.dut->wr_enablehi_i = 0;
        sim.dut->wr_addr_i = i;
        sim.dut->wr_data_lo_i = (0x100000000ULL + i) & kHalfMask36;
        sim.dut->wr_data_hi_i = (0x200000000ULL + i) & kHalfMask36;
        tick_only(sim, model);

        sim.dut->wr_enablelo_pre_i = 0;
        sim.dut->wr_enablehi_pre_i = 0;
        sim.dut->wr_enablelo_i = 1;
        sim.dut->wr_enablehi_i = 1;
        sim.dut->wr_addr_i = i;
        tick_only(sim, model);
    }
    sim.dut->wr_enablelo_i = 0;
    sim.dut->wr_enablehi_i = 0;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    Model model;

    sim.dut->rd_addr_i = 0;
    sim.dut->wr_addr_i = 0;
    sim.dut->rd_enable_i = 0;
    sim.dut->rd_enablelob_i = 0;
    sim.dut->rd_enablehib_i = 0;
    sim.dut->wr_enablehi_i = 0;
    sim.dut->wr_enablehi_pre_i = 0;
    sim.dut->wr_enablelo_i = 0;
    sim.dut->wr_enablelo_pre_i = 0;
    sim.dut->wr_data_lo_i = 0;
    sim.dut->wr_data_hi_i = 0;
    sim.reset(2);
    sim.dut->eval();
    init_array(sim, model);

    printf("test 1: directed reads and writes\n");
    sim.dut->rd_enable_i = 1;
    sim.dut->rd_addr_i = 0;
    tick_and_check(sim, model, "read seeded word");

    sim.dut->rd_enable_i = 0;
    sim.dut->wr_enablelo_pre_i = 1;
    sim.dut->wr_enablehi_pre_i = 1;
    sim.dut->wr_data_lo_i = 0x123456789ULL & kHalfMask36;
    sim.dut->wr_data_hi_i = 0x0ABCDEF01ULL & kHalfMask36;
    tick_and_check(sim, model, "prime full write");

    sim.dut->wr_enablelo_pre_i = 0;
    sim.dut->wr_enablehi_pre_i = 0;
    sim.dut->wr_enablelo_i = 1;
    sim.dut->wr_enablehi_i = 1;
    sim.dut->wr_addr_i = 11;
    tick_and_check(sim, model, "commit full write");

    sim.dut->wr_enablelo_i = 0;
    sim.dut->wr_enablehi_i = 0;
    sim.dut->rd_enable_i = 1;
    sim.dut->rd_addr_i = 11;
    tick_and_check(sim, model, "read full word");

    sim.dut->rd_enable_i = 0;
    sim.dut->wr_enablehi_pre_i = 1;
    sim.dut->wr_data_hi_i = 0x012345678ULL & kHalfMask36;
    tick_and_check(sim, model, "prime high-half write");

    sim.dut->wr_enablehi_pre_i = 0;
    sim.dut->wr_enablehi_i = 1;
    sim.dut->wr_addr_i = 13;
    tick_and_check(sim, model, "commit high-half write");

    sim.dut->wr_enablehi_i = 0;
    sim.dut->rd_enable_i = 1;
    sim.dut->rd_enablelob_i = 1;
    sim.dut->rd_enablehib_i = 0;
    sim.dut->rd_addr_i = 13;
    tick_and_check(sim, model, "read high half only");

    sim.dut->rd_enable_i = 0;
    sim.dut->rd_addr_i = 5;
    tick_and_check(sim, model, "hold read state when rd_enable=0");

    printf("test 2: random stimulus\n");
    uint32_t seed = 0x10293847u;
    for (int i = 0; i < 256; ++i) {
        sim.dut->rd_addr_i = xorshift32(seed) & 0x7fu;
        sim.dut->wr_addr_i = xorshift32(seed) & 0x7fu;
        sim.dut->rd_enable_i = xorshift32(seed) & 1u;
        sim.dut->rd_enablelob_i = xorshift32(seed) & 1u;
        sim.dut->rd_enablehib_i = xorshift32(seed) & 1u;
        sim.dut->wr_enablehi_i = xorshift32(seed) & 1u;
        sim.dut->wr_enablehi_pre_i = xorshift32(seed) & 1u;
        sim.dut->wr_enablelo_i = xorshift32(seed) & 1u;
        sim.dut->wr_enablelo_pre_i = xorshift32(seed) & 1u;
        sim.dut->wr_data_lo_i = xorshift32(seed) & kHalfMask36;
        sim.dut->wr_data_hi_i = xorshift32(seed) & kHalfMask36;
        tick_and_check(sim, model, "random cycle " + std::to_string(i));
    }

    return sim.finish();
}
