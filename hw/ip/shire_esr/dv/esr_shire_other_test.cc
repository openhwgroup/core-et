// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vesr_shire_other_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

namespace {
constexpr uint16_t kUser = 0u << 14;
constexpr uint16_t kSupervisor = 1u << 14;
constexpr uint16_t kDebug = 2u << 14;
constexpr uint16_t kMachine = 3u << 14;
constexpr uint16_t kMinionFeature = kMachine | 0x000u;
constexpr uint16_t kShireConfig = kMachine | 0x001u;
constexpr uint16_t kThread1Disable = kMachine | 0x002u;
constexpr uint16_t kBuildConfig = kMachine | 0x003u;
constexpr uint16_t kRevisionId = kMachine | 0x004u;
constexpr uint16_t kIpiRedirectFilter = kMachine | 0x011u;
constexpr uint16_t kIpiTrigger = kMachine | 0x012u;
constexpr uint16_t kIpiTriggerClear = kMachine | 0x013u;
constexpr uint16_t kFccCredinc0 = kUser | 0x018u;
constexpr uint16_t kBarrier7 = kUser | 0x027u;
constexpr uint16_t kAndOrTree = kDebug | 0x3ff0u;
constexpr uint16_t kMtime = kMachine | 0x043u;
constexpr uint16_t kPowerCtrl = kMachine | 0x044u;
constexpr uint16_t kNeighNsleepin = kMachine | 0x045u;
constexpr uint16_t kNeighIso = kMachine | 0x046u;
constexpr uint16_t kThread0Disable = kMachine | 0x048u;
constexpr uint16_t kErrLog = kMachine | 0x049u;
constexpr uint16_t kPllAuto = kMachine | 0x04au;
constexpr uint16_t kPllData0 = kMachine | 0x04bu;
constexpr uint16_t kPllRead = kMachine | 0x051u;
constexpr uint16_t kCoopMode = kSupervisor | 0x052u;
constexpr uint16_t kClockmux = kMachine | 0x053u;
constexpr uint16_t kRamCfg2 = kMachine | 0x055u;
constexpr uint16_t kNocStatus = kMachine | 0x058u;
constexpr uint16_t kDllAuto = kMachine | 0x059u;
constexpr uint16_t kDllData0 = kMachine | 0x05au;
constexpr uint16_t kDllRead = kMachine | 0x05cu;
constexpr uint16_t kUcConfig = kSupervisor | 0x05du;
constexpr uint16_t kIcacheU = kUser | 0x05fu;
constexpr uint16_t kClkGate = kMachine | 0x062u;
constexpr uint16_t kDebugClkGate = kDebug | 0x3ff4u;
constexpr uint16_t kEco = kMachine | 0x068u;
constexpr uint16_t kClkDly = kMachine | 0x069u;
constexpr uint16_t kDllDlyCtl = kMachine | 0x06au;
constexpr uint16_t kDllDlySts = kMachine | 0x06bu;
constexpr uint16_t kBad = kMachine | 0x100u;

void set_defaults(SimCtrl<Vesr_shire_other_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_ni = 1;
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    d->dft_reset_byp_i = 0;
    d->dft_reset_i = 0;
    d->shire_id_reset_val_i = 2;
    d->apb_paddr_i = 0;
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwdata_i = 0;
    d->build_config_i = 0x123456789abcdef0ull;
    d->revision_id_i = 0x0fedcba987654321ull;
    d->and_or_tree_l1_i = 0x5a5;
    d->noc_status_i = 0x54321;
    d->barrier7_value_i = 0x7c;
    d->neigh_nsleepout_i = 0xa5a55a5a;
    d->glb_nsleepout_i = 0x5;
    d->neigh_err_detected_i = 0x9;
    d->neigh_err_logged_i = 0x6;
    d->bank_err_detected_i = 0x3;
    d->bank_err_logged_i = 0xc;
    d->pll_busy_i = 0;
    d->pll_rdata_i = 0;
    d->pll_rrdy_i = 0;
    d->pll_lock_i = 0;
    d->dll_busy_i = 0;
    d->dll_rdata_i = 0;
    d->dll_rrdy_i = 0;
    d->dll_lock_i = 0;
    d->dll_sts_done_i = 0;
    d->dll_sts_errn_i = 0;
    d->tbox_halt_ack_i = 0;
    d->tbox_resume_ack_i = 0;
    d->rbox_halt_ack_i = 0;
    d->rbox_resume_ack_i = 0;
    d->rbox_reset_ack_i = 0;
    d->prefetch_done_i = 0;
}

void reset_all(SimCtrl<Vesr_shire_other_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_ni = 0;
    d->rst_c_ni = 0;
    d->rst_w_ni = 0;
    d->rst_d_ni = 0;
    for (int i = 0; i < 5; ++i) sim.tick();
    d->rst_ni = 1;
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    for (int i = 0; i < 3; ++i) sim.tick();
}

void idle(SimCtrl<Vesr_shire_other_tb>& sim, int cycles = 1) {
    auto* d = sim.dut.get();
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwrite_i = 0;
    for (int i = 0; i < cycles; ++i) sim.tick();
}

uint64_t apb_read(SimCtrl<Vesr_shire_other_tb>& sim, uint16_t addr, bool expect_err = false) {
    auto* d = sim.dut.get();
    d->apb_paddr_i = addr;
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    d->apb_penable_i = 1;
    sim.tick();
    sim.check(d->apb_pready_o == 1, "shire-other APB pready tied high");
    sim.check(d->apb_pslverr_o == (expect_err ? 1 : 0), "shire-other APB read pslverr");
    uint64_t value = d->apb_prdata_o;
    idle(sim);
    return value;
}

void apb_write(SimCtrl<Vesr_shire_other_tb>& sim, uint16_t addr, uint64_t data, bool expect_err = false) {
    auto* d = sim.dut.get();
    d->apb_paddr_i = addr;
    d->apb_pwdata_i = data;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    d->apb_penable_i = 1;
    sim.tick();
    sim.check(d->apb_pslverr_o == (expect_err ? 1 : 0), "shire-other APB write pslverr");
    idle(sim);
}
}  // namespace

