// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_buffer_array — Verilator test.

#include "Vminion_dcache_buffer_array_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vminion_dcache_buffer_array_tb;
using U256 = std::array<uint32_t, 8>;

namespace {

void tick_and_settle(SimCtrl<DUT>& sim) {
    sim.dut->eval();  // Settle combinational wrapper inputs before the negedge capture.
    sim.tick();
    sim.dut->eval();  // Settle readback/output muxes after the write edge.
}

void set_u256(WData* dst, const U256& value) {
    for (int i = 0; i < 8; ++i) {
        dst[i] = value[i];
    }
}

U256 get_u256(const WData* src) {
    U256 value {};
    for (int i = 0; i < 8; ++i) {
        value[i] = src[i];
    }
    return value;
}

void check_u256(SimCtrl<DUT>& sim, const U256& expect, const U256& got, const std::string& label) {
    for (int i = 0; i < 8; ++i) {
        sim.check(got[i] == expect[i], label + " word[" + std::to_string(i) + "]");
    }
}

U256 make_pattern(uint32_t seed) {
    U256 out {};
    for (int i = 0; i < 8; ++i) {
        out[i] = seed ^ (0x11111111u * uint32_t(i + 1));
    }
    return out;
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

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_ctrls(sim.dut.get());
    sim.reset();
    sim.dut->eval();

    printf("test 1: directed control and storage checks\n");
    sim.check(sim.dut->full_o == 0, "buffer not full after reset");
    sim.check(sim.dut->reduce_full_o == 0, "reduce buffer not full after reset");
    sim.check(sim.dut->s2_write_entry_o == 7, "highest free entry after reset");
    sim.check(sim.dut->s3_write_en_o == 0, "s3 write enable reset");
    // The backing RF is intentionally unreset in both the original and reimplementation.
    // Only entries written through the buffer protocol have defined data.

    const U256 ext_data = make_pattern(0xA5A50000u);
    clear_ctrls(sim.dut.get());
    sim.dut->s1_read_entry_i = 2;
    sim.dut->s3_write_en_ext_i = 1;
    sim.dut->s2_write_en_ext_prev_i = 1;
    sim.dut->s3_write_entry_ext_i = 2;
    set_u256(sim.dut->s3_write_data_ext_i, ext_data);
    tick_and_settle(sim);
    // Latch is transparent at negedge: data captured and committed in same cycle.
    check_u256(sim, ext_data, get_u256(sim.dut->s1_read_data_o), "external write/read");

    clear_ctrls(sim.dut.get());
    sim.dut->s2_write_needed_i = 1;
    sim.dut->s2_write_en_i = 1;
    set_u256(sim.dut->s3_write_data_i, make_pattern(0x12340000u));
    tick_and_settle(sim);
    sim.check(sim.dut->s3_write_en_o == 1, "internal write valid delayed to s3");
    sim.dut->eval();
    sim.check(sim.dut->s2_write_entry_o == 6, "entry 7 consumed by internal write");

    clear_ctrls(sim.dut.get());
    sim.dut->s2_dealloc_i = 1;
    sim.dut->s2_dealloc_entry_i = 7;
    tick_and_settle(sim);
    sim.dut->eval();
    sim.check(sim.dut->s2_write_entry_o == 7, "entry 7 returned after dealloc");

    clear_ctrls(sim.dut.get());
    sim.dut->s2_write_replay_i = 1;
    sim.dut->s2_write_replay_entry_i = 3;
    sim.dut->eval();
    sim.check(sim.dut->s2_write_entry_o == 3, "replay entry override");

    clear_ctrls(sim.dut.get());
    for (int i = 0; i < 4; ++i) {
        sim.dut->reduce_alloc_req_i = 1;
        tick_and_settle(sim);
        clear_ctrls(sim.dut.get());
    }
    sim.dut->eval();
    sim.check(sim.dut->reduce_full_o == 1, "reduce buffer full after 4 allocations");

    sim.dut->reduce_dealloc_req_i = 1;
    sim.dut->reduce_dealloc_entry_i = 0;
    tick_and_settle(sim);
    clear_ctrls(sim.dut.get());
    sim.dut->eval();
    sim.check(sim.dut->reduce_full_o == 0, "reduce buffer available after dealloc");

    printf("test 2: mixed traffic smoke\n");
    const U256 ext_data_b = make_pattern(0x55AA0000u);
    clear_ctrls(sim.dut.get());
    sim.dut->s1_read_entry_i = 5;
    sim.dut->s3_write_en_ext_i = 1;
    sim.dut->s2_write_en_ext_prev_i = 1;
    sim.dut->s3_write_entry_ext_i = 5;
    set_u256(sim.dut->s3_write_data_ext_i, ext_data_b);
    tick_and_settle(sim);
    check_u256(sim, ext_data_b, get_u256(sim.dut->s1_read_data_o), "second external write/read");

    clear_ctrls(sim.dut.get());
    sim.dut->s2_write_needed_i = 1;
    sim.dut->s2_write_replay_i = 1;
    sim.dut->s2_write_replay_entry_i = 1;
    sim.dut->eval();
    sim.check(sim.dut->s2_write_entry_o == 1, "replay entry still wins with write_needed");
    tick_and_settle(sim);
    sim.check(sim.dut->s3_write_en_o == 0, "replay path alone does not raise s3 write");

    return sim.finish();
}
