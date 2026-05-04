; The snippet initializes the L1SCP with random data using tensor loads
; Next, it clears up the fp registers with a full sized tensor fma32
; Then, it does a random tensor_fma32, followed by randomizing r_gen_reduce
; Note that tensor load B is not used
; If r_gen_reduce is set a random tensor_reduce is done
; Then it radomizes r_gen_quant, and it that is set, does a random tensor_quant
; It repeats the sequence of fma32, optional reduce, optional quant in a loop of 10 

import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource
cpu_t   x
;unum_t var = 10
num_t r_tensor_mask
num_t ten_mask = snp_urandrange(0,2) 
unum_t ten_mask_shift
unum_t var = 10
num_t index
num_t Bcol = 3
num_t Bcol_shift
num_t Arow = 15
num_t Arow_shift
num_t Acol = 15
num_t Acol_shift 
num_t Astartcol = 0
num_t Astartcol_shift

num_t Loc_of_B = 0
num_t Loc_of_B_shift 
num_t B_start_cache_line = 0 
num_t B_start_cache_line_shift    
num_t A_start_cache_line = 0
num_t A_start_cache_line_shift  

num_t clear_src_fp

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

; reduce
num_t num_fp_reg 
num_t operation_control
num_t start_fp_reg
num_t start_fp_reg_shift
num_t num_fp_reg_shift
num_t tree_depth = 0
num_t tree_depth_shift

num_t Xs_10 = 3
num_t operation
num_t operation_shift

num_t Xs_formation_a
num_t Xs_formation_b
num_t Xs_formation_c
num_t Xs_formation_d

; Quant
num_t start_reg 
num_t start_reg_shift
num_t Acol_q 
num_t Acol_q_shift
num_t Arow_q 
num_t Arow_q_shift
 
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

num_t T_0
num_t T_1
num_t T_2
num_t T_3
num_t T_4
num_t T_5
num_t T_6
num_t T_7
num_t T_8
num_t T_9
num_t T_10
num_t T_11
num_t T_12

num_t r_gen_reduce
num_t r_gen_quant
num_t r_rand_tensor_mask
num_t r_tensor_mask_pos_of_one
unum_t sixteen_ones = 65535

base_t  rb1[256]@64

unum_t      off = 128 

pfreg_t rf1, rf2, rf3, rf4
axreg_t rx1, rx2, rx3

gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
unum_t  cpuid = snp_cpu_id()

snip_setup

snip_init
    ; Empty

snip_run
@cpu: 1,3..BOARD_CONF.NUM_CPUS
    for index in (0, 10, 1):
       r_gen_reduce = snp_urandrange_glbl(0,2)

       if r_gen_reduce == 1:
          num_fp_reg = snp_urandrange_glbl(0,128)
          start_fp_reg = snp_urandrange_glbl(0,32)
          add x0, x0, x0
       endif
       add x0, x0, x0
    endfor

