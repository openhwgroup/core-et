; This snippet runs on a random mask of HARTs in the shire.
; Each participant sends a credit to everyone (including itself) and will then
; consume as many credits as there are participants.
;
import "$MINION_DIAGS/sting/snippets/common.pb"

resource

unum_t  rand_init = snp_randnum_glbl()
unum_t  cpuid = snp_cpu_id()
unum_t  index = 0
axreg_t ax1, ax2, ax3

unum_t fcc_cntr = snp_urandrange_glbl(0,2)
unum_t active_num_shires = snp_get_num_active_shires()
unum_t idx_sid
unum_t fcc_esr_addr
unum_t even_mask
unum_t odd_mask

unum_t mask_out_participants = 1
unum_t per_shire_participants_mask = snp_randnum_glbl()
unum_t per_shire_odd_participants_mask  = snp_get_odd_bits(per_shire_participants_mask)
unum_t per_shire_even_participants_mask = snp_get_even_bits(per_shire_participants_mask)
unum_t total_players  = snp_get_fcc_total_players(per_shire_participants_mask)

unum_t phy_cpuid_shire = snp_get_phycpu_shire(cpuid)
unum_t do_something = snp_bit_set_in_mask(phy_cpuid_shire, per_shire_participants_mask)

snip_setup
   ; Empty
snip_init
   ; Empty

snip_run

@cpu: *
   snippet_start(random_num = rand_init, rega = ax1, regb = ax2)

   if (do_something > 0):

      for index in (0, active_num_shires, 1):
         ; Get shireid corresponding to 'index'
         idx_sid = snp_get_shireid_from_set_idx(index)

         ; Get the FCC ESR addr
         fcc_esr_addr = snp_get_fcc_addr(idx_sid, 0, fcc_cntr)

         ; Get active minion's thread mask
         even_mask = snp_get_fcc_mask(idx_sid, 0)
         odd_mask  = snp_get_fcc_mask(idx_sid, 1)
         if (mask_out_participants == 1):
            even_mask = snp_and(per_shire_even_participants_mask, even_mask)
            odd_mask  = snp_and(per_shire_odd_participants_mask, odd_mask)
         endif

         li ax1, fcc_esr_addr
         li ax2, even_mask
         li ax3, odd_mask

         sd ax2, 0  (ax1)
         sd ax3, 16 (ax1)
      endfor

      for index in (1, total_players, 1):
         csrwi fcc, fcc_cntr
      endfor

   endif

snip_check
    ; Empty

