import "$MINION_DIAGS/sting/snippets/common.pb"
resource
cpu_t   x
unum_t  rand64 = snp_randnum()
unum_t  rand_enc = snp_randnum()
unum_t  offset = snp_and(rand64, 0xfffff)

axreg_t prefetch_addr
axreg_t esr_addr
axreg_t esr_enc

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x

    rand_enc = snp_and(rand_enc, 0xf00000000003f)
    li esr_enc, rand_enc
    auipc prefetch_addr, offset
    srli prefetch_addr, prefetch_addr, 6
    slli prefetch_addr, prefetch_addr, 6

if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.USER:
    li esr_addr, 0x803402F8
endif
if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.SUPERVISOR:
    li esr_addr, 0x80740300
endif
if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE:
    li esr_addr, 0x80F40308
endif

    or esr_enc, esr_enc, prefetch_addr
    sd esr_enc, 0(esr_addr)

snip_check
    ; Empty


