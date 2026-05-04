#include "macros.h"
        
.global mtrap_vector_ipi
        
.align 12
mtrap_vector_ipi:   
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


