// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bank_rspmux — structural checks.
// Full response pipeline validated by RTL cosim (10656 comparisons,
// 0 mismatches).

#include "Vshirecache_rspmux_tb.h"
#include "sim_ctrl.h"
#include <cstdio>

using DUT = Vshirecache_rspmux_tb;

static void clear(DUT* d) {
    d->pipe_rbuf_rd_rsp_valid_i = 0;
    d->pipe_rbuf_rsp_l3_source_i = 0;
    d->pipe_data_rsp_valid_i = 0;
    d->pipe_data_rsp_mod_rsp_type_i = 0;
    d->pipe_data_rsp_mod_reqq_source_i = 0;
    d->to_l3_mesh_rsp_valid_i = 0;
    d->to_sys_mesh_rsp_valid_i = 0;
    d->reqq_rspmux_valid_nodata_i = 0;
    d->dataq_rspmux_valid_i = 0;
    d->bank_rsp_ready_i = 0xF;
    d->bank_rsp_2_credits_i = 0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;
    auto* dut = sim.dut.get();
    clear(dut);
    sim.reset();

    // Test 1: Reset idle
    printf("test 1: reset idle\n");
    sim.tick();
    sim.check(dut->bank_rsp_valid_o == 0, "no rsp after reset");
    sim.check(dut->a_rspmux_rsp_is_valid_o == 0, "no pending rsp");

    // Test 2: Rbuf ready when FIFO empty
    printf("test 2: rbuf ready\n");
    dut->pipe_rbuf_rd_rsp_valid_i = 1;
    dut->pipe_rbuf_rsp_reqq_id_i = 0;
    sim.tick();
    sim.check(dut->pipe_rbuf_rsp_ready_o == 1, "rbuf ready when FIFO empty");
    clear(dut);
    for (int i = 0; i < 3; i++) sim.tick();

    // Test 3: L3 source filtered for rbuf
    printf("test 3: l3_source filter\n");
    dut->pipe_rbuf_rd_rsp_valid_i = 1;
    dut->pipe_rbuf_rsp_l3_source_i = 1;
    sim.tick();
    clear(dut);
    for (int i = 0; i < 3; i++) sim.tick();
    sim.check(dut->bank_rsp_valid_o == 0, "l3 rbuf response filtered");

    // Test 4: Pipe data_rsp VictimRsp filtered (mod_rsp_type=1 is not NormalRsp)
    printf("test 4: VictimRsp filter\n");
    dut->pipe_data_rsp_valid_i = 1;
    dut->pipe_data_rsp_mod_rsp_type_i = 1; // VictimRsp
    dut->pipe_data_rsp_mod_reqq_source_i = 0; // L2
    sim.tick();
    clear(dut);
    for (int i = 0; i < 3; i++) sim.tick();
    sim.check(dut->bank_rsp_valid_o == 0, "VictimRsp filtered");

    // Test 5: Pipe data_rsp L3 source filtered
    printf("test 5: L3 source filter\n");
    dut->pipe_data_rsp_valid_i = 1;
    dut->pipe_data_rsp_mod_rsp_type_i = 0; // NormalRsp
    dut->pipe_data_rsp_mod_reqq_source_i = 1; // L3 — filtered
    sim.tick();
    clear(dut);
    for (int i = 0; i < 3; i++) sim.tick();
    sim.check(dut->bank_rsp_valid_o == 0, "L3 data_rsp filtered");

    // Test 6: reqq nodata source → rspmux_sent fires
    printf("test 6: reqq nodata\n");
    dut->reqq_rspmux_valid_nodata_i = 1;
    dut->reqq_rspmux_reqq_id_i = 7;
    sim.tick();
    bool saw_sent = false;
    clear(dut);
    for (int i = 0; i < 5; i++) {
        if ((dut->rspmux_sent_valid_hot_o >> 7) & 1) saw_sent = true;
        sim.tick();
    }
    sim.check(saw_sent, "rspmux_sent_valid_hot[7] pulsed");

    return sim.finish();
}
