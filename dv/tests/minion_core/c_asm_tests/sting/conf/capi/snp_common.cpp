
#include "snp_common.h"
#include "lib_snip_transform.h"
#include <assert.h>

// Given an integer "n", returns the log2(n) value
unum_t log2n(CpuResource * cpu_resource, unum_t n) {
    return (n > 1) ? 1 + log2n(cpu_resource, n/2) : 0;
}

unum_t snp_get_bits(CpuResource * cpu_resource, unum_t value, unum_t head, unum_t tail) {
    // Just in case the programmer does not specify the arguments in the correct order
    if (head < tail) {
        unum_t tmp = head;
        head = tail;
        tail = tmp;
    }
    // we want to extract k-bits
    unum_t k = (head - tail) + 1;
    return (((1 << k) - 1) & (value >> tail));
}

unum_t snp_set_bits(CpuResource * cpu_resource, unum_t value, unum_t head, unum_t tail, unum_t set_value) {
    // Just in case the programmer does not specify the arguments in the correct order
    if (head < tail) {
        unum_t tmp = head;
        head = tail;
        tail = tmp;
    }
    // we want to set k-bits
    unum_t k = (head - tail) + 1;
    unum_t mask = ((1 << k) - 1);

    set_value = (set_value & mask) << tail;
    mask = ~(mask << tail);

    value = value & mask;
    value = value | set_value;

    return value;
}

unum_t snp_in_range(CpuResource * cpu_resource, unum_t min, unum_t max, unum_t max_inclusive, unum_t value) {
   unum_t tmp = min;
   if (min > max) {
      min = max;
      max = tmp;
   }
   tmp = (max_inclusive == 0) ? (value < max) : (value <= max);
   return ((value >= min) && tmp);
}

// This functions returns the number of ones in the mask. The "mask_size" parameter
// specifies the how many bits (starting from bit 0) must be considered
unum_t countones(CpuResource * cpu_resource, unum_t mask_size, unum_t value) {
    unum_t ones = 0;
    for (int i = 0; i < mask_size; i++) {
        ones = ones + (value & 0x0000000000001);
        value = value >> 1;
    }
    return ones;
}

unum_t snp_if_zero_else(CpuResource * cpu_resource, unum_t bias, unum_t if_val, unum_t else_val) {
   if (bias == 0) return if_val;
   else           return else_val;
}

unum_t et_urandrange(CpuResource * cpu_resource, const unum_t is_glbl, const unum_t head, const unum_t tail) {
   if (is_glbl == NOT_GLBL_RAND) {
      return snp_urandrange(cpu_resource, head, tail);
   }
   else if (is_glbl == IS_GLBL_RAND) {
      return snp_urandrange_glbl(cpu_resource, head, tail);
   }
   assert("Unreachable code" && 0);
   return 0xbadc0de;
}
unum_t et_randnum(CpuResource * cpu_resource, const unum_t is_glbl) {
   if (is_glbl == NOT_GLBL_RAND) {
      return snp_randnum(cpu_resource);
   }
   else if (is_glbl == IS_GLBL_RAND) {
      return snp_randnum_glbl(cpu_resource);
   }
   assert("Unreachable code" && 0);
   return 0xbadc0de;
}
