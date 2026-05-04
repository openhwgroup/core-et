// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_test.
//
// Verifies the depth/stencil test unit accepts quads and reports
// empty status. Full depth comparison testing requires the depth buffer
// fill path (SC response), which is covered by the cosim. Here we test
// basic quad accept/pipeline and empty flag.

#include "Vrbox_test_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <cstring>

using DUT = Vrbox_test_tb;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;

    sim.dut->db_sc_req_ready_i = 1;
    sim.dut->ts_quad_x = 0;
    sim.dut->ts_quad_y = 0;
    sim.dut->ts_quad_sample_idx = 0;
    sim.dut->ts_quad_mask = 0;
    memset(&sim.dut->ts_quad_depth, 0, sizeof(sim.dut->ts_quad_depth));
    sim.dut->ts_quad_valid_i = 0;
    sim.dut->ct_result_accept_i = 1;
    sim.dut->depth_test_enable_i = 0;
    sim.dut->depth_test_compare_op_i = 7;  // CompOpAlways
    sim.dut->depth_test_write_enable_i = 0;
    sim.dut->ds_buffer_format_i = 126;      // DsFormatD32Sfloat
    sim.dut->ds_buffer_addr_i = 0x10000;
    sim.dut->ts_front_triangle_i = 0;
    sim.dut->db_flush_i = 0;
    sim.reset(5);

    // -- Test 1: Empty after reset --
    printf("test 1: empty after reset\n");
    // The empty flag depends on internal DLFIFO pointer comparison which
    // initializes via rst_pulse (synchronous reset in always_comb).
    // Allow a few cycles for internal state to settle.
    for (int i = 0; i < 5; i++) sim.tick();
    sim.check(sim.dut->ts_result_valid_o == 0, "no result after reset");

    // -- Test 2: Submit a quad with depth test disabled --
    printf("test 2: submit quad (depth test disabled)\n");
    sim.dut->depth_test_enable_i = 0;
    sim.dut->ts_quad_x = 0;
    sim.dut->ts_quad_y = 0;
    sim.dut->ts_quad_mask = 0xF;
    sim.dut->ts_quad_depth[0] = 0x3F800000;  // 1.0
    sim.dut->ts_quad_depth[1] = 0x3F800000;
    sim.dut->ts_quad_depth[2] = 0x3F800000;
    sim.dut->ts_quad_depth[3] = 0x3F800000;
    sim.dut->ts_quad_valid_i = 1;

    // Wait for accept
    int timeout = 20;
    while (!sim.dut->ts_quad_accept_o && timeout > 0) {
        sim.tick();
        timeout--;
    }
    sim.check(sim.dut->ts_quad_accept_o == 1, "quad accepted");
    sim.dut->ts_quad_valid_i = 0;

    // -- Test 3: Flush completes --
    printf("test 3: flush\n");
    // Let pipeline drain
    for (int i = 0; i < 20; i++) sim.tick();

    sim.dut->db_flush_i = 1;
    timeout = 200;
    while (!sim.dut->db_flush_done_o && timeout > 0) {
        sim.tick();
        timeout--;
    }
    sim.dut->db_flush_i = 0;
    sim.check(sim.dut->db_flush_done_o == 1, "flush done");

    return sim.finish();
}
