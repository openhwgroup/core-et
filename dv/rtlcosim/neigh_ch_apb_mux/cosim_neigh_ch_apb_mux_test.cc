// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <cstdint>
#include <random>
#include <string>

#include "Vcosim_neigh_ch_apb_mux_tb.h"
#include "cosim_ctrl.h"

namespace {

struct BitCov {
    uint64_t seen0 = 0;
    uint64_t seen1 = 0;
    unsigned width = 0;

    explicit BitCov(unsigned w = 1) : width(w) {}

    void sample(uint64_t v) {
        if (width == 64) {
            seen1 |= v;
            seen0 |= ~v;
        } else {
            const uint64_t mask = (1ull << width) - 1ull;
            seen1 |= v & mask;
            seen0 |= (~v) & mask;
        }
    }

    bool full() const {
        const uint64_t mask = (width == 64) ? ~0ull : ((1ull << width) - 1ull);
        return (seen0 & mask) == mask && (seen1 & mask) == mask;
    }
};

struct Cov {
    BitCov paddr{17};
    BitCov pwrite{1};
    BitCov psel{1};
    BitCov penable{1};
    BitCov pwdata{64};
    BitCov minion_pready{8};
    BitCov minion_pslverr{8};
    std::array<BitCov, 8> minion_prdata{{BitCov(64), BitCov(64), BitCov(64), BitCov(64),
                                         BitCov(64), BitCov(64), BitCov(64), BitCov(64)}};
    BitCov esr_prdata{64};
    BitCov esr_pready{1};
    BitCov esr_pslverr{1};
    BitCov icache_prdata{64};
    BitCov icache_pready{1};
    BitCov icache_pslverr{1};
    BitCov tbox_prdata{64};
    BitCov tbox_pready{1};
    BitCov tbox_pslverr{1};

    void sample(const Vcosim_neigh_ch_apb_mux_tb* d) {
        paddr.sample(d->apb_req_paddr_i);
        pwrite.sample(d->apb_req_pwrite_i);
        psel.sample(d->apb_req_psel_i);
        penable.sample(d->apb_req_penable_i);
        pwdata.sample(d->apb_req_pwdata_i);
        minion_pready.sample(d->minion_apb_pready_i);
        minion_pslverr.sample(d->minion_apb_pslverr_i);
        minion_prdata[0].sample(d->minion_apb_prdata0_i);
        minion_prdata[1].sample(d->minion_apb_prdata1_i);
        minion_prdata[2].sample(d->minion_apb_prdata2_i);
        minion_prdata[3].sample(d->minion_apb_prdata3_i);
        minion_prdata[4].sample(d->minion_apb_prdata4_i);
        minion_prdata[5].sample(d->minion_apb_prdata5_i);
        minion_prdata[6].sample(d->minion_apb_prdata6_i);
        minion_prdata[7].sample(d->minion_apb_prdata7_i);
        esr_prdata.sample(d->esr_rsp_prdata_i);
        esr_pready.sample(d->esr_rsp_pready_i);
        esr_pslverr.sample(d->esr_rsp_pslverr_i);
        icache_prdata.sample(d->icache_rsp_prdata_i);
        icache_pready.sample(d->icache_rsp_pready_i);
        icache_pslverr.sample(d->icache_rsp_pslverr_i);
        tbox_prdata.sample(d->tbox_rsp_prdata_i);
        tbox_pready.sample(d->tbox_rsp_pready_i);
        tbox_pslverr.sample(d->tbox_rsp_pslverr_i);
    }

