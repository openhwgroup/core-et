/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"

#include "ipi.h"

inline void set_ipi_trigger(uint64_t local_hart_id) {
   uint64_t *esr_local_ipi_trigger = (uint64_t *) 0x80F40090;
   // Trigger all the HARTS within the shire, except for the 'local_hart_id'
   uint64_t my_mask = ((uint64_t) 1) << local_hart_id;
   *esr_local_ipi_trigger = ETDV_SHIRE_HARTS_MASK & (~my_mask);
}

uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart) {
  (void)tval;
  uint64_t local_hart_id = hart % 64;
  if (local_hart_id == ETDV_LEAD_HART) {
    C_TEST_FAIL;
  }
  else {
    if (cause != CAUSE_IPI_TRIGGER) {
      C_TEST_FAIL;
    }
    else {
      // Clear IPI trigger
      uint64_t *esr_local_ipi_trigger_clear = (uint64_t *) 0x80F40098;
      *esr_local_ipi_trigger_clear = ((uint64_t)1) << local_hart_id;
      switch (local_hart_id%3) {
        case 0:
          // Infinite loop
          return epc+4;
        case 1:
          // STALL
          return epc;
        case 2:
          // WFI
          return epc;
      }
    }
  }
  return epc;
}

int main() {
  uint64_t local_hart_id = get_hart_id()%64;

  C_TEST_START;

  if (local_hart_id == ETDV_LEAD_HART) {
     for (int i = 0; i < 20000; i++) {
        NOP;
     }
     set_ipi_trigger(local_hart_id);
  }
  else {
    // Enable SW Interrupts
    setup_interrupts();
    // Setup Trap Vector
    setup_trap_vector();
    switch (local_hart_id%3) {
      case 0:
        // Infinite loop
        __asm__ __volatile__ (
          "wait_ipi:\n"
          "j wait_ipi\n"
        );
        break;
      case 1:
        STALL;
        break;
      case 2:
        WFI;
        break;
    }
  }
  C_TEST_PASS;

  return 0;
}
