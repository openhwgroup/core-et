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
    li a1,0

    // setup mask and data pointer
    li x2, 0x0f
    mova.m.x x2
    la a0, __data

    // load operands
    flw f0, 0(a0)
    flq2 f1, 8(a0)
    flw.ps f0, 16(a0)
    flw.ps f1, 16(a0)        
    flw.ps f2, 16(a0)

    fmadd.s f4, f0, f1, f2
    fmadd.s f4, f0, f1, f2
    fmadd.s f4, f0, f1, f2

    nop
    fmadd.s f4, f0, f1, f2
    nop
    fmadd.s f4, f0, f1, f2    
    nop
    fmadd.s f4, f0, f1, f2
    nop
end:
    ASM_TEST_PASS
end_loop:
    j end_loop
    
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
