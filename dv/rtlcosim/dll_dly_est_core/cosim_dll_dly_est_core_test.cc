// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_dll_dly_est_core_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <random>

using DUT = Vcosim_dll_dly_est_core_tb;

static uint16_t pack_ctl(bool enable, bool init, bool start, uint8_t txn) {
    return (static_cast<uint16_t>(enable) << 10) |
           (static_cast<uint16_t>(init) << 9) |
           (static_cast<uint16_t>(start) << 8) |
           txn;
}

static bool sts_done(uint16_t sts) { return (sts & 0x1u) != 0; }

static void init_inputs(DUT* dut) {
    dut->reset_i = 0;
    dut->dll_dly_est_ctl_i = 0;
    dut->rx_dly_est_code_i = 0;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("dll_dly_est_sts", sim.dut->orig_sts_o, sim.dut->new_sts_o);
    sim.compare("tx_dly_est_write", sim.dut->orig_tx_dly_est_write_o,
                sim.dut->new_tx_dly_est_write_o);
    sim.compare("tx_dly_est_code", sim.dut->orig_tx_dly_est_code_o,
                sim.dut->new_tx_dly_est_code_o);
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
    std::mt19937 rng(0xd11d1e5u);
    init_inputs(sim.dut.get());
    sim.reset(6);
    compare_outputs(sim);

    // Disabled pulses toggle ctl_start/ctl_init/ctl_txn without changing state.
    sim.dut->dll_dly_est_ctl_i = pack_ctl(false, true, true, 0xff);
    sim.dut->rx_dly_est_code_i = 0xa5;
    for (int i = 0; i < 8; ++i) tick_compare(sim);
    sim.dut->dll_dly_est_ctl_i = 0;
    for (int i = 0; i < 4; ++i) tick_compare(sim);

    // Directed init and short run exercise LFSR, error-count, done, and restart.
    pulse_init(sim);
    sim.dut->rx_dly_est_code_i = 0x00;
    pulse_start(sim, 0x00);
    for (int i = 0; i < 120; ++i) tick_compare(sim);

    pulse_init(sim);

    // Longer mode with ctl_txn[1:0] == 1 reaches the tx_cnt[11] terminal path.
    // RX data is randomized to exercise both match and mismatch comparisons.
    pulse_start(sim, 0x01);
    for (int i = 0; i < 2300; ++i) {
        sim.dut->rx_dly_est_code_i = rng() & 0xff;
        tick_compare(sim);
        if (sts_done(sim.dut->orig_sts_o) && sts_done(sim.dut->new_sts_o)) break;
    }

    pulse_init(sim);

    // High ctl_txn bits keep rx_end low in the original; abort with init after
    // observing the non-completing path.
    sim.dut->rx_dly_est_code_i = 0xff;
    pulse_start(sim, 0x84);
    for (int i = 0; i < 140; ++i) tick_compare(sim);
    pulse_init(sim);

    // Explicit reset pulse after activity covers the reset input outside the
    // initial global reset.
    sim.dut->reset_i = 1;
    for (int i = 0; i < 3; ++i) tick_compare(sim);
    sim.dut->reset_i = 0;
    for (int i = 0; i < 5; ++i) tick_compare(sim);

    // Random control traffic. Every non-clock input is driven low in the reset
    // phases and high/non-zero here; init pulses periodically recover from long
    // transaction modes.
    for (int cyc = 0; cyc < 2200; ++cyc) {
        bool enable = (rng() % 5) != 0;
        bool init = (rng() % 97) == 0;
        bool start = !init && ((rng() % 83) == 0);
        uint8_t txn = static_cast<uint8_t>(rng());
        if ((cyc % 400) == 399) {
            enable = true;
            init = true;
            start = false;
            txn = 0;
        }
        sim.dut->dll_dly_est_ctl_i = pack_ctl(enable, init, start, txn);
        sim.dut->rx_dly_est_code_i = rng() & 0xff;
        sim.dut->reset_i = ((cyc % 701) == 700) ? 1 : 0;
        tick_compare(sim);
        if (sim.dut->reset_i) {
            sim.dut->reset_i = 0;
            tick_compare(sim);
        }
    }

    sim.dut->dll_dly_est_ctl_i = pack_ctl(true, true, false, 0);
    sim.dut->rx_dly_est_code_i = 0;
    for (int i = 0; i < 8; ++i) tick_compare(sim);

    return sim.finish();
}
