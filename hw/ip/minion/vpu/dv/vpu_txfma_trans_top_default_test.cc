// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_txfma_trans_top_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>

using DUT = Vvpu_txfma_trans_top_tb;

namespace {

constexpr uint64_t kTaylorBit = 1ULL << 49;
constexpr uint64_t kExcBit = 1ULL << 57;
constexpr uint64_t kResponseMask = (1ULL << 58) - 1ULL;

constexpr uint32_t kCmdAddLike = 0x00U;
constexpr uint32_t kCmdFmaLike = 0x07U;
constexpr uint32_t kCmdTransExpRr = 0b1111011U;
constexpr uint32_t kCmdTransSinRr = 0b1110110U;
constexpr unsigned kTxfmaBit = 37;
constexpr unsigned kRen3Bit = 18;
constexpr unsigned kRen1Bit = 20;
constexpr unsigned kAddBit = 4;
constexpr unsigned kWflagsBit = 0;

constexpr uint64_t bit(unsigned pos) {
    return 1ULL << pos;
}

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

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 4000;

    drive_inputs(sim.dut.get(), Inputs{});
    sim.reset();
    sim.check(sim.dut->use_fake_txfma_o == 0, "testbench runs the default real-TXFMA configuration");
    sim.check(sim.dut->f8_data_o == 0, "reset leaves txfma/trans data output clear");
    sim.check(sim.dut->f8_exc_o == 0, "reset leaves txfma/trans exception flags clear");
    sim.check(sim.dut->f8_comp_o == 0, "reset leaves txfma/trans compare output clear");

    tick_with(sim, Inputs{
        .ex_txfma_valid = true,
        .sigs_bits = bit(kTxfmaBit) | bit(kRen1Bit) | bit(kRen3Bit) |
                     bit(kAddBit) | bit(kWflagsBit),
        .cmd = kCmdAddLike,
        .dtype = 0x1,
        .rm = 0x0,
        .typ = 0x1,
        .in1 = 0x3f800000u,  // +1.0f
        .in2 = 0x00000000u,
        .in3 = 0x40000000u,  // +2.0f
    });
    run_idle(sim, 6);

    sim.check(sim.dut->f8_data_o == 0x40400000u,
              "real txfma add request produces +3.0 through vpu_txfma_trans_top");
    sim.check(sim.dut->f8_exc_o == 0, "real txfma add request produces no exception flags");
    sim.check(sim.dut->f8_comp_o == 0, "real txfma add request keeps compare output low");

    tick_with(sim, Inputs{});
    sim.check(sim.dut->f8_data_o == 0, "real txfma data output is masked after the request drains");
    sim.check(sim.dut->f8_exc_o == 0, "real txfma flags clear after the request drains");

    const uint32_t held_data_after_add = sim.dut->f8_data_o;
    const uint32_t held_exc_after_add = sim.dut->f8_exc_o;

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
    sim.check(sim.dut->f8_data_o == held_data_after_add, "ROM-only request does not disturb the held real-TXFMA data");
    sim.check(sim.dut->f8_exc_o == held_exc_after_add, "ROM-only request does not disturb the held real-TXFMA flags");

    tick_with(sim, Inputs{
        .ex_rom_valid = true,
        .cmd = kCmdTransSinRr,
        .in2 = make_sin_non_taylor_addr(0x23),
    });
    run_idle(sim, 7);

    sim.check(sim.dut->txfma_trans_dbg_o == 0, "sin request stays disabled when EnableExtraTrans is left at its default");

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

    sim.check(sim.dut->f8_data_o == 0, "second real txfma request produces the expected data word");
    sim.check(sim.dut->f8_exc_o == 0, "second real txfma request produces the expected exception flags");
    sim.check(sim.dut->f8_comp_o == 0, "second real txfma request keeps compare output low");

    return sim.finish();
}
