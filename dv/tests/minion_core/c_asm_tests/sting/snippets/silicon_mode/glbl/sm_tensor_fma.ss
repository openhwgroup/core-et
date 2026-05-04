
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/silicon_mode.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/sm_et_abi.pb"
import "$MINION_DIAGS/sting/snippets/sequences/sequences.pb"

resource ;## STING PHASE
unum_t  cpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)
unum_t  cpuid = snp_cpu_id()
unum_t  phid  = snp_get_phycpu(cpuid)
unum_t  pmid  = snp_rshift(phid, 1)
unum_t  tensor_mask_val = snp_randnum()
unum_t  rand_start = snp_randnum()
unum_t  lmid = snp_rshift(cpuid, 1)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor FMA
unum_t  tfma_csr_val = glbl_snp_get_sm_tfma()
unum_t  pre_tensor_wait = snp_get_tfma_pre_ten_waits(tfma_csr_val)

axreg_t tfma_csr
axreg_t rx1
axreg_t rx2
axreg_t rx3

gmemory_t mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
gmemory_t selfcheck_info[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
gmemory_t smg_pre_l1scp[ET_TEST_ENV.NUM_MINIONS][3072]@64:ET_DRAM_REGION
gmemory_t smg_pre_vpurf[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION
gmemory_t smg_post_vpurf[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION
gmemory_t smg_pre_vputenc[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION
gmemory_t smg_post_vputenc[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION


snip_setup ;## STING PHASE
    ; Empty


snip_init ;## STING PHASE
   ; Empty


snip_run ;## STING PHASE
@cpu: cpu_x

if ((cpu_id%2) == 0):
   snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
endif

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Backup Phase
   backup_x31()
if ((cpu_id%2) == 0):
   backup_fp_regs(memory = mem_fp[cpu_x], rega = rx1)
endif


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Config Phase

if ((tfma_csr_val & 0x8000000000000000) != 0) :
   ; [Setup] For Tensor FMA, if bit 63 is set, we are using Tensor Mask (PRM-10)
   set_tensor_mask(value = tensor_mask_val, rega = tfma_csr)
endif

   ; Tensor FMA
   li tfma_csr, tfma_csr_val

if ((cpu_id%2) == 0):
   dump_vpu_rf_tenc(pb_rx1 = rx1, pb_rx2 = rx2, pb_vpu_rf_mem_addr = smg_pre_vpurf[lmid], pb_vpu_tenc_mem_addr = smg_pre_vputenc[lmid])
   dump_l1scp(pb_rx1 = rx1, pb_rx2 = rx2, pb_rx3 = rx3, pb_mem_addr = smg_pre_l1scp[lmid])
   sm_set_tfma_info(regx1 = rx1, regx2 = rx2, et_info_addr = selfcheck_info[cpuid], csr_enc_reg = tfma_csr, x31_reg = x0, pre_data_addr = smg_pre_l1scp[lmid], pre_vpurf_addr = smg_pre_vpurf[lmid], tl_tenb_csr_reg = x0, post_data_addr = smg_post_vpurf[lmid], pre_tenc_addr = smg_pre_vputenc[lmid], post_tenc_addr = smg_post_vputenc[lmid], phy_mid = pmid)
endif

   write_tensor_wait(pb_wait_type = pre_tensor_wait)

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase
   seq_tfma(_cpu_id = cpu_id, _rx1 = rx1, _tfma_csr = tfma_csr, _self_check = 1)


if ((cpu_id%2) == 0):
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Self-Check Return is 0

      ; Tensor FMA
      bne    x0, tfma_csr, tensor_return_not_0

      j tensor_return_is_0
   tensor_return_not_0:
      test_fail(regz = rx1)
   tensor_return_is_0:


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Self-Check Tensor Error

   ; Tensor FMA
   selfcheck_tensor_fma:
      tensor_fma_wait()
      ; FUTURE: add self check

      j pass_sequence
   fail_tensor_error:
      test_fail(regz = rx1)
   pass_sequence:
      dump_vpu_rf_tenc(pb_rx1 = rx1, pb_rx2 = rx2, pb_vpu_rf_mem_addr = smg_post_vpurf[lmid], pb_vpu_tenc_mem_addr = smg_post_vputenc[lmid])
endif

if ((cpu_id%2) == 0):
   reset_tensor_error()
   restore_fp_regs(memory = mem_fp[cpu_x], rega = rx1)
endif
   restore_x31()

if (ET_TEST_ENV.SCP_ENABLE == 1):
   if ((cpu_id%2) == 0):
      snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
      selfcheck_tfma(et_info_addr = selfcheck_info[cpuid])
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
