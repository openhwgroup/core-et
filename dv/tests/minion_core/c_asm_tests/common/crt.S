
#=========================================================================
# crt.S : Entry point for RISC-V user programs
#=========================================================================

  .text
  .global _start
_start:
  # Initialize global pointer; don't use relaxed relocation because it will
  # generate a "mv gp, gp" which sets gp to zero.
  .option push
  .option norelax
1:auipc gp, %pcrel_hi(__global_pointer$)
  addi  gp, gp, %pcrel_lo(1b)
  .option pop
  call    main
  wfi

