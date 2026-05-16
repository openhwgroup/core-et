// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <cstdint>
#include <random>
#include <string>

#include "Vcosim_bpam2minions_tb.h"
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
    BitCov pready{8};
    BitCov pslverr{8};
    std::array<BitCov, 8> prdata{{BitCov(64), BitCov(64), BitCov(64), BitCov(64),
                                  BitCov(64), BitCov(64), BitCov(64), BitCov(64)}};

    void sample(const Vcosim_bpam2minions_tb* d) {
        paddr.sample(d->req_apb_paddr_i);
        pwrite.sample(d->req_apb_pwrite_i);
        psel.sample(d->req_apb_psel_i);
        penable.sample(d->req_apb_penable_i);
        pwdata.sample(d->req_apb_pwdata_i);
        pready.sample(d->minion_apb_pready_i);
        pslverr.sample(d->minion_apb_pslverr_i);
        prdata[0].sample(d->minion_apb_prdata0_i);
        prdata[1].sample(d->minion_apb_prdata1_i);
        prdata[2].sample(d->minion_apb_prdata2_i);
        prdata[3].sample(d->minion_apb_prdata3_i);
        prdata[4].sample(d->minion_apb_prdata4_i);
        prdata[5].sample(d->minion_apb_prdata5_i);
        prdata[6].sample(d->minion_apb_prdata6_i);
        prdata[7].sample(d->minion_apb_prdata7_i);
    }

    void check(CosimCtrl<Vcosim_bpam2minions_tb>& sim) const {
        sim.check(paddr.full(), "input toggle coverage: req_apb_paddr_i all bits saw 0 and 1");
        sim.check(pwrite.full(), "input toggle coverage: req_apb_pwrite_i saw 0 and 1");
        sim.check(psel.full(), "input toggle coverage: req_apb_psel_i saw 0 and 1");
        sim.check(penable.full(), "input toggle coverage: req_apb_penable_i saw 0 and 1");
        sim.check(pwdata.full(), "input toggle coverage: req_apb_pwdata_i all bits saw 0 and 1");
        sim.check(pready.full(), "input toggle coverage: minion_apb_pready_i all bits saw 0 and 1");
        sim.check(pslverr.full(), "input toggle coverage: minion_apb_pslverr_i all bits saw 0 and 1");
        for (int i = 0; i < 8; ++i) {
            sim.check(prdata[i].full(), "input toggle coverage: minion_apb_prdata" +
                                         std::to_string(i) + " all bits saw 0 and 1");
        }
    }
};

uint32_t make_addr(uint32_t minion, uint32_t cls, uint32_t pp, uint32_t thread, uint32_t reg) {
    return ((cls & 0x3u) << 15) | ((pp & 0x3u) << 13) | ((minion & 0x7u) << 10) |
           ((thread & 0x1u) << 9) | (reg & 0x1ffu);
}

void set_prdata(Vcosim_bpam2minions_tb* d, uint32_t idx, uint64_t value) {
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

void set_defaults(Vcosim_bpam2minions_tb* d) {
    d->req_apb_paddr_i = 0;
    d->req_apb_pwrite_i = 0;
    d->req_apb_psel_i = 0;
    d->req_apb_penable_i = 0;
    d->req_apb_pwdata_i = 0;
    d->minion_apb_pready_i = 0;
    d->minion_apb_pslverr_i = 0;
    for (uint32_t i = 0; i < 8; ++i) set_prdata(d, i, 0);
}

void compare_all(CosimCtrl<Vcosim_bpam2minions_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("rsp_apb_pready", d->orig_rsp_apb_pready_o, d->new_rsp_apb_pready_o);
    sim.compare("rsp_apb_prdata", d->orig_rsp_apb_prdata_o, d->new_rsp_apb_prdata_o);
    sim.compare("rsp_apb_pslverr", d->orig_rsp_apb_pslverr_o, d->new_rsp_apb_pslverr_o);
    sim.compare("minion_apb_paddr", d->orig_minion_apb_paddr_o, d->new_minion_apb_paddr_o);
    sim.compare("minion_apb_pwrite", d->orig_minion_apb_pwrite_o, d->new_minion_apb_pwrite_o);
    sim.compare("minion_apb_psel", d->orig_minion_apb_psel_o, d->new_minion_apb_psel_o);
    sim.compare("minion_apb_penable", d->orig_minion_apb_penable_o, d->new_minion_apb_penable_o);
    sim.compare("minion_apb_pwdata", d->orig_minion_apb_pwdata_o, d->new_minion_apb_pwdata_o);
}

void tick(CosimCtrl<Vcosim_bpam2minions_tb>& sim, Cov& cov) {
    cov.sample(sim.dut.get());
    sim.tick();
    compare_all(sim);
}

void drive_transaction(CosimCtrl<Vcosim_bpam2minions_tb>& sim, Cov& cov,
                       uint32_t minion, uint32_t cls, bool write, bool err,
                       int stall_cycles, uint64_t salt) {
    auto* d = sim.dut.get();
    const uint32_t addr = make_addr(minion, cls, (minion + cls) & 3u, minion & 1u,
                                    (0x40u + minion + cls) & 0x1ffu);
    const uint64_t rdata = 0x9000000000000000ull ^ (salt << 1) ^ minion;
    set_prdata(d, minion, rdata);
    d->minion_apb_pready_i = 0;
    d->minion_apb_pslverr_i = err ? (1u << minion) : 0u;
    d->req_apb_paddr_i = addr;
    d->req_apb_pwrite_i = write;
    d->req_apb_psel_i = 1;
    d->req_apb_penable_i = 0;
    d->req_apb_pwdata_i = 0x0102030405060708ull ^ salt;
    tick(sim, cov);

    d->req_apb_penable_i = 1;
    tick(sim, cov);
    for (int i = 0; i < stall_cycles; ++i) tick(sim, cov);
    d->minion_apb_pready_i = 1u << minion;
    tick(sim, cov);
    d->req_apb_psel_i = 0;
    d->req_apb_penable_i = 0;
    d->minion_apb_pready_i = 0;
    d->minion_apb_pslverr_i = 0;
    tick(sim, cov);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_bpam2minions_tb> sim(argc, argv);
    Cov cov;
    set_defaults(sim.dut.get());
    sim.reset();
    compare_all(sim);

    for (uint32_t minion = 0; minion < 8; ++minion) {
        drive_transaction(sim, cov, minion, minion & 3u, minion & 1u, (minion & 2u) != 0,
                          static_cast<int>(minion % 4), 0x1000ull + minion);
    }

    std::mt19937_64 rng(0xb2a42d55c0ffeeull);
    for (int cycle = 0; cycle < 1200; ++cycle) {
        auto* d = sim.dut.get();
        const uint64_t r0 = rng();
        const uint64_t r1 = rng();
        d->req_apb_paddr_i = r0 & 0x1ffffu;
        d->req_apb_pwrite_i = (r0 >> 17) & 1u;
        d->req_apb_psel_i = (cycle % 7 == 0) ? 0 : ((r0 >> 18) & 1u);
        d->req_apb_penable_i = (r0 >> 19) & 1u;
        d->req_apb_pwdata_i = r1;
        d->minion_apb_pready_i = (r0 >> 24) & 0xffu;
        d->minion_apb_pslverr_i = (r1 >> 8) & 0xffu;
        for (uint32_t i = 0; i < 8; ++i) set_prdata(d, i, rng());
        tick(sim, cov);
    }

    cov.check(sim);
    // Reset was driven low and high by CosimCtrl::reset(); all other inputs are
    // checked above for both 0 and 1 on every bit.
    return sim.finish();
}
