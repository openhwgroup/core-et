// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vicache_data_array_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <initializer_list>

using DUT = Vicache_data_array_tb;

namespace {

constexpr uint32_t kAddrMask = (1u << 9) - 1u;

void clear_inputs(DUT* dut) {
  dut->f1_read_req_valid_i = 0;
  dut->f1_read_req_way_i = 0;
  dut->f1_read_req_set_i = 0;
  dut->f1_write_req_valid_i = 0;
  dut->f1_write_req_way_i = 0;
  dut->f1_write_req_set_i = 0;
  dut->f2_mem_req_ready_i = 0;
  dut->f0_mem_resp_valid_i = 0;
  dut->f0_mem_resp_mem0_lo_i = 0;
  dut->f0_mem_resp_mem0_hi_i = 0;
  dut->f0_mem_resp_mem0_ecc_i = 0;
  dut->f0_mem_resp_mem1_lo_i = 0;
  dut->f0_mem_resp_mem1_hi_i = 0;
  dut->f0_mem_resp_mem1_ecc_i = 0;
  dut->f0_mem_resp_mem2_lo_i = 0;
  dut->f0_mem_resp_mem2_hi_i = 0;
  dut->f0_mem_resp_mem2_ecc_i = 0;
  dut->f0_mem_resp_mem3_lo_i = 0;
  dut->f0_mem_resp_mem3_hi_i = 0;
  dut->f0_mem_resp_mem3_ecc_i = 0;
}

void set_mem_word(DUT* dut, int idx, uint64_t lo, uint64_t hi, uint16_t ecc) {
  switch (idx) {
    case 0:
      dut->f0_mem_resp_mem0_lo_i = lo;
      dut->f0_mem_resp_mem0_hi_i = hi;
      dut->f0_mem_resp_mem0_ecc_i = ecc;
      break;
    case 1:
      dut->f0_mem_resp_mem1_lo_i = lo;
      dut->f0_mem_resp_mem1_hi_i = hi;
      dut->f0_mem_resp_mem1_ecc_i = ecc;
      break;
    case 2:
      dut->f0_mem_resp_mem2_lo_i = lo;
      dut->f0_mem_resp_mem2_hi_i = hi;
      dut->f0_mem_resp_mem2_ecc_i = ecc;
      break;
    case 3:
      dut->f0_mem_resp_mem3_lo_i = lo;
      dut->f0_mem_resp_mem3_hi_i = hi;
      dut->f0_mem_resp_mem3_ecc_i = ecc;
      break;
    default:
      break;
  }
}

uint32_t encode_addr(uint32_t set, uint32_t way) {
  return ((set << 2) | way) & kAddrMask;
}

bool data_bit(uint64_t data, int idx) {
  return ((data >> idx) & 1ULL) != 0;
}

bool xor_bits(uint64_t data, std::initializer_list<int> idxs) {
  bool parity = false;
  for (int idx : idxs) parity ^= data_bit(data, idx);
  return parity;
}

uint8_t encode_ecc64(uint64_t data) {
  uint8_t ecc = 0;
  ecc |= static_cast<uint8_t>(xor_bits(data, {2,5,7,10,12,15,18,21,24,26,28,33,35,38,41,44,
                                              46,48,50,53,55,57,59,60,62,63})) << 0;
  ecc |= static_cast<uint8_t>(xor_bits(data, {2,4,7,10,13,15,17,20,24,25,29,32,36,37,40,44,
                                              47,49,51,52,54,57,58,60,62,63})) << 1;
  ecc |= static_cast<uint8_t>(xor_bits(data, {1,4,7,9,12,14,18,20,23,26,28,30,31,34,41,42,
                                              43,45,49,52,54,57,58,60,61,63})) << 2;
  ecc |= static_cast<uint8_t>(xor_bits(data, {1,4,6,9,12,15,16,19,22,25,27,33,35,38,39,40,
                                              43,45,47,51,55,56,58,60,61,63})) << 3;
  ecc |= static_cast<uint8_t>(xor_bits(data, {1,3,6,8,11,14,17,20,22,25,28,30,32,35,36,39,
                                              42,44,48,50,53,56,58,59,61,63})) << 4;
  ecc |= static_cast<uint8_t>(xor_bits(data, {0,3,6,9,11,14,17,19,21,24,27,29,31,34,37,40,
                                              41,46,49,53,55,56,58,59,61,62})) << 5;
  ecc |= static_cast<uint8_t>(xor_bits(data, {0,3,5,8,10,13,16,19,22,23,26,30,32,34,37,38,
                                              43,46,48,51,54,56,57,59,61,62})) << 6;
  ecc |= static_cast<uint8_t>(xor_bits(data, {0,2,5,8,11,13,16,18,21,23,27,29,31,33,36,39,
                                              42,45,47,50,52,56,57,59,60,62})) << 7;
  return ecc;
}

uint16_t encode_mem_ecc(uint64_t lo, uint64_t hi) {
  return static_cast<uint16_t>(encode_ecc64(lo))
       | (static_cast<uint16_t>(encode_ecc64(hi)) << 8);
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();

  clear_inputs(dut);
  sim.reset(2);
  clear_inputs(dut);
  dut->eval();

  sim.check(dut->f0_read_resp_early_valid_o == 0, "reset clears early valid");
  sim.check(dut->f0_read_resp_valid_o == 0, "reset clears response valid");
  sim.check(dut->f2_mem_req_valid_o == 0, "reset clears request valid");
  sim.check(dut->f0_mem_resp_ready_o == 1, "SRAM response path is always ready");

  dut->f1_read_req_valid_i = 1;
  dut->f1_read_req_way_i = 2;
  dut->f1_read_req_set_i = 0x55;
  dut->f2_mem_req_ready_i = 1;
  sim.tick();
  sim.check(dut->f2_mem_req_valid_o == 1, "read request issues into F2");
  sim.check(dut->f2_mem_req_write_o == 0, "read request clears write bit");
  sim.check(dut->f2_mem_req_addr_o == encode_addr(0x55, 2), "read request address encodes set and way");

  clear_inputs(dut);
  dut->f1_read_req_valid_i = 1;
  dut->f1_read_req_way_i = 3;
  dut->f1_read_req_set_i = 0x7f;
  dut->f1_write_req_valid_i = 1;
  dut->f1_write_req_way_i = 1;
  dut->f1_write_req_set_i = 0x12;
  dut->f2_mem_req_ready_i = 1;
  sim.tick();
  sim.check(dut->f2_mem_req_valid_o == 1, "write request also issues into F2");
  sim.check(dut->f2_mem_req_write_o == 1, "write request has priority over read");
  sim.check(dut->f2_mem_req_addr_o == encode_addr(0x12, 1), "write request address uses write set/way");

  clear_inputs(dut);
  dut->f2_mem_req_ready_i = 1;
  sim.tick();
  sim.check(dut->f2_mem_req_valid_o == 0, "previous request drains before stall tests");

  clear_inputs(dut);
  dut->f1_read_req_valid_i = 1;
  dut->f1_read_req_way_i = 1;
  dut->f1_read_req_set_i = 0x2a;
  dut->f2_mem_req_ready_i = 0;
  sim.tick();
  sim.check(dut->f2_mem_req_valid_o == 1, "empty F2 slot captures request even when downstream stalls");
  sim.check(dut->f2_mem_req_addr_o == encode_addr(0x2a, 1), "stalled request captured its address");

  clear_inputs(dut);
  dut->f1_read_req_valid_i = 1;
  dut->f1_read_req_way_i = 0;
  dut->f1_read_req_set_i = 0x01;
  dut->f2_mem_req_ready_i = 0;
  sim.tick();
  sim.check(dut->f2_mem_req_valid_o == 1, "occupied stalled slot remains valid");
  sim.check(dut->f2_mem_req_addr_o == encode_addr(0x2a, 1), "stalled request holds previous address");

  clear_inputs(dut);
  dut->f2_mem_req_ready_i = 1;
  sim.tick();
  sim.check(dut->f2_mem_req_valid_o == 0, "valid drops once downstream accepts bubble");

  clear_inputs(dut);
  dut->f0_mem_resp_valid_i = 1;
  set_mem_word(dut, 0, 0x0123456789abcdefULL, 0xfedcba9876543210ULL,
               encode_mem_ecc(0x0123456789abcdefULL, 0xfedcba9876543210ULL));
  set_mem_word(dut, 1, 0x1111111111111111ULL, 0x2222222222222222ULL,
               encode_mem_ecc(0x1111111111111111ULL, 0x2222222222222222ULL));
  set_mem_word(dut, 2, 0x3333333333333333ULL, 0x4444444444444444ULL,
               encode_mem_ecc(0x3333333333333333ULL, 0x4444444444444444ULL));
  set_mem_word(dut, 3, 0x5555555555555555ULL, 0x6666666666666666ULL,
               encode_mem_ecc(0x5555555555555555ULL, 0x6666666666666666ULL));
  sim.tick();
  sim.check(dut->f0_read_resp_early_valid_o == 1, "early valid asserts one cycle after SRAM valid");
  sim.check(dut->f0_read_resp_valid_o == 0, "final valid is still delayed by one cycle");

  clear_inputs(dut);
  sim.tick();
  sim.check(dut->f0_read_resp_early_valid_o == 0, "early valid drops after capture");
  sim.check(dut->f0_read_resp_valid_o == 1, "final valid asserts on corrected response");
  sim.check(dut->f0_read_resp_qw0_o == 0x0123456789abcdefULL, "qword0 matches clean SRAM data");
  sim.check(dut->f0_read_resp_qw1_o == 0xfedcba9876543210ULL, "qword1 matches clean SRAM data");
  sim.check(dut->f0_read_resp_qw2_o == 0x1111111111111111ULL, "qword2 matches clean SRAM data");
  sim.check(dut->f0_read_resp_qw7_o == 0x6666666666666666ULL, "qword7 matches clean SRAM data");
  sim.check(dut->f0_read_resp_sbe_per_block_o == 0, "clean response has no SBE flags");
  sim.check(dut->f0_read_resp_dbe_per_block_o == 0, "clean response has no DBE flags");

  clear_inputs(dut);
  dut->f0_mem_resp_valid_i = 1;
  set_mem_word(dut, 0, 1ULL, 0, 0);
  sim.tick();
  clear_inputs(dut);
  sim.tick();
  sim.check(dut->f0_read_resp_valid_o == 1, "single-bit response still produces valid");
  sim.check(dut->f0_read_resp_qw0_o == 0, "single-bit error is corrected back to zero");
  sim.check((dut->f0_read_resp_sbe_per_block_o & 0x1U) == 0x1U, "single-bit error flags SBE on block 0");
  sim.check((dut->f0_read_resp_dbe_per_block_o & 0x1U) == 0x0U, "single-bit error does not flag DBE");

  clear_inputs(dut);
  dut->f0_mem_resp_valid_i = 1;
  set_mem_word(dut, 0, 3ULL, 0, 0);
  sim.tick();
  clear_inputs(dut);
  sim.tick();
  sim.check(dut->f0_read_resp_valid_o == 1, "double-bit response still produces valid");
  sim.check(dut->f0_read_resp_qw0_o == 3ULL, "double-bit error remains uncorrected");
  sim.check((dut->f0_read_resp_sbe_per_block_o & 0x1U) == 0x0U, "double-bit error clears SBE");
  sim.check((dut->f0_read_resp_dbe_per_block_o & 0x1U) == 0x1U, "double-bit error flags DBE");

  return sim.finish();
}
