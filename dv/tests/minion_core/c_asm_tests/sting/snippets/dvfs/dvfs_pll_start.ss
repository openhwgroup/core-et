
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/dvfs/dvfs.pb"

resource
unum_t cpuid = snp_cpu_id()
;unum_t x = snp_urandrange(0, BOARD_CONF.NUM_CPUS)

axreg_t rx1

snip_setup
    ; Empty


snip_init
@cpu: ET_TEST_ENV.DVFS_PLL_HART

   if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
      jump_to_dvfs_api(pb_rx1 = rx1, pb_c_api_addr = sting_init_dvfs)
   else: ; Call mhandler
      et_jump_to_m_mode()
   endif

snip_run
    ; Empty

snip_check
   ; Empty

snip_handler
mhandler@cpu: ET_TEST_ENV.DVFS_PLL_HART
   jump_to_dvfs_api(pb_rx1 = rx1, pb_c_api_addr = sting_init_dvfs)
exit_handler:
   ; Skip instruction that trapped
   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1
