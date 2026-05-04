#include "macros.h"
        
.global mtrap_vector
        
.align 12
mtrap_vector:   
    j bad_trap 
    j bad_trap 
    j bad_trap 
    j irq_handler // MSIP => the only one supported!
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap


