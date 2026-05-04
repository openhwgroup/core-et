
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/silicon_mode.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/sm_et_abi.pb"
 
; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE

; Tensor Reduce (Auto/Broadcast)
unum_t  tr_csr_val     = glbl_snp_get_treduce_auto_bcast_wrapper()

unum_t rand_init = snp_randnum_glbl()

unum_t  cpuid = snp_cpu_id()
unum_t  phid  = snp_get_phycpu(cpuid)
unum_t  pmid  = snp_rshift(phid, 1)

unum_t  is_reducing = snp_in_reduce(tr_csr_val, cpuid)

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t tr_csr  ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

unum_t l_mid = snp_rshift(cpuid, 1)
unum_t partner_l_mid = get_partner_lmid(cpuid, tr_csr_val)
memory_t sm_pre_vpurf[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION
gmemory_t smg_post_vpurf[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION
gmemory_t sm_tr_info[ET_TEST_ENV.NUM_MINIONS][128]@64:ET_DRAM_REGION

gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION



snip_setup
    ; Empty


snip_init
   ; Empty


snip_run
@cpu: *
   snippet_start(random_num = rand_init, rega = rx1, regb = rx2)
if (is_reducing == 1):
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Backup Phase
   if ((cpuid%2) == 0):
      reset_tensor_error()
      backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)
   endif
   
   
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Config Phase
      li tr_csr, tr_csr_val
   
   if ((cpu_id%2) == 0):
      dump_vpu_rf(_pb_rx1 = rx1, _pb_mem_addr = sm_pre_vpurf[l_mid])
      flush_vpu_rf(pb_rx1 = rx1, pb_rx2 = rx2, pb_mem_addr = sm_pre_vpurf[l_mid])
      sm_set_tr_info(regx1 = rx1, regx2 = rx2, et_info_addr = sm_tr_info[l_mid], csr_enc_reg = tr_csr, partnr_tr_info_addr = sm_tr_info[partner_l_mid], pre_vpurf_addr = sm_pre_vpurf[l_mid], post_vpurf_addr = smg_post_vpurf[l_mid], phy_mid = pmid)
      flush_et_info(pb_rx1 = rx1, pb_rx2 = rx2, pb_mem_addr = sm_tr_info[l_mid])
   endif

 
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Write to CSR Phase
      csrrw rx1, tensor_reduce, tr_csr


      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Self-Check Return is 0
   if ((cpuid%2) == 0):
      beq    x0, rx1, csr_return_is_0_x
      test_fail(regz = rx1)
   csr_return_is_0_x:
      tensor_reduce_wait()

      dump_vpu_rf(_pb_rx1 = rx1, _pb_mem_addr = smg_post_vpurf[l_mid])

      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Restore Phase
      restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)
   else:
      test_fail(regz = rx1)
   endif
endif

if (is_reducing == 1):
   if ((cpu_id%2) == 0):
      snippet_start(random_num = rand_init, rega = rx1, regb = rx2)
      selfcheck_tr(et_info_addr = sm_tr_info[l_mid])
   endif
endif


snip_check
; Empty


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
