// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vintpipe_alu_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vintpipe_alu_tb;

namespace {

enum : uint8_t {
  kAluAdd     = 0b00000,
  kAluSl      = 0b00001,
  kAluSeq     = 0b00010,
  kAluSne     = 0b00011,
  kAluXor     = 0b00100,
  kAluSr      = 0b00101,
  kAluOr      = 0b00110,
  kAluAnd     = 0b00111,
  kAluPackb   = 0b01000,
  kAluBitmixb = 0b01001,
  kAluSub     = 0b01010,
  kAluSra     = 0b01011,
  kAluSlt     = 0b01100,
  kAluSge     = 0b01101,
  kAluSltu    = 0b01110,
  kAluSgeu    = 0b01111,
};

uint64_t sext32(uint32_t v) {
  return (v & 0x80000000u) ? (0xffffffff00000000ull | v) : static_cast<uint64_t>(v);
}

uint64_t adder_ref(uint8_t fn, uint64_t in1, uint64_t in2) {
  const bool is_sub = (fn & 0x08) != 0;
  return in1 + (is_sub ? ~in2 : in2) + (is_sub ? 1ull : 0ull);
}

uint64_t bitmixb_ref(uint64_t in1, uint64_t in2) {
  uint16_t result = 0;
  for (int result_bit = 0; result_bit < 16; ++result_bit) {
    uint8_t low_sel = 0;
    uint8_t high_sel = 8;
    for (int mask_bit = 0; mask_bit < result_bit; ++mask_bit) {
      if ((in1 >> mask_bit) & 1ull) {
        ++high_sel;
      } else {
        ++low_sel;
      }
    }
    const uint8_t sel = ((in1 >> result_bit) & 1ull) ? high_sel : low_sel;
    result |= static_cast<uint16_t>(((in2 >> sel) & 1ull) << result_bit);
  }
  return result;
}

uint64_t out_ref(uint8_t dw, uint8_t fn, uint64_t in1, uint64_t in2) {
  const uint64_t add = adder_ref(fn, in1, in2);
  uint64_t out = 0;

  switch (fn) {
    case kAluAdd:
    case kAluSub:
      out = add;
      break;
    case kAluSeq:
      out = (in1 == in2) ? 1 : 0;
      break;
    case kAluSne:
      out = (in1 != in2) ? 1 : 0;
      break;
    case kAluSlt:
      out = (static_cast<int64_t>(in1) < static_cast<int64_t>(in2)) ? 1 : 0;
      break;
    case kAluSge:
      out = (static_cast<int64_t>(in1) >= static_cast<int64_t>(in2)) ? 1 : 0;
      break;
    case kAluSltu:
      out = (in1 < in2) ? 1 : 0;
      break;
    case kAluSgeu:
      out = (in1 >= in2) ? 1 : 0;
      break;
    case kAluXor:
      out = in1 ^ in2;
      break;
    case kAluOr:
      out = in1 | in2;
      break;
    case kAluAnd:
      out = in1 & in2;
      break;
    case kAluSl:
      out = dw ? (in1 << (in2 & 0x3f)) : sext32(static_cast<uint32_t>(in1) << (in2 & 0x1f));
      break;
    case kAluSr:
      out = dw ? (in1 >> (in2 & 0x3f)) : sext32(static_cast<uint32_t>(in1) >> (in2 & 0x1f));
      break;
    case kAluSra:
      out = dw ? static_cast<uint64_t>(static_cast<int64_t>(in1) >> (in2 & 0x3f))
               : sext32(static_cast<uint32_t>(static_cast<int32_t>(in1) >> (in2 & 0x1f)));
      break;
    case kAluPackb:
      out = ((in2 & 0xffull) << 8) | (in1 & 0xffull);
      break;
    case kAluBitmixb:
      out = bitmixb_ref(in1, in2);
      break;
    default:
      out = 0;
      break;
  }

  if (!dw) {
    out = sext32(static_cast<uint32_t>(out));
  }
  return out;
}

void check_alu(SimCtrl<DUT>& sim, const char* name, uint8_t dw, uint8_t fn,
               uint64_t in1, uint64_t in2) {
  sim.dut->dw_i = dw;
  sim.dut->fn_i = fn;
  sim.dut->in1_i = in1;
  sim.dut->in2_i = in2;
  sim.dut->eval();

  const uint64_t exp_out = out_ref(dw, fn, in1, in2);
  const uint64_t exp_adder = adder_ref(fn, in1, in2);

  char msg[256];
  std::snprintf(msg, sizeof(msg),
                "%s out dw=%u fn=0x%02x in1=0x%016llx in2=0x%016llx got=0x%016llx exp=0x%016llx",
                name, dw, fn, static_cast<unsigned long long>(in1),
                static_cast<unsigned long long>(in2),
                static_cast<unsigned long long>(sim.dut->out_o),
                static_cast<unsigned long long>(exp_out));
  sim.check(sim.dut->out_o == exp_out, msg);

  std::snprintf(msg, sizeof(msg),
                "%s adder dw=%u fn=0x%02x got=0x%016llx exp=0x%016llx",
                name, dw, fn,
                static_cast<unsigned long long>(sim.dut->adder_out_o),
                static_cast<unsigned long long>(exp_adder));
  sim.check(sim.dut->adder_out_o == exp_adder, msg);
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.dut->dw_i = 0;
  sim.dut->fn_i = 0;
  sim.dut->in1_i = 0;
  sim.dut->in2_i = 0;
  sim.reset(2);

  check_alu(sim, "add64_carry", 1, kAluAdd, 0xffffffffffffffffull, 0x2ull);
  check_alu(sim, "add32_sign_extend", 0, kAluAdd, 0x000000007fffffffull, 0x1ull);
  check_alu(sim, "sub64", 1, kAluSub, 0x1000ull, 0x1ull);
  check_alu(sim, "sub32_negative", 0, kAluSub, 0x0ull, 0x1ull);
  check_alu(sim, "seq_equal", 1, kAluSeq, 0x123456789abcdef0ull, 0x123456789abcdef0ull);
  check_alu(sim, "seq_not_equal", 1, kAluSeq, 0x1234ull, 0x5678ull);
  check_alu(sim, "sne_not_equal", 1, kAluSne, 0x1234ull, 0x5678ull);
  check_alu(sim, "slt_signed", 1, kAluSlt, 0x8000000000000000ull, 0x1ull);
  check_alu(sim, "sge_signed", 1, kAluSge, 0x7fffffffffffffffull, 0xffffffffffffffffull);
  check_alu(sim, "sltu_unsigned", 1, kAluSltu, 0x1ull, 0xffffffffffffffffull);
  check_alu(sim, "sgeu_unsigned", 1, kAluSgeu, 0xffffffffffffffffull, 0x1ull);
  check_alu(sim, "xor", 1, kAluXor, 0x55aa55aa00ff00ffull, 0x0f0ff0f0ff00ff00ull);
  check_alu(sim, "or", 1, kAluOr, 0x0000ffff0000ffffull, 0xffff0000ffff0000ull);
  check_alu(sim, "and", 1, kAluAnd, 0x0f0f0f0fffffffffull, 0x333333330000ffffull);
  check_alu(sim, "sll64", 1, kAluSl, 0x0000000000000003ull, 40);
  check_alu(sim, "sll32_sign_extend", 0, kAluSl, 0x0000000000000001ull, 31);
  check_alu(sim, "srl64", 1, kAluSr, 0x8000000000000000ull, 4);
  check_alu(sim, "srl32_sign_extend", 0, kAluSr, 0x0000000080000000ull, 1);
  check_alu(sim, "sra64", 1, kAluSra, 0x8000000000000000ull, 4);
  check_alu(sim, "sra32", 0, kAluSra, 0x0000000080000000ull, 4);
  check_alu(sim, "packb", 1, kAluPackb, 0x123456789abcde11ull, 0xfedcba9876543222ull);
  check_alu(sim, "bitmixb", 1, kAluBitmixb, 0x000000000000a55aull, 0x0000000000f0c33cull);

  return sim.finish();
}
