
#=========================================================================
# Bootrom code for Maxion core
#=========================================================================


  .extern __stack

  .section .bootrom

  .global _boot
_boot:

  li    t0, 0x603
  csrw  dcsr, t0
  la    t0, 1f
  csrw  dpc, t0
  dret

1:
  # init some csr regs
  csrwi satp, 0
  csrr  t0, mstatus
  li    t1, 0x6000
  or    t0, t0, t1
  csrw  mstatus, t0 # set fs=11 (dirty)
  csrrw x0, fcsr, x0
  csrrw x0, mip, x0

  # set the stack pointer and start code, different for each thread
  csrr  a1, mhartid
  and   a1, a1, 3

  li    t0, 0
  beq   a1, t0, _hart0
  li    t0, 1
  beq   a1, t0, _hart1
  li    t0, 2
  beq   a1, t0, _hart2
  li    t0, 3
  beq   a1, t0, _hart3

_hart2:
  la    sp, __stack


  addi x1,  x0, 0
  addi x3,  x0, 0
  addi x4,  x0, 0
  addi x5,  x0, 0
  addi x6,  x0, 0
  addi x7,  x0, 0
  addi x8,  x0, 0
  addi x9,  x0, 0
  addi x10, x0, 0
  addi x11, x0, 0
  addi x12, x0, 0
  addi x13, x0, 0
  addi x14, x0, 0
  addi x15, x0, 0
  addi x16, x0, 0
  addi x17, x0, 0
  addi x18, x0, 0
  addi x19, x0, 0
  addi x20, x0, 0
  addi x21, x0, 0
  addi x22, x0, 0
  addi x23, x0, 0
  addi x24, x0, 0
  addi x25, x0, 0
  addi x26, x0, 0
  addi x27, x0, 0
  addi x28, x0, 0
  addi x29, x0, 0
  addi x30, x0, 0
  addi x31, x0, 0

  fcvt.s.w f0,  x0
  fcvt.s.w f1,  x0
  fcvt.s.w f2,  x0
  fcvt.s.w f3,  x0
  fcvt.s.w f4,  x0
  fcvt.s.w f5,  x0
  fcvt.s.w f6,  x0
  fcvt.s.w f7,  x0
  fcvt.s.w f8,  x0
  fcvt.s.w f9,  x0
  fcvt.s.w f10, x0
  fcvt.s.w f11, x0
  fcvt.s.w f12, x0
  fcvt.s.w f13, x0
  fcvt.s.w f14, x0
  fcvt.s.w f15, x0
  fcvt.s.w f16, x0
  fcvt.s.w f17, x0
  fcvt.s.w f18, x0
  fcvt.s.w f19, x0
  fcvt.s.w f20, x0
  fcvt.s.w f21, x0
  fcvt.s.w f22, x0
  fcvt.s.w f23, x0
  fcvt.s.w f24, x0
  fcvt.s.w f25, x0
  fcvt.s.w f26, x0
  fcvt.s.w f27, x0
  fcvt.s.w f28, x0
  fcvt.s.w f29, x0
  fcvt.s.w f30, x0
  fcvt.s.w f31, x0

  li zero, 0xf
#  mova.m.x zero

  la    a1, _start
  jalr  x0, a1

# Stalled harts
_hart0:
_hart1:
_hart3:
  nop
  wfi

