#include "macros.h"

.text
.global _start
_start: 
    ASM_TEST_START

    // setup mask and data pointer
    li t0, 0
    li t1, 0xffffffff80000000

    fmv.s.x f0, t0   // positive zero
    fmv.s.x f1, t1   // negative zero


    fmax.s f2, f0, f1
    fmv.x.s t2, f2
    bne t2, t0, fail  // FMAX(+0,-0) = +0


    fmin.s f2, f0, f1
    fmv.x.s t2, f2
    bne t2, t1, fail  // FMIN(+0,-0) = -0

end:
    ASM_TEST_PASS
end_loop:
    j end_loop
fail:
    ASM_TEST_FAIL
