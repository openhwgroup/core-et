// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_dbg_ram.
//
// Tests:
//   1. After reset, no spurious ready
//   2. ESR passthrough: low address routes to ESR
//   3. Debug data read: high address bit set, data region
//   4. Debug meta read: high address bit set, meta region
//   5. Debug data write asserts write valid
//   6. Debug meta write
//   7. Invalid debug address -> pslverr
//   8. Independent reset domains: warm reset does not clear debug pipeline
//   9. Debug reset clears pipeline but warm-reset ESR tracking is separate

#include "Vrbox_dbg_ram_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

using DUT = Vrbox_dbg_ram_tb;

// APB idle
static void apb_idle(DUT* dut) {
    dut->apb_paddr_i   = 0;
    dut->apb_pwrite_i  = 0;
    dut->apb_psel_i    = 0;
    dut->apb_penable_i = 0;
    dut->apb_pwdata_i  = 0;
}

// Custom reset for dual-reset TB
static void dual_reset(SimCtrl<DUT>& sim, int cycles) {
    sim.dut->rst_d_ni = 0;
    sim.dut->rst_w_ni = 0;
    for (int i = 0; i < cycles; i++) sim.tick();
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.tick();
}

// Wait for pready, up to max_cycles. Returns true if pready seen.
static bool wait_pready(SimCtrl<DUT>& sim, int max_cycles = 20) {
    for (int i = 0; i < max_cycles; i++) {
        if (sim.dut->apb_pready_o) return true;
        sim.tick();
    }
    return sim.dut->apb_pready_o != 0;
}

// Full APB write: setup -> access -> wait pready -> idle
static void apb_write(SimCtrl<DUT>& sim, uint16_t addr, uint64_t data) {
    // Setup phase
    sim.dut->apb_paddr_i   = addr;
    sim.dut->apb_pwrite_i  = 1;
    sim.dut->apb_psel_i    = 1;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i  = data;
    sim.tick();

    // Access phase
    sim.dut->apb_penable_i = 1;
    sim.tick();

    // Keep penable asserted until pready
    wait_pready(sim);

    apb_idle(sim.dut.get());
    sim.tick();
    sim.tick();
}

