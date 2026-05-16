// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cstdio>
#include <string>

#include "Vbpam2minions_tb.h"
#include "sim_ctrl.h"

namespace {

constexpr uint32_t kMinions = 8;
constexpr uint32_t kDataWidth = 64;

uint32_t make_addr(uint32_t minion, uint32_t dest, uint32_t pp, uint32_t thread,
                   uint32_t reg, uint32_t ignored_class_bit = 0) {
    return ((dest & 0x1u) << 16) |
           ((ignored_class_bit & 0x1u) << 15) |
           ((pp & 0x3u) << 13) |
           ((minion & 0x7u) << 10) |
           ((thread & 0x1u) << 9) |
           (reg & 0x1ffu);
}

uint32_t expected_minion_addr(uint32_t dest, uint32_t pp, uint32_t thread,
                              uint32_t reg) {
    return ((dest & 0x1u) << 12) |
           ((pp & 0x3u) << 10) |
           ((thread & 0x1u) << 9) |
           (reg & 0x1ffu);
}

void set_defaults(Vbpam2minions_tb* d) {
    d->req_apb_paddr_i = 0;
    d->req_apb_pwrite_i = 0;
    d->req_apb_psel_i = 0;
    d->req_apb_penable_i = 0;
    d->req_apb_pwdata_i = 0;
    d->minion_apb_pready_i = 0;
    d->minion_apb_pslverr_i = 0;
    d->minion_apb_prdata0_i = 0x1000;
    d->minion_apb_prdata1_i = 0x2001;
    d->minion_apb_prdata2_i = 0x3002;
    d->minion_apb_prdata3_i = 0x4003;
    d->minion_apb_prdata4_i = 0x5004;
    d->minion_apb_prdata5_i = 0x6005;
    d->minion_apb_prdata6_i = 0x7006;
    d->minion_apb_prdata7_i = 0x8007;
}

uint64_t minion_data_for(uint32_t minion) {
    return 0x1111000000000000ull ^ (0x0101010101010101ull * minion) ^
           (0x55aa000000000000ull >> minion);
}

void set_minion_data(Vbpam2minions_tb* d, uint32_t minion, uint64_t value) {
    switch (minion) {
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

void check_reset(SimCtrl<Vbpam2minions_tb>& sim) {
    auto* d = sim.dut.get();
    sim.check(d->rsp_apb_pready_o == 0, "reset clears rsp_apb_pready");
    sim.check(d->minion_apb_psel_o == 0, "reset clears minion_apb_psel");
    sim.check(d->minion_apb_penable_o == 0, "reset clears minion_apb_penable");
}

void run_transaction(SimCtrl<Vbpam2minions_tb>& sim, uint32_t minion, uint32_t dest,
                     uint32_t pp, uint32_t thread, uint32_t reg, bool write,
                     bool slverr, int stall_cycles) {
    auto* d = sim.dut.get();
    const uint32_t addr = make_addr(minion, dest, pp, thread, reg, minion & 1u);
    const uint32_t encoded = expected_minion_addr(dest, pp, thread, reg);
    const uint64_t wdata = 0xa5a5000000000000ull ^ (static_cast<uint64_t>(addr) << 7) ^
                           (write ? 0x1234ull : 0x5678ull);
    const uint64_t rdata = minion_data_for(minion) ^ (static_cast<uint64_t>(reg) << 16);
    set_minion_data(d, minion, rdata);

    d->req_apb_paddr_i = addr;
    d->req_apb_pwrite_i = write;
    d->req_apb_psel_i = 1;
    d->req_apb_penable_i = 0;
    d->req_apb_pwdata_i = wdata;
    d->minion_apb_pready_i = 0;
    d->minion_apb_pslverr_i = slverr ? (1u << minion) : 0u;
    sim.tick();

    sim.check(d->minion_apb_psel_o == (1u << minion), "setup selects expected minion");
    sim.check(d->minion_apb_penable_o == 0, "setup cycle keeps minion penable low");
    sim.check(d->minion_apb_paddr_o == encoded, "setup captures encoded minion address");
    sim.check(d->minion_apb_pwrite_o == static_cast<uint8_t>(write), "setup captures pwrite");
    sim.check(d->minion_apb_pwdata_o == wdata, "setup captures pwdata");
    sim.check(d->rsp_apb_pready_o == 0, "setup does not complete BPAM response");

    d->req_apb_penable_i = 1;
    sim.tick();
    sim.check(d->minion_apb_psel_o == (1u << minion), "access holds selected minion psel");
    sim.check(d->minion_apb_penable_o == 1, "access asserts minion penable");
    sim.check(d->rsp_apb_pready_o == 0, "access waits for selected minion ready");

    for (int i = 0; i < stall_cycles; ++i) {
        d->minion_apb_pready_i = 0;
        sim.tick();
        sim.check(d->minion_apb_psel_o == (1u << minion), "stall holds selected minion psel");
        sim.check(d->minion_apb_penable_o == 1, "stall holds minion penable");
        sim.check(d->rsp_apb_pready_o == 0, "stall keeps BPAM pready low");
    }

    d->minion_apb_pready_i = (1u << minion);
    sim.tick();
    sim.check(d->minion_apb_psel_o == 0, "completion clears minion psel");
    sim.check(d->minion_apb_penable_o == 0, "completion clears minion penable");
    sim.check(d->rsp_apb_pready_o == 1, "completion pulses BPAM pready");
    sim.check(d->rsp_apb_prdata_o == rdata, "completion returns selected minion read data");
    sim.check(d->rsp_apb_pslverr_o == static_cast<uint8_t>(slverr),
              "completion returns selected minion slave error");

    d->req_apb_psel_i = 0;
    d->req_apb_penable_i = 0;
    d->minion_apb_pready_i = 0;
    d->minion_apb_pslverr_i = 0;
    sim.tick();
    sim.check(d->rsp_apb_pready_o == 0, "response pready self-clears after pulse");
    sim.check(d->minion_apb_psel_o == 0, "idle keeps minion psel clear");
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<Vbpam2minions_tb> sim(argc, argv);
    set_defaults(sim.dut.get());
    sim.reset();
    check_reset(sim);

    for (uint32_t minion = 0; minion < kMinions; ++minion) {
        run_transaction(sim, minion, minion & 1u, minion & 3u, (minion >> 1) & 1u,
                        0x20u + minion, minion & 1u, (minion & 2u) != 0,
                        static_cast<int>(minion % 3));
    }

    // Back-to-back requests after the registered pready pulse exercise the
    // req_start/minion_done update enable on minion_apb_psel_o.
    run_transaction(sim, 3, 1, 2, 1, 0x1fe, true, true, 0);
    run_transaction(sim, 4, 0, 1, 0, 0x101, false, false, 1);

    // Idle psel=0 cycles must not alter the registered request outputs.
    const uint32_t paddr_before = sim.dut->minion_apb_paddr_o;
    const uint64_t pwdata_before = sim.dut->minion_apb_pwdata_o;
    sim.dut->req_apb_paddr_i = make_addr(7, 1, 3, 1, 0x1ff);
    sim.dut->req_apb_pwdata_i = 0xffff0000ffff0000ull;
    sim.dut->req_apb_psel_i = 0;
    sim.dut->req_apb_penable_i = 0;
    sim.tick();
    sim.check(sim.dut->minion_apb_paddr_o == paddr_before,
              "idle cycle preserves unenabled minion paddr register");
    sim.check(sim.dut->minion_apb_pwdata_o == pwdata_before,
              "idle cycle preserves unenabled minion pwdata register");

    (void)kDataWidth;
    return sim.finish();
}
