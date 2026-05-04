#include "macros.h"
        
////////////////////////////////////////////////////////////////////////////////
// test.s contains:
//  * test code: The .test section is located (linker.ld) 4G before the end of the DRAM
//  * gsc ofsets: in the .gsc_offsets section
////////////////////////////////////////////////////////////////////////////////
#define O32K (32*1024)
#define TEST_START_NO_VMEM   (0x40000000 + O32K) // constant needed to do the JALR to start the test.. must be the same as TEST_START in linker.ld
#define TEST_START_VMEM      0x02A0000000 // when using vmem, mret to this address (can't use the same because it doesn't fit in Sv39)
.include "test.s"

ASM_TEST_PASS
1:   j 1b

////////////////////////////////////////////////////////////////////////////////
// data section used for loads, stores, gsc...
// It ends aligned to the end of the DRAM so that we can easily force access
// faults with PMA
////////////////////////////////////////////////////////////////////////////////
.section .scratch, "aw"
.global __scratch_area

// region all the loads/stores use
__scratch_area: 
.incbin "data.raw" 

////////////////////////////////////////////////////////////////////////////////
// entry point
// basic initialization and jumps to test start
////////////////////////////////////////////////////////////////////////////////
#define ESR_FEATURE     (0x1C0340000ULL | (0xFF << 22))
.text
.global _start   
_start: 
    ASM_TEST_START
    // setup trap vector
    la t0, trapHandler
    csrw mtvec, t0
    // prepare debug registers
    // 0x5B: action=0 (ebreak xcpt);  match=0 (full match of tdata2); match on m/s/u and load/store (but not exec)   
    li t0, 0x5b 
    csrw tdata1, t0
    csrwi tdata2, 0
    // set initial value for m0
    mov.m.x m0, x0, 0xFF
    // disable ML operations (to have illegal insn on WB)
    // note that in neigh build, trap_on_ml plusarg has to be 1 because there are no shire ESR
    li t0, ESR_FEATURE
    li t1, 2
    sd t1, 0(t0)
    ld t1, 0 (t0) // to make sure we don't proceed until the write has taken effect
    // allow hartid to be read in u-mode
    csrwi menable_shadows, 1

    // and start test, either in u-mode + vmem, or m-mode
    la t0, __vmem_conf
    ld t1, 0(t0)  //vmem mode
    bne t1, x0, 1f
    li t0, TEST_START_NO_VMEM            
    jr t0 // no vmem => jump to test start
1:
    //compose STAP (mode <<60) | (root_table_ppn)
    slli t1, t1, 60
    ld t2, 8(t0) // table base
    srli t2, t2, 12
    or t2, t2, t1
    csrw satp, t2
        
    // set mstatus.mpp to the appropiate mode, and mret to test start
    li t1, 0x1800
    csrc mstatus, t1 // clear mpp bits
    ld t1, 16(t0)
    slli t1, t1, 11
    csrs mstatus, t1 // set mpp[0] = s-mode?
    li t0, TEST_START_VMEM
    csrw mepc, t0
    mret
    ASM_TEST_FAIL
        
////////////////////////////////////////////////////////////////////////////////
// trap handler
// just just skips the instruction that caused the fault
////////////////////////////////////////////////////////////////////////////////

.align 12
trapHandler:
    csrw mscratch, t0
    // if ecall => trying to write tdata2 (data is in validation3)
    csrr t0, mcause
    addi t0, t0, -8 // check if cause is a value from 8 to B (sub 8 and and remove 2 lsb)
    srli t0, t0, 2
    bne t0, x0, 1f
    csrr t0, validation3
    csrw tdata2, t0
1:  
    // simply, mepc+=4 and mret
    csrr t0, mepc
    addi t0, t0, 4
    csrw mepc, t0
    csrr t0, mscratch
    mret

////////////////////////////////////////////////////////////////////////////////
// virtual memory configuration (disabled) => contents replaced by script
////////////////////////////////////////////////////////////////////////////////
        
.section vmem_conf, "a"
__vmem_conf:  
.dword 0 //  mode (0=disabled, 8=Sv38, 9=Sv39.. other values reserved)
.dword 0 //  root table base pointer
.dword 0 //  0 for u-mode, 1 for s-mode
        
