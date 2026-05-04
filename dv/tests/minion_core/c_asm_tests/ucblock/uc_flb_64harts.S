#include "macros.h"

#define ESR_FLB_REG_00                       0x80340100ULL
#define FLB_DATA(EXPECTED_HARTIDS, BARRIER)  ((((EXPECTED_HARTIDS)-1)<<5)| (BARRIER))

.macro test_case num_hartids, barrier
    li x4, \num_hartids
    li x5, ESR_FLB_REG_00 + 8*\barrier
    li x6, FLB_DATA(\num_hartids, \barrier)
    li x7, 64
    csrr x3, mhartid

    bnez x3, 1f
    call do_hartid_0
    j 4f

1:	blt x3, x7, 2f
    wfi

2:	blt x3, x4, 3f
    call set_fcc_counter_done
    j 4f

3:  call do_hartid_others
4:
.endm

.text
.global _start
_start:

    ASM_TEST_START

    // Toggle all the num_hartids and barrier bits
    test_case    64,  0
    test_case 17+16,  1
    test_case  9+16,  2
    test_case  7+16,  4
    test_case  5+16,  8
    test_case  3+16, 16
    test_case  2+16,  8
    test_case  1+16,  4
    test_case    64,  2

end:
    ASM_TEST_PASS
1:  j 1b
fail_end: ASM_TEST_FAIL
1:  j 1b

do_hartid_0:
    // Initialize the FLB register
    add a1,  x0, x5
    sd zero, 0(a1)
    fence
    add x3, x0,  x6
    // perform the flb operation
    csrrw x4, 0x820, x3
    bnez x4, end

    // when a thread is going to get a 1 on flb, it will ESR write to this fcc counter
    csrwi 0x821, 0

    // Send one credit to all threads 0 (but hartid 0)
    li	x2, 0xfffffffe
    li 	a1, 0x803400C0
    sd	x2, (a1)

    // Send one credit to all threads 1
    li	x2, 0xffffffff
    li 	a1, 0x803400D0
    sd	x2, (a1)

    ret
	
do_hartid_others:
.rept 128
    nop
.endr

    add x3, x0, x6
    // perform the flb operation
    csrrw x4, 0x820, x3
    beqz x4, set_fcc_counter_done

    // set fcc counter 0 of this shire
    li	x2, 1
    li 	a1, 0x803400C0
    sd	x2, (a1)

set_fcc_counter_done:
    csrr x2, mhartid
    andi x2, x2, 1
    add  x2, x2, x2

    // when a thread is going to get a 1 on flb, it will ESR write to this fcc counter
    csrw 0x821, x2

    ret
