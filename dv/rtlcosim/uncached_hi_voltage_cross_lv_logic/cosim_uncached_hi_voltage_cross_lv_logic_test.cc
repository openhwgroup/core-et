// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncached_hi_voltage_cross_lv_logic_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

static uint32_t xorshift32(uint32_t& s) {
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return s;
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_uncached_hi_voltage_cross_lv_logic_tb> sim(argc, argv);
    sim.dut->to_axi_l3_ar_ready_lo_i = 0;
    sim.dut->to_axi_l3_ar_vcfifo_valid_i = 0;
    sim.dut->to_axi_l3_aw_ready_lo_i = 0;
    sim.dut->to_axi_l3_w_ready_lo_i = 0;
    sim.dut->to_axi_l3_aw_vcfifo_valid_i = 0;
    sim.dut->to_axi_l3_w_vcfifo_valid_i = 0;
    sim.dut->to_axi_sys_ar_ready_lo_i = 0;
    sim.dut->to_axi_sys_ar_vcfifo_valid_i = 0;
    sim.dut->to_axi_sys_aw_ready_lo_i = 0;
    sim.dut->to_axi_sys_w_ready_lo_i = 0;
    sim.dut->to_axi_sys_aw_vcfifo_valid_i = 0;
    sim.dut->to_axi_sys_w_vcfifo_valid_i = 0;
    sim.dut->from_axi_sys_b_ready_lo_i = 0;
    sim.dut->from_axi_sys_b_vcfifo_valid_i = 0;
    sim.dut->from_axi_sys_b_w_bits_i = 0;
    sim.dut->from_axi_sys_r_ready_lo_i = 0;
    sim.dut->from_axi_sys_r_vcfifo_valid_i = 0;
    sim.reset();

    uint32_t seed = 0x48495643u;
    for (int i = 0; i < 4096; ++i) {
        uint32_t r = (i < 512) ? static_cast<uint32_t>(i) : xorshift32(seed);
        sim.dut->to_axi_l3_ar_ready_lo_i = (r >> 0) & 1;
        sim.dut->to_axi_l3_ar_vcfifo_valid_i = (r >> 1) & 1;
        sim.dut->to_axi_l3_aw_ready_lo_i = (r >> 2) & 1;
        sim.dut->to_axi_l3_w_ready_lo_i = (r >> 3) & 1;
        sim.dut->to_axi_l3_aw_vcfifo_valid_i = (r >> 4) & 1;
        sim.dut->to_axi_l3_w_vcfifo_valid_i = (r >> 5) & 1;
        sim.dut->to_axi_sys_ar_ready_lo_i = (r >> 6) & 1;
        sim.dut->to_axi_sys_ar_vcfifo_valid_i = (r >> 7) & 1;
        sim.dut->to_axi_sys_aw_ready_lo_i = (r >> 8) & 1;
        sim.dut->to_axi_sys_w_ready_lo_i = (r >> 9) & 1;
        sim.dut->to_axi_sys_aw_vcfifo_valid_i = (r >> 10) & 1;
        sim.dut->to_axi_sys_w_vcfifo_valid_i = (r >> 11) & 1;
        sim.dut->from_axi_sys_b_ready_lo_i = (r >> 12) & 1;
        sim.dut->from_axi_sys_b_vcfifo_valid_i = (r >> 13) & 1;
        sim.dut->from_axi_sys_r_ready_lo_i = (r >> 14) & 1;
        sim.dut->from_axi_sys_r_vcfifo_valid_i = (r >> 15) & 1;
        sim.dut->from_axi_sys_b_w_bits_i = (xorshift32(seed) & ((1u << 21) - 1));
        sim.tick();
        sim.compare("ctrl", sim.dut->orig_ctrl_o, sim.dut->new_ctrl_o);
        sim.compare("from_axi_sys_b", sim.dut->orig_b_bits_o, sim.dut->new_b_bits_o);
    }

    return sim.finish();
}
