
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

unum_t x31_val = glbl_snp_get_tload_x31_wrapper(3)
unum_t tensor_mask_val = snp_randnum()

unum_t l1scp_ptr = init_l1scp_ptr()
unum_t vpu_rf_ptr = init_vpu_rf_ptr()
unum_t et_ins_conf_ptr = snp_init_et_ins_conf(vpu_rf_ptr)

; Post Silicon clean AMO
unum_t rn0 = snp_randnum()
unum_t rn1 = snp_randnum()

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   PRM-10 TENSOR OPERATIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t TEN_SEQ_SIZE = 5
unum_t ten_seq_ptr = init_tensor_sequence(TEN_SEQ_SIZE)

; Compute the TENSOR sequence
unum_t ten_op_seq = glbl_snp_set_tensor_ops_sequence(l1scp_ptr, vpu_rf_ptr, ten_seq_ptr, TEN_SEQ_SIZE, rand_init)

unum_t tensor_op0_type     = get_csr_op_type_from_idx(0, ten_seq_ptr)
unum_t tensor_op0_csr_val = get_csr_op_value_from_idx(0, ten_seq_ptr)
unum_t tensor_op0_mem_off =   get_mem_offset_from_idx(0, ten_seq_ptr, x31_val)

unum_t tensor_op1_type      = get_csr_op_type_from_idx(1, ten_seq_ptr)
unum_t tensor_op1_csr_val  = get_csr_op_value_from_idx(1, ten_seq_ptr)
unum_t tensor_op1_tw_type = get_tenwait_value_from_idx(1, ten_seq_ptr, TEN_SEQ_SIZE)
unum_t tensor_op1_mem_off  =   get_mem_offset_from_idx(1, ten_seq_ptr, x31_val)
unum_t tensor_op1_use_x31 =          uses_x31_from_idx(1, ten_seq_ptr, TEN_SEQ_SIZE)

unum_t tensor_op2_type      = get_csr_op_type_from_idx(2, ten_seq_ptr)
unum_t tensor_op2_csr_val  = get_csr_op_value_from_idx(2, ten_seq_ptr)
unum_t tensor_op2_tw_type = get_tenwait_value_from_idx(2, ten_seq_ptr, TEN_SEQ_SIZE)
unum_t tensor_op2_mem_off  =   get_mem_offset_from_idx(2, ten_seq_ptr, x31_val)
unum_t tensor_op2_use_x31 =          uses_x31_from_idx(2, ten_seq_ptr, TEN_SEQ_SIZE)

unum_t tensor_op3_type      = get_csr_op_type_from_idx(3, ten_seq_ptr)
unum_t tensor_op3_csr_val  = get_csr_op_value_from_idx(3, ten_seq_ptr)
unum_t tensor_op3_tw_type = get_tenwait_value_from_idx(3, ten_seq_ptr, TEN_SEQ_SIZE)
unum_t tensor_op3_mem_off  =   get_mem_offset_from_idx(3, ten_seq_ptr, x31_val)
unum_t tensor_op3_use_x31 =          uses_x31_from_idx(3, ten_seq_ptr, TEN_SEQ_SIZE)

unum_t tensor_op4_type      = get_csr_op_type_from_idx(4, ten_seq_ptr)
unum_t tensor_op4_csr_val  = get_csr_op_value_from_idx(4, ten_seq_ptr)
unum_t tensor_op4_tw_type = get_tenwait_value_from_idx(4, ten_seq_ptr, TEN_SEQ_SIZE)
unum_t tensor_op4_mem_off  =   get_mem_offset_from_idx(4, ten_seq_ptr, x31_val)
unum_t tensor_op4_use_x31 =          uses_x31_from_idx(4, ten_seq_ptr, TEN_SEQ_SIZE)

; Compute Clean TenB
unum_t clean_tenb = get_clean_tenb_tfma(l1scp_ptr, vpu_rf_ptr, ten_seq_ptr, TEN_SEQ_SIZE)
unum_t clean_tenb_twait = get_clean_tenb_twait(ten_seq_ptr, TEN_SEQ_SIZE)

; Tensor Waits logic
unum_t pre_tensor_wait = get_pre_tensor_wait(ten_seq_ptr, TEN_SEQ_SIZE, clean_tenb)

; Selfcheck memory size is enough
unum_t mem_occ = selfcheck_mem_occupation(40960, ten_seq_ptr, TEN_SEQ_SIZE, x31_val)

; Check if tensor_mask is required
unum_t set_tmask = uses_tensor_mask(ten_seq_ptr, TEN_SEQ_SIZE)

; Check if register x31 is required
unum_t set_x31 = uses_x31(ten_seq_ptr, TEN_SEQ_SIZE)

; Post Tensor Waits (for Sting Rand)
unum_t post_ten_waits = get_post_tensor_wait(ten_seq_ptr, TEN_SEQ_SIZE, clean_tenb, x31_val)

axreg_t tensor_op0_csr
axreg_t tensor_op1_csr
axreg_t tensor_op2_csr
axreg_t tensor_op3_csr
axreg_t tensor_op4_csr


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
;;;   PRM-5 PACKEDSINGLE OPERATIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t ps_op0_val = get_seq_ps_ins(et_ins_conf_ptr)
unum_t ps_op1_val = get_seq_ps_ins(et_ins_conf_ptr)
unum_t ps_op2_val = get_seq_ps_ins(et_ins_conf_ptr)
unum_t ps_op3_val = get_seq_ps_ins(et_ins_conf_ptr)
unum_t ps_op4_val = get_seq_ps_ins(et_ins_conf_ptr)
unum_t ps_op5_val = get_seq_ps_ins(et_ins_conf_ptr)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   PRM-7 ATOMIC OPERATIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t ba_reg_amo = get_et_ins_aligned_ba_reg(et_ins_conf_ptr)
unum_t amo_op0_val = get_seq_amo_ins(et_ins_conf_ptr)
unum_t amo_op1_val = get_seq_amo_ins(et_ins_conf_ptr)
unum_t amo_op2_val = get_seq_amo_ins(et_ins_conf_ptr)
unum_t amo_op3_val = get_seq_amo_ins(et_ins_conf_ptr)
unum_t amo_op4_val = get_seq_amo_ins(et_ins_conf_ptr)
unum_t amo_op5_val = get_seq_amo_ins(et_ins_conf_ptr)

