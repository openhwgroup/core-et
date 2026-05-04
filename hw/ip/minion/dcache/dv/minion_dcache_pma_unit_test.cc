// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_pma_unit — Verilator test.

#include "Vminion_dcache_pma_unit_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vminion_dcache_pma_unit_tb;

namespace {

constexpr uint8_t kPrvU = 0;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;

constexpr uint8_t kIoModeU = 0;
constexpr uint8_t kIoModeS = 1;
constexpr uint8_t kIoModeF = 2;
constexpr uint8_t kIoModeM = 3;

constexpr uint8_t kDramSize8G  = 0;
constexpr uint8_t kDramSize16G = 1;
constexpr uint8_t kDramSize24G = 2;
constexpr uint8_t kDramSize32G = 3;

constexpr uint8_t kDcTypeD = 3;

struct VmStatus {
    uint8_t prv;
    bool mprv;
    uint8_t mpp;
    bool sum;
    bool mxr;
    bool debug;
};

struct Mprot {
    bool enable_secure_memory;
    uint8_t dram_size;
    bool disable_osbox_access;
    bool disable_pcie_access;
    uint8_t io_access_mode;
};

struct PmaResp {
    bool access_fault;
    bool cacheable;
};

uint8_t pack_vm_status(const VmStatus& vm) {
    return uint8_t(((vm.prv & 0x3U) << 6)
                 | ((vm.mprv ? 1U : 0U) << 5)
                 | ((vm.mpp & 0x3U) << 3)
                 | ((vm.sum ? 1U : 0U) << 2)
                 | ((vm.mxr ? 1U : 0U) << 1)
                 |  (vm.debug ? 1U : 0U));
}

uint8_t pack_mprot(const Mprot& mp) {
    return uint8_t(((mp.enable_secure_memory ? 1U : 0U) << 6)
                 | ((mp.dram_size & 0x3U) << 4)
                 | ((mp.disable_osbox_access ? 1U : 0U) << 3)
                 | ((mp.disable_pcie_access ? 1U : 0U) << 2)
                 |  (mp.io_access_mode & 0x3U));
}

bool paddr_is_io_space(uint64_t paddr)         { return ((paddr >> 30) & 0x3FFULL) == 0x000ULL; }
bool paddr_is_sp_space(uint64_t paddr)         { return ((paddr >> 30) & 0x3FFULL) == 0x001ULL; }
bool paddr_is_sp_rom(uint64_t paddr)           { return paddr_is_sp_space(paddr) && (((paddr >> 17) & 0x1FFFULL) == 0x0000ULL); }
bool paddr_is_sp_sram(uint64_t paddr)          { return paddr_is_sp_space(paddr) && (((paddr >> 20) & 0x3FFULL) == 0x004ULL); }
bool paddr_is_sp_sram_code(uint64_t paddr)     { return paddr_is_sp_space(paddr) && (((paddr >> 19) & 0x7FFULL) == 0x008ULL); }
bool paddr_is_sp_sram_data(uint64_t paddr)     { return paddr_is_sp_space(paddr) && (((paddr >> 19) & 0x7FFULL) == 0x009ULL); }
bool paddr_is_sp_cacheable(uint64_t paddr)     { return paddr_is_sp_rom(paddr) || paddr_is_sp_sram(paddr); }
bool paddr_is_scratchpad(uint64_t paddr)       { return ((paddr >> 31) & 0x1FFULL) == 0x001ULL; }
bool paddr_is_esr_space(uint64_t paddr)        { return ((paddr >> 32) & 0xFFULL) == 0x01ULL; }
bool paddr_is_reserved(uint64_t paddr)         { return (((paddr >> 38) & 0x3ULL) == 0U) && (((paddr >> 33) & 0x1FULL) != 0U); }
bool paddr_is_pcie_space(uint64_t paddr)       { return ((paddr >> 38) & 0x3ULL) == 0x1ULL; }
bool paddr_is_dram(uint64_t paddr)             { return ((paddr >> 39) & 1ULL) != 0ULL; }
bool paddr_is_dram_cacheable(uint64_t paddr)   { return paddr_is_dram(paddr) && (((paddr >> 38) & 1ULL) == 0ULL); }
bool paddr_is_dram_uncacheable(uint64_t paddr) { return paddr_is_dram(paddr) && (((paddr >> 38) & 1ULL) != 0ULL); }
bool paddr_is_dram_mbox(uint64_t paddr)        { return paddr_is_dram(paddr) && (((paddr >> 23) & 0x7FFFULL) == 0ULL); }
bool paddr_is_dram_mcode(uint64_t paddr)       { return paddr_is_dram_mbox(paddr) && (((paddr >> 21) & 0x3ULL) == 0ULL); }
bool paddr_is_dram_mdata(uint64_t paddr)       { return paddr_is_dram_mbox(paddr) && (((paddr >> 21) & 0x3ULL) != 0ULL); }
bool paddr_is_dram_scode(uint64_t paddr)       { return paddr_is_dram(paddr) && (((paddr >> 26) & 0xFFFULL) == 0ULL) && (((paddr >> 23) & 0x7ULL) == 0x1ULL); }
bool paddr_is_dram_sdata(uint64_t paddr)       { return paddr_is_dram(paddr) && (((paddr >> 26) & 0xFFFULL) == 0ULL) && (((paddr >> 24) & 0x3ULL) != 0ULL); }
bool paddr_is_dram_osbox(uint64_t paddr)       { return paddr_is_dram(paddr) && (((paddr >> 32) & 0x3FULL) == 0ULL) && (((paddr >> 26) & 0x3FULL) != 0ULL); }
bool paddr_is_dram_over_8g(uint64_t paddr)     { return paddr_is_dram(paddr) && (((paddr >> 33) & 0x1FULL) != 0ULL); }
bool paddr_is_dram_over_16g(uint64_t paddr)    { return paddr_is_dram(paddr) && (((paddr >> 34) & 0xFULL) != 0ULL); }
bool paddr_is_dram_over_24g(uint64_t paddr)    { return paddr_is_dram(paddr) && ((((paddr >> 33) & 0x1FULL) == 0x03ULL) || (((paddr >> 35) & 0x7ULL) != 0ULL)); }
bool paddr_is_dram_over_32g(uint64_t paddr)    { return paddr_is_dram(paddr) && (((paddr >> 35) & 0x7ULL) != 0ULL); }

bool paddr_is_dram_reserved(uint64_t paddr, uint8_t dram_size) {
    return paddr_is_dram(paddr) && (((paddr >> 32) & 0x3FULL) != 0ULL) &&
           (((dram_size == kDramSize8G)  && paddr_is_dram_over_8g(paddr)) ||
            ((dram_size == kDramSize16G) && paddr_is_dram_over_16g(paddr)) ||
            ((dram_size == kDramSize24G) && paddr_is_dram_over_24g(paddr)) ||
            ((dram_size == kDramSize32G) && paddr_is_dram_over_32g(paddr)));
}

PmaResp pma_model(bool ioshire, bool af_skip, bool valid_addr, uint64_t paddr, uint8_t typ, bool amo,
                  bool write, bool mem_global, bool ts_tl_co, const Mprot& mprot, const VmStatus& vm) {
    const uint8_t privilege_mode = vm.mprv ? vm.mpp : vm.prv;

    const bool io_permitted = !amo && !ts_tl_co
                           && (mprot.io_access_mode <= privilege_mode)
                           && (mprot.io_access_mode != kIoModeF);

    const bool sp_sram_code_permitted = write ? (privilege_mode == kPrvM) : (privilege_mode != kPrvU);
    const bool sp_sram_data_permitted = privilege_mode != kPrvU;

    const bool sp_permitted = !amo
                           && (!ts_tl_co || paddr_is_sp_cacheable(paddr))
                           && (sp_sram_code_permitted || !paddr_is_sp_sram_code(paddr))
                           && (sp_sram_data_permitted || !paddr_is_sp_sram_data(paddr))
                           && ioshire;

    const bool scp_permitted = !amo || mem_global;

    const bool esr_permitted = !amo
                            && !ts_tl_co
                            && (typ == kDcTypeD)
                            && ((paddr & 0x7ULL) == 0ULL)
                            && (((paddr >> 30) & 0x3ULL) <= privilege_mode)
                            && (ioshire || (((paddr >> 30) & 0x3ULL) != 0x2U));

    const bool pcie_permitted = !amo && !ts_tl_co && (ioshire || !mprot.disable_pcie_access);
    const bool mcode_permitted = ioshire || ((privilege_mode == kPrvM) && !(write && mprot.enable_secure_memory));
    const bool mdata_permitted = ioshire || (privilege_mode == kPrvM);
    const bool scode_permitted = mprot.enable_secure_memory
                               ? (ioshire || (privilege_mode == kPrvM) || ((privilege_mode == kPrvS) && !write))
                               : (ioshire || !mprot.disable_osbox_access);
    const bool sdata_permitted = mprot.enable_secure_memory
                               ? (ioshire || (privilege_mode != kPrvU))
                               : (ioshire || !mprot.disable_osbox_access);
    const bool osbox_permitted = ioshire || !mprot.disable_osbox_access;

    PmaResp resp {};
    resp.cacheable = !amo && (paddr_is_dram_cacheable(paddr) || paddr_is_scratchpad(paddr) || paddr_is_sp_cacheable(paddr));
    resp.access_fault = valid_addr && !af_skip &&
                        ((paddr_is_io_space(paddr)         && !io_permitted) ||
                         (paddr_is_sp_space(paddr)         && !sp_permitted) ||
                         (paddr_is_scratchpad(paddr)       && !scp_permitted) ||
                         (paddr_is_esr_space(paddr)        && !esr_permitted) ||
                          paddr_is_reserved(paddr) ||
                         (paddr_is_pcie_space(paddr)       && !pcie_permitted) ||
                         (paddr_is_dram_mcode(paddr)       && !mcode_permitted) ||
                         (paddr_is_dram_mdata(paddr)       && !mdata_permitted) ||
                         (paddr_is_dram_scode(paddr)       && !scode_permitted) ||
                         (paddr_is_dram_sdata(paddr)       && !sdata_permitted) ||
                         (paddr_is_dram_osbox(paddr)       && !osbox_permitted) ||
                         (paddr_is_dram_uncacheable(paddr) && !ioshire) ||
                          paddr_is_dram_reserved(paddr, mprot.dram_size));
    return resp;
}

void check_case(SimCtrl<DUT>& sim, bool ioshire, bool af_skip, bool valid_addr, uint64_t paddr, uint8_t typ,
                bool amo, bool write, bool mem_global, bool ts_tl_co, const Mprot& mprot, const VmStatus& vm,
                const std::string& label) {
    sim.dut->ioshire_i = ioshire;
    sim.dut->af_skip_i = af_skip;
    sim.dut->valid_addr_i = valid_addr;
    sim.dut->paddr_i = paddr;
    sim.dut->typ_i = typ;
    sim.dut->amo_i = amo;
    sim.dut->write_i = write;
    sim.dut->mem_global_i = mem_global;
    sim.dut->ts_tl_co_i = ts_tl_co;
    sim.dut->mprot_i = pack_mprot(mprot);
    sim.dut->vm_status_i = pack_vm_status(vm);
    sim.tick();

    const PmaResp expect = pma_model(ioshire, af_skip, valid_addr, paddr, typ, amo, write, mem_global, ts_tl_co, mprot, vm);
    sim.check(sim.dut->access_fault_o == expect.access_fault, label + " access_fault");
    sim.check(sim.dut->cacheable_o == expect.cacheable, label + " cacheable");
}

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->ioshire_i = 0;
    sim.dut->af_skip_i = 0;
    sim.dut->valid_addr_i = 0;
    sim.dut->paddr_i = 0;
    sim.dut->typ_i = 0;
    sim.dut->amo_i = 0;
    sim.dut->write_i = 0;
    sim.dut->mem_global_i = 0;
    sim.dut->ts_tl_co_i = 0;
    sim.dut->mprot_i = 0;
    sim.dut->vm_status_i = 0;
    sim.reset();

