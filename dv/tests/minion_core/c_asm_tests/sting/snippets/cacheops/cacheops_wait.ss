import "$MINION_DIAGS/sting/snippets/common.pb"
resource
cpu_t   x
unum_t  warl = snp_randnum()
unum_t  wait_event = 0
unum_t  csr_enc = 0
unum_t  csr_imm = 0

axreg_t csr_reg
axreg_t return0

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x

    warl = snp_lshift(warl, 4)

SELECT 1 {
    1:  wait_event = 0
    1:  wait_event = 1
    1:  wait_event = 2
    1:  wait_event = 3
    5:  wait_event = 4
    5:  wait_event = 5
    50: wait_event = 6
    1:  wait_event = 7
    1:  wait_event = 8
    1:  wait_event = 9
    1:  wait_event = 10
    1:  wait_event = 11
    1:  wait_event = 12
    1:  wait_event = 13
    1:  wait_event = 14
    1:  wait_event = 15
}
    csr_enc = snp_or(warl, wait_event)
    csr_imm = snp_and(csr_enc, 0x1f)

    li csr_reg, csr_enc

SELECT 1 {
    1: csrrw  return0, tensor_wait, csr_reg
    1: csrrs  return0, tensor_wait, csr_reg
    1: csrrwi return0, tensor_wait, csr_imm
    1: csrrsi return0, tensor_wait, csr_imm
}

if (ET_TEST_ENV.SELF_CHECK_ENABLE == 1):
    beq return0, x0, pass
    test_fail (regz = csr_reg)
pass:
endif

snip_check
    ; Empty

