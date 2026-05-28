// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hi_voltage_cross_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>
#include <random>

using DUT = Vcosim_neigh_hi_voltage_cross_tb;

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
    dut->dft_reset_byp_hv_i = 0;
    dut->dft_reset_hv_i = 0;
    dut->dft_reset_byp_lv_i = 0;
    dut->dft_reset_lv_i = 0;
    dut->shire_id_i = 0;
    dut->neigh_id_i = 0;
    dut->tbox_id_i = 0;
    dut->esr_minion_features_i = 0;
    dut->chicken_bits_i = 0;
    dut->esr_shire_coop_mode_i = 0;
    dut->tbox_en_i = 0;
    dut->neigh_t0_en_i = 0;
    dut->neigh_t1_en_i = 0;
    dut->dmctrl_i = 0;
    dut->bpam_run_control_i = 0;
    dut->esr_icache_prefetch_conf_i = 0;
    dut->esr_icache_prefetch_start_i = 0;
    dut->neigh_sc_rsp_valid_i = 0;
    zero_wide(dut->neigh_sc_rsp_info_i);
    dut->neigh_sc_rsp_ready_i = 0;
    dut->neigh_sm_gpio_i = 0;
    dut->hw_dbg_sm_monitor_enabled_i = 0;
    dut->apb_pready_neigh_i = 0;
    zero_wide(dut->apb_esr_req_i);
    dut->uc_to_neigh_fcc_target_i = 0;
    dut->uc_to_neigh_fcc_i = 0;
    dut->int_mtip_i = 0;
    dut->plic_meip_i = 0;
    dut->plic_seip_i = 0;
    dut->esr_to_neigh_ipi_msip_i = 0;
    dut->esr_to_neigh_ipi_trigger_i = 0;
    dut->flb_l2_neigh_resp_valid_i = 0;
    dut->flb_l2_neigh_resp_data_i = 0;
    zero_wide(dut->icache_f0_sram_resp_dout_i);
    dut->icache_f0_sram_resp_valid_i = 0;
    dut->icache_f0_sram_resp_ready_i = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    std::mt19937 rng(0x48495643u);
    init_inputs(sim.dut.get());
    sim.reset(6);

    // Directed static/level inputs: all non-trivial level synchronizers toggle.
    sim.dut->shire_id_i = 0xa5;
    sim.dut->neigh_id_i = 0x2;
    sim.dut->tbox_id_i = 0x3;
    sim.dut->esr_minion_features_i = 0x2d;
    sim.dut->chicken_bits_i = 0x35;
    sim.dut->esr_shire_coop_mode_i = 1;
    sim.dut->tbox_en_i = 1;
    sim.dut->neigh_t0_en_i = 0x5a;
    sim.dut->neigh_t1_en_i = 0xc3;
    sim.dut->int_mtip_i = 0x81;
    sim.dut->plic_meip_i = 1;
    sim.dut->plic_seip_i = 1;
    sim.dut->esr_to_neigh_ipi_msip_i = 0xa55a;
    sim.dut->neigh_sm_gpio_i = 0x3a5;
    sim.dut->hw_dbg_sm_monitor_enabled_i = 1;
    for (int i = 0; i < 8; ++i) tick_compare(sim);

    // Exercise all FIFO/pulse push and pop paths with backpressure.
    sim.dut->icache_f0_sram_resp_dout_i[0] = 0x11223344;
    sim.dut->icache_f0_sram_resp_dout_i[1] = 0x55667788;
    sim.dut->icache_f0_sram_resp_valid_i = 1;
    sim.dut->icache_f0_sram_resp_ready_i = 0;
    tick_compare(sim);
    sim.dut->icache_f0_sram_resp_valid_i = 0;
    for (int i = 0; i < 4; ++i) tick_compare(sim);
    sim.dut->icache_f0_sram_resp_ready_i = 1;
    for (int i = 0; i < 3; ++i) tick_compare(sim);
    sim.dut->icache_f0_sram_resp_ready_i = 0;

    sim.dut->apb_esr_req_i[0] = 0xcafebabeu << 1;
    sim.dut->apb_esr_req_i[2] = 0x00012345u << 3;
    sim.dut->apb_esr_req_i[2] |= (1u << 1); // psel
    sim.dut->apb_esr_req_i[2] |= (1u << 2); // penable
    sim.dut->apb_pready_neigh_i = 0;
    for (int i = 0; i < 3; ++i) tick_compare(sim);
    sim.dut->apb_esr_req_i[2] &= ~(1u << 1);
    sim.dut->apb_esr_req_i[2] &= ~(1u << 2);
    sim.dut->apb_pready_neigh_i = 1;
    for (int i = 0; i < 10; ++i) tick_compare(sim);
    sim.dut->apb_pready_neigh_i = 0;

    sim.dut->neigh_sc_rsp_info_i[0] = 0x01234567;
    sim.dut->neigh_sc_rsp_info_i[1] = 0x89abcdef;
    sim.dut->neigh_sc_rsp_valid_i = 1;
    sim.dut->neigh_sc_rsp_ready_i = 0;
    tick_compare(sim);
    sim.dut->neigh_sc_rsp_valid_i = 0;
    for (int i = 0; i < 4; ++i) tick_compare(sim);
    sim.dut->neigh_sc_rsp_ready_i = 1;
    for (int i = 0; i < 3; ++i) tick_compare(sim);
    sim.dut->neigh_sc_rsp_ready_i = 0;

    sim.dut->flb_l2_neigh_resp_valid_i = 1;
    sim.dut->flb_l2_neigh_resp_data_i = 1;
    tick_compare(sim);
    sim.dut->flb_l2_neigh_resp_valid_i = 0;
    sim.dut->flb_l2_neigh_resp_data_i = 0;
    for (int i = 0; i < 6; ++i) tick_compare(sim);

    sim.dut->esr_to_neigh_ipi_trigger_i = 0x0042;
    sim.dut->uc_to_neigh_fcc_i = 0x81;
    sim.dut->uc_to_neigh_fcc_target_i = 0x2;
    sim.dut->esr_icache_prefetch_conf_i = 0x12345;
    sim.dut->esr_icache_prefetch_start_i = 1;
    sim.dut->bpam_run_control_i = 0x155;
    sim.dut->dmctrl_i = 0x001; // write_enable is packed LSB.
    tick_compare(sim);
    sim.dut->esr_to_neigh_ipi_trigger_i = 0;
    sim.dut->uc_to_neigh_fcc_i = 0;
    sim.dut->uc_to_neigh_fcc_target_i = 0;
    sim.dut->esr_icache_prefetch_start_i = 0;
    sim.dut->dmctrl_i = 0;
    for (int i = 0; i < 12; ++i) tick_compare(sim);

    // DFT reset bypass on both domains.
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

    // Individual reset domains: c-domain and functional push/pop domains.
    sim.dut->reset_c_push_i = 1;
    sim.dut->reset_c_pop_i = 1;
    for (int i = 0; i < 2; ++i) tick_compare(sim);
    sim.dut->reset_c_push_i = 0;
    sim.dut->reset_c_pop_i = 0;
    for (int i = 0; i < 4; ++i) tick_compare(sim);
    sim.dut->reset_push_i = 1;
    sim.dut->reset_pop_i = 1;
    for (int i = 0; i < 2; ++i) tick_compare(sim);
    sim.dut->reset_push_i = 0;
    sim.dut->reset_pop_i = 0;
    for (int i = 0; i < 8; ++i) tick_compare(sim);

    // Random 1:1-clock traffic. Every practical input port is driven both low
    // (by init/reset phases) and high/non-zero (directed phases plus random).
    for (int cyc = 0; cyc < 3200; ++cyc) {
        sim.dut->shire_id_i = rng() & 0xff;
        sim.dut->neigh_id_i = rng() & 0x3;
        sim.dut->tbox_id_i = rng() & 0x3;
        sim.dut->esr_minion_features_i = rng() & 0x3f;
        sim.dut->chicken_bits_i = rng() & 0x3f;
        sim.dut->esr_shire_coop_mode_i = (rng() >> 0) & 1;
        sim.dut->tbox_en_i = (rng() >> 1) & 1;
        sim.dut->neigh_t0_en_i = rng() & 0xff;
        sim.dut->neigh_t1_en_i = rng() & 0xff;
        sim.dut->dmctrl_i = rng() & 0x3ff;
        sim.dut->bpam_run_control_i = rng() & 0x7ff;
        sim.dut->esr_icache_prefetch_conf_i = rng();
        sim.dut->esr_icache_prefetch_start_i = (rng() % 11) == 0;
        sim.dut->neigh_sc_rsp_valid_i = (rng() % 5) == 0;
        fill_wide(sim.dut->neigh_sc_rsp_info_i, rng);
        sim.dut->neigh_sc_rsp_ready_i = (rng() % 3) != 0;
        sim.dut->neigh_sm_gpio_i = rng() & 0x7ff;
        sim.dut->hw_dbg_sm_monitor_enabled_i = (rng() >> 2) & 1;
        fill_wide(sim.dut->apb_esr_req_i, rng);
        if ((rng() % 7) == 0) sim.dut->apb_esr_req_i[2] |= (1u << 1);
        if ((rng() % 9) == 0) sim.dut->apb_esr_req_i[2] |= (1u << 2);
        sim.dut->apb_pready_neigh_i = (rng() % 5) == 0;
        sim.dut->uc_to_neigh_fcc_target_i = rng() & 0x3;
        sim.dut->uc_to_neigh_fcc_i = ((rng() % 13) == 0) ? (rng() & 0xff) : 0;
        sim.dut->int_mtip_i = rng() & 0xff;
        sim.dut->plic_meip_i = (rng() >> 3) & 1;
        sim.dut->plic_seip_i = (rng() >> 4) & 1;
        sim.dut->esr_to_neigh_ipi_msip_i = rng() & 0xffff;
        sim.dut->esr_to_neigh_ipi_trigger_i = ((rng() % 17) == 0) ? (rng() & 0xffff) : 0;
        sim.dut->flb_l2_neigh_resp_valid_i = (rng() % 19) == 0;
        sim.dut->flb_l2_neigh_resp_data_i = (rng() >> 5) & 1;
        fill_wide(sim.dut->icache_f0_sram_resp_dout_i, rng);
        sim.dut->icache_f0_sram_resp_valid_i = (rng() % 4) == 0;
        sim.dut->icache_f0_sram_resp_ready_i = (rng() % 3) != 0;
        tick_compare(sim);
    }

    sim.check(sim.comparisons > 50000, "hi-voltage crossing cosim made 50k+ comparisons");
    return sim.finish();
}
