#include "macros.h"

.global mtrap_vector_v

.align 12
mtrap_vector_v:
    j bad_trap
    j bad_trap
    j bad_trap
    j irq_handler // bit3 - mip.MSIP
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
