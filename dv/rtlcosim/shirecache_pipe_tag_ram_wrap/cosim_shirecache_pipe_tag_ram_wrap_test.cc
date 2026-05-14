// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim driver for shirecache_pipe_tag_ram_wrap.

#include "Vcosim_shirecache_pipe_tag_ram_wrap_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vcosim_shirecache_pipe_tag_ram_wrap_tb;

static constexpr int TagWords = 4;
static constexpr uint32_t TagLastMask = 0x000fffffU;  // 116 bits total.

static void set_wide(VlWide<TagWords>& dst, const std::array<uint32_t, TagWords>& src) {
    for (int i = 0; i < TagWords; ++i) dst[i] = src[i];
    dst[TagWords - 1] &= TagLastMask;
}

static std::array<uint32_t, TagWords> tag_pattern(uint32_t seed) {
    return {0x50000000U ^ seed,
            0xa5a50000U ^ (seed * 17U),
            0x12340000U ^ (seed * 257U),
            (0x00010000U ^ (seed * 4099U)) & TagLastMask};
}

static void clear_inputs(DUT* dut) {
    dut->dft_sram_clk_i = 0;
    dut->clock_en_i = 0;
    dut->ram_delay_i = 2;
    dut->rd_en_i = 0;
    dut->wr_en_i = 0;
    dut->addr_i = 0;
    dut->dft_scanmode_i = 0;
    dut->dft_scan_reset_n_i = 0;
    dut->dft_sram_clk_override_i = 0;
    dut->dft_ram_rei_i = 0;
    dut->dft_ram_wei_i = 0;
    dut->ram_cfg_test_en_i = 0;
    dut->ram_cfg_rm_i = 0;
    dut->ram_cfg_rme_i = 0;
    dut->ram_cfg_ra_i = 0;
    dut->ram_cfg_wa_i = 0;
    dut->ram_cfg_wpulse_i = 0;
    dut->ram_cfg_deep_sleep_i = 0;
    dut->ram_cfg_shut_down_i = 0;
    std::array<uint32_t, TagWords> zero{};
    set_wide(dut->wr_data_i, zero);
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    for (int i = 0; i < TagWords; ++i) {
        const uint32_t mask = (i == TagWords - 1) ? TagLastMask : 0xffffffffU;
        sim.compare("rd_data[" + std::to_string(i) + "]",
                    d->orig_rd_data_o[i] & mask,
                    d->new_rd_data_o[i] & mask);
    }
}

static void tick_cmp(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_all(sim);
}

static void idle(CosimCtrl<DUT>& sim, int cycles) {
    auto* d = sim.dut.get();
    d->clock_en_i = 0;
    d->rd_en_i = 0;
    d->wr_en_i = 0;
    for (int i = 0; i < cycles; ++i) tick_cmp(sim);
}

static void pulse_dft_sram_clock(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    d->dft_sram_clk_i = 0;
    tick_cmp(sim);
    d->dft_sram_clk_i = 1;
    tick_cmp(sim);
    d->dft_sram_clk_i = 0;
    tick_cmp(sim);
}

static void write_addr(CosimCtrl<DUT>& sim, uint16_t addr,
                       const std::array<uint32_t, TagWords>& data,
                       uint8_t delay, bool override_clk = false) {
    auto* d = sim.dut.get();
    d->ram_delay_i = delay;
    d->addr_i = addr;
    set_wide(d->wr_data_i, data);
    d->wr_en_i = 1;
    d->rd_en_i = 0;
    d->clock_en_i = 1;
    d->dft_sram_clk_override_i = override_clk ? 1 : 0;
    if (override_clk) pulse_dft_sram_clock(sim);
    else tick_cmp(sim);
    d->wr_en_i = 0;
    d->clock_en_i = 0;
    d->dft_sram_clk_override_i = 0;
    idle(sim, delay + 1);
}

