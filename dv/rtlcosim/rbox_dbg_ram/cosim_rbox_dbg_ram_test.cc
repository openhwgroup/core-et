// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_dbg_ram vs new rbox_dbg_ram.
//
// Exercises ESR passthrough, debug data/meta read/write paths,
// and invalid address error handling.
// Compares ALL outputs on EVERY cycle with ZERO exceptions.

#include "Vcosim_rbox_dbg_ram_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_rbox_dbg_ram_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("apb_pready",   sim.dut->orig_apb_pready_o,   sim.dut->new_apb_pready_o);
    sim.compare("apb_prdata",   sim.dut->orig_apb_prdata_o,   sim.dut->new_apb_prdata_o);
    sim.compare("apb_pslverr",  sim.dut->orig_apb_pslverr_o,  sim.dut->new_apb_pslverr_o);
    sim.compare("esr_psel",     sim.dut->orig_esr_psel_o,     sim.dut->new_esr_psel_o);
    sim.compare("esr_penable",  sim.dut->orig_esr_penable_o,  sim.dut->new_esr_penable_o);
    sim.compare("esr_pwrite",   sim.dut->orig_esr_pwrite_o,   sim.dut->new_esr_pwrite_o);
    sim.compare("dbg_ram_ptr",  (uint32_t)sim.dut->orig_dbg_ram_ptr_o,   (uint32_t)sim.dut->new_dbg_ram_ptr_o);
    sim.compare("dbg_db_read_valid",  sim.dut->orig_dbg_db_read_valid_o,  sim.dut->new_dbg_db_read_valid_o);
    sim.compare("dbg_db_write_valid", sim.dut->orig_dbg_db_write_valid_o, sim.dut->new_dbg_db_write_valid_o);
    sim.compare("dbg_db_meta_read_valid",  sim.dut->orig_dbg_db_meta_read_valid_o,  sim.dut->new_dbg_db_meta_read_valid_o);
    sim.compare("dbg_db_meta_write_valid", sim.dut->orig_dbg_db_meta_write_valid_o, sim.dut->new_dbg_db_meta_write_valid_o);
}

static void tick_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

static void apb_idle(DUT* dut) {
    dut->apb_paddr_i   = 0;
    dut->apb_pwrite_i  = 0;
    dut->apb_psel_i    = 0;
    dut->apb_penable_i = 0;
    dut->apb_pwdata_i  = 0;
}

