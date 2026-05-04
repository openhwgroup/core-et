#include "macros.h"
     
.text
.global _start
_start: 
    ASM_TEST_START
    // setup trap vector
    la t0, trap_vector
    csrw mtvec, t0
    // go to test
    csrr t0, mhartid
    andi t0, t0, 15
    slli t0, t0, 2
    la t1, 1f
    add t1,t0,t1
    jr t1
1:  j test_hart_0
    j test_hart_1
    j test_hart_2
    j test_hart_3
    j test_hart_4
    j test_hart_5
    j test_hart_6
    j test_hart_7
    j test_hart_8
    j test_hart_9
    j test_hart_a
    j test_hart_b
    j test_hart_c
    j test_hart_d
    j test_hart_e
    j test_hart_f
    ASM_TEST_FAIL
        
.option rvc
#include "code.s"
        
.p2align 12
trap_vector:
    csrw mscratch, t0
    // only illegal instruction expected
    csrr t0, mcause
    addi t0, t0, -2
    bnez t0, bad_cause
    // add +2 or +4 to mepc and return
    csrr t0, mtval
    andi t0, t0, 3
    addi t0, t0, -3
    bnez t0, illegal_rvc
    csrr t0, mepc
    addi t0, t0, 4
    csrw mepc, t0
    csrr t0, mscratch
    mret
illegal_rvc:
    csrr t0, mepc
    addi t0, t0, 2
    csrw mepc, t0
    csrr t0, mscratch
    mret
bad_cause:
        ASM_TEST_FAIL

.data
.p2align 6
#include "data.s"
