// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_txfma_trans_top_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <string>

#include "svdpi.h"

using DUT = Vvpu_txfma_trans_top_tb;

namespace {

constexpr uint64_t kTaylorBit = 1ULL << 49;
constexpr uint64_t kExcBit = 1ULL << 57;
constexpr uint64_t kResponseMask = (1ULL << 58) - 1ULL;

constexpr uint32_t kCmdAddLike = 0x00U;
constexpr uint32_t kCmdFmaLike = 0x07U;
constexpr uint32_t kCmdTransExpRr = 0b1111011U;
constexpr uint32_t kCmdTransSinRr = 0b1110110U;

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

std::array<uint64_t, 256> load_hex_table(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        std::fprintf(stderr, "failed to open ROM table %s\n", path.c_str());
        std::exit(1);
    }

    std::array<uint64_t, 256> table{};
    std::string line;
    size_t i = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        table[i++] = std::stoull(line, nullptr, 16) & kResponseMask;
    }
    return table;
}

uint64_t set_exc(uint64_t value, bool exc) {
    value &= ~kExcBit;
    if (exc) value |= kExcBit;
    return value & kResponseMask;
}

uint32_t make_rcp_addr(uint8_t index) {
    return static_cast<uint32_t>(index & 0x7fu) << 16;
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

void drive_inputs(DUT* dut, const Inputs& in) {
    dut->ex_rom_valid_i = in.ex_rom_valid;
    dut->ex_txfma_valid_i = in.ex_txfma_valid;
    dut->use_prev_sigs_i = in.use_prev_sigs;
    dut->sigs_i = in.sigs_bits;
    dut->cmd_i = in.cmd;
    dut->dtype_i = in.dtype;
    dut->imm_i = in.imm;
    dut->rm_i = in.rm;
    dut->typ_i = in.typ;
    dut->in1_i = in.in1;
    dut->in2_i = in.in2;
    dut->in3_i = in.in3;
}

void tick_with(SimCtrl<DUT>& sim, const Inputs& in) {
    drive_inputs(sim.dut.get(), in);
    sim.tick();
}

void run_idle(SimCtrl<DUT>& sim, int cycles) {
    for (int i = 0; i < cycles; ++i) tick_with(sim, Inputs{});
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
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 4000;

    drive_inputs(sim.dut.get(), Inputs{});
    sim.reset();
    sim.check(sim.dut->use_fake_txfma_o == 1, "testbench runs the fake-TXFMA configuration");

    // 1. Fake TXFMA request should appear after seven pipeline stages.
    uint8_t add_flags = 0;
    const uint32_t add_expected = txfma_emu_model(
        0x01020304u, 0x11121314u, 0x21222324u, 0x00, 0x1, 0x2, &add_flags);

    tick_with(sim, Inputs{
        .ex_txfma_valid = true,
        .cmd = kCmdAddLike,
        .dtype = 0x1,
        .rm = 0x2,
        .in1 = 0x01020304u,
        .in2 = 0x11121314u,
        .in3 = 0x21222324u,
    });
    run_idle(sim, 7);

    sim.check(sim.dut->f8_data_o == add_expected, "fake txfma add-like request appears after seven stages");
    sim.check(sim.dut->f8_exc_o == add_flags, "fake txfma flags follow the add-like request");
    sim.check(sim.dut->f8_comp_o == 0, "fake txfma compare output stays low");

    const uint32_t held_data_after_add = sim.dut->f8_data_o;
    const uint32_t held_exc_after_add = sim.dut->f8_exc_o;
    run_idle(sim, 2);
    sim.check(sim.dut->f8_data_o == held_data_after_add, "fake txfma data holds while idle");
    sim.check(sim.dut->f8_exc_o == held_exc_after_add, "fake txfma flags hold while idle");

    // 2. ROM-only requests should update the debug bits without disturbing the
    // held fake-TXFMA output. The visible debug output comes from the packed
    // ROM response itself, not from the later internal f8_exc feedback flop.
    const auto exp1 = load_hex_table("../data/trans_rom/trans_exp_rom_case_c1c2.packed.hex");
    const uint64_t exp_expected = exp1[0x12];
    const uint8_t exp_dbg_expected =
        static_cast<uint8_t>(((exp_expected >> 57) & 0x1u) << 1 |
                             ((exp_expected >> 49) & 0x1u));

    tick_with(sim, Inputs{
        .ex_rom_valid = true,
        .cmd = kCmdTransExpRr,
        .in2 = make_exp_addr(0x12),
    });
    run_idle(sim, 7);

    sim.check(sim.dut->txfma_trans_dbg_o == exp_dbg_expected,
              "ExpRr request drives debug bits from the packed ROM response");
    sim.check(sim.dut->f8_data_o == held_data_after_add, "ROM-only request does not disturb the held fake-TXFMA data");
    sim.check(sim.dut->f8_exc_o == held_exc_after_add, "ROM-only request does not disturb the held fake-TXFMA flags");

    // 3. Extra transcendental flow is disabled by default, so a sin request
    // should not assert the debug bits in the default configuration.
    tick_with(sim, Inputs{
        .ex_rom_valid = true,
        .cmd = kCmdTransSinRr,
        .in2 = make_sin_non_taylor_addr(0x23),
    });
    run_idle(sim, 7);

    sim.check(sim.dut->txfma_trans_dbg_o == 0, "sin request stays disabled when EnableExtraTrans is left at its default");

    // 4. A second fake TXFMA request should overwrite the held output later.
    uint8_t fma_flags = 0;
    const uint32_t fma_expected = txfma_emu_model(
        0x89abcdefu, 0x10203040u, 0x55667788u, 0x07, 0xe, 0x5, &fma_flags);

    tick_with(sim, Inputs{
        .ex_txfma_valid = true,
        .cmd = kCmdFmaLike,
        .dtype = 0xe,
        .rm = 0x5,
        .in1 = 0x89abcdefu,
        .in2 = 0x10203040u,
        .in3 = 0x55667788u,
    });
    run_idle(sim, 7);

    sim.check(sim.dut->f8_data_o == fma_expected, "second fake txfma request overwrites the held data");
    sim.check(sim.dut->f8_exc_o == fma_flags, "second fake txfma request overwrites the held flags");
    sim.check(sim.dut->f8_comp_o == 0, "fake txfma compare output remains low after the second request");

    return sim.finish();
}
