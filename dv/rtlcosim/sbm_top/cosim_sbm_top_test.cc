// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_sbm_top_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_sbm_top_tb;
static constexpr uint32_t kIf = 11;
static constexpr uint32_t kSbmIf = 10;
static constexpr uint8_t kShireId = 0x2a;

static uint32_t xs(uint32_t& s) { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }

static uint64_t make_addr(uint32_t kind, uint32_t idx, uint32_t reg, uint32_t pp = 3) {
    uint64_t a = (1ull << 32) | (static_cast<uint64_t>(pp & 3) << 30) |
                 (static_cast<uint64_t>(kShireId) << 22);
    if (kind == 0) { // minion/neigh lane by minion index
        a |= 0ull << 20;
        a |= static_cast<uint64_t>((idx * 8) & 0x7f) << 13;
        a |= static_cast<uint64_t>(reg & 0x1fff) << 3;
    } else if (kind == 1) { // neighborhood ESR/debug
        a |= 1ull << 20;
        a |= static_cast<uint64_t>(idx & 0xf) << 16;
        a |= static_cast<uint64_t>(reg & 0x1fff) << 3;
    } else if (kind == 2) { // bank
        a |= 3ull << 20;
        a |= 0ull << 17;
        a |= static_cast<uint64_t>(idx & 0xf) << 13;
        a |= static_cast<uint64_t>(reg & 0x3ff) << 3;
    } else if (kind == 3) { // shire ESR
        a |= 3ull << 20;
        a |= 2ull << 17;
        a |= static_cast<uint64_t>(reg & 0x3fff) << 3;
    } else { // rbox
        a |= 3ull << 20;
        a |= 1ull << 17;
        a |= static_cast<uint64_t>(reg & 0x3fff) << 3;
    }
    return a;
}

static void defaults(DUT* d) {
    d->rst_neigh_ni = 0xf;
    d->rst_rbox_ni = 1;
    d->rst_shire_cache_ni = 1;
    d->shire_virtual_id_i = kShireId;
    d->ar_valid_i = 0;
    d->aw_valid_i = 0;
    d->ar_id_i = 0;
    d->aw_id_i = 0;
    d->ar_addr_i = 0;
    d->aw_addr_i = 0;
    d->ar_len_i = 0;
    d->aw_len_i = 0;
    d->wdata_i = 0;
    d->b_ready_i = 1;
    d->r_ready_i = 1;
    d->apb_pready_i = (1u << kIf) - 1u;
    d->apb_pslverr_i = 0;
    d->apb_prdata_i = 0x1122334455667788ull;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("ar_ready", d->orig_ar_ready_o, d->new_ar_ready_o);
    sim.compare("aw_ready", d->orig_aw_ready_o, d->new_aw_ready_o);
    sim.compare("w_ready", d->orig_w_ready_o, d->new_w_ready_o);
    sim.compare("credit_return_write", d->orig_credit_return_write_o, d->new_credit_return_write_o);
    sim.compare("b_valid", d->orig_b_valid_o, d->new_b_valid_o);
    sim.compare("b_id", d->orig_b_id_o, d->new_b_id_o);
    sim.compare("b_resp", d->orig_b_resp_o, d->new_b_resp_o);
    sim.compare("r_valid", d->orig_r_valid_o, d->new_r_valid_o);
    sim.compare("r_id", d->orig_r_id_o, d->new_r_id_o);
    sim.compare("r_resp", d->orig_r_resp_o, d->new_r_resp_o);
    sim.compare("r_last", d->orig_r_last_o, d->new_r_last_o);
    sim.compare("r_data0", d->orig_r_data0_o, d->new_r_data0_o);
    sim.compare("r_data1", d->orig_r_data1_o, d->new_r_data1_o);
    sim.compare("r_data2", d->orig_r_data2_o, d->new_r_data2_o);
    sim.compare("r_data3", d->orig_r_data3_o, d->new_r_data3_o);
    sim.compare("apb_psel", d->orig_apb_psel_o, d->new_apb_psel_o);
    sim.compare("apb_penable", d->orig_apb_penable_o, d->new_apb_penable_o);
    sim.compare("apb_pwrite", d->orig_apb_pwrite_o, d->new_apb_pwrite_o);
    for (int i = 0; i < static_cast<int>(kIf); ++i) {
        char name[64];
        std::snprintf(name, sizeof(name), "apb_paddr[%d]", i);
        sim.compare(name, d->orig_apb_paddr_o[i], d->new_apb_paddr_o[i]);
        std::snprintf(name, sizeof(name), "apb_pwdata[%d]", i);
        sim.compare(name, d->orig_apb_pwdata_o[i], d->new_apb_pwdata_o[i]);
    }
}

