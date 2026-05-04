
import "$MINION_DIAGS/sting/snippets/tensor/tensor_store.pb"
import "$MINION_DIAGS/sting/snippets/sequences/sequences.pb"
 
; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
unum_t  cpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)
unum_t  cpuid = snp_cpu_id()

unum_t ten_wait = 0


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Store
unum_t  ts_rand_init    = snp_urandrange(0, 32)
; Tensor Store Transformation Bias to reduce the number of wrong transformations
unum_t  ts_config_bias  = snp_urandrange(0, 27)

unum_t  ts_csr_val_rand = snp_randnum()
unum_t  ts_csr_val_base = snp_and(ts_csr_val_rand, 0xfff800000000000f)
unum_t  ts_csr_valid    = snp_cts_set_valid_config(ts_csr_val_base, ts_rand_init)
unum_t  ts_csr_invalid  = snp_cts_set_invalid_config(ts_csr_val_base, ts_rand_init)
unum_t  ts_csr_val      = snp_if_zero_else(ts_config_bias, ts_csr_invalid, ts_csr_valid)
unum_t  ts_fregs_mask   = get_tstore_ow_unpr_fregs(ts_csr_val)

unum_t  ts_x31_rand     = snp_urandrange(1,5)
unum_t  ts_x31_val      = snp_get_ts_stride(ts_csr_val, ts_x31_rand)

axreg_t ts_x31
axreg_t ts_csr


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Load TenB
unum_t  tensor_mask_val   = snp_randnum()
unum_t  tl_csr_val_rand   = snp_randnum()
unum_t  tl_csr_val_masked = snp_and(tl_csr_val_rand,  0xbfef00000000003f)
; Set the Tensor Load B bit
unum_t  tl_csr_base       = snp_or(tl_csr_val_masked, 0x0010000000000000)
; Tensor Load Transformation Bias to reduce the number of wrong transformations
unum_t  tl_trans_bias        = snp_urandrange(0, 27)
unum_t  tl_csr_valid      = snp_ctl_set_valid_transformation(tl_csr_base)
unum_t  tl_csr_invalid    = snp_ctl_set_invalid_transformation(tl_csr_base)
unum_t  tl_csr_val        = snp_if_zero_else(tl_trans_bias, tl_csr_invalid, tl_csr_valid)

unum_t  tl_x31_rand       = snp_randnum()
unum_t  tl_x31_masked     = snp_and(tl_x31_rand,     0xffff00000000003f)
unum_t  tl_mem_stride     = snp_urandrange(0, 3)
unum_t  tl_mem_stride_lsh = snp_lshift(tl_mem_stride, 6)
unum_t  tl_x31_val        = snp_or(tl_x31_masked,    tl_mem_stride_lsh)

axreg_t tl_x31
axreg_t tl_csr


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor FMA (TenB)
; Tensor FMA IMA8A32
unum_t  tfma_rand = snp_randnum()
unum_t  tfma_csr_val = glbl_snp_get_tfma_tenb_value(tl_csr_val, 0)
unum_t  tfma_fregs_mask = get_tfma_ow_unpr_fregs(tfma_csr_val)

unum_t  ow_fregs = snp_or(ts_fregs_mask, tfma_fregs_mask)

axreg_t tfma_csr


pxreg_t rx1
pxreg_t rx2

unum_t ts_off_rand = snp_urandrange(0, 32)
unum_t ts_off = snp_lshift(ts_off_rand, 6) ; max offset of 1984

unum_t tl_off_rand = snp_urandrange(0, 32)
unum_t tl_off = snp_lshift(tl_off_rand, 6) ; max offset of 1984

unum_t l_mid = snp_rshift(cpuid, 1)
gmemory_t ts_mem[ET_TEST_ENV.NUM_MINIONS][6592]@64:ET_MINION_SHIRE_MEM_SET ; 4544 + 1984 + 64 = 6592
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

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Backup Phase
   backup_x31()
if ((cpu_id%2) == 0):
   backup_fp_regs(memory = mem_fp[cpu_x], rega = rx1)
