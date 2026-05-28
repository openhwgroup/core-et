// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_esr_cache_bank_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <random>

namespace {
constexpr uint16_t M = 3u << 10;
constexpr uint16_t U = 0u << 10;
constexpr uint16_t D = 2u << 10;
constexpr uint16_t kTraceAddrEnable = D | 0x3f0u;
constexpr uint16_t kTraceAddrValue = D | 0x3f1u;
constexpr uint16_t kTraceCtl = D | 0x3f2u;
constexpr std::array<uint16_t, 35> kAddrs = {
    uint16_t(M | 0x000), uint16_t(M | 0x001), uint16_t(M | 0x002),
    uint16_t(M | 0x003), uint16_t(M | 0x004), uint16_t(M | 0x005),
    uint16_t(M | 0x006), uint16_t(M | 0x007), uint16_t(M | 0x008),
    uint16_t(M | 0x009), uint16_t(M | 0x00a), uint16_t(M | 0x00b),
    uint16_t(M | 0x00c), uint16_t(M | 0x00d), uint16_t(M | 0x00e),
    uint16_t(M | 0x00f), uint16_t(M | 0x010), uint16_t(M | 0x011),
    uint16_t(M | 0x012), uint16_t(M | 0x013), uint16_t(M | 0x014),
    uint16_t(M | 0x017), uint16_t(M | 0x018), uint16_t(M | 0x019),
    uint16_t(M | 0x01a), uint16_t(M | 0x01b), uint16_t(M | 0x01c),
    uint16_t(U | 0x020), uint16_t(D | 0x3f0), uint16_t(D | 0x3f1),
    uint16_t(D | 0x3f2), uint16_t(M | 0x155), uint16_t(U | 0x155),
    uint16_t(D | 0x155), uint16_t(M | 0x3ff)};

void set_defaults(CosimCtrl<Vcosim_esr_cache_bank_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    d->test_en_i = 0;
    d->apb_paddr_i = 0;
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwdata_i = 0;
    d->status_seed_i = 0;
}

void reset_all(CosimCtrl<Vcosim_esr_cache_bank_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_c_ni = 0;
    d->rst_w_ni = 0;
    d->rst_d_ni = 0;
    for (int i = 0; i < 5; ++i) sim.tick();
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    sim.tick();
}

void compare_all(CosimCtrl<Vcosim_esr_cache_bank_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("apb_pready", d->orig_apb_pready_o, d->new_apb_pready_o);
    sim.compare("apb_prdata", d->orig_apb_prdata_o, d->new_apb_prdata_o);
    sim.compare("apb_pslverr", d->orig_apb_pslverr_o, d->new_apb_pslverr_o);
    sim.compare("ctl_fields_match", static_cast<uint8_t>(1), d->ctl_fields_match_o);
    sim.compare("swizzle", d->orig_swizzle_o, d->new_swizzle_o);
    sim.compare("eco", d->orig_eco_o, d->new_eco_o);
}

void tick_cmp(CosimCtrl<Vcosim_esr_cache_bank_tb>& sim, uint64_t& seed) {
    seed = seed * 6364136223846793005ull + 1442695040888963407ull;
    sim.dut->status_seed_i = seed;
    sim.tick();
    compare_all(sim);
}

void apb_write(CosimCtrl<Vcosim_esr_cache_bank_tb>& sim, uint16_t addr, uint64_t data,
               uint64_t& seed) {
    auto* d = sim.dut.get();
    d->apb_paddr_i = addr;
    d->apb_pwdata_i = data;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    tick_cmp(sim, seed);
    d->apb_penable_i = 1;
    tick_cmp(sim, seed);
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwrite_i = 0;
    tick_cmp(sim, seed);
}

void pulse_debug_reset(CosimCtrl<Vcosim_esr_cache_bank_tb>& sim, uint64_t& seed) {
    sim.dut->rst_d_ni = 0;
    tick_cmp(sim, seed);
    sim.dut->rst_d_ni = 1;
    tick_cmp(sim, seed);
}

void pulse_cache_reset(CosimCtrl<Vcosim_esr_cache_bank_tb>& sim, uint64_t& seed) {
    sim.dut->rst_c_ni = 0;
    tick_cmp(sim, seed);
    sim.dut->rst_c_ni = 1;
    tick_cmp(sim, seed);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_esr_cache_bank_tb> sim(argc, argv);
    set_defaults(sim);
    reset_all(sim);
    uint64_t seed = 0x1234;
    compare_all(sim);

    apb_write(sim, kTraceAddrEnable, 0x000000f00dull, seed);
    apb_write(sim, kTraceAddrValue, 0x0012345678ull, seed);
    apb_write(sim, kTraceCtl, 0x5a5u, seed);
    pulse_debug_reset(sim, seed);
    apb_write(sim, kTraceAddrEnable, 0x0000000aceull, seed);
    apb_write(sim, kTraceCtl, 0x3c3u, seed);
    pulse_cache_reset(sim, seed);

    auto* d = sim.dut.get();
    std::mt19937_64 rng(0xcaceba5e);
    for (int i = 0; i < 2200; ++i) {
        const uint64_t r = rng();
        d->test_en_i = (r >> 8) & 1u;
        d->apb_paddr_i = kAddrs[r % kAddrs.size()];
        d->apb_pwdata_i = rng();
        d->apb_pwrite_i = (r >> 16) & 1u;
        d->apb_psel_i = 1;
        d->apb_penable_i = 0;
        tick_cmp(sim, seed);
        d->apb_penable_i = 1;
        tick_cmp(sim, seed);
        d->apb_psel_i = 0;
        d->apb_penable_i = 0;
        d->apb_pwrite_i = 0;
        tick_cmp(sim, seed);
        if ((i % 257) == 3) {
            d->rst_w_ni = 0;
            tick_cmp(sim, seed);
            d->rst_w_ni = 1;
            tick_cmp(sim, seed);
        }
    }

    return sim.finish();
}
