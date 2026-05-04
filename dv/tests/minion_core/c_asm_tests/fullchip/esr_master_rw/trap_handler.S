#include "macros.h"

.global trap_vector
        
.align 12
trap_vector:   
    j machine_handler 
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
    j bad_trap
    j bad_trap
    j bad_trap

