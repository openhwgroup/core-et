// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_atomic_alu_unit — Verilator test.

#include "Vminion_dcache_atomic_alu_unit_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vminion_dcache_atomic_alu_unit_tb;

namespace {

constexpr uint8_t kDcCmdXaAdd  = 0x08;
constexpr uint8_t kDcCmdXaXor  = 0x09;
constexpr uint8_t kDcCmdXaOr   = 0x0A;
constexpr uint8_t kDcCmdXaAnd  = 0x0B;
constexpr uint8_t kDcCmdXaMin  = 0x0C;
constexpr uint8_t kDcCmdXaMax  = 0x0D;
constexpr uint8_t kDcCmdXaMinu = 0x0E;
constexpr uint8_t kDcCmdXaMaxu = 0x0F;

uint64_t atomic_model(uint8_t addr, uint8_t cmd, uint8_t typ, uint64_t lhs, uint64_t rhs) {
    const bool sgned   = (cmd == kDcCmdXaMin) || (cmd == kDcCmdXaMax);
    const bool max_sel = (cmd == kDcCmdXaMax) || (cmd == kDcCmdXaMaxu);
    const bool min_sel = (cmd == kDcCmdXaMin) || (cmd == kDcCmdXaMinu);

    uint64_t rhs_int = rhs;
    if (typ == 0) {
        rhs_int = 0x0101010101010101ULL * (rhs & 0xFFULL);
    } else if (typ == 1) {
        rhs_int = 0x0001000100010001ULL * (rhs & 0xFFFFULL);
    } else if (typ == 2) {
        rhs_int = ((rhs & 0xFFFFFFFFULL) << 32) | (rhs & 0xFFFFFFFFULL);
    }

    const uint64_t adder_mask = ~0ULL ^ (uint64_t((addr >> 2) & 1U) << 31);
    const uint64_t adder_out  = (lhs & adder_mask) + (rhs_int & adder_mask);

    const bool word    = !(typ & 1U);
    const bool cmp_lhs = (word && ((addr & 4U) == 0U)) ? ((lhs >> 31) & 1U) : ((lhs >> 63) & 1U);
    const bool cmp_rhs = (word && ((addr & 4U) == 0U)) ? ((rhs_int >> 31) & 1U) : ((rhs_int >> 63) & 1U);
    const bool lt_lo   = uint32_t(lhs & 0xFFFFFFFFULL) < uint32_t(rhs_int & 0xFFFFFFFFULL);
    const bool lt_hi   = uint32_t(lhs >> 32) < uint32_t(rhs_int >> 32);
    const bool eq_hi   = uint32_t(lhs >> 32) == uint32_t(rhs_int >> 32);
    const bool lt      = word ? (((addr & 4U) != 0U) ? lt_hi : lt_lo) : (lt_hi || (eq_hi && lt_lo));
    const bool less    = (cmp_lhs == cmp_rhs) ? lt : (sgned ? cmp_lhs : cmp_rhs);

    uint64_t out_mux = rhs_int;
    if (cmd == kDcCmdXaAdd) {
        out_mux = adder_out;
    } else if (cmd == kDcCmdXaAnd) {
        out_mux = lhs & rhs_int;
    } else if (cmd == kDcCmdXaOr) {
        out_mux = lhs | rhs_int;
    } else if (cmd == kDcCmdXaXor) {
        out_mux = lhs ^ rhs_int;
    } else {
        out_mux = ((less ? min_sel : max_sel) ? lhs : rhs_int);
    }

    uint8_t wmask_byte = 0xFF;
    if (typ == 0) {
        wmask_byte = 0x01;
    } else if (typ == 1) {
        wmask_byte = 0x03;
    } else if (typ == 2) {
        wmask_byte = 0x0F;
    }
    wmask_byte = uint8_t(wmask_byte << (addr & 7U));

    uint64_t out = lhs;
    for (int i = 0; i < 8; i++) {
      const uint64_t mask = uint64_t((wmask_byte >> i) & 1U ? 0xFFU : 0U) << (i * 8);
      out = (out & ~mask) | (out_mux & mask);
    }
    return out;
}

void check_case(SimCtrl<DUT>& sim, uint8_t addr, uint8_t cmd, uint8_t typ,
                uint64_t lhs, uint64_t rhs, const std::string& label) {
    sim.dut->addr_i = addr;
    sim.dut->cmd_i  = cmd;
    sim.dut->typ_i  = typ;
    sim.dut->lhs_i  = lhs;
    sim.dut->rhs_i  = rhs;
    sim.tick();

    const uint64_t expect = atomic_model(addr, cmd, typ, lhs, rhs);
    sim.check(sim.dut->out_o == expect,
              label + " expected=0x" + std::to_string(expect));
}

uint64_t xorshift64(uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->addr_i = 0;
    sim.dut->cmd_i  = 0;
    sim.dut->typ_i  = 0;
    sim.dut->lhs_i  = 0;
    sim.dut->rhs_i  = 0;
    sim.reset();

    printf("test 1: directed AMO cases\n");
    check_case(sim, 0, kDcCmdXaAdd,  0, 0x1122334455667788ULL, 0x0000000000000011ULL, "byte add");
    check_case(sim, 2, kDcCmdXaAnd,  1, 0xFFFF0000AAAA5555ULL, 0x000000000000F0F0ULL, "half and");
    check_case(sim, 4, kDcCmdXaOr,   2, 0x1111222233334444ULL, 0x00000000AAAABBBBULL, "word or hi");
    check_case(sim, 0, kDcCmdXaXor,  3, 0x0123456789ABCDEFULL, 0x1111111111111111ULL, "dword xor");
    check_case(sim, 0, kDcCmdXaMin,  3, 0xFFFFFFFFFFFFFFF0ULL, 0x0000000000000010ULL, "signed min");
    check_case(sim, 0, kDcCmdXaMax,  3, 0xFFFFFFFFFFFFFFF0ULL, 0x0000000000000010ULL, "signed max");
    check_case(sim, 0, kDcCmdXaMinu, 3, 0xFFFFFFFFFFFFFFF0ULL, 0x0000000000000010ULL, "unsigned min");
    check_case(sim, 0, kDcCmdXaMaxu, 3, 0xFFFFFFFFFFFFFFF0ULL, 0x0000000000000010ULL, "unsigned max");
    check_case(sim, 7, kDcCmdXaAdd,  0, 0x0011223344556677ULL, 0x00000000000000AAULL, "byte add msb");
    check_case(sim, 6, kDcCmdXaMin,  1, 0x0011223344556677ULL, 0x0000000000008001ULL, "half min hi");
    check_case(sim, 4, kDcCmdXaMaxu, 2, 0x0011223344556677ULL, 0x00000000FFFFFFFFULL, "word maxu hi");
    check_case(sim, 0, kDcCmdXaAnd,  3, 0xFEDCBA9876543210ULL, 0x0F0F0F0F0F0F0F0FULL, "dword and");

    printf("test 2: random stimulus\n");
    uint64_t seed = 0x0123456789ABCDEFULL;
    const uint8_t cmds[] = {
        kDcCmdXaAdd, kDcCmdXaXor, kDcCmdXaOr, kDcCmdXaAnd,
        kDcCmdXaMin, kDcCmdXaMax, kDcCmdXaMinu, kDcCmdXaMaxu
    };
    for (int i = 0; i < 96; i++) {
        const uint8_t addr = uint8_t(xorshift64(seed) & 7U);
        const uint8_t cmd  = cmds[xorshift64(seed) % (sizeof(cmds) / sizeof(cmds[0]))];
        const uint8_t typ  = uint8_t(xorshift64(seed) & 3U);
        const uint64_t lhs = xorshift64(seed);
        const uint64_t rhs = xorshift64(seed);
        check_case(sim, addr, cmd, typ, lhs, rhs, "random " + std::to_string(i));
    }

    return sim.finish();
}
