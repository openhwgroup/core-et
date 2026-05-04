///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread0 does multiple ESR write to all the minions for all the 8 FCCs (2 Shires) to increment the hart counters.
// All the other threads will perform csr writes to their FCC registers to decrement the counters
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "macros.h"

#ifndef TARGET_HART
#define TARGET_HART 0
#endif
	
.macro setThreadsPC reg0
	csrr 	\reg0, mhartid
	beqz	\reg0, HART_MAIN_PC
	j 	HART_OTHER
	wfi
.endm

.macro perform_csr_write
	.rept 9
	csrwi 	0x821, 0
	csrwi 	0x821, 1
	.endr
	j	end
.endm
	
	
.text
.global _start
_start:	
	ASM_TEST_START

start_of_test:	
	setThreadsPC x3
	.rept 16
	    nop
	.endr


HART_MAIN_PC:
	// increment all the counters
	li	x4, 0xffffffffffffffff
	li 	a1, 0x803400C0
	li 	a2, 0x803400C0
	li	x5, 9
0:	
	.set i, 0
	.rept 4
	  sd	x4, (i<<3)(a1)
	  sd	x4, (i<<3)(a2)
	  .set i, i+1
	.endr
	addi	x5, x5, -1
	bnez	x5, 0b
	perform_csr_write
	
	
end:
    ASM_TEST_PASS
end_loop:
    j end_loop
	
HART_OTHER:
	perform_csr_write
	.rept 16
	    nop
	.endr
.data
.global __data
.align 8
__data: 
