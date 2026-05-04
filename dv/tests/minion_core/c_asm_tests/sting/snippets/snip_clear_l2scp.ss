
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource

unum_t  rand_start = snp_randnum_glbl()
unum_t  cpuid = snp_cpu_id()
unum_t  index = 0
axreg_t rx1, rx2, rx3

unum_t  shire_id = snp_get_shire_id(cpuid)
unum_t  is_init_hart = snp_is_min_shire(cpuid)

unum_t fcc_cntr = snp_urandrange_glbl(0,2)
unum_t active_num_shires = snp_get_num_active_shires()
unum_t idx_sid
unum_t fcc_esr_addr
unum_t even_mask
unum_t odd_mask

unum_t total_players  = BOARD_CONF.NUM_CPUS

snip_setup
    ; Empty


snip_init
@cpu: *
   snippet_start(random_num = rand_start, rega = rx1, regb = rx2)
   if (is_init_hart == 1):
      if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
         clear_SCP_values(rega = rx1, regb = rx2, regc = rx3, shireid = shire_id)
      else: ; Call mhandler
         et_jump_to_m_mode()
      endif
   endif
   for index in (0, active_num_shires, 1):
      ; Get shireid corresponding to 'index'
      idx_sid = snp_get_shireid_from_set_idx(index)

      ; Get the FCC ESR addr
      fcc_esr_addr = snp_get_fcc_addr(idx_sid, 0, fcc_cntr)

      ; Get active minion's thread mask
      even_mask = snp_get_fcc_mask(idx_sid, 0)
      odd_mask  = snp_get_fcc_mask(idx_sid, 1)

      li rx1, fcc_esr_addr
      li rx2, even_mask
      li rx3, odd_mask

      sd rx2, 0  (rx1)
      sd rx3, 16 (rx1)
   endfor

   for index in (1, total_players, 1):
      csrwi fcc, fcc_cntr
   endfor

snip_run
    ; Empty

snip_check
   ; Empty

snip_handler

mhandler@cpu: *

   clear_SCP_values(rega = rx1, regb = rx2, regc = rx3, shireid = shire_id)

exit_handler:
    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1