endif
   ow_unpred_fregs(fregs_mask = ow_fregs)


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Config Phase

   ; Tensor Store
   li ts_x31, ts_x31_val
   li ts_csr, ts_csr_val
   la rx1, ts_mem[l_mid]
   addi rx1, rx1, ts_off
   li rx2, 0x0000fffffffffff0
   and rx1, rx1, rx2
   or  ts_csr, ts_csr, rx1

   ; Tensor Load B
if (((tl_csr_val & 0x8000000000000000) != 0) | ((tfma_csr_val & 0x8000000000000000) != 0)):
   ; [Setup] For Tensor Loads, if bit 63 is set, we are using Tensor Mask (PRM-10)
   set_tensor_mask(value = tensor_mask_val, rega = tl_csr)
endif
   li tl_x31, tl_x31_val
   li tl_csr, tl_csr_val
   la  rx1, tl_mem[l_mid]
   addi rx1, rx1, tl_off
   li  rx2, 0x0000ffffffffffc0
   and rx1, rx1, rx2
   or  tl_csr, tl_csr, rx1

   ; Tensor FMA IMA8A32
   li tfma_csr, tfma_csr_val

   ; Before a Tensor Load TenB, you have to make sure there is no other TFMA (non-tenb) pending
   pre_writting_l1scp()
   ; Before a Tensor FMA, you have to make sure there is no other Tensor Load (non-tenb) pending
   pre_reading_l1scp()


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase

if (SEQ_ENV.MIX == 0) :
   ; Execute the sequence TS -> TL -> TFMA
   ts_tlb_tfma(pb_cpu_id = cpu_id, pb_rx1 = rx1, pb_ts_x31 = ts_x31, pb_ts_csr = ts_csr, pb_tl_x31 = tl_x31, pb_tl_csr = tl_csr, pb_tfma_csr = tfma_csr)
else :
   ; Mix the sequence. The snippet will contain any combination of TS, TL, TFMA
   ; Including repetitions of the same operation
   mix_ts_tlb_tfma(pb_cpu_id = cpu_id, pb_rx1 = rx1, pb_ts_x31 = ts_x31, pb_ts_csr = ts_csr, pb_tl_x31 = tl_x31, pb_tl_csr = tl_csr, pb_tfma_csr = tfma_csr)
   clean_tlb_tfma(pb_cpu_id = cpu_id, pb_rx1 = rx1, pb_tl_x31 = tl_x31, pb_tl_csr = tl_csr, pb_tfma_csr = tfma_csr)
   if (ten_wait == 1):
      tensor_store_wait()
      tensor_fma_wait()
   endif
endif


if ((cpu_id%2) == 0 && SEQ_ENV.MIX == 0):
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Self-Check Return is 0
   
      ; Tensor Store
      bne    x0, ts_x31,   tensor_return_not_0
      ; Tensor Load B
      bne    x0, tl_x31,   tensor_return_not_0
      ; Tensor FMA IMA8A32
      bne    x0, tfma_csr, tensor_return_not_0
   
      j tensor_return_is_0
   tensor_return_not_0:
      test_fail(regz = rx1)
   tensor_return_is_0:
   
   
   if (ten_wait == 1):
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Self-Check Tensor Error
   
      ; Tensor Store
      selfcheck_tensor_store:
         tensor_store_wait()
         check_tensor_store_error(num_rand_csr = ts_csr_val, from_scp = 0, rega = rx1, regb = ts_x31, regc = ts_csr, pass_label = selfcheck_tensor_load, fail_label = fail_tensor_error)
      ; Tensor Load B
      selfcheck_tensor_load:
         tensor_load_wait(x31_reg_value = tl_x31_val)
         check_tensor_load_error(num_tload_value = tl_csr_val, rega = rx1, regb = tl_x31, reg_csr_val = tl_csr)
      ; Tensor FMA IMA8A32
      selfcheck_tensor_fma:
         tensor_fma_wait()
         ; FUTURE: Add self-check
     
         j pass_sequence    
      fail_tensor_error:
         test_fail(regz = rx1)
      pass_sequence:
   endif
elif ((cpu_id%2) == 0 && ten_wait == 1):
   tensor_store_wait()
   tensor_load_wait(x31_reg_value = tl_x31_val)
   tensor_fma_wait()
endif

if ((cpu_id%2) == 0):
   reset_tensor_error()
   restore_fp_regs(memory = mem_fp[cpu_x], rega = rx1)
endif
   restore_x31()



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
