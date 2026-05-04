
resource
cpu_t x
axreg_t rx1

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x

    ; cause an illegal instruction
    .space 4


snip_check
   ; Empty

snip_handler

mhandler@cpu: *
    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1

