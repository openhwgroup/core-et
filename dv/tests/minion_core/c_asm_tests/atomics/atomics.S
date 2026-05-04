#include "macros.h"

.text
.global _start
_start: 
    ASM_TEST_START

    //enable lanes
    li x2, 0xff
    mova.m.x x2

    // Initialize memory region to use it for atomics
    // First 256 bits with some values
    // Second 256 bits with zeros
    la a0, __data

    li t0, 8
    loop_init1:
    sw t0, 0(a0)
    add a0, a0, 4
    add t0, t0, -1
    bne t0, zero, loop_init1
    nop
   
    li t0, 4
    loop_init0:
    sd x0, 0(a0)
    add a0, a0, 8
    add t0, t0, -1
    bne t0, zero, loop_init0
    nop

    la a0, __data
    
    // Init floating-point registers
    // f0 will contain some values for AMO operations
    // f1 will contain zeroes to operate always on same address
    flw.ps f0, 0(a0)
    flw.ps f1, 32(a0)
    nop

    // Evict initialized line to update main memory
    mv t0, a0
    li t1,0xffffffffffffffc0  // Mask bits [5:0]
    and t0,t0,t1
    li t1,0x0400000000000000  // Set bits [59:58] to 01=L2 and bits [62:60] to 3'b111=Evict_VA
    or t0,t0,t1
    csrw 0x89f,t0
    call delay

    ///////////////////////////////////////////////
    // repeat the tests 10 times
    //  (will do the same, but with different data)
    ///////////////////////////////////////////////
    li t5,10
main_loop:  
        
    // Scalar Atomics
    call amo_local_scalar_test
    call delay
    
    // Vectorial Atomics
    call amo_local_vector_test
    call delay

    addi t5, t5, -1
    bne t4, zero, main_loop

        
end:
    ASM_TEST_PASS
end_loop:
    j end_loop


delay:
    li t0, 64
loop_delay:
    add t0, t0, -1
    bne t0, zero, loop_delay
    ret
    
amo_local_scalar_test:

    mv t1, a0
    addi t1, t1, 32
    li t2, 0x4400
    amoaddl.w t3, t2, (t1)
    nop
    //amoaddl.w t3, t2, (t1)
    //nop
    
    // When calling another function, save and then restore return address
    //mv t4, ra
    //call delay
    //mv ra, t4
    
    addi t1, t1, 8
    li t2, 0x18008100
    amoaddl.d t3, t2, (t1)
    nop
    //amoaddl.d t3, t2, (t1)
    //nop

    ret

amo_local_vector_test:

    mv t1, a0
    addi t1, t1, 32
    famoaddl.pi f0,f1(t1)
    nop

    ret

.data
.global __data
.align 8
__data: 
.incbin "data.raw"            // generated with dd if=/dev/urandom of=data.raw bs=1M count=1
