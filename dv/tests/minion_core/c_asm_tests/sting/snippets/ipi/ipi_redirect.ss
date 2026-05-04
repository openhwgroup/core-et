
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/ipi/ipi.pb"

resource 

unum_t rand_num = snp_randnum_glbl()
unum_t cpuid = snp_cpu_id()
unum_t is_reset_cpu = snp_is_reset_cpu(cpuid)

; Using a random barrier to sync all the HARTs
unum_t barrier = glb_snp_get_ipi_flb_num(ET_TEST_ENV.STING_RAND, cpuid, EOT_ENV.FLB_NUM)
; snp_get_flb_addr() will return barrier's FLB address with the correct Shire bits set
unum_t  flb_addr = snp_get_flb_addr(barrier)
; flb_match_val corresponds to the total number of HARTS in the same Neigh as 'cpuid'
unum_t flb_match_val = snp_get_ipi_flb_match_val(ET_TEST_ENV.STING_RAND, cpuid)
; snp_get_flb_csr_value_cov() returns the value to write into the ESR address considering that we are using the random barrier chosen above
unum_t flb_csr_val = snp_get_flb_csr_value_cov(barrier, flb_match_val)

; IPI Redirect Trigger
unum_t esr_addr_redirect_trigger = snp_get_redirect_trigger_addr(cpuid)
unum_t ipi_redirect_trigger_val  = snp_get_ipi_redirect_trigger_value(ET_TEST_ENV.STING_RAND, cpuid, IPI_REDIRECT.AND_MODE)

; Randomly chose if trigger from User or Machine mode
unum_t sm_mode = snp_urandrange(0, 2)

axreg_t rx1, rx2, rx3, rx4, bak_x12


;we always store to X12 register

snip_setup 
    ; Empty
    ; For current Sting version, this is a placeholder


snip_init 
@cpu: *
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; Reset FLB Barrier
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
if (is_reset_cpu == 1):
    li rx1, flb_addr
    sd x0, 0(rx1)
    fence iorw, iorw
endif





snip_run 
@cpu: *
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; WATERMARK
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   snippet_start(random_num = rand_num, rega = rx1, regb = rx2)


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; Backup register x12
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   add bak_x12, x0, x12
   la rx3, redirect_return_pc

   
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; FLB Synchronization
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ; Check the Barrier
   li rx1, flb_csr_val
   csrrw rx1, flb0, rx1
   ; If I am not the last one, stall
   ; otherwise, IPI Redirect PC
   bne x0, rx1, trigger_ipi

wait_ipi:
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; Wait for IPI Redirect
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   add x12, x0, rx3 ; rx3 contains redirect_return_pc
   csrw stall, x0

trigger_ipi:
if ET_TEST_ENV.SILICON_MODE == 0:
   RANDINST 200:300
else:
   li rx4, 300
ipi_redirect_nops:
   add x0, x0, x0
   addi rx4, rx4, -1
   bne rx4, x0, ipi_redirect_nops
endif
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; Set IPI Redirect Trigger
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

   ;; This snippet only runs in User mode
if (sm_mode == 1):
   ;; But if we want to test the S/M Mode case
if (IPI_REDIRECT.DELEGATE_TO_S == 1):
   li x10, 2
   ecall
else:
   et_jump_to_m_mode()
endif
else:
   ;; User Mode case
   set_ipi_redirect_trigger(redirect_trigger_val = ipi_redirect_trigger_val, ipi_esr_addr = esr_addr_redirect_trigger, regx1 = rx1, regx2 = rx2)
endif



redirect_return_pc:


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; Retore register x12
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   add x12, x0, bak_x12


   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; WATERMARK
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   snippet_end(random_num = rand_num, rega = rx1, regb = rx2)



snip_check 
   ; Empty

snip_handler
shandler@cpu: *

    set_ipi_redirect_trigger(redirect_trigger_val = ipi_redirect_trigger_val, ipi_esr_addr = esr_addr_redirect_trigger, regx1 = rx1, regx2 = rx2)

    ; Skip instruction that trapped
    csrr rx1, sepc
    addi rx1, rx1, 4
    csrw sepc, rx1

mhandler@cpu: *

    set_ipi_redirect_trigger(redirect_trigger_val = ipi_redirect_trigger_val, ipi_esr_addr = esr_addr_redirect_trigger, regx1 = rx1, regx2 = rx2)

    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1
