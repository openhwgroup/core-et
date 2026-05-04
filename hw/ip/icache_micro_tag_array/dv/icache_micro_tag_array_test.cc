// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vicache_micro_tag_array_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vicache_micro_tag_array_tb;

namespace {

constexpr uint64_t kTagA = 0x012345678ULL;
constexpr uint64_t kTagB = 0x02aaaaaaaULL;
constexpr uint64_t kTagC = 0x03cccccccULL;
constexpr uint64_t kDbgWord =
    (uint64_t{1} << 34) | kTagB;

void clear_inputs(DUT* dut) {
  dut->rd_tag_i = 0;
  dut->wr_early_valid_i = 0;
  dut->wr_valid_i = 0;
  dut->wr_entry_i = 0;
  dut->wr_tag_i = 0;
  dut->miss_i = 0;
  dut->miss_entry_i = 0;
  dut->miss_state_i = 0;
  dut->flush_data_i = 0;
  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

void drive_fill(DUT* dut, uint32_t entry, uint64_t tag) {
  dut->wr_early_valid_i = 1;
  dut->wr_valid_i = 1;
  dut->wr_entry_i = entry;
  dut->wr_tag_i = tag;
}

void drive_apb_write_setup(DUT* dut, uint32_t entry, uint64_t data) {
  dut->apb_paddr_i = entry;
  dut->apb_pwrite_i = 1;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = data;
}

void drive_apb_write_access(DUT* dut, uint32_t entry, uint64_t data) {
  dut->apb_paddr_i = entry;
  dut->apb_pwrite_i = 1;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 1;
  dut->apb_pwdata_i = data;
}

void drive_apb_read_setup(DUT* dut, uint32_t entry) {
  dut->apb_paddr_i = entry;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 0;
}

void drive_apb_read_access(DUT* dut, uint32_t entry) {
  dut->apb_paddr_i = entry;
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
  sim.check(sim.dut->rd_hit_array_o == 0, "empty micro-cache misses");

  clear_inputs(sim.dut.get());
  drive_fill(sim.dut.get(), 3, kTagA);
  sim.tick();

  clear_inputs(sim.dut.get());
  sim.dut->rd_tag_i = kTagA;
  sim.tick();
  sim.check(sim.dut->rd_hit_array_o == (1u << 3), "filled entry 3 hits");

  clear_inputs(sim.dut.get());
  sim.dut->rd_tag_i = kTagB;
  sim.tick();
  sim.check(sim.dut->rd_hit_array_o == 0, "different tag misses");

  clear_inputs(sim.dut.get());
  sim.dut->miss_i = 1;
  sim.dut->miss_entry_i = 3;
  sim.tick();

  clear_inputs(sim.dut.get());
  sim.dut->rd_tag_i = kTagA;
  sim.tick();
  sim.check(sim.dut->rd_hit_array_o == 0, "miss invalidates selected entry");

  clear_inputs(sim.dut.get());
  drive_fill(sim.dut.get(), 5, kTagC);
  sim.tick();

  clear_inputs(sim.dut.get());
  sim.dut->rd_tag_i = kTagC;
  sim.tick();
  sim.check(sim.dut->rd_hit_array_o == (1u << 5), "second fill hits the right entry");

  clear_inputs(sim.dut.get());
  sim.dut->flush_data_i = 1;
  sim.tick();
  sim.check(sim.dut->rd_hit_array_o == 0, "flush suppresses compares");

  clear_inputs(sim.dut.get());
  sim.dut->miss_state_i = 0;
  sim.tick();

  clear_inputs(sim.dut.get());
  drive_apb_write_setup(sim.dut.get(), 7, kDbgWord);
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 0, "debug write setup not ready");

  clear_inputs(sim.dut.get());
  drive_apb_write_access(sim.dut.get(), 7, kDbgWord);
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 1, "debug write completes during access phase");

  clear_inputs(sim.dut.get());
  sim.dut->rd_tag_i = kTagB;
  sim.tick();
  sim.check(sim.dut->rd_hit_array_o == (1u << 7), "debug write sets entry 7");

  clear_inputs(sim.dut.get());
  drive_apb_read_setup(sim.dut.get(), 7);
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 0, "debug read setup not ready");

  clear_inputs(sim.dut.get());
  drive_apb_read_access(sim.dut.get(), 7);
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 1, "debug read completes during access phase");
  sim.check(sim.dut->apb_prdata_o == kDbgWord, "debug read returns valid bit and tag");

  clear_inputs(sim.dut.get());
  sim.tick();
  sim.check(sim.dut->apb_pready_o == 0, "debug ready clears after access");

  for (uint32_t entry = 0; entry < 8; ++entry) {
    clear_inputs(sim.dut.get());
    drive_fill(sim.dut.get(), entry, kTagA + entry);
    sim.tick();

    clear_inputs(sim.dut.get());
    sim.dut->rd_tag_i = kTagA + entry;
    sim.tick();
    sim.check(sim.dut->rd_hit_array_o == (1u << entry),
              "per-entry fill/read round-trip " + std::to_string(entry));
  }

  return sim.finish();
}
