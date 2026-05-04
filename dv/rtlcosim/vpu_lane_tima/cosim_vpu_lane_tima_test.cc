// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_lane_tima_tb.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vcosim_vpu_lane_tima_tb;

namespace {

struct TimaInputs {
    bool f1_mul_en = false;
    uint32_t f1_in1 = 0;
    uint32_t f1_in2 = 0;
    bool f1_in1_signed = false;
    bool f1_in2_signed = false;
    uint32_t f2_in3 = 0;
    bool f2_zap_opd3 = false;
    bool f2_tima_en = false;
    bool f2_in3_en = false;
};

struct LaneInputs {
    bool te = false;
    bool tima_clk_en = false;
    bool f1_tima_mul_en = false;
    bool tima_in1_signed = false;
    bool tima_in2_signed = false;
    bool f2_tima_en = false;
    bool f2_tima_in3_en = false;
    bool f2_tima_zap_opdc = false;
    uint32_t tena_rd_data = 0;
    bool tenb_wr_en = false;
    bool tenb_wr_en_early = false;
    uint32_t tenb_rd_addr = 0;
    uint32_t tenb_wr_addr = 0;
    uint32_t tenb_wr_data = 0;
    bool tenc_wr_en = false;
    bool tenc_wr_en_early = false;
    uint32_t tenc_rd_addr = 0;
    uint32_t tenc_wr_addr = 0;
};

struct TimaModel {
    uint32_t f2_in1 = 0;
    uint32_t f2_in2 = 0;
    std::array<bool, 4> f2_in1_sign{};
    std::array<bool, 4> f2_in2_sign{};
    int32_t f3_prod_add = 0;
    uint32_t f3_in3 = 0;
    bool f3_zap_opd3 = false;

    static int32_t lane_operand(uint8_t byte, bool is_signed) {
        return is_signed ? static_cast<int32_t>(static_cast<int8_t>(byte))
                         : static_cast<int32_t>(byte);
    }

    static int32_t prod_add(uint32_t in1, uint32_t in2,
                            const std::array<bool, 4>& in1_sign,
                            const std::array<bool, 4>& in2_sign) {
        int32_t sum = 0;
        for (int lane = 0; lane < 4; ++lane) {
            const uint8_t byte1 = static_cast<uint8_t>((in1 >> (lane * 8)) & 0xffu);
            const uint8_t byte2 = static_cast<uint8_t>((in2 >> (lane * 8)) & 0xffu);
            sum += lane_operand(byte1, in1_sign[lane]) * lane_operand(byte2, in2_sign[lane]);
        }
        return sum;
    }

    void step(const TimaInputs& in) {
        const int32_t current_prod_add = prod_add(f2_in1, f2_in2, f2_in1_sign, f2_in2_sign);

        if (in.f2_tima_en) {
            f3_prod_add = current_prod_add;
        }
        if (in.f2_in3_en) {
            f3_in3 = in.f2_in3;
        }
        f3_zap_opd3 = in.f2_zap_opd3;

        if (in.f1_mul_en) {
            f2_in1 = in.f1_in1;
            f2_in2 = in.f1_in2;
            for (int lane = 0; lane < 4; ++lane) {
                f2_in1_sign[lane] = in.f1_in1_signed && (((in.f1_in1 >> (lane * 8 + 7)) & 1u) != 0u);
                f2_in2_sign[lane] = in.f1_in2_signed && (((in.f1_in2 >> (lane * 8 + 7)) & 1u) != 0u);
            }
        }
    }

    uint32_t output() const {
        const uint32_t in3_zapped = f3_zap_opd3 ? f3_in3 : 0u;
        return static_cast<uint32_t>(f3_prod_add) + in3_zapped;
    }
};

struct LaneModel {
    template <size_t Depth>
    struct RfModel {
        std::array<uint32_t, Depth> mem{};
        uint32_t wr_data_del = 0;

        void low_phase(bool wr_data_en, uint32_t wr_data) {
            if (wr_data_en) {
                wr_data_del = wr_data;
            }
        }

        void high_phase(bool wr_en, uint32_t wr_addr) {
            if (wr_en) {
                mem[wr_addr % Depth] = wr_data_del;
            }
        }

