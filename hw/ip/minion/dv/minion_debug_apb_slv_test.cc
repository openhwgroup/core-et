// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Standalone unit test for minion_debug_apb_slv.

#include "Vminion_debug_apb_slv_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vminion_debug_apb_slv_tb;

namespace {

constexpr uint32_t kPpMsg = 2;
constexpr uint32_t kPpUmode = 0;
constexpr uint32_t kPpShift = 10;
constexpr uint32_t kThreadShift = 9;
constexpr uint64_t kDataMask32 = 0xffff'ffffULL;

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
    kInvalid    = 0x9,
    kInvalidHi  = 0xa,
};

uint32_t addr(DebugReg reg, uint32_t thread = 0, uint32_t pp = kPpMsg,
              uint32_t reg_sel_hi = 0) {
    return ((pp & 0x3u) << kPpShift) |
           ((thread & 0x1u) << kThreadShift) |
           ((reg_sel_hi & 0x1fu) << 4) |
           (static_cast<uint32_t>(reg) & 0xfu);
}

std::string tag(const char* msg, const char* suffix) {
    return std::string(msg) + ": " + suffix;
}

uint64_t ddata(const SimCtrl<DUT>& sim, uint32_t thread) {
    return thread ? sim.dut->read_ddata0_t1_o : sim.dut->read_ddata0_t0_o;
}

void eval(SimCtrl<DUT>& sim) {
    sim.dut->eval();
}

void drive_idle(SimCtrl<DUT>& sim) {
    sim.dut->apb_paddr_i = 0;
    sim.dut->apb_pwrite_i = 0;
    sim.dut->apb_psel_i = 0;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i = 0;
    sim.dut->csr_wdata_i = 0;
    sim.dut->update_ddata0_i = 0;
    sim.dut->in_debug_mode_i = 0x3;
    eval(sim);
}

void clear_sideband(SimCtrl<DUT>& sim) {
    sim.dut->csr_wdata_i = 0;
    sim.dut->update_ddata0_i = 0;
    eval(sim);
}

void check_idle_outputs(SimCtrl<DUT>& sim, const char* msg) {
    eval(sim);
    sim.check(sim.dut->apb_pready_o == 0, tag(msg, "idle APB ready is low"));
    sim.check(sim.dut->ffb_en_o == 0, tag(msg, "frontend write enables are idle"));
    sim.check(sim.dut->ex_program_buffer_o == 0, tag(msg, "execute pulse bus is idle"));
}

void check_reset_outputs(SimCtrl<DUT>& sim) {
    eval(sim);
    sim.check(sim.dut->apb_pready_o == 0, "reset leaves APB ready low while idle");
    sim.check(sim.dut->apb_prdata_o == 0, "reset clears read-data pipeline");
    sim.check(sim.dut->apb_pslverr_o == 1, "reset clears valid decode state so APB error is high");
    sim.check(sim.dut->ffb_en_o == 0, "reset clears frontend enables");
    sim.check(sim.dut->ex_program_buffer_o == 0, "reset clears execute pulse pipeline after reset clocks");
}

void csr_update(SimCtrl<DUT>& sim, uint64_t value, uint32_t update_mask,
                const char* msg) {
    sim.dut->apb_psel_i = 0;
    sim.dut->apb_penable_i = 0;
    sim.dut->csr_wdata_i = value;
    sim.dut->update_ddata0_i = update_mask & 0x3u;
    eval(sim);
    sim.tick();
    clear_sideband(sim);
    sim.tick();
    if (update_mask & 0x1u) {
        sim.check(sim.dut->read_ddata0_t0_o == value, tag(msg, "CSR update writes thread 0 ddata0"));
    }
    if (update_mask & 0x2u) {
        sim.check(sim.dut->read_ddata0_t1_o == value, tag(msg, "CSR update writes thread 1 ddata0"));
    }
}

