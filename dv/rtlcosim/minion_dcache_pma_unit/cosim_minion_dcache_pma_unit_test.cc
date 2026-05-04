// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: dcache_pma_unit (original) vs minion_dcache_pma_unit
// (new).

#include "Vcosim_minion_dcache_pma_unit_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_minion_dcache_pma_unit_tb;

namespace {

uint8_t pack_vm_status(uint8_t prv, bool mprv, uint8_t mpp, bool sum, bool mxr, bool debug) {
    return uint8_t(((prv & 0x3U) << 6)
                 | ((mprv ? 1U : 0U) << 5)
                 | ((mpp & 0x3U) << 3)
                 | ((sum ? 1U : 0U) << 2)
                 | ((mxr ? 1U : 0U) << 1)
                 |  (debug ? 1U : 0U));
}

uint8_t pack_mprot(bool secure, uint8_t dram_size, bool no_osbox, bool no_pcie, uint8_t io_mode) {
    return uint8_t(((secure ? 1U : 0U) << 6)
                 | ((dram_size & 0x3U) << 4)
                 | ((no_osbox ? 1U : 0U) << 3)
                 | ((no_pcie ? 1U : 0U) << 2)
                 |  (io_mode & 0x3U));
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("access_fault", sim.dut->orig_access_fault_o, sim.dut->new_access_fault_o);
    sim.compare("cacheable", sim.dut->orig_cacheable_o, sim.dut->new_cacheable_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

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
    sim.reset(4);
    compare_outputs(sim);

    const uint64_t directed_addrs[] = {
        0x0000000010ULL, 0x0040400000ULL, 0x0040800000ULL, 0x0080000000ULL,
        0x0100000000ULL, 0x0100000004ULL, 0x4000000000ULL, 0x8000001000ULL,
        0x8000800000ULL, 0xC000000000ULL, 0x80FF000000ULL, 0x0000001000ULL,
        0x8001000000ULL, 0x8080000000ULL, 0x4040000000ULL, 0x8100000000ULL
    };
    for (int i = 0; i < 16; i++) {
        sim.dut->ioshire_i = (i >> 0) & 1U;
        sim.dut->af_skip_i = (i >> 1) & 1U;
        sim.dut->valid_addr_i = 1;
        sim.dut->paddr_i = directed_addrs[i] & ((1ULL << 40) - 1ULL);
        sim.dut->typ_i = i & 0xFU;
        sim.dut->amo_i = (i >> 2) & 1U;
        sim.dut->write_i = (i >> 3) & 1U;
        sim.dut->mem_global_i = (i >> 1) & 1U;
        sim.dut->ts_tl_co_i = (i >> 2) & 1U;
        sim.dut->mprot_i = pack_mprot((i >> 2) & 1U, i & 3U, (i >> 1) & 1U, (i >> 3) & 1U, i & 3U);
        sim.dut->vm_status_i = pack_vm_status(i & 3U, (i >> 2) & 1U, (i >> 1) & 3U,
                                              (i >> 3) & 1U, (i >> 1) & 1U, (i >> 2) & 1U);
        tick_and_compare(sim);
    }

    uint32_t seed = 0x31415926U;
    for (int i = 0; i < 2048; i++) {
        const uint64_t paddr = (((uint64_t)xorshift32(seed)) << 8) | (uint64_t(xorshift32(seed)) & 0xFFU);
        sim.dut->ioshire_i = xorshift32(seed) & 1U;
        sim.dut->af_skip_i = xorshift32(seed) & 1U;
        sim.dut->valid_addr_i = xorshift32(seed) & 1U;
        sim.dut->paddr_i = paddr & ((1ULL << 40) - 1ULL);
        sim.dut->typ_i = xorshift32(seed) & 0xFU;
        sim.dut->amo_i = xorshift32(seed) & 1U;
        sim.dut->write_i = xorshift32(seed) & 1U;
        sim.dut->mem_global_i = xorshift32(seed) & 1U;
        sim.dut->ts_tl_co_i = xorshift32(seed) & 1U;
        sim.dut->mprot_i = pack_mprot(xorshift32(seed) & 1U, xorshift32(seed) & 3U,
                                      xorshift32(seed) & 1U, xorshift32(seed) & 1U,
                                      xorshift32(seed) & 3U);
        sim.dut->vm_status_i = pack_vm_status(xorshift32(seed) & 3U, xorshift32(seed) & 1U,
                                              xorshift32(seed) & 3U, xorshift32(seed) & 1U,
                                              xorshift32(seed) & 1U, xorshift32(seed) & 1U);
        tick_and_compare(sim);
    }

    return sim.finish();
}
