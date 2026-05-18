// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncacheable_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <string>

static uint32_t xorshift32(uint32_t &state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

template <typename W>
static void clear_words(W &w) {
    for (int i = 0; i < static_cast<int>(sizeof(w) / sizeof(w[0])); ++i) w[i] = 0;
}

template <typename W>
static void fill_words(W &w, uint32_t &seed) {
    for (int i = 0; i < static_cast<int>(sizeof(w) / sizeof(w[0])); ++i) w[i] = xorshift32(seed);
}

template <typename SimT, typename W>
static void compare_words(SimT &sim, const char *name, const W &orig, const W &newv, int valid_bits) {
    const int nwords = static_cast<int>(sizeof(orig) / sizeof(orig[0]));
    for (int i = 0; i < nwords; ++i) {
        int remaining = valid_bits - i * 32;
        uint32_t mask = remaining >= 32 ? 0xffffffffu : (remaining <= 0 ? 0u : ((1u << remaining) - 1u));
        sim.compare(std::string(name) + "[" + std::to_string(i) + "]", orig[i] & mask, newv[i] & mask);
    }
}

template <typename SimT, typename W>
static void compare_words_when(SimT &sim, const char *name, const W &orig, const W &newv, int valid_bits, bool valid) {
    if (valid) compare_words(sim, name, orig, newv, valid_bits);
}

template <typename SimT>
static void compare_scalar_when(SimT &sim, const char *name, uint64_t orig, uint64_t newv, uint64_t mask, bool valid) {
    if (valid) sim.compare(name, orig & mask, newv & mask);
}

static void init_inputs(Vcosim_uncacheable_tb *d) {
    d->dft_hv_scanmode_i = 0;
    d->dft_hv_scan_reset_ni = 1;
    d->dft_lv_scanmode_i = 0;
    d->dft_lv_scan_reset_ni = 1;
    d->shire_id_i = 0x7f;
    d->esr_enables_i = 0;
    d->esr_wdata_i = 0;
    d->esr_uc_config_i = 0;
    d->flb_neigh_l2_req_valid_i = 0;
    d->flb_neigh_l2_req_data_i = 0;
    d->neigh_uc_req_valid_i = 0;
    clear_words(d->neigh_uc_req_info_i);
    d->neigh_uc_min_rsp_ready_i = 0x7;
    d->sbm_write_credit_return_i = 0;
    d->sbm_sys_axi_ar_ready_i = 1;
    d->sbm_sys_axi_aw_ready_i = 1;
    d->sbm_sys_axi_w_ready_i = 1;
    d->sbm_sys_axi_b_i = 0;
    d->sbm_sys_axi_b_valid_i = 0;
    clear_words(d->sbm_sys_axi_r_i);
    d->sbm_sys_axi_r_valid_i = 0;
    d->l3_enabled_i = 1;
    d->remote_scp_enabled_i = 1;
    clear_words(d->sys_axi_ar_i);
    d->sys_axi_ar_valid_i = 0;
    clear_words(d->sys_axi_aw_i);
    d->sys_axi_aw_valid_i = 0;
    clear_words(d->sys_axi_w_i);
    d->sys_axi_w_valid_i = 0;
    d->sys_axi_b_ready_i = 1;
    d->sys_axi_r_ready_i = 1;
    d->sys_axi_aw_vcvalid_i = 1;
    d->sys_axi_w_vcvalid_i = 1;
    d->to_l3_ar_ready_i = 1;
    d->to_l3_aw_ready_i = 1;
    d->to_l3_w_ready_i = 1;
    d->to_l3_b_i = 0;
    d->to_l3_b_valid_i = 0;
    clear_words(d->to_l3_r_i);
    d->to_l3_r_valid_i = 0;
    d->to_sys_ar_ready_i = 1;
    d->to_sys_aw_ready_i = 1;
    d->to_sys_w_ready_i = 1;
    d->to_sys_b_i = 0;
    d->to_sys_b_valid_i = 0;
    clear_words(d->to_sys_r_i);
    d->to_sys_r_valid_i = 0;
}

template <typename SimT>
static void compare_all(SimT &sim) {
    auto *d = sim.dut.get();
    compare_words(sim, "esr_values", d->orig_esr_values_o, d->new_esr_values_o, 256);
    sim.compare("flb_resp_valid", d->orig_flb_resp_valid_o, d->new_flb_resp_valid_o);
    sim.compare("flb_resp_data", d->orig_flb_resp_data_o, d->new_flb_resp_data_o);
    sim.compare("fcc", d->orig_fcc_o, d->new_fcc_o);
    sim.compare("fcc_target", d->orig_fcc_target_o, d->new_fcc_target_o);
    sim.compare("neigh_ready", d->orig_neigh_ready_o, d->new_neigh_ready_o);
    sim.compare("neigh_rsp_valid", d->orig_neigh_rsp_valid_o, d->new_neigh_rsp_valid_o);
    compare_words_when(sim, "neigh_rsp", d->orig_neigh_rsp_info_o, d->new_neigh_rsp_info_o, 550,
                       (d->orig_neigh_rsp_valid_o & d->new_neigh_rsp_valid_o) != 0);

    sim.compare("sbm_enable_read", d->orig_sbm_enable_read_o, d->new_sbm_enable_read_o);
    sim.compare("sbm_enable_write", d->orig_sbm_enable_write_o, d->new_sbm_enable_write_o);
    compare_words_when(sim, "sbm_ar", d->orig_sbm_ar_o, d->new_sbm_ar_o, 85, d->orig_sbm_enable_read_o && d->new_sbm_enable_read_o);
    compare_words_when(sim, "sbm_aw", d->orig_sbm_aw_o, d->new_sbm_aw_o, 89, d->orig_sbm_enable_write_o && d->new_sbm_enable_write_o);
    compare_words_when(sim, "sbm_w", d->orig_sbm_w_o, d->new_sbm_w_o, 289, d->orig_sbm_enable_write_o && d->new_sbm_enable_write_o);
    sim.compare("sbm_b_ready", d->orig_sbm_b_ready_o, d->new_sbm_b_ready_o);
    sim.compare("sbm_r_ready", d->orig_sbm_r_ready_o, d->new_sbm_r_ready_o);

    sim.compare("sys_ar_ready", d->orig_sys_ar_ready_o, d->new_sys_ar_ready_o);
    sim.compare("sys_aw_ready", d->orig_sys_aw_ready_o, d->new_sys_aw_ready_o);
    sim.compare("sys_w_ready", d->orig_sys_w_ready_o, d->new_sys_w_ready_o);
    sim.compare("sys_b_valid", d->orig_sys_b_valid_o, d->new_sys_b_valid_o);
    compare_scalar_when(sim, "sys_b", d->orig_sys_b_o, d->new_sys_b_o, 0x1fffffu, d->orig_sys_b_valid_o && d->new_sys_b_valid_o);
    sim.compare("sys_r_valid", d->orig_sys_r_valid_o, d->new_sys_r_valid_o);
    compare_words_when(sim, "sys_r", d->orig_sys_r_o, d->new_sys_r_o, 278, d->orig_sys_r_valid_o && d->new_sys_r_valid_o);
    sim.compare("sys_aw_credit", d->orig_sys_aw_credit_o, d->new_sys_aw_credit_o);

    sim.compare("to_l3_ar_valid", d->orig_to_l3_ar_valid_o, d->new_to_l3_ar_valid_o);
    compare_words_when(sim, "to_l3_ar", d->orig_to_l3_ar_o, d->new_to_l3_ar_o, 75, d->orig_to_l3_ar_valid_o && d->new_to_l3_ar_valid_o);
    sim.compare("to_l3_aw_valid", d->orig_to_l3_aw_valid_o, d->new_to_l3_aw_valid_o);
    compare_words_when(sim, "to_l3_aw", d->orig_to_l3_aw_o, d->new_to_l3_aw_o, 79, d->orig_to_l3_aw_valid_o && d->new_to_l3_aw_valid_o);
    sim.compare("to_l3_w_valid", d->orig_to_l3_w_valid_o, d->new_to_l3_w_valid_o);
    compare_words_when(sim, "to_l3_w", d->orig_to_l3_w_o, d->new_to_l3_w_o, 577, d->orig_to_l3_w_valid_o && d->new_to_l3_w_valid_o);
    sim.compare("to_l3_b_ready", d->orig_to_l3_b_ready_o, d->new_to_l3_b_ready_o);
    sim.compare("to_l3_r_ready", d->orig_to_l3_r_ready_o, d->new_to_l3_r_ready_o);

    sim.compare("to_sys_ar_valid", d->orig_to_sys_ar_valid_o, d->new_to_sys_ar_valid_o);
    compare_words_when(sim, "to_sys_ar", d->orig_to_sys_ar_o, d->new_to_sys_ar_o, 75, d->orig_to_sys_ar_valid_o && d->new_to_sys_ar_valid_o);
    sim.compare("to_sys_aw_valid", d->orig_to_sys_aw_valid_o, d->new_to_sys_aw_valid_o);
    compare_words_when(sim, "to_sys_aw", d->orig_to_sys_aw_o, d->new_to_sys_aw_o, 79, d->orig_to_sys_aw_valid_o && d->new_to_sys_aw_valid_o);
    sim.compare("to_sys_w_valid", d->orig_to_sys_w_valid_o, d->new_to_sys_w_valid_o);
    compare_words_when(sim, "to_sys_w", d->orig_to_sys_w_o, d->new_to_sys_w_o, 577, d->orig_to_sys_w_valid_o && d->new_to_sys_w_valid_o);
    sim.compare("to_sys_b_ready", d->orig_to_sys_b_ready_o, d->new_to_sys_b_ready_o);
    sim.compare("to_sys_r_ready", d->orig_to_sys_r_ready_o, d->new_to_sys_r_ready_o);
}

int main(int argc, char **argv) {
    CosimCtrl<Vcosim_uncacheable_tb> sim(argc, argv);
    std::printf("──── cosim uncacheable ────\n");
    init_inputs(sim.dut.get());
    sim.reset();
    for (int i = 0; i < 24; ++i) sim.tick();
    compare_all(sim);

    uint32_t seed = 0x1234abcd;
    for (int i = 0; i < 2048; ++i) {
        auto *d = sim.dut.get();
        d->esr_enables_i = 0;
        d->flb_neigh_l2_req_valid_i = 0;
        d->neigh_uc_req_valid_i = 0;
        d->sys_axi_ar_valid_i = 0;
        d->sys_axi_aw_valid_i = 0;
        d->sys_axi_w_valid_i = 0;
        d->to_l3_b_valid_i = 0;
        d->to_l3_r_valid_i = 0;
        d->to_sys_b_valid_i = 0;
        d->to_sys_r_valid_i = 0;
        d->sbm_sys_axi_b_valid_i = 0;
        d->sbm_sys_axi_r_valid_i = 0;
        d->sbm_write_credit_return_i = ((i & 0x7f) == 0x40);

        if (i == 4) { d->esr_enables_i = 1; d->esr_wdata_i = 0xa5; }
        if (i == 8) { d->esr_enables_i = (1ull << 4); d->esr_wdata_i = 7; }
        if (i == 16) { d->flb_neigh_l2_req_valid_i = 1; d->flb_neigh_l2_req_data_i = (1u << 5); }

        fill_words(d->neigh_uc_req_info_i, seed);
        fill_words(d->sys_axi_ar_i, seed);
        fill_words(d->sys_axi_aw_i, seed);
        fill_words(d->sys_axi_w_i, seed);
        fill_words(d->to_l3_r_i, seed);
        fill_words(d->to_sys_r_i, seed);
        clear_words(d->sbm_sys_axi_r_i);
        d->to_l3_b_i = xorshift32(seed) & 0x7ffu;
        d->to_sys_b_i = xorshift32(seed) & 0x7ffu;
        d->sbm_sys_axi_b_i = xorshift32(seed) & 0x1fffffu;
        d->neigh_uc_min_rsp_ready_i = ((i & 0x20) ? 0x5 : 0x7);
        d->sys_axi_b_ready_i = ((i & 0x3f) != 0x11);
        d->sys_axi_r_ready_i = ((i & 0x3f) != 0x12);
        d->to_l3_ar_ready_i = ((i & 0x3f) != 0x13);
        d->to_l3_aw_ready_i = ((i & 0x3f) != 0x14);
        d->to_l3_w_ready_i = ((i & 0x3f) != 0x15);
        d->to_sys_ar_ready_i = ((i & 0x3f) != 0x16);
        d->to_sys_aw_ready_i = ((i & 0x3f) != 0x17);
        d->to_sys_w_ready_i = ((i & 0x3f) != 0x18);
        d->sbm_sys_axi_ar_ready_i = ((i & 0x3f) != 0x19);
        d->sbm_sys_axi_aw_ready_i = ((i & 0x3f) != 0x1a);
        d->sbm_sys_axi_w_ready_i = ((i & 0x3f) != 0x1b);
        d->l3_enabled_i = ((i & 0x100) == 0);
        d->remote_scp_enabled_i = ((i & 0x80) == 0);
        d->dft_hv_scanmode_i = (i == 512);
        d->dft_lv_scanmode_i = (i == 1024);
        d->dft_hv_scan_reset_ni = 1;
        d->dft_lv_scan_reset_ni = 1;

        sim.tick();
        compare_all(sim);
    }
    return sim.finish();
}
