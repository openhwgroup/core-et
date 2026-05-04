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

    // loads
    li x2, 0xff
    call load_test

    li x2, 0x0
    call load_test

    li x2, 0xf0
    call load_test

    li x2, 0x3c
    call load_test

     // stores
    li x2, 0xff
    mova.m.x x2
    li t0, 4
    la a0, __data    
loop_store:
    addi a0, a0, 48
    fsw fs0, 0(a0)    
#    fsd fs0, 16(a0)
    fsw.ps fs0, 32(a0)
    add t0, t0, -1
    bne t0, zero, loop_store

end:
    ASM_TEST_PASS
end_loop:
    j end_loop


   //enters with x2=mask
load_test:
     mova.m.x x2
     // load
    la a0, __data
    add a0, a0, a1
    flw fs0, 0(a0)
#    flq fs0, 8(a0)    
    flw.ps fs0, 32(a0)
    addi a1, a1, 48
//    ret // commented out => falls through to  delay

    
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
