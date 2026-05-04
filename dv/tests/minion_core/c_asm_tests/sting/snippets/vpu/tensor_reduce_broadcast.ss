; The snippet tests tensor broadcast form of reduce b/w minions
; The snippet is scalable across the number of minions available
; tree_depth is set as 0

import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource
cpu_t   x

num_t num_fp_reg = snp_urandrange_glbl(0,128) 
num_t start_fp_reg = snp_urandrange_glbl(0,32) 
num_t operation_control = snp_urandrange(0,7) 
num_t start_fp_reg_shift
num_t num_fp_reg_shift
num_t tree_depth = 0
num_t tree_depth_shift

num_t Xs_10 = 2
num_t operation
num_t operation_shift

num_t Xs_formation_a
num_t Xs_formation_b
num_t Xs_formation_c
num_t Xs_formation_d

pfreg_t rf1, rf2, rf3, rf4
axreg_t rx1, rx2, rx3

gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
unum_t  cpuid = snp_cpu_id()

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run

@cpu: 0,2..BOARD_CONF.NUM_CPUS

    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)

    flog.ps rf1, rf2
    
    start_fp_reg_shift = snp_lshift(start_fp_reg, 57)
    num_fp_reg_shift = snp_lshift(num_fp_reg, 16)
    
    if operation_control == 0:
       operation = 0
    endif 
    if operation_control == 1:
       operation = 2
    endif
    if operation_control == 2:
       operation = 3
    endif
    if operation_control == 3:
       operation = 4
    endif
    if operation_control == 4:
       operation = 6
    endif
    if operation_control == 5:
       operation = 7
    endif
    if operation_control == 6:
       operation = 8
    endif

    operation_shift = snp_lshift(operation, 24)
    tree_depth_shift = snp_lshift(tree_depth,3)

    Xs_formation_a = snp_or(start_fp_reg_shift, num_fp_reg_shift) 
    Xs_formation_b = snp_or(Xs_formation_a,operation_shift)
    Xs_formation_c = snp_or(Xs_formation_b,tree_depth_shift)
    Xs_formation_d = snp_or(Xs_formation_c,Xs_10)

    li x5, Xs_formation_d

    csrrw  rx1,  tensor_reduce, x5 
    csrwi tensor_wait, 0x9

    flog.ps rf3, rf4

    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

snip_check
    ; Empty
