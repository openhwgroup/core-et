import "$MINION_DIAGS/sting/snippets/common.pb"
resource
cpu_t   x
axreg_t rx1

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x
    csrrs rx1, mhartid, x0

snip_check
    ; Empty

