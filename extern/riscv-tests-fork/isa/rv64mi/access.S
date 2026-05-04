# See LICENSE for license details.

#*****************************************************************************
# access.S
#-----------------------------------------------------------------------------
#
# Test access-exception behavior.
#

#include "riscv_test.h"
#include "test_macros.h"

RVTEST_RV64M
RVTEST_CODE_BEGIN

  .align 2

  # jalr to an illegal address should commit (hence should write rd).
  # after the pc is set to rs1, the fetch should just proceed by dropping higher order bits.
  li TESTNUM, 2
  la t3, 1f

  # Flipping just the MSB should result in an illegal address for RV64.
  li t0, 1 << (__riscv_xlen - 1)
  xor t0, t0, t3

  jalr t2, t0
  nop
1:

  j pass

  TEST_PASSFAIL

  .align 12
  .global mtvec_handler
mtvec_handler:
  li a0, 2
  beq TESTNUM, a0, fail
  li a0, 3
  beq TESTNUM, a0, 2f
  j fail

2:
  bne t2, t3, fail

  csrr t2, mcause
  bne t2, t1, fail

  csrw mepc, t3
  mret

RVTEST_CODE_END

  .data
RVTEST_DATA_BEGIN

  TEST_DATA

RVTEST_DATA_END
