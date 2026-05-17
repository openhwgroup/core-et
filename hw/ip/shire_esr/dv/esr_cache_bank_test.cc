// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vesr_cache_bank_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

namespace {
constexpr uint16_t kMachine = 3u << 10;
constexpr uint16_t kUser = 0u << 10;
constexpr uint16_t kDebug = 2u << 10;
constexpr uint16_t kReqqCtl = kMachine | 0x001u;
constexpr uint16_t kPipeCtl = kMachine | 0x002u;
constexpr uint16_t kL2Ctl = kMachine | 0x003u;
constexpr uint16_t kL3Ctl = kMachine | 0x004u;
constexpr uint16_t kScpCtl = kMachine | 0x005u;
constexpr uint16_t kIdxCtl = kMachine | 0x006u;
constexpr uint16_t kErrInfo = kMachine | 0x00cu;
constexpr uint16_t kPerfCyc = kMachine | 0x018u;
constexpr uint16_t kIdxCtlUser = kUser | 0x020u;
constexpr uint16_t kTraceAddrEnable = kDebug | 0x3f0u;
constexpr uint16_t kTraceAddrValue = kDebug | 0x3f1u;
constexpr uint16_t kTraceCtl = kDebug | 0x3f2u;
constexpr uint16_t kBadAddr = kMachine | 0x155u;

void set_defaults(SimCtrl<Vesr_cache_bank_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_ni = 1;
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    d->apb_paddr_i = 0;
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwdata_i = 0;
    d->status_idx_ctl_i = 0x1111222233334444ull;
    d->status_idx_index_i = 0x2222333344445555ull;
    d->status_idx_data0_i = 0x3333444455556666ull;
    d->status_err_info_i = 0x4444555566667777ull;
    d->status_err_addr_i = 0x5555666677778888ull;
    d->status_counts_i = 0x6666777788889999ull;
    d->status_perf_cyc_i = 0x777788889999aaaauLL;
}

void reset_all(SimCtrl<Vesr_cache_bank_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_ni = 0;
    d->rst_c_ni = 0;
    d->rst_w_ni = 0;
    d->rst_d_ni = 0;
    for (int i = 0; i < 4; ++i) sim.tick();
    d->rst_ni = 1;
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    sim.tick();
}

void idle(SimCtrl<Vesr_cache_bank_tb>& sim, int cycles = 1) {
    auto* d = sim.dut.get();
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwrite_i = 0;
    for (int i = 0; i < cycles; ++i) sim.tick();
}

uint64_t apb_read(SimCtrl<Vesr_cache_bank_tb>& sim, uint16_t addr, bool expect_err = false) {
    auto* d = sim.dut.get();
    d->apb_paddr_i = addr;
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    d->apb_penable_i = 1;
    sim.tick();
    sim.check(d->apb_pready_o == 1, "cache bank APB pready tied high");
    sim.check(d->apb_pslverr_o == (expect_err ? 1 : 0), "cache bank APB read pslverr");
    uint64_t value = d->apb_prdata_o;
    idle(sim);
    return value;
}

void apb_write(SimCtrl<Vesr_cache_bank_tb>& sim, uint16_t addr, uint64_t data, bool expect_err = false) {
    auto* d = sim.dut.get();
    d->apb_paddr_i = addr;
    d->apb_pwdata_i = data;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    sim.check(d->access_in_flight_o == 1, "cache bank access_in_flight asserted on setup");
    d->apb_penable_i = 1;
    sim.tick();
    sim.check(d->apb_pslverr_o == (expect_err ? 1 : 0), "cache bank APB write pslverr");
    idle(sim);
}

void pulse_debug_reset(SimCtrl<Vesr_cache_bank_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_d_ni = 0;
    sim.tick();
    d->rst_d_ni = 1;
    sim.tick();
}

void pulse_cache_reset(SimCtrl<Vesr_cache_bank_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_c_ni = 0;
    sim.tick();
    d->rst_c_ni = 1;
    sim.tick();
}
}  // namespace

