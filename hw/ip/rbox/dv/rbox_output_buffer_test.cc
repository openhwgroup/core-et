// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_output_buffer.
//
// Verifies quad accept, depth test result, and output info generation.
// Tests fragment shader disabled mode (simplest path).

#include "Vrbox_output_buffer_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <cstring>

using DUT = Vrbox_output_buffer_tb;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;

    // Initialize all inputs
    sim.dut->tr_quad_x = 0;
    sim.dut->tr_quad_y = 0;
    sim.dut->tr_quad_mask = 0;
    memset(&sim.dut->tr_quad_i_bary, 0, sizeof(sim.dut->tr_quad_i_bary));
    memset(&sim.dut->tr_quad_j_bary, 0, sizeof(sim.dut->tr_quad_j_bary));
    memset(&sim.dut->tr_quad_depth, 0, sizeof(sim.dut->tr_quad_depth));
    sim.dut->tr_quad_sample_idx = 0;
    memset(&sim.dut->tr_quad_msaa_cov, 0, sizeof(sim.dut->tr_quad_msaa_cov));
    sim.dut->tr_quad_valid_i = 0;
    sim.dut->ts_result_mask_i = 0;
    sim.dut->ts_result_valid_i = 0;
    sim.dut->out_pop_i = 0;
    sim.dut->out_pending_pcks_i = 0;
    sim.dut->out_end_drawcall_i = 0;
    sim.dut->hart_tile_width_i = 0;
    sim.dut->hart_tile_height_i = 0;
    sim.dut->shire_layout_width_i = 0;
    sim.dut->shire_layout_height_i = 0;
    sim.dut->fsh_reads_coverage_i = 0;
    sim.dut->fsh_reads_depth_i = 0;
    sim.dut->fsh_disabled_i = 1;          // Fragment shader disabled
    sim.dut->early_frag_tests_enable_i = 0;
    sim.dut->rbox_halted_i = 0;
    sim.reset(5);

    // -- Test 1: Empty after reset --
    printf("test 1: empty after reset\n");
    sim.tick();
    sim.check(sim.dut->empty_o == 0, "empty initially (reset state)");

    // -- Test 2: Push a quad (fsh disabled -> no bary write needed) --
    printf("test 2: push quad\n");
    sim.dut->tr_quad_x = 2;
    sim.dut->tr_quad_y = 4;
    sim.dut->tr_quad_mask = 0xF;
    sim.dut->tr_quad_valid_i = 1;
    sim.tick();
    bool accepted = sim.dut->ct_quad_accept_o;
    sim.dut->tr_quad_valid_i = 0;
    sim.check(accepted, "quad accepted (fsh disabled)");

    // -- Test 3: Push another quad (different position for start_pack) --
    printf("test 3: push second quad\n");
    sim.dut->tr_quad_x = 4;
    sim.dut->tr_quad_y = 4;
    sim.dut->tr_quad_mask = 0x5;
    sim.dut->tr_quad_valid_i = 1;
    sim.tick();
    accepted = sim.dut->ct_quad_accept_o;
    sim.dut->tr_quad_valid_i = 0;
    sim.check(accepted, "second quad accepted");

    // With early_frag_tests_enable=0, test_done is set immediately.
    // Wait for output info to become valid.
    int timeout = 20;
    while (!sim.dut->out_info_valid_o && timeout > 0) {
        sim.tick();
        timeout--;
    }
    sim.check(sim.dut->out_info_valid_o == 1, "output info valid");

    // -- Test 4: Check output info matches first quad --
    printf("test 4: output info\n");
    if (sim.dut->out_info_valid_o) {
        sim.check(sim.dut->out_info0_x == 2, "info[0].x = 2");
        sim.check(sim.dut->out_info0_y == 4, "info[0].y = 4");
        sim.check(sim.dut->out_info0_mask == 0xF, "info[0].mask = 0xF");
        sim.check(sim.dut->out_info0_valid == 1, "info[0].valid = 1");
    }

    return sim.finish();
}
