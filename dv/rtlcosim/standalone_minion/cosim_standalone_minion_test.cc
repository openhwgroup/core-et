// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_standalone_minion_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using DUT = Vcosim_standalone_minion_tb;

namespace {

constexpr int kDftScanWidth = 12;
constexpr int kDftModeWidth = 4;
constexpr int kDftCntlWidth = 10;
constexpr int kEcoInWidth = 10;
constexpr int kEcoOutWidth = 10;
constexpr int kThreads = 2;
constexpr int kShireIdWidth = 8;
constexpr int kMinPerSWidth = 5;
constexpr int kVaWidth = 48;
constexpr int kMprotWidth = 7;
constexpr int kVmPageWidth = 2;
constexpr int kFeatureWidth = 6;
constexpr int kMemOverrideWidth = 3;
constexpr int kDebugInWidth = 8;
constexpr int kDbgMuxWidth = 7;
constexpr int kRamCfgWidth = 15;
constexpr int kErrCtlWidth = 4;
constexpr int kBistReqWidth = 161;
constexpr int kBistRspWidth = 144;
constexpr int kRspWidth = 534;
constexpr int kReqWidth = 584;
constexpr int kTraceWidth = 143;
constexpr int kDbgMonitorWidth = 395;
constexpr int kErrInfoWidth = 56;
constexpr int kApbAddrWidth = 16;
constexpr int kApbDataWidth = 64;
constexpr int kSpdmctrlAddr = 0x3002;
constexpr int kSphastatusAddr = 0x3003;
constexpr int kIcacheApbAddr = 0xa000;
constexpr int kIcacheDataApbAddr = 0xc000;
constexpr int kMinionApbAddr = 0x0000;
constexpr int kSrcMinion0 = 0;
constexpr int kSrcIcache = 8;
constexpr int kSrcPtw0 = 9;
constexpr int kRspAckData = 1;
constexpr int kSizeLine = 6;

static uint64_t mask_bits(int bits) {
    if (bits >= 64) return ~0ull;
    return (1ull << bits) - 1ull;
}

template <std::size_t N>
static void zero_wide(VlWide<N>& v) {
    for (std::size_t i = 0; i < N; ++i) v[i] = 0;
}

template <std::size_t N>
static void fill_wide(VlWide<N>& v, int bits, uint32_t seed) {
    uint32_t x = seed ? seed : 0x1u;
    for (std::size_t i = 0; i < N; ++i) {
        x = x * 1664525u + 1013904223u;
        v[i] = x;
    }
    const int used = bits % 32;
    if (used != 0) v[N - 1] &= ((1u << used) - 1u);
}

template <std::size_t N>
static void ones_wide(VlWide<N>& v, int bits) {
    for (std::size_t i = 0; i < N; ++i) v[i] = 0xffffffffu;
    const int used = bits % 32;
    if (used != 0) v[N - 1] &= ((1u << used) - 1u);
}

template <std::size_t N>
static void compare_wide(CosimCtrl<DUT>& sim, const char* name,
                         const VlWide<N>& orig, const VlWide<N>& newv) {
    for (std::size_t i = 0; i < N; ++i) {
        char buf[96];
        std::snprintf(buf, sizeof(buf), "%s[%zu]", name, i);
        sim.compare(buf, orig[i], newv[i]);
    }
}

struct BitCov {
    std::string name;
    int bits;
    std::vector<uint8_t> seen0;
    std::vector<uint8_t> seen1;

    BitCov(std::string n, int b) : name(std::move(n)), bits(b), seen0(b, 0), seen1(b, 0) {}

    void sample_u64(uint64_t value) {
        for (int i = 0; i < bits; ++i) {
            if ((value >> i) & 1ull) seen1[i] = 1;
            else seen0[i] = 1;
        }
    }

