// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original minion_top vs new minion_top.

#include "Vcosim_minion_top_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

using DUT = Vcosim_minion_top_tb;

namespace {

constexpr int kResetCycles = 5;
constexpr int kDirectedFetchCycles = 96;
constexpr int kRandomCycles = 4096;
constexpr uint16_t kDebugAxProgBuf0Addr = 0x0804;
constexpr uint8_t kMuxVpu0 = 0x00;
constexpr uint8_t kMuxVpu1 = 0x04;
constexpr uint8_t kMuxDcacheTl0 = 0x08;
constexpr uint8_t kMuxDcacheTl1 = 0x0c;
constexpr uint8_t kMuxIntpipe = 0x28;
constexpr uint32_t kFrcpPs = 0x5870007Bu;

uint32_t rng_state = 0x41B16EADu;

uint32_t xorshift32() {
  rng_state ^= rng_state << 13;
  rng_state ^= rng_state >> 17;
  rng_state ^= rng_state << 5;
  return rng_state;
}

template <typename T>
void print_bits(const T& value) {
  if constexpr (sizeof(T) <= sizeof(uint64_t)) {
    uint64_t raw = 0;
    std::memcpy(&raw, &value, sizeof(T));
    std::printf("0x%llx", static_cast<unsigned long long>(raw));
  } else {
    const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
    std::printf("0x");
    for (int i = static_cast<int>(sizeof(T)) - 1; i >= 0; --i) {
      std::printf("%02x", bytes[i]);
    }
  }
}

template <typename T>
void compare_bits(CosimCtrl<DUT>& sim, const char* name, const T& orig, const T& newv) {
  sim.comparisons++;
  if (std::memcmp(&orig, &newv, sizeof(T)) != 0) {
    sim.mismatches++;
    std::printf("  MISMATCH [t=%lu] %s: orig=",
                static_cast<unsigned long>(sim.sim_time), name);
    print_bits(orig);
    std::printf(" new=");
    print_bits(newv);
    std::printf("\n");
  }
}

#define CMP(sig) compare_bits(sim, #sig, d->orig_##sig, d->new_##sig)

void compare_all(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();

  CMP(eco_o);
  CMP(nsleepout);
  CMP(l2_dcache_evict_req_valid);
  CMP(l2_dcache_evict_req);
  CMP(l2_dcache_miss_req_valid);
  CMP(l2_dcache_miss_req);
  CMP(l2_dcache_resp_ready);
  CMP(icache_req_valid);
  CMP(icache_req);
  CMP(icache_flush_data);
  CMP(satp_info);
  CMP(matp_info);
  CMP(tlb_invalidate);
  CMP(dc_ptw_req_data);
  CMP(dc_ptw_req_valid);
  CMP(flb_neigh_req_valid);
  CMP(flb_neigh_req_data);
  CMP(trace_encoder);
  CMP(apb_prdata);
  CMP(apb_pready);
  CMP(apb_pslverr);
  CMP(debug_out);
  CMP(minion_dbg_signals);
  CMP(pmu_count_up);
  CMP(pmu_read_sel);
  CMP(pmu_write_en);
  CMP(pmu_write_data);
  CMP(pmu_neigh_event_sel);
}

#undef CMP

void tick_and_compare(CosimCtrl<DUT>& sim) {
  sim.tick();
  compare_all(sim);
}

void set_fetch_line(DUT* d, uint32_t salt, bool include_vpu) {
  static constexpr uint32_t kOps[] = {
      0x00000013u,  // nop
      0x00108093u,  // addi x1, x1, 1
      0xfff10113u,  // addi x2, x2, -1
      0x00418193u,  // addi x3, x3, 4
      0x00820213u,  // addi x4, x4, 8
      0x01028293u,  // addi x5, x5, 16
      0x02030313u,  // addi x6, x6, 32
      0x00000013u   // nop
  };

  for (int i = 0; i < 8; ++i) {
    d->stim_icache_resp_data[i] = kOps[(salt + static_cast<uint32_t>(i)) & 0x7u];
  }

  if (include_vpu) {
    d->stim_icache_resp_data[(salt >> 3) & 0x7u] = kFrcpPs;  // FRCP_PS exercises real VPU decode/control.
  }
}

void clear_inputs(DUT* d) {
  d->stim_enabled = 0x1;
  d->stim_reset_vector = 0x1000;
  d->stim_eco_i = 0x3a5;
  d->stim_nsleepin = 1;
  d->stim_icache_req_ready = 1;
  d->stim_icache_resp_valid = 0;
  set_fetch_line(d, 0, false);
  d->stim_debug_halt = 0;
  d->stim_debug_resume = 0;
  d->stim_debug_resethalt = 0;
  d->stim_debug_ackhavereset = 0;
  d->stim_apb_paddr = 0;
  d->stim_apb_pwrite = 0;
  d->stim_apb_psel = 0;
  d->stim_apb_penable = 0;
  d->stim_apb_pwdata = 0;
  d->stim_dbg_mux = kMuxIntpipe;
  d->stim_dbg_enable = 0;
}

void pulse_dbg_mux(CosimCtrl<DUT>& sim, uint8_t mux) {
  sim.dut->stim_dbg_mux = mux;
  sim.dut->stim_dbg_enable = 1;
  tick_and_compare(sim);
  sim.dut->stim_dbg_enable = 0;
  tick_and_compare(sim);
}

void apb_idle(DUT* d) {
  d->stim_apb_paddr = 0;
  d->stim_apb_pwrite = 0;
  d->stim_apb_psel = 0;
  d->stim_apb_penable = 0;
  d->stim_apb_pwdata = 0;
}

void apb_write(CosimCtrl<DUT>& sim, uint16_t addr, uint64_t data,
               bool* saw_pready, bool* saw_pslverr) {
  *saw_pready = false;
  *saw_pslverr = false;

  sim.dut->stim_apb_paddr = addr;
  sim.dut->stim_apb_pwrite = 1;
  sim.dut->stim_apb_psel = 1;
  sim.dut->stim_apb_penable = 0;
  sim.dut->stim_apb_pwdata = data;
  tick_and_compare(sim);

  sim.dut->stim_apb_penable = 1;
  for (int i = 0; i < 8; ++i) {
    tick_and_compare(sim);
    if (sim.dut->new_apb_pready) {
      *saw_pready = true;
      *saw_pslverr = sim.dut->new_apb_pslverr;
      break;
    }
  }

  apb_idle(sim.dut.get());
  tick_and_compare(sim);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 500000;

  clear_inputs(sim.dut.get());
  sim.dut->clk_i = 0;
  sim.dut->rst_c_ni = 0;
  sim.dut->rst_w_ni = 0;
  sim.dut->rst_d_ni = 0;

  for (int i = 0; i < kResetCycles; ++i) {
    tick_and_compare(sim);
  }

  sim.dut->rst_c_ni = 1;
  sim.dut->rst_w_ni = 1;
  sim.dut->rst_d_ni = 1;
  for (int i = 0; i < 8; ++i) {
    tick_and_compare(sim);
  }

  pulse_dbg_mux(sim, kMuxVpu0);
  pulse_dbg_mux(sim, kMuxVpu1);
  pulse_dbg_mux(sim, kMuxIntpipe);

  bool saw_fetch = false;
  for (int i = 0; i < 32; ++i) {
    tick_and_compare(sim);
    if (sim.dut->orig_icache_req_valid && sim.dut->new_icache_req_valid) {
      saw_fetch = true;
      break;
    }
  }
  sim.check(saw_fetch, "real-VPU minion_top issues an icache request after reset");

  for (int cycle = 0; cycle < kDirectedFetchCycles; ++cycle) {
    sim.dut->stim_icache_resp_valid = ((cycle & 3) == 1) ? 1 : 0;
    set_fetch_line(sim.dut.get(), static_cast<uint32_t>(cycle), (cycle % 5) == 0);

    if (cycle == 12) sim.dut->stim_debug_ackhavereset = 0x1;
    if (cycle == 13) sim.dut->stim_debug_ackhavereset = 0x0;

    if (cycle == 16) {
      pulse_dbg_mux(sim, kMuxVpu0);
    } else if (cycle == 32) {
      pulse_dbg_mux(sim, kMuxVpu1);
    } else if (cycle == 48) {
      pulse_dbg_mux(sim, kMuxDcacheTl0);
    } else if (cycle == 64) {
      pulse_dbg_mux(sim, kMuxDcacheTl1);
    } else if (cycle == 80) {
      pulse_dbg_mux(sim, kMuxIntpipe);
    }

    tick_and_compare(sim);
  }
  sim.dut->stim_icache_resp_valid = 0;

  bool saw_pready = false;
  bool saw_pslverr = false;
  apb_write(sim, kDebugAxProgBuf0Addr, 0x1122334455667788ull, &saw_pready, &saw_pslverr);
  sim.check(saw_pready, "debug APB write completes under real-VPU cosim");
  sim.check(saw_pslverr, "debug APB program-buffer write is rejected outside debug mode");

  sim.dut->stim_nsleepin = 0;
  tick_and_compare(sim);
  sim.dut->stim_nsleepin = 1;
  tick_and_compare(sim);

  for (int cycle = 0; cycle < kRandomCycles; ++cycle) {
    sim.dut->stim_icache_req_ready = (xorshift32() & 0x3u) != 0;
    if ((xorshift32() & 0x3u) == 0) {
      sim.dut->stim_icache_resp_valid = 1;
      uint32_t salt = xorshift32();
      set_fetch_line(sim.dut.get(), salt, (salt & 0x3u) == 0);
    } else {
      sim.dut->stim_icache_resp_valid = 0;
    }

    sim.dut->stim_debug_halt = ((xorshift32() & 0x1fu) == 0) ? 0x1 : 0x0;
    sim.dut->stim_debug_resume = ((xorshift32() & 0x1fu) == 1) ? 0x1 : 0x0;
    sim.dut->stim_debug_resethalt = ((xorshift32() & 0x3fu) == 2) ? 0x1 : 0x0;
    sim.dut->stim_debug_ackhavereset = ((xorshift32() & 0x3fu) == 3) ? 0x1 : 0x0;

    if ((xorshift32() & 0x1fu) == 0) {
      static constexpr uint8_t kMuxChoices[] = {kMuxVpu0, kMuxVpu1, kMuxDcacheTl0, kMuxDcacheTl1, kMuxIntpipe};
      sim.dut->stim_dbg_mux = kMuxChoices[xorshift32() % 5u];
      sim.dut->stim_dbg_enable = 1;
    } else {
      sim.dut->stim_dbg_enable = 0;
    }

    sim.dut->stim_nsleepin = (xorshift32() & 0x0fu) == 0 ? 0 : 1;

    tick_and_compare(sim);

    sim.dut->stim_debug_halt = 0;
    sim.dut->stim_debug_resume = 0;
    sim.dut->stim_debug_resethalt = 0;
    sim.dut->stim_debug_ackhavereset = 0;
    sim.dut->stim_dbg_enable = 0;
    apb_idle(sim.dut.get());
  }

  return sim.finish();
}
