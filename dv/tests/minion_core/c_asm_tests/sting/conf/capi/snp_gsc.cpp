
#include "snp_common.h"
#include "snp_gsc.h"
#include "board_conf.h"
#include "lib_snip_transform.h"
#include <stdio.h>
#include <assert.h>

unum_t snp_get_gsc_global_alignment(CpuResource * cpu_resource, unum_t op_type) {
    //There are 4 different type of aligments:
    // 0) Cache Line alignment (64bytes) => FLWG.PS, FSWG.PS
    // 1) No exception (alignment does not matter) => FGBG.PS, FSCBG.PS
    // 2) 16 bits alignment => FGHG.PS, FSCHG.PS
    // 3) 32 bits alignment => FGWG.PS, FSCWG.PS

    op_type = op_type % 4;
    int ret;
    switch (op_type) {
        case 0:
            ret = 64;
            break;
        case 1:
            ret = -1;
            break;
        case 2:
            ret =  2;
            break;
        case 3:
            ret =  4;
            break;
    }
    return ret;
}

unum_t snp_get_gsc_offset_base(CpuResource * cpu_resource, unum_t op_alignment, unum_t must_trap, unum_t gsc_mask, unum_t lane, unum_t base_align) {
    num_t offset = 0;
    unum_t mod = (base_align%op_alignment);
    unum_t rand_stride = snp_urandrange(cpu_resource, 0, 12) * op_alignment;
    unum_t is_masked = ~snp_get_bits(cpu_resource, gsc_mask, lane, lane);

    if (is_masked == 1) {
        must_trap = snp_urandrange(cpu_resource, 0, 2);
    }

    if (must_trap == 1) {
        unum_t rand_off = snp_urandrange(cpu_resource, 1, op_alignment);
        offset = rand_off - mod;
    }
    else {
        offset = (op_alignment - mod);
    }
    unum_t ret = offset+rand_stride;

    if (is_masked == 0) assert((((64+base_align)+ret)%op_alignment) == 0);
    return ret;
}

unum_t snp_get_gsc_offset(CpuResource * cpu_resource, unum_t op_type, unum_t gsc_mask, unum_t lane, unum_t base_align) {

    unum_t must_trap = snp_urandrange(cpu_resource, 0, 2);

    num_t op_alignment = snp_get_gsc_global_alignment(cpu_resource, op_type);

    if (op_alignment < 0) return 0;

    unum_t ret = snp_get_gsc_offset_base(cpu_resource, op_alignment, must_trap, gsc_mask, lane, base_align);
    return ret;
}

unum_t snp_get_gsc_offset_notrap(CpuResource * cpu_resource, unum_t op_type, unum_t gsc_mask, unum_t lane, unum_t base_align) {

    unum_t must_trap = 0;

    num_t op_alignment = snp_get_gsc_global_alignment(cpu_resource, op_type);

    if (op_alignment < 0) return 0;

    unum_t ret = snp_get_gsc_offset_base(cpu_resource, op_alignment, must_trap, gsc_mask, lane, base_align);
    return ret;
}
