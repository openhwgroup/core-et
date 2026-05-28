// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim driver for shirecache_pipe_sub_bank_mem.

#include "Vcosim_shirecache_pipe_sub_bank_mem_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vcosim_shirecache_pipe_sub_bank_mem_tb;

static constexpr uint64_t TsDataMask = (1ULL << 40) - 1ULL;
static constexpr int TagWords = 4;
static constexpr uint32_t TagLastMask = 0x000fffffU;
static constexpr int DataWords = 18;

static uint64_t ts_pattern(uint32_t seed) {
    return (0x8500000000ULL ^ (uint64_t(seed) * 0x1020411ULL)) & TsDataMask;
}

static std::array<uint32_t, TagWords> tag_pattern(uint32_t seed) {
    return {0x71000000U ^ seed,
            0x5a5a0000U ^ (seed * 19U),
            0xc3c30000U ^ (seed * 263U),
            (0x00080000U ^ (seed * 3259U)) & TagLastMask};
}

static std::array<uint32_t, DataWords> data_pattern(uint32_t seed) {
    std::array<uint32_t, DataWords> out{};
    uint32_t x = seed ^ 0x9e3779b9U;
    for (int i = 0; i < DataWords; ++i) {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        out[i] = x + 0x00020003U * uint32_t(i + 1);
    }
    return out;
}

static void set_tag(VlWide<TagWords>& dst, const std::array<uint32_t, TagWords>& src) {
    for (int i = 0; i < TagWords; ++i) dst[i] = src[i];
    dst[TagWords - 1] &= TagLastMask;
}

static void set_data(VlWide<DataWords>& dst, const std::array<uint32_t, DataWords>& src) {
    for (int i = 0; i < DataWords; ++i) dst[i] = src[i];
}

static void clear_reqs(DUT* d) {
    d->ts_rd_en_i = 0;
    d->ts_wr_en_i = 0;
    d->t_rd_en_i = 0;
    d->t_wr_en_i = 0;
    d->d_rd_en_i = 0;
    d->d_wr_en_i = 0;
    d->d_amo_wr_en_i = 0;
    d->d_wr_qwen_i = 0;
}

static void clear_inputs(DUT* d) {
    d->esr_ram_delay_i = 2;
    d->ts_rd_addr_i = 0;
    d->ts_wr_addr_i = 0;
    d->ts_wr_data_i = 0;
    d->ts_rd_trans_id_i = 0;
    d->ts_rd_reqq_id_i = 0;
    d->t_addr_i = 0;
    d->t_rd_trans_id_i = 0;
    d->t_rd_reqq_id_i = 0;
    d->d_addr_i = 0;
    d->d_rd_trans_id_i = 0;
    d->d_rd_reqq_id_i = 0;
    d->dft_sram_clk_i = 0;
    d->dft_scanmode_i = 0;
    d->dft_scan_reset_n_i = 0;
    d->dft_sram_clk_override_i = 0;
    d->dft_ram_rei_i = 0;
    d->dft_ram_wei_i = 0;
    d->ram_cfg_test_en_i = 0;
    d->ram_cfg_rm_i = 0;
    d->ram_cfg_rme_i = 0;
    d->ram_cfg_ra_i = 0;
    d->ram_cfg_wa_i = 0;
    d->ram_cfg_wpulse_i = 0;
    d->ram_cfg_deep_sleep_i = 0;
    d->ram_cfg_shut_down_i = 0;
    std::array<uint32_t, TagWords> zt{};
    std::array<uint32_t, DataWords> zd{};
    set_tag(d->t_wr_data_i, zt);
    set_data(d->d_wr_data_i, zd);
    clear_reqs(d);
}

static void compare_wide_tag(CosimCtrl<DUT>& sim, const char* name,
                             const VlWide<TagWords>& orig,
                             const VlWide<TagWords>& newv) {
    for (int i = 0; i < TagWords; ++i) {
        const uint32_t mask = (i == TagWords - 1) ? TagLastMask : 0xffffffffU;
        sim.compare(std::string(name) + "[" + std::to_string(i) + "]",
                    orig[i] & mask, newv[i] & mask);
    }
}

