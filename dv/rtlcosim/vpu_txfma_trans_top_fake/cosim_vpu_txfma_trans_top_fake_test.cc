// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_txfma_trans_top_fake_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <sstream>

#include "svdpi.h"

using DUT = Vcosim_vpu_txfma_trans_top_fake_tb;

namespace {

constexpr uint64_t kVpuCtrlSigsMask = (1ULL << 45) - 1ULL;
constexpr uint64_t kRequiredCtrlMask =
    (1ULL << 37) |  // txfma
    (1ULL << 35) |  // rom
    (1ULL << 33) |  // cvt
    (1ULL << 32) |  // trans
    (1ULL << 6)  |  // fma
    (1ULL << 5)  |  // fms
    (1ULL << 4)  |  // add
    (1ULL << 3)  |  // sub
    (1ULL << 2)  |  // mul
    (1ULL << 1)  |  // round
    (1ULL << 0);    // wflags

constexpr uint32_t kCmdAddLike = 0x00U;
constexpr uint32_t kCmdSubLike = 0x01U;
constexpr uint32_t kCmdMixLike = 0x02U;
constexpr uint32_t kCmdFmaLike = 0x07U;
constexpr uint32_t kCmdAddi = 0b1001110U;
constexpr uint32_t kCmdTransRcpRr = 0b1100010U;
constexpr uint32_t kCmdTransRcpFma1 = 0b1100011U;
constexpr uint32_t kCmdTransRcpFma2 = 0b1100100U;
constexpr uint32_t kCmdTransRsqrtRr = 0b1100101U;
constexpr uint32_t kCmdTransRsqrtFma1 = 0b1100110U;
constexpr uint32_t kCmdTransRsqrtFma2 = 0b1100111U;
constexpr uint32_t kCmdTransLogRr = 0b1110001U;
constexpr uint32_t kCmdTransLogFma1 = 0b1110010U;
constexpr uint32_t kCmdTransLogFma2 = 0b1110011U;
constexpr uint32_t kCmdTransLogMul = 0b1110100U;
constexpr uint32_t kCmdTransSinTrans = 0b1110101U;
constexpr uint32_t kCmdTransSinRr = 0b1110110U;
constexpr uint32_t kCmdTransSinP1 = 0b1110111U;
constexpr uint32_t kCmdTransSinP2 = 0b1111000U;
constexpr uint32_t kCmdTransSinP3 = 0b1111001U;
constexpr uint32_t kCmdTransExpFrac = 0b1111010U;
constexpr uint32_t kCmdTransExpRr = 0b1111011U;
constexpr uint32_t kCmdTransExpFma1 = 0b1111100U;
constexpr uint32_t kCmdTransExpFma2 = 0b1111101U;

struct Inputs {
    bool ex_rom_valid = false;
    bool ex_txfma_valid = false;
    bool use_prev_sigs = false;
    uint64_t sigs_bits = 0;
    uint32_t cmd = 0;
    uint32_t dtype = 0;
    uint32_t imm = 0;
    uint32_t rm = 0;
    uint32_t typ = 0;
    uint32_t in1 = 0;
    uint32_t in2 = 0;
    uint32_t in3 = 0;
};

uint32_t txfma_emu_model(uint32_t src1, uint32_t src2, uint32_t src3,
                         uint8_t cmd, uint8_t dtype, uint8_t rm,
                         uint8_t* out_flags) {
    uint32_t result = 0;
    switch (cmd) {
    case 0x00:
        result = src1 + src3;
        break;
    case 0x01:
        result = src1 - src3;
        break;
    case 0x02:
        result = src1 ^ ((src2 << 1) | (src2 >> 31));
        break;
    case 0x07:
        result = src1 + (src2 ^ 0x9e3779b9u) + src3;
        break;
    default: {
        const uint32_t rot = ((src2 << (rm & 31u)) | (src2 >> ((32u - rm) & 31u)));
        result = src1 ^ rot ^ (src3 + static_cast<uint32_t>(cmd)) ^
                 (static_cast<uint32_t>(dtype) << 28);
        break;
    }
    }

    const uint8_t flags = static_cast<uint8_t>(
        (cmd ^ dtype ^ rm ^
         static_cast<uint8_t>(src1) ^
         static_cast<uint8_t>(src2 >> 8) ^
         static_cast<uint8_t>(src3 >> 16) ^
         static_cast<uint8_t>(result >> 5)) &
        0x3fu);

    if (out_flags) {
        *out_flags = flags;
    }
    return result;
}

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint32_t make_rcp_addr(uint8_t index) {
    return static_cast<uint32_t>(index & 0x7fu) << 16;
}

uint32_t make_rsqrt_addr(uint8_t index) {
    return static_cast<uint32_t>(index) << 16;
}

uint32_t make_log_addr(uint8_t index) {
    return static_cast<uint32_t>(index & 0x3fu) << 17;
}

uint32_t make_exp_addr(uint8_t index) {
    return static_cast<uint32_t>(index & 0x3fu) << 18;
}

uint32_t make_sin_non_taylor_addr(uint8_t index) {
    if (index >= 32) return (124u << 23) | (static_cast<uint32_t>(index & 0x1fu) << 18);
    if (index >= 16) return (123u << 23) | (static_cast<uint32_t>(index & 0x0fu) << 19);
    if (index >= 8) return (122u << 23) | (static_cast<uint32_t>(index & 0x07u) << 20);
    return (121u << 23) | (static_cast<uint32_t>(index & 0x01u) << 22);
}

uint32_t make_sin_taylor_addr(uint32_t low_bits = 0) {
    return (119u << 23) | (static_cast<uint32_t>(low_bits) & 0x7fffffu);
}

void drive_inputs(CosimCtrl<DUT>& sim, const Inputs& in) {
    sim.dut->ex_rom_valid_i = in.ex_rom_valid;
    sim.dut->ex_txfma_valid_i = in.ex_txfma_valid;
    sim.dut->use_prev_sigs_i = in.use_prev_sigs;
    sim.dut->sigs_i = in.sigs_bits;
    sim.dut->cmd_i = in.cmd;
    sim.dut->dtype_i = in.dtype;
    sim.dut->imm_i = in.imm;
    sim.dut->rm_i = in.rm;
    sim.dut->typ_i = in.typ;
    sim.dut->in1_i = in.in1;
    sim.dut->in2_i = in.in2;
    sim.dut->in3_i = in.in3;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("f8_data", sim.dut->orig_f8_data_o, sim.dut->new_f8_data_o);
    sim.compare("f8_exc", sim.dut->orig_f8_exc_o, sim.dut->new_f8_exc_o);
    sim.compare("f8_comp", sim.dut->orig_f8_comp_o, sim.dut->new_f8_comp_o);
    sim.compare("txfma_trans_dbg", sim.dut->orig_txfma_trans_dbg_o, sim.dut->new_txfma_trans_dbg_o);
}

void run_cycle(CosimCtrl<DUT>& sim, const Inputs& in) {
    drive_inputs(sim, in);
    sim.tick();
    compare_outputs(sim);
}

}  // namespace

