// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_uinst_decoder_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vvpu_uinst_decoder_tb;

namespace {

struct Expected {
    uint8_t cmd;
    bool txfma;
    bool shsw;
    bool rom;
    bool scatter;
    bool cvt;
    bool trans;
    bool gcvt;
    bool fromintf;
    bool tointm;
    bool tointf;
    bool maskop;
    bool ldst;
    bool m0ren;
    bool mallren;
    bool mren1;
    bool mren2;
    bool wen;
    bool ren1;
    bool ren2;
    bool ren3;
    bool rend;
    bool swap12;
    bool swap23;
    bool swap13;
    bool vector;
    uint8_t dtype;
    bool fromint;
    bool toint;
    bool fma;
    bool fms;
    bool add;
    bool sub;
    bool mul;
    bool round;
    bool wflags;
};

constexpr uint8_t kCmdAdd          = 0x00;
constexpr uint8_t kCmdSub          = 0x01;
constexpr uint8_t kCmdMul          = 0x02;
constexpr uint8_t kCmdMadd         = 0x07;
constexpr uint8_t kCmdMin          = 0x0f;
constexpr uint8_t kCmdMax          = 0x10;
constexpr uint8_t kCmdMvXf         = 0x14;
constexpr uint8_t kCmdCvtIntf32    = 0x15;
constexpr uint8_t kCmdXor          = 0x1b;
constexpr uint8_t kCmdFcmovm       = 0x2e;
constexpr uint8_t kCmdPackrepb     = 0x6d;
constexpr uint8_t kCmdNr2Frcpfxp   = 0x0d;
constexpr uint8_t kCmdFsatu8       = 0x61;
constexpr uint8_t kCmdFsat8        = 0x7e;
constexpr uint8_t kTransRcpFma1    = 0x63;
constexpr uint8_t kTransRsqrtFma1  = 0x66;
constexpr uint8_t kTransRsqrtFma2  = 0x67;
constexpr uint8_t kTransLogFma1    = 0x72;
constexpr uint8_t kTransSinTrans   = 0x75;
constexpr uint8_t kTransSinRr      = 0x76;
constexpr uint8_t kTransSinP1      = 0x77;
constexpr uint8_t kTransSinP2      = 0x78;
constexpr uint8_t kTransSinP3      = 0x79;
constexpr uint8_t kTransExpRr      = 0x7b;

constexpr uint8_t kDtypeInt        = 0x0;
constexpr uint8_t kDtypeF32        = 0x1;
constexpr uint8_t kDtypeF16F32     = 0xe;

uint32_t make_inst(uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3,
                   uint32_t rd, uint32_t opcode) {
    return ((funct7 & 0x7fu) << 25) | ((rs2 & 0x1fu) << 20) | ((rs1 & 0x1fu) << 15) |
           ((funct3 & 0x7u) << 12) | ((rd & 0x1fu) << 7) | (opcode & 0x7fu);
}

uint64_t pack_expected(const Expected& e) {
    uint64_t packed = 0;
    const auto push = [&packed](unsigned width, uint64_t value) {
        packed = (packed << width) | (value & ((1ULL << width) - 1ULL));
    };

    push(7, e.cmd);
    push(1, e.txfma);
    push(1, e.shsw);
    push(1, e.rom);
    push(1, e.scatter);
    push(1, e.cvt);
    push(1, e.trans);
    push(1, e.gcvt);
    push(1, e.fromintf);
    push(1, e.tointm);
    push(1, e.tointf);
    push(1, e.maskop);
    push(1, e.ldst);
    push(1, e.m0ren);
    push(1, e.mallren);
    push(1, e.mren1);
    push(1, e.mren2);
    push(1, e.wen);
    push(1, e.ren1);
    push(1, e.ren2);
    push(1, e.ren3);
    push(1, e.rend);
    push(1, e.swap12);
    push(1, e.swap23);
    push(1, e.swap13);
    push(1, e.vector);
    push(4, e.dtype);
    push(1, e.fromint);
    push(1, e.toint);
    push(1, e.fma);
    push(1, e.fms);
    push(1, e.add);
    push(1, e.sub);
    push(1, e.mul);
    push(1, e.round);
    push(1, e.wflags);

    return packed;
}

Expected zero_expected() {
    return {0, false,false,false, false,false,false,false, false,false,false,false,
            false,false,false,false,false, false,false,false,false,false, false,false,false,
            false, 0, false,false,false,false,false,false,false,false,false};
}

void check_case(SimCtrl<DUT>& sim, uint32_t inst, const Expected& expected, const char* name) {
    sim.dut->id_inst_i = inst;
    sim.dut->eval();
    sim.check(sim.dut->id_sigs_o == pack_expected(expected), std::string(name) + ": packed decode mismatch");
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.dut->id_inst_i = 0;
    sim.dut->eval();

    const bool enable_extra_trans = sim.dut->enable_extra_trans_o;

    check_case(sim, make_inst(0x00, 0x00, 0x00, 0x6, 0x00, 0x0b),
               {kCmdMvXf, false,false,false, false,false,false,false, false,false,false,false,
                true,true,false,false,false, false,false,true,false,false, true,false,false,
                true, kDtypeF32, false,false,false,false,false,false,false,false,false},
               "FSW_PS");

    check_case(sim, make_inst(0x00, 0x02, 0x03, 0x0, 0x01, 0x7b),
               {kCmdAdd, true,false,false, false,false,false,false, false,false,false,false,
                false,true,false,false,false, true,true,true,false,false, false,true,false,
                true, kDtypeF32, false,false,false,false,true,false,false,true,true},
               "FADD_PS");

    check_case(sim, make_inst(0x00, 0x00, 0x00, 0x0, 0x04, 0x77),
               {kCmdFcmovm, false,true,false, false,false,false,false, true,false,false,false,
                false,true,false,false,false, true,true,true,false,false, false,false,true,
                true, kDtypeF32, false,false,false,false,false,false,false,false,false},
               "FCMOVM_PS");

    check_case(sim, make_inst(0x00, 0x00, 0x00, 0x0, 0x05, 0x33),
               {kTransRcpFma1, true,false,true, false,false,true,false, false,false,false,false,
                false,false,false,false,false, false,false,true,false,false, false,false,false,
                true, kDtypeInt, false,false,true,false,false,false,false,false,false},
               "TR_FMA1_RCP");

    check_case(sim, make_inst(0x07, 0x00, 0x00, 0x0, 0x06, 0x33),
               {kTransLogFma1, true,true,true, false,false,true,false, false,false,false,false,
                false,false,false,false,false, false,false,true,false,false, false,false,false,
                true, kDtypeInt, false,false,true,false,false,false,false,false,false},
               "TR_FMA1_LOG");

    check_case(sim, make_inst(0x0f, 0x00, 0x00, 0x0, 0x07, 0x33),
               {kTransExpRr, false,false,true, false,false,true,false, false,false,false,false,
                false,false,false,false,false, false,false,true,false,false, false,false,false,
                true, kDtypeF32, false,false,false,false,false,false,false,false,false},
               "TR_RR_EXP");

    check_case(sim, make_inst(0x04, 0x00, 0x00, 0x0, 0x08, 0x33),
               {kCmdNr2Frcpfxp, true,false,false, false,false,true,false, false,false,false,false,
                false,false,false,false,false, true,false,true,true,false, false,false,false,
                true, kDtypeInt, false,false,true,false,false,false,false,false,true},
               "TR_NR");

    check_case(sim, make_inst(0x03, 0x02, 0x03, 0x0, 0x09, 0x7b),
               {kCmdAdd, false,true,false, false,false,false,false, false,false,false,false,
                false,true,false,false,false, true,true,true,false,false, false,true,false,
                true, kDtypeInt, false,false,false,false,true,false,false,false,false},
               "FADD_PI");

    check_case(sim, make_inst(0x03, 0x01, 0x00, 0x3, 0x0a, 0x7b),
               {kCmdFsatu8, false,true,false, false,false,false,false, false,false,false,false,
                false,true,false,false,false, true,true,false,false,false, false,false,false,
                true, kDtypeF32, false,false,false,false,false,false,false,false,false},
               "FSATU8_PI");

    check_case(sim, make_inst(0x13, 0x00, 0x0b, 0x0, 0x0c, 0x7b),
               {kCmdPackrepb, false,true,false, false,false,false,false, true,false,false,false,
                false,true,false,false,false, true,true,false,false,false, false,false,false,
                true, kDtypeF32, false,false,false,false,false,false,false,false,false},
               "FPACKREPB_PI");

    check_case(sim, make_inst(0x05, 0x02, 0x03, 0x0, 0x0d, 0x43),
               {kCmdMul, true,false,false, false,false,false,false, false,false,false,false,
                false,true,false,false,false, true,true,true,false,false, false,false,false,
                true, kDtypeF16F32, false,false,false,false,false,false,true,true,true},
               "TMUL_PH_PS");

    check_case(sim, make_inst(0x01, 0x02, 0x03, 0x0, 0x0e, 0x4b),
               {kCmdMadd, true,false,false, false,false,false,false, false,false,false,false,
                false,true,false,false,false, true,true,true,true,false, false,false,false,
                true, kDtypeF16F32, false,false,true,false,false,false,false,true,true},
               "TFMA_PH_PS");

    check_case(sim, make_inst(0x02, 0x00, 0x00, 0x0, 0x1f, 0x33),
               enable_extra_trans
                   ? Expected{kTransRsqrtFma1, true,false,true, false,false,true,false, false,false,false,false,
                              false,false,false,false,false, false,false,true,false,false, false,false,false,
                              true, kDtypeInt, false,false,true,false,false,false,false,false,false}
                   : zero_expected(),
               enable_extra_trans ? "TR_FMA1_RSQRT enabled" : "TR_FMA1_RSQRT defaults to zero");

    check_case(sim, make_inst(0x03, 0x00, 0x00, 0x0, 0x1e, 0x33),
               enable_extra_trans
                   ? Expected{kTransRsqrtFma2, true,false,false, false,false,true,false, false,false,false,false,
                              false,false,false,false,false, true,false,true,false,false, false,false,false,
                              true, kDtypeInt, false,false,true,false,false,false,false,false,true}
                   : zero_expected(),
               enable_extra_trans ? "TR_FMA2_RSQRT enabled" : "TR_FMA2_RSQRT defaults to zero");

    check_case(sim, make_inst(0x0a, 0x00, 0x00, 0x0, 0x1d, 0x33),
               enable_extra_trans
                   ? Expected{kTransSinTrans, true,false,false, false,false,true,false, false,false,false,false,
                              false,false,false,false,false, true,false,false,false,false, false,false,false,
                              true, kDtypeF32, false,false,false,false,false,false,false,true,false}
                   : zero_expected(),
               enable_extra_trans ? "TR_TR_SIN enabled" : "TR_TR_SIN defaults to zero");

    check_case(sim, make_inst(0x0b, 0x00, 0x00, 0x0, 0x1c, 0x33),
               enable_extra_trans
                   ? Expected{kTransSinRr, false,false,true, false,false,true,false, false,false,false,false,
                              false,false,false,false,false, false,false,true,false,false, false,false,false,
                              true, kDtypeInt, false,false,false,false,false,false,false,false,false}
                   : zero_expected(),
               enable_extra_trans ? "TR_RR_SIN enabled" : "TR_RR_SIN defaults to zero");

    check_case(sim, make_inst(0x0c, 0x00, 0x00, 0x0, 0x1b, 0x33),
               enable_extra_trans
                   ? Expected{kTransSinP1, true,false,true, false,false,true,false, false,false,false,false,
                              false,false,false,false,false, false,true,true,false,false, false,false,false,
                              true, kDtypeF32, false,false,false,false,false,false,true,true,false}
                   : zero_expected(),
               enable_extra_trans ? "TR_P1_SIN enabled" : "TR_P1_SIN defaults to zero");

    check_case(sim, make_inst(0x0d, 0x00, 0x00, 0x0, 0x1a, 0x33),
               enable_extra_trans
                   ? Expected{kTransSinP2, true,false,true, false,false,true,false, false,false,false,false,
                              false,false,false,false,false, false,false,true,false,false, false,false,false,
                              true, kDtypeF32, false,false,true,false,false,false,false,true,false}
                   : zero_expected(),
               enable_extra_trans ? "TR_P2_SIN enabled" : "TR_P2_SIN defaults to zero");

    check_case(sim, make_inst(0x0e, 0x00, 0x00, 0x0, 0x19, 0x33),
               enable_extra_trans
                   ? Expected{kTransSinP3, true,false,false, false,false,true,false, false,false,false,false,
                              false,false,false,false,false, true,false,true,false,false, false,false,false,
                              true, kDtypeF32, false,false,false,false,false,false,true,true,true}
                   : zero_expected(),
               enable_extra_trans ? "TR_P3_SIN enabled" : "TR_P3_SIN defaults to zero");

    return sim.finish();
}