void apb_setup(SimCtrl<DUT>& sim, uint32_t paddr, bool write, uint64_t wdata,
               const char* msg) {
    sim.dut->apb_paddr_i = paddr;
    sim.dut->apb_pwrite_i = write ? 1 : 0;
    sim.dut->apb_psel_i = 1;
    sim.dut->apb_penable_i = 0;
    sim.dut->apb_pwdata_i = wdata;
    eval(sim);
    sim.check(sim.dut->apb_pready_o == 0, tag(msg, "setup phase keeps pready low"));
    sim.tick();
}

void apb_to_access(SimCtrl<DUT>& sim) {
    sim.dut->apb_psel_i = 1;
    sim.dut->apb_penable_i = 1;
    eval(sim);
}

void apb_finish(SimCtrl<DUT>& sim, const char* msg) {
    sim.tick();
    sim.dut->apb_psel_i = 0;
    sim.dut->apb_penable_i = 0;
    eval(sim);
    sim.tick();
    check_idle_outputs(sim, msg);
}

uint64_t expected_after_word_write(uint64_t old_value, DebugReg reg, uint64_t wdata) {
    if (reg == kNxData0 || reg == kAxData0) {
        return (old_value & ~kDataMask32) | (wdata & kDataMask32);
    }
    if (reg == kNxData1 || reg == kAxData1) {
        return (old_value & kDataMask32) | ((wdata & kDataMask32) << 32);
    }
    return old_value;
}

void apb_read(SimCtrl<DUT>& sim, uint32_t paddr, uint64_t expected_prdata,
              const char* msg, bool expect_error = false) {
    apb_setup(sim, paddr, false, 0, msg);
    apb_to_access(sim);
    sim.check(sim.dut->apb_pready_o == 1, tag(msg, "read access is ready immediately"));
    sim.check(sim.dut->apb_pslverr_o == (expect_error ? 1 : 0), tag(msg, "read error status"));
    sim.check(sim.dut->apb_prdata_o == expected_prdata, tag(msg, "read data matches expected value"));
    sim.check(sim.dut->ffb_en_o == 0, tag(msg, "read does not drive frontend enables"));
    sim.check(sim.dut->ex_program_buffer_o == 0, tag(msg, "read does not execute program buffer"));
    apb_finish(sim, msg);
}

void apb_write_nonexec(SimCtrl<DUT>& sim, uint32_t paddr, uint64_t wdata,
                       uint32_t thread, DebugReg reg, const char* msg,
                       bool expect_error = false, uint8_t expected_ffb = 0) {
    const uint64_t before = ddata(sim, thread);
    apb_setup(sim, paddr, true, wdata, msg);
    apb_to_access(sim);
    const uint8_t gated_ffb = expect_error ? 0 : expected_ffb;
    sim.check(sim.dut->apb_pready_o == 1, tag(msg, "non-execute write is ready immediately"));
    sim.check(sim.dut->apb_pslverr_o == (expect_error ? 1 : 0), tag(msg, "write error status"));
    sim.check(sim.dut->ffb_wdata_o == wdata, tag(msg, "frontend write data follows APB write data"));
    sim.check(sim.dut->ffb_en_o == gated_ffb, tag(msg, "frontend write enables"));
    sim.check(sim.dut->ffb_thread_sel_o == thread, tag(msg, "frontend thread select follows APB address"));
    sim.check(sim.dut->ex_program_buffer_o == 0, tag(msg, "non-execute write does not pulse execute"));
    apb_finish(sim, msg);

    const uint64_t expected = expect_error ? before : expected_after_word_write(before, reg, wdata);
    sim.check(ddata(sim, thread) == expected, tag(msg, "ddata0 update result"));
}

