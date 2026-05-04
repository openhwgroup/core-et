// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtrans_top_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using DUT = Vtrans_top_tb;

namespace {

constexpr uint64_t kTaylorBit = 1ULL << 49;
constexpr uint64_t kExcBit = 1ULL << 57;
constexpr uint64_t kResponseMask = (1ULL << 58) - 1ULL;
constexpr uint32_t kSinP1TaylorC0 = 0xc90fdb00U;

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

constexpr uint8_t kRomNone = 0;
constexpr uint8_t kRomRcp1 = 1;
constexpr uint8_t kRomRcp2 = 2;
constexpr uint8_t kRomRsqrt1 = 3;
constexpr uint8_t kRomRsqrt2 = 4;
constexpr uint8_t kRomLog1 = 5;
constexpr uint8_t kRomLog2 = 6;
constexpr uint8_t kRomExp1 = 7;
constexpr uint8_t kRomExp2 = 8;
constexpr uint8_t kRomSin1 = 9;
constexpr uint8_t kRomSin2 = 10;
constexpr uint8_t kRomSin3 = 11;

struct Inputs {
    bool ex_valid = false;
    uint32_t ex_cmd = 0;
    uint32_t ex_in2 = 0;
    uint8_t ex_txfma_exc = 0;
};

struct RomCtrl {
    uint8_t cmd = kRomNone;
    uint8_t index = 0;
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

std::string hex_u64(uint64_t value) {
    std::ostringstream oss;
    oss << "0x" << std::hex << value;
    return oss.str();
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
        if (i >= table.size()) {
            std::fprintf(stderr, "ROM table %s exceeds the supported 8-bit index space\n", path.c_str());
            std::exit(1);
        }
        table[i++] = std::stoull(line, nullptr, 16) & kResponseMask;
    }
    return table;
}

uint64_t set_taylor(uint64_t value, bool taylor) {
    value &= ~kTaylorBit;
    if (taylor) value |= kTaylorBit;
    return value & kResponseMask;
}

uint64_t set_exc(uint64_t value, bool exc) {
    value &= ~kExcBit;
    if (exc) value |= kExcBit;
    return value & kResponseMask;
}

uint64_t packed_c0_only(uint32_t c0) {
    return static_cast<uint64_t>(c0) & kResponseMask;
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

struct Model {
    bool enable_extra_trans;

    std::array<uint64_t, 256> rcp1 = load_hex_table("../data/trans_rom/trans_rcp_rom_case_c1c2.packed.hex");
    std::array<uint64_t, 256> rcp2 = load_hex_table("../data/trans_rom/trans_rcp_rom_case_c0.packed.hex");
    std::array<uint64_t, 256> rsqrt1 = load_hex_table("../data/trans_rom/trans_rsqrt_rom_case_c1c2.packed.hex");
    std::array<uint64_t, 256> rsqrt2 = load_hex_table("../data/trans_rom/trans_rsqrt_rom_case_c0.packed.hex");
    std::array<uint64_t, 256> log1 = load_hex_table("../data/trans_rom/trans_log_rom_case_c1c2.packed.hex");
    std::array<uint64_t, 256> log2 = load_hex_table("../data/trans_rom/trans_log_rom_case_c0.packed.hex");
    std::array<uint64_t, 256> exp1 = load_hex_table("../data/trans_rom/trans_exp_rom_case_c1c2.packed.hex");
    std::array<uint64_t, 256> exp2 = load_hex_table("../data/trans_rom/trans_exp_rom_case_c0.packed.hex");
    std::array<uint64_t, 256> sin1 = load_hex_table("../data/trans_rom/trans_sin_rom_case_c1c2.packed.hex");
    std::array<uint64_t, 256> sin2 = load_hex_table("../data/trans_rom/trans_sin_rom_case_c0.packed.hex");

    bool f2_trans_valid = false;
    bool f3_trans_valid = false;
    bool f4_trans_valid = false;
    bool f5_trans_valid = false;
    bool f6_trans_valid = false;
    bool f7_trans_valid = false;

    uint32_t f2_rom_raddr = 0;
    uint32_t f2_rom_cmd = 0;
    bool f2_exc = false;
    bool f3_exc = false;
    bool f4_exc = false;
    bool f5_exc = false;
    bool f6_exc = false;
    bool f7_exc = false;
    bool f8_exc = false;

    bool f3_sin_taylor = false;
    bool f4_sin_taylor = false;
    bool f5_sin_taylor = false;

    RomCtrl f3_rom_ctrl;
    RomCtrl f4_rom_ctrl;
    RomCtrl f5_rom_ctrl;

    uint64_t f5_rom_response_l = 0;
    uint64_t f6_rom_response = 0;
    uint64_t f7_rom_response = 0;
    uint64_t f8_rom_response = 0;

    explicit Model(bool enable_extra_trans_) : enable_extra_trans(enable_extra_trans_) {}

    static uint8_t sin_addr(uint32_t raddr) {
        switch ((raddr >> 23) & 0xffu) {
            case 124: return static_cast<uint8_t>(0x20u | ((raddr >> 18) & 0x1fu));
            case 123: return static_cast<uint8_t>(0x10u | ((raddr >> 19) & 0x0fu));
            case 122: return static_cast<uint8_t>(0x08u | ((raddr >> 20) & 0x07u));
            case 121: return static_cast<uint8_t>(0x02u | ((raddr >> 22) & 0x01u));
            default: return 0;
        }
    }

    static bool sin_taylor(uint32_t raddr) {
        return ((raddr >> 23) & 0xffu) < 120u;
    }

    RomCtrl decode_ctrl(uint32_t cmd, uint32_t raddr) const {
        RomCtrl ctrl;
        switch (cmd) {
            case kCmdTransRcpRr:
                ctrl.cmd = kRomRcp1;
                ctrl.index = static_cast<uint8_t>((raddr >> 16) & 0x7fu);
                break;
            case kCmdTransRcpFma1:
                ctrl.cmd = kRomRcp2;
                ctrl.index = static_cast<uint8_t>((raddr >> 16) & 0x7fu);
                break;
            case kCmdTransRsqrtRr:
                if (enable_extra_trans) {
                    ctrl.cmd = kRomRsqrt1;
                    ctrl.index = static_cast<uint8_t>((raddr >> 16) & 0xffu);
                }
                break;
            case kCmdTransRsqrtFma1:
                if (enable_extra_trans) {
                    ctrl.cmd = kRomRsqrt2;
                    ctrl.index = static_cast<uint8_t>((raddr >> 16) & 0xffu);
                }
                break;
            case kCmdTransLogRr:
                ctrl.cmd = kRomLog1;
                ctrl.index = static_cast<uint8_t>((raddr >> 17) & 0x3fu);
                break;
            case kCmdTransLogFma1:
                ctrl.cmd = kRomLog2;
                ctrl.index = static_cast<uint8_t>((raddr >> 17) & 0x3fu);
                break;
            case kCmdTransExpRr:
                ctrl.cmd = kRomExp1;
                ctrl.index = static_cast<uint8_t>((raddr >> 18) & 0x3fu);
                break;
            case kCmdTransExpFma1:
                ctrl.cmd = kRomExp2;
                ctrl.index = static_cast<uint8_t>((raddr >> 18) & 0x3fu);
                break;
            case kCmdTransSinRr:
                if (enable_extra_trans) {
                    ctrl.cmd = kRomSin1;
                    ctrl.index = sin_addr(raddr);
                }
                break;
            case kCmdTransSinP1:
                if (enable_extra_trans) {
                    ctrl.cmd = kRomSin2;
                    ctrl.index = sin_addr(raddr);
                }
                break;
            case kCmdTransSinP2:
                if (enable_extra_trans) {
                    ctrl.cmd = kRomSin3;
                }
                break;
            default:
                break;
        }
        return ctrl;
    }

    uint64_t rom_lookup(uint8_t cmd, uint8_t index) const {
        switch (cmd) {
            case kRomRcp1: return rcp1[index];
            case kRomRcp2: return rcp2[index];
            case kRomRsqrt1: return rsqrt1[index];
            case kRomRsqrt2: return rsqrt2[index];
            case kRomLog1: return log1[index];
            case kRomLog2: return log2[index];
            case kRomExp1: return exp1[index];
            case kRomExp2: return exp2[index];
            case kRomSin1: return sin1[index];
            case kRomSin2: return sin2[index];
            default: return 0;
        }
    }

    uint64_t evaluate_f5_response() const {
        switch (f5_rom_ctrl.cmd) {
            case kRomRcp1:
            case kRomRcp2:
            case kRomRsqrt1:
            case kRomRsqrt2:
            case kRomLog1:
            case kRomLog2:
            case kRomExp1:
                return rom_lookup(f5_rom_ctrl.cmd, f5_rom_ctrl.index);
            case kRomExp2:
                return set_exc(rom_lookup(kRomExp2, f5_rom_ctrl.index), f5_exc);
            case kRomSin1:
                return set_taylor(rom_lookup(kRomSin1, f5_rom_ctrl.index), f5_sin_taylor);
            case kRomSin2:
                if (f5_sin_taylor) {
                    return set_taylor(packed_c0_only(kSinP1TaylorC0), true);
                }
                return set_taylor(rom_lookup(kRomSin2, f5_rom_ctrl.index), false);
            case kRomSin3:
                return set_taylor(f5_rom_response_l, f5_sin_taylor);
            default:
                return 0;
        }
    }

    void step(const Inputs& in) {
        const uint64_t f5_response_now = evaluate_f5_response();
        const bool ex_exc_int = (in.ex_cmd == kCmdTransExpRr) ? (in.ex_txfma_exc != 0) : f8_exc;
        const bool next_f2_sin_taylor = enable_extra_trans && sin_taylor(f2_rom_raddr);
        const RomCtrl next_f2_rom_ctrl = decode_ctrl(f2_rom_cmd, f2_rom_raddr);

        const bool cur_f2_trans_valid = f2_trans_valid;
        const bool cur_f3_trans_valid = f3_trans_valid;
        const bool cur_f4_trans_valid = f4_trans_valid;
        const bool cur_f5_trans_valid = f5_trans_valid;
        const bool cur_f6_trans_valid = f6_trans_valid;
        const bool cur_f7_trans_valid = f7_trans_valid;
        const bool cur_f2_exc = f2_exc;
        const bool cur_f3_exc = f3_exc;
        const bool cur_f4_exc = f4_exc;
        const bool cur_f5_exc = f5_exc;
        const bool cur_f6_exc = f6_exc;
        const bool cur_f7_exc = f7_exc;
        const bool cur_f8_exc = f8_exc;
        const bool cur_f3_sin_taylor = f3_sin_taylor;
        const bool cur_f4_sin_taylor = f4_sin_taylor;
        const RomCtrl cur_f3_rom_ctrl = f3_rom_ctrl;
        const RomCtrl cur_f4_rom_ctrl = f4_rom_ctrl;
        const uint64_t cur_f6_rom_response = f6_rom_response;
        const uint64_t cur_f7_rom_response = f7_rom_response;

        f2_trans_valid = in.ex_valid;
        f3_trans_valid = cur_f2_trans_valid;
        f4_trans_valid = cur_f3_trans_valid;
        f5_trans_valid = cur_f4_trans_valid;
        f6_trans_valid = cur_f5_trans_valid;
        f7_trans_valid = cur_f6_trans_valid;

        if (in.ex_valid) {
            f2_rom_raddr = in.ex_in2;
            f2_rom_cmd = in.ex_cmd;
            f2_exc = ex_exc_int;
        }

        if (cur_f2_trans_valid) {
            f3_rom_ctrl = next_f2_rom_ctrl;
            f3_exc = cur_f2_exc;
            f3_sin_taylor = next_f2_sin_taylor;
        }
        if (cur_f3_trans_valid) {
            f4_rom_ctrl = cur_f3_rom_ctrl;
            f4_exc = cur_f3_exc;
            f4_sin_taylor = cur_f3_sin_taylor;
        }
        if (cur_f4_trans_valid) {
            f5_rom_ctrl = cur_f4_rom_ctrl;
            f5_exc = cur_f4_exc;
            f5_sin_taylor = cur_f4_sin_taylor;
        }
        if (cur_f5_trans_valid) {
            f6_rom_response = f5_response_now;
            f6_exc = cur_f5_exc;
        }
        if (cur_f6_trans_valid) {
            f7_rom_response = cur_f6_rom_response;
            f7_exc = cur_f6_exc;
        }
        if (cur_f7_trans_valid) {
            f8_rom_response = cur_f7_rom_response;
            f8_exc = cur_f7_exc;
        }

        f5_rom_response_l = f5_response_now;
        if (!cur_f7_trans_valid) {
            f8_exc = cur_f8_exc;
        }
    }
};

void drive_inputs(SimCtrl<DUT>& sim, const Inputs& in) {
    sim.dut->ex_valid_i = in.ex_valid;
    sim.dut->ex_cmd_i = in.ex_cmd;
    sim.dut->ex_in2_i = in.ex_in2;
    sim.dut->ex_txfma_exc_i = in.ex_txfma_exc;
}

void run_cycle(SimCtrl<DUT>& sim, Model& model, const Inputs& in, const std::string& label) {
    drive_inputs(sim, in);
    sim.tick();
    model.step(in);
    const uint64_t dut_out = static_cast<uint64_t>(sim.dut->f8_rom_response_o) & kResponseMask;
    sim.check(dut_out == model.f8_rom_response,
              label + ": dut=" + hex_u64(dut_out) + " model=" + hex_u64(model.f8_rom_response));
}

void run_idle(SimCtrl<DUT>& sim, Model& model, int cycles, const std::string& label) {
    for (int i = 0; i < cycles; ++i) run_cycle(sim, model, Inputs{}, label);
}

uint64_t launch_and_flush(SimCtrl<DUT>& sim, Model& model, const Inputs& in, const std::string& label) {
    run_cycle(sim, model, in, label + " launch");
    run_idle(sim, model, 7, label + " flush");
    return static_cast<uint64_t>(sim.dut->f8_rom_response_o) & kResponseMask;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    drive_inputs(sim, Inputs{});
    sim.reset();

    const bool enable_extra_trans = sim.dut->enable_extra_trans_o;
    Model model(enable_extra_trans);
    run_idle(sim, model, 2, "post-reset idle");

    const uint64_t rcp1_expected = model.rcp1[0x12];
    sim.check(launch_and_flush(sim, model,
                               Inputs{true, kCmdTransRcpRr, make_rcp_addr(0x12), 0},
                               "rcp rr") == rcp1_expected,
              "RCP_RR should return ROM c1/c2 coefficients");

    const uint64_t rcp2_expected = model.rcp2[0x31];
    sim.check(launch_and_flush(sim, model,
                               Inputs{true, kCmdTransRcpFma1, make_rcp_addr(0x31), 0},
                               "rcp fma1") == rcp2_expected,
              "RCP_FMA1 should return ROM c0 coefficients");

    const uint64_t log1_expected = model.log1[0x1a];
    sim.check(launch_and_flush(sim, model,
                               Inputs{true, kCmdTransLogRr, make_log_addr(0x1a), 0},
                               "log rr") == log1_expected,
              "LOG_RR should return ROM c1/c2 coefficients");

    const uint64_t log2_expected = model.log2[0x05];
    sim.check(launch_and_flush(sim, model,
                               Inputs{true, kCmdTransLogFma1, make_log_addr(0x05), 0},
                               "log fma1") == log2_expected,
              "LOG_FMA1 should return ROM c0 coefficients");

    const uint64_t exp1_expected = model.exp1[0x07];
    sim.check(launch_and_flush(sim, model,
                               Inputs{true, kCmdTransExpRr, make_exp_addr(0x07), 0x15},
                               "exp rr no carry yet") == exp1_expected,
              "EXP_RR should return ROM coefficients without overriding exc");

    const uint64_t exp1_seeded = launch_and_flush(sim, model,
                                                  Inputs{true, kCmdTransExpRr, make_exp_addr(0x03), 0x01},
                                                  "exp rr seed exc");
    sim.check(exp1_seeded == model.exp1[0x03], "EXP_RR seeded path should still use exp1 ROM");

    const uint64_t exp2_expected = set_exc(model.exp2[0x09], true);
    sim.check(launch_and_flush(sim, model,
                               Inputs{true, kCmdTransExpFma1, make_exp_addr(0x09), 0x00},
                               "exp fma1 carry exc") == exp2_expected,
              "EXP_FMA1 should copy the carried exception bit into the packed ROM response");

    const uint64_t unsupported_expected = 0;
    sim.check(launch_and_flush(sim, model,
                               Inputs{true, 0, 0, 0},
                               "zero command") == unsupported_expected,
              "A zero-valued command should take the default zero-output path");
    sim.check(launch_and_flush(sim, model,
                               Inputs{true, kCmdTransRcpFma2, make_rcp_addr(0x22), 0},
                               "unsupported trans op") == unsupported_expected,
              "Unsupported trans commands should produce zero output");

    if (!enable_extra_trans) {
        sim.check(launch_and_flush(sim, model,
                                   Inputs{true, kCmdTransRsqrtRr, make_rsqrt_addr(0x44), 0},
                                   "rsqrt disabled") == 0,
                  "RSQRT should be disabled when EnableExtraTrans=0");
        sim.check(launch_and_flush(sim, model,
                                   Inputs{true, kCmdTransSinRr, make_sin_non_taylor_addr(0x23), 0},
                                   "sin rr disabled") == 0,
                  "SIN_RR should be disabled when EnableExtraTrans=0");
        sim.check(launch_and_flush(sim, model,
                                   Inputs{true, kCmdTransSinP1, make_sin_taylor_addr(0x1555), 0},
                                   "sin p1 disabled") == 0,
                  "SIN_P1 should be disabled when EnableExtraTrans=0");
        sim.check(launch_and_flush(sim, model,
                                   Inputs{true, kCmdTransSinP2, make_sin_non_taylor_addr(0x3d), 0},
                                   "sin p2 disabled") == 0,
                  "SIN_P2 should be disabled when EnableExtraTrans=0");
    } else {
        const uint64_t rsqrt1_expected = model.rsqrt1[0x4d];
        sim.check(launch_and_flush(sim, model,
                                   Inputs{true, kCmdTransRsqrtRr, make_rsqrt_addr(0x4d), 0},
                                   "rsqrt rr") == rsqrt1_expected,
                  "RSQRT_RR should use the RSQRT c1/c2 ROM");

        const uint64_t rsqrt2_expected = model.rsqrt2[0xa2];
        sim.check(launch_and_flush(sim, model,
                                   Inputs{true, kCmdTransRsqrtFma1, make_rsqrt_addr(0xa2), 0},
                                   "rsqrt fma1") == rsqrt2_expected,
                  "RSQRT_FMA1 should use the RSQRT c0 ROM");

        const uint64_t sin1_expected = set_taylor(model.sin1[0x2a], false);
        sim.check(launch_and_flush(sim, model,
                                   Inputs{true, kCmdTransSinRr, make_sin_non_taylor_addr(0x2a), 0},
                                   "sin rr non-taylor") == sin1_expected,
                  "SIN_RR should use the sine c1/c2 ROM and clear taylor outside the Taylor range");

        const uint64_t sinp1_expected = set_taylor(model.sin2[0x33], false);
        sim.check(launch_and_flush(sim, model,
                                   Inputs{true, kCmdTransSinP1, make_sin_non_taylor_addr(0x33), 0},
                                   "sin p1 non-taylor") == sinp1_expected,
                  "SIN_P1 should use the sine c0 ROM outside the Taylor range");

        const uint64_t sinp1_taylor_expected = set_taylor(packed_c0_only(kSinP1TaylorC0), true);
        sim.check(launch_and_flush(sim, model,
                                   Inputs{true, kCmdTransSinP1, make_sin_taylor_addr(0x004321), 0},
                                   "sin p1 taylor") == sinp1_taylor_expected,
                  "SIN_P1 Taylor mode should override c0 with the hardcoded constant");

        run_cycle(sim, model, Inputs{true, kCmdTransSinP1, make_sin_non_taylor_addr(0x24), 0},
                  "sin p1 before sin p2");
        run_cycle(sim, model, Inputs{true, kCmdTransSinP2, make_sin_taylor_addr(0x012345), 0},
                  "sin p2 latch carry");
        run_idle(sim, model, 7, "sin p2 drain");
        const uint64_t sinp2_expected = set_taylor(model.sin2[0x24], true);
        sim.check((static_cast<uint64_t>(sim.dut->f8_rom_response_o) & kResponseMask) == sinp2_expected,
                  "SIN_P2 should keep the previous SIN_P1 coefficients and only update the Taylor bit");

        sim.check(launch_and_flush(sim, model,
                                   Inputs{true, kCmdTransSinP3, make_sin_non_taylor_addr(0x2f), 0},
                                   "sin p3 unsupported") == 0,
                  "SIN_P3 should not select a ROM entry in trans_top");
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

    uint32_t seed = enable_extra_trans ? 0x5e3adf19u : 0x31cace51u;
    for (int i = 0; i < 8192; ++i) {
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

        run_cycle(sim, model, in, "random cycle");
    }

    run_idle(sim, model, 8, "random drain");

    sim.check(saw_ex_valid_0 && saw_ex_valid_1, "ex_valid exercised both values");
    sim.check((ex_cmd_ones & 0x7fu) == 0x7fu && (ex_cmd_zeros & 0x7fu) == 0x7fu, "each ex_cmd bit exercised both values");
    sim.check(ex_in2_ones == 0xffffffffu && ex_in2_zeros == 0xffffffffu, "each ex_in2 bit exercised both values");
    sim.check((ex_exc_ones & 0x3fu) == 0x3fu && (ex_exc_zeros & 0x3fu) == 0x3fu, "each ex_txfma_exc bit exercised both values");
    for (size_t idx = 0; idx < cmd_pool.size(); ++idx) {
        sim.check(saw_cmd[idx], "random stimulus should exercise every tracked trans command");
    }

    return sim.finish();
}
