// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: dcache_top vs minion_dcache_top.

#include "Vcosim_minion_dcache_top_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

using DUT = Vcosim_minion_dcache_top_tb;

namespace {

constexpr uint8_t kCmdXrd = 0;
constexpr uint8_t kCmdXwr = 1;
constexpr uint8_t kTypeW = 2;
constexpr uint8_t kTypeD = 3;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kPrvS = 1;
constexpr uint16_t kDaBase = 0x200;
constexpr uint64_t kPhysAddr = 0x0000000000001234ULL;
constexpr uint64_t kVirtAddr = 0x00000000ABCDE000ULL;
constexpr uint64_t kExpectedPtwPpn = 0x0000000000012345ULL;
constexpr uint64_t kExpectedPtwAddr = 0x00000000000ABCDEULL;
constexpr uint64_t kLoadStorePhysAddr = 0x0000008100001200ULL;
constexpr uint64_t kLoadStoreVirtAddr = kLoadStorePhysAddr;
constexpr uint64_t kFpgaLoadStorePhysAddr = 0x0000000000001200ULL;
constexpr uint64_t kFpgaLoadStoreVirtAddr = kFpgaLoadStorePhysAddr;
constexpr uint64_t kLoadSeed = 0x0123456789ABCDEFULL;
constexpr uint64_t kStoreWrite = 0xFEDCBA9876543210ULL;
constexpr bool kLoadStorePhys = true;
constexpr uint8_t kPrefillWay = 1;
constexpr uint8_t kStateShared = 1;
constexpr uint8_t kStateModified = 3;
constexpr int kPaTagExtSize = 33;
constexpr int kDcacheWays = 4;
constexpr int kDcacheDbgSmDataGroupBits = 128;
constexpr int kCoDbgGroup1Lsb = kDcacheDbgSmDataGroupBits;
constexpr int kCoDbgMetaReadRespWayEnLsb = kCoDbgGroup1Lsb + kPaTagExtSize;
constexpr int kRandomCycles = 12000;

enum class LoadObserveResult {
  Completed,
  TimedOut,
  BadOriginalResponse,
};

uint32_t rng_state = 0x1A2B3C4D;

uint32_t xorshift32() {
  rng_state ^= rng_state << 13;
  rng_state ^= rng_state >> 17;
  rng_state ^= rng_state << 5;
  return rng_state;
}

uint64_t xorshift64() {
  return (static_cast<uint64_t>(xorshift32()) << 32) | xorshift32();
}

void fill_random_bytes(void* ptr, size_t size) {
  auto* bytes = static_cast<unsigned char*>(ptr);
  for (size_t i = 0; i < size; ++i) {
    bytes[i] = static_cast<unsigned char>(xorshift32());
  }
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

  CMP(core_ctrl_resp);
  CMP(id_core_ready);
  CMP(s3_vpu_scp_resp);
  CMP(s3_vpu_scp_data);
  CMP(s3_vpu_tenb_data);
  CMP(vpu_reduce_ctrl);
  CMP(s2_core_resp_int_valid);
  CMP(s3_core_resp_valid);
  CMP(s3_core_resp);
  CMP(id_core_scoreboard);
  CMP(id_core_sb_fp_dealloc);
  CMP(id_core_sb_int_dealloc);
  CMP(s1_core_replay_next);
  CMP(s1_core_xcpt);
  CMP(s2_core_flush_pipeline);
  CMP(s3_ordered);
  CMP(ptw_req_data);
  CMP(ptw_req_valid);
  CMP(l2_evict_req_valid);
  CMP(l2_evict_req);
  CMP(l2_miss_req_valid);
  CMP(l2_miss_req);
  CMP(l2_resp_ready);
  CMP(l2_resp_ready_int);
  CMP(l2_resp_valid_int);
  CMP(l2_resp_int);
  CMP(s1_bp_conf);
  CMP(s1_bp_conf_valid);
  CMP(dcache_idle_excl_mode);
  CMP(tensor_load_err_flags);
  CMP(cache_ops_err_flags);
  CMP(tensor_reduce_err_flags);
  CMP(apb_pready);
  CMP(apb_prdata);
  CMP(apb_pslverr);
  CMP(io_events);
  CMP(bus_err);
  CMP(bus_err_addr);
  CMP(csr_debug_sigs);
  CMP(sm_match_debug_signals_tl0);
  CMP(sm_filter_debug_signals_tl0);
  CMP(sm_data_debug_signals_tl0);
  CMP(sm_match_debug_signals_tl1);
  CMP(sm_filter_debug_signals_tl1);
  CMP(sm_data_debug_signals_tl1);
  CMP(sm_match_debug_signals_ts);
  CMP(sm_filter_debug_signals_ts);
  CMP(sm_data_debug_signals_ts);
  CMP(sm_match_debug_signals_co);
  CMP(sm_filter_debug_signals_co);
  CMP(sm_data_debug_signals_co);
  CMP(sm_match_debug_signals_col2);
  CMP(sm_filter_debug_signals_col2);
  CMP(sm_data_debug_signals_col2);
  CMP(sm_match_debug_signals_mh);
  CMP(sm_filter_debug_signals_mh);
  CMP(sm_data_debug_signals_mh);
  CMP(sm_match_debug_signals_dc);
  CMP(sm_filter_debug_signals_dc);
  CMP(sm_data_debug_signals_dc);
  CMP(mh_idle);
  CMP(mh_l2_evict_valid);
  CMP(s2_mh_handler_ready);
  CMP(id_rq_full);
  CMP(id_rq_half_full);
  CMP(s2_rq_empty);
  CMP(rq_entry_mask_available);
  CMP(rq_push_age);
  CMP(rq_pending);
  CMP(rq_wait_uc_load);
  CMP(rq_replay_valid_prev);
  CMP(rq_replay_entry_prev);
  CMP(s1_valid);
  CMP(s1_valid_masked);
  CMP(s1_valid_to_tlb);
  CMP(s1_valid_to_s2);
  CMP(s1_phys_addr);
  CMP(s1_req_addr);
  CMP(s1_req_cmd);
  CMP(s1_req_typ);
  CMP(s1_mr_read_valid);
  CMP(s1_tag_match);
  CMP(s1_req_replay);
  CMP(s1_req_cache_op);
  CMP(s1_tlb_miss);
  CMP(s1_pma_bus_err);
  CMP(s1_rq_push_conservative);
  CMP(s2_valid);
  CMP(s2_valid_qual);
  CMP(s2_rq_push_cancel);
  CMP(s2_rq_push);
  CMP(s2_rq_renack);
  CMP(s2_rq_dealloc);
  CMP(mh_state_0);
  CMP(mh_state_1);
  CMP(l2_resp_is_for_mh_fill);
  CMP(l2_da_write);
  CMP(l2_fill_slot_avail);
  CMP(s4_mh_refill_way);
  CMP(s4_mh_refill_addr);
  CMP(s2_tag_match);
  CMP(s2_tag_match_qual);
  CMP(s2_tag_match_any);
  CMP(s2_state_hit);
  CMP(s2_way_hit_state);
  CMP(s2_hit);
  CMP(s2_mh_req_valid);
  CMP(s2_mh_req_ready);
  CMP(s2_req_replay);
  CMP(s2_req_cacheable);
  CMP(s2_req_uc_load_2nd);
  CMP(s2_req_addr);
  CMP(s2_req_cmd);
  CMP(s2_req_typ);
  CMP(s4_valid);
  CMP(s4_da_write_en);
  CMP(s4_da_write_en_but_dbg);
  CMP(s4_req_addr);
  CMP(s4_req_cmd);
  CMP(s4_req_typ);
  CMP(s4_req_data_lo);
  CMP(s4_da_write_data_0);
}

#undef CMP

void tick_and_compare(CosimCtrl<DUT>& sim) {
  sim.tick();
  compare_all(sim);
}

uint16_t make_md_addr(uint8_t way, uint8_t set) {
  return static_cast<uint16_t>((way << 4) | (set & 0xf));
}

uint16_t make_da_addr(uint8_t way, uint8_t idx) {
  return static_cast<uint16_t>(kDaBase | ((way & 0x3) << 7) | idx);
}

uint8_t cache_set(uint64_t addr) {
  return static_cast<uint8_t>((addr >> 6) & 0xf);
}

uint64_t cache_tag(uint64_t addr) {
  return addr >> 7;
}

uint8_t cache_lram_debug_idx(uint64_t addr, uint8_t bank) {
  return static_cast<uint8_t>(((addr >> 3) & 0x7c) | (bank & 0x3));
}

uint64_t cache_meta_word(uint8_t state, uint64_t addr) {
  return (static_cast<uint64_t>(state) << kPaTagExtSize)
       | (cache_tag(addr) & ((uint64_t{1} << kPaTagExtSize) - 1));
}

void clear_inputs(DUT* d) {
  d->stim_shire_id_i = 0;
  d->stim_shire_min_id_i = 0;
  d->stim_ioshire_i = 0;
  d->stim_chicken_bit_dcache_i = 0;
  std::memset(&d->stim_core_ctrl_i, 0, sizeof(d->stim_core_ctrl_i));
  d->stim_force_ts_scp_start_i = 0;
  d->stim_id_core_alloc_rq_pre_i = 0;
  d->stim_s0_core_alloc_rq_val_i = 0;
  d->stim_id_core_gsc_i = 0;
  d->stim_s0_core_req_valid_i = 0;
  d->stim_req_addr_i = 0;
  d->stim_req_addr_msb_err_i = 0;
  d->stim_req_dest_fp_i = 0;
  d->stim_req_dest_addr_i = 0;
  d->stim_req_thread_id_i = 0;
  d->stim_req_cmd_i = 0;
  d->stim_req_typ_i = 0;
  d->stim_req_gsc_cnt_i = 0;
  d->stim_req_ps_mask_i = 0xff;
  d->stim_req_phys_i = 0;
  d->stim_req_mem_global_i = 0;
  d->stim_req_gsc32_idx_i = 0;
  d->stim_s0_core_gsc_i = 0;
  d->stim_s1_core_kill_i = 0;
  std::memset(&d->stim_s1_core_store_data_i, 0, sizeof(d->stim_s1_core_store_data_i));
  std::memset(&d->stim_s1_mprot_i, 0, sizeof(d->stim_s1_mprot_i));
  d->stim_s2_core_kill_i = 0;
  d->stim_s3_core_x31_i = 0;
  std::memset(&d->stim_s1_vpu_ctrl_i, 0, sizeof(d->stim_s1_vpu_ctrl_i));
  d->stim_s3_invalidate_lr_i = 0;
  d->stim_wb_dcache_fp_toint_i = 0;
  d->stim_vmspagesize_i = 0;
  d->stim_bypass_dcache_i = 0;
  d->stim_mem_ctrl_override_i = 0;
  d->stim_satp_mode_i = 0;
  d->stim_satp_ppn_i = 0;
  d->stim_matp_mode_i = 0;
  d->stim_matp_ppn_i = 0;
  d->stim_satp_info_en_i = 0;
  d->stim_matp_info_en_i = 0;
  d->stim_vm_prv_t0_i = kPrvS;
  d->stim_vm_mprv_t0_i = 0;
  d->stim_vm_mpp_t0_i = 0;
  d->stim_vm_sum_t0_i = 0;
  d->stim_vm_mxr_t0_i = 0;
  d->stim_vm_debug_t0_i = 0;
  d->stim_vm_prv_t1_i = kPrvS;
  d->stim_vm_mprv_t1_i = 0;
  d->stim_vm_mpp_t1_i = 0;
  d->stim_vm_sum_t1_i = 0;
  d->stim_vm_mxr_t1_i = 0;
  d->stim_vm_debug_t1_i = 0;
  d->stim_tlb_invalidate_i = 0;
  d->stim_ptw_req_ready_i = 1;
  d->stim_ptw_resp_canceled_req_i = 0;
  d->stim_ptw_resp_access_fault_i = 0;
  d->stim_ptw_resp_rsvd_for_hw_i = 0;
  d->stim_ptw_resp_ppn_i = 0;
  d->stim_ptw_resp_rsvd_for_sw_i = 0;
  d->stim_ptw_resp_d_i = 0;
  d->stim_ptw_resp_a_i = 0;
  d->stim_ptw_resp_g_i = 0;
  d->stim_ptw_resp_u_i = 0;
  d->stim_ptw_resp_x_i = 0;
  d->stim_ptw_resp_w_i = 0;
  d->stim_ptw_resp_r_i = 0;
  d->stim_ptw_resp_v_i = 0;
  d->stim_ptw_resp_valid_i = 0;
  d->stim_l2_evict_req_ready_i = 0xff;
  d->stim_l2_miss_req_ready_i = 0xff;
  d->stim_l2_resp_valid_i = 0;
  std::memset(&d->stim_l2_resp_i, 0, sizeof(d->stim_l2_resp_i));
  d->stim_apb_paddr_i = 0;
  d->stim_apb_pwrite_i = 0;
  d->stim_apb_psel_i = 0;
  d->stim_apb_penable_i = 0;
  d->stim_apb_pwdata_i = 0;
}

bool wait_for_ready(CosimCtrl<DUT>& sim, int max_cycles = 512) {
  for (int i = 0; i < max_cycles; ++i) {
    if (sim.dut->orig_id_core_ready && sim.dut->new_id_core_ready) return true;
    tick_and_compare(sim);
  }
  return sim.dut->orig_id_core_ready && sim.dut->new_id_core_ready;
}

void issue_core_req(DUT* d, uint64_t addr, uint8_t thread_id, uint8_t cmd, uint8_t typ,
                    bool phys, uint8_t dest_addr) {
  d->stim_s0_core_req_valid_i = 1;
  d->stim_req_addr_i = addr;
  d->stim_req_addr_msb_err_i = 0;
  d->stim_req_dest_fp_i = 0;
  d->stim_req_dest_addr_i = dest_addr;
  d->stim_req_thread_id_i = thread_id;
  d->stim_req_cmd_i = cmd;
  d->stim_req_typ_i = typ;
  d->stim_req_gsc_cnt_i = 0;
  d->stim_req_ps_mask_i = 0xff;
  d->stim_req_phys_i = phys ? 1 : 0;
  d->stim_req_mem_global_i = 0;
  d->stim_req_gsc32_idx_i = 0;
}

void drive_core_req_fields(DUT* d, uint64_t addr, uint8_t thread_id, uint8_t cmd,
                           uint8_t typ, bool phys, uint8_t dest_addr, bool valid) {
  issue_core_req(d, addr, thread_id, cmd, typ, phys, dest_addr);
  d->stim_s0_core_req_valid_i = valid ? 1 : 0;
}

void set_wide_bits(uint32_t* words, int lsb, int width, uint64_t value) {
  for (int bit = 0; bit < width; ++bit) {
    const int dst_bit = lsb + bit;
    const uint32_t mask = uint32_t{1} << (dst_bit & 31);
    if ((value >> bit) & 1ULL) {
      words[dst_bit >> 5] |= mask;
    } else {
      words[dst_bit >> 5] &= ~mask;
    }
  }
}

uint64_t get_wide_bits(const uint32_t* words, int lsb, int width) {
  uint64_t value = 0;
  for (int bit = 0; bit < width; ++bit) {
    const int src_bit = lsb + bit;
    if ((words[src_bit >> 5] >> (src_bit & 31)) & 1U) {
      value |= uint64_t{1} << bit;
    }
  }
  return value;
}

void set_store_data_lo(DUT* d, uint64_t value) {
  std::memset(&d->stim_s1_core_store_data_i, 0, sizeof(d->stim_s1_core_store_data_i));
  set_wide_bits(&d->stim_s1_core_store_data_i[0], 0, 64, value);
}

uint8_t miss_id(const uint32_t* miss_req) {
  return static_cast<uint8_t>(get_wide_bits(miss_req, 81, 5));
}

uint8_t miss_size(const uint32_t* miss_req) {
  return static_cast<uint8_t>(get_wide_bits(miss_req, 11, 3));
}

uint64_t miss_addr(const uint32_t* miss_req) {
  return get_wide_bits(miss_req, 34, 40);
}

uint64_t core_resp_addr(const uint32_t* core_resp) {
  return get_wide_bits(core_resp, 0, 49);
}

uint64_t core_resp_data_lo(const uint32_t* core_resp) {
  return get_wide_bits(core_resp, 49, 64);
}

uint8_t co_debug_meta_read_resp_way_en(const uint32_t* co_debug_data) {
  return static_cast<uint8_t>(
      get_wide_bits(co_debug_data, kCoDbgMetaReadRespWayEnLsb, kDcacheWays));
}

bool reset_and_wait_ready(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  clear_inputs(d);
  d->rst_ni = 0;
  for (int i = 0; i < 4; ++i) sim.tick();
  d->rst_ni = 1;
  clear_inputs(d);

  for (int i = 0; i < 2; ++i) tick_and_compare(sim);
  return wait_for_ready(sim);
}

bool run_physical_smoke(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  if (!reset_and_wait_ready(sim)) {
    std::printf("dcache directed: not ready before physical smoke\n");
    return false;
  }

  clear_inputs(d);
  drive_core_req_fields(d, kPhysAddr, 0, kCmdXrd, kTypeW, true, 1, true);
  tick_and_compare(sim);
  bool saw_bp = d->orig_s1_bp_conf_valid;

  clear_inputs(d);
  drive_core_req_fields(d, kPhysAddr, 0, kCmdXrd, kTypeW, true, 1, true);
  tick_and_compare(sim);
  saw_bp |= d->orig_s1_bp_conf_valid;

  for (int i = 0; i < 16; ++i) {
    clear_inputs(d);
    tick_and_compare(sim);
    saw_bp |= d->orig_s1_bp_conf_valid;
    if (saw_bp) break;
  }

  if (!saw_bp) {
    std::printf("dcache directed: physical smoke did not reach breakpoint output\n");
  }
  return saw_bp;
}

bool run_ptw_smoke(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  if (!reset_and_wait_ready(sim)) {
    std::printf("dcache directed: not ready before PTW smoke\n");
    return false;
  }

  clear_inputs(d);
  d->stim_satp_info_en_i = 1;
  tick_and_compare(sim);

  clear_inputs(d);
  d->stim_satp_mode_i = kModeSv39;
  d->stim_satp_ppn_i = static_cast<uint32_t>(kExpectedPtwPpn);
  tick_and_compare(sim);

  clear_inputs(d);
  d->stim_vm_prv_t0_i = kPrvS;
  drive_core_req_fields(d, kVirtAddr, 0, kCmdXrd, kTypeW, false, 3, true);
  tick_and_compare(sim);

  bool saw_ptw = false;
  for (int i = 0; i < 32; ++i) {
    clear_inputs(d);
    d->stim_vm_prv_t0_i = kPrvS;
    tick_and_compare(sim);
    if (d->orig_ptw_req_valid) {
      saw_ptw = true;
      const uint64_t ptw_addr = get_wide_bits(&d->orig_ptw_req_data[0], 0, 36);
      if (ptw_addr != kExpectedPtwAddr) {
        std::printf("dcache directed: PTW addr mismatch in original, got 0x%llx\n",
                    static_cast<unsigned long long>(ptw_addr));
        return false;
      }
      break;
    }
  }

  if (!saw_ptw) {
    std::printf("dcache directed: virtual smoke did not issue PTW request\n");
  }
  return saw_ptw;
}

void drive_load_request_addr(DUT* d, uint64_t addr, bool alloc_pre, bool alloc_val, bool valid) {
  clear_inputs(d);
  d->stim_id_core_alloc_rq_pre_i = alloc_pre ? 1 : 0;
  d->stim_s0_core_alloc_rq_val_i = alloc_val ? 1 : 0;
  drive_core_req_fields(d, addr, 0, kCmdXrd, kTypeD, kLoadStorePhys, 4, valid);
}

void drive_load_request(DUT* d, bool alloc_pre, bool alloc_val, bool valid) {
  drive_load_request_addr(d, kLoadStoreVirtAddr, alloc_pre, alloc_val, valid);
}

void drive_store_request(DUT* d, bool alloc_pre, bool alloc_val, bool valid) {
  clear_inputs(d);
  d->stim_id_core_alloc_rq_pre_i = alloc_pre ? 1 : 0;
  d->stim_s0_core_alloc_rq_val_i = alloc_val ? 1 : 0;
  drive_core_req_fields(d, kLoadStoreVirtAddr, 0, kCmdXwr, kTypeD, kLoadStorePhys, 5, valid);
  set_store_data_lo(d, kStoreWrite);
}

LoadObserveResult observe_load_response_addr(CosimCtrl<DUT>& sim, uint64_t expected_addr,
                                             uint64_t expected_data, const char* label) {
  auto* d = sim.dut.get();
  bool saw_orig_resp = false;
  bool saw_new_resp = false;
  int replay_cycles = 0;
  int s2_resp_cycles = 0;
  int extra_miss_cycles = 0;
  int fill_accept_cycles = 0;
  int fill_write_cycles = 0;
  int mh_fill_resp_cycles = 0;
  int meta_write_cycles = 0;
  int replay_hit_cycles = 0;
  int replay_dealloc_cycles = 0;
  int replay_renack_cycles = 0;
  int replay_uc_cycles = 0;
  bool saw_target_s1 = false;
  bool saw_target_s1_meta_valid = false;
  bool saw_target_s1_tag_match = false;
  bool saw_target_s2 = false;
  bool saw_target_s2_tag_match = false;
  bool saw_target_s2_state_hit = false;
  bool saw_target_s2_hit = false;
  bool saw_target_s2_miss_req = false;
  bool saw_target_co_debug_way_en = false;
  uint8_t extra_miss_id = 0;
  uint8_t extra_miss_size = 0;
  uint64_t extra_miss_addr = 0;
  for (int i = 0; i < 1024; ++i) {
    drive_load_request_addr(d, expected_addr, false, false, false);
    tick_and_compare(sim);
    const bool target_s1 =
        d->orig_s1_valid_to_s2
        && d->orig_s1_req_addr == expected_addr
        && d->orig_s1_phys_addr == expected_addr
        && d->orig_s1_req_cmd == kCmdXrd
        && d->orig_s1_req_typ == kTypeD;
    const bool target_s2 =
        d->orig_s2_valid_qual
        && d->orig_s2_req_addr == expected_addr
        && d->orig_s2_req_cmd == kCmdXrd
        && d->orig_s2_req_typ == kTypeD;
    if (target_s1) {
      saw_target_s1 = true;
      saw_target_s1_meta_valid |= d->orig_s1_mr_read_valid != 0;
      saw_target_s1_tag_match |= d->orig_s1_tag_match != 0;
    }
    if (target_s2) {
      saw_target_s2 = true;
      saw_target_s2_tag_match |= d->orig_s2_tag_match_any;
      saw_target_s2_state_hit |= d->orig_s2_state_hit;
      saw_target_s2_hit |= d->orig_s2_hit;
      saw_target_s2_miss_req |= d->orig_s2_mh_req_valid;
      if (d->orig_s2_tag_match != 0) {
        const uint8_t orig_dbg_way =
            co_debug_meta_read_resp_way_en(&d->orig_sm_data_debug_signals_co[0]);
        const uint8_t new_dbg_way =
            co_debug_meta_read_resp_way_en(&d->new_sm_data_debug_signals_co[0]);
        const uint8_t orig_way = d->orig_s2_tag_match & ((1u << kDcacheWays) - 1u);
        const uint8_t new_way = d->new_s2_tag_match & ((1u << kDcacheWays) - 1u);
        if (orig_dbg_way != orig_way || new_dbg_way != new_way) {
          std::printf("dcache directed: %s cache-op debug way-en mismatch "
                      "orig_dbg=0x%x orig_s2=0x%x new_dbg=0x%x new_s2=0x%x\n",
                      label, orig_dbg_way, orig_way, new_dbg_way, new_way);
          return LoadObserveResult::BadOriginalResponse;
        }
        saw_target_co_debug_way_en = true;
      }
    }
    replay_cycles += d->orig_s1_core_replay_next ? 1 : 0;
    s2_resp_cycles += d->orig_s2_core_resp_int_valid ? 1 : 0;
    fill_accept_cycles +=
        (d->orig_l2_resp_valid_int && d->orig_l2_resp_ready_int
         && d->orig_l2_resp_is_for_mh_fill) ? 1 : 0;
    fill_write_cycles += d->orig_l2_da_write ? 1 : 0;
    mh_fill_resp_cycles +=
        ((d->orig_mh_state_0 == 3) || (d->orig_mh_state_1 == 3)) ? 1 : 0;
    meta_write_cycles +=
        ((d->orig_mh_state_0 == 4) || (d->orig_mh_state_1 == 4)) ? 1 : 0;
    replay_hit_cycles +=
        (d->orig_s2_req_replay && d->orig_s2_hit && d->orig_s2_req_cacheable) ? 1 : 0;
    replay_dealloc_cycles += d->orig_s2_rq_dealloc ? 1 : 0;
    replay_renack_cycles += d->orig_s2_rq_renack ? 1 : 0;
    replay_uc_cycles +=
        (d->orig_s2_req_replay && d->orig_s2_req_uc_load_2nd) ? 1 : 0;
    if (d->orig_l2_miss_req_valid) {
      extra_miss_cycles++;
      extra_miss_id = miss_id(&d->orig_l2_miss_req[0]);
      extra_miss_addr = miss_addr(&d->orig_l2_miss_req[0]);
      extra_miss_size = miss_size(&d->orig_l2_miss_req[0]);
    }
    saw_orig_resp |= d->orig_s3_core_resp_valid;
    saw_new_resp |= d->new_s3_core_resp_valid;
    if (d->orig_s3_core_resp_valid) {
      const uint64_t data = core_resp_data_lo(&d->orig_s3_core_resp[0]);
      const uint64_t addr = core_resp_addr(&d->orig_s3_core_resp[0]);
      if (data != expected_data || addr != expected_addr) {
        std::printf("dcache directed: %s original response data=0x%llx addr=0x%llx\n",
                    label, static_cast<unsigned long long>(data),
                    static_cast<unsigned long long>(addr));
        return LoadObserveResult::BadOriginalResponse;
      }
    }
    if (saw_orig_resp && saw_new_resp) {
      // This observer starts after the request issue helper has already ticked the
      // S1 cycle. The S2 lookup/miss decision is the stable evidence for hit-path
      // classification here.
      if (!saw_target_s2 || !saw_target_s2_tag_match || !saw_target_s2_state_hit
          || !saw_target_s2_hit || saw_target_s2_miss_req
          || !saw_target_co_debug_way_en || extra_miss_cycles != 0) {
        std::printf("dcache directed: %s lookup evidence failed s1=%u meta=%u tag1=%u s2=%u tag2=%u state=%u hit=%u miss=%u co_dbg=%u extra_miss=%d\n",
                    label,
                    saw_target_s1 ? 1 : 0,
                    saw_target_s1_meta_valid ? 1 : 0,
                    saw_target_s1_tag_match ? 1 : 0,
                    saw_target_s2 ? 1 : 0,
                    saw_target_s2_tag_match ? 1 : 0,
                    saw_target_s2_state_hit ? 1 : 0,
                    saw_target_s2_hit ? 1 : 0,
                    saw_target_s2_miss_req ? 1 : 0,
                    saw_target_co_debug_way_en ? 1 : 0,
                    extra_miss_cycles);
        return LoadObserveResult::BadOriginalResponse;
      }
      return LoadObserveResult::Completed;
    }
  }

  std::printf("dcache directed: %s replay=%d s2_resp=%d fill_accept=%d fill_write=%d mh_fill=%d meta=%d replay_hit=%d dealloc=%d renack=%d replay_uc=%d extra_miss=%d id=%u size=%u addr=0x%llx orig_resp=%u new_resp=%u lookup s1=%u meta=%u tag1=%u s2=%u tag2=%u state=%u hit=%u miss=%u co_dbg=%u\n",
              label,
              replay_cycles, s2_resp_cycles, fill_accept_cycles, fill_write_cycles,
              mh_fill_resp_cycles, meta_write_cycles, replay_hit_cycles,
              replay_dealloc_cycles, replay_renack_cycles, replay_uc_cycles,
              extra_miss_cycles, extra_miss_id,
              extra_miss_size,
              static_cast<unsigned long long>(extra_miss_addr), saw_orig_resp ? 1 : 0,
              saw_new_resp ? 1 : 0,
              saw_target_s1 ? 1 : 0,
              saw_target_s1_meta_valid ? 1 : 0,
              saw_target_s1_tag_match ? 1 : 0,
              saw_target_s2 ? 1 : 0,
              saw_target_s2_tag_match ? 1 : 0,
              saw_target_s2_state_hit ? 1 : 0,
              saw_target_s2_hit ? 1 : 0,
              saw_target_s2_miss_req ? 1 : 0,
              saw_target_co_debug_way_en ? 1 : 0);
  std::printf("dcache directed: last orig state mh0=%u mh1=%u l2v=%u l2r=%u is_fill=%u da_write=%u slot=%u refill_way=%u refill_addr=0x%x s2v=%u s2q=%u hit=%u mhv=%u mhr=%u replay=%u cacheable=%u uc2=%u cmd=%u typ=%u addr=0x%llx\n",
              d->orig_mh_state_0, d->orig_mh_state_1, d->orig_l2_resp_valid_int,
              d->orig_l2_resp_ready_int, d->orig_l2_resp_is_for_mh_fill,
              d->orig_l2_da_write, d->orig_l2_fill_slot_avail,
              d->orig_s4_mh_refill_way, d->orig_s4_mh_refill_addr,
              d->orig_s2_valid, d->orig_s2_valid_qual, d->orig_s2_hit,
              d->orig_s2_mh_req_valid, d->orig_s2_mh_req_ready,
              d->orig_s2_req_replay, d->orig_s2_req_cacheable,
              d->orig_s2_req_uc_load_2nd, d->orig_s2_req_cmd, d->orig_s2_req_typ,
              static_cast<unsigned long long>(d->orig_s2_req_addr));
  return LoadObserveResult::TimedOut;
}

LoadObserveResult observe_load_response(CosimCtrl<DUT>& sim, uint64_t expected_data,
                                        const char* label) {
  return observe_load_response_addr(sim, kLoadStoreVirtAddr, expected_data, label);
}

bool run_direct_dcache_smoke_sequence(CosimCtrl<DUT>& sim) {
  return run_physical_smoke(sim)
      && run_ptw_smoke(sim);
}

void apb_begin_write(DUT* d, uint16_t addr, uint64_t data) {
  d->stim_apb_paddr_i = addr;
  d->stim_apb_pwrite_i = 1;
  d->stim_apb_psel_i = 1;
  d->stim_apb_penable_i = 0;
  d->stim_apb_pwdata_i = data;
}

void apb_begin_read(DUT* d, uint16_t addr) {
  d->stim_apb_paddr_i = addr;
  d->stim_apb_pwrite_i = 0;
  d->stim_apb_psel_i = 1;
  d->stim_apb_penable_i = 0;
  d->stim_apb_pwdata_i = 0;
}

void apb_clear(DUT* d) {
  d->stim_apb_psel_i = 0;
  d->stim_apb_penable_i = 0;
  d->stim_apb_pwrite_i = 0;
  d->stim_apb_paddr_i = 0;
  d->stim_apb_pwdata_i = 0;
}

void apb_write(CosimCtrl<DUT>& sim, uint16_t addr, uint64_t data) {
  apb_begin_write(sim.dut.get(), addr, data);
  tick_and_compare(sim);
  sim.dut->stim_apb_penable_i = 1;
  for (int i = 0; i < 12; ++i) {
    tick_and_compare(sim);
    if (sim.dut->orig_apb_pready && sim.dut->new_apb_pready) break;
  }
  apb_clear(sim.dut.get());
  tick_and_compare(sim);
}

void apb_read(CosimCtrl<DUT>& sim, uint16_t addr) {
  apb_begin_read(sim.dut.get(), addr);
  tick_and_compare(sim);
  sim.dut->stim_apb_penable_i = 1;
  for (int i = 0; i < 12; ++i) {
    tick_and_compare(sim);
    if (sim.dut->orig_apb_pready && sim.dut->new_apb_pready) break;
  }
  apb_clear(sim.dut.get());
  tick_and_compare(sim);
}

void program_cache_line_via_apb(CosimCtrl<DUT>& sim, uint8_t way, uint64_t addr, uint8_t state,
                                const uint64_t data[4]) {
  apb_write(sim, make_md_addr(way, cache_set(addr)), cache_meta_word(state, addr));
  for (uint8_t bank = 0; bank < 4; ++bank) {
    apb_write(sim, make_da_addr(way, cache_lram_debug_idx(addr, bank)), data[bank]);
  }
}

void program_cache_line_via_apb(CosimCtrl<DUT>& sim, uint8_t way, uint64_t addr,
                                const uint64_t data[4]) {
  program_cache_line_via_apb(sim, way, addr, kStateModified, data);
}

bool issue_direct_load(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  if (!wait_for_ready(sim)) {
    std::printf("dcache directed: not ready before direct load\n");
    return false;
  }

  drive_load_request(d, true, false, false);
  tick_and_compare(sim);
  drive_load_request(d, false, true, true);
  tick_and_compare(sim);
  drive_load_request(d, false, false, false);
  return true;
}

bool issue_direct_load_addr(CosimCtrl<DUT>& sim, uint64_t addr) {
  auto* d = sim.dut.get();
  if (!wait_for_ready(sim)) {
    std::printf("dcache directed: not ready before direct load\n");
    return false;
  }

  drive_load_request_addr(d, addr, true, false, false);
  tick_and_compare(sim);
  drive_load_request_addr(d, addr, false, true, true);
  tick_and_compare(sim);
  drive_load_request_addr(d, addr, false, false, false);
  return true;
}

bool issue_direct_store(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  if (!wait_for_ready(sim)) {
    std::printf("dcache directed: not ready before direct store\n");
    return false;
  }

  drive_store_request(d, true, false, false);
  tick_and_compare(sim);
  drive_store_request(d, false, true, true);
  tick_and_compare(sim);
  drive_store_request(d, false, false, false);
  return true;
}

bool wait_for_no_miss_store_hit(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  bool saw_l2_miss = false;
  bool saw_ready = false;
  bool saw_s2_store = false;
  bool saw_s4_write = false;
  uint64_t s4_write_data = 0;
  uint64_t s4_write_da0 = 0;
  for (int i = 0; i < 128; ++i) {
    drive_store_request(d, false, false, false);
    tick_and_compare(sim);
    saw_l2_miss |= d->orig_l2_miss_req_valid != 0;
    saw_ready |= d->orig_id_core_ready && d->new_id_core_ready;
    if (d->orig_s2_valid && d->orig_s2_req_cmd == kCmdXwr) {
      saw_s2_store = true;
    }
    if (d->orig_s4_da_write_en) {
      saw_s4_write = true;
      s4_write_data = d->orig_s4_req_data_lo;
      s4_write_da0 = d->orig_s4_da_write_data_0;
    }
    if (saw_ready && i >= 8) break;
  }

  if (saw_l2_miss) {
    std::printf("dcache directed: APB-prefilled store unexpectedly missed\n");
  }
  if (!saw_s2_store || !saw_s4_write) {
    std::printf("dcache directed: store hit path incomplete s2=%u s4_write=%u ready=%u\n",
                saw_s2_store ? 1 : 0, saw_s4_write ? 1 : 0, saw_ready ? 1 : 0);
  }
  if (s4_write_data != kStoreWrite || s4_write_da0 != kStoreWrite) {
    std::printf("dcache directed: store write data mismatch s4=0x%llx da0=0x%llx\n",
                static_cast<unsigned long long>(s4_write_data),
                static_cast<unsigned long long>(s4_write_da0));
    return false;
  }
  return !saw_l2_miss && saw_ready && saw_s2_store && saw_s4_write;
}

bool run_apb_prefilled_load_store_sequence(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  if (!reset_and_wait_ready(sim)) {
    std::printf("dcache directed: not ready before APB-prefilled load/store\n");
    return false;
  }

  const uint64_t initial_line[4] = {
      kLoadSeed,
      0x1111222233334444ULL,
      0x5555666677778888ULL,
      0x9999AAAABBBBCCCCULL,
  };
  program_cache_line_via_apb(sim, kPrefillWay, kLoadStorePhysAddr, initial_line);

  clear_inputs(d);
  for (int i = 0; i < 8; ++i) tick_and_compare(sim);

  if (!issue_direct_load(sim)) return false;
  if (observe_load_response(sim, kLoadSeed, "APB-prefilled load hit")
      != LoadObserveResult::Completed) {
    return false;
  }

  if (!issue_direct_store(sim)) return false;
  return wait_for_no_miss_store_hit(sim);
}

bool run_fpga_harness_apb_prefilled_load_sequence(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  if (!reset_and_wait_ready(sim)) {
    std::printf("dcache directed: not ready before FPGA-harness APB-prefilled load\n");
    return false;
  }

  const uint64_t initial_line[4] = {
      kLoadSeed,
      0x1111222233334444ULL,
      0x5555666677778888ULL,
      0x9999AAAABBBBCCCCULL,
  };
  program_cache_line_via_apb(sim, kPrefillWay, kFpgaLoadStorePhysAddr, kStateShared, initial_line);

  clear_inputs(d);
  for (int i = 0; i < 8; ++i) tick_and_compare(sim);

  if (!issue_direct_load_addr(sim, kFpgaLoadStoreVirtAddr)) return false;
  return observe_load_response_addr(sim, kFpgaLoadStoreVirtAddr, kLoadSeed,
                                    "FPGA-harness APB-prefilled load hit")
      == LoadObserveResult::Completed;
}

void randomize_inputs(DUT* d, int cycle) {
  clear_inputs(d);

  d->stim_shire_id_i = static_cast<uint8_t>(xorshift32());
  d->stim_shire_min_id_i = static_cast<uint8_t>(xorshift32());
  d->stim_ioshire_i = xorshift32() & 1;
  d->stim_chicken_bit_dcache_i = xorshift32() & 1;
  d->stim_s1_core_kill_i = xorshift32() & 1;
  d->stim_s2_core_kill_i = xorshift32() & 1;
  d->stim_s3_core_x31_i = xorshift64();
  d->stim_s3_invalidate_lr_i = xorshift32() & 1;
  d->stim_wb_dcache_fp_toint_i = xorshift32() & 1;
  d->stim_vmspagesize_i = static_cast<uint8_t>(xorshift32());
  d->stim_bypass_dcache_i = xorshift32() & 1;
  d->stim_mem_ctrl_override_i = static_cast<uint8_t>(xorshift32() & 0x3);
  d->stim_satp_mode_i = (xorshift32() & 1) ? 0 : kModeSv39;
  d->stim_satp_ppn_i = xorshift32() & ((1u << 22) - 1u);
  d->stim_matp_mode_i = xorshift32() & 1;
  d->stim_matp_ppn_i = xorshift32() & ((1u << 22) - 1u);
  d->stim_satp_info_en_i = xorshift32() & 1;
  d->stim_matp_info_en_i = xorshift32() & 1;
  d->stim_vm_prv_t0_i = static_cast<uint8_t>(xorshift32() & 0x3);
  d->stim_vm_mprv_t0_i = xorshift32() & 1;
  d->stim_vm_mpp_t0_i = static_cast<uint8_t>(xorshift32() & 0x3);
  d->stim_vm_sum_t0_i = xorshift32() & 1;
  d->stim_vm_mxr_t0_i = xorshift32() & 1;
  d->stim_vm_debug_t0_i = xorshift32() & 1;
  d->stim_vm_prv_t1_i = static_cast<uint8_t>(xorshift32() & 0x3);
  d->stim_vm_mprv_t1_i = xorshift32() & 1;
  d->stim_vm_mpp_t1_i = static_cast<uint8_t>(xorshift32() & 0x3);
  d->stim_vm_sum_t1_i = xorshift32() & 1;
  d->stim_vm_mxr_t1_i = xorshift32() & 1;
  d->stim_vm_debug_t1_i = xorshift32() & 1;
  d->stim_tlb_invalidate_i = xorshift32() & 1;
  d->stim_ptw_req_ready_i = xorshift32() & 1;
  d->stim_l2_evict_req_ready_i = static_cast<uint8_t>(xorshift32());
  d->stim_l2_miss_req_ready_i = static_cast<uint8_t>(xorshift32());

  if ((xorshift32() & 0x7) == 0) {
    d->stim_s0_core_req_valid_i = 1;
    d->stim_req_addr_i = xorshift64() & ((1ULL << 48) - 1ULL);
    d->stim_req_addr_msb_err_i = 0;
    d->stim_req_dest_fp_i = xorshift32() & 1;
    d->stim_req_dest_addr_i = static_cast<uint8_t>(xorshift32());
    d->stim_req_thread_id_i = xorshift32() & 1;
    d->stim_req_ps_mask_i = 0xff;
    d->stim_req_phys_i = (xorshift32() & 0x3) != 0;
    d->stim_req_mem_global_i = xorshift32() & 1;
    d->stim_req_gsc32_idx_i = 0;

    switch (xorshift32() & 0x7) {
      case 0: d->stim_req_cmd_i = kCmdXrd; break;
      case 1: d->stim_req_cmd_i = kCmdXwr; break;
      case 2: d->stim_req_cmd_i = 2; break;   // DcCmdPfr
      case 3: d->stim_req_cmd_i = 3; break;   // DcCmdPfw
      case 4: d->stim_req_cmd_i = 6; break;   // DcCmdXlr
      case 5: d->stim_req_cmd_i = 7; break;   // DcCmdXsc
      case 6: d->stim_req_cmd_i = 8; break;   // DcCmdXaAdd
      default: d->stim_req_cmd_i = 10; break; // DcCmdXaOr
    }

    switch (xorshift32() & 0x7) {
      case 0: d->stim_req_typ_i = 0; break;   // DcTypeB
      case 1: d->stim_req_typ_i = 1; break;   // DcTypeH
      case 2: d->stim_req_typ_i = kTypeW; break;
      case 3: d->stim_req_typ_i = kTypeD; break;
      case 4: d->stim_req_typ_i = 4; break;   // DcTypeBu
      case 5: d->stim_req_typ_i = 5; break;   // DcTypeHu
      case 6: d->stim_req_typ_i = 6; break;   // DcTypeWu
      default: d->stim_req_typ_i = 0xb; break; // DcTypePs
    }

    fill_random_bytes(&d->stim_s1_core_store_data_i, sizeof(d->stim_s1_core_store_data_i));
  }
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);

