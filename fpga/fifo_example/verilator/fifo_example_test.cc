// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// fifo_example — Verilator test.

#include "Vfifo_example.h"
#include "sim_ctrl.h"

using DUT = Vfifo_example;

static void write(SimCtrl<DUT>& s, uint8_t data) {
    s.dut->wvalid_i = 1;
    s.dut->wdata_i  = data;
    s.tick();
    s.dut->wvalid_i = 0;
}

static uint8_t read(SimCtrl<DUT>& s) {
    uint8_t val = s.dut->rdata_o;
    s.dut->rready_i = 1;
    s.tick();
    s.dut->rready_i = 0;
    return val;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->wvalid_i = 0;
    sim.dut->rready_i = 0;
    sim.dut->clr_i    = 0;
    sim.reset();

    // Write and read back
    printf("test: write and read\n");
    write(sim, 0xAB);
    sim.check(sim.dut->rvalid_o == 1, "rvalid after write");
    sim.check(sim.dut->rdata_o == 0xAB, "data matches");

    uint8_t v = read(sim);
    sim.check(v == 0xAB, "read returns 0xAB");
    sim.check(sim.dut->empty_o == 1, "empty after read");

    // Fill to capacity
    printf("test: fill to capacity\n");
    for (int i = 0; i < 4; i++) write(sim, 0x10 + i);
    sim.check(sim.dut->full_o == 1, "full after 4 writes");

    // Read back in order
    printf("test: FIFO ordering\n");
    for (int i = 0; i < 4; i++) {
        sim.check(sim.dut->rdata_o == (uint8_t)(0x10 + i),
                  "data[" + std::to_string(i) + "] correct");
        read(sim);
    }
    sim.check(sim.dut->empty_o == 1, "empty after drain");

    return sim.finish();
}
