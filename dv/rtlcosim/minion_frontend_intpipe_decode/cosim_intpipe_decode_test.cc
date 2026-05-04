// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: intpipe_decode (new) vs intpipe_decode (original).
//
// Tests millions of random 32-bit instruction encodings and compares the
// full decode output bit-for-bit. Both modules are purely combinational.

#include "Vcosim_intpipe_decode_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>

using DUT = Vcosim_intpipe_decode_tb;

// Simple xorshift32 PRNG for reproducible results.
static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;

    sim.dut->rst_ni = 1;

    // Phase 1: Known instruction patterns.
    // Test specific encodings from each instruction category.
    printf("phase 1: known instruction patterns\n");
    struct {
        uint32_t in;
        const char* name;
    } known[] = {
        // Basic RISC-V
        {0x00000013, "NOP (ADDI x0,x0,0)"},
        {0x00100093, "ADDI x1,x0,1"},
        {0x00A00113, "ADDI x2,x0,10"},
        {0x002081B3, "ADD x3,x1,x2"},
        {0x40208233, "SUB x4,x1,x2"},
        {0x0020A2B3, "SLT x5,x1,x2"},
        {0x0020B333, "SLTU x6,x1,x2"},
        {0x0020C3B3, "XOR x7,x1,x2"},
        {0x0020E433, "OR x8,x1,x2"},
        {0x0020F4B3, "AND x9,x1,x2"},
        {0x00209533, "SLL x10,x1,x2"},
        {0x0020D5B3, "SRL x11,x1,x2"},
        {0x4020D633, "SRA x12,x1,x2"},

        // Branches
        {0x00208063, "BEQ x1,x2,0"},
        {0x00209063, "BNE x1,x2,0"},
        {0x0020C063, "BLT x1,x2,0"},
        {0x0020D063, "BGE x1,x2,0"},
        {0x0020E063, "BLTU x1,x2,0"},
        {0x0020F063, "BGEU x1,x2,0"},

        // Jumps
        {0x000000EF, "JAL x1,0"},
        {0x00008067, "JALR x0,x1,0 (RET)"},

        // Loads / Stores
        {0x00008003, "LB x0,0(x1)"},
        {0x00009003, "LH x0,0(x1)"},
        {0x0000A003, "LW x0,0(x1)"},
        {0x0000B003, "LD x0,0(x1)"},
        {0x0000C003, "LBU x0,0(x1)"},
        {0x0000D003, "LHU x0,0(x1)"},
        {0x0000E003, "LWU x0,0(x1)"},
        {0x00008023, "SB x0,0(x1)"},
        {0x00009023, "SH x0,0(x1)"},
        {0x0000A023, "SW x0,0(x1)"},
        {0x0000B023, "SD x0,0(x1)"},

        // Upper immediate
        {0x00000037, "LUI x0,0"},
        {0x00000017, "AUIPC x0,0"},

        // CSR
        {0x00001073, "CSRRW x0,x0,0x000"},
        {0x00002073, "CSRRS x0,x0,0x000"},
        {0x00003073, "CSRRC x0,x0,0x000"},
        {0x00005073, "CSRRWI x0,0,0x000"},
        {0x00006073, "CSRRSI x0,0,0x000"},
        {0x00007073, "CSRRCI x0,0,0x000"},

        // System
        {0x00000073, "ECALL"},
        {0x00100073, "EBREAK"},
        {0x30200073, "MRET"},
        {0x10200073, "SRET"},
        {0x7B200073, "DRET"},
        {0x10500073, "WFI"},

        // Fence
        {0x0FF0000F, "FENCE"},
        {0x0000100F, "FENCE.I"},

        // Mul/Div
        {0x02208033, "MUL x0,x1,x2"},
        {0x02209033, "MULH x0,x1,x2"},
        {0x0220A033, "MULHSU x0,x1,x2"},
        {0x0220B033, "MULHU x0,x1,x2"},
        {0x0220C033, "DIV x0,x1,x2"},
        {0x0220D033, "DIVU x0,x1,x2"},
        {0x0220E033, "REM x0,x1,x2"},
        {0x0220F033, "REMU x0,x1,x2"},

        // W-suffix
        {0x0000001B, "ADDIW x0,x0,0"},
        {0x0220003B, "MULW x0,x0,x2"},

        // FP single
        {0x00007053, "FADD.S"},
        {0x08007053, "FSUB.S"},
        {0x10007053, "FMUL.S"},
        {0xE0001053, "FMV.X.S"},
        {0xF0000053, "FMV.S.X"},
        {0x5880007B, "FRSQ_PS"},
        {0x5860007B, "FSIN_PS"},

        // FP loads/stores
        {0x00002007, "FLW"},
        {0x00002027, "FSW"},
        {0x00003007, "FLD"},
        {0x00003027, "FSD"},

        // Illegal
        {0x00000000, "illegal (all zeros)"},
        {0xFFFFFFFF, "illegal (all ones)"},
    };

    for (auto& k : known) {
        sim.dut->stim_i = k.in;
        sim.dut->eval();

        uint64_t orig = 0, newv = 0;
        // Verilator exposes VlWide for > 64-bit, but 46 bits fits in uint64.
        // Access as 2x 32-bit words.
        orig = sim.dut->orig_out_o;
        newv = sim.dut->new_out_o;

        // Mask to 45 bits.
        orig &= 0x1FFFFFFFFFFFULL;
        newv &= 0x1FFFFFFFFFFFULL;

        if (orig != newv) {
            printf("  MISMATCH %s: inst=0x%08x orig=0x%012llx new=0x%012llx\n",
                   k.name, k.in,
                   (unsigned long long)orig, (unsigned long long)newv);
        }
        sim.compare(k.name, orig, newv);
    }

    // Phase 2: Random instruction sweep.
    printf("phase 2: random instruction sweep (2M values)\n");
    int phase2_mismatches = 0;
    for (int i = 0; i < 2000000; i++) {
        uint32_t inst = xorshift32();
        sim.dut->stim_i = inst;
        sim.dut->eval();

        uint64_t orig = sim.dut->orig_out_o & 0x1FFFFFFFFFFFULL;
        uint64_t newv = sim.dut->new_out_o & 0x1FFFFFFFFFFFULL;

        if (orig != newv) {
            if (phase2_mismatches < 20) {
                printf("  MISMATCH inst=0x%08x orig=0x%012llx new=0x%012llx\n",
                       inst, (unsigned long long)orig, (unsigned long long)newv);
            }
            phase2_mismatches++;
        }
        sim.compare("random", orig, newv);
    }

    if (phase2_mismatches > 20) {
        printf("  ... and %d more mismatches (truncated)\n", phase2_mismatches - 20);
    }

    return sim.finish();
}
