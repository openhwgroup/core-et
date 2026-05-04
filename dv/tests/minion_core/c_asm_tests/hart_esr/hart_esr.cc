/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <inttypes.h>
#include <stdlib.h>

#include "et_test_common.h"
#include "macros.h"
#include "minion.h"
#include "hart_esr.h"
#include "et_postsi_common.h"

void setup()
{
  // Setup interrupts
  __asm__ volatile ("csrsi mstatus, 0x8\n"); // Enable MIE
  __asm__ volatile ("li x31, 0x800000\n" // Enable bus error interrupt
                    "csrs mie, x31\n"
                    ::: "x31"
                    );
  
  // Set trap vector
  __asm__ __volatile__
    (
     "la t0, test_mtrap_vector\n"
     "csrw mtvec, t0\n"
     : : : "t0");
  
}

int main()
{
  C_TEST_START;
  setup();
  
  const uint64_t hart_id = get_hart_id();

  // Ignore threads > 0
  if (hart_id != 0)
    c_test_pass();
  
  const uint64_t local_hart_id = hart_id & 0x3f;
  const uint64_t next_hart_id = local_hart_id + (1 << 1); // minion 1 hart 0
  constexpr uint64_t SHIRE_ID = 0xFFULL;
  constexpr uint64_t BASE_HART_ADDR = 1ULL << 32 |
    SHIRE_ID << 22;

  // Test list
  // Test1: access hart space (no esr)
  // Test2: access esr dbg (abscmd)
  // Test3: access hart space (no esr) (target next minion which should be disabled)
  // Test4: access esr dbg (abscmd)  (target next minion which should be disabled)
  
  constexpr uint64_t ABSCMD_ADDR = (0x8 << 3) | (0xf << 7);
  const uint64_t ADDR_HART = BASE_HART_ADDR | (local_hart_id << 12);
  const uint64_t ADDR_HART_DBG = BASE_HART_ADDR | (local_hart_id << 12) | ABSCMD_ADDR;
  const uint64_t ADDR_NEXT_HART = BASE_HART_ADDR | (next_hart_id << 12);
  const uint64_t ADDR_NEXT_HART_DBG = BASE_HART_ADDR | (next_hart_id << 12) | ABSCMD_ADDR;
  
  volatile uint64_t *addr;
  uint64_t mscratch;
  
  // Test 1 start
  addr = reinterpret_cast<uint64_t*>(ADDR_HART);
  __asm__ volatile("la %[lb_addr], test1\n"
                   : [lb_addr] "=r" (mscratch)
                   ::);
  set_mscratch(mscratch);
  *addr = 0;
  FENCE;
  NOP;
  C_TEST_FAIL;
  __asm__ __volatile__ ("test1:");
  // Test 1 ends
  // Test 2 start
  addr = reinterpret_cast<uint64_t*>(ADDR_HART_DBG);
  __asm__ volatile("la %[lb_addr], test2\n"
                   : [lb_addr] "=r" (mscratch)
                   ::);
  set_mscratch(mscratch);
  *addr = 0;
  FENCE;
  NOP;
  C_TEST_FAIL;
  __asm__ __volatile__ ("test2:");
  // Test 2 ends
  // Test 3 start
  addr = reinterpret_cast<uint64_t*>(ADDR_NEXT_HART);
  __asm__ volatile("la %[lb_addr], test3\n"
                   : [lb_addr] "=r" (mscratch)
                   ::);
  set_mscratch(mscratch);
  *addr = 0;
  FENCE;
  NOP;
  C_TEST_FAIL;
  __asm__ __volatile__ ("test3:");
  // Test 3 ends
  // Test 4 start
  addr = reinterpret_cast<uint64_t*>(ADDR_NEXT_HART_DBG);
  __asm__ volatile("la %[lb_addr], test4\n"
                   : [lb_addr] "=r" (mscratch)
                   ::);
  set_mscratch(mscratch);
  *addr = 0;
  FENCE;
  NOP;
  C_TEST_FAIL;
  __asm__ __volatile__ ("test4:");
  // Test 4 ends
  
  // All test done
  c_test_pass();
  return 0;
}


uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart)
{
  (void)cause;
  (void)epc;
  (void)tval;
  (void)hart;
  bool interrupt = (cause >> 63) & 0x1;
  uint64_t cause_code = cause & ((1 << 5)-1);
  
  // It should report error
  if (interrupt && cause_code != CAUSE_BUS_ERROR)
    C_TEST_FAIL;

  // clear interrupts
  __asm__ volatile ("csrwi mip, 0");
  
  // Get the next test address and continue
  const uint64_t next_addr = get_mscratch();
  return next_addr;
}