    template <std::size_t N>
    void sample_wide(const VlWide<N>& value) {
        for (int i = 0; i < bits; ++i) {
            const uint32_t word = value[static_cast<std::size_t>(i / 32)];
            if ((word >> (i % 32)) & 1u) seen1[i] = 1;
            else seen0[i] = 1;
        }
    }

    bool complete() const {
        for (int i = 0; i < bits; ++i) {
            if (!seen0[i] || !seen1[i]) return false;
        }
        return true;
    }

    void report(CosimCtrl<DUT>& sim) const {
        for (int i = 0; i < bits; ++i) {
            if (!seen0[i] || !seen1[i]) {
                char buf[160];
                std::snprintf(buf, sizeof(buf), "input coverage %s[%d] saw0=%d saw1=%d",
                              name.c_str(), i, seen0[i], seen1[i]);
                sim.check(false, buf);
                return;
            }
        }
    }
};

struct Coverage {
    BitCov rst{"rst_ni", 1};
    BitCov rst_debug{"rst_debug_ni", 1};
    BitCov rst_non_debug{"rst_non_debug_ni", 1};
    BitCov dft_scanin{"stim_dft_scanin", kDftScanWidth};
    BitCov dft_scan_mode{"stim_dft_scan_mode", 1};
    BitCov dft_reset_byp{"stim_dft_reset_byp", 1};
    BitCov dft_scan_enable{"stim_dft_scan_enable", 1};
    BitCov dft_test_mode{"stim_dft_test_mode", kDftModeWidth};
    BitCov dft_reset{"stim_dft_reset", 1};
    BitCov dft_clock_gate_en{"stim_dft_clock_gate_en", 1};
    BitCov dft_cntl{"stim_dft_cntl", kDftCntlWidth};
    BitCov dft_occ_scanin{"stim_dft_occ_scanin", 1};
    BitCov dft_occ_reset{"stim_dft_occ_reset", 1};
    BitCov dft_occ_testmode{"stim_dft_occ_testmode", 1};
    BitCov dft_scan_ate_clk{"stim_dft_scan_ate_clk", 1};
    BitCov dft_occ_bypass{"stim_dft_occ_bypass", 1};
    BitCov dft_sram_clk{"stim_dft_sram_clk", 1};
    BitCov dft_clk_override{"stim_dft_clk_override", 1};
    BitCov eco_i{"stim_eco_i", kEcoInWidth};
    BitCov ioshire{"stim_ioshire", 1};
    BitCov int_mieco{"stim_int_mieco", 1};
    BitCov int_mtip{"stim_int_mtip", 1};
    BitCov int_meip{"stim_int_meip", 1};
    BitCov int_seip{"stim_int_seip", 1};
    BitCov int_msip{"stim_int_msip", kThreads};
    BitCov shire_id{"stim_shire_id", kShireIdWidth};
    BitCov shire_min_id{"stim_shire_min_id", kMinPerSWidth};
    BitCov thread_enable{"stim_thread_enable", kThreads};
    BitCov boot_addr{"stim_minion_boot_addr", kVaWidth};
    BitCov mprot{"stim_mprot", kMprotWidth};
    BitCov vmspage{"stim_vmspagesize", kVmPageWidth};
    BitCov features{"stim_minion_features", kFeatureWidth};
    BitCov coop{"stim_shire_coop_mode", 1};
    BitCov mem_override{"stim_minion_mem_override", kMemOverrideWidth};
    BitCov debug_in{"stim_debug_in", kDebugInWidth};
    BitCov resumeack{"stim_spio_rc_resumeack", 1};
    BitCov dbg_enable{"stim_minion_dbg_sig_enable", 1};
    BitCov dbg_mux{"stim_minion_dbg_signals_mux", kDbgMuxWidth};
    BitCov bypass_icache{"stim_esr_bypass_icache", 1};
    BitCov bypass_dcache{"stim_esr_bypass_dcache", 1};
    BitCov ram_cfg{"stim_ram_cfg", kRamCfgWidth};
    BitCov err_ctl{"stim_esr_icache_err_log_ctl", kErrCtlWidth};
    BitCov bist_req{"stim_icache_bist_req_info", kBistReqWidth};
    BitCov etl_req_ready{"stim_etl_req_ready", 1};
    BitCov etl_rsp_valid{"stim_etl_rsp_valid", 1};
    BitCov etl_rsp_info{"stim_etl_rsp_info", kRspWidth};
    BitCov te_enable{"stim_te_enable", 1};
    BitCov apb_addr{"stim_apb_paddr", kApbAddrWidth};
    BitCov apb_penable{"stim_apb_penable", 1};
    BitCov apb_psel{"stim_apb_psel", 1};
    BitCov apb_pwdata{"stim_apb_pwdata", kApbDataWidth};
    BitCov apb_pwrite{"stim_apb_pwrite", 1};

