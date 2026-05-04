#include "macros.h"

#define ITERATIONS_EXCL_MODE 16
//#define ITERATIONS_THREAD0 30
//#define ITERATIONS_THREAD1 160
#define ITERATIONS_THREAD0 1
#define ITERATIONS_THREAD1 160
 

.text
.global _start
_start: 
    ASM_TEST_START

	//set mask to 1
	li t0, 0xFF
	mova.m.x t0

    //enable hartid shadow
    csrwi menable_shadows, 0xf

	// enable access to counters
	li t1, -1
	csrw mcounteren, t1
	csrw scounteren, t1
	////////////////////////////////////////////////////////////////////////////////
	// The threads enter exclusive mode while the pipe is full
	// The thread that doesn't win the 'mutex' will have his insn killed
	////////////////////////////////////////////////////////////////////////////////
	

	li t1, ITERATIONS_EXCL_MODE
	
loop1_start:	
	// both threads entering exclusive mode
	fadd.ps f10, f1, f2
	fadd.ps f11, f1, f2
	fadd.ps f12, f1, f2
	fadd.ps f13, f1, f2
	fadd.ps f14, f1, f2
	csrwi excl_mode, 1
	fadd.ps f10, f1, f2
	fadd.ps f11, f1, f2
	fadd.ps f12, f1, f2
	fadd.ps f13, f1, f2
	fadd.ps f14, f1, f2
	csrwi excl_mode, 0
	fadd.ps f10, f1, f2
	fadd.ps f11, f1, f2
	fadd.ps f12, f1, f2
	fadd.ps f13, f1, f2
	fadd.ps f14, f1, f2
	fadd.ps f15, f1, f2
	fadd.ps f16, f1, f2
	fadd.ps f17, f1, f2
	fadd.ps f18, f1, f2
	fadd.ps f19, f1, f2
	addi t1, t1, -1
	bne t1, x0, loop1_start

	////////////////////////////////////////////////////////////////////////////////
	// THIS WILL PRODUCE KILLS IN EX AND MEM at the same time (only using thread1)
	// forcing an xcpt in WB (from CSR) while the pipeline is full of FP insns
	////////////////////////////////////////////////////////////////////////////////
	
	// configure trap handler (ecall to end test)
	la t0, trap_handler
	
	csrw mtvec, t0
	
	// go to u-mode
	li t0, 0x1800
	csrrc x0, mstatus, t0  // clear mstatus.mpp
	la t0, 1f
	csrw mepc, t0          // set mepc to user-mode entry point
	mret                   // go to user mod
	1:                     // label to referr to user-mode


	csrwi validation3, 1
	
	// thread0 will jump to just doing random VPU/INT instructions
	// thread1 to a loop having xcpts in WB (to produce the kills)
	csrr t0, hartid
	andi t0, t0, 1
	beq t0, zero, thread0_loop

	// thread1 loop
thread1_loop:
	li t1, ITERATIONS_THREAD1

loop2_start:	
        // FORCE A XCPT IN WB => will assert kills for instructions in EX and TAG
	// fill the pipeline with instructions
	fadd.ps f10, f1, f2
	fadd.ps f11, f1, f2
	fadd.ps f12, f1, f2
	fadd.ps f13, f1, f2
	fadd.ps f14, f1, f2
	fadd.ps f0, f1, f2

	// repeat the test with different aligments
	
	// try to do a tensor load
	// thread1 cannot do tensorXXX operations => will produce an exception (which will kill instructions in the pipeline)


	csrw tensor_load, x0 // xcpt
	// fill the pipeline with instructions
	fadd.ps f10,f1,f2
	fadd.ps f11,f1,f2
	fadd.ps f12,f1,f2
	fadd.ps f13,f1,f2
	fadd.ps f14,f1,f2
	fadd.ps f15,f1,f2
	fadd.ps f16,f1,f2
	fadd.ps f17,f1,f2

	csrw tensor_load, x0 // xcpt
	// fill the pipeline with instructions
	fadd.ps f10,f1,f2
	fadd.ps f11,f1,f2
	fadd.ps f12,f1,f2
	fadd.ps f13,f1,f2
	fadd.ps f14,f1,f2
	fadd.ps f15,f1,f2
	fadd.ps f16,f1,f2
	fadd.ps f17,f1,f2
	fadd.ps f18,f1,f2

	csrw tensor_load, x0 // xcpt
	// fill the pipeline with instructions
	fadd.ps f10,f1,f2
	fadd.ps f11,f1,f2
	fadd.ps f12,f1,f2
	fadd.ps f13,f1,f2
	fadd.ps f14,f1,f2
	fadd.ps f15,f1,f2
	fadd.ps f16,f1,f2
	fadd.ps f17,f1,f2
	fadd.ps f18,f1,f2
	fadd.ps f19,f1,f2

	csrwi excl_mode, 1
	nop
	nop
	csrwi excl_mode, 0
	
	addi t1, t1, -1
	bne t1, x0, loop2_start
	// end test (ecall  => so that we go back to m mode and we can do 'wfi')
	ecall
