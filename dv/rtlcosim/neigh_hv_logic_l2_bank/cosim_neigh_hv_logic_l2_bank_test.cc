// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_l2_bank_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <random>

namespace {

void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_l2_bank_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("bank_req_info_pop", d->orig_bank_req_info_pop_o,
                d->new_bank_req_info_pop_o);
    sim.compare("bank_req_data_pop", d->orig_bank_req_data_pop_o,
                d->new_bank_req_data_pop_o);
    sim.compare("miss_req_bank_id", d->orig_miss_req_bank_id_o,
                d->new_miss_req_bank_id_o);
    sim.compare("miss_req_bank_source", d->orig_miss_req_bank_source_o,
                d->new_miss_req_bank_source_o);
    sim.compare("miss_req_bank_wdata", d->orig_miss_req_bank_wdata_o,
                d->new_miss_req_bank_wdata_o);
    sim.compare("miss_req_bank_opcode", d->orig_miss_req_bank_opcode_o,
                d->new_miss_req_bank_opcode_o);
    sim.compare("miss_req_bank_subopcode", d->orig_miss_req_bank_subopcode_o,
                d->new_miss_req_bank_subopcode_o);
    sim.compare("miss_req_bank_address", d->orig_miss_req_bank_address_o,
                d->new_miss_req_bank_address_o);
    sim.compare("miss_req_bank_size", d->orig_miss_req_bank_size_o,
                d->new_miss_req_bank_size_o);
    sim.compare("miss_req_bank_qwen", d->orig_miss_req_bank_qwen_o,
                d->new_miss_req_bank_qwen_o);
    sim.compare("miss_req_bank_data0", d->orig_miss_req_bank_data0_o,
                d->new_miss_req_bank_data0_o);
    sim.compare("miss_req_bank_data1", d->orig_miss_req_bank_data1_o,
                d->new_miss_req_bank_data1_o);
    sim.compare("miss_req_bank_data2", d->orig_miss_req_bank_data2_o,
                d->new_miss_req_bank_data2_o);
    sim.compare("miss_req_bank_data3", d->orig_miss_req_bank_data3_o,
                d->new_miss_req_bank_data3_o);
    sim.compare("miss_req_bank_data4", d->orig_miss_req_bank_data4_o,
                d->new_miss_req_bank_data4_o);
    sim.compare("miss_req_bank_data5", d->orig_miss_req_bank_data5_o,
                d->new_miss_req_bank_data5_o);
    sim.compare("miss_req_bank_data6", d->orig_miss_req_bank_data6_o,
                d->new_miss_req_bank_data6_o);
    sim.compare("miss_req_bank_data7", d->orig_miss_req_bank_data7_o,
                d->new_miss_req_bank_data7_o);
    sim.compare("miss_req_bank_valid", d->orig_miss_req_bank_valid_o,
                d->new_miss_req_bank_valid_o);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_l2_bank_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->bank_req_out_is_first_half_i = 0;
    d->bank_req_id_i = 0;
    d->bank_req_source_i = 0;
    d->bank_req_wdata_i = 0;
    d->bank_req_opcode_i = 0;
    d->bank_req_subopcode_i = 0;
    d->bank_req_address_i = 0;
    d->bank_req_size_i = 0;
    d->bank_req_qwen_i = 0;
    d->bank_req_data0_i = 0;
    d->bank_req_data1_i = 0;
    d->bank_req_data2_i = 0;
    d->bank_req_data3_i = 0;
    d->bank_req_info_valid_i = 0;
    d->miss_req_bank_ready_i = 0;
    sim.reset();

    std::mt19937_64 rng(0x12babc001ull);
    uint64_t small_ones = 0;
    uint64_t small_zeros = 0;
    uint64_t addr_ones = 0;
    uint64_t addr_zeros = 0;
    std::array<uint64_t, 4> data_ones = {0, 0, 0, 0};
    std::array<uint64_t, 4> data_zeros = {0, 0, 0, 0};

