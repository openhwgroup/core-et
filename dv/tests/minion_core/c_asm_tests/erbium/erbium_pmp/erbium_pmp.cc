/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "cacheops.h"
#include "et_test_common.h"

#define MRAM_BASE 0x40000000ULL  // MRAM starts at 1GB
#define SMODE_SIZE 11 // Leave some room for code execution 
#define MMODE_SIZE 11 // 2^11 * 4KB = 8MB region 

typedef struct {
   uint64_t reserved;   // [63:8]
   uint64_t en;         // [8] MPROT enable
   uint64_t mmode_size; // [7:4] M-mode region size (encoded as 2^n * 4KB)
   uint64_t smode_size; // [3:0] S-mode region size (encoded as 2^n * 4KB)
} esr_mprot_t;

// Test counters and state
volatile uint32_t test_fail = 0;
volatile uint32_t exceptions_caught = 0;
volatile esr_prot_t cur_priv = PRV_M; // Start in M-mode
volatile esr_mprot_t next_mprot; // Start in M-mode
volatile uint64_t *mprot = (uint64_t*) ESR_NEIGH(0, MPROT);
const char* priv_str[] = {"U", "S", "H", "M"};

/**
 * Write MPROT register
 */
void write_mprot() {
   //et_printf_long("Updating:  MPROT = {EN=%d, MMODE_SIZE=%d, SMODE_SIZE=%d}\n", next_mprot.en, next_mprot.mmode_size, next_mprot.smode_size);
   *mprot = next_mprot.en << 8 | (next_mprot.mmode_size << 4) | (next_mprot.smode_size << 0);
}

void req_mprot_update() {
   // Trigger ECALL to update MPROT in trap handler
   if (cur_priv == PRV_S || cur_priv == PRV_U) {
      __asm__ __volatile__ ("ecall");
   } else {
      write_mprot();
   }
}

extern "C" void test_mtrap_vector();
extern "C" __attribute__((section(".text"))) uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart) {
   (void) tval;
   (void) hart;
   if (cause == MINION_XCTP_CAUSE_LOAD_ACCESS_FAULT) {
      exceptions_caught++;
      //et_printf_long("INFO: Caught exception cause %d, tval %lx and epc %lx\n", cause, tval, epc);
      epc+= 8;
   } else if (cause == MINION_XCTP_CAUSE_STORE_ACCESS_FAULT) {
      exceptions_caught++;
      //et_printf_long("INFO: Caught exception cause %d, tval %lx and epc %lx\n", cause, tval, epc);
      epc+= 8;
   } else if (cause == MINION_XCTP_CAUSE_ENV_CALL_FROM_S_MODE) {
      //et_printf_long("INFO: Caught expected ECALL from S-mode\n");
      write_mprot(); // Update MPROT for next test
      epc+= 8;
   } else if(cause == MINION_XCTP_CAUSE_ENV_CALL_FROM_U_MODE) {
      //et_printf_long("INFO: Caught expected ECALL from U-mode\n");
      write_mprot(); // Update MPROT for next test
      epc+= 8;
   } else {
      //et_printf_long("FAIL: Caught unexpected exception cause %d\n", cause);
      //et_printf_long("FAIL: tval %lx\n", tval);
      //et_printf_long("FAIL: epc %lx\n", epc);
      C_TEST_FAIL;
   } // Unexpected exception
   
   return epc;
}

void set_trap_vector()
{
    void (*mtvec_ptr)(void) = &test_mtrap_vector;
    __asm__ __volatile__ ("csrw mtvec, %[mtvec]\n\t" : :[mtvec] "r" (mtvec_ptr) :);
}

// Naked attribute to ensure no prologue/epilogue is generated, allowing us to control the exact instructions executed
__attribute__((naked)) void drop_to_s_mode(uintptr_t entry) {
   uint64_t status = 0;
   
   __asm__ __volatile__ ("csrr %[result], mstatus\n": [result] "=r" (status));
   status &= ~(0x3ULL << 11);
   status |= (0x1ULL << 11); // Set to S-mode
   __asm__ __volatile__ ("csrw mstatus, %[val]\n"  :: [val] "r" (status): );
   __asm__ __volatile__ ("csrw mepc, %[val]\n"     :: [val] "r" (entry):);
   __asm__ __volatile__ ("mret" ::: "memory");
}

