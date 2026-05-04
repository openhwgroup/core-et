
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor_load.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor_fma.pb"
 
; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE

unum_t cpuId = snp_cpu_id()
gnum_t tl_coop_id[BOARD_CONF.NUM_CPUS]
unum_t tmp_coop_id = 0

; ### Tensor Coop Mask ###
; Between all the valid combinations, get a random Neigh Coop Mask
unum_t rand_neigh_mask   = glbl_snp_get_coop_neigh_mask()
; Between all the valid combinations, get a random Minion Coop Mask
unum_t rand_minion_mask  = glbl_snp_get_coop_minion_mask()
; Get a random value for the WARL(0) fields and 'Cooperation ID'
unum_t coop_mask_rand   = snp_randnum_glbl()
; Build the final value to be written into the Tensor Coop CSR
unum_t coop_mask_val    = snp_tensor_mask(BOARD_CONF.NUM_CPUS, coop_mask_rand, rand_neigh_mask, rand_minion_mask)
; ########################


; ### Tensor Coop Mask AllActive ###
; The purpose of this is to set a CTL with all the available minions
;
; Compute the BIAS of how often this happen
unum_t aa_bias = snp_urandrange_glbl(0, 32)
; Get a Neigh Coop Mask with all the active Neighs
unum_t aa_rand_neigh_mask  = snp_get_coop_neigh_mask_all_active()
; Get a Neigh Coop Mask with all the active Minions
unum_t aa_rand_minion_mask = snp_get_coop_minion_mask_all_active()
; Build the final value to be written into the Tensor Coop CSR
unum_t aa_coop_mask_val    = snp_tensor_mask(BOARD_CONF.NUM_CPUS, coop_mask_rand, aa_rand_neigh_mask, aa_rand_minion_mask)
; #############################

unum_t  rand_start    = snp_randnum_glbl()

; ### Tensor Mask ###
unum_t  tl_mask_val   = snp_randnum_glbl()
; ###################


; ### Tensor Load ###
unum_t  tl_csr_val_rand    = snp_randnum_glbl()
unum_t  tl_csr_val_masked  = snp_and(tl_csr_val_rand,  0xbfef00000000003f)
; Set the Cooperative Bit to 1
unum_t  tl_csr_val_tena    = snp_or(tl_csr_val_masked, 0x4000000000000000)
; ###################

; ### Tensor Load tenb ###
unum_t  tenb_bias          = snp_urandrange_glbl(0, 12)
unum_t  tl_csr_tmp_tenb    = snp_or(tl_csr_val_masked, 0x0010000000000000)
; Set the Cooperative Bit to 1
unum_t  tl_csr_val_tenb    = snp_or(tl_csr_tmp_tenb,   0x4000000000000000)
; ########################

; ### Tensor FMA IMA8A32 ###
unum_t  tfma_csr_value = glbl_snp_get_tfma_tenb_value(tl_csr_val_tenb, 1)
; ##########################

; Tensor Load Transformation Bias to reduce the number of wrong transformations
unum_t  tl_csr_val = 0
unum_t  tl_csr_valid    = snp_ctl_set_valid_transformation(tl_csr_val_tena)
unum_t  tl_csr_invalid  = snp_ctl_set_invalid_transformation(tl_csr_val_tena)
unum_t  trans_bias      = snp_urandrange_glbl(0, 12)
unum_t  th1_bias        = snp_urandrange(0, 27)


; ### Stride and ID ###
unum_t  x31_val_rand       = snp_randnum_glbl()
unum_t  x31_val_masked     = snp_and(x31_val_rand,     0xffff00000000003f)
unum_t  mem_stride         = snp_urandrange_glbl(0, 5)
unum_t  mem_stride_shifted = snp_lshift(mem_stride, 6)
unum_t  x31_val            = snp_or(x31_val_masked,    mem_stride_shifted)
; ####################


; ### Tensor Wait ###
unum_t ten_wait_en = 0
; ###################

unum_t  in_coop_mask = 0

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 4(MAX_STRIDE) = 4096)
; Tensor Load Transpose 8 require more elements, therefore we have to multiply the size by 4
gmemory_t  ctl_mem2[16384]@64:ET_MINION_SHIRE_MEM_SET
gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION



