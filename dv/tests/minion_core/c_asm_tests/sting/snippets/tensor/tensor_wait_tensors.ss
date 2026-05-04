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
   10: wait_event = 0
   10: wait_event = 1
   10: wait_event = 2
   10: wait_event = 3
   1: wait_event = 4
   1: wait_event = 5
   1: wait_event = 6
   10: wait_event = 7
   10: wait_event = 8
   10: wait_event = 9
   10: wait_event = 10
   10: wait_event = 11
   10: wait_event = 12
   10: wait_event = 13
   10: wait_event = 14
   10: wait_event = 15
}
    csr_enc = snp_or(warl, wait_event)
    csr_imm = snp_and(csr_enc, 0xf)

    li csr_reg, csr_enc

SELECT 1 {
    1: csrrw  return0, tensor_wait, csr_reg
    1: csrrs  return0, tensor_wait, csr_reg
    1: csrrwi return0, tensor_wait, csr_imm
    1: csrrsi return0, tensor_wait, csr_imm
}

    beq return0, x0, pass
    test_fail (regz = csr_reg)
pass:

snip_check
    ; Empty

