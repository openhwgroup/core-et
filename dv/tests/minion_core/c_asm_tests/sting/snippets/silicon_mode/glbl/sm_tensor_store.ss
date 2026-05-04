
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
unum_t  lmid = snp_rshift(cpuid, 1)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Store
unum_t ts_csr_val = snp_get_tstore_wrapper()

unum_t x31_rand   = snp_urandrange(1,5)
unum_t ts_x31_val = snp_get_ts_stride(ts_csr_val, x31_rand)

unum_t pre_tensor_wait = get_tstore_ten_waits(ts_csr_val);

axreg_t ts_x31
axreg_t ts_csr

axreg_t rx_mem
axreg_t rx1
axreg_t rx2
axreg_t rx3

gmemory_t smg_pre_vpurf[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION
memory_t sm_post_data[4544]@64:ET_MINION_SHIRE_MEM_SET
gmemory_t selfcheck_info[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION

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

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Config Phase

   ; Tensor Store
   li ts_x31, ts_x31_val
   la rx_mem, sm_post_data
   prepare_tensor_csr(pb_csr = ts_csr, pb_csr_val = ts_csr_val, pb_op_type = ET_SEQ_OP.CSR_TSTORE, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = 0)


if ((cpu_id%2) == 0):
   dump_vpu_rf(_pb_rx1 = rx1, _pb_mem_addr = smg_pre_vpurf[lmid])
   sm_set_ts_info(regx1 = rx1, regx2 = rx2, et_info_addr = selfcheck_info[cpuid], csr_enc_reg = ts_csr, x31_reg = ts_x31, pre_vpurf_addr = smg_pre_vpurf[lmid], post_data_addr = sm_post_data, phy_mid = pmid)
endif

   write_tensor_wait(pb_wait_type = pre_tensor_wait)

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase
   seq_ts(_cpu_id = cpu_id, _rx1 = rx1, _ts_csr = ts_csr, _ts_x31 = ts_x31, _self_check = 1)


if ((cpu_id%2) == 0):
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Self-Check Return is 0
      bne    x0, ts_x31,   tensor_return_not_0
      j tensor_return_is_0
   tensor_return_not_0:
      test_fail(regz = rx1)
   tensor_return_is_0:


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Self-Check Tensor Error
   selfcheck_tensor_store:
      ; Wait until the operation is done
      tensor_store_wait()
      ; Tensor Error Self Check
      check_tensor_store_error(num_rand_csr = ts_csr_val, from_scp = 0, rega = rx1, regb = rx2, regc = rx3, pass_label = pass_ts_tensor_error, fail_label = fail_ts_tensor_error)

   fail_ts_tensor_error:
      test_fail(regz = rx1)

   pass_ts_tensor_error:
      csrrw x0, tensor_error, x0
endif

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Restore Phase
   restore_x31()

if ((cpu_id%2) == 0):
   snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
   selfcheck_tstore(et_info_addr = selfcheck_info[cpuid])
endif


snip_check
; Empty


snip_handler
mhandler@cpu: cpu_x
    tensor_mhandler(rega = rx1, regb = rx2)
    csrr rx1, mepc
    addi rx1, rx1, 16
    csrw mepc, rx1
