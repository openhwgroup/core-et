
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource ;## STING PHASE
unum_t  cpu_x = snp_get_hartid_biased(BOARD_CONF.NUM_CPUS)

; ####################################################################
; ##                                                                ##
; ##                   COMMON VARIABLES                             ##
; ##                                                                ##
; ####################################################################
; Tensor Wait
;unum_t  ten_wait_en   = snp_urandrange(0,2)
unum_t  ten_wait_en   = 1

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

unum_t     cpuid = snp_cpu_id()
gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION


; ####################################################################
; ##                                                                ##
; ##                         TENSOR QUANT                           ##
; ##                                                                ##
; ####################################################################
unum_t tquant_csr_val  = snp_get_tquant_wrapper()

snip_setup ;## STING PHASE
    ; Empty



snip_init ;## STING PHASE
   ; Empty
  


snip_run ;## STING PHASE
@cpu: cpu_x

if ((cpuid%2) == 0):
   backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)
endif
   ; Save x31
   addi rx2, x31, 0

   li csr_reg, tquant_csr_val
   ; Write to the CSR to trigger the Tensor Quant Operation 
   csrrw rx1, tensor_quant, csr_reg

if ((cpuid%2) == 0):
   beq    x0, rx1, tquant_csr_return_is_0

   test_fail(regz = rx1)

tquant_csr_return_is_0:
   ; Restore x31
   addi x31, rx2, 0

if (ten_wait_en == 1):
   tensor_quant_wait()
endif
else:
   ; Thread 1 must trap if writes to tensor_store
   ; if trapped, mret will skip test_fail()
   test_fail(regz = rx1)
endif

if ((cpuid%2) == 0):
   restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)
endif



snip_check
   ; Empty



snip_handler

mhandler@cpu: cpu_x
   tensor_mhandler(rega = rx1, regb = rx2)
   csrr rx1, mepc
   addi rx1, rx1, 16
   csrw mepc, rx1
