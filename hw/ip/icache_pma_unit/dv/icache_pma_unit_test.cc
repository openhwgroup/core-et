// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vicache_pma_unit_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vicache_pma_unit_tb;

namespace {

constexpr uint8_t kPrvU = 0;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;

constexpr uint8_t kDramSize8G  = 0;
constexpr uint8_t kDramSize16G = 1;
constexpr uint8_t kDramSize24G = 2;
constexpr uint8_t kDramSize32G = 3;

struct Mprot {
  bool enable_secure_memory;
  uint8_t dram_size;
  bool disable_osbox_access;
  bool disable_pcie_access;
  uint8_t io_access_mode;
};

struct PmaResp {
  bool access_fault;
  bool cacheable;
};

uint8_t pack_mprot(const Mprot& mp) {
  return uint8_t(((mp.enable_secure_memory ? 1U : 0U) << 6)
               | ((mp.dram_size & 0x3U) << 4)
               | ((mp.disable_osbox_access ? 1U : 0U) << 3)
               | ((mp.disable_pcie_access ? 1U : 0U) << 2)
               |  (mp.io_access_mode & 0x3U));
}

bool paddr_is_sp_space(uint64_t paddr)         { return ((paddr >> 30) & 0x3FFULL) == 0x001ULL; }
bool paddr_is_sp_rom(uint64_t paddr)           { return paddr_is_sp_space(paddr) && (((paddr >> 17) & 0x1FFFULL) == 0x0000ULL); }
bool paddr_is_sp_sram(uint64_t paddr)          { return paddr_is_sp_space(paddr) && (((paddr >> 20) & 0x3FFULL) == 0x004ULL); }
bool paddr_is_sp_sram_code(uint64_t paddr)     { return paddr_is_sp_space(paddr) && (((paddr >> 19) & 0x7FFULL) == 0x008ULL); }
bool paddr_is_sp_sram_data(uint64_t paddr)     { return paddr_is_sp_space(paddr) && (((paddr >> 19) & 0x7FFULL) == 0x009ULL); }
bool paddr_is_sp_cacheable(uint64_t paddr)     { return paddr_is_sp_rom(paddr) || paddr_is_sp_sram(paddr); }
bool paddr_is_dram(uint64_t paddr)             { return ((paddr >> 39) & 1ULL) != 0ULL; }
bool paddr_is_dram_cacheable(uint64_t paddr)   { return paddr_is_dram(paddr) && (((paddr >> 38) & 1ULL) == 0ULL); }
bool paddr_is_dram_mbox(uint64_t paddr)        { return paddr_is_dram(paddr) && (((paddr >> 23) & 0x7FFFULL) == 0ULL); }
bool paddr_is_dram_mcode(uint64_t paddr)       { return paddr_is_dram_mbox(paddr) && (((paddr >> 21) & 0x3ULL) == 0ULL); }
bool paddr_is_dram_mdata(uint64_t paddr)       { return paddr_is_dram_mbox(paddr) && (((paddr >> 21) & 0x3ULL) != 0ULL); }
bool paddr_is_dram_scode(uint64_t paddr)       { return paddr_is_dram(paddr) && (((paddr >> 26) & 0xFFFULL) == 0ULL) && (((paddr >> 23) & 0x7ULL) == 0x1ULL); }
bool paddr_is_dram_sdata(uint64_t paddr)       { return paddr_is_dram(paddr) && (((paddr >> 26) & 0xFFFULL) == 0ULL) && (((paddr >> 24) & 0x3ULL) != 0ULL); }
bool paddr_is_dram_osbox(uint64_t paddr)       { return paddr_is_dram(paddr) && (((paddr >> 32) & 0x3FULL) == 0ULL) && (((paddr >> 26) & 0x3FULL) != 0ULL); }
bool paddr_is_dram_over_8g(uint64_t paddr)     { return paddr_is_dram(paddr) && (((paddr >> 33) & 0x1FULL) != 0ULL); }
bool paddr_is_dram_over_16g(uint64_t paddr)    { return paddr_is_dram(paddr) && (((paddr >> 34) & 0xFULL) != 0ULL); }
bool paddr_is_dram_over_24g(uint64_t paddr)    { return paddr_is_dram(paddr) && ((((paddr >> 33) & 0x1FULL) == 0x03ULL) || (((paddr >> 35) & 0x7ULL) != 0ULL)); }
bool paddr_is_dram_over_32g(uint64_t paddr)    { return paddr_is_dram(paddr) && (((paddr >> 35) & 0x7ULL) != 0ULL); }

bool paddr_is_dram_other(uint64_t paddr, uint8_t dram_size) {
  return paddr_is_dram(paddr) && (((paddr >> 32) & 0x3FULL) != 0ULL) &&
         !(((dram_size == kDramSize8G)  && paddr_is_dram_over_8g(paddr)) ||
           ((dram_size == kDramSize16G) && paddr_is_dram_over_16g(paddr)) ||
           ((dram_size == kDramSize24G) && paddr_is_dram_over_24g(paddr)) ||
           ((dram_size == kDramSize32G) && paddr_is_dram_over_32g(paddr)));
}

PmaResp pma_model(bool ioshire, uint64_t paddr, const Mprot& mprot, uint8_t ptw_prv) {
  const bool icache_req_dram_mcode_permitted = !ioshire && (ptw_prv == kPrvM);
  const bool icache_req_dram_mdata_permitted = !ioshire && !mprot.enable_secure_memory && (ptw_prv == kPrvM);
  const bool icache_req_dram_scode_permitted = mprot.enable_secure_memory
                                             ? (!ioshire && (ptw_prv == kPrvS))
                                             : (!ioshire && !mprot.disable_osbox_access);
  const bool icache_req_dram_sdata_permitted = !ioshire && !mprot.enable_secure_memory && !mprot.disable_osbox_access;
  const bool icache_req_dram_osbox_permitted = !ioshire && !mprot.disable_osbox_access &&
                                               (!mprot.enable_secure_memory || (ptw_prv == kPrvU));
  const bool icache_req_dram_other_permitted = !ioshire && paddr_is_dram_cacheable(paddr) &&
                                               (!mprot.enable_secure_memory || (ptw_prv == kPrvU));
  const bool icache_req_sp_sram_code_permitted = ioshire && (ptw_prv != kPrvU);
  const bool icache_req_sp_sram_data_permitted = ioshire && (ptw_prv == kPrvM);

  PmaResp resp {};
  resp.cacheable = paddr_is_dram(paddr) || (ioshire && paddr_is_sp_cacheable(paddr));
  resp.access_fault =
      !((paddr_is_sp_rom(paddr)                     && ioshire) ||
        (paddr_is_sp_sram_code(paddr)              && icache_req_sp_sram_code_permitted) ||
        (paddr_is_sp_sram_data(paddr)              && icache_req_sp_sram_data_permitted) ||
        (paddr_is_dram_mcode(paddr)                && icache_req_dram_mcode_permitted) ||
        (paddr_is_dram_mdata(paddr)                && icache_req_dram_mdata_permitted) ||
        (paddr_is_dram_scode(paddr)                && icache_req_dram_scode_permitted) ||
        (paddr_is_dram_sdata(paddr)                && icache_req_dram_sdata_permitted) ||
        (paddr_is_dram_osbox(paddr)                && icache_req_dram_osbox_permitted) ||
        (paddr_is_dram_other(paddr, mprot.dram_size) && icache_req_dram_other_permitted));
  return resp;
}

void check_case(SimCtrl<DUT>& sim, bool ioshire, uint64_t paddr, const Mprot& mprot, uint8_t ptw_prv,
                const std::string& label) {
  sim.dut->ioshire_i = ioshire;
  sim.dut->paddr_i = paddr;
  sim.dut->mprot_i = pack_mprot(mprot);
  sim.dut->ptw_prv_i = ptw_prv;
  sim.tick();

  const PmaResp expect = pma_model(ioshire, paddr, mprot, ptw_prv);
  sim.check(sim.dut->access_fault_o == expect.access_fault, label + " access_fault");
  sim.check(sim.dut->cacheable_o == expect.cacheable, label + " cacheable");
}

uint32_t xorshift32(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.dut->ioshire_i = 0;
  sim.dut->paddr_i = 0;
  sim.dut->mprot_i = 0;
  sim.dut->ptw_prv_i = 0;
  sim.reset();

  const Mprot mprot_default {false, kDramSize16G, false, false, 3};
  const Mprot mprot_secure {true, kDramSize16G, false, false, 3};
  const Mprot mprot_no_osbox {false, kDramSize16G, true, false, 3};

  check_case(sim, true,  0x0040000000ULL, mprot_default, kPrvS, "sp rom allowed");
  check_case(sim, true,  0x0040400000ULL, mprot_default, kPrvS, "sp sram code supervisor");
  check_case(sim, true,  0x0040800000ULL, mprot_default, kPrvU, "sp sram data user blocked");
  check_case(sim, false, 0x8000001000ULL, mprot_default, kPrvM, "dram mcode machine");
  check_case(sim, false, 0x8000200000ULL, mprot_default, kPrvM, "dram mdata machine");
  check_case(sim, false, 0x8000800000ULL, mprot_no_osbox, kPrvS, "dram osbox disabled");
  check_case(sim, false, 0x8002000000ULL, mprot_default, kPrvU, "dram other user allowed");
  check_case(sim, false, 0x8002000000ULL, mprot_secure, kPrvS, "dram other secure blocked");

  uint32_t seed = 0x2468ACE1U;
  for (int i = 0; i < 120; i++) {
    Mprot mp {
      (xorshift32(seed) & 1u) != 0u,
      uint8_t(xorshift32(seed) & 0x3u),
      (xorshift32(seed) & 1u) != 0u,
      (xorshift32(seed) & 1u) != 0u,
      uint8_t(xorshift32(seed) & 0x3u)
    };
    const uint64_t addr =
        ((uint64_t(xorshift32(seed)) << 20) ^ uint64_t(xorshift32(seed))) & ((1ULL << 40) - 1);
    const bool ioshire = (xorshift32(seed) & 1u) != 0u;
    const uint8_t prv = uint8_t(xorshift32(seed) & 0x3u);
    check_case(sim, ioshire, addr, mp, prv, "random");
  }

  return sim.finish();
}
