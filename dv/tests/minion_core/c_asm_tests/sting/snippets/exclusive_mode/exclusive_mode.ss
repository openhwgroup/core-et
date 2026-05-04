import "$MINION_DIAGS/sting/snippets/common.pb"

resource
cpu_t   x
unum_t  randinst = snp_urandrange(1,6)
unum_t  nop1 = snp_urandrange(0,100)
unum_t  nop2 = snp_urandrange(0,100)

axreg_t previous_mode
axreg_t reg_1
axreg_t rx1

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x

    ; reg_1 holds a 1
    li reg_1, 1

    ; Do from 1 to 5 random instructions
    RANDINST 0:randinst

    ; Enter exclusive mode
    exclusive_mode(set_it = 1, previous = previous_mode)

    ; Do from 1 to 5 random instructions
    RANDINST 0:randinst

; If we are running in machine mode, this did not cause a trap so
; self check that the value returned ; by the exclusive mode CSR access was 0,
; as we should not have been in exclusive mode before that
if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE:
    bne previous_mode, x0, fail
endif

    ; Do from 1 to 5 random instructions
    RANDINST 0:randinst

; 10% of the time, try to go to exclusive mode again.
; If in machine mode, self check that this returned 1 as we were already in exclusive mode
if nop1 < 10:
    exclusive_mode(set_it = 1, previous = previous_mode)
    if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE:
        bne previous_mode, reg_1, fail
    endif
endif

    ; Get ouf of exclusive mode
    exclusive_mode(set_it = 0, previous = previous_mode)

    ; Do from 1 to 5 random instructions
    RANDINST 0:randinst

; If in machine mode, self check that this returned 1 as we were already in exclusive mode
if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE:
    bne previous_mode, reg_1, fail
endif

; 10% of the time, try to go out of exclusive mode again.
; If in machine mode, self check that this returned 0 as we were already not exclusive
if nop2 < 10:
    exclusive_mode(set_it = 0, previous = previous_mode)
    if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE:
        bne previous_mode, x0, fail
    endif
endif

    j pass
fail:
    test_fail(regz = reg_1)
pass:

snip_check
    ; Empty


snip_handler

mhandler@cpu: x
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1

shandler@cpu: x
    csrr rx1, sepc
    addi rx1, rx1, 4
    csrw sepc, rx1

