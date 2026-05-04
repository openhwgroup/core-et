
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource 

; Evict SW
unum_t  evict_csr_val  = snp_randnum()
unum_t  l1_scp_csr_val = snp_randnum()

axreg_t csr_reg 
axreg_t rx1     
axreg_t rx2     

snip_setup 
    ; Empty
    ; For current Sting version, this is a placeholder


snip_init 
@cpu: 0,2 .. BOARD_CONF.NUM_CPUS
; ET_TEST_ENV.SCP_ENABLE is defined in et.conf
if (ET_TEST_ENV.SCP_ENABLE == 1):
if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
   enable_l1scp(regx1 = rx1, regx2 = rx2, en_l1scp_pass_label = snip_l1scp_pass)

snip_l1scp_pass:
else: ; Call mhandler
   et_jump_to_m_mode()
endif
endif

snip_run
   ; Empty

snip_check 
   ; Empty

snip_handler
mhandler@cpu: *

   enable_l1scp(regx1 = rx1, regx2 = rx2, en_l1scp_pass_label = exit_handler)

exit_handler:
    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1
