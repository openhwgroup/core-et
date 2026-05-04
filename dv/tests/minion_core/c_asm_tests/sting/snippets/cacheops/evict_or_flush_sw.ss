import "$MINION_DIAGS/sting/snippets/common.pb"

resource
cpu_t   x

unum_t   csr_val   = snp_randnum()
unum_t   tmask     = snp_randnum()
unum_t   repeat    = snp_urandrange(0, 2)
unum_t   tensor_id = snp_urandrange(0, 2)
unum_t   do_evict  = snp_urandrange(0, 2)
unum_t   csr_enc   = 0
unum_t   x31_enc   = snp_randnum()

axreg_t rx1
axreg_t rx2
axreg_t saved_x31

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x

    ; Save x31
    addi saved_x31, x31, 0

    ;
    ; rx1 is the register used to write to CSR
    ; Everything can be random because all fields are either WARL or legal
    ; Clear bottom 4 bits (repeat count) to have a biased random
    ;
    csr_enc = snp_and(csr_val, 0xfffffffffffffff0)

    if (ET_L2SCP.L2SCP_IN_MEM_SET == 1):
       csr_enc = snp_and(csr_enc, 0xf7ffffffffffffff)
    endif

if repeat > 0:
    ; Select the repeat count, with a bias on smaller values
    SELECT 1 {
     16 : csr_enc = snp_or(csr_enc, 1)
     12 : csr_enc = snp_or(csr_enc, 2)
     12 : csr_enc = snp_or(csr_enc, 3)
     10 : csr_enc = snp_or(csr_enc, 4)
     10 : csr_enc = snp_or(csr_enc, 5)
     10 : csr_enc = snp_or(csr_enc, 6)
     10 : csr_enc = snp_or(csr_enc, 7)
     8 :  csr_enc = snp_or(csr_enc, 8)
     8 :  csr_enc = snp_or(csr_enc, 9)
     4 :  csr_enc = snp_or(csr_enc, 10)
     4 :  csr_enc = snp_or(csr_enc, 11)
     2 :  csr_enc = snp_or(csr_enc, 12)
     2 :  csr_enc = snp_or(csr_enc, 13)
     1 :  csr_enc = snp_or(csr_enc, 14)
     1 :  csr_enc = snp_or(csr_enc, 15)
    }

    ; Randomize stride to some cache line multiples
    SELECT 1 {
     1 : x31_enc = snp_or(64,  0)
     1 : x31_enc = snp_or(128, 0)
     1 : x31_enc = snp_or(256, 0)
     1 : x31_enc = snp_or(512, 0)
    }
endif
    x31_enc = snp_or(x31_enc, tensor_id)
    li x31, x31_enc
    li rx1, csr_enc

; Setup mask if used
if csr_val >= 0x8000000000000000:
    li rx2, tmask
    csrw tensor_mask, rx2
endif

if do_evict > 0:
    csrw evict_sw, rx1
else:
    csrw flush_sw, rx1
endif

; If executing in user mode, should cause an exception
; Following block makes the test fail, but should be skipped by the exception handler
if TEST_PROPS.TEST_MODE != RV_PARAMS.RV_MODE.MACHINE:
    test_fail(regz = rx2)
endif

    ; Restore x31
    addi x31, saved_x31, 0

snip_check
    ; Empty


snip_handler

mhandler@cpu: x
    csrr rx1, mepc
    addi rx1, rx1, 16
    csrw mepc, rx1

shandler@cpu: x
    csrr rx1, sepc
    addi rx1, rx1, 16
    csrw sepc, rx1
