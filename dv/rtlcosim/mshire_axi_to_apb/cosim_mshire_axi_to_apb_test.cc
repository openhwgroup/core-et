// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_mshire_axi_to_apb_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_mshire_axi_to_apb_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("ready", d->orig_ready_o, d->new_ready_o);
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
    sim.compare("apb_paddr", d->orig_apb_paddr_o, d->new_apb_paddr_o);
    sim.compare("apb_penable", d->orig_apb_penable_o, d->new_apb_penable_o);
    sim.compare("apb_psel", d->orig_apb_psel_o, d->new_apb_psel_o);
    sim.compare("apb_pwdata", d->orig_apb_pwdata_o, d->new_apb_pwdata_o);
    sim.compare("apb_pwrite", d->orig_apb_pwrite_o, d->new_apb_pwrite_o);
}

static void tick(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

static uint32_t xorshift(uint32_t& s) {
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return s;
}

static void defaults(DUT* d) {
    d->ar_valid_i = 0;
    d->aw_valid_i = 0;
    d->ar_id_i = 0;
    d->aw_id_i = 0;
    d->ar_addr_i = 0;
    d->aw_addr_i = 0;
    d->wdata_i = 0;
    d->b_ready_i = 1;
    d->r_ready_i = 1;
    d->apb_prdata_i = 0;
    d->apb_pready_i = 0;
    d->apb_pslverr_i = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    defaults(d);
    sim.reset(8);
    compare_outputs(sim);

    printf("directed write with delayed BREADY\n");
    d->aw_valid_i = 1;
    d->aw_id_i = 0x11;
    d->aw_addr_i = 0x100001234ull;
    d->wdata_i = 0xdeadbeef01234567ull;
    d->apb_pready_i = 0;
    d->b_ready_i = 0;
    tick(sim);
    d->aw_valid_i = 0;
    for (int i = 0; i < 3; ++i) tick(sim);
    d->apb_pready_i = 1;
    d->apb_pslverr_i = 0;
    tick(sim);
    tick(sim);
    d->b_ready_i = 1;
    for (int i = 0; i < 3; ++i) tick(sim);

    printf("directed read with delayed RREADY and SLVERR\n");
    d->ar_valid_i = 1;
    d->ar_id_i = 0x22;
    d->ar_addr_i = 0x100005678ull;
    d->apb_prdata_i = 0xa5a55a5acc33cc33ull;
    d->apb_pslverr_i = 1;
    d->r_ready_i = 0;
    tick(sim);
    d->ar_valid_i = 0;
    for (int i = 0; i < 4; ++i) tick(sim);
    d->r_ready_i = 1;
    for (int i = 0; i < 4; ++i) tick(sim);
    d->apb_pslverr_i = 0;

    printf("simultaneous AW/AR queues read after write\n");
    d->aw_valid_i = 1;
    d->ar_valid_i = 1;
    d->aw_id_i = 0x33;
    d->ar_id_i = 0x44;
    d->aw_addr_i = 0x100000080ull;
    d->ar_addr_i = 0x1000000c0ull;
    d->wdata_i = 0x1111222233334444ull;
    d->apb_prdata_i = 0x777788889999aaaall;
    tick(sim);
    d->aw_valid_i = 0;
    d->ar_valid_i = 0;
    for (int i = 0; i < 16; ++i) tick(sim);

    printf("random traffic\n");
    uint32_t seed = 0x1234abcd;
    for (int cyc = 0; cyc < 6500; ++cyc) {
        uint32_t r = xorshift(seed);
        d->apb_pready_i = ((r >> 0) & 3) != 0;
        d->apb_pslverr_i = (r >> 2) & 1;
        d->b_ready_i = (r >> 3) & 1;
        d->r_ready_i = (r >> 4) & 1;
        if (d->new_ready_o && ((r >> 5) & 3) != 0) {
            d->aw_valid_i = (r >> 7) & 1;
            d->ar_valid_i = (r >> 8) & 1;
            d->aw_id_i = r & 0x7ffff;
            d->ar_id_i = (r ^ 0x15555u) & 0x7ffff;
            d->aw_addr_i = (0x100000000ull | (static_cast<uint64_t>(r & 0xffff) << 3));
            d->ar_addr_i = (0x100000000ull | (static_cast<uint64_t>((r >> 4) & 0xffff) << 3));
            d->wdata_i = (static_cast<uint64_t>(r) << 32) | xorshift(seed);
        } else {
            d->aw_valid_i = 0;
            d->ar_valid_i = 0;
        }
        d->apb_prdata_i = (static_cast<uint64_t>(xorshift(seed)) << 32) | xorshift(seed);
        tick(sim);
    }

    d->aw_valid_i = 0;
    d->ar_valid_i = 0;
    d->apb_pready_i = 1;
    d->b_ready_i = 1;
    d->r_ready_i = 1;
    for (int i = 0; i < 20; ++i) tick(sim);

    return sim.finish();
}
