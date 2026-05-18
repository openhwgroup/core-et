// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncacheable_noc_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <string>

static uint32_t xorshift32(uint32_t& s) {
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return s;
}

template <typename T>
static void fill_words(T& words, int n, uint32_t& seed) {
    for (int i = 0; i < n; ++i) words[i] = xorshift32(seed);
}

template <typename T, typename U>
static void compare_words(CosimCtrl<Vcosim_uncacheable_noc_tb>& sim,
                          const char* name, const T& orig, const U& nw, int n) {
    for (int i = 0; i < n; ++i) {
        sim.compare(std::string(name) + "[" + std::to_string(i) + "]", orig[i], nw[i]);
    }
}

static void compare_all(CosimCtrl<Vcosim_uncacheable_noc_tb>& sim) {
    sim.compare("neigh_uc_req_ready", sim.dut->orig_neigh_uc_req_ready_o, sim.dut->new_neigh_uc_req_ready_o);
    sim.compare("neigh_uc_min_rsp_valid", sim.dut->orig_neigh_uc_min_rsp_valid_o, sim.dut->new_neigh_uc_min_rsp_valid_o);
    compare_words(sim, "neigh_uc_min_rsp_info", sim.dut->orig_neigh_uc_min_rsp_info_bits_o,
                  sim.dut->new_neigh_uc_min_rsp_info_bits_o, 18);
    sim.compare("credit_return", sim.dut->orig_credit_return_o, sim.dut->new_credit_return_o);
    sim.compare("from_sys_aw_ready", sim.dut->orig_from_sys_aw_ready_o, sim.dut->new_from_sys_aw_ready_o);
    sim.compare("from_sys_w_ready", sim.dut->orig_from_sys_w_ready_o, sim.dut->new_from_sys_w_ready_o);
    sim.compare("from_sys_b", sim.dut->orig_from_sys_b_bits_o, sim.dut->new_from_sys_b_bits_o);
    sim.compare("from_sys_b_valid", sim.dut->orig_from_sys_b_valid_o, sim.dut->new_from_sys_b_valid_o);
    compare_words(sim, "to_l3_ar", sim.dut->orig_to_l3_ar_bits_o, sim.dut->new_to_l3_ar_bits_o, 3);
    sim.compare("to_l3_ar_valid", sim.dut->orig_to_l3_ar_valid_o, sim.dut->new_to_l3_ar_valid_o);
    compare_words(sim, "to_l3_aw", sim.dut->orig_to_l3_aw_bits_o, sim.dut->new_to_l3_aw_bits_o, 3);
    sim.compare("to_l3_aw_valid", sim.dut->orig_to_l3_aw_valid_o, sim.dut->new_to_l3_aw_valid_o);
    compare_words(sim, "to_l3_w", sim.dut->orig_to_l3_w_bits_o, sim.dut->new_to_l3_w_bits_o, 19);
    sim.compare("to_l3_w_valid", sim.dut->orig_to_l3_w_valid_o, sim.dut->new_to_l3_w_valid_o);
    sim.compare("to_l3_b_ready", sim.dut->orig_to_l3_b_ready_o, sim.dut->new_to_l3_b_ready_o);
    sim.compare("to_l3_r_ready", sim.dut->orig_to_l3_r_ready_o, sim.dut->new_to_l3_r_ready_o);
    compare_words(sim, "to_sys_ar", sim.dut->orig_to_sys_ar_bits_o, sim.dut->new_to_sys_ar_bits_o, 3);
    sim.compare("to_sys_ar_valid", sim.dut->orig_to_sys_ar_valid_o, sim.dut->new_to_sys_ar_valid_o);
    compare_words(sim, "to_sys_aw", sim.dut->orig_to_sys_aw_bits_o, sim.dut->new_to_sys_aw_bits_o, 3);
    sim.compare("to_sys_aw_valid", sim.dut->orig_to_sys_aw_valid_o, sim.dut->new_to_sys_aw_valid_o);
    compare_words(sim, "to_sys_w", sim.dut->orig_to_sys_w_bits_o, sim.dut->new_to_sys_w_bits_o, 19);
    sim.compare("to_sys_w_valid", sim.dut->orig_to_sys_w_valid_o, sim.dut->new_to_sys_w_valid_o);
    sim.compare("to_sys_b_ready", sim.dut->orig_to_sys_b_ready_o, sim.dut->new_to_sys_b_ready_o);
    sim.compare("to_sys_r_ready", sim.dut->orig_to_sys_r_ready_o, sim.dut->new_to_sys_r_ready_o);
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_uncacheable_noc_tb> sim(argc, argv);
    sim.dut->shire_id_i = 0;
    sim.dut->uc_config_bits_i = 0;
    sim.dut->neigh_uc_req_valid_i = 0;
    for (int i = 0; i < 18; ++i) sim.dut->neigh_uc_req_info_bits_i[i] = 0;
    sim.dut->neigh_uc_min_rsp_ready_i = 0xf;
    sim.dut->credit_i = 0;
    for (int i = 0; i < 3; ++i) sim.dut->from_sys_aw_bits_i[i] = 0;
    for (int i = 0; i < 10; ++i) sim.dut->from_sys_w_bits_i[i] = 0;
    sim.dut->from_sys_aw_valid_i = 0;
    sim.dut->from_sys_w_valid_i = 0;
    sim.dut->from_sys_b_ready_i = 1;
    sim.dut->l3_enabled_i = 0;
    sim.dut->remote_scp_enabled_i = 0;
    sim.dut->to_l3_ar_ready_i = 1;
    sim.dut->to_l3_aw_ready_i = 1;
    sim.dut->to_l3_w_ready_i = 1;
    sim.dut->to_l3_b_bits_i = 0;
    sim.dut->to_l3_b_valid_i = 0;
    for (int i = 0; i < 17; ++i) sim.dut->to_l3_r_bits_i[i] = 0;
    sim.dut->to_l3_r_valid_i = 0;
    sim.dut->to_sys_ar_ready_i = 1;
    sim.dut->to_sys_aw_ready_i = 1;
    sim.dut->to_sys_w_ready_i = 1;
    sim.dut->to_sys_b_bits_i = 0;
    sim.dut->to_sys_b_valid_i = 0;
    for (int i = 0; i < 17; ++i) sim.dut->to_sys_r_bits_i[i] = 0;
    sim.dut->to_sys_r_valid_i = 0;
    sim.reset();
    compare_all(sim);

    uint32_t seed = 0x0c0c0c0cu;
    for (int i = 0; i < 12288; ++i) {
        uint32_t r = (i < 2048) ? static_cast<uint32_t>(i) : xorshift32(seed);
        fill_words(sim.dut->neigh_uc_req_info_bits_i, 18, seed);
        fill_words(sim.dut->from_sys_aw_bits_i, 3, seed);
        fill_words(sim.dut->from_sys_w_bits_i, 10, seed);
        fill_words(sim.dut->to_l3_r_bits_i, 17, seed);
        fill_words(sim.dut->to_sys_r_bits_i, 17, seed);
        sim.dut->to_l3_b_bits_i = xorshift32(seed) & 0x7ffu;
        sim.dut->to_sys_b_bits_i = xorshift32(seed) & 0x7ffu;

        sim.dut->shire_id_i = (r >> 0) & 0xffu;
        sim.dut->uc_config_bits_i = (r >> 8) & 1;
        sim.dut->neigh_uc_req_valid_i = (r >> 9) & 1;
        sim.dut->neigh_uc_min_rsp_ready_i = (r >> 10) & 0xfu;
        if ((i & 15) == 0) sim.dut->neigh_uc_min_rsp_ready_i = 0xf;
        sim.dut->credit_i = (r >> 14) & 1;
        sim.dut->from_sys_aw_valid_i = (r >> 15) & 1;
        sim.dut->from_sys_w_valid_i = (r >> 16) & 1;
        sim.dut->from_sys_b_ready_i = ((r >> 17) & 1) || (i % 5 != 0);
        sim.dut->l3_enabled_i = (r >> 18) & 1;
        sim.dut->remote_scp_enabled_i = (r >> 19) & 1;
        sim.dut->to_l3_ar_ready_i = (r >> 20) & 1;
        sim.dut->to_l3_aw_ready_i = (r >> 21) & 1;
        sim.dut->to_l3_w_ready_i = (r >> 22) & 1;
        sim.dut->to_l3_b_valid_i = (r >> 23) & 1;
        sim.dut->to_l3_r_valid_i = (r >> 24) & 1;
        sim.dut->to_sys_ar_ready_i = (r >> 25) & 1;
        sim.dut->to_sys_aw_ready_i = (r >> 26) & 1;
        sim.dut->to_sys_w_ready_i = (r >> 27) & 1;
        sim.dut->to_sys_b_valid_i = (r >> 28) & 1;
        sim.dut->to_sys_r_valid_i = (r >> 29) & 1;
        // Sweep opcode/address/size/resp low bits early and keep response IDs in range often.
        sim.dut->neigh_uc_req_info_bits_i[0] = (sim.dut->neigh_uc_req_info_bits_i[0] & ~0x7fffffu) | (r & 0x7fffffu);
        sim.dut->to_l3_b_bits_i = (sim.dut->to_l3_b_bits_i & ~0xffu) | ((r >> 1) & 0xffu);
        sim.dut->to_sys_b_bits_i = (sim.dut->to_sys_b_bits_i & ~0xffu) | ((r >> 3) & 0xffu);
        sim.dut->to_l3_r_bits_i[0] = (sim.dut->to_l3_r_bits_i[0] & ~0xffu) | ((r >> 5) & 0xffu);
        sim.dut->to_sys_r_bits_i[0] = (sim.dut->to_sys_r_bits_i[0] & ~0xffu) | ((r >> 7) & 0xffu);
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}
