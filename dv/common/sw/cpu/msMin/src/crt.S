
#=========================================================================
# crt.S : Entry point for RISC-V user programs
#=========================================================================

  .extern __global_pointer$
  .extern __gpr__
  .extern __bss_start__

  .text
  .global _start
_start:
  # Initialize global pointer
  mv    a5, gp
  la    a5, __gpr__
  addi  gp, a5, 0
  la    a5, __bss_start__
#  auipc gp, %pcrel_hi(__global_pointer$)
#  addi  gp, gp, %pcrel_lo(1b)

  call    main
  wfi

