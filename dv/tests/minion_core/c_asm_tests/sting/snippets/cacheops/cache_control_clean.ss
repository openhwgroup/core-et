import "$MINION_DIAGS/sting/snippets/common.pb"

resource
cpu_t   x

num_t rand_val = snp_randnum()
num_t next_state = snp_randnum()

axreg_t rx1
axreg_t rx2
axreg_t rx3

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x
    exclusive_mode(set_it = 1, previous = x0)
    flush_all_l1(value = rand_val, rega = rx1, regb = rx2, regc = rx3)
    li rx1, next_state
    csrw mcache_control, rx1
    exclusive_mode(set_it = 0, previous = x0)

snip_check
    ; Empty