static void compare_wide_data(CosimCtrl<DUT>& sim, const char* name,
                              const VlWide<DataWords>& orig,
                              const VlWide<DataWords>& newv) {
    for (int i = 0; i < DataWords; ++i) {
        sim.compare(std::string(name) + "[" + std::to_string(i) + "]",
                    orig[i], newv[i]);
    }
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("ts_rsp_valid", (uint32_t)d->orig_ts_rsp_valid_o,
                (uint32_t)d->new_ts_rsp_valid_o);
    sim.compare("ts_rsp_trans_id", (uint64_t)d->orig_ts_rsp_trans_id_o,
                (uint64_t)d->new_ts_rsp_trans_id_o);
    sim.compare("ts_rsp_reqq_id", (uint32_t)d->orig_ts_rsp_reqq_id_o,
                (uint32_t)d->new_ts_rsp_reqq_id_o);
    sim.compare("ts_rsp_data", (uint64_t)(d->orig_ts_rsp_data_o & TsDataMask),
                (uint64_t)(d->new_ts_rsp_data_o & TsDataMask));

    sim.compare("t_rsp_valid", (uint32_t)d->orig_t_rsp_valid_o,
                (uint32_t)d->new_t_rsp_valid_o);
    sim.compare("t_rsp_trans_id", (uint64_t)d->orig_t_rsp_trans_id_o,
                (uint64_t)d->new_t_rsp_trans_id_o);
    sim.compare("t_rsp_reqq_id", (uint32_t)d->orig_t_rsp_reqq_id_o,
                (uint32_t)d->new_t_rsp_reqq_id_o);
    compare_wide_tag(sim, "t_rsp_data", d->orig_t_rsp_data_o, d->new_t_rsp_data_o);

    sim.compare("d_rsp_valid", (uint32_t)d->orig_d_rsp_valid_o,
                (uint32_t)d->new_d_rsp_valid_o);
    sim.compare("d_rsp_trans_id", (uint64_t)d->orig_d_rsp_trans_id_o,
                (uint64_t)d->new_d_rsp_trans_id_o);
    sim.compare("d_rsp_reqq_id", (uint32_t)d->orig_d_rsp_reqq_id_o,
                (uint32_t)d->new_d_rsp_reqq_id_o);
    compare_wide_data(sim, "d_rsp_data", d->orig_d_rsp_data_o, d->new_d_rsp_data_o);
}

static void tick_cmp(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_all(sim);
}

