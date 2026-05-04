// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: vpu_decoder (new) vs vpu_decoder (original).
//
// Tests known FP instruction encodings and millions of random 32-bit
// instruction encodings, comparing the full decode output bit-for-bit.
// Both modules are purely combinational.

#include "Vcosim_vpu_decoder_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>

using DUT = Vcosim_vpu_decoder_tb;

// Simple xorshift32 PRNG for reproducible results.
static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static constexpr uint64_t MASK_45 = 0x1FFFFFFFFFFFULL;

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;

    sim.dut->rst_ni = 1;

    // Phase 1: Known FP/VPU instruction patterns.
    printf("phase 1: known FP/VPU instruction patterns\n");
    struct {
        uint32_t in;
        const char* name;
    } known[] = {
        // FP loads/stores (single)
        {0x00002007, "FLW"},
        {0x00002027, "FSW"},

        // FP loads/stores (double)
        {0x00003007, "FLD"},
        {0x00003027, "FSD"},

        // FP single arithmetic
        {0x00007053, "FADD.S"},
        {0x08007053, "FSUB.S"},
        {0x10007053, "FMUL.S"},
        {0x18007053, "FDIV.S"},
        {0x58007053, "FSQRT.S"},

        // FP single sign-injection
        {0x20000053, "FSGNJ.S"},
        {0x20001053, "FSGNJN.S"},
        {0x20002053, "FSGNJX.S"},

        // FP single min/max
        {0x28000053, "FMIN.S"},
        {0x28001053, "FMAX.S"},

        // FP single compare
        {0xA0002053, "FEQ.S"},
        {0xA0001053, "FLT.S"},
        {0xA0000053, "FLE.S"},

        // FP single conversion (int<->fp)
        {0xC0000053, "FCVT.W.S"},
        {0xC0100053, "FCVT.WU.S"},
        {0xD0000053, "FCVT.S.W"},
        {0xD0100053, "FCVT.S.WU"},

        // FP single move (int<->fp)
        {0xE0000053, "FMV.X.W"},
        {0xF0000053, "FMV.W.X"},

        // FP single classify
        {0xE0001053, "FCLASS.S"},

        // Original ENABLE_EXTRA_TRANS seam
        {0x5880007B, "FRSQ_PS"},
        {0x5860007B, "FSIN_PS"},

        // FP single fused multiply-add
        {0x00000043, "FMADD.S"},
        {0x00000047, "FMSUB.S"},
        {0x0000004B, "FNMSUB.S"},
        {0x0000004F, "FNMADD.S"},

        // FP double arithmetic
        {0x02007053, "FADD.D"},
        {0x0A007053, "FSUB.D"},
        {0x12007053, "FMUL.D"},
        {0x1A007053, "FDIV.D"},
        {0x5A007053, "FSQRT.D"},

        // FP double fused multiply-add
        {0x02000043, "FMADD.D"},
        {0x02000047, "FMSUB.D"},
        {0x0200004B, "FNMSUB.D"},
        {0x0200004F, "FNMADD.D"},

        // FP double conversion
        {0xC2000053, "FCVT.W.D"},
        {0xC2100053, "FCVT.WU.D"},
        {0xD2000053, "FCVT.D.W"},
        {0xD2100053, "FCVT.D.WU"},
        {0x40100053, "FCVT.S.D"},
        {0x42000053, "FCVT.D.S"},

        // FP 64-bit int conversions
        {0xC0200053, "FCVT.L.S"},
        {0xC0300053, "FCVT.LU.S"},
        {0xD0200053, "FCVT.S.L"},
        {0xD0300053, "FCVT.S.LU"},

        // Non-FP instructions (should set id_vpu_insn = 0)
        {0x00000013, "NOP (ADDI x0,x0,0)"},
        {0x002081B3, "ADD x3,x1,x2"},
        {0x00208063, "BEQ x1,x2,0"},
        {0x000000EF, "JAL x1,0"},
        {0x0000A003, "LW x0,0(x1)"},
        {0x0000A023, "SW x0,0(x1)"},

        // Illegal
        {0x00000000, "illegal (all zeros)"},
        {0xFFFFFFFF, "illegal (all ones)"},
    };

    for (auto& k : known) {
        sim.dut->stim_i = k.in;
        sim.dut->eval();

        uint64_t orig_sigs = (uint64_t)sim.dut->orig_sigs_o & MASK_45;
        uint64_t new_sigs  = (uint64_t)sim.dut->new_sigs_o  & MASK_45;
        uint8_t  orig_vpu  = sim.dut->orig_vpu_o;
        uint8_t  new_vpu   = sim.dut->new_vpu_o;

        if (orig_sigs != new_sigs) {
            printf("  MISMATCH sigs %s: inst=0x%08x orig=0x%012llx new=0x%012llx\n",
                   k.name, k.in,
                   (unsigned long long)orig_sigs, (unsigned long long)new_sigs);
        }
        sim.compare(k.name, orig_sigs, new_sigs);

        if (orig_vpu != new_vpu) {
            printf("  MISMATCH vpu  %s: inst=0x%08x orig=%d new=%d\n",
                   k.name, k.in, orig_vpu, new_vpu);
        }
        sim.compare(k.name, orig_vpu, new_vpu);
    }

    // Phase 2: Random instruction sweep.
    printf("phase 2: random instruction sweep (2M values)\n");
    int phase2_sigs_mismatches = 0;
    int phase2_vpu_mismatches = 0;
    for (int i = 0; i < 2000000; i++) {
        uint32_t inst = xorshift32();
        sim.dut->stim_i = inst;
        sim.dut->eval();

        uint64_t orig_sigs = (uint64_t)sim.dut->orig_sigs_o & MASK_45;
        uint64_t new_sigs  = (uint64_t)sim.dut->new_sigs_o  & MASK_45;
        uint8_t  orig_vpu  = sim.dut->orig_vpu_o;
        uint8_t  new_vpu   = sim.dut->new_vpu_o;

        if (orig_sigs != new_sigs) {
            if (phase2_sigs_mismatches < 20) {
                printf("  MISMATCH sigs inst=0x%08x orig=0x%012llx new=0x%012llx\n",
                       inst, (unsigned long long)orig_sigs, (unsigned long long)new_sigs);
            }
            phase2_sigs_mismatches++;
        }
        sim.compare("random_sigs", orig_sigs, new_sigs);

        if (orig_vpu != new_vpu) {
            if (phase2_vpu_mismatches < 20) {
                printf("  MISMATCH vpu  inst=0x%08x orig=%d new=%d\n",
                       inst, orig_vpu, new_vpu);
            }
            phase2_vpu_mismatches++;
        }
        sim.compare("random_vpu", orig_vpu, new_vpu);
    }

    if (phase2_sigs_mismatches > 20) {
        printf("  ... and %d more sigs mismatches (truncated)\n",
               phase2_sigs_mismatches - 20);
    }
    if (phase2_vpu_mismatches > 20) {
        printf("  ... and %d more vpu mismatches (truncated)\n",
               phase2_vpu_mismatches - 20);
    }

    return sim.finish();
}
