// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_top_tb.h"
#include "cosim_ctrl.h"

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_top_tb> sim(argc, argv);
    sim.reset(8);

    constexpr int kChunks = 128;
    constexpr int kWarmupCycles = 2300;
    constexpr int kCycles = 4096;

    // Exercise reset/DFT/isolation phases before strict full-output compare.
    for (int cycle = 0; cycle < kWarmupCycles; ++cycle) {
        sim.tick();
    }

    for (int cycle = 0; cycle < kCycles; ++cycle) {
        sim.tick();
        for (int chunk = 0; chunk < kChunks; ++chunk) {
            sim.compare("neigh_top_chunk_" + std::to_string(chunk),
                        sim.dut->orig_chunks_o[chunk],
                        sim.dut->new_chunks_o[chunk]);
        }
    }

    sim.check(sim.dut->input_toggle_ok_o,
              "neigh_top stimulus toggled all practical random input salts both ways");
    return sim.finish();
}
