// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vicache_tag_array_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vicache_tag_array_tb;

namespace {

constexpr uint32_t kTagA = 0x0123456u;
constexpr uint32_t kTagB = 0x03abcdeu;
constexpr uint32_t kTagC = 0x05aaaaau;
constexpr uint64_t kDebugPair01 =
    (uint64_t{1} << 59) | (uint64_t{kTagB} << 32) | (uint64_t{1} << 27) | kTagA;

void clear_inputs(DUT* dut) {
  dut->read_valid_i = 0;
  dut->read_set_i = 0;
  dut->read_tag_i = 0;
  dut->write_early_valid_i = 0;
  dut->write_valid_i = 0;
  dut->write_req_valid_i = 0;
  dut->write_req_set_i = 0;
  dut->write_req_way_i = 0;
  dut->write_req_tag_i = 0;
  dut->f1_miss_i = 0;
  dut->f1_miss_idx_i = 0;
  dut->f1_miss_way_i = 0;
  dut->miss_state_i = 0;
  dut->flush_data_i = 0;
  dut->apb_pair_i = 0;
  dut->apb_set_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

void drive_read(DUT* dut, uint32_t set, uint32_t tag) {
  dut->read_valid_i = 1;
  dut->read_set_i = set;
  dut->read_tag_i = tag;
}

void drive_fill(DUT* dut, uint32_t set, uint32_t way, uint32_t tag, bool valid = true) {
  dut->write_early_valid_i = 1;
  dut->write_valid_i = 1;
  dut->write_req_valid_i = valid ? 1 : 0;
  dut->write_req_set_i = set;
  dut->write_req_way_i = way;
  dut->write_req_tag_i = tag;
}

void drive_apb_write_setup(DUT* dut, uint32_t set, uint32_t pair, uint64_t data) {
  dut->apb_set_i = set;
  dut->apb_pair_i = pair;
  dut->apb_pwrite_i = 1;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = data;
}

void drive_apb_write_access(DUT* dut, uint32_t set, uint32_t pair, uint64_t data) {
  dut->apb_set_i = set;
  dut->apb_pair_i = pair;
  dut->apb_pwrite_i = 1;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 1;
  dut->apb_pwdata_i = data;
}

void drive_apb_read_setup(DUT* dut, uint32_t set, uint32_t pair) {
  dut->apb_set_i = set;
  dut->apb_pair_i = pair;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 0;
}

void drive_apb_read_access(DUT* dut, uint32_t set, uint32_t pair) {
  dut->apb_set_i = set;
  dut->apb_pair_i = pair;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 1;
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 20000;

  clear_inputs(sim.dut.get());
  sim.reset();

  sim.check(sim.dut->apb_pready_o == 0, "apb_pready low after reset");
  sim.check(sim.dut->apb_pslverr_o == 0, "apb_pslverr permanently low");

  sim.dut->miss_state_i = 0;
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 0, "apb_pready stays low while idle");

  clear_inputs(sim.dut.get());
  drive_read(sim.dut.get(), 3, kTagA);
  sim.tick();
  sim.check(sim.dut->f1_read_tag_hit_o == 0, "empty set misses");

  clear_inputs(sim.dut.get());
  drive_fill(sim.dut.get(), 3, 1, kTagA);
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 0, "normal fill does not raise APB ready");

  clear_inputs(sim.dut.get());
  drive_read(sim.dut.get(), 3, kTagA);
  sim.tick();
  sim.check(sim.dut->f1_read_tag_hit_o == 0x2, "filled way 1 hits");

  clear_inputs(sim.dut.get());
  drive_read(sim.dut.get(), 3, kTagB);
  sim.tick();
  sim.check(sim.dut->f1_read_tag_hit_o == 0, "different tag still misses");

  clear_inputs(sim.dut.get());
  sim.dut->f1_miss_i = 1;
  sim.dut->f1_miss_idx_i = 3;
  sim.dut->f1_miss_way_i = 1;
  sim.tick();
  sim.check(sim.dut->f1_read_tag_hit_o == 0, "victim invalidate cycle has no stale hit");

  clear_inputs(sim.dut.get());
  drive_read(sim.dut.get(), 3, kTagA);
  sim.tick();
  sim.check(sim.dut->f1_read_tag_hit_o == 0, "invalidated way no longer hits");

  clear_inputs(sim.dut.get());
  drive_fill(sim.dut.get(), 3, 1, kTagA);
  sim.tick();

  clear_inputs(sim.dut.get());
  sim.dut->flush_data_i = 1;
  sim.tick();
  sim.check(sim.dut->f1_read_tag_hit_o == 0, "flush cycle suppresses compares");

  clear_inputs(sim.dut.get());
  sim.dut->miss_state_i = 0;
  sim.tick();

  clear_inputs(sim.dut.get());
  drive_fill(sim.dut.get(), 5, 2, kTagC);
  sim.tick();

  clear_inputs(sim.dut.get());
  drive_read(sim.dut.get(), 5, kTagC);
  sim.tick();
  sim.check(sim.dut->f1_read_tag_hit_o == 0x4, "fill after flush hits again");

  clear_inputs(sim.dut.get());
  drive_apb_write_setup(sim.dut.get(), 9, 0, kDebugPair01);
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 0, "APB write request is not ready immediately");

  clear_inputs(sim.dut.get());
  drive_apb_write_access(sim.dut.get(), 9, 0, kDebugPair01);
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 1, "APB write completes during the access phase");

  clear_inputs(sim.dut.get());
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 0, "APB write ready deasserts after the access phase");

  clear_inputs(sim.dut.get());
  drive_read(sim.dut.get(), 9, kTagA);
  sim.tick();
  sim.check(sim.dut->f1_read_tag_hit_o == 0x1, "debug write sets way 0");

  clear_inputs(sim.dut.get());
  drive_read(sim.dut.get(), 9, kTagB);
  sim.tick();
  sim.check(sim.dut->f1_read_tag_hit_o == 0x2, "debug write sets way 1");

  clear_inputs(sim.dut.get());
  drive_apb_read_setup(sim.dut.get(), 9, 0);
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 0, "APB read request is not ready immediately");

  clear_inputs(sim.dut.get());
  drive_apb_read_access(sim.dut.get(), 9, 0);
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 1, "APB read completes during the access phase");
  sim.check(sim.dut->apb_prdata_o == kDebugPair01, "APB read returns packed valid bits and tags");

  clear_inputs(sim.dut.get());
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 0, "APB read ready deasserts after the access phase");
  sim.check(sim.dut->apb_pslverr_o == 0, "APB read keeps slave error low");

  clear_inputs(sim.dut.get());
  drive_fill(sim.dut.get(), 9, 0, kTagA, false);
  sim.tick();

  clear_inputs(sim.dut.get());
  drive_read(sim.dut.get(), 9, kTagA);
  sim.tick();
  sim.check(sim.dut->f1_read_tag_hit_o == 0, "invalid fill request clears the selected valid bit");

  for (uint32_t set = 0; set < 8; ++set) {
    clear_inputs(sim.dut.get());
    drive_fill(sim.dut.get(), set, set & 0x3u, kTagA + set);
    sim.tick();

    clear_inputs(sim.dut.get());
    drive_read(sim.dut.get(), set, kTagA + set);
    sim.tick();
    sim.check(sim.dut->f1_read_tag_hit_o == (1u << (set & 0x3u)),
              "per-set fill/read round-trip " + std::to_string(set));
  }

  return sim.finish();
}
