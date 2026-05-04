// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Debug-disabled smoke test for minion_top.

#include "Vminion_top_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <string>

using DUT = Vminion_top_tb;

static constexpr uint16_t kDebugAxProgBuf0Addr = 0x0804;
static constexpr uint16_t kDcacheDebugAddr = 0x0000;
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

static void check_apb_error(SimCtrl<DUT>& sim, uint16_t addr, bool write, const char* what) {
    sim.dut->apb_paddr_i = addr;
    sim.dut->apb_pwrite_i = write ? 1 : 0;
    sim.dut->apb_psel_i = 1;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i = 0x1122334455667788ull;
    sim.tick();

    sim.dut->apb_penable_i = 1;
    bool saw_pready = false;
    bool saw_pslverr = false;
    uint64_t prdata = ~uint64_t{0};
    for (int i = 0; i < 4; ++i) {
        sim.tick();
        if (sim.dut->apb_pready_o) {
            saw_pready = true;
            saw_pslverr = sim.dut->apb_pslverr_o;
            prdata = sim.dut->apb_prdata_o;
            break;
        }
    }

    sim.check(saw_pready, std::string(what) + " completes");
    sim.check(saw_pslverr, std::string(what) + " returns pslverr");
    sim.check(prdata == 0, std::string(what) + " returns zero read data");

    sim.dut->apb_paddr_i = 0;
    sim.dut->apb_pwrite_i = 0;
    sim.dut->apb_psel_i = 0;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i = 0;
    sim.tick();
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 50000;

    idle_inputs(sim.dut.get());
    sim.dut->rst_c_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.dut->rst_d_ni = 1;

    reset_all(sim, 5);

    check_apb_error(sim, kDcacheDebugAddr, false, "DebugApbEn=0 dcache-window APB read");
    check_apb_error(sim, kDcacheDebugAddr, true, "DebugApbEn=0 dcache-window APB write");
    check_apb_error(sim, kDebugAxProgBuf0Addr, false, "DebugApbEn=0 debug-module APB read");
    check_apb_error(sim, kDebugAxProgBuf0Addr, true, "DebugApbEn=0 debug-module APB write");

    sim.dut->minion_dbg_signals_mux_i = kMuxVpu0;
    sim.dut->minion_dbg_sig_enable_i = 1;
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.dut->minion_dbg_sig_enable_i = 0;
    sim.tick();
    sim.check(sim.dut->dbg_filter_valid_o == 0, "DebugMonEn=0 debug mux filter_valid is tied off");
    sim.check(sim.dut->dbg_match_valid_o == 0, "DebugMonEn=0 debug mux match_valid is tied off");
    sim.check(sim.dut->dbg_data_valid_o == 0, "DebugMonEn=0 debug mux data_valid is tied off");

    return sim.finish();
}
