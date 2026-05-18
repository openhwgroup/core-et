// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncacheable_noc_rsp_tb.h"
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
static void compare_words(CosimCtrl<Vcosim_uncacheable_noc_rsp_tb>& sim,
                          const char* name, const T& orig, const U& nw, int n) {
    for (int i = 0; i < n; ++i) {
        sim.compare(std::string(name) + "[" + std::to_string(i) + "]", orig[i], nw[i]);
    }
}

static void compare_all(CosimCtrl<Vcosim_uncacheable_noc_rsp_tb>& sim) {
    compare_words(sim, "rsp_data", sim.dut->orig_rsp_data_bits_o, sim.dut->new_rsp_data_bits_o, 18);
    sim.compare("rsp_valid", sim.dut->orig_rsp_valid_o, sim.dut->new_rsp_valid_o);
    sim.compare("reqq_index", sim.dut->orig_reqq_index_o, sim.dut->new_reqq_index_o);
    sim.compare("reqq_entry_clear", sim.dut->orig_reqq_entry_clear_bits_o, sim.dut->new_reqq_entry_clear_bits_o);
    sim.compare("broadcast_new_ack", sim.dut->orig_broadcast_new_ack_o, sim.dut->new_broadcast_new_ack_o);
    sim.compare("broadcast_req_done", sim.dut->orig_broadcast_req_done_o, sim.dut->new_broadcast_req_done_o);
    sim.compare("broadcast_rsp_is_error", sim.dut->orig_broadcast_rsp_is_error_o, sim.dut->new_broadcast_rsp_is_error_o);
    sim.compare("credit_return", sim.dut->orig_credit_return_o, sim.dut->new_credit_return_o);
    sim.compare("from_sys_aw_ready", sim.dut->orig_from_sys_aw_ready_o, sim.dut->new_from_sys_aw_ready_o);
    sim.compare("from_sys_w_ready", sim.dut->orig_from_sys_w_ready_o, sim.dut->new_from_sys_w_ready_o);
    sim.compare("from_sys_b", sim.dut->orig_from_sys_b_bits_o, sim.dut->new_from_sys_b_bits_o);
    sim.compare("from_sys_b_valid", sim.dut->orig_from_sys_b_valid_o, sim.dut->new_from_sys_b_valid_o);
    sim.compare("to_l3_b_ready", sim.dut->orig_to_l3_b_ready_o, sim.dut->new_to_l3_b_ready_o);
    sim.compare("to_l3_r_ready", sim.dut->orig_to_l3_r_ready_o, sim.dut->new_to_l3_r_ready_o);
    sim.compare("to_sys_b_ready", sim.dut->orig_to_sys_b_ready_o, sim.dut->new_to_sys_b_ready_o);
    sim.compare("to_sys_r_ready", sim.dut->orig_to_sys_r_ready_o, sim.dut->new_to_sys_r_ready_o);
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_uncacheable_noc_rsp_tb> sim(argc, argv);
    for (int i = 0; i < 12; ++i) sim.dut->reqq_entry_bits_i[i] = 0;
    for (int i = 0; i < 3; ++i) sim.dut->from_sys_aw_bits_i[i] = 0;
    for (int i = 0; i < 10; ++i) sim.dut->from_sys_w_bits_i[i] = 0;
    sim.dut->to_l3_b_bits_i = 0;
    sim.dut->to_sys_b_bits_i = 0;
    for (int i = 0; i < 17; ++i) {
        sim.dut->to_l3_r_bits_i[i] = 0;
        sim.dut->to_sys_r_bits_i[i] = 0;
    }
    sim.dut->rsp_send_i = 0;
    sim.dut->reqq_entry_valid_i = 0;
    sim.dut->broadcast_rsp_is_error_reg_i = 0;
    sim.dut->broadcast_acks_i = 0;
    sim.dut->broadcast_req_index_i = 0;
    sim.dut->credit_i = 0;
    sim.dut->from_sys_aw_valid_i = 0;
    sim.dut->from_sys_w_valid_i = 0;
    sim.dut->from_sys_b_ready_i = 1;
    sim.dut->remote_scp_enabled_i = 0;
    sim.dut->l3_enabled_i = 0;
    sim.dut->to_l3_b_valid_i = 0;
    sim.dut->to_l3_r_valid_i = 0;
    sim.dut->to_sys_b_valid_i = 0;
    sim.dut->to_sys_r_valid_i = 0;
    sim.reset();
    compare_all(sim);

    uint32_t seed = 0xc012050u;
    for (int i = 0; i < 8192; ++i) {
        uint32_t r = (i < 1024) ? static_cast<uint32_t>(i) : xorshift32(seed);
        fill_words(sim.dut->reqq_entry_bits_i, 12, seed);
        fill_words(sim.dut->from_sys_aw_bits_i, 3, seed);
        fill_words(sim.dut->from_sys_w_bits_i, 10, seed);
        fill_words(sim.dut->to_l3_r_bits_i, 17, seed);
        fill_words(sim.dut->to_sys_r_bits_i, 17, seed);
        sim.dut->to_l3_b_bits_i = xorshift32(seed) & 0x7ffu;
        sim.dut->to_sys_b_bits_i = xorshift32(seed) & 0x7ffu;

        sim.dut->rsp_send_i = ((r >> 0) & 1) || (i % 3 == 0);
        sim.dut->reqq_entry_valid_i = (r >> 1) & 1;
        sim.dut->broadcast_rsp_is_error_reg_i = (r >> 2) & 1;
        sim.dut->broadcast_acks_i = (r >> 3) & 0xf;
        if ((i & 127) == 3) sim.dut->broadcast_acks_i = 1;
        sim.dut->broadcast_req_index_i = (r >> 7) & 0x3;
        sim.dut->credit_i = (r >> 9) & 1;
        sim.dut->from_sys_aw_valid_i = (r >> 10) & 1;
        sim.dut->from_sys_w_valid_i = (r >> 11) & 1;
        sim.dut->from_sys_b_ready_i = (r >> 12) & 1;
        sim.dut->remote_scp_enabled_i = (r >> 13) & 1;
        sim.dut->l3_enabled_i = (r >> 14) & 1;
        sim.dut->to_l3_b_valid_i = (r >> 15) & 1;
        sim.dut->to_l3_r_valid_i = (r >> 16) & 1;
        sim.dut->to_sys_b_valid_i = (r >> 17) & 1;
        sim.dut->to_sys_r_valid_i = (r >> 18) & 1;
        // Sweep IDs/RESP low bits and avoid starving the FIFO drain.
        sim.dut->to_l3_b_bits_i = (sim.dut->to_l3_b_bits_i & ~0xffu) | (r & 0xffu);
        sim.dut->to_sys_b_bits_i = (sim.dut->to_sys_b_bits_i & ~0xffu) | ((r >> 2) & 0xffu);
        sim.dut->to_l3_r_bits_i[0] = (sim.dut->to_l3_r_bits_i[0] & ~0xffu) | ((r >> 4) & 0xffu);
        sim.dut->to_sys_r_bits_i[0] = (sim.dut->to_sys_r_bits_i[0] & ~0xffu) | ((r >> 6) & 0xffu);
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}