    for (int cycle = 0; cycle < 8192; ++cycle) {
        if (cycle == 1000) d->rst_ni = 0;
        if (cycle == 1003) d->rst_ni = 1;

        const uint64_t r0 = rng();
        const uint64_t r1 = rng();
        d->bank_req_out_is_first_half_i = ((cycle % 9) == 0) || (r0 & 1ull);
        d->bank_req_id_i = (cycle == 2) ? 0xff : (cycle == 3) ? 0x00 : (r0 >> 8);
        d->bank_req_source_i = (cycle == 2) ? 0xf : (cycle == 3) ? 0x0 : (r0 >> 16);
        d->bank_req_wdata_i = ((cycle % 5) != 0);
        d->bank_req_opcode_i = (cycle == 2) ? 0x1f : (cycle == 3) ? 0x00 : (r0 >> 20);
        d->bank_req_subopcode_i = (cycle == 2) ? 0x7f : (cycle == 3) ? 0x00 : (r0 >> 25);
        d->bank_req_address_i = (cycle == 2) ? ((1ull << 40) - 1ull) :
                                (cycle == 3) ? 0ull : (r1 & ((1ull << 40) - 1ull));
        d->bank_req_size_i = (cycle == 2) ? 0x7 : (cycle == 3) ? 0x0 : (r0 >> 32);
        d->bank_req_qwen_i = (cycle == 2) ? 0xf : (cycle == 3) ? 0x0 : (r0 >> 36);
        d->bank_req_data0_i = (cycle == 2) ? ~0ull : (cycle == 3) ? 0ull : rng();
        d->bank_req_data1_i = (cycle == 2) ? ~0ull : (cycle == 3) ? 0ull : rng();
        d->bank_req_data2_i = (cycle == 2) ? ~0ull : (cycle == 3) ? 0ull : rng();
        d->bank_req_data3_i = (cycle == 2) ? ~0ull : (cycle == 3) ? 0ull : rng();
        d->bank_req_info_valid_i = ((cycle % 4) != 1);
        d->miss_req_bank_ready_i = ((cycle % 7) != 2);

        small_ones |= d->rst_ni ? (1ull << 0) : 0ull;
        small_zeros |= d->rst_ni ? 0ull : (1ull << 0);
        small_ones |= d->bank_req_out_is_first_half_i ? (1ull << 1) : 0ull;
        small_zeros |= d->bank_req_out_is_first_half_i ? 0ull : (1ull << 1);
        small_ones |= (uint64_t(d->bank_req_id_i) << 2);
        small_zeros |= ((~uint64_t(d->bank_req_id_i)) & 0xffull) << 2;
        small_ones |= (uint64_t(d->bank_req_source_i) << 10);
        small_zeros |= ((~uint64_t(d->bank_req_source_i)) & 0xfull) << 10;
        small_ones |= d->bank_req_wdata_i ? (1ull << 14) : 0ull;
        small_zeros |= d->bank_req_wdata_i ? 0ull : (1ull << 14);
        small_ones |= (uint64_t(d->bank_req_opcode_i) << 15);
        small_zeros |= ((~uint64_t(d->bank_req_opcode_i)) & 0x1full) << 15;
        small_ones |= d->bank_req_info_valid_i ? (1ull << 20) : 0ull;
        small_zeros |= d->bank_req_info_valid_i ? 0ull : (1ull << 20);
        small_ones |= d->miss_req_bank_ready_i ? (1ull << 21) : 0ull;
        small_zeros |= d->miss_req_bank_ready_i ? 0ull : (1ull << 21);
        small_ones |= (uint64_t(d->bank_req_size_i) << 22);
        small_zeros |= ((~uint64_t(d->bank_req_size_i)) & 0x7ull) << 22;
        small_ones |= (uint64_t(d->bank_req_qwen_i) << 25);
        small_zeros |= ((~uint64_t(d->bank_req_qwen_i)) & 0xfull) << 25;
        small_ones |= (uint64_t(d->bank_req_subopcode_i) << 29);
        small_zeros |= ((~uint64_t(d->bank_req_subopcode_i)) & 0x7full) << 29;
        addr_ones |= d->bank_req_address_i;
        addr_zeros |= (~uint64_t(d->bank_req_address_i)) & ((1ull << 40) - 1ull);
        data_ones[0] |= d->bank_req_data0_i;
        data_ones[1] |= d->bank_req_data1_i;
        data_ones[2] |= d->bank_req_data2_i;
        data_ones[3] |= d->bank_req_data3_i;
        data_zeros[0] |= ~d->bank_req_data0_i;
        data_zeros[1] |= ~d->bank_req_data1_i;
        data_zeros[2] |= ~d->bank_req_data2_i;
        data_zeros[3] |= ~d->bank_req_data3_i;

        sim.tick();
        compare_all(sim);
    }

    constexpr uint64_t kSmallMask = (1ull << 36) - 1ull;
    sim.check((small_ones & kSmallMask) == kSmallMask, "small/vector inputs reached 1");
    sim.check((small_zeros & kSmallMask) == kSmallMask, "small/vector inputs reached 0");
    sim.check(addr_ones == ((1ull << 40) - 1ull), "address saw every bit high");
    sim.check(addr_zeros == ((1ull << 40) - 1ull), "address saw every bit low");
    for (int i = 0; i < 4; ++i) {
        sim.check(data_ones[i] == ~0ull, "data word saw every bit high");
        sim.check(data_zeros[i] == ~0ull, "data word saw every bit low");
    }
    return sim.finish();
}
