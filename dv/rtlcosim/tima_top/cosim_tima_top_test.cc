// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_tima_top_tb.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vcosim_tima_top_tb;

namespace {

constexpr int kRandomIters = 25000;

struct PortSeen {
    bool saw_zero = false;
    bool saw_nonzero = false;

    void sample(uint64_t value) {
        if (value == 0) {
            saw_zero = true;
        } else {
            saw_nonzero = true;
        }
    }

    bool full() const { return saw_zero && saw_nonzero; }
};

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
    int errors = 0;
    int checks = 0;
    int mismatches = 0;
    int comparisons = 0;

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

    void compare(const std::string& phase) {
        comparisons++;
        if (dut->orig_f3_out_o != dut->new_f3_out_o) {
            mismatches++;
            std::printf("  MISMATCH [t=%lu] %s: orig=0x%08x new=0x%08x\n",
                        static_cast<unsigned long>(sim_time), phase.c_str(),
                        dut->orig_f3_out_o, dut->new_f3_out_o);
        }
    }

    void check(bool cond, const std::string& msg) {
        checks++;
        if (!cond) {
            std::printf("  FAIL [t=%lu]: %s\n", static_cast<unsigned long>(sim_time), msg.c_str());
            errors++;
        }
    }

    int finish() {
        std::printf("\n%d comparisons, %d mismatches\n", comparisons, mismatches);
        if (mismatches || errors) {
            std::printf("COSIM FAILED: %d mismatch(es), %d error(s)\n", mismatches, errors);
            return 1;
        }
        std::printf("COSIM PASSED\n");
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

void sample_inputs(const Inputs& in,
                   PortSeen& f1_mul_en_seen,
                   PortSeen& f1_in1_seen,
                   PortSeen& f1_in2_seen,
                   PortSeen& f1_in1_signed_seen,
                   PortSeen& f1_in2_signed_seen,
                   PortSeen& f2_in3_seen,
                   PortSeen& f2_zap_opd3_seen,
                   PortSeen& f2_tima_en_seen,
                   PortSeen& f2_in3_en_seen) {
    f1_mul_en_seen.sample(in.f1_mul_en);
    f1_in1_seen.sample(in.f1_in1);
    f1_in2_seen.sample(in.f1_in2);
    f1_in1_signed_seen.sample(in.f1_in1_signed);
    f1_in2_signed_seen.sample(in.f1_in2_signed);
    f2_in3_seen.sample(in.f2_in3);
    f2_zap_opd3_seen.sample(in.f2_zap_opd3);
    f2_tima_en_seen.sample(in.f2_tima_en);
    f2_in3_en_seen.sample(in.f2_in3_en);
}

void run_cycle(Sim& sim, Model& model, const Inputs& in, const std::string& label) {
    drive_inputs(sim, in);
    sim.low();
    sim.compare(label + " low");
    model.step(in);
    sim.high();
    sim.compare(label + " high");
    sim.check(sim.dut->new_f3_out_o == model.output(), label);
}

}  // namespace

int main(int argc, char** argv) {
    Sim sim(argc, argv);
    Model model;
    PortSeen f1_mul_en_seen;
    PortSeen f1_in1_seen;
    PortSeen f1_in2_seen;
    PortSeen f1_in1_signed_seen;
    PortSeen f1_in2_signed_seen;
    PortSeen f2_in3_seen;
    PortSeen f2_zap_opd3_seen;
    PortSeen f2_tima_en_seen;
    PortSeen f2_in3_en_seen;

    sim.dut->clk_i = 0;
    drive_inputs(sim, Inputs{});
    sim.dut->eval();

    Inputs in;
    in.f1_mul_en = true;
    in.f2_tima_en = true;
    in.f2_in3_en = true;
    sample_inputs(in, f1_mul_en_seen, f1_in1_seen, f1_in2_seen, f1_in1_signed_seen,
                  f1_in2_signed_seen, f2_in3_seen, f2_zap_opd3_seen, f2_tima_en_seen,
                  f2_in3_en_seen);
    run_cycle(sim, model, in, "prime known zero state");

    in = {};
    in.f1_mul_en = true;
    in.f1_in1 = 0x04030201u;
    in.f1_in2 = 0x01010101u;
    sample_inputs(in, f1_mul_en_seen, f1_in1_seen, f1_in2_seen, f1_in1_signed_seen,
                  f1_in2_signed_seen, f2_in3_seen, f2_zap_opd3_seen, f2_tima_en_seen,
                  f2_in3_en_seen);
    run_cycle(sim, model, in, "load unsigned operands");

    in = {};
    in.f2_in3 = 0x00000005u;
    in.f2_zap_opd3 = true;
    in.f2_tima_en = true;
    in.f2_in3_en = true;
    sample_inputs(in, f1_mul_en_seen, f1_in1_seen, f1_in2_seen, f1_in1_signed_seen,
                  f1_in2_signed_seen, f2_in3_seen, f2_zap_opd3_seen, f2_tima_en_seen,
                  f2_in3_en_seen);
    run_cycle(sim, model, in, "capture unsigned dot product plus in3");

    in = {};
    in.f2_zap_opd3 = false;
    sample_inputs(in, f1_mul_en_seen, f1_in1_seen, f1_in2_seen, f1_in1_signed_seen,
                  f1_in2_signed_seen, f2_in3_seen, f2_zap_opd3_seen, f2_tima_en_seen,
                  f2_in3_en_seen);
    run_cycle(sim, model, in, "zap strobe updates even when data holds");

    in = {};
    in.f1_mul_en = true;
    in.f1_in1 = 0xfe03fe01u;
    in.f1_in2 = 0x04ff0203u;
    in.f1_in1_signed = true;
    in.f1_in2_signed = true;
    sample_inputs(in, f1_mul_en_seen, f1_in1_seen, f1_in2_seen, f1_in1_signed_seen,
                  f1_in2_signed_seen, f2_in3_seen, f2_zap_opd3_seen, f2_tima_en_seen,
                  f2_in3_en_seen);
    run_cycle(sim, model, in, "load signed operands");

    in = {};
    in.f2_in3 = 0xfffffff0u;
    in.f2_zap_opd3 = true;
    in.f2_tima_en = true;
    in.f2_in3_en = true;
    sample_inputs(in, f1_mul_en_seen, f1_in1_seen, f1_in2_seen, f1_in1_signed_seen,
                  f1_in2_signed_seen, f2_in3_seen, f2_zap_opd3_seen, f2_tima_en_seen,
                  f2_in3_en_seen);
    run_cycle(sim, model, in, "capture signed dot product plus negative in3");

    in = {};
    in.f1_mul_en = true;
    in.f1_in1 = 0x80ff7f01u;
    in.f1_in2 = 0x01027f80u;
    in.f1_in1_signed = true;
    in.f1_in2_signed = false;
    sample_inputs(in, f1_mul_en_seen, f1_in1_seen, f1_in2_seen, f1_in1_signed_seen,
                  f1_in2_signed_seen, f2_in3_seen, f2_zap_opd3_seen, f2_tima_en_seen,
                  f2_in3_en_seen);
    run_cycle(sim, model, in, "load mixed-sign operands");

    in = {};
    in.f2_in3 = 0x12345678u;
    in.f2_in3_en = true;
    sample_inputs(in, f1_mul_en_seen, f1_in1_seen, f1_in2_seen, f1_in1_signed_seen,
                  f1_in2_signed_seen, f2_in3_seen, f2_zap_opd3_seen, f2_tima_en_seen,
                  f2_in3_en_seen);
    run_cycle(sim, model, in, "update in3 only");

    in = {};
    in.f2_tima_en = true;
    sample_inputs(in, f1_mul_en_seen, f1_in1_seen, f1_in2_seen, f1_in1_signed_seen,
                  f1_in2_signed_seen, f2_in3_seen, f2_zap_opd3_seen, f2_tima_en_seen,
                  f2_in3_en_seen);
    run_cycle(sim, model, in, "hold in3 while recapturing multiply");

    uint32_t seed = 0x61a04a33u;
    for (int i = 0; i < kRandomIters; ++i) {
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
        sample_inputs(in, f1_mul_en_seen, f1_in1_seen, f1_in2_seen, f1_in1_signed_seen,
                      f1_in2_signed_seen, f2_in3_seen, f2_zap_opd3_seen, f2_tima_en_seen,
                      f2_in3_en_seen);
        run_cycle(sim, model, in, "random cycle");
    }

    sim.check(f1_mul_en_seen.full(), "f1_mul_en_i should toggle");
    sim.check(f1_in1_seen.full(), "f1_in1_i should see zero and non-zero values");
    sim.check(f1_in2_seen.full(), "f1_in2_i should see zero and non-zero values");
    sim.check(f1_in1_signed_seen.full(), "f1_in1_signed_i should toggle");
    sim.check(f1_in2_signed_seen.full(), "f1_in2_signed_i should toggle");
    sim.check(f2_in3_seen.full(), "f2_in3_i should see zero and non-zero values");
    sim.check(f2_zap_opd3_seen.full(), "f2_zap_opd3_i should toggle");
    sim.check(f2_tima_en_seen.full(), "f2_tima_en_i should toggle");
    sim.check(f2_in3_en_seen.full(), "f2_in3_en_i should toggle");
    sim.check(sim.comparisons >= 50000, "tima_top cosim should exceed 50k comparisons");
    return sim.finish();
}
