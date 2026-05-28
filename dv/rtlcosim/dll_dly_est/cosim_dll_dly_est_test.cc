// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_dll_dly_est_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <random>

using DUT = Vcosim_dll_dly_est_tb;

static uint16_t pack_ctl(bool enable, bool init, bool start, uint8_t txn) {
    return (static_cast<uint16_t>(enable) << 10) |
           (static_cast<uint16_t>(init) << 9) |
           (static_cast<uint16_t>(start) << 8) |
           txn;
}

static bool sts_done(uint16_t sts) { return (sts & 0x1u) != 0; }

static void init_inputs(DUT* dut) {
    dut->reset_shire_i = 0;
    dut->reset_neigh_i = 0;
    dut->dll_dly_est_ctl_i = 0;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("dll_dly_est_sts", sim.dut->orig_sts_o, sim.dut->new_sts_o);
}

static void tick_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

static void pulse_init(CosimCtrl<DUT>& sim) {
    sim.dut->dll_dly_est_ctl_i = pack_ctl(true, true, false, 0);
    tick_compare(sim);
    sim.dut->dll_dly_est_ctl_i = pack_ctl(true, false, false, 0);
    for (int i = 0; i < 5; ++i) tick_compare(sim);
}

static void pulse_start(CosimCtrl<DUT>& sim, uint8_t txn) {
    sim.dut->dll_dly_est_ctl_i = pack_ctl(true, false, true, txn);
    tick_compare(sim);
    sim.dut->dll_dly_est_ctl_i = pack_ctl(true, false, false, txn);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    std::mt19937 rng(0xd117de5u);
    init_inputs(sim.dut.get());
    sim.reset(6);
    compare_outputs(sim);

    // Disabled controls and both explicit reset domains.
    sim.dut->dll_dly_est_ctl_i = pack_ctl(false, true, true, 0xff);
    for (int i = 0; i < 8; ++i) tick_compare(sim);
    sim.dut->reset_shire_i = 1;
    for (int i = 0; i < 3; ++i) tick_compare(sim);
    sim.dut->reset_shire_i = 0;
    sim.dut->reset_neigh_i = 1;
    for (int i = 0; i < 3; ++i) tick_compare(sim);
    sim.dut->reset_neigh_i = 0;
    for (int i = 0; i < 6; ++i) tick_compare(sim);

    // Short transaction with both semisync register crossings active.
    pulse_init(sim);
    pulse_start(sim, 0x00);
    for (int i = 0; i < 140; ++i) tick_compare(sim);

    // Longer terminal-counter path.
    pulse_init(sim);
    pulse_start(sim, 0x01);
    for (int i = 0; i < 2350; ++i) {
        tick_compare(sim);
        if (sts_done(sim.dut->orig_sts_o) && sts_done(sim.dut->new_sts_o)) break;
    }

    // ctl_txn[7:2] non-zero path: TX finishes but RX end is suppressed until
    // software reinitializes the block.
    pulse_init(sim);
    pulse_start(sim, 0x44);
    for (int i = 0; i < 160; ++i) tick_compare(sim);
    pulse_init(sim);

    // Hold the LV/neighborhood side in reset while the HV core runs. The return
    // rd_valid is unused in both designs; the core's own delayed valid still
    // drives completion/error behavior.
    sim.dut->reset_neigh_i = 1;
    for (int i = 0; i < 4; ++i) tick_compare(sim);
    pulse_start(sim, 0x00);
    for (int i = 0; i < 140; ++i) tick_compare(sim);
    sim.dut->reset_neigh_i = 0;
    pulse_init(sim);

    // Random control and reset stimulus. All practical inputs are driven both
    // low and high/non-zero; periodic init pulses keep long random transactions
    // from monopolizing the run.
    for (int cyc = 0; cyc < 3600; ++cyc) {
        bool enable = (rng() % 6) != 0;
        bool init = (rng() % 113) == 0;
        bool start = !init && ((rng() % 97) == 0);
        uint8_t txn = static_cast<uint8_t>(rng());
        if ((cyc % 512) == 511) {
            enable = true;
            init = true;
            start = false;
            txn = 0;
        }
        sim.dut->dll_dly_est_ctl_i = pack_ctl(enable, init, start, txn);
        sim.dut->reset_shire_i = ((cyc % 1301) == 1300) ? 1 : 0;
        sim.dut->reset_neigh_i = ((cyc % 887) == 886) ? 1 : 0;
        tick_compare(sim);
        if (sim.dut->reset_shire_i || sim.dut->reset_neigh_i) {
            sim.dut->reset_shire_i = 0;
            sim.dut->reset_neigh_i = 0;
            tick_compare(sim);
        }
    }

    sim.dut->dll_dly_est_ctl_i = pack_ctl(true, true, false, 0);
    for (int i = 0; i < 8; ++i) tick_compare(sim);

    return sim.finish();
}
