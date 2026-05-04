// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: intpipe_imm (new) vs intpipe_imm (original).
//
// Tests all 6 immediate formats with known RISC-V instructions, then
// 2M random instruction/format combinations.

#include "Vcosim_intpipe_imm_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>

using DUT = Vcosim_intpipe_imm_tb;

// Simple xorshift32 PRNG for reproducible results.
static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

// Immediate format encoding (matches both imm_ctrl_e and core_imm_ctrl).
enum {
    IMM_S  = 0,
    IMM_SB = 1,
    IMM_U  = 2,
    IMM_UJ = 3,
    IMM_I  = 4,
    IMM_Z  = 5,
    IMM_COUNT = 6
};

static const char* imm_name[] = {
    "ImmS", "ImmSb", "ImmU", "ImmUj", "ImmI", "ImmZ"
};

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;

    sim.dut->rst_ni = 1;

    // Phase 1: Known instruction patterns for each immediate format.
    printf("phase 1: known instruction patterns\n");

    struct {
        uint32_t inst;
        uint8_t  sel;
        const char* desc;
    } known[] = {
        // ImmI: ADDI x1,x0,1 => imm12 = 0x001
        {0x00100093, IMM_I,  "ADDI x1,x0,1 (ImmI)"},
        // ImmI: ADDI x1,x0,-1 => imm12 = 0xFFF
        {0xFFF00093, IMM_I,  "ADDI x1,x0,-1 (ImmI)"},
        // ImmI: LW x2,4(x1)
        {0x0040A103, IMM_I,  "LW x2,4(x1) (ImmI)"},
        // ImmI: JALR x0,x1,0
        {0x00008067, IMM_I,  "JALR x0,x1,0 (ImmI)"},

        // ImmS: SW x2,8(x1) => imm = {inst[31:25], inst[11:7]}
        {0x0020A423, IMM_S,  "SW x2,8(x1) (ImmS)"},
        // ImmS: SD x3,16(x1)
        {0x0030B823, IMM_S,  "SD x3,16(x1) (ImmS)"},
        // ImmS: SB x0,-1(x1)
        {0xFE008FA3, IMM_S,  "SB x0,-1(x1) (ImmS)"},

        // ImmSb: BEQ x1,x2,0
        {0x00208063, IMM_SB, "BEQ x1,x2,0 (ImmSb)"},
        // ImmSb: BNE x1,x2,8
        {0x00209463, IMM_SB, "BNE x1,x2,8 (ImmSb)"},
        // ImmSb: BLT with negative offset
        {0xFE20CEE3, IMM_SB, "BLT x1,x2,-4 (ImmSb)"},

        // ImmU: LUI x1,0x12345
        {0x12345037, IMM_U,  "LUI x1,0x12345 (ImmU)"},
        // ImmU: AUIPC x2,0
        {0x00000117, IMM_U,  "AUIPC x2,0 (ImmU)"},
        // ImmU: LUI x3,0xFFFFF
        {0xFFFFF1B7, IMM_U,  "LUI x3,0xFFFFF (ImmU)"},

        // ImmUj: JAL x1,0
        {0x000000EF, IMM_UJ, "JAL x1,0 (ImmUj)"},
        // ImmUj: JAL x1,4
        {0x004000EF, IMM_UJ, "JAL x1,4 (ImmUj)"},
        // ImmUj: JAL with large offset
        {0x7FFFF0EF, IMM_UJ, "JAL large offset (ImmUj)"},

        // ImmZ: CSRRWI x0,0,0x000
        {0x00005073, IMM_Z,  "CSRRWI x0,0 (ImmZ)"},
        // ImmZ: CSRRSI with imm=31
        {0x000FE073, IMM_Z,  "CSRRSI imm=31 (ImmZ)"},

        // Edge cases
        {0x00000000, IMM_I,  "all-zeros (ImmI)"},
        {0xFFFFFFFF, IMM_I,  "all-ones (ImmI)"},
        {0x00000000, IMM_S,  "all-zeros (ImmS)"},
        {0xFFFFFFFF, IMM_S,  "all-ones (ImmS)"},
        {0x00000000, IMM_SB, "all-zeros (ImmSb)"},
        {0xFFFFFFFF, IMM_SB, "all-ones (ImmSb)"},
        {0x00000000, IMM_U,  "all-zeros (ImmU)"},
        {0xFFFFFFFF, IMM_U,  "all-ones (ImmU)"},
        {0x00000000, IMM_UJ, "all-zeros (ImmUj)"},
        {0xFFFFFFFF, IMM_UJ, "all-ones (ImmUj)"},
        {0x00000000, IMM_Z,  "all-zeros (ImmZ)"},
        {0xFFFFFFFF, IMM_Z,  "all-ones (ImmZ)"},
        {0x80000000, IMM_I,  "sign-bit-only (ImmI)"},
        {0x7FFFFFFF, IMM_I,  "max-positive (ImmI)"},
    };

    for (auto& k : known) {
        sim.dut->stim_inst_bits = k.inst;
        sim.dut->stim_sel_imm  = k.sel;
        sim.dut->eval();

        uint64_t orig = sim.dut->orig_imm_o;
        uint64_t newv = sim.dut->new_imm_o;

        if (orig != newv) {
            printf("  MISMATCH %s: inst=0x%08x sel=%s orig=0x%016llx new=0x%016llx\n",
                   k.desc, k.inst, imm_name[k.sel],
                   (unsigned long long)orig, (unsigned long long)newv);
        }
        sim.compare(k.desc, orig, newv);
    }

    // Phase 2: Random instruction/format sweep (2M).
    printf("phase 2: random instruction/format sweep (2M values)\n");
    int phase2_mismatches = 0;
    for (int i = 0; i < 2000000; i++) {
        uint32_t inst = xorshift32();
        uint8_t  sel  = xorshift32() % IMM_COUNT;

        sim.dut->stim_inst_bits = inst;
        sim.dut->stim_sel_imm  = sel;
        sim.dut->eval();

        uint64_t orig = sim.dut->orig_imm_o;
        uint64_t newv = sim.dut->new_imm_o;

        if (orig != newv) {
            if (phase2_mismatches < 20) {
                printf("  MISMATCH inst=0x%08x sel=%s orig=0x%016llx new=0x%016llx\n",
                       inst, imm_name[sel],
                       (unsigned long long)orig, (unsigned long long)newv);
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
