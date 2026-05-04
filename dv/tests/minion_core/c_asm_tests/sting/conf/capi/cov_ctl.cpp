
#include "cov_ctl.h"
#include "snp_common.h"
#include "et_soc.h"
#include "lib_snip_transform.h"

unum_t glbl_ctl_urandrange(CpuResource * cpu_resource, unum_t is_coop, unum_t min, unum_t max) {
    if (is_coop == 1) return snp_urandrange_glbl(cpu_resource, min, max + 1);
    else return snp_urandrange(cpu_resource, min, max + 1);
} 

// This function returns a mask with N bits set to 1, where (min <= N <= max)
unum_t glbl_snp_get_mask_range(CpuResource * cpu_resource, unum_t is_coop, unum_t mask_size, unum_t min, unum_t max) {
    unum_t range = glbl_ctl_urandrange(cpu_resource, is_coop, min, max);
    unum_t mask = (1 << mask_size) - 1;
    while (countones(cpu_resource, mask_size, mask) > range) {
        unum_t dis_bit = glbl_ctl_urandrange(cpu_resource, is_coop, 0, mask_size-1);
        mask = snp_set_bits(cpu_resource, mask, dis_bit, dis_bit, 0);
    }
    return mask;
}

unum_t glbl_cov_tensor_load_lines(CpuResource * cpu_resource, unum_t csr_value, unum_t is_coop){


    /******************************************/
    /** SET MASK IN TLOAD *********************/
    unum_t set_mask = glbl_ctl_urandrange(cpu_resource, is_coop, 0, 1);
    csr_value = snp_set_bits(cpu_resource, csr_value, 63, 63, set_mask);


    /******************************************/
    /**  NUM LINES SELECTION ******************/
    unum_t num_lines = glbl_ctl_urandrange(cpu_resource, is_coop, 0, 3);
    switch (num_lines) {
        case 0:
            num_lines = 0;
            break;
        case 1:
            num_lines = glbl_ctl_urandrange(cpu_resource, is_coop, 1, 8);
            break;
        case 2:
            num_lines = glbl_ctl_urandrange(cpu_resource, is_coop, 8, 14);
            break;
        case 3:
            num_lines = 15;
            break;
    }
    return snp_set_bits(cpu_resource, csr_value, 3, 0, num_lines);
}

unum_t glbl_cov_tensor_load_mask(CpuResource * cpu_resource, unum_t is_coop) {

    unum_t csr_value = snp_randnum(cpu_resource); // For WARL(0) values
    unum_t mask_size = 16; // 16 bits for tensor load mask
 
    /******************************************/
    /** NUM MASKED LINES SELECTION*************/
    unum_t num_lines = glbl_ctl_urandrange(cpu_resource, is_coop, 0, 3);
    switch (num_lines) {
        case 0:
            num_lines = 0;
            break;
        case 1:
            num_lines = glbl_snp_get_mask_range(cpu_resource, is_coop, mask_size, 1, 7);
            break;
        case 2:
            num_lines = glbl_snp_get_mask_range(cpu_resource, is_coop, mask_size, 8, 15);
            break;
        case 3:
            num_lines = 0xffff;
            break;
    }
    return snp_set_bits(cpu_resource, csr_value, 15, 0, num_lines);
}
