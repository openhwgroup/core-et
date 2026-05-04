
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor_load.pb"
import "$MINION_DIAGS/sting/snippets/sequences/sequences.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
unum_t  cpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)

; Tensor Wait
unum_t ten_wait = 0


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Convolution (tensor_mask)
unum_t conv_size_val = glbl_snp_get_tensor_conv_size()
unum_t conv_ctrl_val = glbl_snp_get_tensor_conv_ctrl(conv_size_val)
axreg_t conv_size_csr
axreg_t conv_ctrl_csr



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Load
unum_t  tl_csr_val_rand = snp_randnum()
unum_t  tl_csr_val_base = snp_and(tl_csr_val_rand, 0xbfef00000000003f)
unum_t  x31_val_rand    = snp_randnum()
unum_t  x31_val_masked  = snp_and(x31_val_rand,     0xffff00000000003f)
unum_t  mem_stride      = snp_urandrange(0, 3)
unum_t  mem_stride_lsh  = snp_lshift(mem_stride, 6)
unum_t  tl_x31_val         = snp_or(x31_val_masked,    mem_stride_lsh)

; Tensor Load Transformation Bias to reduce the number of wrong transformations
unum_t  tl_csr_val = 0
unum_t  tl_csr_valid    = snp_ctl_set_valid_transformation(tl_csr_val_base)
unum_t  tl_csr_invalid  = snp_ctl_set_invalid_transformation(tl_csr_val_base)
unum_t  trans_bias      = snp_urandrange(0, 27)
unum_t  tenb_bias       = snp_urandrange(0, 12)


axreg_t tl_csr
axreg_t tl_x31

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor FMA (TenB)
; Tensor FMA IMA8A32
unum_t  tfma_csr_val = glbl_snp_get_tfma_tenb_value(tl_csr_val_base, 0)

axreg_t tfma_csr


pxreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
pxreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

unum_t tl_off_rand = snp_urandrange(0, 32)
unum_t tl_off = snp_lshift(tl_off_rand, 6) ; max offset of 1984

unum_t l_mid = snp_rshift(cpu_x, 1)
; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 2(MAX_STRIDE) = 2048)
; Tensor Load Transpose 8 require more elements, therefore we have to multiply the size by 4
gmemory_t tl_mem[ET_TEST_ENV.NUM_MINIONS][10240]@64:ET_MINION_SHIRE_MEM_SET ; 8192 + 1984 + 64 = 10240
gmemory_t mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION



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
   ten_wait = 1
else :
   ten_wait = snp_urandrange(0,2)
endif

if (trans_bias == 0):
   tl_csr_val = snp_or(tl_csr_invalid, 0x8000000000000000)
else:
   tl_csr_val = snp_or(tl_csr_valid, 0x8000000000000000)
endif

if (tenb_bias == 0):
   tl_csr_val = snp_or(tl_csr_val, 0x0010000000000000)
endif

if ((cpu_id%2) == 0):
   reset_tensor_error()
endif

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Backup Phase
   backup_x31()
if ((cpu_id%2) == 0):
   if (tenb_bias == 0):
      backup_fp_regs(memory = mem_fp[cpu_x], rega = rx1)
   endif
endif



   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Config Phase

   ; Tensor Convolution
   li   conv_size_csr, conv_size_val
   li   conv_ctrl_csr, conv_ctrl_val

   ; Tensor Load

   li tl_x31, tl_x31_val
   li tl_csr, tl_csr_val
   la  rx1, tl_mem[l_mid]
   addi rx1, rx1, tl_off
   li  rx2, 0x0000ffffffffffc0
   and rx1, rx1, rx2
   or  tl_csr, tl_csr, rx1
if (tenb_bias == 0):
   li tfma_csr, tfma_csr_val
endif

if (tenb_bias != 0):
   ; Tensor Load non-tenb writes into the L1SCP
   pre_writting_l1scp()
else:
   ; Tensor Load tenb does not write the L1SCP
   ; But a TFMA tenb does read the L1SCP
   pre_reading_l1scp()
endif

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase

if (SEQ_ENV.MIX == 0) :
   conv_tl    (pb_cpu_id = cpu_id, pb_rx1 = rx1, pb_tl_x31 = tl_x31, pb_tl_csr = tl_csr, pb_conv_size_csr = conv_size_csr, pb_conv_ctrl_csr = conv_ctrl_csr)
else :
   mix_conv_tl(pb_cpu_id = cpu_id, pb_rx1 = rx1, pb_tl_x31 = tl_x31, pb_tl_csr = tl_csr, pb_conv_size_csr = conv_size_csr, pb_conv_ctrl_csr = conv_ctrl_csr)
endif

if (tenb_bias == 0):
   SELECT 1 {
       1 : clean_tlb_tfma(pb_cpu_id = cpu_id, pb_rx1 = rx1, pb_tl_x31 = tl_x31, pb_tl_csr = tl_csr, pb_tfma_csr = tfma_csr)
      10 : seq_tfma(_cpu_id = cpu_id, _rx1 = rx1, _tfma_csr = tfma_csr, _self_check = 0)
   }
endif


if ((cpu_id%2) == 0 && SEQ_ENV.MIX == 0):
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Self-Check Return is 0

      ; Tensor Load
      bne    x0, tl_x31,   tensor_return_not_0

      j tensor_return_is_0
   tensor_return_not_0:
      test_fail(regz = rx1)
   tensor_return_is_0:

   if (ten_wait != 0):
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Self-Check Tensor Error

      ; Tensor Load
      selfcheck_tensor_load:
         if (tenb_bias != 0): ; Only do wait for non-tenb tensor loads
            tensor_load_wait(x31_reg_value = tl_x31_val)
         endif
         check_tensor_load_error(num_tload_value = tl_csr_val, rega = rx1, regb = tl_x31, reg_csr_val = tl_csr)

         j pass_sequence
      fail_tensor_error:
         test_fail(regz = rx1)
      pass_sequence:
   endif
elif ((cpu_id%2) == 0 && ten_wait != 0):
   tensor_load_wait(x31_reg_value = tl_x31_val)
endif


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Restore Phase
   restore_x31()

if ((cpu_id%2) == 0):
   reset_tensor_error()
   if (tenb_bias == 0):
      restore_fp_regs(memory = mem_fp[cpu_x], rega = rx1)
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
;
snip_handler

mhandler@cpu: cpu_x
   tensor_mhandler(rega = rx1, regb = rx2)
   csrr rx1, mepc
   addi rx1, rx1, 16
   csrw mepc, rx1
