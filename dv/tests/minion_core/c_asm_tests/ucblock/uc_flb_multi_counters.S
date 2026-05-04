#include "macros.h"

#define ESR_FLB_REG_00 0x80340100ULL
	//[12:5] = 0x8 [4:0] = 0
#define FLB_DATA_0 0x000000E0ULL
#define FLB_DATA_1 0x000000E1ULL
#define FLB_DATA_2 0x000000E2ULL
#define FLB_DATA_3 0x000000E3ULL
#define FLB_DATA_4 0x000000E4ULL
#define FLB_DATA_5 0x000000E5ULL
#define FLB_DATA_6 0x000000E6ULL
#define FLB_DATA_7 0x000000E7ULL	


.macro setThreadsPC reg0, reg1
	li \reg1, 8
	csrr \reg0, mhartid
	beqz \reg0, HART_00_PC
	blt  \reg0, \reg1, HART_01_SET
	add  \reg0, \reg0, -8
	blt  \reg0, \reg1, HART_02_SET
	add  \reg0, \reg0, -8
	blt  \reg0, \reg1, HART_03_SET
	add  \reg0, \reg0, -8
	blt  \reg0, \reg1, HART_04_SET
	add  \reg0, \reg0, -8
	blt  \reg0, \reg1, HART_05_SET
	add  \reg0, \reg0, -8
	blt  \reg0, \reg1, HART_06_SET
	add  \reg0, \reg0, -8
	blt  \reg0, \reg1, HART_07_SET
	add  \reg0, \reg0, -8
	blt  \reg0, \reg1, HART_08_SET
	wfi
.endm

.macro perform_barrier reg0, reg1, data
    // wait for H0 to tell registers are initialized to 0
    csrwi fcc, 0

    
	li    \reg0, \data
	// perform the flb operation
	csrrw \reg1, 0x820, \reg0
	bnez  \reg1, set_fcc_counter
	j	end
.endm

.macro perform_barrier_0 reg0, reg1, data
	li    \reg0, \data
	// perform the flb operation
	csrrw \reg1, 0x820, \reg0
	bnez  \reg1, wait_for_other_harts
.endm


	
	
.text
.global _start
_start:	
	ASM_TEST_START

	setThreadsPC x3, x4
	.rept 16
	    nop
	.endr


HART_00_PC:
	// Initialize the FLB registers
        li a1,  ESR_FLB_REG_00
	.set i, 0
	.rept 8
	  sd zero, (i<<3)(a1)
	  .set i, i+1
	.endr
    
    //notify other harts they can start
	li	x2, 0xFFFFFFFE
	li 	a1, 0x803400C0
	sd	x2, (a1) // even harts (except H0)
    li	x2, -1
    sd	x2, 16(a1) // odd harts

	fence
        
	perform_barrier_0 x3, x4, FLB_DATA_0
	csrwi 0x821, 0
wait_for_other_harts:	
	// wait for 8 fcc writes
	.rept 7
	csrwi 0x821, 0
	.endr
	
end:
    ASM_TEST_PASS
end_loop:
    j end_loop
	
HART_01_SET:
	perform_barrier x3, x4, FLB_DATA_0
	
HART_02_SET:
	perform_barrier x3, x4, FLB_DATA_1
	
HART_03_SET:
	perform_barrier x3, x4, FLB_DATA_2
	
HART_04_SET:
	perform_barrier x3, x4, FLB_DATA_3
	
HART_05_SET:
	perform_barrier x3, x4, FLB_DATA_4
	
HART_06_SET:
	perform_barrier x3, x4, FLB_DATA_5
	
HART_07_SET:
	perform_barrier x3, x4, FLB_DATA_6
	
HART_08_SET:
	perform_barrier x3, x4, FLB_DATA_7

set_fcc_counter:
	// set fcc counter 0 of this shire
	li	x2, 1
	li 	a1, 0x803400C0
	sd	x2, (a1)
	j 	end

set_fcc_counter_and_wait:
	// set fcc counter 0 of this shire
	li	x2, 1
	li 	a1, 0x803400C0
	sd	x2, (a1)
	j 	end


    	
.data
.global __data
.align 8
__data: 
