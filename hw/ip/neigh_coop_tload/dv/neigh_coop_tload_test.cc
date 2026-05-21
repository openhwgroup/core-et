// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_coop_tload_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

namespace {
constexpr int kMinPerN = 8;
constexpr int kMissReqs = 2;
constexpr int kSlots = kMinPerN * kMissReqs;
constexpr int kCoopDataBits = 20;
constexpr int kPaBits = 40;
constexpr int kSlvRdyBits = 7;
constexpr int kMissIdBits = 3;

uint32_t coop_data(uint32_t neigh_mask, uint32_t min_mask, uint32_t seq_id, uint32_t coop_id) {
  return (min_mask & 0xffu) | ((seq_id & 0x7u) << 8) |
         ((coop_id & 0x1fu) << 11) | ((neigh_mask & 0xfu) << 16);
}

uint32_t slv_rdy_bits(uint32_t dest_neigh, uint32_t coop_id) {
  return ((dest_neigh & 0x3u) << 5) | (coop_id & 0x1fu);
}

uint32_t tag(uint32_t coop_id, uint32_t seq_id) {
  return ((coop_id & 0x1fu) << 3) | (seq_id & 0x7u);
}

template <typename T>
void set_scalar_bits(T& dst, int bit, int width, uint64_t value) {
  uint64_t cur = static_cast<uint64_t>(dst);
  uint64_t mask = (width == 64) ? ~uint64_t{0} : ((uint64_t{1} << width) - 1u);
  cur &= ~(mask << bit);
  cur |= (value & mask) << bit;
  dst = static_cast<T>(cur);
}

void clear_words(WData* words, int nwords) {
  for (int i = 0; i < nwords; ++i) words[i] = 0;
}

void set_wide_bits(WData* words, int bit, int width, uint64_t value) {
  for (int i = 0; i < width; ++i) {
    const int pos = bit + i;
    const uint32_t mask = uint32_t{1} << (pos & 31);
    if ((value >> i) & 1u) words[pos >> 5] |= mask;
    else words[pos >> 5] &= ~mask;
  }
}

void clear_top_req_inputs(Vneigh_coop_tload_tb* dut) {
  dut->tl_req_valid_bits_i = 0;
  clear_words(dut->tl_req_id_bits_i, 3);
  clear_words(dut->tl_req_addr_bits_i, 20);
  clear_words(dut->tl_req_data_bits_i, 10);
  dut->tl_req_size_bits_i = 0;
  clear_words(dut->tl_req_subopcode_bits_i, 4);
}

void set_top_slot(Vneigh_coop_tload_tb* dut, int slot, uint32_t id, uint64_t addr,
                  uint32_t data, uint32_t size, uint32_t subopcode) {
  set_wide_bits(dut->tl_req_id_bits_i, slot * 5, 5, id);
  set_wide_bits(dut->tl_req_addr_bits_i, slot * kPaBits, kPaBits, addr);
  set_wide_bits(dut->tl_req_data_bits_i, slot * kCoopDataBits, kCoopDataBits, data);
  set_scalar_bits(dut->tl_req_size_bits_i, slot * 3, 3, size);
  set_wide_bits(dut->tl_req_subopcode_bits_i, slot * 7, 7, subopcode);
}

void drive_top_request(Vneigh_coop_tload_tb* dut, int slot, uint32_t req_id,
                       uint32_t addr_sel, uint32_t data, uint32_t size,
                       uint32_t subopcode) {
  clear_top_req_inputs(dut);
  dut->tl_req_valid_bits_i = static_cast<uint16_t>(uint32_t{1} << slot);
  set_top_slot(dut, slot, req_id, uint64_t{addr_sel & 0x3u} << 6, data, size, subopcode);
}

bool wait_for_slv_valid(SimCtrl<Vneigh_coop_tload_tb>& sim, int cycles) {
  for (int i = 0; i < cycles; ++i) {
    sim.tick();
    if (sim.dut->tl_slv_rdy_out_valid_o) return true;
  }
  return false;
}

bool wait_for_req_bid(SimCtrl<Vneigh_coop_tload_tb>& sim, int cycles) {
  for (int i = 0; i < cycles; ++i) {
    sim.tick();
    if (sim.dut->tl_req_bid_o) return true;
  }
  return false;
}

void clear_all_inputs(Vneigh_coop_tload_tb* dut) {
  dut->tt_wr_tag_bits_i = 0;
  dut->tt_wr_min_mask_bits_i = 0;
  dut->tt_wr_miss_id_bits_i = 0;
  dut->tt_wr_valid_i = 0;
  dut->tt_cam_pop_i = 0;
  dut->tt_cam_tag_i = 0;

  dut->ports_slv_rdy_out_data_lv_bits_i = 0;
  dut->ports_slv_rdy_out_valid_lv_i = 0;
  dut->ports_slv_rdy_in_data_bits_i = 0;
  dut->ports_slv_rdy_in_valid_i = 0;
  dut->ports_mst_done_out_coop_id_lv_i = 0;
  dut->ports_mst_done_out_valid_lv_i = 0;
  dut->ports_mst_done_in_coop_id_bits_i = 0;
  dut->ports_mst_done_in_valid_i = 0;

  dut->tl_neigh_id_i = 0;
  clear_top_req_inputs(dut);
  dut->tl_disable_next_i = 0;
  dut->tl_req_ready_i = 1;
  dut->tl_slv_rdy_in_data_bits_i = 0;
  dut->tl_slv_rdy_in_valid_i = 0;
  dut->tl_mst_done_in_coop_id_bits_i = 0;
  dut->tl_mst_done_in_valid_i = 0;
  dut->tl_tag_cam_pop_i = 0;
  dut->tl_tag_cam_tag_i = 0;
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<Vneigh_coop_tload_tb> sim(argc, argv);
  sim.max_time = 200000;
  clear_all_inputs(sim.dut.get());
  sim.reset();

  auto* d = sim.dut.get();

  sim.check(d->tt_wr_ready_o == 0x3, "tag table ready for two writes after reset");
  sim.check(d->tl_req_ready_bits_o == 0xffff, "all cooperative miss slots ready after reset");
  sim.check(d->ports_slv_rdy_out_valid_o == 0, "ports slave-ready output valid reset");
  sim.check(d->ports_mst_done_out_valid_o == 0, "ports master-done output valid reset");

  // Tag-table single write, CAM hit, pop, then dual write into two available entries.
  d->tt_wr_tag_bits_i = tag(3, 2);
  d->tt_wr_min_mask_bits_i = 0x05;
  d->tt_wr_miss_id_bits_i = 0;
  set_scalar_bits(d->tt_wr_miss_id_bits_i, 0, kMissIdBits, 4);
  set_scalar_bits(d->tt_wr_miss_id_bits_i, 2 * kMissIdBits, kMissIdBits, 6);
  d->tt_wr_valid_i = 0x1;
  sim.tick();
  d->tt_wr_valid_i = 0;
  d->tt_cam_tag_i = tag(3, 2);
  d->eval();
  sim.check(d->tt_cam_min_mask_o == 0x05, "tag table CAM returns written minion mask");
  sim.check((d->tt_cam_miss_id_bits_o & 0x7) == 4, "tag table CAM returns minion0 miss id");
  sim.check(((d->tt_cam_miss_id_bits_o >> 6) & 0x7) == 6, "tag table CAM returns minion2 miss id");
  d->tt_cam_pop_i = 1;
  sim.tick();
  d->tt_cam_pop_i = 0;

  d->tt_wr_tag_bits_i = tag(1, 0) | (tag(2, 1) << 8);
  d->tt_wr_min_mask_bits_i = 0x01 | (0x80 << 8);
  d->tt_wr_miss_id_bits_i = 0;
  set_scalar_bits(d->tt_wr_miss_id_bits_i, 0, kMissIdBits, 1);
  set_scalar_bits(d->tt_wr_miss_id_bits_i, 8 * kMissIdBits + 7 * kMissIdBits, kMissIdBits, 5);
  d->tt_wr_valid_i = 0x3;
  sim.tick();
  d->tt_wr_valid_i = 0;
  d->tt_cam_tag_i = tag(2, 1);
  d->eval();
  sim.check(d->tt_cam_min_mask_o == 0x80, "tag table accepts second simultaneous write");
  sim.check(((d->tt_cam_miss_id_bits_o >> (7 * kMissIdBits)) & 0x7) == 5,
            "tag table second write preserves minion7 miss id");

  // Ports wrapper: check both low->high and high->low registered paths.
  d->ports_slv_rdy_out_data_lv_bits_i = slv_rdy_bits(3, 17);
  d->ports_slv_rdy_out_valid_lv_i = 1;
  d->ports_mst_done_out_coop_id_lv_i = 9;
  d->ports_mst_done_out_valid_lv_i = 0x5;
  d->ports_slv_rdy_in_data_bits_i = slv_rdy_bits(0, 4) |
                                    (slv_rdy_bits(1, 5) << kSlvRdyBits) |
                                    (slv_rdy_bits(2, 6) << (2 * kSlvRdyBits));
  d->ports_slv_rdy_in_valid_i = 0x7;
  d->ports_mst_done_in_coop_id_bits_i = 3 | (7 << 5) | (11 << 10);
  d->ports_mst_done_in_valid_i = 0x6;
  sim.tick();
  sim.check(d->ports_slv_rdy_out_valid_o == 1, "ports L2H slave-ready valid flops high");
  sim.check(d->ports_slv_rdy_out_data_bits_o == slv_rdy_bits(3, 17),
            "ports L2H slave-ready data flops with valid");
  sim.check(d->ports_mst_done_out_valid_o == 0x5, "ports L2H done valid vector flops");
  sim.check(d->ports_mst_done_out_coop_id_o == 9, "ports L2H done id flops with valid");
  sim.check(d->ports_slv_rdy_in_valid_lv_o == 0x7, "ports H2L slave-ready valid vector flops");
  sim.check(d->ports_slv_rdy_in_data_lv_bits_o == d->ports_slv_rdy_in_data_bits_i,
            "ports H2L slave-ready data vector flops");
  sim.check(d->ports_mst_done_in_valid_lv_o == 0x6, "ports H2L done valid vector flops");
  sim.check(d->ports_mst_done_in_coop_id_lv_bits_o == d->ports_mst_done_in_coop_id_bits_i,
            "ports H2L done id vector flops");
  d->ports_slv_rdy_out_valid_lv_i = 0;
  d->ports_mst_done_out_valid_lv_i = 0;
  d->ports_slv_rdy_in_valid_i = 0;
  d->ports_mst_done_in_valid_i = 0;
  sim.tick();
  sim.check(d->ports_slv_rdy_out_valid_o == 0, "ports L2H valid clears when source valid clears");
  sim.check(d->ports_slv_rdy_out_data_bits_o == slv_rdy_bits(3, 17),
            "ports L2H data holds when valid is low");

  // Slave-neighborhood path: local request is not master, so it emits a ready notification
  // and writes the cooperative tag table.
  d->tl_neigh_id_i = 0;
  const uint32_t slave_data = coop_data(0x2, 0x01, 2, 5);
  drive_top_request(d, 0, 0x12, 1, slave_data, 4, 0x2a);
  sim.tick();
  d->tl_req_valid_bits_i = 0;
  sim.check(wait_for_slv_valid(sim, 8), "top emits slave-ready notification for remote master");
  sim.check(d->tl_slv_rdy_out_data_bits_o == slv_rdy_bits(1, 5),
            "top slave-ready notification carries master neighborhood and coop id");
  d->tl_tag_cam_tag_i = tag(5, 2);
  d->eval();
  sim.check(d->tl_tag_cam_min_mask_o == 0x01, "top tag table records slave minion mask");
  sim.check((d->tl_tag_cam_miss_id_bits_o & 0x7) == (0x12 & 0x7),
            "top tag table records slave miss id LSBs");

  // Master path with no other neighborhoods converts to a regular read.
  const uint32_t local_master_data = coop_data(0x1, 0x01, 3, 6);
  drive_top_request(d, 0, 0x13, 0, local_master_data, 5, 0x31);
  sim.tick();
  d->tl_req_valid_bits_i = 0;
  sim.check(wait_for_req_bid(sim, 8), "top emits local-master cooperative request");
  sim.check(d->tl_req_id_o == tag(6, 3), "top local-master request id is coop_id+seq_id");
  sim.check(d->tl_req_opcode_o == 1, "top local-master request is regular read opcode");
  sim.check(d->tl_req_wdata_o == 0, "top local-master request clears wdata");
  sim.check(d->tl_req_qwen_o == 0, "top local-master request clears qwen");
  sim.check(d->tl_req_data_low_o == local_master_data, "top local-master request preserves coop data bits");
  sim.check(d->tl_mst_done_out_valid_o == 0, "top local-only master request does not send done sideband");

  // External ready credit then inter-neighborhood master request.
  d->tl_slv_rdy_in_data_bits_i = slv_rdy_bits(0, 7);
  d->tl_slv_rdy_in_valid_i = 0x1;
  sim.tick();
  d->tl_slv_rdy_in_valid_i = 0;
  d->tl_slv_rdy_in_data_bits_i = 0;
  sim.tick();
  sim.tick();

  const uint32_t inter_master_data = coop_data(0x3, 0x01, 1, 7);
  drive_top_request(d, 0, 0x14, 0, inter_master_data, 6, 0x12);
  sim.tick();
  d->tl_req_valid_bits_i = 0;
  sim.check(wait_for_req_bid(sim, 10), "top emits inter-neighborhood master request after ready sideband");
  sim.check(d->tl_req_id_o == tag(7, 1), "top inter-master request id is coop_id+seq_id");
  sim.check(d->tl_req_opcode_o == 3, "top inter-master request is ReadCoop opcode");
  sim.check(d->tl_req_wdata_o == 1, "top inter-master request marks cooperative data");
  sim.check(d->tl_req_qwen_o == 1, "top inter-master request sets cooperative qwen bit");
  sim.check((d->tl_mst_done_out_valid_o & 0x1) == 0x1,
            "top inter-master request sends done to the ready remote neighborhood");
  sim.check(d->tl_mst_done_out_coop_id_o == 7, "top inter-master done sideband carries coop id");

  return sim.finish();
}
