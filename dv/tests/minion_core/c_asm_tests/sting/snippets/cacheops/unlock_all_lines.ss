import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/cacheops/cops.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource

unum_t DCACHE_NSETS = 16
unum_t DCACHE_NWAYS = 4

axreg_t rx1
axreg_t rx2
axreg_t rx3
axreg_t rx4

snip_setup
    ; Empty

snip_init
@cpu: 0,2 .. BOARD_CONF.NUM_CPUS
if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE:
   unlock_all_sw(regx1 = rx1, regx2 = rx2, regx3 = rx3, regx4 = rx4, num_sets = DCACHE_NSETS, num_ways = DCACHE_NWAYS)
else:
   et_jump_to_m_mode()
endif

snip_run
   ; Empty

snip_check
   ; Empty


snip_handler
mhandler@cpu: 0,2 .. BOARD_CONF.NUM_CPUS
   unlock_all_sw(regx1 = rx1, regx2 = rx2, regx3 = rx3, regx4 = rx4, num_sets = DCACHE_NSETS, num_ways = DCACHE_NWAYS)
   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1

