// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Standalone co-simulation for minion_debug_apb_slv.

#include "Vcosim_minion_debug_apb_slv_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_minion_debug_apb_slv_tb;

namespace {

constexpr uint32_t kPpMsg = 2;
constexpr uint32_t kPpShift = 10;
constexpr uint32_t kThreadShift = 9;
constexpr uint32_t kAddrMask = 0xfff;
constexpr uint64_t kAll64 = ~uint64_t{0};

enum DebugReg : uint32_t {
    kNxData0    = 0x0,
    kNxData1    = 0x1,
    kAxData0    = 0x2,
    kAxData1    = 0x3,
    kAxProgBuf0 = 0x4,
    kAxProgBuf1 = 0x5,
    kNxProgBuf0 = 0x6,
    kNxProgBuf1 = 0x7,
    kAbsCmd     = 0x8,
    kInvalidHi  = 0xa,
};

uint32_t rng_state = 0x4d595df4u;

uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

uint64_t xorshift64() {
    return (uint64_t{xorshift32()} << 32) | xorshift32();
}

uint32_t addr(DebugReg reg, uint32_t thread = 0, uint32_t pp = kPpMsg,
              uint32_t reg_sel_hi = 0) {
    return ((pp & 0x3u) << kPpShift) |
           ((thread & 0x1u) << kThreadShift) |
           ((reg_sel_hi & 0x1fu) << 4) |
           (static_cast<uint32_t>(reg) & 0xfu);
}

struct InputCoverage {
    uint32_t paddr_one = 0;
    uint32_t paddr_zero = 0;
    uint64_t pwdata_one = 0;
    uint64_t pwdata_zero = 0;
    uint64_t csr_one = 0;
    uint64_t csr_zero = 0;
    uint32_t update_one = 0;
    uint32_t update_zero = 0;
    uint32_t debug_one = 0;
    uint32_t debug_zero = 0;
    bool pwrite_one = false;
    bool pwrite_zero = false;
    bool psel_one = false;
    bool psel_zero = false;
    bool penable_one = false;
    bool penable_zero = false;
    bool reset_one = false;
    bool reset_zero = false;

    void sample_reset(uint8_t rst_ni) {
        reset_one |= rst_ni != 0;
        reset_zero |= rst_ni == 0;
    }

    void sample(const DUT& dut) {
        const uint32_t paddr = dut.stim_apb_paddr & kAddrMask;
        paddr_one |= paddr;
        paddr_zero |= (~paddr) & kAddrMask;

        const uint64_t pwdata = dut.stim_apb_pwdata;
        pwdata_one |= pwdata;
        pwdata_zero |= ~pwdata;

        const uint64_t csr = dut.stim_csr_wdata;
        csr_one |= csr;
        csr_zero |= ~csr;

        const uint32_t update = dut.stim_update_ddata0 & 0x3u;
        update_one |= update;
        update_zero |= (~update) & 0x3u;

        const uint32_t debug = dut.stim_in_debug_mode & 0x3u;
        debug_one |= debug;
        debug_zero |= (~debug) & 0x3u;

        pwrite_one |= dut.stim_apb_pwrite != 0;
        pwrite_zero |= dut.stim_apb_pwrite == 0;
        psel_one |= dut.stim_apb_psel != 0;
        psel_zero |= dut.stim_apb_psel == 0;
        penable_one |= dut.stim_apb_penable != 0;
        penable_zero |= dut.stim_apb_penable == 0;
        sample_reset(dut.rst_ni);
    }

