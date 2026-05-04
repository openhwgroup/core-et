
import "$MINION_DIAGS/sting/snippets/tensor/tensor_store.pb"
import "$MINION_DIAGS/sting/snippets/sequences/sequences.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   COMMON VARIABLES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t  cpuid = snp_cpu_id()
unum_t rand_init = snp_randnum_glbl()
unum_t rand_value = 0
unum_t sting_loop_index = 0
unum_t l_mid = snp_rshift(cpuid, 1)

unum_t l1scp_ptr = init_l1scp_ptr()
unum_t vpu_rf_ptr = init_vpu_rf_ptr()
unum_t et_ins_conf_ptr = snp_init_et_ins_conf(vpu_rf_ptr)

; Post Silicon clean AMO
unum_t rn0 = snp_randnum()
unum_t rn1 = snp_randnum()

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   ET INSTRUCTIONS CONFIG
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t et_conf_seq = snp_set_et_ins_conf(vpu_rf_ptr, et_ins_conf_ptr)
unum_t et_ins_fs1 = get_et_ins_fs1_reg(et_ins_conf_ptr)
unum_t et_ins_fs2 = get_et_ins_fs2_reg(et_ins_conf_ptr)
unum_t et_ins_fs3 = get_et_ins_fs3_reg(et_ins_conf_ptr)

unum_t require_fs1 = et_ins_requires_fs1(et_ins_conf_ptr)
unum_t require_fs2 = et_ins_requires_fs2(et_ins_conf_ptr)
unum_t require_fs3 = et_ins_requires_fs3(et_ins_conf_ptr)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   PRM-4 MASK OPERATIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t mask_op0_val = get_seq_mask_ins(et_ins_conf_ptr)
unum_t mask_op1_val = get_seq_mask_ins(et_ins_conf_ptr)
unum_t mask_op2_val = get_seq_mask_ins(et_ins_conf_ptr)
unum_t mask_op3_val = get_seq_mask_ins(et_ins_conf_ptr)
unum_t mask_op4_val = get_seq_mask_ins(et_ins_conf_ptr)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   PRM-5 PACKEDSINGLE OPERATIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t ps_op0_val = get_seq_ps_ins(et_ins_conf_ptr)
unum_t ps_op1_val = get_seq_ps_ins(et_ins_conf_ptr)
unum_t ps_op2_val = get_seq_ps_ins(et_ins_conf_ptr)
unum_t ps_op3_val = get_seq_ps_ins(et_ins_conf_ptr)
unum_t ps_op4_val = get_seq_ps_ins(et_ins_conf_ptr)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   PRM-6 PACKEDINTEGER OPERATIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t pi_op0_val = get_seq_pi_ins(et_ins_conf_ptr)
unum_t pi_op1_val = get_seq_pi_ins(et_ins_conf_ptr)
unum_t pi_op2_val = get_seq_pi_ins(et_ins_conf_ptr)
unum_t pi_op3_val = get_seq_pi_ins(et_ins_conf_ptr)
unum_t pi_op4_val = get_seq_pi_ins(et_ins_conf_ptr)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   PRM-7 ATOMIC OPERATIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t ba_reg_amo = get_et_ins_aligned_ba_reg(et_ins_conf_ptr)
unum_t amo_op0_val = get_seq_amo_ins(et_ins_conf_ptr)
unum_t amo_op1_val = get_seq_amo_ins(et_ins_conf_ptr)
unum_t amo_op2_val = get_seq_amo_ins(et_ins_conf_ptr)
unum_t amo_op3_val = get_seq_amo_ins(et_ins_conf_ptr)
unum_t amo_op4_val = get_seq_amo_ins(et_ins_conf_ptr)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   RISC-V BASE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t rv_op0_val = get_seq_riscv_base_ins(et_ins_conf_ptr)
unum_t rv_op1_val = get_seq_riscv_base_ins(et_ins_conf_ptr)
unum_t rv_op2_val = get_seq_riscv_base_ins(et_ins_conf_ptr)
unum_t rv_op3_val = get_seq_riscv_base_ins(et_ins_conf_ptr)
unum_t rv_op4_val = get_seq_riscv_base_ins(et_ins_conf_ptr)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   FREE COMMON VARIABLES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t done_l1scp_ptr = free_l1scp_ptr(l1scp_ptr)
unum_t done_vpu_rf_ptr = free_vpu_rf_ptr(vpu_rf_ptr)
unum_t done_et_ins_conf_ptr = free_et_ins_conf_ptr(et_ins_conf_ptr)


axreg_t rx_mem
pxreg_t rx1
pxreg_t rx2

