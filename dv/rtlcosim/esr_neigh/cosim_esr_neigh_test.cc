// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_esr_neigh_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <random>
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

constexpr std::array<uint64_t, 20> kAddresses = {
    kAddrMinionBoot, kAddrMprot, kAddrVmspagesize, kAddrIpiRedirectPc,
    kAddrHactrl, kAddrHastatus0, kAddrHastatus1, kAddrAndOrTreeL0,
    kAddrPmuCtrl, kAddrNeighChicken, kAddrIcacheErrLogCtl,
    kAddrIcacheErrLogInfo, kAddrIcacheErrLogAddress, kAddrIcacheCounts,
    kAddrTextureControl, kAddrTextureStatus, kAddrTexturePtr,
    0x1555, 0x3fff, 0x0001};

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

struct BitCoverage {
    uint64_t ones = 0;
    uint64_t zeros = 0;
    unsigned width = 1;

    void sample(uint64_t value) {
        const uint64_t m = mask(width);
        ones |= value & m;
        zeros |= (~value) & m;
    }

    void check(CosimCtrl<Vcosim_esr_neigh_tb>& sim, const std::string& name) const {
        const uint64_t m = mask(width);
        sim.check((ones & m) == m, name + " saw every bit high");
        sim.check((zeros & m) == m, name + " saw every bit low");
    }
};

struct CoverageTracker {
    BitCoverage rst_c{0, 0, 1};
    BitCoverage rst_w{0, 0, 1};
    BitCoverage rst_d{0, 0, 1};
    BitCoverage test_en{0, 0, 1};
    BitCoverage dft_byp{0, 0, 1};
    BitCoverage dft_rst{0, 0, 1};
    BitCoverage paddr{0, 0, 15};
    BitCoverage pwrite{0, 0, 1};
    BitCoverage psel{0, 0, 1};
    BitCoverage penable{0, 0, 1};
    BitCoverage pwdata{0, 0, 64};
    BitCoverage hastatus0{0, 0, 64};
    BitCoverage and_or{0, 0, 10};
    BitCoverage hastatus1{0, 0, 48};
    BitCoverage dmctrl{0, 0, 10};
    BitCoverage sbe{0, 0, 1};
    BitCoverage dbe{0, 0, 1};
    BitCoverage err_info{0, 0, 59};
    BitCoverage tex_status{0, 0, 16};

    void sample(const Vcosim_esr_neigh_tb* d) {
        rst_c.sample(d->rst_c_ni);
        rst_w.sample(d->rst_w_ni);
        rst_d.sample(d->rst_d_ni);
        test_en.sample(d->test_en_i);
        dft_byp.sample(d->dft_reset_byp_i);
        dft_rst.sample(d->dft_reset_i);
        paddr.sample(d->apb_paddr_i);
        pwrite.sample(d->apb_pwrite_i);
        psel.sample(d->apb_psel_i);
        penable.sample(d->apb_penable_i);
        pwdata.sample(d->apb_pwdata_i);
        hastatus0.sample(d->hastatus0_i);
        and_or.sample(d->and_or_tree_l0_i);
        hastatus1.sample(d->hastatus1_min_i);
        dmctrl.sample(d->dmctrl_i);
        sbe.sample(d->icache_sbe_i);
        dbe.sample(d->icache_dbe_i);
        err_info.sample(d->icache_err_info_i);
        tex_status.sample(d->texture_status_i);
    }

