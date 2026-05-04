/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


extern "C" void test_mtrap_vector();
extern "C" uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart);

void setup_interrupts() {
  // Configure interrupts
  __asm__ volatile
    (
     "li x29, 0x10008\n" // Set SW and Bad redirect
     "csrr x30, mie\n"    // Read mie
     "or x31, x30, x29\n" // Set bit 3 (msi)
     "csrw mie, x31\n"    // Set mie
     "csrr x31, mstatus\n"
     "or x31, x31, x29\n"
     "csrw mstatus, x31\n" // Set mstatus mie (enable mmode interrupts)
     :
     :
     : "x31", "x30", "x29"
     );

}

void setup_trap_vector() {
  __asm__ __volatile__
    (
     "la t0, test_mtrap_vector\n"
     "csrw mtvec, t0\n"
     : : : "t0");
}

inline uint64_t __attribute__((always_inline)) get_hart_id() {
    uint64_t ret;
    __asm__ __volatile__ (
        "csrr %[ret], mhartid\n"
      : [ret] "=r" (ret)
      :
      :
    );
    return ret;
}
