
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
unum_t  cpuid = snp_cpu_id()
unum_t  rand_start = snp_randnum_glbl()
gnum_t tl_coop_id[BOARD_CONF.NUM_CPUS]
unum_t tmp_coop_id
unum_t  th1_bias   = snp_urandrange(0, 27)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Load
unum_t  tensor_mask_val = snp_randnum()
unum_t  tl_csr_val = glbl_snp_get_coop_tload_wrapper()
; FUTURE: support stride greater than 4
unum_t  tl_x31_val = glbl_snp_get_tload_x31_wrapper(5)

unum_t  pre_tensor_wait = snp_get_tl_pre_ten_waits(tl_csr_val)

axreg_t tl_x31
axreg_t tl_csr

axreg_t rx1
axreg_t rx2
axreg_t rx3

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Tensor Coop Mask
; Between all the valid combinations, get a random Neigh Coop Mask
unum_t rand_neigh_mask   = glbl_snp_get_coop_neigh_mask()
; Between all the valid combinations, get a random Minion Coop Mask
unum_t rand_minion_mask  = glbl_snp_get_coop_minion_mask()
; Get a random value for the WARL(0) fields and 'Cooperation ID'
unum_t coop_mask_rand   = snp_randnum_glbl()
; Build the final value to be written into the Tensor Coop CSR
unum_t coop_mask_val    = snp_tensor_mask(BOARD_CONF.NUM_CPUS, coop_mask_rand, rand_neigh_mask, rand_minion_mask)
; ########################

unum_t  in_coop_mask = snp_cpu_in_mask(cpuid, coop_mask_val)

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 4(MAX_STRIDE) = 4096)
; Tensor Load Transpose 8 require more elements, therefore we have to multiply the size by 4
gmemory_t  ctl_mem2[16384]@64:ET_MINION_SHIRE_MEM_SET

; Export data from L1SCP to MEM using TS SCP
; This MEM will be read by the self-checking happening at the end

memory_t sm_pre_data[3072]@64:ET_DRAM_REGION
memory_t sm_post_data[3072]@64:ET_DRAM_REGION
memory_t sm_tl_info[128]@64:ET_DRAM_REGION


snip_setup ;## STING PHASE
    ; Empty

snip_init ;## STING PHASE
   ; Empty

snip_run ;## STING PHASE
@cpu: *

tmp_coop_id   = snp_and(tl_coop_id[cpuid], 0x01f)
coop_mask_val = snp_or(coop_mask_val, tmp_coop_id)

if ((cpuid%2) == 0):
   snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
endif

if ((in_coop_mask != 0) | ((cpuid%2 == 1) & (th1_bias == 0))):

   if ((cpuid%2) == 0):
      reset_tensor_error()
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
   la  rx1, ctl_mem2
   li  rx2, 0x0000ffffffffffc0
   and rx1, rx1, rx2
   or  tl_csr, tl_csr, rx1

   if ((cpuid%2) == 0):
      dump_l1scp(pb_rx1 = rx1, pb_rx2 = rx2, pb_rx3 = rx3, pb_mem_addr = sm_pre_data)
      sm_set_tl_info(regx1 = rx1, regx2 = rx2, et_info_addr = sm_tl_info, csr_enc_reg = tl_csr, x31_reg = tl_x31, pre_data_addr = sm_pre_data, post_data_addr = sm_post_data)
   endif

   write_tensor_wait(pb_wait_type = pre_tensor_wait)

   seq_tensor_coop(_cpu_id = cpuid, _rx1 = rx1, _coop_mask_val = coop_mask_val, _self_check = 1)

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Write to CSR Phase
   seq_tl (_cpu_id = cpuid, _rx1 = rx1, _tl_csr = tl_csr, _tl_x31 = tl_x31, _self_check = 1)

   if ((cpuid%2) == 0):
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

endif
   tl_coop_id[cpuid] = snp_add(tl_coop_id[cpuid], 1)


snip_check
@cpu: *
   ; Only check if you are taking part on the cooperation
if ((ET_TEST_ENV.SCP_ENABLE == 1) && (in_coop_mask != 0)):
   if ((cpuid%2) == 0):
      snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
      selfcheck_tl(et_info_addr = sm_tl_info)
   endif
endif



snip_handler
mhandler@cpu: *
    tensor_coop_mhandler(regx1 = rx1, regx2 = rx2, is_coop = 1)
    csrr rx1, mepc
    addi rx1, rx1, 16
    csrw mepc, rx1
