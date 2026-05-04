
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/silicon_mode.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/sm_et_abi.pb"

resource ;## STING PHASE
unum_t  cpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)
unum_t  cpuid = snp_cpu_id()
unum_t  phid  = snp_get_phycpu(cpuid)
unum_t  pmid  = snp_rshift(phid, 1)
unum_t  rand_start = snp_randnum()
unum_t  lmid = snp_rshift(cpuid, 1)

; ####################################################################
; ##                                                                ##
; ##                   COMMON VARIABLES                             ##
; ##                                                                ##
; ####################################################################
; Tensor Wait
;unum_t  ten_wait_en   = snp_urandrange(0,2)
unum_t  ten_wait_en   = 1

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION


; ####################################################################
; ##                                                                ##
; ##                         TENSOR QUANT                           ##
; ##                                                                ##
; ####################################################################
unum_t tquant_csr_val  = snp_get_tquant_wrapper()

gmemory_t selfcheck_info[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
gmemory_t smg_pre_l1scp[ET_TEST_ENV.NUM_MINIONS][3072]@64:ET_DRAM_REGION
gmemory_t smg_pre_vpurf[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION
gmemory_t smg_post_vpurf[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION

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
   li csr_reg, tquant_csr_val

if ((cpu_id%2) == 0):
   dump_vpu_rf(_pb_rx1 = rx1, _pb_mem_addr = smg_pre_vpurf[lmid])
   dump_l1scp(pb_rx1 = rx1, pb_rx2 = rx2, pb_rx3 = rx3, pb_mem_addr = smg_pre_l1scp[lmid])
   sm_set_tquant_info(regx1 = rx1, regx2 = rx2, et_info_addr = selfcheck_info[cpuid], csr_enc_reg = csr_reg, pre_data_addr = smg_pre_l1scp[lmid], pre_vpurf_addr = smg_pre_vpurf[lmid], post_data_addr = smg_post_vpurf[lmid], phy_mid = pmid)
endif

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase
   csrrw rx1, tensor_quant, csr_reg

if ((cpuid%2) == 0):
   beq    x0, rx1, tquant_csr_return_is_0

   test_fail(regz = rx1)

tquant_csr_return_is_0:

   if (ten_wait_en == 1):
      tensor_quant_wait()
   endif
   dump_vpu_rf(_pb_rx1 = rx1, _pb_mem_addr = smg_post_vpurf[lmid])
else:
   ; Thread 1 must trap if writes to tensor_store
   ; if trapped, mret will skip test_fail()
   test_fail(regz = rx1)
endif

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Restore Phase
if ((cpu_id%2) == 0):
   reset_tensor_error()
   restore_fp_regs(memory = mem_fp[cpu_x], rega = rx1)
endif
   restore_x31()

if ((cpu_id%2) == 0):
   snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
   selfcheck_tquant(et_info_addr = selfcheck_info[cpuid])
endif


snip_check
; Empty



snip_handler

mhandler@cpu: cpu_x
   tensor_mhandler(rega = rx1, regb = rx2)
   csrr rx1, mepc
   addi rx1, rx1, 16
   csrw mepc, rx1
