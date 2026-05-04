; This snippet runs on two random HARTs.
; The first hart peforms wfi or CSR stall for U/S.
; The second hart sends credits to the first one until it overflows, and then wake it with IPI.
; The first IPI self check TensorError[3].
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/flb_fcc/flb_fcc.pb"
resource

unum_t x = snp_urandrange_glbl(0, BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP)
unum_t counter_select = snp_urandrange_glbl(0,2)

unum_t fcc_addr_to_x = snp_get_fcc_addr_local_shire_same_tid(counter_select)
unum_t fcc_mask = snp_get_fcc_overflow_mask(x)

unum_t rand_init = snp_randnum_glbl()

axreg_t rx1, rx2, rx3, rx4

snip_setup
    ; Empty
    ;
snip_init
    ; Empty

snip_run
@cpu: x
    snippet_start(random_num = rand_init, rega = rx1, regb = rx2)

    li rx3, 0x10000 ;65535 + 1 credits
    li rx4, 1

give_credit_to_x:
    ; Write into the ESR to give credits
    li rx1, fcc_addr_to_x
    li rx2, fcc_mask
    sd rx2, 0(rx1)

    sub rx3, rx3, rx4
    bne x0, rx3, give_credit_to_x

    fence iorw, iorw

    check_fcc_overflow(rega = rx1, regb = rx2, regc = rx3, fail_label = snip_fail)
    j snip_pass
snip_fail:
    test_fail (regz = rx3)
snip_pass:
    reset_tensor_error()

snip_check
    ; Empty

snip_handler

mhandler@cpu: *
    addi rx4,  rx4, 4
    csrw mepc, rx4

