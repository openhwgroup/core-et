
import "$MINION_DIAGS/sting/snippets/tensor/tensor_ldst_mixed/tensor_ldst_mixed.pb"
import "$MINION_DIAGS/sting/snippets/sequences/sequences.pb"

resource ;## STING PHASE
unum_t  cpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)

; ####################################################################
; ##                                                                ##
; ##                   COMMON VARIABLES                             ##
; ##                                                                ##
; ####################################################################
; Tensor Mask Value
unum_t  tl_mask_val   = snp_randnum()
; Tensor Wait
;unum_t  ten_wait_en   = snp_urandrange(0,2)
unum_t  ten_wait_en   = 1
; CSR Random Value
unum_t  csr_val_rand = snp_randnum()

unum_t  rand_init         = snp_urandrange(0, 32)

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

unum_t  mem_offset_rand = snp_urandrange(0, 4096)
unum_t  mem_offset      = snp_and(mem_offset_rand,0xffffffffffffffc0)

unum_t  config_bias     = snp_urandrange(0, 27)

gmemory_t  mem_snp[BOARD_CONF.NUM_CPUS][16384]@64:ET_SEQ_MEM_SET
; ####################################################################


; ####################################################################
; ##                                                                ##
; ##                   TENSOR LOAD (MEM -> L1SCP)                   ##
; ##                                                                ##
; ####################################################################
unum_t  tl_csr_val_base = snp_and(csr_val_rand, 0xbfef00000000003f)
unum_t  tl_x31_rand       = snp_randnum()
unum_t  tl_x31_masked     = snp_and(tl_x31_rand, 0xffff00000000003f)
unum_t  tl_mem_stride     = snp_urandrange(0, 3)
unum_t  tl_mem_stride_lsh = snp_lshift(tl_mem_stride, 6)
unum_t  tl_x31_val        = snp_or(tl_x31_masked, tl_mem_stride_lsh)

; Tensor Load Transformation Bias to reduce the number of wrong transformations
unum_t  tl_csr_valid    = snp_ctl_set_valid_transformation(tl_csr_val_base)
unum_t  tl_csr_invalid  = snp_ctl_set_invalid_transformation(tl_csr_val_base)
unum_t  tl_csr_val      = snp_if_zero_else(config_bias, tl_csr_invalid, tl_csr_valid)
; ####################################################################


; ####################################################################
; ##                                                                ##
; ##                  TENSOR STORE (F.REGS -> MEM)                  ##
; ##                                                                ##
; ####################################################################
unum_t  ts_csr_val_base = snp_and(csr_val_rand, 0xfff800000000000f)
unum_t  ts_x31_rand     = snp_urandrange(1,9)

; Tensor Store Transformation Bias to reduce the number of wrong transformations
unum_t  ts_csr_valid    = snp_cts_set_valid_config(ts_csr_val_base, rand_init)
unum_t  ts_csr_invalid  = snp_cts_set_invalid_config(ts_csr_val_base, rand_init)
unum_t  ts_csr_val      = snp_if_zero_else(config_bias, ts_csr_invalid, ts_csr_valid)
unum_t  ts_x31_val = snp_get_ts_stride(ts_csr_val, ts_x31_rand)
unum_t  ts_fregs_mask   = get_tstore_ow_unpr_fregs(ts_csr_val)


; ####################################################################


; ####################################################################
; ##                                                                ##
; ##               TENSOR STORE FROM SCP(L1SCP -> MEM)              ##
; ##                                                                ##
; ####################################################################
unum_t  ts_scp_csr_val    = snp_and(csr_val_rand, 0xfffe00000000003f)
unum_t  ts_scp_x31_rand   = snp_urandrange(0,9)
unum_t  ts_scp_x31_val    = snp_lshift(ts_scp_x31_rand, 6)
; ####################################################################



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

    ow_unpred_fregs(fregs_mask = ts_fregs_mask)
    SELECT 1 {
        6 : tensor_store_snip_run(num_ts_csr_val = ts_csr_val, num_x31_val = ts_x31_val, memx = mem_snp[cpu_x], addr_off = mem_offset, csr_regx = csr_reg, regx1 = rx1, regx2 = rx2, regx3 = rx3, is_coop = 0, ten_wait = ten_wait_en)
        3 : tensor_load_mix(tl_value = tl_csr_val, tl_mask_value = tl_mask_val, x31_value = tl_x31_val, mem_value = mem_snp[cpu_x], mem_off_value = mem_offset, reg1 = csr_reg, reg2 = rx1, reg3 = rx2, reg4 = rx3)
        1 : tensor_store_from_scp_snip_run(num_ts_csr_val = ts_scp_csr_val, num_x31_val = ts_scp_x31_val, memx = mem_snp[cpu_x], addr_off = mem_offset, csr_regx = csr_reg, regx1 = rx1, regx2 = rx2, regx3 = rx3, ten_wait = ten_wait_en)
    }


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
;
snip_handler

mhandler@cpu: cpu_x
   tensor_mhandler(rega = rx1, regb = rx2)
   csrr rx1, mepc
   addi rx1, rx1, 16
   csrw mepc, rx1

