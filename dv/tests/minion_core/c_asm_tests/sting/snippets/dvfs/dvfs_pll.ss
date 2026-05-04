
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/dvfs/dvfs.pb"

resource
unum_t cpuid = snp_cpu_id()
unum_t x = snp_urandrange(0, BOARD_CONF.NUM_CPUS)
unum_t rand_freq = snp_urandrange(300, 1001) ; random number between 300 and 1000, it represents the target frequency in MHz

axreg_t rx1, rx2
memory_t snp_info[64]@64

snip_setup
    ; Empty


snip_init
@cpu: ET_TEST_ENV.DVFS_PLL_HART
   la rx1, snp_info
   li rx2, rand_freq
   sd rx2, 0(rx1)

snip_run
@cpu: ET_TEST_ENV.DVFS_PLL_HART
   if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
      jump_to_dvfs_api_with_arg(pb_c_api_addr = sting_update_minion_pll_freq, pb_c_arg_addr = snp_info)
   else: ; Call mhandler
      et_jump_to_m_mode()
   endif

snip_check
   ; Empty

snip_handler
mhandler@cpu: ET_TEST_ENV.DVFS_PLL_HART
   jump_to_dvfs_api_with_arg(pb_c_api_addr = sting_update_minion_pll_freq, pb_c_arg_addr = snp_info)
exit_handler:
   ; Skip instruction that trapped
   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1
