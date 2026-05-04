// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_rf_1r1w -- Verilator test.
//
// The register file uses 2-phase write timing:
//   negedge: if wr_data_en_i, capture wr_data_i into wr_data_del
//   posedge: if wr_en_i, write wr_data_del to rf[wr_addr_i]
//
// In Verilator's tick model (negedge then posedge in one tick()),
// a single-cycle write with both enables high captures the PREVIOUS
// wr_data_i value (stale).  Tests use a 2-cycle pattern:
//   cycle 1: wr_data_en_i=1, wr_data_i=val  (captures val at negedge)
//   cycle 2: wr_en_i=1, wr_addr_i=addr      (commits at posedge)

#include "Vprim_rf_1r1w.h"
#include "verilated.h"

#include <cstdio>
#include <cstdlib>
#include <string>

#ifdef TRACE
#include "verilated_vcd_c.h"
#endif

// Minimal harness -- prim_rf_1r1w has no rst_ni, so we cannot use sim_ctrl.h.
struct Sim {
    Vprim_rf_1r1w* dut;
    uint64_t sim_time = 0;
    int errors  = 0;
    int checks  = 0;
#ifdef TRACE
    VerilatedVcdC* tfp = nullptr;
#endif

    Sim(int argc, char** argv) {
        Verilated::commandArgs(argc, argv);
        dut = new Vprim_rf_1r1w;
#ifdef TRACE
        Verilated::traceEverOn(true);
        tfp = new VerilatedVcdC;
        dut->trace(tfp, 99);
        tfp->open("sim.vcd");
#endif
    }

    ~Sim() {
#ifdef TRACE
        if (tfp) { tfp->close(); delete tfp; }
#endif
        delete dut;
    }

    void dump() {
#ifdef TRACE
        if (tfp) tfp->dump(sim_time);
#endif
    }

    // One full clock cycle: negedge first, then posedge.
    void tick() {
        dut->clk_i = 0;
        dut->eval();
        dump();
        sim_time++;

        dut->clk_i = 1;
        dut->eval();
        dump();
        sim_time++;
    }

    void check(bool cond, const std::string& msg) {
        checks++;
        if (!cond) {
            printf("  FAIL [t=%lu]: %s\n", (unsigned long)sim_time, msg.c_str());
            errors++;
        }
    }

    int finish() {
        printf("\n%d/%d checks passed\n", checks - errors, checks);
        if (errors) {
            printf("TEST FAILED: %d error(s)\n", errors);
            return 1;
        }
        printf("TEST PASSED\n");
        return 0;
    }
};

// 2-cycle write helper:
//   cycle 1: drive wr_data_en_i=1 with data
//   cycle 2: drive wr_en_i=1 with address
static void write_rf(Sim& sim, int addr, uint32_t data) {
    // Cycle 1: capture data
    sim.dut->wr_data_en_i = 1;
    sim.dut->wr_en_i      = 0;
    sim.dut->wr_data_i    = data;
    sim.tick();

    // Cycle 2: commit to RF
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_en_i      = 1;
    sim.dut->wr_addr_i    = addr;
    sim.tick();

    // Deassert
    sim.dut->wr_en_i = 0;
}

// Read helper: combinational, just set address and eval.
static uint32_t read_rf(Sim& sim, int addr) {
    sim.dut->rd_addr_i = addr;
    sim.dut->eval();
    return sim.dut->rd_data_o;
}

