// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vdll_dly_est_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <vector>

using DUT = Vdll_dly_est_tb;

static uint16_t pack_ctl(bool enable, bool init, bool start, uint8_t txn) {
    return (static_cast<uint16_t>(enable) << 10) |
           (static_cast<uint16_t>(init) << 9) |
           (static_cast<uint16_t>(start) << 8) |
           txn;
}

static bool sts_done(uint16_t sts) { return (sts & 0x1u) != 0; }
static uint8_t sts_errn(uint16_t sts) { return static_cast<uint8_t>((sts >> 1) & 0xffu); }

static void init_inputs(DUT* dut) {
    dut->core_ctl_i = 0;
    dut->core_rx_code_i = 0;
    dut->wrap_ctl_i = 0;
    dut->wrap_rst_neigh_ni = 1;
}

static void idle_enabled_core(DUT* dut, uint8_t txn = 0) {
    dut->core_ctl_i = pack_ctl(true, false, false, txn);
}

static void idle_enabled_wrap(DUT* dut, uint8_t txn = 0) {
    dut->wrap_ctl_i = pack_ctl(true, false, false, txn);
}

static void pulse_core_init(SimCtrl<DUT>& sim) {
    sim.dut->core_ctl_i = pack_ctl(true, true, false, 0);
    sim.tick();
    idle_enabled_core(sim.dut.get());
    for (int i = 0; i < 4; ++i) sim.tick();
}

static void pulse_core_start(SimCtrl<DUT>& sim, uint8_t txn) {
    sim.dut->core_ctl_i = pack_ctl(true, false, true, txn);
    sim.tick();
    idle_enabled_core(sim.dut.get(), txn);
}

static void pulse_wrap_init(SimCtrl<DUT>& sim) {
    sim.dut->wrap_ctl_i = pack_ctl(true, true, false, 0);
    sim.tick();
    idle_enabled_wrap(sim.dut.get());
    for (int i = 0; i < 4; ++i) sim.tick();
}

static void pulse_wrap_start(SimCtrl<DUT>& sim, uint8_t txn) {
    sim.dut->wrap_ctl_i = pack_ctl(true, false, true, txn);
    sim.tick();
    idle_enabled_wrap(sim.dut.get(), txn);
}

static bool wait_core_done(SimCtrl<DUT>& sim, int max_cycles) {
    for (int i = 0; i < max_cycles; ++i) {
        sim.tick();
        if (sts_done(sim.dut->core_sts_o)) return true;
    }
    return false;
}

static bool wait_wrap_done(SimCtrl<DUT>& sim, int max_cycles) {
    for (int i = 0; i < max_cycles; ++i) {
        sim.tick();
        if (sts_done(sim.dut->wrap_sts_o)) return true;
    }
    return false;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    init_inputs(sim.dut.get());
    sim.reset(6);

    sim.check(!sts_done(sim.dut->core_sts_o), "core reset clears done");
    sim.check(sts_errn(sim.dut->core_sts_o) == 0, "core reset clears error count");
    sim.check(sim.dut->core_tx_code_o == 1, "core reset initializes TX LFSR to PolyInit");
    sim.check(!sts_done(sim.dut->wrap_sts_o), "wrapper reset clears done");
    sim.check(sts_errn(sim.dut->wrap_sts_o) == 0, "wrapper reset clears error count");

    // Disabled start/init pulses must not launch the estimator.
    sim.dut->core_ctl_i = pack_ctl(false, true, true, 0xff);
    for (int i = 0; i < 6; ++i) sim.tick();
    sim.check(!sim.dut->core_tx_write_o, "disabled core control does not assert TX write");
    sim.check(!sts_done(sim.dut->core_sts_o), "disabled core control does not set done");

    // Core init and short transaction: collect the first LFSR words and force
    // RX mismatches so the error path and done path are both observable.
    pulse_core_init(sim);
    sim.check(sts_errn(sim.dut->core_sts_o) == 0, "core init clears error count");
    sim.check(!sts_done(sim.dut->core_sts_o), "core init clears done");
    sim.dut->core_rx_code_i = 0x00;
    pulse_core_start(sim, 0x00);

    std::vector<uint8_t> writes;
    for (int i = 0; i < 24; ++i) {
        sim.tick();
        if (sim.dut->core_tx_write_o) writes.push_back(sim.dut->core_tx_code_o);
    }
    sim.check(writes.size() >= 5, "core short transaction emits multiple TX writes");
    if (writes.size() >= 5) {
        sim.check(writes[0] == 0x01, "core first TX LFSR word is 0x01");
        sim.check(writes[1] == 0x02, "core second TX LFSR word is 0x02");
        sim.check(writes[2] == 0x04, "core third TX LFSR word is 0x04");
        sim.check(writes[3] == 0x08, "core fourth TX LFSR word is 0x08");
        sim.check(writes[4] == 0x10, "core fifth TX LFSR word is 0x10");
    }
    bool core_done = wait_core_done(sim, 80);
    sim.check(core_done, "core short transaction reaches done");
    sim.check(sts_errn(sim.dut->core_sts_o) > 0, "core counts forced RX mismatches");

    pulse_core_init(sim);
    sim.check(sts_errn(sim.dut->core_sts_o) == 0, "core second init clears mismatch count");
    sim.check(!sts_done(sim.dut->core_sts_o), "core second init clears done");

    // ctl_txn[7:2] being non-zero preserves the original no-rx-end path.
    sim.dut->core_rx_code_i = 0xff;
    pulse_core_start(sim, 0x84);
    for (int i = 0; i < 90; ++i) sim.tick();
    sim.check(!sts_done(sim.dut->core_sts_o), "core high ctl_txn bits suppress RX done");
    pulse_core_init(sim);

    // Wrapper loopback and independent neighborhood reset behavior. With both
    // domains released the short run completes; with the LV side held in reset
    // the core still times out its expected return and counts mismatches.
    pulse_wrap_init(sim);
    sim.check(sts_errn(sim.dut->wrap_sts_o) == 0, "wrapper init clears error count");
    sim.check(!sts_done(sim.dut->wrap_sts_o), "wrapper init clears done");
    pulse_wrap_start(sim, 0x00);
    bool wrap_done = wait_wrap_done(sim, 120);
    sim.check(wrap_done, "wrapper short transaction reaches done");

    pulse_wrap_init(sim);
    sim.dut->wrap_rst_neigh_ni = 0;
    for (int i = 0; i < 3; ++i) sim.tick();
    pulse_wrap_start(sim, 0x00);
    bool wrap_neigh_reset_done = wait_wrap_done(sim, 120);
    sim.check(wrap_neigh_reset_done, "wrapper completes with neighborhood reset held");
    sim.check(sts_errn(sim.dut->wrap_sts_o) > 0,
              "wrapper counts loopback errors while neighborhood reset is held");
    sim.dut->wrap_rst_neigh_ni = 1;
    pulse_wrap_init(sim);
    sim.check(sts_errn(sim.dut->wrap_sts_o) == 0,
              "wrapper init clears errors after neighborhood reset release");

    return sim.finish();
}