@cpu: 0,2..BOARD_CONF.NUM_CPUS

    ; If the snippet is rendered again, the tensor reduce that was from the previous renderation 
    ; should not affect the tensor load 
    if r_gen_reduce == 1:
          csrwi tensor_wait, 0x9
    endif

    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)

    r_rand_tensor_mask = snp_urandrange(0,2) 
    if r_rand_tensor_mask == 1:
      r_tensor_mask = snp_urandrange(0,64) 
    else:
      r_tensor_mask_pos_of_one = snp_urandrange(0,17)
      r_tensor_mask = snp_lshift(sixteen_ones, r_tensor_mask_pos_of_one)
    endif

    li rx2, r_tensor_mask
    csrrw x0, tensor_mask, rx2
    ten_mask_shift = 0 ;snp_lshift(ten_mask,63)

    li     x31, 0x40
    addi   x11, rb1, 0x0

    li  x7, ten_mask_shift
    li  x6, 0xb
    add x5, x6, x11
    add x5, x5, x7
    csrrw  x0, tensor_load, x5
    csrwi tensor_wait, 0x0
	
    li   x6, 0x018000000000000b
    addi x11, x11, off
    add  x5, x6, x11
    add x5, x5, x7
    csrrw x0, tensor_load, x5 
    csrwi tensor_wait, 0x0

    li x6, 0x030000000000000b
    addi x11, x11, off
    add x5, x6, x11
    add x5, x5, x7
    csrrw x0, tensor_load, x5 
    csrwi tensor_wait, 0x0

    li x6, 0x048000000000000b
    addi x11, x11, off
    add x5, x6, x11
    add   x5, x5, x7
    csrrw x0, tensor_load, x5
    csrwi tensor_wait, 0x0

    ; To clear fp registers
    Bcol_shift = snp_lshift(Bcol, 55)
    Arow_shift = snp_lshift(Arow, 51)
    Acol_shift = snp_lshift(Acol, 47)
    Astartcol_shift = snp_lshift(Astartcol, 43)
    Loc_of_B_shift = snp_lshift(Loc_of_B, 20)
    B_start_cache_line_shift = snp_lshift(B_start_cache_line, 12)   
    A_start_cache_line_shift = snp_lshift(A_start_cache_line, 4) 

    S_0 = snp_or(0x0000000000000001, Bcol_shift) 
    S_1 = snp_or(S_0, Arow_shift) 
    S_2 = snp_or(S_1, Acol_shift) 
    S_3 = snp_or(S_2, ten_mask_shift) 
      
    li x5, S_3
    csrrw  rx1,  tensor_fma, x5 


    for index in (0, 10, 1):
       ;RANDINST 0:var
       Bcol = snp_urandrange(0,4)
       Bcol_shift = snp_lshift(Bcol, 55)
       Arow = snp_urandrange(0,16)
       Arow_shift = snp_lshift(Arow, 51)
       Acol = snp_urandrange(0,16)
       Acol_shift = snp_lshift(Acol, 47)
       Astartcol = snp_urandrange(0,16)
       Astartcol_shift = snp_lshift(Astartcol, 43)
       ;Loc_of_B = snp_urandrange(0,2)
       Loc_of_B_shift = snp_lshift(Loc_of_B, 20)

       B_start_cache_line = snp_urandrange(0,256)
       B_start_cache_line_shift = snp_lshift(B_start_cache_line, 12)   
       A_start_cache_line = snp_urandrange(0,256)
       A_start_cache_line_shift = snp_lshift(A_start_cache_line, 4) 
       clear_src_fp = snp_urandrange(0,2)
       ten_mask_shift = snp_lshift(ten_mask,63)

       S_0 = snp_or(0x0000000000000000, Bcol_shift) 
       S_1 = snp_or(S_0, Arow_shift) 
       S_2 = snp_or(S_1, Acol_shift) 
       S_3 = snp_or(S_2, Astartcol_shift) 
       S_4 = snp_or(S_3, Loc_of_B_shift) 
       S_5 = snp_or(S_4, B_start_cache_line_shift) 
       S_6 = snp_or(S_5, A_start_cache_line_shift) 
       S_7 = snp_or(S_6, clear_src_fp) 
        

       S_8 = snp_or(S_7, ten_mask_shift) 
       li x5, S_8

       csrrw  rx1,  tensor_fma, x5 
       ;csrwi tensor_wait, 0x7

       r_gen_reduce = snp_urandrange_glbl(0,2)

       if r_gen_reduce == 1:
      
          num_fp_reg = snp_urandrange_glbl(0,128) 
          start_fp_reg = snp_urandrange_glbl(0,32) 
          operation_control = snp_urandrange(0,7)

          ; reduce
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
          ;csrwi tensor_wait, 0x9

       endif


       r_gen_quant = snp_urandrange(0,2)

       if r_gen_quant == 1:

          ; Tensor quant
          start_reg = snp_urandrange(0,32)
          start_reg_shift = snp_lshift(start_reg,57)
          Acol_q = snp_urandrange(0,16)
          Acol_q_shift = snp_lshift(Acol_q, 55)
          Arow_q = snp_urandrange(0,16)
          Arow_q_shift = snp_lshift(Arow_q, 51)

          First_vec_cache_line = snp_urandrange(0,64)
          First_vec_cache_line_shift  = snp_lshift(First_vec_cache_line, 45)    

          T_0 = snp_or(start_reg_shift,Acol_q_shift)
          T_1 = snp_or(T_0,Arow_q_shift)
          T_2 = snp_or(T_1,First_vec_cache_line_shift)

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
 
          T_3 = snp_or(T_2,txn_0_shift)
          T_4 = snp_or(T_3,txn_1_shift)
          T_5 = snp_or(T_4,txn_2_shift)
          T_6 = snp_or(T_5,txn_3_shift)
          T_7 = snp_or(T_6,txn_4_shift)
          T_8 = snp_or(T_7,txn_5_shift)
          T_9 = snp_or(T_8,txn_6_shift)
          T_10 = snp_or(T_9,txn_7_shift)
          T_11 = snp_or(T_10,txn_8_shift)
          T_12 = snp_or(T_11,txn_9_shift)
  
          li rx2, T_12
          csrrw  rx1,  tensor_quant, rx2

       endif 

       
    endfor



    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

snip_check
    ; Empty