1:
    j 1b


thread0_loop:
	li t1, ITERATIONS_THREAD0
loop3_start:	
	fadd.ps f0, f7, f6
	fadd.ps f7, f5, f22
	fadd.ps f6, f0, f6
	fadd.ps f5, f12, f2
	fadd.ps f2, f29, f8
	fadd.ps f3, f21, f28
	fadd.ps f5, f24, f7
	fadd.ps f5, f26, f14
	fadd.ps f7, f25, f21
	fadd.ps f6, f24, f20
	fadd.ps f6, f18, f15
	fadd.ps f7, f17, f13
	fadd.ps f5, f1, f26
	fadd.ps f0, f7, f6
	fadd.ps f2, f17, f18
	fadd.ps f7, f23, f22
	fadd.ps f7, f18, f26
	divw x10, t1, t1
	addi x0, x10, 0
	fadd.ps f3, f24, f7
	j 1f
1:
	fadd.ps f2, f17, f16
	fadd.ps f5, f9, f8
	fadd.ps f7, f9, f8
	fadd.ps f7, f6, f28
	fadd.ps f1, f16, f18
	fadd.ps f5, f12, f8
	fadd.ps f1, f22, f10
	fadd.ps f1, f28, f27
	fadd.ps f4, f27, f19
	fadd.ps f7, f3, f26
	fadd.ps f1, f24, f2
	fadd.ps f2, f26, f7
	fadd.ps f4, f6, f29
	fadd.ps f6, f17, f27
	fadd.ps f3, f22, f11
	fadd.ps f4, f2, f5
	fadd.ps f5, f16, f7
	fadd.ps f6, f5, f30
	fadd.ps f3, f8, f20
	fadd.ps f0, f0, f0
	fadd.ps f5, f14, f4
	j 1f
1:
	fadd.ps f4, f25, f23
	fadd.ps f3, f16, f21
	fadd.ps f4, f31, f7
	j 1f
1:
	fadd.ps f7, f10, f30
	fadd.ps f6, f21, f5
	fadd.ps f2, f8, f30
	fadd.ps f3, f29, f1
	fadd.ps f3, f22, f9
	fadd.ps f1, f2, f30
	fadd.ps f6, f18, f8
	fadd.ps f3, f29, f18
	fadd.ps f2, f22, f1
	fadd.ps f3, f24, f2
	fadd.ps f3, f31, f21
	fadd.ps f0, f22, f31
	fadd.ps f0, f5, f9
	j 1f
1:
	fadd.ps f3, f0, f14
	fadd.ps f3, f15, f10
	fadd.ps f3, f4, f1
	fadd.ps f2, f20, f1
	fadd.ps f6, f4, f31
	fadd.ps f3, f1, f12
	fadd.ps f5, f13, f22
	fadd.ps f0, f16, f7
	fadd.ps f6, f17, f29
	fadd.ps f3, f30, f17
	fadd.ps f7, f31, f22
	csrr x0,hpmcounter3
	csrr x0,hpmcounter4
	fadd.ps f4, f12, f27
	fadd.ps f7, f12, f27
	csrr x0,hpmcounter3
	csrr x0,hpmcounter4
	fadd.ps f1, f18, f27
	fadd.ps f4, f23, f20
	fadd.ps f2, f4, f11
	fadd.ps f6, f5, f19
	fadd.ps f5, f19, f29
	fadd.ps f0, f23, f11
	fadd.ps f7, f11, f22
	fadd.ps f2, f10, f8
	fadd.ps f1, f17, f24
	fadd.ps f1, f19, f4
	fadd.ps f1, f1, f2
	fadd.ps f0, f4, f21
	fadd.ps f3, f23, f26
	fadd.ps f0, f14, f24
	fadd.ps f2, f21, f28
	fadd.ps f1, f25, f18
	fadd.ps f5, f31, f9
	fadd.ps f4, f13, f1
	fadd.ps f6, f21, f10
	fadd.ps f4, f23, f9
	fadd.ps f2, f20, f8
	j 1f
