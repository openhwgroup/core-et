; This snippet should be run in TEST_START to randomly select
; the cache mode between Shared/Split/Scratchpad.

import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource

; Evict SW
unum_t  evict_csr_val  = snp_randnum()
unum_t  l1_scp_csr_val = snp_randnum()
unum_t  dcache_mode    = snp_urandrange(0,3)

axreg_t csr_reg
axreg_t rx1
axreg_t rx2

snip_setup
    ; Empty

snip_init
   ; Empty

snip_run
@cpu: 0,2 .. BOARD_CONF.NUM_CPUS
if (dcache_mode == 2):
   if (ET_TEST_ENV.SCP_ENABLE == 1):
      et_jump_to_m_mode()
   endif
elif (dcache_mode == 1):
   if (ET_TEST_ENV.SPLIT_ENABLE == 1):
      et_jump_to_m_mode()
   endif
endif

snip_check
   ; Empty

snip_handler
mhandler@cpu: 0,2 .. BOARD_CONF.NUM_CPUS
if (dcache_mode == 2):
   exclusive_mode(set_it = 1, previous = x0)
   evict_all_l1(value = evict_csr_val, rega = csr_reg, regb = rx1, regc = rx2)
   set_l1_scp(value = l1_scp_csr_val, rega = csr_reg, regb = rx1)
   exclusive_mode(set_it = 0, previous = x0)
elif (dcache_mode == 1):
   exclusive_mode(set_it = 1, previous = x0)
   evict_all_l1(value = evict_csr_val, rega = csr_reg, regb = rx1, regc = rx2)
   set_l1_split(value = l1_scp_csr_val, rega = csr_reg, regb = rx1)
   exclusive_mode(set_it = 0, previous = x0)
endif
   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1

