#include "macros.h"

#define ITERATIONS 256
#define UNTAG 12
#define UNSET 6
#define WAYS 4
#define TICKETER 0xFFF00000
    
.text
.global _start
_start: 
    ASM_TEST_START

    // 1 % 2
    li x2, 0x8000000000000001
    li x3, 2
    remw x1, x2, x3

    // 0 % 2
    li x2, 0x8000000000000000
    li x3, 0xfffffffe // -2
    remw x1, x2, x3

    
    // 0 % -1955
    li x2, 0
    li x3, -1955
    remw x1, x2, x3

    // 0 % -232323
    li x3, -232323
    remw x1, x2, x3

     // 0 % 1955
    li x3, 1955
    remw x1, x2, x3
    
    // 0 % 232323
    li x3, 232323
    remw x1, x2, x3


     // 12399 %  -1955
    li x2, 12399
    li x3, -1955
    remw x1, x2, x3

    // 12399 %  -232323
    li x3, -232323
    remw x1, x2, x3

     // 12399 %  1955
    li x3, 1955
    remw x1, x2, x3
    
    // 12399 %  232323
    li x3, 232323
    remw x1, x2, x3


    // -1955 %  -1955
    li x2, -1955
    li x3, -1955
    remw x1, x2, x3

    // -1955 %  -232323
    li x3, -232323
    remw x1, x2, x3

     // -1955 %  1955
    li x3, 1955
    remw x1, x2, x3
    
    // -1955 %  232323
    li x3, 232323
    remw x1, x2, x3



    # repeat, with some upper bits (63..32) set
    li x5, 0x8000800000000000

    // 0 % -1955
    li x2, 0
    or x2, x2, x5
    li x3, -1955
    remw x1, x2, x3

    // 0 % -232323
    li x3, -232323
    remw x1, x2, x3

     // 0 % 1955
    li x3, 1955
    remw x1, x2, x3
    
    // 0 % 232323
    li x3, 232323
    remw x1, x2, x3


     // 12399 %  -1955
    li x2, 12399
    or x2, x2, x5
    li x3, -1955
    remw x1, x2, x3

    // 12399 %  -232323
    li x3, -232323
    remw x1, x2, x3

     // 12399 %  1955
    li x3, 1955
    remw x1, x2, x3
    
    // 12399 %  232323
    li x3, 232323
    remw x1, x2, x3


    // -1955 %  -1955
    li x2, -1955
    or x2, x2, x5
    li x3, -1955
    remw x1, x2, x3

    // -1955 %  -232323
    li x3, -232323
    remw x1, x2, x3

     // -1955 %  1955
    li x3, 1955
    remw x1, x2, x3
    
    // -1955 %  232323
    li x3, 232323
    remw x1, x2, x3



    
    end:
    ASM_TEST_PASS
end_loop:
    j end_loop