1:
	fadd.ps f1, f22, f1
	fadd.ps f1, f11, f0
	fadd.ps f0, f29, f6
	fadd.ps f5, f22, f7
	fadd.ps f2, f25, f4
	fadd.ps f0, f26, f5
	fadd.ps f3, f29, f18
	fadd.ps f0, f23, f10
	fadd.ps f4, f26, f21
	j 1f
1:
	fadd.ps f4, f22, f13
	fadd.ps f4, f17, f1
	fadd.ps f1, f25, f2
	fadd.ps f1, f25, f26
	fadd.ps f1, f3, f15
	fadd.ps f2, f27, f26
	fadd.ps f0, f26, f4
	fadd.ps f1, f13, f22
	fadd.ps f3, f14, f6
	fadd.ps f2, f6, f14
	fadd.ps f4, f24, f8
	fadd.ps f2, f19, f24
	fadd.ps f6, f7, f22
	fadd.ps f1, f0, f20
	fadd.ps f3, f11, f7
	fadd.ps f1, f7, f7
	fadd.ps f2, f7, f1
	fadd.ps f4, f20, f0
	j 1f
1:
	fadd.ps f0, f10, f10
	fadd.ps f3, f30, f26
	fadd.ps f2, f3, f23
	fadd.ps f2, f13, f21
	fadd.ps f3, f22, f25
	fadd.ps f3, f24, f4
	fadd.ps f4, f4, f12
	fadd.ps f6, f14, f1
	fadd.ps f0, f25, f12
	fadd.ps f2, f14, f30
	fadd.ps f4, f15, f14
	fadd.ps f1, f19, f4
	fadd.ps f5, f8, f14
	fadd.ps f4, f10, f4
	fadd.ps f2, f17, f31
	fadd.ps f2, f5, f14
	fadd.ps f1, f19, f12
	fadd.ps f7, f31, f2
	fadd.ps f0, f24, f14
	fadd.ps f2, f11, f8
	fadd.ps f3, f1, f6
	fadd.ps f6, f13, f11
	fadd.ps f0, f26, f30
	j 1f
1:
	fadd.ps f3, f8, f28
	fadd.ps f0, f10, f20
	divw x10, t1, t1
	addi x0, x10, 0
	fadd.ps f6, f31, f25
	fadd.ps f5, f19, f1
	fadd.ps f6, f21, f26
	fadd.ps f7, f9, f29
	fadd.ps f2, f0, f22
	fadd.ps f0, f10, f10
	fadd.ps f3, f2, f18
	fadd.ps f6, f2, f14
	fadd.ps f5, f16, f12
	fadd.ps f4, f22, f3
	j 1f
1:
	fadd.ps f6, f30, f9
	fadd.ps f2, f8, f7
	fadd.ps f5, f25, f20
	j 1f
1:
	fadd.ps f1, f15, f18
	fadd.ps f7, f19, f20
	fadd.ps f1, f31, f23
	fadd.ps f6, f7, f2
	fadd.ps f6, f22, f10
	fadd.ps f4, f1, f25
	fadd.ps f6, f18, f28
	fadd.ps f3, f5, f10
	fadd.ps f3, f26, f0
	fadd.ps f6, f0, f6
	fadd.ps f7, f13, f30
	fadd.ps f1, f13, f1
	fadd.ps f0, f1, f12
	fadd.ps f1, f1, f14
	fadd.ps f1, f23, f25
	fadd.ps f2, f29, f20
	fadd.ps f2, f8, f24
	fadd.ps f4, f7, f6
	fadd.ps f3, f21, f9
	fadd.ps f1, f29, f26
	fadd.ps f6, f9, f19
	fadd.ps f7, f28, f18
	fadd.ps f6, f16, f6
	csrr x0,hpmcounter3
	csrr x0,hpmcounter4
	fadd.ps f2, f28, f11
	fadd.ps f1, f18, f26
	fadd.ps f5, f22, f10
	j 1f
