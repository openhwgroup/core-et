import "$MINION_DIAGS/sting/snippets/common.pb"

resource

axreg_t rx1

unum_t  barrier = EOT_ENV.FLB_NUM
; snp_get_flb_addr() will return barrier's FLB address with the correct Shire bits set
unum_t  flb_addr = snp_get_flb_addr(barrier)

unum_t  cpuid = snp_cpu_id()
unum_t  is_reset_cpu = snp_is_reset_cpu(cpuid)

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
    ; Empty

snip_check
    ; Empty
