// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: intpipe_mul_div_top (new) vs original.
//
// Multi-cycle multiplier/divider. Tests MUL, MULH, MULHU, MULHSU,
// DIV, DIVU, REM, REMU in DW32/DW64 with targeted and random vectors,
// plus kill and back-to-back coverage.

#include "Vcosim_mul_div_top_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_mul_div_top_tb;

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

// ALU function codes (match both alu_func_e and core_alu_func encodings).
enum {
    FN_MUL    = 0b10000,
    FN_MULH   = 0b10001,
    FN_DIV    = 0b10100,
    FN_DIVU   = 0b10101,
    FN_REM    = 0b10110,
    FN_REMU   = 0b10111,
    FN_MULHSU = 0b11100,
    FN_MULHU  = 0b11110,
};

static const uint8_t mul_fns[] = { FN_MUL, FN_MULH, FN_MULHU, FN_MULHSU };
static const uint8_t div_fns[] = { FN_DIV, FN_DIVU, FN_REM, FN_REMU };
static const uint8_t all_fns[] = {
    FN_MUL, FN_MULH, FN_MULHU, FN_MULHSU,
    FN_DIV, FN_DIVU, FN_REM, FN_REMU,
};

static const char* fn_name(uint8_t fn) {
    switch (fn) {
        case FN_MUL:    return "MUL";
        case FN_MULH:   return "MULH";
        case FN_MULHU:  return "MULHU";
        case FN_MULHSU: return "MULHSU";
        case FN_DIV:    return "DIV";
        case FN_DIVU:   return "DIVU";
        case FN_REM:    return "REM";
        case FN_REMU:   return "REMU";
        default:        return "???";
    }
}

// Compare all outputs between new and original.
static void compare_outputs(CosimCtrl<DUT>& sim, const char* phase) {
    sim.compare("req_ready",        (uint8_t)sim.dut->orig_req_ready,
                                     (uint8_t)sim.dut->new_req_ready);
    sim.compare("resp_valid",       (uint8_t)sim.dut->orig_resp_valid,
                                     (uint8_t)sim.dut->new_resp_valid);
    sim.compare("resp_valid_early", (uint8_t)sim.dut->orig_resp_valid_early,
                                     (uint8_t)sim.dut->new_resp_valid_early);
    sim.compare("resp_data",        (uint64_t)sim.dut->orig_resp_data,
                                     (uint64_t)sim.dut->new_resp_data);
    sim.compare("resp_dest",        (uint8_t)sim.dut->orig_resp_dest,
                                     (uint8_t)sim.dut->new_resp_dest);
}

// Drive idle inputs (no request).
static void drive_idle(DUT* dut) {
    dut->stim_req_valid       = 0;
    dut->stim_req_valid_early = 0;
    dut->stim_req_fn          = FN_MUL;
    dut->stim_req_fn_early    = FN_MUL;
    dut->stim_req_dw          = 0;
    dut->stim_req_in1         = 0;
    dut->stim_req_in2         = 0;
    dut->stim_req_dest        = 0;
    dut->stim_kill            = 0;
    dut->stim_chicken_bit     = 0;
    dut->stim_resp_ready      = 1;
}

// Issue one operation and wait for resp_valid. Compare every cycle.
// Returns number of cycles taken.
static int issue_and_wait(CosimCtrl<DUT>& sim, uint8_t fn, uint8_t dw,
                          uint64_t in1, uint64_t in2, uint8_t dest,
                          const char* desc, int max_cycles = 200) {
    // The protocol: req_valid_early + req_fn_early must be asserted one cycle
    // before req_valid + req_fn. So we do:
    //   Cycle N:   req_valid_early=1, req_fn_early=fn, req_valid=0
    //   Cycle N+1: req_valid=1, req_fn=fn, req_valid_early=0
    //   Then wait for resp_valid.

    // Cycle 0: early phase
    sim.dut->stim_req_valid_early = 1;
    sim.dut->stim_req_fn_early    = fn;
    sim.dut->stim_req_valid       = 0;
    sim.dut->stim_req_fn          = fn;
    sim.dut->stim_req_dw          = dw;
    sim.dut->stim_req_in1         = in1;
    sim.dut->stim_req_in2         = in2;
    sim.dut->stim_req_dest        = dest;
    sim.dut->stim_kill            = 0;
    sim.dut->stim_chicken_bit     = 0;
    sim.dut->stim_resp_ready      = 1;
    sim.tick();
    compare_outputs(sim, desc);

    // Cycle 1: actual request
    sim.dut->stim_req_valid_early = 0;
    sim.dut->stim_req_valid       = 1;
    sim.tick();
    compare_outputs(sim, desc);

    // Deassert request, wait for completion
    sim.dut->stim_req_valid       = 0;
    sim.dut->stim_req_valid_early = 0;

    int cycles = 2;
    while (cycles < max_cycles) {
        sim.tick();
        compare_outputs(sim, desc);
        cycles++;
        if (sim.dut->orig_resp_valid && sim.dut->new_resp_valid)
            break;
    }

    if (cycles >= max_cycles) {
        printf("  TIMEOUT waiting for resp_valid: %s\n", desc);
    }

    // One more idle cycle to let resp_valid clear
    drive_idle(sim.dut.get());
    sim.tick();
    compare_outputs(sim, desc);

    return cycles;
}

