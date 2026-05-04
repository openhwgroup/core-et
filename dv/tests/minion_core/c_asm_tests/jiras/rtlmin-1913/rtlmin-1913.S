//
// This test has to be run with +disable_random_data plusarg
// to avoid checker mismatch due to non initialized randomized
// data that we access in Mcode DRAM region
//
// While a privileged access to Mcode region is outstanding
// in the dcache, the privileged is lowered to user mode.
// When the request is replayed, it should not raised an exception.
//
// In a second part, the test checks that executing an mret in U-mode
// does cause a trap
//

#include "macros.h"

.text
.global _start
_start: 
    ASM_TEST_START

    li x10, 1

    // Set up trap vector to fail if we take an exception
    la x1, fail
    csrw mtvec, x1
    csrw stvec, x1

    // Set up mstatus and mepc so that mret puts us in S-mode and continues
    la x1, keepgoing
    csrw mepc, x1
    li x1, 0x1000
    csrc mstatus, x1

    // L1 miss with address to M-mode region
    li x2, 0x40000100
    ld x3, 0(x2)

    // Add a few NOPs to allow the Load to leave the dcache
    nop
    nop
    nop
    nop

    // jump to S-mode while miss outstanding
    // should not trigger PMA fault
    mret

keepgoing:
    nop
    nop
    nop
    fence
    addi x10, x10, -1

    // MRET from u-mode should take an exception
    mret
    ASM_TEST_FAIL

.align 12
fail:
    beq x0, x10, pass
    ASM_TEST_FAIL

pass:
    ASM_TEST_PASS
