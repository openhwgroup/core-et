// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_tensor_store_buffer_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

namespace {
constexpr int kReqBits = 328;
constexpr int kReqWords = (kReqBits + 31) / 32;
constexpr int kBlockReqWords = (4 * kReqBits + 31) / 32;
constexpr int kTopReqWords = (8 * kReqBits + 31) / 32;
constexpr int kRspBits = 275;
constexpr int kIdOff = 320;
constexpr int kSourceOff = 316;
constexpr int kWdataOff = 315;
constexpr int kOpcodeOff = 310;
constexpr int kAddrOff = 270;
constexpr int kDataOff = 14;
constexpr int kSizeOff = 11;
constexpr int kQwenOff = 7;
constexpr int kSubopcodeOff = 0;
constexpr int kRspIdOff = 267;
constexpr int kRspOpcodeOff = 263;
constexpr int kRspSizeOff = 4;

void clear_words(WData* words, int nwords) {
  for (int i = 0; i < nwords; ++i) words[i] = 0;
}

void set_bits(WData* words, int bit, int width, uint64_t value) {
  for (int i = 0; i < width; ++i) {
    const int pos = bit + i;
    const uint32_t mask = uint32_t{1} << (pos & 31);
    if ((value >> i) & 1ull) words[pos >> 5] |= mask;
    else words[pos >> 5] &= ~mask;
  }
}

uint64_t get_bits(const WData* words, int bit, int width) {
  uint64_t value = 0;
  for (int i = 0; i < width; ++i) {
    const int pos = bit + i;
    if ((words[pos >> 5] >> (pos & 31)) & 1u) value |= uint64_t{1} << i;
  }
  return value;
}

void set_req(WData* words, int slot, uint32_t subopcode, uint32_t qwen,
             uint32_t size, uint64_t addr, uint64_t data0, uint64_t data1,
             uint64_t data2, uint64_t data3, uint32_t wdata = 1) {
  const int base = slot * kReqBits;
  set_bits(words, base + kSubopcodeOff, 7, subopcode);
  set_bits(words, base + kQwenOff, 4, qwen);
  set_bits(words, base + kSizeOff, 3, size);
  set_bits(words, base + kDataOff + 0, 64, data0);
  set_bits(words, base + kDataOff + 64, 64, data1);
  set_bits(words, base + kDataOff + 128, 64, data2);
  set_bits(words, base + kDataOff + 192, 64, data3);
  set_bits(words, base + kAddrOff, 40, addr);
  set_bits(words, base + kOpcodeOff, 5, 2);
  set_bits(words, base + kWdataOff, 1, wdata);
}

uint64_t req_field(const WData* req, int off, int width) {
  return get_bits(req, off, width);
}

uint64_t req_data64(const WData* req, int lane64) {
  return get_bits(req, kDataOff + lane64 * 64, 64);
}

uint64_t rsp_field(const WData* rsp_bits, int slot, int off, int width) {
  return get_bits(rsp_bits, slot * kRspBits + off, width);
}

void clear_all_inputs(Vneigh_tensor_store_buffer_tb* d) {
  clear_words(d->block_req_bits_i, kBlockReqWords);
  d->block_valid_i = 0;
  d->block_out_ready_i = 0;
  d->block_out_disable_next_i = 0;
  d->block_ack_i = 0;
  d->block_resp_ready_i = 0xf;

  clear_words(d->top_req_bits_i, kTopReqWords);
  d->top_valid_i = 0;
  d->top_out_ready_i = 0;
  d->top_out_disable_next_i = 0;
  d->top_ack_i = 0;
  d->top_resp_ready_i = 0xff;
}

bool wait_block_valid(SimCtrl<Vneigh_tensor_store_buffer_tb>& sim, int cycles) {
  for (int i = 0; i < cycles; ++i) {
    sim.tick();
    if (sim.dut->block_out_valid_o) return true;
  }
  return false;
}

bool wait_top_valid(SimCtrl<Vneigh_tensor_store_buffer_tb>& sim, int cycles) {
  for (int i = 0; i < cycles; ++i) {
    sim.tick();
    if (sim.dut->top_out_valid_o) return true;
  }
  return false;
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<Vneigh_tensor_store_buffer_tb> sim(argc, argv);
  sim.max_time = 200000;
  clear_all_inputs(sim.dut.get());
  sim.reset();
  auto* d = sim.dut.get();

  sim.check(d->block_out_valid_o == 0, "block output valid reset");
  sim.check(d->block_resp_valid_o == 0, "block slave response valid reset");
  sim.check(d->top_out_valid_o == 0, "top output valid reset");
  sim.check(d->top_resp_valid_o == 0, "top slave response valid reset");

  // 2-way QWord store: two minions merge into one HLine request and ACK returns to slave.
  set_req(d->block_req_bits_i, 0, 1, 0x1, 4, 0x1000, 0x100, 0x101, 0x102, 0x103);
  set_req(d->block_req_bits_i, 1, 1, 0x2, 4, 0x1000, 0x200, 0x201, 0x202, 0x203);
  d->block_valid_i = 0x3;
  sim.check(wait_block_valid(sim, 12), "block 2-way QWord produces output valid");
  sim.check(req_field(d->block_out_req_bits_o, kIdOff, 8) == 1, "block request id is tensor-store coop");
  sim.check(req_field(d->block_out_req_bits_o, kSourceOff, 4) == 0, "block source offset 0 source minion0");
  sim.check(req_field(d->block_out_req_bits_o, kOpcodeOff, 5) == 2, "block opcode is WriteAround");
  sim.check(req_field(d->block_out_req_bits_o, kSubopcodeOff, 7) == 0, "block subopcode is plain WriteAround");
  sim.check(req_field(d->block_out_req_bits_o, kSizeOff, 3) == 5, "block 2-way QWord emits HLine");
  sim.check(req_field(d->block_out_req_bits_o, kQwenOff, 4) == 0x3, "block 2-way QWord merges qwen");
  sim.check(req_data64(d->block_out_req_bits_o, 0) == 0x100, "block 2-way keeps master low data");
  sim.check(req_data64(d->block_out_req_bits_o, 2) == 0x202, "block 2-way takes slave upper data");
  d->block_out_ready_i = 1;
  d->eval();
  sim.check((d->block_ready_o & 0x3) == 0x3, "block 2-way ready returns to both minions");
  sim.tick();
  d->block_valid_i = 0;
  d->block_out_ready_i = 0;
  d->block_ack_i = 0x1;
  sim.tick();
  d->block_ack_i = 0;
  d->eval();
  sim.check((d->block_resp_valid_o & 0x2) == 0x2, "block 2-way ACK pending for slave minion");
  sim.check(rsp_field(d->block_resp_bits_o, 1, kRspIdOff, 8) == 1, "block slave ACK id");
  sim.check(rsp_field(d->block_resp_bits_o, 1, kRspOpcodeOff, 2) == 0, "block slave ACK opcode");
  sim.check(rsp_field(d->block_resp_bits_o, 1, kRspSizeOff, 3) == 0, "block slave ACK byte size");
  d->block_resp_ready_i = 0x2;
  sim.tick();
  d->block_resp_ready_i = 0xf;
  d->eval();
  sim.check((d->block_resp_valid_o & 0x2) == 0, "block slave ACK drains on ready");

  // 4-way QWord store: first pass combines minions 0/1, second pass combines 2/3.
  clear_words(d->block_req_bits_i, kBlockReqWords);
  for (int m = 0; m < 4; ++m) {
    set_req(d->block_req_bits_i, m, 2, 0x1u << m, 4, 0x2000,
            0x1000 + m*0x10, 0x1001 + m*0x10, 0x1002 + m*0x10, 0x1003 + m*0x10);
  }
  d->block_valid_i = 0xf;
  sim.check(wait_block_valid(sim, 12), "block 4-way QWord first pass valid");
  sim.check(d->block_out_valid_1st_o == 1, "block 4-way marks first pass");
  sim.check(req_field(d->block_out_req_bits_o, kSizeOff, 3) == 6, "block 4-way QWord emits Line");
  sim.check(req_data64(d->block_out_req_bits_o, 0) == 0x1000, "block 4-way first pass lower data from minion0");
  sim.check(req_data64(d->block_out_req_bits_o, 2) == 0x1012, "block 4-way first pass upper data from minion1");
  d->block_out_ready_i = 1;
  sim.tick();
  d->eval();
  sim.check(d->block_out_valid_2nd_o == 1, "block 4-way marks second pass");
  sim.check(req_data64(d->block_out_req_bits_o, 0) == 0x1020, "block 4-way second pass lower data from minion2");
  sim.check(req_data64(d->block_out_req_bits_o, 2) == 0x1032, "block 4-way second pass upper data from minion3");
  sim.check((d->block_ready_o & 0xf) == 0xf, "block 4-way ready returns to all minions on second pass");
  sim.tick();
  d->block_valid_i = 0;
  d->block_out_ready_i = 0;
  d->block_ack_i = 0x1;
  sim.tick();
  d->block_ack_i = 0;
  d->eval();
  sim.check((d->block_resp_valid_o & 0xe) == 0xe, "block 4-way ACK pending for three slave minions");
  d->block_resp_ready_i = 0xe;
  sim.tick();
  d->block_resp_ready_i = 0xf;

  // Top-level arbitration: north source 0 and south source 4 both make progress.
  clear_all_inputs(d);
  sim.reset();
  set_req(d->top_req_bits_i, 0, 1, 0x1, 4, 0x3000, 0x300, 0x301, 0x302, 0x303);
  set_req(d->top_req_bits_i, 1, 1, 0x2, 4, 0x3000, 0x400, 0x401, 0x402, 0x403);
  set_req(d->top_req_bits_i, 4, 1, 0x1, 4, 0x4000, 0x500, 0x501, 0x502, 0x503);
  set_req(d->top_req_bits_i, 5, 1, 0x2, 4, 0x4000, 0x600, 0x601, 0x602, 0x603);
  d->top_valid_i = 0x33;
  d->top_out_ready_i = 1;
  bool saw_north = false;
  bool saw_south = false;
  for (int cycle = 0; cycle < 48 && !(saw_north && saw_south); ++cycle) {
    d->eval();
    const uint8_t ready = d->top_ready_o;
    sim.tick();
    if (d->top_out_valid_o) {
      const uint32_t source = static_cast<uint32_t>(req_field(d->top_out_req_bits_o, kSourceOff, 4));
      if (source == 0) saw_north = true;
      if (source == 4) saw_south = true;
    }
    if (ready & 0x03) d->top_valid_i &= ~0x03u;
    if (ready & 0x30) d->top_valid_i &= ~0x30u;
  }
  sim.check(saw_north, "top arbitration emits north source 0 request");
  sim.check(saw_south, "top arbitration emits south source-offset 4 request");

  // Output-disable path suppresses visible valid without losing the queued request.
  clear_all_inputs(d);
  sim.reset();
  set_req(d->top_req_bits_i, 0, 1, 0x1, 4, 0x5000, 0x700, 0x701, 0x702, 0x703);
  set_req(d->top_req_bits_i, 1, 1, 0x2, 4, 0x5000, 0x800, 0x801, 0x802, 0x803);
  d->top_valid_i = 0x3;
  d->top_out_ready_i = 0;
  sim.check(wait_top_valid(sim, 20), "top has queued output before disable");
  d->top_out_disable_next_i = 1;
  sim.tick();
  d->top_out_disable_next_i = 0;
  d->eval();
  sim.check(d->top_out_valid_o == 0, "top output valid suppressed by registered disable");
  sim.tick();
  d->eval();
  sim.check(d->top_out_valid_o == 1, "top output valid returns after disable clears");
  d->top_out_ready_i = 1;
  sim.tick();

  return sim.finish();
}
