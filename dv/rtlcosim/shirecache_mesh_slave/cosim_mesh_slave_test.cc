// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_mesh_slave vs shirecache_mesh_slave

#include "Vcosim_mesh_slave_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>

using DUT = Vcosim_mesh_slave_tb;

static constexpr uint8_t ReqRead  = 1;
static constexpr uint8_t ReqWrite = 0;

static void clear_inputs(DUT* d) {
    d->axi_ar_valid_i = 0;
    d->axi_ar_id_i    = 0;
    d->axi_ar_addr_i  = 0;
    d->axi_ar_len_i   = 0;
    d->axi_ar_size_i  = 6;
    d->axi_ar_burst_i = 1;
    d->axi_ar_lock_i  = 0;
    d->axi_ar_cache_i = 0;
    d->axi_ar_prot_i  = 0;
    d->axi_ar_qos_i   = 0;
    d->axi_ar_user_i  = 0;

    d->axi_aw_valid_i = 0;
    d->axi_aw_id_i    = 0;
    d->axi_aw_addr_i  = 0;
    d->axi_aw_len_i   = 0;
    d->axi_aw_size_i  = 6;
    d->axi_aw_burst_i = 1;
    d->axi_aw_lock_i  = 0;
    d->axi_aw_cache_i = 0;
    d->axi_aw_prot_i  = 0;
    d->axi_aw_qos_i   = 0;
    d->axi_aw_user_i  = 0;

    d->axi_w_valid_i  = 0;
    memset(&d->axi_w_data_i, 0, sizeof(d->axi_w_data_i));
    d->axi_w_strb_i   = 0;
    d->axi_w_last_i   = 1;

    d->axi_r_ready_i  = 1;
    d->axi_b_ready_i  = 1;

    d->bank_req_ready_i = 0xF;

    d->bank_rsp_valid_i = 0;
    d->rsp_trans_id_i   = 0;
    d->rsp_id_i         = 0;
    d->rsp_port_id_i    = 0;
    d->rsp_source_i     = 0;
    d->rsp_err_i        = 0;
    d->rsp_wdata_i      = 0;
    memset(&d->rsp_data_i, 0, sizeof(d->rsp_data_i));

    // Default swizzle: bank from addr bits [7:6]
    d->swz_all_shire_aliasing_i = 0;
    d->swz_two_shire_aliasing_i = 0;
    d->swz_sub_bank_sel_b2_i = 0;
    d->swz_sub_bank_sel_b1_i = 0;
    d->swz_sub_bank_sel_b0_i = 0;
    d->swz_bank_sel_b2_i     = 2;
    d->swz_bank_sel_b1_i     = 1;
    d->swz_bank_sel_b0_i     = 0;
    d->swz_shire_sel_b5_i    = 0;
    d->swz_shire_sel_b4_i    = 0;
    d->swz_shire_sel_b3_i    = 0;
    d->swz_shire_sel_b2_i    = 0;
    d->swz_shire_sel_b1_i    = 0;
    d->swz_shire_sel_b0_i    = 0;
}

