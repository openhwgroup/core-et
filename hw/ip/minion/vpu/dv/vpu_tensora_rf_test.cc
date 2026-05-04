// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_tensora_rf_tb.h"
#include "verilated.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

using DUT = Vvpu_tensora_rf_tb;

namespace {

struct Sim {
    DUT* dut;
    uint64_t sim_time = 0;
    int errors = 0;
    int checks = 0;

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

    void tick() {
        low();
        high();
    }

    void check(bool cond, const std::string& msg) {
        checks++;
        if (!cond) {
            std::printf("  FAIL [t=%lu]: %s\n", (unsigned long)sim_time, msg.c_str());
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

void write_single_tick(Sim& sim, uint32_t addr, uint32_t data) {
    sim.dut->wr_addr_i = addr;
    sim.dut->wr_data_i = data;
    sim.dut->wr_en_early_i = 1;
    sim.dut->wr_en_i = 1;
    sim.tick();
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_en_i = 0;
}

uint32_t read_addr(Sim& sim, uint32_t addr) {
    sim.dut->rd_addr_i = addr;
    sim.dut->eval();
    return sim.dut->rd_data_o;
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

    write_single_tick(sim, 0, 0x11111111u);
    write_single_tick(sim, 1, 0x22222222u);
    sim.check(read_addr(sim, 0) == 0x11111111u, "read addr 0 after write");
    sim.check(read_addr(sim, 1) == 0x22222222u, "read addr 1 after write");

    sim.dut->wr_addr_i = 1;
    sim.dut->wr_data_i = 0x33333333u;
    sim.dut->wr_en_early_i = 1;
    sim.dut->wr_en_i = 0;
    sim.tick();
    sim.dut->wr_en_early_i = 0;
    sim.check(read_addr(sim, 1) == 0x22222222u, "wr_en low blocks write");

    sim.dut->wr_addr_i = 1;
    sim.dut->wr_data_i = 0x44444444u;
    sim.dut->wr_en_early_i = 1;
    sim.dut->wr_en_i = 1;
    sim.low();
    sim.dut->wr_data_i = 0x55555555u;
    sim.high();
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_en_i = 0;
    sim.check(read_addr(sim, 1) == 0x44444444u, "write uses low-phase captured data");

    sim.dut->wr_addr_i = 0;
    sim.dut->wr_data_i = 0xaaaaaaaau;
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_en_i = 1;
    sim.tick();
    sim.dut->wr_en_i = 0;
    sim.check(read_addr(sim, 0) == 0x44444444u, "missing early strobe reuses stale latched data");

    return sim.finish();
}
