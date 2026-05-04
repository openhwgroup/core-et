// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: intpipe_alu (new) vs intpipe_alu (original).
//
// Tests all ALU operations in both DW64 and DW32 modes with targeted
// values, then 2M random combinations.

#include "Vcosim_intpipe_alu_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>

using DUT = Vcosim_intpipe_alu_tb;

// Simple xorshift32 PRNG for reproducible results.
static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static uint64_t xorshift64() {
    uint64_t hi = xorshift32();
    uint64_t lo = xorshift32();
    return (hi << 32) | lo;
}

// ALU function codes (match both alu_func_e and core_alu_func).
enum {
    FN_ADD     = 0b00000,
    FN_SL      = 0b00001,
    FN_SEQ     = 0b00010,
    FN_SNE     = 0b00011,
    FN_XOR     = 0b00100,
    FN_SR      = 0b00101,
    FN_OR      = 0b00110,
    FN_AND     = 0b00111,
    FN_PACKB   = 0b01000,
    FN_BITMIXB = 0b01001,
    FN_SUB     = 0b01010,
    FN_SRA     = 0b01011,
    FN_SLT     = 0b01100,
    FN_SGE     = 0b01101,
    FN_SLTU    = 0b01110,
    FN_SGEU    = 0b01111,
};

static const uint8_t all_fns[] = {
    FN_ADD, FN_SL, FN_SEQ, FN_SNE, FN_XOR, FN_SR, FN_OR, FN_AND,
    FN_PACKB, FN_BITMIXB, FN_SUB, FN_SRA, FN_SLT, FN_SGE, FN_SLTU, FN_SGEU,
};

static const char* fn_name(uint8_t fn) {
    switch (fn) {
        case FN_ADD:     return "ADD";
        case FN_SL:      return "SL";
        case FN_SEQ:     return "SEQ";
        case FN_SNE:     return "SNE";
        case FN_XOR:     return "XOR";
        case FN_SR:      return "SR";
        case FN_OR:      return "OR";
        case FN_AND:     return "AND";
        case FN_PACKB:   return "PACKB";
        case FN_BITMIXB: return "BITMIXB";
        case FN_SUB:     return "SUB";
        case FN_SRA:     return "SRA";
        case FN_SLT:     return "SLT";
        case FN_SGE:     return "SGE";
        case FN_SLTU:    return "SLTU";
        case FN_SGEU:    return "SGEU";
        default:         return "???";
    }
}