// Issue an operation for back-to-back testing: assert early, then on the
// NEXT call we'll assert valid. This function prepares the early phase.
// Returns after the early cycle.
static void issue_early(CosimCtrl<DUT>& sim, uint8_t fn, uint8_t dw,
                        uint64_t in1, uint64_t in2, uint8_t dest) {
    sim.dut->stim_req_valid_early = 1;
    sim.dut->stim_req_fn_early    = fn;
    sim.dut->stim_req_valid       = 0;
    sim.dut->stim_req_fn          = fn;
    sim.dut->stim_req_dw          = dw;
    sim.dut->stim_req_in1         = in1;
    sim.dut->stim_req_in2         = in2;
    sim.dut->stim_req_dest        = dest;
    sim.dut->stim_kill            = 0;
    sim.dut->stim_resp_ready      = 1;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 50000000;  // 50M time units

    // Reset
    drive_idle(sim.dut.get());
    sim.reset(10);

    // Let things settle after reset
    for (int i = 0; i < 5; i++) {
        drive_idle(sim.dut.get());
        sim.tick();
        compare_outputs(sim, "post_reset");
    }

    char desc[256];

    // ================================================================
    // Phase 1: MUL operations with known values
    // ================================================================
    printf("phase 1: MUL operations (MUL, MULH, MULHU, MULHSU)\n");

    struct { uint64_t a; uint64_t b; const char* name; } mul_operands[] = {
        {0x0000000000000002ULL, 0x0000000000000003ULL, "2x3"},
        {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, "neg1xneg1"},
        {0x7FFFFFFFFFFFFFFFULL, 0x0000000000000002ULL, "maxposxn2"},
        {0x8000000000000000ULL, 0x0000000000000002ULL, "minnegx2"},
        {0x0000000000000000ULL, 0x123456789ABCDEF0ULL, "0xany"},
        {0x00000000DEADBEEFULL, 0x00000000CAFEBABCULL, "lo32xlo32"},
        {0x0000000080000000ULL, 0x0000000080000000ULL, "w32signxw32sign"},
        {0xFFFFFFFF80000000ULL, 0x0000000000000002ULL, "neg_w32x2"},
        {0x0000000000000001ULL, 0xFFFFFFFFFFFFFFFFULL, "1xneg1"},
        {0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL, "bigxbig"},
    };

    for (uint8_t dw = 0; dw <= 1; dw++) {
        for (auto fn : mul_fns) {
            for (auto& op : mul_operands) {
                snprintf(desc, sizeof(desc), "p1_dw%d_%s_%s",
                         dw ? 64 : 32, fn_name(fn), op.name);
                issue_and_wait(sim, fn, dw, op.a, op.b, 0x15, desc);
            }
        }
    }

    // ================================================================
    // Phase 2: DIV operations with known values, including divide-by-zero
    // ================================================================
    printf("phase 2: DIV operations (DIV, DIVU, REM, REMU)\n");

    struct { uint64_t a; uint64_t b; const char* name; } div_operands[] = {
        {0x0000000000000064ULL, 0x000000000000000AULL, "100div10"},
        {0x0000000000000064ULL, 0x0000000000000007ULL, "100div7"},
        {0xFFFFFFFFFFFFFFFFULL, 0x0000000000000001ULL, "neg1div1"},
        {0x8000000000000000ULL, 0xFFFFFFFFFFFFFFFFULL, "minnegdivneg1"},
        {0x0000000000000000ULL, 0x0000000000000005ULL, "0div5"},
        {0x0000000000000005ULL, 0x0000000000000000ULL, "5div0"},
        {0xFFFFFFFFFFFFFFFFULL, 0x0000000000000000ULL, "neg1div0"},
        {0x8000000000000000ULL, 0x0000000000000000ULL, "minnegdiv0"},
        {0x00000000DEADBEEFULL, 0x0000000000001000ULL, "lo32div4k"},
        {0x123456789ABCDEF0ULL, 0x0000000100000000ULL, "bigdiv4g"},
        {0xFFFFFFFF80000000ULL, 0xFFFFFFFFFFFFFFFFULL, "negw32divneg1"},
        {0x0000000080000000ULL, 0xFFFFFFFFFFFFFFFFULL, "w32signbdivneg1"},
    };

    for (uint8_t dw = 0; dw <= 1; dw++) {
        for (auto fn : div_fns) {
            for (auto& op : div_operands) {
                snprintf(desc, sizeof(desc), "p2_dw%d_%s_%s",
                         dw ? 64 : 32, fn_name(fn), op.name);
                issue_and_wait(sim, fn, dw, op.a, op.b, 0x2A, desc);
            }
        }
    }

    // ================================================================
    // Phase 3: Kill during operation
    // ================================================================
    printf("phase 3: kill during operation\n");

    // Start a long divide, then kill it mid-flight
    for (int trial = 0; trial < 4; trial++) {
        uint8_t fn = div_fns[trial % 4];
        uint8_t dw = trial & 1;
        snprintf(desc, sizeof(desc), "p3_kill_%s_dw%d", fn_name(fn), dw ? 64 : 32);

        // Early phase
        sim.dut->stim_req_valid_early = 1;
        sim.dut->stim_req_fn_early    = fn;
        sim.dut->stim_req_valid       = 0;
        sim.dut->stim_req_fn          = fn;
        sim.dut->stim_req_dw          = dw;
        sim.dut->stim_req_in1         = 0x123456789ABCDEF0ULL;
        sim.dut->stim_req_in2         = 0x0000000000000003ULL;
        sim.dut->stim_req_dest        = 0x07;
        sim.dut->stim_kill            = 0;
        sim.dut->stim_resp_ready      = 1;
        sim.tick();
        compare_outputs(sim, desc);

        // Valid phase
        sim.dut->stim_req_valid_early = 0;
        sim.dut->stim_req_valid       = 1;
        sim.tick();
        compare_outputs(sim, desc);

        // Let it run for a few cycles
        sim.dut->stim_req_valid = 0;
        for (int i = 0; i < 5; i++) {
            sim.tick();
            compare_outputs(sim, desc);
        }

        // Kill
        sim.dut->stim_kill = 1;
        sim.tick();
        compare_outputs(sim, desc);

        sim.dut->stim_kill = 0;
        // Wait until req_ready returns
        for (int i = 0; i < 10; i++) {
            drive_idle(sim.dut.get());
            sim.tick();
            compare_outputs(sim, desc);
            if (sim.dut->orig_req_ready && sim.dut->new_req_ready)
                break;
        }

        // Now issue a normal op to confirm recovery
        snprintf(desc, sizeof(desc), "p3_recover_%d", trial);
        issue_and_wait(sim, FN_MUL, 1, 7, 11, 0x03, desc);
    }

    // ================================================================
    // Phase 4: 10K random operations
    // ================================================================
    printf("phase 4: 10K random operations\n");

    for (int i = 0; i < 10000; i++) {
        uint8_t fn   = all_fns[xorshift32() % 8];
        uint8_t dw   = xorshift32() & 1;
        uint64_t in1 = xorshift64();
        uint64_t in2 = xorshift64();
        uint8_t dest = xorshift32() & 0x7F;

        snprintf(desc, sizeof(desc), "p4_rand_%d", i);
        issue_and_wait(sim, fn, dw, in1, in2, dest, desc);

        if (sim.mismatches > 50) {
            printf("  Too many mismatches, stopping phase 4 early at iteration %d\n", i);
            break;
        }
    }

    // ================================================================
    // Phase 5: Back-to-back operations
    // ================================================================
    printf("phase 5: back-to-back operations\n");

    // Issue operations back-to-back: as soon as resp_valid fires, issue
    // the next one on the same cycle (early), then valid next cycle.
    uint8_t b2b_fns[]  = { FN_MUL, FN_MULH, FN_DIV, FN_DIVU, FN_REM, FN_MUL,
                           FN_MULHU, FN_MULHSU, FN_REMU, FN_MUL };
    uint8_t b2b_dws[]  = { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 };

    for (int op_idx = 0; op_idx < 10; op_idx++) {
        uint8_t fn   = b2b_fns[op_idx];
        uint8_t dw   = b2b_dws[op_idx];
        uint64_t in1 = xorshift64();
        uint64_t in2 = xorshift64();
        // Avoid divide-by-zero to keep timing deterministic in b2b
        if (fn == FN_DIV || fn == FN_DIVU || fn == FN_REM || fn == FN_REMU) {
            in2 |= 1;  // ensure non-zero
        }
        uint8_t dest = (op_idx + 1) & 0x1F;

        snprintf(desc, sizeof(desc), "p5_b2b_%d_%s_dw%d",
                 op_idx, fn_name(fn), dw ? 64 : 32);

        // Wait for req_ready if needed
        int wait = 0;
        while (!sim.dut->orig_req_ready && wait < 200) {
            sim.tick();
            compare_outputs(sim, desc);
            wait++;
        }

        // Issue: early phase
        issue_early(sim, fn, dw, in1, in2, dest);
        sim.tick();
        compare_outputs(sim, desc);

        // Valid phase
        sim.dut->stim_req_valid_early = 0;
        sim.dut->stim_req_valid       = 1;
        sim.tick();
        compare_outputs(sim, desc);

        // Deassert and wait for result
        sim.dut->stim_req_valid = 0;
        for (int cyc = 0; cyc < 200; cyc++) {
            sim.tick();
            compare_outputs(sim, desc);
            if (sim.dut->orig_resp_valid && sim.dut->new_resp_valid)
                break;
        }
    }

    // Drain
    for (int i = 0; i < 5; i++) {
        drive_idle(sim.dut.get());
        sim.tick();
        compare_outputs(sim, "drain");
    }

    // ================================================================
    // Summary
    // ================================================================
    return sim.finish();
}
