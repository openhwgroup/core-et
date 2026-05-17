// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_top_pwrstub_tb.h"
#include "cosim_ctrl.h"

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_top_pwrstub_tb> sim(argc, argv);
    sim.reset(4);

    constexpr int kChunks = 8;
    constexpr int kCycles = 4096;
    for (int cycle = 0; cycle < kCycles; ++cycle) {
        sim.tick();
        for (int chunk = 0; chunk < kChunks; ++chunk) {
            sim.compare("pwrstub_chunk_" + std::to_string(chunk),
                        sim.dut->orig_chunks_o[chunk],
                        sim.dut->new_chunks_o[chunk]);
        }
    }
    sim.check(sim.dut->input_toggle_ok_o, "pwrstub stimulus toggled every input salt both ways");
    return sim.finish();
}
