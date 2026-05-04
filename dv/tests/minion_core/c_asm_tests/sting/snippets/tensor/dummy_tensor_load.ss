
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor_load.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
unum_t cpuid = snp_cpu_id()
unum_t ten_wait_en = 1

; Tensor Mask
unum_t  tl_mask_val     = 0

; Tensor Load
unum_t  mem_stride      = snp_urandrange(0, 3)
unum_t  mem_stride_lsh  = snp_lshift(mem_stride, 6)
unum_t  x31_val         = 0

; Tensor Load No Transformation and using tensor_mask
unum_t  tl_csr_val = 0x8000000000000000

axreg_t csr_reg
axreg_t rx1
axreg_t rx2
axreg_t rx3
axreg_t rx4

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 2(MAX_STRIDE) = 2048)
; Tensor Load Transpose 8 require more elements, therefore we have to multiply the size by 4
memory_t  mem1[BOARD_CONF.NUM_CPUS][64]@64:ET_MINION_SHIRE_MEM_SET


; Evict SW
unum_t  evict_csr_val  = snp_randnum()
unum_t  l1_scp_csr_val = snp_randnum()


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
@cpu: 0,2..BOARD_CONF.NUM_CPUS

   reset_tensor_error()

dummy_tensor_load:
   ; Save x31
   addi rx3, x31, 0

   set_tensor_mask(value = tl_mask_val, rega = csr_reg)
   tensor_load_snip_run(num_x31_val = x31_val, csr_val = tl_csr_val, mem = mem1[cpuid], addr_off = 0, trega = rx1, tregb = rx2, tregc = rx3)

   csrrs rx1, tensor_error, x0
   li rx2, 0x10 ; tensor_error bits [4:4] : L1 scratchpad disabled when operation requires it
   and rx1, rx1, rx2
   bne rx1, rx2, l1_scp_is_enabled

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; ENABLE L1 SCP LOGIC
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
   csrrs rx4, mcache_control, x0
   enable_l1scp(regx1 = rx1, regx2 = rx2, en_l1scp_pass_label = snip_l1scp_pass)
snip_l1scp_pass:
   tensor_load_snip_run(num_x31_val = x31_val, csr_val = tl_csr_val, mem = mem1[cpuid], addr_off = 0, trega = rx1, tregb = rx2, tregc = rx3)
   restore_dcache_mode(prev_val_reg = rx4, regx1 = rx1, regx2 = rx2)

else: ; Call mhandler
   et_jump_to_m_mode()
endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

l1_scp_is_enabled:
   reset_tensor_error()


;
; Similarly the 'snip_check' section is invoked at the end of the test after all
; the hardware threads are done with their random code section. It can be used to
; carry out any test specific checks like verifying the contents of a memory buffer
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

mhandler@cpu: 0,2..BOARD_CONF.NUM_CPUS

   csrrs rx4, mcache_control, x0
   enable_l1scp(regx1 = rx1, regx2 = rx2, en_l1scp_pass_label = exit_handler)
   tensor_load_snip_run(num_x31_val = x31_val, csr_val = tl_csr_val, mem = mem1[cpuid], addr_off = 0, trega = rx1, tregb = rx2, tregc = rx3)
   restore_dcache_mode(prev_val_reg = rx4, regx1 = rx1, regx2 = rx2)

exit_handler:
    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1