1:
	fadd.ps f1, f13, f31
	fadd.ps f2, f6, f15
	fadd.ps f6, f30, f8
	fadd.ps f0, f7, f22
	fadd.ps f2, f5, f27
	fadd.ps f7, f0, f0
	fadd.ps f1, f1, f31
	fadd.ps f2, f20, f9
	fadd.ps f2, f2, f29
	j 1f
1:
	fadd.ps f7, f3, f21
	fadd.ps f0, f31, f21
	fadd.ps f2, f9, f19
	fadd.ps f3, f12, f30
	fadd.ps f3, f14, f28
	fadd.ps f2, f3, f1
	fadd.ps f2, f22, f6
	fadd.ps f6, f12, f17
	fadd.ps f1, f2, f21
	fadd.ps f5, f6, f13
	fadd.ps f0, f14, f12
	fadd.ps f0, f15, f2
	fadd.ps f3, f29, f22
	fadd.ps f4, f27, f13
	fadd.ps f3, f2, f26
	fadd.ps f6, f14, f5
	fadd.ps f3, f17, f29
	fadd.ps f7, f22, f16
	fadd.ps f7, f11, f31
	fadd.ps f1, f20, f28
	fadd.ps f4, f16, f3
	fadd.ps f5, f4, f6
	fadd.ps f7, f30, f5
	fadd.ps f3, f28, f21
	fadd.ps f1, f22, f15
	fadd.ps f5, f29, f14
	fadd.ps f5, f20, f24
	fadd.ps f3, f17, f25
	fadd.ps f3, f25, f26
	fadd.ps f7, f27, f5
	fadd.ps f5, f8, f15
	fadd.ps f0, f11, f1
	fadd.ps f0, f9, f1
	fadd.ps f2, f18, f7
	fadd.ps f6, f29, f31
	csrr x0,hpmcounter3
	csrr x0,hpmcounter4
	fadd.ps f0, f30, f30
	fadd.ps f0, f27, f4
	fadd.ps f3, f15, f16
	fadd.ps f5, f23, f21
	fadd.ps f0, f11, f26
	fadd.ps f3, f1, f1
	fadd.ps f0, f15, f19
	fadd.ps f2, f12, f20
	fadd.ps f3, f4, f18
	fadd.ps f4, f21, f11
	fadd.ps f3, f14, f21
	fadd.ps f6, f5, f30
	fadd.ps f6, f8, f13
	fadd.ps f0, f18, f6
	fadd.ps f7, f28, f26
	fadd.ps f3, f13, f1
	fadd.ps f2, f18, f8
	fadd.ps f1, f11, f14
	fadd.ps f1, f5, f26
	fadd.ps f5, f3, f12
	fadd.ps f4, f16, f12
	fadd.ps f5, f1, f31
	csrr x0,hpmcounter3
	csrr x0,hpmcounter4
	fadd.ps f0, f25, f27
	fadd.ps f3, f1, f11
	fadd.ps f2, f29, f15
	fadd.ps f3, f18, f23
	fadd.ps f4, f5, f29
	fadd.ps f7, f1, f16
	fadd.ps f2, f6, f6
	fadd.ps f2, f22, f18
	j 1f
1:
	fadd.ps f6, f7, f14
	j 1f
1:
	fadd.ps f7, f0, f1
	fadd.ps f6, f15, f28
	fadd.ps f4, f3, f9
	fadd.ps f0, f17, f23
	fadd.ps f3, f4, f6
	fadd.ps f2, f29, f7
	fadd.ps f0, f7, f30
	fadd.ps f5, f23, f22
	fadd.ps f2, f17, f17
	fadd.ps f7, f5, f2
	fadd.ps f5, f29, f23
	fadd.ps f1, f18, f7
	fadd.ps f2, f6, f2
	fadd.ps f4, f6, f11
	fadd.ps f2, f16, f27
	fadd.ps f5, f26, f4
	fadd.ps f7, f17, f30
	fadd.ps f0, f21, f14
	divw x10, t1, t1
	addi x0, x10, 0
	fadd.ps f3, f24, f19
	fadd.ps f1, f23, f11
	fadd.ps f6, f2, f10
	fadd.ps f6, f27, f19
	fadd.ps f3, f20, f23
	fadd.ps f0, f11, f18
	fadd.ps f2, f5, f18
	fadd.ps f0, f10, f14
	fadd.ps f1, f8, f23
	fadd.ps f6, f7, f16
	fadd.ps f6, f30, f22
	fadd.ps f7, f15, f21
	fadd.ps f7, f6, f4
	fadd.ps f7, f9, f14
	fadd.ps f4, f27, f14
	fadd.ps f4, f8, f26
	fadd.ps f7, f5, f5
	fadd.ps f2, f22, f4
	fadd.ps f6, f9, f26
	fadd.ps f7, f18, f2
	j 1f
