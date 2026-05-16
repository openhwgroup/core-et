// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_hv_logic_neigh_sc_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <random>

namespace {

void compare_all(CosimCtrl<Vcosim_neigh_hv_logic_neigh_sc_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("miss_req_bank_ready", d->orig_miss_req_bank_ready_o,
                d->new_miss_req_bank_ready_o);
    sim.compare("uc_req_info_pop", d->orig_uc_req_info_pop_o,
                d->new_uc_req_info_pop_o);
    sim.compare("uc_req_data_pop", d->orig_uc_req_data_pop_o,
                d->new_uc_req_data_pop_o);
    sim.compare("neigh_sc_req_valid", d->orig_neigh_sc_req_valid_o,
                d->new_neigh_sc_req_valid_o);
    sim.compare("neigh_sc_req_id", d->orig_neigh_sc_req_id_o,
                d->new_neigh_sc_req_id_o);
    sim.compare("neigh_sc_req_source", d->orig_neigh_sc_req_source_o,
                d->new_neigh_sc_req_source_o);
    sim.compare("neigh_sc_req_wdata", d->orig_neigh_sc_req_wdata_o,
                d->new_neigh_sc_req_wdata_o);
    sim.compare("neigh_sc_req_opcode", d->orig_neigh_sc_req_opcode_o,
                d->new_neigh_sc_req_opcode_o);
    sim.compare("neigh_sc_req_subopcode", d->orig_neigh_sc_req_subopcode_o,
                d->new_neigh_sc_req_subopcode_o);
    sim.compare("neigh_sc_req_address", d->orig_neigh_sc_req_address_o,
                d->new_neigh_sc_req_address_o);
    sim.compare("neigh_sc_req_size", d->orig_neigh_sc_req_size_o,
                d->new_neigh_sc_req_size_o);
    sim.compare("neigh_sc_req_qwen", d->orig_neigh_sc_req_qwen_o,
                d->new_neigh_sc_req_qwen_o);
    sim.compare("neigh_sc_req_data0", d->orig_neigh_sc_req_data0_o,
                d->new_neigh_sc_req_data0_o);
    sim.compare("neigh_sc_req_data1", d->orig_neigh_sc_req_data1_o,
                d->new_neigh_sc_req_data1_o);
    sim.compare("neigh_sc_req_data2", d->orig_neigh_sc_req_data2_o,
                d->new_neigh_sc_req_data2_o);
    sim.compare("neigh_sc_req_data3", d->orig_neigh_sc_req_data3_o,
                d->new_neigh_sc_req_data3_o);
    sim.compare("neigh_sc_req_data4", d->orig_neigh_sc_req_data4_o,
                d->new_neigh_sc_req_data4_o);
    sim.compare("neigh_sc_req_data5", d->orig_neigh_sc_req_data5_o,
                d->new_neigh_sc_req_data5_o);
    sim.compare("neigh_sc_req_data6", d->orig_neigh_sc_req_data6_o,
                d->new_neigh_sc_req_data6_o);
    sim.compare("neigh_sc_req_data7", d->orig_neigh_sc_req_data7_o,
                d->new_neigh_sc_req_data7_o);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_hv_logic_neigh_sc_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    d->miss_req_bank_valid_i = 0;
    d->neigh_sc_req_ready_i = 0;
    d->bank0_seed_i = 0;
    d->bank1_seed_i = 0;
    d->bank2_seed_i = 0;
    d->bank3_seed_i = 0;
    d->uc_seed_i = 0;
    d->uc_req_info_pop_valid_i = 0;
    d->uc_wdata_i = 0;
    sim.reset();

    std::mt19937_64 rng(0x5c001ull);
    uint32_t small_ones = 0;
    uint32_t small_zeros = 0;
    std::array<uint64_t, 5> seed_ones = {0, 0, 0, 0, 0};
    std::array<uint64_t, 5> seed_zeros = {0, 0, 0, 0, 0};

    for (int cycle = 0; cycle < 8192; ++cycle) {
        if (cycle == 513) d->rst_ni = 0;
        if (cycle == 516) d->rst_ni = 1;

        d->miss_req_bank_valid_i = (cycle == 2) ? 0xf :
                                   (cycle == 3) ? 0x0 : (rng() & 0xfu);
        d->neigh_sc_req_ready_i = (cycle == 2) ? 0x1f :
                                  (cycle == 3) ? 0x0 : (rng() & 0x1fu);
        d->bank0_seed_i = (cycle == 2) ? ~0ull : (cycle == 3) ? 0ull : rng();
        d->bank1_seed_i = (cycle == 2) ? ~0ull : (cycle == 3) ? 0ull : rng();
        d->bank2_seed_i = (cycle == 2) ? ~0ull : (cycle == 3) ? 0ull : rng();
        d->bank3_seed_i = (cycle == 2) ? ~0ull : (cycle == 3) ? 0ull : rng();
        d->uc_seed_i = (cycle == 2) ? ~0ull : (cycle == 3) ? 0ull : rng();
        d->uc_req_info_pop_valid_i = ((cycle % 4) != 0);
        d->uc_wdata_i = ((cycle % 5) != 1);

        small_ones |= d->rst_ni ? (1u << 0) : 0u;
        small_zeros |= d->rst_ni ? 0u : (1u << 0);
        small_ones |= (uint32_t(d->miss_req_bank_valid_i) << 1);
        small_zeros |= ((~uint32_t(d->miss_req_bank_valid_i)) & 0xfu) << 1;
        small_ones |= (uint32_t(d->neigh_sc_req_ready_i) << 5);
        small_zeros |= ((~uint32_t(d->neigh_sc_req_ready_i)) & 0x1fu) << 5;
        small_ones |= d->uc_req_info_pop_valid_i ? (1u << 10) : 0u;
        small_zeros |= d->uc_req_info_pop_valid_i ? 0u : (1u << 10);
        small_ones |= d->uc_wdata_i ? (1u << 11) : 0u;
        small_zeros |= d->uc_wdata_i ? 0u : (1u << 11);
        seed_ones[0] |= d->bank0_seed_i;
        seed_ones[1] |= d->bank1_seed_i;
        seed_ones[2] |= d->bank2_seed_i;
        seed_ones[3] |= d->bank3_seed_i;
        seed_ones[4] |= d->uc_seed_i;
        seed_zeros[0] |= ~d->bank0_seed_i;
        seed_zeros[1] |= ~d->bank1_seed_i;
        seed_zeros[2] |= ~d->bank2_seed_i;
        seed_zeros[3] |= ~d->bank3_seed_i;
        seed_zeros[4] |= ~d->uc_seed_i;

        sim.tick();
        compare_all(sim);
    }

    constexpr uint32_t kSmallMask = (1u << 12) - 1u;
    sim.check((small_ones & kSmallMask) == kSmallMask, "scalar/vector inputs reached 1");
    sim.check((small_zeros & kSmallMask) == kSmallMask, "scalar/vector inputs reached 0");
    for (int i = 0; i < 5; ++i) {
        sim.check(seed_ones[i] == ~0ull, "seed-derived input payload saw every bit high");
        sim.check(seed_zeros[i] == ~0ull, "seed-derived input payload saw every bit low");
    }
    return sim.finish();
}
