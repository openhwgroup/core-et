// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_data_array — Verilator test.

#include "Vminion_dcache_data_array_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vminion_dcache_data_array_tb;

namespace {

constexpr int kBanks = 4;
constexpr int kRows = 128;

struct ReadReq {
    bool valid_l = false;
    bool valid_h = false;
    uint8_t way = 0;
    uint16_t addr = 0;
};

struct WriteReq {
    bool valid_l = false;
    bool valid_h = false;
    uint8_t way = 0;
    uint16_t addr = 0;
    uint64_t data = 0;
};

struct ModelState {
    std::array<std::array<uint64_t, kRows>, kBanks> mem{};
    std::array<uint8_t, kBanks> read_addr_lo{};
    std::array<uint8_t, kBanks> read_addr_hi{};
    std::array<uint32_t, kBanks> capture_lo{};
    std::array<uint32_t, kBanks> capture_hi{};
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint8_t row_addr(uint16_t addr, uint8_t way) {
    const uint8_t set = (addr >> 6) & 0xF;
    const uint8_t idx = (addr >> 5) & 0x1;
    return uint8_t((set << 3) | (idx << 2) | (way & 0x3));
}

uint16_t addr_for_row(int row, uint8_t low_bits) {
    const uint16_t set = uint16_t((row >> 3) & 0xF);
    const uint16_t idx = uint16_t((row >> 2) & 0x1);
    return uint16_t((set << 6) | (idx << 5) | (low_bits & 0x1F));
}

uint8_t way_for_row(int row) {
    return uint8_t(row & 0x3);
}

uint64_t init_data(int bank, int row) {
    return (uint64_t(0xA5000000u + (bank << 12) + row) << 32)
         | uint64_t(0x5A000000u + (bank << 12) + row);
}

void set_read_req(DUT* dut, int bank, const ReadReq& req) {
    switch (bank) {
    case 0:
        dut->bank0_read_valid_l_i = req.valid_l;
        dut->bank0_read_valid_h_i = req.valid_h;
        dut->bank0_read_way_i = req.way;
        dut->bank0_read_addr_i = req.addr;
        return;
    case 1:
        dut->bank1_read_valid_l_i = req.valid_l;
        dut->bank1_read_valid_h_i = req.valid_h;
        dut->bank1_read_way_i = req.way;
        dut->bank1_read_addr_i = req.addr;
        return;
    case 2:
        dut->bank2_read_valid_l_i = req.valid_l;
        dut->bank2_read_valid_h_i = req.valid_h;
        dut->bank2_read_way_i = req.way;
        dut->bank2_read_addr_i = req.addr;
        return;
    case 3:
        dut->bank3_read_valid_l_i = req.valid_l;
        dut->bank3_read_valid_h_i = req.valid_h;
        dut->bank3_read_way_i = req.way;
        dut->bank3_read_addr_i = req.addr;
        return;
    default:
        return;
    }
}

void set_write_req(DUT* dut, int bank, const WriteReq& req) {
    switch (bank) {
    case 0:
        dut->bank0_write_valid_l_i = req.valid_l;
        dut->bank0_write_valid_h_i = req.valid_h;
        dut->bank0_write_way_i = req.way;
        dut->bank0_write_addr_i = req.addr;
        dut->bank0_write_data_i = req.data;
        return;
    case 1:
        dut->bank1_write_valid_l_i = req.valid_l;
        dut->bank1_write_valid_h_i = req.valid_h;
        dut->bank1_write_way_i = req.way;
        dut->bank1_write_addr_i = req.addr;
        dut->bank1_write_data_i = req.data;
        return;
    case 2:
        dut->bank2_write_valid_l_i = req.valid_l;
        dut->bank2_write_valid_h_i = req.valid_h;
        dut->bank2_write_way_i = req.way;
        dut->bank2_write_addr_i = req.addr;
        dut->bank2_write_data_i = req.data;
        return;
    case 3:
        dut->bank3_write_valid_l_i = req.valid_l;
        dut->bank3_write_valid_h_i = req.valid_h;
        dut->bank3_write_way_i = req.way;
        dut->bank3_write_addr_i = req.addr;
        dut->bank3_write_data_i = req.data;
        return;
    default:
        return;
    }
}

uint64_t get_read_data(const DUT* dut, int bank) {
    switch (bank) {
    case 0: return dut->bank0_read_data_o;
    case 1: return dut->bank1_read_data_o;
    case 2: return dut->bank2_read_data_o;
    case 3: return dut->bank3_read_data_o;
    default: return 0;
    }
}

void drive_cycle(DUT* dut,
                 bool s1_read_valid,
                 const std::array<ReadReq, kBanks>& read_req,
                 bool s3_write_valid,
                 bool s4_write_valid,
                 const std::array<WriteReq, kBanks>& write_req) {
    dut->s1_read_valid_i = s1_read_valid;
    dut->s3_write_valid_i = s3_write_valid;
    dut->s4_write_valid_i = s4_write_valid;
    for (int bank = 0; bank < kBanks; ++bank) {
        set_read_req(dut, bank, read_req[bank]);
        set_write_req(dut, bank, write_req[bank]);
    }
}

void tick_and_settle(SimCtrl<DUT>& sim) {
    sim.dut->eval();  // Settle wrapper fanout before the negedge capture.
    sim.tick();
    sim.dut->eval();  // Settle unpacked-array wrapper outputs after the posedge update.
}

std::array<uint64_t, kBanks> model_cycle(ModelState& model,
                                         bool s1_read_valid,
                                         const std::array<ReadReq, kBanks>& read_req,
                                         bool s3_write_valid,
                                         bool s4_write_valid,
                                         const std::array<WriteReq, kBanks>& write_req) {
    // Transparent latch: at negedge, current s3_write_valid gates the data capture.
    for (int bank = 0; bank < kBanks; ++bank) {
        if (s3_write_valid) {
            model.capture_lo[bank] = uint32_t(write_req[bank].data & 0xFFFFFFFFu);
            model.capture_hi[bank] = uint32_t((write_req[bank].data >> 32) & 0xFFFFFFFFu);
        }
    }

    for (int bank = 0; bank < kBanks; ++bank) {
        if (!s4_write_valid) continue;
        const uint8_t row = row_addr(write_req[bank].addr, write_req[bank].way);
        if (write_req[bank].valid_l) {
            model.mem[bank][row] =
                (model.mem[bank][row] & 0xFFFFFFFF00000000ULL) | model.capture_lo[bank];
        }
        if (write_req[bank].valid_h) {
            model.mem[bank][row] =
                (model.mem[bank][row] & 0x00000000FFFFFFFFULL) |
                (uint64_t(model.capture_hi[bank]) << 32);
        }
    }

    for (int bank = 0; bank < kBanks; ++bank) {
        if (!s1_read_valid) continue;
        const uint8_t row = row_addr(read_req[bank].addr, read_req[bank].way);
        if (read_req[bank].valid_l) model.read_addr_lo[bank] = row;
        if (read_req[bank].valid_h) model.read_addr_hi[bank] = row;
    }

    std::array<uint64_t, kBanks> expected{};
    for (int bank = 0; bank < kBanks; ++bank) {
        const uint64_t low_word = model.mem[bank][model.read_addr_lo[bank]];
        const uint64_t high_word = model.mem[bank][model.read_addr_hi[bank]];
        expected[bank] =
            (low_word & 0x00000000FFFFFFFFULL) | (high_word & 0xFFFFFFFF00000000ULL);
    }

    return expected;
}

void check_outputs(SimCtrl<DUT>& sim,
                   const std::array<uint64_t, kBanks>& expected,
                   const std::string& label) {
    for (int bank = 0; bank < kBanks; ++bank) {
        sim.check(get_read_data(sim.dut.get(), bank) == expected[bank],
                  label + " bank" + std::to_string(bank) +
                  " got=0x" + std::to_string(get_read_data(sim.dut.get(), bank)) +
                  " expect=0x" + std::to_string(expected[bank]));
    }
}

WriteReq write_req_for_row(int row, uint8_t low_bits, uint64_t data, bool valid_l, bool valid_h) {
    WriteReq req;
    req.valid_l = valid_l;
    req.valid_h = valid_h;
    req.way = way_for_row(row);
    req.addr = addr_for_row(row, low_bits);
    req.data = data;
    return req;
}

ReadReq read_req_for_row(int row, uint8_t low_bits, bool valid_l, bool valid_h) {
    ReadReq req;
    req.valid_l = valid_l;
    req.valid_h = valid_h;
    req.way = way_for_row(row);
    req.addr = addr_for_row(row, low_bits);
    return req;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    ModelState model;
    std::array<ReadReq, kBanks> read_req{};
    std::array<WriteReq, kBanks> write_req{};

    drive_cycle(sim.dut.get(), false, read_req, false, false, write_req);
    sim.reset(2);
    sim.dut->eval();

    printf("test 1: initialize all banks and rows\n");
    for (int bank = 0; bank < kBanks; ++bank) {
        write_req[bank] = write_req_for_row(0, uint8_t(bank), init_data(bank, 0), true, true);
    }
    drive_cycle(sim.dut.get(), false, read_req, true, false, write_req);
    model_cycle(model, false, read_req, true, false, write_req);
    tick_and_settle(sim);

    for (int row = 0; row < kRows; ++row) {
        for (int bank = 0; bank < kBanks; ++bank) {
            write_req[bank] =
                write_req_for_row(row, uint8_t(row + bank), init_data(bank, row), true, true);
        }
        drive_cycle(sim.dut.get(), false, read_req, true, true, write_req);
        model_cycle(model, false, read_req, true, true, write_req);
        tick_and_settle(sim);
    }

    for (int bank = 0; bank < kBanks; ++bank) {
        read_req[bank] = read_req_for_row((bank * 17) & 0x7F, uint8_t(bank * 3), true, true);
        write_req[bank] = {};
    }
    drive_cycle(sim.dut.get(), true, read_req, false, false, write_req);
    const auto init_expected = model_cycle(model, true, read_req, false, false, write_req);
    tick_and_settle(sim);
    check_outputs(sim, init_expected, "initial read");

    printf("test 2: directed partial reads and writes\n");
    read_req[0] = read_req_for_row(11, 0x03, true, false);
    read_req[1] = read_req_for_row(22, 0x07, false, true);
    read_req[2] = read_req_for_row(33, 0x0B, true, true);
    read_req[3] = read_req_for_row(44, 0x0F, false, false);
    drive_cycle(sim.dut.get(), true, read_req, false, false, write_req);
    auto expected = model_cycle(model, true, read_req, false, false, write_req);
    tick_and_settle(sim);
    check_outputs(sim, expected, "mixed read enables");

    for (int bank = 0; bank < kBanks; ++bank) write_req[bank] = {};
    write_req[0] = write_req_for_row(55, 0x13, 0x1122334455667788ULL, true, false);
    write_req[1] = write_req_for_row(66, 0x17, 0x99AABBCCDDEEFF00ULL, false, true);
    drive_cycle(sim.dut.get(), false, read_req, true, false, write_req);
    model_cycle(model, false, read_req, true, false, write_req);
    tick_and_settle(sim);

    drive_cycle(sim.dut.get(), false, read_req, false, true, write_req);
    model_cycle(model, false, read_req, false, true, write_req);
    tick_and_settle(sim);

    read_req[0] = read_req_for_row(55, 0x1B, true, true);
    read_req[1] = read_req_for_row(66, 0x1F, true, true);
    read_req[2] = read_req_for_row(33, 0x02, true, true);
    read_req[3] = read_req_for_row(44, 0x06, true, true);
    drive_cycle(sim.dut.get(), true, read_req, false, false, write_req);
    expected = model_cycle(model, true, read_req, false, false, write_req);
    tick_and_settle(sim);
    check_outputs(sim, expected, "partial write readback");

    for (int bank = 0; bank < kBanks; ++bank) write_req[bank] = {};
    write_req[2] = write_req_for_row(77, 0x09, 0xCAFEBABE12345678ULL, true, true);
    drive_cycle(sim.dut.get(), false, read_req, true, false, write_req);
    model_cycle(model, false, read_req, true, false, write_req);
    tick_and_settle(sim);

    drive_cycle(sim.dut.get(), false, read_req, false, false, write_req);
    model_cycle(model, false, read_req, false, false, write_req);
    tick_and_settle(sim);

    write_req[2] = write_req_for_row(77, 0x1D, 0x0BADF00DDEADBEEFULL, false, true);
    drive_cycle(sim.dut.get(), false, read_req, false, true, write_req);
    model_cycle(model, false, read_req, false, true, write_req);
    tick_and_settle(sim);

    read_req[0] = read_req_for_row(55, 0x01, true, true);
    read_req[1] = read_req_for_row(66, 0x05, true, true);
    read_req[2] = read_req_for_row(77, 0x09, true, true);
    read_req[3] = read_req_for_row(44, 0x0D, true, true);
    drive_cycle(sim.dut.get(), true, read_req, false, false, write_req);
    expected = model_cycle(model, true, read_req, false, false, write_req);
    tick_and_settle(sim);
    check_outputs(sim, expected, "stale write readback");

    printf("test 3: random data-array traffic\n");
    uint32_t seed = 0x5D3C91E2u;
    for (int cycle = 0; cycle < 1024; ++cycle) {
        const bool s1_read_valid = (xorshift32(seed) & 1u) != 0;
        const bool s3_write_valid = (xorshift32(seed) & 1u) != 0;
        const bool s4_write_valid = (xorshift32(seed) & 1u) != 0;

        for (int bank = 0; bank < kBanks; ++bank) {
            read_req[bank].valid_l = (xorshift32(seed) & 1u) != 0;
            read_req[bank].valid_h = (xorshift32(seed) & 1u) != 0;
            read_req[bank].way = xorshift32(seed) & 0x3u;
            read_req[bank].addr = xorshift32(seed) & 0x3FFu;

            write_req[bank].valid_l = (xorshift32(seed) & 1u) != 0;
            write_req[bank].valid_h = (xorshift32(seed) & 1u) != 0;
            write_req[bank].way = xorshift32(seed) & 0x3u;
            write_req[bank].addr = xorshift32(seed) & 0x3FFu;
            write_req[bank].data =
                (uint64_t(xorshift32(seed)) << 32) | uint64_t(xorshift32(seed));
        }

        drive_cycle(sim.dut.get(), s1_read_valid, read_req, s3_write_valid, s4_write_valid, write_req);
        expected = model_cycle(model, s1_read_valid, read_req, s3_write_valid, s4_write_valid, write_req);
        tick_and_settle(sim);
        check_outputs(sim, expected, "random cycle " + std::to_string(cycle));
    }

    return sim.finish();
}
