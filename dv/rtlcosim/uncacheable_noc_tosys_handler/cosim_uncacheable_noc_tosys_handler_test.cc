// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncacheable_noc_tosys_handler_tb.h"
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
static void compare_words(CosimCtrl<Vcosim_uncacheable_noc_tosys_handler_tb>& sim,
                          const char* name, const T& orig, const U& nw, int n) {
    for (int i = 0; i < n; ++i) {
        sim.compare(std::string(name) + "[" + std::to_string(i) + "]", orig[i], nw[i]);
    }
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_uncacheable_noc_tosys_handler_tb> sim(argc, argv);
    for (int i = 0; i < 12; ++i) sim.dut->reqq_entry_bits_i[i] = 0;
    sim.dut->to_sys_b_bits_i = 0;
    for (int i = 0; i < 17; ++i) sim.dut->to_sys_r_bits_i[i] = 0;
    sim.dut->wr_reqq_entry_valid_i = 0;
    sim.dut->rd_reqq_entry_valid_i = 0;
    sim.dut->broadcast_req_index_i = 0;
    sim.dut->broadcast_rsp_is_error_reg_i = 0;
    sim.dut->broadcast_acks_i = 0;
    sim.dut->to_sys_b_valid_i = 0;
    sim.dut->to_sys_r_valid_i = 0;
    sim.reset();

    uint32_t seed = 0x70595u;
    for (int i = 0; i < 4096; ++i) {
        uint32_t r = (i < 512) ? static_cast<uint32_t>(i) : xorshift32(seed);
        fill_words(sim.dut->reqq_entry_bits_i, 12, seed);
        fill_words(sim.dut->to_sys_r_bits_i, 17, seed);
        sim.dut->to_sys_b_bits_i = xorshift32(seed) & 0x7ffu;
        sim.dut->wr_reqq_entry_valid_i = (r >> 0) & 1;
        sim.dut->rd_reqq_entry_valid_i = (r >> 1) & 1;
        sim.dut->broadcast_req_index_i = (r >> 2) & 0x3;
        sim.dut->broadcast_rsp_is_error_reg_i = (r >> 4) & 1;
        sim.dut->broadcast_acks_i = (r >> 5) & 0xf;
        if ((i & 31) == 0) sim.dut->broadcast_acks_i = 1;
        sim.dut->to_sys_b_valid_i = (r >> 9) & 1;
        sim.dut->to_sys_r_valid_i = (r >> 10) & 1;
        // Sweep low ID bits; values >= REQQ_SIZE cover broadcast response ID handling.
        sim.dut->to_sys_b_bits_i = (sim.dut->to_sys_b_bits_i & ~0xffu) | (r & 0xffu);
        sim.dut->to_sys_r_bits_i[0] = (sim.dut->to_sys_r_bits_i[0] & ~0xffu) | ((r >> 1) & 0xffu);
        sim.tick();
        sim.compare("wr_reqq_index", sim.dut->orig_wr_reqq_index_o, sim.dut->new_wr_reqq_index_o);
        sim.compare("rd_reqq_index", sim.dut->orig_rd_reqq_index_o, sim.dut->new_rd_reqq_index_o);
        sim.compare("wr_reqq_entry_clear", sim.dut->orig_wr_reqq_entry_clear_bits_o, sim.dut->new_wr_reqq_entry_clear_bits_o);
        sim.compare("rd_reqq_entry_clear", sim.dut->orig_rd_reqq_entry_clear_bits_o, sim.dut->new_rd_reqq_entry_clear_bits_o);
        sim.compare("broadcast_new_ack", sim.dut->orig_broadcast_new_ack_o, sim.dut->new_broadcast_new_ack_o);
        sim.compare("broadcast_req_done", sim.dut->orig_broadcast_req_done_o, sim.dut->new_broadcast_req_done_o);
        sim.compare("broadcast_rsp_is_error", sim.dut->orig_broadcast_rsp_is_error_o, sim.dut->new_broadcast_rsp_is_error_o);
        sim.compare("wr_new_rsp", sim.dut->orig_wr_new_rsp_o, sim.dut->new_wr_new_rsp_o);
        sim.compare("rd_new_rsp", sim.dut->orig_rd_new_rsp_o, sim.dut->new_rd_new_rsp_o);
        compare_words(sim, "wr_rsp_info", sim.dut->orig_wr_rsp_info_bits_o, sim.dut->new_wr_rsp_info_bits_o, 18);
        compare_words(sim, "rd_rsp_info", sim.dut->orig_rd_rsp_info_bits_o, sim.dut->new_rd_rsp_info_bits_o, 18);
    }

    return sim.finish();
}
