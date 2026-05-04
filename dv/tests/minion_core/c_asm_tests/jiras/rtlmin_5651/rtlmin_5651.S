#include "macros.h"
   
#define OFF 0
#define INITIAL 2        
#define CLEAN 2
#define DIRTY 3

#define fs_mask x10
#define xcpt_expected x11

#define TENSOR_FMA_CONF  ( \
 ( 0 << 63)  | /* use tensor_mask CSR to skip operations in a A row granularity */ \
 ( 3 << 55)  | /* B matrix number of columns divided by 4 (minus 1) */ \
 ( 3<<51)    | /* A matrix number of rows (minus 1) */ \
 ( 3<<47)    | /* A matrix number of columns (minus 1) */ \
 ( 0<<43)    | /*A matrix starting column for the operation */ \
 ( 0 << 20)    /*Location of matrix B (0=L1 scratchpad, 1=memory) */ \
)



.macro write_fs val
    li t0, 3<<13        
    csrc mstatus, t0
    li t0, \val << 13
    csrs mstatus, t0
.endm


.macro start_iter val
    bne xcpt_expected, x0, xcpt_err
    csrwi validation3, \val
    write_fs INITIAL
.endm

.text
.global _start

_start: 
    ASM_TEST_START
        
    la t0, trap_vector
    csrw mtvec, t0   
    li t0, 0xFF
    mova.m.x t0
    li fs_mask, 3<<13
    li xcpt_expected, 0
        
// set FS=initial/clean
// do FP instruction, followed by mstatus.FS=OFF (alignment to ensure they are in the same cache line)
// read mstatus (should be OFF)
    start_iter 0
    li xcpt_expected, 0
    .align 5
    fadd.s f0, f1,f2
    csrrc t0, mstatus, fs_mask
    csrr t0, mstatus

// set FS=initial/clean
// do FP instruction, followed by (mstatus.FS=OFF + t0=previous mstatus.FS) t0 should be dirty
// execute FP instruction (should trap because FS=OFF)
    start_iter 1
    li xcpt_expected, 1
    .align 5
    fadd.s f0, f1,f2
    csrrc t0, mstatus, fs_mask
    fadd.s f3, f4, f5
    csrr t0, mstatus
        
// set FS=initial/clean
// do trans instruction, followed by (mstatus.FS=OFF + t0=previous mstatus.FS) t0 should be dirty
// execute FP instruction (should trap because FS=OFF)
    start_iter 2
    li xcpt_expected, 1
    .align 5
    frcp.ps f0, f1
    csrrc t0, mstatus, fs_mask
    fadd.s f3, f4, f5
    csrr t0, mstatus

// same as before, but with nops between trans and csr instruction
// do trans instruction, followed by (mstatus.FS=OFF + t0=previous mstatus.FS) t0 should be dirty
// execute FP instruction (should trap because FS=OFF)
    start_iter 3
    li xcpt_expected, 1
    .align 5
    frcp.ps f0, f1
    nop
    nop
    csrrc t0, mstatus, fs_mask
    fadd.s f3, f4, f5
    csrr t0, mstatus
        
// set FS=initial/clean
// do FLW (because it is the first access, it will miss)        
// set mstatus.FS=OFF
// do FP instruction using the loaded value => it should trap
    start_iter 4
    li xcpt_expected, 1
    la t0, data_ptr
    .align 5
    flw.ps f0, 0(t0)
    csrrc t0, mstatus, fs_mask
    fadd.s f0,f0,f0
    csrr t0, mstatus
        
// same as before, but without cache misss
    start_iter 5
    li xcpt_expected, 1
    la t0, data_ptr
    .align 5
    flw.ps f0, 0(t0)
    csrrc t0, mstatus, fs_mask
    fadd.s f0,f0,f0
    csrr t0, mstatus

// now with a tensorfma touching FP regs
    start_iter 6
    li xcpt_expected, 1


    csrwi 0x7e0, 0x1 // First enable split
    fence            // VERIF-3295 (PRM-8)
    csrwi 0x7e0, 0x3 // Then enable scratchpad
    fence
    li t0, TENSOR_FMA_CONF
    .align 5
    csrw tensor_fma, t0
    csrrc t0, mstatus, fs_mask
    csrwi tensor_wait,7
    fadd.s f0,f0,f0        
    csrr t0, mstatus

// some NOP guard
.rept
    nop
.endr

// and finish the test        
    ASM_TEST_PASS

end_loop:
    j end_loop

    
xcpt_err:
        ASM_TEST_FAIL
        j end_loop

// trap vector => just return to mepc + 4 
.align 12
trap_vector:
        beq xcpt_expected, x0, xcpt_err
        li xcpt_expected, 0
        csrr t0, mepc
        addi t0, t0, 4
        csrw mepc, t0
        mret


// data ptr... using alignment to make it stand in a completely different cache line
.align 12
data_ptr:   
.rept 64
        .word 0xBABAFEFE
.endr


