
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/ipi/ipi.pb"

resource 

gnum_t first_ipi_redirect_filter[BOARD_CONF.NUM_CPUS]
unum_t cpuid = snp_cpu_id()

; IPI Redirect Filter
unum_t is_set_redirect_filter = snp_is_set_redirect_filter(cpuid)
unum_t esr_addr_redirect_filter = snp_get_ipi_redirect_filter_addr(cpuid)
unum_t ipi_redirect_filter = snp_get_ipi_redirect_filter(cpuid, IPI_REDIRECT.AND_MODE)

axreg_t rx1, rx2


snip_setup 
    ; Empty
    ; For current Sting version, this is a placeholder



snip_init 
@cpu: *
if (first_ipi_redirect_filter[cpuid] == 0):
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; Set IPI Redirect Filter
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
if (is_set_redirect_filter == 1):
   ; IPI redirect filter can only be set in Machine Mode
   et_jump_to_m_mode()
endif

   first_ipi_redirect_filter[cpuid] = snp_add(first_ipi_redirect_filter[cpuid], 1)
endif



snip_run 
   ; Empty
 


snip_check 
   ; Empty



snip_handler
mhandler@cpu: *

    ; IPI Redirect Filter can only be set by Machine Mode
    set_ipi_redirect_filter(redirect_filter = ipi_redirect_filter, ipi_esr_addr = esr_addr_redirect_filter, regx1 = rx1, regx2 = rx2)

    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1
