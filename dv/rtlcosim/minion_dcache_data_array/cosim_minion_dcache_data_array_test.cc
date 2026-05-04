// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_data_array vs minion_dcache_data_array.

#include "Vcosim_minion_dcache_data_array_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>

using DUT = Vcosim_minion_dcache_data_array_tb;

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

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
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
    return (uint64_t(0xC3000000u + (bank << 12) + row) << 32)
         | uint64_t(0x3C000000u + (bank << 12) + row);
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

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("bank0_read_data", sim.dut->orig_bank0_read_data_o, sim.dut->new_bank0_read_data_o);
    sim.compare("bank1_read_data", sim.dut->orig_bank1_read_data_o, sim.dut->new_bank1_read_data_o);
    sim.compare("bank2_read_data", sim.dut->orig_bank2_read_data_o, sim.dut->new_bank2_read_data_o);
    sim.compare("bank3_read_data", sim.dut->orig_bank3_read_data_o, sim.dut->new_bank3_read_data_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->eval();  // Settle wrapper fanout before the negedge write-data capture.
    sim.tick();
    sim.dut->eval();
    compare_outputs(sim);
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
    CosimCtrl<DUT> sim(argc, argv);
    std::array<ReadReq, kBanks> read_req{};
    std::array<WriteReq, kBanks> write_req{};

    drive_cycle(sim.dut.get(), false, read_req, false, false, write_req);
    sim.reset(2);
    sim.dut->eval();

    for (int bank = 0; bank < kBanks; ++bank) {
        write_req[bank] = write_req_for_row(0, uint8_t(bank), init_data(bank, 0), true, true);
    }
    drive_cycle(sim.dut.get(), false, read_req, true, false, write_req);
    tick_and_compare(sim);

    for (int row = 0; row < kRows; ++row) {
        for (int bank = 0; bank < kBanks; ++bank) {
            write_req[bank] =
                write_req_for_row(row, uint8_t(row + bank), init_data(bank, row), true, true);
        }
        drive_cycle(sim.dut.get(), false, read_req, true, true, write_req);
        tick_and_compare(sim);
    }

    printf("cosim test 1: directed reads and writes\n");
    for (int bank = 0; bank < kBanks; ++bank) {
        read_req[bank] = read_req_for_row((bank * 19) & 0x7F, uint8_t(bank * 5), true, true);
    }
    drive_cycle(sim.dut.get(), true, read_req, false, false, write_req);
    tick_and_compare(sim);

    read_req[0] = read_req_for_row(13, 0x03, true, false);
    read_req[1] = read_req_for_row(27, 0x07, false, true);
    read_req[2] = read_req_for_row(41, 0x0B, true, true);
    read_req[3] = read_req_for_row(55, 0x0F, false, false);
    drive_cycle(sim.dut.get(), true, read_req, false, false, write_req);
    tick_and_compare(sim);

    for (int bank = 0; bank < kBanks; ++bank) write_req[bank] = {};
    write_req[0] = write_req_for_row(63, 0x11, 0x0123456789ABCDEFULL, true, false);
    write_req[1] = write_req_for_row(74, 0x15, 0xFEDCBA9876543210ULL, false, true);
    write_req[2] = write_req_for_row(85, 0x19, 0x55AA00FF11EE22DDULL, true, true);
    drive_cycle(sim.dut.get(), false, read_req, true, false, write_req);
    tick_and_compare(sim);
    drive_cycle(sim.dut.get(), false, read_req, false, true, write_req);
    tick_and_compare(sim);

    read_req[0] = read_req_for_row(63, 0x1D, true, true);
    read_req[1] = read_req_for_row(74, 0x01, true, true);
    read_req[2] = read_req_for_row(85, 0x05, true, true);
    read_req[3] = read_req_for_row(55, 0x09, true, true);
    drive_cycle(sim.dut.get(), true, read_req, false, false, write_req);
    tick_and_compare(sim);

    write_req[3] = write_req_for_row(91, 0x0D, 0xCAFEBABE12345678ULL, true, true);
    drive_cycle(sim.dut.get(), false, read_req, true, false, write_req);
    tick_and_compare(sim);
    drive_cycle(sim.dut.get(), false, read_req, false, false, write_req);
    tick_and_compare(sim);
    write_req[3] = write_req_for_row(91, 0x11, 0x0BADF00DDEADBEEFULL, false, true);
    drive_cycle(sim.dut.get(), false, read_req, false, true, write_req);
    tick_and_compare(sim);
    read_req[3] = read_req_for_row(91, 0x15, true, true);
    drive_cycle(sim.dut.get(), true, read_req, false, false, write_req);
    tick_and_compare(sim);

    printf("cosim test 2: random stimulus\n");
    uint32_t seed = 0x23B4A7C1u;
    for (int cycle = 0; cycle < 4096; ++cycle) {
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
        tick_and_compare(sim);
    }

    return sim.finish();
}