    void check(CosimCtrl<DUT>& sim) const {
        sim.check((paddr_one & kAddrMask) == kAddrMask && (paddr_zero & kAddrMask) == kAddrMask,
                  "cosim drove every APB address bit to both 0 and 1");
        sim.check(pwdata_one == kAll64 && pwdata_zero == kAll64,
                  "cosim drove every APB write-data bit to both 0 and 1");
        sim.check(csr_one == kAll64 && csr_zero == kAll64,
                  "cosim drove every CSR write-data bit to both 0 and 1");
        sim.check((update_one & 0x3u) == 0x3u && (update_zero & 0x3u) == 0x3u,
                  "cosim drove every update_ddata0 bit to both 0 and 1");
        sim.check((debug_one & 0x3u) == 0x3u && (debug_zero & 0x3u) == 0x3u,
                  "cosim drove every in_debug_mode bit to both 0 and 1");
        sim.check(pwrite_one && pwrite_zero, "cosim toggled apb_pwrite");
        sim.check(psel_one && psel_zero, "cosim toggled apb_psel");
        sim.check(penable_one && penable_zero, "cosim toggled apb_penable");
        sim.check(reset_one && reset_zero, "cosim toggled reset");
    }
};

std::string label(const char* phase, const char* signal) {
    return std::string(phase) + ":" + signal;
}

void compare_all(CosimCtrl<DUT>& sim, const char* phase) {
    sim.compare(label(phase, "apb_pready"),
                sim.dut->orig_apb_pready_o, sim.dut->new_apb_pready_o);
    sim.compare(label(phase, "apb_prdata"),
                sim.dut->orig_apb_prdata_o, sim.dut->new_apb_prdata_o);
    sim.compare(label(phase, "apb_pslverr"),
                sim.dut->orig_apb_pslverr_o, sim.dut->new_apb_pslverr_o);
    sim.compare(label(phase, "read_ddata0_t0"),
                sim.dut->orig_read_ddata0_t0_o, sim.dut->new_read_ddata0_t0_o);
    sim.compare(label(phase, "read_ddata0_t1"),
                sim.dut->orig_read_ddata0_t1_o, sim.dut->new_read_ddata0_t1_o);
    sim.compare(label(phase, "ffb_wdata"),
                sim.dut->orig_ffb_wdata_o, sim.dut->new_ffb_wdata_o);
    sim.compare(label(phase, "ffb_en"),
                sim.dut->orig_ffb_en_o, sim.dut->new_ffb_en_o);
    sim.compare(label(phase, "ffb_thread_sel"),
                sim.dut->orig_ffb_thread_sel_o, sim.dut->new_ffb_thread_sel_o);
    sim.compare(label(phase, "ex_program_buffer"),
                sim.dut->orig_ex_program_buffer_o, sim.dut->new_ex_program_buffer_o);
}

void eval_compare(CosimCtrl<DUT>& sim, InputCoverage& cov, const char* phase) {
    cov.sample(*sim.dut);
    sim.dut->eval();
    compare_all(sim, phase);
}

void tick_compare(CosimCtrl<DUT>& sim, InputCoverage& cov, const char* phase) {
    cov.sample(*sim.dut);
    sim.tick();
    compare_all(sim, phase);
}

void drive_idle(CosimCtrl<DUT>& sim) {
    sim.dut->stim_apb_paddr = 0;
    sim.dut->stim_apb_pwrite = 0;
    sim.dut->stim_apb_psel = 0;
    sim.dut->stim_apb_penable = 0;
    sim.dut->stim_apb_pwdata = 0;
    sim.dut->stim_csr_wdata = 0;
    sim.dut->stim_update_ddata0 = 0;
    sim.dut->stim_in_debug_mode = 0x3;
}

void drive_setup(CosimCtrl<DUT>& sim, uint32_t paddr, bool write, uint64_t wdata,
                 uint64_t csr_wdata = 0, uint32_t update = 0, uint32_t debug = 0x3) {
    sim.dut->stim_apb_paddr = paddr;
    sim.dut->stim_apb_pwrite = write ? 1 : 0;
    sim.dut->stim_apb_psel = 1;
    sim.dut->stim_apb_penable = 0;
    sim.dut->stim_apb_pwdata = wdata;
    sim.dut->stim_csr_wdata = csr_wdata;
    sim.dut->stim_update_ddata0 = update & 0x3u;
    sim.dut->stim_in_debug_mode = debug & 0x3u;
}

void drive_access(CosimCtrl<DUT>& sim, uint64_t csr_wdata = 0,
                  uint32_t update = 0, uint32_t debug = 0x3) {
    sim.dut->stim_apb_psel = 1;
    sim.dut->stim_apb_penable = 1;
    sim.dut->stim_csr_wdata = csr_wdata;
    sim.dut->stim_update_ddata0 = update & 0x3u;
    sim.dut->stim_in_debug_mode = debug & 0x3u;
}

void apb_read(CosimCtrl<DUT>& sim, InputCoverage& cov, uint32_t paddr,
              const char* name, uint32_t debug = 0x3) {
    drive_setup(sim, paddr, false, 0, 0, 0, debug);
    eval_compare(sim, cov, name);
    tick_compare(sim, cov, name);
    drive_access(sim, 0, 0, debug);
    eval_compare(sim, cov, name);
    tick_compare(sim, cov, name);
    drive_idle(sim);
    eval_compare(sim, cov, name);
    tick_compare(sim, cov, name);
}

void apb_write_nonexec(CosimCtrl<DUT>& sim, InputCoverage& cov, uint32_t paddr,
                       uint64_t wdata, const char* name, uint32_t debug = 0x3,
                       uint64_t csr_wdata = 0, uint32_t update = 0) {
    drive_setup(sim, paddr, true, wdata, 0, 0, debug);
    eval_compare(sim, cov, name);
    tick_compare(sim, cov, name);
    drive_access(sim, csr_wdata, update, debug);
    eval_compare(sim, cov, name);
    tick_compare(sim, cov, name);
    drive_idle(sim);
    eval_compare(sim, cov, name);
    tick_compare(sim, cov, name);
}

void apb_write_exec(CosimCtrl<DUT>& sim, InputCoverage& cov, uint32_t paddr,
                    uint64_t wdata, const char* name, uint32_t debug = 0x3) {
    drive_setup(sim, paddr, true, wdata, 0, 0, debug);
    eval_compare(sim, cov, name);
    tick_compare(sim, cov, name);
    drive_access(sim, 0, 0, debug);
    eval_compare(sim, cov, name);
    tick_compare(sim, cov, name);
    drive_access(sim, 0, 0, debug);
    eval_compare(sim, cov, name);
    tick_compare(sim, cov, name);
    drive_idle(sim);
    eval_compare(sim, cov, name);
    tick_compare(sim, cov, name);
}

void drive_random(CosimCtrl<DUT>& sim, uint32_t mode) {
    sim.dut->stim_apb_paddr = xorshift32() & kAddrMask;
    sim.dut->stim_apb_pwrite = xorshift32() & 0x1u;
    sim.dut->stim_apb_pwdata = xorshift64();
    sim.dut->stim_csr_wdata = xorshift64();
    sim.dut->stim_update_ddata0 = xorshift32() & 0x3u;
    sim.dut->stim_in_debug_mode = xorshift32() & 0x3u;

    switch (mode & 0x7u) {
        case 0:
            sim.dut->stim_apb_psel = 0;
            sim.dut->stim_apb_penable = 0;
            break;
        case 1:
            sim.dut->stim_apb_psel = 1;
            sim.dut->stim_apb_penable = 0;
            break;
        case 2:
            sim.dut->stim_apb_psel = 1;
            sim.dut->stim_apb_penable = 1;
            break;
        default:
            sim.dut->stim_apb_psel = xorshift32() & 0x1u;
            sim.dut->stim_apb_penable = xorshift32() & 0x1u;
            break;
    }
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 200000;
    InputCoverage cov;

    drive_idle(sim);
    cov.sample_reset(0);
    cov.sample_reset(1);
    sim.reset();
    eval_compare(sim, cov, "post_reset");

    // Directed protocol and decode coverage.
    apb_write_nonexec(sim, cov, addr(kNxData0, 0), 0x0000'0000'1122'3344ULL,
                      "write_t0_nxdata0");
    apb_write_nonexec(sim, cov, addr(kNxData1, 0), 0x0000'0000'5566'7788ULL,
                      "write_t0_nxdata1");
    apb_read(sim, cov, addr(kNxData0, 0), "read_t0_nxdata0");
    apb_read(sim, cov, addr(kNxData1, 0), "read_t0_nxdata1");
    apb_write_nonexec(sim, cov, addr(kNxData0, 1), 0x0000'0000'99aa'bbccULL,
                      "write_t1_nxdata0");
    apb_write_nonexec(sim, cov, addr(kNxData1, 1), 0x0000'0000'ddee'ff00ULL,
                      "write_t1_nxdata1");
    apb_read(sim, cov, addr(kNxData0, 1), "read_t1_nxdata0");
    apb_read(sim, cov, addr(kNxData1, 1), "read_t1_nxdata1");

    apb_write_exec(sim, cov, addr(kAxData0, 0), 0x1111'2222'3333'4444ULL,
                   "exec_axdata0_t0");
    apb_write_exec(sim, cov, addr(kAxData1, 1), 0x5555'6666'7777'8888ULL,
                   "exec_axdata1_t1");
    apb_write_exec(sim, cov, addr(kAxProgBuf0, 0), 0x9999'aaaa'bbbb'ccccULL,
                   "exec_axprogbuf0_t0");
    apb_write_exec(sim, cov, addr(kAxProgBuf1, 1), 0xdddd'eeee'ffff'0001ULL,
                   "exec_axprogbuf1_t1");
    apb_write_exec(sim, cov, addr(kAbsCmd, 0), 0x2345'6789'abcd'ef01ULL,
                   "exec_abscmd_t0");

    apb_write_nonexec(sim, cov, addr(kNxProgBuf0, 0), 0x1357'9bdf'2468'ace0ULL,
                      "ffb_nxprogbuf0_t0");
    apb_write_nonexec(sim, cov, addr(kNxProgBuf1, 1), 0xfdb9'7531'eca8'6420ULL,
                      "ffb_nxprogbuf1_t1");

    apb_read(sim, cov, addr(kNxData0, 0, 0), "bad_pp_read");
    apb_write_nonexec(sim, cov, addr(kInvalidHi, 0), 0x0bad'c0de'0000'0001ULL,
                      "invalid_register_write");
    apb_write_nonexec(sim, cov, addr(kNxData0, 1), 0x0000'0000'aaaa'5555ULL,
                      "debug_disabled_write", 0x1);
    apb_write_exec(sim, cov, addr(kAxProgBuf0, 1), 0xdead'beef'feed'f00dULL,
                   "debug_disabled_execute", 0x1);

    // CSR writeback path and APB/CSR same-cycle priority.
    drive_idle(sim);
    sim.dut->stim_csr_wdata = 0x0123'4567'89ab'cdefULL;
    sim.dut->stim_update_ddata0 = 0x3;
    eval_compare(sim, cov, "csr_update_both_eval");
    tick_compare(sim, cov, "csr_update_both_tick");
    apb_write_nonexec(sim, cov, addr(kNxData0, 0), 0x0000'0000'1111'2222ULL,
                      "csr_overrides_apb", 0x3, 0xfedc'ba98'7654'3210ULL, 0x1);

    // Deterministic all-zero/all-one samples guarantee vector input bit coverage.
    sim.dut->stim_apb_paddr = 0;
    sim.dut->stim_apb_pwrite = 0;
    sim.dut->stim_apb_psel = 0;
    sim.dut->stim_apb_penable = 0;
    sim.dut->stim_apb_pwdata = 0;
    sim.dut->stim_csr_wdata = 0;
    sim.dut->stim_update_ddata0 = 0;
    sim.dut->stim_in_debug_mode = 0;
    eval_compare(sim, cov, "all_zero_inputs_eval");
    tick_compare(sim, cov, "all_zero_inputs_tick");

    sim.dut->stim_apb_paddr = kAddrMask;
    sim.dut->stim_apb_pwrite = 1;
    sim.dut->stim_apb_psel = 1;
    sim.dut->stim_apb_penable = 1;
    sim.dut->stim_apb_pwdata = kAll64;
    sim.dut->stim_csr_wdata = kAll64;
    sim.dut->stim_update_ddata0 = 0x3;
    sim.dut->stim_in_debug_mode = 0x3;
    eval_compare(sim, cov, "all_one_inputs_eval");
    tick_compare(sim, cov, "all_one_inputs_tick");

    for (int i = 0; i < 5000; ++i) {
        drive_random(sim, xorshift32());
        eval_compare(sim, cov, "random_eval");
        tick_compare(sim, cov, "random_tick");
    }

    cov.check(sim);
    return sim.finish();
}
