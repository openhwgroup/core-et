resource
cpu_t   x

num_t next_state = snp_randnum()

axreg_t rx1
axreg_t rx2

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x

    li rx1, next_state
if TEST_PROPS.TEST_MODE != RV_PARAMS.RV_MODE.MACHINE:
    ; zero out lowest bit to avoid exception
    li rx2, -1
    xori rx2, rx2, 1
    and rx1, rx1, rx2
    csrw ucache_control, rx1
else:
    csrw mcache_control, rx1
endif

snip_check
    ; Empty


