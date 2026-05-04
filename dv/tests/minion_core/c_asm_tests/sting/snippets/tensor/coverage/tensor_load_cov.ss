
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor_load.pb"
 
; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
unum_t  cpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)

; Tensor Mask
unum_t  tl_mask_val     = glbl_cov_tensor_load_mask(0)

; Tensor Load
unum_t  tl_csr_val_rand = snp_randnum()
unum_t  tl_csr_val_mask = snp_and(tl_csr_val_rand, 0xbfef00000000003f)
unum_t  tl_csr_val_base = glbl_cov_tensor_load_lines(tl_csr_val_mask, 0)
unum_t  x31_val_rand    = snp_randnum()
unum_t  x31_val_masked  = snp_and(x31_val_rand,     0xffff00000000003f)
unum_t  mem_stride      = snp_urandrange(0, 3)
unum_t  mem_stride_lsh  = snp_lshift(mem_stride, 6)
unum_t  x31_val         = snp_or(x31_val_masked,    mem_stride_lsh)

; Tensor Load Transformation Bias to reduce the number of wrong transformations
unum_t  tl_csr_val      = 0
unum_t  tl_csr_valid    = snp_ctl_set_valid_transformation(tl_csr_val_base)
unum_t  tl_csr_invalid  = snp_ctl_set_invalid_transformation(tl_csr_val_base)
unum_t  trans_bias      = snp_urandrange(0, 27)

; Tensor Wait
unum_t ten_wait_en      = 0

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 2(MAX_STRIDE) = 2048)
; Tensor Load Transpose 8 require more elements, therefore we have to multiply the size by 4
memory_t  mem1[8192]@64:ET_MINION_SHIRE_MEM_SET



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
@cpu: cpu_x
if (ET_TEST_ENV.STING_RAND == 1) :
ten_wait_en = 1
else :
ten_wait_en = snp_urandrange(0,2)
endif

if (trans_bias == 0):
   tl_csr_val = tl_csr_invalid
else:
   tl_csr_val = tl_csr_valid
endif

if ((cpu_id%2) == 0):
   reset_tensor_error()
endif

   ; Save x31
   addi rx3, x31, 0

if ((tl_csr_val & 0x8000000000000000) != 0):
   ; [Setup] For Tensor Loads, if bit 63 is set, we are using Tensor Mask (PRM-10)
   write_tensor_mask(value = tl_mask_val, rega = csr_reg)
endif

   tensor_load_snip_run(num_x31_val = x31_val, csr_val = tl_csr_val, mem = mem1, addr_off = 0, trega = rx1, tregb = rx2, tregc = rx3)

;
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

mhandler@cpu: cpu_x
   tensor_mhandler(rega = rx1, regb = rx2)
   csrr rx1, mepc
   addi rx1, rx1, 16
   csrw mepc, rx1