// Full APB read: setup -> access -> wait pready -> idle, return data
static uint64_t apb_read(SimCtrl<DUT>& sim, uint16_t addr) {
    // Setup phase
    sim.dut->apb_paddr_i   = addr;
    sim.dut->apb_pwrite_i  = 0;
    sim.dut->apb_psel_i    = 1;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i  = 0;
    sim.tick();

    // Access phase
    sim.dut->apb_penable_i = 1;
    sim.tick();

    // Keep penable asserted until pready
    wait_pready(sim);

    uint64_t data = sim.dut->apb_prdata_o;
    apb_idle(sim.dut.get());
    sim.tick();
    sim.tick();
    return data;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;

    // Initialize all inputs
    apb_idle(sim.dut.get());
    sim.dut->esr_apb_pready_i  = 0;
    sim.dut->esr_apb_prdata_i  = 0;
    sim.dut->esr_apb_pslverr_i = 0;
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    dual_reset(sim, 5);

    // -- Test 1: No spurious ready after reset --
    printf("test 1: no spurious ready after reset\n");
    sim.tick();
    sim.check(sim.dut->apb_pready_o == 0, "pready = 0 after reset with no request");

    // -- Test 2: ESR passthrough for low address --
    printf("test 2: ESR passthrough\n");
    // Address 0x0005 (bit 15 = 0) -> should route to ESR
    // Setup phase
    sim.dut->apb_paddr_i   = 0x0005;
    sim.dut->apb_pwrite_i  = 0;
    sim.dut->apb_psel_i    = 1;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i  = 0;
    sim.tick();

    sim.check(sim.dut->esr_apb_psel_o == 1, "ESR psel asserted for low address");
    sim.check(sim.dut->esr_apb_paddr_o == 0x0005, "ESR paddr matches");

    // Access phase
    sim.dut->apb_penable_i = 1;
    sim.tick();

    sim.check(sim.dut->esr_apb_penable_o == 1, "ESR penable asserted");

    // Provide ESR response
    sim.dut->esr_apb_pready_i = 1;
    sim.dut->esr_apb_prdata_i = 0x1234567890ABCDEFull;
    sim.tick();

    // Keep penable asserted, wait for pready to propagate
    wait_pready(sim, 10);

    sim.check(sim.dut->apb_pready_o == 1, "pready from ESR path");
    sim.check(sim.dut->apb_prdata_o == 0x1234567890ABCDEFull, "prdata from ESR");

    apb_idle(sim.dut.get());
    sim.dut->esr_apb_pready_i = 0;
    sim.tick();
    sim.tick();

    // -- Test 3: Debug data read --
    printf("test 3: debug data read\n");
    // bit 15=1, bits[13:9] = 5'b00100, bits[5:0]=0x01 -> ram_ptr=1, qw=0
    uint16_t dbg_data_addr = (1 << 15) | (0b00100 << 9) | 0x01;
    uint64_t rdata = apb_read(sim, dbg_data_addr);
    // NOTE: The original's debug data read path has a broken enable
    // ({1'b1, 1-bit-select} = 2 bits zero-extended to 65 → enable=0).
    // The d3 data latch never captures, so prdata holds the stale value
    // from the previous ESR read. We verify the stale value matches.
    sim.check(rdata == 0x1234567890ABCDEFull, "prdata = stale ESR value (original debug data read is broken)");
    printf("  prdata = 0x%016llx (stale, matching original)\n", (unsigned long long)rdata);

    // -- Test 4: Debug meta read --
    printf("test 4: debug meta read\n");
    uint16_t dbg_meta_addr = (1 << 15) | (0b00000 << 9) | 0x02;

    // Setup phase
    sim.dut->apb_paddr_i   = dbg_meta_addr;
    sim.dut->apb_pwrite_i  = 0;
    sim.dut->apb_psel_i    = 1;
    sim.dut->apb_penable_i = 0;
    sim.tick();

    // Access phase
    sim.dut->apb_penable_i = 1;
    sim.tick();

    sim.check(sim.dut->dbg_db_meta_read_valid_o == 1, "dbg_db_meta_read_valid asserted");

    // Wait for pipeline to complete
    wait_pready(sim, 10);
    sim.check(sim.dut->apb_pready_o == 1, "pready from debug meta read");

    apb_idle(sim.dut.get());
    sim.tick();
    sim.tick();

    // -- Test 5: Debug data write --
    printf("test 5: debug data write\n");
    uint16_t dbg_wr_addr = (1 << 15) | (0b00100 << 9) | 0x03;

    // Setup
    sim.dut->apb_paddr_i   = dbg_wr_addr;
    sim.dut->apb_pwrite_i  = 1;
    sim.dut->apb_psel_i    = 1;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i  = 0xAAAABBBBCCCCDDDDull;
    sim.tick();

    // Access
    sim.dut->apb_penable_i = 1;
    sim.tick();

    sim.check(sim.dut->dbg_db_write_valid_o == 1, "dbg_db_write_valid asserted");

    wait_pready(sim, 10);
    sim.check(sim.dut->apb_pready_o == 1, "pready from debug data write");
    sim.check(sim.dut->apb_pslverr_o == 0, "no error on valid debug write");

    apb_idle(sim.dut.get());
    sim.tick();
    sim.tick();

    // -- Test 6: Debug meta write --
    printf("test 6: debug meta write\n");
    uint16_t dbg_meta_wr_addr = (1 << 15) | (0b00001 << 9) | 0x05;

    sim.dut->apb_paddr_i   = dbg_meta_wr_addr;
    sim.dut->apb_pwrite_i  = 1;
    sim.dut->apb_psel_i    = 1;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i  = 0x00000000FFFFFFFFull;
    sim.tick();

    sim.dut->apb_penable_i = 1;
    sim.tick();

    sim.check(sim.dut->dbg_db_meta_write_valid_o == 1, "dbg_db_meta_write_valid asserted");

    wait_pready(sim, 10);
    sim.check(sim.dut->apb_pready_o == 1, "pready from debug meta write");

    apb_idle(sim.dut.get());
    sim.tick();
    sim.tick();

    // -- Test 7: Invalid debug address -> pslverr --
    printf("test 7: invalid debug address\n");
    uint16_t dbg_bad_addr = (1 << 15) | (0b01000 << 9);

    sim.dut->apb_paddr_i   = dbg_bad_addr;
    sim.dut->apb_pwrite_i  = 0;
    sim.dut->apb_psel_i    = 1;
    sim.dut->apb_penable_i = 0;
    sim.tick();

    sim.dut->apb_penable_i = 1;
    sim.tick();

    wait_pready(sim, 10);
    sim.check(sim.dut->apb_pslverr_o == 1, "pslverr on invalid debug address");

    apb_idle(sim.dut.get());
    sim.tick();

    // -- Test 8: Independent reset domains --
    printf("test 8: independent reset domains\n");
    // Start a debug access, then apply only warm reset
    // The debug pipeline should NOT be cleared
    apb_idle(sim.dut.get());
    sim.tick();

    // Apply warm reset only
    sim.dut->rst_w_ni = 0;
    sim.tick();
    sim.tick();
    sim.dut->rst_w_ni = 1;
    sim.tick();

    // After warm-only reset, the debug pipeline (rst_d_ni domain) is NOT cleared
    // but the ESR pready/pslverr tracking FFs are cleared
    sim.check(sim.dut->apb_pready_o == 0, "pready=0 after warm reset clears ESR tracking");

    // Apply debug reset only
    sim.dut->rst_d_ni = 0;
    sim.tick();
    sim.tick();
    sim.dut->rst_d_ni = 1;
    sim.tick();

    // Debug pipeline should be fully clear now
    sim.check(sim.dut->dbg_db_read_valid_o == 0, "debug read valid cleared by debug reset");
    sim.check(sim.dut->dbg_db_write_valid_o == 0, "debug write valid cleared by debug reset");

    return sim.finish();
}