        uint32_t read(uint32_t rd_addr) const {
            return mem[rd_addr % Depth];
        }
    };

    RfModel<4> tenb_rf;
    RfModel<16> tenc_rf;
    TimaModel tima;
    bool tima_clk_latched = false;

    void low_phase(const LaneInputs& in) {
        tenb_rf.low_phase(in.tenb_wr_en_early, in.tenb_wr_data);
        tenc_rf.low_phase(in.tenc_wr_en_early, tima.output());
        tima_clk_latched = in.tima_clk_en;
    }

    void high_phase(const LaneInputs& in) {
        const uint32_t tenb_before = tenb_rf.read(in.tenb_rd_addr);
        const uint32_t tenc_before = tenc_rf.read(in.tenc_rd_addr);
        tenb_rf.high_phase(in.tenb_wr_en, in.tenb_wr_addr);
        tenc_rf.high_phase(in.tenc_wr_en, in.tenc_wr_addr);
        if (tima_clk_latched) {
            TimaInputs tima_in;
            tima_in.f1_mul_en = in.f1_tima_mul_en;
            tima_in.f1_in1 = in.tena_rd_data;
            tima_in.f1_in2 = tenb_before;
            tima_in.f1_in1_signed = in.tima_in1_signed;
            tima_in.f1_in2_signed = in.tima_in2_signed;
            tima_in.f2_in3 = tenc_before;
            tima_in.f2_zap_opd3 = !in.f2_tima_zap_opdc;
            tima_in.f2_tima_en = in.f2_tima_en;
            tima_in.f2_in3_en = in.f2_tima_in3_en;
            tima.step(tima_in);
        }
    }

    uint32_t tenb_out(uint32_t rd_addr) const {
        return tenb_rf.read(rd_addr);
    }

    uint32_t tima_out() const {
        return tima.output();
    }
};

struct Sim {
    DUT* dut;
    uint64_t sim_time = 0;
    int errors = 0;
    int checks = 0;
    int mismatches = 0;
    int comparisons = 0;

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

    void compare(const std::string& phase) {
        comparisons += 2;
        if (dut->orig_tenb_out_o != dut->new_tenb_out_o) {
            mismatches++;
            std::printf("  MISMATCH [t=%lu] %s tenb_out: orig=0x%08x new=0x%08x\n",
                        static_cast<unsigned long>(sim_time), phase.c_str(),
                        dut->orig_tenb_out_o, dut->new_tenb_out_o);
        }
        if (dut->orig_tima_out_o != dut->new_tima_out_o) {
            mismatches++;
            std::printf("  MISMATCH [t=%lu] %s tima_out: orig=0x%08x new=0x%08x\n",
                        static_cast<unsigned long>(sim_time), phase.c_str(),
                        dut->orig_tima_out_o, dut->new_tima_out_o);
        }
    }

    void check(bool cond, const std::string& msg) {
        checks++;
        if (!cond) {
            std::printf("  FAIL [t=%lu]: %s\n", static_cast<unsigned long>(sim_time), msg.c_str());
            errors++;
        }
    }

