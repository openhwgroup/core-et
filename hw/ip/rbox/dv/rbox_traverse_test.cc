// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_traverse.
//
// Verifies the 9-stage triangle rasterizer pipeline.
// The tile_state_t and triangle_state_t are complex packed structs.
// This test verifies reset behavior, tile acceptance, and quad production.
// Full coverage testing is handled by the cosim.

#include "Vrbox_traverse_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <cstring>

using DUT = Vrbox_traverse_tb;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000000;

    // Initialize all inputs
    memset(&sim.dut->tr_triangle_raw_i, 0, sizeof(sim.dut->tr_triangle_raw_i));
    memset(&sim.dut->tr_tile_raw_i, 0, sizeof(sim.dut->tr_tile_raw_i));
    sim.dut->tr_tile_valid_i = 0;
    sim.dut->ct_quad_accept_i = 1;
    sim.dut->ts_quad_accept_i = 1;
    sim.dut->tr_msaa_enable_i = 0;
    sim.dut->tr_msaa_mode_i = 0;
    sim.dut->tr_depth_is_fp32_i = 1;
    sim.dut->tr_depth_is_un24_i = 0;
    sim.dut->tr_depth_is_un16_i = 0;
    sim.dut->tr_scissor_min_x_i = 0;
    sim.dut->tr_scissor_max_x_i = 0x7FFF;
    sim.dut->tr_scissor_min_y_i = 0;
    sim.dut->tr_scissor_max_y_i = 0x7FFF;
    sim.dut->tr_depth_clamp_enable_i = 0;
    sim.dut->tr_depth_min_i = 0;
    sim.dut->tr_depth_max_i = 0x3F800000;
    sim.dut->tr_depth_bound_enable_i = 0;
    sim.dut->tr_depth_bound_min_i = 0;
    sim.dut->tr_depth_bound_max_i = 0x3F800000;
    sim.reset(5);

    // -- Test 1: Empty after reset --
    printf("test 1: empty after reset\n");
    sim.tick();
    sim.check(sim.dut->tr_empty_o == 1, "traverse empty after reset");
    sim.check(sim.dut->quad_data_valid_o == 0, "no quad data after reset");
    sim.check(sim.dut->quad_test_valid_o == 0, "no quad test after reset");

    // -- Test 2: Accept flag when pipeline is idle --
    printf("test 2: accept flag\n");
    sim.check(sim.dut->tr_accept_o == 1, "accept flag high when idle");

    // -- Test 3: Submit a tile (smallest: TileSize4x4 = 5) --
    // Build the packed tile_state_t. The struct is defined as:
    //   typedef struct packed {
    //     tile_size_e  size;          // [190:188] 3 bits
    //     logic [14:2] left;          // [187:175] 13 bits
    //     logic [14:2] top;           // [174:162] 13 bits
    //     logic        fully_covered; // [161] 1 bit
    //     edge_sample_t [2:0] edge_sample; // [160:32] 3 x 43 = 129 bits
    //     depth_sample_t depth_sample;// [31:0] 32 bits
    //   } tile_state_t;
    // Total: 3+13+13+1+129+32 = 191 bits = 5 words + 31 bits
    printf("test 3: submit tile\n");

    memset(&sim.dut->tr_tile_raw_i, 0, sizeof(sim.dut->tr_tile_raw_i));

    // Depth sample at bits [31:0] = 0x3F800000 (1.0)
    sim.dut->tr_tile_raw_i[0] = 0x3F800000;

    // Edge samples at bits [160:32]: set to large positive values
    // so sample_inside_triangle returns true for fully-covered.
    // Each edge_sample_t is 43 bits signed. Set bit 40 (large positive).
    // edge_sample[0] at bits [74:32]
    // edge_sample[1] at bits [117:75]
    // edge_sample[2] at bits [160:118]
    // For fully_covered mode, these are ignored, but set them positive anyway.
    // Bit 32 is bit 0 of word 1.
    sim.dut->tr_tile_raw_i[1] = 0x00000100;  // edge_sample[0]: positive at bit 40 from base = bit 8 of this word
    sim.dut->tr_tile_raw_i[2] = 0x00000800;  // edge_sample[1]: positive
    sim.dut->tr_tile_raw_i[3] = 0x00400000;  // edge_sample[2]: positive

    // Fully covered at bit 161 = word 5, bit 1
    sim.dut->tr_tile_raw_i[5] = (1 << 1);

    // Left = 0 at bits [187:175], Top = 0 at bits [174:162] (already zero)

    // Size = TileSize4x4 = 5 at bits [190:188]
    // Bit 188 = word 5, bit 28
    sim.dut->tr_tile_raw_i[5] |= (5 << 28);

    sim.dut->tr_tile_valid_i = 1;
    sim.tick();
    bool accepted = sim.dut->tr_accept_o;
    sim.dut->tr_tile_valid_i = 0;
    sim.check(accepted, "tile accepted");

    // -- Test 4: Wait for quads and pipeline drain --
    printf("test 4: quad production and drain\n");
    int quad_count = 0;
    bool emptied = false;
    for (int i = 0; i < 2000 && !emptied; i++) {
        sim.tick();
        if (sim.dut->quad_data_valid_o) quad_count++;
        emptied = sim.dut->tr_empty_o;
    }
    printf("  produced %d quads, emptied=%d\n", quad_count, emptied);
    sim.check(quad_count > 0, "at least one quad produced");
    sim.check(emptied, "pipeline drained");

    return sim.finish();
}