// APB transaction: setup -> access -> hold penable until pready -> idle
static void apb_transaction(CosimCtrl<DUT>& sim, uint16_t addr, bool write,
                            uint64_t wdata, uint64_t esr_rdata = 0, bool esr_respond = false) {
    // Setup phase
    sim.dut->apb_paddr_i   = addr;
    sim.dut->apb_pwrite_i  = write ? 1 : 0;
    sim.dut->apb_psel_i    = 1;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i  = wdata;
    tick_compare(sim);

    // Access phase
    sim.dut->apb_penable_i = 1;
    tick_compare(sim);

    // Hold penable, optionally provide ESR response
    if (esr_respond) {
        sim.dut->esr_pready_i = 1;
        sim.dut->esr_prdata_i = esr_rdata;
    }

    // Wait for pready (compare each cycle)
    for (int i = 0; i < 10; i++) {
        tick_compare(sim);
        if (sim.dut->orig_apb_pready_o) break;
    }

    // Clean up
    sim.dut->esr_pready_i = 0;
    apb_idle(sim.dut.get());
    tick_compare(sim);
    tick_compare(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;

    // Initialize
    apb_idle(sim.dut.get());
    sim.dut->esr_pready_i = 0;
    sim.dut->esr_prdata_i = 0;
    sim.dut->esr_pslverr_i = 0;
    sim.dut->dbg_db_read_data_i[0] = 0xCAFEBABE;
    sim.dut->dbg_db_read_data_i[1] = 0xDEADBEEF;
    sim.dut->dbg_db_read_data_i[2] = 0x11223344;
    sim.dut->dbg_db_read_data_i[3] = 0x55667788;
    sim.dut->dbg_db_read_data_i[4] = 0x99AABBCC;
    sim.dut->dbg_db_read_data_i[5] = 0xDDEEFF00;
    sim.dut->dbg_db_read_data_i[6] = 0xAAAABBBB;
    sim.dut->dbg_db_read_data_i[7] = 0xCCCCDDDD;
    sim.dut->dbg_db_read_data_i[8] = 0x12345678;
    sim.dut->dbg_db_read_data_i[9] = 0x9ABCDEF0;
    sim.dut->dbg_db_read_data_i[10] = 0xFEDCBA98;
    sim.dut->dbg_db_read_data_i[11] = 0x76543210;
    sim.dut->dbg_db_read_data_i[12] = 0xA1B2C3D4;
    sim.dut->dbg_db_read_data_i[13] = 0xE5F60718;
    sim.dut->dbg_db_read_data_i[14] = 0x192A3B4C;
    sim.dut->dbg_db_read_data_i[15] = 0x5D6E7F80;
    sim.dut->dbg_db_meta_read_data_i = 0x1FFFFFFFFFFull;  // 41-bit value, all 1s

    // Reset both domains
    sim.dut->rst_d_ni = 0;
    sim.dut->rst_w_ni = 0;
    for (int i = 0; i < 5; i++) sim.tick();
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.tick();
    tick_compare(sim);

    printf("Phase A: ESR passthrough\n");
    // Low address (bit 15 = 0) -> ESR path
    for (int addr = 0; addr < 8; addr++) {
        printf("  ESR read addr %d\n", addr);
        apb_transaction(sim, addr, false, 0, 0x100 + addr, true);
    }

    printf("Phase B: ESR write passthrough\n");
    for (int addr = 0; addr < 8; addr++) {
        printf("  ESR write addr %d\n", addr);
        sim.dut->esr_pready_i = 1; // ESR always ready for writes
        apb_transaction(sim, addr, true, 0xDEAD0000 + addr, 0, true);
        sim.dut->esr_pready_i = 0;
    }

    printf("Phase C: Debug data read\n");
    for (int qw = 0; qw < 8; qw++) {
        // bit 15 = 1, bits[13:9] = 5'b00100, bits[5:3] = qw
        uint16_t addr = (1 << 15) | (0b00100 << 9) | (qw << 3);
        printf("  debug data read qw=%d addr=0x%04x\n", qw, addr);
        apb_transaction(sim, addr, false, 0);
    }

    printf("Phase D: Debug meta read\n");
    for (int ptr = 0; ptr < 4; ptr++) {
        uint16_t addr = (1 << 15) | (0b00000 << 9) | ptr;
        printf("  debug meta read ptr=%d addr=0x%04x\n", ptr, addr);
        apb_transaction(sim, addr, false, 0);
    }

    printf("Phase E: Debug data write\n");
    for (int qw = 0; qw < 4; qw++) {
        uint16_t addr = (1 << 15) | (0b00100 << 9) | (qw << 3) | 0x01;
        printf("  debug data write qw=%d\n", qw);
        apb_transaction(sim, addr, true, 0xAA00BB00CC00DD00ull + qw);
    }

    printf("Phase F: Debug meta write\n");
    for (int ptr = 0; ptr < 4; ptr++) {
        uint16_t addr = (1 << 15) | (0b00001 << 9) | ptr;
        printf("  debug meta write ptr=%d\n", ptr);
        apb_transaction(sim, addr, true, 0x1234567800000000ull + ptr);
    }

    printf("Phase G: Invalid debug address\n");
    for (int hi = 2; hi < 8; hi++) {
        uint16_t addr = (1 << 15) | (hi << 11);
        printf("  invalid addr 0x%04x\n", addr);
        apb_transaction(sim, addr, false, 0);
    }

    printf("Phase H: Mixed traffic (1000+ cycles)\n");
    for (int i = 0; i < 100; i++) {
        uint16_t addr;
        if (i % 3 == 0) {
            addr = i % 8;  // ESR
            apb_transaction(sim, addr, (i % 2) == 0, i * 0x1111111111111111ull, 0xF00D + i, true);
        } else if (i % 3 == 1) {
            addr = (1 << 15) | (0b00100 << 9) | ((i % 8) << 3);  // debug data
            apb_transaction(sim, addr, (i % 2) == 0, i * 0x2222222222222222ull);
        } else {
            addr = (1 << 15) | (0b00000 << 9) | (i % 4);  // debug meta
            apb_transaction(sim, addr, (i % 2) == 0, i * 0x3333333333333333ull);
        }
    }

    return sim.finish();
}
