// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_mul_div_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vprim_mul_div_tb;

namespace {

enum : uint8_t {
    FN_MUL    = 0b10000,
    FN_MULH   = 0b10001,
    FN_DIV    = 0b10100,
    FN_DIVU   = 0b10101,
    FN_REM    = 0b10110,
    FN_REMU   = 0b10111,
    FN_MULHSU = 0b11100,
    FN_MULHU  = 0b11110,
};

struct TestCase {
    const char* name;
    uint8_t fn;
    uint8_t dw;
    uint64_t in1;
    uint64_t in2;
    uint64_t exp;
    uint8_t dest;
};

constexpr std::array<TestCase, 20> kCases = {{
    {"mul64_2x3",          FN_MUL,    1, 0x0000000000000002ULL, 0x0000000000000003ULL, 0x0000000000000006ULL, 0x01},
    {"mulh64_neg1",        FN_MULH,   1, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x0000000000000000ULL, 0x02},
    {"mulhu64_big",        FN_MULHU,  1, 0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL, 0x121FA00AD77D7422ULL, 0x03},
    {"mulhsu64_neg",       FN_MULHSU, 1, 0xFFFFFFFFFFFFFFFEULL, 0x0000000000000003ULL, 0xFFFFFFFFFFFFFFFFULL, 0x04},
    {"div64_100_7",        FN_DIV,    1, 0x0000000000000064ULL, 0x0000000000000007ULL, 0x000000000000000EULL, 0x05},
    {"div64_minneg",       FN_DIV,    1, 0x8000000000000000ULL, 0xFFFFFFFFFFFFFFFFULL, 0x8000000000000000ULL, 0x06},
    {"divu64_big",         FN_DIVU,   1, 0x123456789ABCDEF0ULL, 0x0000000100000000ULL, 0x0000000012345678ULL, 0x07},
    {"divu64_by_zero",     FN_DIVU,   1, 0x0000000000000005ULL, 0x0000000000000000ULL, 0xFFFFFFFFFFFFFFFFULL, 0x08},
    {"rem64_100_7",        FN_REM,    1, 0x0000000000000064ULL, 0x0000000000000007ULL, 0x0000000000000002ULL, 0x09},
    {"rem64_minneg",       FN_REM,    1, 0x8000000000000000ULL, 0xFFFFFFFFFFFFFFFFULL, 0x0000000000000000ULL, 0x0A},
    {"remu64_big",         FN_REMU,   1, 0x123456789ABCDEF0ULL, 0x0000000100000000ULL, 0x000000009ABCDEF0ULL, 0x0B},
    {"remu64_by_zero",     FN_REMU,   1, 0x0000000000000005ULL, 0x0000000000000000ULL, 0x0000000000000005ULL, 0x0C},
    {"mul32_neg",          FN_MUL,    0, 0xFFFFFFFFFFFFFFFFULL, 0x0000000000000002ULL, 0xFFFFFFFFFFFFFFFEULL, 0x0D},
    {"mulh32_neg1",        FN_MULH,   0, 0xFFFFFFFF80000000ULL, 0x0000000000000002ULL, 0xFFFFFFFFFFFFFFFFULL, 0x0E},
    {"mulhu32_big",        FN_MULHU,  0, 0x00000000FFFFFFFFULL, 0x00000000FFFFFFFFULL, 0xFFFFFFFFFFFFFFFEULL, 0x0F},
    {"mulhsu32_mix",       FN_MULHSU, 0, 0xFFFFFFFF80000000ULL, 0x00000000FFFFFFFFULL, 0xFFFFFFFF80000000ULL, 0x10},
    {"div32_minneg",       FN_DIV,    0, 0xFFFFFFFF80000000ULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFF80000000ULL, 0x11},
    {"divu32_deadbeef",    FN_DIVU,   0, 0x00000000DEADBEEFULL, 0x0000000000001000ULL, 0x00000000000DEADBULL, 0x12},
    {"rem32_100_7",        FN_REM,    0, 0x0000000000000064ULL, 0x0000000000000007ULL, 0x0000000000000002ULL, 0x13},
    {"remu32_deadbeef",    FN_REMU,   0, 0x00000000DEADBEEFULL, 0x0000000000001000ULL, 0x0000000000000EEFULL, 0x14},
}};

void drive_idle(DUT* dut) {
    dut->req_valid_i = 0;
    dut->req_valid_early_i = 0;
    dut->req_fn_i = FN_MUL;
    dut->req_fn_early_i = FN_MUL;
    dut->req_dw_i = 0;
    dut->req_in1_i = 0;
    dut->req_in2_i = 0;
    dut->req_dest_i = 0;
    dut->kill_i = 0;
    dut->chicken_bit_mul_div_i = 0;
    dut->resp_ready_i = 1;
}

std::string hex64(uint64_t value) {
    char buf[19];
    std::snprintf(buf, sizeof(buf), "%016llX",
                  static_cast<unsigned long long>(value));
    return std::string(buf);
}

void wait_for_ready(SimCtrl<DUT>& sim, const std::string& name, int max_cycles = 64) {
    for (int i = 0; i < max_cycles; ++i) {
        if (sim.dut->req_ready_o) return;
        sim.tick();
    }
    sim.check(false, name + ": req_ready asserted before timeout");
}

void issue_request(SimCtrl<DUT>& sim, const TestCase& tc, bool resp_ready = true) {
    wait_for_ready(sim, tc.name);

    // Match the real leaf harness: observe req_ready in one cycle, then
    // present req_valid_early in the following cycle.
    sim.tick();

    sim.dut->req_valid_early_i = 1;
    sim.dut->req_valid_i = 0;
    sim.dut->req_fn_i = tc.fn;
    sim.dut->req_fn_early_i = tc.fn;
    sim.dut->req_dw_i = tc.dw;
    sim.dut->req_in1_i = tc.in1;
    sim.dut->req_in2_i = tc.in2;
    sim.dut->req_dest_i = tc.dest;
    sim.dut->kill_i = 0;
    sim.dut->resp_ready_i = resp_ready;
    sim.tick();

    sim.dut->req_valid_early_i = 0;
    sim.dut->req_valid_i = 1;
    sim.tick();

    sim.dut->req_valid_i = 0;
}

void run_and_check(SimCtrl<DUT>& sim, const TestCase& tc) {
    issue_request(sim, tc, false);

    int early_cycle = -1;
    int resp_cycle = -1;
    sim.tick();
    for (int cycle = 0; cycle < 256; ++cycle) {
        if (sim.dut->resp_valid_early_o && early_cycle < 0) early_cycle = cycle;
        if (sim.dut->resp_valid_o) {
            resp_cycle = cycle;
            sim.tick();
            sim.check(sim.dut->resp_valid_o, std::string(tc.name) + ": response holds until consumed");
            sim.check(sim.dut->resp_data_o == tc.exp,
                      std::string(tc.name) + ": response data matches got=" +
                          hex64(sim.dut->resp_data_o) + " exp=" + hex64(tc.exp));
            sim.check(sim.dut->resp_dest_o == tc.dest,
                      std::string(tc.name) + ": response dest matches");
            break;
        }
        sim.tick();
    }

    sim.check(resp_cycle >= 0, std::string(tc.name) + ": response observed");
    sim.check(early_cycle >= 0, std::string(tc.name) + ": resp_valid_early observed");
    if (early_cycle >= 0 && resp_cycle >= 0) {
        sim.check(early_cycle <= resp_cycle,
                  std::string(tc.name) + ": resp_valid_early arrives no later than resp_valid");
    }

    sim.dut->resp_ready_i = 1;
    sim.tick();
    drive_idle(sim.dut.get());
    sim.tick();
}

void run_stall_case(SimCtrl<DUT>& sim) {
    TestCase tc{
        "stall_divu64_big", FN_DIVU, 1,
        0x123456789ABCDEF0ULL, 0x0000000100000000ULL,
        0x0000000012345678ULL, 0x55
    };

    issue_request(sim, tc, false);

    int hold_cycles = 0;
    sim.tick();
    for (int cycle = 0; cycle < 256; ++cycle) {
        if (sim.dut->resp_valid_o) {
            sim.check(sim.dut->resp_data_o == tc.exp,
                      std::string("stall case: response data matches got=") +
                          hex64(sim.dut->resp_data_o) + " exp=" + hex64(tc.exp));
            sim.check(sim.dut->resp_dest_o == tc.dest, "stall case: response dest matches");
            break;
        }
        sim.tick();
    }

    while (hold_cycles < 3) {
        sim.check(sim.dut->resp_valid_o, "stall case: resp_valid holds while resp_ready is low");
        sim.check(sim.dut->resp_data_o == tc.exp, "stall case: response data stays stable");
        sim.check(sim.dut->resp_dest_o == tc.dest, "stall case: response dest stays stable");
        sim.tick();
        hold_cycles++;
    }

    sim.dut->resp_ready_i = 1;
    sim.tick();
    drive_idle(sim.dut.get());
    sim.tick();
    wait_for_ready(sim, "stall case recovery");
}

void run_kill_case(SimCtrl<DUT>& sim) {
    TestCase long_div{
        "kill_long_div", FN_DIV, 1,
        0x123456789ABCDEF0ULL, 0x0000000000000003ULL,
        0, 0x21
    };
    issue_request(sim, long_div, true);

    for (int i = 0; i < 5; ++i) sim.tick();

    sim.dut->kill_i = 1;
    sim.tick();
    sim.dut->kill_i = 0;

    wait_for_ready(sim, "kill case ready recovery", 32);

    TestCase recovery{
        "kill_recovery_mul", FN_MUL, 1,
        0x0000000000000007ULL, 0x000000000000000BULL,
        0x000000000000004DULL, 0x33
    };
    run_and_check(sim, recovery);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000000;

    drive_idle(sim.dut.get());
    sim.reset(10);

    for (const auto& tc : kCases) {
        run_and_check(sim, tc);
    }

    run_stall_case(sim);
    run_kill_case(sim);

    return sim.finish();
}
