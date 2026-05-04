
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/silicon_mode.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/sm_et_abi.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE

unum_t bias      = snp_urandrange_glbl(0, 27)
unum_t rand_hart = snp_urandrange_glbl(0, BOARD_CONF.NUM_CPUS)
unum_t rand_op   = snp_urandrange_glbl(0, 2)
unum_t x         = snp_get_hartid_biased_from_rand(BOARD_CONF.NUM_CPUS, bias, rand_hart, rand_op)

unum_t rand_init = snp_randnum_glbl()

unum_t  phy_x = BOARD_CONF.LCPUID_TO_PCPUID_MAP[x]
unum_t  mid_x = snp_rshift(phy_x, 1)
unum_t  lmid_x = snp_rshift(x, 1)
unum_t  tid_x = snp_and(phy_x, 0x0000000000000001)
;unum_t  y = snp_urandrange_glbl(0, BOARD_CONF.NUM_CPUS)
unum_t  y = snp_get_tensor_reduce_partner(BOARD_CONF.NUM_CPUS, x)
unum_t  phy_y = BOARD_CONF.LCPUID_TO_PCPUID_MAP[y]
unum_t  mid_y = snp_rshift(phy_y, 1)
unum_t  lmid_y = snp_rshift(y, 1)
unum_t  tid_y = snp_and(phy_y, 0x0000000000000001)

; Tensor Reduce
; Initial random value
unum_t  tr_csr_val_rand = snp_randnum_glbl()
unum_t  tr_csr_val_op   = snp_set_legal_reduce_op(tr_csr_val_rand, tr_csr_val_rand)
; Mask to leave the Sender/Receiver MinionID as 0 and Snd/Rcv field to 0
unum_t  tr_csr_val_id   = snp_and(tr_csr_val_op,   0xffffffffffff0004)

unum_t  tr_csr_val_snd = 0
unum_t  tr_csr_val_snd_tmp = 0

unum_t  tr_csr_val_rcv = 0
unum_t  tr_csr_val_rcv_tmp = 0

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t tr_csr  ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

