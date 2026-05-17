// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_esr_shire_other_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <random>

namespace {
constexpr uint16_t U = 0u << 14;
constexpr uint16_t S = 1u << 14;
constexpr uint16_t D = 2u << 14;
constexpr uint16_t M = 3u << 14;

constexpr std::array<uint16_t, 56> kAddrs = {
    uint16_t(M | 0x000), uint16_t(M | 0x001), uint16_t(M | 0x002),
    uint16_t(M | 0x003), uint16_t(M | 0x004), uint16_t(U | 0x010),
    uint16_t(M | 0x011), uint16_t(M | 0x012), uint16_t(M | 0x013),
    uint16_t(U | 0x018), uint16_t(U | 0x019), uint16_t(U | 0x01a),
    uint16_t(U | 0x01b), uint16_t(U | 0x020), uint16_t(U | 0x021),
    uint16_t(U | 0x022), uint16_t(U | 0x023), uint16_t(U | 0x024),
    uint16_t(U | 0x025), uint16_t(U | 0x026), uint16_t(U | 0x027),
    uint16_t(U | 0x03f), uint16_t(D | 0x3ff0), uint16_t(M | 0x043),
    uint16_t(M | 0x044), uint16_t(M | 0x045), uint16_t(M | 0x046),
    uint16_t(M | 0x047), uint16_t(M | 0x048), uint16_t(M | 0x049),
    uint16_t(M | 0x04a), uint16_t(M | 0x04b), uint16_t(M | 0x04c),
    uint16_t(M | 0x04d), uint16_t(M | 0x04e), uint16_t(M | 0x051),
    uint16_t(S | 0x052), uint16_t(M | 0x053), uint16_t(M | 0x054),
    uint16_t(M | 0x055), uint16_t(M | 0x056), uint16_t(M | 0x057),
    uint16_t(M | 0x058), uint16_t(M | 0x059), uint16_t(M | 0x05a),
    uint16_t(M | 0x05b), uint16_t(M | 0x05c), uint16_t(D | 0x3ff1),
    uint16_t(S | 0x05d), uint16_t(U | 0x05f), uint16_t(S | 0x060),
    uint16_t(M | 0x061), uint16_t(M | 0x062), uint16_t(D | 0x3ff4),
    uint16_t(M | 0x068), uint16_t(M | 0x069)};

void set_defaults(CosimCtrl<Vcosim_esr_shire_other_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    d->test_en_i = 0;
    d->dft_reset_byp_i = 0;
    d->dft_reset_i = 0;
    d->shire_id_reset_val_i = 2;
    d->apb_paddr_i = 0;
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwdata_i = 0;
    d->status_seed_i = 0;
    d->prefetch_done_i = 0xf;
    d->dll_sts_done_i = 0;
    d->dll_sts_errn_i = 0;
    d->tbox_halt_ack_i = 0;
    d->tbox_resume_ack_i = 0;
    d->rbox_halt_ack_i = 0;
    d->rbox_resume_ack_i = 0;
    d->rbox_reset_ack_i = 0;
}

void reset_all(CosimCtrl<Vcosim_esr_shire_other_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_c_ni = 0;
    d->rst_w_ni = 0;
    d->rst_d_ni = 0;
    for (int i = 0; i < 5; ++i) sim.tick();
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    for (int i = 0; i < 3; ++i) sim.tick();
}

void compare_all(CosimCtrl<Vcosim_esr_shire_other_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("apb_pready", d->orig_apb_pready_o, d->new_apb_pready_o);
    sim.compare("apb_prdata", d->orig_apb_prdata_o, d->new_apb_prdata_o);
    sim.compare("apb_pslverr", d->orig_apb_pslverr_o, d->new_apb_pslverr_o);
    sim.compare("minion_feature", d->orig_minion_feature_o, d->new_minion_feature_o);
    sim.compare("shire_config", d->orig_shire_config_o, d->new_shire_config_o);
    sim.compare("ipi_trigger", d->orig_ipi_trigger_o, d->new_ipi_trigger_o);
    sim.compare("thread0_enable", d->orig_thread0_enable_o, d->new_thread0_enable_o);
    sim.compare("thread1_enable", d->orig_thread1_enable_o, d->new_thread1_enable_o);
    sim.compare("ipi_redirect_trigger", d->orig_ipi_redirect_trigger_o, d->new_ipi_redirect_trigger_o);
    sim.compare("mtime_target", d->orig_mtime_target_o, d->new_mtime_target_o);
    sim.compare("power_glb_nsleepin", d->orig_power_glb_nsleepin_o, d->new_power_glb_nsleepin_o);
    sim.compare("power_glb_iso", d->orig_power_glb_iso_o, d->new_power_glb_iso_o);
    sim.compare("power_neigh_nsleepin", d->orig_power_neigh_nsleepin_o, d->new_power_neigh_nsleepin_o);
    sim.compare("power_neigh_iso", d->orig_power_neigh_iso_o, d->new_power_neigh_iso_o);
    sim.compare("shire_coop_mode", d->orig_shire_coop_mode_o, d->new_shire_coop_mode_o);
    sim.compare("uc_config", d->orig_uc_config_o, d->new_uc_config_o);
    sim.compare("clk_gate_ctrl", d->orig_clk_gate_ctrl_o, d->new_clk_gate_ctrl_o);
    sim.compare("debug_clk_gate_ctrl", d->orig_debug_clk_gate_ctrl_o, d->new_debug_clk_gate_ctrl_o);
    sim.compare("eco", d->orig_eco_o, d->new_eco_o);
    sim.compare("clk_dly_ctl", d->orig_clk_dly_ctl_o, d->new_clk_dly_ctl_o);
    sim.compare("pll_ctrl", d->orig_pll_ctrl_o, d->new_pll_ctrl_o);
    sim.compare("pll_conf0", d->orig_pll_conf0_o, d->new_pll_conf0_o);
    sim.compare("pll_conf1", d->orig_pll_conf1_o, d->new_pll_conf1_o);
    sim.compare("pll_conf2", d->orig_pll_conf2_o, d->new_pll_conf2_o);
    sim.compare("pll_conf3", d->orig_pll_conf3_o, d->new_pll_conf3_o);
    sim.compare("dll_ctrl", d->orig_dll_ctrl_o, d->new_dll_ctrl_o);
    sim.compare("dll_conf0", d->orig_dll_conf0_o, d->new_dll_conf0_o);
    sim.compare("dll_conf1", d->orig_dll_conf1_o, d->new_dll_conf1_o);
    sim.compare("clockmux", d->orig_clockmux_o, d->new_clockmux_o);
    sim.compare("dll_dly_est_ctl", d->orig_dll_dly_est_ctl_o, d->new_dll_dly_est_ctl_o);
    sim.compare("ram_cfg", d->orig_ram_cfg_o, d->new_ram_cfg_o);
    sim.compare("barrier_en", d->orig_barrier_en_o, d->new_barrier_en_o);
    sim.compare("fcc_en", d->orig_fcc_en_o, d->new_fcc_en_o);
    sim.compare("prefetch_conf", d->orig_prefetch_conf_o, d->new_prefetch_conf_o);
    sim.compare("prefetch_start", d->orig_prefetch_start_o, d->new_prefetch_start_o);
    sim.compare("debug_ss", d->orig_debug_ss_o, d->new_debug_ss_o);
}

void idle(CosimCtrl<Vcosim_esr_shire_other_tb>& sim, uint64_t& seed, int cycles = 1) {
    auto* d = sim.dut.get();
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwrite_i = 0;
    for (int i = 0; i < cycles; ++i) {
        seed = seed * 6364136223846793005ull + 1442695040888963407ull;
        d->status_seed_i = seed;
        sim.tick();
        compare_all(sim);
    }
}

void apb_cycle(CosimCtrl<Vcosim_esr_shire_other_tb>& sim, uint64_t& seed,
               uint16_t addr, bool write, uint64_t data) {
    auto* d = sim.dut.get();
    d->apb_paddr_i = addr;
    d->apb_pwdata_i = data;
    d->apb_pwrite_i = write ? 1 : 0;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    seed = seed * 6364136223846793005ull + 1442695040888963407ull;
    d->status_seed_i = seed;
    sim.tick();
    compare_all(sim);
    d->apb_penable_i = 1;
    seed = seed * 6364136223846793005ull + 1442695040888963407ull;
    d->status_seed_i = seed;
    sim.tick();
    compare_all(sim);
    idle(sim, seed, 1);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_esr_shire_other_tb> sim(argc, argv);
    set_defaults(sim);
    reset_all(sim);
    uint64_t seed = 0x12345678abcdef00ull;
    sim.dut->status_seed_i = seed;
    compare_all(sim);

    // Directed phase: enable multithreading and cooperative prefetch, then touch every retained address.
    apb_cycle(sim, seed, M | 0x000, true, 0x05);  // feature: disable_multithreading=0
    apb_cycle(sim, seed, S | 0x052, true, 0x1);   // cooperative mode enables prefetch side effects
    for (uint16_t addr : kAddrs) {
        apb_cycle(sim, seed, addr, false, 0);
        apb_cycle(sim, seed, addr, true, seed ^ (uint64_t{addr} << 32));
    }
    apb_cycle(sim, seed, M | 0x06a, true, 0x7ff);
    apb_cycle(sim, seed, M | 0x06b, false, 0);
    apb_cycle(sim, seed, M | 0x155, false, 0);  // invalid address path

    auto* d = sim.dut.get();
    std::mt19937_64 rng(0x5010beef);
    for (int i = 0; i < 3000; ++i) {
        const uint64_t r = rng();
        d->test_en_i = ((i & 0x1f) == 0) ? 1 : 0;
        d->dft_reset_byp_i = ((i & 0x3f) == 1) ? 1 : 0;
        d->dft_reset_i = ((i & 0x3f) == 2) ? 1 : 0;
        d->shire_id_reset_val_i = r & 0x3u;
        d->prefetch_done_i = (r >> 8) & 0xfu;
        d->dll_sts_done_i = (r >> 12) & 0xfu;
        d->dll_sts_errn_i = static_cast<uint32_t>(rng());
        d->tbox_halt_ack_i = (r >> 20) & 0xfu;
        d->tbox_resume_ack_i = (r >> 24) & 0xfu;
        d->rbox_halt_ack_i = (r >> 28) & 1u;
        d->rbox_resume_ack_i = (r >> 29) & 1u;
        d->rbox_reset_ack_i = (r >> 30) & 1u;

        const uint16_t addr = kAddrs[r % kAddrs.size()];
        const bool write = ((r >> 33) & 1u) != 0;
        apb_cycle(sim, seed, addr, write, rng());

        if ((i % 503) == 17) {
            d->rst_w_ni = 0;
            idle(sim, seed, 1);
            d->rst_w_ni = 1;
            idle(sim, seed, 2);
        }
        if ((i % 701) == 19) {
            d->rst_d_ni = 0;
            idle(sim, seed, 1);
            d->rst_d_ni = 1;
            idle(sim, seed, 2);
        }
        if ((i % 997) == 23) {
            d->rst_c_ni = 0;
            idle(sim, seed, 1);
            d->rst_c_ni = 1;
            idle(sim, seed, 2);
            apb_cycle(sim, seed, M | 0x000, true, 0x05);
            apb_cycle(sim, seed, S | 0x052, true, 0x1);
        }
    }

    return sim.finish();
}
