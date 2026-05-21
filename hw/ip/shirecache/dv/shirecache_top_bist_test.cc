// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vshirecache_top_bist_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <cstdio>

int main(int argc, char** argv) {
    SimCtrl<Vshirecache_top_bist_tb> sim(argc, argv);
    auto* dut = sim.dut.get();
    sim.max_time = 2000;

    dut->dft_mbist_en_i = 0;
    sim.reset(5);

    sim.check(dut->icache_bist_mbist_on_o == 0,
              "icache BIST mbist_on starts low after reset");
    sim.check(dut->icache_bist_mbi_sel_o == 0,
              "icache BIST mbi_sel is tied off when shared MBIST is idle");
    sim.check(dut->icache_bist_mbi_rd_en_o == 0,
              "icache BIST mbi_rd_en is tied off when shared MBIST is idle");
    sim.check(dut->icache_bist_mbi_wr_en_o == 0,
              "icache BIST mbi_wr_en is tied off when shared MBIST is idle");
    sim.check(dut->icache_bist_mbi_addr_o == 0,
              "icache BIST mbi_addr is tied off when shared MBIST is idle");
    sim.check(static_cast<uint64_t>(dut->icache_bist_mbi_wdata_lo_o) == 0,
              "icache BIST mbi_wdata[63:0] is tied off when shared MBIST is idle");
    sim.check(static_cast<uint64_t>(dut->icache_bist_mbi_wdata_mid_o) == 0,
              "icache BIST mbi_wdata[127:64] is tied off when shared MBIST is idle");
    sim.check(static_cast<uint32_t>(dut->icache_bist_mbi_wdata_hi_o) == 0,
              "icache BIST mbi_wdata[143:128] is tied off when shared MBIST is idle");

    printf("Assert top-level dft_mbist_en_i\n");
    dut->dft_mbist_en_i = 1;
    bool saw_mbist_on = false;
    for (int i = 0; i < 8; i++) {
        sim.tick();
        saw_mbist_on |= dut->icache_bist_mbist_on_o;
    }
    sim.check(saw_mbist_on,
              "dft_mbist_en_i propagates through shirecache_top to icache mbist_on");

    printf("Deassert top-level dft_mbist_en_i\n");
    dut->dft_mbist_en_i = 0;
    bool saw_mbist_off = false;
    for (int i = 0; i < 8; i++) {
        sim.tick();
        saw_mbist_off |= !dut->icache_bist_mbist_on_o;
    }
    sim.check(saw_mbist_off,
              "icache mbist_on drops after dft_mbist_en_i deasserts");

    return sim.finish();
}
