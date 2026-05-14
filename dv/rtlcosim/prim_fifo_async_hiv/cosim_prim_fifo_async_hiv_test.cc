// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Standalone RTL cosim for prim_fifo_async_hiv against vcfifo_wr_hiv_gcd.

#include "Vcosim_prim_fifo_async_hiv_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdlib>
#include <deque>
#include <string>

using DUT = Vcosim_prim_fifo_async_hiv_tb;

struct InputCoverage {
    bool clk_wr_0 = false;
    bool clk_wr_1 = false;
    bool clk_rd_0 = false;
    bool clk_rd_1 = false;
    bool rst_wr_0 = false;
    bool rst_wr_1 = false;
    bool rst_rd_0 = false;
    bool rst_rd_1 = false;
    bool push_0 = false;
    bool push_1 = false;
    bool pop_0 = false;
    bool pop_1 = false;
    bool data_0 = false;
    bool data_1 = false;
    bool dft_hv_scanmode_0 = false;
    bool dft_hv_scanmode_1 = false;
    bool dft_hv_scan_reset_0 = false;
    bool dft_hv_scan_reset_1 = false;
    bool dft_lv_scanmode_0 = false;
    bool dft_lv_scanmode_1 = false;
    bool dft_lv_scan_reset_0 = false;
    bool dft_lv_scan_reset_1 = false;
};

static void record_inputs(CosimCtrl<DUT>& sim, InputCoverage& cov) {
    cov.clk_wr_0 |= !sim.dut->clk_wr_i;
    cov.clk_wr_1 |= sim.dut->clk_wr_i;
    cov.clk_rd_0 |= !sim.dut->clk_rd_i;
    cov.clk_rd_1 |= sim.dut->clk_rd_i;
    cov.rst_wr_0 |= !sim.dut->rst_wr_ni;
    cov.rst_wr_1 |= sim.dut->rst_wr_ni;
    cov.rst_rd_0 |= !sim.dut->rst_rd_ni;
    cov.rst_rd_1 |= sim.dut->rst_rd_ni;
    cov.push_0 |= !sim.dut->push_i;
    cov.push_1 |= sim.dut->push_i;
    cov.pop_0 |= !sim.dut->pop_i;
    cov.pop_1 |= sim.dut->pop_i;
    cov.data_0 |= sim.dut->wdata_i == 0;
    cov.data_1 |= sim.dut->wdata_i != 0;
    cov.dft_hv_scanmode_0 |= !sim.dut->dft_hv_scanmode_i;
    cov.dft_hv_scanmode_1 |= sim.dut->dft_hv_scanmode_i;
    cov.dft_hv_scan_reset_0 |= !sim.dut->dft_hv_scan_reset_ni;
    cov.dft_hv_scan_reset_1 |= sim.dut->dft_hv_scan_reset_ni;
    cov.dft_lv_scanmode_0 |= !sim.dut->dft_lv_scanmode_i;
    cov.dft_lv_scanmode_1 |= sim.dut->dft_lv_scanmode_i;
    cov.dft_lv_scan_reset_0 |= !sim.dut->dft_lv_scan_reset_ni;
    cov.dft_lv_scan_reset_1 |= sim.dut->dft_lv_scan_reset_ni;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("ready_o", sim.dut->orig_ready_o, sim.dut->new_ready_o);
    sim.compare("valid_o", sim.dut->orig_valid_o, sim.dut->new_valid_o);
    sim.compare("rdata_o", sim.dut->orig_rdata_o, sim.dut->new_rdata_o);
}

static void eval_step(CosimCtrl<DUT>& sim, InputCoverage& cov, bool compare = true) {
    sim.dut->eval();
    record_inputs(sim, cov);
    if (compare) compare_outputs(sim);
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
        std::exit(2);
    }
}

static void set_wr_clk(CosimCtrl<DUT>& sim, InputCoverage& cov, uint8_t value, bool compare = true) {
    sim.dut->clk_wr_i = value;
    eval_step(sim, cov, compare);
}

static void set_rd_clk(CosimCtrl<DUT>& sim, InputCoverage& cov, uint8_t value, bool compare = true) {
    sim.dut->clk_rd_i = value;
    eval_step(sim, cov, compare);
}

static void wr_tick(CosimCtrl<DUT>& sim, InputCoverage& cov, bool compare = true) {
    set_wr_clk(sim, cov, 0, compare);
    set_wr_clk(sim, cov, 1, compare);
}

