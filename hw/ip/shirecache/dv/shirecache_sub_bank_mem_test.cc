// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_sub_bank_mem — Verilator test.
// Tests write→read through all three RAM types with variable delay.
// The _tap→_ta input pipeline adds 1 cycle before the RAM, plus
// sram_clock_pre adds another. Total read latency from rd_en assertion:
// 1 (tap→ta) + 1 (sram_clock_pre) + ram_delay (response pipeline).

#include "Vshirecache_pipe_sub_bank_mem.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_pipe_sub_bank_mem;

static void clear_inputs(SimCtrl<DUT>& sim) {
    sim.dut->ts_rd_en_i = 0; sim.dut->ts_wr_en_i = 0;
    sim.dut->t_rd_en_i  = 0; sim.dut->t_wr_en_i  = 0;
    sim.dut->d_rd_en_i  = 0; sim.dut->d_wr_en_i  = 0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_inputs(sim);
    sim.dut->esr_ram_delay_i  = 2;
    sim.dut->dft_sram_clk_i   = 0;
    sim.dut->ts_rd_addr_i = 0; sim.dut->ts_wr_addr_i = 0;
    sim.dut->t_addr_i = 0;
    sim.dut->d_addr_i = 0; sim.dut->d_wr_qwen_i = 0;
    sim.dut->ts_rd_trans_id_i = 0; sim.dut->ts_rd_reqq_id_i = 0;
    sim.dut->t_rd_trans_id_i = 0; sim.dut->t_rd_reqq_id_i = 0;
    sim.dut->d_rd_trans_id_i = 0; sim.dut->d_rd_reqq_id_i = 0;
    sim.dut->ts_wr_data_i = 0;
    for (int i = 0; i < 4; i++) sim.dut->t_wr_data_i[i] = 0;
    for (int i = 0; i < 18; i++) sim.dut->d_wr_data_i[i] = 0;
    sim.dut->ram_cfg_i = 0;
    sim.dut->dft_i = 0;
    sim.reset();

    // ── Test 1: Tag RAM write/read ──────────────────────
    printf("test 1: tag RAM write/read (delay=2)\n");
    sim.dut->esr_ram_delay_i = 2;

    // Write
    sim.dut->t_wr_en_i = 1;
    sim.dut->t_addr_i = 5;
    sim.dut->t_wr_data_i[0] = 0xFACE0001;
    sim.tick();
    clear_inputs(sim);
    // Let write propagate: _tap→_ta (1 clk) + sram_clock_pre (1 clk) + RAM write
    for (int i = 0; i < 4; i++) sim.tick();

    // Read
    sim.dut->t_rd_en_i = 1;
    sim.dut->t_addr_i = 5;
    sim.dut->t_rd_trans_id_i = 42;
    sim.dut->t_rd_reqq_id_i = 7;
    sim.tick();
    clear_inputs(sim);

    // Wait for response — poll for valid
    bool got_rsp = false;
    for (int i = 0; i < 10; i++) {
        sim.tick();
        if (sim.dut->t_rsp_valid_o) { got_rsp = true; break; }
    }
    sim.check(got_rsp, "tag rsp valid");
    if (got_rsp) {
        sim.check(sim.dut->t_rsp_data_o[0] == 0xFACE0001, "tag data matches");
        sim.check(sim.dut->t_rsp_reqq_id_o == 7, "tag reqq_id matches");
    }

    // ── Test 2: Tag state RAM write/read ────────────────
    printf("test 2: tag state RAM write/read\n");
    sim.dut->ts_wr_en_i = 1;
    sim.dut->ts_wr_addr_i = 3;
    sim.dut->ts_wr_data_i = 0xABCDE;
    sim.tick();
    clear_inputs(sim);
    for (int i = 0; i < 4; i++) sim.tick();

    sim.dut->ts_rd_en_i = 1;
    sim.dut->ts_rd_addr_i = 3;
    sim.dut->ts_rd_trans_id_i = 10;
    sim.dut->ts_rd_reqq_id_i = 2;
    sim.tick();
    clear_inputs(sim);

    got_rsp = false;
    for (int i = 0; i < 10; i++) {
        sim.tick();
        if (sim.dut->ts_rsp_valid_o) { got_rsp = true; break; }
    }
    sim.check(got_rsp, "tag_state rsp valid");
    if (got_rsp) {
        sim.check((sim.dut->ts_rsp_data_o & 0xFFFFF) == 0xABCDE, "tag_state data matches");
    }

    // ── Test 3: Data RAM write/read ─────────────────────
    printf("test 3: data RAM write/read\n");
    sim.dut->d_wr_en_i = 1;
    sim.dut->d_addr_i = 2;
    sim.dut->d_wr_qwen_i = 0xF;
    sim.dut->d_wr_data_i[0] = 0xDEAD0000;
    sim.tick();
    clear_inputs(sim);
    for (int i = 0; i < 4; i++) sim.tick();

    sim.dut->d_rd_en_i = 1;
    sim.dut->d_addr_i = 2;
    sim.dut->d_rd_trans_id_i = 55;
    sim.dut->d_rd_reqq_id_i = 4;
    sim.tick();
    clear_inputs(sim);

    got_rsp = false;
    for (int i = 0; i < 10; i++) {
        sim.tick();
        if (sim.dut->d_rsp_valid_o) { got_rsp = true; break; }
    }
    sim.check(got_rsp, "data rsp valid");
    if (got_rsp) {
        sim.check(sim.dut->d_rsp_data_o[0] == 0xDEAD0000, "data QW0 matches");
    }

    return sim.finish();
}
