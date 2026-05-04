; The snippet initializes the L1SCP with random data using tensor loads
; Next, it clears up the fp registers with a full sized tensor fma16
; Then, it does a random tensor_fma16 in a loop of 10 iterations

import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource
cpu_t   x
;unum_t var = 10
num_t r_tensor_mask = snp_urandrange(0,64) 
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

;gmemory_t   snp_mem_tgt[STR_MEM.MEM_SIZE]@STR_MEM.MEM_ALIGN
base_t  rb1[256]@64

unum_t      off = 128 ;//snp_select_memop_offset(0, STR_MEM.MEM_SIZE, 4, RV_PARAMS.HAS_UNALIGNED)

pfreg_t rf1, rf2, rf3, rf4
axreg_t rx1, rx2, rx3

gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
unum_t  cpuid = snp_cpu_id()

snip_setup
    

snip_init
    ; Empty

snip_run
@cpu: 0,2..BOARD_CONF.NUM_CPUS

    backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)
    li rx2, r_tensor_mask
    csrrw x0, tensor_mask, rx2
    ten_mask_shift = snp_lshift(ten_mask,63)

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

    ; To clear fp registers
    Bcol_shift = snp_lshift(Bcol, 55)
    Arow_shift = snp_lshift(Arow, 51)
    Acol_shift = snp_lshift(Acol, 47)
    Astartcol_shift = snp_lshift(Astartcol, 43)
    Loc_of_B_shift = snp_lshift(Loc_of_B, 20)
    B_start_cache_line_shift = snp_lshift(B_start_cache_line, 12)   
    A_start_cache_line_shift = snp_lshift(A_start_cache_line, 4) 

    S_0 = snp_or(0x0000000000000003, Bcol_shift) 
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

       S_0 = snp_or(0x0000000000000002, Bcol_shift) 
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

    endfor

    restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

snip_check
    ; Empty

