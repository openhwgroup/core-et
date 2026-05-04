// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// sim_ctrl.h — Minimal Verilator test harness.
//
// Usage:
//   #include "Vprim_fifo_sync.h"
//   #include "sim_ctrl.h"
//   int main(int argc, char** argv) {
//     SimCtrl<Vprim_fifo_sync> sim(argc, argv);
//     sim.reset();
//     // drive signals, call sim.tick(), sim.check(...)
//     return sim.finish();
//   }

#pragma once

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

#include "verilated.h"
#ifdef TRACE
#include "verilated_vcd_c.h"
#endif
#if VM_COVERAGE
#include "verilated_cov.h"
#endif

template <typename DUT>
class SimCtrl {
public:
    std::unique_ptr<DUT> dut;
    uint64_t sim_time = 0;
    int errors  = 0;
    int checks  = 0;
    uint64_t max_time = 200000;  // safety limit

    SimCtrl(int argc, char** argv) {
        Verilated::commandArgs(argc, argv);
        // X-rand runs intentionally start the DUT from randomized internal state.
        // Keep assertions masked until the test has applied reset.
        Verilated::assertOn(false);
        dut = std::make_unique<DUT>();
#ifdef TRACE
        Verilated::traceEverOn(true);
        tfp_ = new VerilatedVcdC;
        dut->trace(tfp_, 99);
        tfp_->open("sim.vcd");
#endif
    }

    ~SimCtrl() {
#ifdef TRACE
        if (tfp_) { tfp_->close(); delete tfp_; }
#endif
    }

    // Advance one clock cycle (posedge to posedge).
    void tick() {
        dut->clk_i = 0;
        dut->eval();
        dump_();
        sim_time++;

        dut->clk_i = 1;
        dut->eval();
        dump_();
        sim_time++;

        if (sim_time >= max_time) {
            printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim_time);
            std::exit(2);
        }
    }

    // Assert reset for `cycles` clocks, then release.
    void reset(int cycles = 5) {
        Verilated::assertOn(false);
        dut->rst_ni = 0;
        for (int i = 0; i < cycles; i++) tick();
        dut->rst_ni = 1;
        tick();  // one clean cycle after release
        Verilated::assertOn(true);
    }

    // Check a condition. Logs on failure.
    void check(bool cond, const std::string& msg) {
        checks++;
        if (!cond) {
            printf("  FAIL [t=%lu]: %s\n", (unsigned long)sim_time, msg.c_str());
            errors++;
        }
    }

    // Print summary and return exit code.
    int finish() {
#if VM_COVERAGE
        VerilatedCov::write();
#endif
        printf("\n%d/%d checks passed\n", checks - errors, checks);
        if (errors) {
            printf("TEST FAILED: %d error(s)\n", errors);
            return 1;
        }
        printf("TEST PASSED\n");
        return 0;
    }

private:
    void dump_() {
#ifdef TRACE
        if (tfp_) tfp_->dump(sim_time);
#endif
    }
#ifdef TRACE
    VerilatedVcdC* tfp_ = nullptr;
#endif
};