  clear_inputs(sim.dut.get());
  sim.dut->rst_ni = 0;
  for (int i = 0; i < 10; ++i) sim.tick();
  sim.dut->rst_ni = 1;
  clear_inputs(sim.dut.get());

  for (int i = 0; i < 32; ++i) tick_and_compare(sim);
  sim.check(wait_for_ready(sim), "both tops reach ready after reset");
  for (int i = 0; i < 32; ++i) tick_and_compare(sim);

  sim.check(run_direct_dcache_smoke_sequence(sim),
            "direct dcache physical/PTW smoke sequence matches original");

  sim.check(run_apb_prefilled_load_store_sequence(sim),
            "APB-prefilled scalar dcache load hit and store write interface match original");

  sim.check(run_fpga_harness_apb_prefilled_load_sequence(sim),
            "FPGA-harness APB-prefilled scalar load hit matches original");

  sim.check(reset_and_wait_ready(sim), "both tops reach ready before APB debug sequence");

  apb_write(sim, make_md_addr(2, 5), (uint64_t{1} << 33) | 0x123456789ULL);
  apb_write(sim, make_md_addr(1, 9), (uint64_t{2} << 33) | 0x1ABCDEFFFULL);
  apb_read(sim, make_md_addr(2, 5));
  apb_read(sim, make_md_addr(1, 9));

