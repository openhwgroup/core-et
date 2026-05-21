// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_pll_wrapper_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

uint32_t xs(uint32_t& s) { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }
void clear_w(WData* w) { for (int i = 0; i < 8; ++i) w[i] = 0; }
void set_reg(WData* w, int idx, uint16_t val) {
    const int bit = idx * 16;
    w[bit >> 5] &= ~(0xffffu << (bit & 31));
    w[bit >> 5] |= uint32_t(val) << (bit & 31);
}
uint32_t pll_cfg(bool reset, bool enable, bool run, bool write, uint32_t first,
                 uint32_t num, uint32_t pclk_sel) {
    return (reset ? 1u : 0u) | (enable ? 2u : 0u) | (run ? 4u : 0u) |
           (write ? 8u : 0u) | ((first & 0x3fu) << 4) | ((num & 0xfu) << 10) |
           ((pclk_sel & 0x3u) << 15);
}
uint32_t dll_cfg(bool reset, bool enable, bool run, bool write, uint32_t first,
                 uint32_t num, uint32_t pclk_sel, bool dll_enable) {
    return (reset ? 1u : 0u) | (enable ? 2u : 0u) | (run ? 4u : 0u) |
           (write ? 8u : 0u) | ((first & 0x7u) << 4) | ((num & 0x7u) << 7) |
           ((pclk_sel & 0x3u) << 11) | (dll_enable ? (1u << 13) : 0u);
}

void idle(CosimCtrl<Vcosim_shire_pll_wrapper_tb>& sim) {
    auto* d = sim.dut.get();
    d->clk_sel_bits_i = 8;  // Original shire output is inverted while neigh uses DLL/reference.
    d->clk_gate_neigh_disable_i = 0;
    d->clk_gate_debug_disable_i = 0;
    d->pll_cfg_bits_i = pll_cfg(false, true, false, false, 0, 0, 0);
    d->dll_cfg_bits_i = dll_cfg(false, true, false, false, 0, 0, 0, true);
    clear_w(d->pll_wdata_i);
    d->dll_wdata_i = 0;
}

void compare_all(CosimCtrl<Vcosim_shire_pll_wrapper_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("clk_neigh", d->orig_clk_neigh_o, d->new_clk_neigh_o);
    sim.compare("clk_shire", d->orig_clk_shire_o, d->new_clk_shire_o);
    sim.compare("clk_debug", d->orig_clk_debug_o, d->new_clk_debug_o);
    sim.compare("pll_lock", d->orig_pll_lock_o, d->new_pll_lock_o);
    sim.compare("dll_lock", d->orig_dll_lock_o, d->new_dll_lock_o);
    sim.compare("pll_busy", d->orig_pll_busy_o, d->new_pll_busy_o);
    sim.compare("pll_rrdy", d->orig_pll_rrdy_o, d->new_pll_rrdy_o);
    sim.compare("pll_rdata", d->orig_pll_rdata_o, d->new_pll_rdata_o);
    sim.compare("dll_busy", d->orig_dll_busy_o, d->new_dll_busy_o);
    sim.compare("dll_rrdy", d->orig_dll_rrdy_o, d->new_dll_rrdy_o);
    sim.compare("dll_rdata", d->orig_dll_rdata_o, d->new_dll_rdata_o);
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_pll_wrapper_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    idle(sim);
    sim.reset();
    for (int i = 0; i < 16; ++i) sim.tick();

    uint32_t seed = 0x10203040u;
    for (int tx = 0; tx < 360; ++tx) {
        // Exercise clock-gate inputs, then allow the original/new synchronizers to settle.
        if ((tx % 40) == 0) {
            d->clk_gate_neigh_disable_i = (tx / 40) & 0xfu;
            d->clk_gate_debug_disable_i = (tx / 80) & 1u;
            for (int s = 0; s < 8; ++s) sim.tick();
        }

        clear_w(d->pll_wdata_i);
        for (int r = 0; r < 16; ++r) set_reg(d->pll_wdata_i, r, xs(seed) & 0xffffu);
        d->dll_wdata_i = xs(seed) & 0xffffu;

        const bool write = (tx & 1) != 0;
        d->pll_cfg_bits_i = pll_cfg(false, true, true, write, xs(seed) & 0x3fu,
                                    xs(seed) & 0xfu, tx & 3u);
        d->dll_cfg_bits_i = dll_cfg(false, true, true, write, xs(seed) & 0x7u,
                                    xs(seed) & 0x7u, tx & 3u, (tx & 2) != 0);
        sim.tick();
        compare_all(sim);

        d->pll_cfg_bits_i &= ~4u;
        d->dll_cfg_bits_i &= ~4u;
        for (int cyc = 0; cyc < 10; ++cyc) {
            sim.tick();
            compare_all(sim);
        }
    }
    return sim.finish();
}
