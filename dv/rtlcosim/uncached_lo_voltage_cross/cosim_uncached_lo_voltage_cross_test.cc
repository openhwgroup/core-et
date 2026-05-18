// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncached_lo_voltage_cross_tb.h"
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

static void fill_words(uint32_t *words, int nwords, uint32_t &seed) {
    for (int i = 0; i < nwords; ++i) words[i] = xorshift32(seed);
}

template <typename SimT>
static void compare_words_if_valid(SimT &sim, const char *name,
                                   const uint32_t *orig, const uint32_t *newv,
                                   int nwords, uint32_t last_mask,
                                   bool orig_valid, bool new_valid) {
    if (!(orig_valid && new_valid)) return;
    for (int i = 0; i < nwords; ++i) {
        uint32_t mask = (i == nwords - 1) ? last_mask : 0xffffffffu;
        sim.compare(std::string(name) + "[" + std::to_string(i) + "]", orig[i] & mask, newv[i] & mask);
    }
}

template <typename SimT>
static void compare_scalar_if_valid(SimT &sim, const char *name,
                                    uint32_t orig, uint32_t newv,
                                    uint32_t mask, bool orig_valid, bool new_valid) {
    if (orig_valid && new_valid) sim.compare(name, orig & mask, newv & mask);
}

template <typename SimT>
static void compare_all(SimT &sim) {
    auto *d = sim.dut.get();
    sim.compare("l3_b_valid", d->orig_to_axi_l3_b_valid_o, d->new_to_axi_l3_b_valid_o);
    sim.compare("l3_b_ready_lo", d->orig_to_axi_l3_b_ready_lo_o, d->new_to_axi_l3_b_ready_lo_o);
    compare_scalar_if_valid(sim, "l3_b", d->orig_to_axi_l3_b_bits_o, d->new_to_axi_l3_b_bits_o, 0x7ffu,
                            d->orig_to_axi_l3_b_valid_o, d->new_to_axi_l3_b_valid_o);
    sim.compare("l3_r_valid", d->orig_to_axi_l3_r_valid_o, d->new_to_axi_l3_r_valid_o);
    sim.compare("l3_r_ready_lo", d->orig_to_axi_l3_r_ready_lo_o, d->new_to_axi_l3_r_ready_lo_o);
    compare_words_if_valid(sim, "l3_r", d->orig_to_axi_l3_r_bits_o, d->new_to_axi_l3_r_bits_o, 17, 0xfffu,
                           d->orig_to_axi_l3_r_valid_o, d->new_to_axi_l3_r_valid_o);

    sim.compare("sys_b_valid", d->orig_to_axi_sys_b_valid_o, d->new_to_axi_sys_b_valid_o);
    sim.compare("sys_b_ready_lo", d->orig_to_axi_sys_b_ready_lo_o, d->new_to_axi_sys_b_ready_lo_o);
    compare_scalar_if_valid(sim, "sys_b", d->orig_to_axi_sys_b_bits_o, d->new_to_axi_sys_b_bits_o, 0x7ffu,
                            d->orig_to_axi_sys_b_valid_o, d->new_to_axi_sys_b_valid_o);
    sim.compare("sys_r_valid", d->orig_to_axi_sys_r_valid_o, d->new_to_axi_sys_r_valid_o);
    sim.compare("sys_r_ready_lo", d->orig_to_axi_sys_r_ready_lo_o, d->new_to_axi_sys_r_ready_lo_o);
    compare_words_if_valid(sim, "sys_r", d->orig_to_axi_sys_r_bits_o, d->new_to_axi_sys_r_bits_o, 17, 0xfffu,
                           d->orig_to_axi_sys_r_valid_o, d->new_to_axi_sys_r_valid_o);

    sim.compare("sys_ar_valid", d->orig_from_axi_sys_ar_valid_o, d->new_from_axi_sys_ar_valid_o);
    sim.compare("sys_ar_ready_lo", d->orig_from_axi_sys_ar_ready_lo_o, d->new_from_axi_sys_ar_ready_lo_o);
    compare_words_if_valid(sim, "sys_ar", d->orig_from_axi_sys_ar_bits_o, d->new_from_axi_sys_ar_bits_o, 3, 0x1fffffu,
                           d->orig_from_axi_sys_ar_valid_o, d->new_from_axi_sys_ar_valid_o);
    sim.compare("sys_uc_aw_valid", d->orig_from_axi_sys_uc_aw_valid_o, d->new_from_axi_sys_uc_aw_valid_o);
    sim.compare("sys_sbm_aw_valid", d->orig_from_axi_sys_sbm_aw_valid_o, d->new_from_axi_sys_sbm_aw_valid_o);
    sim.compare("sys_aw_ready_lo", d->orig_from_axi_sys_aw_ready_lo_o, d->new_from_axi_sys_aw_ready_lo_o);
    compare_words_if_valid(sim, "sys_uc_aw", d->orig_from_axi_sys_uc_aw_bits_o, d->new_from_axi_sys_uc_aw_bits_o, 3, 0x1ffffffu,
                           d->orig_from_axi_sys_uc_aw_valid_o, d->new_from_axi_sys_uc_aw_valid_o);
    compare_words_if_valid(sim, "sys_sbm_aw", d->orig_from_axi_sys_sbm_aw_bits_o, d->new_from_axi_sys_sbm_aw_bits_o, 3, 0x1ffffffu,
                           d->orig_from_axi_sys_sbm_aw_valid_o, d->new_from_axi_sys_sbm_aw_valid_o);
    sim.compare("sys_uc_w_valid", d->orig_from_axi_sys_uc_w_valid_o, d->new_from_axi_sys_uc_w_valid_o);
    sim.compare("sys_sbm_w_valid", d->orig_from_axi_sys_sbm_w_valid_o, d->new_from_axi_sys_sbm_w_valid_o);
    sim.compare("sys_w_ready_lo", d->orig_from_axi_sys_w_ready_lo_o, d->new_from_axi_sys_w_ready_lo_o);
    compare_words_if_valid(sim, "sys_uc_w", d->orig_from_axi_sys_uc_w_bits_o, d->new_from_axi_sys_uc_w_bits_o, 10, 0x1u,
                           d->orig_from_axi_sys_uc_w_valid_o, d->new_from_axi_sys_uc_w_valid_o);
    compare_words_if_valid(sim, "sys_sbm_w", d->orig_from_axi_sys_sbm_w_bits_o, d->new_from_axi_sys_sbm_w_bits_o, 10, 0x1u,
                           d->orig_from_axi_sys_sbm_w_valid_o, d->new_from_axi_sys_sbm_w_valid_o);
}