int main(int argc, char** argv) {
    Sim sim(argc, argv);

    // Default parameters: Width=32, Depth=32
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_en_i      = 0;
    sim.dut->wr_addr_i    = 0;
    sim.dut->wr_data_i    = 0;
    sim.dut->rd_addr_i    = 0;

    // Establish clock phase; the RF has no reset or defined power-up contents.
    sim.tick();
    sim.tick();

    // -- Test 1: Deterministic initialization by writes -------------
    printf("test 1: deterministic initialization by writes\n");
    for (int i = 0; i < 8; i++) {
        write_rf(sim, i, 0x1000 + i);
    }
    for (int i = 0; i < 8; i++) {
        uint32_t v = read_rf(sim, i);
        sim.check(v == (uint32_t)(0x1000 + i),
                  "rf[" + std::to_string(i) + "] == seeded value");
    }

    // -- Test 2: 2-phase write and read back ------------------------
    printf("test 2: write and read back (2-phase)\n");
    write_rf(sim, 0, 0xDEADBEEF);
    sim.check(read_rf(sim, 0) == 0xDEADBEEF, "rf[0] == 0xDEADBEEF");

    write_rf(sim, 5, 0x12345678);
    sim.check(read_rf(sim, 5) == 0x12345678, "rf[5] == 0x12345678");

    // -- Test 3: wr_en_i=0 does not modify RF -----------------------
    printf("test 3: wr_en_i=0 does not write\n");
    // Capture new data but do NOT commit.
    sim.dut->wr_data_en_i = 1;
    sim.dut->wr_en_i      = 0;
    sim.dut->wr_data_i    = 0xFFFFFFFF;
    sim.tick();
    sim.dut->wr_data_en_i = 0;
    // Do NOT assert wr_en_i.
    sim.tick();
    sim.check(read_rf(sim, 0) == 0xDEADBEEF, "rf[0] unchanged (wr_en_i=0)");
    sim.check(read_rf(sim, 5) == 0x12345678, "rf[5] unchanged (wr_en_i=0)");

    // -- Test 4: wr_data_en_i=0 does not capture data ---------------
    printf("test 4: wr_data_en_i=0 does not capture\n");
    // Drive new wr_data_i but with wr_data_en_i=0.
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_en_i      = 0;
    sim.dut->wr_data_i    = 0xBADBAD00;
    sim.tick();
    // Now commit -- should write the PREVIOUS wr_data_del, not 0xBADBAD00.
    // wr_data_del still holds 0xFFFFFFFF from test 3.
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_addr_i = 10;
    sim.tick();
    sim.dut->wr_en_i = 0;
    sim.check(read_rf(sim, 10) == 0xFFFFFFFF,
              "rf[10] == 0xFFFFFFFF (stale wr_data_del, not 0xBADBAD00)");

    // -- Test 5: split capture/commit preserves captured value ------
    printf("test 5: split capture/commit preserves captured value\n");
    // Capture data on one cycle, then change wr_data_i before commit.
    sim.dut->wr_data_en_i = 1;
    sim.dut->wr_en_i      = 0;
    sim.dut->wr_data_i    = 0xCAFEBABE;
    sim.tick();
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_data_i    = 0x13579BDF;
    sim.dut->wr_en_i      = 1;
    sim.dut->wr_addr_i    = 11;
    sim.tick();
    sim.dut->wr_en_i      = 0;
    sim.check(read_rf(sim, 11) == 0xCAFEBABE,
              "rf[11] uses data captured on prior negedge, not later wr_data_i");

    // -- Test 6: single-cycle both-enables writes current data --------
    printf("test 6: single-cycle both-enables writes current data\n");
    // First preload wr_data_del with a known value.
    sim.dut->wr_data_en_i = 1;
    sim.dut->wr_en_i      = 0;
    sim.dut->wr_data_i    = 0x2468ACE0;
    sim.tick();
    // Then assert both enables in one cycle with different data.
    sim.dut->wr_data_en_i = 1;
    sim.dut->wr_en_i      = 1;
    sim.dut->wr_addr_i    = 12;
    sim.dut->wr_data_i    = 0x0BADF00D;
    sim.tick();
    sim.dut->wr_data_en_i = 0;
    sim.dut->wr_en_i      = 0;
    sim.check(read_rf(sim, 12) == 0x0BADF00D,
              "rf[12] writes current wr_data_i when wr_data_en_i+wr_en_i are both high");

    // -- Test 7: Multiple independent entries -----------------------
    printf("test 7: multiple independent entries\n");
    for (int i = 0; i < 8; i++) {
        write_rf(sim, i, 0xA000 + i);
    }
    for (int i = 0; i < 8; i++) {
        uint32_t v = read_rf(sim, i);
        sim.check(v == (uint32_t)(0xA000 + i),
                  "rf[" + std::to_string(i) + "] == 0x" +
                  std::to_string(0xA000 + i));
    }

    // -- Test 8: Read different addresses returns independent values -
    printf("test 8: read independence\n");
    // rf[0]=0xA000, rf[5]=0xA005 from test 5.
    sim.check(read_rf(sim, 0) == 0xA000, "rf[0] still 0xA000");
    sim.check(read_rf(sim, 5) == 0xA005, "rf[5] still 0xA005");
    sim.check(read_rf(sim, 10) == 0xFFFFFFFF, "rf[10] still 0xFFFFFFFF");

    return sim.finish();
}
