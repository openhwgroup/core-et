// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <string>

#include "Vneigh_ch_apb_mux_tb.h"
#include "sim_ctrl.h"

namespace {

uint32_t minion_addr(uint32_t cls, uint32_t minion, uint32_t pp, uint32_t thread,
                     uint32_t reg) {
    return ((cls & 0x3u) << 15) |
           ((pp & 0x3u) << 13) |
           ((minion & 0x7u) << 10) |
           ((thread & 0x1u) << 9) |
           (reg & 0x1ffu);
}

uint32_t esr_addr(uint32_t low_addr) {
    return (1u << 15) | (low_addr & 0x7fffu);  // class 01: neighborhood ESR.
}

uint32_t icache_addr(uint32_t low_addr) {
    return (3u << 15) | (low_addr & 0x3ffu);  // class 11, bit14 clear.
}

uint32_t tbox_addr(uint32_t low_addr) {
    return (3u << 15) | (1u << 14) | (low_addr & 0xfffu);  // class 11, bit14 set.
}

uint32_t encoded_minion_addr(uint32_t cls, uint32_t pp, uint32_t thread, uint32_t reg) {
    const uint32_t dest = (cls >> 1) & 1u;
    return (dest << 12) | ((pp & 0x3u) << 10) | ((thread & 0x1u) << 9) | (reg & 0x1ffu);
}

void set_defaults(Vneigh_ch_apb_mux_tb* d) {
    d->apb_req_paddr_i = 0;
    d->apb_req_pwrite_i = 0;
    d->apb_req_psel_i = 0;
    d->apb_req_penable_i = 0;
    d->apb_req_pwdata_i = 0;
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

void set_req(Vneigh_ch_apb_mux_tb* d, uint32_t addr, bool write, bool psel,
             bool penable, uint64_t wdata) {
    d->apb_req_paddr_i = addr;
    d->apb_req_pwrite_i = write;
    d->apb_req_psel_i = psel;
    d->apb_req_penable_i = penable;
    d->apb_req_pwdata_i = wdata;
}

void check_zero_resource_reqs(SimCtrl<Vneigh_ch_apb_mux_tb>& sim, const std::string& tag) {
    auto* d = sim.dut.get();
    sim.check(d->esr_apb_psel_o == 0, tag + ": ESR psel zero");
    sim.check(d->icache_apb_psel_o == 0, tag + ": icache psel zero");
    sim.check(d->tbox_apb_psel_o == 0, tag + ": tbox psel zero");
}

void run_esr_decode(SimCtrl<Vneigh_ch_apb_mux_tb>& sim) {
    auto* d = sim.dut.get();
    const uint32_t low = 0x1234;
    const uint64_t wdata = 0x1111222233334444ull;
    d->esr_rsp_prdata_i = 0x0badcafe11223344ull;
    d->esr_rsp_pready_i = 1;
    d->esr_rsp_pslverr_i = 1;

    set_req(d, esr_addr(low), true, true, false, wdata);
    sim.tick();
    sim.check(d->esr_apb_paddr_o == low, "ESR setup forwards low 15-bit address");
    sim.check(d->esr_apb_pwrite_o == 1, "ESR setup forwards pwrite");
    sim.check(d->esr_apb_psel_o == 1, "ESR setup forwards psel");
    sim.check(d->esr_apb_penable_o == 0, "ESR setup forwards penable low");
    sim.check(d->esr_apb_pwdata_o == wdata, "ESR setup forwards pwdata");
    sim.check(d->icache_apb_psel_o == 0, "ESR setup disables icache request");
    sim.check(d->tbox_apb_psel_o == 0, "ESR setup keeps registered tbox request zero");
    sim.check(d->apb_rsp_prdata_o == 0, "ESR setup response is gated low");

    set_req(d, esr_addr(low), true, true, true, wdata);
    sim.tick();
    sim.check(d->esr_apb_penable_o == 1, "ESR access forwards penable high");
    sim.check(d->apb_rsp_prdata_o == 0, "ESR access first cycle still gated low");
    sim.tick();
    sim.check(d->apb_rsp_prdata_o == 0, "ESR valid gate adds one more registered cycle");
    sim.tick();
    sim.check(d->apb_rsp_prdata_o == 0x0badcafe11223344ull, "ESR response data appears after valid gate");
    sim.check(d->apb_rsp_pready_o == 1, "ESR response pready appears after valid gate");
    sim.check(d->apb_rsp_pslverr_o == 1, "ESR response pslverr appears after valid gate");

    set_req(d, 0, false, false, false, 0);
    sim.tick();
}

void run_icache_decode(SimCtrl<Vneigh_ch_apb_mux_tb>& sim) {
    auto* d = sim.dut.get();
    const uint32_t low = 0x155;
    const uint64_t wdata = 0x2222333344445555ull;
    d->icache_rsp_prdata_i = 0xfeed000000001234ull;
    d->icache_rsp_pready_i = 1;
    d->icache_rsp_pslverr_i = 0;

    set_req(d, icache_addr(low), false, true, false, wdata);
    sim.tick();
    sim.check(d->icache_apb_paddr_o == low, "icache setup forwards low 10-bit address");
    sim.check(d->icache_apb_pwrite_o == 0, "icache setup forwards read pwrite");
    sim.check(d->icache_apb_psel_o == 1, "icache setup forwards psel");
    sim.check(d->icache_apb_penable_o == 0, "icache setup forwards setup penable");
    sim.check(d->icache_apb_pwdata_o == wdata, "icache setup forwards pwdata");
    sim.check(d->esr_apb_psel_o == 0, "icache setup disables ESR request");
    sim.check(d->tbox_apb_psel_o == 0, "icache setup keeps tbox request zero");
    sim.tick();
    sim.check(d->apb_rsp_prdata_o == 0xfeed000000001234ull,
              "icache response is selected by registered shire response");
    sim.check(d->apb_rsp_pready_o == 1, "icache response forwards pready");
    sim.check(d->apb_rsp_pslverr_o == 0, "icache response forwards pslverr");
}

void run_tbox_decode(SimCtrl<Vneigh_ch_apb_mux_tb>& sim) {
    auto* d = sim.dut.get();
    const uint32_t low = 0xabc;
    const uint64_t wdata = 0x3333444455556666ull;
    d->tbox_rsp_prdata_i = 0x123456789abcdef0ull;
    d->tbox_rsp_pready_i = 1;
    d->tbox_rsp_pslverr_i = 1;

    set_req(d, tbox_addr(low), true, true, true, wdata);
    sim.tick();
    sim.check(d->tbox_apb_psel_o == 0, "tbox request remains registered one cycle after shire input");
    sim.check(d->icache_apb_psel_o == 0, "tbox setup disables icache request");
    sim.check(d->esr_apb_psel_o == 0, "tbox setup disables ESR request");
    sim.tick();
    sim.check(d->tbox_apb_paddr_o == low, "tbox registered request forwards low 12-bit address");
    sim.check(d->tbox_apb_pwrite_o == 1, "tbox registered request forwards pwrite");
    sim.check(d->tbox_apb_psel_o == 1, "tbox registered request forwards psel");
    sim.check(d->tbox_apb_penable_o == 1, "tbox registered request forwards penable");
    sim.check(d->tbox_apb_pwdata_o == wdata, "tbox registered request forwards pwdata");
    sim.check(d->apb_rsp_prdata_o == 0x123456789abcdef0ull, "tbox response forwards data");
    sim.check(d->apb_rsp_pready_o == 1, "tbox response forwards pready");
    sim.check(d->apb_rsp_pslverr_o == 1, "tbox response forwards pslverr");

    set_req(d, 0, false, false, false, 0);
    sim.tick();
    sim.tick();
    sim.check(d->tbox_apb_psel_o == 0, "idle clears registered tbox request after tbox access");
}

void set_minion_data(Vneigh_ch_apb_mux_tb* d, uint32_t minion, uint64_t value) {
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

void run_minion_decode(SimCtrl<Vneigh_ch_apb_mux_tb>& sim, uint32_t cls, uint32_t minion) {
    auto* d = sim.dut.get();
    const uint32_t pp = (minion + 1u) & 3u;
    const uint32_t thread = minion & 1u;
    const uint32_t reg = 0x40u + minion;
    const uint64_t wdata = 0x4444555500000000ull | (static_cast<uint64_t>(cls) << 8) | minion;
    const uint64_t rdata = 0xabc0000000000000ull | (static_cast<uint64_t>(minion) << 20) | cls;
    set_minion_data(d, minion, rdata);

    set_req(d, minion_addr(cls, minion, pp, thread, reg), true, true, false, wdata);
    d->minion_apb_pready_i = 0;
    d->minion_apb_pslverr_i = (1u << minion);
    sim.tick();
    check_zero_resource_reqs(sim, "minion class setup");
    sim.check(d->minion_apb_psel_o == 0, "minion bridge has one-cycle request-register latency");

    set_req(d, minion_addr(cls, minion, pp, thread, reg), true, true, true, wdata);
    sim.tick();
    sim.check(d->minion_apb_psel_o == (1u << minion), "minion bridge selects decoded minion");
    sim.check(d->minion_apb_penable_o == 0, "minion bridge setup keeps penable low");
    sim.check(d->minion_apb_paddr_o == encoded_minion_addr(cls, pp, thread, reg),
              "minion bridge forwards encoded address");
    sim.check(d->minion_apb_pwrite_o == 1, "minion bridge forwards pwrite");
    sim.check(d->minion_apb_pwdata_o == wdata, "minion bridge forwards pwdata");

    sim.tick();
    sim.check(d->minion_apb_penable_o == 1, "minion bridge asserts penable after start");
    d->minion_apb_pready_i = (1u << minion);
    sim.tick();
    sim.check(d->minion_apb_penable_o == 1, "registered minion ready is not consumed in same edge");
    sim.tick();
    sim.check(d->minion_apb_psel_o == 0, "minion bridge clears psel on registered ready");
    sim.check(d->minion_apb_penable_o == 0, "minion bridge clears penable on registered ready");
    sim.tick();
    sim.check(d->apb_rsp_prdata_o == rdata, "minion response is returned through shire response register");
    sim.check(d->apb_rsp_pready_o == 1, "minion response returns pready through shire response register");
    sim.check(d->apb_rsp_pslverr_o == 1, "minion response returns pslverr through shire response register");

    d->minion_apb_pready_i = 0;
    d->minion_apb_pslverr_i = 0;
    set_req(d, 0, false, false, false, 0);
    sim.tick();
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<Vneigh_ch_apb_mux_tb> sim(argc, argv);
    set_defaults(sim.dut.get());
    sim.reset();

    sim.check(sim.dut->apb_rsp_prdata_o == 0, "reset clears shire response data");
    sim.check(sim.dut->apb_rsp_pready_o == 0, "reset clears shire response pready");
    sim.check(sim.dut->apb_rsp_pslverr_o == 0, "reset clears shire response pslverr");
    sim.check(sim.dut->minion_apb_psel_o == 0, "reset clears minion psel");
    sim.check(sim.dut->tbox_apb_psel_o == 0, "reset clears registered tbox psel");

    run_esr_decode(sim);
    run_icache_decode(sim);
    run_tbox_decode(sim);
    run_minion_decode(sim, 0u, 2u);  // original minion-ESR class 00.
    run_minion_decode(sim, 2u, 6u);  // original minion-D$ class 10.

    return sim.finish();
}
