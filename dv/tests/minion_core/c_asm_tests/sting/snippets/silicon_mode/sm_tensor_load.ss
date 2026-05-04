
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
unum_t  rand_start = snp_randnum()


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Load
unum_t  tensor_mask_val = snp_randnum()
unum_t  tl_csr_val = snp_get_tload_wrapper()
; FUTURE: support stride greater than 1
unum_t  tl_x31_val = glbl_snp_get_tload_x31_wrapper(2)

unum_t  pre_tensor_wait = snp_get_tl_pre_ten_waits(tl_csr_val)

axreg_t tl_x31
axreg_t tl_csr

axreg_t rx1
axreg_t rx2
axreg_t rx3

unum_t tl_off_rand = snp_urandrange(0, 32)
unum_t tl_off = snp_lshift(tl_off_rand, 6) ; max offset of 1984

; Export data from L1SCP to MEM using TS SCP
; This MEM will be read by the self-checking happening at the end
;unum_t ts_csr_val = snp_get_tstore_from_tlcsr(tl_csr_val)
;unum_t ts_start_scp_line = snp_get_ts_scp_start_line(ts_csr_val)

gmemory_t op_mem[41216]@64:ET_SEQ_MEM_SET

memory_t sm_pre_data[3072]@64:ET_DRAM_REGION
memory_t sm_post_data[3072]@64:ET_DRAM_REGION
memory_t sm_tl_info[128]@64:ET_DRAM_REGION


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

   ; Tensor Load
if ((tl_csr_val & 0x8000000000000000) != 0) :
   ; [Setup] For Tensor Loads, if bit 63 is set, we are using Tensor Mask (PRM-10)
   set_tensor_mask(value = tensor_mask_val, rega = tl_csr)
endif
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

if ((cpu_id%2) == 0):
   dump_l1scp(pb_rx1 = rx1, pb_rx2 = rx2, pb_rx3 = rx3, pb_mem_addr = sm_pre_data)
   sm_set_tl_info(regx1 = rx1, regx2 = rx2, et_info_addr = sm_tl_info, csr_enc_reg = tl_csr, x31_reg = tl_x31, pre_data_addr = sm_pre_data, post_data_addr = sm_post_data)
endif

   write_tensor_wait(pb_wait_type = pre_tensor_wait)

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase
   seq_tl (_cpu_id = cpu_id, _rx1 = rx1, _tl_csr = tl_csr, _tl_x31 = tl_x31, _self_check = 1)


if ((cpu_id%2) == 0):
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; Self-Check Return is 0
      bne    x0, tl_x31,   tensor_return_not_0
      j tensor_return_is_0
   tensor_return_not_0:
      test_fail(regz = rx1)
   tensor_return_is_0:


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Self-Check Tensor Error
   selfcheck_tensor_load:
      tensor_load_wait(x31_reg_value = tl_x31_val)
      check_tensor_load_error(num_tload_value = tl_csr_val, rega = rx1, regb = tl_x31, reg_csr_val = tl_csr)
      j pass_sequence
   fail_tensor_error:
      test_fail(regz = rx1)
   pass_sequence:

   dump_l1scp(pb_rx1 = rx1, pb_rx2 = rx2, pb_rx3 = rx3, pb_mem_addr = sm_post_data)
endif

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Restore Phase
   restore_x31()


snip_check
@cpu: cpu_x
if (ET_TEST_ENV.SCP_ENABLE == 1):
   if ((cpu_id%2) == 0):
      snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
      selfcheck_tl(et_info_addr = sm_tl_info)
   endif
endif


snip_handler
mhandler@cpu: cpu_x
    tensor_mhandler(rega = rx1, regb = rx2)
    csrr rx1, mepc
    addi rx1, rx1, 16
    csrw mepc, rx1
