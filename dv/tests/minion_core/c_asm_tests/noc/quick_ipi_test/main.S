
.text
.global _start
_start:
   li a2, 0x80000000LL //maxion0 address
   wfi
        
  .rept 1024 // 1024 nops at around ad=0x10000 => ipi will jump to a PC to cause a nop slide
  nop
  .endr

  csrr a1, mhartid
  bnez a1, to_wfi
  sd zero, 0(a2)
to_wfi: 
  wfi
    
