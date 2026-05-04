// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_trans_log_rom_case_c0_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_trans_log_rom_case_c0_tb;

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->index_i = 0;
    sim.tick();
    sim.compare("data", sim.dut->orig_data_o, sim.dut->new_data_o);

    for (int i = 0; i < 256; ++i) {
        sim.dut->index_i = i;
        sim.tick();
        sim.compare("data", sim.dut->orig_data_o, sim.dut->new_data_o);
    }

    sim.check(sim.comparisons >= 257,
              "trans ROM cosim should compare the reset sample plus every 8-bit index");
    return sim.finish();
}