;
; The 'snip_setup' section is invoked only once in the entire duration of the
; image execution during the kernel setup. Essential setup tasks are carried
; out in this stage. For example, device driver snippets can make use of this
; stage to carry out device initialization
;
snip_setup ;## STING PHASE
    ; Empty



;
; The 'snip_init' section is invoked at the starting of the test and is used to
; carry out test specific initializations like setting up the initial values
; in the memory buffers which will be accessed in the test.
;
snip_init ;## STING PHASE
   ; Empty
  


;
; The 'snip_run' section gets rendered inside the random code section depending
; on the bias specified for this particular snippet.
;
snip_run ;## STING PHASE
@cpu: *
if (ET_TEST_ENV.STING_RAND == 1) :
   ten_wait_en = 1
else :
   ten_wait_en = snp_urandrange(0,2)
endif

   tmp_coop_id   = snp_and(tl_coop_id[cpuId], 0x01f)

if (aa_bias == 0):
   coop_mask_val = snp_or(aa_coop_mask_val, tmp_coop_id)
else:
   coop_mask_val = snp_or(coop_mask_val, tmp_coop_id)
endif

   snippet_start(random_num = rand_start, rega = rx1, regb = rx2)

if (tenb_bias == 0):
   tl_csr_val     = tl_csr_val_tenb
if ((cpuId%2) == 0):
   backup_fp_regs(memory = mem_fp[cpuId], rega = rx1)
   tensor_fma_wait()
endif
else:
if (trans_bias == 0):
   tl_csr_val = tl_csr_invalid
else:
   tl_csr_val = tl_csr_valid
endif
endif

in_coop_mask = snp_cpu_in_mask(cpuId, coop_mask_val)
if ((in_coop_mask != 0) | ((cpuId%2 == 1) & (th1_bias == 0))):

if ((cpuId%2) == 0):
   reset_tensor_error()
endif

   ; Save x31
   addi rx3, x31, 0

if (((tl_csr_val & 0x8000000000000000) != 0) | (((tfma_csr_value & 0x8000000000000000) != 0) & (tenb_bias == 0))):
   ; [Setup] For Tensor Loads, if bit 63 is set, we are using Tensor Mask (PRM-10)
   ; Also for TFMA if it is using the mask, we have to set it too
   write_tensor_mask(value = tl_mask_val, rega = csr_reg)
endif
   li rx1, coop_mask_val
   csrw tensor_cooperation, rx1

if ((cpuId%2) == 1):
   ; Odd HARTS should trap and skip this test_fail()
   test_fail(regz = rx1)
endif

   tensor_load_snip_run(num_x31_val = x31_val, csr_val = tl_csr_val, mem = ctl_mem2, addr_off = 0, trega = rx1, tregb = rx2, tregc = rx3)
if (tenb_bias == 0):
   tensor_fma_base_snip_run(csr_reg_a = rx1, regx_a = rx2, regx_b = rx3, csr_val = tfma_csr_value, tfma_wait = 1)
if ((cpuId%2) == 0):
   restore_fp_regs(memory = mem_fp[cpuId], rega = rx1)
endif
endif
endif
   tl_coop_id[cpuId] = snp_add(tl_coop_id[cpuId], 1)

; Similarly the 'snip_check' section is invoked at the end of the test after all
; the hardware threads are done with their random code section. It can be used to
; carry out any test specific checks like verifying the contents of a memory buffer
; against the expected values.
;
snip_check
   ; Empty



; The notation given below allows user to register handlers which will
; either run in machine mode (mhandler), or in supervisor mode (shandler)
; when trap delegation is enabled. In both the cases the exception program
; counter is read and incremented by 4 so as to allow the execution to proceed
; from the program counter following the instruction which caused the exception.
;
snip_handler

mhandler@cpu: *
   tensor_coop_mhandler(regx1 = rx1, regx2 = rx2, is_coop = 1)
   csrr rx1, mepc
   addi rx1, rx1, 16
   csrw mepc, rx1