    int finish() {
        std::printf("\n%d comparisons, %d mismatches\n", comparisons, mismatches);
        if (mismatches || errors) {
            std::printf("COSIM FAILED: %d mismatch(es), %d error(s)\n", mismatches, errors);
            return 1;
        }
        std::printf("COSIM PASSED\n");
        return 0;
    }
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void drive_inputs(Sim& sim, const LaneInputs& in) {
    sim.dut->te_i = in.te;
    sim.dut->tima_clk_en_i = in.tima_clk_en;
    sim.dut->f1_tima_mul_en_i = in.f1_tima_mul_en;
    sim.dut->tima_in1_signed_i = in.tima_in1_signed;
    sim.dut->tima_in2_signed_i = in.tima_in2_signed;
    sim.dut->f2_tima_en_i = in.f2_tima_en;
    sim.dut->f2_tima_in3_en_i = in.f2_tima_in3_en;
    sim.dut->f2_tima_zap_opdc_i = in.f2_tima_zap_opdc;
    sim.dut->tena_rd_data_i = in.tena_rd_data;
    sim.dut->tenb_wr_en_i = in.tenb_wr_en;
    sim.dut->tenb_wr_en_early_i = in.tenb_wr_en_early;
    sim.dut->tenb_rd_addr_i = in.tenb_rd_addr;
    sim.dut->tenb_wr_addr_i = in.tenb_wr_addr;
    sim.dut->tenb_wr_data_i = in.tenb_wr_data;
    sim.dut->tenc_wr_en_i = in.tenc_wr_en;
    sim.dut->tenc_wr_en_early_i = in.tenc_wr_en_early;
    sim.dut->tenc_rd_addr_i = in.tenc_rd_addr;
    sim.dut->tenc_wr_addr_i = in.tenc_wr_addr;
}

void run_cycle(Sim& sim, LaneModel& model, const LaneInputs& in, const std::string& label) {
    drive_inputs(sim, in);
    model.low_phase(in);
    sim.low();
    sim.compare(label + " low");
    sim.check(sim.dut->new_tenb_out_o == model.tenb_out(in.tenb_rd_addr), label + " low tenb_out");
    sim.check(sim.dut->new_tima_out_o == model.tima_out(), label + " low tima_out");
    model.high_phase(in);
    sim.high();
    sim.compare(label + " high");
    sim.check(sim.dut->new_tenb_out_o == model.tenb_out(in.tenb_rd_addr), label + " high tenb_out");
    sim.check(sim.dut->new_tima_out_o == model.tima_out(), label + " high tima_out");
}

}  // namespace

int main(int argc, char** argv) {
    Sim sim(argc, argv);
    LaneModel model;

    sim.dut->clk_i = 0;
    drive_inputs(sim, LaneInputs{});
    sim.dut->eval();

    LaneInputs in;
    in.tima_clk_en = true;
    run_cycle(sim, model, in, "prime");

    in = {};
    in.tenb_wr_en = true;
    in.tenb_wr_en_early = true;
    in.tenb_wr_addr = 1;
    in.tenb_rd_addr = 1;
    in.tenb_wr_data = 0x01010101u;
    run_cycle(sim, model, in, "write tensorb entry");

    in = {};
    in.tima_clk_en = true;
    in.f1_tima_mul_en = true;
    in.tena_rd_data = 0x04030201u;
    in.tenb_rd_addr = 1;
    run_cycle(sim, model, in, "load tima operands");

    in = {};
    in.tima_clk_en = true;
    in.f2_tima_en = true;
    in.f2_tima_in3_en = true;
    in.f2_tima_zap_opdc = false;
    run_cycle(sim, model, in, "capture tima result");

    in = {};
    in.tenb_wr_en_early = true;
    in.tenb_wr_addr = 2;
    in.tenb_wr_data = 0x11223344u;
    run_cycle(sim, model, in, "capture stale tensorb write data");

    in = {};
    in.tenb_wr_en = true;
    in.tenb_wr_addr = 2;
    in.tenb_rd_addr = 2;
    in.tenb_wr_data = 0x55667788u;
    run_cycle(sim, model, in, "commit stale tensorb write data");

    in = {};
    in.tenc_wr_en_early = true;
    in.tenc_wr_addr = 4;
    run_cycle(sim, model, in, "capture stale tensorc write data");

    in = {};
    in.tima_clk_en = true;
    in.f1_tima_mul_en = true;
    in.tena_rd_data = 0x02020202u;
    in.tenb_rd_addr = 1;
    run_cycle(sim, model, in, "reload tima operands");

    in = {};
    in.tima_clk_en = true;
    in.f2_tima_en = true;
    in.f2_tima_in3_en = true;
    in.f2_tima_zap_opdc = true;
    run_cycle(sim, model, in, "update tima output after stale capture");

    in = {};
    in.tenc_wr_en = true;
    in.tenc_wr_addr = 4;
    in.tenc_rd_addr = 4;
    run_cycle(sim, model, in, "commit stale tensorc write data");

    bool saw_te_0 = false;
    bool saw_te_1 = false;
    bool saw_clk_en_0 = false;
    bool saw_clk_en_1 = false;
    bool saw_f1_mul_0 = false;
    bool saw_f1_mul_1 = false;
    bool saw_in1_signed_0 = false;
    bool saw_in1_signed_1 = false;
    bool saw_in2_signed_0 = false;
    bool saw_in2_signed_1 = false;
    bool saw_f2_en_0 = false;
    bool saw_f2_en_1 = false;
    bool saw_f2_in3_en_0 = false;
    bool saw_f2_in3_en_1 = false;
    bool saw_f2_zap_opdc_0 = false;
    bool saw_f2_zap_opdc_1 = false;
    bool saw_tenb_wr_en_0 = false;
    bool saw_tenb_wr_en_1 = false;
    bool saw_tenb_wr_en_early_0 = false;
    bool saw_tenb_wr_en_early_1 = false;
    bool saw_tenc_wr_en_0 = false;
    bool saw_tenc_wr_en_1 = false;
    bool saw_tenc_wr_en_early_0 = false;
    bool saw_tenc_wr_en_early_1 = false;
    // Directed setup cycles already exercise these buses at zero before the
    // random sweep starts.
    bool saw_tena_zero = true;
    bool saw_tena_nonzero = false;
    bool saw_tenb_rd_zero = false;
    bool saw_tenb_rd_nonzero = false;
    bool saw_tenb_wr_zero = false;
    bool saw_tenb_wr_nonzero = false;
    bool saw_tenb_data_zero = true;
    bool saw_tenb_data_nonzero = false;
    bool saw_tenc_rd_zero = false;
    bool saw_tenc_rd_nonzero = false;
    bool saw_tenc_wr_zero = false;
    bool saw_tenc_wr_nonzero = false;

    std::array<uint32_t, 2> seeds = {0x4a1e0f33u, 0xc2d75b91u};
    for (uint32_t& seed : seeds) {
        for (int i = 0; i < 25000; ++i) {
            LaneInputs rnd;
            rnd.te = (xorshift32(seed) & 1u) != 0u;
            rnd.tima_clk_en = (xorshift32(seed) & 1u) != 0u;
            rnd.f1_tima_mul_en = (xorshift32(seed) & 1u) != 0u;
            rnd.tima_in1_signed = (xorshift32(seed) & 1u) != 0u;
            rnd.tima_in2_signed = (xorshift32(seed) & 1u) != 0u;
            rnd.f2_tima_en = (xorshift32(seed) & 1u) != 0u;
            rnd.f2_tima_in3_en = (xorshift32(seed) & 1u) != 0u;
            rnd.f2_tima_zap_opdc = (xorshift32(seed) & 1u) != 0u;
            rnd.tena_rd_data = xorshift32(seed);
            rnd.tenb_wr_en = (xorshift32(seed) & 1u) != 0u;
            rnd.tenb_wr_en_early = rnd.tenb_wr_en ? ((xorshift32(seed) & 3u) != 0u) : ((xorshift32(seed) & 1u) != 0u);
            rnd.tenb_rd_addr = xorshift32(seed) & 3u;
            rnd.tenb_wr_addr = xorshift32(seed) & 3u;
            rnd.tenb_wr_data = xorshift32(seed);
            rnd.tenc_wr_en = (xorshift32(seed) & 1u) != 0u;
            rnd.tenc_wr_en_early = rnd.tenc_wr_en ? ((xorshift32(seed) & 3u) != 0u) : ((xorshift32(seed) & 1u) != 0u);
            rnd.tenc_rd_addr = xorshift32(seed) & 15u;
            rnd.tenc_wr_addr = xorshift32(seed) & 15u;

            saw_te_0 |= !rnd.te;
            saw_te_1 |= rnd.te;
            saw_clk_en_0 |= !rnd.tima_clk_en;
            saw_clk_en_1 |= rnd.tima_clk_en;
            saw_f1_mul_0 |= !rnd.f1_tima_mul_en;
            saw_f1_mul_1 |= rnd.f1_tima_mul_en;
            saw_in1_signed_0 |= !rnd.tima_in1_signed;
            saw_in1_signed_1 |= rnd.tima_in1_signed;
            saw_in2_signed_0 |= !rnd.tima_in2_signed;
            saw_in2_signed_1 |= rnd.tima_in2_signed;
            saw_f2_en_0 |= !rnd.f2_tima_en;
            saw_f2_en_1 |= rnd.f2_tima_en;
            saw_f2_in3_en_0 |= !rnd.f2_tima_in3_en;
            saw_f2_in3_en_1 |= rnd.f2_tima_in3_en;
            saw_f2_zap_opdc_0 |= !rnd.f2_tima_zap_opdc;
            saw_f2_zap_opdc_1 |= rnd.f2_tima_zap_opdc;
            saw_tenb_wr_en_0 |= !rnd.tenb_wr_en;
            saw_tenb_wr_en_1 |= rnd.tenb_wr_en;
            saw_tenb_wr_en_early_0 |= !rnd.tenb_wr_en_early;
            saw_tenb_wr_en_early_1 |= rnd.tenb_wr_en_early;
            saw_tenc_wr_en_0 |= !rnd.tenc_wr_en;
            saw_tenc_wr_en_1 |= rnd.tenc_wr_en;
            saw_tenc_wr_en_early_0 |= !rnd.tenc_wr_en_early;
            saw_tenc_wr_en_early_1 |= rnd.tenc_wr_en_early;
            saw_tena_zero |= rnd.tena_rd_data == 0u;
            saw_tena_nonzero |= rnd.tena_rd_data != 0u;
            saw_tenb_rd_zero |= rnd.tenb_rd_addr == 0u;
            saw_tenb_rd_nonzero |= rnd.tenb_rd_addr != 0u;
            saw_tenb_wr_zero |= rnd.tenb_wr_addr == 0u;
            saw_tenb_wr_nonzero |= rnd.tenb_wr_addr != 0u;
            saw_tenb_data_zero |= rnd.tenb_wr_data == 0u;
            saw_tenb_data_nonzero |= rnd.tenb_wr_data != 0u;
            saw_tenc_rd_zero |= rnd.tenc_rd_addr == 0u;
            saw_tenc_rd_nonzero |= rnd.tenc_rd_addr != 0u;
            saw_tenc_wr_zero |= rnd.tenc_wr_addr == 0u;
            saw_tenc_wr_nonzero |= rnd.tenc_wr_addr != 0u;

            run_cycle(sim, model, rnd, "random cycle");
        }
    }

    sim.check(saw_te_0 && saw_te_1, "te exercised both values");
    sim.check(saw_clk_en_0 && saw_clk_en_1, "tima_clk_en exercised both values");
    sim.check(saw_f1_mul_0 && saw_f1_mul_1, "f1_tima_mul_en exercised both values");
    sim.check(saw_in1_signed_0 && saw_in1_signed_1, "tima_in1_signed exercised both values");
    sim.check(saw_in2_signed_0 && saw_in2_signed_1, "tima_in2_signed exercised both values");
    sim.check(saw_f2_en_0 && saw_f2_en_1, "f2_tima_en exercised both values");
    sim.check(saw_f2_in3_en_0 && saw_f2_in3_en_1, "f2_tima_in3_en exercised both values");
    sim.check(saw_f2_zap_opdc_0 && saw_f2_zap_opdc_1, "f2_tima_zap_opdc exercised both values");
    sim.check(saw_tenb_wr_en_0 && saw_tenb_wr_en_1, "tenb_wr_en exercised both values");
    sim.check(saw_tenb_wr_en_early_0 && saw_tenb_wr_en_early_1, "tenb_wr_en_early exercised both values");
    sim.check(saw_tenc_wr_en_0 && saw_tenc_wr_en_1, "tenc_wr_en exercised both values");
    sim.check(saw_tenc_wr_en_early_0 && saw_tenc_wr_en_early_1, "tenc_wr_en_early exercised both values");
    sim.check(saw_tena_zero && saw_tena_nonzero, "tena_rd_data exercised zero and non-zero");
    sim.check(saw_tenb_rd_zero && saw_tenb_rd_nonzero, "tenb_rd_addr exercised zero and non-zero");
    sim.check(saw_tenb_wr_zero && saw_tenb_wr_nonzero, "tenb_wr_addr exercised zero and non-zero");
    sim.check(saw_tenb_data_zero && saw_tenb_data_nonzero, "tenb_wr_data exercised zero and non-zero");
    sim.check(saw_tenc_rd_zero && saw_tenc_rd_nonzero, "tenc_rd_addr exercised zero and non-zero");
    sim.check(saw_tenc_wr_zero && saw_tenc_wr_nonzero, "tenc_wr_addr exercised zero and non-zero");

    return sim.finish();
}
