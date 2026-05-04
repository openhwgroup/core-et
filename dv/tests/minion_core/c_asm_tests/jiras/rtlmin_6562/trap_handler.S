
#include "macros.h"
#include "rtlmin_6562.h"

.global test_pass_vector
    .align 12

test_pass_vector:
loop_init:
    li a0, WAIT_REQ_CYCLES
wait:                 // Wait until the request finish. Otherwise we can't read CSR
    addi a0, a0, -1
    bnez a0, wait
check_cause:          // Slow delay done. Double check we trap for the right cause
    csrr a0, mcause
    li   a1, MSIP_CAUSE
    sub a0, a1, a0
    beqz a0, test_pass
    ASM_TEST_FAIL
test_pass:
    ASM_TEST_PASS
