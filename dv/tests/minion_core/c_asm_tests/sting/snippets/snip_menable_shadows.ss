
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource

axreg_t rx1

snip_setup
    ; Empty


snip_init
@cpu: *
   if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
      csrrwi x0, 0x7d2, 1 ; menable_shadows
   else: ; Call mhandler
      et_jump_to_m_mode()
   endif

snip_run
    ; Empty

snip_check
   ; Empty

snip_handler

mhandler@cpu: *

   if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
      ; We should never trap in Machine mode
      test_fail(regz = rx1)
   else:
      csrrwi x0, 0x7d2, 1 ; menable_shadows
   endif

   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1
