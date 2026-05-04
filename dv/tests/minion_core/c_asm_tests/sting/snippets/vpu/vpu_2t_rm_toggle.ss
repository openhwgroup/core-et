; This snippet rus two threaded test on minion0
; Rounding modes are changed randmonly while both threads execute fp operations
; The idea is to test when rounding mode of one thread affects operations on the other thread of the core 

import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource
cpu_t   x
;unum_t var = 10
num_t r_rm  
num_t r_gen_fadd
num_t index

base_t  rb1[256]@64

unum_t      off = 128 ;snp_select_memop_offset(0, STR_MEM.MEM_SIZE, 4, RV_PARAMS.HAS_UNALIGNED)

pfreg_t rf1, rf2, rf3, rf4, rf5, rf6, rf7
axreg_t rx1, rx2, rx3, rx4, rx5, rx6

gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
unum_t  cpuid = snp_cpu_id()

snip_setup

snip_init
    ; Empty

snip_run
@cpu: 2,3..BOARD_CONF.NUM_CPUS
    for index in (0, 100, 1):
       r_gen_fadd = snp_urandrange_glbl(0,2)
       add x0, x0, x0
    endfor

@cpu: 0,1

    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)

    for index in (0, 100, 1):
       ; Not generating 7
       ; Sting does not have an easy mechanism for ignoring numbers within a range, 
       ; plus the intent of the test can be tested without it 
       r_rm = snp_urandrange(0,5) 
       r_gen_fadd = snp_urandrange_glbl(0,2)   
       csrwi frm, r_rm
       if r_gen_fadd == 1:
       SELECT 1 {
         1 : fadd.ps rf1, rf2, rf3
         1 : fmadd.ps rf2,rf3,rf4,rf5
         1 : fsub.ps rf3, rf6, rf7
       }
       endif
    endfor

    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

snip_check
    ; Empty

