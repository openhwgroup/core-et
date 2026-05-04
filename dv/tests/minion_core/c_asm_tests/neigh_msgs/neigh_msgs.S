#include "macros.h"
.text
.global _start
_start: 
    ASM_TEST_START

    li a1, 0xDEADBEAF

    // Build address for minion messages
    // Load bits 39:20

    //li a2, 0x01FFC000
    //li a2, 0x013FC000
    li a2, 0x01000000
    slli a2, a2, 8

    // Load bits 11:0
    li t0, 0x8
    slli t0, t0, 8
    or a2, a2, t0

    // Take neigh_id and thread_id and load it
    csrr t0, mhartid
    andi t0, t0, 0x71
    slli t0, t0, 12
    or a2, a2, t0

    // Take min_id
    csrr a3, mhartid
    srli a3, a3, 1
    andi a3, a3, 0x7

    // Send to all other minions in the neighborhood
    //li t0, 8
    li t0, 5
    li t1, 0

send_msg:
    // Do not send message to itself
    sub t2, t1, a3
    beqz t2, next_msg

    // Load destination min_id
    slli a4, t1, 13
    or a4, a4, a2

    // Write to port 0
    sd a1, 0(a4)    
    // Write to port 1
    sd a1, 0x40(a4)    
    // Write to port 2
    sd a1, 0x80(a4)    
    // Write to port 3
    sd a1, 0xc0(a4)    

next_msg:
    // Go to next minion
    addi t1, t1, 1
  
    // If min_id == 8, finish
    sub t2, t1, t0
    beqz t2, end

    li t2, 3
    sub t2, t1, t2
    beqz t2, next_msg

    j send_msg

end:
    ASM_TEST_PASS

