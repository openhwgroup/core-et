// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtima_top_tb.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vtima_top_tb;

namespace {

struct Inputs {
    bool f1_mul_en = false;
    uint32_t f1_in1 = 0;
    uint32_t f1_in2 = 0;
    bool f1_in1_signed = false;
    bool f1_in2_signed = false;
    uint32_t f2_in3 = 0;
    bool f2_zap_opd3 = false;
    bool f2_tima_en = false;
    bool f2_in3_en = false;
};

struct Model {
    uint32_t f2_in1 = 0;
    uint32_t f2_in2 = 0;
    std::array<bool, 4> f2_in1_sign{};
    std::array<bool, 4> f2_in2_sign{};
    int32_t f3_prod_add = 0;
    uint32_t f3_in3 = 0;
    bool f3_zap_opd3 = false;

    static int32_t lane_operand(uint8_t byte, bool is_signed) {
        return is_signed ? static_cast<int32_t>(static_cast<int8_t>(byte))
                         : static_cast<int32_t>(byte);
    }

    static int32_t prod_add(uint32_t in1, uint32_t in2,
                            const std::array<bool, 4>& in1_sign,
                            const std::array<bool, 4>& in2_sign) {
        int32_t sum = 0;
        for (int lane = 0; lane < 4; ++lane) {
            const uint8_t byte1 = static_cast<uint8_t>((in1 >> (lane * 8)) & 0xffu);
            const uint8_t byte2 = static_cast<uint8_t>((in2 >> (lane * 8)) & 0xffu);
            sum += lane_operand(byte1, in1_sign[lane]) * lane_operand(byte2, in2_sign[lane]);
        }
        return sum;
    }

    void step(const Inputs& in) {
        const int32_t current_prod_add = prod_add(f2_in1, f2_in2, f2_in1_sign, f2_in2_sign);

        if (in.f2_tima_en) {
            f3_prod_add = current_prod_add;
        }
        if (in.f2_in3_en) {
            f3_in3 = in.f2_in3;
        }
        f3_zap_opd3 = in.f2_zap_opd3;

        if (in.f1_mul_en) {
            f2_in1 = in.f1_in1;
            f2_in2 = in.f1_in2;
            for (int lane = 0; lane < 4; ++lane) {
                f2_in1_sign[lane] = in.f1_in1_signed && (((in.f1_in1 >> (lane * 8 + 7)) & 1u) != 0u);
                f2_in2_sign[lane] = in.f1_in2_signed && (((in.f1_in2 >> (lane * 8 + 7)) & 1u) != 0u);
            }
        }
    }

    uint32_t output() const {
        const uint32_t in3_zapped = f3_zap_opd3 ? f3_in3 : 0u;
        return static_cast<uint32_t>(f3_prod_add) + in3_zapped;
    }
};

struct Sim {
    DUT* dut;
    uint64_t sim_time = 0;
    int checks = 0;
    int errors = 0;

    Sim(int argc, char** argv) {
        Verilated::commandArgs(argc, argv);
        dut = new DUT;
    }

    ~Sim() { delete dut; }

    void low() {
        dut->clk_i = 0;
        dut->eval();
        sim_time++;
    }

    void high() {
        dut->clk_i = 1;
        dut->eval();
        sim_time++;
    }

    void check(bool cond, const std::string& msg) {
        checks++;
        if (!cond) {
            std::printf("  FAIL [t=%lu]: %s\n", static_cast<unsigned long>(sim_time), msg.c_str());
            errors++;
        }
    }

