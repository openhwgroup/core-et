
.global mtrap_vector
    .align 12
mtrap_vector:
   // make copy of registers
    addi sp, sp, -272 // 16B aligned
    sd a0, 80(sp)
    sd a1, 88(sp)
        
    // set mepc to next instruction
    csrr a0, mepc        
    addi a0, a0, 4
    csrw mepc, a0

    // if mcause=0x1f, set trapped to 1
    csrr a0, mcause
    li a1, 0x1f
    bne  a0, a1, end_trap
    csrwi 0x8d2, 1
end_trap:   
    ld a0, 80(sp)
    ld a0, 88(sp)        
    add sp, sp, 272
    
    // and return
    mret
