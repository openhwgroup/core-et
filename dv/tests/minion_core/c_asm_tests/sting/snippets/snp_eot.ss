; This snippet synchronises every HART then signify test pass to the DV environment.
; It is intended to be uses at the end of the sting test.
;

import "$MINION_DIAGS/sting/snippets/flb_fcc/flb_fcc.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource

unum_t  index = 0
axreg_t ax1, ax2, ax3, ax4

unum_t fcc_cntr = snp_urandrange_glbl(0,2)
unum_t active_num_shires = snp_get_num_active_shires()
unum_t idx_sid
unum_t fcc_esr_addr
unum_t even_mask
unum_t odd_mask

unum_t total_players = BOARD_CONF.NUM_CPUS

snip_setup
   ; Empty
snip_init
   ; Empty

snip_run

@cpu: *

   ; Wait for any long operation to be done (memory accesses, tensors, cacheops...)
   fence iorw, iorw
   csrwi tensor_wait, 0
   csrwi tensor_wait, 1
   csrwi tensor_wait, 2
   csrwi tensor_wait, 3
   csrwi tensor_wait, 4
   csrwi tensor_wait, 5
   csrwi tensor_wait, 6
   csrwi tensor_wait, 7
   csrwi tensor_wait, 8
   csrwi tensor_wait, 9
   csrwi tensor_wait, 10

   ; FCC Barrier
   for index in (0, active_num_shires, 1):
      ; Get shireid corresponding to 'index'
      idx_sid = snp_get_shireid_from_set_idx(index)

      ; Get the FCC ESR addr
      fcc_esr_addr = snp_get_fcc_addr(idx_sid, 0, fcc_cntr)

      ; Get active minion's thread mask
      even_mask = snp_get_fcc_mask(idx_sid, 0)
      odd_mask  = snp_get_fcc_mask(idx_sid, 1)

      li ax1, fcc_esr_addr
      li ax2, even_mask
      li ax3, odd_mask

      sd ax2, 0  (ax1)
      sd ax3, 16 (ax1)
   endfor

   for index in (1, total_players, 1):
      csrwi fcc, fcc_cntr
   endfor


   ; Make sure FCC counter is 0
   if (ET_TEST_ENV.EOT_SELF_CHECK_FCC == 1):
      check_fcc:
         flb_fcc_self_check(rega = ax1, regb = ax2, counter_fccnb = 0)
         flb_fcc_self_check(rega = ax1, regb = ax2, counter_fccnb = 1)
   endif

   if ET_TEST_ENV.SILICON_MODE == 0:
      ; Signal the End of Test through validation0 CSR
      lui   ax1, 0x1FEED
      csrw  validation0, ax1

      if ET_TEST_ENV.EOT_ZEBU_HINTS == 1:
         zebu_pass()
      endif
   endif

   ; Wait for the End of the Simulation
   wait_for_sim_end()


snip_check
    ; Empty


snip_handler
shandler@cpu: *
   wfi
   csrr ax1, mepc
   addi ax1, ax1, 4
   csrw mepc, ax1

mhandler@cpu: *
   wfi
   csrr ax1, mepc
   addi ax1, ax1, 4
   csrw mepc, ax1