; 41216 = 40960 + 256
gmemory_t op_mem[ET_TEST_ENV.NUM_MINIONS][41216]@64:ET_SEQ_MEM_SET
uc_gmemory_t amo_mem[ET_TEST_ENV.NUM_MINIONS][2112]@64:ET_DRAM_REGION
memory_t et_ins_fsx_mem[ET_TEST_ENV.NUM_MINIONS][128]@64:ET_DRAM_REGION



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
@cpu: 0,2..BOARD_CONF.NUM_CPUS
if (require_fs1 || require_fs2 || require_fs3):
   ; 8 lanes == 32 bytes
   la rx_mem, et_ins_fsx_mem[l_mid]
   if (require_fs1): ;fs1
      rand_value = snp_randnum()
      store_rand_32bytes(loop_index = sting_loop_index, num_var = rand_value, reg_mem = rx_mem, rega = rx1, regb = rx2)
   endif
   if (require_fs2): ;fs2
      if (require_fs1):
         addi rx_mem, rx_mem, 32
      endif
      rand_value = snp_randnum()
      store_rand_32bytes(loop_index = sting_loop_index, num_var = rand_value, reg_mem = rx_mem, rega = rx1, regb = rx2)
   endif
   if (require_fs3): ;fs3
      if (require_fs2):
         addi rx_mem, rx_mem, 32
      endif
      rand_value = snp_randnum()
      store_rand_32bytes(loop_index = sting_loop_index, num_var = rand_value, reg_mem = rx_mem, rega = rx1, regb = rx2)
   endif
endif


;
; The 'snip_run' section gets rendered inside the random code section depending
; on the bias specified for this particular snippet.
;
snip_run ;## STING PHASE
@cpu: *
   snippet_start(random_num = rand_init, rega = rx_mem, regb = rx1)

@cpu: 0,2..BOARD_CONF.NUM_CPUS
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Backup Phase
   backup_rx(imm_rsx = ba_reg_amo)

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Pre Load PS FS1, FS2, FS3 values
if (require_fs1 || require_fs2 || require_fs3):
   la rx_mem, et_ins_fsx_mem[l_mid]
   if (require_fs1): ;fs1
      load_mem_to_vpu_lanes(reg_mem = rx_mem, imm_fp = et_ins_fs1)
   endif
   if (require_fs2): ;fs2
      if (require_fs1):
         addi rx_mem, rx_mem, 32
      endif
      load_mem_to_vpu_lanes(reg_mem = rx_mem, imm_fp = et_ins_fs2)
   endif
   if (require_fs3): ;fs3
      if (require_fs2):
         addi rx_mem, rx_mem, 32
      endif
      load_mem_to_vpu_lanes(reg_mem = rx_mem, imm_fp = et_ins_fs3)
   endif
endif
   ; Setup AMO Aligned Base Address
   load_address_imm_rsx(imm_rsx = ba_reg_amo, imm_addr = amo_mem[l_mid])

   ; Do tensor wait just in case there are other Tensor Operations ongoing from a previous snippet
   if (ET_TEST_ENV.STING_RAND == 1):
      write_tensor_wait(pb_wait_type = 0x0780)
   endif

   ; OP0
   avow_inline_ins(pb_ins0 = ps_op0_val, pb_ins1 = amo_op0_val, pb_ins2 = mask_op0_val, pb_ins3 = pi_op0_val, pb_ins4 = rv_op0_val)
   ; OP1
   avow_inline_ins(pb_ins0 = ps_op1_val, pb_ins1 = amo_op1_val, pb_ins2 = mask_op1_val, pb_ins3 = pi_op1_val, pb_ins4 = rv_op1_val)
   ; OP2
   avow_inline_ins(pb_ins0 = ps_op2_val, pb_ins1 = amo_op2_val, pb_ins2 = mask_op2_val, pb_ins3 = pi_op2_val, pb_ins4 = rv_op2_val)
   ; OP3
   avow_inline_ins(pb_ins0 = ps_op3_val, pb_ins1 = amo_op3_val, pb_ins2 = mask_op3_val, pb_ins3 = pi_op3_val, pb_ins4 = rv_op3_val)
   ; OP4
   avow_inline_ins(pb_ins0 = ps_op4_val, pb_ins1 = amo_op4_val, pb_ins2 = mask_op4_val, pb_ins3 = pi_op4_val, pb_ins4 = rv_op4_val)

   restore_rx(imm_rsx = ba_reg_amo)
if ET_TEST_ENV.SILICON_MODE == 1:
   clean_amo_rd(pb_rn0 = rn0 , pb_rn1 = rn1)
endif



;
; Similarly the 'snip_check' section is invoked at the end of the test after all
; the hardware threads are done with their random code section. It can be used to
; carry out any test specific checks like verifying the contents of a memory buffer
; against the expected values.
;
snip_check
   ; Empty
