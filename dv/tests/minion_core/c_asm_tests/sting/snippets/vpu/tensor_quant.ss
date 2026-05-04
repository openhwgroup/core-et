; The snippet tests a tensor_quant operation
; Tensor loads are done to set up scratchpad
; A loop of 10 random tensor quants are executed back-to-back

import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource
cpu_t   x

;unum_t var = 10
num_t index
num_t start_reg 
num_t start_reg_shift
num_t Acol 
num_t Acol_shift
num_t Arow 
num_t Arow_shift
 
num_t First_vec_cache_line 
num_t First_vec_cache_line_shift 

num_t txn_0
num_t txn_1
num_t txn_2
num_t txn_3
num_t txn_4
num_t txn_5
num_t txn_6
num_t txn_7
num_t txn_8
num_t txn_9
num_t txn_0_shift
num_t txn_1_shift
num_t txn_2_shift
num_t txn_3_shift
num_t txn_4_shift
num_t txn_5_shift
num_t txn_6_shift
num_t txn_7_shift
num_t txn_8_shift
num_t txn_9_shift

num_t S_0
num_t S_1
num_t S_2
num_t S_3
num_t S_4
num_t S_5
num_t S_6
num_t S_7
num_t S_8
num_t S_9
num_t S_10
num_t S_11
num_t S_12


unum_t      off = 128 ;//snp_select_memop_offset(0, STR_MEM.MEM_SIZE, 4, RV_PARAMS.HAS_UNALIGNED)

pfreg_t rf1, rf2, rf3, rf4
axreg_t rx1, rx2, rx3

base_t  rb1[256]@64

gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
unum_t  cpuid = snp_cpu_id()

snip_setup
    

snip_init

snip_run
@cpu: 0,2..BOARD_CONF.NUM_CPUS


    li     x31, 0x40
    addi   x11, rb1, 0x0

    li     x6, 0xb
    add    x5, x6, x11
    csrrw  x0, tensor_load, x5
    csrwi tensor_wait, 0x0
	
    li x6, 0x018000000000000b
    addi x11, x11, off
    add x5, x6, x11
    csrrw x0, tensor_load, x5 
    csrwi tensor_wait, 0x0

    li x6, 0x030000000000000b
    addi x11, x11, off
    add x5, x6, x11
    csrrw x0, tensor_load, x5 
    csrwi tensor_wait, 0x0

    li x6, 0x048000000000000b
    addi x11, x11, off
    add x5, x6, x11
    csrrw x0, tensor_load, x5
    csrwi tensor_wait, 0x0

    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)

    for index in (0, 10, 1):
       ;RANDINST 0:var
       start_reg = snp_urandrange(0,32)
       start_reg_shift = snp_lshift(start_reg,57)
       Acol = snp_urandrange(0,16)
       Acol_shift = snp_lshift(Acol, 55)
       Arow = snp_urandrange(0,16)
       Arow_shift = snp_lshift(Arow, 51)

       First_vec_cache_line = snp_urandrange(0,64)
       First_vec_cache_line_shift  = snp_lshift(First_vec_cache_line, 45)    

       S_0 = snp_or(start_reg_shift,Acol_shift)
       S_1 = snp_or(S_0,Arow_shift)
       S_2 = snp_or(S_1,First_vec_cache_line_shift)

       txn_0 = snp_urandrange(0,11)
       txn_1 = snp_urandrange(0,11)
       txn_2 = snp_urandrange(0,11)
       txn_3 = snp_urandrange(0,11)
       txn_4 = snp_urandrange(0,11)
       txn_5 = snp_urandrange(0,11)
       txn_6 = snp_urandrange(0,11)
       txn_7 = snp_urandrange(0,11)
       txn_8 = snp_urandrange(0,11)
       txn_9 = snp_urandrange(0,11)
  
       txn_0_shift = snp_lshift(txn_0,0)
       txn_1_shift = snp_lshift(txn_1,4)
       txn_2_shift = snp_lshift(txn_2,8)
       txn_3_shift = snp_lshift(txn_3,12)
       txn_4_shift = snp_lshift(txn_4,16)
       txn_5_shift = snp_lshift(txn_5,20)
       txn_6_shift = snp_lshift(txn_6,24)
       txn_7_shift = snp_lshift(txn_7,28)
       txn_8_shift = snp_lshift(txn_8,32)
       txn_9_shift = snp_lshift(txn_9,36)

       S_3 = snp_or(S_2,txn_0_shift)
       S_4 = snp_or(S_3,txn_1_shift)
       S_5 = snp_or(S_4,txn_2_shift)
       S_6 = snp_or(S_5,txn_3_shift)
       S_7 = snp_or(S_6,txn_4_shift)
       S_8 = snp_or(S_7,txn_5_shift)
       S_9 = snp_or(S_8,txn_6_shift)
       S_10 = snp_or(S_9,txn_7_shift)
       S_11 = snp_or(S_10,txn_8_shift)
       S_12 = snp_or(S_11,txn_9_shift)
  
       li rx2, S_12
       csrrw  rx1,  tensor_quant, rx2     
       ;csrwi tensor_wait, 0xa

   
    endfor

    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

snip_check
    ; Empty

