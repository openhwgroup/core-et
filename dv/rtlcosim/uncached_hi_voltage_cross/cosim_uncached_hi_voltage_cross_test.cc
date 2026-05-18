// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncached_hi_voltage_cross_tb.h"
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

template <typename T>
static void drive_source(T& words, int n, CData& valid, bool ready,
                         bool next_valid, uint32_t& seed) {
    // AXI source payloads must remain stable while VALID is high and READY is low.
    if (!valid || ready) {
        valid = next_valid ? 1 : 0;
        if (valid) fill_words(words, n, seed);
    }
}

static void drive_scalar_source(IData& word, CData& valid, bool ready,
                                bool next_valid, uint32_t& seed) {
    if (!valid || ready) {
        valid = next_valid ? 1 : 0;
        if (valid) word = xorshift32(seed) & 0x1fffffu;
    }
}

template <typename T, typename U>
static void compare_words(CosimCtrl<Vcosim_uncached_hi_voltage_cross_tb>& sim,
                          const char* name, const T& orig, const U& nw,
                          int n, int valid_bits) {
    for (int i = 0; i < n; ++i) {
        const int remaining = valid_bits - (i * 32);
        const uint32_t mask = (remaining >= 32) ? 0xffffffffu : ((1u << remaining) - 1u);
        sim.compare(std::string(name)+"["+std::to_string(i)+"]", orig[i] & mask, nw[i] & mask);
    }
}

