// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_sp_apb_mux_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_sp_apb_mux_tb;

static uint32_t rng_state = 0x1badf00dU;

static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static uint64_t xorshift64() {
    return (uint64_t(xorshift32()) << 32) | xorshift32();
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* dut = sim.dut.get();
    sim.compare("apb_prdata", dut->orig_apb_prdata, dut->new_apb_prdata);
    sim.compare("apb_pready", dut->orig_apb_pready, dut->new_apb_pready);
    sim.compare("apb_pslverr", dut->orig_apb_pslverr, dut->new_apb_pslverr);
    sim.compare("esr_apb_paddr", dut->orig_esr_apb_paddr, dut->new_esr_apb_paddr);
    sim.compare("esr_apb_penable", dut->orig_esr_apb_penable, dut->new_esr_apb_penable);
    sim.compare("esr_apb_psel", dut->orig_esr_apb_psel, dut->new_esr_apb_psel);
    sim.compare("esr_apb_pwdata", dut->orig_esr_apb_pwdata, dut->new_esr_apb_pwdata);
    sim.compare("esr_apb_pwrite", dut->orig_esr_apb_pwrite, dut->new_esr_apb_pwrite);
    sim.compare("icache_apb_paddr", dut->orig_icache_apb_paddr, dut->new_icache_apb_paddr);
    sim.compare("icache_apb_pwrite", dut->orig_icache_apb_pwrite, dut->new_icache_apb_pwrite);
    sim.compare("icache_apb_psel", dut->orig_icache_apb_psel, dut->new_icache_apb_psel);
    sim.compare("icache_apb_penable", dut->orig_icache_apb_penable, dut->new_icache_apb_penable);
    sim.compare("icache_apb_pwdata", dut->orig_icache_apb_pwdata, dut->new_icache_apb_pwdata);
    sim.compare("minion_apb_paddr", dut->orig_minion_apb_paddr, dut->new_minion_apb_paddr);
    sim.compare("minion_apb_penable", dut->orig_minion_apb_penable, dut->new_minion_apb_penable);
    sim.compare("minion_apb_psel", dut->orig_minion_apb_psel, dut->new_minion_apb_psel);
    sim.compare("minion_apb_pwdata", dut->orig_minion_apb_pwdata, dut->new_minion_apb_pwdata);
    sim.compare("minion_apb_pwrite", dut->orig_minion_apb_pwrite, dut->new_minion_apb_pwrite);
    sim.compare("ic_data_apb_paddr", dut->orig_ic_data_apb_paddr, dut->new_ic_data_apb_paddr);
    sim.compare("ic_data_apb_pwrite", dut->orig_ic_data_apb_pwrite, dut->new_ic_data_apb_pwrite);
    sim.compare("ic_data_apb_psel", dut->orig_ic_data_apb_psel, dut->new_ic_data_apb_psel);
    sim.compare("ic_data_apb_penable", dut->orig_ic_data_apb_penable, dut->new_ic_data_apb_penable);
    sim.compare("ic_data_apb_pwdata", dut->orig_ic_data_apb_pwdata, dut->new_ic_data_apb_pwdata);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();

    dut->clk_i = 0;
    dut->rst_ni = 1;

    printf("phase 1: directed decode cases\n");
    const uint16_t directed_addr[] = {
        0x0000, 0x0845, 0x0600, 0x8005, 0x9C00, 0x2803, 0x2404, 0xA155,
        0xBC00, 0xD321, 0xE000
    };

    for (uint16_t addr : directed_addr) {
        dut->apb_paddr = addr;
        dut->apb_penable = xorshift32() & 1;
        dut->apb_psel = 1;
        dut->apb_pwdata = xorshift64();
        dut->apb_pwrite = xorshift32() & 1;

        dut->esr_apb_prdata = xorshift64();
        dut->esr_apb_pready = xorshift32() & 1;
        dut->esr_apb_pslverr = xorshift32() & 1;
        dut->icache_apb_prdata = xorshift64();
        dut->icache_apb_pready = xorshift32() & 1;
        dut->icache_apb_pslverr = xorshift32() & 1;
        dut->minion_apb_prdata = xorshift64();
        dut->minion_apb_pready = xorshift32() & 1;
        dut->minion_apb_pslverr = xorshift32() & 1;
        dut->ic_data_apb_prdata = xorshift64();
        dut->ic_data_apb_pready = xorshift32() & 1;
        dut->ic_data_apb_pslverr = xorshift32() & 1;

        dut->eval();
        compare_outputs(sim);
        if (sim.mismatches > 20) break;
    }

    printf("phase 2: random stimulus (10000 vectors)\n");
    for (int i = 0; i < 10000; ++i) {
        dut->apb_paddr = xorshift32() & 0xFFFFu;
        dut->apb_penable = xorshift32() & 1;
        dut->apb_psel = xorshift32() & 1;
        dut->apb_pwdata = xorshift64();
        dut->apb_pwrite = xorshift32() & 1;

        dut->esr_apb_prdata = xorshift64();
        dut->esr_apb_pready = xorshift32() & 1;
        dut->esr_apb_pslverr = xorshift32() & 1;
        dut->icache_apb_prdata = xorshift64();
        dut->icache_apb_pready = xorshift32() & 1;
        dut->icache_apb_pslverr = xorshift32() & 1;
        dut->minion_apb_prdata = xorshift64();
        dut->minion_apb_pready = xorshift32() & 1;
        dut->minion_apb_pslverr = xorshift32() & 1;
        dut->ic_data_apb_prdata = xorshift64();
        dut->ic_data_apb_pready = xorshift32() & 1;
        dut->ic_data_apb_pslverr = xorshift32() & 1;

        dut->eval();
        compare_outputs(sim);
        if (sim.mismatches > 20) break;
    }

    return sim.finish();
}
