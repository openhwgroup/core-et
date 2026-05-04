// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_tima_adder_tb.h"
#include "verilated.h"

#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vcosim_tima_adder_tb;

namespace {

constexpr int kRandomIters = 25000;

struct WordCoverage {
    uint32_t or_mask = 0u;
    uint32_t and_mask = 0xffffffffu;

    void observe(uint32_t value) {
        or_mask |= value;
        and_mask &= value;
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

    void compare(const std::string& name) {
        comparisons++;
        if (dut->orig_sum_o != dut->new_sum_o) {
            mismatches++;
            std::printf("  MISMATCH [t=%lu] %s: orig=0x%08x new=0x%08x\n",
                        (unsigned long)sim_time, name.c_str(),
                        dut->orig_sum_o, dut->new_sum_o);
        }
    }

    void check(bool cond, const std::string& msg) {
        checks++;
        if (!cond) {
            std::printf("  FAIL [t=%lu]: %s\n", (unsigned long)sim_time, msg.c_str());
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

void drive_and_compare(Sim& sim, uint32_t a, uint32_t b, const char* name) {
    sim.dut->a_i = a;
    sim.dut->b_i = b;
    sim.dut->eval();
    sim.compare(name);
    sim.check(sim.dut->new_sum_o == static_cast<uint32_t>(a + b), name);
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    Sim sim(argc, argv);
    WordCoverage a_cov;
    WordCoverage b_cov;

    auto drive = [&](uint32_t a, uint32_t b, const char* name) {
        a_cov.observe(a);
        b_cov.observe(b);
        drive_and_compare(sim, a, b, name);
    };

    drive(0x00000000u, 0x00000000u, "zero");
    drive(0x00000001u, 0x00000002u, "small");
    drive(0xffffffffu, 0x00000001u, "wrap");
    drive(0x80000000u, 0x80000000u, "high-bit");
    for (int bit = 0; bit < 32; ++bit) {
        const uint32_t one_hot = 1u << bit;
        drive(one_hot, 0u, "walk_a");
        drive(0u, one_hot, "walk_b");
    }

    uint32_t seed = 0x71adda32u;
    for (int i = 0; i < kRandomIters; ++i) {
        drive(xorshift32(seed), xorshift32(seed), "random");
    }

    sim.check(a_cov.or_mask == 0xffffffffu, "a_i should drive every bit high");
    sim.check(a_cov.and_mask == 0x00000000u, "a_i should drive every bit low");
    sim.check(b_cov.or_mask == 0xffffffffu, "b_i should drive every bit high");
    sim.check(b_cov.and_mask == 0x00000000u, "b_i should drive every bit low");
    sim.check(sim.comparisons >= 25000, "tima_adder cosim should exceed 25k comparisons");

    return sim.finish();
}
