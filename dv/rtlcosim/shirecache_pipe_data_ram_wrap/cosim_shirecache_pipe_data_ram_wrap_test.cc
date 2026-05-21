// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim driver for shirecache_pipe_data_ram_wrap.

#include "Vcosim_shirecache_pipe_data_ram_wrap_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vcosim_shirecache_pipe_data_ram_wrap_tb;

static constexpr int DataWords = 18;

static std::array<uint32_t, DataWords> data_pattern(uint32_t seed) {
    std::array<uint32_t, DataWords> out{};
    uint32_t x = seed ^ 0x6d2b79f5U;
    for (int i = 0; i < DataWords; ++i) {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        out[i] = x + 0x10001U * uint32_t(i + 1);
    }
    return out;
}

static void set_wide(VlWide<DataWords>& dst, const std::array<uint32_t, DataWords>& src) {
    for (int i = 0; i < DataWords; ++i) dst[i] = src[i];
}

static void clear_inputs(DUT* dut) {
    dut->dft_sram_clk_i = 0;
    dut->clock_en_i = 0;
    dut->ram_delay_i = 2;
    dut->rd_en_i = 0;
    dut->wr_en_i = 0;
    dut->addr_i = 0;
    dut->wr_qwen_i = 0;
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
    std::array<uint32_t, DataWords> zero{};
    set_wide(dut->wr_data_i, zero);
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    for (int i = 0; i < DataWords; ++i) {
        sim.compare("rd_data[" + std::to_string(i) + "]",
                    d->orig_rd_data_o[i], d->new_rd_data_o[i]);
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
                       const std::array<uint32_t, DataWords>& data,
                       uint8_t qwen, uint8_t delay) {
    auto* d = sim.dut.get();
    d->ram_delay_i = delay;
    d->addr_i = addr;
    set_wide(d->wr_data_i, data);
    d->wr_qwen_i = qwen;
    d->wr_en_i = 1;
    d->rd_en_i = 0;
    d->clock_en_i = 1;
    tick_cmp(sim);
    d->wr_en_i = 0;
    d->clock_en_i = 0;
    d->wr_qwen_i = 0;
    idle(sim, delay + 1);
}

static void read_addr(CosimCtrl<DUT>& sim, uint16_t addr, uint8_t delay) {
    auto* d = sim.dut.get();
    d->ram_delay_i = delay;
    d->addr_i = addr;
    d->rd_en_i = 1;
    d->wr_en_i = 0;
    d->clock_en_i = 1;
    tick_cmp(sim);
    d->rd_en_i = 0;
    d->clock_en_i = 0;
    idle(sim, delay + 1);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset();
    compare_all(sim);

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

    for (uint8_t delay : {uint8_t{2}, uint8_t{3}, uint8_t{4}}) {
        for (uint16_t addr = 0; addr < 8; ++addr) {
            write_addr(sim, addr + delay * 16, data_pattern(addr + delay * 31), 0xf, delay);
        }
        for (uint16_t addr = 0; addr < 8; ++addr) {
            read_addr(sim, addr + delay * 16, delay);
        }
    }

    // Per-QW write masks: overwrite selected quadwords only.
    write_addr(sim, 100, data_pattern(100), 0xf, 2);
    write_addr(sim, 100, data_pattern(101), 0x1, 2);
    write_addr(sim, 100, data_pattern(102), 0x4, 2);
    read_addr(sim, 100, 2);

    // DFT write inhibit and read-enable inhibit.
    d->dft_ram_wei_i = 1;
    write_addr(sim, 100, data_pattern(103), 0xf, 2);
    d->dft_ram_wei_i = 0;
    read_addr(sim, 100, 2);
    d->dft_ram_rei_i = 1;
    read_addr(sim, 100, 2);
    d->dft_ram_rei_i = 0;
    read_addr(sim, 100, 2);

    d->dft_sram_clk_override_i = 1;
    pulse_dft_sram_clock(sim);
    d->dft_sram_clk_override_i = 0;
    idle(sim, 3);

    for (uint16_t addr = 128; addr < 192; ++addr) {
        write_addr(sim, addr, data_pattern(addr), 0xf, 2 + (addr % 3));
    }

    uint32_t lfsr = 0x0ddc0ffeU;
    for (int i = 0; i < 450; ++i) {
        lfsr = (lfsr >> 1) ^ (0u - (lfsr & 1u) & 0xd0000001U);
        uint8_t delay = 2 + (lfsr % 3);
        uint16_t addr = 128 + ((lfsr >> 8) & 0x3fU);
        if (lfsr & 0x80U) write_addr(sim, addr, data_pattern(lfsr), (lfsr >> 4) & 0xfU, delay);
        else read_addr(sim, addr, delay);
    }

    return sim.finish();
}
