//
// Purpose: Check that no exception is generated when vector accesses are completely masked
//

#include "macros.h"

.text
.global _start
_start: 

    ASM_TEST_START

    csrw mtval, t0
    csrw stval, t0

    //configure mtvec and stvec
    la t0, mtrap_vector
    csrw mtvec, t0
    la t0, strap_vector
    csrw stvec, t0

    li t0, 0
    mova.m.x t0

    // Masked accesses to address zero. Would cause exception if not
    // masked but here it should not
    flw.ps f1, 0(t0)
    fsw.ps f1, 0(t0)
    fbc.ps f1, 0(t0)
    fgw.ps f1, f0(t0)
    fgh.ps f1, f0(t0)
    fgb.ps f1, f0(t0)
    fscw.ps f1, f0(t0)
    fsch.ps f1, f0(t0)
    fscb.ps f1, f0(t0)
    fg32w.ps f1, x0(t0)
    fg32h.ps f1, x0(t0)
    fg32b.ps f1, x0(t0)
    fsc32w.ps f1, x0(t0)
    fsc32h.ps f1, x0(t0)
    fsc32b.ps f1, x0(t0) 
    
    // FUTURE: Emmanuel Marie Page cross masked accesses

test_pass:
    ASM_TEST_PASS
    j 1f
test_fail:
    ASM_TEST_FAIL
1:
    j 1b


// Fail if we take an exception
.align 12
mtrap_vector:
    j test_fail

.align 12
strap_vector:
    j test_fail


