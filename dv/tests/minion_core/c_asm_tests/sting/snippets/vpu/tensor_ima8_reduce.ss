; The snippet initializes the L1SCP with random data using tensor loads
; Next, it clears up the tenC register with a full sized tensor ima8
; It does a random ima8 followed by a tensor reduce in a loop of 10 iterations

import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"


resource
cpu_t   x
;unum_t var = 10
num_t r_tensor_mask = snp_urandrange(0,64) 
num_t ten_mask = snp_urandrange(0,2) 
unum_t ten_mask_shift
unum_t var = 10
num_t index
num_t Arow = 15
num_t Arow_shift
num_t Bcol = 3
num_t Bcol_shift
num_t Acol = 15
num_t Acol_shift 
num_t Astartcol = 0
num_t Astartcol_shift
num_t Loc_of_res = 0
num_t Loc_of_res_shift 
num_t TenBsign = 0
num_t TenBsign_shift  
num_t TenAsign = 0
num_t TenAsign_shift 
num_t Loc_of_B = 0
num_t Loc_of_B_shift 
num_t B_start_cache_line = 0 
num_t B_start_cache_line_shift    
num_t A_start_cache_line = 0
num_t A_start_cache_line_shift  
num_t TenC_clear = 1
unum_t       cntr


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


;gmemory_t   snp_mem_tgt_ima8[STR_MEM.MEM_SIZE]@STR_MEM.MEM_ALIGN

base_t  rb1[256]@64

unum_t      off = 128 ;snp_select_memop_offset(0, STR_MEM.MEM_SIZE, 4, RV_PARAMS.HAS_UNALIGNED)

pfreg_t rf1, rf2, rf3, rf4
axreg_t rx1, rx2, rx3, rx4, rx5, rx6

gmemory_t mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
unum_t  cpuid = snp_cpu_id()

snip_setup

    

snip_init
    ; Empty

snip_run
@cpu: 1,3..BOARD_CONF.NUM_CPUS
    for index in (0, 10, 1):
       num_fp_reg = snp_urandrange_glbl(0,128)
       start_fp_reg = snp_urandrange_glbl(0,32)
       add x0, x0, x0
    endfor

@cpu: 0,2..BOARD_CONF.NUM_CPUS

    ;sd  x1, 0 (rb1)
    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)

    li rx2, r_tensor_mask
    csrrw x0, tensor_mask, rx2
    ten_mask_shift = 0

    li     x31, 0x40
    addi   x11, rb1, 0x0
   
    li     x7, ten_mask_shift
    li     x6, 0xb
    add    x5, x6, x11
    add   x5, x5, x7
    csrrw  x0, tensor_load, x5
    csrwi tensor_wait, 0x0
	
    li x6, 0x018000000000000b
    addi x11, x11, off
    add x5, x6, x11
    add   x5, x5, x7
    csrrw x0, tensor_load, x5 
    csrwi tensor_wait, 0x0

    li x6, 0x030000000000000b
    addi x11, x11, off
    add x5, x6, x11
    add   x5, x5, x7
    csrrw x0, tensor_load, x5 
    csrwi tensor_wait, 0x0

    li x6, 0x048000000000000b
    addi x11, x11, off
    add x5, x6, x11
    add   x5, x5, x7
    csrrw x0, tensor_load, x5
    csrwi tensor_wait, 0x0


    ; To set TenC with max matrix config
    Bcol_shift = snp_lshift(Bcol, 55)
    Arow_shift = snp_lshift(Arow, 51)
    Acol_shift = snp_lshift(Acol, 47)
    Astartcol_shift = snp_lshift(Astartcol, 43)
    Loc_of_res_shift = snp_lshift(Loc_of_res, 23)
    TenBsign_shift = snp_lshift(TenBsign, 22)  
    TenAsign_shift = snp_lshift(TenAsign, 21)
    Loc_of_B_shift = snp_lshift(Loc_of_B, 20)
    B_start_cache_line_shift = snp_lshift(B_start_cache_line, 12)   
    A_start_cache_line_shift = snp_lshift(A_start_cache_line, 4) 

    S_0 = snp_or(0x0000000000000006, Bcol_shift) 
    S_1 = snp_or(S_0, Arow_shift) 
    S_2 = snp_or(S_1, Acol_shift) 
    S_3 = snp_or(S_2, Astartcol_shift) 
    S_4 = snp_or(S_3, Loc_of_res_shift) 
    S_5 = snp_or(S_4, TenBsign_shift) 
    S_6 = snp_or(S_5, TenAsign_shift) 
    S_7 = snp_or(S_6, Loc_of_B_shift) 
    S_8 = snp_or(S_7, B_start_cache_line_shift) 
    S_9 = snp_or(S_8, A_start_cache_line_shift) 
    S_10 = snp_or(S_9, TenC_clear) 
    S_11 = snp_or(S_10, ten_mask_shift) 

    li x5, S_11
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
       Loc_of_res = snp_urandrange(0,2)
       Loc_of_res_shift = snp_lshift(Loc_of_res, 23)
       TenBsign = snp_urandrange(0,2)
       TenBsign_shift = snp_lshift(TenBsign, 22)  
       TenAsign = snp_urandrange(0,2)
       TenAsign_shift = snp_lshift(TenAsign, 21)
       ;Loc_of_B = snp_urandrange(0,2)
       Loc_of_B_shift = snp_lshift(Loc_of_B, 20)
       B_start_cache_line = snp_urandrange(0,256)
       B_start_cache_line_shift = snp_lshift(B_start_cache_line, 12)   
       A_start_cache_line = snp_urandrange(0,256)
       A_start_cache_line_shift = snp_lshift(A_start_cache_line, 4) 
       TenC_clear = snp_urandrange(0,2)
       ten_mask_shift = snp_lshift(ten_mask,63)
   
       S_0 = snp_or(0x0000000000000006, Bcol_shift) 
       S_1 = snp_or(S_0, Arow_shift) 
       S_2 = snp_or(S_1, Acol_shift) 
       S_3 = snp_or(S_2, Astartcol_shift) 
       S_4 = snp_or(S_3, Loc_of_res_shift) 
       S_5 = snp_or(S_4, TenBsign_shift) 
       S_6 = snp_or(S_5, TenAsign_shift) 
       S_7 = snp_or(S_6, Loc_of_B_shift) 
       S_8 = snp_or(S_7, B_start_cache_line_shift) 
       S_9 = snp_or(S_8, A_start_cache_line_shift) 
       S_10 = snp_or(S_9, TenC_clear) 
       S_11 = snp_or(S_10, ten_mask_shift) 

       li x5, S_11
       csrrw  rx1,  tensor_fma, x5 
       if Loc_of_res == 0:
          csrwi tensor_wait, 0x7
       endif

       num_fp_reg = snp_urandrange_glbl(0,128) 
       operation_control = snp_urandrange(0,7) 
       start_fp_reg = snp_urandrange_glbl(0,32)

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
       
    endfor
 
    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

snip_check
    ; Empty