static void rd_tick(CosimCtrl<DUT>& sim, InputCoverage& cov, bool compare = true) {
    set_rd_clk(sim, cov, 0, compare);
    set_rd_clk(sim, cov, 1, compare);
}

static void tick_both(CosimCtrl<DUT>& sim, InputCoverage& cov, bool compare = true) {
    sim.dut->clk_wr_i = 0;
    sim.dut->clk_rd_i = 0;
    eval_step(sim, cov, compare);
    sim.dut->clk_wr_i = 1;
    sim.dut->clk_rd_i = 1;
    eval_step(sim, cov, compare);
}

static void wait_wr(CosimCtrl<DUT>& sim, InputCoverage& cov, int cycles, bool compare = true) {
    for (int i = 0; i < cycles; ++i) wr_tick(sim, cov, compare);
}

static void wait_rd(CosimCtrl<DUT>& sim, InputCoverage& cov, int cycles, bool compare = true) {
    for (int i = 0; i < cycles; ++i) rd_tick(sim, cov, compare);
}

static void wait_both(CosimCtrl<DUT>& sim, InputCoverage& cov, int cycles, bool compare = true) {
    for (int i = 0; i < cycles; ++i) tick_both(sim, cov, compare);
}

static void release_and_settle(CosimCtrl<DUT>& sim, InputCoverage& cov, bool compare = true) {
    sim.dut->rst_wr_ni = 1;
    sim.dut->rst_rd_ni = 1;
    wait_both(sim, cov, 8, compare);
}

static void reset_and_compare(CosimCtrl<DUT>& sim, InputCoverage& cov) {
    sim.dut->push_i = 0;
    sim.dut->pop_i = 0;
    sim.dut->rst_wr_ni = 0;
    sim.dut->rst_rd_ni = 0;
    wait_both(sim, cov, 4, true);
    release_and_settle(sim, cov, false);
    wait_both(sim, cov, 2, true);
}

static void push_byte(CosimCtrl<DUT>& sim, InputCoverage& cov, uint8_t data, bool compare = true) {
    sim.dut->wdata_i = data;
    sim.dut->push_i = 1;
    wr_tick(sim, cov, compare);
    sim.dut->push_i = 0;
    eval_step(sim, cov, compare);
}

static void pop_byte(CosimCtrl<DUT>& sim, InputCoverage& cov, bool compare = true) {
    sim.dut->pop_i = 1;
    rd_tick(sim, cov, compare);
    sim.dut->pop_i = 0;
    eval_step(sim, cov, compare);
}

static void prime_memory(CosimCtrl<DUT>& sim, InputCoverage& cov) {
    sim.dut->rst_wr_ni = 0;
    sim.dut->rst_rd_ni = 0;
    wait_both(sim, cov, 4, false);
    release_and_settle(sim, cov, false);
    push_byte(sim, cov, 0x00, false);
    wait_rd(sim, cov, 8, false);
    pop_byte(sim, cov, false);
    wait_both(sim, cov, 8, false);
}

static void dft_write_reset(CosimCtrl<DUT>& sim, InputCoverage& cov) {
    // HIV FIFO: write side is the HV domain.
    for (int i = 0; i < 8; ++i) push_byte(sim, cov, static_cast<uint8_t>(0x80 + i));
    wait_both(sim, cov, 4);
    sim.dut->dft_hv_scanmode_i = 1;
    sim.dut->dft_hv_scan_reset_ni = 0;
    wait_wr(sim, cov, 3);
    sim.dut->dft_hv_scan_reset_ni = 1;
    wait_wr(sim, cov, 2);
    sim.dut->dft_hv_scanmode_i = 0;
    wait_both(sim, cov, 8, false);
    wait_both(sim, cov, 2, true);
}

static void dft_read_reset(CosimCtrl<DUT>& sim, InputCoverage& cov) {
    // HIV FIFO: read side is the LV domain.
    push_byte(sim, cov, 0x3c);
    wait_rd(sim, cov, 8);
    sim.dut->dft_lv_scanmode_i = 1;
    sim.dut->dft_lv_scan_reset_ni = 0;
    wait_rd(sim, cov, 3);
    sim.dut->dft_lv_scan_reset_ni = 1;
    wait_rd(sim, cov, 2);
    sim.dut->dft_lv_scanmode_i = 0;
    wait_both(sim, cov, 8, false);
    wait_both(sim, cov, 2, true);
}

static uint32_t lcg_next(uint32_t& state) {
    state = state * 1664525u + 1013904223u;
    return state;
}

