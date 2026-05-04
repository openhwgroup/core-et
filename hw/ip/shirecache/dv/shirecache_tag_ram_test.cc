// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_tag_ram_wrap — Verilator test.
// SRAM clock is data-derived (sram_clock_pre): must toggle clock_en_i.

#include "Vshirecache_pipe_tag_ram_wrap.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_pipe_tag_ram_wrap;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.dut->clock_en_i = 0;
    sim.dut->dft_sram_clk_i = 0;
    sim.dut->ram_delay_i = 2;
    sim.dut->rd_en_i = 0;
    sim.dut->wr_en_i = 0;
    sim.dut->addr_i = 0;
    for (int i = 0; i < 4; i++) sim.dut->wr_data_i[i] = 0;
    sim.dut->ram_cfg_i = 0;
    sim.dut->dft_i = 0;
    sim.reset();

    printf("test 1: write and read\n");
    sim.dut->clock_en_i = 1; sim.dut->wr_en_i = 1; sim.dut->addr_i = 0;
    sim.dut->wr_data_i[0] = 0xCAFE1234; sim.dut->wr_data_i[1] = 0xDEADBEEF;
    sim.dut->wr_data_i[2] = 0x12345678; sim.dut->wr_data_i[3] = 0x000FFFFF;
    sim.tick(); sim.dut->wr_en_i = 0; sim.dut->clock_en_i = 0; sim.tick();
    sim.dut->clock_en_i = 1; sim.dut->rd_en_i = 1; sim.dut->addr_i = 0;
    sim.tick(); sim.dut->rd_en_i = 0; sim.dut->clock_en_i = 0; sim.tick();
    sim.check(sim.dut->rd_data_o[0] == 0xCAFE1234, "word0");
    sim.check(sim.dut->rd_data_o[1] == 0xDEADBEEF, "word1");
    sim.check(sim.dut->rd_data_o[2] == 0x12345678, "word2");

    printf("test 2: multiple addresses\n");
    for (int i = 0; i < 8; i++) {
        sim.dut->clock_en_i = 1; sim.dut->wr_en_i = 1; sim.dut->addr_i = i;
        sim.dut->wr_data_i[0] = 0xA000+i; sim.dut->wr_data_i[1]=0; sim.dut->wr_data_i[2]=0; sim.dut->wr_data_i[3]=0;
        sim.tick(); sim.dut->wr_en_i = 0; sim.dut->clock_en_i = 0; sim.tick();
    }
    for (int i = 0; i < 8; i++) {
        sim.dut->clock_en_i = 1; sim.dut->rd_en_i = 1; sim.dut->addr_i = i;
        sim.tick(); sim.dut->rd_en_i = 0; sim.dut->clock_en_i = 0; sim.tick();
        sim.check(sim.dut->rd_data_o[0] == (uint32_t)(0xA000+i), "addr["+std::to_string(i)+"]");
    }

    printf("test 3: output holds\n");
    uint32_t held = sim.dut->rd_data_o[0];
    sim.tick(); sim.tick();
    sim.check(sim.dut->rd_data_o[0] == held, "output holds");

    return sim.finish();
}
