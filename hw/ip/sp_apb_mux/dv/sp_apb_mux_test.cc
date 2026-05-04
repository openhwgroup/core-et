// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vsp_apb_mux_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <cstdio>

using DUT = Vsp_apb_mux_tb;

static void clear_slave_inputs(DUT* dut) {
    dut->esr_apb_prdata    = 0;
    dut->esr_apb_pready    = 0;
    dut->esr_apb_pslverr   = 0;
    dut->icache_apb_prdata = 0;
    dut->icache_apb_pready = 0;
    dut->icache_apb_pslverr = 0;
    dut->minion_apb_prdata = 0;
    dut->minion_apb_pready = 0;
    dut->minion_apb_pslverr = 0;
    dut->ic_data_apb_prdata = 0;
    dut->ic_data_apb_pready = 0;
    dut->ic_data_apb_pslverr = 0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();

    dut->clk_i = 0;
    dut->rst_ni = 1;

    auto eval = [&]() {
        dut->eval();
    };

    auto clear_master = [&]() {
        dut->apb_paddr   = 0;
        dut->apb_penable = 0;
        dut->apb_psel    = 0;
        dut->apb_pwdata  = 0;
        dut->apb_pwrite  = 0;
    };

    clear_master();
    clear_slave_inputs(dut);
    eval();

    printf("test 1: invalid decode returns slave error\n");
    dut->apb_paddr   = 0xE000;
    dut->apb_penable = 1;
    dut->apb_psel    = 1;
    eval();
    sim.check(dut->apb_prdata == 0, "invalid decode prdata is zero");
    sim.check(dut->apb_pready == 1, "invalid decode pready is one");
    sim.check(dut->apb_pslverr == 1, "invalid decode sets pslverr");
    sim.check(dut->minion_apb_psel == 0 && dut->esr_apb_psel == 0 &&
              dut->icache_apb_psel == 0 && dut->ic_data_apb_psel == 0,
              "invalid decode selects no target");

    printf("test 2: minion register region decode and address rewrite\n");
    clear_master();
    clear_slave_inputs(dut);
    dut->apb_paddr        = 0x0845;
    dut->apb_penable      = 1;
    dut->apb_psel         = 1;
    dut->apb_pwdata       = 0x1122334455667788ULL;
    dut->apb_pwrite       = 1;
    dut->minion_apb_pready  = 1;
    dut->minion_apb_prdata  = 0x0102030405060708ULL;
    dut->minion_apb_pslverr = 0;
    eval();
    sim.check(dut->minion_apb_psel == 1, "minion region drives minion psel");
    sim.check(dut->minion_apb_penable == 1, "minion region drives minion penable");
    sim.check(dut->minion_apb_paddr == 0x445, "minion register address rewrite matches original");
    sim.check(dut->apb_prdata == 0x0102030405060708ULL, "minion region returns minion prdata");
    sim.check(dut->apb_pready == 1, "minion region forwards pready");
    sim.check(dut->apb_pslverr == 0, "minion region forwards pslverr");

    printf("test 3: invalid minion register subencoding errors\n");
    clear_slave_inputs(dut);
    dut->apb_paddr   = 0x0600;
    dut->apb_penable = 1;
    dut->apb_psel    = 1;
    eval();
    sim.check(dut->apb_pready == 1, "bad minion reg encoding pready");
    sim.check(dut->apb_pslverr == 1, "bad minion reg encoding pslverr");
    sim.check(dut->minion_apb_psel == 0, "bad minion reg encoding does not select minion");

    printf("test 4: minion dcache region decode and address rewrite\n");
    clear_slave_inputs(dut);
    dut->apb_paddr         = 0x8005;
    dut->apb_penable       = 1;
    dut->apb_psel          = 1;
    dut->minion_apb_pready = 1;
    dut->minion_apb_prdata = 0xAA55AA55AA55AA55ULL;
    eval();
    sim.check(dut->minion_apb_paddr == 0x1005, "dcache region address rewrite matches original");
    sim.check(dut->apb_prdata == 0xAA55AA55AA55AA55ULL, "dcache region returns minion prdata");

    printf("test 5: ESR region decode\n");
    clear_slave_inputs(dut);
    dut->apb_paddr       = 0x2803;
    dut->apb_penable     = 1;
    dut->apb_psel        = 1;
    dut->esr_apb_pready  = 1;
    dut->esr_apb_prdata  = 0x123456789ABCDEF0ULL;
    dut->esr_apb_pslverr = 0;
    eval();
    sim.check(dut->esr_apb_psel == 1, "ESR region drives ESR psel");
    sim.check(dut->esr_apb_paddr == 0x7, "ESR address rewrite matches original");
    sim.check(dut->apb_prdata == 0x123456789ABCDEF0ULL, "ESR region returns ESR prdata");

    printf("test 6: invalid ESR subencoding errors\n");
    clear_slave_inputs(dut);
    dut->apb_paddr   = 0x2404;
    dut->apb_penable = 1;
    dut->apb_psel    = 1;
    eval();
    sim.check(dut->apb_pslverr == 1, "bad ESR encoding sets pslverr");
    sim.check(dut->esr_apb_psel == 0, "bad ESR encoding does not select ESR");

    printf("test 7: icache region decode\n");
    clear_slave_inputs(dut);
    dut->apb_paddr          = 0xA155;
    dut->apb_penable        = 1;
    dut->apb_psel           = 1;
    dut->icache_apb_pready  = 1;
    dut->icache_apb_prdata  = 0xCAFEBABECAFED00DULL;
    dut->icache_apb_pslverr = 0;
    eval();
    sim.check(dut->icache_apb_psel == 1, "icache region drives icache psel");
    sim.check(dut->icache_apb_paddr == 0x155, "icache address mapping matches original");
    sim.check(dut->apb_prdata == 0xCAFEBABECAFED00DULL, "icache region returns icache prdata");

    printf("test 8: icache data region decode\n");
    clear_slave_inputs(dut);
    dut->apb_paddr           = 0xD321;
    dut->apb_penable         = 1;
    dut->apb_psel            = 1;
    dut->ic_data_apb_pready  = 1;
    dut->ic_data_apb_prdata  = 0x0F0E0D0C0B0A0908ULL;
    dut->ic_data_apb_pslverr = 0;
    eval();
    sim.check(dut->ic_data_apb_psel == 1, "icache data region drives data psel");
    sim.check(dut->ic_data_apb_paddr == 0x1321, "icache data address mapping matches original");
    sim.check(dut->apb_prdata == 0x0F0E0D0C0B0A0908ULL, "icache data region returns data prdata");

    printf("test 9: pwrite and pwdata broadcast to every target\n");
    clear_slave_inputs(dut);
    dut->apb_pwdata = 0xFEEDFACE01234567ULL;
    dut->apb_pwrite = 1;
    eval();
    sim.check(dut->minion_apb_pwrite == 1 && dut->esr_apb_pwrite == 1 &&
              dut->icache_apb_pwrite == 1 && dut->ic_data_apb_pwrite == 1,
              "pwrite broadcast");
    sim.check(dut->minion_apb_pwdata == 0xFEEDFACE01234567ULL &&
              dut->esr_apb_pwdata == 0xFEEDFACE01234567ULL &&
              dut->icache_apb_pwdata == 0xFEEDFACE01234567ULL &&
              dut->ic_data_apb_pwdata == 0xFEEDFACE01234567ULL,
              "pwdata broadcast");

    return sim.finish();
}
