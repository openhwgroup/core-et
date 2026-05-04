//
// This test exposes a bypass network bug where T1 WB stage
// bypasses data to T0 ID stage
//

#include "macros.h"

.text
.global _start
_start: 
    ASM_TEST_START
 
    csrr t3, mhartid
    li t0, 16
    li t1, -16

    beq t3, x0, thread_0

thread_1:
    mulw t0, t0, t1

    li t2, -256
    beq t0, t2, pass
    j fail

thread_0:
    addi t0, t0, -8

    li t2, 8
    beq t0, t2, pass
    j fail

fail:
    ASM_TEST_FAIL
    j end_loop
pass:
    ASM_TEST_PASS

end_loop:
    j end_loop

