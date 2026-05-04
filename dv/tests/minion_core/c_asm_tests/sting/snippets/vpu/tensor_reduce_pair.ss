; The snippet tests tensor reduce pair instruction
; Minion1 is the receiver, Minion0 is the sender

import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource
cpu_t   x
;unum_t var = 10

num_t num_fp_reg = snp_urandrange_glbl(0,128)
num_t start_fp_reg
num_t start_fp_reg_2
num_t start_fp_reg_shift 
num_t start_fp_reg_shift_2 
num_t num_fp_reg_shift
num_t Xs_3_sen = 0
num_t Xs_3_sen_shift
num_t Xs_3_rec = 1
num_t Xs_3_rec_shift
num_t Xs_formation_a_0
num_t Xs_formation_b_0
num_t Xs_formation_a_1
num_t Xs_formation_b_1

num_t Xs_formation_c
num_t Xs_formation_d
num_t Xs_0 = 1
num_t operation 
num_t operation_shift
num_t operation_control


gmemory_t   snp_mem_tgt[STR_MEM.MEM_SIZE]@STR_MEM.MEM_ALIGN

unum_t      off = 4 ;//snp_select_memop_offset(0, STR_MEM.MEM_SIZE, 4, RV_PARAMS.HAS_UNALIGNED)

pfreg_t rf1, rf2, rf3, rf4
axreg_t rx1, rx2, rx3

gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
unum_t  cpuid = snp_cpu_id()

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run

@cpu: 0
    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)
    fnmadd.s rf1, rf2, rf3, rf4
    ;RANDINST 0:var
 
    start_fp_reg = snp_urandrange(0,32) 
    start_fp_reg_shift = snp_lshift(start_fp_reg, 57)
    num_fp_reg_shift = snp_lshift(num_fp_reg, 16)
    
    ;Receiver minion-id
    Xs_3_rec_shift = snp_lshift(Xs_3_rec, 3)

    Xs_formation_a_0 = snp_or(start_fp_reg_shift, num_fp_reg_shift) 
    Xs_formation_b_0 = snp_or(Xs_formation_a_0, Xs_3_rec_shift) 

    li x5, Xs_formation_b_0
    
    csrrw  rx1,  tensor_reduce, x5 

    csrwi tensor_wait, 0x9
    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)
@cpu: 2
    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)
    fmadd.s rf1, rf2, rf3, rf4
    ;RANDINST 0:var

    start_fp_reg_2 = snp_urandrange(0,32) 
    start_fp_reg_shift_2 = snp_lshift(start_fp_reg_2, 57)
    num_fp_reg_shift = snp_lshift(num_fp_reg, 16)
    
    operation_control = snp_urandrange(0,7) 
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


    ;Sender minion-id
    Xs_3_sen_shift = snp_lshift(Xs_3_sen, 3)

    Xs_formation_a_1 = snp_or(start_fp_reg_shift_2, num_fp_reg_shift) 
    Xs_formation_b_1 = snp_or(Xs_formation_a_1, operation_shift) 
    Xs_formation_c = snp_or(Xs_formation_b_1, Xs_3_sen_shift) 
    Xs_formation_d = snp_or(Xs_formation_c, Xs_0) 
   
    li x6, Xs_formation_d
    
    csrrw  rx1,  tensor_reduce, x6 

    csrwi tensor_wait, 0x9
    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)
snip_check
    ; Empty
