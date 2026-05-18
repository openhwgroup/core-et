// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vshire_uncached_fcc_tb.h"
#include "sim_ctrl.h"

int main(int argc, char** argv) {
    SimCtrl<Vshire_uncached_fcc_tb> sim(argc, argv);

    sim.dut->esr_enables_i = 0;
    sim.dut->esr_wdata_i = 0;
    sim.reset();

    sim.check(sim.dut->fcc_flat_o == 0, "FCC idle data is zero");
    sim.check(sim.dut->fcc_target_o == 0, "FCC idle target is zero");

    sim.dut->esr_wdata_i = 0x0000a5a5u;
    sim.dut->esr_enables_i = 0x1;
    sim.tick();
    sim.check(sim.dut->fcc_flat_o == 0xa5a5u, "FCC register 0 forwards ESR data");
    sim.check(sim.dut->fcc_target_o == 0, "FCC register 0 target");

    sim.dut->esr_enables_i = 0x2;
    sim.dut->esr_wdata_i = 0x00005a3cu;
    sim.tick();
    sim.check(sim.dut->fcc_flat_o == 0x5a3cu, "FCC register 1 forwards ESR data");
    sim.check(sim.dut->fcc_target_o == 1, "FCC register 1 target");

    sim.dut->esr_enables_i = 0x8;
    sim.dut->esr_wdata_i = 0x000000f0u;
    sim.tick();
    sim.check(sim.dut->fcc_flat_o == 0x00f0u, "FCC register 3 forwards ESR data");
    sim.check(sim.dut->fcc_target_o == 3, "FCC register 3 target");

    sim.dut->esr_enables_i = 0;
    sim.tick();
    sim.check(sim.dut->fcc_flat_o == 0, "FCC output clears without write enable");

    return sim.finish();
}
