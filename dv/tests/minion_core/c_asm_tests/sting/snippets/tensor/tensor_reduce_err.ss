
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
 
; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
unum_t  lcpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)
unum_t  cpu_x  = snp_get_phycpu(lcpu_x)

; Tensor Reduce
; Initial random value
unum_t  tr_csr_val_rand = snp_randnum()
; Mask to only match the Send/Receive operation
unum_t  tr_csr_val_op   = snp_and(tr_csr_val_rand, 0xfffffffffffffffd) 
; Mask to leave the Sender/Receiver MinionID as 0
unum_t  tr_csr_val_id   = snp_and(tr_csr_val_op,   0xffffffffffff0004)

unum_t  tr_csr_val = snp_tensor_reduce_err(tr_csr_val_id)

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)



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
@cpu: lcpu_x
   reset_tensor_error()

   li rx3, tr_csr_val
   csrrw rx1, tensor_reduce, rx3
if ((cpu_x % 2) == 0):
   beq    x0, rx1, csr_return_is_0
   test_fail(regz = rx1)

csr_return_is_0:
   tensor_reduce_wait()
   check_tensor_reduce_error(rega = rx1, regb = rx2, regc = rx3, same_minion = 1, fail_reduce = fail_tr_error, pass_reduce = pass_tr_error)
fail_tr_error:
   test_fail(regz = rx1)
pass_tr_error:
   reset_tensor_error()
else:
   test_fail(regz = rx1)
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
;
snip_handler
mhandler@cpu: lcpu_x
   ; HART1 will always generate an illegal instruction exception
   ; This type of exception is always handled in Machine mode
   csrrs rx1, mcause, x0

if ((cpu_x%2) == 1):
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