int main(int argc, char** argv) {
    SimCtrl<Vesr_shire_other_tb> sim(argc, argv);
    set_defaults(sim);
    reset_all(sim);
    auto* d = sim.dut.get();

    sim.check((d->minion_feature_o & 0x3f) == 0x1, "minion feature reset value");
    sim.check((d->shire_config_o & 0xff) == 2, "shire config reset captures shire ID");
    sim.check(d->thread0_enable_o == 0, "thread0 disabled after reset");
    sim.check(d->thread1_enable_o == 0, "thread1 disabled after reset");
    sim.check(d->power_glb_nsleepin_o == 0xf, "global nsleepin reset value");
    sim.check(apb_read(sim, kBuildConfig) == 0x123456789abcdef0ull, "build config read");
    sim.check(apb_read(sim, kRevisionId) == 0x0fedcba987654321ull, "revision ID read");
    sim.check((apb_read(sim, kAndOrTree) & 0x7ffu) == 0x5a5u, "and/or tree L1 read");
    sim.check(apb_read(sim, kBad, true) == 0, "invalid address errors and returns zero");

    apb_write(sim, kMinionFeature, 0x05);
    sim.check((apb_read(sim, kMinionFeature) & 0x3f) == 0x05, "minion feature first write sticks");
    apb_write(sim, kMinionFeature, 0x00);
    sim.check((apb_read(sim, kMinionFeature) & 0x3f) == 0x05, "minion feature ignores second write");

    apb_write(sim, kThread0Disable, 0x0);
    apb_write(sim, kThread1Disable, 0x0);
    sim.check(d->thread0_enable_o == 0xffffffffu, "thread0 enable is inverse disable register");
    sim.check(d->thread1_enable_o == 0xffffffffu, "thread1 enable is inverse disable register");

    apb_write(sim, kIpiRedirectFilter, 0x00ffu);
    d->apb_paddr_i = kUser | 0x010u;
    d->apb_pwdata_i = 0xf0f0u;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    sim.check(d->ipi_redirect_trigger_o == 0x00f0u, "IPI redirect mask write pulses filtered trigger");
    d->apb_penable_i = 1;
    sim.tick();
    idle(sim);
    apb_write(sim, kIpiTrigger, 0xf0f0u);
    sim.check(d->ipi_trigger_o == 0xf0f0u, "IPI trigger write ORs bits");
    apb_write(sim, kIpiTriggerClear, 0xf000u);
    sim.check(d->ipi_trigger_o == 0x00f0u, "IPI trigger clear removes selected bits");

    d->apb_paddr_i = kBarrier7;
    d->apb_pwdata_i = 1;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    sim.check((d->barrier_en_o & (1u << 7)) != 0, "barrier write pulses barrier enable");
    d->apb_penable_i = 1;
    sim.tick();
    idle(sim);
    d->apb_paddr_i = kFccCredinc0;
    d->apb_pwdata_i = 1;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    sim.check((d->fcc_en_o & 0x1u) != 0, "FCC write pulses FCC enable");
    d->apb_penable_i = 1;
    sim.tick();
    idle(sim);
    sim.check((apb_read(sim, kBarrier7) & 0xffu) == 0x7c, "barrier read returns UC value input");

    apb_write(sim, kMtime, 0xaaaa5555u);
    sim.check(d->mtime_target_o == 0xaaaa5555u, "mtime target write updates output");
    apb_write(sim, kPowerCtrl, 0xabc);
    sim.check(d->power_glb_nsleepin_o == 0xc, "power ctrl nsleepin slice");
    sim.check(d->power_glb_iso_o == 0xb, "power ctrl isolation slice");
    sim.check((apb_read(sim, kPowerCtrl) >> 8 & 0xfu) == 0x5, "power ctrl status slice follows nsleepout input");
    apb_write(sim, kNeighNsleepin, 0x11223344u);
    apb_write(sim, kNeighIso, 0x55667788u);
    sim.check(d->power_neigh_nsleepin_o == 0x11223344u, "neighborhood nsleepin write");
    sim.check(d->power_neigh_iso_o == 0x55667788u, "neighborhood isolation write");
    sim.check((apb_read(sim, kErrLog) & 0xffffu) == 0x6c93u, "shire error log packs logged/detected flags");

    const uint64_t ram_cfg2 = (1ull << 13) | (5ull << 10) | (3ull << 7) |
                              (2ull << 5) | (0xaull << 1) | 1ull;
    apb_write(sim, kRamCfg2, ram_cfg2);
    sim.check(((d->ram_cfg_o >> 14) & 1u) == 1, "RAM cfg test_en derived from cfg2 test bits");
    sim.check(((d->ram_cfg_o >> 10) & 0xfu) == 0xa, "RAM cfg rm derived from cfg2");
    sim.check(((d->ram_cfg_o >> 2) & 0x7u) == 5, "RAM cfg wpulse derived from cfg2");

    apb_write(sim, kPllAuto, 0x7ffu);
    apb_write(sim, kPllData0, 0xfacefeed01234567ull);
    sim.check((d->pll_ctrl_o & 0x7ffu) == 0x7ffu, "PLL auto config write");
    sim.check(d->pll_conf0_o == 0xfacefeed01234567ull, "PLL config data0 write");
    d->pll_busy_i = 1;
    d->pll_lock_i = 1;
    d->pll_rdata_i = 0xbeef;
    d->pll_rrdy_i = 1;
    sim.tick();
    d->pll_rrdy_i = 0;
    sim.check((apb_read(sim, kPllRead) & 0x3ffffu) == 0x3beefu, "PLL read data packs data/busy/lock");

    apb_write(sim, kDllAuto, 0x3ffu);
    apb_write(sim, kDllData0, 0x1122334455667788ull);
    sim.check((d->dll_ctrl_o & 0x3ffu) == 0x3ffu, "DLL auto config write");
    sim.check(d->dll_conf0_o == 0x1122334455667788ull, "DLL config data0 write");
    d->dll_busy_i = 1;
    d->dll_lock_i = 0;
    d->dll_rdata_i = 0x1234;
    d->dll_rrdy_i = 1;
    sim.tick();
    d->dll_rrdy_i = 0;
    sim.check((apb_read(sim, kDllRead) & 0x3ffffu) == 0x11234u, "DLL read data packs data/busy/lock");

    apb_write(sim, kCoopMode, 1);
    apb_write(sim, kClockmux, 0xb);
    apb_write(sim, kUcConfig, 1);
    apb_write(sim, kClkGate, 0x3ab);
    apb_write(sim, kDebugClkGate, 1);
    apb_write(sim, kEco, 0x5a);
    apb_write(sim, kClkDly, 0x123456789ull);
    sim.check(d->shire_coop_mode_o == 1, "shire coop mode write");
    sim.check((d->clockmux_o & 0xfu) == 0xb, "clockmux write");
    sim.check((d->uc_config_o & 1u) == 1, "UC config write");
    sim.check((d->clk_gate_ctrl_o & 0x7ffu) == 0x3ab, "clock gate control write");
    sim.check(d->debug_clk_gate_ctrl_o == 1, "debug clock gate control write");
    sim.check(d->shire_channel_eco_ctl_o == 0x5a, "shire channel ECO write");
    sim.check((d->clk_dly_ctl_o & 0xffffffffffull) == 0x123456789ull, "clock delay control write");

    d->apb_paddr_i = kDllDlyCtl;
    d->apb_pwdata_i = 0x7ffu;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    d->apb_penable_i = 1;
    sim.tick();
    sim.check((d->dll_dly_est_ctl_o & 0x7ffu) == 0x7ffu, "DLL delay control write visible for one cycle");
    idle(sim);
    sim.check((d->dll_dly_est_ctl_o & 0x7ffu) == 0x4ffu, "DLL delay init/start autoclear");
    d->dll_sts_done_i = 0xb;
    d->dll_sts_errn_i = 0x44332211u;
    sim.tick();
    sim.check((apb_read(sim, kDllDlySts) & 0xfffffffffull) == 0x44332211bull,
              "DLL delay status packs done and errn fields");

    const uint64_t prefetch = (uint64_t{0x5} << 48) | (uint64_t{0x12345} << 6) | 0x12u;
    d->apb_paddr_i = kIcacheU;
    d->apb_pwdata_i = prefetch;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    d->apb_penable_i = 1;
    sim.tick();
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwrite_i = 0;
    sim.tick();
    sim.check(d->prefetch_start_o == 0x5, "ICache U prefetch starts selected masks");
    sim.check((d->prefetch_conf_o & 0x3full) == 0x12u, "ICache prefetch num_lines forwarded");

    sim.check(apb_read(sim, kNocStatus) == 0x54321u, "NoC interrupt status read");

    return sim.finish();
}
