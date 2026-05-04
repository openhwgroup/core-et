
import "$MINION_DIAGS/sting/snippets/tensor/tensor_fma.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource ;## STING PHASE
unum_t  cpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)

; ####################################################################
; ##                                                                ##
; ##                   COMMON VARIABLES                             ##
; ##                                                                ##
; ####################################################################
; Tensor Mask Value
unum_t  tfma_mask_val   = snp_randnum()
; Tensor Wait
;unum_t  ten_wait_en   = snp_urandrange(0,2)
unum_t  ten_wait_en   = 1
; CSR Random Value
unum_t  csr_val_rand = snp_randnum()

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

unum_t     cpuid = snp_cpu_id()
gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION


; ####################################################################
; ##                                                                ##
; ##                         TENSOR FMA                             ##
; ##                                                                ##
; ####################################################################
unum_t tfma_scp_csr_val  = snp_set_valid_tfma_op(csr_val_rand, 0)

snip_setup ;## STING PHASE
    ; Empty



snip_init ;## STING PHASE
   ; Empty
  


snip_run ;## STING PHASE
@cpu: cpu_x
   tensor_fma_snip_run(csr_regx = csr_reg, regxa = rx1, regxb = rx2, rand_csr_val = tfma_scp_csr_val, ten_mask_val = tfma_mask_val, ten_wait = ten_wait_en, backup_mem = mem_fp, hartid = cpuid)


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

