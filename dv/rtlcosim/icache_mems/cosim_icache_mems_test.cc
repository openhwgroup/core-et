// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_mems_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <string>

uint32_t xs(uint32_t& s) { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }

template <typename T>
void clear_words(T& arr, int n) { for (int i = 0; i < n; ++i) arr[i] = 0; }

template <typename T>
void random_words(T& arr, int n, uint32_t& seed) { for (int i = 0; i < n; ++i) arr[i] = xs(seed); }

template <typename T>
void compare_words(CosimCtrl<Vcosim_icache_mems_tb>& sim, const char* name,
                   const T& a, const T& b, int n) {
    for (int i = 0; i < n; ++i) sim.compare(std::string(name) + std::to_string(i), a[i], b[i]);
}

void compare_all(CosimCtrl<Vcosim_icache_mems_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("req_ready", d->orig_req_ready_o, d->new_req_ready_o);
    compare_words(sim, "resp_dout", d->orig_resp_dout_o, d->new_resp_dout_o, 72);
    sim.compare("resp_valid", d->orig_resp_valid_o, d->new_resp_valid_o);
    compare_words(sim, "bist_rsp", d->orig_bist_rsp_o, d->new_bist_rsp_o, 5);
    sim.compare("apb_pready", d->orig_apb_pready_o, d->new_apb_pready_o);
    sim.compare("apb_prdata", d->orig_apb_prdata_o, d->new_apb_prdata_o);
    sim.compare("apb_pslverr", d->orig_apb_pslverr_o, d->new_apb_pslverr_o);
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_icache_mems_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    uint32_t seed = 0x55667788u;

    d->rst_vec_ni_i = 0xf;
    d->dft_sram_clk_i = 0;
    d->dft_clk_override_i = 0;
    d->icache_req_write_i = 0;
    d->icache_req_addr_i = 0;
    d->icache_req_valid_i = 0;
    d->icache_resp_ready_i = 0xf;
    clear_words(d->neigh_rsp_info_bits_i, 67);
    d->neigh_rsp_valid_i = 0;
    d->neigh_rsp_ready_i = 0;
    clear_words(d->bist_req_bits_i, 6);
    d->apb_paddr_i = 0;
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwdata_i = 0;
    sim.reset();

    // Directed DFT SRAM-clock override exercise with functional accesses idle.
    for (int i = 0; i < 8; ++i) {
        d->dft_sram_clk_i = i & 1u;
        d->dft_clk_override_i = (i >> 1) & 1u;
        d->rst_vec_ni_i = (i == 6) ? 0x7u : 0xfu;
        sim.tick();
        compare_all(sim);
    }
    d->rst_vec_ni_i = 0xf;
    d->dft_clk_override_i = 0;

    for (int i = 0; i < 2200; ++i) {
        uint32_t r = xs(seed);
        d->dft_sram_clk_i = (r >> 1) & 1u;
        d->dft_clk_override_i = 0;  // Random functional accesses use the functional clock for deterministic compare.
        d->icache_req_write_i = (r >> 4) & 0xfu;
        d->icache_req_addr_i = (xs(seed) & 0x7ffu) | ((uint64_t)(xs(seed) & 0x7ffu) << 11) |
                               ((uint64_t)(xs(seed) & 0x7ffu) << 22) | ((uint64_t)(xs(seed) & 0x7ffu) << 33);
        d->icache_req_valid_i = (i % 7) == 0 ? (1u << ((i / 7) & 3)) : 0u;
        if (d->icache_req_valid_i != 0) d->icache_req_write_i = 0;  // Read path is deterministic after reset.
        d->icache_resp_ready_i = xs(seed) & 0xfu;
        random_words(d->neigh_rsp_info_bits_i, 67, seed);
        d->neigh_rsp_valid_i = (i % 11) == 0 ? (1u << ((i / 11) & 3)) : 0u;
        d->neigh_rsp_ready_i = xs(seed) & 0xfu;
        if ((i % 13) == 0) {
            random_words(d->bist_req_bits_i, 6, seed);
        } else {
            clear_words(d->bist_req_bits_i, 6);
        }

        const uint32_t neigh = (i >> 2) & 0x3u;
        d->apb_paddr_i = (neigh << 13) | (xs(seed) & 0x1fffu);
        d->apb_pwrite_i = (i >> 1) & 1u;
        d->apb_psel_i = (i % 5) != 0;
        d->apb_penable_i = (i % 5) >= 2;
        d->apb_pwdata_i = xs(seed) & 0xffffu;

        d->rst_vec_ni_i = 0xf;

        sim.tick();
        compare_all(sim);
    }
    return sim.finish();
}