static void init_inputs(Vcosim_uncached_lo_voltage_cross_tb *d) {
    d->dft_lv_scanmode_i = 0; d->dft_lv_scan_reset_ni = 1;
    d->dft_hv_scanmode_i = 0; d->dft_hv_scan_reset_ni = 1;
    d->to_axi_l3_b_lo_bits_i = 0; d->to_axi_l3_b_valid_lo_i = 0; d->to_axi_l3_b_ready_i = 1;
    d->to_axi_l3_r_valid_lo_i = 0; d->to_axi_l3_r_ready_i = 1;
    d->to_axi_sys_b_lo_bits_i = 0; d->to_axi_sys_b_valid_lo_i = 0; d->to_axi_sys_b_ready_i = 1;
    d->to_axi_sys_r_valid_lo_i = 0; d->to_axi_sys_r_ready_i = 1;
    d->from_axi_sys_ar_valid_lo_i = 0; d->from_axi_sys_ar_ready_i = 1;
    d->from_axi_sys_aw_valid_lo_i = 0; d->from_axi_sys_aw_vcvalid_lo_i = 0;
    d->from_axi_sys_uc_aw_ready_i = 1; d->from_axi_sys_sbm_aw_ready_i = 1;
    d->from_axi_sys_w_valid_lo_i = 0; d->from_axi_sys_w_vcvalid_lo_i = 0;
    d->from_axi_sys_uc_w_ready_i = 1; d->from_axi_sys_sbm_w_ready_i = 1;
}

