// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_sh_sw_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vvpu_sh_sw_tb;

namespace {

constexpr uint8_t kDtypeInt = 0x0;
constexpr uint8_t kDtypeF32 = 0x1;

constexpr uint8_t kCmdAdd          = 0x00;
constexpr uint8_t kCmdSub          = 0x01;
constexpr uint8_t kCmdMvXf         = 0x14;
constexpr uint8_t kCmdNot          = 0x18;
constexpr uint8_t kCmdAndi         = 0x4f;
constexpr uint8_t kCmdSll          = 0x1c;
constexpr uint8_t kCmdSlli         = 0x50;
constexpr uint8_t kCmdSra          = 0x1e;
constexpr uint8_t kCmdSrai         = 0x54;
constexpr uint8_t kCmdFeq          = 0x1f;
constexpr uint8_t kCmdFle          = 0x20;
constexpr uint8_t kCmdFltu         = 0x22;
constexpr uint8_t kCmdFsetm        = 0x27;
constexpr uint8_t kCmdFcmov        = 0x2d;
constexpr uint8_t kCmdFcmovm       = 0x2e;
constexpr uint8_t kCmdFswizz       = 0x2f;
constexpr uint8_t kCmdMvsFx        = 0x31;
constexpr uint8_t kCmdFbci         = 0x52;
constexpr uint8_t kCmdFsgnjx       = 0x5a;
constexpr uint8_t kCmdMaxu         = 0x60;
constexpr uint8_t kCmdFsatu8       = 0x61;
constexpr uint8_t kCmdCubeface     = 0x69;
constexpr uint8_t kCmdCubefaceIdx  = 0x6a;
constexpr uint8_t kCmdCubesgnsc    = 0x6b;
constexpr uint8_t kCmdCubesgntc    = 0x6c;
constexpr uint8_t kCmdPackrepb     = 0x6d;
constexpr uint8_t kCmdPackreph     = 0x6e;
constexpr uint8_t kCmdTransLogFma1 = 0x72;
constexpr uint8_t kCmdAddi         = 0x4e;
constexpr uint8_t kCmdFsat8        = 0x7e;

constexpr uint8_t kFlagInvalid = 4;

struct Issue {
    uint8_t lane_id;
    uint8_t cmd;
    uint8_t dtype;
    bool mask_in1;
    uint32_t in1;
    uint32_t in2;
    uint32_t in3;
    uint32_t imm;
    uint32_t swizz1;
    uint32_t swizz2;
    uint32_t swizz3;
    uint32_t expected_data;
    uint8_t expected_exc;
    const char* name;
};

uint32_t sign_extend(uint32_t value, unsigned bits) {
    const uint32_t sign_bit = 1u << (bits - 1);
    const uint32_t mask = (bits == 32) ? 0xffffffffu : ((1u << bits) - 1u);
    value &= mask;
    return (value ^ sign_bit) - sign_bit;
}

uint32_t fbci_ps_expected(uint32_t imm20) {
    const uint32_t low = imm20 & 0xfu;
    const uint32_t low12 = (low < 8u) ? ((low << 8) | (low << 4) | low)
                                      : ((low << 8) | (low << 4) | ((low + 1u) & 0xfu));
    return ((imm20 & 0xfffffu) << 12) | low12;
}

void set_defaults(DUT* dut) {
    dut->lane_id_i = 0;
    dut->ex_in_valid_i = 0;
    dut->ex_cmd_i = 0;
    dut->ex_in1_i = 0;
    dut->ex_in2_i = 0;
    dut->ex_in3_i = 0;
    dut->ex_imm_i = 0;
    dut->ex_dtype_i = kDtypeInt;
    dut->ex_mask_in1_i = 0;
    dut->f2_swizz_in1_i = 0;
    dut->f2_swizz_in2_i = 0;
    dut->f2_swizz_in3_i = 0;
}

void drive_issue(DUT* dut, const Issue& issue) {
    dut->lane_id_i = issue.lane_id;
    dut->ex_in_valid_i = 1;
    dut->ex_cmd_i = issue.cmd;
    dut->ex_in1_i = issue.in1;
    dut->ex_in2_i = issue.in2;
    dut->ex_in3_i = issue.in3;
    dut->ex_imm_i = issue.imm;
    dut->ex_dtype_i = issue.dtype;
    dut->ex_mask_in1_i = issue.mask_in1;
    dut->f2_swizz_in1_i = issue.swizz1;
    dut->f2_swizz_in2_i = issue.swizz2;
    dut->f2_swizz_in3_i = issue.swizz3;
}

void clear_issue(DUT* dut, const Issue& issue) {
    dut->ex_in_valid_i = 0;
    dut->ex_cmd_i = 0;
    dut->ex_in1_i = 0;
    dut->ex_in2_i = 0;
    dut->ex_in3_i = 0;
    dut->ex_imm_i = 0;
    dut->ex_dtype_i = kDtypeInt;
    dut->ex_mask_in1_i = 0;
    // These sideband swizzle operands are consumed in the F2->F3 cycle without
    // being flopped, so they must remain stable through the capture edge.
    dut->f2_swizz_in1_i = issue.swizz1;
    dut->f2_swizz_in2_i = issue.swizz2;
    dut->f2_swizz_in3_i = issue.swizz3;
}

void issue_and_check(SimCtrl<DUT>& sim, const Issue& issue) {
    drive_issue(sim.dut.get(), issue);
    sim.tick();
    sim.check(sim.dut->f2_fswizz_rdata_o == issue.in1,
              std::string(issue.name) + ": F2 swizz read data should reflect the latched in1 operand");

    clear_issue(sim.dut.get(), issue);
    sim.tick();
    sim.check(sim.dut->f3_out_data_o == issue.expected_data,
              std::string(issue.name) + ": unexpected F3 data");
    sim.check(sim.dut->f3_out_exc_o == issue.expected_exc,
              std::string(issue.name) + ": unexpected F3 exception flags");
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 5000;

    set_defaults(sim.dut.get());
    sim.reset();

    const Issue issues[] = {
        {0, kCmdSll,         kDtypeInt, false, 0x00000003u, 0x00000004u, 0,          0,      0,          0,          0,          0x00000030u, 0x00, "sll"},
        {0, kCmdSlli,        kDtypeInt, false, 0x00000001u, 0,          0,          4,      0,          0,          0,          0x00000010u, 0x00, "slli"},
        {0, kCmdSrai,        kDtypeInt, false, 0x80000000u, 0,          0,          1,      0,          0,          0,          0xc0000000u, 0x00, "srai"},
        {0, kCmdFeq,         kDtypeInt, false, 0x12345678u, 0x12345678u,0,          0,      0,          0,          0,          0xffffffffu, 0x00, "feq"},
        {0, kCmdFle,         kDtypeInt, false, 0xfffffffeu, 0xffffffffu,0,          0,      0,          0,          0,          0xffffffffu, 0x00, "fle"},
        {0, kCmdFltu,        kDtypeInt, false, 0x00000001u, 0x00000002u,0,          0,      0,          0,          0,          0xffffffffu, 0x00, "fltu"},
        {0, kCmdFbci,        kDtypeInt, false, 0,          0,          0,          0x80001, 0,          0,          0,          sign_extend(0x80001u, 20), 0x00, "fbci-int"},
        {0, kCmdFbci,        kDtypeF32, false, 0,          0,          0,          0xabcde, 0,          0,          0,          fbci_ps_expected(0xabcdeu), 0x00, "fbci-f32"},
        {0, kCmdMaxu,        kDtypeInt, false, 0x00000001u, 0xffffffffu,0,          0,      0,          0,          0,          0xffffffffu, 0x00, "maxu"},
        {0, kCmdAndi,        kDtypeInt, false, 0x12345679u, 0,          0,          0x001,  0,          0,          0,          0x00000001u, 0x00, "andi"},
        {0, kCmdNot,         kDtypeInt, false, 0x00ff00ffu, 0,          0,          0,      0,          0,          0,          0xff00ff00u, 0x00, "not"},
        {0, kCmdFcmov,       kDtypeInt, false, 0x00000000u, 0x22222222u,0x33333333u,0,      0,          0,          0,          0x33333333u, 0x00, "fcmov"},
        {0, kCmdFcmovm,      kDtypeInt, true,  0xaaaaaaaau, 0x11111111u,0x44444444u,0,      0,          0,          0,          0x44444444u, 0x00, "fcmovm"},
        {0, kCmdFswizz,      kDtypeInt, false, 0x01020304u, 0,          0,          1,      0x11111111u,0x22222222u,0x33333333u,0x11111111u, 0x00, "fswizz"},
        {0, kCmdPackrepb,    kDtypeInt, false, 0xaaaaaaaau, 0,          0,          0,      0x5555aaaau,0x22222222u,0x33333333u,0x5555aaaau, 0x00, "packrepb"},
        {0, kCmdPackreph,    kDtypeInt, false, 0xaaaaaaaau, 0,          0,          0,      0xfacecafeu,0x22222222u,0x33333333u,0xfacecafeu, 0x00, "packreph"},
        {0, kCmdFsetm,       kDtypeInt, false, 0x00000001u, 0,          0,          0,      0,          0,          0,          0x00000001u, 0x00, "fsetm"},
        {0, kCmdCubeface,    kDtypeInt, false, 0x00000000u, 0x00000000u,0x00000001u,0,      0,          0,          0,          0x00000001u, 0x00, "cubeface"},
        {0, kCmdCubefaceIdx, kDtypeInt, false, 0x00000003u, 0x00000000u,0,          0,      0,          0,          0,          0x7fc00000u, static_cast<uint8_t>(1u << kFlagInvalid), "cubeface-idx-invalid"},
        {0, kCmdCubesgnsc,   kDtypeInt, false, 0x00000005u, 0x01234567u,0,          0,      0,          0,          0,          0x81234567u, 0x00, "cubesgnsc"},
        {0, kCmdCubesgntc,   kDtypeInt, false, 0x00000002u, 0x01234567u,0,          0,      0,          0,          0,          0x01234567u, 0x00, "cubesgntc"},
        {0, kCmdMvsFx,       kDtypeInt, false, 0xcafef00du, 0,          0,          0,      0,          0,          0,          0xcafef00du, 0x00, "mvs-fx"},
        {0, kCmdTransLogFma1,kDtypeInt, false, 0x11111111u, 0x22222222u,0x33333333u,0,      0,          0,          0,          0x33333333u, 0x00, "mvxf-from-trans"},
        {0, kCmdFsgnjx,      kDtypeInt, false, 0x80000011u, 0x80000022u,0,          0,      0,          0,          0,          0x00000011u, 0x00, "fsgnjx"},
        {0, kCmdFsat8,       kDtypeInt, false, 0x00000180u, 0,          0,          0,      0,          0,          0,          0x0000007fu, 0x00, "fsat8"},
        {0, kCmdFsatu8,      kDtypeInt, false, 0xffffff80u, 0,          0,          0,      0,          0,          0,          0x00000000u, 0x00, "fsatu8"},
        {0, kCmdAdd,         kDtypeInt, false, 0x00000005u, 0,          0x00000007u,0,      0,          0,          0,          0x0000000cu, 0x00, "add"},
        {0, kCmdAddi,        kDtypeInt, false, 0x00000005u, 0,          0,          7,      0,          0,          0,          0x0000000cu, 0x00, "addi"},
        {0, kCmdSub,         kDtypeInt, false, 0x0000000au, 0,          0x00000003u,0,      0,          0,          0,          0x00000007u, 0x00, "sub"},
    };

    for (const Issue& issue : issues) {
        issue_and_check(sim, issue);
    }

    return sim.finish();
}