    const VmStatus vm_u {kPrvU, false, kPrvU, false, false, false};
    const VmStatus vm_s {kPrvS, false, kPrvS, false, false, false};
    const VmStatus vm_m {kPrvM, false, kPrvM, false, false, false};
    const Mprot mprot_default {false, kDramSize16G, false, false, kIoModeM};
    const Mprot mprot_strict_io {false, kDramSize16G, false, false, kIoModeF};
    const Mprot mprot_secure {true, kDramSize16G, false, false, kIoModeM};
    const Mprot mprot_no_pcie {false, kDramSize16G, false, true, kIoModeM};
    const Mprot mprot_no_osbox {false, kDramSize16G, true, false, kIoModeM};

    printf("test 1: directed PMA cases\n");
    check_case(sim, false, false, true, 0x0000000010ULL, kDcTypeD, false, false, false, false,
               mprot_default, vm_m, "io allowed");
    check_case(sim, false, false, true, 0x0000000010ULL, kDcTypeD, true, false, false, false,
               mprot_default, vm_m, "io amo blocked");
    check_case(sim, false, false, true, 0x0000000010ULL, kDcTypeD, false, false, false, false,
               mprot_strict_io, vm_m, "io mode forbidden");
    check_case(sim, true, false, true, 0x0040400000ULL, kDcTypeD, false, false, false, false,
               mprot_default, vm_s, "sp sram code read");
    check_case(sim, true, false, true, 0x0040400000ULL, kDcTypeD, false, true, false, false,
               mprot_default, vm_s, "sp sram code write blocked");
    check_case(sim, true, false, true, 0x0040800000ULL, kDcTypeD, false, false, false, false,
               mprot_default, vm_u, "sp sram data user blocked");
    check_case(sim, false, false, true, 0x0080000000ULL, kDcTypeD, true, false, false, false,
               mprot_default, vm_m, "scratchpad local amo blocked");
    check_case(sim, false, false, true, 0x0080000000ULL, kDcTypeD, true, false, true, false,
               mprot_default, vm_m, "scratchpad global amo allowed");
    check_case(sim, true, false, true, 0x0100000000ULL, kDcTypeD, false, false, false, false,
               mprot_default, vm_m, "esr aligned dword");
    check_case(sim, true, false, true, 0x0100000004ULL, kDcTypeD, false, false, false, false,
               mprot_default, vm_m, "esr misaligned blocked");
    check_case(sim, false, false, true, 0x4000000000ULL, kDcTypeD, false, false, false, false,
               mprot_no_pcie, vm_m, "pcie disabled");
    check_case(sim, false, false, true, 0x8000001000ULL, kDcTypeD, false, false, false, false,
               mprot_default, vm_m, "dram mcode machine allowed");
    check_case(sim, false, false, true, 0x8000001000ULL, kDcTypeD, false, true, false, false,
               mprot_secure, vm_m, "dram mcode secure write blocked");
    check_case(sim, false, false, true, 0x8000800000ULL, kDcTypeD, false, false, false, false,
               mprot_no_osbox, vm_s, "dram osbox disabled");
    check_case(sim, false, false, true, 0xC000000000ULL, kDcTypeD, false, false, false, false,
               mprot_default, vm_m, "dram uncacheable blocked outside ioshire");
    check_case(sim, false, true, true, 0xC000000000ULL, kDcTypeD, false, false, false, false,
               mprot_default, vm_m, "af_skip suppresses fault");