int main(int argc, char **argv) {
    CosimCtrl<Vcosim_uncached_lo_voltage_cross_tb> sim(argc, argv);
    std::printf("──── cosim uncached_lo_voltage_cross ────\n");
    init_inputs(sim.dut.get());
    sim.reset();
    for (int i = 0; i < 16; ++i) sim.tick();
    compare_all(sim);

    uint32_t seed = 0x55667788u;
    for (int i = 0; i < 4096; ++i) {
        auto *d = sim.dut.get();
        d->to_axi_l3_b_valid_lo_i = 0; d->to_axi_l3_r_valid_lo_i = 0;
        d->to_axi_sys_b_valid_lo_i = 0; d->to_axi_sys_r_valid_lo_i = 0;
        d->from_axi_sys_ar_valid_lo_i = 0;
        d->from_axi_sys_aw_valid_lo_i = 0; d->from_axi_sys_aw_vcvalid_lo_i = 0;
        d->from_axi_sys_w_valid_lo_i = 0; d->from_axi_sys_w_vcvalid_lo_i = 0;

        switch (i & 0x7f) {
            case 0x00: d->to_axi_l3_b_lo_bits_i = xorshift32(seed) & 0x7ffu; d->to_axi_l3_b_valid_lo_i = 1; break;
            case 0x08: fill_words(d->to_axi_l3_r_lo_bits_i, 17, seed); d->to_axi_l3_r_valid_lo_i = 1; break;
            case 0x18: d->to_axi_sys_b_lo_bits_i = xorshift32(seed) & 0x7ffu; d->to_axi_sys_b_valid_lo_i = 1; break;
            case 0x20: fill_words(d->to_axi_sys_r_lo_bits_i, 17, seed); d->to_axi_sys_r_valid_lo_i = 1; break;
            case 0x30: fill_words(d->from_axi_sys_ar_lo_bits_i, 3, seed); d->from_axi_sys_ar_valid_lo_i = 1; break;
            case 0x40: fill_words(d->from_axi_sys_aw_lo_bits_i, 3, seed); d->from_axi_sys_aw_valid_lo_i = 1; d->from_axi_sys_aw_vcvalid_lo_i = 1; break;
            case 0x48: fill_words(d->from_axi_sys_aw_lo_bits_i, 3, seed); d->from_axi_sys_aw_valid_lo_i = 1; d->from_axi_sys_aw_vcvalid_lo_i = 2; break;
            case 0x58: fill_words(d->from_axi_sys_w_lo_bits_i, 10, seed); d->from_axi_sys_w_valid_lo_i = 1; d->from_axi_sys_w_vcvalid_lo_i = 1; break;
            case 0x60: fill_words(d->from_axi_sys_w_lo_bits_i, 10, seed); d->from_axi_sys_w_valid_lo_i = 1; d->from_axi_sys_w_vcvalid_lo_i = 2; break;
            default: break;
        }

        d->to_axi_l3_b_ready_i = ((i & 0xff) != 0xa1);
        d->to_axi_l3_r_ready_i = ((i & 0xff) != 0xa2);
        d->to_axi_sys_b_ready_i = ((i & 0xff) != 0xa3);
        d->to_axi_sys_r_ready_i = ((i & 0xff) != 0xa4);
        d->from_axi_sys_ar_ready_i = ((i & 0xff) != 0xa5);
        d->from_axi_sys_uc_aw_ready_i = ((i & 0xff) != 0xa6);
        d->from_axi_sys_sbm_aw_ready_i = ((i & 0xff) != 0xa7);
        d->from_axi_sys_uc_w_ready_i = ((i & 0xff) != 0xa8);
        d->from_axi_sys_sbm_w_ready_i = ((i & 0xff) != 0xa9);
        d->dft_lv_scanmode_i = (i == 1024);
        d->dft_hv_scanmode_i = (i == 2048);
        d->dft_lv_scan_reset_ni = 1;
        d->dft_hv_scan_reset_ni = 1;

        sim.tick();
        compare_all(sim);
    }
    return sim.finish();
}
