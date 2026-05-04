// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_top_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

using DUT = Vcosim_txfma_top_tb;

namespace {

constexpr uint64_t kVpuCtrlSigsMask = (1ull << 45) - 1ull;
constexpr uint64_t kTransCoeffMask = (1ull << 58) - 1ull;
constexpr uint64_t kImmMask = (1ull << 20) - 1ull;
constexpr uint64_t kRequiredCtrlMask =
    (1ull << 37) |  // txfma
    (1ull << 35) |  // rom
    (1ull << 33) |  // cvt
    (1ull << 32) |  // trans
    (1ull << 6)  |  // fma
    (1ull << 5)  |  // fms
    (1ull << 4)  |  // add
    (1ull << 3)  |  // sub
    (1ull << 2)  |  // mul
    (1ull << 1)  |  // round
    (1ull << 0);    // wflags

struct Request {
    bool valid = false;
    bool use_prev_sigs = false;
    uint32_t in1 = 0;
    uint32_t in2 = 0;
    uint32_t in3 = 0;
    uint64_t sigs = 0;
    uint32_t imm = 0;
    uint8_t rm = 0;
    uint8_t typ = 0;
    uint64_t coeff = 0;
};

struct InputCoverage {
    bool valid_0 = false;
    bool valid_1 = false;
    bool use_prev_0 = false;
    bool use_prev_1 = false;
    uint64_t in1_ones = 0, in1_zeros = 0;
    uint64_t in2_ones = 0, in2_zeros = 0;
    uint64_t in3_ones = 0, in3_zeros = 0;
    uint64_t sigs_ones = 0, sigs_zeros = 0;
    uint64_t imm_ones = 0, imm_zeros = 0;
    uint64_t rm_ones = 0, rm_zeros = 0;
    uint64_t typ_ones = 0, typ_zeros = 0;
    uint64_t coeff_ones = 0, coeff_zeros = 0;
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t random64(uint32_t& state) {
    const uint64_t lo = xorshift32(state);
    const uint64_t hi = xorshift32(state);
    return (hi << 32) | lo;
}

void drive_request(CosimCtrl<DUT>& sim, const Request& req) {
    sim.dut->in_valid_i = req.valid;
    sim.dut->use_prev_sigs_i = req.use_prev_sigs;
    sim.dut->in1_i = req.in1;
    sim.dut->in2_i = req.in2;
    sim.dut->in3_i = req.in3;
    sim.dut->sigs_i = req.sigs;
    sim.dut->imm_i = req.imm;
    sim.dut->rm_i = req.rm;
    sim.dut->typ_i = req.typ;
    sim.dut->trans_coeff_i = req.coeff;
}

void compare_all(CosimCtrl<DUT>& sim, const std::string& phase) {
    sim.compare("data " + phase, sim.dut->orig_data_o, sim.dut->new_data_o);
    sim.compare("exc " + phase, sim.dut->orig_exc_o, sim.dut->new_exc_o);
    sim.compare("comp " + phase, sim.dut->orig_comp_o, sim.dut->new_comp_o);
}

void eval_phase(CosimCtrl<DUT>& sim, bool clk, const std::string& phase) {
    sim.dut->clk_i = clk;
    sim.dut->eval();
    compare_all(sim, phase);
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        std::printf("TIMEOUT at sim_time=%lu\n", static_cast<unsigned long>(sim.sim_time));
        std::exit(2);
    }
}

void run_cycle(CosimCtrl<DUT>& sim, const Request& req, const std::string& label) {
    drive_request(sim, req);
    eval_phase(sim, false, label + " low");
    eval_phase(sim, true, label + " high");
}

void update_bool(bool& saw0, bool& saw1, bool v) {
    saw0 |= !v;
    saw1 |= v;
}

void update_mask(uint64_t& ones, uint64_t& zeros, uint64_t value, uint64_t mask) {
    ones |= value & mask;
    zeros |= (~value) & mask;
}

void observe(InputCoverage& cov, const Request& req) {
    update_bool(cov.valid_0, cov.valid_1, req.valid);
    update_bool(cov.use_prev_0, cov.use_prev_1, req.use_prev_sigs);
    update_mask(cov.in1_ones, cov.in1_zeros, req.in1, 0xffff'ffffull);
    update_mask(cov.in2_ones, cov.in2_zeros, req.in2, 0xffff'ffffull);
    update_mask(cov.in3_ones, cov.in3_zeros, req.in3, 0xffff'ffffull);
    update_mask(cov.sigs_ones, cov.sigs_zeros, req.sigs, kVpuCtrlSigsMask);
    update_mask(cov.imm_ones, cov.imm_zeros, req.imm, kImmMask);
    update_mask(cov.rm_ones, cov.rm_zeros, req.rm, 0x07ull);
    update_mask(cov.typ_ones, cov.typ_zeros, req.typ, 0x03ull);
    update_mask(cov.coeff_ones, cov.coeff_zeros, req.coeff, kTransCoeffMask);
}

void check_coverage(CosimCtrl<DUT>& sim, const InputCoverage& cov) {
    sim.check(cov.valid_0 && cov.valid_1, "in_valid exercised both values");
    sim.check(cov.use_prev_0 && cov.use_prev_1, "use_prev_sigs exercised both values");
    sim.check(cov.in1_ones == 0xffff'ffffull && cov.in1_zeros == 0xffff'ffffull,
              "in1 exercised all bits both ways");
    sim.check(cov.in2_ones == 0xffff'ffffull && cov.in2_zeros == 0xffff'ffffull,
              "in2 exercised all bits both ways");
    sim.check(cov.in3_ones == 0xffff'ffffull && cov.in3_zeros == 0xffff'ffffull,
              "in3 exercised all bits both ways");
    sim.check(cov.sigs_ones == kVpuCtrlSigsMask && cov.sigs_zeros == kVpuCtrlSigsMask,
              "vpu_ctrl_sigs exercised all packed bits both ways");
    sim.check((cov.sigs_ones & kRequiredCtrlMask) == kRequiredCtrlMask &&
                  (cov.sigs_zeros & kRequiredCtrlMask) == kRequiredCtrlMask,
              "txfma/rom/cvt/trans/fma/fms/add/sub/mul/round/wflags exercised both ways");
    sim.check(cov.imm_ones == kImmMask && cov.imm_zeros == kImmMask,
              "imm exercised all bits both ways");
    sim.check(cov.rm_ones == 0x07ull && cov.rm_zeros == 0x07ull,
              "rm exercised all bits both ways");
    sim.check(cov.typ_ones == 0x03ull && cov.typ_zeros == 0x03ull,
              "typ exercised all bits both ways");
    sim.check(cov.coeff_ones == kTransCoeffMask && cov.coeff_zeros == kTransCoeffMask,
              "trans_coeff exercised all 58 bits both ways");
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    sim.dut->rst_ni = 0;
    run_cycle(sim, {}, "reset");
    sim.dut->rst_ni = 1;
    Verilated::assertOn(true);

    run_cycle(sim, {}, "idle");

    InputCoverage cov;

    const Request all_zero{};
    Request all_one;
    all_one.valid = true;
    all_one.use_prev_sigs = true;
    all_one.in1 = 0xffff'ffffu;
    all_one.in2 = 0xffff'ffffu;
    all_one.in3 = 0xffff'ffffu;
    all_one.sigs = kVpuCtrlSigsMask;
    all_one.imm = static_cast<uint32_t>(kImmMask);
    all_one.rm = 0x7u;
    all_one.typ = 0x3u;
    all_one.coeff = kTransCoeffMask;

    observe(cov, all_zero);
    run_cycle(sim, all_zero, "directed zero");
    observe(cov, all_one);
    run_cycle(sim, all_one, "directed one");

    uint32_t rng = 1u;
    for (int cycle = 0; cycle < 2500; ++cycle) {
        Request req;
        const uint32_t r0 = xorshift32(rng);
        req.valid = (r0 & 1u) != 0u;
        req.use_prev_sigs = (r0 & 2u) != 0u;
        req.in1 = xorshift32(rng);
        req.in2 = xorshift32(rng);
        req.in3 = xorshift32(rng);
        req.sigs = random64(rng) & kVpuCtrlSigsMask;
        req.imm = static_cast<uint32_t>(xorshift32(rng) & kImmMask);
        req.rm = static_cast<uint8_t>((r0 >> 11) & 0x07u);
        req.typ = static_cast<uint8_t>((r0 >> 14) & 0x03u);
        req.coeff = random64(rng) & kTransCoeffMask;

        observe(cov, req);
        run_cycle(sim, req, "random " + std::to_string(cycle));
    }

    check_coverage(sim, cov);
    sim.check(sim.comparisons >= 15000,
              "txfma_top cosim should generate at least 15000 comparisons");

    return sim.finish();
}
