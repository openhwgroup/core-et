//////////////////////////////////////////////////////////////////////////////////////////
// All Threads but one performs an ESR as many times as their hartid
// The one thread (TARGET_HART) performs as many csrs as the ESRs from other threads
//////////////////////////////////////////////////////////////////////////////////////////


#include "macros.h"

#ifndef TARGET_HART
#define TARGET_HART 0
#endif
	
.macro setThreadsPC reg0, reg1
	li 	\reg1, TARGET_HART
	csrr 	\reg0, mhartid
	beq	\reg0, \reg1, HART_MAIN_PC
	j 	HART_OTHER
	wfi
.endm
	
.text
.global _start
_start:	
	ASM_TEST_START

start_of_test:	
	setThreadsPC x3, x4
	.rept 16
	    nop
	.endr


HART_MAIN_PC:
	// loop ((63 + 62 + .. + 1) - hartid) times
	li  x5, 2016
	sub	x5, x5, x3
0:
	csrwi 	0x821, 0
	addi	x5, x5, -1
	bnez	x5, 0b
	
end:
    ASM_TEST_PASS
end_loop:
    j end_loop
	
HART_OTHER:
	beqz	x3, end
	// set fcc counter 0 of this shire
	// set the target HART bit
	li	x4, (1<<(TARGET_HART/2))
	// Set the target HART FCC ESR register
	li 	a1, (0x803400C0 + (((TARGET_HART & 0x1) * 2) << 3))
0:
	sd	x4, (a1)
	addi	x3, x3, -1
	bnez	x3, 0b
	j	end
	.rept 16
	nop
	.endr
	
.data
.global __data
.align 8
__data: 