void apb_write_exec(SimCtrl<DUT>& sim, uint32_t paddr, uint64_t wdata,
                    uint32_t thread, DebugReg reg, const char* msg,
                    bool expect_error = false, uint8_t expected_ffb = 0) {
    const uint64_t before = ddata(sim, thread);
    apb_setup(sim, paddr, true, wdata, msg);
    apb_to_access(sim);
    const uint8_t gated_ffb = expect_error ? 0 : expected_ffb;
    const uint8_t gated_ex = expect_error ? 0 : (1u << thread);

    sim.check(sim.dut->apb_pready_o == 0, tag(msg, "execute write waits one access cycle"));
    sim.check(sim.dut->apb_pslverr_o == (expect_error ? 1 : 0), tag(msg, "execute first-cycle error status"));
    sim.check(sim.dut->ffb_wdata_o == wdata, tag(msg, "execute first-cycle ffb data"));
    sim.check(sim.dut->ffb_en_o == gated_ffb, tag(msg, "execute first-cycle ffb enables"));
    sim.check(sim.dut->ffb_thread_sel_o == thread, tag(msg, "execute first-cycle thread select"));
    sim.check(sim.dut->ex_program_buffer_o == gated_ex, tag(msg, "execute pulse asserted before pready"));

    sim.tick();
    eval(sim);
    sim.check(sim.dut->apb_pready_o == 1, tag(msg, "execute write is ready on second access cycle"));
    sim.check(sim.dut->apb_pslverr_o == (expect_error ? 1 : 0), tag(msg, "execute second-cycle error status"));
    sim.check(sim.dut->ffb_en_o == gated_ffb, tag(msg, "execute ffb enables persist until ready"));
    sim.check(sim.dut->ex_program_buffer_o == 0, tag(msg, "execute pulse is one cycle wide"));

    apb_finish(sim, msg);
    const uint64_t expected = expect_error ? before : expected_after_word_write(before, reg, wdata);
    sim.check(ddata(sim, thread) == expected, tag(msg, "execute ddata0 update result"));
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 10000;

    drive_idle(sim);
    sim.dut->in_debug_mode_i = 0;
    sim.reset();
    check_reset_outputs(sim);

    drive_idle(sim);
    check_idle_outputs(sim, "post-reset idle");

    csr_update(sim, 0x1122'3344'5566'7788ULL, 0x1, "seed thread 0");
    csr_update(sim, 0x8877'6655'4433'2211ULL, 0x2, "seed thread 1");

    apb_read(sim, addr(kNxData0, 0), 0x0000'0000'5566'7788ULL, "read thread0 nxdata0");
    apb_read(sim, addr(kNxData1, 0), 0x0000'0000'1122'3344ULL, "read thread0 nxdata1");
    apb_read(sim, addr(kNxData0, 1), 0x0000'0000'4433'2211ULL, "read thread1 nxdata0");
    apb_read(sim, addr(kNxData1, 1), 0x0000'0000'8877'6655ULL, "read thread1 nxdata1");

    apb_write_nonexec(sim, addr(kNxData0, 0), 0xaaaa'bbbb'dead'beefULL, 0, kNxData0,
                      "write thread0 nxdata0");
    sim.check(sim.dut->read_ddata0_t0_o == 0x1122'3344'dead'beefULL,
              "thread0 low word write updates only ddata0[31:0]");

    apb_write_nonexec(sim, addr(kNxData1, 0), 0x1357'2468'caf0'1234ULL, 0, kNxData1,
                      "write thread0 nxdata1");
    sim.check(sim.dut->read_ddata0_t0_o == 0xcaf0'1234'dead'beefULL,
              "thread0 high word write updates only ddata0[63:32]");

    apb_write_nonexec(sim, addr(kNxData0, 1), 0xffff'ffff'0102'0304ULL, 1, kNxData0,
                      "write thread1 nxdata0");
    sim.check(sim.dut->read_ddata0_t0_o == 0xcaf0'1234'dead'beefULL,
              "thread1 write leaves thread0 ddata0 unchanged");
    sim.check(sim.dut->read_ddata0_t1_o == 0x8877'6655'0102'0304ULL,
              "thread1 low word write updates selected thread");

    apb_write_nonexec(sim, addr(kNxData1, 1), 0x0bad'c0de'face'b00cULL, 1, kNxData1,
                      "write thread1 nxdata1");
    sim.check(sim.dut->read_ddata0_t1_o == 0xface'b00c'0102'0304ULL,
              "thread1 high word write updates selected thread");

    sim.dut->in_debug_mode_i = 0x1;
    apb_write_nonexec(sim, addr(kNxData0, 1), 0x0000'0000'a5a5'a5a5ULL, 1, kNxData0,
                      "debug disabled blocks thread1 ddata write", true);
    sim.check(sim.dut->read_ddata0_t1_o == 0xface'b00c'0102'0304ULL,
              "debug-disabled write does not change ddata0");
    sim.dut->in_debug_mode_i = 0x3;

    apb_read(sim, addr(kNxData0, 0, kPpUmode), 0x0000'0000'dead'beefULL,
             "bad protection read reports error", true);
    apb_write_nonexec(sim, addr(kInvalidHi, 0), 0x0000'0000'1111'2222ULL, 0, kInvalidHi,
                      "invalid register write reports default error", true);
    sim.check(sim.dut->read_ddata0_t0_o == 0xcaf0'1234'dead'beefULL,
              "invalid write does not change ddata0");

    apb_write_nonexec(sim, addr(kNxProgBuf0, 0), 0x1111'2222'3333'4444ULL, 0, kNxProgBuf0,
                      "nxprogbuf0 write drives ffb word 2", false, 0b0100);
    apb_write_nonexec(sim, addr(kNxProgBuf1, 1), 0x5555'6666'7777'8888ULL, 1, kNxProgBuf1,
                      "nxprogbuf1 write drives ffb word 3", false, 0b1000);

    apb_write_exec(sim, addr(kAbsCmd, 0), 0x9999'aaaa'bbbb'ccccULL, 0, kAbsCmd,
                   "abscmd write drives ffb low words and execute", false, 0b0011);
    apb_write_exec(sim, addr(kAxProgBuf0, 0), 0xdddd'eeee'ffff'0001ULL, 0, kAxProgBuf0,
                   "axprogbuf0 write drives ffb word 2 and execute", false, 0b0100);
    apb_write_exec(sim, addr(kAxProgBuf1, 1), 0x2345'6789'abcd'ef01ULL, 1, kAxProgBuf1,
                   "axprogbuf1 write drives ffb word 3 and execute", false, 0b1000);

    apb_write_exec(sim, addr(kAxData0, 0), 0x0123'4567'7654'3210ULL, 0, kAxData0,
                   "axdata0 write updates low word and executes");
    sim.check(sim.dut->read_ddata0_t0_o == 0xcaf0'1234'7654'3210ULL,
              "axdata0 updates only low word after delayed ready");

    apb_write_exec(sim, addr(kAxData1, 1), 0xfedc'ba98'89ab'cdefULL, 1, kAxData1,
                   "axdata1 write updates high word and executes");
    sim.check(sim.dut->read_ddata0_t1_o == 0x89ab'cdef'0102'0304ULL,
              "axdata1 updates only high word after delayed ready");

    sim.dut->in_debug_mode_i = 0x1;
    apb_write_exec(sim, addr(kAxProgBuf0, 1), 0xaaaa'0000'bbbb'1111ULL, 1, kAxProgBuf0,
                   "debug disabled blocks ffb and execute", true, 0b0100);
    sim.dut->in_debug_mode_i = 0x3;

    // APB and CSR writes in the same cycle intentionally follow the RTL order:
    // CSR update_ddata0 wins because it is applied after the APB update loop.
    apb_setup(sim, addr(kNxData0, 0), true, 0x0000'0000'1111'1111ULL,
              "CSR override setup");
    apb_to_access(sim);
    sim.dut->csr_wdata_i = 0xfedc'ba98'7654'3210ULL;
    sim.dut->update_ddata0_i = 0x1;
    eval(sim);
    sim.check(sim.dut->apb_pready_o == 1, "CSR override APB write is otherwise ready");
    sim.check(sim.dut->apb_pslverr_o == 0, "CSR override APB write has no APB error");
    sim.tick();
    clear_sideband(sim);
    sim.dut->apb_psel_i = 0;
    sim.dut->apb_penable_i = 0;
    eval(sim);
    sim.tick();
    sim.check(sim.dut->read_ddata0_t0_o == 0xfedc'ba98'7654'3210ULL,
              "CSR update overrides same-cycle APB ddata0 write");

    return sim.finish();
}
