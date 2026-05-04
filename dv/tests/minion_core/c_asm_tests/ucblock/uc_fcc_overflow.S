///////////////////////////////////////////////////////////////////////////
// Threads (0-3) performs 0xffff +1 writes.
// the Tensor Error Register [3] should be set
///////////////////////////////////////////////////////////////////////////


#include "macros.h"

#ifndef TARGET_HART
#define TARGET_HART 0
#endif
	
.text
.global _start
_start:	
	ASM_TEST_START

start_of_test:
	li 	a1, 0x803400C0
	csrr	x3, mhartid
	// x3 has hartid; shift left by 3 to form base address for FCC
	slli	x4, x3, 3
	add	a1, a1, x4
	// set the store data (always target bit 1 (hart0) of the FCC ESR)
	li	x4, 1
	li	x2, 0x10000
0:	
	.rept	16
		sd	x4, (a1)
	.endr
	addi	x2, x2, -16
	bnez	x2, 0b
	// HART2& 3 just jump to pass
	addi	x3, x3, -2
	bgez	x3, end
	// read tensor error reg to test bit[3] for overflow error
	csrr	x2, 0x808
	li	x4, 0x8
	and	x2, x2, x4
	beqz	x2, fail_end

end:
   	ASM_TEST_PASS
end_loop:
	j end_loop
fail_end:
	ASM_TEST_FAIL
	j	end_loop
	
	
.data
.global __data
.align 8
__data: 
