#include "macros.h"
        
.global mtrap_vector_ipi
        
.align 12
mtrap_vector_ipi:   
    j bad_trap 
    j bad_trap 
    j bad_trap 
    j safe_gate // MSIP
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


