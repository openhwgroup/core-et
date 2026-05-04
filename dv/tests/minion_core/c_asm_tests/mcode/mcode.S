#include "macros.h"
#define CSR_MATCH = 0x2d8

.text
.global _start
.align 8
_start:

  .equ DEF_MINSTMASK,  0x0
  .equ DEF_MINSTMATCH, 0x0
  .equ FADD_PS_MINSTMASK,  0x106000fff
  .equ FADD_PS_MINSTMATCH, 0x2db

      # init marker
    li t5, 0xc001

      # machine trap-handler base address
    la t0, trap_vector
    csrw mtvec,t0

      # m-code instructions
    li t3, 0x0
    fcvt.s.l  ft0, t5           #fcvt.s.l
    fmv.x.s t6, f0
    bne	t6,t5,fail

    addi t3, t3,8
    fcvt.s.lu ft1, t5           #fcvt.s.lu
    fmv.x.s t6, f1
    bne	t6,t5,fail

    addi t3, t3,8
    fcvt.l.s  t5,ft2            #fcvt.l.s
    fmv.x.s t6, f2
    bne	t6,t5,fail

    addi t3, t3,8
    fcvt.lu.s t5,ft3            #fcvt.lu.s
    fmv.x.s t6, f3
    bne	t6,t5,fail

    addi t3, t3,8
    fdiv.pi   ft4,ft0,ft1       #fdiv.pi
    fmv.x.s t6, f4
    bne	t6,t5,fail

    addi t3, t3,8
    fdivu.pi  ft5,ft0,ft1       #fdivu.pi
    fmv.x.s t6, f5
    bne	t6,t5,fail

    addi t3, t3,8
    fremu.pi  ft6,ft0,ft1       #fremu.pi
    fmv.x.s t6, f6
    bne	t6,t5,fail

    addi t3, t3,8
    frem.pi   ft7,ft0,ft1       #frem.pi
    fmv.x.s t6, f7
    bne	t6,t5,fail

    addi t3, t3,8
    fdiv.s    ft8,ft0,ft1       #fdiv.s
    fmv.x.s t6, f8
    bne	t6,t5,fail

    addi t3, t3,8
    fdiv.ps   ft9,ft0,ft1       #fdiv.ps
    fmv.x.s t6, f9
    bne	t6,t5,fail

    addi t3, t3,8
    fsqrt.s    ft10,ft0         #fsqrt.s
    fmv.x.s t6, f10
    bne	t6,t5,fail

    addi t3, t3,8
    fsqrt.s ft11,ft0            #fsqrt.ps
    fmv.x.s t6, f11
    bne	t6,t5,fail

      #enable to patch fmadd into mcode
    addi t3, t3,8
    li a0, FADD_PS_MINSTMASK
    csrw 0x7cd, a0            # minstmask
    li a0, FADD_PS_MINSTMATCH
    csrw 0x7ce, a0            # minstmatch to match fmadd.ps and fd==5
    fmadd.ps f5,ft0,ft1,ft2   #fmadd.ps in mcode
    fmv.x.s t6, f12
    bne	t6,t5,fail

      #disable mcode patch
    li t3, 0xff
    li a0, DEF_MINSTMASK
    csrw 0x7cd, a0            # minstmask
    li a0, DEF_MINSTMATCH
    csrw 0x7ce, a0            # minstmatch to match fmadd.ps and fd==5
    fmadd.ps ft5,ft0,ft1,ft2  # regular fmadd.ps, shouldn't trap

    call pass


.align 12 //trap vector aligned to 4k... restriction of Xtvec CSRs
trap_vector:
  csrw mscratch, sp                       # mscratch <- sp
  csrr a1, mcause                         # a1 <- mcause
  li a0, 0x1e
//  bne a1,a0,fail                        # check cause is m-code
  bgez a1, .Lhandle_trap_in_machine_mode  # handle trap in machine mode


  .Lhandle_trap_in_machine_mode:
    # Preserve the registers.  Compute the address of the trap handler

    csrr a2, mepc                    # a2   <- mepc
    addi a3, a2,4                    # a3   <- mepc+4
    csrw mepc, a3                    # mepc <- a3
    csrr t0, mscratch                # t0   <- user sp

    # decode instructions. should decode instruction bits,
    # using predefined t6 instead
    la t4, mcode_table  # address of mcode_table
    add t1, t3, t4
    ld t0, 0(t1)
    jr t0               # jump to mcode_table


mcode_fcvt_s_l:
  fmv.s.x f0, t5
  call Lmret       # machine mode return


mcode_fcvt_s_lu:
  fmv.s.x f1, t5
  call Lmret       # machine mode return

mcode_fcvt_l_s:
  fmv.s.x f2, t5
  call Lmret       # machine mode return

mcode_fcvt_lu_s:
  fmv.s.x f3, t5
  call Lmret       # machine mode return

mcode_fdiv_pi:
  fmv.s.x f4, t5
  call Lmret       # machine mode return

mcode_fdivu_pi:
  fmv.s.x f5, t5
  call Lmret       # machine mode return

mcode_fremu_pi:
  fmv.s.x f6, t5
  call Lmret       # machine mode return

mcode_frem_pi:
  fmv.s.x f7, t5
  call Lmret       # machine mode return

mcode_fdiv_s:
  fmv.s.x f8, t5
  call Lmret       # machine mode return

mcode_fdiv_ps:
  fmv.s.x f9, t5
  call Lmret       # machine mode return

mcode_fsqrt_s:
  fmv.s.x f10, t5
  call Lmret       # machine mode return

mcode_fsqrt_ps:
  fmv.s.x f11, t5
  call Lmret       # machine mode return

mcode_instmcode_ps:
  fmv.s.x f12, t5
  call Lmret       # machine mode return


Lmret:
  # Go back where we came.
  csrr sp, mscratch
  mret


fail:
  slli	a0,t3,0x1
  ori	a0,a0,1
  ASM_TEST_FAIL

pass:
  ASM_TEST_PASS


/*
.data
.global __data
.align 8

__data:
.incbin "data.raw"            // generated with dd if=/dev/urandom of=data.raw bs=1M count=1
*/

.section .rodata
.align 8
.global mcode_table
mcode_table:
  .quad mcode_fcvt_s_l
  .quad mcode_fcvt_s_lu
  .quad mcode_fcvt_l_s
  .quad mcode_fcvt_lu_s
  .quad mcode_fdiv_pi
  .quad mcode_fdivu_pi
  .quad mcode_fremu_pi
  .quad mcode_frem_pi
  .quad mcode_fdiv_s
  .quad mcode_fdiv_ps
  .quad mcode_fsqrt_s
  .quad mcode_fsqrt_ps
  .quad mcode_instmcode_ps
