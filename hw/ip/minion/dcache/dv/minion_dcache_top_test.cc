// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_top.

#include "Vminion_dcache_top_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vminion_dcache_top_tb;

namespace {

constexpr uint8_t kPrvS = 1;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kDbgBpLoad = 0;
constexpr uint8_t kDbgBpStore = 1;
constexpr uint8_t kCmdXrd = 0;
constexpr uint8_t kCmdXwr = 1;
constexpr uint8_t kTypeW = 2;
constexpr uint8_t kTypeD = 3;
constexpr uint16_t kDaBase = 0x200;
constexpr uint64_t kLoadStoreAddr = 0x1200;
constexpr uint8_t kLoadStoreWay = 1;
constexpr uint64_t kLoadInit = 0x0123456789ABCDEFULL;
constexpr uint64_t kStoreUpdate = 0xFEDCBA9876543210ULL;
constexpr uint64_t kMetaTagMask = (uint64_t{1} << 33) - 1;
constexpr uint8_t kMetaStateShared = 1;
constexpr uint8_t kMetaStateModified = 3;

void clear_inputs(DUT* dut) {
  dut->id_core_alloc_rq_pre_i = 0;
  dut->s0_core_alloc_rq_val_i = 0;
  dut->id_core_gsc_i = 0;
  dut->s0_core_req_valid_i = 0;
  dut->req_addr_i = 0;
  dut->req_addr_msb_err_i = 0;
  dut->req_dest_fp_i = 0;
  dut->req_dest_addr_i = 0;
  dut->req_thread_id_i = 0;
  dut->req_cmd_i = 0;
  dut->req_typ_i = 0;
  dut->store_data_lo_i = 0;
  dut->req_gsc_cnt_i = 0;
  dut->req_ps_mask_i = 0xff;
  dut->req_phys_i = 0;
  dut->req_mem_global_i = 0;
  dut->req_gsc32_idx_i = 0;
  dut->vmspagesize_i = 0;
  dut->vm_prv_t0_i = kPrvS;
  dut->vm_prv_t1_i = kPrvS;
  dut->satp_mode_i = 0;
  dut->satp_ppn_i = 0;
  dut->matp_mode_i = 0;
  dut->matp_ppn_i = 0;
  dut->satp_info_en_i = 0;
  dut->matp_info_en_i = 0;
  dut->ptw_req_ready_i = 0;
  dut->ptw_resp_canceled_req_i = 0;
  dut->ptw_resp_access_fault_i = 0;
  dut->ptw_resp_rsvd_for_hw_i = 0;
  dut->ptw_resp_ppn_i = 0;
  dut->ptw_resp_rsvd_for_sw_i = 0;
  dut->ptw_resp_d_i = 0;
  dut->ptw_resp_a_i = 0;
  dut->ptw_resp_g_i = 0;
  dut->ptw_resp_u_i = 0;
  dut->ptw_resp_x_i = 0;
  dut->ptw_resp_w_i = 0;
  dut->ptw_resp_r_i = 0;
  dut->ptw_resp_v_i = 0;
  dut->ptw_resp_valid_i = 0;
  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
  dut->l2_resp_valid_i = 0;
}

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void tick_n(SimCtrl<DUT>& sim, int cycles) {
  for (int i = 0; i < cycles; ++i) tick_and_eval(sim);
}

void reset_and_clear(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

bool wait_for_id_ready(SimCtrl<DUT>& sim, int max_cycles = 256) {
  for (int i = 0; i < max_cycles; ++i) {
    if (sim.dut->id_core_ready_o == 1) return true;
    tick_and_eval(sim);
  }
  return sim.dut->id_core_ready_o == 1;
}

void issue_core_req(DUT* dut, uint64_t addr, uint8_t thread_id, uint8_t cmd, uint8_t typ, bool phys,
                    uint8_t dest_addr, uint64_t store_data = 0) {
  dut->s0_core_req_valid_i = 1;
  dut->req_addr_i = addr;
  dut->req_addr_msb_err_i = 0;
  dut->req_dest_fp_i = 0;
  dut->req_dest_addr_i = dest_addr;
  dut->req_thread_id_i = thread_id;
  dut->req_cmd_i = cmd;
  dut->req_typ_i = typ;
  dut->store_data_lo_i = store_data;
  dut->req_ps_mask_i = 0xff;
  dut->req_phys_i = phys ? 1 : 0;
  dut->req_mem_global_i = 0;
  dut->req_gsc32_idx_i = 0;
}

void pulse_core_req(SimCtrl<DUT>& sim, uint64_t addr, uint8_t thread_id, uint8_t cmd, uint8_t typ,
                    bool phys, uint8_t dest_addr, uint64_t store_data = 0,
                    bool hold_store_stage = false) {
  issue_core_req(sim.dut.get(), addr, thread_id, cmd, typ, phys, dest_addr, store_data);
  tick_and_eval(sim);

  sim.dut->s0_core_req_valid_i = 0;
  if (hold_store_stage) {
    tick_and_eval(sim);
  }

  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

void program_satp(SimCtrl<DUT>& sim, uint8_t mode, uint32_t ppn) {
  clear_inputs(sim.dut.get());
  sim.dut->satp_info_en_i = 1;
  tick_and_eval(sim);
  clear_inputs(sim.dut.get());
  sim.dut->satp_mode_i = mode;
  sim.dut->satp_ppn_i = ppn;
  tick_and_eval(sim);
}

uint16_t make_md_addr(uint8_t way, uint8_t set) {
  return static_cast<uint16_t>((way << 4) | (set & 0xf));
}

uint16_t make_da_addr(uint8_t way, uint8_t idx) {
  return static_cast<uint16_t>(kDaBase | ((way & 0x3) << 7) | idx);
}

uint16_t make_md_addr_from_phys(uint8_t way, uint64_t phys_addr) {
  return make_md_addr(way, static_cast<uint8_t>((phys_addr >> 6) & 0xf));
}

uint16_t make_da_addr_from_phys(uint8_t way, uint64_t phys_addr) {
  return make_da_addr(way, static_cast<uint8_t>((phys_addr >> 3) & 0x7f));
}

uint64_t make_meta(uint8_t coh_state, uint64_t phys_addr) {
  return (static_cast<uint64_t>(coh_state) << 33) | ((phys_addr >> 7) & kMetaTagMask);
}

void apb_begin_write(DUT* dut, uint16_t addr, uint64_t data) {
  dut->apb_paddr_i = addr;
  dut->apb_pwrite_i = 1;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = data;
}

void apb_begin_read(DUT* dut, uint16_t addr) {
  dut->apb_paddr_i = addr;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

void apb_clear(DUT* dut) {
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_paddr_i = 0;
  dut->apb_pwdata_i = 0;
}

void apb_write(SimCtrl<DUT>& sim, uint16_t addr, uint64_t data, const char* what) {
  apb_begin_write(sim.dut.get(), addr, data);
  tick_and_eval(sim);
  sim.dut->apb_penable_i = 1;
  bool saw_ready = false;
  for (int i = 0; i < 12; ++i) {
    tick_and_eval(sim);
    if (sim.dut->apb_pready_o == 1) {
      saw_ready = true;
      break;
    }
  }
  sim.check(saw_ready, std::string(what) + " write completes");
  sim.check(sim.dut->apb_pslverr_o == 0, std::string(what) + " write stays error-free");
  apb_clear(sim.dut.get());
  tick_and_eval(sim);
}

uint64_t apb_read(SimCtrl<DUT>& sim, uint16_t addr, const char* what) {
  apb_begin_read(sim.dut.get(), addr);
  tick_and_eval(sim);
  sim.dut->apb_penable_i = 1;
  bool saw_ready = false;
  for (int i = 0; i < 12; ++i) {
    tick_and_eval(sim);
    if (sim.dut->apb_pready_o == 1) {
      saw_ready = true;
      break;
    }
  }
  sim.check(saw_ready, std::string(what) + " read completes");
  sim.check(sim.dut->apb_pslverr_o == 0, std::string(what) + " read stays error-free");
  uint64_t data = sim.dut->apb_prdata_o;
  apb_clear(sim.dut.get());
  tick_and_eval(sim);
  return data;
}

bool wait_for_core_resp(SimCtrl<DUT>& sim, int max_cycles = 12) {
  for (int i = 0; i < max_cycles; ++i) {
    if (sim.dut->s3_core_resp_valid_o == 0) break;
    tick_and_eval(sim);
  }
  for (int i = 0; i < max_cycles; ++i) {
    if (sim.dut->s3_core_resp_valid_o == 1) return true;
    tick_and_eval(sim);
  }
  return sim.dut->s3_core_resp_valid_o == 1;
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  reset_and_clear(sim);
  sim.check(wait_for_id_ready(sim), "reset reaches an ID-ready idle state");
  sim.check(sim.dut->ptw_req_valid_o == 0, "reset leaves PTW request idle");
  sim.check(sim.dut->apb_pslverr_o == 0, "reset leaves APB error low");
  sim.check(sim.dut->io_events_o == 0, "reset clears dcache event outputs");
  sim.check(sim.dut->bus_err_o == 0, "reset clears bus error outputs");
  sim.check(sim.dut->metadata_mask_o != 0, "metadata debug mask exposes implemented metadata bits");

  issue_core_req(sim.dut.get(), 0x1234, 0, kCmdXrd, kTypeW, true, 1);
  tick_and_eval(sim);
  sim.check(sim.dut->s1_bp_conf_valid_o == 1, "load request reaches the S1 breakpoint surface");
  sim.check(sim.dut->s1_bp_conf_cmd_o == kDbgBpLoad, "load request is classified as a load breakpoint");
  sim.check(sim.dut->s1_bp_conf_thread_id_o == 0, "load request keeps thread 0");
  sim.check(sim.dut->s1_bp_conf_addr_o == 0x1234, "load request keeps the S1 breakpoint address");
  sim.check(sim.dut->io_events_o == 0x001, "thread-0 request updates the event counter outputs");
  sim.check(sim.dut->bus_err_o == 0, "simple physical load does not raise a bus error");

  clear_inputs(sim.dut.get());
  issue_core_req(sim.dut.get(), 0x5678, 1, kCmdXwr, kTypeD, true, 2);
  tick_and_eval(sim);
  sim.check(sim.dut->s1_bp_conf_valid_o == 1, "store request reaches the S1 breakpoint surface");
  sim.check(sim.dut->s1_bp_conf_cmd_o == kDbgBpStore, "store request is classified as a store breakpoint");
  sim.check(sim.dut->s1_bp_conf_thread_id_o == 1, "store request keeps thread 1");
  sim.check(sim.dut->io_events_o == 0x002, "thread-1 request updates the event counter outputs");

  reset_and_clear(sim);
  sim.check(wait_for_id_ready(sim), "request test reset reaches the ID-ready state");
  program_satp(sim, kModeSv39, 0x0012345);
  sim.dut->vm_prv_t0_i = kPrvS;
  issue_core_req(sim.dut.get(), 0xABCDE000ULL, 0, kCmdXrd, kTypeW, false, 3);
  bool saw_ptw_req = false;
  for (int i = 0; i < 6; ++i) {
    tick_and_eval(sim);
    if (sim.dut->ptw_req_valid_o == 1) {
      saw_ptw_req = true;
      break;
    }
  }
  sim.check(saw_ptw_req, "translated miss launches a PTW request");
  sim.check(sim.dut->ptw_req_satp_mode_o == kModeSv39, "PTW request uses the programmed SATP mode");
  sim.check(sim.dut->ptw_req_satp_ppn_o == 0x0012345, "PTW request uses the programmed SATP PPN");
  sim.check(sim.dut->ptw_req_prv_o == kPrvS, "PTW request keeps the request privilege");
  sim.check(sim.dut->ptw_req_addr_o == 0xABCDEULL, "PTW request carries the translated VPN");

  reset_and_clear(sim);
  sim.check(wait_for_id_ready(sim), "metadata test reset reaches the ID-ready state");
  const uint64_t meta_write0 = (uint64_t{1} << 33) | 0x123456789ULL;
  const uint64_t meta_write1 = (uint64_t{2} << 33) | 0x1ABCDEFFFULL;
  apb_write(sim, make_md_addr(2, 5), meta_write0, "metadata entry 0");
  apb_write(sim, make_md_addr(1, 9), meta_write1, "metadata entry 1");
  const uint64_t meta_read0 = apb_read(sim, make_md_addr(2, 5), "metadata entry 0");
  const uint64_t meta_read1 = apb_read(sim, make_md_addr(1, 9), "metadata entry 1");
  sim.check(meta_read0 == (meta_write0 & sim.dut->metadata_mask_o),
            "metadata debug path round-trips entry 0");
  sim.check(meta_read1 == (meta_write1 & sim.dut->metadata_mask_o),
            "metadata debug path round-trips entry 1");

  reset_and_clear(sim);
  sim.check(wait_for_id_ready(sim), "data-array test reset reaches the ID-ready state");
  const uint64_t data_words[4] = {
      0x1111222233334444ULL,
      0x5555666677778888ULL,
      0x9999AAAABBBBCCCCULL,
      0xDDDDEEEEFFFF0000ULL,
  };
  for (int i = 0; i < 4; ++i) {
    apb_write(sim, make_da_addr(1, 0x10 + i), data_words[i], "data-array word");
  }
  for (int i = 0; i < 4; ++i) {
    sim.check(apb_read(sim, make_da_addr(1, 0x10 + i), "data-array word") == data_words[i],
              "data-array debug path round-trips 64-bit word");
  }

  reset_and_clear(sim);
  sim.check(wait_for_id_ready(sim), "load/store test reset reaches the ID-ready state");
  apb_write(sim, make_md_addr_from_phys(kLoadStoreWay, kLoadStoreAddr),
            make_meta(kMetaStateShared, kLoadStoreAddr), "load/store metadata shared");
  apb_write(sim, make_da_addr_from_phys(kLoadStoreWay, kLoadStoreAddr), kLoadInit,
            "load/store data seed");

  pulse_core_req(sim, kLoadStoreAddr, 0, kCmdXrd, kTypeD, true, 4);
  sim.check(wait_for_core_resp(sim), "cache-hit load produces a core response");
  sim.check(sim.dut->s3_core_resp_data_lo_o == kLoadInit, "cache-hit load returns seeded data");
  sim.check(sim.dut->s3_core_resp_addr_o == kLoadStoreAddr, "cache-hit load reports the accessed address");

  reset_and_clear(sim);
  sim.check(wait_for_id_ready(sim), "store-hit test reset reaches the ID-ready state");
  const uint64_t store_meta = make_meta(kMetaStateModified, kLoadStoreAddr);
  apb_write(sim, make_md_addr_from_phys(kLoadStoreWay, kLoadStoreAddr), store_meta,
            "store metadata modified");
  apb_write(sim, make_da_addr_from_phys(kLoadStoreWay, kLoadStoreAddr), kLoadInit,
            "store data seed");
  const uint64_t store_meta_read =
      apb_read(sim, make_md_addr_from_phys(kLoadStoreWay, kLoadStoreAddr), "store metadata readback");
  sim.check(store_meta_read == store_meta, "store metadata readback preserves modified-state line");

  pulse_core_req(sim, kLoadStoreAddr, 0, kCmdXwr, kTypeD, true, 5, kStoreUpdate, true);
  sim.check(wait_for_id_ready(sim), "store-hit drains back to the ID-ready state");
  pulse_core_req(sim, kLoadStoreAddr, 0, kCmdXrd, kTypeD, true, 6);
  sim.check(wait_for_core_resp(sim), "load after store-hit produces a core response");
  sim.check(sim.dut->s3_core_resp_data_lo_o == kStoreUpdate,
            "load after store-hit returns updated data");

  return sim.finish();
}
