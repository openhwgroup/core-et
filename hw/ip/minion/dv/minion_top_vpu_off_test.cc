// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// VpuEn=0 regression for minion_top's intentional null_vpu path.

#include "Vminion_top_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vminion_top_tb;

namespace {

constexpr uint8_t kMuxVpu0 = 0x00;

void idle_inputs(DUT* dut) {
    dut->enabled_i = 0x1;
    dut->reset_vector_i = 0x1000;
    dut->icache_req_ready_i = 1;
    dut->icache_resp_valid_i = 0;
    for (int i = 0; i < 8; ++i) dut->icache_resp_data_i[i] = 0;
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

void reset_all(SimCtrl<DUT>& sim, int cycles) {
    sim.dut->rst_c_ni = 0;
    sim.dut->rst_w_ni = 0;
    sim.dut->rst_d_ni = 0;
    for (int i = 0; i < cycles; ++i) sim.tick();
    sim.dut->rst_c_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.dut->rst_d_ni = 1;
    sim.tick();
}

bool is_zero_128(const VlWide<4>& bus) {
    return bus[0] == 0 && bus[1] == 0 && bus[2] == 0 && bus[3] == 0;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 50000;

    idle_inputs(sim.dut.get());
    sim.dut->rst_c_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.dut->rst_d_ni = 1;

    reset_all(sim, 5);

    bool saw_fetch = false;
    for (int i = 0; i < 32; ++i) {
        sim.tick();
        if (sim.dut->icache_req_valid_o) {
            saw_fetch = true;
            break;
        }
    }
    sim.check(saw_fetch, "VpuEn=0 null path still allows scalar fetch bring-up");

    sim.dut->minion_dbg_signals_mux_i = kMuxVpu0;
    sim.dut->minion_dbg_sig_enable_i = 1;
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.dut->minion_dbg_sig_enable_i = 0;
    sim.tick();

    sim.check(sim.dut->dbg_filter_valid_o == 1, "VpuEn=0 VPU debug mux remains selectable");
    sim.check(sim.dut->dbg_match_valid_o == 1, "VpuEn=0 VPU debug mux reports match_valid");
    sim.check(sim.dut->dbg_data_valid_o == 1, "VpuEn=0 VPU debug mux reports data_valid");
    sim.check(is_zero_128(sim.dut->dbg_data_o), "VpuEn=0 null_vpu drives zero debug data");

    return sim.finish();
}
