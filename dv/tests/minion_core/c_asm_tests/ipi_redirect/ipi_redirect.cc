/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"

#include "ipi.h"

// needed for sysemu, `add_diag()` provides these in et-dvrun
#ifndef ETDV_SHIRE_HARTS_MASK
#define ETDV_SHIRE_HARTS_MASK 0xffff
#endif

#ifndef ETDV_LEAD_HART
#define ETDV_LEAD_HART 0x0
#endif

inline void set_ipi_redirect_filter(uint64_t local_hart_id) {
  uint64_t* ipi_redirect_filter = (uint64_t*) (0x80F40088);
  uint64_t my_mask = ((uint64_t) 1) << local_hart_id;
  *ipi_redirect_filter = ETDV_SHIRE_HARTS_MASK & (~my_mask);
}

inline void set_ipi_redirect_pc(uint64_t redirected_pc) {
  uint64_t* ipi_redirect_pc = (uint64_t*) (0x80100040);
  *ipi_redirect_pc = redirected_pc;
}

inline void set_ipi_redirect_trigger() {
  uint64_t* ipi_redirect_trigger = (uint64_t*) (0x80340080);
  *ipi_redirect_trigger = 0xFFFF; // 16 harts
}

uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart) {
  (void)cause;
  (void)tval;
  (void)hart;
  uint64_t test_pass;
  __asm__ __volatile__ (
    "la %[test_pass], follower_test_pass\n"
    : [test_pass] "=r" (test_pass)
    :
    :
  );
  if (epc == test_pass) {
    __asm__ __volatile__ ( "lead_test_pass:\n" );
    C_TEST_PASS;
  }
  else {
    // IPI redirect PC should jump to the PC instead of executing the trap handler
    C_TEST_FAIL;
  }
  return 0xbadc0de;
}

void m_to_u_mode() {
  __asm__ __volatile__
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6000\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mode
     "1:\n"                     // label to refer to user-mode
     :
     :
     : "t0"
     );
}

inline void to_m_mode() {
  __asm__ __volatile__ ("follower_test_pass:\n"
                        "ecall\n");
}

int main() {
  uint64_t local_hart_id = get_hart_id()%64;

  C_TEST_START;

  if (local_hart_id == ETDV_LEAD_HART) {
    for (int i = 0; i < 10000; i++) {
      NOP;
    }

    // Set the filter to exclude myself
    set_ipi_redirect_filter(local_hart_id);

    // Set the redirect PC to point to C_TEST_PASS
    uint64_t redirected_pc;
    __asm__ __volatile__ (
        "la %[redirected_pc], ipi_redirected_pc\n"
      : [redirected_pc] "=r" (redirected_pc)
      :
      :
    );
    set_ipi_redirect_pc(redirected_pc);

    FENCE;

    // Trigger the redirect
    set_ipi_redirect_trigger();
    __asm__ __volatile__ ( "j lead_test_pass" );
  }
  else {
    // Enable SW Interrupts
    setup_interrupts();
    // Setup Trap Vector
    setup_trap_vector();
    // IPI redirect only works in U-mode
    m_to_u_mode();

    uint64_t my_mask = ((uint64_t) 1) << local_hart_id;
    if ((my_mask & ETDV_SHIRE_HARTS_MASK) != 0) {
      // Wait for the interrupt
      STALL;
    }
    C_TEST_FAIL;
    __asm__ __volatile__ ( "ipi_redirected_pc:\n" );
    to_m_mode();
  }

  return 0;
}
