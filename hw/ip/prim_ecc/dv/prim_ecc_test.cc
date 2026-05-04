// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for prim_ecc_enc + prim_ecc_dec (ECC8, DataWidth=64)

#include "Vprim_ecc_dec.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <cstdlib>

using DUT = Vprim_ecc_dec;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();

    // The prim_ecc_dec top-level instantiates prim_ecc_enc internally.
    // Default parameter: DataWidth=64, EccWidth=8, DataAndEccWidth=72.
    // Input: data_and_ecc_i[71:0] = {ecc[7:0], data[63:0]}

    // ── Helper: encode data (use the DUT's internal encoder behavior) ──
    // We feed clean data through: set data_and_ecc_i with correct ECC,
    // then verify sbe=0, dbe=0, data_o=data.
    // To get the correct ECC, we use a trick: inject data with ecc=0,
    // read syndrome from the output, then fix the ECC.

    // Actually, since prim_ecc_dec embeds prim_ecc_enc, the decoder itself
    // computes expected_ecc. We test by:
    // 1. Encode: set data_and_ecc_i = {correct_ecc, data} → sbe=0, dbe=0
    // 2. SBE: flip one data bit → sbe=1, dbe=0, data_o = corrected
    // 3. DBE: flip two data bits → sbe=0, dbe=1

    // To get correct_ecc for a given data, we can observe: when we set
    // data_and_ecc_i = {8'h00, data}, the decoder computes expected_ecc
    // from data. The syndrome = 0 ^ expected_ecc = expected_ecc.
    // If syndrome has odd parity, sbe=1; even parity, dbe=1 (or 0 if no error).
    // We need syndrome=0 for no error, so ecc must equal expected_ecc.

    // Strategy: use a two-pass approach. First pass gets expected_ecc.

    auto encode = [&](uint64_t data) -> uint64_t {
        // Pass 1: feed {ecc=0, data}, read what the decoder thinks the syndrome is
        // The syndrome IS the expected_ecc (since received_ecc=0, syndrome=0^expected)
        // But we can't directly read syndrome. Instead, we note:
        // - With ecc=0 and non-zero data, sbe_o = ^expected_ecc (odd parity of expected_ecc)
        // - This doesn't help directly.
        //
        // Better: prim_ecc_dec contains prim_ecc_enc. The encoder's output
        // data_and_ecc_o = {expected_ecc, data}. We can't read internal signals.
        //
        // Simplest: brute-force. Feed {ecc_candidate, data}, sweep ecc until sbe=dbe=0.
        // For 8-bit ECC that's only 256 iterations.
        for (uint32_t e = 0; e < 256; e++) {
            uint64_t ecc = (uint64_t)e;
            // data_and_ecc_i is 72 bits: [71:64]=ecc, [63:0]=data
            dut->data_and_ecc_i[0] = (uint32_t)(data & 0xFFFFFFFF);
            dut->data_and_ecc_i[1] = (uint32_t)((data >> 32) & 0xFFFFFFFF);
            dut->data_and_ecc_i[2] = (uint32_t)(ecc & 0xFF);
            dut->eval();
            if (!dut->sbe_o && !dut->dbe_o) {
                return ecc;
            }
        }
        return 0xFF; // should not happen
    };

    auto set_data_and_ecc = [&](uint64_t data, uint8_t ecc) {
        dut->data_and_ecc_i[0] = (uint32_t)(data & 0xFFFFFFFF);
        dut->data_and_ecc_i[1] = (uint32_t)((data >> 32) & 0xFFFFFFFF);
        dut->data_and_ecc_i[2] = (uint32_t)ecc;
        dut->eval();
    };

    auto get_data_o = [&]() -> uint64_t {
        return dut->data_o;
    };

    // ── Test 1: Clean data, no errors ──────────────────────
    printf("test 1: clean encode/decode round-trip\n");
    uint64_t test_data[] = {0, 0xDEADBEEFCAFEBABEULL, 0xFFFFFFFFFFFFFFFFULL,
                            0x0000000000000001ULL, 0x8000000000000000ULL,
                            0x123456789ABCDEF0ULL, 0xA5A5A5A5A5A5A5A5ULL};
    for (auto td : test_data) {
        uint64_t ecc = encode(td);
        set_data_and_ecc(td, (uint8_t)ecc);
        sim.check(!dut->sbe_o && !dut->dbe_o, "no error on clean data");
        sim.check(get_data_o() == td, "data unchanged on clean decode");
    }

    // ── Test 2: Single-bit error correction ────────────────
    printf("test 2: single-bit error correction\n");
    uint64_t data = 0x123456789ABCDEF0ULL;
    uint64_t ecc_val = encode(data);
    for (int bit = 0; bit < 64; bit++) {
        uint64_t corrupted = data ^ (1ULL << bit);
        set_data_and_ecc(corrupted, (uint8_t)ecc_val);
        sim.check(dut->sbe_o == 1, "SBE detected for bit " + std::to_string(bit));
        sim.check(dut->dbe_o == 0, "no DBE for single-bit flip " + std::to_string(bit));
        sim.check(get_data_o() == data, "corrected data for bit " + std::to_string(bit));
    }

    // ── Test 3: Double-bit error detection ─────────────────
    printf("test 3: double-bit error detection\n");
    for (int i = 0; i < 32; i++) {
        int bit0 = i;
        int bit1 = (i + 17) % 64;  // spread the second bit
        if (bit0 == bit1) bit1 = (bit1 + 1) % 64;
        uint64_t corrupted = data ^ (1ULL << bit0) ^ (1ULL << bit1);
        set_data_and_ecc(corrupted, (uint8_t)ecc_val);
        sim.check(dut->dbe_o == 1, "DBE detected for bits " + std::to_string(bit0) + "+" + std::to_string(bit1));
        sim.check(dut->sbe_o == 0, "no SBE for double-bit");
    }

    // ── Test 4: ECC bit error (single) ─────────────────────
    printf("test 4: ECC bit error (correctable)\n");
    for (int bit = 0; bit < 8; bit++) {
        uint8_t corrupted_ecc = (uint8_t)(ecc_val ^ (1ULL << bit));
        set_data_and_ecc(data, corrupted_ecc);
        // ECC-only error: syndrome has odd parity → sbe, data unchanged
        sim.check(dut->sbe_o == 1, "SBE for ECC bit " + std::to_string(bit));
        sim.check(dut->dbe_o == 0, "no DBE for ECC bit " + std::to_string(bit));
        sim.check(get_data_o() == data, "data unchanged for ECC-only error");
    }

    return sim.finish();
}
