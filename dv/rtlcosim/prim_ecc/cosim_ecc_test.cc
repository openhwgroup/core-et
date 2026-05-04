// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: ecc_gen/ecc_corr vs prim_ecc_enc/prim_ecc_dec
// Tests all three ECC widths (6, 7, 8) with encode and decode.

#include "Vcosim_ecc_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdlib>

using DUT = Vcosim_ecc_tb;

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();

    // ── Phase 1: ECC8 encoder (64-bit) ─────────────────────
    printf("cosim phase 1: ECC8 encoder\n");
    uint64_t test8[] = {0, 1, 0xDEADBEEFCAFEBABEULL, 0xFFFFFFFFFFFFFFFFULL,
                        0xA5A5A5A5A5A5A5A5ULL, 0x123456789ABCDEF0ULL};
    for (auto d : test8) {
        dut->data8_i = d;
        dut->data_and_ecc8_i[0] = 0; dut->data_and_ecc8_i[1] = 0; dut->data_and_ecc8_i[2] = 0;
        dut->eval();
        sim.compare("enc8[0]", dut->orig_enc8_o[0], dut->new_enc8_o[0]);
        sim.compare("enc8[1]", dut->orig_enc8_o[1], dut->new_enc8_o[1]);
        sim.compare("enc8[2]", dut->orig_enc8_o[2], dut->new_enc8_o[2]);
    }

    // ── Phase 2: ECC8 decoder with SBE ─────────────────────
    printf("cosim phase 2: ECC8 decoder (SBE)\n");
    for (auto d : test8) {
        dut->data8_i = d;
        dut->eval();
        // Get correct encoding
        uint32_t enc0 = dut->orig_enc8_o[0], enc1 = dut->orig_enc8_o[1], enc2 = dut->orig_enc8_o[2];
        // Test clean decode
        dut->data_and_ecc8_i[0] = enc0; dut->data_and_ecc8_i[1] = enc1; dut->data_and_ecc8_i[2] = enc2;
        dut->eval();
        sim.compare("dec8_data", dut->orig_dec8_data_o, dut->new_dec8_data_o);
        sim.compare("dec8_sbe",  (uint8_t)dut->orig_dec8_sbe_o,  (uint8_t)dut->new_dec8_sbe_o);
        sim.compare("dec8_dbe",  (uint8_t)dut->orig_dec8_dbe_o,  (uint8_t)dut->new_dec8_dbe_o);
        // Flip each data bit (SBE test)
        for (int bit = 0; bit < 64; bit++) {
            uint32_t f0 = enc0, f1 = enc1;
            if (bit < 32) f0 ^= (1u << bit); else f1 ^= (1u << (bit - 32));
            dut->data_and_ecc8_i[0] = f0; dut->data_and_ecc8_i[1] = f1; dut->data_and_ecc8_i[2] = enc2;
            dut->eval();
            sim.compare("dec8_sbe_data", dut->orig_dec8_data_o, dut->new_dec8_data_o);
            sim.compare("dec8_sbe_flag", (uint8_t)dut->orig_dec8_sbe_o, (uint8_t)dut->new_dec8_sbe_o);
            sim.compare("dec8_dbe_flag", (uint8_t)dut->orig_dec8_dbe_o, (uint8_t)dut->new_dec8_dbe_o);
        }
    }

    // ── Phase 3: ECC7 encoder (33-bit) ─────────────────────
    // data7_i is 33 bits → uint64_t; data_and_ecc7_i is 40 bits → uint64_t
    printf("cosim phase 3: ECC7 encoder\n");
    uint64_t test7[] = {0, 1, 0x1FFFFFFFFULL, 0x1A5A5A5A5ULL, 0x123456789ULL};
    for (auto d : test7) {
        dut->data7_i = d & 0x1FFFFFFFFULL;
        dut->data_and_ecc7_i = 0;
        dut->eval();
        sim.compare("enc7", dut->orig_enc7_o, dut->new_enc7_o);
    }

    // ── Phase 4: ECC7 decoder with SBE ─────────────────────
    printf("cosim phase 4: ECC7 decoder (SBE)\n");
    for (auto d : test7) {
        dut->data7_i = d & 0x1FFFFFFFFULL;
        dut->eval();
        uint64_t enc7 = dut->orig_enc7_o;
        dut->data_and_ecc7_i = enc7;
        dut->eval();
        sim.compare("dec7_data", dut->orig_dec7_data_o, dut->new_dec7_data_o);
        sim.compare("dec7_sbe", (uint8_t)dut->orig_dec7_sbe_o, (uint8_t)dut->new_dec7_sbe_o);
        sim.compare("dec7_dbe", (uint8_t)dut->orig_dec7_dbe_o, (uint8_t)dut->new_dec7_dbe_o);
        for (int bit = 0; bit < 33; bit++) {
            dut->data_and_ecc7_i = enc7 ^ (1ULL << bit);
            dut->eval();
            sim.compare("dec7_sbe_data", dut->orig_dec7_data_o, dut->new_dec7_data_o);
            sim.compare("dec7_sbe_flag", (uint8_t)dut->orig_dec7_sbe_o, (uint8_t)dut->new_dec7_sbe_o);
        }
    }

    // ── Phase 5: ECC6 encoder (23-bit) ─────────────────────
    printf("cosim phase 5: ECC6 encoder\n");
    uint32_t test6[] = {0, 1, 0x7FFFFF, 0x555555, 0x123456};
    for (auto d : test6) {
        dut->data6_i = d & 0x7FFFFF;
        dut->data_and_ecc6_i = 0;
        dut->eval();
        sim.compare("enc6", dut->orig_enc6_o, dut->new_enc6_o);
    }

    // ── Phase 6: ECC6 decoder with SBE ─────────────────────
    printf("cosim phase 6: ECC6 decoder (SBE)\n");
    for (auto d : test6) {
        dut->data6_i = d & 0x7FFFFF;
        dut->eval();
        uint32_t enc = dut->orig_enc6_o;
        dut->data_and_ecc6_i = enc;
        dut->eval();
        sim.compare("dec6_data", dut->orig_dec6_data_o, dut->new_dec6_data_o);
        sim.compare("dec6_sbe", (uint8_t)dut->orig_dec6_sbe_o, (uint8_t)dut->new_dec6_sbe_o);
        for (int bit = 0; bit < 23; bit++) {
            dut->data_and_ecc6_i = enc ^ (1u << bit);
            dut->eval();
            sim.compare("dec6_sbe_data", dut->orig_dec6_data_o, dut->new_dec6_data_o);
            sim.compare("dec6_sbe_flag", (uint8_t)dut->orig_dec6_sbe_o, (uint8_t)dut->new_dec6_sbe_o);
        }
    }

    return sim.finish();
}
