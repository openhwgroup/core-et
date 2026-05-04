
import "$MINION_DIAGS/sting/snippets/tensor/tensor_fma.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"


; This is a resource block to will use in TEST_END option sting.
; The purpose of this snippet is consuming the tensor loaded in B by TFMA .
; It avoids test failing by Co-sim errors by instruction pendings
resource

; CSR Random Value
unum_t  csr_val_rand = snp_randnum()

unum_t tfma_scp_csr_base   = snp_set_valid_tfma_op(csr_val_rand, 0)
unum_t tfma_scp_csr_ima8   = snp_tfma_set_ima8a32(tfma_scp_csr_base)
unum_t tfma_scp_csr_val    = snp_or(tfma_scp_csr_ima8, 0x01F8800000000001)

; Tensor Mask Value
unum_t  tl_mask_val   = snp_randnum()
; Tensor Wait
;unum_t  ten_wait_en   = snp_urandrange(0,2)
unum_t  ten_wait_en   = 1

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

unum_t     cpuid = snp_cpu_id()
gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION

snip_setup
;Empty

snip_init
@cpu: 0,2 .. BOARD_CONF.NUM_CPUS
if (ET_TEST_ENV.SCP_ENABLE == 1):
   tensor_fma_snip_run(csr_regx = csr_reg, regxa = rx1, regxb = rx2, rand_csr_val = tfma_scp_csr_val, ten_mask_val = tl_mask_val, ten_wait = ten_wait_en, backup_mem = mem_fp, hartid = cpuid)
endif

snip_run
;Empty

snip_check
;Empty