int main(int argc, char** argv) {
    SimCtrl<Vesr_cache_bank_tb> sim(argc, argv);
    set_defaults(sim);
    reset_all(sim);
    auto* d = sim.dut.get();

    sim.check(d->apb_pready_o == 1, "cache bank pready reset value");
    sim.check(d->cbuf_enable_o == 1, "REQ queue cbuf reset enabled");
    sim.check(d->num_l3_reqq_entries_o == 21, "REQ queue reset L3 entries = ReqqDepth/3");
    sim.check(d->l2_set_base_o == 640, "L2 set-base reset value");
    sim.check(d->l3_set_base_o == 768, "L3 set-base reset value");
    sim.check(d->scp_set_base_o == 0, "SCP set-base reset value");
    sim.check(d->idx_user_enable_o == 1, "IDX user control reset enabled");

    sim.check(apb_read(sim, kIdxCtl) == 0x1111222233334444ull, "IDX ctl read returns status input");
    sim.check(apb_read(sim, kErrInfo) == 0x4444555566667777ull, "error-log info read returns status input");
    sim.check(apb_read(sim, kPerfCyc) == 0x777788889999aaaauLL, "perf cycle counter read returns status input");
    (void)apb_read(sim, kBadAddr, true);

    apb_write(sim, kReqqCtl, 0);
    sim.check(d->cbuf_enable_o == 0, "REQ queue write updates cbuf enable");
    sim.check(d->num_l3_reqq_entries_o == 0, "REQ queue write updates L3 entries");

    apb_write(sim, kL2Ctl, (uint64_t{0x12} << 48) | (uint64_t{0x123} << 32) |
                           (uint64_t{0x3f} << 16) | 0x55u);
    sim.check(d->l2_set_base_o == 0x12, "L2 cache-ctl write updates set base");

    d->apb_paddr_i = kIdxCtlUser;
    d->apb_pwdata_i = 0x0a02u;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    sim.check(d->wr_en_idx_ctl_o == 1, "user IDX write generates IDX ctl write enable");
    sim.check((d->ctl_wr_data_o & 0x2u) == 0, "user IDX write masks privileged bit 1");
    d->apb_penable_i = 1;
    sim.tick();
    idle(sim);

    apb_write(sim, kPipeCtl, 0);
    d->apb_paddr_i = kIdxCtlUser;
    d->apb_pwdata_i = 0x0a02u;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    sim.check(d->wr_en_idx_ctl_o == 0, "user IDX write blocked when user enable is cleared");
    d->apb_penable_i = 1;
    sim.tick();
    idle(sim);

    apb_write(sim, kTraceCtl, 0x12345u);
    idle(sim, 16);
    sim.check(d->access_in_flight_o == 0, "access_in_flight drains after timeout window");

    constexpr uint64_t kTraceEnableBeforeD = 0x000000f00dull;
    constexpr uint64_t kTraceEnableAfterD = 0x0000000aceull;
    constexpr uint64_t kTraceAddressValue = 0x0012345678ull;
    apb_write(sim, kTraceAddrEnable, kTraceEnableBeforeD);
    apb_write(sim, kTraceAddrValue, kTraceAddressValue);
    apb_write(sim, kTraceCtl, 0x5a5u);
    sim.check(d->trace_address_enable_o == kTraceEnableBeforeD,
              "trace address-enable write visible before debug reset");
    sim.check(d->trace_address_value_o == kTraceAddressValue,
              "trace address-value write visible before reset pulses");
    sim.check(d->trace_type_hot_enable_o == 0x5a5u,
              "trace control write visible before debug reset");

    pulse_debug_reset(sim);
    sim.check(d->trace_address_enable_o == 0,
              "debug reset clears trace address-enable register");
    sim.check(d->trace_type_hot_enable_o == 0,
              "debug reset clears trace control register");
    sim.check(d->trace_address_value_o == kTraceAddressValue,
              "debug reset preserves no-reset trace address-value register");

    apb_write(sim, kTraceAddrEnable, kTraceEnableAfterD);
    apb_write(sim, kTraceCtl, 0x3c3u);
    pulse_cache_reset(sim);
    sim.check(d->cbuf_enable_o == 1, "cache reset restores REQ queue cbuf enable");
    sim.check(d->num_l3_reqq_entries_o == 21,
              "cache reset restores REQ queue L3 entries default");
    sim.check(d->l2_set_base_o == 640, "cache reset restores L2 set-base default");
    sim.check(d->trace_address_enable_o == kTraceEnableAfterD,
              "cache reset preserves debug-reset trace address-enable register");
    sim.check(d->trace_type_hot_enable_o == 0x3c3u,
              "cache reset preserves debug-reset trace control register");
    sim.check(d->trace_address_value_o == kTraceAddressValue,
              "cache reset preserves no-reset trace address-value register");

    return sim.finish();
}