  apb_write(sim, make_da_addr(1, 0x10), 0x1111222233334444ULL);
  apb_write(sim, make_da_addr(1, 0x11), 0x5555666677778888ULL);
  apb_write(sim, make_da_addr(1, 0x12), 0x9999AAAABBBBCCCCULL);
  apb_write(sim, make_da_addr(1, 0x13), 0xDDDDEEEEFFFF0000ULL);
  apb_read(sim, make_da_addr(1, 0x10));
  apb_read(sim, make_da_addr(1, 0x11));
  apb_read(sim, make_da_addr(1, 0x12));
  apb_read(sim, make_da_addr(1, 0x13));

  clear_inputs(sim.dut.get());
  for (int i = 0; i < 64; ++i) tick_and_compare(sim);

  clear_inputs(sim.dut.get());
  sim.dut->stim_force_ts_scp_start_i = 1;
  tick_and_compare(sim);
  clear_inputs(sim.dut.get());
  for (int i = 0; i < 8; ++i) tick_and_compare(sim);

  for (int cycle = 0; cycle < kRandomCycles; ++cycle) {
    clear_inputs(sim.dut.get());
    sim.dut->rst_ni = 1;
    if (cycle >= 128 && cycle < 130) {
      sim.dut->rst_ni = 0;
    } else {
      randomize_inputs(sim.dut.get(), cycle);
    }
    tick_and_compare(sim);
  }

  return sim.finish();
}