    int finish() {
        std::printf("\n%d/%d checks passed\n", checks - errors, checks);
        if (errors) {
            std::printf("TEST FAILED: %d error(s)\n", errors);
            return 1;
        }
        std::printf("TEST PASSED\n");
        return 0;
    }
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void drive_inputs(Sim& sim, const Inputs& in) {
    sim.dut->f1_mul_en_i = in.f1_mul_en;
    sim.dut->f1_in1_i = in.f1_in1;
    sim.dut->f1_in2_i = in.f1_in2;
    sim.dut->f1_in1_signed_i = in.f1_in1_signed;
    sim.dut->f1_in2_signed_i = in.f1_in2_signed;
    sim.dut->f2_in3_i = in.f2_in3;
    sim.dut->f2_zap_opd3_i = in.f2_zap_opd3;
    sim.dut->f2_tima_en_i = in.f2_tima_en;
    sim.dut->f2_in3_en_i = in.f2_in3_en;
}

void run_cycle(Sim& sim, Model& model, const Inputs& in, const std::string& label) {
    drive_inputs(sim, in);
    sim.low();
    model.step(in);
    sim.high();
    sim.check(sim.dut->f3_out_o == model.output(), label);
}

}  // namespace

int main(int argc, char** argv) {
    Sim sim(argc, argv);
    Model model;

    sim.dut->clk_i = 0;
    drive_inputs(sim, Inputs{});
    sim.dut->eval();

    Inputs in;
    in.f1_mul_en = true;
    in.f2_tima_en = true;
    in.f2_in3_en = true;
    run_cycle(sim, model, in, "prime known zero state");

    in = {};
    in.f1_mul_en = true;
    in.f1_in1 = 0x04030201u;
    in.f1_in2 = 0x01010101u;
    run_cycle(sim, model, in, "load unsigned operands");

    in = {};
    in.f2_in3 = 0x00000005u;
    in.f2_zap_opd3 = true;
    in.f2_tima_en = true;
    in.f2_in3_en = true;
    run_cycle(sim, model, in, "capture unsigned dot product plus in3");

    in = {};
    in.f2_zap_opd3 = false;
    run_cycle(sim, model, in, "zap strobe updates even when data holds");

    in = {};
    in.f1_mul_en = true;
    in.f1_in1 = 0xfe03fe01u;
    in.f1_in2 = 0x04ff0203u;
    in.f1_in1_signed = true;
    in.f1_in2_signed = true;
    run_cycle(sim, model, in, "load signed operands");

    in = {};
    in.f2_in3 = 0xfffffff0u;
    in.f2_zap_opd3 = true;
    in.f2_tima_en = true;
    in.f2_in3_en = true;
    run_cycle(sim, model, in, "capture signed dot product plus negative in3");

    in = {};
    in.f2_zap_opd3 = true;
    run_cycle(sim, model, in, "hold F3 operands when enables are low");

    bool saw_f1_mul_en_0 = true;
    bool saw_f1_mul_en_1 = true;
    bool saw_f1_in1_signed_0 = true;
    bool saw_f1_in1_signed_1 = true;
    bool saw_f1_in2_signed_0 = true;
    bool saw_f1_in2_signed_1 = true;
    bool saw_f2_zap_0 = true;
    bool saw_f2_zap_1 = true;
    bool saw_f2_tima_en_0 = true;
    bool saw_f2_tima_en_1 = true;
    bool saw_f2_in3_en_0 = true;
    bool saw_f2_in3_en_1 = true;

    uint32_t seed = 0x71a04a33u;
    for (int i = 0; i < 8192; ++i) {
        Inputs in;
        in.f1_mul_en = (xorshift32(seed) & 1u) != 0u;
        in.f1_in1 = xorshift32(seed);
        in.f1_in2 = xorshift32(seed);
        in.f1_in1_signed = (xorshift32(seed) & 1u) != 0u;
        in.f1_in2_signed = (xorshift32(seed) & 1u) != 0u;
        in.f2_in3 = xorshift32(seed);
        in.f2_zap_opd3 = (xorshift32(seed) & 1u) != 0u;
        in.f2_tima_en = (xorshift32(seed) & 1u) != 0u;
        in.f2_in3_en = (xorshift32(seed) & 1u) != 0u;

        saw_f1_mul_en_0 |= !in.f1_mul_en;
        saw_f1_mul_en_1 |= in.f1_mul_en;
        saw_f1_in1_signed_0 |= !in.f1_in1_signed;
        saw_f1_in1_signed_1 |= in.f1_in1_signed;
        saw_f1_in2_signed_0 |= !in.f1_in2_signed;
        saw_f1_in2_signed_1 |= in.f1_in2_signed;
        saw_f2_zap_0 |= !in.f2_zap_opd3;
        saw_f2_zap_1 |= in.f2_zap_opd3;
        saw_f2_tima_en_0 |= !in.f2_tima_en;
        saw_f2_tima_en_1 |= in.f2_tima_en;
        saw_f2_in3_en_0 |= !in.f2_in3_en;
        saw_f2_in3_en_1 |= in.f2_in3_en;

        run_cycle(sim, model, in, "random cycle");
    }

    sim.check(saw_f1_mul_en_0 && saw_f1_mul_en_1, "f1_mul_en exercised both values");
    sim.check(saw_f1_in1_signed_0 && saw_f1_in1_signed_1, "f1_in1_signed exercised both values");
    sim.check(saw_f1_in2_signed_0 && saw_f1_in2_signed_1, "f1_in2_signed exercised both values");
    sim.check(saw_f2_zap_0 && saw_f2_zap_1, "f2_zap_opd3 exercised both values");
    sim.check(saw_f2_tima_en_0 && saw_f2_tima_en_1, "f2_tima_en exercised both values");
    sim.check(saw_f2_in3_en_0 && saw_f2_in3_en_1, "f2_in3_en exercised both values");

    return sim.finish();
}