static void random_slow_traffic(CosimCtrl<DUT>& sim, InputCoverage& cov) {
    uint32_t rng = 0x13579bdfu;
    int model_level = 0;
    for (int i = 0; i < 700; ++i) {
        uint32_t r = lcg_next(rng);
        if ((r & 0x3u) == 0 && model_level < 6) {
            push_byte(sim, cov, static_cast<uint8_t>(r >> 8));
            model_level++;
            wait_rd(sim, cov, 4);
        } else if ((r & 0x3u) == 1 && model_level > 0) {
            wait_rd(sim, cov, 4);
            pop_byte(sim, cov);
            model_level--;
            wait_wr(sim, cov, 4);
        } else if ((r & 0x1u) == 0) {
            wr_tick(sim, cov);
        } else {
            rd_tick(sim, cov);
        }
    }
    wait_rd(sim, cov, 8);
    while (model_level > 0) {
        pop_byte(sim, cov);
        model_level--;
    }
    wait_both(sim, cov, 8);
}

static void check_input_coverage(CosimCtrl<DUT>& sim, const InputCoverage& cov) {
    sim.check(cov.clk_wr_0 && cov.clk_wr_1, "clk_wr_i toggled both ways");
    sim.check(cov.clk_rd_0 && cov.clk_rd_1, "clk_rd_i toggled both ways");
    sim.check(cov.rst_wr_0 && cov.rst_wr_1, "rst_wr_ni exercised 0 and 1");
    sim.check(cov.rst_rd_0 && cov.rst_rd_1, "rst_rd_ni exercised 0 and 1");
    sim.check(cov.push_0 && cov.push_1, "push_i exercised 0 and 1");
    sim.check(cov.pop_0 && cov.pop_1, "pop_i exercised 0 and 1");
    sim.check(cov.data_0 && cov.data_1, "wdata_i exercised zero and nonzero");
    sim.check(cov.dft_hv_scanmode_0 && cov.dft_hv_scanmode_1,
              "dft_hv_i.scanmode exercised 0 and 1");
    sim.check(cov.dft_hv_scan_reset_0 && cov.dft_hv_scan_reset_1,
              "dft_hv_i.scan_reset_n exercised 0 and 1");
    sim.check(cov.dft_lv_scanmode_0 && cov.dft_lv_scanmode_1,
              "dft_lv_i.scanmode exercised 0 and 1");
    sim.check(cov.dft_lv_scan_reset_0 && cov.dft_lv_scan_reset_1,
              "dft_lv_i.scan_reset_n exercised 0 and 1");
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    InputCoverage cov;
    sim.max_time = 300000;

    sim.dut->clk_wr_i = 0;
    sim.dut->clk_rd_i = 0;
    sim.dut->rst_wr_ni = 1;
    sim.dut->rst_rd_ni = 1;
    sim.dut->push_i = 0;
    sim.dut->pop_i = 0;
    sim.dut->wdata_i = 0;
    sim.dut->dft_hv_scanmode_i = 0;
    sim.dut->dft_hv_scan_reset_ni = 1;
    sim.dut->dft_lv_scanmode_i = 0;
    sim.dut->dft_lv_scan_reset_ni = 1;
    eval_step(sim, cov, false);

    prime_memory(sim, cov);
    reset_and_compare(sim, cov);

    push_byte(sim, cov, 0x11);
    wait_rd(sim, cov, 8);
    pop_byte(sim, cov);
    wait_both(sim, cov, 8);

    for (int i = 0; i < 8; ++i) push_byte(sim, cov, static_cast<uint8_t>(0x20 + i));
    wait_rd(sim, cov, 8);
    for (int i = 0; i < 8; ++i) pop_byte(sim, cov);
    wait_both(sim, cov, 8);

    for (int i = 0; i < 4; ++i) push_byte(sim, cov, static_cast<uint8_t>(0x40 + i));
    wait_rd(sim, cov, 8);
    for (int i = 0; i < 2; ++i) pop_byte(sim, cov);
    wait_wr(sim, cov, 8);
    for (int i = 0; i < 3; ++i) push_byte(sim, cov, static_cast<uint8_t>(0x50 + i));
    wait_rd(sim, cov, 8);
    for (int i = 0; i < 5; ++i) pop_byte(sim, cov);
    wait_both(sim, cov, 8);

    dft_write_reset(sim, cov);
    reset_and_compare(sim, cov);
    dft_read_reset(sim, cov);
    reset_and_compare(sim, cov);
    random_slow_traffic(sim, cov);

    check_input_coverage(sim, cov);
    return sim.finish();
}