static void read_addr(CosimCtrl<DUT>& sim, uint16_t addr, uint8_t delay,
                      bool override_clk = false) {
    auto* d = sim.dut.get();
    d->ram_delay_i = delay;
    d->addr_i = addr;
    d->rd_en_i = 1;
    d->wr_en_i = 0;
    d->clock_en_i = 1;
    d->dft_sram_clk_override_i = override_clk ? 1 : 0;
    if (override_clk) pulse_dft_sram_clock(sim);
    else tick_cmp(sim);
    d->rd_en_i = 0;
    d->clock_en_i = 0;
    d->dft_sram_clk_override_i = 0;
    idle(sim, delay + 1);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset();
    compare_all(sim);

    // Exercise RAM config and scan fields during idle cycles.  These are
    // configuration/test controls; functional comparisons are meaningful while
    // no request is active.
    d->dft_scanmode_i = 1;
    d->dft_scan_reset_n_i = 1;
    d->ram_cfg_test_en_i = 1;
    d->ram_cfg_rm_i = 0xf;
    d->ram_cfg_rme_i = 1;
    d->ram_cfg_ra_i = 3;
    d->ram_cfg_wa_i = 2;
    d->ram_cfg_wpulse_i = 5;
    d->ram_cfg_deep_sleep_i = 1;
    d->ram_cfg_shut_down_i = 1;
    idle(sim, 3);
    d->dft_scanmode_i = 0;
    d->dft_scan_reset_n_i = 0;
    d->ram_cfg_test_en_i = 0;
    d->ram_cfg_rm_i = 0;
    d->ram_cfg_rme_i = 0;
    d->ram_cfg_ra_i = 0;
    d->ram_cfg_wa_i = 0;
    d->ram_cfg_wpulse_i = 0;
    d->ram_cfg_deep_sleep_i = 0;
    d->ram_cfg_shut_down_i = 0;

    // Directed write/read checks for all supported RAM delay encodings.
    for (uint8_t delay : {uint8_t{2}, uint8_t{3}, uint8_t{4}}) {
        for (uint16_t addr = 0; addr < 8; ++addr) {
            write_addr(sim, addr + delay * 16, tag_pattern(addr + delay * 31), delay);
        }
        for (uint16_t addr = 0; addr < 8; ++addr) {
            read_addr(sim, addr + delay * 16, delay);
        }
    }

    // DFT write inhibit: attempted write must not replace the previous value.
    write_addr(sim, 100, tag_pattern(100), 2);
    d->dft_ram_wei_i = 1;
    write_addr(sim, 100, tag_pattern(101), 2);
    d->dft_ram_wei_i = 0;
    read_addr(sim, 100, 2);

    // DFT read-enable inhibit: read request should leave output held.
    d->dft_ram_rei_i = 1;
    read_addr(sim, 101, 2);
    d->dft_ram_rei_i = 0;
    read_addr(sim, 100, 2);

    // DFT SRAM-clock override using the separate DFT SRAM clock.  The mux path
    // is exercised during idle so the original and new generated functional
    // SRAM clocks are not switched mid-transaction.
    d->dft_sram_clk_override_i = 1;
    pulse_dft_sram_clock(sim);
    d->dft_sram_clk_override_i = 0;
    idle(sim, 3);

    // Pre-initialize a bounded random address pool so every later read observes
    // defined data in both memories.
    for (uint16_t addr = 128; addr < 192; ++addr) {
        write_addr(sim, addr, tag_pattern(addr), 2 + (addr % 3));
    }

    // Pseudo-random valid traffic, spaced so it obeys the original wrapper's
    // no-new-request-while-busy assertion contract.
    uint32_t lfsr = 0x1badc0deU;
    for (int i = 0; i < 450; ++i) {
        lfsr = (lfsr >> 1) ^ (0u - (lfsr & 1u) & 0xd0000001U);
        uint8_t delay = 2 + (lfsr % 3);
        uint16_t addr = 128 + ((lfsr >> 8) & 0x3fU);
        if (lfsr & 0x20U) write_addr(sim, addr, tag_pattern(lfsr), delay);
        else read_addr(sim, addr, delay);
    }

    return sim.finish();
}