static void compare_all(CosimCtrl<Vcosim_uncached_hi_voltage_cross_tb>& sim) {
    sim.compare("l3_ar_ready", sim.dut->orig_to_axi_l3_ar_ready_o, sim.dut->new_to_axi_l3_ar_ready_o);
    sim.compare("l3_aw_ready", sim.dut->orig_to_axi_l3_aw_ready_o, sim.dut->new_to_axi_l3_aw_ready_o);
    sim.compare("l3_w_ready", sim.dut->orig_to_axi_l3_w_ready_o, sim.dut->new_to_axi_l3_w_ready_o);
    sim.compare("l3_ar_valid_lo", sim.dut->orig_to_axi_l3_ar_valid_lo_o, sim.dut->new_to_axi_l3_ar_valid_lo_o);
    if (sim.dut->orig_to_axi_l3_ar_valid_lo_o && sim.dut->new_to_axi_l3_ar_valid_lo_o) {
        compare_words(sim, "l3_ar_lo", sim.dut->orig_to_axi_l3_ar_lo_bits_o,
                      sim.dut->new_to_axi_l3_ar_lo_bits_o, 3, 75);
    }
    sim.compare("l3_aw_valid_lo", sim.dut->orig_to_axi_l3_aw_valid_lo_o, sim.dut->new_to_axi_l3_aw_valid_lo_o);
    if (sim.dut->orig_to_axi_l3_aw_valid_lo_o && sim.dut->new_to_axi_l3_aw_valid_lo_o) {
        compare_words(sim, "l3_aw_lo", sim.dut->orig_to_axi_l3_aw_lo_bits_o,
                      sim.dut->new_to_axi_l3_aw_lo_bits_o, 3, 79);
    }
    sim.compare("l3_w_valid_lo", sim.dut->orig_to_axi_l3_w_valid_lo_o, sim.dut->new_to_axi_l3_w_valid_lo_o);
    if (sim.dut->orig_to_axi_l3_w_valid_lo_o && sim.dut->new_to_axi_l3_w_valid_lo_o) {
        compare_words(sim, "l3_w_lo", sim.dut->orig_to_axi_l3_w_lo_bits_o,
                      sim.dut->new_to_axi_l3_w_lo_bits_o, 19, 577);
    }
    sim.compare("sys_ar_ready", sim.dut->orig_to_axi_sys_ar_ready_o, sim.dut->new_to_axi_sys_ar_ready_o);
    sim.compare("sys_aw_ready", sim.dut->orig_to_axi_sys_aw_ready_o, sim.dut->new_to_axi_sys_aw_ready_o);
    sim.compare("sys_w_ready", sim.dut->orig_to_axi_sys_w_ready_o, sim.dut->new_to_axi_sys_w_ready_o);
    sim.compare("sys_ar_valid_lo", sim.dut->orig_to_axi_sys_ar_valid_lo_o, sim.dut->new_to_axi_sys_ar_valid_lo_o);
    if (sim.dut->orig_to_axi_sys_ar_valid_lo_o && sim.dut->new_to_axi_sys_ar_valid_lo_o) {
        compare_words(sim, "sys_ar_lo", sim.dut->orig_to_axi_sys_ar_lo_bits_o,
                      sim.dut->new_to_axi_sys_ar_lo_bits_o, 3, 75);
    }
    sim.compare("sys_aw_valid_lo", sim.dut->orig_to_axi_sys_aw_valid_lo_o, sim.dut->new_to_axi_sys_aw_valid_lo_o);
    if (sim.dut->orig_to_axi_sys_aw_valid_lo_o && sim.dut->new_to_axi_sys_aw_valid_lo_o) {
        compare_words(sim, "sys_aw_lo", sim.dut->orig_to_axi_sys_aw_lo_bits_o,
                      sim.dut->new_to_axi_sys_aw_lo_bits_o, 3, 79);
    }
    sim.compare("sys_w_valid_lo", sim.dut->orig_to_axi_sys_w_valid_lo_o, sim.dut->new_to_axi_sys_w_valid_lo_o);
    if (sim.dut->orig_to_axi_sys_w_valid_lo_o && sim.dut->new_to_axi_sys_w_valid_lo_o) {
        compare_words(sim, "sys_w_lo", sim.dut->orig_to_axi_sys_w_lo_bits_o,
                      sim.dut->new_to_axi_sys_w_lo_bits_o, 19, 577);
    }
    sim.compare("sys_r_ready", sim.dut->orig_from_axi_sys_r_ready_o, sim.dut->new_from_axi_sys_r_ready_o);
    sim.compare("sys_r_valid_lo", sim.dut->orig_from_axi_sys_r_valid_lo_o, sim.dut->new_from_axi_sys_r_valid_lo_o);
    if (sim.dut->orig_from_axi_sys_r_valid_lo_o && sim.dut->new_from_axi_sys_r_valid_lo_o) {
        compare_words(sim, "sys_r_lo", sim.dut->orig_from_axi_sys_r_lo_bits_o,
                      sim.dut->new_from_axi_sys_r_lo_bits_o, 9, 278);
    }
    sim.compare("sys_b_ready", sim.dut->orig_from_axi_sys_b_ready_o, sim.dut->new_from_axi_sys_b_ready_o);
    sim.compare("sys_credit_lo", sim.dut->orig_from_axi_sys_credit_lo_o, sim.dut->new_from_axi_sys_credit_lo_o);
    sim.compare("sys_b_valid_lo", sim.dut->orig_from_axi_sys_b_valid_lo_o, sim.dut->new_from_axi_sys_b_valid_lo_o);
    if (sim.dut->orig_from_axi_sys_b_valid_lo_o && sim.dut->new_from_axi_sys_b_valid_lo_o) {
        sim.compare("sys_b_lo", sim.dut->orig_from_axi_sys_b_lo_bits_o, sim.dut->new_from_axi_sys_b_lo_bits_o);
    }
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_uncached_hi_voltage_cross_tb> sim(argc, argv);
    sim.dut->dft_hv_scanmode_i = 0; sim.dut->dft_hv_scan_reset_ni = 1;
    sim.dut->dft_lv_scanmode_i = 0; sim.dut->dft_lv_scan_reset_ni = 1;
    for (int i=0;i<3;i++) { sim.dut->to_axi_l3_ar_bits_i[i]=0; sim.dut->to_axi_l3_aw_bits_i[i]=0; sim.dut->to_axi_sys_ar_bits_i[i]=0; sim.dut->to_axi_sys_aw_bits_i[i]=0; }
    for (int i=0;i<19;i++) { sim.dut->to_axi_l3_w_bits_i[i]=0; sim.dut->to_axi_sys_w_bits_i[i]=0; }
    for (int i=0;i<9;i++) sim.dut->from_axi_sys_r_bits_i[i]=0;
    sim.dut->from_axi_sys_b_bits_i=0; sim.dut->from_axi_sys_credit_i=0;
    sim.dut->to_axi_l3_ar_valid_i=0; sim.dut->to_axi_l3_aw_valid_i=0; sim.dut->to_axi_l3_w_valid_i=0;
    sim.dut->to_axi_sys_ar_valid_i=0; sim.dut->to_axi_sys_aw_valid_i=0; sim.dut->to_axi_sys_w_valid_i=0;
    sim.dut->from_axi_sys_r_valid_i=0; sim.dut->from_axi_sys_b_valid_i=0;
    sim.dut->to_axi_l3_ar_ready_lo_i=1; sim.dut->to_axi_l3_aw_ready_lo_i=1; sim.dut->to_axi_l3_w_ready_lo_i=1;
    sim.dut->to_axi_sys_ar_ready_lo_i=1; sim.dut->to_axi_sys_aw_ready_lo_i=1; sim.dut->to_axi_sys_w_ready_lo_i=1;
    sim.dut->from_axi_sys_r_ready_lo_i=1; sim.dut->from_axi_sys_b_ready_lo_i=1;
    sim.reset();
    for (int i = 0; i < 16; ++i) sim.tick();
    compare_all(sim);

    uint32_t seed=0x91827364u;
    for (int i=0;i<4096;i++) {
        uint32_t r=(i<512)?(uint32_t)i:xorshift32(seed);
        sim.dut->to_axi_l3_ar_valid_i=0; sim.dut->to_axi_l3_aw_valid_i=0; sim.dut->to_axi_l3_w_valid_i=0;
        sim.dut->to_axi_sys_ar_valid_i=0; sim.dut->to_axi_sys_aw_valid_i=0; sim.dut->to_axi_sys_w_valid_i=0;
        sim.dut->from_axi_sys_r_valid_i=0; sim.dut->from_axi_sys_b_valid_i=0;
        switch (i & 0x3f) {
            case 0x00: fill_words(sim.dut->to_axi_l3_ar_bits_i,3,seed); sim.dut->to_axi_l3_ar_valid_i=1; break;
            case 0x04: fill_words(sim.dut->to_axi_l3_aw_bits_i,3,seed); sim.dut->to_axi_l3_aw_valid_i=1; break;
            case 0x05: fill_words(sim.dut->to_axi_l3_w_bits_i,19,seed); sim.dut->to_axi_l3_w_valid_i=1; break;
            case 0x10: fill_words(sim.dut->to_axi_sys_ar_bits_i,3,seed); sim.dut->to_axi_sys_ar_valid_i=1; break;
            case 0x14: fill_words(sim.dut->to_axi_sys_aw_bits_i,3,seed); sim.dut->to_axi_sys_aw_valid_i=1; break;
            case 0x15: fill_words(sim.dut->to_axi_sys_w_bits_i,19,seed); sim.dut->to_axi_sys_w_valid_i=1; break;
            case 0x20: fill_words(sim.dut->from_axi_sys_r_bits_i,9,seed); sim.dut->from_axi_sys_r_valid_i=1; break;
            case 0x28: sim.dut->from_axi_sys_b_bits_i=xorshift32(seed)&0x1fffffu; sim.dut->from_axi_sys_b_valid_i=1; break;
            default: break;
        }
        sim.dut->from_axi_sys_credit_i=((i & 0x7f) == 0x30) ? 1 : (((i & 0x7f) == 0x31) ? 2 : 0);
        sim.dut->to_axi_l3_ar_ready_lo_i=((i & 0x7f) != 0x41); sim.dut->to_axi_l3_aw_ready_lo_i=((i & 0x7f) != 0x42); sim.dut->to_axi_l3_w_ready_lo_i=((i & 0x7f) != 0x43);
        sim.dut->to_axi_sys_ar_ready_lo_i=((i & 0x7f) != 0x44); sim.dut->to_axi_sys_aw_ready_lo_i=((i & 0x7f) != 0x45); sim.dut->to_axi_sys_w_ready_lo_i=((i & 0x7f) != 0x46);
        sim.dut->from_axi_sys_r_ready_lo_i=((i & 0x7f) != 0x47); sim.dut->from_axi_sys_b_ready_lo_i=((i & 0x7f) != 0x48);
        sim.dut->dft_hv_scanmode_i=(i==1024); sim.dut->dft_lv_scanmode_i=(i==2048);
        sim.dut->dft_hv_scan_reset_ni=1; sim.dut->dft_lv_scan_reset_ni=1;
        (void)r;
        sim.tick(); compare_all(sim);
    }
    return sim.finish();
}
