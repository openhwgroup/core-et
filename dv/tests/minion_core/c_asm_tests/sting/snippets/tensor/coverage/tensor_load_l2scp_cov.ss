
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
unum_t  cpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)

; Tensor Mask
unum_t  tl_l2scp_mask_val   = glbl_cov_tensor_load_mask(0)

; Tensor Load
unum_t  tl_l2scp_csr_base   = snp_randnum()
unum_t  x31_val_rand        = snp_randnum()
unum_t  x31_val_masked      = snp_and(x31_val_rand,     0xffff00000000003f)
unum_t  mem_stride          = snp_urandrange(0, 3)
unum_t  mem_stride_lsh      = snp_lshift(mem_stride, 6)
unum_t  x31_val             = snp_or(x31_val_masked,    mem_stride_lsh)

; Only access valid L2 SCP Lines
unum_t  tl_l2scp_num_lines  = glbl_cov_tensor_load_lines(tl_l2scp_csr_base, 0)
unum_t  tl_l2scp_csr_val    = snp_set_valid_num_scp_lines(tl_l2scp_num_lines)

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 2(MAX_STRIDE) = 2048)
; SET defined in $MINION_DIAGS/sting/conf/utils/et_minion_shire_mem.conf
memory_t  mem1[2048]@64:ET_MINION_SHIRE_MEM_SET



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

   ; Save x31
   addi rx3, x31, 0

if ((tl_l2scp_csr_val & 0x8000000000000000) != 0):
   ; [Setup] For Tensor Loads, if bit 63 is set, we are using Tensor Mask (PRM-10)
   write_tensor_mask(value = tl_l2scp_mask_val, rega = csr_reg)
endif

   ; [x31] Set stride (bit [47:4])
   ; We do not mask because the other fields are ignored
   li  x31, x31_val

   ; [CSR] Set the register to random values and keep the ones we need
   li  csr_reg, tl_l2scp_csr_val
   li  rx2, 0xffff00000000003f
   and csr_reg, csr_reg, rx2

   ; [CSR] Mask the VA and merge it with the CSR value
   la  rx1, mem1
   li  rx2, 0x0000ffffffffffc0
   and rx1, rx1, rx2
   or  csr_reg, csr_reg, rx1

   ; Write to the CSR to trigger the Tensor Load Operation
   csrrw rx1, tensor_load_l2scp, csr_reg
   beq    x0, rx1, csr_return_is_0

   test_fail(regz = rx1)

csr_return_is_0:
   ; Restore x31
   addi x31, rx3, 0

;
; Similarly the 'snip_check' section is invoked at the end of the test after all
; the hardware threads are done with their random code section. It can be used to
; carry out any test specific checks like verifying the contents of a memory buffer
; against the expected values.
;
snip_check
   ; Empty