static void tick(CosimCtrl<DUT>& sim) { sim.tick(); compare_outputs(sim); }

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    defaults(d);
    sim.reset(8);
    compare_outputs(sim);

    printf("directed lanes\n");
    for (uint32_t lane = 0; lane < kSbmIf; ++lane) {
        uint32_t kind = (lane < 4) ? 1 : ((lane < 8) ? 2 : ((lane == 8) ? 3 : 4));
        uint32_t idx = (lane < 4) ? lane : ((lane < 8) ? (lane - 4) : 0);
        d->ar_valid_i = 1;
        d->ar_id_i = 0x40 + lane;
        d->ar_addr_i = make_addr(kind, idx, 0x10 + lane);
        d->apb_prdata_i = 0xabc00000ull + lane;
        tick(sim);
        d->ar_valid_i = 0;
        for (int i = 0; i < 12; ++i) tick(sim);
    }

    printf("broadcast neighborhoods and banks\n");
    d->aw_valid_i = 1;
    d->aw_id_i = 0x101;
    d->aw_addr_i = make_addr(1, 0xf, 0x20);
    d->wdata_i = 0x0102030405060708ull;
    tick(sim);
    d->aw_valid_i = 0;
    for (int i = 0; i < 30; ++i) tick(sim);
    d->aw_valid_i = 1;
    d->aw_id_i = 0x102;
    d->aw_addr_i = make_addr(2, 0xf, 0x21);
    d->wdata_i = 0x1112131415161718ull;
    tick(sim);
    d->aw_valid_i = 0;
    for (int i = 0; i < 30; ++i) tick(sim);

    printf("APB stalls, response backpressure, and slave errors\n");
    d->apb_pready_i = ((1u << kIf) - 1u) & ~(1u << 8);
    d->apb_pslverr_i = 0;
    d->r_ready_i = 0;
    d->ar_valid_i = 1;
    d->ar_id_i = 0x180;
    d->ar_addr_i = make_addr(3, 0, 0x55);
    tick(sim);
    d->ar_valid_i = 0;
    tick(sim);
    tick(sim);
    d->apb_pready_i = (1u << kIf) - 1u;
    d->apb_pslverr_i = 1u << 8;
    tick(sim);
    d->apb_pslverr_i = 0;
    tick(sim);
    d->r_ready_i = 1;
    for (int i = 0; i < 6; ++i) tick(sim);

    d->apb_pready_i = ((1u << kIf) - 1u) & ~(1u << 4);
    d->b_ready_i = 0;
    d->aw_valid_i = 1;
    d->aw_id_i = 0x181;
    d->aw_addr_i = make_addr(2, 0, 0x56);
    d->wdata_i = 0xa5a5a5a55a5a5a5aull;
    tick(sim);
    d->aw_valid_i = 0;
    tick(sim);
    tick(sim);
    d->apb_pready_i = (1u << kIf) - 1u;
    d->apb_pslverr_i = 1u << 4;
    tick(sim);
    d->apb_pslverr_i = 0;
    tick(sim);
    d->b_ready_i = 1;
    for (int i = 0; i < 8; ++i) tick(sim);

    printf("invalid and reset paths\n");
    d->ar_valid_i = 1;
    d->ar_id_i = 0x1aa;
    d->ar_addr_i = make_addr(3, 0, 0x33) ^ (1ull << 22);
    tick(sim);
    d->ar_valid_i = 0;
    for (int i = 0; i < 8; ++i) tick(sim);
    d->rst_neigh_ni = 0xe;
    d->aw_valid_i = 1;
    d->aw_id_i = 0x1bb;
    d->aw_addr_i = make_addr(1, 0, 0x44);
    tick(sim);
    d->aw_valid_i = 0;
    d->rst_neigh_ni = 0xf;
    for (int i = 0; i < 8; ++i) tick(sim);

    printf("random traffic\n");
    uint32_t seed = 0x98761234;
    for (int cyc = 0; cyc < 12000; ++cyc) {
        uint32_t r = xs(seed);
        d->b_ready_i = ((r >> 0) & 7) != 0;
        d->r_ready_i = ((r >> 3) & 7) != 0;
        d->apb_pready_i = (1u << kIf) - 1u;
        d->apb_pslverr_i = 0;
        d->apb_prdata_i = (static_cast<uint64_t>(r) << 32) | xs(seed);
        d->rst_neigh_ni = 0xf;
        d->rst_rbox_ni = 1;
        d->rst_shire_cache_ni = 1;

        d->ar_valid_i = 0;
        d->aw_valid_i = 0;
        if (d->new_ar_ready_o && ((r >> 4) & 3) == 1) {
            uint32_t lane = (r >> 8) % kSbmIf;
            uint32_t kind = (lane < 4) ? 1 : ((lane < 8) ? 2 : ((lane == 8) ? 3 : 4));
            uint32_t idx = (lane < 4) ? lane : ((lane < 8) ? (lane - 4) : 0);
            d->ar_valid_i = 1;
            d->ar_id_i = r & 0x7ffff;
            d->ar_addr_i = make_addr(kind, idx, (r >> 12) & 0x3ff);
            d->ar_len_i = 0;
        }
        if (d->new_aw_ready_o && ((r >> 6) & 3) == 2) {
            uint32_t lane = (r >> 10) % kSbmIf;
            uint32_t kind = (lane < 4) ? 1 : ((lane < 8) ? 2 : ((lane == 8) ? 3 : 4));
            uint32_t idx = (lane < 4) ? lane : ((lane < 8) ? (lane - 4) : 0);
            if (((r >> 20) & 0x1f) == 0) { kind = ((r >> 25) & 1) ? 1 : 2; idx = 0xf; }
            d->aw_valid_i = 1;
            d->aw_id_i = (r ^ 0x15555u) & 0x7ffff;
            d->aw_addr_i = make_addr(kind, idx, (r >> 12) & 0x3ff);
            d->aw_len_i = 0;
            d->wdata_i = (static_cast<uint64_t>(xs(seed)) << 32) | xs(seed);
        }
        tick(sim);
    }
    d->ar_valid_i = 0;
    d->aw_valid_i = 0;
    d->b_ready_i = 1;
    d->r_ready_i = 1;
    d->apb_pready_i = (1u << kIf) - 1u;
    d->apb_pslverr_i = 0;
    d->rst_neigh_ni = 0xf;
    d->rst_rbox_ni = 1;
    d->rst_shire_cache_ni = 1;
    for (int i = 0; i < 60; ++i) tick(sim);

    return sim.finish();
}
