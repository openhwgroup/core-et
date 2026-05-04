
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/ipi/ipi.pb"

resource 

cpu_t x

unum_t cpuid = snp_cpu_id()

;; IPI Redirect Filter
unum_t esr_addr_redirect_filter = snp_get_ipi_redirect_filter_addr(cpuid)
unum_t ipi_redirect_filter = snp_randnum()

;; IPI Trigger
unum_t esr_addr_ipi_trigger = snp_get_trigger_addr(cpuid)
unum_t ipi_trigger_value = snp_randnum()

;; IPI Trigger Clear
unum_t esr_addr_ipi_trigger_clear = snp_get_trigger_clear_addr(cpuid)
unum_t ipi_trigger_clear_value = snp_randnum()

axreg_t rx1, rx2

snip_setup 
    ; Empty
    ; For current Sting version, this is a placeholder



snip_init 
   ; Empty



snip_run 
@cpu: x

   SELECT 1 {
      1 : set_ipi_redirect_filter(redirect_filter = ipi_redirect_filter, ipi_esr_addr = esr_addr_redirect_filter, regx1 = rx1, regx2 = rx2)
      1 : set_ipi_trigger(trigger_val = ipi_trigger_value, ipi_esr_addr = esr_addr_ipi_trigger, regx1 = rx1, regx2 = rx2)
      1 : set_ipi_trigger_clear(trigger_clear_val = ipi_trigger_clear_value, ipi_esr_addr = esr_addr_ipi_trigger_clear, regx1 = rx1, regx2 = rx2)
   }
   test_fail(regz = rx1)

snip_check 
   ; Empty

snip_handler

mhandler@cpu: x
    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 16
    csrw mepc, rx1
