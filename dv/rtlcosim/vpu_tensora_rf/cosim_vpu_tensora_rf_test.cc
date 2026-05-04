// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_tensora_rf_tb.h"
#include "verilated.h"

#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vcosim_vpu_tensora_rf_tb;

namespace {

constexpr int kRandomCycles = 25000;

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
        compare("low");
        sim_time++;
    }

    void high() {
        dut->clk_i = 1;
        dut->eval();
        compare("high");
        sim_time++;
    }

    void tick() {
        low();
        high();
    }

    void compare(const std::string& phase) {
        comparisons++;
        if (dut->orig_rd_data_o != dut->new_rd_data_o) {
            mismatches++;
            std::printf("  MISMATCH [t=%lu] %s: orig=0x%08x new=0x%08x\n",
                        (unsigned long)sim_time, phase.c_str(),
                        dut->orig_rd_data_o, dut->new_rd_data_o);
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

uint32_t read_addr(Sim& sim, uint32_t addr) {
    sim.dut->rd_addr_i = addr;
    sim.dut->eval();
    sim.compare("read");
    return sim.dut->new_rd_data_o;
}

void write_single_tick(Sim& sim, uint32_t addr, uint32_t data) {
    sim.dut->wr_addr_i = addr;
    sim.dut->wr_data_i = data;
    sim.dut->wr_en_early_i = 1;
    sim.dut->wr_en_i = 1;
    sim.tick();
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_en_i = 0;
}

}  // namespace

int main(int argc, char** argv) {
    Sim sim(argc, argv);

    sim.dut->clk_i = 1;
    sim.dut->rd_addr_i = 0;
    sim.dut->wr_en_i = 0;
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_addr_i = 0;
    sim.dut->wr_data_i = 0;
    sim.dut->eval();
    sim.compare("init");

    write_single_tick(sim, 0, 0x11111111u);
    write_single_tick(sim, 1, 0x22222222u);
    sim.check(read_addr(sim, 0) == 0x11111111u, "read addr 0 after write");
    sim.check(read_addr(sim, 1) == 0x22222222u, "read addr 1 after write");

    sim.dut->wr_addr_i = 1;
    sim.dut->wr_data_i = 0x44444444u;
    sim.dut->wr_en_early_i = 1;
    sim.dut->wr_en_i = 1;
    sim.low();
    sim.dut->wr_data_i = 0x55555555u;
    sim.high();
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_en_i = 0;
    sim.check(read_addr(sim, 1) == 0x44444444u, "low-phase capture");

    uint32_t seed = 0x10a0a0a1u;
    for (int i = 0; i < kRandomCycles; ++i) {
      sim.dut->rd_addr_i = xorshift32(seed) & 1u;
      sim.dut->wr_addr_i = xorshift32(seed) & 1u;
      sim.dut->wr_data_i = xorshift32(seed);
      sim.dut->wr_en_early_i = xorshift32(seed) & 1u;
      sim.dut->wr_en_i = xorshift32(seed) & 1u;
      sim.low();
      if (xorshift32(seed) & 1u) sim.dut->wr_data_i = xorshift32(seed);
      sim.high();
    }

    sim.check(sim.comparisons >= 50000, "vpu_tensora_rf cosim should exceed 50k comparisons");
    return sim.finish();
}
