// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_uinst_decoder_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_vpu_uinst_decoder_tb;

namespace {

constexpr int kRandomIters = 50000;

struct WordCoverage {
    uint32_t or_mask = 0u;
    uint32_t and_mask = 0xffffffffu;

    void observe(uint32_t value) {
        or_mask |= value;
        and_mask &= value;
    }
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint32_t make_inst(uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3,
                   uint32_t rd, uint32_t opcode) {
    return ((funct7 & 0x7fu) << 25) | ((rs2 & 0x1fu) << 20) | ((rs1 & 0x1fu) << 15) |
           ((funct3 & 0x7u) << 12) | ((rd & 0x1fu) << 7) | (opcode & 0x7fu);
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("id_sigs", sim.dut->orig_id_sigs_o, sim.dut->new_id_sigs_o);
}

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& inst_cov, uint32_t inst) {
    sim.dut->id_inst_i = inst;
    inst_cov.observe(inst);
    sim.tick();
    compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage inst_cov;

    std::array<uint32_t, 37> directed = {{
        0x0000600bU,                              // FSW_PS
        make_inst(0x00, 1, 2, 0, 3, 0x7b),       // FADD_PS
        make_inst(0x04, 4, 5, 0, 6, 0x7b),       // FSUB_PS
        make_inst(0x08, 7, 8, 0, 9, 0x7b),       // FMUL_PS
        make_inst(0x14, 1, 2, 0, 3, 0x7b),       // FMIN_PS
        make_inst(0x14, 1, 2, 1, 3, 0x7b),       // FMAX_PS
        make_inst(0x00, 1, 2, 0, 3, 0x5b),       // FMADD_PS
        make_inst(0x60, 0, 0, 0, 1, 0x7b),       // FCVT_PW_PS
        make_inst(0x68, 0, 0, 0, 1, 0x7b),       // FCVT_PS_PW
        make_inst(0x00, 1, 2, 0, 3, 0x77),       // FCMOVM_PS
        make_inst(0x00, 0, 0, 0, 1, 0x33),       // TR_FMA1_RCP
        make_inst(0x01, 0, 0, 0, 1, 0x33),       // TR_FMA2_RCP
        make_inst(0x02, 0, 0, 0, 1, 0x33),       // TR_FMA1_RSQRT
        make_inst(0x03, 0, 0, 0, 1, 0x33),       // TR_FMA2_RSQRT
        make_inst(0x07, 0, 0, 0, 1, 0x33),       // TR_FMA1_LOG
        make_inst(0x08, 0, 0, 0, 1, 0x33),       // TR_FMA2_LOG
        make_inst(0x09, 0, 0, 0, 1, 0x33),       // TR_MUL_LOG
        make_inst(0x0a, 0, 0, 0, 1, 0x33),       // TR_TR_SIN
        make_inst(0x0b, 0, 0, 0, 1, 0x33),       // TR_RR_SIN
        make_inst(0x0c, 0, 0, 0, 1, 0x33),       // TR_P1_SIN
        make_inst(0x0d, 0, 0, 0, 1, 0x33),       // TR_P2_SIN
        make_inst(0x0e, 0, 0, 0, 1, 0x33),       // TR_P3_SIN
        make_inst(0x0f, 0, 0, 0, 1, 0x33),       // TR_RR_EXP
        make_inst(0x10, 0, 0, 0, 1, 0x33),       // TR_FMA1_EXP
        make_inst(0x11, 0, 0, 0, 1, 0x33),       // TR_FMA2_EXP
        make_inst(0x04, 0, 0, 0, 1, 0x33),       // TR_NR
        make_inst(0x03, 1, 2, 0, 3, 0x7b),       // FADD_PI
        make_inst(0x07, 1, 2, 0, 3, 0x7b),       // FSUB_PI
        make_inst(0x17, 1, 2, 0, 3, 0x7b),       // FMIN_PI
        make_inst(0x17, 1, 2, 1, 3, 0x7b),       // FMAX_PI
        make_inst(0x03, 1, 2, 4, 3, 0x7b),       // FXOR_PI
        make_inst(0x03, 0, 1, 3, 2, 0x7b),       // FSAT8_PI
        make_inst(0x03, 1, 1, 3, 2, 0x7b),       // FSATU8_PI
        make_inst(0x13, 0, 1, 0, 2, 0x7b),       // FPACKREPB_PI
        make_inst(0x05, 1, 2, 0, 3, 0x43),       // TMUL_PH_PS
        make_inst(0x01, 1, 2, 0, 3, 0x4b),       // TFMA_PH_PS
        0x00000000U                              // default
    }};

    drive_and_compare(sim, inst_cov, 0);

    for (uint32_t inst : directed) {
        drive_and_compare(sim, inst_cov, inst);
    }

    for (uint32_t funct7 = 0; funct7 < 0x80u; ++funct7) {
        drive_and_compare(sim, inst_cov, make_inst(funct7, funct7 ^ 0x1fu, funct7 >> 1, funct7 & 0x7u, funct7 >> 2, 0x33));
        drive_and_compare(sim, inst_cov, make_inst(funct7, funct7 ^ 0x1fu, funct7 >> 1, funct7 & 0x7u, funct7 >> 2, 0x7b));
    }

    uint32_t seed = 0x41d1decaU;
    for (int i = 0; i < kRandomIters; ++i) {
        drive_and_compare(sim, inst_cov, xorshift32(seed));
    }

    sim.check(inst_cov.or_mask == 0xffffffffu, "id_inst_i should drive every bit high");
    sim.check(inst_cov.and_mask == 0x00000000u, "id_inst_i should drive every bit low");
    sim.check(sim.comparisons >= 50000, "cosim should generate at least 50k comparisons for this combinational decoder");

    return sim.finish();
}
