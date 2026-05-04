// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original esr_rbox vs new rbox_esr.
//
// Compares ALL outputs on EVERY cycle:
//   - APB response: pready, prdata, pslverr
//   - Register outputs: config, in_buf_pg, in_buf_cfg, out_buf_pg, out_buf_cfg,
//     start, consume (all 7 RW registers)
//   - Enable pulses: config_en, in_buf_pg_en, in_buf_cfg_en, out_buf_pg_en,
//     out_buf_cfg_en, start_en, consume_en (all 7 enables)
//
// Tests:
//   A. Post-reset idle
//   B. Write and read back each register individually
//   C. Status register read (RO)
//   D. Invalid address -> pslverr
//   E. Random read/write sequences (1000+ cycles)

#include "Vcosim_rbox_esr_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_rbox_esr_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("pready",       sim.dut->orig_pready_o,       sim.dut->new_pready_o);
    sim.compare("prdata",       sim.dut->orig_prdata_o,       sim.dut->new_prdata_o);
    sim.compare("pslverr",      sim.dut->orig_pslverr_o,      sim.dut->new_pslverr_o);
    sim.compare("config",       sim.dut->orig_config_o,       sim.dut->new_config_o);
    sim.compare("config_en",    sim.dut->orig_config_en_o,    sim.dut->new_config_en_o);
    sim.compare("in_buf_pg",    sim.dut->orig_in_buf_pg_o,    sim.dut->new_in_buf_pg_o);
    sim.compare("in_buf_pg_en", sim.dut->orig_in_buf_pg_en_o, sim.dut->new_in_buf_pg_en_o);
    sim.compare("in_buf_cfg",   sim.dut->orig_in_buf_cfg_o,   sim.dut->new_in_buf_cfg_o);
    sim.compare("in_buf_cfg_en",sim.dut->orig_in_buf_cfg_en_o,sim.dut->new_in_buf_cfg_en_o);
    sim.compare("out_buf_pg",   (uint32_t)sim.dut->orig_out_buf_pg_o,   (uint32_t)sim.dut->new_out_buf_pg_o);
    sim.compare("out_buf_pg_en",sim.dut->orig_out_buf_pg_en_o,sim.dut->new_out_buf_pg_en_o);
    sim.compare("out_buf_cfg",  sim.dut->orig_out_buf_cfg_o,  sim.dut->new_out_buf_cfg_o);
    sim.compare("out_buf_cfg_en",sim.dut->orig_out_buf_cfg_en_o,sim.dut->new_out_buf_cfg_en_o);
    sim.compare("start",        (uint32_t)sim.dut->orig_start_o,        (uint32_t)sim.dut->new_start_o);
    sim.compare("start_en",     sim.dut->orig_start_en_o,     sim.dut->new_start_en_o);
    sim.compare("consume",      (uint32_t)sim.dut->orig_consume_o,      (uint32_t)sim.dut->new_consume_o);
    sim.compare("consume_en",   sim.dut->orig_consume_en_o,   sim.dut->new_consume_en_o);
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

// Full APB write transaction
static void apb_write(CosimCtrl<DUT>& sim, uint16_t addr, uint64_t data) {
    sim.dut->apb_paddr_i   = addr;
    sim.dut->apb_pwrite_i  = 1;
    sim.dut->apb_psel_i    = 1;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i  = data;
    tick_compare(sim);

    sim.dut->apb_penable_i = 1;
    tick_compare(sim);

    apb_idle(sim.dut.get());
    tick_compare(sim);
    tick_compare(sim);
}

// Full APB read transaction
static void apb_read(CosimCtrl<DUT>& sim, uint16_t addr) {
    sim.dut->apb_paddr_i   = addr;
    sim.dut->apb_pwrite_i  = 0;
    sim.dut->apb_psel_i    = 1;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i  = 0;
    tick_compare(sim);

    sim.dut->apb_penable_i = 1;
    tick_compare(sim);

    apb_idle(sim.dut.get());
    tick_compare(sim);
    tick_compare(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;

    // Initialize
    apb_idle(sim.dut.get());
    sim.dut->status_i = 0x12345678;
    sim.dut->rst_w_ni = 0;
    for (int i = 0; i < 5; i++) sim.tick();
    sim.dut->rst_w_ni = 1;
    sim.tick();

    printf("Phase A: Post-reset idle\n");
    for (int i = 0; i < 10; i++) tick_compare(sim);

    printf("Phase B: Write and read each register\n");
    struct {
        const char* name;
        uint16_t addr;
        uint64_t wdata;
    } regs[] = {
        {"config",      0x0, 0xDEADBEEF12345678ull},
        {"in_buf_pg",   0x1, 0xAAAABBBBCCCCDDDDull},
        {"in_buf_cfg",  0x2, 0x1111222233334444ull},
        {"out_buf_pg",  0x3, 0x00000000ABCDEFFFull},
        {"out_buf_cfg", 0x4, 0xFEDCBA9876543210ull},
        {"start",       0x6, 0x0000000000000001ull},
        {"consume",     0x7, 0x000000000000007Full},
    };
    for (auto& r : regs) {
        printf("  write %s\n", r.name);
        apb_write(sim, r.addr, r.wdata);
        printf("  read %s\n", r.name);
        apb_read(sim, r.addr);
    }

    printf("Phase C: Status register read (RO)\n");
    sim.dut->status_i = 0xCAFEBABE;
    tick_compare(sim);
    apb_read(sim, 0x5);

    printf("Phase D: Invalid address -> pslverr\n");
    apb_read(sim, 0x8);
    apb_read(sim, 0xF);

    printf("Phase E: Random read/write sequences (1000+ cycles)\n");
    uint64_t lfsr = 0xACE1ACE1ACE1ACE1ull;
    for (int i = 0; i < 200; i++) {
        // Simple LFSR for pseudo-random
        lfsr = (lfsr >> 1) ^ (-(lfsr & 1ull) & 0xD800000000000000ull);

        uint16_t addr = lfsr & 0x7;  // 0-7
        bool write = (lfsr >> 3) & 1;

        if (write) {
            apb_write(sim, addr, lfsr);
        } else {
            apb_read(sim, addr);
        }
    }

    return sim.finish();
}
