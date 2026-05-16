// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_channel_routing_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vneigh_channel_routing_tb;

namespace {

constexpr int kMinPerN = 8;
constexpr int kNrThreads = 2;
constexpr int kFlbWidth = 13;
constexpr int kMaxLocalConnect = 3;
constexpr int kNeighRspBits = 278;
constexpr int kNeighReqBits = 331;
constexpr int kRspBits = 534;
constexpr int kReqNoDataBits = 75;
constexpr int kPmuMinionCounters = 8;
constexpr int kPmuTotalCounters = 12;
constexpr int kPmuSelBits = 4;
constexpr int kPmuEventSelBits = 5;
constexpr int kPmuNeighCounters = 4;
constexpr int kPmuEventCntSelBits = kPmuEventSelBits * kPmuNeighCounters;

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void clear_words(WData* words, int nwords) {
  for (int i = 0; i < nwords; ++i) words[i] = 0;
}

void set_bits(WData* words, int pos, int width, uint64_t value) {
  for (int bit = 0; bit < width; ++bit) {
    const int idx = pos + bit;
    const uint32_t mask = uint32_t{1} << (idx & 31);
    if ((value >> bit) & 1ull) words[idx >> 5] |= mask;
    else words[idx >> 5] &= ~mask;
  }
}

uint64_t get_bits(const WData* words, int pos, int width) {
  uint64_t value = 0;
  for (int bit = 0; bit < width; ++bit) {
    const int idx = pos + bit;
    if ((words[idx >> 5] >> (idx & 31)) & 1u) value |= uint64_t{1} << bit;
  }
  return value;
}

void set_u64(WData* words, int pos, uint64_t value) {
  set_bits(words, pos, 32, value & 0xffffffffull);
  set_bits(words, pos + 32, 32, value >> 32);
}

uint64_t get_u64(const WData* words, int pos) {
  return get_bits(words, pos, 32) | (get_bits(words, pos + 32, 32) << 32);
}

void clear_inputs(DUT* d) {
  clear_words(d->flb_req_data_bits_i, 4);
  d->flb_req_valid_i = 0;
  d->flb_l2_resp_valid_i = 0;
  d->flb_l2_resp_data_i = 0;

  d->lmn_valid_out_bits_i = 0;
  clear_words(d->lmn_info_out_bits_i, 70);
  d->lmn_ready_in_i = 0xff;

  d->pmu_count_up_bits_i = 0;
  d->pmu_read_sel_bits_i = 0xf0f0f0f0f0f0f0f0ull;
  clear_words(d->pmu_write_en_bits_i, 3);
  clear_words(d->pmu_write_data_bits_i, 16);
  clear_words(d->pmu_neigh_event_sel_bits_i, 5);
  d->pmu_neigh_events_i = 0;
  d->pmu_disable_clock_i = 0;

  clear_words(d->min_tbox_req_info_bits_i, 11);
  d->min_tbox_req_valid_early_i = 0;
  d->min_tbox_req_valid_i = 0;
  d->tbox_min_rsp_ready_i = 1;
  d->tbox_sc_req_disable_next_i = 0;
  d->tbox_sc_req_ready_i = 1;
  clear_words(d->sc_tbox_rsp_info_bits_i, 17);
  d->sc_tbox_rsp_valid_i = 0;
  clear_words(d->tbox_neigh_req_info_bits_i, 11);
  d->tbox_neigh_req_valid_i = 0;
  d->tbox_neigh_rsp_ready_i = 1;
}

void set_flb_req_data(DUT* d, int minion, uint16_t data) {
  set_bits(d->flb_req_data_bits_i, minion * kFlbWidth, kFlbWidth, data);
}

uint64_t lmn_info_offset(int minion) { return uint64_t(minion) * kNeighRspBits; }

void set_neigh_rsp(WData* words, int base, uint8_t id, uint8_t dest, bool wdata,
                   uint8_t opcode, uint64_t data0, uint8_t size, uint8_t qwen) {
  set_bits(words, base + 0, 4, qwen);
  set_bits(words, base + 4, 3, size);
  set_u64(words, base + 7, data0);
  set_bits(words, base + 263, 2, opcode);
  set_bits(words, base + 265, 1, wdata ? 1 : 0);
  set_bits(words, base + 266, 4, dest);
  set_bits(words, base + 270, 8, id);
}

void set_lmn_info(DUT* d, int minion, uint8_t id, uint64_t data0) {
  set_neigh_rsp(d->lmn_info_out_bits_i, minion * kNeighRspBits, id, minion, true, 2, data0, 5, 0xf);
}

uint8_t get_lmn_info_id(DUT* d, int minion) {
  return static_cast<uint8_t>(get_bits(d->lmn_info_in_bits_o, minion * kNeighRspBits + 270, 8));
}

uint64_t pmu_read_data(DUT* d, int minion, int thread) {
  return get_u64(d->pmu_read_data_bits_o, (minion * kNrThreads + thread) * 64);
}

void pmu_set_write(DUT* d, int minion, int counter, uint64_t data) {
  set_bits(d->pmu_write_en_bits_i, minion * kPmuTotalCounters + counter, 1, 1);
  set_u64(d->pmu_write_data_bits_i, minion * 64, data);
}

void pmu_set_read_sel(DUT* d, int minion, int thread, uint8_t sel) {
  const int pos = (minion * kNrThreads + thread) * kPmuSelBits;
  d->pmu_read_sel_bits_i &= ~(uint64_t{0xf} << pos);
  d->pmu_read_sel_bits_i |= uint64_t(sel & 0xfu) << pos;
}

void pmu_set_event_sel(DUT* d, int minion, int counter, uint8_t event) {
  set_bits(d->pmu_neigh_event_sel_bits_i,
           minion * kPmuEventCntSelBits + counter * kPmuEventSelBits,
           kPmuEventSelBits, event);
}

void set_neigh_req(WData* words, uint8_t id, uint8_t source, bool wdata, uint8_t opcode,
                   uint64_t address, uint64_t data0, uint8_t size, uint8_t qwen,
                   uint8_t subopcode) {
  clear_words(words, 11);
  set_bits(words, 0, 10, subopcode);
  set_bits(words, 10, 4, qwen);
  set_bits(words, 14, 3, size);
  set_u64(words, 17, data0);
  set_bits(words, 273, 40, address);
  set_bits(words, 313, 5, opcode);
  set_bits(words, 318, 1, wdata ? 1 : 0);
  set_bits(words, 319, 4, source);
  set_bits(words, 323, 8, id);
}

void set_rsp(WData* words, uint8_t id, uint8_t dest, bool wdata, uint8_t opcode,
             uint64_t data0, uint8_t size, uint8_t qwen) {
  clear_words(words, 17);
  set_bits(words, 0, 4, qwen);
  set_bits(words, 4, 3, size);
  set_u64(words, 7, data0);
  set_bits(words, 519, 2, opcode);
  set_bits(words, 521, 1, wdata ? 1 : 0);
  set_bits(words, 522, 4, dest);
  set_bits(words, 526, 8, id);
}

uint8_t get_rsp_id(const WData* words) { return get_bits(words, 526, 8); }
uint8_t get_rsp_opcode(const WData* words) { return get_bits(words, 519, 2); }
uint64_t get_rsp_data0(const WData* words) { return get_u64(words, 7); }
uint8_t get_neigh_rsp_id(const WData* words) { return get_bits(words, 270, 8); }
uint8_t get_neigh_rsp_dest(const WData* words) { return get_bits(words, 266, 4); }
uint64_t get_neigh_rsp_data0(const WData* words) { return get_u64(words, 7); }
uint8_t get_req_nodata_opcode(const WData* words) { return get_bits(words, 57, 5); }
uint64_t get_req_nodata_addr(const WData* words) { return get_bits(words, 17, 40); }

void run_flb_checks(SimCtrl<DUT>& sim) {
  DUT* d = sim.dut.get();
  set_flb_req_data(d, 2, 0x123);
  d->flb_req_valid_i = 1u << 2;
  sim.tick();
  d->flb_req_valid_i = 0;
  sim.tick();
  sim.check(d->flb_l2_req_valid_o == 1, "FLB emits L2 request for pending minion");
  sim.check(d->flb_l2_req_data_o == 0x123, "FLB forwards stored request data");
  sim.tick();
  d->flb_l2_resp_valid_i = 1;
  d->flb_l2_resp_data_i = 1;
  sim.tick();
  d->flb_l2_resp_valid_i = 0;
  sim.check(d->flb_resp_valid_o == (1u << 2), "FLB response returns to original minion");
  sim.check(d->flb_resp_data_o == 1, "FLB response data is registered");

  for (int m = 0; m < 4; ++m) {
    set_flb_req_data(d, m, 0x40 + m);
    d->flb_req_valid_i = 1u << m;
    sim.tick();
  }
  d->flb_req_valid_i = 0;
  bool saw_backpressure = false;
  for (int i = 0; i < 12; ++i) {
    saw_backpressure |= (d->flb_l2_req_valid_o == 0);
    sim.tick();
  }
  sim.check(saw_backpressure, "FLB request path eventually stalls once outstanding FIFO fills without responses");
}

void run_lmn_checks(SimCtrl<DUT>& sim) {
  DUT* d = sim.dut.get();
  sim.check(d->lmn_ready_out_o == 0xff, "LMN all outputs ready after reset");
  sim.check(d->lmn_channel_mask_bits_o == 0x2cf2cfu, "LMN channel masks match original fixed topology");
  sim.check(d->lmn_dest_id_bits_o != 0, "LMN destination IDs are driven");

  set_lmn_info(d, 0, 0x10, 0x1111);
  d->lmn_valid_out_bits_i = 0x7;  // minion 0 channels to 1, 2, 4
  d->lmn_ready_in_i = 0xff;
  sim.tick();
  sim.check((d->lmn_valid_in_o & ((1u << 1) | (1u << 2) | (1u << 4))) ==
                ((1u << 1) | (1u << 2) | (1u << 4)),
            "LMN routes minion0 local messages to minions 1/2/4");
  sim.check((d->lmn_dbg_valid_out_o & 0x7u) == 0x7u, "LMN debug bits mark accepted minion0 routes");

  d->lmn_valid_out_bits_i = 0;
  sim.tick();
  set_lmn_info(d, 0, 0x20, 0xaaaa);
  set_lmn_info(d, 5, 0x55, 0xbbbb);
  set_lmn_info(d, 6, 0x66, 0xcccc);
  d->lmn_valid_out_bits_i = (1u << 2) | (1u << (5 * 3)) | (1u << (6 * 3));
  d->lmn_ready_in_i = 0xff;
  sim.tick();
  sim.check(d->lmn_valid_in_o & (1u << 4), "LMN arbitration presents valid to minion4");
  sim.check(get_lmn_info_id(d, 4) == 0x20, "LMN minion4 priority selects minion0 over minions5/6");

  d->lmn_valid_out_bits_i = 0;
  sim.tick();
  set_lmn_info(d, 0, 0x30, 0xdddd);
  d->lmn_valid_out_bits_i = (1u << 2);
  d->lmn_ready_in_i = 0xff & ~(1u << 4);
  sim.tick();
  sim.check((d->lmn_ready_out_o & 1u) == 0, "LMN backpressure holds minion0 when destination 4 not ready");
  d->lmn_ready_in_i = 0xff;
  sim.tick();
  sim.check((d->lmn_ready_out_o & 1u) != 0, "LMN releases held message when destination ready");
}

void run_pmu_checks(SimCtrl<DUT>& sim) {
  DUT* d = sim.dut.get();
  pmu_set_write(d, 2, 0, 0x1234);
  sim.tick();
  clear_words(d->pmu_write_en_bits_i, 3);
  sim.tick();
  pmu_set_read_sel(d, 0, 0, 0);
  sim.tick();
  sim.tick();
  sim.check(pmu_read_data(d, 0, 0) == 0x1234, "PMU write data is readable through selected counter");

  d->pmu_count_up_bits_i = 1ull;  // minion0 counter0
  sim.tick();
  d->pmu_count_up_bits_i = 0;
  for (int i = 0; i < 3; ++i) sim.tick();
  pmu_set_read_sel(d, 0, 0, 0);
  sim.tick();
  sim.tick();
  sim.check(pmu_read_data(d, 0, 0) == 0x1235, "PMU minion event increments pre-counter by one");

  pmu_set_write(d, 1, 1, 0x7f);
  sim.tick();
  clear_words(d->pmu_write_en_bits_i, 3);
  sim.tick();
  d->pmu_count_up_bits_i = 1ull << 1;
  sim.tick();
  d->pmu_count_up_bits_i = 0;
  for (int i = 0; i < 8; ++i) sim.tick();
  pmu_set_read_sel(d, 0, 0, 1);
  sim.tick();
  sim.tick();
  sim.check(pmu_read_data(d, 0, 0) == 0x80, "PMU pre-counter overflow increments post-counter");

  d->pmu_disable_clock_i = 1;
  d->pmu_count_up_bits_i = 1ull << 2;
  sim.tick();
  d->pmu_count_up_bits_i = 0;
  for (int i = 0; i < 4; ++i) sim.tick();
  pmu_set_read_sel(d, 0, 0, 2);
  sim.tick();
  sim.tick();
  sim.check(pmu_read_data(d, 0, 0) == 0, "PMU disable_clock suppresses counter update");
  d->pmu_disable_clock_i = 0;

  pmu_set_event_sel(d, 3, 0, 5);
  d->pmu_neigh_events_i = 1u << 4;  // port range [25:1], event 5 maps to bit index 5.
  sim.tick();
  for (int i = 0; i < 4; ++i) sim.tick();
  pmu_set_read_sel(d, 0, 0, 8);
  sim.tick();
  sim.tick();
  sim.check(pmu_read_data(d, 0, 0) != 0, "PMU selected neighborhood event reaches neighborhood counter");
}

void run_tbox_checks(SimCtrl<DUT>& sim) {
  DUT* d = sim.dut.get();
  sim.check(d->min_tbox_req_ready_o == 1, "TBOX minion request FIFO ready after reset");
  sim.check(d->tbox_neigh_req_ready_o == 1, "TBOX neighborhood request FIFO ready after reset");
  sim.check(d->sc_tbox_rsp_ready_o == 1, "TBOX SC response FIFO ready after reset");
  sim.check(d->tbox_neigh_rsp_valid_o == 0, "TBOX neighborhood response invalid after reset");

  const uint64_t msg_addr = (uint64_t{0x5a} << 3) | (uint64_t{0x9} << 12);
  set_neigh_req(d->min_tbox_req_info_bits_i, 0x11, 3, true, 4, msg_addr, 0xdeadbeef, 5, 0xf, 0);
  d->min_tbox_req_valid_early_i = 1;
  d->min_tbox_req_valid_i = 0;
  sim.tick();
  sim.tick();
  d->min_tbox_req_valid_early_i = 0;
  d->min_tbox_req_valid_i = 1;
  sim.tick();
  d->min_tbox_req_valid_i = 0;
  bool saw_min_req_accepted = false;
  for (int i = 0; i < 8; ++i) {
    sim.tick();
    saw_min_req_accepted |= d->min_tbox_req_ready_o;
  }
  sim.check(saw_min_req_accepted, "TBOX minion request FIFO accepts and drains a message request");

  set_neigh_req(d->tbox_neigh_req_info_bits_i, 0x22, 4, false, 1, 0x123456789aull, 0, 3, 0x7, 0x12);
  d->tbox_neigh_req_valid_i = 1;
  sim.tick();
  d->tbox_neigh_req_valid_i = 0;
  bool saw_sc_req = false;
  for (int i = 0; i < 6; ++i) {
    sim.tick();
    if (d->tbox_sc_req_valid_o) {
      saw_sc_req = true;
      break;
    }
  }
  sim.check(saw_sc_req, "TBOX non-message neighborhood request goes to SC");
  sim.check(get_req_nodata_opcode(d->tbox_sc_req_info_bits_o) == 1, "TBOX SC request keeps opcode");
  sim.check(get_req_nodata_addr(d->tbox_sc_req_info_bits_o) == 0x123456789aull, "TBOX SC request keeps address");
  for (int i = 0; i < 3; ++i) sim.tick();

  set_neigh_req(d->tbox_neigh_req_info_bits_i, 0x33, 4, true, 4, msg_addr, 0xabc, 5, 0xf, 0);
  d->tbox_neigh_req_valid_i = 1;
  sim.tick();
  d->tbox_neigh_req_valid_i = 0;
  bool saw_warmup_sc = false;
  for (int i = 0; i < 6; ++i) {
    sim.tick();
    if (d->tbox_sc_req_valid_o && get_req_nodata_opcode(d->tbox_sc_req_info_bits_o) == 4) {
      saw_warmup_sc = true;
      break;
    }
  }
  sim.check(saw_warmup_sc, "TBOX preserves original stale-route behavior for the first message after an SC request");
  for (int i = 0; i < 3; ++i) sim.tick();

  set_rsp(d->sc_tbox_rsp_info_bits_i, 0x44, 2, true, 1, 0xfeedface, 6, 0xf);
  d->sc_tbox_rsp_valid_i = 1;
  sim.tick();
  d->sc_tbox_rsp_valid_i = 0;
  bool saw_sc_rsp_ready = false;
  for (int i = 0; i < 8; ++i) {
    sim.tick();
    saw_sc_rsp_ready |= d->sc_tbox_rsp_ready_o;
  }
  sim.check(saw_sc_rsp_ready, "TBOX SC response FIFO accepts and drains a response");
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 200000;
  clear_inputs(sim.dut.get());
  sim.reset();

  sim.check(sim.dut->flb_resp_valid_o == 0, "FLB response valids reset low");
  sim.check(sim.dut->flb_l2_req_valid_o == 0, "FLB L2 request valid resets low");
  sim.check(sim.dut->tbox_sc_req_valid_o == 0, "TBOX SC request valid resets low");
  sim.check(sim.dut->tbox_min_rsp_valid_o == 0, "TBOX minion response valid resets low");

  run_flb_checks(sim);
  clear_inputs(sim.dut.get());
  sim.reset();
  run_lmn_checks(sim);
  clear_inputs(sim.dut.get());
  sim.reset();
  run_pmu_checks(sim);
  clear_inputs(sim.dut.get());
  sim.reset();
  run_tbox_checks(sim);

  return sim.finish();
}
