
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
unum_t  rand_start = snp_randnum_glbl()
unum_t  lmid = snp_rshift(cpuid, 1)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Store
unum_t ts_csr_val = glbl_snp_get_coop_tstore_wrapper()

unum_t x31_rand   = snp_urandrange_glbl(1,5)
unum_t ts_x31_val = snp_get_ts_stride(ts_csr_val, x31_rand)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Store Coop
unum_t  in_cts = snp_in_cts(ts_csr_val)
unum_t  base_offset_rand = snp_urandrange_glbl(0, 2048)
unum_t  base_offset      = snp_and(base_offset_rand, 0xffffffffffffffc0)
unum_t  coop_offset      = snp_align_coop_addr(ts_csr_val, base_offset)
unum_t  cts_base_lcpuid  = snp_get_cts_base_lcpuid(ts_csr_val)
unum_t  mem_idx          = snp_rshift(cts_base_lcpuid, 1)

unum_t pre_tensor_wait = get_tstore_ten_waits(ts_csr_val)

axreg_t ts_x31
axreg_t ts_csr

axreg_t rx_mem
axreg_t rx1
axreg_t rx2
axreg_t rx3

memory_t sm_cts_post_data[ET_TEST_ENV.NUM_MINIONS][4544]@64:ET_MINION_SHIRE_MEM_SET
gmemory_t smg_pre_vpurf[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION
gmemory_t sm_ts_info[ET_TEST_ENV.NUM_MINIONS][128]@64:ET_DRAM_REGION

snip_setup ;## STING PHASE
    ; Empty

snip_init ;## STING PHASE
   ; Empty

snip_run ;## STING PHASE
@cpu: *

if ((cpu_id%2) == 0):
   snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
endif

if (in_cts == 1):
   li rx1, 0xfeed
   li rx1, cts_base_lcpuid
   li rx1, 0xfeed
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Backup Phase
   backup_x31()

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Config Phase

   ; Tensor Store
   li ts_x31, ts_x31_val
   la rx_mem, sm_cts_post_data[mem_idx]
   ; prepare tensor CSR
   li  ts_csr, ts_csr_val
   li  rx1, 0x0000fffffffffff0
   and rx1, rx_mem, rx1
   if (coop_offset != 0):
      addi rx1, rx1, coop_offset
   endif
   or  ts_csr, ts_csr, rx1


   if ((cpu_id%2) == 0):
      dump_vpu_rf(_pb_rx1 = rx1, _pb_mem_addr = smg_pre_vpurf[lmid])
      sm_set_cts_info(regx1 = rx1, regx2 = rx2, et_info_addr = sm_ts_info[lmid], csr_enc_reg = ts_csr, x31_reg = ts_x31, pre_vpurf_addr = smg_pre_vpurf[lmid], post_data_addr = sm_cts_post_data[mem_idx], post_data_off = coop_offset, phy_mid = pmid)
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

   ; Self-Check
   ; Because the memory is shared (gmemory_t) in Cooperative Tensor Stores, the checking must be done
   ; immediately after doing the operation
   if ((cpu_id%2) == 0):
      snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
      selfcheck_tstore(et_info_addr = sm_ts_info[lmid])
   endif
endif

snip_check
; Empty


snip_handler
mhandler@cpu: *
    tensor_mhandler(rega = rx1, regb = rx2)
    csrr rx1, mepc
    addi rx1, rx1, 16
    csrw mepc, rx1
