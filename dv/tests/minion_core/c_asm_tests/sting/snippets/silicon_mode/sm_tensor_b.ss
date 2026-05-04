
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/silicon_mode.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/sm_et_abi.pb"
import "$MINION_DIAGS/sting/snippets/sequences/sequences.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
unum_t  cpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)
unum_t  cpuid = snp_cpu_id()
unum_t  phid  = snp_get_phycpu(cpuid)
unum_t  pmid  = snp_rshift(phid, 1)
unum_t  rand_start = snp_randnum()


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Load
unum_t  tensor_mask_val = snp_randnum()
unum_t  tl_csr_val = snp_get_tload_tenb_wrapper()
; FUTURE: support stride greater than 1
unum_t  tl_x31_val = glbl_snp_get_tload_x31_wrapper(2)

axreg_t tl_x31
axreg_t tl_csr

axreg_t rx1
axreg_t rx2
axreg_t rx3

unum_t tl_off_rand = snp_urandrange(0, 32)
unum_t tl_off = snp_lshift(tl_off_rand, 6) ; max offset of 1984


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor FMA
unum_t  tfma_csr_val = glbl_snp_get_sm_tenb_tfma_wrapper(tl_csr_val)
axreg_t tfma_csr

unum_t  pre_tensor_wait = snp_get_tenb_pre_ten_waits(tl_csr_val, tfma_csr_val)

gmemory_t op_mem[41216]@64:ET_SEQ_MEM_SET
gmemory_t mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
memory_t sm_tenb_info[128]@64:ET_DRAM_REGION
memory_t sm_pre_data[3072]@64:ET_DRAM_REGION
memory_t sm_pre_vpurf[1024]@64:ET_DRAM_REGION
memory_t sm_post_vpurf[1024]@64:ET_DRAM_REGION
memory_t sm_pre_vputenc[1024]@64:ET_DRAM_REGION
memory_t sm_post_vputenc[1024]@64:ET_DRAM_REGION

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

if (((tl_csr_val & 0x8000000000000000) != 0) | ((tfma_csr_val & 0x8000000000000000) != 0)):
   ; [Setup] For Tensor Loads, if bit 63 is set, we are using Tensor Mask (PRM-10)
   set_tensor_mask(value = tensor_mask_val, rega = tl_csr)
endif

   ; Tensor Load
   li tl_x31, tl_x31_val
   li tl_csr, tl_csr_val
   la  rx1, op_mem
   ; As we use gmemory_t, randomly add an offset to not start always with the same address
   SELECT 1 {
      1 : RANDINST 0:0
      1 : addi rx1, rx1, 64
      1 : addi rx1, rx1, 128
      1 : addi rx1, rx1, 192
      1 : addi rx1, rx1, 256
   }
   addi rx1, rx1, tl_off
   li  rx2, 0x0000ffffffffffc0
   and rx1, rx1, rx2
   or  tl_csr, tl_csr, rx1

   ; Tensor FMA
   li tfma_csr, tfma_csr_val

if ((cpu_id%2) == 0):
   dump_vpu_rf_tenc(pb_rx1 = rx1, pb_rx2 = rx2, pb_vpu_rf_mem_addr = sm_pre_vpurf, pb_vpu_tenc_mem_addr = sm_pre_vputenc)
   dump_l1scp(pb_rx1 = rx1, pb_rx2 = rx2, pb_rx3 = rx3, pb_mem_addr = sm_pre_data)
   sm_set_tfma_info(regx1 = rx1, regx2 = rx2, et_info_addr = sm_tenb_info, csr_enc_reg = tfma_csr, x31_reg = tl_x31, pre_data_addr = sm_pre_data, pre_vpurf_addr = sm_pre_vpurf, tl_tenb_csr_reg = tl_csr, post_data_addr = sm_post_vpurf, pre_tenc_addr = sm_pre_vputenc, post_tenc_addr = sm_post_vputenc, phy_mid = pmid)
endif

   write_tensor_wait(pb_wait_type = pre_tensor_wait)

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase
   seq_tl (_cpu_id = cpu_id, _rx1 = rx1, _tl_csr = tl_csr, _tl_x31 = tl_x31, _self_check = 1)
   seq_tfma(_cpu_id = cpu_id, _rx1 = rx1, _tfma_csr = tfma_csr, _self_check = 1)


if ((cpu_id%2) == 0):
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Self-Check Return is 0

      ; Tensor Load
      bne    x0, tl_x31,   tensor_return_not_0
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
      ; FUTURE: add self-check

      j pass_sequence
   fail_tensor_error:
      test_fail(regz = rx1)
   pass_sequence:
      dump_vpu_rf_tenc(pb_rx1 = rx1, pb_rx2 = rx2, pb_vpu_rf_mem_addr = sm_post_vpurf, pb_vpu_tenc_mem_addr = sm_post_vputenc)
endif

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Restore Phase
if ((cpu_id%2) == 0):
   reset_tensor_error()
   restore_fp_regs(memory = mem_fp[cpu_x], rega = rx1)
endif
   restore_x31()


snip_check
@cpu: cpu_x
if (ET_TEST_ENV.SCP_ENABLE == 1):
   if ((cpu_id%2) == 0):
      snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
      selfcheck_tfma(et_info_addr = sm_tenb_info)
   endif
endif


snip_handler
mhandler@cpu: cpu_x
    tensor_mhandler(rega = rx1, regb = rx2)
    csrr rx1, mepc
    addi rx1, rx1, 16
    csrw mepc, rx1
