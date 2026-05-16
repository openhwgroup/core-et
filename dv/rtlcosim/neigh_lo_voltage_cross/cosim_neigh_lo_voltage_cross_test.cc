// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_lo_voltage_cross_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>
#include <random>

using DUT = Vcosim_neigh_lo_voltage_cross_tb;

template <size_t N>
static void zero_wide(VlWide<N>& v) {
    for (size_t i = 0; i < N; ++i) v[i] = 0;
}

template <size_t N>
static void fill_wide(VlWide<N>& v, std::mt19937& rng) {
    for (size_t i = 0; i < N; ++i) v[i] = rng();
}

template <size_t N>
static void compare_wide(CosimCtrl<DUT>& sim, const char* name,
                         const VlWide<N>& orig, const VlWide<N>& newv) {
    for (size_t i = 0; i < N; ++i) {
        char buf[96];
        std::snprintf(buf, sizeof(buf), "%s[%zu]", name, i);
        sim.compare(buf, orig[i], newv[i]);
    }
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    compare_wide(sim, "out", sim.dut->orig_out_o, sim.dut->new_out_o);
}

static void tick_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

static void init_inputs(DUT* dut) {
    dut->reset_c_push_i = 0;
    dut->reset_c_pop_i = 0;
    dut->reset_push_i = 0;
    dut->reset_pop_i = 0;
    dut->debug_reset_push_i = 0;
    dut->debug_reset_pop_i = 0;
    dut->reset_w_icache_i = 0;
    dut->dft_reset_byp_hv_i = 0;
    dut->dft_reset_hv_i = 0;
    dut->dft_reset_byp_lv_i = 0;
    dut->dft_reset_lv_i = 0;
    dut->esr_icache_prefetch_done_i = 0;
    dut->esr_icache_err_detected_i = 0;
    dut->esr_icache_err_logged_i = 0;
    dut->bpam_rc_tbox_ack_i = 0;
    zero_wide(dut->neigh_sm_signals_i);
    zero_wide(dut->apb_esr_rsp_i);
    dut->flb_neigh_l2_req_valid_i = 0;
    dut->flb_neigh_l2_req_data_i = 0;
    dut->icache_f2_sram_req_write_i = 0;
    dut->icache_f2_sram_req_addr_i = 0;
    dut->icache_f2_sram_req_valid_i = 0;
    dut->icache_f2_sram_req_ready_i = 0;
    dut->esr_and_or_tree_l0_i = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    std::mt19937 rng(0x4c4f5643u);
    init_inputs(sim.dut.get());
    sim.reset(6);

    for (int i = 0; i < 8; ++i) tick_compare(sim);

    // I-cache request FIFO, held by read-side backpressure then consumed.
    sim.dut->icache_f2_sram_req_write_i = 1;
    sim.dut->icache_f2_sram_req_addr_i = 0x12a;
    sim.dut->icache_f2_sram_req_valid_i = 1;
    sim.dut->icache_f2_sram_req_ready_i = 0;
    tick_compare(sim);
    sim.dut->icache_f2_sram_req_valid_i = 0;
    for (int i = 0; i < 4; ++i) tick_compare(sim);
    sim.dut->icache_f2_sram_req_ready_i = 1;
    for (int i = 0; i < 3; ++i) tick_compare(sim);
    sim.dut->icache_f2_sram_req_ready_i = 0;

    // APB response pulse. Packed order is {prdata, pready, pslverr}.
    sim.dut->apb_esr_rsp_i[0] = 0x55aa0102u;
    sim.dut->apb_esr_rsp_i[1] = 0x03040506u;
    sim.dut->apb_esr_rsp_i[2] = 0x1u;       // pslverr
    sim.dut->apb_esr_rsp_i[2] |= (1u << 1); // pready
    for (int i = 0; i < 2; ++i) tick_compare(sim);
    sim.dut->apb_esr_rsp_i[2] = 0;
    for (int i = 0; i < 8; ++i) tick_compare(sim);

    // TBOX ack change-detect FIFO.
    sim.dut->bpam_rc_tbox_ack_i = 0x3;
    for (int i = 0; i < 12; ++i) tick_compare(sim);
    sim.dut->bpam_rc_tbox_ack_i = 0;
    for (int i = 0; i < 8; ++i) tick_compare(sim);

    // Fast-local-barrier request and debug/status paths.
    sim.dut->flb_neigh_l2_req_valid_i = 1;
    sim.dut->flb_neigh_l2_req_data_i = 0x1555;
    sim.dut->neigh_sm_signals_i[0] = 0xdeadbeef;
    sim.dut->neigh_sm_signals_i[1] = 0x01020304;
    sim.dut->esr_and_or_tree_l0_i = 0x2aa;
    tick_compare(sim);
    sim.dut->flb_neigh_l2_req_valid_i = 0;
    for (int i = 0; i < 16; ++i) tick_compare(sim);

    // Prefetch-done edge FIFO and error pulse paths.
    sim.dut->esr_icache_prefetch_done_i = 1;
    for (int i = 0; i < 10; ++i) tick_compare(sim);
    sim.dut->esr_icache_prefetch_done_i = 0;
    for (int i = 0; i < 8; ++i) tick_compare(sim);
    sim.dut->esr_icache_err_detected_i = 1;
    sim.dut->esr_icache_err_logged_i = 1;
    tick_compare(sim);
    sim.dut->esr_icache_err_detected_i = 0;
    sim.dut->esr_icache_err_logged_i = 0;
    for (int i = 0; i < 12; ++i) tick_compare(sim);

    // DFT and warm-reset bypass paths.
    sim.dut->reset_w_icache_i = 1;
    for (int i = 0; i < 3; ++i) tick_compare(sim);
    sim.dut->reset_w_icache_i = 0;
    for (int i = 0; i < 4; ++i) tick_compare(sim);
    sim.dut->dft_reset_byp_hv_i = 1;
    sim.dut->dft_reset_byp_lv_i = 1;
    sim.dut->dft_reset_hv_i = 1;
    sim.dut->dft_reset_lv_i = 1;
    for (int i = 0; i < 3; ++i) tick_compare(sim);
    sim.dut->dft_reset_hv_i = 0;
    sim.dut->dft_reset_lv_i = 0;
    sim.dut->dft_reset_byp_hv_i = 0;
    sim.dut->dft_reset_byp_lv_i = 0;
    for (int i = 0; i < 8; ++i) tick_compare(sim);

    // Reset-domain separation: c-domain, functional domain, debug domain.
    sim.dut->reset_c_push_i = 1;
    sim.dut->reset_c_pop_i = 1;
    for (int i = 0; i < 2; ++i) tick_compare(sim);
    sim.dut->reset_c_push_i = 0;
    sim.dut->reset_c_pop_i = 0;
    for (int i = 0; i < 4; ++i) tick_compare(sim);
    sim.dut->debug_reset_push_i = 1;
    sim.dut->debug_reset_pop_i = 1;
    for (int i = 0; i < 2; ++i) tick_compare(sim);
    sim.dut->debug_reset_push_i = 0;
    sim.dut->debug_reset_pop_i = 0;
    for (int i = 0; i < 4; ++i) tick_compare(sim);
    sim.dut->reset_push_i = 1;
    sim.dut->reset_pop_i = 1;
    for (int i = 0; i < 2; ++i) tick_compare(sim);
    sim.dut->reset_push_i = 0;
    sim.dut->reset_pop_i = 0;
    for (int i = 0; i < 8; ++i) tick_compare(sim);

    // Random 1:1-clock traffic; all ports were zero during init/reset and are
    // driven high/non-zero in directed and random phases.
    for (int cyc = 0; cyc < 3600; ++cyc) {
        sim.dut->esr_icache_prefetch_done_i = (rng() >> 0) & 1;
        sim.dut->esr_icache_err_detected_i = (rng() % 29) == 0;
        sim.dut->esr_icache_err_logged_i = (rng() % 31) == 0;
        sim.dut->bpam_rc_tbox_ack_i = rng() & 0x3;
        fill_wide(sim.dut->neigh_sm_signals_i, rng);
        fill_wide(sim.dut->apb_esr_rsp_i, rng);
        if ((rng() % 5) == 0) sim.dut->apb_esr_rsp_i[2] |= (1u << 1); // pready
        sim.dut->flb_neigh_l2_req_valid_i = (rng() % 17) == 0;
        sim.dut->flb_neigh_l2_req_data_i = rng() & 0x1fff;
        sim.dut->icache_f2_sram_req_write_i = (rng() >> 1) & 1;
        sim.dut->icache_f2_sram_req_addr_i = rng() & 0x1ff;
        sim.dut->icache_f2_sram_req_valid_i = (rng() % 4) == 0;
        sim.dut->icache_f2_sram_req_ready_i = (rng() % 3) != 0;
        sim.dut->esr_and_or_tree_l0_i = rng() & 0x3ff;
        tick_compare(sim);
    }

    sim.check(sim.comparisons > 50000, "lo-voltage crossing cosim made 50k+ comparisons");
    return sim.finish();
}