extern "C" int dpi_TXFMA_EMU(svBitVecVal* src1, svBitVecVal* src2, svBitVecVal* src3,
                             svBitVecVal* cmd, svBitVecVal* dtype, svBitVecVal* rm,
                             svBitVecVal* out_flags) {
    uint8_t flags = 0;
    const uint32_t result = txfma_emu_model(src1[0], src2[0], src3[0],
                                            static_cast<uint8_t>(cmd[0] & 0x7fu),
                                            static_cast<uint8_t>(dtype[0] & 0x0fu),
                                            static_cast<uint8_t>(rm[0] & 0x07u),
                                            &flags);
    out_flags[0] = flags;
    return static_cast<int32_t>(result);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    drive_inputs(sim, Inputs{});
    sim.reset();

    std::array<uint32_t, 25> cmd_pool = {{
        kCmdAddLike, kCmdSubLike, kCmdMixLike, kCmdFmaLike,
        kCmdAddi, kCmdTransRcpRr, kCmdTransRcpFma1, kCmdTransRcpFma2,
        kCmdTransRsqrtRr, kCmdTransRsqrtFma1, kCmdTransRsqrtFma2,
        kCmdTransLogRr, kCmdTransLogFma1, kCmdTransLogFma2, kCmdTransLogMul,
        kCmdTransSinTrans, kCmdTransSinRr, kCmdTransSinP1, kCmdTransSinP2,
        kCmdTransSinP3, kCmdTransExpFrac, kCmdTransExpRr, kCmdTransExpFma1,
        kCmdTransExpFma2, 0x55u
    }};

    for (size_t idx = 0; idx < cmd_pool.size(); ++idx) {
        Inputs in;
        in.ex_rom_valid = true;
        in.ex_txfma_valid = true;
        in.use_prev_sigs = (idx & 1u) != 0u;
        in.sigs_bits = (static_cast<uint64_t>(idx) << 13) ^ 0x1a5a5a5au;
        in.cmd = cmd_pool[idx];
        in.dtype = static_cast<uint32_t>(idx & 0x0fu);
        in.imm = 0x20000u ^ static_cast<uint32_t>(idx * 0x2222u);
        in.rm = static_cast<uint32_t>(idx & 0x07u);
        in.typ = static_cast<uint32_t>(idx & 0x03u);
        in.in1 = 0x10203040u ^ static_cast<uint32_t>(idx * 0x01010101u);
        in.in2 = ((idx & 1u) != 0u) ? make_log_addr(static_cast<uint8_t>(idx * 3u)) :
                                       make_exp_addr(static_cast<uint8_t>(idx * 5u));
        in.in3 = 0x55667788u ^ static_cast<uint32_t>(idx * 0x1020304u);
        run_cycle(sim, in);
    }

    for (int i = 0; i < 24; ++i) {
        run_cycle(sim, Inputs{});
    }

    bool saw_ex_rom_valid_0 = false;
    bool saw_ex_rom_valid_1 = false;
    bool saw_ex_txfma_valid_0 = false;
    bool saw_ex_txfma_valid_1 = false;
    bool saw_use_prev_0 = false;
    bool saw_use_prev_1 = false;
    uint32_t cmd_ones = 0;
    uint32_t cmd_zeros = 0;
    uint32_t dtype_ones = 0;
    uint32_t dtype_zeros = 0;
    uint32_t imm_ones = 0;
    uint32_t imm_zeros = 0;
    uint32_t rm_ones = 0;
    uint32_t rm_zeros = 0;
    uint32_t typ_ones = 0;
    uint32_t typ_zeros = 0;
    uint32_t in1_ones = 0;
    uint32_t in1_zeros = 0;
    uint64_t sigs_ones = 0;
    uint64_t sigs_zeros = 0;
    uint32_t in2_ones = 0;
    uint32_t in2_zeros = 0;
    uint32_t in3_ones = 0;
    uint32_t in3_zeros = 0;
    std::array<bool, 25> saw_cmd{};

    auto sample = [&](const Inputs& in) {
        saw_ex_rom_valid_0 |= !in.ex_rom_valid;
        saw_ex_rom_valid_1 |= in.ex_rom_valid;
        saw_ex_txfma_valid_0 |= !in.ex_txfma_valid;
        saw_ex_txfma_valid_1 |= in.ex_txfma_valid;
        saw_use_prev_0 |= !in.use_prev_sigs;
        saw_use_prev_1 |= in.use_prev_sigs;
        sigs_ones |= in.sigs_bits & kVpuCtrlSigsMask;
        sigs_zeros |= (~in.sigs_bits) & kVpuCtrlSigsMask;
        cmd_ones |= in.cmd;
        cmd_zeros |= ~in.cmd;
        dtype_ones |= in.dtype;
        dtype_zeros |= ~in.dtype;
        imm_ones |= in.imm;
        imm_zeros |= ~in.imm;
        rm_ones |= in.rm;
        rm_zeros |= ~in.rm;
        typ_ones |= in.typ;
        typ_zeros |= ~in.typ;
        in1_ones |= in.in1;
        in1_zeros |= ~in.in1;
        in2_ones |= in.in2;
        in2_zeros |= ~in.in2;
        in3_ones |= in.in3;
        in3_zeros |= ~in.in3;
        for (size_t idx = 0; idx < cmd_pool.size(); ++idx) {
            if (cmd_pool[idx] == in.cmd) saw_cmd[idx] = true;
        }
    };

    Inputs all_zero;
    sample(all_zero);
    run_cycle(sim, all_zero);

    Inputs all_one;
    all_one.ex_rom_valid = true;
    all_one.ex_txfma_valid = true;
    all_one.use_prev_sigs = true;
    all_one.sigs_bits = kVpuCtrlSigsMask;
    all_one.cmd = 0x7fu;
    all_one.dtype = 0x0fu;
    all_one.imm = 0x000fffffu;
    all_one.rm = 0x07u;
    all_one.typ = 0x03u;
    all_one.in1 = 0xffffffffu;
    all_one.in2 = 0xffffffffu;
    all_one.in3 = 0xffffffffu;
    sample(all_one);
    run_cycle(sim, all_one);

    uint32_t seed = 0x3518f4c2u;
    for (int i = 0; i < 65536; ++i) {
        Inputs in;
        in.ex_rom_valid = (xorshift32(seed) & 1u) != 0u;
        in.ex_txfma_valid = (xorshift32(seed) & 1u) != 0u;
        in.use_prev_sigs = (xorshift32(seed) & 1u) != 0u;
        in.sigs_bits = (static_cast<uint64_t>(xorshift32(seed)) << 7) ^ xorshift32(seed);
        in.cmd = cmd_pool[xorshift32(seed) % cmd_pool.size()];
        in.dtype = xorshift32(seed) & 0x0fu;
        in.imm = xorshift32(seed) & 0x000fffffu;
        in.rm = xorshift32(seed) & 0x07u;
        in.typ = xorshift32(seed) & 0x03u;
        in.in1 = xorshift32(seed);
        switch (xorshift32(seed) % 6u) {
            case 0: in.in2 = make_rcp_addr(static_cast<uint8_t>(xorshift32(seed))); break;
            case 1: in.in2 = make_rsqrt_addr(static_cast<uint8_t>(xorshift32(seed))); break;
            case 2: in.in2 = make_log_addr(static_cast<uint8_t>(xorshift32(seed) & 0x3fu)); break;
            case 3: in.in2 = make_exp_addr(static_cast<uint8_t>(xorshift32(seed) & 0x3fu)); break;
            case 4: in.in2 = make_sin_non_taylor_addr(static_cast<uint8_t>(xorshift32(seed) & 0x3fu)); break;
            default: in.in2 = make_sin_taylor_addr(xorshift32(seed) & 0x7fffffu); break;
        }
        in.in3 = xorshift32(seed);

        sample(in);
        run_cycle(sim, in);
    }

    for (int i = 0; i < 24; ++i) {
        run_cycle(sim, Inputs{});
    }

    sim.check(saw_ex_rom_valid_0 && saw_ex_rom_valid_1, "ex_rom_valid exercised both values");
    sim.check(saw_ex_txfma_valid_0 && saw_ex_txfma_valid_1, "ex_txfma_valid exercised both values");
    sim.check(saw_use_prev_0 && saw_use_prev_1, "use_prev_sigs exercised both values");
    sim.check(sigs_ones == kVpuCtrlSigsMask && sigs_zeros == kVpuCtrlSigsMask,
              "each packed vpu_ctrl_sigs bit exercised both values on the exposed input port");
    sim.check((sigs_ones & kRequiredCtrlMask) == kRequiredCtrlMask &&
                  (sigs_zeros & kRequiredCtrlMask) == kRequiredCtrlMask,
              "txfma/rom/cvt/trans/fma/fms/add/sub/mul/round/wflags exercised both values");
    sim.check((cmd_ones & 0x7fu) == 0x7fu && (cmd_zeros & 0x7fu) == 0x7fu, "each cmd bit exercised both values");
    sim.check((dtype_ones & 0x0fu) == 0x0fu && (dtype_zeros & 0x0fu) == 0x0fu, "each dtype bit exercised both values");
    sim.check((imm_ones & 0x000fffffu) == 0x000fffffu && (imm_zeros & 0x000fffffu) == 0x000fffffu, "each imm bit exercised both values");
    sim.check((rm_ones & 0x07u) == 0x07u && (rm_zeros & 0x07u) == 0x07u, "each rm bit exercised both values");
    sim.check((typ_ones & 0x03u) == 0x03u && (typ_zeros & 0x03u) == 0x03u, "each typ bit exercised both values");
    sim.check(in1_ones == 0xffffffffu && in1_zeros == 0xffffffffu, "each in1 bit exercised both values");
    sim.check(in2_ones == 0xffffffffu && in2_zeros == 0xffffffffu, "each in2 bit exercised both values");
    sim.check(in3_ones == 0xffffffffu && in3_zeros == 0xffffffffu, "each in3 bit exercised both values");
    for (size_t idx = 0; idx < cmd_pool.size(); ++idx) {
        std::ostringstream msg;
        msg << "stimulus should exercise command pool index " << idx;
        sim.check(saw_cmd[idx], msg.str().c_str());
    }
    sim.check(sim.comparisons >= 200000, "fake txfma/trans top cosim should generate at least 200000 comparisons");

    return sim.finish();
}