static void drive_and_compare(CosimCtrl<DUT>& sim, uint8_t dw, uint8_t fn,
                               uint64_t in1, uint64_t in2, const char* desc) {
    sim.dut->stim_dw  = dw;
    sim.dut->stim_fn  = fn;
    sim.dut->stim_in1 = in1;
    sim.dut->stim_in2 = in2;
    sim.dut->eval();

    uint64_t orig_out       = sim.dut->orig_out_o;
    uint64_t new_out        = sim.dut->new_out_o;
    uint64_t orig_adder_out = sim.dut->orig_adder_out_o;
    uint64_t new_adder_out  = sim.dut->new_adder_out_o;

    if (orig_out != new_out) {
        printf("  MISMATCH out %s dw=%d fn=%s: in1=0x%016llx in2=0x%016llx "
               "orig=0x%016llx new=0x%016llx\n",
               desc, dw, fn_name(fn),
               (unsigned long long)in1, (unsigned long long)in2,
               (unsigned long long)orig_out, (unsigned long long)new_out);
    }
    sim.compare(desc, orig_out, new_out);

    if (orig_adder_out != new_adder_out) {
        printf("  MISMATCH adder_out %s dw=%d fn=%s: in1=0x%016llx in2=0x%016llx "
               "orig=0x%016llx new=0x%016llx\n",
               desc, dw, fn_name(fn),
               (unsigned long long)in1, (unsigned long long)in2,
               (unsigned long long)orig_adder_out, (unsigned long long)new_adder_out);
    }
    sim.compare(desc, orig_adder_out, new_adder_out);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;

    sim.dut->rst_ni = 1;

    // Phase 1: Targeted tests for each function in both DW64 and DW32 modes.
    printf("phase 1: targeted tests for all ALU functions\n");

    // Representative operand pairs.
    struct { uint64_t a; uint64_t b; const char* name; } operands[] = {
        {0x0000000000000000ULL, 0x0000000000000000ULL, "zero/zero"},
        {0x0000000000000001ULL, 0x0000000000000001ULL, "one/one"},
        {0x0000000000000001ULL, 0xFFFFFFFFFFFFFFFFULL, "one/allones"},
        {0xFFFFFFFFFFFFFFFFULL, 0x0000000000000001ULL, "allones/one"},
        {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, "allones/allones"},
        {0x7FFFFFFFFFFFFFFFULL, 0x0000000000000001ULL, "maxpos/one"},
        {0x8000000000000000ULL, 0x0000000000000001ULL, "mineg/one"},
        {0x8000000000000000ULL, 0x8000000000000000ULL, "mineg/mineg"},
        {0x00000000DEADBEEFULL, 0x00000000CAFEBABCULL, "lo32/lo32"},
        {0xDEADBEEF00000000ULL, 0xCAFEBABC00000000ULL, "hi32/hi32"},
        {0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL, "mixed1/mixed2"},
        {0x0000000000000020ULL, 0x0000000000000020ULL, "shift32/shift32"},
        {0x000000000000003FULL, 0x000000000000003FULL, "shift63/shift63"},
        {0x00000000FFFFFFFFULL, 0x0000000000000000ULL, "lo32max/zero"},
        {0xFFFFFFFF00000000ULL, 0x0000000000000000ULL, "hi32max/zero"},
        {0x00000000FFFFFFFFULL, 0x00000000FFFFFFFFULL, "lo32max/lo32max"},
        {0x0000000080000000ULL, 0x0000000000000001ULL, "w-signbit/one"},
    };

    char desc[128];
    for (uint8_t dw = 0; dw <= 1; dw++) {
        for (auto& fn : all_fns) {
            for (auto& op : operands) {
                snprintf(desc, sizeof(desc), "dw%d_%s_%s", dw ? 64 : 32,
                         fn_name(fn), op.name);
                drive_and_compare(sim, dw, fn, op.a, op.b, desc);
            }
        }
    }

    // Phase 2: Random sweep (2M).
    printf("phase 2: random sweep (2M combinations)\n");
    int phase2_mismatches = 0;
    for (int i = 0; i < 2000000; i++) {
        uint8_t  dw  = xorshift32() & 1;
        uint8_t  fn  = all_fns[xorshift32() % (sizeof(all_fns)/sizeof(all_fns[0]))];
        uint64_t in1 = xorshift64();
        uint64_t in2 = xorshift64();

        sim.dut->stim_dw  = dw;
        sim.dut->stim_fn  = fn;
        sim.dut->stim_in1 = in1;
        sim.dut->stim_in2 = in2;
        sim.dut->eval();

        uint64_t orig_out       = sim.dut->orig_out_o;
        uint64_t new_out        = sim.dut->new_out_o;
        uint64_t orig_adder_out = sim.dut->orig_adder_out_o;
        uint64_t new_adder_out  = sim.dut->new_adder_out_o;

        bool mismatch = false;
        if (orig_out != new_out) {
            if (phase2_mismatches < 20) {
                printf("  MISMATCH out: dw=%d fn=%s in1=0x%016llx in2=0x%016llx "
                       "orig=0x%016llx new=0x%016llx\n",
                       dw, fn_name(fn),
                       (unsigned long long)in1, (unsigned long long)in2,
                       (unsigned long long)orig_out, (unsigned long long)new_out);
            }
            mismatch = true;
        }
        sim.compare("random_out", orig_out, new_out);

        if (orig_adder_out != new_adder_out) {
            if (phase2_mismatches < 20) {
                printf("  MISMATCH adder_out: dw=%d fn=%s in1=0x%016llx in2=0x%016llx "
                       "orig=0x%016llx new=0x%016llx\n",
                       dw, fn_name(fn),
                       (unsigned long long)in1, (unsigned long long)in2,
                       (unsigned long long)orig_adder_out, (unsigned long long)new_adder_out);
            }
            mismatch = true;
        }
        sim.compare("random_adder", orig_adder_out, new_adder_out);

        if (mismatch) phase2_mismatches++;
    }

    if (phase2_mismatches > 20) {
        printf("  ... and %d more mismatches (truncated)\n", phase2_mismatches - 20);
    }

    return sim.finish();
}
