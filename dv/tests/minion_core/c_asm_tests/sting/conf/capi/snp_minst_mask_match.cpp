
#include "snp_minst_mask_match.h"

#include <stdlib.h>
#include "lib_snip_transform.h"

/**
 * (Auxiliary function) 
 * Shuffles randomly the bits that within a number
*/
unum_t shuffle_bits(CpuResource * cpu_resource, unum_t number, unum_t length) {
    uint i, bit_i, r, bit_r;
    for (i = 0; i < length; ++i) {
        // Take the value of the ith bit from number
        bit_i = (number >> i) & 1;
        // Take the value of a random bit from number
        r = snp_randnum(cpu_resource) % length;
        bit_r = (number >> r) & 1;
        // Swap these two bits
        number = (number & ~(1 << i)) | (bit_r << i);
        number = (number & ~(1 << r)) | (bit_i << r);
    }
    return number;
}

unum_t snp_get_minst_mask(CpuResource * cpu_resource, unum_t min_one_bits, unum_t max_one_bits) {
    uint number_one_bits;  // The number of bits that will be set to 1
    unum_t number;  // A number with "number_one_bits" to 1

    number_one_bits = min_one_bits + snp_randnum(cpu_resource) % (max_one_bits - min_one_bits);
    number = 0xFFFFFFFF;
    number = ~((number >> number_one_bits) << number_one_bits);

    return shuffle_bits(cpu_resource, number, 32);
}
