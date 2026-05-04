; The snippet does tensor reduce send and receive 
; Minion0 receives from Minions 1,2,3 back-to-back
; Minion1 should complete first, Minion2 next, followed by Minion3
;M0:
;Receive from M1
;Receive from M2
;Receive from M3
;M1:
;Send to M0
;M2:
;Send to M0
;M3:
;Send to M0 

import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource
cpu_t   x
;unum_t var = 10

num_t num_fp_reg = snp_urandrange_glbl(0,128)
num_t start_fp_reg
num_t start_fp_reg_shift 
num_t num_fp_reg_shift 
num_t Xs_3_sen 
num_t Xs_3_sen_shift
num_t Xs_3_rec 
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

    start_fp_reg = snp_urandrange(0,32) 
    start_fp_reg_shift = snp_lshift(start_fp_reg, 57)
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
    Xs_3_sen = snp_get_global_minion_id(2)
    Xs_3_sen_shift = snp_lshift(Xs_3_sen, 3)

    Xs_formation_a_1 = snp_or(start_fp_reg_shift, num_fp_reg_shift) 
    Xs_formation_b_1 = snp_or(Xs_formation_a_1, operation_shift) 
    Xs_formation_c = snp_or(Xs_formation_b_1, Xs_3_sen_shift) 
    Xs_formation_d = snp_or(Xs_formation_c, Xs_0) 
    li x6, Xs_formation_d
    
    csrrw  rx1,  tensor_reduce, x6 


    ; Operation 2 - Receive from Minion2
    Xs_3_sen = snp_get_global_minion_id(4)
    Xs_3_sen_shift = snp_lshift(Xs_3_sen, 3)

    Xs_formation_a_1 = snp_or(start_fp_reg_shift, num_fp_reg_shift) 
    Xs_formation_b_1 = snp_or(Xs_formation_a_1, operation_shift) 
    Xs_formation_c = snp_or(Xs_formation_b_1, Xs_3_sen_shift) 
    Xs_formation_d = snp_or(Xs_formation_c, Xs_0) 
    li x6, Xs_formation_d
    csrrw  rx1,  tensor_reduce, x6 

    ; Operation 3 - Receive from Minion2
    Xs_3_sen = snp_get_global_minion_id(6)
    Xs_3_sen_shift = snp_lshift(Xs_3_sen, 3)

    Xs_formation_a_1 = snp_or(start_fp_reg_shift, num_fp_reg_shift) 
    Xs_formation_b_1 = snp_or(Xs_formation_a_1, operation_shift) 
    Xs_formation_c = snp_or(Xs_formation_b_1, Xs_3_sen_shift) 
    Xs_formation_d = snp_or(Xs_formation_c, Xs_0) 
    li x6, Xs_formation_d
    csrrw  rx1,  tensor_reduce, x6 

    csrwi tensor_wait, 0x9
    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

@cpu: 2
    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)
 
    start_fp_reg = snp_urandrange(0,32) 
    start_fp_reg_shift = snp_lshift(start_fp_reg, 57)
    num_fp_reg_shift = snp_lshift(num_fp_reg, 16)
    
    ;Receiver minion-id
    Xs_3_rec = snp_get_global_minion_id(0)
    Xs_3_rec_shift = snp_lshift(Xs_3_rec, 3)

    Xs_formation_a_0 = snp_or(start_fp_reg_shift, num_fp_reg_shift) 
    Xs_formation_b_0 = snp_or(Xs_formation_a_0, Xs_3_rec_shift) 
    li x5, Xs_formation_b_0
    
    csrrw  rx1,  tensor_reduce, x5 

    csrwi tensor_wait, 0x9
    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

@cpu: 4
    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)
 
    start_fp_reg = snp_urandrange(0,32) 
    start_fp_reg_shift = snp_lshift(start_fp_reg, 57)
    num_fp_reg_shift = snp_lshift(num_fp_reg, 16)
    
    ;Receiver minion-id
    Xs_3_rec = snp_get_global_minion_id(0)
    Xs_3_rec_shift = snp_lshift(Xs_3_rec, 3)

    Xs_formation_a_0 = snp_or(start_fp_reg_shift, num_fp_reg_shift) 
    Xs_formation_b_0 = snp_or(Xs_formation_a_0, Xs_3_rec_shift) 
    li x5, Xs_formation_b_0
    
    csrrw  rx1,  tensor_reduce, x5 

    csrwi tensor_wait, 0x9
    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

@cpu: 6
    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)
 
    start_fp_reg = snp_urandrange(0,32) 
    start_fp_reg_shift = snp_lshift(start_fp_reg, 57)
    num_fp_reg_shift = snp_lshift(num_fp_reg, 16)
    
    ;Receiver minion-id
    Xs_3_rec = snp_get_global_minion_id(0)
    Xs_3_rec_shift = snp_lshift(Xs_3_rec, 3)

    Xs_formation_a_0 = snp_or(start_fp_reg_shift, num_fp_reg_shift) 
    Xs_formation_b_0 = snp_or(Xs_formation_a_0, Xs_3_rec_shift) 
    li x5, Xs_formation_b_0
    
    csrrw  rx1,  tensor_reduce, x5 

    csrwi tensor_wait, 0x9
    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)


snip_check
    ; Empty
