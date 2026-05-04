#include "macros.h"

    
.text
.global _start
_start: 
    ASM_TEST_START

    la a1, __data

    // Int values to be stored in cacheline
    li a4, 1  
    li a5, 2    

    lw a6, 0(a1)   

    //Store 16-bit values within same dword
    //bypass should happen
    sh a4, 0(a1)    
    sh a5, 2(a1)
    //Load previous dword
    lw a6, 0(a1)

    //Store 32-bit of data
    sw a4, 4(a1)
    //Overwrite 16 upper bits
    sh a5, 6(a1)
    //Load previous dword
    lw a7, 4(a1)

    //Self-check that both yield the same result
    bne a6, a7, fail
    
    ASM_TEST_PASS 

    fail: ASM_TEST_FAIL

.data
.global __data
.align 8
__data: 
.incbin "data.raw"            // generated with dd if=/dev/urandom of=data.raw bs=1M count=1
