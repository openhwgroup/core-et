#include "macros.h"

// TEST MACRO    
.macro fgw_test progress, ptr, xs1, xs2, fs, fd1, fd2, prefetch
    .if \ptr % 64 > 0
    error "ptr has to be CL aligned"
    .endif
    // SETUP TEST STEP
    csrwi gsc_progress,\progress // set progress
    la \xs1, __data  + \ptr      // set data pointer for first gather (first CL half)
    la \xs2, __data  + \ptr +32  // set data pointer for 2nd gather (second CL half)
    fxor.pi \fs, \fs, \fs        // set offsets reg to 0
    .if \prefetch == 1
    ld x0, 0(\xs1)
    .endif

    // GATHERS UNDER TEST (aligned at the start of a cache line)
    .p2align 6
    fgw.ps \fd1, \fs(\xs1)
    fgw.ps \fd2, \fs(\xs2)

    // WAIT FOR END (some nops and a dummy fadd)
    .rept 4
    nop
    .endr
    fadd.ps \fd1, \fd1, \fd2 //won't execute until both gathers finish
.endm    

// TEST START    
.text
.global _start
_start: 
    ASM_TEST_START
mov.m.x m0, x0, 0xff

    
.irp progress 0,1,2,3,4,5,6,7
    // first: testing dcache missing
    fgw_test \progress, (\progress*64*2), x1, x2, f0, f1, f2, 0
    // second: testing dcache hitting
    fgw_test \progress, (\progress*64*2 + 64), x1, x2, f0, f1, f2, 1
.endr
    
    ASM_TEST_PASS


    // SCRATCH DATA
.data
.global __data
.p2align 6
__data: 
.incbin "data.raw"            // generated with dd if=/dev/urandom of=data.raw bs=1M count=1
