
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/ipi/ipi.pb"

resource 

cpu_t x

unum_t cpuid = snp_cpu_id()

; IPI Redirect Trigger
unum_t esr_addr_redirect_trigger = snp_get_redirect_trigger_addr(cpuid)
axreg_t rx1, rx2


snip_setup 
   ; Empty
   ; For current Sting version, this is a placeholder

snip_init 
   ; Empty

snip_run 
@cpu: x
   li rx1, esr_addr_redirect_trigger
   ld rx2, 0(rx1)

   ; FUTURE: Add the possibility to disable self-check
ipi_redirect_read_self_check:
   beq rx2, x0, ipi_redirect_read_pass
ipi_redirect_read_fail:
   test_fail(regz = rx1)

ipi_redirect_read_pass:


snip_check 
   ; Empty
