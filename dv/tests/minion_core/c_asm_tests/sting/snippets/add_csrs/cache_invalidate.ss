import "$MINION_DIAGS/sting/snippets/common.pb"
resource
cpu_t   x
unum_t  warl = snp_randnum()
unum_t  cinvalidate_event = 0
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

    warl = snp_lshift(warl, 2)

SELECT 1 {
    1:  cinvalidate_event = 0
    1:  cinvalidate_event = 1
    1:  cinvalidate_event = 2
    1:  cinvalidate_event = 3
}
    csr_enc = snp_or(warl, cinvalidate_event)
    csr_imm = snp_and(csr_enc, 0x3)

    li csr_reg, csr_enc

SELECT 1 {
    1: csrrw  return0, flush_icache, csr_reg
    1: csrrs  return0, flush_icache, csr_reg
    1: csrrwi return0, flush_icache, csr_imm
    1: csrrsi return0, flush_icache, csr_imm
}

    beq return0, x0, pass
    test_fail (regz = csr_reg)
pass:

snip_check
    ; Empty

