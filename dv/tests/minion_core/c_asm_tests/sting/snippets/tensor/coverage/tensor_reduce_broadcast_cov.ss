
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
 
; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE

; Tensor Reduce (Auto)
; Initial random value
unum_t  tr_val_rand    = snp_randnum()
unum_t  tr_val_gl_rand = snp_randnum_glbl()
unum_t  tr_csr_base    = glbl_snp_tensor_reduce_broadcast(BOARD_CONF.NUM_CPUS, tr_val_rand, tr_val_gl_rand)
unum_t  tr_csr_cov     = cov_tensor_reduce_start_reg(tr_csr_base)
unum_t  tr_csr_val     = glbl_cov_tensor_reduce_num_reg(tr_csr_cov)

unum_t rand_init = snp_randnum_glbl()

unum_t  cpuid = snp_cpu_id()
unum_t  is_reducing = snp_in_reduce(tr_csr_val, cpuid)

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

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
   snippet_start(random_num = rand_init, rega = rx1, regb = rx2)
if (is_reducing == 1):
if ((cpuid%2) == 0):
   reset_tensor_error()
   backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)
endif

   li rx3, tr_csr_val
   csrrw rx1, tensor_reduce, rx3
if ((cpuid%2) == 0):
   beq    x0, rx1, csr_return_is_0_x
   test_fail(regz = rx1)
csr_return_is_0_x:
   tensor_reduce_wait()
   restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)
else:
   test_fail(regz = rx1)
endif
endif

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

snip_handler
mhandler@cpu: *
   ; HART1 will always generate an illegal instruction exception
   ; This type of exception is always handled in Machine mode
   csrrs rx1, mcause, x0

if ((cpuid%2) == 1):
   ; HART1 cannot execute Tensor Operations, so it is normal to trap
   li    rx2, 0x2 ;Illegal Instruction
   beq   rx1, rx2, pass_check_tensor_trap
endif
   ; We received an unexpected trap, we go to fail
   test_fail(regz = rx1)

pass_check_tensor_trap:
   csrr rx1, mepc
   addi rx1, rx1, 16
   csrw mepc, rx1