; Post Silicon
unum_t ow_fregs = snp_ow_unpr_fregs(ten_seq_ptr, TEN_SEQ_SIZE, vpu_rf_ptr)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;   FREE COMMON VARIABLES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
unum_t done_l1scp_ptr = free_l1scp_ptr(l1scp_ptr)
unum_t done_vpu_rf_ptr = free_vpu_rf_ptr(vpu_rf_ptr)
unum_t done_ten_seq_ptr = free_tensor_sequence(ten_seq_ptr)
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
if (set_x31 == 1):
   backup_x31()
endif
   backup_rx(imm_rsx = ba_reg_amo)
   ow_unpred_fregs(fregs_mask = ow_fregs)

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

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Config Phase

if (set_tmask == 1):
   set_tensor_mask(value = tensor_mask_val, rega = rx1)
endif

if (mem_occ > 0):
   la rx_mem, op_mem[l_mid]
   ; As we use gmemory_t, randomly add an offset to not start always with the same address
   SELECT 1 {
      1 : addi rx_mem, rx_mem, 0
      1 : addi rx_mem, rx_mem, 64
      1 : addi rx_mem, rx_mem, 128
      1 : addi rx_mem, rx_mem, 192
      1 : addi rx_mem, rx_mem, 256
   }
endif

   prepare_tensor_csr(pb_csr = tensor_op0_csr, pb_csr_val = tensor_op0_csr_val, pb_op_type = tensor_op0_type, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = tensor_op0_mem_off)
   prepare_tensor_csr(pb_csr = tensor_op1_csr, pb_csr_val = tensor_op1_csr_val, pb_op_type = tensor_op1_type, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = tensor_op1_mem_off)
   prepare_tensor_csr(pb_csr = tensor_op2_csr, pb_csr_val = tensor_op2_csr_val, pb_op_type = tensor_op2_type, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = tensor_op2_mem_off)
   prepare_tensor_csr(pb_csr = tensor_op3_csr, pb_csr_val = tensor_op3_csr_val, pb_op_type = tensor_op3_type, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = tensor_op3_mem_off)
   prepare_tensor_csr(pb_csr = tensor_op4_csr, pb_csr_val = tensor_op4_csr_val, pb_op_type = tensor_op4_type, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = tensor_op4_mem_off)

   write_tensor_wait(pb_wait_type = pre_tensor_wait)

if (set_x31 == 1):
   li x31, x31_val
   add rx_mem, x0, x31
endif

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase

   ; OP0
   inline_comb_inst(pb_inst0_val = ps_op0_val, pb_inst1_val = amo_op0_val)
   write_tensor_csr(pb_csr = tensor_op0_csr, pb_op_type = tensor_op0_type)

   ; OP1
   inline_comb_inst(pb_inst0_val = ps_op1_val, pb_inst1_val = amo_op1_val)
   write_tensor_wait(pb_wait_type = tensor_op1_tw_type)
   write_tensor_csr(pb_csr = tensor_op1_csr, pb_op_type = tensor_op1_type)

   ; OP2
   inline_comb_inst(pb_inst0_val = ps_op2_val, pb_inst1_val = amo_op2_val)
   write_tensor_wait(pb_wait_type = tensor_op2_tw_type)
   write_tensor_csr(pb_csr = tensor_op2_csr, pb_op_type = tensor_op2_type)

   ; OP3
   inline_comb_inst(pb_inst0_val = ps_op3_val, pb_inst1_val = amo_op3_val)
   write_tensor_wait(pb_wait_type = tensor_op3_tw_type)
   write_tensor_csr(pb_csr = tensor_op3_csr, pb_op_type = tensor_op3_type)

   ; OP4
   inline_comb_inst(pb_inst0_val = ps_op4_val, pb_inst1_val = amo_op4_val)
   write_tensor_wait(pb_wait_type = tensor_op4_tw_type)
   write_tensor_csr(pb_csr = tensor_op4_csr, pb_op_type = tensor_op4_type)

   inline_comb_inst(pb_inst0_val = ps_op5_val, pb_inst1_val = amo_op5_val)
   if (clean_tenb != 0):
      li tensor_op0_csr, clean_tenb
      write_tensor_wait(pb_wait_type = clean_tenb_twait)
      csrrw x0, tensor_fma, tensor_op0_csr
   endif

   if (ET_TEST_ENV.STING_RAND == 1):
      write_tensor_wait(pb_wait_type = post_ten_waits)
   endif
   reset_tensor_error()

   restore_rx(imm_rsx = ba_reg_amo)
if ET_TEST_ENV.SILICON_MODE == 1:
   clean_amo_rd(pb_rn0 = rn0 , pb_rn1 = rn1)
endif
if (set_x31 == 1):
   restore_x31()
endif



;
; Similarly the 'snip_check' section is invoked at the end of the test after all
; the hardware threads are done with their random code section. It can be used to
; carry out any test specific checks like verifying the contents of a memory buffer
; against the expected values.
;
snip_check
   ; Empty
