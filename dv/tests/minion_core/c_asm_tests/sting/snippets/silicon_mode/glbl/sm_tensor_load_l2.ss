
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
unum_t cpuid = snp_cpu_id()
unum_t rand_init = snp_randnum()

; Tensor Mask
unum_t  tl_l2scp_mask_val   = snp_randnum()

; Tensor Load
unum_t  tl_l2scp_csr_base   = snp_randnum()
unum_t  x31_val_rand        = snp_randnum()
unum_t  x31_val_masked      = snp_and(x31_val_rand,     0xffff00000000003f)
unum_t  mem_stride          = snp_urandrange(0, 3)
unum_t  mem_stride_lsh      = snp_lshift(mem_stride, 6)
unum_t  x31_val             = snp_or(x31_val_masked,    mem_stride_lsh)

; Only access valid L2 SCP Lines
unum_t  tl_l2scp_csr_val    = snp_constrain_l2_lines(tl_l2scp_csr_base)
unum_t  pre_tensor_wait     = snp_get_tl_l2_pre_ten_waits(tl_l2scp_csr_val)

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 2(MAX_STRIDE) = 2048)
; SET defined in $MINION_DIAGS/sting/conf/utils/et_minion_shire_mem.conf
gmemory_t op_mem[41216]@64:ET_SEQ_MEM_SET
gmemory_t selfcheck_info[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION


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
@cpu: cpu_x

   snippet_start(random_num = rand_init, rega = rx1, regb = rx2)

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Backup Phase
   backup_x31()



   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Config Phase
if ((tl_l2scp_csr_val & 0x8000000000000000) != 0):
   ; [Setup] For Tensor Loads, if bit 63 is set, we are using Tensor Mask (PRM-10)
   set_tensor_mask(value = tl_l2scp_mask_val, rega = csr_reg)
endif

   ; [x31] Set stride (bit [47:4])
   ; We do not mask because the other fields are ignored
   li  x31, x31_val

   ; [CSR] Set the register to random values and keep the ones we need
   li  csr_reg, tl_l2scp_csr_val
   li  rx2, 0xffff00000000003f
   and csr_reg, csr_reg, rx2

   ; [CSR] Mask the VA and merge it with the CSR value
   la  rx1, op_mem
   ; As we use gmemory_t, randomly add an offset to not start always with the same address
   SELECT 1 {
      1 : RANDINST 0:0
      1 : addi rx1, rx1, 64
      1 : addi rx1, rx1, 128
      1 : addi rx1, rx1, 192
      1 : addi rx1, rx1, 256
   }
   li  rx2, 0x0000ffffffffffc0
   and rx1, rx1, rx2
   or  csr_reg, csr_reg, rx1

   sm_set_tl_l2_info(regx1 = rx1, regx2 = rx2, et_info_addr = selfcheck_info[cpuid], csr_enc_reg = csr_reg, x31_reg = x31)

   write_tensor_wait(pb_wait_type = pre_tensor_wait)

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase
   csrrw rx1, tensor_load_l2scp, csr_reg



   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Self-Check Return is 0
   beq    x0, rx1, csr_return_is_0
   test_fail(regz = rx1)
csr_return_is_0:
   selfcheck_tl_l2(et_info_addr = selfcheck_info[cpuid])



   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Restore Phase
   restore_x31()


;
; Similarly the 'snip_check' section is invoked at the end of the test after all
; the hardware threads are done with their random code section. It can be used to
; carry out any test specific checks like verifying the contents of a memory buffer
; against the expected values.
;
snip_check
   ; Empty