    void check(CosimCtrl<Vcosim_esr_neigh_tb>& sim) const {
        rst_c.check(sim, "rst_c_ni");
        rst_w.check(sim, "rst_w_ni");
        rst_d.check(sim, "rst_d_ni");
        test_en.check(sim, "test_en_i");
        dft_byp.check(sim, "dft_reset_byp_i");
        dft_rst.check(sim, "dft_reset_i");
        paddr.check(sim, "apb_paddr_i");
        pwrite.check(sim, "apb_pwrite_i");
        psel.check(sim, "apb_psel_i");
        penable.check(sim, "apb_penable_i");
        pwdata.check(sim, "apb_pwdata_i");
        hastatus0.check(sim, "hastatus0_i");
        and_or.check(sim, "and_or_tree_l0_i");
        hastatus1.check(sim, "hastatus1_min_i");
        dmctrl.check(sim, "dmctrl_i");
        sbe.check(sim, "icache_sbe_i");
        dbe.check(sim, "icache_dbe_i");
        err_info.check(sim, "icache_err_info_i");
        tex_status.check(sim, "texture_status_i");
    }
};

void compare_all(CosimCtrl<Vcosim_esr_neigh_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("esr_minion_boot", d->orig_minion_boot_o, d->new_minion_boot_o);
    sim.compare("esr_mprot", d->orig_mprot_o, d->new_mprot_o);
    sim.compare("esr_vmspagesize", d->orig_vmspagesize_o, d->new_vmspagesize_o);
    sim.compare("esr_ipi_redirect_pc", d->orig_ipi_redirect_pc_o, d->new_ipi_redirect_pc_o);
    sim.compare("esr_hactrl", d->orig_hactrl_o, d->new_hactrl_o);
    sim.compare("esr_pmu_ctrl", d->orig_pmu_ctrl_o, d->new_pmu_ctrl_o);
    sim.compare("esr_neigh_chicken", d->orig_neigh_chicken_o, d->new_neigh_chicken_o);
    sim.compare("esr_icache_err_log_ctl", d->orig_icache_err_log_ctl_o, d->new_icache_err_log_ctl_o);
    sim.compare("esr_texture_control", d->orig_texture_control_o, d->new_texture_control_o);
    sim.compare("esr_texture_image_table_ptr", d->orig_texture_image_table_ptr_o,
                d->new_texture_image_table_ptr_o);
    sim.compare("esr_icache_err_detected", d->orig_icache_err_detected_o,
                d->new_icache_err_detected_o);
    sim.compare("esr_icache_err_logged", d->orig_icache_err_logged_o,
                d->new_icache_err_logged_o);
    sim.compare("esr_icache_ecc_count_ov", d->orig_icache_ecc_count_ov_o,
                d->new_icache_ecc_count_ov_o);
    sim.compare("apb_pready", d->orig_apb_pready_o, d->new_apb_pready_o);
    sim.compare("apb_prdata", d->orig_apb_prdata_o, d->new_apb_prdata_o);
    sim.compare("apb_pslverr", d->orig_apb_pslverr_o, d->new_apb_pslverr_o);
}

void tick(CosimCtrl<Vcosim_esr_neigh_tb>& sim, CoverageTracker& cov) {
    sim.tick();
    cov.sample(sim.dut.get());
    compare_all(sim);
}