    printf("test 2: random PMA stimulus\n");
    uint32_t seed = 0x2468ACE1U;
    for (int i = 0; i < 160; i++) {
        VmStatus vm {
            uint8_t(xorshift32(seed) & 0x3U),
            (xorshift32(seed) & 1U) != 0U,
            uint8_t(xorshift32(seed) & 0x3U),
            (xorshift32(seed) & 1U) != 0U,
            (xorshift32(seed) & 1U) != 0U,
            (xorshift32(seed) & 1U) != 0U
        };
        Mprot mp {
            (xorshift32(seed) & 1U) != 0U,
            uint8_t(xorshift32(seed) & 0x3U),
            (xorshift32(seed) & 1U) != 0U,
            (xorshift32(seed) & 1U) != 0U,
            uint8_t(xorshift32(seed) & 0x3U)
        };
        const uint64_t paddr = (uint64_t(xorshift32(seed) & 0xFFU) << 32) | xorshift32(seed);
        const uint8_t typ = uint8_t(xorshift32(seed) & 0xFU);
        check_case(sim,
                   (xorshift32(seed) & 1U) != 0U,
                   (xorshift32(seed) & 1U) != 0U,
                   (xorshift32(seed) & 1U) != 0U,
                   paddr,
                   typ,
                   (xorshift32(seed) & 1U) != 0U,
                   (xorshift32(seed) & 1U) != 0U,
                   (xorshift32(seed) & 1U) != 0U,
                   (xorshift32(seed) & 1U) != 0U,
                   mp,
                   vm,
                   "random " + std::to_string(i));
    }

    return sim.finish();
}
