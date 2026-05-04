
; This snippet only syncHARTs whithin one Shire

import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/flb_fcc/flb_fcc.pb"
import "$MINION_DIAGS/sting/snippets/amo.pb"

resource

unum_t  index = 0
axreg_t rx1, rx2

unum_t val = snp_urandrange(0, 5)

unum_t rn0 = snp_randnum()
unum_t rn1 = snp_randnum()

unum_t  fcc_cntr  = snp_urandrange_glbl(0,2)

unum_t  cpuid = snp_cpu_id()
unum_t  thread_id = snp_and(cpuid, 0x00001)

; flb_match_val corresponds to the total number of HARTS
unum_t  flb_match_val = snp_get_num_harts_local_shire(cpuid)
; Using a random barrier to sync all the HARTs
; This barrier is chosen in the et.conf file
; All the other snippets that make use of FLB will avoid choosing
; this same value for the FLB as it may hang the core
unum_t  barrier = EOT_ENV.FLB_NUM
; snp_get_flb_addr() will return barrier's FLB address with the correct Shire bits set
unum_t  flb_addr = snp_get_flb_addr(barrier)
; snp_get_flb_csr_value_cov() returns the value to write into the ESR address considering that we are using the random barrier chosen above
unum_t  flb_csr_val = snp_get_flb_csr_value_cov(barrier, flb_match_val)

; Get FCC mask of your local shire
unum_t  shire_id = snp_get_shire_id(cpuid)
unum_t  even_mask = snp_get_fcc_mask(shire_id, 0)
unum_t  odd_mask  = snp_get_fcc_mask(shire_id, 1)
; snp_get_fcc_addr_local_shire() returns the FCC ESR addresses
unum_t  fcc_addr_th0 = snp_get_fcc_addr_local_shire(0, fcc_cntr)
unum_t  fcc_addr_th1 = snp_get_fcc_addr_local_shire(1, fcc_cntr)


unum_t  is_reset_cpu = snp_is_reset_cpu(cpuid)
unum_t  rand_init = snp_randnum_glbl()

unum_t num_shires = snp_get_num_active_shires()
uc_memory_t amo_barrier_mem[64]@64

snip_setup
   ; Empty

snip_init

@cpu: *
    ; Reset AMO counter
if (is_reset_cpu == 1):
   if (thread_id == 0):
      amo_barrier_init(addr = amo_barrier_mem, init_value = num_shires, regx1 = rx1, regx2 = rx2)
   endif
endif

snip_run

@cpu: *

    snippet_start(random_num = rand_init, rega = rx1, regb = rx2)

    ; Check the Barrier
    li rx1, flb_csr_val
    csrrw rx1, flb0, rx1

    ; If I am not the last one, wait for a credit
    ; otherwise, give credits
    beq x0, rx1, wait_for_credit

    amo_barrier(num_thr = num_shires, addr = amo_barrier_mem, regx1 = rx1, regx2 = rx2)

load_even_fcc_mask:
    li rx2, even_mask

give_credit_th0:
    li rx1, fcc_addr_th0
    sd rx2, 0(rx1)

load_odd_fcc_mask:
    li rx2, odd_mask

give_credit_th1:
    li rx1, fcc_addr_th1
    sd rx2, 0(rx1)

    if ET_TEST_ENV.SILICON_MODE == 1:
       li rx2, rn1
    endif
    j done_flb_fcc

wait_for_credit:
    do_fcc(regc = rx1, counter_fcc = fcc_cntr)

done_flb_fcc: 
    if ET_TEST_ENV.SILICON_MODE == 1:
       li rx1, rn0
    endif

snip_check
    ; Empty

