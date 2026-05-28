// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncacheable_noc_slv_handler_tb.h"
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
static void compare_words(CosimCtrl<Vcosim_uncacheable_noc_slv_handler_tb>& sim,
                          const char* name, const T& orig, const U& nw, int n) {
    for (int i = 0; i < n; ++i) {
        sim.compare(std::string(name) + "[" + std::to_string(i) + "]", orig[i], nw[i]);
    }
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_uncacheable_noc_slv_handler_tb> sim(argc, argv);
    for (int i = 0; i < 12; ++i) sim.dut->reqq_entry_bits_i[i] = 0;
    for (int i = 0; i < 3; ++i) sim.dut->from_sys_aw_bits_i[i] = 0;
    for (int i = 0; i < 10; ++i) sim.dut->from_sys_w_bits_i[i] = 0;
    sim.dut->reqq_entry_valid_i = 0;
    sim.dut->credit_i = 0;
    sim.dut->from_sys_aw_valid_i = 0;
    sim.dut->from_sys_w_valid_i = 0;
    sim.reset();

    uint32_t seed = 0x51d5a11u;
    for (int i = 0; i < 4096; ++i) {
        uint32_t r = (i < 256) ? static_cast<uint32_t>(i) : xorshift32(seed);
        fill_words(sim.dut->reqq_entry_bits_i, 12, seed);
        fill_words(sim.dut->from_sys_aw_bits_i, 3, seed);
        fill_words(sim.dut->from_sys_w_bits_i, 10, seed);
        sim.dut->reqq_entry_valid_i = (r >> 0) & 1;
        sim.dut->credit_i = (r >> 1) & 1;
        sim.dut->from_sys_aw_valid_i = (r >> 2) & 1;
        sim.dut->from_sys_w_valid_i = (r >> 3) & 1;
        // Sweep AXI size/len/user fields and data-valid combinations.
        sim.dut->from_sys_aw_bits_i[0] = (sim.dut->from_sys_aw_bits_i[0] & ~0xffffu) | (r & 0xffffu);
        sim.tick();
        sim.compare("reqq_index", sim.dut->orig_reqq_index_o, sim.dut->new_reqq_index_o);
        sim.compare("reqq_entry_clear", sim.dut->orig_reqq_entry_clear_bits_o, sim.dut->new_reqq_entry_clear_bits_o);
        sim.compare("new_rsp", sim.dut->orig_new_rsp_o, sim.dut->new_new_rsp_o);
        compare_words(sim, "rsp_info", sim.dut->orig_rsp_info_bits_o, sim.dut->new_rsp_info_bits_o, 18);
        sim.compare("from_sys_b", sim.dut->orig_from_sys_b_bits_o, sim.dut->new_from_sys_b_bits_o);
        sim.compare("from_sys_b_valid", sim.dut->orig_from_sys_b_valid_o, sim.dut->new_from_sys_b_valid_o);
    }

    return sim.finish();
}
