// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_xll_control_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

uint32_t xs(uint32_t& s) { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }
void clear_w(WData* w) { for (int i = 0; i < 8; ++i) w[i] = 0; }
void set_reg(WData* w, int idx, uint16_t val) {
    const int bit = idx * 16;
    w[bit >> 5] &= ~(0xffffu << (bit & 31));
    w[bit >> 5] |= uint32_t(val) << (bit & 31);
}

void compare_all(CosimCtrl<Vcosim_shire_xll_control_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("busy", d->orig_busy_o, d->new_busy_o);
    sim.compare("rrdy", d->orig_rrdy_o, d->new_rrdy_o);
    sim.compare("rdata", d->orig_rdata_o, d->new_rdata_o);
    sim.compare("pclk", d->orig_pclk_o, d->new_pclk_o);
    sim.compare("preset_n", d->orig_preset_no, d->new_preset_no);
    sim.compare("paddr", d->orig_paddr_o, d->new_paddr_o);
    sim.compare("psel", d->orig_psel_o, d->new_psel_o);
    sim.compare("penable", d->orig_penable_o, d->new_penable_o);
    sim.compare("pwrite", d->orig_pwrite_o, d->new_pwrite_o);
    sim.compare("pwdata", d->orig_pwdata_o, d->new_pwdata_o);
    sim.compare("pstrb", d->orig_pstrb_o, d->new_pstrb_o);
}

void idle(CosimCtrl<Vcosim_shire_xll_control_tb>& sim) {
    auto* d = sim.dut.get();
    d->esr_enable_i = 1;
    d->esr_reset_i = 0;
    d->esr_run_i = 0;
    d->esr_write_i = 0;
    d->esr_reg_first_i = 0;
    d->esr_reg_num_i = 0;
    d->esr_pclk_sel_i = 0;
    d->prdata_i = 0;
    d->pready_i = 1;
    clear_w(d->esr_wdata_i);
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_xll_control_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    idle(sim);
    sim.reset();

    uint32_t seed = 0x2468ace0u;
    for (int tx = 0; tx < 260; ++tx) {
        // Idle phase also toggles reset/enable and APB clock select inputs.
        d->esr_enable_i = (tx & 3) != 0;
        d->esr_reset_i = (tx & 31) == 3;
        d->esr_pclk_sel_i = tx & 3;
        d->pready_i = 1;
        sim.tick();
        compare_all(sim);

        const bool write = (tx & 1) != 0;
        clear_w(d->esr_wdata_i);
        for (int r = 0; r < 16; ++r) set_reg(d->esr_wdata_i, r, xs(seed) & 0xffffu);
        d->esr_enable_i = 1;
        d->esr_reset_i = 0;
        d->esr_write_i = write;
        d->esr_run_i = 1;
        d->esr_reg_first_i = xs(seed) & 0x3fu;
        d->esr_reg_num_i = xs(seed) & 0xfu;
        d->prdata_i = xs(seed) & 0xffffu;
        d->pready_i = (tx & 7) != 2;

        for (int cyc = 0; cyc < 10; ++cyc) {
            if (cyc == 0) d->esr_run_i = 1;
            else d->esr_run_i = 0;
            if (cyc == 3) d->pready_i = 0;
            if (cyc == 4) d->pready_i = 1;
            d->prdata_i = xs(seed) & 0xffffu;
            sim.tick();
            compare_all(sim);
        }
    }
    return sim.finish();
}
