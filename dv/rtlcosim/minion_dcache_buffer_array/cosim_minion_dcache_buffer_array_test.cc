// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: dcache_buffer_array (original) vs minion_dcache_buffer_array (new).

#include "Vcosim_minion_dcache_buffer_array_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>

using DUT = Vcosim_minion_dcache_buffer_array_tb;
using U256 = std::array<uint32_t, 8>;

namespace {

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void set_u256(WData* dst, const U256& value) {
    for (int i = 0; i < 8; ++i) {
        dst[i] = value[i];
    }
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("full", sim.dut->orig_full_o, sim.dut->new_full_o);
    sim.compare("reduce_full", sim.dut->orig_reduce_full_o, sim.dut->new_reduce_full_o);
    sim.compare("s2_write_entry", sim.dut->orig_s2_write_entry_o, sim.dut->new_s2_write_entry_o);
    sim.compare("s3_write_en", sim.dut->orig_s3_write_en_o, sim.dut->new_s3_write_en_o);
    for (int i = 0; i < 8; ++i) {
        sim.compare("s1_read_data[" + std::to_string(i) + "]",
                    sim.dut->orig_s1_read_data_o[i],
                    sim.dut->new_s1_read_data_o[i]);
    }
}

void clear_ctrls(DUT* dut) {
    dut->alloc_req_pre_i = 0;
    dut->alloc_req_val_i = 0;
    dut->alloc_gsc_i = 0;
    dut->reduce_alloc_req_i = 0;
    dut->s2_write_needed_core_i = 0;
    dut->s2_write_needed_i = 0;
    dut->s2_write_en_i = 0;
    dut->s2_write_replay_i = 0;
    dut->s2_write_replay_entry_i = 0;
    dut->s3_write_en_ext_i = 0;
    dut->s2_write_en_ext_prev_i = 0;
    dut->s3_write_entry_ext_i = 0;
    dut->s2_dealloc_i = 0;
    dut->s2_dealloc_entry_i = 0;
    dut->s1_dealloc_uc_i = 0;
    dut->s1_dealloc_uc_entry_i = 0;
    dut->reduce_dealloc_req_i = 0;
    dut->reduce_dealloc_entry_i = 0;
    dut->s1_read_entry_i = 0;
    for (int i = 0; i < 8; ++i) {
        dut->s3_write_data_i[i] = 0;
        dut->s3_write_data_ext_i[i] = 0;
    }
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->eval();  // Settle combinational wrapper inputs before the negedge capture.
    sim.tick();
    compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    clear_ctrls(sim.dut.get());
    sim.reset(4);
    compare_outputs(sim);

    for (int i = 0; i < 16; ++i) {
        U256 ext {};
        for (int w = 0; w < 8; ++w) {
            ext[w] = 0x10000000u ^ uint32_t(i * 0x01010101u) ^ uint32_t(w * 0x11111111u);
        }
        clear_ctrls(sim.dut.get());
        sim.dut->s1_read_entry_i = i & 0x7;
        sim.dut->s3_write_en_ext_i = 1;
        sim.dut->s2_write_en_ext_prev_i = 1;
        sim.dut->s3_write_entry_ext_i = i & 0x7;
        set_u256(sim.dut->s3_write_data_ext_i, ext);
        sim.dut->s2_write_en_i = i & 1;
        sim.dut->s2_write_needed_i = (i >> 1) & 1;
        sim.dut->s2_write_replay_i = (i >> 2) & 1;
        sim.dut->s2_write_replay_entry_i = (i + 3) & 0x7;
        sim.dut->reduce_alloc_req_i = (i >> 3) & 1;
        tick_and_compare(sim);
    }

    uint32_t seed = 0x0F1E2D3Cu;
    for (int i = 0; i < 1024; ++i) {
        U256 data {};
        U256 data_ext {};
        for (int w = 0; w < 8; ++w) {
            data[w] = xorshift32(seed);
            data_ext[w] = xorshift32(seed);
        }
        clear_ctrls(sim.dut.get());
        sim.dut->alloc_req_pre_i = xorshift32(seed) & 1u;
        sim.dut->alloc_req_val_i = xorshift32(seed) & 1u;
        sim.dut->alloc_gsc_i = xorshift32(seed) & 1u;
        sim.dut->reduce_alloc_req_i = xorshift32(seed) & 1u;
        sim.dut->s2_write_needed_core_i = xorshift32(seed) & 1u;
        sim.dut->s2_write_needed_i = xorshift32(seed) & 1u;
        sim.dut->s2_write_en_i = xorshift32(seed) & 1u;
        sim.dut->s2_write_replay_i = xorshift32(seed) & 1u;
        sim.dut->s2_write_replay_entry_i = xorshift32(seed) & 0x7u;
        set_u256(sim.dut->s3_write_data_i, data);
        sim.dut->s3_write_en_ext_i = xorshift32(seed) & 1u;
        sim.dut->s2_write_en_ext_prev_i = xorshift32(seed) & 1u;
        sim.dut->s3_write_entry_ext_i = xorshift32(seed) & 0x7u;
        set_u256(sim.dut->s3_write_data_ext_i, data_ext);
        sim.dut->s2_dealloc_i = xorshift32(seed) & 1u;
        sim.dut->s2_dealloc_entry_i = xorshift32(seed) & 0x7u;
        sim.dut->s1_dealloc_uc_i = xorshift32(seed) & 1u;
        sim.dut->s1_dealloc_uc_entry_i = xorshift32(seed) & 0x7u;
        sim.dut->reduce_dealloc_req_i = xorshift32(seed) & 1u;
        sim.dut->reduce_dealloc_entry_i = xorshift32(seed) & 0x7u;
        sim.dut->s1_read_entry_i = xorshift32(seed) & 0x7u;
        tick_and_compare(sim);
    }

    return sim.finish();
}