static void idle(CosimCtrl<DUT>& sim, int cycles) {
    clear_reqs(sim.dut.get());
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

static void write_ts(CosimCtrl<DUT>& sim, uint16_t addr, uint64_t data, uint8_t delay) {
    auto* d = sim.dut.get();
    d->esr_ram_delay_i = delay;
    d->ts_wr_addr_i = addr;
    d->ts_wr_data_i = data & TsDataMask;
    d->ts_rd_trans_id_i = 0x1000 + addr;
    d->ts_rd_reqq_id_i = addr & 0x3f;
    d->ts_wr_en_i = 1;
    d->ts_rd_en_i = 0;
    tick_cmp(sim);
    d->ts_wr_en_i = 0;
    idle(sim, delay + 1);
}

static void read_ts(CosimCtrl<DUT>& sim, uint16_t addr, uint8_t delay, uint32_t id_seed) {
    auto* d = sim.dut.get();
    d->esr_ram_delay_i = delay;
    d->ts_rd_addr_i = addr;
    d->ts_rd_trans_id_i = 0x2100000000ULL | id_seed;
    d->ts_rd_reqq_id_i = id_seed & 0x3f;
    d->ts_rd_en_i = 1;
    d->ts_wr_en_i = 0;
    tick_cmp(sim);
    d->ts_rd_en_i = 0;
    idle(sim, delay + 2);
}

static void readwrite_ts(CosimCtrl<DUT>& sim, uint16_t rd_addr, uint16_t wr_addr,
                         uint64_t data, uint8_t delay, uint32_t id_seed) {
    auto* d = sim.dut.get();
    d->esr_ram_delay_i = delay;
    d->ts_rd_addr_i = rd_addr;
    d->ts_wr_addr_i = wr_addr;
    d->ts_wr_data_i = data & TsDataMask;
    d->ts_rd_trans_id_i = 0x2200000000ULL | id_seed;
    d->ts_rd_reqq_id_i = id_seed & 0x3f;
    d->ts_rd_en_i = 1;
    d->ts_wr_en_i = 1;
    tick_cmp(sim);
    d->ts_rd_en_i = 0;
    d->ts_wr_en_i = 0;
    idle(sim, delay + 2);
}

static void write_tag(CosimCtrl<DUT>& sim, uint16_t addr,
                      const std::array<uint32_t, TagWords>& data, uint8_t delay) {
    auto* d = sim.dut.get();
    d->esr_ram_delay_i = delay;
    d->t_addr_i = addr;
    set_tag(d->t_wr_data_i, data);
    d->t_rd_trans_id_i = 0x3000 + addr;
    d->t_rd_reqq_id_i = addr & 0x3f;
    d->t_wr_en_i = 1;
    d->t_rd_en_i = 0;
    tick_cmp(sim);
    d->t_wr_en_i = 0;
    idle(sim, delay + 1);
}

static void read_tag(CosimCtrl<DUT>& sim, uint16_t addr, uint8_t delay, uint32_t id_seed) {
    auto* d = sim.dut.get();
    d->esr_ram_delay_i = delay;
    d->t_addr_i = addr;
    d->t_rd_trans_id_i = 0x2300000000ULL | id_seed;
    d->t_rd_reqq_id_i = id_seed & 0x3f;
    d->t_rd_en_i = 1;
    d->t_wr_en_i = 0;
    tick_cmp(sim);
    d->t_rd_en_i = 0;
    idle(sim, delay + 2);
}

static void write_data(CosimCtrl<DUT>& sim, uint16_t addr,
                       const std::array<uint32_t, DataWords>& data,
                       uint8_t qwen, uint8_t delay) {
    auto* d = sim.dut.get();
    d->esr_ram_delay_i = delay;
    d->d_addr_i = addr;
    set_data(d->d_wr_data_i, data);
    d->d_wr_qwen_i = qwen;
    d->d_rd_trans_id_i = 0x4000 + addr;
    d->d_rd_reqq_id_i = addr & 0x3f;
    d->d_wr_en_i = 1;
    d->d_amo_wr_en_i = 0;
    d->d_rd_en_i = 0;
    tick_cmp(sim);
    d->d_wr_en_i = 0;
    d->d_wr_qwen_i = 0;
    idle(sim, delay + 1);
}

static void write_data_amo(CosimCtrl<DUT>& sim, uint16_t addr,
                           const std::array<uint32_t, DataWords>& data,
                           uint8_t qwen, uint8_t delay) {
    auto* d = sim.dut.get();
    d->esr_ram_delay_i = delay;
    d->d_addr_i = addr;
    set_data(d->d_wr_data_i, data);
    d->d_wr_qwen_i = qwen;
    d->d_rd_trans_id_i = 0x5000 + addr;
    d->d_rd_reqq_id_i = addr & 0x3f;
    d->d_wr_en_i = 0;
    d->d_amo_wr_en_i = 1;
    d->d_rd_en_i = 0;
    tick_cmp(sim);
    d->d_amo_wr_en_i = 0;
    d->d_wr_qwen_i = 0;
    idle(sim, delay + 1);
}

static void read_data(CosimCtrl<DUT>& sim, uint16_t addr, uint8_t delay, uint32_t id_seed) {
    auto* d = sim.dut.get();
    d->esr_ram_delay_i = delay;
    d->d_addr_i = addr;
    d->d_rd_trans_id_i = 0x2400000000ULL | id_seed;
    d->d_rd_reqq_id_i = id_seed & 0x3f;
    d->d_rd_en_i = 1;
    d->d_wr_en_i = 0;
    d->d_amo_wr_en_i = 0;
    tick_cmp(sim);
    d->d_rd_en_i = 0;
    idle(sim, delay + 2);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset();
    compare_all(sim);

    // Exercise non-functional DFT/config fields while idle.
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

    // Directed write/read checks for each supported response delay.
    for (uint8_t delay : {uint8_t{2}, uint8_t{3}, uint8_t{4}}) {
        for (uint16_t addr = 0; addr < 6; ++addr) {
            uint16_t a = addr + delay * 16;
            write_ts(sim, a, ts_pattern(a), delay);
            write_tag(sim, a, tag_pattern(a), delay);
            write_data(sim, a, data_pattern(a), 0xf, delay);
        }
        for (uint16_t addr = 0; addr < 6; ++addr) {
            uint16_t a = addr + delay * 16;
            read_ts(sim, a, delay, 0x100 + a);
            read_tag(sim, a, delay, 0x200 + a);
            read_data(sim, a, delay, 0x300 + a);
        }
    }

    // Dual-port tag-state simultaneous read/write to different addresses.
    write_ts(sim, 96, ts_pattern(96), 2);
    readwrite_ts(sim, 96, 97, ts_pattern(97), 2, 0x444);
    read_ts(sim, 97, 2, 0x445);

    // Per-QW data write masks, including the original AMO-write input mapped
    // onto the translated write port.
    write_data(sim, 100, data_pattern(100), 0xf, 2);
    write_data(sim, 100, data_pattern(101), 0x1, 2);
    write_data_amo(sim, 100, data_pattern(102), 0x4, 2);
    read_data(sim, 100, 2, 0x501);

    // DFT write/read inhibits.  The response-valid pipeline still advances;
    // data comes from the held RAM output in both models.
    d->dft_ram_wei_i = 1;
    write_ts(sim, 96, ts_pattern(196), 2);
    write_tag(sim, 96, tag_pattern(196), 2);
    write_data(sim, 100, data_pattern(196), 0xf, 2);
    d->dft_ram_wei_i = 0;
    read_ts(sim, 96, 2, 0x601);
    read_tag(sim, 96, 2, 0x602);
    read_data(sim, 100, 2, 0x603);

    d->dft_ram_rei_i = 1;
    read_ts(sim, 96, 2, 0x611);
    read_tag(sim, 96, 2, 0x612);
    read_data(sim, 100, 2, 0x613);
    d->dft_ram_rei_i = 0;
    read_ts(sim, 96, 2, 0x621);
    read_tag(sim, 96, 2, 0x622);
    read_data(sim, 100, 2, 0x623);

    // DFT SRAM-clock override path is toggled while idle.
    d->dft_sram_clk_override_i = 1;
    pulse_dft_sram_clock(sim);
    d->dft_sram_clk_override_i = 0;
    idle(sim, 3);

    // Pre-initialize a bounded random address pool for defined read data.
    for (uint16_t addr = 128; addr < 160; ++addr) {
        uint8_t delay = 2 + (addr % 3);
        write_ts(sim, addr, ts_pattern(addr), delay);
        write_tag(sim, addr, tag_pattern(addr), delay);
        write_data(sim, addr, data_pattern(addr), 0xf, delay);
    }

    // Pseudo-random but spaced legal traffic across all three RAMs.
    uint32_t lfsr = 0x31415927U;
    for (int i = 0; i < 300; ++i) {
        lfsr = (lfsr >> 1) ^ (0u - (lfsr & 1u) & 0xd0000001U);
        uint8_t delay = 2 + (lfsr % 3);
        uint16_t addr = 128 + ((lfsr >> 8) & 0x1fU);
        switch ((lfsr >> 5) & 0x7U) {
            case 0: write_ts(sim, addr, ts_pattern(lfsr), delay); break;
            case 1: write_tag(sim, addr, tag_pattern(lfsr), delay); break;
            case 2: write_data(sim, addr, data_pattern(lfsr), (lfsr >> 12) & 0xfU, delay); break;
            case 3: write_data_amo(sim, addr, data_pattern(lfsr), ((lfsr >> 12) & 0xfU) | 1U, delay); break;
            case 4: read_ts(sim, addr, delay, lfsr); break;
            case 5: read_tag(sim, addr, delay, lfsr); break;
            default: read_data(sim, addr, delay, lfsr); break;
        }
    }

    return sim.finish();
}