1:
	fadd.ps f3, f27, f28
	fadd.ps f1, f21, f25
	fadd.ps f0, f21, f17
	fadd.ps f5, f0, f12
	fadd.ps f0, f29, f29
	fadd.ps f1, f14, f6
	fadd.ps f7, f19, f19
	fadd.ps f4, f11, f20
	fadd.ps f5, f2, f22
	fadd.ps f6, f29, f24
	fadd.ps f0, f14, f18
	fadd.ps f4, f24, f9
	fadd.ps f3, f21, f7
	fadd.ps f6, f30, f17
	fadd.ps f6, f27, f11
	fadd.ps f3, f16, f25
	fadd.ps f6, f11, f22
	fadd.ps f6, f27, f10
	fadd.ps f1, f4, f19
	fadd.ps f2, f4, f6
	fadd.ps f6, f13, f15
	fadd.ps f2, f8, f10
	fadd.ps f4, f9, f30
	fadd.ps f4, f20, f29
	j 1f
1:
	fadd.ps f6, f1, f0
	fadd.ps f5, f27, f9
	fadd.ps f3, f31, f2
	fadd.ps f4, f6, f2
	fadd.ps f3, f12, f14
	fadd.ps f6, f1, f23
	fadd.ps f2, f27, f18
	fadd.ps f7, f26, f22
	fadd.ps f0, f1, f6
	fadd.ps f6, f12, f3
	j 1f
1:
	fadd.ps f4, f10, f25
	fadd.ps f3, f3, f7
	fadd.ps f3, f20, f4
	fadd.ps f5, f29, f1
	fadd.ps f6, f27, f24
	fadd.ps f6, f4, f20
	fadd.ps f5, f10, f11
	fadd.ps f3, f11, f8
	fadd.ps f0, f17, f22
	fadd.ps f7, f7, f31
	fadd.ps f2, f20, f10
	j 1f
1:
	fadd.ps f6, f3, f4
	fadd.ps f3, f24, f9
	fadd.ps f0, f31, f26
	fadd.ps f4, f15, f22
	fadd.ps f6, f0, f22
	fadd.ps f3, f11, f17
	fadd.ps f6, f4, f18
	fadd.ps f4, f4, f1
	fadd.ps f6, f11, f10
	fadd.ps f6, f17, f22
	fadd.ps f4, f22, f24
	fadd.ps f0, f10, f17
	fadd.ps f3, f18, f8
	fadd.ps f1, f19, f12
	fadd.ps f3, f23, f1
	fadd.ps f3, f30, f21
	fadd.ps f4, f23, f14
	fadd.ps f3, f12, f8
	fadd.ps f0, f7, f10
	fadd.ps f6, f17, f2
	fadd.ps f2, f16, f19
	fadd.ps f6, f23, f2
	fadd.ps f2, f3, f7
	j 1f
1:
	fadd.ps f0, f3, f4
	fadd.ps f6, f12, f3
	fadd.ps f3, f13, f29
	fadd.ps f2, f25, f18
	fadd.ps f0, f3, f11
	fadd.ps f7, f12, f11
	fadd.ps f1, f22, f26
	fadd.ps f5, f12, f26
	fadd.ps f7, f20, f12
	fadd.ps f6, f19, f11
	fadd.ps f7, f31, f21
	fadd.ps f2, f4, f3
	
	bne t1, x0, loop3_start
	// end test (ecall  => so that we go back to m mode and we can do 'wfi')
	ecall
1:
    j 1b



	
.align 12
trap_handler:
	// if illegal instruction... epc+=4, and mret
	// else (ecall) end test
	csrr t0, mcause
	xori t0, t0, 2
	bne t0, x0, test_end
	csrr t0, mepc
	addi t0, t0, 4
	csrw mepc,t0
	mret
test_end:	
	    ASM_TEST_PASS
1:
	j 1b