__attribute__((naked)) void drop_to_u_mode(uintptr_t entry) {
   uint64_t status = 0;
   
   __asm__ __volatile__ ("csrr %[result], sstatus\n": [result] "=r" (status));
   status &= ~(0x3ULL << 11);
   status |= (0x0ULL << 11); // Set to U-mode
   __asm__ __volatile__ ("csrw sstatus, %[val]\n"  :: [val] "r" (status): );
   __asm__ __volatile__ ("csrw sepc, %[val]\n"     :: [val] "r" (entry):);
   __asm__ __volatile__ ("sret");
}

/**
 * Determine if an access to the given address should cause an exception based on current MPROT configuration and privilege level
 */
uint64_t is_exception_expected(uint64_t addr) {
   if (next_mprot.en == 0) {
      return false; // All accesses allowed
   }
   uint64_t mmode_limit = MRAM_BASE + (1ULL << (next_mprot.mmode_size + 12)); // M-mode region limit
   uint64_t smode_limit = MRAM_BASE + (1ULL << (next_mprot.smode_size + 12)); // S-mode region limit

   if (addr < mmode_limit) {
      return (cur_priv == PRV_S || cur_priv == PRV_U) ? true : false; // M-mode only
   } else if (addr < smode_limit) {
      return (cur_priv == PRV_U) ? true : false; // S-mode only
   } else {
      return false; // M/S/U accessible
   }
}

/**
 * Check exceptions 
 */
void check_exception(uint64_t addr) {
   if (is_exception_expected(addr)) {
      if(exceptions_caught>0) {
         //et_printf_long("OK: Caught expected exception\n");
      } else {
         //et_printf_long("FAIL: Expected exception but none caught\n");
         test_fail++;
         C_TEST_FAIL;
      }
   } else {
      if(exceptions_caught>0) {
         //et_printf_long("FAIL: Caught unexpected exception\n");
         test_fail++;
         C_TEST_FAIL;
      } else {
         //et_printf_long("OK: No exception as expected\n");
      }
   }
   exceptions_caught = 0; // Reset for next test
}

uint64_t calc_region_limit(uint64_t size) {
   return MRAM_BASE + (1ULL << (size + 12)); // size is encoded as 2^n * 4KB
}

uint64_t addr_to_region(uint64_t addr) {
   uint64_t mmode_limit = calc_region_limit(next_mprot.mmode_size);
   uint64_t smode_limit = calc_region_limit(next_mprot.smode_size);

   if (addr < mmode_limit) {
      return PRV_M;
   } else if (addr < smode_limit) {
      return PRV_S;
   } else {
      return PRV_U;
   }
}

uint64_t test_mram_address(uint64_t addr) {
   uint64_t buf;
   //et_printf_long("Accessing %s-mode region with privilege %s at 0x%lx: ", priv_str[addr_to_region(addr)], priv_str[cur_priv], addr);
   buf = *(volatile uint64_t *)addr;
   FENCE;
   check_exception(addr);
   return buf; // Attempt to read from the address
}

/**
 * Tests access to MRAM regions under different MPROT configurations
 */
