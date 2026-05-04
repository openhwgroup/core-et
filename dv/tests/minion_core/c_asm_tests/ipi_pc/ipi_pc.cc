/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "ipi_pc.h"
#include "tensors.h"
#include "et_postsi_common.h"

constexpr uint64_t NUM_THREADS = 2048;
constexpr uint64_t NUM_TESTS = 5;
constexpr uint64_t ESR_ADDR = 1ULL << 32 | // IPI_REDIRECT ESR
  0b00ULL << 30 |
  0xFFULL << 22 |
  0b01ULL << 20 |
  0x08ULL << 3;


float array[64][64] = {0};

int main()
{
  const uint64_t hart_id = get_hart_id();

  C_TEST_START;

  if (hart_id != 0)
    c_test_pass();

  // Setup
  setup_trap_vector();
  setup_cache_scp();
  setup_interrupts();
  setup_pc_redirect();
  m_to_u_mode();

  // Start test
  test0();

  C_TEST_FAIL;

  return 0;
}

void test0()
{

  // Start long latecy operation
  uint64_t addr = reinterpret_cast<uint64_t>(&array);
  tensor_load_setup_b(0, //use coop
		      addr,
		      0xF, // num lines
		      1, // stride
		      0); // ID

  tensor_fma(0, // use tmask
	     0x3, // num columns B
	     0xF, // num rows A
	     0xF, // num cols B
	     0, // offset
	     0, // tenc_loc (unused)
	     0, // tenb_unsigned (unused)
	     0, // tena_unsigned (unused)
	     1, // tenb_location
	     0, // scp_loc_b (not used when it is in memory)
	     0, // scp_loc_b starting cache line
	     0, // Tensor type (FMA32)
	     true);

  // Trigger redirect
  constexpr uint64_t CURRENT_SHIRE = 0xFF << 22;
  constexpr uint64_t IPI_REDIRECT_TRIGGER = 0x80340080 | CURRENT_SHIRE;
  constexpr uint64_t IPI_REDIRECT_TRIGGER_VALUE = 0x1;
  volatile uint64_t* ipi_redirect_trigger_esr = reinterpret_cast<uint64_t*>(IPI_REDIRECT_TRIGGER);

  *ipi_redirect_trigger_esr = IPI_REDIRECT_TRIGGER_VALUE;

  // Stall to receive the redirect
  STALL;
}




inline void delay(int d)
{
  while (d--) {
    __asm__ __volatile__ ("nop");
  }
}


uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart)
{
  (void)tval;
  uint64_t thread = hart & 0x1;
  if (thread != 0) {
    C_TEST_FAIL;
  } else {
    if (cause != CAUSE_USER_ECALL) {
      delay(10);
      C_TEST_FAIL;
    } else {
      // Next instruction and set m-mode
      __asm__ __volatile__ ("csrr t0, mstatus\n"
			    "li t1, 0x1800\n"
			    "or t0, t0, t1\n"
			    "csrw mstatus, t0\n"
			    ::: "t0", "t1");
      return epc+4;
    }
  }

  // Bad redirect. Retry
  return epc;
}

void handler0()
{
  to_m_mode();
  c_test_pass();
}


////////////////////////////////////////////////////////////////
// Aux funcs
////////////////////////////////////////////////////////////////
void setup_interrupts()
{
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

void setup_trap_vector()
{

  // Enabla mhaid on u mode
  __asm__ __volatile__ ("csrwi 0x7d2, 1");

  //configure trap vector
  __asm__ __volatile__
    (
     "la t0, test_mtrap_vector\n"
     "csrw mtvec, t0\n"
     : : : "t0");
}


uint64_t get_flb(uint64_t cnt, uint64_t n)
{
  uint64_t rd, rs;
  rs = ((n-1) << 5) | cnt;

  __asm__ __volatile__
    ( "csrrw %[rd], 0x820, %[rs]\n"
      : [rd] "=r" (rd)
      : [rs] "r"  (rs)
      );
  return rd;

}

void m_to_u_mode()
{
  __asm__ __volatile__
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6000\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mod
     "1:\n"                     // label to referr to user-mode
     :
     :
     : "t0"
     );
}

void to_m_mode()
{
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
}

void setup_pc_redirect()
{
  constexpr uint64_t CURRENT_SHIRE = 0xFF << 22;
  constexpr uint64_t IPI_REDIRECT_FILTER = 0x80F40088 | CURRENT_SHIRE;
  constexpr uint64_t IPI_REDIRECT_PC = 0x80100040 | CURRENT_SHIRE;
  constexpr uint64_t IPI_REDIRECT_FILTER_VALUE  = 0x1;
  const uint64_t IPI_REDIRECT_PC_VALUE = reinterpret_cast<uint64_t>(&handler0);

  volatile uint64_t* ipi_redirect_pc_esr = reinterpret_cast<uint64_t*>(IPI_REDIRECT_PC);
  volatile uint64_t* ipi_redirect_filter_esr = reinterpret_cast<uint64_t*>(IPI_REDIRECT_FILTER);

  *ipi_redirect_pc_esr = IPI_REDIRECT_PC_VALUE;
  *ipi_redirect_filter_esr = IPI_REDIRECT_FILTER_VALUE;
  FENCE;
}

inline uint64_t __attribute__((always_inline)) get_hart_id()
{
    uint64_t ret;
    __asm__ __volatile__ (
        "csrr %[ret], hartid\n"
      : [ret] "=r" (ret)
      :
      :
    );
    return ret;
}

inline uint64_t __attribute__((always_inline)) get_shire_id()
{
   return (get_hart_id() >> 6) & 0x3FULL;
}

inline uint64_t __attribute__((always_inline)) get_minion_id()
{
   return (get_hart_id() >> 1) & 0x1FULL;
}


inline uint64_t __attribute__((always_inline)) get_thread_id()
{
   return get_hart_id() & 1;
}
