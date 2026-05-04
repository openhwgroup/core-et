// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_trans_top_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>

using DUT = Vcosim_trans_top_tb;

namespace {

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
    bool ex_valid = false;
    uint32_t ex_cmd = 0;
    uint32_t ex_in2 = 0;
    uint8_t ex_txfma_exc = 0;
};

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

uint32_t make_sin_taylor_addr(uint8_t low_bits = 0) {
    return (119u << 23) | (static_cast<uint32_t>(low_bits) & 0x7fffffu);
}

void drive_inputs(CosimCtrl<DUT>& sim, const Inputs& in) {
    sim.dut->ex_valid_i = in.ex_valid;
    sim.dut->ex_cmd_i = in.ex_cmd;
    sim.dut->ex_in2_i = in.ex_in2;
    sim.dut->ex_txfma_exc_i = in.ex_txfma_exc;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("f8_rom_response", sim.dut->orig_f8_rom_response_o, sim.dut->new_f8_rom_response_o);
}

void run_cycle(CosimCtrl<DUT>& sim, const Inputs& in) {
    drive_inputs(sim, in);
    sim.tick();
    compare_outputs(sim);
}

void run_idle(CosimCtrl<DUT>& sim, int cycles) {
    for (int i = 0; i < cycles; ++i) run_cycle(sim, Inputs{});
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    drive_inputs(sim, Inputs{});
    sim.reset();
    const bool enable_extra_trans = sim.dut->enable_extra_trans_o;

    run_idle(sim, 2);

    run_cycle(sim, Inputs{true, kCmdTransRcpRr, make_rcp_addr(0x12), 0x00});
    run_idle(sim, 7);

    run_cycle(sim, Inputs{true, kCmdTransRcpFma1, make_rcp_addr(0x31), 0x00});
    run_idle(sim, 7);

    run_cycle(sim, Inputs{true, kCmdTransLogRr, make_log_addr(0x1a), 0x00});
    run_idle(sim, 7);

    run_cycle(sim, Inputs{true, kCmdTransLogFma1, make_log_addr(0x05), 0x00});
    run_idle(sim, 7);

    run_cycle(sim, Inputs{true, kCmdTransExpRr, make_exp_addr(0x03), 0x01});
    run_idle(sim, 7);

    run_cycle(sim, Inputs{true, kCmdTransExpFma1, make_exp_addr(0x09), 0x00});
    run_idle(sim, 7);

    run_cycle(sim, Inputs{true, 0, 0, 0});
    run_idle(sim, 7);

    run_cycle(sim, Inputs{true, kCmdTransRcpFma2, make_rcp_addr(0x22), 0x00});
    run_idle(sim, 7);

    if (enable_extra_trans) {
        run_cycle(sim, Inputs{true, kCmdTransRsqrtRr, make_rsqrt_addr(0x4d), 0x00});
        run_idle(sim, 7);

        run_cycle(sim, Inputs{true, kCmdTransRsqrtFma1, make_rsqrt_addr(0xa2), 0x00});
        run_idle(sim, 7);

        run_cycle(sim, Inputs{true, kCmdTransSinRr, make_sin_non_taylor_addr(0x2a), 0x00});
        run_idle(sim, 7);

        run_cycle(sim, Inputs{true, kCmdTransSinP1, make_sin_non_taylor_addr(0x33), 0x00});
        run_idle(sim, 7);

        run_cycle(sim, Inputs{true, kCmdTransSinP1, make_sin_non_taylor_addr(0x24), 0x00});
        run_cycle(sim, Inputs{true, kCmdTransSinP2, make_sin_taylor_addr(0x35), 0x00});
        run_idle(sim, 7);

        run_cycle(sim, Inputs{true, kCmdTransSinP1, make_sin_taylor_addr(0x55), 0x00});
        run_idle(sim, 7);
    } else {
        run_cycle(sim, Inputs{true, kCmdTransRsqrtRr, make_rsqrt_addr(0x44), 0x00});
        run_idle(sim, 7);

        run_cycle(sim, Inputs{true, kCmdTransSinRr, make_sin_non_taylor_addr(0x23), 0x00});
        run_idle(sim, 7);
    }

    bool saw_ex_valid_0 = false;
    bool saw_ex_valid_1 = false;
    uint32_t ex_cmd_ones = 0;
    uint32_t ex_cmd_zeros = 0;
    uint32_t ex_in2_ones = 0;
    uint32_t ex_in2_zeros = 0;
    uint8_t ex_exc_ones = 0;
    uint8_t ex_exc_zeros = 0;
    std::array<bool, 21> saw_cmd{};
    const std::array<uint32_t, 21> cmd_pool = {{
        0, kCmdAddi, kCmdTransRcpRr, kCmdTransRcpFma1, kCmdTransRcpFma2,
        kCmdTransRsqrtRr, kCmdTransRsqrtFma1, kCmdTransRsqrtFma2,
        kCmdTransLogRr, kCmdTransLogFma1, kCmdTransLogFma2, kCmdTransLogMul,
        kCmdTransSinTrans, kCmdTransSinRr, kCmdTransSinP1, kCmdTransSinP2,
        kCmdTransSinP3, kCmdTransExpFrac, kCmdTransExpRr, kCmdTransExpFma1,
        kCmdTransExpFma2
    }};

    uint32_t seed = enable_extra_trans ? 0x7b6d19efu : 0x2f65c91bu;
    for (int i = 0; i < 65536; ++i) {
        Inputs in;
        in.ex_valid = (xorshift32(seed) & 3u) != 0u;
        in.ex_cmd = cmd_pool[xorshift32(seed) % cmd_pool.size()];
        in.ex_in2 = xorshift32(seed);
        in.ex_txfma_exc = static_cast<uint8_t>(xorshift32(seed) & 0x3fu);

        saw_ex_valid_0 |= !in.ex_valid;
        saw_ex_valid_1 |= in.ex_valid;
        ex_cmd_ones |= in.ex_cmd;
        ex_cmd_zeros |= ~in.ex_cmd;
        ex_in2_ones |= in.ex_in2;
        ex_in2_zeros |= ~in.ex_in2;
        ex_exc_ones |= in.ex_txfma_exc;
        ex_exc_zeros |= static_cast<uint8_t>(~in.ex_txfma_exc);
        if (in.ex_valid) {
            for (size_t idx = 0; idx < cmd_pool.size(); ++idx) {
                if (cmd_pool[idx] == in.ex_cmd) saw_cmd[idx] = true;
            }
        }

        run_cycle(sim, in);
    }

    run_idle(sim, 8);

    sim.check(saw_ex_valid_0 && saw_ex_valid_1, "ex_valid exercised both values");
    sim.check((ex_cmd_ones & 0x7fu) == 0x7fu && (ex_cmd_zeros & 0x7fu) == 0x7fu, "each ex_cmd bit exercised both values");
    sim.check(ex_in2_ones == 0xffffffffu && ex_in2_zeros == 0xffffffffu, "each ex_in2 bit exercised both values");
    sim.check((ex_exc_ones & 0x3fu) == 0x3fu && (ex_exc_zeros & 0x3fu) == 0x3fu, "each ex_txfma_exc bit exercised both values");
    for (size_t idx = 0; idx < cmd_pool.size(); ++idx) {
        sim.check(saw_cmd[idx], "random stimulus should exercise every tracked trans command");
    }
    sim.check(sim.comparisons >= 50000, "cosim should generate at least 50000 comparisons for trans_top");

    return sim.finish();
}
