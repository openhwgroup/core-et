#include "macros.h"

#define NR_ITERATIONS 20

#define CHECK_MTVAL
    
#define CAUSE_ILLEGAL_INSTRUCTION 2
#define CAUSE_MACHINE_ECALL  0xb
#define CAUSE_SUPER_ECALL 0x9

#define CAUSE_MCODE_INSTRUCTION 0x1e

#define ERROR_COUNT s11
#define EXP_CAUSE s10
#define EXP_EPC s9
#define XCPT_COUNT s8
#define EXP_XCPT_COUNT s7
#define ITER s6
#define THREAD_ID s5

.macro setExpected x
    addi EXP_XCPT_COUNT, EXP_XCPT_COUNT, 1
    la EXP_EPC, 1f 
    li EXP_CAUSE, \x
    1:
.endm

.macro getMode x y
    la \x, mode
    csrr \y, hartid
    slli \y, \y, 2
    add \x, \x, \y
    lw \x, 0(\x)
.endm

.macro setMode x y z
    la \x, mode
    csrr \y, hartid
    slli \y, \y, 2
    add \x, \x, \y
    sw \z, 0(\x)
.endm
    
.text
.global _start
_start: 

    ASM_TEST_START

    // initialization
    li ERROR_COUNT, 0
    li XCPT_COUNT, 0
    li EXP_XCPT_COUNT, 0
    li t0, 0xDEADBEEF

    csrw mtval, t0
    csrw stval, t0

    // Enable hartid shadows
    csrwi menable_shadows, 1

    //configure mtvec and stvec
    la t0, mtrap_vector
    csrw mtvec, t0
    la t0, strap_vector
    csrw stvec, t0

    // when in M-mode: find hart_id, then thread_id: 
    csrr s5, mhartid  
    andi s5, s5, 1    // mhartid[0] = thread_id (s5 = THREAD_ID)


m_s_start:      
    li ITER, NR_ITERATIONS    
loop_start:     
    // illegal instruction
    setExpected(CAUSE_ILLEGAL_INSTRUCTION)
     .word 0

// FUTURE: Emmanuel Marie remove following ifdef when passing
#if defined (MCODE_INSTRUCTION_FAIL)
    // instructions emulated in mcode
    setExpected(CAUSE_MCODE_INSTRUCTION)
    fence.i
#endif

    // floating point instructions.. should not raise exception
    fcvt.s.w f0, x0
    fcvt.s.w f1, x1

    
    // set mstatus.fs=off, just when in m-mode
    getMode t0, t1
    bne t0, x0, 2f
    li t0, (0x3 << 13)
    csrrc x0, mstatus, t0
    setExpected(CAUSE_ILLEGAL_INSTRUCTION)
    fcvt.s.w f1, x1
    setExpected(CAUSE_ILLEGAL_INSTRUCTION)
    fcvt.s.w f0, x0

    // restore mstatus.fs.. fp should not raise exceptions any more
    li t0, (0x3 << 13)
    csrrs x0, mstatus, t0
    fcvt.s.w f1, x1
2:
    
    // read invalid csr.. should raise exception
    setExpected( CAUSE_ILLEGAL_INSTRUCTION)            
    csrr x1, 0xfff

    getMode t0, t1
    bne t0, x0, 2f
    setExpected(CAUSE_MACHINE_ECALL)
    ecall
    j end
2:      
    setExpected(CAUSE_SUPER_ECALL)
    ecall
            
end:
    beq XCPT_COUNT, EXP_XCPT_COUNT, 1f
    addi ERROR_COUNT, ERROR_COUNT, 1
1:
    addi ITER, ITER, -1
    bne ITER, x0, loop_start

    // after NR_ITERATIONS, jump to s mode
    getMode t1, t0
    bne t1, x0, 1f
    li t1, 1
    setMode t0, t2, t1
    #sw t1, 0(t0)
    la t0, m_s_start
    li t1, 1<<11    
    csrs mstatus, t1
    li t1, 1<<12
    csrc mstatus, t1
    csrw mepc, t0

    mret
1:
    beq ERROR_COUNT, zero, test_pass

test_fail:
    ASM_TEST_FAIL
    j 1f

test_pass:
    ASM_TEST_PASS

1:
    j 1b


.align 12
mtrap_vector:
    addi XCPT_COUNT, XCPT_COUNT,1    
    // read some csrs so that emu can check its contents
    csrr s1, mtval
    csrr s2, mcause
    csrr s3, mepc
    jal ra, checks
    addi s3, s3, 4
    csrw mepc, s3
    mret

.align 12
strap_vector:
    addi XCPT_COUNT, XCPT_COUNT,1
    // read some csrs so that emu can check its contents
    csrr s1, stval
    csrr s2, scause
    csrr s3, sepc
    jal ra, checks
    addi s3, s3, 4
    csrw sepc, s3
    mret


checks:
    //enters with s1=tval, s2=cause, s3=epc
    
    li a4, 0

    // check cause
    xor EXP_CAUSE, s2, EXP_CAUSE
    sltiu a4, EXP_CAUSE, 1
    xori a4, a4, 1
    add ERROR_COUNT, ERROR_COUNT, a4

    // check epc
    li a4, 0
    xor EXP_EPC, s3, EXP_EPC
    sltiu a4, EXP_EPC, 1
    xori a4, a4, 1
    add ERROR_COUNT, ERROR_COUNT, a4
    
    // check mtval
    la t0, cause_table
    mv t1, s2
    slli t1, t1, 3
    add t0, t0,t1 //t0=cause_table + cause*8
    ld t0, 0(t0)
    jalr x0, t0, 0


unimp_trap:
    addi ERROR_COUNT, ERROR_COUNT, 1
    ret

illegal_insn:
#ifdef CHECK_MTVAL
    // just check tval (in s1) is correct.. s3=epc
    lw t0, 0(s3)
    subw t0, s1, t0
    beq t0, x0, 1f
    addi ERROR_COUNT, ERROR_COUNT, 1
#endif
1:
    ret
s_ecall:
    // expect mode to be 1
    getMode t1, t0
    li t0, 0
    sltiu t0, t1, 1
    j 1f
        
m_ecall:
    getMode t1, t0
    li t0, 0
    sltiu t0, t1, 1
    xori t0, t0, 1
1:     
    add ERROR_COUNT, ERROR_COUNT, t0        
    
    // tval (in s1) has to be zero
    beq s1, x0, 1f
    addi ERROR_COUNT, ERROR_COUNT, 1
1:      
    ret

mcode_insn:
    ret
    
.data

cause_table:
    .quad unimp_trap
    .quad unimp_trap
    .quad illegal_insn
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad s_ecall
    .quad unimp_trap
    .quad m_ecall
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad unimp_trap
    .quad mcode_insn

// mode:     0=M, 1=S, for Thread 0; 
// mode_th1: 0=M, 1=S, for Thread 1;
mode:
    .rept 2048
    .word 0
    .endr
