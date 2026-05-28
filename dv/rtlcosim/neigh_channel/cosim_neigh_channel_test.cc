// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_channel_tb.h"
#include "cosim_ctrl.h"

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_channel_tb> sim(argc, argv);
    sim.reset(12);

    constexpr int kChunks = 512;
    constexpr int kWarmupCycles = 2300;
    constexpr int kCycles = 5000;

    // Warm up through reset/DFT exercise phases before strict output comparison.
    for (int cycle = 0; cycle < kWarmupCycles; ++cycle) {
        sim.tick();
    }

    for (int cycle = 0; cycle < kCycles; ++cycle) {
        sim.tick();
        for (int chunk = 0; chunk < kChunks; ++chunk) {
            sim.compare("out_chunk_" + std::to_string(chunk),
                        sim.dut->orig_chunks_o[chunk],
                        sim.dut->new_chunks_o[chunk]);
        }
    }

    sim.check(sim.dut->input_toggle_ok_o, "deterministic cosim stimulus completed all input-port toggle phases");
    return sim.finish();
}
