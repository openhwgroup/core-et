// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_fifo_async_lov — standalone Verilator unit test.

#include "Vprim_fifo_async_lov.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vprim_fifo_async_lov;

static void eval_step(SimCtrl<DUT>& sim) {
    sim.dut->eval();
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
        std::exit(2);
    }
}

static void set_wr_clk(SimCtrl<DUT>& sim, uint8_t value) {
    sim.dut->clk_wr_i = value;
    eval_step(sim);
}

static void set_rd_clk(SimCtrl<DUT>& sim, uint8_t value) {
    sim.dut->clk_rd_i = value;
    eval_step(sim);
}

static void wr_tick(SimCtrl<DUT>& sim) {
    set_wr_clk(sim, 0);
    set_wr_clk(sim, 1);
}

static void rd_tick(SimCtrl<DUT>& sim) {
    set_rd_clk(sim, 0);
    set_rd_clk(sim, 1);
}

static void tick_both(SimCtrl<DUT>& sim) {
    sim.dut->clk_wr_i = 0;
    sim.dut->clk_rd_i = 0;
    eval_step(sim);
    sim.dut->clk_wr_i = 1;
    sim.dut->clk_rd_i = 1;
    eval_step(sim);
}

static void wait_wr(SimCtrl<DUT>& sim, int cycles) {
    for (int i = 0; i < cycles; ++i) wr_tick(sim);
}

static void wait_rd(SimCtrl<DUT>& sim, int cycles) {
    for (int i = 0; i < cycles; ++i) rd_tick(sim);
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
    sim.check(sim.dut->ready_o == 0, "ready masked during functional reset");
    sim.check(sim.dut->valid_o == 0, "valid masked during functional reset");
    sim.dut->rst_wr_ni = 1;
    sim.dut->rst_rd_ni = 1;
    wait_both(sim, 4);
    sim.check(sim.dut->ready_o == 1, "ready after reset release");
    sim.check(sim.dut->valid_o == 0, "empty after reset release");
}

static void push_word(SimCtrl<DUT>& sim, uint32_t data) {
    sim.dut->wdata_i = data;
    sim.dut->push_i = 1;
    wr_tick(sim);
    sim.dut->push_i = 0;
}

static void pop_word(SimCtrl<DUT>& sim) {
    sim.dut->pop_i = 1;
    rd_tick(sim);
    sim.dut->pop_i = 0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

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

    printf("test 1: functional reset masks outputs\n");
    functional_reset(sim);

    printf("test 2: independent clocks delay visibility until read clock ticks\n");
    push_word(sim, 0xA5A50001u);
    sim.check(sim.dut->ready_o == 1, "ready after one independent write");
    sim.check(sim.dut->valid_o == 0, "read side still empty before read clock sync");
    wait_rd(sim, 3);
    sim.check(sim.dut->valid_o == 1, "valid after read-side sync clocks");
    sim.check(sim.dut->rdata_o == 0xA5A50001u, "rdata after independent write");
    pop_word(sim);
    wait_rd(sim, 2);
    sim.check(sim.dut->valid_o == 0, "empty after independent read drain");
    wait_wr(sim, 3);
    sim.check(sim.dut->ready_o == 1, "write side ready after read pointer sync");

    printf("test 3: fill to full and drain in order\n");
    for (int i = 0; i < 8; ++i) {
        sim.check(sim.dut->ready_o == 1,
                  "ready before fill push " + std::to_string(i));
        push_word(sim, 0xC0000000u + static_cast<uint32_t>(i));
    }
    sim.check(sim.dut->ready_o == 0, "not ready after eight queued entries");
    wait_rd(sim, 3);
    for (int i = 0; i < 8; ++i) {
        sim.check(sim.dut->valid_o == 1,
                  "valid before drain pop " + std::to_string(i));
        sim.check(sim.dut->rdata_o == 0xC0000000u + static_cast<uint32_t>(i),
                  "ordered drain data " + std::to_string(i));
        pop_word(sim);
    }
    wait_rd(sim, 2);
    sim.check(sim.dut->valid_o == 0, "empty after full drain");
    wait_wr(sim, 3);
    sim.check(sim.dut->ready_o == 1, "ready after full drain pointer sync");

    printf("test 4: interleaved push/pop order\n");
    for (int i = 0; i < 4; ++i) push_word(sim, 0xD0000000u + static_cast<uint32_t>(i));
    wait_rd(sim, 3);
    for (int i = 0; i < 2; ++i) {
        sim.check(sim.dut->rdata_o == 0xD0000000u + static_cast<uint32_t>(i),
                  "interleave first data " + std::to_string(i));
        pop_word(sim);
    }
    wait_wr(sim, 3);
    for (int i = 0; i < 3; ++i) push_word(sim, 0xE0000000u + static_cast<uint32_t>(i));
    wait_rd(sim, 3);
    for (int i = 0; i < 5; ++i) {
        uint32_t expected = (i < 2) ? (0xD0000002u + static_cast<uint32_t>(i))
                                    : (0xE0000000u + static_cast<uint32_t>(i - 2));
        sim.check(sim.dut->valid_o == 1,
                  "interleave valid " + std::to_string(i));
        sim.check(sim.dut->rdata_o == expected,
                  "interleave ordered data " + std::to_string(i));
        pop_word(sim);
    }
    wait_rd(sim, 2);
    sim.check(sim.dut->valid_o == 0, "empty after interleaved drain");
    wait_wr(sim, 3);

    printf("test 5: LOV DFT controls write-side reset bypass\n");
    for (int i = 0; i < 8; ++i) push_word(sim, 0xF0000000u + static_cast<uint32_t>(i));
    sim.check(sim.dut->ready_o == 0, "full before LV DFT write reset");
    sim.dut->dft_lv_i = 0x10;  // scanmode=1, scan_reset_n=0 in packed dft_t
    wr_tick(sim);
    sim.check(sim.dut->ready_o == 0, "ready masked while LV scan reset is asserted");
    sim.dut->dft_lv_i = 0x18;  // scanmode=1, scan_reset_n=1
    wr_tick(sim);
    sim.dut->dft_lv_i = 0;
    wait_wr(sim, 1);
    sim.check(sim.dut->ready_o == 1, "LV DFT reset clears LOV write side");
    functional_reset(sim);

    printf("test 6: HIV DFT controls read-side reset bypass\n");
    push_word(sim, 0x12345678u);
    wait_rd(sim, 3);
    sim.check(sim.dut->valid_o == 1, "valid before HV DFT read reset");
    sim.check(sim.dut->rdata_o == 0x12345678u, "data before HV DFT read reset");
    sim.dut->dft_hv_i = 0x10;  // scanmode=1, scan_reset_n=0
    rd_tick(sim);
    sim.check(sim.dut->valid_o == 0, "valid masked while HV scan reset is asserted");
    sim.dut->dft_hv_i = 0x18;  // scanmode=1, scan_reset_n=1
    rd_tick(sim);
    sim.dut->dft_hv_i = 0;
    wait_rd(sim, 3);
    sim.check(sim.dut->valid_o == 1, "HV DFT reset release resynchronizes read side");
    sim.check(sim.dut->rdata_o == 0x12345678u, "data remains at head after read reset");
    pop_word(sim);
    wait_rd(sim, 2);
    sim.check(sim.dut->valid_o == 0, "empty after DFT read-reset item popped");

    return sim.finish();
}
