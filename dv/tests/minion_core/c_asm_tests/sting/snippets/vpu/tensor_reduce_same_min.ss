; This snippet does back to back receives on Minion0 from different minions in the neigh

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

num_t start_fp_reg 
num_t start_fp_reg_shift
num_t num_fp_reg 
num_t num_fp_reg_shift
num_t Xs_3_sen = 0
num_t Xs_3_sen_shift
num_t Xs_3_rec = 0
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
    num_fp_reg = snp_urandrange(0,128) 
    num_fp_reg_shift = snp_lshift(num_fp_reg, 16)
    
    ;Receiver minion-id
    ;Xs_3_rec = 1
    Xs_3_rec_shift = snp_lshift(Xs_3_rec, 3)

    Xs_formation_a_0 = snp_or(start_fp_reg_shift, num_fp_reg_shift) 
    Xs_formation_b_0 = snp_or(Xs_formation_a_0, Xs_3_rec_shift) 

    li x5, Xs_formation_b_0
    

    csrrw  rx1,  tensor_reduce, x5 
    csrrs rx2, tensor_error, rx3
    csrwi tensor_wait, 0x9
    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

@cpu: 0
    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)
    fmadd.s rf1, rf2, rf3, rf4
    ;RANDINST 0:var
    start_fp_reg = snp_urandrange(0,32) 
    start_fp_reg_shift = snp_lshift(start_fp_reg, 57)
    num_fp_reg = snp_urandrange(0,128) 
    num_fp_reg_shift = snp_lshift(num_fp_reg, 16)

    operation = snp_urandrange(0,9) 
    operation_shift = snp_lshift(operation, 24)

    Xs_3_sen_shift = snp_lshift(Xs_3_sen, 3)

    Xs_formation_a_1 = snp_or(start_fp_reg_shift, num_fp_reg_shift) 
    Xs_formation_b_1 = snp_or(Xs_formation_a_1, operation_shift) 
    Xs_formation_c = snp_or(Xs_formation_b_1, Xs_3_sen_shift) 
    Xs_formation_d = snp_or(Xs_formation_c, Xs_0) 
   
    li x6, Xs_formation_d

    csrrw  rx1,  tensor_reduce, x6 
    csrrs rx2, tensor_error, rx3
    csrwi tensor_wait, 0x9
    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)
snip_check
    ; Empty
