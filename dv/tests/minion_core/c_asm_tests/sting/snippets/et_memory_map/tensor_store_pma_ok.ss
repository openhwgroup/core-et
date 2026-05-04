
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor_load.pb"
 
; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
cpu_t   x ;0003_cpu_selection_schemes.ss

; Tensor Mask
unum_t  ts_mask_val   = snp_randnum()

; Tensor Load
unum_t  ts_csr_val_rand = snp_randnum()
unum_t  ts_csr_val      = snp_and(ts_csr_val_rand, 0xfe8800000000000f)
unum_t  x31_rand        = snp_urandrange(0,9)
unum_t  x31_stride      = snp_get_ts_stride(ts_csr_val, x31_rand)
unum_t  x31_val         = snp_lshift(x31_stride, 4)

; Tensor Wait
unum_t  ten_wait_en   = snp_urandrange(0,2)

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 2(MAX_STRIDE) = 2048)
memory_t  mem1[2048]@64:ET_TENSOR_LOAD_OK_MEM_SET

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
@cpu: x

if ((x%2) == 0):
   reset_tensor_error()

 if ((ts_csr_val & 0x8000000000000000) != 0):
   ; [Setup] For Tensor Loads, if bit 63 is set, we are using Tensor Mask (PRM-10)
   li   csr_reg, ts_mask_val
   csrw tensor_mask, csr_reg
 endif

   ; Remove x31[47:6], which is used for stride access
   ; Save x31
   addi rx3, x31, 0
   li  x31, x31_val

   ; [CSR] Set the register to random values and keep the ones we need 
   li  csr_reg, ts_csr_val

   ; [CSR] Mask the VA and merge it with the CSR value
   la  rx1, mem1
   li  rx2, 0x0000fffffffffff0
   and rx1, rx2, rx1
   or  csr_reg, csr_reg, rx1

   ; Write to the CSR to trigger the Tensor Load Operation 
   csrrw rx3, tensor_store, csr_reg

   beq    x0, rx3, csr_return_is_0
   test_fail(regz = rx3)


 csr_return_is_0:
   ; Need tensor wait before cheking error
   tensor_store_wait()

 if (ET_TEST_ENV.SELF_CHECK_ENABLE == 1):
   ; mem1 should not be in shire memory, then we should see tensor error
   ; Read Tensor Error CSR
    csrrs rx1, tensor_error, x0

   ; Check Mem access fault
   ; All tensor loads should work in this snippet
   beq  rx1, x0, pass_tensor_store_error

   fail_tensor_store:
     ; if Mprot is different from 0 value an error is expected
     ; currently it will catched and check in RTL monitorization but not in sysemu.
     ; It would be possible to do self check here reading @4 esr verify Mprot value
     ; different from 0 and continue or fail depends on this value.
     li rx2, 0x7f
     and rx2, rx1 ,rx2  ; only bit 8 in tensor error doesn't make fail.
     beq rx2, x0, pass_tensor_store_error     
     test_fail(regz = rx2)
 endif

 pass_tensor_store_error:
   reset_tensor_error()
   addi x31, rx3, 0

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

mhandler@cpu: x
  csrrs rx1, mcause, x0
  test_fail(regz = rx1)

  csrr rx1, mepc
  addi rx1, rx1, 4
  csrw mepc, rx1