    void sample(const DUT* d) {
        rst.sample_u64(d->rst_ni);
        rst_debug.sample_u64(d->rst_debug_ni);
        rst_non_debug.sample_u64(d->rst_non_debug_ni);
        dft_scanin.sample_u64(d->stim_dft_scanin);
        dft_scan_mode.sample_u64(d->stim_dft_scan_mode);
        dft_reset_byp.sample_u64(d->stim_dft_reset_byp);
        dft_scan_enable.sample_u64(d->stim_dft_scan_enable);
        dft_test_mode.sample_u64(d->stim_dft_test_mode);
        dft_reset.sample_u64(d->stim_dft_reset);
        dft_clock_gate_en.sample_u64(d->stim_dft_clock_gate_en);
        dft_cntl.sample_u64(d->stim_dft_cntl);
        dft_occ_scanin.sample_u64(d->stim_dft_occ_scanin);
        dft_occ_reset.sample_u64(d->stim_dft_occ_reset);
        dft_occ_testmode.sample_u64(d->stim_dft_occ_testmode);
        dft_scan_ate_clk.sample_u64(d->stim_dft_scan_ate_clk);
        dft_occ_bypass.sample_u64(d->stim_dft_occ_bypass);
        dft_sram_clk.sample_u64(d->stim_dft_sram_clk);
        dft_clk_override.sample_u64(d->stim_dft_clk_override);
        eco_i.sample_u64(d->stim_eco_i);
        ioshire.sample_u64(d->stim_ioshire);
        int_mieco.sample_u64(d->stim_int_mieco);
        int_mtip.sample_u64(d->stim_int_mtip);
        int_meip.sample_u64(d->stim_int_meip);
        int_seip.sample_u64(d->stim_int_seip);
        int_msip.sample_u64(d->stim_int_msip);
        shire_id.sample_u64(d->stim_shire_id);
        shire_min_id.sample_u64(d->stim_shire_min_id);
        thread_enable.sample_u64(d->stim_thread_enable);
        boot_addr.sample_u64(d->stim_minion_boot_addr);
        mprot.sample_u64(d->stim_mprot);
        vmspage.sample_u64(d->stim_vmspagesize);
        features.sample_u64(d->stim_minion_features);
        coop.sample_u64(d->stim_shire_coop_mode);
        mem_override.sample_u64(d->stim_minion_mem_override);
        debug_in.sample_u64(d->stim_debug_in);
        resumeack.sample_u64(d->stim_spio_rc_resumeack);
        dbg_enable.sample_u64(d->stim_minion_dbg_sig_enable);
        dbg_mux.sample_u64(d->stim_minion_dbg_signals_mux);
        bypass_icache.sample_u64(d->stim_esr_bypass_icache);
        bypass_dcache.sample_u64(d->stim_esr_bypass_dcache);
        ram_cfg.sample_u64(d->stim_ram_cfg);
        err_ctl.sample_u64(d->stim_esr_icache_err_log_ctl);
        bist_req.sample_wide(d->stim_icache_bist_req_info);
        etl_req_ready.sample_u64(d->stim_etl_req_ready);
        etl_rsp_valid.sample_u64(d->stim_etl_rsp_valid);
        etl_rsp_info.sample_wide(d->stim_etl_rsp_info);
        te_enable.sample_u64(d->stim_te_enable);
        apb_addr.sample_u64(d->stim_apb_paddr);
        apb_penable.sample_u64(d->stim_apb_penable);
        apb_psel.sample_u64(d->stim_apb_psel);
        apb_pwdata.sample_u64(d->stim_apb_pwdata);
        apb_pwrite.sample_u64(d->stim_apb_pwrite);
    }

