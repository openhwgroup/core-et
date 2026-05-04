; This snippet runs on a random mask of HARTs in the shire.
; Each participant in one shire issue the FLB and then waits FCC credits. 
; The last participant to reach the barrier gives FCC credits to everybody else.

import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/flb_fcc/flb_fcc.pb"

resource
cpu_t x

unum_t  index = 0
axreg_t rx1, rx2, rx4

unum_t val = snp_urandrange(0, 5)
unum_t rn0 = snp_randnum()
unum_t rn1 = snp_randnum()
unum_t rn2 = snp_randnum()

unum_t  counter_select  = snp_urandrange_glbl(0,2)

unum_t  cpuid = snp_cpu_id()
unum_t  thread_id = snp_and(cpuid, 0x00001)

unum_t barrier = glbl_snp_get_flb_barrier_num(cpuid, EOT_ENV.FLB_NUM)

unum_t  flb_addr = snp_get_flb_addr(barrier)
unum_t  fcc_mask = snp_get_fcc_mask_local_shire_same_tid()
unum_t  fcc_addr = snp_get_fcc_addr_local_shire_same_tid(counter_select)

;;;;;;;;;; FOR COVERAGE ;;;;;;;;;
unum_t  num_minions   = snp_rshift(BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP, 1)
unum_t  flb_match_val = snp_urandrange_glbl(num_minions, 256)
unum_t  num_flb       = snp_get_num_flb_cov(flb_match_val)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

unum_t  flb_csr_val = snp_get_flb_csr_value_cov(barrier, flb_match_val)

unum_t  is_reset_cpu = snp_is_reset_cpu(cpuid)
unum_t rand_init = snp_randnum_glbl()

snip_setup
   ; Empty

snip_init

@cpu: *
    ; Reset FLB Barrier
if (is_reset_cpu == 1):
    li rx1, flb_addr
    sd x0, 0(rx1)
    fence iorw, iorw
endif

snip_run

@cpu: *

    snippet_start(random_num = rand_init, rega = rx1, regb = rx2)


    ; Check the Barrier
    li rx1, flb_csr_val

    for index in (0, num_flb, 1):
       csrrw rx4, flb0, rx1
    endfor

    ; If I am the last one, give credit to others
    ; otherwise, wait for a credit
    bne x0, rx4, give_credit

wait_for_credit:
    SELECT 1 {
        2 : do_fcc_nb(rega = rx1, regb = rx2, counter_fccnb = counter_select, rand_num_inst = val)
        7 : do_fcc(regc = rx1, counter_fcc = counter_select)
    }

    j done_flb_fcc

give_credit:
    li rx1, fcc_addr
    li rx2, fcc_mask
    sd rx2, 0(rx1)

done_flb_fcc: 
    if ET_TEST_ENV.SILICON_MODE == 1:
       li rx1, rn0
       li rx2, rn1
       li rx4, rn2
    endif

snip_check
    ; Empty