void test_mram_regions()
{
   uint64_t mmode_limit;
   uint64_t smode_limit;

   // Test 1: MPROT Disabled
   // When MPROT_EN=0, all privilege levels can access all memory
   // Do not cover the entire range of sizes to save test time
   
   //et_printf_long("Test 1: Reading with MPROT_EN=0 at privilege level %d\n", cur_priv);
   next_mprot.en = 0;

   for (int mmode_size = 0; mmode_size <= MMODE_SIZE; mmode_size=mmode_size+4) {
      for (int smode_size = mmode_size; smode_size <= SMODE_SIZE; smode_size=smode_size+4) {
         next_mprot.smode_size = smode_size;
         next_mprot.mmode_size = mmode_size;
         FENCE;
         req_mprot_update();

         FENCE; // Ensure MPROT update is visible before testing addresses
         mmode_limit = calc_region_limit(next_mprot.mmode_size);
         smode_limit = calc_region_limit(next_mprot.smode_size);

         test_mram_address(mmode_limit - 8ULL); // M-mode region limit (inside)
         test_mram_address(mmode_limit - 1ULL); // M-mode region limit (crossing)
         test_mram_address(mmode_limit);        // M-mode region limit (outside)

         if (smode_size > mmode_size) { // Only test S-mode limits if S-mode region is larger than M-mode
            test_mram_address(smode_limit - 8ULL); // S-mode region limit (inside)
            test_mram_address(smode_limit - 1ULL); // S-mode region limit (crossing)
            test_mram_address(smode_limit);        // S-mode region limit (outside)
         }
      }
   }
   
   //et_printf_long("End of Test 1 with privilege level %d\n\n", cur_priv);

   // Test 2: MPROT Enabled
   // Cover the entire range of sizes to ensure all configurations are tested

   //et_printf_long("Test 2: Reading with MPROT_EN=1 at privilege level %d\n", cur_priv);
   next_mprot.en = 1;
   
   for (int mmode_size = 0; mmode_size <= MMODE_SIZE; mmode_size++) {
      for (int smode_size = mmode_size; smode_size <= SMODE_SIZE; smode_size++) {
         next_mprot.smode_size = smode_size;
         next_mprot.mmode_size = mmode_size;
         FENCE;
         req_mprot_update();

         FENCE; // Ensure MPROT update is visible before testing addresses
         mmode_limit = calc_region_limit(next_mprot.mmode_size);
         smode_limit = calc_region_limit(next_mprot.smode_size);

         test_mram_address(mmode_limit - 8ULL); // M-mode region limit (inside)
         test_mram_address(mmode_limit - 1ULL); // M-mode region limit (crossing)
         test_mram_address(mmode_limit);        // M-mode region limit (outside)

         if (smode_size > mmode_size) { // Only test S-mode limits if S-mode region is larger than M-mode
            test_mram_address(smode_limit - 8ULL); // S-mode region limit (inside)
            test_mram_address(smode_limit - 1ULL); // S-mode region limit (crossing)
            test_mram_address(smode_limit);        // S-mode region limit (outside)
         }
      }
   }
   //et_printf_long("End of Test 2 with privilege level %d\n\n", cur_priv);
}

int main() {
   C_TEST_START;

   uint64_t tid;
   uint64_t v;
   uint64_t bootrom = 0x200a000ULL;
   tid = get_thread_id();

   // Setup machine trap vector
   set_trap_vector();

   // Only thread 0 executes tests
   if (tid == 0) {
      
      et_printf_long("Quick prefetch test\n");
      __asm__ __volatile__ ("csrw prefetch_va, %[val]\n"  :: [val] "r" (bootrom): );
      WAIT_CACHEOPS; // Ensure prefetch is issued before starting tests
      asm volatile ("csrr %0, 0x808" : "=r"(v));
      et_printf_long("TensorErr status: %d\n", v);

      et_printf_long("\n=== Erbium PMP Specification Tests ===\n\n");

      et_printf_long("Current Privilege Level: M-mode\n");
      cur_priv = PRV_M;
      test_mram_regions();

      et_printf_long("Current Privilege Level: S-mode\n");
      cur_priv = PRV_S;
      drop_to_s_mode((uintptr_t)test_mram_regions);

      et_printf_long("Current Privilege Level: U-mode\n");
      cur_priv = PRV_U;
      drop_to_u_mode((uintptr_t)test_mram_regions);

      if (test_fail > 0) {
         C_TEST_FAIL;
         et_printf_long("FAIL: %d test(s) failed\n", test_fail);
      }
   }

   C_TEST_PASS;
   return 0;
}
