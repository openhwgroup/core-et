// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_noc_etlink_to_axi_tb.h"
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
static void compare_words(CosimCtrl<Vcosim_noc_etlink_to_axi_tb>& sim,
                          const char* name, const T& orig, const U& nw, int n) {
    for (int i = 0; i < n; ++i) {
        sim.compare(std::string(name) + "[" + std::to_string(i) + "]", orig[i], nw[i]);
    }
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_noc_etlink_to_axi_tb> sim(argc, argv);
    sim.dut->uc_config_qos_dram_i = 0;
    sim.dut->wr_req_axi_valid_i = 0;
    sim.dut->rd_req_axi_valid_i = 0;
    sim.dut->to_sys_ar_ready_i = 0;
    sim.dut->to_sys_aw_ready_i = 0;
    sim.dut->to_sys_w_ready_i = 0;
    sim.dut->to_l3_ar_ready_i = 0;
    sim.dut->to_l3_aw_ready_i = 0;
    sim.dut->to_l3_w_ready_i = 0;
    for (int i = 0; i < 18; ++i) sim.dut->wr_req_axi_info_bits_i[i] = 0;
    sim.dut->rd_req_axi_info_bits_i = 0;
    sim.reset();

    uint32_t seed = 0x0e7a117u;
    for (int i = 0; i < 4096; ++i) {
        uint32_t r = (i < 512) ? static_cast<uint32_t>(i) : xorshift32(seed);
        sim.dut->uc_config_qos_dram_i = (r >> 0) & 1;
        sim.dut->wr_req_axi_valid_i = (r >> 1) & 1;
        sim.dut->rd_req_axi_valid_i = (r >> 2) & 1;
        sim.dut->to_sys_ar_ready_i = (r >> 3) & 1;
        sim.dut->to_sys_aw_ready_i = (r >> 4) & 1;
        sim.dut->to_sys_w_ready_i = (r >> 5) & 1;
        sim.dut->to_l3_ar_ready_i = (r >> 6) & 1;
        sim.dut->to_l3_aw_ready_i = (r >> 7) & 1;
        sim.dut->to_l3_w_ready_i = (r >> 8) & 1;
        fill_words(sim.dut->wr_req_axi_info_bits_i, 18, seed);
        sim.dut->rd_req_axi_info_bits_i = (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed);
        if (i < 128) {
            // Sweep legal opcode/size/l3 combinations early to hit each decode/QOS/STRB path.
            sim.dut->wr_req_axi_info_bits_i[0] = (sim.dut->wr_req_axi_info_bits_i[0] & ~0x3ffffu) |
                                                ((i & 0x1f) << 13) | ((i & 0x7) << 10) | ((i >> 3) & 1);
            sim.dut->rd_req_axi_info_bits_i = (sim.dut->rd_req_axi_info_bits_i & ~0x3ffffULL) |
                                              (static_cast<uint64_t>((i & 0x1f) << 13) |
                                               static_cast<uint64_t>((i & 0x7) << 10) |
                                               static_cast<uint64_t>((i >> 3) & 1));
        }
        sim.tick();
        sim.compare("ready", sim.dut->orig_ready_o, sim.dut->new_ready_o);
        sim.compare("to_sys_ar_valid", sim.dut->orig_to_sys_ar_valid_o, sim.dut->new_to_sys_ar_valid_o);
        sim.compare("to_sys_aw_valid", sim.dut->orig_to_sys_aw_valid_o, sim.dut->new_to_sys_aw_valid_o);
        sim.compare("to_sys_w_valid", sim.dut->orig_to_sys_w_valid_o, sim.dut->new_to_sys_w_valid_o);
        sim.compare("to_l3_ar_valid", sim.dut->orig_to_l3_ar_valid_o, sim.dut->new_to_l3_ar_valid_o);
        sim.compare("to_l3_aw_valid", sim.dut->orig_to_l3_aw_valid_o, sim.dut->new_to_l3_aw_valid_o);
        sim.compare("to_l3_w_valid", sim.dut->orig_to_l3_w_valid_o, sim.dut->new_to_l3_w_valid_o);
        compare_words(sim, "to_sys_ar", sim.dut->orig_to_sys_ar_bits_o, sim.dut->new_to_sys_ar_bits_o, 3);
        compare_words(sim, "to_sys_aw", sim.dut->orig_to_sys_aw_bits_o, sim.dut->new_to_sys_aw_bits_o, 3);
        compare_words(sim, "to_l3_ar", sim.dut->orig_to_l3_ar_bits_o, sim.dut->new_to_l3_ar_bits_o, 3);
        compare_words(sim, "to_l3_aw", sim.dut->orig_to_l3_aw_bits_o, sim.dut->new_to_l3_aw_bits_o, 3);
        compare_words(sim, "to_sys_w", sim.dut->orig_to_sys_w_bits_o, sim.dut->new_to_sys_w_bits_o, 19);
        compare_words(sim, "to_l3_w", sim.dut->orig_to_l3_w_bits_o, sim.dut->new_to_l3_w_bits_o, 19);
    }

    return sim.finish();
}
