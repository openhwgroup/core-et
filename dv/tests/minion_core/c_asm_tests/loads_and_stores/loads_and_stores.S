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

    la a1, __data
    ld a4, 0(a1)
    
    lw a4, 8(a1)    
    ld a4, 16(a1)
    ld a4, 8(a1)    
    
    ld a4, 64(a1)
    ld a4, 128(a1)
    ld a4, 192(a1)
    ld a4, 256(a1)
    ld a4, 320(a1)
    ld a4, 384(a1)
    ld a4, 448(a1)

    sw a4, 8(a1)    
    sd a4, 16(a1)
    sd a4, 8(a1)
    sw a4, 4(a1)
    sw a4, 0(a1)    
    
    
//    li a1, TICKETER
//    ld zero, 0(a1)
//    ld zero, 0(a1)
    
    call delay
    // load hits
    li a2, (3<<UNSET) // 4 different sets
    li a1,  (1<<UNSET) //set
    call loads

    // load misses
    li a2, (3<<UNSET) | (7<<UNTAG) // 4 different sets, 8 different tags
    li a1, (1<<UNSET) | (1<<UNTAG)
    call loads

    // store hits
    li a2, (3<<UNSET)  // 4 different sets
    li a1,  (1<<UNSET)
    call stores

    // stores misses
    li a2, (3<<UNSET) | (7<<UNTAG) // 4 different sets, 8 different tags
    li a1, (1<<UNSET) | (1<<UNTAG)
    call loads
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
    
////////////////////////////////////////////////////////////////////////////////
// loads __data[i + ((a1*i)&a2)] for i=0..ITERATIONS-1
loads:
    li t1, 0
    li t0, ITERATIONS

loop_loads:
    la t2, __data
    add t2, t2, t1
    ld zero, 0(t2)
    add t0, t0, -1
    add t1, t1, a1              //go to next tag
    and t1,t1, a2               //and with tag mask..
    bne t0, zero, loop_loads
    j delay

////////////////////////////////////////////////////////////////////////////////
// loads __data[i + (1<<UNTAG)*(i%nrTags)] for i=0..ITERATIONS-1
// nrTags is read from a2
stores: 
    li t1, 0
    li t0, ITERATIONS
    li a1,  1<<UNTAG // next tag

loop_stores:
    la t2, __data
    add t2, t2, t1
    sw zero, 0(t2)
    add t0, t0, -1
    add t1, t1, a1              //go to next tag
    and t1,t1, a2               //and with tag mask..
    bne t0, zero, loop_stores
    j delay


.data
.global __data
.align 8
__data: 
.incbin "data.raw"            // generated with dd if=/dev/urandom of=data.raw bs=1M count=1