void set_defaults(CosimCtrl<Vcosim_esr_neigh_tb>& sim) {
    auto* d = sim.dut.get();
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

void reset_all(CosimCtrl<Vcosim_esr_neigh_tb>& sim, CoverageTracker& cov) {
    auto* d = sim.dut.get();
    d->rst_c_ni = 0;
    d->rst_w_ni = 0;
    d->rst_d_ni = 0;
    for (int i = 0; i < 5; ++i) tick(sim, cov);
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    for (int i = 0; i < 4; ++i) tick(sim, cov);
}

void idle(CosimCtrl<Vcosim_esr_neigh_tb>& sim, CoverageTracker& cov, int cycles = 1) {
    auto* d = sim.dut.get();
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwrite_i = 0;
    for (int i = 0; i < cycles; ++i) tick(sim, cov);
}

void apb_write(CosimCtrl<Vcosim_esr_neigh_tb>& sim, CoverageTracker& cov,
               uint64_t addr, uint64_t data) {
    auto* d = sim.dut.get();
    d->apb_paddr_i = static_cast<uint16_t>(addr);
    d->apb_pwdata_i = data;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    tick(sim, cov);
    d->apb_penable_i = 1;
    tick(sim, cov);
    idle(sim, cov);
}

void apb_read(CosimCtrl<Vcosim_esr_neigh_tb>& sim, CoverageTracker& cov, uint64_t addr) {
    auto* d = sim.dut.get();
    d->apb_paddr_i = static_cast<uint16_t>(addr);
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    tick(sim, cov);
    d->apb_penable_i = 1;
    tick(sim, cov);
    idle(sim, cov);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_esr_neigh_tb> sim(argc, argv);
    CoverageTracker cov;
    set_defaults(sim);
    reset_all(sim, cov);
    auto* d = sim.dut.get();

    d->hastatus0_i = pack_hastatus0(0xffff, 0x0000, 0xa5a5, 0x5a5a);
    d->and_or_tree_l0_i = 0x3ff;
    d->hastatus1_min_i = pack_hastatus1(0xffff, 0x0000, 0xa5a5);
    d->texture_status_i = 0xffff;
    d->dmctrl_i = pack_dmctrl(true, true, true, true, true, true, true, true, true, true);
    d->icache_sbe_i = 1;
    d->icache_dbe_i = 1;
    d->icache_err_info_i = pack_icache_info(3, 0x7f, 0xff, 0xff, mask(34));
    d->test_en_i = 1;
    tick(sim, cov);
    d->dmctrl_i = 0;
    d->icache_sbe_i = 0;
    d->icache_dbe_i = 0;
    d->test_en_i = 0;
    tick(sim, cov);

    for (uint64_t addr : kAddresses) {
        apb_read(sim, cov, addr);
    }

    apb_write(sim, cov, kAddrMinionBoot, 0x123456789abull);
    apb_write(sim, cov, kAddrMprot, 0x7full);
    apb_write(sim, cov, kAddrVmspagesize, 0x3ull);
    apb_write(sim, cov, kAddrIpiRedirectPc, 0xffffffffffffffffull);
    apb_write(sim, cov, kAddrHactrl, (0x0000ull << 32) | (0x00ffull << 16) | 0x0f0full);
    apb_write(sim, cov, kAddrHastatus1, pack_hastatus1(0xaaaa, 0x5555, 0xffff));
    apb_write(sim, cov, kAddrPmuCtrl, 0x1ull);
    apb_write(sim, cov, kAddrNeighChicken, 0xffull);
    apb_write(sim, cov, kAddrIcacheErrLogCtl, 0xeull);
    apb_write(sim, cov, kAddrTextureControl, 0xffffull);
    apb_write(sim, cov, kAddrTexturePtr, 0x00ffffffffffull);

    d->dmctrl_i = pack_dmctrl(true, false, false, true, true, false, false, false, false, true);
    tick(sim, cov);
    d->dmctrl_i = pack_dmctrl(true, false, true, false, false, false, false, false, false, true);
    tick(sim, cov);
    d->dmctrl_i = 0;
    tick(sim, cov);

    d->icache_err_info_i = pack_icache_info(1, 0x12, 0x5a, 0xa5, 0x123456789ull);
    d->icache_sbe_i = 1;
    tick(sim, cov);
    d->icache_sbe_i = 0;
    tick(sim, cov);
    d->icache_err_info_i = pack_icache_info(2, 0x34, 0x3c, 0xc3, 0x2abcdef01ull);
    d->icache_dbe_i = 1;
    tick(sim, cov);
    d->icache_dbe_i = 0;
    tick(sim, cov);
    apb_write(sim, cov, kAddrIcacheErrLogInfo, 1ull | (1ull << 4));
    apb_write(sim, cov, kAddrIcacheCounts, 0x7ffull);

    d->dft_reset_byp_i = 1;
    d->dft_reset_i = 1;
    tick(sim, cov);
    d->dft_reset_i = 0;
    tick(sim, cov);
    d->dft_reset_byp_i = 0;
    tick(sim, cov);

    d->rst_w_ni = 0;
    tick(sim, cov);
    d->rst_w_ni = 1;
    tick(sim, cov);
    d->rst_d_ni = 0;
    tick(sim, cov);
    d->rst_d_ni = 1;
    tick(sim, cov);
    d->rst_c_ni = 0;
    tick(sim, cov);
    d->rst_c_ni = 1;
    tick(sim, cov);

    std::mt19937_64 rng(0x5eed1234ULL);
    int apb_phase = 0;
    uint16_t held_addr = 0;
    uint64_t held_wdata = 0;
    bool held_write = false;

    for (int cycle = 0; cycle < 6000; ++cycle) {
        const uint64_t r0 = rng();
        const uint64_t r1 = rng();
        d->hastatus0_i = r0;
        d->and_or_tree_l0_i = static_cast<uint16_t>(r1 & 0x3ffu);
        d->hastatus1_min_i = (rng() & mask(48));
        d->texture_status_i = static_cast<uint16_t>(rng());
        d->icache_err_info_i = pack_icache_info(static_cast<uint8_t>(rng()),
                                                static_cast<uint8_t>(rng()),
                                                static_cast<uint8_t>(rng()),
                                                static_cast<uint8_t>(rng()), rng());
        d->icache_sbe_i = ((rng() >> 4) & 0xfu) == 0;
        d->icache_dbe_i = ((rng() >> 8) & 0x1fu) == 0;
        d->dmctrl_i = (rng() & 0x3ffu);
        d->test_en_i = ((rng() >> 1) & 0x1u);

        if ((rng() & 0x7ffu) == 0) {
            d->rst_c_ni = 0;
        } else if ((rng() & 0x3u) == 0) {
            d->rst_c_ni = 1;
        }
        if ((rng() & 0x7ffu) == 1) {
            d->rst_w_ni = 0;
        } else if ((rng() & 0x3u) == 1) {
            d->rst_w_ni = 1;
        }
        if ((rng() & 0x7ffu) == 2) {
            d->rst_d_ni = 0;
        } else if ((rng() & 0x3u) == 2) {
            d->rst_d_ni = 1;
        }
        if ((rng() & 0x3ffu) == 3) {
            d->dft_reset_byp_i = 1;
            d->dft_reset_i = 1;
        } else if ((rng() & 0x7u) == 0) {
            d->dft_reset_byp_i = 0;
            d->dft_reset_i = 0;
        }

        if (apb_phase == 0) {
            if ((rng() & 0x3u) != 0) {
                held_addr = static_cast<uint16_t>(kAddresses[rng() % kAddresses.size()]);
                held_wdata = rng();
                held_write = (rng() & 1u) != 0;
                d->apb_paddr_i = held_addr;
                d->apb_pwdata_i = held_wdata;
                d->apb_pwrite_i = held_write;
                d->apb_psel_i = 1;
                d->apb_penable_i = 0;
                apb_phase = 1;
            } else {
                d->apb_psel_i = 0;
                d->apb_penable_i = 0;
                d->apb_pwrite_i = (rng() & 1u);
                d->apb_paddr_i = static_cast<uint16_t>(rng());
                d->apb_pwdata_i = rng();
            }
        } else if (apb_phase == 1) {
            d->apb_paddr_i = held_addr;
            d->apb_pwdata_i = held_wdata;
            d->apb_pwrite_i = held_write;
            d->apb_psel_i = 1;
            d->apb_penable_i = 1;
            apb_phase = 2;
        } else {
            d->apb_psel_i = 0;
            d->apb_penable_i = 0;
            d->apb_pwrite_i = 0;
            apb_phase = 0;
        }

        tick(sim, cov);
    }

    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    d->dft_reset_byp_i = 0;
    d->dft_reset_i = 0;
    idle(sim, cov, 5);

    cov.check(sim);
    return sim.finish();
}
