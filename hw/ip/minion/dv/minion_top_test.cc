// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Smoke test for minion_top.

#include "Vminion_top_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

using DUT = Vminion_top_tb;

static constexpr uint16_t kDebugAxProgBuf0Addr = 0x0804;
static constexpr uint8_t kMuxVpu0 = 0x00;

static void idle_inputs(DUT* dut) {
    dut->enabled_i = 0x1;
    dut->reset_vector_i = 0x1000;
    dut->icache_req_ready_i = 1;
    dut->icache_resp_valid_i = 0;
    dut->icache_resp_data_i[0] = 0;
    dut->icache_resp_data_i[1] = 0;
    dut->icache_resp_data_i[2] = 0;
    dut->icache_resp_data_i[3] = 0;
    dut->icache_resp_data_i[4] = 0;
    dut->icache_resp_data_i[5] = 0;
    dut->icache_resp_data_i[6] = 0;
    dut->icache_resp_data_i[7] = 0;
    dut->debug_halt_i = 0;
    dut->debug_resume_i = 0;
    dut->debug_resethalt_i = 0;
    dut->debug_ackhavereset_i = 0;
    dut->apb_paddr_i = 0;
    dut->apb_pwrite_i = 0;
    dut->apb_psel_i = 0;
    dut->apb_penable_i = 0;
    dut->apb_pwdata_i = 0;
    dut->minion_dbg_signals_mux_i = 0;
    dut->minion_dbg_sig_enable_i = 0;
    dut->te_enable_i = 0;
    dut->nsleepin_i = 1;
}

static void reset_all(SimCtrl<DUT>& sim, int cycles) {
    sim.dut->rst_c_ni = 0;
    sim.dut->rst_w_ni = 0;
    sim.dut->rst_d_ni = 0;
    for (int i = 0; i < cycles; ++i) sim.tick();
    sim.dut->rst_c_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.dut->rst_d_ni = 1;
    sim.tick();
}

static void apb_setup(DUT* dut, uint16_t addr, bool write, uint64_t data) {
    dut->apb_paddr_i = addr;
    dut->apb_pwrite_i = write ? 1 : 0;
    dut->apb_psel_i = 1;
    dut->apb_penable_i = 0;
    dut->apb_pwdata_i = data;
}

static void apb_idle(DUT* dut) {
    dut->apb_paddr_i = 0;
    dut->apb_pwrite_i = 0;
    dut->apb_psel_i = 0;
    dut->apb_penable_i = 0;
    dut->apb_pwdata_i = 0;
}

static void apb_write(SimCtrl<DUT>& sim, uint16_t addr, uint64_t data,
                      bool* saw_pready, bool* saw_pslverr) {
    *saw_pready = false;
    *saw_pslverr = false;

    apb_setup(sim.dut.get(), addr, true, data);
    sim.tick();

    sim.dut->apb_penable_i = 1;
    for (int i = 0; i < 8; ++i) {
        sim.tick();
        if (sim.dut->apb_pready_o) {
            *saw_pready = true;
            *saw_pslverr = sim.dut->apb_pslverr_o;
            break;
        }
    }

    apb_idle(sim.dut.get());
    sim.tick();
}

static bool is_zero_128(const VlWide<4>& bus) {
    return bus[0] == 0 && bus[1] == 0 && bus[2] == 0 && bus[3] == 0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 250000;

    idle_inputs(sim.dut.get());
    sim.dut->rst_c_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.dut->rst_d_ni = 1;

    reset_all(sim, 5);

    sim.check(sim.dut->nsleepout_o == 1, "nsleepout mirrors nsleepin");
    sim.check(sim.dut->eco_o == 0, "eco outputs stay tied low");
    bool trace_cleared = false;
    bool saw_have_reset = false;
    for (int i = 0; i < 8; ++i) {
        if (sim.dut->trace_min_reset_o == 0) trace_cleared = true;
        if ((sim.dut->debug_have_reset_o & 0x1) == 0x1) saw_have_reset = true;
        if (trace_cleared && saw_have_reset) break;
        sim.tick();
    }
    sim.check(trace_cleared, "trace min_reset clears after synchronized reset release");
    sim.check(saw_have_reset, "thread0 reports have_reset after synchronized reset release");

    bool saw_fetch = false;
    for (int i = 0; i < 32; ++i) {
        sim.tick();
        if (sim.dut->icache_req_valid_o) {
            saw_fetch = true;
            break;
        }
    }
    sim.check(saw_fetch, "wrapped core issues an icache request after reset");
    if (saw_fetch) {
        sim.check(sim.dut->icache_req_thread_id_o == 0, "first icache request is for thread0");
        sim.check(sim.dut->icache_req_addr_o == 0x1000, "first icache request uses reset_vector");
    }

    sim.dut->debug_ackhavereset_i = 0x1;
    sim.tick();
    sim.dut->debug_ackhavereset_i = 0x0;
    sim.tick();
    sim.check((sim.dut->debug_have_reset_o & 0x1) == 0x0, "ackhavereset clears thread0 have_reset");

    bool saw_pready = false;
    bool saw_pslverr = false;
    apb_write(sim, kDebugAxProgBuf0Addr, 0x1122334455667788ull, &saw_pready, &saw_pslverr);
    sim.check(saw_pready, "debug APB write completes");
    sim.check(saw_pslverr, "debug APB rejects program-buffer write while not in debug mode");

    sim.dut->minion_dbg_signals_mux_i = kMuxVpu0;
    sim.dut->minion_dbg_sig_enable_i = 1;
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.dut->minion_dbg_sig_enable_i = 0;
    sim.tick();
    sim.check(sim.dut->dbg_filter_valid_o == 1, "VPU debug mux reports filter_valid");
    sim.check(sim.dut->dbg_match_valid_o == 1, "VPU debug mux reports match_valid");
    sim.check(sim.dut->dbg_data_valid_o == 1, "VPU debug mux reports data_valid");
    sim.check(is_zero_128(sim.dut->dbg_data_o), "null_vpu drives zero debug data");

    sim.dut->nsleepin_i = 0;
    sim.tick();
    sim.check(sim.dut->nsleepout_o == 0, "nsleepout tracks nsleepin low");

    sim.dut->rst_c_ni = 0;
    sim.dut->rst_w_ni = 0;
    sim.dut->rst_d_ni = 0;
    sim.tick();
    sim.check(sim.dut->trace_min_reset_o == 1, "trace min_reset asserts while any reset is active");
    sim.dut->rst_c_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.dut->rst_d_ni = 1;
    sim.tick();

    return sim.finish();
}
