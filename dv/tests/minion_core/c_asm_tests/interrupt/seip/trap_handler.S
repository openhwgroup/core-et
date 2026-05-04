#include "macros.h"
        
.global mtrap_vector
.global trap_vector

.align 12
mtrap_vector:   
    j bad_trap 
    j bad_trap 
    j bad_trap 
    j bad_trap    
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap 
    j bad_trap
    j irq_handler  // SEIP => the only one supported!
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

.align 12
trap_vector:   
    j bad_trap 
    j bad_trap 
    j bad_trap 
    j bad_trap    
    j bad_trap
    j bad_trap
    j bad_trap
    j bad_trap 
    j bad_trap
    j irq_handler  // SEIP => the only one supported!
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
