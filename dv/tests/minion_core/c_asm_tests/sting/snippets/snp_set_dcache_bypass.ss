;
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"

resource

unum_t index = 0
axreg_t ax1, ax2, ax3

snip_setup
   ; Empty
snip_init
   ; Empty

snip_run
@cpu: 0
   et_jump_to_m_mode()

snip_check
    ; Empty

snip_handler
mhandler@cpu: 0
   set_dcache_bypass(loop_index = index, tmp_reg0 = ax1, tmp_reg1 = ax2, tmp_reg2 = ax3)
   csrr ax1, mepc
   addi ax1, ax1, 4
   csrw mepc, ax1
