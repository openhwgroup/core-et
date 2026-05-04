
#=========================================================================
# Bootrom code for Minion core
#=========================================================================

#ifndef STACK_BASE
#define STACK_BASE 0x41000000
#endif
#ifndef STACK_SIZE_LOG2
#define STACK_SIZE_LOG2 14
#endif
#ifndef STACK_SHIRE_OFFSET
#define STACK_SHIRE_OFFSET 0
#endif

  .section .bootrom, "ax", @progbits
  .global _boot
_boot:
  # Setup Validation CSR to indicate Start of Boot
  li   a7, 0xDEAD0001;
  csrw validation0, a7;
  # init some csr regs
  csrwi satp, 0
  csrr  t0, mstatus
  li    t1, 0x6000
  or    t0, t0, t1
  csrw  mstatus, t0 # set fs=11 (dirty)
  csrrw x0, fcsr, x0
  csrrw x0, mip, x0
  csrwi tensor_mask, 0
  csrwi tensor_conv_size, 0
  csrwi tensor_conv_ctrl, 0
  # set the stack pointer, different for each thread
  csrr  a1, mhartid
  slli  a1, a1, STACK_SIZE_LOG2
  li    sp, STACK_BASE
  sub   sp, sp, a1
  # Offset stack pointer for performance for each shire
  addi  x31, x0, -STACK_SHIRE_OFFSET
  csrr  x30, mhartid
  andi  x30, x30, -64
  and   x31, x30, x31
  add   sp, sp, x31

###################################################################\\
# FUTURE: Emmanuel Marie                                            #
#####################################################################
# Uncomment mtime/mtimecmp initialization when soc_tb, shiretop_tb  #
# support ESR accesses without notifying BAD accesses to mem region #
#####################################################################
## set mtime to 0, different for each thread
#  csrr  t0, mhartid
#  andi  t0, t0, ~63
#  slli  t0, t0, 16
#  li    a1, 0x80F001D8  # mtime address
#  or    a1, a1, t0
#  sd    x0, 0(a1)
## set mtimecmp to "infinity"
#  li    t0, -1
#  sd    t0, 8(a1)
###################################################################//

###################################################################\\
# FUTURE: Emmanuel Marie                                           #
#####################################################################
# Enabling interrupts generates several test errors
#####################################################################
# enable interrupts
#  csrrw x0, mie, t0

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
  mova.m.x zero

  # Setup Validation CSR to indicate End of Boot
  li   a7, 0xDEAD0002;
  csrw validation0, a7;
  
  la a1, _start
  jalr x0, a1


