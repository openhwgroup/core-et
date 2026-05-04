
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource 

unum_t to_write_rand = snp_randnum() 
unum_t to_write      = snp_or(to_write_rand, 0x1)

unum_t cpuid = snp_cpu_id()
unum_t csr_val = snp_get_esr_addr_coop_mode(cpuid)
axreg_t csr_reg 
axreg_t rx1
axreg_t rx2

snip_setup 
    ; Empty
    ; For current Sting version, this is a placeholder

snip_init 
@cpu: *

if (ET_TEST_ENV.COOP_MODE_ENABLE == 1):
if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
   set_shire_coop_mode(rega = rx1, regc = csr_reg, esr_addr = csr_val, esr_value = to_write)
else: ; Call mhandler
   et_jump_to_m_mode()
endif
endif

   ; FUTURE: self check to make sure the value is set correctly

snip_run
   ; Empty

snip_check 
   ; Empty

snip_handler
mhandler@cpu: *

   set_shire_coop_mode(rega = rx1, regc = csr_reg, esr_addr = csr_val, esr_value = to_write)

exit_handler:
    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1
