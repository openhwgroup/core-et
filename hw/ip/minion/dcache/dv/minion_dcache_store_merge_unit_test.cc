// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_store_merge_unit — Verilator test.

#include "Vminion_dcache_store_merge_unit_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vminion_dcache_store_merge_unit_tb;
using U256 = std::array<uint32_t, 8>;

namespace {

constexpr uint8_t kDcTypeB      = 0x0;
constexpr uint8_t kDcTypeH      = 0x1;
constexpr uint8_t kDcTypeW      = 0x2;
constexpr uint8_t kDcTypeD      = 0x3;
constexpr uint8_t kDcTypeBu     = 0x4;
constexpr uint8_t kDcTypeHu     = 0x5;
constexpr uint8_t kDcTypeWu     = 0x6;
constexpr uint8_t kDcTypePsBr   = 0xA;
constexpr uint8_t kDcTypePs     = 0xB;
constexpr uint8_t kDcTypePsGs8  = 0xC;
constexpr uint8_t kDcTypePsGs16 = 0xD;
constexpr uint8_t kDcTypePsGs32 = 0xE;

bool type_is_byte(uint8_t typ) {
    return (typ == kDcTypeB) || (typ == kDcTypeBu) || (typ == kDcTypePsGs8);
}

bool type_is_hword(uint8_t typ) {
    return (typ == kDcTypeH) || (typ == kDcTypeHu) || (typ == kDcTypePsGs16);
}

bool type_is_word(uint8_t typ) {
    return (typ == kDcTypeW) || (typ == kDcTypeWu) || (typ == kDcTypePsBr) || (typ == kDcTypePsGs32);
}

void set_u256(WData* dst, const U256& value) {
    for (int i = 0; i < 8; i++) {
        dst[i] = value[i];
    }
}

U256 get_u256(const WData* src) {
    U256 value {};
    for (int i = 0; i < 8; i++) {
        value[i] = src[i];
    }
    return value;
}

uint8_t get_byte(const U256& value, int index) {
    return uint8_t((value[index / 4] >> ((index % 4) * 8)) & 0xFFU);
}

void set_byte(U256& value, int index, uint8_t byte) {
    const int word = index / 4;
    const int shift = (index % 4) * 8;
    value[word] &= ~(0xFFU << shift);
    value[word] |= uint32_t(byte) << shift;
}

U256 store_merge_model(uint8_t addr, uint8_t typ, uint8_t ps_mask, const U256& orig_data,
                       const U256& store_data, bool enable_lo, bool enable_hi) {
    uint32_t wmask_orig = 0x000000FFU;
    if (typ == kDcTypePs) {
        wmask_orig = 0;
        for (int i = 0; i < 8; i++) {
            if ((ps_mask >> i) & 1U) {
                wmask_orig |= 0xFU << (i * 4);
            }
        }
    } else if (type_is_byte(typ)) {
        wmask_orig = 0x00000001U;
    } else if (type_is_hword(typ)) {
        wmask_orig = 0x00000003U;
    } else if (type_is_word(typ)) {
        wmask_orig = 0x0000000FU;
    }

    uint64_t mask_pair = (uint64_t(enable_hi ? wmask_orig : 0U) << 32)
                       | uint64_t(enable_lo ? wmask_orig : 0U);
    mask_pair <<= addr;
    const uint32_t wmask = uint32_t(mask_pair >> 32);

    U256 result = orig_data;
    for (int i = 0; i < 32; i++) {
        if ((wmask >> i) & 1U) {
            set_byte(result, i, get_byte(store_data, (i - addr) & 31));
        }
    }
    return result;
}

void check_case(SimCtrl<DUT>& sim, uint8_t addr, uint8_t typ, uint8_t ps_mask, const U256& orig_data,
                const U256& store_data, bool enable_lo, bool enable_hi, const std::string& label) {
    sim.dut->addr_i = addr;
    sim.dut->typ_i = typ;
    sim.dut->ps_mask_i = ps_mask;
    set_u256(sim.dut->orig_data_i, orig_data);
    set_u256(sim.dut->store_data_i, store_data);
    sim.dut->enable_lo_i = enable_lo;
    sim.dut->enable_hi_i = enable_hi;
    sim.tick();

    const U256 expect = store_merge_model(addr, typ, ps_mask, orig_data, store_data, enable_lo, enable_hi);
    const U256 got = get_u256(sim.dut->merge_out_o);
    for (int i = 0; i < 8; i++) {
        sim.check(got[i] == expect[i],
                  label + " word[" + std::to_string(i) + "]");
    }
}

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

U256 make_pattern(uint32_t seed) {
    U256 out {};
    for (int i = 0; i < 8; i++) {
        out[i] = seed + (0x11111111U * i);
    }
    return out;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->addr_i = 0;
    sim.dut->typ_i = 0;
    sim.dut->ps_mask_i = 0;
    sim.dut->enable_lo_i = 0;
    sim.dut->enable_hi_i = 0;
    for (int i = 0; i < 8; i++) {
        sim.dut->orig_data_i[i] = 0;
        sim.dut->store_data_i[i] = 0;
    }
    sim.reset();

    const U256 orig_a = make_pattern(0x10001000U);
    const U256 store_a = make_pattern(0xAA00AA00U);
    const U256 orig_b = make_pattern(0x20002000U);
    const U256 store_b = make_pattern(0x55005500U);

    printf("test 1: directed store merges\n");
    check_case(sim, 0,  kDcTypeB,    0x00, orig_a, store_a, 0, 1, "byte low");
    check_case(sim, 7,  kDcTypeB,    0x00, orig_a, store_a, 0, 1, "byte shifted");
    check_case(sim, 2,  kDcTypeH,    0x00, orig_a, store_a, 0, 1, "half shifted");
    check_case(sim, 4,  kDcTypeW,    0x00, orig_a, store_a, 0, 1, "word upper");
    check_case(sim, 24, kDcTypeD,    0x00, orig_a, store_a, 0, 1, "dword high bytes");
    check_case(sim, 0,  kDcTypePs,   0x81, orig_b, store_b, 0, 1, "ps sparse");
    check_case(sim, 5,  kDcTypePs,   0x3C, orig_b, store_b, 0, 1, "ps shifted");
    check_case(sim, 3,  kDcTypePsBr, 0x00, orig_b, store_b, 0, 1, "ps broadcast");
    check_case(sim, 1,  kDcTypePsGs8, 0x00, orig_b, store_b, 0, 1, "gs8");
    check_case(sim, 2,  kDcTypePsGs16, 0x00, orig_b, store_b, 0, 1, "gs16");
    check_case(sim, 4,  kDcTypePsGs32, 0x00, orig_b, store_b, 0, 1, "gs32");
    check_case(sim, 0,  kDcTypeW,    0x00, orig_b, store_b, 1, 0, "enable_lo only");

    printf("test 2: random stimulus\n");
    uint32_t seed = 0x13579BDFU;
    const uint8_t types[] = {
        kDcTypeB, kDcTypeH, kDcTypeW, kDcTypeD, kDcTypeBu, kDcTypeHu,
        kDcTypeWu, kDcTypePsBr, kDcTypePs, kDcTypePsGs8, kDcTypePsGs16, kDcTypePsGs32
    };
    for (int i = 0; i < 64; i++) {
        U256 orig {};
        U256 store {};
        for (int w = 0; w < 8; w++) {
            orig[w] = xorshift32(seed);
            store[w] = xorshift32(seed);
        }
        const uint8_t addr = uint8_t(xorshift32(seed) & 31U);
        const uint8_t typ = types[xorshift32(seed) % (sizeof(types) / sizeof(types[0]))];
        const uint8_t ps_mask = uint8_t(xorshift32(seed) & 0xFFU);
        const bool enable_lo = (xorshift32(seed) & 1U) != 0U;
        const bool enable_hi = (xorshift32(seed) & 1U) != 0U;
        check_case(sim, addr, typ, ps_mask, orig, store, enable_lo, enable_hi,
                  "random " + std::to_string(i));
    }

    return sim.finish();
}
