
import "$MINION_DIAGS/sting/snippets/tensor/tensor_store.pb"
import "$MINION_DIAGS/sting/snippets/sequences/sequences.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
unum_t  cpuid = snp_cpu_id()
unum_t rand_init = snp_randnum_glbl()

unum_t x31_val = glbl_snp_get_tload_x31_wrapper(3)
unum_t tensor_mask_val = snp_randnum()

unum_t TEN_SEQ_SIZE = 5
; Initialize data structures
unum_t l1scp_ptr = init_l1scp_ptr()
unum_t vpu_rf_ptr = init_vpu_rf_ptr()
unum_t ten_seq_ptr = init_tensor_sequence(TEN_SEQ_SIZE)

; Compute the sequences
unum_t ten_op_seq = glbl_snp_set_tensor_ops_sequence(l1scp_ptr, vpu_rf_ptr, ten_seq_ptr, TEN_SEQ_SIZE, rand_init)

unum_t op0_type     = get_csr_op_type_from_idx(0, ten_seq_ptr)
unum_t op0_csr_val = get_csr_op_value_from_idx(0, ten_seq_ptr)
unum_t op0_mem_off =   get_mem_offset_from_idx(0, ten_seq_ptr, x31_val)

unum_t op1_type      = get_csr_op_type_from_idx(1, ten_seq_ptr)
unum_t op1_csr_val  = get_csr_op_value_from_idx(1, ten_seq_ptr)
unum_t op1_tw_type = get_tenwait_value_from_idx(1, ten_seq_ptr, TEN_SEQ_SIZE)
unum_t op1_mem_off  =   get_mem_offset_from_idx(1, ten_seq_ptr, x31_val)
unum_t op1_use_x31 =          uses_x31_from_idx(1, ten_seq_ptr, TEN_SEQ_SIZE)

unum_t op2_type      = get_csr_op_type_from_idx(2, ten_seq_ptr)
unum_t op2_csr_val  = get_csr_op_value_from_idx(2, ten_seq_ptr)
unum_t op2_tw_type = get_tenwait_value_from_idx(2, ten_seq_ptr, TEN_SEQ_SIZE)
unum_t op2_mem_off  =   get_mem_offset_from_idx(2, ten_seq_ptr, x31_val)
unum_t op2_use_x31 =          uses_x31_from_idx(2, ten_seq_ptr, TEN_SEQ_SIZE)

unum_t op3_type      = get_csr_op_type_from_idx(3, ten_seq_ptr)
unum_t op3_csr_val  = get_csr_op_value_from_idx(3, ten_seq_ptr)
unum_t op3_tw_type = get_tenwait_value_from_idx(3, ten_seq_ptr, TEN_SEQ_SIZE)
unum_t op3_mem_off  =   get_mem_offset_from_idx(3, ten_seq_ptr, x31_val)
unum_t op3_use_x31 =          uses_x31_from_idx(3, ten_seq_ptr, TEN_SEQ_SIZE)

unum_t op4_type      = get_csr_op_type_from_idx(4, ten_seq_ptr)
unum_t op4_csr_val  = get_csr_op_value_from_idx(4, ten_seq_ptr)
unum_t op4_tw_type = get_tenwait_value_from_idx(4, ten_seq_ptr, TEN_SEQ_SIZE)
unum_t op4_mem_off  =   get_mem_offset_from_idx(4, ten_seq_ptr, x31_val)
unum_t op4_use_x31 =          uses_x31_from_idx(4, ten_seq_ptr, TEN_SEQ_SIZE)

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

; Post Silicon
unum_t ow_fregs = snp_ow_unpr_fregs(ten_seq_ptr, TEN_SEQ_SIZE, vpu_rf_ptr)

; Free data structures
unum_t done_l1scp_ptr = free_l1scp_ptr(l1scp_ptr)
unum_t done_vpu_rf_ptr = free_vpu_rf_ptr(vpu_rf_ptr)
unum_t done_ten_seq_ptr = free_tensor_sequence(ten_seq_ptr)


axreg_t rx_mem
axreg_t op0_csr
axreg_t op1_csr
axreg_t op2_csr
axreg_t op3_csr
axreg_t op4_csr

pxreg_t rx1
pxreg_t rx2

unum_t l_mid = snp_rshift(cpuid, 1)

; 41216 = 40960 + 256
gmemory_t op_mem[ET_TEST_ENV.NUM_MINIONS][41216]@64:ET_SEQ_MEM_SET



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
@cpu: *
   snippet_start(random_num = rand_init, rega = rx_mem, regb = rx1)

@cpu: 0,2..BOARD_CONF.NUM_CPUS
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Backup Phase
if (set_x31 == 1):
   backup_x31()
endif
   ow_unpred_fregs(fregs_mask = ow_fregs)


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

   prepare_tensor_csr(pb_csr = op0_csr, pb_csr_val = op0_csr_val, pb_op_type = op0_type, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = op0_mem_off)
   prepare_tensor_csr(pb_csr = op1_csr, pb_csr_val = op1_csr_val, pb_op_type = op1_type, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = op1_mem_off)
   prepare_tensor_csr(pb_csr = op2_csr, pb_csr_val = op2_csr_val, pb_op_type = op2_type, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = op2_mem_off)
   prepare_tensor_csr(pb_csr = op3_csr, pb_csr_val = op3_csr_val, pb_op_type = op3_type, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = op3_mem_off)
   prepare_tensor_csr(pb_csr = op4_csr, pb_csr_val = op4_csr_val, pb_op_type = op4_type, pb_rx_mem = rx_mem, pb_rx1 = rx1, pb_mem_off = op4_mem_off)

   write_tensor_wait(pb_wait_type = pre_tensor_wait)

if (set_x31 == 1):
   li x31, x31_val
   add rx_mem, x0, x31
endif

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase

   ; OP0
   write_tensor_csr(pb_csr = op0_csr, pb_op_type = op0_type)

   ; OP1
   seq_rand_inst_wrapper(pb_rx_x31 = rx_mem, pb_uses_x31 = op1_use_x31)
   write_tensor_wait(pb_wait_type = op1_tw_type)
   write_tensor_csr(pb_csr = op1_csr, pb_op_type = op1_type)

   ; OP2
   seq_rand_inst_wrapper(pb_rx_x31 = rx_mem, pb_uses_x31 = op2_use_x31)
   write_tensor_wait(pb_wait_type = op2_tw_type)
   write_tensor_csr(pb_csr = op2_csr, pb_op_type = op2_type)

   ; OP3
   seq_rand_inst_wrapper(pb_rx_x31 = rx_mem, pb_uses_x31 = op3_use_x31)
   write_tensor_wait(pb_wait_type = op3_tw_type)
   write_tensor_csr(pb_csr = op3_csr, pb_op_type = op3_type)

   ; OP4
   seq_rand_inst_wrapper(pb_rx_x31 = rx_mem, pb_uses_x31 = op4_use_x31)
   write_tensor_wait(pb_wait_type = op4_tw_type)
   write_tensor_csr(pb_csr = op4_csr, pb_op_type = op4_type)

   if (clean_tenb != 0):
      li op0_csr, clean_tenb
      write_tensor_wait(pb_wait_type = clean_tenb_twait)
      csrrw x0, tensor_fma, op0_csr
   endif

   if (ET_TEST_ENV.STING_RAND == 1):
      write_tensor_wait(pb_wait_type = post_ten_waits)
   endif
   reset_tensor_error()

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
