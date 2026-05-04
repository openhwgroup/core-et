#include "macros.h"
        
.global mtrap_vector
        
.align 12
mtrap_vector:   
    j bad_trap 
    j bad_trap 
    j bad_trap 
    j bad_trap    
    j bad_trap
    j bad_trap
    j bad_trap
    j irq_handler // MTIP => the only one supported!
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

