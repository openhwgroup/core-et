// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vesr_neigh_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

namespace {
constexpr uint64_t kPpUser = 0ull << 13;
constexpr uint64_t kPpDebug = 2ull << 13;
constexpr uint64_t kPpMachine = 3ull << 13;
constexpr uint64_t kAddrMinionBoot = kPpMachine | 0x003ull;
constexpr uint64_t kAddrMprot = kPpMachine | 0x004ull;
constexpr uint64_t kAddrVmspagesize = kPpMachine | 0x007ull;
constexpr uint64_t kAddrIpiRedirectPc = kPpUser | 0x008ull;
constexpr uint64_t kAddrHactrl = kPpDebug | 0x1ff0ull;
constexpr uint64_t kAddrHastatus0 = kPpDebug | 0x1ff1ull;
constexpr uint64_t kAddrHastatus1 = kPpDebug | 0x1ff2ull;
constexpr uint64_t kAddrAndOrTreeL0 = kPpDebug | 0x1ff3ull;
constexpr uint64_t kAddrPmuCtrl = kPpMachine | 0x00dull;
constexpr uint64_t kAddrNeighChicken = kPpMachine | 0x00eull;
constexpr uint64_t kAddrIcacheErrLogCtl = kPpMachine | 0x00full;
constexpr uint64_t kAddrIcacheErrLogInfo = kPpMachine | 0x010ull;
constexpr uint64_t kAddrIcacheErrLogAddress = kPpMachine | 0x011ull;
constexpr uint64_t kAddrIcacheCounts = kPpMachine | 0x012ull;
constexpr uint64_t kAddrTextureControl = kPpUser | 0x1000ull;
constexpr uint64_t kAddrTextureStatus = kPpUser | 0x1001ull;
constexpr uint64_t kAddrTexturePtr = kPpUser | 0x1002ull;

constexpr uint64_t kMinionBootReset = 0x8000001000ull;
constexpr uint64_t kMprotReset = 0x13ull;
constexpr uint64_t kIcacheCtlReset = 0x6ull;
constexpr uint64_t kTextureCtlReset = 0x5ull;

uint64_t mask(unsigned bits) {
    return (bits >= 64) ? ~0ull : ((1ull << bits) - 1ull);
}

uint64_t pack_hastatus0(uint16_t havereset, uint16_t resumeack,
                        uint16_t running, uint16_t halted) {
    return (uint64_t{havereset} << 48) | (uint64_t{resumeack} << 32) |
           (uint64_t{running} << 16) | halted;
}

uint64_t pack_hastatus1(uint16_t error, uint16_t exception, uint16_t busy) {
    return (uint64_t{error} << 32) | (uint64_t{exception} << 16) | busy;
}

uint16_t hactrl_resethalt(uint64_t hactrl) { return static_cast<uint16_t>((hactrl >> 32) & 0xffffu); }
uint16_t hactrl_hartmask(uint64_t hactrl) { return static_cast<uint16_t>((hactrl >> 16) & 0xffffu); }
uint16_t hactrl_hawindow(uint64_t hactrl) { return static_cast<uint16_t>(hactrl & 0xffffu); }

uint64_t pack_dmctrl(bool dmactive, bool ndmreset, bool clr, bool set,
                     bool hasel, bool ack, bool hartreset, bool resume,
                     bool halt, bool write_enable) {
    return (uint64_t{dmactive} << 9) | (uint64_t{ndmreset} << 8) |
           (uint64_t{clr} << 7) | (uint64_t{set} << 6) |
           (uint64_t{hasel} << 5) | (uint64_t{ack} << 4) |
           (uint64_t{hartreset} << 3) | (uint64_t{resume} << 2) |
           (uint64_t{halt} << 1) | uint64_t{write_enable};
}

uint64_t pack_icache_info(uint8_t way, uint8_t set, uint8_t dbe,
                          uint8_t sbe, uint64_t address_ca) {
    return ((uint64_t{way} & 0x3ull) << 57) |
           ((uint64_t{set} & 0x7full) << 50) |
           ((uint64_t{dbe} & 0xffull) << 42) |
           ((uint64_t{sbe} & 0xffull) << 34) |
           (address_ca & mask(34));
}

uint64_t pack_log_clear(uint8_t err_code) {
    return 1ull | ((uint64_t{err_code} & 0xfu) << 4);
}

void set_defaults(SimCtrl<Vesr_neigh_tb>& sim) {
    auto* d = sim.dut.get();
    d->rst_ni = 1;
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    d->test_en_i = 0;
    d->dft_reset_byp_i = 0;
    d->dft_reset_i = 0;
    d->apb_paddr_i = 0;
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwdata_i = 0;
    d->hastatus0_i = 0;
    d->and_or_tree_l0_i = 0;
    d->hastatus1_min_i = 0;
    d->dmctrl_i = 0;
    d->icache_sbe_i = 0;
    d->icache_dbe_i = 0;
    d->icache_err_info_i = 0;
    d->texture_status_i = 0;
}

void reset_all(SimCtrl<Vesr_neigh_tb>& sim) {
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
    for (int i = 0; i < 3; ++i) sim.tick();
}

void idle(SimCtrl<Vesr_neigh_tb>& sim, int cycles = 1) {
    auto* d = sim.dut.get();
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwrite_i = 0;
    for (int i = 0; i < cycles; ++i) sim.tick();
}

uint64_t apb_read(SimCtrl<Vesr_neigh_tb>& sim, uint64_t addr, bool expect_err = false) {
    auto* d = sim.dut.get();
    d->apb_paddr_i = static_cast<uint16_t>(addr);
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    d->apb_penable_i = 1;
    sim.tick();
    sim.check(d->apb_pready_o == 1, "APB pready is tied high");
    sim.check(d->apb_pslverr_o == (expect_err ? 1 : 0),
              std::string("APB pslverr for address 0x") + std::to_string(addr));
    uint64_t value = d->apb_prdata_o;
    idle(sim);
    return value;
}

void apb_write(SimCtrl<Vesr_neigh_tb>& sim, uint64_t addr, uint64_t data,
               bool expect_err = false) {
    auto* d = sim.dut.get();
    d->apb_paddr_i = static_cast<uint16_t>(addr);
    d->apb_pwdata_i = data;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    sim.tick();
    d->apb_penable_i = 1;
    sim.tick();
    sim.check(d->apb_pready_o == 1, "APB write pready is tied high");
    sim.check(d->apb_pslverr_o == (expect_err ? 1 : 0), "APB write pslverr");
    idle(sim);
}
}  // namespace

int main(int argc, char** argv) {
    SimCtrl<Vesr_neigh_tb> sim(argc, argv);
    set_defaults(sim);
    reset_all(sim);
    auto* d = sim.dut.get();

    sim.check(d->minion_boot_o == kMinionBootReset, "minion boot reset value");
    sim.check(d->mprot_o == kMprotReset, "MPROT reset value");
    sim.check(d->vmspagesize_o == 0, "VMSPAGESIZE reset value is 4K");
    sim.check(d->ipi_redirect_pc_o == 0, "IPI redirect reset value");
    sim.check(d->hactrl_o == 0, "HACTRL reset value");
    sim.check(d->pmu_ctrl_o == 0, "PMU reset value");
    sim.check(d->neigh_chicken_o == 0, "neigh chicken reset value");
    sim.check(d->icache_err_log_ctl_o == kIcacheCtlReset, "icache err ctl reset value");
    sim.check(d->texture_control_o == kTextureCtlReset, "texture control reset value");
    sim.check(d->texture_image_table_ptr_o == 0, "texture image table ptr reset value");
    sim.check(d->icache_err_detected_o == 0, "icache detected reset value");
    sim.check(d->icache_err_logged_o == 0, "icache logged reset value");
    sim.check(d->icache_ecc_count_ov_o == 0, "icache count overflow reset value");

    sim.check(apb_read(sim, kAddrMinionBoot) == kMinionBootReset, "read MINION_BOOT reset");
    sim.check(apb_read(sim, kAddrMprot) == kMprotReset, "read MPROT reset");
    sim.check(apb_read(sim, kAddrVmspagesize) == 0, "read VMSPAGESIZE reset");
    sim.check(apb_read(sim, kAddrIcacheErrLogCtl) == kIcacheCtlReset, "read ICACHE_ERR_LOG_CTL reset");
    sim.check(apb_read(sim, kAddrTextureControl) == kTextureCtlReset, "read TEXTURE_CONTROL reset");

    d->hastatus0_i = pack_hastatus0(0x1234, 0x5678, 0x9abc, 0xdef0);
    d->and_or_tree_l0_i = 0x2aa;
    d->texture_status_i = 0xa5a5;
    sim.check(apb_read(sim, kAddrHastatus0) == d->hastatus0_i, "read RO HASTATUS0 input");
    sim.check((apb_read(sim, kAddrAndOrTreeL0) & 0x3ffu) == 0x2aau, "read RO AND_OR_TREEL0 input");
    sim.check((apb_read(sim, kAddrTextureStatus) & 0xffffu) == 0xa5a5u, "read RO TEXTURE_STATUS input");
    (void)apb_read(sim, 0x1555, true);
    apb_write(sim, 0x1556, 0xdeadbeef, true);

    apb_write(sim, kAddrMinionBoot, 0x123456789abull);
    sim.check(d->minion_boot_o == 0x123456789abull, "write MINION_BOOT");
    apb_write(sim, kAddrMprot, 0x6d);
    sim.check(d->mprot_o == 0x6d, "write MPROT");
    apb_write(sim, kAddrVmspagesize, 0x2);
    sim.check(d->vmspagesize_o == 0x2, "write VMSPAGESIZE 2M");
    apb_write(sim, kAddrIpiRedirectPc, 0xffffffffffffffffull);
    sim.check(d->ipi_redirect_pc_o == 0x0000fffffffffffeull, "IPI redirect clears reserved bit 0");
    apb_write(sim, kAddrPmuCtrl, 0x1);
    sim.check(d->pmu_ctrl_o == 0x1, "write PMU_CTRL");
    apb_write(sim, kAddrNeighChicken, 0xad);
    sim.check(d->neigh_chicken_o == 0xad, "write NEIGH_CHICKEN");
    apb_write(sim, kAddrIcacheErrLogCtl, 0xf);
    sim.check(d->icache_err_log_ctl_o == 0xf, "write ICACHE_ERR_LOG_CTL");
    apb_write(sim, kAddrTextureControl, 0x5a5a);
    sim.check(d->texture_control_o == 0x5a5a, "write TEXTURE_CONTROL");
    apb_write(sim, kAddrTexturePtr, 0x00fedcba9876ull);
    sim.check(d->texture_image_table_ptr_o == 0x00fedcba9876ull, "write TEXTURE_IMAGE_TABLE_PTR");

    apb_write(sim, kAddrHactrl, (0x0000ull << 32) | (0x0002ull << 16) | 0x0001ull);
    sim.check(hactrl_hawindow(d->hactrl_o) == 0x0001, "HACTRL hawindow write");
    sim.check(hactrl_hartmask(d->hactrl_o) == 0x0002, "HACTRL hartmask write");
    sim.check(hactrl_resethalt(d->hactrl_o) == 0x0000, "HACTRL resethalt is RO to APB write");
    d->dmctrl_i = pack_dmctrl(true, false, false, true, true, false, false, false, false, true);
    sim.tick();
    d->dmctrl_i = 0;
    sim.tick();
    sim.check((hactrl_resethalt(d->hactrl_o) & 0x0003u) == 0x0003u,
              "DMCTRL setresethaltreq updates selected harts");
    d->dmctrl_i = pack_dmctrl(true, false, true, false, true, false, false, false, false, true);
    sim.tick();
    d->dmctrl_i = 0;
    sim.tick();
    sim.check((hactrl_resethalt(d->hactrl_o) & 0x0003u) == 0x0000u,
              "DMCTRL clresethaltreq clears selected resethalt bits");

    d->hastatus1_min_i = pack_hastatus1(0x00f0, 0x0f00, 0xaaaa);
    sim.tick();
    d->hastatus1_min_i = pack_hastatus1(0x0000, 0x0000, 0x5555);
    sim.tick();
    uint64_t hastatus1 = apb_read(sim, kAddrHastatus1);
    sim.check(((hastatus1 >> 32) & 0xffffu) == 0x00f0u, "HASTATUS1 sticky error bits set");
    sim.check(((hastatus1 >> 16) & 0xffffu) == 0x0f00u, "HASTATUS1 sticky exception bits set");
    sim.check((hastatus1 & 0xffffu) == 0x5555u, "HASTATUS1 busy is live RO input");
    apb_write(sim, kAddrHastatus1, 0x0);
    sim.check(((apb_read(sim, kAddrHastatus1) >> 16) & 0xffffffffu) == 0,
              "HASTATUS1 write clears sticky fields when minion inputs are low");

    reset_all(sim);
    apb_write(sim, kAddrIcacheErrLogCtl, kIcacheCtlReset);
    d->icache_err_info_i = pack_icache_info(1, 0x12, 0x5a, 0xa5, 0x123456789ull);
    d->icache_sbe_i = 1;
    sim.tick();
    d->icache_sbe_i = 0;
    sim.tick();
    uint64_t log_info = apb_read(sim, kAddrIcacheErrLogInfo);
    sim.check((log_info & 0x1u) == 1, "SBE creates valid icache log entry");
    sim.check(((log_info >> 4) & 0xfu) == 0, "SBE log err_code");
    sim.check(((log_info >> 2) & 0x1u) == 0, "SBE log disabled by reset interrupt mask");
    sim.check((apb_read(sim, kAddrIcacheCounts) & 0xffu) == 1, "SBE counter increments");

    d->icache_err_info_i = pack_icache_info(2, 0x34, 0x3c, 0xc3, 0x2abcdef01ull);
    d->icache_dbe_i = 1;
    sim.tick();
    d->icache_dbe_i = 0;
    sim.tick();
    sim.tick();
    log_info = apb_read(sim, kAddrIcacheErrLogInfo);
    sim.check((log_info & 0x1u) == 1, "DBE keeps log valid");
    sim.check(((log_info >> 4) & 0xfu) == 1, "DBE log err_code");
    sim.check(((log_info >> 2) & 0x1u) == 1, "DBE log enabled");
    sim.check(((log_info >> 3) & 0x1u) == 1, "DBE log imprecise when L2 responses disabled");
    sim.check(d->icache_err_detected_o == 1, "icache_err_detected follows imprecise enabled log");
    sim.check(d->icache_err_logged_o == 1, "icache_err_logged follows enabled log");
    sim.check(apb_read(sim, kAddrIcacheErrLogAddress) == (0x2abcdef01ull & mask(34)),
              "icache error address logged");
    apb_write(sim, kAddrIcacheErrLogInfo, pack_log_clear(1));
    sim.check((apb_read(sim, kAddrIcacheErrLogInfo) & 0x1u) == 0,
              "matching valid/code write clears icache error log");
    apb_write(sim, kAddrIcacheCounts, 0x7ff);
    sim.check(apb_read(sim, kAddrIcacheCounts) == 0, "all-ones write clears SBE/DBE counters");

    apb_write(sim, kAddrIcacheErrLogCtl, 0xe);
    for (int i = 0; i < 260; ++i) {
        d->icache_sbe_i = 1;
        sim.tick();
    }
    d->icache_sbe_i = 0;
    sim.tick();
    sim.check((apb_read(sim, kAddrIcacheCounts) & 0xffu) == 0xffu,
              "SBE counter saturates at all ones");
    sim.check(d->icache_ecc_count_ov_o == 1, "ECC counter overflow output is gated by ctl");

    d->dft_reset_byp_i = 1;
    d->dft_reset_i = 1;
    sim.tick();
    d->dft_reset_byp_i = 0;
    d->dft_reset_i = 0;
    sim.tick();
    sim.tick();
    sim.check(d->minion_boot_o == kMinionBootReset, "DFT reset bypass resets latch-backed C-domain register");
    sim.check(d->vmspagesize_o == 0, "DFT reset bypass resets latch-backed W-domain register");

    apb_write(sim, kAddrHactrl, (0x0000ull << 32) | (0x0004ull << 16) | 0x0004ull);
    d->rst_w_ni = 0;
    sim.tick();
    d->rst_w_ni = 1;
    sim.tick();
    sim.tick();
    sim.check(d->texture_control_o == kTextureCtlReset, "warm reset restores W-domain texture control");
    sim.check(d->hactrl_o != 0, "warm reset does not clear D-domain hactrl");
    d->rst_d_ni = 0;
    sim.tick();
    d->rst_d_ni = 1;
    sim.tick();
    sim.check(d->hactrl_o == 0, "debug reset clears D-domain hactrl");

    return sim.finish();
}
