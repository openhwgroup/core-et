// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim driver for shirecache_pipe_tag_state_ram_wrap.

#include "Vcosim_shirecache_pipe_tag_state_ram_wrap_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdio>

using DUT = Vcosim_shirecache_pipe_tag_state_ram_wrap_tb;

static constexpr uint64_t DataMask = (1ULL << 40) - 1ULL;

static uint64_t data_pattern(uint32_t seed) {
    return (0x9a00000000ULL ^ (uint64_t(seed) * 0x1020305ULL)) & DataMask;
}

static void clear_inputs(DUT* dut) {
    dut->dft_sram_clk_i = 0;
    dut->clock_en_i = 0;
    dut->ram_delay_i = 2;
    dut->rd_en_i = 0;
    dut->wr_en_i = 0;
    dut->rd_addr_i = 0;
    dut->wr_addr_i = 0;
    dut->wr_data_i = 0;
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
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("rd_data", (uint64_t)(d->orig_rd_data_o & DataMask),
                (uint64_t)(d->new_rd_data_o & DataMask));
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

static void write_addr(CosimCtrl<DUT>& sim, uint16_t addr, uint64_t data,
                       uint8_t delay) {
    auto* d = sim.dut.get();
    d->ram_delay_i = delay;
    d->wr_addr_i = addr;
    d->wr_data_i = data & DataMask;
    d->wr_en_i = 1;
    d->rd_en_i = 0;
    d->clock_en_i = 1;
    tick_cmp(sim);
    d->wr_en_i = 0;
    d->clock_en_i = 0;
    idle(sim, delay + 1);
}

static void read_addr(CosimCtrl<DUT>& sim, uint16_t addr, uint8_t delay) {
    auto* d = sim.dut.get();
    d->ram_delay_i = delay;
    d->rd_addr_i = addr;
    d->rd_en_i = 1;
    d->wr_en_i = 0;
    d->clock_en_i = 1;
    tick_cmp(sim);
    d->rd_en_i = 0;
    d->clock_en_i = 0;
    idle(sim, delay + 1);
}

static void read_write(CosimCtrl<DUT>& sim, uint16_t rd_addr, uint16_t wr_addr,
                       uint64_t wr_data, uint8_t delay) {
    auto* d = sim.dut.get();
    d->ram_delay_i = delay;
    d->rd_addr_i = rd_addr;
    d->wr_addr_i = wr_addr;
    d->wr_data_i = wr_data & DataMask;
    d->rd_en_i = 1;
    d->wr_en_i = 1;
    d->clock_en_i = 1;
    tick_cmp(sim);
    d->rd_en_i = 0;
    d->wr_en_i = 0;
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
            write_addr(sim, addr + delay * 16, data_pattern(addr + delay * 29), delay);
        }
        for (uint16_t addr = 0; addr < 8; ++addr) {
            read_addr(sim, addr + delay * 16, delay);
        }
    }

    // Dual-port simultaneous read/write to different addresses.
    write_addr(sim, 100, data_pattern(100), 2);
    read_write(sim, 100, 101, data_pattern(101), 2);
    read_addr(sim, 101, 2);

    // DFT write inhibit.  The original generic dual-port model does not honor
    // ram_rei on port-B reads, so the active ram_rei read targets the currently
    // held address and keeps all visible outputs comparable while still toggling
    // the inhibit through both wrappers.
    d->dft_ram_wei_i = 1;
    write_addr(sim, 101, data_pattern(102), 2);
    d->dft_ram_wei_i = 0;
    read_addr(sim, 101, 2);
    d->dft_ram_rei_i = 1;
    read_addr(sim, 101, 2);
    d->dft_ram_rei_i = 0;

    d->dft_sram_clk_override_i = 1;
    pulse_dft_sram_clock(sim);
    d->dft_sram_clk_override_i = 0;
    idle(sim, 3);

    for (uint16_t addr = 128; addr < 192; ++addr) {
        write_addr(sim, addr, data_pattern(addr), 2 + (addr % 3));
    }

    uint32_t lfsr = 0x2455aa31U;
    for (int i = 0; i < 450; ++i) {
        lfsr = (lfsr >> 1) ^ (0u - (lfsr & 1u) & 0xd0000001U);
        uint8_t delay = 2 + (lfsr % 3);
        uint16_t addr = 128 + ((lfsr >> 7) & 0x3fU);
        if (lfsr & 0x40U) write_addr(sim, addr, data_pattern(lfsr), delay);
        else read_addr(sim, addr, delay);
    }

    return sim.finish();
}