static void compare_all(CosimCtrl<DUT>& sim, DUT* d) {
    sim.compare("bank_clk_en",     d->orig_bank_clk_en_o,    d->new_bank_clk_en_o);
    sim.compare("bank_req_valid",  d->orig_bank_req_valid_o, d->new_bank_req_valid_o);
    sim.compare("bank_rsp_ready",  d->orig_bank_rsp_ready_o, d->new_bank_rsp_ready_o);
    sim.compare("axi_ar_ready",    d->orig_axi_ar_ready_o,   d->new_axi_ar_ready_o);
    sim.compare("axi_aw_ready",    d->orig_axi_aw_ready_o,   d->new_axi_aw_ready_o);
    sim.compare("axi_w_ready",     d->orig_axi_w_ready_o,    d->new_axi_w_ready_o);
    sim.compare("axi_r_valid",     d->orig_axi_r_valid_o,    d->new_axi_r_valid_o);
    sim.compare("axi_b_valid",     d->orig_axi_b_valid_o,    d->new_axi_b_valid_o);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();
    sim.max_time = 800000;
    clear_inputs(dut);
    sim.reset(10);

    // ── Phase 1: Idle ───────────────────────────────────────
    printf("phase 1: idle\n");
    clear_inputs(dut);
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    // ── Phase 2: AR request ─────────────────────────────────
    printf("phase 2: AR request\n");
    clear_inputs(dut);

    uint32_t slave_id = (0x15 << 9) | 0x42;
    dut->axi_ar_valid_i = 1;
    dut->axi_ar_id_i    = slave_id;
    dut->axi_ar_addr_i  = 0x1000;  // bank 0
    dut->axi_ar_size_i  = 6;
    dut->axi_ar_qos_i   = 3;

    for (int i = 0; i < 20; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    dut->axi_ar_valid_i = 0;
    for (int i = 0; i < 20; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    // ── Phase 3: AW+W request ───────────────────────────────
    printf("phase 3: AW+W request\n");
    clear_inputs(dut);

    dut->axi_aw_valid_i = 1;
    dut->axi_aw_id_i    = slave_id;
    dut->axi_aw_addr_i  = 0x1000;
    dut->axi_aw_size_i  = 6;
    dut->axi_aw_qos_i   = 2;
    dut->axi_aw_user_i  = ReqWrite;

    dut->axi_w_valid_i  = 1;
    dut->axi_w_strb_i   = 0xFFFF;
    dut->axi_w_last_i   = 1;

    for (int i = 0; i < 20; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    dut->axi_aw_valid_i = 0;
    dut->axi_w_valid_i  = 0;
    for (int i = 0; i < 20; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    // ── Phase 4: R response ─────────────────────────────────
    printf("phase 4: R response\n");
    clear_inputs(dut);

    dut->bank_rsp_valid_i = 1;  // bank 0
    dut->rsp_id_i         = 0x23;
    dut->rsp_source_i     = 0x07;
    dut->rsp_port_id_i    = 0;
    dut->rsp_err_i        = 0;
    dut->rsp_wdata_i      = 1;  // R

    for (int i = 0; i < 30; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    dut->bank_rsp_valid_i = 0;
    for (int i = 0; i < 20; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    // ── Phase 5: B response ─────────────────────────────────
    printf("phase 5: B response\n");
    clear_inputs(dut);

    dut->bank_rsp_valid_i = 1;
    dut->rsp_id_i         = 0x44;
    dut->rsp_source_i     = 0x0A;
    dut->rsp_port_id_i    = 0;
    dut->rsp_err_i        = 1;
    dut->rsp_wdata_i      = 0;  // B

    for (int i = 0; i < 30; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    dut->bank_rsp_valid_i = 0;
    for (int i = 0; i < 20; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    // ── Phase 6: Backpressure ───────────────────────────────
    printf("phase 6: backpressure\n");
    clear_inputs(dut);
    dut->bank_req_ready_i = 0;

    // Full pressure: prim_fifo_async now correctly supports Depth=6.
    dut->axi_ar_valid_i = 1;
    dut->axi_ar_id_i    = 0x100;
    dut->axi_ar_addr_i  = 0x1000;

    for (int i = 0; i < 30; i++) {
        sim.tick();
        compare_all(sim, dut);
    }
    dut->axi_ar_valid_i = 0;
    dut->bank_req_ready_i = 0xF;
    for (int i = 0; i < 30; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    // ── Phase 7: 500-cycle random ───────────────────────────
    printf("phase 7: random stimulus\n");
    srand(42);
    clear_inputs(dut);

    for (int cyc = 0; cyc < 500; cyc++) {
        // Random AR
        if (rand() % 8 == 0) {
            dut->axi_ar_valid_i = 1;
            dut->axi_ar_id_i    = rand() & 0x7FFFF;
            uint64_t addr = ((uint64_t)(rand() & 0xFFF)) << 6;
            dut->axi_ar_addr_i  = addr;
            dut->axi_ar_size_i  = 6;
            dut->axi_ar_qos_i   = rand() & 0xF;
        } else {
            dut->axi_ar_valid_i = 0;
        }

        // Random AW+W
        if (rand() % 8 == 0) {
            dut->axi_aw_valid_i = 1;
            dut->axi_aw_id_i    = rand() & 0x7FFFF;
            uint64_t addr = ((uint64_t)(rand() & 0xFFF)) << 6;
            dut->axi_aw_addr_i  = addr;
            dut->axi_aw_size_i  = 6;
            dut->axi_aw_user_i  = ReqWrite;
            dut->axi_w_valid_i  = 1;
            dut->axi_w_strb_i   = rand() & 0xFFFFFFFF;
            dut->axi_w_last_i   = 1;
        } else {
            dut->axi_aw_valid_i = 0;
            dut->axi_w_valid_i  = 0;
        }

        // Random bank rsp
        if (rand() % 6 == 0) {
            int bank = rand() % 4;
            dut->bank_rsp_valid_i = 1 << bank;
            dut->rsp_id_i      = rand() & 0x1FF;
            dut->rsp_source_i  = rand() & 0x3FF;
            dut->rsp_port_id_i = 0;
            dut->rsp_err_i     = rand() & 1;
            dut->rsp_wdata_i   = rand() & 1;
        } else {
            dut->bank_rsp_valid_i = 0;
        }

        // Random backpressure
        dut->bank_req_ready_i = (rand() % 4 == 0) ? 0 : 0xF;
        dut->axi_r_ready_i   = (rand() % 5 != 0);
        dut->axi_b_ready_i   = (rand() % 5 != 0);

        sim.tick();
        compare_all(sim, dut);
    }

    // Drain
    clear_inputs(dut);
    for (int i = 0; i < 50; i++) {
        sim.tick();
        compare_all(sim, dut);
    }

    return sim.finish();
}