    void check(CosimCtrl<Vcosim_neigh_ch_apb_mux_tb>& sim) const {
        sim.check(paddr.full(), "input toggle coverage: APB request address bits all toggled");
        sim.check(pwrite.full(), "input toggle coverage: APB pwrite toggled");
        sim.check(psel.full(), "input toggle coverage: APB psel toggled");
        sim.check(penable.full(), "input toggle coverage: APB penable toggled");
        sim.check(pwdata.full(), "input toggle coverage: APB pwdata bits all toggled");
        sim.check(minion_pready.full(), "input toggle coverage: minion pready bits all toggled");
        sim.check(minion_pslverr.full(), "input toggle coverage: minion pslverr bits all toggled");
        for (int i = 0; i < 8; ++i) {
            sim.check(minion_prdata[i].full(), "input toggle coverage: minion prdata" +
                                               std::to_string(i) + " bits all toggled");
        }
        sim.check(esr_prdata.full(), "input toggle coverage: ESR response data bits all toggled");
        sim.check(esr_pready.full(), "input toggle coverage: ESR response pready toggled");
        sim.check(esr_pslverr.full(), "input toggle coverage: ESR response pslverr toggled");
        sim.check(icache_prdata.full(), "input toggle coverage: icache response data bits all toggled");
        sim.check(icache_pready.full(), "input toggle coverage: icache response pready toggled");
        sim.check(icache_pslverr.full(), "input toggle coverage: icache response pslverr toggled");
        sim.check(tbox_prdata.full(), "input toggle coverage: TBOX response data bits all toggled");
        sim.check(tbox_pready.full(), "input toggle coverage: TBOX response pready toggled");
        sim.check(tbox_pslverr.full(), "input toggle coverage: TBOX response pslverr toggled");
    }
};

uint32_t minion_addr(uint32_t cls, uint32_t minion, uint32_t pp, uint32_t thread,
                     uint32_t reg) {
    return ((cls & 0x3u) << 15) | ((pp & 0x3u) << 13) | ((minion & 0x7u) << 10) |
           ((thread & 0x1u) << 9) | (reg & 0x1ffu);
}

uint32_t esr_addr(uint32_t low) { return (1u << 15) | (low & 0x7fffu); }
uint32_t icache_addr(uint32_t low) { return (3u << 15) | (low & 0x3ffu); }
uint32_t tbox_addr(uint32_t low) { return (3u << 15) | (1u << 14) | (low & 0xfffu); }

void set_prdata(Vcosim_neigh_ch_apb_mux_tb* d, uint32_t idx, uint64_t value) {
    switch (idx) {
        case 0: d->minion_apb_prdata0_i = value; break;
        case 1: d->minion_apb_prdata1_i = value; break;
        case 2: d->minion_apb_prdata2_i = value; break;
        case 3: d->minion_apb_prdata3_i = value; break;
        case 4: d->minion_apb_prdata4_i = value; break;
        case 5: d->minion_apb_prdata5_i = value; break;
        case 6: d->minion_apb_prdata6_i = value; break;
        case 7: d->minion_apb_prdata7_i = value; break;
        default: break;
    }
}

void set_defaults(Vcosim_neigh_ch_apb_mux_tb* d) {
    d->apb_req_paddr_i = 0;
    d->apb_req_pwrite_i = 0;
    d->apb_req_psel_i = 0;
    d->apb_req_penable_i = 0;
    d->apb_req_pwdata_i = 0;
    d->minion_apb_pready_i = 0;
    d->minion_apb_pslverr_i = 0;
    for (uint32_t i = 0; i < 8; ++i) set_prdata(d, i, 0);
    d->esr_rsp_prdata_i = 0;
    d->esr_rsp_pready_i = 0;
    d->esr_rsp_pslverr_i = 0;
    d->icache_rsp_prdata_i = 0;
    d->icache_rsp_pready_i = 0;
    d->icache_rsp_pslverr_i = 0;
    d->tbox_rsp_prdata_i = 0;
    d->tbox_rsp_pready_i = 0;
    d->tbox_rsp_pslverr_i = 0;
}

void set_req(Vcosim_neigh_ch_apb_mux_tb* d, uint32_t addr, bool write, bool psel,
             bool penable, uint64_t wdata) {
    d->apb_req_paddr_i = addr;
    d->apb_req_pwrite_i = write;
    d->apb_req_psel_i = psel;
    d->apb_req_penable_i = penable;
    d->apb_req_pwdata_i = wdata;
}

void compare_all(CosimCtrl<Vcosim_neigh_ch_apb_mux_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("apb_rsp_prdata", d->orig_apb_rsp_prdata_o, d->new_apb_rsp_prdata_o);
    sim.compare("apb_rsp_pready", d->orig_apb_rsp_pready_o, d->new_apb_rsp_pready_o);
    sim.compare("apb_rsp_pslverr", d->orig_apb_rsp_pslverr_o, d->new_apb_rsp_pslverr_o);
    sim.compare("minion_apb_penable", d->orig_minion_apb_penable_o,
                d->new_minion_apb_penable_o);
    sim.compare("minion_apb_psel", d->orig_minion_apb_psel_o, d->new_minion_apb_psel_o);
    sim.compare("minion_apb_pwrite", d->orig_minion_apb_pwrite_o,
                d->new_minion_apb_pwrite_o);
    sim.compare("minion_apb_paddr", d->orig_minion_apb_paddr_o, d->new_minion_apb_paddr_o);
    sim.compare("minion_apb_pwdata", d->orig_minion_apb_pwdata_o,
                d->new_minion_apb_pwdata_o);
    sim.compare("esr_apb_paddr", d->orig_esr_apb_paddr_o, d->new_esr_apb_paddr_o);
    sim.compare("esr_apb_pwrite", d->orig_esr_apb_pwrite_o, d->new_esr_apb_pwrite_o);
    sim.compare("esr_apb_psel", d->orig_esr_apb_psel_o, d->new_esr_apb_psel_o);
    sim.compare("esr_apb_penable", d->orig_esr_apb_penable_o, d->new_esr_apb_penable_o);
    sim.compare("esr_apb_pwdata", d->orig_esr_apb_pwdata_o, d->new_esr_apb_pwdata_o);
    sim.compare("icache_apb_paddr", d->orig_icache_apb_paddr_o, d->new_icache_apb_paddr_o);
    sim.compare("icache_apb_pwrite", d->orig_icache_apb_pwrite_o,
                d->new_icache_apb_pwrite_o);
    sim.compare("icache_apb_psel", d->orig_icache_apb_psel_o, d->new_icache_apb_psel_o);
    sim.compare("icache_apb_penable", d->orig_icache_apb_penable_o,
                d->new_icache_apb_penable_o);
    sim.compare("icache_apb_pwdata", d->orig_icache_apb_pwdata_o,
                d->new_icache_apb_pwdata_o);
    sim.compare("tbox_apb_paddr", d->orig_tbox_apb_paddr_o, d->new_tbox_apb_paddr_o);
    sim.compare("tbox_apb_pwrite", d->orig_tbox_apb_pwrite_o, d->new_tbox_apb_pwrite_o);
    sim.compare("tbox_apb_psel", d->orig_tbox_apb_psel_o, d->new_tbox_apb_psel_o);
    sim.compare("tbox_apb_penable", d->orig_tbox_apb_penable_o, d->new_tbox_apb_penable_o);
    sim.compare("tbox_apb_pwdata", d->orig_tbox_apb_pwdata_o, d->new_tbox_apb_pwdata_o);
}

void tick(CosimCtrl<Vcosim_neigh_ch_apb_mux_tb>& sim, Cov& cov) {
    cov.sample(sim.dut.get());
    sim.tick();
    compare_all(sim);
}

void drive_req_sequence(CosimCtrl<Vcosim_neigh_ch_apb_mux_tb>& sim, Cov& cov,
                        uint32_t addr, bool write, uint64_t wdata, int hold_cycles) {
    auto* d = sim.dut.get();
    set_req(d, addr, write, true, false, wdata);
    tick(sim, cov);
    set_req(d, addr, write, true, true, wdata ^ 0x0101010101010101ull);
    for (int i = 0; i < hold_cycles; ++i) tick(sim, cov);
    set_req(d, addr ^ 0x15555u, !write, false, false, ~wdata);
    tick(sim, cov);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_neigh_ch_apb_mux_tb> sim(argc, argv);
    Cov cov;
    set_defaults(sim.dut.get());
    sim.reset();
    compare_all(sim);

    auto* d = sim.dut.get();
    d->esr_rsp_prdata_i = 0x1111222233334444ull;
    d->esr_rsp_pready_i = 1;
    d->esr_rsp_pslverr_i = 1;
    d->icache_rsp_prdata_i = 0x2222333344445555ull;
    d->icache_rsp_pready_i = 1;
    d->icache_rsp_pslverr_i = 0;
    d->tbox_rsp_prdata_i = 0x3333444455556666ull;
    d->tbox_rsp_pready_i = 0;
    d->tbox_rsp_pslverr_i = 1;
    for (uint32_t i = 0; i < 8; ++i) set_prdata(d, i, 0x8000000000000000ull | i);

    drive_req_sequence(sim, cov, esr_addr(0x0123), true, 0xa000000000000001ull, 4);
    drive_req_sequence(sim, cov, icache_addr(0x02a), false, 0xb000000000000002ull, 3);
    drive_req_sequence(sim, cov, tbox_addr(0x456), true, 0xc000000000000003ull, 3);
    drive_req_sequence(sim, cov, minion_addr(0, 1, 2, 1, 0x1a), true, 0xd000000000000004ull, 5);
    d->minion_apb_pready_i = 0x02;
    tick(sim, cov);
    d->minion_apb_pready_i = 0;
    drive_req_sequence(sim, cov, minion_addr(2, 6, 1, 0, 0x55), false, 0xe000000000000005ull, 5);
    d->minion_apb_pready_i = 0x40;
    d->minion_apb_pslverr_i = 0x40;
    tick(sim, cov);
    d->minion_apb_pready_i = 0;
    d->minion_apb_pslverr_i = 0;

    std::mt19937_64 rng(0x4e43484150424d55ull);
    for (int cycle = 0; cycle < 1000; ++cycle) {
        const uint64_t r0 = rng();
        const uint64_t r1 = rng();
        set_req(d, r0 & 0x1ffffu, (r0 >> 17) & 1u, (r0 >> 18) & 1u, (r0 >> 19) & 1u, r1);
        d->minion_apb_pready_i = (r0 >> 24) & 0xffu;
        d->minion_apb_pslverr_i = (r1 >> 13) & 0xffu;
        for (uint32_t i = 0; i < 8; ++i) set_prdata(d, i, rng());
        d->esr_rsp_prdata_i = rng();
        d->esr_rsp_pready_i = (r0 >> 35) & 1u;
        d->esr_rsp_pslverr_i = (r0 >> 36) & 1u;
        d->icache_rsp_prdata_i = rng();
        d->icache_rsp_pready_i = (r0 >> 37) & 1u;
        d->icache_rsp_pslverr_i = (r0 >> 38) & 1u;
        d->tbox_rsp_prdata_i = rng();
        d->tbox_rsp_pready_i = (r0 >> 39) & 1u;
        d->tbox_rsp_pslverr_i = (r0 >> 40) & 1u;
        tick(sim, cov);
    }

    cov.check(sim);
    // CosimCtrl::reset drives reset low and high; the coverage checks above
    // prove every practical functional input bit toggled both ways.
    return sim.finish();
}