    void check(CosimCtrl<DUT>& sim) const {
        const BitCov* all[] = {&rst, &rst_debug, &rst_non_debug, &dft_scanin, &dft_scan_mode,
            &dft_reset_byp, &dft_scan_enable, &dft_test_mode, &dft_reset,
            &dft_clock_gate_en, &dft_cntl, &dft_occ_scanin, &dft_occ_reset,
            &dft_occ_testmode, &dft_scan_ate_clk, &dft_occ_bypass, &dft_sram_clk,
            &dft_clk_override, &eco_i, &ioshire, &int_mieco, &int_mtip, &int_meip,
            &int_seip, &int_msip, &shire_id, &shire_min_id, &thread_enable, &boot_addr,
            &mprot, &vmspage, &features, &coop, &mem_override, &debug_in, &resumeack,
            &dbg_enable, &dbg_mux, &bypass_icache, &bypass_dcache, &ram_cfg, &err_ctl,
            &bist_req, &etl_req_ready, &etl_rsp_valid, &etl_rsp_info, &te_enable,
            &apb_addr, &apb_penable, &apb_psel, &apb_pwdata, &apb_pwrite};
        for (const BitCov* cov : all) cov->report(sim);
    }
};

static void build_rsp(VlWide<17>& rsp, uint8_t id, uint8_t dest, bool wdata,
                      uint8_t opcode, uint32_t seed, uint8_t size, uint8_t qwen) {
    zero_wide(rsp);
    rsp[0] = (qwen & 0xfu) | ((size & 0x7u) << 4);

    VlWide<16> data;
    fill_wide(data, 512, seed);
    uint32_t carry = 0;
    for (int w = 0; w < 16; ++w) {
        uint64_t shifted = (static_cast<uint64_t>(data[w]) << 7) | carry;
        rsp[w] |= static_cast<uint32_t>(shifted & 0xffffffffu);
        carry = static_cast<uint32_t>(shifted >> 32);
    }
    rsp[16] |= carry;
    rsp[16] |= static_cast<uint32_t>(opcode & 0x3u) << 7;
    rsp[16] |= static_cast<uint32_t>(wdata ? 1u : 0u) << 9;
    rsp[16] |= static_cast<uint32_t>(dest & 0xfu) << 10;
    rsp[16] |= static_cast<uint32_t>(id) << 14;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("dft_scanout", d->orig_dft_scanout, d->new_dft_scanout);
    sim.compare("dft_occ_scanout", d->orig_dft_occ_scanout, d->new_dft_occ_scanout);
    sim.compare("eco_o", d->orig_eco_o, d->new_eco_o);
    compare_wide(sim, "minion_dbg_signals", d->orig_minion_dbg_signals, d->new_minion_dbg_signals);
    sim.compare("esr_icache_err_log_sbe", d->orig_esr_icache_err_log_sbe, d->new_esr_icache_err_log_sbe);
    sim.compare("esr_icache_err_log_dbe", d->orig_esr_icache_err_log_dbe, d->new_esr_icache_err_log_dbe);
    sim.compare("esr_icache_err_log_info", d->orig_esr_icache_err_log_info, d->new_esr_icache_err_log_info);
    compare_wide(sim, "icache_bist_rsp_info", d->orig_icache_bist_rsp_info, d->new_icache_bist_rsp_info);
    sim.compare("etl_req_valid", d->orig_etl_req_valid, d->new_etl_req_valid);
    compare_wide(sim, "etl_req_info", d->orig_etl_req_info, d->new_etl_req_info);
    sim.compare("etl_rsp_ready", d->orig_etl_rsp_ready, d->new_etl_rsp_ready);
    compare_wide(sim, "traceEncoder", d->orig_trace_encoder, d->new_trace_encoder);
    sim.compare("esr_spdmctrl", d->orig_esr_spdmctrl, d->new_esr_spdmctrl);
    sim.compare("esr_sphastatus", d->orig_esr_sphastatus, d->new_esr_sphastatus);
    sim.compare("apb_prdata", d->orig_apb_prdata, d->new_apb_prdata);
    sim.compare("apb_pready", d->orig_apb_pready, d->new_apb_pready);
    sim.compare("apb_pslverr", d->orig_apb_pslverr, d->new_apb_pslverr);
}

static void tick_and_compare(CosimCtrl<DUT>& sim, Coverage& cov) {
    cov.sample(sim.dut.get());
    sim.tick();
    compare_outputs(sim);
}

static void drive_idle(DUT* d) {
    d->rst_ni = 1;
    d->rst_debug_ni = 1;
    d->rst_non_debug_ni = 1;
    d->stim_dft_scanin = 0;
    d->stim_dft_scan_mode = 0;
    d->stim_dft_reset_byp = 0;
    d->stim_dft_scan_enable = 0;
    d->stim_dft_test_mode = 0;
    d->stim_dft_reset = 0;
    d->stim_dft_clock_gate_en = 0;
    d->stim_dft_cntl = 0;
    d->stim_dft_occ_scanin = 0;
    d->stim_dft_occ_reset = 0;
    d->stim_dft_occ_testmode = 0;
    d->stim_dft_scan_ate_clk = 0;
    d->stim_dft_occ_bypass = 0;
    d->stim_dft_sram_clk = 0;
    d->stim_dft_clk_override = 0;
    d->stim_eco_i = 0;
    d->stim_ioshire = 0;
    d->stim_int_mieco = 0;
    d->stim_int_mtip = 0;
    d->stim_int_meip = 0;
    d->stim_int_seip = 0;
    d->stim_int_msip = 0;
    d->stim_shire_id = 2;
    d->stim_shire_min_id = 3;
    d->stim_thread_enable = 1;
    d->stim_minion_boot_addr = 0x1000;
    d->stim_mprot = 0;
    d->stim_vmspagesize = 0;
    d->stim_minion_features = 0x0f;
    d->stim_shire_coop_mode = 0;
    d->stim_minion_mem_override = 0;
    d->stim_debug_in = 0;
    d->stim_spio_rc_resumeack = 0;
    d->stim_minion_dbg_sig_enable = 0;
    d->stim_minion_dbg_signals_mux = 0;
    d->stim_esr_bypass_icache = 0;
    d->stim_esr_bypass_dcache = 0;
    d->stim_ram_cfg = 0;
    d->stim_esr_icache_err_log_ctl = 0;
    zero_wide(d->stim_icache_bist_req_info);
    d->stim_etl_req_ready = 1;
    d->stim_etl_rsp_valid = 0;
    zero_wide(d->stim_etl_rsp_info);
    d->stim_te_enable = 0;
    d->stim_apb_paddr = 0;
    d->stim_apb_penable = 0;
    d->stim_apb_psel = 0;
    d->stim_apb_pwdata = 0;
    d->stim_apb_pwrite = 0;
}

static void drive_all_ones(DUT* d) {
    d->stim_dft_scanin = mask_bits(kDftScanWidth);
    d->stim_dft_scan_mode = 1;
    d->stim_dft_reset_byp = 1;
    d->stim_dft_scan_enable = 1;
    d->stim_dft_test_mode = mask_bits(kDftModeWidth);
    d->stim_dft_reset = 1;
    d->stim_dft_clock_gate_en = 1;
    d->stim_dft_cntl = mask_bits(kDftCntlWidth);
    d->stim_dft_occ_scanin = 1;
    d->stim_dft_occ_reset = 1;
    d->stim_dft_occ_testmode = 1;
    d->stim_dft_scan_ate_clk = 1;
    d->stim_dft_occ_bypass = 1;
    d->stim_dft_sram_clk = 1;
    d->stim_dft_clk_override = 1;
    d->stim_eco_i = mask_bits(kEcoInWidth);
    d->stim_ioshire = 1;
    d->stim_int_mieco = 1;
    d->stim_int_mtip = 1;
    d->stim_int_meip = 1;
    d->stim_int_seip = 1;
    d->stim_int_msip = mask_bits(kThreads);
    d->stim_shire_id = mask_bits(kShireIdWidth);
    d->stim_shire_min_id = mask_bits(kMinPerSWidth);
    d->stim_thread_enable = mask_bits(kThreads);
    d->stim_minion_boot_addr = mask_bits(kVaWidth);
    d->stim_mprot = mask_bits(kMprotWidth);
    d->stim_vmspagesize = mask_bits(kVmPageWidth);
    d->stim_minion_features = mask_bits(kFeatureWidth);
    d->stim_shire_coop_mode = 1;
    d->stim_minion_mem_override = mask_bits(kMemOverrideWidth);
    d->stim_debug_in = mask_bits(kDebugInWidth);
    d->stim_spio_rc_resumeack = 1;
    d->stim_minion_dbg_sig_enable = 1;
    d->stim_minion_dbg_signals_mux = mask_bits(kDbgMuxWidth);
    d->stim_esr_bypass_icache = 1;
    d->stim_esr_bypass_dcache = 1;
    d->stim_ram_cfg = mask_bits(kRamCfgWidth);
    d->stim_esr_icache_err_log_ctl = mask_bits(kErrCtlWidth);
    ones_wide(d->stim_icache_bist_req_info, kBistReqWidth);
    d->stim_etl_req_ready = 1;
    d->stim_etl_rsp_valid = 1;
    ones_wide(d->stim_etl_rsp_info, kRspWidth);
    d->stim_te_enable = 1;
    d->stim_apb_paddr = mask_bits(kApbAddrWidth);
    d->stim_apb_penable = 1;
    d->stim_apb_psel = 1;
    d->stim_apb_pwdata = mask_bits(kApbDataWidth);
    d->stim_apb_pwrite = 1;
}

static void apb_cycle(CosimCtrl<DUT>& sim, Coverage& cov, uint16_t addr, uint64_t data, bool write) {
    auto* d = sim.dut.get();
    d->stim_apb_paddr = addr;
    d->stim_apb_pwdata = data;
    d->stim_apb_pwrite = write ? 1 : 0;
    d->stim_apb_psel = 1;
    d->stim_apb_penable = 0;
    tick_and_compare(sim, cov);
    d->stim_apb_penable = 1;
    tick_and_compare(sim, cov);
    d->stim_apb_psel = 0;
    d->stim_apb_penable = 0;
    d->stim_apb_pwrite = 0;
    tick_and_compare(sim, cov);
}

static void reset_all(CosimCtrl<DUT>& sim, Coverage& cov) {
    auto* d = sim.dut.get();
    d->rst_ni = 0;
    d->rst_debug_ni = 0;
    d->rst_non_debug_ni = 0;
    d->stim_thread_enable = 0;
    d->stim_minion_features = 0;
    for (int i = 0; i < 2; ++i) tick_and_compare(sim, cov);
    drive_idle(d);
    d->rst_ni = 0;
    d->rst_debug_ni = 0;
    d->rst_non_debug_ni = 0;
    for (int i = 0; i < 3; ++i) tick_and_compare(sim, cov);
    drive_all_ones(d);
    d->rst_ni = 0;
    d->rst_debug_ni = 0;
    d->rst_non_debug_ni = 0;
    tick_and_compare(sim, cov);
    drive_idle(d);
    d->rst_ni = 0;
    d->rst_debug_ni = 0;
    d->rst_non_debug_ni = 0;
    for (int i = 0; i < 2; ++i) tick_and_compare(sim, cov);
    d->rst_ni = 1;
    d->rst_debug_ni = 1;
    d->rst_non_debug_ni = 1;
    for (int i = 0; i < 10; ++i) tick_and_compare(sim, cov);
}

static uint32_t lcg(uint32_t& s) {
    s = s * 1664525u + 1013904223u;
    return s;
}

static void directed_phases(CosimCtrl<DUT>& sim, Coverage& cov) {
    auto* d = sim.dut.get();
    apb_cycle(sim, cov, kSpdmctrlAddr, 1ull << 31, true);
    apb_cycle(sim, cov, kSphastatusAddr, 0, false);
    apb_cycle(sim, cov, kMinionApbAddr, 0x1122334455667788ull, true);
    apb_cycle(sim, cov, kIcacheApbAddr, 0, false);
    apb_cycle(sim, cov, kIcacheDataApbAddr, 0x55aa00ffcc33dd77ull, true);

    for (int mux = 0; mux < 8; ++mux) {
        d->stim_minion_dbg_signals_mux = mux;
        d->stim_minion_dbg_sig_enable = 1;
        d->stim_te_enable = mux & 1;
        tick_and_compare(sim, cov);
    }
    d->stim_minion_dbg_sig_enable = 0;

    for (int dest : {kSrcIcache, kSrcMinion0, kSrcPtw0}) {
        build_rsp(d->stim_etl_rsp_info, static_cast<uint8_t>(0x40 + dest), static_cast<uint8_t>(dest),
                  true, kRspAckData, 0x12340000u + static_cast<uint32_t>(dest), kSizeLine, 0xf);
        d->stim_etl_rsp_valid = 1;
        d->stim_etl_req_ready = 1;
        for (int i = 0; i < 4; ++i) tick_and_compare(sim, cov);
        d->stim_etl_rsp_valid = 0;
        zero_wide(d->stim_etl_rsp_info);
        for (int i = 0; i < 4; ++i) tick_and_compare(sim, cov);
    }

    d->stim_dft_clk_override = 1;
    d->stim_dft_cntl = 0x3;
    for (int i = 0; i < 8; ++i) {
        d->stim_dft_sram_clk = i & 1;
        tick_and_compare(sim, cov);
    }
    d->stim_dft_clk_override = 0;
    d->stim_dft_cntl = 0;

    d->rst_debug_ni = 0;
    for (int i = 0; i < 3; ++i) tick_and_compare(sim, cov);
    d->rst_debug_ni = 1;
    for (int i = 0; i < 3; ++i) tick_and_compare(sim, cov);
    d->rst_non_debug_ni = 0;
    for (int i = 0; i < 3; ++i) tick_and_compare(sim, cov);
    d->rst_non_debug_ni = 1;
    for (int i = 0; i < 6; ++i) tick_and_compare(sim, cov);
}

static void random_phases(CosimCtrl<DUT>& sim, Coverage& cov, int cycles) {
    auto* d = sim.dut.get();
    uint32_t s = 0x5eed1234u;
    for (int i = 0; i < cycles; ++i) {
        const uint32_t r = lcg(s);
        d->rst_ni = (i % 4096) == 4095 ? 0 : 1;
        d->rst_debug_ni = (i % 3001) == 3000 ? 0 : 1;
        d->rst_non_debug_ni = (i % 3571) == 3570 ? 0 : 1;
        d->stim_ioshire = (r >> 0) & 1;
        d->stim_int_mieco = (r >> 1) & 1;
        d->stim_int_mtip = (r >> 2) & 1;
        d->stim_int_meip = (r >> 3) & 1;
        d->stim_int_seip = (r >> 4) & 1;
        d->stim_int_msip = (r >> 5) & 0x3;
        d->stim_shire_id = (r >> 7) & 0xff;
        d->stim_shire_min_id = (r >> 15) & 0x1f;
        d->stim_thread_enable = ((r >> 20) & 0x3) | 0x1;
        d->stim_minion_boot_addr = (0x1000ull + ((uint64_t)(r & 0xffff) << 2)) & mask_bits(kVaWidth);
        d->stim_mprot = (r >> 3) & 0x7f;
        d->stim_vmspagesize = (r >> 11) & 0x3;
        d->stim_minion_features = ((r >> 13) & 0x3f) | 0x0c;
        d->stim_shire_coop_mode = (r >> 19) & 1;
        d->stim_minion_mem_override = (r >> 21) & 0x7;
        d->stim_debug_in = (r >> 8) & 0xff;
        d->stim_spio_rc_resumeack = (r >> 24) & 1;
        d->stim_minion_dbg_sig_enable = (i % 16) == 0;
        d->stim_minion_dbg_signals_mux = (r >> 17) & 0x7f;
        d->stim_esr_bypass_icache = (i % 64) == 0;
        d->stim_esr_bypass_dcache = (i % 97) == 0;
        d->stim_ram_cfg = (r >> 4) & mask_bits(kRamCfgWidth);
        d->stim_esr_icache_err_log_ctl = (r >> 12) & 0xf;
        d->stim_etl_req_ready = (r >> 6) & 1;
        d->stim_te_enable = (r >> 14) & 1;
        d->stim_eco_i = (r >> 10) & 0x3ff;
        d->stim_dft_sram_clk = (r >> 26) & 1;
        d->stim_dft_clk_override = (i % 257) == 0;
        d->stim_dft_scan_mode = 0;
        d->stim_dft_reset = 0;
        d->stim_dft_cntl = (i % 257) == 0 ? 0x3 : 0;

        if ((i % 23) == 0) {
            const int dests[3] = {kSrcIcache, kSrcMinion0, kSrcPtw0};
            build_rsp(d->stim_etl_rsp_info, static_cast<uint8_t>(r >> 24),
                      static_cast<uint8_t>(dests[(i / 23) % 3]), true,
                      kRspAckData, r, kSizeLine, 0xf);
            d->stim_etl_rsp_valid = 1;
        } else {
            d->stim_etl_rsp_valid = 0;
            zero_wide(d->stim_etl_rsp_info);
        }

        if ((i % 211) == 0) {
            fill_wide(d->stim_icache_bist_req_info, kBistReqWidth, r);
        } else {
            zero_wide(d->stim_icache_bist_req_info);
        }

        // Keep APB random traffic protocol-clean. Directed phases above cover
        // setup/access cycles through every standalone APB target; the long
        // random phase keeps APB idle to avoid intentionally illegal master
        // phasing that would test APB error recovery rather than this wrapper.
        d->stim_apb_psel = 0;
        d->stim_apb_penable = 0;
        d->stim_apb_pwrite = 0;
        d->stim_apb_paddr = 0;
        d->stim_apb_pwdata = 0;

        tick_and_compare(sim, cov);
    }
    d->rst_ni = 1;
    d->rst_debug_ni = 1;
    d->rst_non_debug_ni = 1;
    drive_idle(d);
    for (int i = 0; i < 20; ++i) tick_and_compare(sim, cov);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000000;
    Coverage cov;

    drive_idle(sim.dut.get());
    reset_all(sim, cov);
    directed_phases(sim, cov);
    random_phases(sim, cov, 32000);
    cov.check(sim);
    sim.check(sim.comparisons >= 500000, "standalone_minion cosim reached at least 500k output comparisons");

    return sim.finish();
}