gmemory_t selfcheck_info[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION
memory_t sm_pre_vpurf[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION
gmemory_t smg_post_vpurf[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION

gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION


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
   ; ## Pre-Processing ##
   ; X sends to Y
   ;
   ;> Sender pre-processing
   ; Left-shift the MinionID 3 positions, as MinionID starts on bit 3
   tr_csr_val_snd_tmp = snp_lshift(mid_y, 3)
   ; OR the values
   tr_csr_val_snd     = snp_or(tr_csr_val_id, tr_csr_val_snd_tmp)
   ; ####################

   snippet_start(random_num = rand_init, rega = rx1, regb = rx2)

if ((tid_y == 0) | (tid_x == tid_y)):
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Backup Phase
   if ((x%2) == 0):
      backup_fp_regs(memory = mem_fp[x], rega = rx1)
      reset_tensor_error()
   endif


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Config Phase
   li tr_csr, tr_csr_val_snd

   if ((x%2) == 0):
      dump_vpu_rf(_pb_rx1 = rx1, _pb_mem_addr = sm_pre_vpurf[lmid_x])
      flush_vpu_rf(pb_rx1 = rx1, pb_rx2 = rx2, pb_mem_addr = sm_pre_vpurf[lmid_x])
      sm_set_tr_info(regx1 = rx1, regx2 = rx2, et_info_addr = selfcheck_info[x], csr_enc_reg = tr_csr, partnr_tr_info_addr = selfcheck_info[y], pre_vpurf_addr = sm_pre_vpurf[lmid_x], post_vpurf_addr = smg_post_vpurf[lmid_x], phy_mid = mid_x)
      flush_et_info(pb_rx1 = rx1, pb_rx2 = rx2, pb_mem_addr = selfcheck_info[x])
   endif


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase
   csrrw rx1, tensor_reduce, tr_csr


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Self-Check Return is 0
   if ((x%2) == 0):
         beq    x0, rx1, csr_return_is_0_x
         test_fail(regz = rx1)
      csr_return_is_0_x:
         tensor_reduce_wait()

         dump_vpu_rf(_pb_rx1 = rx1, _pb_mem_addr = smg_post_vpurf[lmid_x])

      if (mid_x == mid_y):
         check_tensor_reduce_error(rega = rx1, regb = rx2, regc = tr_csr, same_minion = 1, fail_reduce = fail_tr_error_x, pass_reduce = pass_tr_error_x)
      else :
         check_tensor_reduce_error(rega = rx1, regb = rx2, regc = tr_csr, same_minion = 0, fail_reduce = fail_tr_error_x, pass_reduce = pass_tr_error_x)
      endif

      fail_tr_error_x:
         test_fail(regz = rx1)
      pass_tr_error_x:


      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Restore Phase
         reset_tensor_error()
         restore_fp_regs(memory = mem_fp[x], rega = rx1)
   else:
      test_fail(regz = rx1)
   endif
endif

if ((tid_y == 0) | (tid_x == tid_y)):
   if ((x%2) == 0):
      snippet_start(random_num = rand_init, rega = rx1, regb = rx2)
      selfcheck_tr(et_info_addr = selfcheck_info[x])
   endif
endif



@cpu: y
   ; ## Pre-Processing ##
   ; X sends to Y
   ;
   ;> Receiver pre-processing
   ; Left-shift the MinionID 3 positions, as MinionID starts on bit 3
   tr_csr_val_rcv_tmp = snp_lshift(mid_x, 3)
   ; Set the operation as a Recv
   tr_csr_val_rcv_tmp = snp_or(tr_csr_val_rcv_tmp, 1)
   ; OR the values
   tr_csr_val_rcv     = snp_or(tr_csr_val_id, tr_csr_val_rcv_tmp)
   ; ####################

   snippet_start(random_num = rand_init, rega = rx1, regb = rx2)

if ((tid_x == 0) | (tid_x == tid_y)):
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Backup Phase
   if ((y%2) == 0):
      reset_tensor_error()
      backup_fp_regs(memory = mem_fp[y], rega = rx1)
   endif


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Config Phase
   li tr_csr, tr_csr_val_rcv

   if ((y%2) == 0):
      dump_vpu_rf(_pb_rx1 = rx1, _pb_mem_addr = sm_pre_vpurf[lmid_y])
      flush_vpu_rf(pb_rx1 = rx1, pb_rx2 = rx2, pb_mem_addr = sm_pre_vpurf[lmid_y])
      sm_set_tr_info(regx1 = rx1, regx2 = rx2, et_info_addr = selfcheck_info[y], csr_enc_reg = tr_csr, partnr_tr_info_addr = selfcheck_info[x], pre_vpurf_addr = sm_pre_vpurf[lmid_y], post_vpurf_addr = smg_post_vpurf[lmid_y], phy_mid = mid_y)
      flush_et_info(pb_rx1 = rx1, pb_rx2 = rx2, pb_mem_addr = selfcheck_info[y])
   endif


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase
   csrrw rx1, tensor_reduce, tr_csr


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Self-Check Return is 0
   if ((y%2) == 0):
         beq    x0, rx1, csr_return_is_0_y
         test_fail(regz = rx1)
      csr_return_is_0_y:
         tensor_reduce_wait()

         dump_vpu_rf(_pb_rx1 = rx1, _pb_mem_addr = smg_post_vpurf[lmid_y])

      if (mid_y == mid_x):
         check_tensor_reduce_error(rega = rx1, regb = rx2, regc = tr_csr, same_minion = 1, fail_reduce = fail_tr_error_y, pass_reduce = pass_tr_error_y)
      else :
         check_tensor_reduce_error(rega = rx1, regb = rx2, regc = tr_csr, same_minion = 0, fail_reduce = fail_tr_error_y, pass_reduce = pass_tr_error_y)
      endif

      fail_tr_error_y:
         test_fail(regz = rx1)
      pass_tr_error_y:

      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Restore Phase
         reset_tensor_error()
         restore_fp_regs(memory = mem_fp[y], rega = rx1)
   else:
      test_fail(regz = rx1)
   endif
endif

if ((tid_x == 0) | (tid_x == tid_y)):
   if ((y%2) == 0):
      snippet_start(random_num = rand_init, rega = rx1, regb = rx2)
      selfcheck_tr(et_info_addr = selfcheck_info[y])
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

snip_handler
mhandler@cpu: *
   ; HART1 will always generate an illegal instruction exception
   ; This type of exception is always handled in Machine mode
   csrrs rx1, mcause, x0

if ((cpu_id%2) == 1):
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
