// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Retire-path execution test for minion_top.

#include "Vminion_top_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vminion_top_tb;

namespace {

constexpr uint32_t kNop = 0x00000013u;  // addi x0, x0, 0
constexpr uint64_t kResetVector = 0x1000ull;

void idle_inputs(DUT* dut) {
    dut->enabled_i = 0x1;
    dut->reset_vector_i = kResetVector;
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

void drive_fetch_line(DUT* dut, uint32_t inst_word) {
    for (int i = 0; i < 8; ++i) dut->icache_resp_data_i[i] = inst_word;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 250000;

    idle_inputs(sim.dut.get());
    sim.dut->rst_c_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.dut->rst_d_ni = 1;

    reset_all(sim, 5);

    bool saw_first_fetch = false;
    for (int i = 0; i < 32; ++i) {
        sim.tick();
        if (sim.dut->icache_req_valid_o) {
            saw_first_fetch = true;
            break;
        }
    }
    sim.check(saw_first_fetch, "thread0 issues an initial fetch request");
    if (!saw_first_fetch) return sim.finish();

    sim.check(sim.dut->icache_req_thread_id_o == 0, "initial fetch stays on thread0");
    sim.check(sim.dut->icache_req_addr_o == kResetVector, "initial fetch uses the reset vector");

    drive_fetch_line(sim.dut.get(), kNop);
    sim.dut->te_enable_i = 1;
    sim.dut->icache_resp_valid_i = 1;

    bool saw_retire_1000 = false;
    bool saw_retire_1004 = false;

    for (int i = 0; i < 96; ++i) {
        sim.tick();

        if (sim.dut->trace_instr_valid_o) {
            sim.check(sim.dut->trace_exception_o == 0, "retired scalar NOP does not raise an exception");
            sim.check(sim.dut->trace_instr_bus_o == kNop, "retired instruction matches the supplied NOP");

            if (sim.dut->trace_instr_addr_o == kResetVector) {
                saw_retire_1000 = true;
            } else if (sim.dut->trace_instr_addr_o == kResetVector + 4) {
                saw_retire_1004 = true;
            }

            if (saw_retire_1000 && saw_retire_1004) break;
        }
    }

    sim.dut->icache_resp_valid_i = 0;
    sim.tick();

    sim.check(saw_retire_1000, "trace encoder reports retirement at reset_vector");
    sim.check(saw_retire_1004, "trace encoder reports retirement of the next sequential instruction");

    return sim.finish();
}
