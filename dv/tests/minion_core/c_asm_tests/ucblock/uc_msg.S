#include "macros.h"

.text
.global _start
_start:	
	ASM_TEST_START

        li 	a1, 0x80800800
	li 	x3, 0x003a003a003a003a
	sd 	x3, 0(a1)
	fence
end:
    ASM_TEST_PASS
end_loop:
    j end_loop

	.rept 64
	nop
	.endr	
	
.data
.global __data
.align 8
__data: 
