
#include "cov_tensor_reduce_auto.h"
#include "snp_common.h"
#include "board_conf.h"

#define pwrtwo(x) (1 << (x))


unum_t cov_tensor_reduce_start_reg(CpuResource * cpu_resource, unum_t csr_value){

    /******************************************/
    /**  START REG SELECTION ******************/
    unum_t start_reg = snp_urandrange(cpu_resource, 0, 4);
    switch (start_reg) {
        case 0:
            start_reg = 0;
            break;
        case 1:
            start_reg = snp_urandrange(cpu_resource, 0x01, 0x0f + 1);
            break;
        case 2:
            start_reg = snp_urandrange(cpu_resource, 0x10, 0x1e + 1);
            break;
        case 3:
            start_reg = 31;
            break;
    }
    return snp_set_bits(cpu_resource, csr_value, 61, 57, start_reg);
}

unum_t glbl_cov_tensor_reduce_num_reg(CpuResource * cpu_resource, unum_t csr_value){

    /******************************************/
    /**  NUM REG SELECTION ********************/
    unum_t num_reg = snp_urandrange_glbl(cpu_resource, 0, 4);
    switch (num_reg) {
        case 0:
            num_reg = snp_urandrange_glbl(cpu_resource, 0x01, 0x08 + 1);
            break;
        case 1:
            num_reg = snp_urandrange_glbl(cpu_resource, 0x09, 0x10 + 1);
            break;
        case 2:
            num_reg = snp_urandrange_glbl(cpu_resource, 0x11, 0x18 + 1);
            break;
        case 3:
            num_reg = snp_urandrange_glbl(cpu_resource, 0x19, 0x20 + 1);
            break;
    }
    return snp_set_bits(cpu_resource, csr_value, 22, 16, num_reg);
}
