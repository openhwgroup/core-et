#include "macros.h"

#define ITERATIONS 256
#define UNTAG 12
#define UNSET 6
#define WAYS 4
#define TICKETER 0xFFF00000
    
.text
.global _start
_start: 
    ASM_TEST_START

    // gathers
	li a0, 512
    call gather32_test

    // scatters
    li a0, 512

    la t0, __data

loop_scatter32:
	lh t1, 0(t0)
	mova.x.m t1
	addi t0, t0, 2
	lw t1, 0(t0)
	addi t0, t0, 4
    fsc32b.ps fs0, t1(t0)
    fsc32h.ps fs0, t1(t0)
    fsc32w.ps fs0, t1(t0)
    addi t0, t0, 48
    addi a0, a0, -1
    bne a0, zero, loop_scatter32

end:
    ASM_TEST_PASS
end_loop:
    j end_loop


   //enters with x2=mask
gather32_test:
    la t0, __data

loop_gather32:
	lh t1, 0(t0)
	addi t0, t0, 2
	mova.m.x t1
	lw t1, 0(t0)
	addi t0, t0, 4
    fg32b.ps fs1, t1(t0)
    fg32h.ps fs2, t1(t0)
    fg32w.ps fs3, t1(t0)
    addi t0, t0, 48
	addi a0, a0, -1
	bne a0, zero, loop_gather32
    
delay:
    li t0, 512
loop_delay:
    add t0, t0, -1
    bne t0, zero, loop_delay
    ret
    


.data
.global __data

.align 8
__data: 
.incbin "data.raw"            // generated with dd if=/dev/urandom of=data.raw bs=1M count=1
