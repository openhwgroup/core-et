// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_fifo_semisync_lov — standalone Verilator unit test.

#include "Vprim_fifo_semisync_lov.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <cstdlib>
#include <string>

using DUT = Vprim_fifo_semisync_lov;

static void eval_step(SimCtrl<DUT>& sim) {
    sim.dut->eval();
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
        std::exit(2);
    }
}

static void tick_both(SimCtrl<DUT>& sim) {
    sim.dut->clk_wr_i = 0;
    sim.dut->clk_rd_i = 0;
    eval_step(sim);
    sim.dut->clk_wr_i = 1;
    sim.dut->clk_rd_i = 1;
    eval_step(sim);
}

static void wait_both(SimCtrl<DUT>& sim, int cycles) {
    for (int i = 0; i < cycles; ++i) tick_both(sim);
}

static void functional_reset(SimCtrl<DUT>& sim) {
    sim.dut->push_i = 0;
    sim.dut->pop_i = 0;
    sim.dut->rst_wr_ni = 0;
    sim.dut->rst_rd_ni = 0;
    wait_both(sim, 4);
    sim.check(sim.dut->ready_o == 0, "ready reset low during functional reset");
    sim.check(sim.dut->valid_o == 0, "valid reset low during functional reset");
    sim.dut->rst_wr_ni = 1;
    sim.dut->rst_rd_ni = 1;
    wait_both(sim, 4);
    sim.check(sim.dut->ready_o == 1, "ready after reset release");
    sim.check(sim.dut->valid_o == 0, "empty after reset release");
}

static void push_burst(SimCtrl<DUT>& sim, uint32_t base, int count) {
    for (int i = 0; i < count; ++i) {
        sim.check(sim.dut->ready_o == 1,
                  "ready before push " + std::to_string(i));
        sim.dut->wdata_i = base + static_cast<uint32_t>(i);
        sim.dut->push_i = 1;
        tick_both(sim);
    }
    sim.dut->push_i = 0;
    tick_both(sim);  // flush the final push pulse into the read domain
}

static void drain_words(SimCtrl<DUT>& sim, uint32_t base, int count) {
    for (int i = 0; i < count; ++i) {
        sim.check(sim.dut->valid_o == 1,
                  "valid before pop " + std::to_string(i));
        sim.check(sim.dut->rdata_o == base + static_cast<uint32_t>(i),
                  "ordered pop data " + std::to_string(i));
        sim.dut->pop_i = 1;
        tick_both(sim);
    }
    sim.dut->pop_i = 0;
    tick_both(sim);  // clear the pop pulse on both sides
}

static void push_one(SimCtrl<DUT>& sim, uint32_t data) {
    push_burst(sim, data, 1);
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 200000;

    sim.dut->clk_wr_i = 0;
    sim.dut->clk_rd_i = 0;
    sim.dut->rst_wr_ni = 1;
    sim.dut->rst_rd_ni = 1;
    sim.dut->push_i = 0;
    sim.dut->pop_i = 0;
    sim.dut->wdata_i = 0;
    sim.dut->dft_hv_i = 0;
    sim.dut->dft_lv_i = 0;
    eval_step(sim);

    printf("test 1: functional reset\n");
    functional_reset(sim);

    printf("test 2: single item latency and drain\n");
    push_one(sim, 0xA5A50001u);
    sim.check(sim.dut->valid_o == 1, "single item valid after pulse relay");
    sim.check(sim.dut->rdata_o == 0xA5A50001u, "single item data");
    drain_words(sim, 0xA5A50001u, 1);
    sim.check(sim.dut->valid_o == 0, "empty after single drain");
    wait_both(sim, 2);
    sim.check(sim.dut->ready_o == 1, "ready after single drain credit returns");

    printf("test 3: fill depth=3 and drain in order\n");
    push_burst(sim, 0xC0000000u, 3);
    sim.check(sim.dut->ready_o == 0, "not ready after three queued entries");
    drain_words(sim, 0xC0000000u, 3);
    sim.check(sim.dut->valid_o == 0, "empty after full drain");
    wait_both(sim, 2);
    sim.check(sim.dut->ready_o == 1, "ready after full drain credits return");

    printf("test 4: interleaved traffic\n");
    push_burst(sim, 0xD0000000u, 2);
    drain_words(sim, 0xD0000000u, 1);
    wait_both(sim, 1);
    push_burst(sim, 0xE0000000u, 2);
    sim.check(sim.dut->valid_o == 1, "interleave valid before remaining drain");
    sim.check(sim.dut->rdata_o == 0xD0000001u, "interleave retained second first-burst data");
    sim.dut->pop_i = 1;
    tick_both(sim);
    sim.check(sim.dut->valid_o == 1, "interleave valid new data 0");
    sim.check(sim.dut->rdata_o == 0xE0000000u, "interleave new data 0");
    tick_both(sim);
    sim.check(sim.dut->valid_o == 1, "interleave valid new data 1");
    sim.check(sim.dut->rdata_o == 0xE0000001u, "interleave new data 1");
    tick_both(sim);
    sim.dut->pop_i = 0;
    tick_both(sim);
    sim.check(sim.dut->valid_o == 0, "empty after interleaved drain");
    wait_both(sim, 2);

    printf("test 5: LV DFT controls write-side reset bypass\n");
    push_burst(sim, 0xF0000000u, 3);
    sim.check(sim.dut->ready_o == 0, "full before LV DFT write reset");
    sim.dut->dft_lv_i = 0x10;  // scanmode=1, scan_reset_n=0
    tick_both(sim);
    sim.check(sim.dut->ready_o == 0, "ready masked while LV scan reset asserted");
    sim.dut->dft_lv_i = 0x18;  // scanmode=1, scan_reset_n=1
    tick_both(sim);
    sim.dut->dft_lv_i = 0;
    wait_both(sim, 4);
    sim.check(sim.dut->ready_o == 1, "LV DFT reset clears write-side credits");
    functional_reset(sim);

    printf("test 6: HV DFT controls read-side reset bypass\n");
    push_one(sim, 0x12345678u);
    sim.check(sim.dut->valid_o == 1, "valid before HV DFT read reset");
    sim.dut->dft_hv_i = 0x10;  // scanmode=1, scan_reset_n=0
    tick_both(sim);
    sim.check(sim.dut->valid_o == 0, "valid masked while HV scan reset asserted");
    sim.dut->dft_hv_i = 0x18;  // scanmode=1, scan_reset_n=1
    tick_both(sim);
    sim.dut->dft_hv_i = 0;
    wait_both(sim, 2);
    sim.check(sim.dut->valid_o == 0, "read-side DFT reset clears destination FIFO");

    return sim.finish();
}
