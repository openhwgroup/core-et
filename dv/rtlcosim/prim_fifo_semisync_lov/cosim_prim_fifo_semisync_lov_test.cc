// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Standalone RTL cosim for prim_fifo_semisync_lov against vcfifo_wr_lov_ss.

#include "Vcosim_prim_fifo_semisync_lov_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdlib>

using DUT = Vcosim_prim_fifo_semisync_lov_tb;

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

static void tick_both(CosimCtrl<DUT>& sim, InputCoverage& cov, bool compare = true) {
    sim.dut->clk_wr_i = 0;
    sim.dut->clk_rd_i = 0;
    eval_step(sim, cov, compare);
    sim.dut->clk_wr_i = 1;
    sim.dut->clk_rd_i = 1;
    eval_step(sim, cov, compare);
}

static void wait_both(CosimCtrl<DUT>& sim, InputCoverage& cov, int cycles,
                      bool compare = true) {
    for (int i = 0; i < cycles; ++i) tick_both(sim, cov, compare);
}

static void functional_reset(CosimCtrl<DUT>& sim, InputCoverage& cov) {
    sim.dut->push_i = 0;
    sim.dut->pop_i = 0;
    sim.dut->rst_wr_ni = 0;
    sim.dut->rst_rd_ni = 0;
    wait_both(sim, cov, 4);
    sim.dut->rst_wr_ni = 1;
    sim.dut->rst_rd_ni = 1;
    wait_both(sim, cov, 4);
}

static void push_burst(CosimCtrl<DUT>& sim, InputCoverage& cov, uint8_t base, int count) {
    for (int i = 0; i < count; ++i) {
        sim.dut->wdata_i = static_cast<uint8_t>(base + i);
        sim.dut->push_i = 1;
        tick_both(sim, cov);
    }
    sim.dut->push_i = 0;
    tick_both(sim, cov);
}

static void pop_count(CosimCtrl<DUT>& sim, InputCoverage& cov, int count) {
    for (int i = 0; i < count; ++i) {
        sim.dut->pop_i = 1;
        tick_both(sim, cov);
    }
    sim.dut->pop_i = 0;
    tick_both(sim, cov);
}

static void dft_write_reset(CosimCtrl<DUT>& sim, InputCoverage& cov) {
    // LOV FIFO: write side is the LV domain.
    push_burst(sim, cov, 0x80, 3);
    sim.dut->dft_lv_scanmode_i = 1;
    sim.dut->dft_lv_scan_reset_ni = 0;
    wait_both(sim, cov, 2);
    sim.dut->dft_lv_scan_reset_ni = 1;
    wait_both(sim, cov, 2);
    sim.dut->dft_lv_scanmode_i = 0;
    wait_both(sim, cov, 2);
}

static void dft_read_reset(CosimCtrl<DUT>& sim, InputCoverage& cov) {
    // LOV FIFO: read side is the HV domain.
    push_burst(sim, cov, 0x44, 1);
    sim.dut->dft_hv_scanmode_i = 1;
    sim.dut->dft_hv_scan_reset_ni = 0;
    wait_both(sim, cov, 2);
    sim.dut->dft_hv_scan_reset_ni = 1;
    wait_both(sim, cov, 2);
    sim.dut->dft_hv_scanmode_i = 0;
    wait_both(sim, cov, 2);
}

static uint32_t lcg_next(uint32_t& state) {
    state = state * 1664525u + 1013904223u;
    return state;
}

static void random_traffic(CosimCtrl<DUT>& sim, InputCoverage& cov) {
    uint32_t rng = 0x51f00d42u;
    for (int i = 0; i < 1200; ++i) {
        uint32_t r = lcg_next(rng);
        sim.dut->push_i = ((r & 0x3u) != 0) && sim.dut->new_ready_o;
        sim.dut->pop_i = (((r >> 3) & 0x3u) != 0) && sim.dut->new_valid_o;
        sim.dut->wdata_i = static_cast<uint8_t>(r >> 8);
        tick_both(sim, cov);
    }
    sim.dut->push_i = 0;
    sim.dut->pop_i = 1;
    for (int i = 0; i < 8; ++i) tick_both(sim, cov);
    sim.dut->pop_i = 0;
    wait_both(sim, cov, 4);
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
    sim.max_time = 500000;

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

    functional_reset(sim, cov);
    push_burst(sim, cov, 0x10, 1);
    pop_count(sim, cov, 1);
    push_burst(sim, cov, 0x20, 3);
    pop_count(sim, cov, 3);
    push_burst(sim, cov, 0x40, 2);
    pop_count(sim, cov, 1);
    push_burst(sim, cov, 0x50, 2);
    pop_count(sim, cov, 3);

    dft_write_reset(sim, cov);
    functional_reset(sim, cov);
    dft_read_reset(sim, cov);
    functional_reset(sim, cov);
    random_traffic(sim, cov);

    check_input_coverage(sim, cov);
    return sim.finish();
}
