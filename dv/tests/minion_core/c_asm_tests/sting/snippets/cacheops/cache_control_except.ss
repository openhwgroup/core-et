import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource
cpu_t   x

num_t rand_val = snp_randnum()
num_t next_state = snp_randnum()
unum_t use_mcctrl = snp_urandrange(0,2)
unum_t cpuid = snp_cpu_id()
unum_t tid = snp_get_thread_id(cpuid)

axreg_t rx1, rx2, rx3
pxreg_t ecall_opcode

snip_setup
    ; Empty

snip_init
    ; Empty

; Notes:
; - Write to mcache_control from !mmode is an illegal instruction
; - Writing 1 to ucache_control[0] is ignored (but no exception)
; - Writing 1 to ucache_control[1] from !hart0 is ignored (but no exception)
; - Valid transitions for mcache_control (all others are ignored without effect)
;     00 -> 01
;     01 -> 00, 11
;     11 -> 00, 01

snip_run
@cpu: x
    addi ecall_opcode, x0, 0 ; Init opcode to zero
    ; Enter exclusive mode and flush L1
if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
    exclusive_mode(set_it = 1, previous = x0)
    flush_all_l1(value = rand_val, rega = rx1, regb = rx2, regc = rx3)
else: ; Call mhandler
    li ecall_opcode, 0x1
    et_jump_to_m_mode()
endif

    li rx1, next_state

if use_mcctrl > 0: ; Write to mcache_control
    csrrw rx2, mcache_control, rx1 ; Update mcache_control
    if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
        ; Verify next state
        andi rx1, rx1, 0x3 ; Next state
        li rx3, 0x2
        beq rx1, rx3, keep_state ; Invalid X -> 10
        addi rx3, rx1, -3
        beq rx2, rx3, keep_state ; Invalid 00 -> 11
        j mcache_check ; Valid transition
keep_state: ; in case of invalid transition
        andi rx1, rx2, 0x3
mcache_check:
        csrr rx3, mcache_control
        andi rx3, rx3, 0x3
        beq rx1, rx3, test_exit
        test_fail(regz = rx1)
    else: ; Try to access mcache_control from U-/S-mode
        ; Should be skipped by handler
        test_fail(regz = rx1)
    endif

else: ; Write to ucache_control
    csrrw rx2, ucache_control, rx1
    andi rx2, rx2, 0x3
    li rx3, tid
    bne rx3, x0, ucache_check ; only thread0 can modify
    ; Get expected value
    ori rx1, rx1, 0x1 ; D1Split
    andi rx2, rx2, 0x1
    slli rx3, rx2, 1
    or rx2, rx2, rx3
    and rx2, rx2, rx1
ucache_check:
    csrr rx3, ucache_control
    andi rx3, rx3, 0x3
    beq rx2, rx3, test_exit
    test_fail(regz = rx1)
endif

test_exit:

    ; Exit exclusive mode
if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
    exclusive_mode(set_it = 0, previous = x0)
else: ; Call mhandler
    li ecall_opcode, 0x2
    et_jump_to_m_mode()
endif

snip_check
    ; Empty

snip_handler

mhandler@cpu: x
    ; Route ecall based on ecall_opcode
    addi rx1, ecall_opcode, 0 ; Read opcode
    addi ecall_opcode, x0, 0  ; Clear opcode
    beq rx1, x0, illegal_inst
    li rx2, 0x1
    beq rx1, rx2, set_excl
    li rx2, 0x2
    beq rx1, rx2, unset_excl
    j fail_label ; Unknown opcode
illegal_inst:
    ; Skip test_fail
    csrr rx1, mepc
    addi rx1, rx1, 12
    csrw mepc, rx1
    j exit_handler
set_excl:
    ; Enter exclusive mode and flush L1
    exclusive_mode(set_it = 1, previous = x0)
    flush_all_l1(value = rand_val, rega = rx1, regb = rx2, regc = rx3)
    j exit_handler
unset_excl:
    ; Exit exclusive mode
    exclusive_mode(set_it = 0, previous = x0)
    j exit_handler
fail_label:
    test_fail(regz = rx2)
exit_handler:
    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1
