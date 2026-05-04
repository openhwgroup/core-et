import "$MINION_DIAGS/sting/snippets/common.pb"

resource
cpu_t   x

unum_t  max_num_rand_instr = snp_urandrange(10, 100)
unum_t  mask_value = snp_get_minst_mask(1, 3)
unum_t  match_value = snp_randnum()

axreg_t rx1
axreg_t rx2

snip_setup
    ; Empty

snip_init
    ; Empty


snip_run
@cpu: x
    ; The reason to set the 1st bit from match to 0 and mask to 1 is to avoid a recursive trap

    ; Set match
    li rx1, match_value
    ; Set the 8th bit to 1
    srli rx1, rx1, 2
    slli rx1, rx1, 2
    csrw minstmatch, rx1

    ; Set mask
    li rx1, mask_value
    ; The 32nd bit must be set to 1 to activate the mask ; Set the 1st bit to 1
    li rx2, 0x80000002
    or rx1, rx2, rx1    
    csrw minstmask, rx1

    ; Generate between 10 and 100 random instructions
    RANDINST 10:max_num_rand_instr

    ; Clear mask
    csrw minstmask, x0


snip_check
    ; Empty


snip_handler

mhandler@cpu: x
    ; Clear mask
    csrw minstmask, x0

    csrrs rx1, mcause, x0
    li rx2, 0x1e
    beq rx1, rx2, pass_check_trap

    ; We received an unexpected trap, we go to fail
    test_fail(regz = rx1)

pass_check_trap:
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1